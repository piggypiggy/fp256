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
my($rd,$ad,$n,$m)=("x0","x1","x2","x3");
my($a0,$a1,$a2,$a3,$a4,$a5,$a6,$a7)=map("x$_",(4..11));
my($t0,$t1,$t2,$t3)=map("x$_",(12..15));

$code.=<<___;
// void ll_u256_lshift(u64 r[5], u64 a[4], size_t n);
.globl	ll_u256_lshift
.type	ll_u256_lshift,%function
.align 5
ll_u256_lshift:
    mov $m,#64
    eor $a4,$a4,$a4
    sub $m,$m,$n              // m = 64 - n
    ldp $a2,$a3,[$ad,#16]
    ldp $a0,$a1,[$ad]

    cmp $n,xzr
    beq .ll_u256_rshift_done

    lsr $a4,$a3,$m            // r[4] = a[3] >> (64-n)
    lsl $a3,$a3,$n            // a[3] <<= n
    lsr $t0,$a2,$m            // a[2] >> (64-n)
    lsl $a2,$a2,$n            // a[2] <<= n
    orr $a3,$a3,$t0           // r[3] = (a[2] >> (64-n)) | (a[3] << n)
    lsr $t0,$a1,$m            // a[1] >> (64-n)
    lsl $a1,$a1,$n            // a[1] <<= n
    orr $a2,$a2,$t0           // r[2] = (a[1] >> (64-n)) | (a[2] << n)
    lsr $t0,$a0,$m            // a[0] >> (64-n)
    lsl $a0,$a0,$n            // r[0] = a[0] << n
    orr $a1,$a1,$t0           // r[1] = (a[0] >> (64-n)) | (a[1] << n)

.ll_u256_rshift_done:
    str $a4,[$rd,#32]
    stp $a2,$a3,[$rd,#16]
    stp $a0,$a1,[$rd]
    ret
.size	ll_u256_lshift,.-ll_u256_lshift


// void ll_u256_rshift(u64 r[4], u64 a[4], size_t n);
.globl	ll_u256_rshift
.type	ll_u256_rshift,%function
.align 5
ll_u256_rshift:
    mov $m,#64
    sub $m,$m,$n              // m = 64 - n
    ldp $a2,$a3,[$ad,#16]
    ldp $a0,$a1,[$ad]

    cmp $n,xzr
    beq .ll_u256_rshift_done

    lsr $a0,$a0,$n            // a[0] >> n
    lsl $t0,$a1,$m            // a[1] << (64-n)
    lsr $a1,$a1,$n            // a[1] >> n
    orr $a0,$a0,$t0           // r[0] = (a[1] << (64-n)) | (a[0] >> n)
    lsl $t0,$a2,$m            // a[2] << (64-n)
    lsr $a2,$a2,$n            // a[2] >> n
    orr $a1,$a1,$t0           // r[1] = (a[2] << (64-n)) | (a[1] >> n)
    lsl $t0,$a3,$m            // a[3] << (64-n)
    lsr $a3,$a3,$n            // r[3] = a[3] >> n
    orr $a2,$a2,$t0           // r[2] = (a[3] << (64-n)) | (a[2] >> n)

.ll_u256_lshift_done:
    stp $a0,$a1,[$rd]
    stp $a2,$a3,[$rd,#16]
    ret
.size	ll_u256_rshift,.-ll_u256_rshift
___
}

print $code;
close STDOUT;
