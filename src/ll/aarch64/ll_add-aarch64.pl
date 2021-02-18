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
my ($rd,$ad,$b,$bd,$l)=("x4","x1","x2","x2","x3");
my ($t1,$t2)=("x5","x6");

$code.=<<___;
# u64 ll_add_limb(u64 *rd, u64 *ad, u64 b, size_t al);
.globl	ll_add_limb
.type	ll_add_limb,%function
.align	5
ll_add_limb:
    mov $rd,x0
    adds x0,$b,xzr   // x0=b, C=0
    cbz $l,.ll_add_limb_done

.ll_add_limb_loop:
    ldr $t1,[$ad],#8
    sub $l,$l,#1
    adcs $t1,$t1,x0
    eor x0,xzr,xzr
    str $t1,[$rd],#8
    cbnz $l,.ll_add_limb_loop

.ll_add_limb_done:
    adc x0,x0,xzr
    ret
.size	ll_add_limb,.-ll_add_limb


# u64 ll_add_limbs(u64 *rd, u64 *ad, u64 bd, size_t al);
.globl	ll_add_limbs
.type	ll_add_limbs,%function
.align	5
ll_add_limbs:
    mov $rd,x0
    adds x7,xzr,xzr  // C=0
    cbz $l,.ll_add_limbs_done

.ll_add_limbs_loop:
    ldr $t1,[$ad],#8
    ldr $t2,[$bd],#8
    sub $l,$l,#1
    adcs $t1,$t1,$t2
    str $t1,[$rd],#8
    cbnz $l,.ll_add_limbs_loop

.ll_add_limbs_done:
    adc x0,xzr,xzr
    ret
.size	ll_add_limbs,.-ll_add_limbs


# u64 ll_sub_limb(u64 *rd, u64 *ad, u64 b, size_t al);
.globl	ll_sub_limb
.type	ll_sub_limb,%function
.align	5
ll_sub_limb:
    mov $rd,x0
    subs x0,$b,xzr  // x0=b, C=1
    cbz $l,.ll_sub_limb_done

.ll_sub_limb_loop:
    ldr $t1,[$ad],#8
    sub $l,$l,#1
    sbcs $t1,$t1,x0
    eor x0,xzr,xzr
    str $t1,[$rd],#8
    cbnz $l,.ll_sub_limb_loop

.ll_sub_limb_done:
    csinc x0,x0,x0,cs
    ret
.size	ll_sub_limb,.-ll_sub_limb


# u64 ll_sub_limbs(u64 *rd, u64 *ad, u64 bd, size_t al);
.globl	ll_sub_limbs
.type	ll_sub_limbs,%function
.align	5
ll_sub_limbs:
    mov $rd,x0
    subs x0,xzr,xzr  // C=1
    cbz $l,.ll_sub_limbs_done

.ll_sub_limbs_loop:
    ldr $t1,[$ad],#8
    ldr $t2,[$bd],#8
    sub $l,$l,#1
    sbcs $t1,$t1,$t2
    str $t1,[$rd],#8
    cbnz $l,.ll_sub_limbs_loop

.ll_sub_limbs_done:
    csinc x0,xzr,xzr,cs
    ret
.size	ll_sub_limbs,.-ll_sub_limbs
___
}

{
my ($rd,$ad,$bd,$l)=("x0","x1","x2","x3");
my ($t1,$t2)=("x5","x6");

$code.=<<___;
# void ll_mont_cond_sub_limbs(u64 *rd, u64 *ad, u64 *bd, size_t l);
.globl	ll_mont_cond_sub_limbs
.type	ll_mont_cond_sub_limbs,%function
.align	5
ll_mont_cond_sub_limbs:
    subs x4,$l,xzr
    cbz $l,.ll_mont_cond_sub_limbs_done

.ll_mont_cond_sub_limbs_loop:
    ldr $t1,[$ad],#8
    ldr $t2,[$bd],#8
    sub $l,$l,#1
    sbcs $t1,$t1,$t2        // ad[i] - bd[i]
    str $t1,[$rd],#8
    cbnz $l,.ll_mont_cond_sub_limbs_loop

    ldr $t1,[$ad]
    sbcs $t1,$t1,xzr        // ad[l] - borrow
    sub $ad,$ad,#8
    sub $rd,$rd,#8
    csel $ad,$rd,$ad,cs
.ll_mont_cond_sub_limbs_move_loop:   // conditional move
    ldr $t1,[$ad],#-8 
    sub x4,x4,#1
    str $t1,[$rd],#-8
    cbnz x4,.ll_mont_cond_sub_limbs_move_loop

.ll_mont_cond_sub_limbs_done:
    ret
.size	ll_mont_cond_sub_limbs,.-ll_mont_cond_sub_limbs
___
}

print $code;
close STDOUT;
