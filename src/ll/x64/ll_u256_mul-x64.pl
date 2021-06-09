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
my ($r_ptr,$a_ptr,$b_org,$b_ptr)=("%rdi","%rsi","%rdx","%r13");
my ($al,$bl)=("%r14","%r15");
my ($acc0,$acc1,$acc2,$acc3)=map("%r$_",(8..11));
my ($t0,$t1,$t2,$t3,$t4)=("%rcx","%rbp","%rbx","%rsi","%r12");
$code.=<<___;
# u64 ll_u256_mul_limb(u64 r[4], u64 a[4], u64 b);
.globl	ll_u256_mul_limb
.export	ll_u256_mul_limb
.type	ll_u256_mul_limb,\@function,3
.align	32
ll_u256_mul_limb:
    mov $b_org, %rcx        # save b

    mov %rcx, %rax
    mulq 0*8($a_ptr)        # a[0]*b
    mov %rax, $acc0
    mov %rdx, $acc1
     mov %rcx, %rax
    mov $acc0, 0*8($r_ptr)

    mulq 1*8($a_ptr)        # a[1]*b
    add %rax, $acc1
    mov %rdx, $acc2
     mov %rcx, %rax
    mov $acc1, 1*8($r_ptr)
    adc \$0, $acc2

    mulq 2*8($a_ptr)        # a[2]*b
    add %rax, $acc2
    mov %rdx, $acc3
     mov %rcx, %rax
    mov $acc2, 2*8($r_ptr)
    adc \$0, $acc3

    mulq 3*8($a_ptr)        # a[3]*b
    add %rax, $acc3
    adc \$0, %rdx           # carry
    mov $acc3, 3*8($r_ptr)
    mov %rdx, %rax

    ret
.size	ll_u256_mul_limb,.-ll_u256_mul_limb


# void ll_u256_mul(u64 r[8], u64 a[4], u64 b[4]);
.globl	ll_u256_mul
.export	ll_u256_mul
.type	ll_u256_mul,\@function,3
.align	32
ll_u256_mul:
    push	%rbp
    push	%rbx
    push	%r12
    push	%r13

    mov $b_org, $b_ptr

    mov 0*8($a_ptr), $t0
    mov 1*8($a_ptr), $t1
    mov 2*8($a_ptr), $t2
    mov 3*8($a_ptr), $t3

#mul_b0:
    mov 0*8($b_ptr), $t4
    mov $t4, %rax
    mulq $t0                # a[0]*b[0]
    mov %rax, $acc0
    mov %rdx, $acc1
     mov $t4, %rax
    mov $acc0, 0*8($r_ptr)

    mulq $t1                # a[1]*b[0]
    add %rax, $acc1
    mov %rdx, $acc2
     mov $t4, %rax
    adc \$0, $acc2

    mulq $t2                # a[2]*b[0]
    add %rax, $acc2
    mov %rdx, $acc3
     mov $t4, %rax
    adc \$0, $acc3

    mulq $t3                # a[3]*b[0]
    add %rax, $acc3
    mov %rdx, $acc0
     mov 1*8($b_ptr), $t4
    adc \$0, $acc0

#mul_b1:
    mov $t4, %rax
    mulq $t0                # a[0]*b[1]
    add %rax, $acc1
    adc %rdx, $acc2
    adc \$0, $acc3 
    mov $acc1, 1*8($r_ptr)
    adc \$0, $acc0
    mov \$0, $acc1
     mov $t4, %rax
    adc \$0, $acc1

    mulq $t1                # a[1]*b[1]
    add %rax, $acc2
    adc %rdx, $acc3
    adc \$0, $acc0
     mov $t4, %rax
    adc \$0, $acc1 

    mulq $t2                # a[2]*b[1]
    add %rax, $acc3
    adc %rdx, $acc0
     mov $t4, %rax
    adc \$0, $acc1

    mulq $t3                # a[3]*b[1]
    add %rax, $acc0
     mov 2*8($b_ptr), $t4
    adc %rdx, $acc1

#mul_b2:
    mov $t4, %rax
    mulq $t0                # a[0]*b[2]
    add %rax, $acc2
    adc %rdx, $acc3
    adc \$0, $acc0
    mov $acc2, 2*8($r_ptr)
    adc \$0, $acc1
    mov \$0, $acc2
     mov $t4, %rax
    adc \$0, $acc2

    mulq $t1                # a[1]*b[2]
    add %rax, $acc3
    adc %rdx, $acc0
    adc \$0, $acc1
     mov $t4, %rax
    adc \$0, $acc2

    mulq $t2                # a[2]*b[2]
    add %rax, $acc0
    adc %rdx, $acc1
     mov $t4, %rax
    adc \$0, $acc2

    mulq $t3                # a[3]*b[2]
    add %rax, $acc1
     mov 3*8($b_ptr), $t4
    adc %rdx, $acc2

