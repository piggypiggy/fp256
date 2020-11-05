##############################################################################
#                                                                            #
# Copyright 2020 Meng-Shan Jiang                                             #
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
$code.=<<___;
// size_t ll_num_bits(u64 a);
.globl	ll_num_bits
.type	ll_num_bits,%function
.align 5
ll_num_bits:
    clz x0,x0
    mov x1,#64
    sub x0,x1,x0
    ret
.size	ll_num_bits,.-ll_num_bits


// size_t ll_leading_zeros(u64 a);
.globl	ll_leading_zeros
.type	ll_leading_zeros,%function
.align	5
ll_leading_zeros:
    clz x0,x0
    ret
.size	ll_leading_zeros,.-ll_leading_zeros


// u32 ll_bswap4(u32 a)
.globl	ll_bswap4
.type	ll_bswap4,%function
.align	5
ll_bswap4:
    rev w0,w0
    ret
.size	ll_bswap4,.-ll_bswap4


// u64 ll_bswap8(u64 a)
.globl	ll_bswap8
.type	ll_bswap8,%function
.align	5
ll_bswap8:
    rev x0,x0
    ret
.size	ll_bswap8,.-ll_bswap8
___
}

print $code;

close STDOUT;
