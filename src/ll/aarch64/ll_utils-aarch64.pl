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

$code.=<<___;
// size_t ll_num_bits(u64 a);
.globl	ll_num_bits
.type	ll_num_bits,%function
.align 4
ll_num_bits:
    clz x0,x0
    mov x1,#64
    sub x0,x1,x0
    ret
.size	ll_num_bits,.-ll_num_bits


// size_t ll_leading_zeros(u64 a);
.globl	ll_leading_zeros
.type	ll_leading_zeros,%function
.align	4
ll_leading_zeros:
    clz x0,x0
    ret
.size	ll_leading_zeros,.-ll_leading_zeros


// u32 ll_bswap4(u32 a)
.globl	ll_bswap4
.type	ll_bswap4,%function
.align	4
ll_bswap4:
    rev w0,w0
    ret
.size	ll_bswap4,.-ll_bswap4


// u64 ll_bswap8(u64 a)
.globl	ll_bswap8
.type	ll_bswap8,%function
.align	4
ll_bswap8:
    rev x0,x0
    ret
.size	ll_bswap8,.-ll_bswap8
___


# void ll_u256_select(u64 r[4], u64 *table, size_t table_size, size_t index)
$code.=<<___;
.globl	ll_u256_select
.type	ll_u256_select,%function
.align	5
ll_u256_select:
    mov x12,xzr
    mov x4,xzr
    mov x5,xzr
    mov x6,xzr
    mov x7,xzr

.Lu256_select_loop:
    ldp x8,x9,[x1],#16
    cmp x12,x3
    ldp x10,x11,[x1],#16
    csel x8,x8,xzr,eq
    csel x9,x9,xzr,eq
    add x12,x12,#1
    csel x10,x10,xzr,eq
    csel x11,x11,xzr,eq
    orr x4,x4,x8
    orr x5,x5,x9
    subs x2,x2,#1
    orr x6,x6,x10
    orr x7,x7,x11
    bne .Lu256_select_loop

    stp x4,x5,[x0]
    stp x6,x7,[x0,#16]
.size	ll_u256_select,.-ll_u256_select
___

print $code;
close STDOUT;
