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
my ($r_ptr,$a_ptr,$al,$shift,$l)=("%rdi","%rsi","%rdx","%rcx","%r8");
my ($t0,$t1,$t2)=("%r9","%r10","%r11");
$code.=<<___;
# u64 ll_lshift(u64 *r, u64 *a, size_t al, size_t n);
.globl	ll_lshift
.export	ll_lshift
.type	ll_lshift,\@function,4
.align	32
ll_lshift:
    xor %rax, %rax
    test $al, $al
    jz .ll_lshift_done_ret
    mov $shift, $l
    xor $t1, $t1
    shrq \$6, $l                   # l = shift / 64
    and \$63, $shift
    mov $al, %rax
    add $l, %rax                   # al + l
    lea -8($a_ptr,$al,8), $a_ptr   # start shifting from the most significant limb
    lea 0($r_ptr,%rax,8), $r_ptr
    mov $r_ptr, $t2
    test $shift, $shift
    jnz .ll_lshift_loop
    mov $t1, 0($r_ptr)
    lea -8($r_ptr), $r_ptr
    jmp .ll_lshift_copy            # if shift % 64 = 0, then copy limbs from a to r

.ll_lshift_loop:
    test $al, $al
    jz .ll_lshift_last_limb
    mov 0($a_ptr), $t0
    lea -8($a_ptr), $a_ptr
    shldq %cl, $t0, $t1
    dec $al
    mov $t1, 0($r_ptr)
    mov $t0, $t1
    lea -8($r_ptr), $r_ptr
    jmp .ll_lshift_loop

.ll_lshift_last_limb:
    shlq %cl, $t0
    mov $t0, 0($r_ptr)
    lea -8($r_ptr), $r_ptr
    jmp .ll_lshift_clear_lower_limbs

.ll_lshift_copy:
    test $al, $al
    jz .ll_lshift_clear_lower_limbs
    mov 0($a_ptr), $t1
    lea -8($a_ptr), $a_ptr
    mov $t1, 0($r_ptr)
    dec $al
    lea -8($r_ptr), $r_ptr
    jmp .ll_lshift_copy

.ll_lshift_clear_lower_limbs:      # set (shift / 64) lower limbs to 0
    xor $t0, $t0
.ll_lshift_clear_loop:
    test $l, $l
    jz .ll_lshift_done
    mov $t0, 0($r_ptr)
    dec $l
    lea -8($r_ptr), $r_ptr
    jmp .ll_lshift_clear_loop

.ll_lshift_done:
    mov 0($t2), %rax
    ret
.ll_lshift_done_ret:
    ret
.size	ll_lshift,.-ll_lshift


# u64 ll_rshift(u64 *r, u64 *a, size_t al, size_t n);
.globl	ll_rshift
.export	ll_rshift
.type	ll_rshift,\@function,4
.align	32
ll_rshift:
    xor %rax, %rax
    test $al, $al
    jz .ll_rshift_done
    mov $shift, $l
    shrq \$6, $l                # l = shift / 64
    and \$63, $shift
    cmp $al, $l
    jae .ll_rshift_r_is_zero    # if l >= al, then r = 0
    sub $l, $al
    dec $al
    lea 0($a_ptr,$l,8), $a_ptr  # discard lower l limbs of a
    mov 0($a_ptr), %rax
    test $shift, $shift         # start shifting from the (l+1)'s limb
    jnz .ll_rshift_loop
    inc $al
    jmp .ll_rshift_copy         # if shift % 64 = 0, then copy limbs from a to r

.ll_rshift_r_is_zero:           # clear r
    test $al, $al
    jz .ll_rshift_done
    mov %rax, 0($r_ptr)
    dec $al
    lea 8($r_ptr), $r_ptr
    jmp .ll_rshift_r_is_zero

.ll_rshift_loop:
    lea 8($a_ptr), $a_ptr
    test $al, $al
    jz .ll_rshift_last_limb
    mov 0($a_ptr), $t1
    shrdq %cl, $t1, %rax
    dec $al
    mov %rax, 0($r_ptr)
    mov $t1, %rax
    lea 8($r_ptr), $r_ptr
    jmp .ll_rshift_loop

.ll_rshift_last_limb:
    shrq %cl, %rax
    mov %rax, 0($r_ptr)
    lea 8($r_ptr), $r_ptr

.ll_rshift_clear_upper_limbs:   # set (shift / 64) higer limbs to 0
    xor $t0, $t0
.ll_rshift_clear_loop:
    test $l, $l
    jz .ll_rshift_done
    mov $t0, 0($r_ptr)
    dec $l
    lea 8($r_ptr), $r_ptr
    jmp .ll_rshift_clear_loop

.ll_rshift_copy:
    test $al, $al
    jz .ll_rshift_done
    mov 0($a_ptr), %rax
    lea 8($a_ptr), $a_ptr
    mov %rax, 0($r_ptr)
    dec $al
    lea 8($r_ptr), $r_ptr
    jmp .ll_rshift_copy

.ll_rshift_done:
    ret
.size	ll_rshift,.-ll_rshift
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
