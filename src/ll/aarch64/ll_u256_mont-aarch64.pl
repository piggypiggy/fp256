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
my($rd,$ad,$bd,$Nd,$k0,$b)=("x0","x1","x2","x3","x4","x17");
my($a0,$a1,$a2,$a3,$N0,$N1,$N2,$N3)=map("x$_",(5..12));
my($acc0,$acc1,$acc2,$acc3,$acc4,$acc5)=map("x$_",(1,3,13..16));
my($t0,$t1,$t2)=("x17","x19","x20");

$code.=<<___;
# void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0);
.globl	ll_u256_mont_mul
.type	ll_u256_mont_mul,%function
.align	5
ll_u256_mont_mul:
    stp x29,x30,[sp,#-32]!
    add x29,sp,#0
    stp x19,x20,[sp,#16]

    ldp $a0,$a1,[$ad]
    ldp $a2,$a3,[$ad,#16]
    ldp $N0,$N1,[$Nd]
    ldp $N2,$N3,[$Nd,#16]

    ldr $b,[$bd],#8
    // a0 * b0
    mul   $acc0,$a0,$b
    umulh $acc1,$a0,$b
    // a1 * b0
    mul     $t1,$a1,$b
    umulh $acc2,$a1,$b
    adds $acc1,$acc1,$t1
     mul    $t1,$a2,$b
    adc $acc2,$acc2,xzr
    // a2 * b0
    umulh $acc3,$a2,$b
    adds $acc2,$acc2,$t1
     mul    $t1,$a3,$b
    adc $acc3,$acc3,xzr
    // a3 * b0
    umulh $acc4,$a3,$b
    adds $acc3,$acc3,$t1
    adc $acc4,$acc4,xzr

    // reduction
    mul $t0,$acc0,$k0
    // acc5,acc4,acc3,acc2,acc1,acc0 += $t0 * N
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc0,$acc0,$t1
    // $acc0 is free now
    // $t0 * N1
    umulh $acc0,$t0,$N1
     adcs $acc1,$acc1,$t2
    mul $t1,$t0,$N1
    adc $acc0,$acc0,xzr
    adds $acc1,$acc1,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc2,$acc2,$acc0
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc2,$acc2,$t1
    // $t0 * N3
    umulh $acc0,$t0,$N3
     adcs $acc3,$acc3,$t2
    mul $t1,$t0,$N3
    adc $acc0,$acc0,xzr
    adds $acc3,$acc3,$t1
    adcs $acc4,$acc4,$acc0
     ldr $b,[$bd],#8
    adc $acc5,xzr,xzr

    // acc0,acc5,acc4,acc3,acc2,acc1 += a * b[1]
    // a0 * b1
    mul $t1,$a0,$b
    umulh $t2,$a0,$b
    adds $acc1,$acc1,$t1
    // a1 * b1
    umulh $acc0,$a1,$b
     adcs $acc2,$acc2,$t2
    mul   $t1,$a1,$b
    adc $acc0,$acc0,xzr
    adds $acc2,$acc2,$t1
    // a2 * b1
    umulh $t2,$a2,$b
     adcs $acc3,$acc3,$acc0
    mul   $t1,$a2,$b
    adc $t2,$t2,xzr
    adds $acc3,$acc3,$t1
    // a3 * b1
    umulh $acc0,$a3,$b
     adcs $acc4,$acc4,$t2
    mul   $t1,$a3,$b
    adc $acc0,$acc0,xzr
    adds $acc4,$acc4,$t1
     mul $t0,$acc1,$k0
    adcs $acc5,$acc5,$acc0

    // reduction
    // acc0,acc5,acc4,acc3,acc2,acc1 += $t0 * N
    // $t0 * N0
    mul $t1,$t0,$N0
     adc $acc0,xzr,xzr
    umulh $t2,$t0,$N0
    adds $acc1,$acc1,$t1
    // $acc1 is free now
    // $t0 * N1
    umulh $acc1,$t0,$N1
     adcs $acc2,$acc2,$t2
    mul $t1,$t0,$N1
    adc $acc1,$acc1,xzr
    adds $acc2,$acc2,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc3,$acc3,$acc1
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc3,$acc3,$t1
    // $t0 * N3
    umulh $acc1,$t0,$N3
     adcs $acc4,$acc4,$t2
    mul $t1,$t0,$N3
    adc $acc1,$acc1,xzr
    adds $acc4,$acc4,$t1
    adcs $acc5,$acc5,$acc1
     ldr $b,[$bd],#8
    adc $acc0,$acc0,xzr

    // acc1,acc0,acc5,acc4,acc3,acc2 += a * b[2]
    // a0 * b2
    mul $t1,$a0,$b
    umulh $t2,$a0,$b
    adds $acc2,$acc2,$t1
    // a1 * b2
    umulh $acc1,$a1,$b
     adcs $acc3,$acc3,$t2
    mul   $t1,$a1,$b
    adc $acc1,$acc1,xzr
    adds $acc3,$acc3,$t1
    // a2 * b2
    umulh $t2,$a2,$b
     adcs $acc4,$acc4,$acc1
    mul   $t1,$a2,$b
    adc $t2,$t2,xzr
    adds $acc4,$acc4,$t1
    // a3 * b2
    umulh $acc1,$a3,$b
     adcs $acc5,$acc5,$t2
    mul   $t1,$a3,$b
    adc $acc1,$acc1,xzr
    adds $acc5,$acc5,$t1
     mul $t0,$acc2,$k0
    adcs $acc0,$acc0,$acc1

    # // reduction
    # // acc1,acc0,acc5,acc4,acc3,acc2 += $t0 * N
    # // $t0 * N0
    mul $t1,$t0,$N0
     adc $acc1,xzr,xzr
    umulh $t2,$t0,$N0
    adds $acc2,$acc2,$t1
    // $acc2 is free now
    // $t0 * N1
    umulh $acc2,$t0,$N1
     adcs $acc3,$acc3,$t2
    mul $t1,$t0,$N1
    adc $acc2,$acc2,xzr
    adds $acc3,$acc3,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc4,$acc4,$acc2
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc4,$acc4,$t1
    // $t0 * N3
    umulh $acc2,$t0,$N3
     adcs $acc5,$acc5,$t2
    mul $t1,$t0,$N3
    adc $acc2,$acc2,xzr
    adds $acc5,$acc5,$t1
    adcs $acc0,$acc0,$acc2
     ldr $b,[$bd],#8
    adc $acc1,$acc1,xzr

    // acc2,acc1,acc0,acc5,acc4,acc3 += a * b[3]
    // a0 * b3
    mul $t1,$a0,$b
    umulh $t2,$a0,$b
    adds $acc3,$acc3,$t1
    // a1 * b3
    umulh $acc2,$a1,$b
     adcs $acc4,$acc4,$t2
    mul   $t1,$a1,$b
    adc $acc2,$acc2,xzr
    adds $acc4,$acc4,$t1
    // a2 * b3
    umulh $t2,$a2,$b
     adcs $acc5,$acc5,$acc2
    mul   $t1,$a2,$b
    adc $t2,$t2,xzr
    adds $acc5,$acc5,$t1
    // a3 * b3
    umulh $acc2,$a3,$b
     adcs $acc0,$acc0,$t2
    mul   $t1,$a3,$b
    adc $acc2,$acc2,xzr
    adds $acc0,$acc0,$t1
     mul $t0,$acc3,$k0
    adcs $acc1,$acc1,$acc2

    // reduction
    // acc2,acc1,acc0,acc5,acc4,acc3 += $t0 * N
    // $t0 * N0
    mul $t1,$t0,$N0
     adc $acc2,xzr,xzr
    umulh $t2,$t0,$N0
    adds $acc3,$acc3,$t1
    // $acc3 is free now
    // $t0 * N1
    umulh $acc3,$t0,$N1
     adcs $acc4,$acc4,$t2
    mul $t1,$t0,$N1
    adc $acc3,$acc3,xzr
    adds $acc4,$acc4,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc5,$acc5,$acc3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc5,$acc5,$t1
    // $t0 * N3
    umulh $acc3,$t0,$N3
     adcs $acc0,$acc0,$t2
    mul $t1,$t0,$N3
    adc $acc3,$acc3,xzr
    adds $acc0,$acc0,$t1
    adcs $acc1,$acc1,$acc3
    adc $acc2,$acc2,xzr

    subs $a0,$acc4,$N0
    sbcs $a1,$acc5,$N1
    sbcs $a2,$acc0,$N2
    sbcs $a3,$acc1,$N3
    sbcs $acc2,$acc2,xzr

    csel $acc4,$acc4,$a0,cc
    csel $acc5,$acc5,$a1,cc
    csel $acc0,$acc0,$a2,cc
    stp $acc4,$acc5,[$rd]
    csel $acc1,$acc1,$a3,cc

    ldp x19,x20,[sp,#16]
    stp $acc0,$acc1,[$rd,#16]
    ldp x29,x30,[sp],#32
    ret
.size	ll_u256_mont_mul,.-ll_u256_mont_mul
___
}

{
my($rd,$ad,$Nd,$k0)=("x0","x1","x2","x3");
my($a0,$a1,$a2,$a3)=("x4","x5","x6","x7");
my($N0,$N1,$N2,$N3)=("x8","x9","x10","x11");
my($acc0,$acc1,$acc2,$acc3,$acc4,$acc5,$acc6,$acc7)=map("x$_",(12..17,4,5));
my($t0,$t1,$t2,$t3)=("x1","x2","x6","x7");

$code.=<<___;
// TODO : this code is ugly.
# void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);
.globl	ll_u256_mont_sqr
.type	ll_u256_mont_sqr,%function
.align	5
ll_u256_mont_sqr:
    ldp $a0,$a1,[$ad]
    ldp $a2,$a3,[$ad,#16]
    // ldp $N0,$N1,[$Nd]
    # ldp $N2,$N3,[$Nd,#16]
    // a1 * a0
    mul   $acc1,$a1,$a0
    umulh $acc2,$a1,$a0
    // a2 * a0
    mul     x11,$a2,$a0
    umulh $acc3,$a2,$a0
    adds $acc2,$acc2,x11
     mul    x10,$a3,$a0
    adc $acc3,$acc3,xzr
    // a3 * a0
    umulh $acc4,$a3,$a0
    adds $acc3,$acc3,x10
     mul  x11,$a2,$a1
    adc $acc4,$acc4,xzr

    // a2 * a1
    umulh x9,$a2,$a1
    adds $acc3,$acc3,x11
    // a3 * a1
    umulh $acc5,$a3,$a1
     adcs $acc4,$acc4,x9
    mul   x11,$a3,$a1
    adc $acc5,$acc5,xzr
    adds $acc4,$acc4,x11
    // a3 * a2
    umulh x8,$a3,$a2
     adcs $acc5,$acc5,xzr
    mul   x11,$a3,$a2
    adc x8,x8,xzr
    adds $acc5,$acc5,x11
     eor x9,xzr,xzr
    adc x8,x8,xzr

    // (acc7 ~ acc1) << 1 + a0^2 + a1^2 + a2^2 + a3^2
    extr x9,x9,x8,#63
     mul $acc0,$a0,$a0
    extr x8,x8,$acc5,#63
     umulh x10,$a0,$a0
    extr $acc5,$acc5,$acc4,#63
     mul x4,$a1,$a1
    extr $acc4,$acc4,$acc3,#63
     umulh x5,$a1,$a1
    extr $acc3,$acc3,$acc2,#63
     mul $t0,$a2,$a2
    extr $acc2,$acc2,$acc1,#63
    lsl $acc1,$acc1,#1
    adds $acc1,$acc1,x10
    adcs $acc2,$acc2,x4
    umulh x10,$a3,$a3
    adcs $acc3,$acc3,x5
    adcs $acc4,$acc4,$t0
     umulh $t0,$a2,$a2
     mul x4,$a3,$a3
    adcs $acc5,$acc5,$t0
    adcs $acc6,x8,x4
    adc $acc7,x9,x10

    // reduction
    ldp $N0,$N1,[$Nd]
    ldp $N2,$N3,[$Nd,#16]
    // loop1: acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + ((acc0 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc0 * k0) mod 2^64
    mul $t0,$acc0,$k0
    // acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc0,$acc0,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc1,$acc1,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc1,$acc1,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc2,$acc2,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc2,$acc2,$t1
    // $t0 * N3
    umulh $acc0,$t0,$N3
     adcs $acc3,$acc3,$t2
    mul $t1,$t0,$N3
    adc $acc0,$acc0,xzr
    adds $acc3,$acc3,$t1
    adc $acc0,$acc0,xzr

    // loop2: acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + ((acc1 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc1 * k0) mod 2^64
    mul $t0,$acc1,$k0
    // acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc1,$acc1,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc2,$acc2,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc2,$acc2,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc3,$acc3,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc3,$acc3,$t1
    // $t0 * N3
    umulh $acc1,$t0,$N3
     adcs $acc0,$acc0,$t2
    mul $t1,$t0,$N3
    adc $acc1,$acc1,xzr
    adds $acc0,$acc0,$t1
    adc $acc1,$acc1,xzr

    // loop3: acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + ((acc2 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc2 * k0) mod 2^64
    mul $t0,$acc2,$k0
    // acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc2,$acc2,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc3,$acc3,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc3,$acc3,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc0,$acc0,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc0,$acc0,$t1
    // $t0 * N3
    umulh $acc2,$t0,$N3
     adcs $acc1,$acc1,$t2
    mul $t1,$t0,$N3
    adc $acc2,$acc2,xzr
    adds $acc1,$acc1,$t1
    adc $acc2,$acc2,xzr

    // loop4: acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + ((acc3 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc3 * k0) mod 2^64
    mul $t0,$acc3,$k0
    // acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc3,$acc3,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc0,$acc0,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc0,$acc0,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc1,$acc1,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc1,$acc1,$t1
    // $t0 * N3
    umulh $acc3,$t0,$N3
     adcs $acc2,$acc2,$t2
    mul $t1,$t0,$N3
    adc $acc3,$acc3,xzr
    adds $acc2,$acc2,$t1
    adc $acc3,$acc3,xzr

    adds $acc4,$acc4,$acc0
    adcs $acc5,$acc5,$acc1
    adcs $acc6,$acc6,$acc2
    adcs $acc7,$acc7,$acc3
    adc $t0,xzr,xzr
    subs x8,$acc4,$N0
    sbcs x9,$acc5,$N1
    sbcs x10,$acc6,$N2
    sbcs x11,$acc7,$N3
    sbcs $t0,$t0,xzr

    csel $acc4,$acc4,x8,cc
    csel $acc5,$acc5,x9,cc
    csel $acc6,$acc6,x10,cc
    stp $acc4,$acc5,[$rd]
    csel $acc7,$acc7,x11,cc
    stp $acc6,$acc7,[$rd,#16]

    ret
.size	ll_u256_mont_sqr,.-ll_u256_mont_sqr


# void ll_u256_mont_reduce(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0);
.globl	ll_u256_mont_reduce
.type	ll_u256_mont_reduce,%function
.align	5
ll_u256_mont_reduce:
    ldp $acc0,$acc1,[$ad]
    ldp $acc2,$acc3,[$ad,#16]
    // reduction
    ldp $N0,$N1,[$Nd]
    ldp $N2,$N3,[$Nd,#16]
    // loop1: acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + ((acc0 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc0 * k0) mod 2^64
    mul $t0,$acc0,$k0
    // acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc0,$acc0,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc1,$acc1,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc1,$acc1,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc2,$acc2,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc2,$acc2,$t1
    // $t0 * N3
    umulh $acc0,$t0,$N3
     adcs $acc3,$acc3,$t2
    mul $t1,$t0,$N3
    adc $acc0,$acc0,xzr
    adds $acc3,$acc3,$t1
    adc $acc0,$acc0,xzr

    // loop2: acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + ((acc1 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc1 * k0) mod 2^64
    mul $t0,$acc1,$k0
    // acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc1,$acc1,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc2,$acc2,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc2,$acc2,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc3,$acc3,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc3,$acc3,$t1
    // $t0 * N3
    umulh $acc1,$t0,$N3
     adcs $acc0,$acc0,$t2
    mul $t1,$t0,$N3
    adc $acc1,$acc1,xzr
    adds $acc0,$acc0,$t1
    adc $acc1,$acc1,xzr

    // loop3: acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + ((acc2 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc2 * k0) mod 2^64
    mul $t0,$acc2,$k0
    // acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc2,$acc2,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc3,$acc3,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc3,$acc3,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc0,$acc0,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc0,$acc0,$t1
    // $t0 * N3
    umulh $acc2,$t0,$N3
     adcs $acc1,$acc1,$t2
    mul $t1,$t0,$N3
    adc $acc2,$acc2,xzr
    adds $acc1,$acc1,$t1
    adc $acc2,$acc2,xzr

    // loop4: acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + ((acc3 * k0) mod 2^64) * N) / 2^64
    // $t0 = (acc3 * k0) mod 2^64
    mul $t0,$acc3,$k0
    // acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + t0 * N) / 2^64
    // $t0 * N0
    mul $t1,$t0,$N0
    umulh $t2,$t0,$N0
    adds $acc3,$acc3,$t1
    // $t0 * N1
    umulh $t3,$t0,$N1
     adcs $acc0,$acc0,$t2
    mul $t1,$t0,$N1
    adc $t3,$t3,xzr
    adds $acc0,$acc0,$t1
    // $t0 * N2
    umulh $t2,$t0,$N2
     adcs $acc1,$acc1,$t3
    mul $t1,$t0,$N2
    adc $t2,$t2,xzr
    adds $acc1,$acc1,$t1
    // $t0 * N3
    umulh $acc3,$t0,$N3
     adcs $acc2,$acc2,$t2
    mul $t1,$t0,$N3
    adc $acc3,$acc3,xzr
    adds $acc2,$acc2,$t1
    adc $acc3,$acc3,xzr

    mov $t0, xzr
    subs x8,$acc0,$N0
    sbcs x9,$acc1,$N1
    sbcs x10,$acc2,$N2
    sbcs x11,$acc3,$N3
    sbcs $t0,$t0,xzr

    csel $acc0,$acc0,x8,cc
    csel $acc1,$acc1,x9,cc
    csel $acc2,$acc2,x10,cc
    stp $acc0,$acc1,[$rd]
    csel $acc3,$acc3,x11,cc
    stp $acc2,$acc3,[$rd,#16]

    ret
.size	ll_u256_mont_reduce,.-ll_u256_mont_reduce
___
}

print $code;
close STDOUT;
