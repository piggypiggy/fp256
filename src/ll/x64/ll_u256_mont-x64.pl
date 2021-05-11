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
my ($r_ptr,$a_ptr,$b_org,$N_ptr,$k0,$b_ptr)=("%rdi","%rsi","%rdx","%rcx","%r8","%r9");
my ($a0,$a1,$a2,$a3,$b)=("%rbp","%rbx","%r8","%rsi","%rdi");
my ($acc0,$acc1,$acc2,$acc3,$acc4,$acc5)=("%r10","%r11","%r12","%r13","%r14","%r15");

$code.=<<___;
# void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0);
.globl	ll_u256_mont_mul
.export	ll_u256_mont_mul
.type	ll_u256_mont_mul,\@function,5
.align	32
ll_u256_mont_mul:
    push %rbx
    push %rbp
    push %r12
    push %r13
    push %r14
    push %r15
    push $r_ptr

    sub \$8, %rsp

    mov $b_org, $b_ptr
    mov $k0, 0(%rsp)
    mov 0($a_ptr), $a0
    mov 8($a_ptr), $a1
    mov 16($a_ptr), $a2
    mov 24($a_ptr), $a3

    # acc4,acc3,acc2,acc1,acc0 = a * b[0]
    mov 0($b_ptr), $b
    mov $b, %rax
    mulq $a0
    mov %rax, $acc0
    mov $b, %rax
    mov %rdx, $acc1

    mulq $a1
    add %rax, $acc1
    adc \$0, %rdx
    mov $b, %rax
    mov %rdx, $acc2

    mulq $a2
    add %rax, $acc2
    adc \$0, %rdx
    mov $b, %rax
    mov %rdx, $acc3

    mulq $a3
    add %rax, $acc3
    adc \$0, %rdx
    mov $acc0, %rax
    mov %rdx, $acc4

    # reduction
    mulq 0(%rsp)            # mulq k0
    mov %rax, %rdi
    ## acc5,acc4,acc3,acc2,acc1,acc0 += %rax * N
    mulq 0($N_ptr)
    add %rax, $acc0
    # $acc0 is free now, use it to hold carry
    adc \$0, %rdx
    mov %rdi, %rax
    mov %rdx, $acc0

    mulq 8($N_ptr)
    add $acc0, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc0

    mulq 16($N_ptr)
     xor $acc5, $acc5
    add $acc0, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc0

    mulq 24($N_ptr)
    add $acc0, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    adc %rdx, $acc4
    adc \$0, $acc5

    # acc0,acc5,acc4,acc3,acc2,acc1 += a * b[1]
    mov 8($b_ptr), $b
    mov $b, %rax
    mulq $a0
    add %rax, $acc1
    adc \$0, %rdx
    mov $b, %rax
    mov %rdx, $acc0

    mulq $a1
    add $acc0, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $b, %rax
    adc \$0, %rdx
    mov %rdx, $acc0

    mulq $a2
    add $acc0, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $b, %rax
    adc \$0, %rdx
    mov %rdx, $acc0

    mulq $a3
    add $acc0, $acc4
    adc \$0, %rdx
    xor $acc0, $acc0
    add %rax, $acc4
    adc %rdx, $acc5
     mov $acc1, %rax
    adc \$0, $acc0

    # reduction
    mulq 0(%rsp)            # mulq k0
    mov %rax, %rdi
    ## acc0,acc5,acc4,acc3,acc2,acc1 += %rax * N
    mulq 0($N_ptr)
    add %rax, $acc1
    # $acc1 is free now, use it to hold carry
    adc \$0, %rdx
    mov %rdi, %rax
    mov %rdx, $acc1

    mulq 8($N_ptr)
    add $acc1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc1

    mulq 16($N_ptr)
    add $acc1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc1

    mulq 24($N_ptr)
    add $acc1, $acc4
    adc \$0, %rdx
    add %rax, $acc4
    adc %rdx, $acc5
    adc \$0, $acc0

    # acc1,acc0,acc5,acc4,acc3,acc2 += a * b[2]
    mov 16($b_ptr), $b
    mov $b, %rax
    mulq $a0
    add %rax, $acc2
    adc \$0, %rdx
    mov $b, %rax
    mov %rdx, $acc1

    mulq $a1
    add $acc1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $b, %rax
    adc \$0, %rdx
    mov %rdx, $acc1

    mulq $a2
    add $acc1, $acc4
    adc \$0, %rdx
    add %rax, $acc4
    mov $b, %rax
    adc \$0, %rdx
    mov %rdx, $acc1

    mulq $a3
    add $acc1, $acc5
    adc \$0, %rdx
    xor $acc1, $acc1
    add %rax, $acc5
    adc %rdx, $acc0
     mov $acc2, %rax
    adc \$0, $acc1

    # reduction
    mulq 0(%rsp)            # mulq k0
    mov %rax, %rdi
    ## acc1,acc0,acc5,acc4,acc3,acc2 += %rax * N
    mulq 0($N_ptr)
    add %rax, $acc2
    # $acc2 is free now, use it to hold carry
    adc \$0, %rdx
    mov %rdi, %rax
    mov %rdx, $acc2

    mulq 8($N_ptr)
    add $acc2, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc2

    mulq 16($N_ptr)
    add $acc2, $acc4
    adc \$0, %rdx
    add %rax, $acc4
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc2

    mulq 24($N_ptr)
    add $acc2, $acc5
    adc \$0, %rdx
    add %rax, $acc5
    adc %rdx, $acc0
    adc \$0, $acc1

    # acc2,acc1,acc0,acc5,acc4,acc3 += a * b[3]
    mov 24($b_ptr), $b
    mov $b, %rax
    mulq $a0
    add %rax, $acc3
    adc \$0, %rdx
    mov $b, %rax
    mov %rdx, $acc2

    mulq $a1
    add $acc2, $acc4
    adc \$0, %rdx
    add %rax, $acc4
    mov $b, %rax
    adc \$0, %rdx
    mov %rdx, $acc2

    mulq $a2
    add $acc2, $acc5
    adc \$0, %rdx
    add %rax, $acc5
    mov $b, %rax
    adc \$0, %rdx
    mov %rdx, $acc2

    mulq $a3
    add $acc2, $acc0
    adc \$0, %rdx
    xor $acc2, $acc2
    add %rax, $acc0
    adc %rdx, $acc1
     mov $acc3, %rax
    adc \$0, $acc2

    # reduction
    mulq 0(%rsp)            # mulq k0
    mov %rax, %rdi
    ## acc2,acc1,acc0,acc5,acc4,acc3 += %rax * N
    mulq 0($N_ptr)
    add %rax, $acc3
    # $acc3 is free now, use it to hold carry
    adc \$0, %rdx
    mov %rdi, %rax
    mov %rdx, $acc3

    mulq 8($N_ptr)
    add $acc3, $acc4
    adc \$0, %rdx
    add %rax, $acc4
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc3

    mulq 16($N_ptr)
     mov $acc4, $a0
    add $acc3, $acc5
    adc \$0, %rdx
    add %rax, $acc5
    mov %rdi, %rax
    adc \$0, %rdx
    mov %rdx, $acc3

    mulq 24($N_ptr)
     mov $acc5, $a1
    add $acc3, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    adc %rdx, $acc1
    adc \$0, $acc2

    add \$8, %rsp
    pop $r_ptr

    sub 0($N_ptr), $acc4
     mov $acc0, $a2
    sbb 8($N_ptr), $acc5
    sbb 16($N_ptr), $acc0
     mov $acc1, $a3
    sbb 24($N_ptr), $acc1
    sbb \$0, $acc2

    cmovc $a0, $acc4
    cmovc $a1, $acc5
    mov $acc4, 0($r_ptr)
    cmovc $a2, $acc0
    mov $acc5, 8($r_ptr)
    cmovc $a3, $acc1
    mov $acc0, 16($r_ptr)
    mov $acc1, 24($r_ptr)

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbp
    pop %rbx
    ret
