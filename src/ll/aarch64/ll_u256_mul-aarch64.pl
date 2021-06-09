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
my($rd,$ad,$bd,$b)=("x0","x1","x2","x2");
my($a0,$a1,$a2,$a3,$b0,$b1,$b2,$b3)=map("x$_",(3..10));
my($acc0,$acc1,$acc2,$acc3,$acc4,$acc5,$acc6,$acc7)=map("x$_",(11..17,10));
my($t0,$t1,$t2,$t3,$t4)=map("x$_",(1,2,7..9));

$code.=<<___;
// u64 ll_u256_mul_limb(u64 r[4], u64 a[4], u64 b);
.globl	ll_u256_mul_limb
.type	ll_u256_mul_limb,%function
.align	5
ll_u256_mul_limb:
    ldp $a0,$a1,[$ad]
    ldp $a2,$a3,[$ad,#16]
    // a0 * b
    mul   $acc0,$a0,$b
    umulh $acc1,$a0,$b
    // a1 * b
    mul     $t0,$a1,$b
    umulh $acc2,$a1,$b
    adds $acc1,$acc1,$t0
     mul    $t2,$a2,$b
    adc $acc2,$acc2,xzr
    // a2 * b
    umulh $acc3,$a2,$b
    adds $acc2,$acc2,$t2
     mul    $t0,$a3,$b
    adc $acc3,$acc3,xzr
    // rd[0] = $acc0, rd[1] = $acc1
    stp $acc0,$acc1,[$rd]
    // a3 * b
    adds $acc3,$acc3,$t0
    umulh $acc4,$a3,$b

    // rd[2] = $acc2, rd[3] = $acc3
    stp $acc2,$acc3,[$rd,#16]
    // carry
    adc x0,$acc4,xzr
    ret
.size	ll_u256_mul_limb,.-ll_u256_mul_limb


// void ll_u256_mul(u64 r[8], u64 a[4], u64 b[4]);
.globl	ll_u256_mul
.type	ll_u256_mul,%function
.align	5
ll_u256_mul:
    ldp $a0,$a1,[$ad]
    ldp $b0,$b1,[$bd]
    ldp $a2,$a3,[$ad,#16]
    ldp $b2,$b3,[$bd,#16]
    // a0 * b0
    mul   $acc0,$a0,$b0
    umulh $acc1,$a0,$b0
    // a1 * b0
    mul     $t0,$a1,$b0
    umulh $acc2,$a1,$b0
    adds $acc1,$acc1,$t0
     mul    $t1,$a2,$b0
    adc $acc2,$acc2,xzr
    // a2 * b0
    umulh $acc3,$a2,$b0
    adds $acc2,$acc2,$t1
     mul    $t0,$a3,$b0
    adc $acc3,$acc3,xzr
    // a3 * b0
    umulh $acc4,$a3,$b0
    adds $acc3,$acc3,$t0
     mul  $t1,$a0,$b1
    adc $acc4,$acc4,xzr

    // a0 * b1
    umulh $t2,$a0,$b1     // lazy t2
    adds $acc1,$acc1,$t1
    // a1 * b1
    umulh $t0,$a1,$b1     // lazy t0
     adcs $acc2,$acc2,$t2
    mul   $t1,$a1,$b1
    adc $t0,$t0,xzr
    adds $acc2,$acc2,$t1
    // a2 * b1
    umulh $t2,$a2,$b1     // lazy t2
     adcs $acc3,$acc3,$t0
    mul   $t1,$a2,$b1
    adc $t2,$t2,xzr
    adds $acc3,$acc3,$t1
    // a3 * b1
    umulh $acc5,$a3,$b1
     adcs $acc4,$acc4,$t2
    mul   $t1,$a3,$b1
    adc $acc5,$acc5,xzr
    adds $acc4,$acc4,$t1
    // rd[0] = $acc0, rd[1] = $acc1
    stp $acc0,$acc1,[$rd]
     adc $acc5,$acc5,xzr

    // a0 * b2
    mul   $t1,$a0,$b2
    umulh $t2,$a0,$b2     // lazy t2
    adds $acc2,$acc2,$t1
    // a1 * b2
    umulh $t0,$a1,$b2     // lazy t0
     adcs $acc3,$acc3,$t2
    mul   $t1,$a1,$b2
    adc $t0,$t0,xzr
    adds $acc3,$acc3,$t1
    // a2 * b2
    umulh $t2,$a2,$b2     // lazy t2
     adcs $acc4,$acc4,$t0
    mul   $t1,$a2,$b2
    adc $t2,$t2,xzr
    adds $acc4,$acc4,$t1
    // a3 * b2
    umulh $acc0,$a3,$b2
     adcs $acc5,$acc5,$t2
    mul   $t1,$a3,$b2
    adc $acc0,$acc0,xzr
    adds $acc5,$acc5,$t1

    // a0 * b3
    mul  $t1,$a0,$b3
     adc $acc0,$acc0,xzr
    eor $acc1,$acc1,$acc1
    umulh $t2,$a0,$b3     // lazy t2
    adds $acc3,$acc3,$t1
    // a1 * b3
    umulh $t0,$a1,$b3     // lazy t0
     adcs $acc4,$acc4,$t2
    mul   $t1,$a1,$b3
    adc $t0,$t0,xzr
    adds $acc4,$acc4,$t1
    // a2 * b3
    umulh $t2,$a2,$b3     // lazy t2
     adcs $acc5,$acc5,$t0
    mul   $t1,$a2,$b3
    adc $t2,$t2,xzr
    adds $acc5,$acc5,$t1
    // a3 * b3
    umulh $acc1,$a3,$b3
     adcs $acc0,$acc0,$t2
    mul   $t1,$a3,$b3
    adc $acc1,$acc1,xzr
    adds $acc0,$acc0,$t1

    // rd[2] = $acc2, rd[3] = $acc3
    stp $acc2,$acc3,[$rd,#16]
     adc $acc1,$acc1,xzr
    // rd[4] = $acc4, rd[5] = $acc5
    stp $acc4,$acc5,[$rd,#32]
    // rd[6] = $acc0, rd[7] = $acc1
    stp $acc0,$acc1,[$rd,#48]
    ret
.size	ll_u256_mul,.-ll_u256_mul


// void ll_u256_mullo(u64 r[4], u64 a[4], u64 b[4]);
.globl	ll_u256_mullo
.type	ll_u256_mullo,%function
.align	5
ll_u256_mullo:
    ldp $a0,$a1,[$ad]
    ldp $b0,$b1,[$bd]
    ldp $a2,$a3,[$ad,#16]
    ldp $b2,$b3,[$bd,#16]
    // a0 * b0
    mul   $acc0,$a0,$b0
    umulh $acc1,$a0,$b0
    // a1 * b0
    mul     $t0,$a1,$b0
    umulh $acc2,$a1,$b0
    adds $acc1,$acc1,$t0
     mul    $t1,$a2,$b0
    adc $acc2,$acc2,xzr
    // a2 * b0
    umulh $acc3,$a2,$b0
    adds $acc2,$acc2,$t1
     mul    $t0,$a3,$b0
    adc $acc3,$acc3,xzr
    // a3 * b0
    add $acc3,$acc3,$t0
     mul  $t1,$a0,$b1

    // a0 * b1
    umulh $t2,$a0,$b1
    adds $acc1,$acc1,$t1
    // a1 * b1
    umulh $t0,$a1,$b1
     adcs $acc2,$acc2,$t2
    mul   $t1,$a1,$b1
    adc $t0,$t0,xzr
    adds $acc2,$acc2,$t1
    // a2 * b1
     adc $acc3,$acc3,$t0
    mul   $t1,$a2,$b1
    add $acc3,$acc3,$t1
    // rd[0] = $acc0, rd[1] = $acc1
    stp $acc0,$acc1,[$rd]

    // a0 * b2
    mul   $t1,$a0,$b2
    umulh $t2,$a0,$b2
    adds $acc2,$acc2,$t1
    // a1 * b2
    umulh $t0,$a1,$b2
     adc $acc3,$acc3,$t2
    mul   $t1,$a1,$b2
    adc $t0,$t0,xzr
    add $acc3,$acc3,$t1

    // a0 * b3
    mul  $t1,$a0,$b3
    add $acc3,$acc3,$t1

    // rd[2] = $acc2, rd[3] = $acc3
    stp $acc2,$acc3,[$rd,#16]
    ret
.size	ll_u256_mullo,.-ll_u256_mullo


// void ll_u256_sqr(u64 r[8], u64 a[4]);
.globl	ll_u256_sqr
.type	ll_u256_sqr,%function
.align	5
ll_u256_sqr:
    ldp $a0,$a1,[$ad]
    ldp $a2,$a3,[$ad,#16]
    // a1 * a0
    mul   $acc1,$a1,$a0
    umulh $acc2,$a1,$a0
    // a2 * a0
    mul     $t1,$a2,$a0
    umulh $acc3,$a2,$a0
    adds $acc2,$acc2,$t1
     mul    $t0,$a3,$a0
    adc $acc3,$acc3,xzr
    // a3 * a0
    umulh $acc4,$a3,$a0
    adds $acc3,$acc3,$t0
     mul  $t1,$a2,$a1
    adc $acc4,$acc4,xzr

    // a2 * a1
    umulh $t2,$a2,$a1     // lazy t2
    adds $acc3,$acc3,$t1
    // a3 * a1
    umulh $acc5,$a3,$a1
     adcs $acc4,$acc4,$t2
    mul   $t1,$a3,$a1
    adc $acc5,$acc5,xzr
    adds $acc4,$acc4,$t1
    // a3 * a2
    umulh $acc6,$a3,$a2
     adcs $acc5,$acc5,xzr
    mul   $t1,$a3,$a2
    adc $acc6,$acc6,xzr
    adds $acc5,$acc5,$t1
     eor $acc7,$acc7,$acc7
    adc $acc6,$acc6,xzr

    // (acc7 ~ acc1) << 1 + a0^2 + a1^2 + a2^2 + a3^2
    extr $acc7,$acc7,$acc6,#63
     mul $acc0,$a0,$a0
    extr $acc6,$acc6,$acc5,#63
     umulh $t1,$a0,$a0
    extr $acc5,$acc5,$acc4,#63
     mul $t2,$a1,$a1
    extr $acc4,$acc4,$acc3,#63
     umulh $t3,$a1,$a1
    extr $acc3,$acc3,$acc2,#63
     mul $t4,$a2,$a2
    extr $acc2,$acc2,$acc1,#63
    lsl $acc1,$acc1,#1
     umulh $t0,$a2,$a2
    adds $acc1,$acc1,$t1
    adcs $acc2,$acc2,$t2
    umulh $t1,$a3,$a3
    adcs $acc3,$acc3,$t3
    // rd[0] = $acc0, rd[1] = $acc1
     stp $acc0,$acc1,[$rd]
    adcs $acc4,$acc4,$t4
     mul $t2,$a3,$a3
    adcs $acc5,$acc5,$t0
    // rd[2] = $acc2, rd[3] = $acc3
     stp $acc2,$acc3,[$rd,#16]
    adcs $acc6,$acc6,$t2
    // rd[4] = $acc4, rd[5] = $acc5
     stp $acc4,$acc5,[$rd,#32]
    adc $acc7,$acc7,$t1
    // rd[6] = $acc6, rd[7] = $acc7
    stp $acc6,$acc7,[$rd,#48]
    ret
.size	ll_u256_sqr,.-ll_u256_sqr


// void ll_u256_sqrlo(u64 r[4], u64 a[4]);
.globl	ll_u256_sqrlo
.type	ll_u256_sqrlo,%function
.align	5
ll_u256_sqrlo:
    ldp $a0,$a1,[$ad]
    ldp $a2,$a3,[$ad,#16]
    // a1 * a0
    mul   $acc1,$a1,$a0
    umulh $acc2,$a1,$a0
    // a2 * a0
    mul     $t1,$a2,$a0
    umulh $acc3,$a2,$a0
    adds $acc2,$acc2,$t1
     mul    $t0,$a3,$a0
    adc $acc3,$acc3,xzr
    // a3 * a0
    add $acc3,$acc3,$t0
     mul  $t1,$a2,$a1

    // a2 * a1
    add $acc3,$acc3,$t1

    // (acc3 ~ acc1) << 1 + a0^2 + a1^2
    mul $acc0,$a0,$a0
    umulh $t1,$a0,$a0
    extr $acc3,$acc3,$acc2,#63
    extr $acc2,$acc2,$acc1,#63
    lsl $acc1,$acc1,#1
    mul $t2,$a1,$a1
    umulh $t3,$a1,$a1

    adds $acc1,$acc1,$t1
    adcs $acc2,$acc2,$t2
    adc $acc3,$acc3,$t3

    // rd[0] = $acc0, rd[1] = $acc1
    stp $acc0,$acc1,[$rd]
    // rd[2] = $acc2, rd[3] = $acc3
    stp $acc2,$acc3,[$rd,#16]
    ret
.size	ll_u256_sqrlo,.-ll_u256_sqrlo
___
}

print $code;
close STDOUT;
