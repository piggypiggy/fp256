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
my($rd,$ad,$bd,$b)=("x15","x1","x2","x2");
my($a0,$a1,$a2,$a3,$b0,$b1,$b2,$b3)=map("x$_",(3..10));
my($r0,$r1,$r2,$r3)=map("x$_",(11..14));

$code.=<<___;
// u64 ll_u256_add_limb(u64 rd[4], const u64 ad[4], u64 b);
.globl	ll_u256_add_limb
.type	ll_u256_add_limb,%function
.align	5
ll_u256_add_limb:
    ldp $a0,$a1,[$ad]
    mov $rd,x0
    adds $r0,$a0,$b       // r0 = a0 + b
    ldp $a2,$a3,[$ad,#16]
    adcs $r1,$a1,xzr      // r1 = a1 + carry
    adcs $r2,$a2,xzr      // r2 = a2 + carry
    stp $r0,$r1,[$rd]
    adcs $r3,$a3,xzr      // r3 = a3 + carry
    stp $r2,$r3,[$rd,#16]
    adc x0,xzr,xzr        // carry
    ret
.size	ll_u256_add_limb,.-ll_u256_add_limb


// u64 ll_u256_add(u64 rd[4], const u64 ad[4], const u64 bd[4]);
.globl	ll_u256_add
.type	ll_u256_add,%function
.align	5
ll_u256_add:
    ldp $a0,$a1,[$ad]
    mov $rd,x0
    ldp $b0,$b1,[$bd]
    adds $r0,$a0,$b0      // r0 = a0 + b0
    ldp $a2,$a3,[$ad,#16]
    adcs $r1,$a1,$b1      // r1 = a1 + b1 + carry
    ldp $b2,$b3,[$bd,#16]
    adcs $r2,$a2,$b2      // r2 = a2 + b2 + carry
    stp $r0,$r1,[$rd]
    adcs $r3,$a3,$b3      // r3 = a3 + b3 + carry
    stp $r2,$r3,[$rd,#16]
    adc x0,xzr,xzr        // carry
    ret
.size	ll_u256_add,.-ll_u256_add


// u64 ll_u256_sub_limb(u64 rd[4], const u64 ad[4], u64 b);
.globl	ll_u256_sub_limb
.type	ll_u256_sub_limb,%function
.align	5
ll_u256_sub_limb:
    ldp $a0,$a1,[$ad]
    mov $rd,x0
    subs $r0,$a0,$b       // r0 = a0 - b
    ldp $a2,$a3,[$ad,#16]
    sbcs $r1,$a1,xzr      // r1 = a1 - borrow
    sbcs $r2,$a2,xzr      // r2 = a2 - borrow
    stp $r0,$r1,[$rd]
    sbcs $r3,$a3,xzr      // r3 = a3 - borrow
    stp $r2,$r3,[$rd,#16]
    csinc x0,xzr,xzr,cs   // borrow
    ret
.size	ll_u256_sub_limb,.-ll_u256_sub_limb


// u64 ll_u256_sub(u64 rd[4], const u64 ad[4], const u64 bd[4]);
.globl	ll_u256_sub
.type	ll_u256_sub,%function
.align	5
ll_u256_sub:
    ldp $a0,$a1,[$ad]
    mov $rd,x0
    ldp $b0,$b1,[$bd]
    subs $r0,$a0,$b0      // r0 = a0 - b0
    ldp $a2,$a3,[$ad,#16]
    sbcs $r1,$a1,$b1      // r1 = a1 - b1 - borrow
    ldp $b2,$b3,[$bd,#16]
    sbcs $r2,$a2,$b2      // r2 = a2 - b2 - borrow
    stp $r0,$r1,[$rd]
    sbcs $r3,$a3,$b3      // r3 = a3 - b3 - borrow
    stp $r2,$r3,[$rd,#16]
    csinc x0,xzr,xzr,cs   // borrow
    ret
.size	ll_u256_sub,.-ll_u256_sub
___
}

print $code;
close STDOUT;