.size	ll_u256_mont_mul,.-ll_u256_mont_mul
___
}

{
my ($r_ptr,$a_ptr,$N_org,$k0,$N_ptr)=("%rdi","%rsi","%rdx","%rcx","%rbp");
my ($t0,$t1,$t4,$t5,$t6,$t7)=("%rbx","%rsi","%rsi","%rdx","%rcx","%rbp");
my ($N0,$N1,$N2,$N3)=("%r12","%r13","%r14","%r15");
my ($acc0,$acc1,$acc2,$acc3,$acc4,$acc5,$acc6,$acc7)=("%r8","%r9","%r10","%r11","%r12","%r13","%r14","%r15");

$code.=<<___;
# void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);
.globl	ll_u256_mont_sqr
.export	ll_u256_mont_sqr
.type	ll_u256_mont_sqr,\@function,4
.align	32
ll_u256_mont_sqr:
    push %rbx
    push %rbp
    push %r12
    push %r13
    push %r14
    push %r15

    # save $acc4~$acc7
    sub \$32, %rsp

    mov $N_org, $N_ptr
    mov 8*0($a_ptr), %rax
    mov 8*1($a_ptr), $acc6
    mov 8*2($a_ptr), $acc7
    mov 8*3($a_ptr), $acc0

    mov %rax, $acc5
    mulq $acc6              # a[1]*a[0]
    mov %rax, $acc1
    mov $acc7, %rax
    mov %rdx, $acc2

    mulq $acc5              # a[0]*a[2]
    add %rax, $acc2
    mov $acc0, %rax
    adc \$0, %rdx
    mov %rdx, $acc3

    mulq $acc5              # a[0]*a[3]
    add %rax, $acc3
     mov $acc7, %rax
    adc \$0, %rdx
    mov %rdx, $acc4

    mulq $acc6              # a[1]*a[2]
    add %rax, $acc3
    mov $acc0, %rax
    adc \$0, %rdx
    mov %rdx, $t0

    mulq $acc6              # a[1]*a[3]
    add %rax, $acc4
     mov $acc0, %rax
    adc \$0, %rdx
    add $t0, $acc4
    mov %rdx, $acc5
    adc \$0, $acc5

    mulq $acc7              # a[2]*a[3]
    xor $acc7, $acc7
    add %rax, $acc5
     mov 8*0($a_ptr), %rax
    mov %rdx, $acc6
    adc \$0, $acc6

    add $acc1, $acc1        # acc1:6<<1
    adc $acc2, $acc2
    adc $acc3, $acc3
    adc $acc4, $acc4
    adc $acc5, $acc5
    adc $acc6, $acc6
    adc \$0, $acc7

    mulq %rax
    mov %rax, $acc0
    mov 8*1($a_ptr), %rax
    mov %rdx, $t0

    mulq %rax
    add $t0, $acc1
    adc %rax, $acc2
    mov 8*2($a_ptr), %rax
    adc \$0, %rdx
    mov %rdx, $t0

    mulq %rax
    add $t0, $acc3
    adc %rax, $acc4
    mov 8*3($a_ptr), %rax
    adc \$0, %rdx
    mov $acc4, 0(%rsp)
    mov %rdx, $t0

    mulq %rax
    add $t0, $acc5
    adc %rax, $acc6
    mov $acc5, 8(%rsp)
    adc %rdx, $acc7
    mov $acc6, 16(%rsp)
    mov $acc7, 24(%rsp)

    # reduction
    mov 0($N_ptr), $N0
    mov 8($N_ptr), $N1
    mov 16($N_ptr), $N2
    mov 24($N_ptr), $N3
    # loop1: acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + ((acc0 * k0) mod 2^64) * N) / 2^64
    mov $acc0, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc0
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $acc0

    # loop2: acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + ((acc1 * k0) mod 2^64) * N) / 2^64
    mov $acc1, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc1
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $acc1

    # loop3: acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + ((acc2 * k0) mod 2^64) * N) / 2^64
    mov $acc2, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc2
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $acc2

    # loop4: acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + ((acc3 * k0) mod 2^64) * N) / 2^64
    mov $acc3, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc3
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $t0, %rax
    adc \$0, %rdx
     xor $t0, $t0
    mov %rdx, $acc3

    # restore acc4~acc7
    mov 0(%rsp), $t4
    mov 8(%rsp), $t5
    add $acc0, $t4
    mov 16(%rsp), $t6
    mov 24(%rsp), $t7
    adc $acc1, $t5
    mov $t4, $acc0
    adc $acc2, $t6
    mov $t5, $acc1
    adc $acc3, $t7
    adc \$0, $t0

    sub $N0, $t4
    mov $t6, $acc2
    sbb $N1, $t5
    sbb $N2, $t6
    mov $t7, $acc3
    sbb $N3, $t7
    sbb \$0, $t0

    cmovc $acc0, $t4
    cmovc $acc1, $t5
    mov	$t4, 8*0($r_ptr)
    cmovc $acc2, $t6
    mov	$t5, 8*1($r_ptr)
    cmovc $acc3, $t7
    mov	$t6, 8*2($r_ptr)
    mov	$t7, 8*3($r_ptr)

    add \$32, %rsp

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbp
    pop %rbx
    ret
.size	ll_u256_mont_sqr,.-ll_u256_mont_sqr


# void ll_u256_mont_reduce(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);
.globl	ll_u256_mont_reduce
.export	ll_u256_mont_reduce
.type	ll_u256_mont_reduce,\@function,4
.align	32
ll_u256_mont_reduce:
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15
    mov 0($a_ptr), $acc0
    mov 8($a_ptr), $acc1
    mov 16($a_ptr), $acc2
    mov 24($a_ptr), $acc3
    mov 0($N_org), $N0
    mov 8($N_org), $N1
    mov 16($N_org), $N2
    mov 24($N_org), $N3
    # loop1: acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + ((acc0 * k0) mod 2^64) * N) / 2^64
    mov $acc0, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc0
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $acc0

    # loop2: acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + ((acc1 * k0) mod 2^64) * N) / 2^64
    mov $acc1, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc1
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $acc1

    # loop3: acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + ((acc2 * k0) mod 2^64) * N) / 2^64
    mov $acc2, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc2
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc3
    adc \$0, %rdx
    add %rax, $acc3
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $acc2

    # loop4: acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + ((acc3 * k0) mod 2^64) * N) / 2^64
    mov $acc3, %rax
    mulq $k0
    mov %rax, $t0
    mulq $N0
    add %rax, $acc3
    adc \$0, %rdx
    mov $t0, %rax
    mov %rdx, $t1

    mulq $N1
    add $t1, $acc0
    adc \$0, %rdx
    add %rax, $acc0
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N2
    add $t1, $acc1
    adc \$0, %rdx
    add %rax, $acc1
    mov $t0, %rax
    adc \$0, %rdx
    mov %rdx, $t1

    mulq $N3
    add $t1, $acc2
    adc \$0, %rdx
    add %rax, $acc2
    mov $t0, %rax
    adc \$0, %rdx
     xor $t0, $t0
    mov %rdx, $acc3

    # conditional sub
    mov $acc0, $t4
    mov $acc1, $t5
    mov $acc2, $t6
    mov $acc3, %rax 
    sub $N0, $t4
    mov $t6, $acc2
    sbb $N1, $t5
    sbb $N2, $t6
    mov %rax, $acc3
    sbb $N3, %rax
    sbb \$0, $t0

    cmovc $acc0, $t4
    cmovc $acc1, $t5
    mov	$t4, 8*0($r_ptr)
    cmovc $acc2, $t6
    mov	$t5, 8*1($r_ptr)
    cmovc $acc3, %rax
    mov	$t6, 8*2($r_ptr)
    mov	%rax, 8*3($r_ptr)

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    ret
.size	ll_u256_mont_reduce,.-ll_u256_mont_reduce
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
