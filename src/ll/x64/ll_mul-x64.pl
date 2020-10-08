##############################################################################
#                                                                            #
# Copyright 2020 Meng-Shan Jiang                                             #
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
# size_t ll_mul_limb(u64 *rd, u64 *ad, u64 b, size_t al);
.globl	ll_mul_limb
.type	ll_mul_limb,\@function,4
.align	32
ll_mul_limb:
    mov $b_org, $b
    xor $t1, $t1
    mov $al, $t0

.ll_mul_limb_loop:
    test $al, $al
    jz .ll_mul_limb_loop_end
    mov 0($a_ptr), %rax
    xor $t2, $t2
    mulq $b
    lea 8($a_ptr), $a_ptr
    add %rax, $t1
    adc %rdx, $t2
    mov $t1, 0($r_ptr)
    mov $t2, $t1
    lea 8($r_ptr), $r_ptr
    dec $al
    jmp .ll_mul_limb_loop

.ll_mul_limb_loop_end:
    mov $t1, 0($r_ptr)
    mov $t0, %rax
    test $t1, $t1
    jz .ll_mul_limb_done
    inc %rax

.ll_mul_limb_done:
    ret
.size	ll_mul_limb,.-ll_mul_limb
___
}

{
my ($r_ptr,$a_ptr,$b_org,$rl,$al,$b)=("%rdi","%rsi","%rdx","%rcx","%r8","%r9");
my ($t0,$t1,$t2,$t3)=("%rbx","%r10","%r11","%rbp",);
$code.=<<___;
# size_t ll_muladd_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al);
.globl	ll_muladd_limb
.type	ll_muladd_limb,\@function,5
.align	32
ll_muladd_limb:
    push %rbp
    push %rbx

    mov $rl, $t3
    mov $b_org, $b
    xor $t1, $t1
    xor %rax, %rax
    sub $al, $t3                # rl - al
    jnc .ll_muladd_limb_loop    # if rl >= al
    mov $al, $rl                # rl = max{rl, al}
    test $rl, $rl
    jz .ll_muladd_limb_done     # if max{rl, al} = 0, do nothing.

.ll_muladd_limb_loop:           # rd[0:al-1] += ad * b
    test $al, $al
    jz .ll_muladd_limb_loop_end
    mov 0($a_ptr), %rax
    xor $t2, $t2
    add 0($r_ptr), $t1
    adc \$0, $t2
    mulq $b
    lea 8($a_ptr), $a_ptr
    add %rax, $t1
    adc %rdx, $t2
    mov $t1, 0($r_ptr)
    mov $t2, $t1
    lea 8($r_ptr), $r_ptr
    dec $al
    jmp .ll_muladd_limb_loop

.ll_muladd_limb_loop_end:
    cmp \$0, $t3
    jge .ll_muladd_limb_loop_r
    jmp .ll_muladd_limb_loop_r_done

.ll_muladd_limb_loop_r:         # rl >= al, rd[al:rl-1] += t1
    test $t3, $t3
    jz .ll_muladd_limb_loop_r_done
    mov 0($r_ptr), $t2
    xor $t0, $t0
    add $t1, $t2
    adc \$0, $t0
    mov $t2, 0($r_ptr)
    mov $t0, $t1
    lea 8($r_ptr), $r_ptr
    dec $t3
    jmp .ll_muladd_limb_loop_r

.ll_muladd_limb_loop_r_done:
    mov $t1, 0($r_ptr)
    mov $rl, %rax
    test $t1, $t1
    jz .ll_muladd_limb_done
    inc %rax

.ll_muladd_limb_done:
    pop %rbx
    pop %rbp
    ret
.size	ll_muladd_limb,.-ll_muladd_limb


# size_t ll_mulsub_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al);
.globl	ll_mulsub_limb
.type	ll_mulsub_limb,\@function,5
.align	32
ll_mulsub_limb:
    push %rbp
    push %rbx

    mov $rl, $t3
    mov $b_org, $b
    xor $t1, $t1
    xor %rax, %rax
    sub $al, $t3                 # rl - al
    test $rl, $rl
    jz .ll_mulsub_limb_done      # if rl = 0, do nothing.

.ll_mulsub_limb_loop:            # rd[0:al-1] -= ad * b
    test $al, $al
    jz .ll_mulsub_limb_loop_end
    mov 0($r_ptr), $t0
    mov 0($a_ptr), %rax
    xor $t2, $t2
    sub $t1, $t0
    adc \$0, $t2
    mulq $b
    lea 8($a_ptr), $a_ptr
    sub %rax, $t0
    adc %rdx, $t2
    mov $t0, 0($r_ptr)
    mov $t2, $t1
    lea 8($r_ptr), $r_ptr
    dec $al
    jmp .ll_mulsub_limb_loop

.ll_mulsub_limb_loop_end:
    test $t3, $t3
    jnz .ll_mulsub_limb_loop_r
    jmp .ll_mulsub_limb_loop_r_done

.ll_mulsub_limb_loop_r:          # rl >= al, rd[al:rl-1] -= t1
    test $t3, $t3
    jz .ll_mulsub_limb_loop_r_done
    mov 0($r_ptr), $t2
    xor $t0, $t0
    sub $t1, $t2
    adc \$0, $t0
    mov $t2, 0($r_ptr)
    mov $t0, $t1
    lea 8($r_ptr), $r_ptr
    dec $t3
    jmp .ll_mulsub_limb_loop_r

.ll_mulsub_limb_loop_r_done:
    lea -8($r_ptr), $r_ptr
    mov $rl, %rax
    jz .ll_mulsub_limb_count_nlimbs

.ll_mulsub_limb_count_nlimbs:
    mov 0($r_ptr), $t0
    test $t0, $t0
    jnz .ll_mulsub_limb_done
    lea -8($r_ptr), $r_ptr
    dec %rax
    test %rax, %rax
    jnz .ll_mulsub_limb_count_nlimbs

.ll_mulsub_limb_done:
    pop %rbx
    pop %rbp
    ret
.size	ll_mulsub_limb,.-ll_mulsub_limb
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";