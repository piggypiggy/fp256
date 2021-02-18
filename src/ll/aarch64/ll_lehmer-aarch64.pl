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

$0 =~ m/(.*[\/\\])[^\/\\]+$/; $dir=$1;
( $xlate="${dir}arm-xlate.pl" and -f $xlate ) or
( $xlate="${dir}../perlasm/arm-xlate.pl" and -f $xlate) or
die "can't locate arm-xlate.pl";

open OUT,"| \"$^X\" $xlate $flavour $output";
*STDOUT=*OUT;

$code.=<<___;
.text
___

{
my ($a1,$a0,$b1,$b0)=("x1","x2","x3","x4");
my ($t0,$t1,$t2,$t3)=("x10","x11","x12","x13");
my ($num,$thres)=("x0","x15");
my ($mat_ptr,$m00,$m01,$m10,$m11)=("x14","x5","x6","x7","x9");
$code.=<<___;
# u64 ll_div_22(u64 a1, u64 a0, u64 b1, u64 b0);
.type	ll_div_22,%function
.align  5
ll_div_22:
    clz $t0,$a1
    mov x16,$b0
    clz $t1,$b1
    mov x17,$b1
    lsr $t3,$b0,#1
    sub $t1,$t1,$t0
    mov $t2,#63
    lsl $b1,$b1,$t1
    sub $t2,$t2,$t1
    lsl $b0,$b0,$t1
    lsr $t3,$t3,$t2
     eor $t0,xzr,xzr
    orr $b1,$b1,$t3

.ll_div_22_loop:
    lsl $t0,$t0,#1
    // conditional sub
    subs $t2,$a0,$b0
     sub $t1,$t1,#1
    sbcs $t3,$a1,$b1
    extr $b0,$b1,$b0,#1
    csel $a0,$t2,$a0,cs
    csel $a1,$t3,$a1,cs
    lsr $b1,$b1,#1
    cinc $t0,$t0,cs
    cmp $t1,xzr
    bge .ll_div_22_loop

    mov $b0,$a0
    mov $b1,$a1
    mov $a0,x16
    mov $a1,x17
    ret
.size	ll_div_22,.-ll_div_22


# u64 ll_lehmer_exgcd11(u64 mat[4], u64 a0, u64 b0);
.globl	ll_lehmer_exgcd11
.type	ll_lehmer_exgcd11,%function
.align	5
ll_lehmer_exgcd11:
    mov $mat_ptr,x0
    eor $num,xzr,xzr         // $num tracks number of matrix multiplications
    mov $b0,x2
    mov $a0,x1

    // init mat = [1, 0;0, 1]
    mov $m00,#1
    mov $m01,xzr
    mov $m10,xzr
    mov $m11,#1

    cmp $a0,$b0
    bls .ll_lehmer_exgcd11_swap
    b .ll_lehmer_exgcd11_loop

.ll_lehmer_exgcd11_swap:
    // init mat = [0, 1;1, 0]
    mov $m00,xzr
    mov $m01,#1
    mov $m10,#1
    mov $m11,xzr
    // swap a and b
    mov $t0,$a0
    mov $a0,$b0
     add $num,$num,#1
    mov $b0,$t0

.ll_lehmer_exgcd11_loop:
    cmp $b0,xzr
    beq .ll_lehmer_exgcd11_done
    // ll_div_11
    udiv $t3,$a0,$b0
    mov $t1,$b0
    mul $t0,$t3,$b0
    sub $b0,$a0,$t0
    mov $a0,$t1
    mul $t2,$t3,$m10
    add $t2,$t2,$m00
    mov $m00,$m10
    mov $m10,$t2
    mul $t3,$t3,$m11
     add $num,$num,#1
    add $t3,$t3,$m01
    mov $m01,$m11
    mov $m11,$t3
    b .ll_lehmer_exgcd11_loop

.ll_lehmer_exgcd11_done:
    stp $m00,$m01,[$mat_ptr]
    stp $m10,$m11,[$mat_ptr,#16]
    ret
.size	ll_lehmer_exgcd11,.-ll_lehmer_exgcd11


# u64 ll_lehmer_simulate(u64 mat[4], u64 a1, u64 a0, u64 b1, u64 b0);
.globl	ll_lehmer_simulate
.type	ll_lehmer_simulate,%function
.align	5
ll_lehmer_simulate:
    stp x29,x30,[sp,#-32]!
    add x29,sp,#0

    mov $mat_ptr,x0
    eor $num,xzr,xzr         // $num tracks number of matrix multiplications

    mov $thres,#0x100000000
    // init mat = [1, 0;0, 1]
    mov $m00,#1
    mov $m01,xzr
    mov $m10,xzr
    mov $m11,#1

    cmp $b1,xzr
    beq .ll_lehmer_simulate_done
    subs $t0,$a0,$b0
    sbcs $t1,$a1,$b1
    blo .ll_lehmer_simulate_swap
    b .ll_lehmer_simulate_div2_loop

.ll_lehmer_simulate_swap:
    // init mat = [0, 1;1, 0]
    mov $m00,xzr
    mov $m01,#1
    mov $m10,#1
    mov $m11,xzr
    // swap a and b
    mov $t0,$a0
    mov $t1,$a1
    mov $a0,$b0
    mov $a1,$b1
     add $num,$num,#1
    mov $b0,$t0
    mov $b1,$t1

.ll_lehmer_simulate_div2_loop:
    cmp $thres,$a1
    bhi .ll_lehmer_simulate_div1_loop_pre
    # usually(~50%), quotient is 1
    subs $t0,$a0,$b0
    sbc $t1,$a1,$b1
    cmp $t1,$b1
    bhi .do_div2                // a1:a0 > 2 * b1:b0
    cmp $t1,xzr
    beq .ll_lehmer_simulate_done
    // mat = [0, 1;1, 1] * mat
    mov $t2,$m10
    mov $a0,$b0
    mov $a1,$b1
    add $m10,$m10,$m00
    mov $m00,$t2
     add $num,$num,#1
    mov $t2,$m11
    mov $b0,$t0
    mov $b1,$t1
    add $m11,$m11,$m01
    mov $m01,$t2
    b .ll_lehmer_simulate_div2_loop

.do_div2:
    bl ll_div_22
    cmp $b1,xzr
    beq .ll_lehmer_simulate_done
    // mat = [0, 1;1, q] * mat
    mul $t2,$t0,$m10
    add $t2,$t2,$m00
    mov $m00,$m10
    mul $t3,$t0,$m11
    mov $m10,$t2
     add $num,$num,#1
    add $t3,$t3,$m01
    mov $m01,$m11
    mov $m11,$t3
    b .ll_lehmer_simulate_div2_loop

.ll_lehmer_simulate_div1_loop_pre:
    mov $thres,#0x200000000
    extr $a0,$a1,$a0,#32
    extr $b0,$b1,$b0,#32

.ll_lehmer_simulate_div1_loop:
    // usually(~50%), quotient is 1
    sub $t0,$a0,$b0
    cmp $t0,$b0
    bhi .do_div1
    cmp $thres,$t0
    bhi .ll_lehmer_simulate_done
    // mat = [0, 1;1, 1] * mat
    mov $t2,$m10
     mov $a0,$b0
    add $m10,$m10,$m00
    mov $m00,$t2
     add $num,$num,#1
    mov $t2,$m11
     mov $b0,$t0
    add $m11,$m11,$m01
    mov $m01,$t2
    b .ll_lehmer_simulate_div1_loop

.do_div1:
    // ll_div_11
    udiv $t3,$a0,$b0
    mov $t1,$b0
    mul $t0,$t3,$b0
    sub $b0,$a0,$t0
    mov $a0,$t1
    cmp $thres,$b0
    bhi .ll_lehmer_simulate_done
    // mat = [0, 1;1, q] * mat
    mul $t2,$t3,$m10
    add $t2,$t2,$m00
    mov $m00,$m10
    mul $t3,$t3,$m11
    mov $m10,$t2
     add $num,$num,#1
    add $t3,$t3,$m01
    mov $m01,$m11
    mov $m11,$t3
    b .ll_lehmer_simulate_div1_loop

.ll_lehmer_simulate_done:
    stp $m00,$m01,[$mat_ptr]
    stp $m10,$m11,[$mat_ptr,#16]

    ldp x29,x30,[sp],#32
    ret
.size	ll_lehmer_simulate,.-ll_lehmer_simulate
___
}

print $code;
close STDOUT;
