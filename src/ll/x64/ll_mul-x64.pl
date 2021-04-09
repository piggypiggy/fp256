#!/usr/bin/env perl

##############################################################################
#                                                                            #
# Copyright 2020-2021 Jiang Mengshan                                         #
#                                                                            #
# Licensed under the Apache License, Version 2.0 (the "License");            #
# you may not use this file except in compliance with the License.           #
# You may obtain a copy of the License at                                    #
#                                                                            #
#    http://www.apache.org/licenses/LICENSE-2.0                              #
#                                                                            #
# Unless required by applicable law or agreed to in writing, software        #
# distributed under the License is distributed on an "AS IS" BASIS,          #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
# See the License for the specific language governing permissions and        #
# limitations under the License.                                             #
#                                                                            #
##############################################################################

$flavour = shift;
$output  = shift;

# if ($flavour =~ /\./) { $output = $flavour; undef $flavour; }

$win64=0; $win64=1 if ($flavour =~ /[nm]asm|mingw64/ || $output =~ /\.asm$/);

$0 =~ m/(.*[\/\\])[^\/\\]+$/; $dir=$1;
( $xlate="${dir}x86_64-xlate.pl" and -f $xlate ) or
( $xlate="${dir}../perlasm/x86_64-xlate.pl" and -f $xlate) or
die "can't locate x86_64-xlate.pl";

open OUT,"| \"$^X\" \"$xlate\" $flavour \"$output\"";
*STDOUT=*OUT;

$code.=<<___;
.text
___

{
my ($r_ptr,$a_ptr,$b_org,$al,$b)=("%rdi","%rsi","%rdx","%rcx","%r8");
my ($t0,$t1,$t2)=("%r9","%r10","%r11");
$code.=<<___;
# u64 ll_mul_limb(u64 *rd, u64 *ad, u64 b, size_t al);
.globl	ll_mul_limb
.export ll_mul_limb
.type	ll_mul_limb,\@function,4
.align	32
ll_mul_limb:
    mov $b_org, $b
    xor $t1, $t1

.ll_mul_limb_loop:
    test $al, $al
    jz .ll_mul_limb_done
    mov 0($a_ptr), %rax
    mulq $b                   # ad[i] * b
    lea 8($a_ptr), $a_ptr
    add %rax, $t1
    adc \$0, %rdx
    mov $t1, 0($r_ptr)
    mov %rdx, $t1
    lea 8($r_ptr), $r_ptr
    dec $al
    jmp .ll_mul_limb_loop

.ll_mul_limb_done:
    mov $t1, 0($r_ptr)
    mov $t1, %rax
    ret
.size	ll_mul_limb,.-ll_mul_limb
___
}

{
my ($r_ptr,$a_ptr,$b_org,$rl,$al,$b)=("%rdi","%rsi","%rdx","%rcx","%r8","%r9");
my ($t1,$t2,$i)=("%r10","%r11","%rbp");
$code.=<<___;
# u64 ll_muladd_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al);
.globl	ll_muladd_limb
.export	ll_muladd_limb
.type	ll_muladd_limb,\@function,5
.align	32
ll_muladd_limb:
    push %rbp
    xor $i, $i
    mov $b_org, $b
    xor $t1, $t1

.ll_muladd_limb_mul_loop:           # rd[0, al-1] += ad * b
    cmp $al, $i
    je .ll_muladd_limb_add_loop
    mov 0($a_ptr), %rax
    lea 8($a_ptr), $a_ptr
    mulq $b
    add 0($r_ptr), $t1
    adc \$0, %rdx
    add %rax, $t1
    adc \$0, %rdx
    mov $t1, 0($r_ptr)
    lea 8($r_ptr), $r_ptr
    mov %rdx, $t1
    inc $i
    jmp .ll_muladd_limb_mul_loop

.ll_muladd_limb_add_loop:           # rl > al, rd[al, rl-1] += t1
    cmp $rl, $i
    jge .ll_muladd_limb_done
    mov 0($r_ptr), $t2
    inc $i
    add $t1, $t2
    mov \$0, $t1
    mov $t2, 0($r_ptr)
    setc %r10b
    lea 8($r_ptr), $r_ptr
    jmp .ll_muladd_limb_add_loop

.ll_muladd_limb_done:
    mov $t1, 0($r_ptr)
    mov $t1, %rax
    pop %rbp
    ret
.size	ll_muladd_limb,.-ll_muladd_limb


# u64 ll_mulsub_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al);
.globl	ll_mulsub_limb
.export	ll_mulsub_limb
.type	ll_mulsub_limb,\@function,5
.align	32
ll_mulsub_limb:
    push %rbp
    xor $i, $i
    mov $b_org, $b
    xor $t1, $t1

.ll_mulsub_limb_mul_loop:          # rd[0, al-1] -= ad * b
    cmp $al, $i
    je .ll_mulsub_limb_sub_loop
    mov 0($a_ptr), %rax
    lea 8($a_ptr), $a_ptr
    mulq $b
    mov 0($r_ptr), $t2
    add %rax, $t1
    adc \$0, %rdx
    sub $t1, $t2
    adc \$0, %rdx
    mov $t2, 0($r_ptr)
    mov %rdx, $t1
    lea 8($r_ptr), $r_ptr
    inc $i
    jmp .ll_mulsub_limb_mul_loop

.ll_mulsub_limb_sub_loop:          # rl > al, rd[al, rl-1] -= t1
    cmp $rl, $i
    jge .ll_mulsub_limb_done
    mov 0($r_ptr), $t2
    inc $i
    sub $t1, $t2
    mov \$0, $t1
    mov $t2, 0($r_ptr)
    setc %r10b
    lea 8($r_ptr), $r_ptr
    jmp .ll_mulsub_limb_sub_loop

.ll_mulsub_limb_done:
    mov $t1, %rax
    pop %rbp
    ret
.size	ll_mulsub_limb,.-ll_mulsub_limb
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
