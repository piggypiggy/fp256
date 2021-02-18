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
my($rd,$ad,$al,$n)=("x0","x1","x2","x3");
my($l,$b,$c)=("x4","x5","x6");
my($t0,$t1,$t2,$t3)=map("x$_",(12..15));

$code.=<<___;
// u64 ll_lshift(u64 *rd, u64 *ad, size_t al, size_t n);
.globl	ll_lshift
.type	ll_lshift,%function
.align 5
ll_lshift:
    cbz $al,.ll_lshift_done_ret      // al ?= 0
    lsr $l,$n,#6                     // l = n / 64
    and $b,$n,#63                    // b = n % 64
    add $t0,$al,$l                   // al + l
    mov $t1,xzr                      // t1 = 0
    neg $c,$b
    add $ad,$ad,$al,lsl #3           // ad += al
    add $rd,$rd,$t0,lsl #3           // rd += (al + l)
    add $c,$c,#64                    // c = 64 - b
    mov $t3,$rd
    cbnz $b,.ll_lshift_loop          // b ?!= 0
    str xzr,[$rd],#-8
    b .ll_lshift_copy

.ll_lshift_loop:
    sub $ad,$ad,#8
    ldr $t0,[$ad]
    lsl $t1,$t1,$b                   // t1 <<= b
    lsr $t2,$t0,$c                   // t2 = t1 >> (64 - b)
    orr $t1,$t1,$t2
    subs $al,$al,#1
    str $t1,[$rd],#-8
    mov $t1,$t0
    bhi .ll_lshift_loop

    lsl $t1,$t1,$b                   // shift the last limb
    str $t1,[$rd],#-8
    b .ll_lshift_clear_lower_limbs

.ll_lshift_copy:
    sub $ad,$ad,#8
    ldr $t0,[$ad]
    subs $al,$al,#1
    str $t0,[$rd],#-8
    bhi .ll_lshift_copy

.ll_lshift_clear_lower_limbs:
    cbz $l,.ll_lshift_done
    sub $l,$l,#1
    str xzr,[$rd],#-8
    b .ll_lshift_clear_lower_limbs

.ll_lshift_done:
    ldr x0,[$t3]
    ret
.ll_lshift_done_ret:
    mov x0,xzr
    ret
.size	ll_lshift,.-ll_lshift


// u64 ll_rshift(u64 *rd, u64 *ad, size_t al, size_t n);
.globl	ll_rshift
.type	ll_rshift,%function
.align 5
ll_rshift:
    cbz $al,.ll_rshift_done          // al ?= 0
    lsr $l,$n,#6                     // l = n / 64
    and $b,$n,#63                    // b = n % 64
    subs $t0,$al,$l                  // al - l
    bls .ll_rshift_r_is_zero
    neg $c,$b
    add $ad,$ad,$l,lsl #3            // ad += l
    mov $al,$t0
    add $c,$c,#64                    // c = 64 - b
    ldr $t0,[$ad],#8
    cbnz $b,.ll_rshift_loop          // b ?!= 0
    sub $ad,$ad,#8
    b .ll_rshift_copy

.ll_rshift_r_is_zero:
    subs $al,$al,#1
    str xzr,[$rd],#8
    bhi .ll_rshift_r_is_zero

.ll_rshift_loop:
    subs $al,$al,#1
    bls .ll_rshift_last_limb
    ldr $t1,[$ad]
    lsr $t0,$t0,$b                   // t0 >>= b
    lsl $t2,$t1,$c                   // t2 = t1 << (64 - b)
    orr $t0,$t0,$t2
    add $ad,$ad,#8
    str $t0,[$rd],#8
    mov $t0,$t1
    b .ll_rshift_loop

.ll_rshift_last_limb:
    lsr $t0,$t0,$b                   // shift the last limb
    str $t0,[$rd],#8
    b .ll_rshift_clear_upper_limbs

.ll_rshift_copy:
    cbz $al,.ll_rshift_done
    ldr $t0,[$ad],#8
    sub $al,$al,#1
    str $t0,[$rd],#8
    b .ll_rshift_copy

.ll_rshift_clear_upper_limbs:
    cbz $l,.ll_rshift_done
    str xzr,[$rd],#8
    sub $l,$l,#1
    b .ll_rshift_clear_upper_limbs

.ll_rshift_done:
    mov x0,$t0
    ret
.size	ll_rshift,.-ll_rshift
___
}

print $code;
close STDOUT;