#mul_b3:
    mov $t4, %rax
    mulq $t0                # a[0]*b[3]
    add %rax, $acc3
    adc %rdx, $acc0
    adc \$0, $acc1
    mov $acc3, 3*8($r_ptr)
    adc \$0, $acc2
    mov \$0, $acc3
     mov $t4, %rax
    adc \$0, $acc3

    mulq $t1                # a[1]*b[3]
    add %rax, $acc0
    adc %rdx, $acc1
    adc \$0, $acc2
     mov $t4, %rax
    adc \$0, $acc3

    mov $acc0, 4*8($r_ptr)
    mulq $t2                # a[2]*b[3]
    add %rax, $acc1
    adc %rdx, $acc2
     mov $t4, %rax
    adc \$0, $acc3

    mov $acc1, 5*8($r_ptr)
    mulq $t3                # a[3]*b[3]
    add %rax, $acc2
    adc %rdx, $acc3

    mov $acc2, 6*8($r_ptr)
    mov $acc3, 7*8($r_ptr)

    pop	%r13
    pop	%r12
    pop	%rbx
    pop	%rbp
    ret
.size	ll_u256_mul,.-ll_u256_mul


# void ll_u256_mullo(u64 r[4], u64 a[4], u64 b[4]);
.globl	ll_u256_mullo
.export	ll_u256_mullo
.type	ll_u256_mullo,\@function,3
.align	32
ll_u256_mullo:
    push	%rbp
    push	%rbx
    push	%r12
    push	%r13

    mov $b_org, $b_ptr

    mov 0*8($a_ptr), $t0
    mov 1*8($a_ptr), $t1
    mov 2*8($a_ptr), $t2
    mov 3*8($a_ptr), $t3

#mul_b0:
    mov 0*8($b_ptr), $t4
    mov $t4, %rax
    mulq $t0                # a[0]*b[0]
    mov %rax, $acc0
    mov %rdx, $acc1
     mov $t4, %rax
    mov $acc0, 0*8($r_ptr)

    mulq $t1                # a[1]*b[0]
    add %rax, $acc1
    mov %rdx, $acc2
     mov $t4, %rax
    adc \$0, $acc2

    mulq $t2                # a[2]*b[0]
    add %rax, $acc2
    mov %rdx, $acc3
     mov $t4, %rax
    adc \$0, $acc3

    mulq $t3                # a[3]*b[0]
     mov 1*8($b_ptr), $t4
    add %rax, $acc3

#mul_b1:
    mov $t4, %rax
    mulq $t0                # a[0]*b[1]
    add %rax, $acc1
    adc %rdx, $acc2
     mov $t4, %rax
    adc \$0, $acc3 
    mov $acc1, 1*8($r_ptr)

    mulq $t1                # a[1]*b[1]
    add %rax, $acc2
     mov $t4, %rax
    adc %rdx, $acc3

    mulq $t2                # a[2]*b[1]
    add %rax, $acc3
     mov 2*8($b_ptr), $t4

#mul_b2:
    mov $t4, %rax
    mulq $t0                # a[0]*b[2]
    add %rax, $acc2
     mov $t4, %rax
    adc %rdx, $acc3
    mov $acc2, 2*8($r_ptr)

    mulq $t1                # a[1]*b[2]
    add %rax, $acc3

#mul_b3:
    mov 3*8($b_ptr), %rax
    mulq $t0                # a[0]*b[3]
    add %rax, $acc3
    mov $acc3, 3*8($r_ptr)

    pop	%r13
    pop	%r12
    pop	%rbx
    pop	%rbp
    ret
.size	ll_u256_mullo,.-ll_u256_mullo
___
}

