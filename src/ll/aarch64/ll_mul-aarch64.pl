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
my ($rd,$ad,$b,$al)=("x4","x1","x2","x3");
my ($t1,$t2,$t,$lo,$hi)=("x0","x5","x6","x7","x9");

$code.=<<___;
# u64 ll_mul_limb(u64 *rd, u64 *ad, u64 b, size_t al);
.globl	ll_mul_limb
.type	ll_mul_limb,%function
.align	5
ll_mul_limb:
    mov $rd,x0
    eor $t1,xzr,xzr  // C=0

.ll_mul_limb_loop:
    cbz $al,.ll_mul_limb_done
    ldr $t,[$ad],#8
    mul $lo,$t,$b
    sub $al,$al,#1
    adds $lo,$lo,$t1
    umulh $t1,$t,$b
    str $lo,[$rd],#8
    adc $t1,$t1,xzr
    b .ll_mul_limb_loop

.ll_mul_limb_done:
    str $t1,[$rd]
    ret
.size	ll_mul_limb,.-ll_mul_limb
___
}

{
my ($rd,$ad,$b,$rl,$al)=("x5","x1","x2","x3","x4");
my ($t1,$t2,$t,$i,$lo,$hi)=("x0","x6","x7","x9","x10","x11");
$code.=<<___;
# u64 ll_muladd_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al);
.globl	ll_muladd_limb
.type	ll_muladd_limb,%function
.align	5
ll_muladd_limb:
    eor $i,xzr,xzr
    mov $rd,x0
    eor $t1,xzr,xzr

.ll_muladd_limb_mul_loop:           // rd[0, al-1] += ad * b
    cmp $i,$al
    beq .ll_muladd_limb_add_loop
    ldr $t,[$ad],#8
    ldr $t2,[$rd]
    umulh $hi,$t,$b
    adds $t1,$t1,$t2
    mul $lo,$t,$b
    adc $hi,$hi,xzr
    adds $lo,$lo,$t1
    add $i,$i,#1
    str $lo,[$rd],#8
    adc $t1,$hi,xzr
    b .ll_muladd_limb_mul_loop

.ll_muladd_limb_add_loop:           // rl > al, rd[al, rl-1] += t1
    cmp $i,$rl
    bhs .ll_muladd_limb_done
    ldr $t2,[$rd]
    add $i,$i,#1
    adds $t2,$t2,$t1
    adc $t1,xzr,xzr
    str $t2,[$rd],#8
    b .ll_muladd_limb_add_loop

.ll_muladd_limb_done:
    str $t1,[$rd]
    ret
.size	ll_muladd_limb,.-ll_muladd_limb


# u64 ll_mulsub_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al);
.globl	ll_mulsub_limb
.type	ll_mulsub_limb,%function
.align	5
ll_mulsub_limb:
    eor $i,xzr,xzr
    mov $rd,x0
    eor $t1,xzr,xzr

.ll_mulsub_limb_mul_loop:           // rd[0, al-1] -= ad * b
    cmp $i,$al
    beq .ll_mulsub_limb_sub_loop
    ldr $t,[$ad],#8
    ldr $t2,[$rd]
    mul $lo,$t,$b
    umulh $hi,$t,$b
    adds $lo,$lo,$t1
    adc $hi,$hi,xzr
    subs $t2,$t2,$lo
    add $i,$i,#1
    str $t2,[$rd],#8
    csinc $t1,$hi,$hi,cs
    b .ll_mulsub_limb_mul_loop

.ll_mulsub_limb_sub_loop:           // rl > al, rd[al, rl-1] -= t1
    cmp $i,$rl
    bhs .ll_mulsub_limb_done
    ldr $t2,[$rd]
    add $i,$i,#1
    subs $t2,$t2,$t1
    csinc $t1,xzr,xzr,cs
    str $t2,[$rd],#8
    b .ll_mulsub_limb_sub_loop

.ll_mulsub_limb_done:
    ret
.size	ll_mulsub_limb,.-ll_mulsub_limb
___
}

print $code;
close STDOUT;
