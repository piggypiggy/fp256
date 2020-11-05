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

# if ($flavour =~ /\./) { $output = $flavour; undef $flavour; }

$win64=0; $win64=1 if ($flavour =~ /[nm]asm|mingw64/ || $output =~ /\.asm$/);

$0 =~ m/(.*[\/\\])[^\/\\]+$/; $dir=$1;
( $xlate="${dir}x86_64-xlate.pl" and -f $xlate ) or
( $xlate="${dir}../perlasm/x86_64-xlate.pl" and -f $xlate) or
die "can't locate x86_64-xlate.pl";

open OUT,"| \"$^X\" \"$xlate\" $flavour \"$output\"";
*STDOUT=*OUT;

$code.=<<___;
.text
___

# TODO : use inline assembly ?
$code.=<<___;
# size_t ll_num_bits(u64 a);
.globl	ll_num_bits
.type	ll_num_bits,\@function,1
.align	32
ll_num_bits:
    # if a = 0, bsr's dest operand is undefined. set %rax = 0 first.
    xor %rax, %rax
    test %rdi, %rdi
    jz .a_is_zero_1    # emmm
    bsrq %rdi, %rax
    inc %rax
.a_is_zero_1:
    ret
.size	ll_num_bits,.-ll_num_bits


# size_t ll_leading_zeros(u64 a);
.globl	ll_leading_zeros
.type	ll_leading_zeros,\@function,1
.align	32
ll_leading_zeros:
    # if a = 0, bsr's dest operand is undefined. set %rcx = 0 first.
    mov \$64, %rax
    xor %rcx, %rcx
    test %rdi, %rdi
    jz .a_is_zero_2
    bsrq %rdi, %rcx
    inc %rcx
.a_is_zero_2:
    sub %rcx, %rax
    ret
.size	ll_leading_zeros,.-ll_leading_zeros


# u32 ll_bswap4(u32 a)
.globl	ll_bswap4
.type	ll_bswap4,\@function,1
.align	32
ll_bswap4:
    bswapl %edi
    mov %edi, %eax
    ret
.size	ll_bswap4,.-ll_bswap4


# u64 ll_bswap8(u64 a)
.globl	ll_bswap8
.type	ll_bswap8,\@function,1
.align	32
ll_bswap8:
    bswapq %rdi
    mov %rdi, %rax
    ret
.size	ll_bswap8,.-ll_bswap8
___

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