{
my ($r_ptr,$a_ptr)=("%rdi","%rsi");
my ($acc0,$acc1,$acc2,$acc3)=("%rcx","%rbp","%rbx","%r12");
my ($acc4,$acc5,$acc6,$acc7)=map("%r$_",(8..11));
my ($t0)=("%r13");
$code.=<<___;

# void ll_u256_sqr(u64 r[8], u64 a[4]);
.globl	ll_u256_sqr
.export	ll_u256_sqr
.type	ll_u256_sqr,\@function,2
.align	32
ll_u256_sqr:
    push %rbp
    push %rbx
    push %r12
    push %r13

    mov 8*0($a_ptr), %rax
    mov 8*1($a_ptr), $acc6
    mov 8*2($a_ptr), $acc7
    mov 8*3($a_ptr), $acc0

    mov %rax, $acc5
    mulq	$acc6			# a[1]*a[0]
    mov %rax, $acc1
    mov $acc7, %rax
    mov %rdx, $acc2

    mulq	$acc5			# a[0]*a[2]
    add %rax, $acc2
    mov $acc0, %rax
    adc \$0, %rdx
    mov %rdx, $acc3

    mulq	$acc5			# a[0]*a[3]
    add %rax, $acc3
     mov	$acc7, %rax
    adc \$0, %rdx
    mov %rdx, $acc4

    #################################
    mulq	$acc6			# a[1]*a[2]
    add %rax, $acc3
    mov $acc0, %rax
    adc \$0, %rdx
    mov %rdx, $t0

    mulq	$acc6			# a[1]*a[3]
    add %rax, $acc4
     mov	$acc0, %rax
    adc \$0, %rdx
    add $t0, $acc4
    mov %rdx, $acc5
    adc \$0, $acc5

    #################################
    mulq	$acc7			# a[2]*a[3]
    xor $acc7, $acc7
    add %rax, $acc5
     mov	8*0($a_ptr), %rax
    mov %rdx, $acc6
    adc \$0, $acc6

    add $acc1, $acc1		# acc1:6<<1
    adc $acc2, $acc2
    adc $acc3, $acc3
    adc $acc4, $acc4
    adc $acc5, $acc5
    adc $acc6, $acc6
    adc \$0, $acc7

    mulq	%rax
    mov %rax, $acc0
    mov 8*1($a_ptr), %rax
    mov %rdx, $t0
    mov $acc0, 0*8($r_ptr)

    mulq	%rax
    add $t0, $acc1
    adc %rax, $acc2
    mov 8*2($a_ptr), %rax
    adc \$0, %rdx
    mov %rdx, $t0
    mov $acc1, 1*8($r_ptr)

    mulq	%rax
    add $t0, $acc3
    mov $acc2, 2*8($r_ptr)
    adc %rax, $acc4
    mov 8*3($a_ptr), %rax
    adc \$0, %rdx
    mov %rdx, $t0
    mov $acc3, 3*8($r_ptr)

    mulq	%rax
    add $t0, $acc5
    mov $acc4, 4*8($r_ptr)
    adc %rax, $acc6
    #  mov	$acc0, %rax
    adc %rdx, $acc7
    mov $acc5, 5*8($r_ptr)
    mov $acc6, 6*8($r_ptr)
    mov $acc7, 7*8($r_ptr)

    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_u256_sqr,.-ll_u256_sqr


# void ll_u256_sqrlo(u64 r[4], u64 a[4]);
.globl	ll_u256_sqrlo
.export	ll_u256_sqrlo
.type	ll_u256_sqrlo,\@function,2
.align	32
ll_u256_sqrlo:
    push %rbp
    push %rbx
    push %r12

    mov 8*0($a_ptr), %rax
    mov 8*1($a_ptr), $acc6
    mov 8*2($a_ptr), $acc7
    mov 8*3($a_ptr), $acc0

    mov %rax, $acc5
    mulq $acc6          # a[1]*a[0]
    mov %rax, $acc1
    mov $acc7, %rax
    mov %rdx, $acc2

    mulq $acc5          # a[0]*a[2]
    add %rax, $acc2
    mov $acc0, %rax
    adc \$0, %rdx
    mov %rdx, $acc3

    mulq $acc5          # a[0]*a[3]
    add %rax, $acc3
    mov $acc7, %rax

    #################################
    mulq $acc6          # a[1]*a[2]
    mov \$0, $acc7
    add %rax, $acc3

    add $acc1, $acc1    # acc1:3<<1
     mov $acc5, %rax
    adc $acc2, $acc2
    adc $acc3, $acc3

    mulq %rax
    mov %rax, $acc0
    mov $acc6, %rax
    add %rdx, $acc1
     mov $acc0, 0*8($r_ptr)
    adc \$0, $acc7      # carry

    mulq %rax
     mov $acc1, 1*8($r_ptr)
    add %rax, $acc2
    adc %rdx, $acc3
    add $acc7, $acc2
    adc \$0, $acc3

    mov $acc2, 2*8($r_ptr)
    mov $acc3, 3*8($r_ptr)

    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_u256_sqrlo,.-ll_u256_sqrlo
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
