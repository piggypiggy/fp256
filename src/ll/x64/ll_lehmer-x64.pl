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

.align	8
.Linverse_table:
.byte	255,251,247,243,239,236,233,229,226,223,220,217,214,211,209,206
.byte	203,201,198,196,194,191,189,187,185,183,181,179,177,175,173,171
.byte	169,167,166,164,162,161,159,158,156,155,153,152,150,149,147,146
.byte	145,143,142,141,140,139,137,136,135,134,133,132,131,130,129,128
___

{
my ($a1,$a0,$b1,$b0)=("%r12","%r13","%r14","%r15");
my ($t0,$t1,$t2,$t3)=("%rdx","%rbx","%rcx","%rbp");
my ($mat_ptr,$m00,$m01,$m10,$m11)=("%rdi","%rdi","%rsi","%r8","%r11");
$code.=<<___;
# u64 ll_div_11(u64 *rd, u64 a0, u64 b0);
.type ll_div_11,\@abi-omnipotent
.align	32
ll_div_11:
    xor %r10, %r10
    xor %r12, %r12
    mov \$63, %rax
    bsrq $b0, %r10
    mov %rax, %rcx
    bsrq $a0, %r12
    sub %r10, %rcx               # number of leading zeros of b0
    sub %r12, %rax               # number of leading zeros of a0
    inc %rcx                     # lz(b0) + 1
    mov $b0, $t1
    mov $a0, %r9
    shlq %cl, $t1
    lea .Linverse_table(%rip), %r12
    mov %rax, %rcx
    shrq \$58, $t1               # (b0 << (lz(b0) + 1)) >> 58
    shlq %cl, %r9
    movzb (%r12,$t1,1), %eax   # inverse_table[$t1]
    shrq \$8, %r9
    add %r10, %rcx
    mulq %r9
    mov $a0, %r12
    shrq %cl, %rax               # approximate quotient
    mov $b0, %r9
    mov %rax, %r10
    mulq $b0
    xor %rbx, %rbx
    sub %rax, %r12               # approximate remainder
    cmp %r12, $b0
    setbe %bl
    neg %rbx
    sub %rbx, %r10               # adjust quotient
    and %rbx, %r9
    mov %r10, %rax
    sub %r9, %r12                # adjust remainder

    cmp %r12, $b0
    ja .ll_div_11_done
    xor %rdx, %rdx               # slow division
    mov $a0, %rax
    divq $b0
    mov %rdx, %r12

.ll_div_11_done:
    mov $b0, $a0
    mov %r12, $b0
    ret
.size	ll_div_11,.-ll_div_11


# u64 ll_div_22(u64 rd[2], u64 a1, u64 a0, u64 b1, u64 b0);
.type ll_div_22,\@abi-omnipotent
.align	32
ll_div_22:
    push $b0
    xor %r10, %r10
    xor %rcx, %rcx
    movq \$63, %r9
    bsrq $a1, %rcx
    bsrq $b1, %r10
    xor %rax, %rax
    sub %r10, %rcx
     mov $b1, %r10

    # left shift b1:b0
    mov $b0, $t0
    mov %rcx, $t1
    sub %rcx, %r9
    shlq %cl, $b1
    shlq %cl, $b0
    mov %r9, %rcx
    shrq \$1, $t0
    shrq %cl, $t0
    mov $t1, %r9
    or $t0, $b1
    inc %r9

.ll_div_22_loop:
    mov $a0, $t0
    mov $a1, $t1
    shlq \$1, %rax
    sub $b0, $t0
    sbb $b1, $t1
    cmovnc $t0, $a0
    cmovnc $t1, $a1
    sbb \$0, %rax
    shrdq \$1, $b1, $b0
    inc %rax
    shrq \$1, $b1
    dec %r9
    jnz .ll_div_22_loop

    mov $a0, $b0
    mov $a1, $b1
    pop $a0
    mov %r10, $a1
    ret
.size	ll_div_22,.-ll_div_22


# u64 ll_lehmer_exgcd11(u64 mat[4], u64 a0, u64 b0);
.globl	ll_lehmer_exgcd11
.type	ll_lehmer_exgcd11,\@function,3
.align	32
ll_lehmer_exgcd11:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15

    push $mat_ptr
    mov %rsi, $a0
    mov %rdx, $b0
    xor %r14, %r14    # %r14 tracks number of matrix multiplications

    # init mat = [1, 0;0, 1]
    mov \$1, $m00
    xor $m01, $m01
    xor $m10, $m10
    mov \$1, $m11

    cmp $a0, $b0
    jae .ll_lehmer_exgcd11_swap
    jmp .ll_lehmer_exgcd11_loop

.ll_lehmer_exgcd11_swap:
    # init mat = [0, 1;1, 0]
    mov \$1, $m01
    xor $m00, $m00
    xor $m11, $m11
    mov \$1, $m10
    # swap a0 and b0
    mov $a0, $t0
    mov $b0, $a0
     inc %r14
    mov $t0, $b0

.ll_lehmer_exgcd11_loop:
    test $b0, $b0
    jz .ll_lehmer_exgcd11_done
    call ll_div_11
    mov %rax, $t1
    mulq $m10
    inc %r14
    add $m00, %rax
    mov $m10, $m00
    mov %rax, $m10
    mov $t1, %rax
    mulq $m11
    add $m01, %rax
    mov $m11, $m01
    mov %rax, $m11
    jmp .ll_lehmer_exgcd11_loop

.ll_lehmer_exgcd11_done:
    mov $m00, $t1
    mov %r14, %rax
    pop $mat_ptr
    mov $t1,  0($mat_ptr)
    mov $m01, 8($mat_ptr)
    mov $m10, 16($mat_ptr)
    mov $m11, 24($mat_ptr)
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_lehmer_exgcd11,.-ll_lehmer_exgcd11


# u64 ll_lehmer_simulate(u64 mat[4], u64 a1, u64 a0, u64 b1, u64 b0);
.globl	ll_lehmer_simulate
.type	ll_lehmer_simulate,\@function,5
.align	32
ll_lehmer_simulate:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15

    # save mat_ptr
    push $mat_ptr

    sub \$8, %rsp
    movq \$0x100000000, %rax
    mov %rsi, $a1
    mov %rdx, $a0
    mov %rcx, $b1
    mov %r8, $b0
    xor $t3, $t3        # $t3 tracks number of matrix multiplications
    movq %rax, 0(%rsp)

    # init mat = [1, 0;0, 1]
    mov \$1, $m00
    xor $m01, $m01
    xor $m10, $m10
    mov \$1, $m11

    test $b1, $b1
    jz .ll_lehmer_simulate_done

    mov $a0, $t0
    mov $a1, $t1
    sub $b0, $t0
    sbb $b1, $t1
    jc .ll_lehmer_simulate_swap
    jmp .ll_lehmer_simulate_div2_loop

.ll_lehmer_simulate_swap:
    # init mat = [0, 1;1, 0]
    mov \$1, $m01
    xor $m00, $m00
    xor $m11, $m11
    mov \$1, $m10
    # swap a and b
    mov $a0, $t0
    mov $a1, $t1
    mov $b0, $a0
    mov $b1, $a1
     inc $t3
    mov $t0, $b0
    mov $t1, $b1

.ll_lehmer_simulate_div2_loop:
    cmp $a1, 0(%rsp)
    ja .ll_lehmer_simulate_div1_loop_pre
    # usually(~50%), quotient is 1
    mov $a0, $t0
    mov $a1, $t1
    sub $b0, $t0
    sbb $b1, $t1
    cmp $b1, $t1
    ja .do_div2         # a1:a0 > 2 * b1:b0
    test $t1, $t1
    jz .ll_lehmer_simulate_done
    mov $m10, %rax      # [0, 1;1, 1] * mat
    mov $b0, $a0
    mov $b1, $a1
    add $m00, $m10
    mov %rax, $m00
     inc $t3
    mov $m11, %rax
    mov $t0, $b0
    mov $t1, $b1
    add $m01, $m11
    mov %rax, $m01
    jmp .ll_lehmer_simulate_div2_loop

.do_div2:
    call ll_div_22
    test $b1, $b1
    jz .ll_lehmer_simulate_done
    mov %rax, $t1       # [0, 1;1, q] * mat
    mulq $m10
    add $m00, %rax
    mov $m10, $m00
    mov %rax, $m10
    mov $t1, %rax
    mulq $m11
     inc $t3
    add $m01, %rax
    mov $m11, $m01
    mov %rax, $m11
    jmp .ll_lehmer_simulate_div2_loop

.ll_lehmer_simulate_div1_loop_pre:
    movq \$0x200000000, %rax
    movq \$32, %rcx
    mov %rax, 0(%rsp)
    shrdq %cl, $a1, $a0
    shrdq %cl, $b1, $b0

.ll_lehmer_simulate_div1_loop:
    # usually(~50%), quotient is 1
    mov $a0, $t0
    sub $b0, $t0
    cmp $b0, $t0
    ja .do_div1         # a0 > 2 * b0
    cmp $t0, 0(%rsp)
    ja .ll_lehmer_simulate_done
    mov $m10, %rax      # [0, 1;1, 1] * mat
    mov $b0, $a0
    add $m00, $m10
    mov %rax, $m00
    mov $m11, %rax
    mov $t0, $b0
     inc $t3
    add $m01, $m11
    mov %rax, $m01
    jmp .ll_lehmer_simulate_div1_loop

.do_div1:
    call ll_div_11
    cmp $b0, 0(%rsp)
    ja .ll_lehmer_simulate_done
    mov %rax, $t1       # [0, 1;1, q] * mat
    mulq $m10
    add $m00, %rax
    mov $m10, $m00
    mov %rax, $m10
    mov $t1, %rax
    mulq $m11
     inc $t3
    add $m01, %rax
    mov $m11, $m01
    mov %rax, $m11
    jmp .ll_lehmer_simulate_div1_loop

.ll_lehmer_simulate_done:
    mov $t3, %rax
    mov $m00, $t0
    add \$8, %rsp
    pop $mat_ptr

    mov $t0, 0($mat_ptr)
    mov $m01, 8($mat_ptr)
    mov $m10, 16($mat_ptr)
    mov $m11, 24($mat_ptr)

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_lehmer_simulate,.-ll_lehmer_simulate
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
