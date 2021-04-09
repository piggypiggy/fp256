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

.LOne:
.long 1,1,1,1,1,1,1,1
___

# TODO : use inline assembly ?
$code.=<<___;
# size_t ll_num_bits(u64 a);
.globl	ll_num_bits
.export	ll_num_bits
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
.export	ll_leading_zeros
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
.export	ll_bswap4
.type	ll_bswap4,\@function,1
.align	32
ll_bswap4:
    bswapl %edi
    mov %edi, %eax
    ret
.size	ll_bswap4,.-ll_bswap4


# u64 ll_bswap8(u64 a)
.globl	ll_bswap8
.export	ll_bswap8
.type	ll_bswap8,\@function,1
.align	32
ll_bswap8:
    bswapq %rdi
    mov %rdi, %rax
    ret
.size	ll_bswap8,.-ll_bswap8
___


# TODO : don't export it because it should require 16 byte alignment.
# void ll_u256_select(u64 r[4], u64 *table, size_t table_size, size_t index)
$code.=<<___;
.globl	ll_u256_select
.export	ll_u256_select
.type	ll_u256_select,\@function,4
.align	32
ll_u256_select:
___
$code.=<<___	if ($win64);
    sub \$32, %rsp
    movdqu %xmm6, 16*0(%rsp)
    movdqu %xmm7, 16*1(%rsp)
___

$code.=<<___;
    movdqa .LOne(%rip), %xmm5  # 1
    movq %rcx, %xmm0           # index
    pxor %xmm1, %xmm1
    pxor %xmm2, %xmm2
    pxor %xmm7, %xmm7
    pshufd \$0, %xmm0, %xmm0

.Lu256_select_loop:
    movdqa %xmm7,%xmm6
    paddd %xmm5,%xmm7
    pcmpeqd %xmm0,%xmm6

    movdqa 16*0(%rsi), %xmm3
    movdqa 16*1(%rsi), %xmm4
    lea 16*2(%rsi), %rsi
    pand %xmm6, %xmm3
    pand %xmm6, %xmm4
    por %xmm3, %xmm1
    por %xmm4, %xmm2
    dec %rdx                   # table_size
    jnz .Lu256_select_loop

    movdqu %xmm1, 16*0(%rdi)
    movdqu %xmm2, 16*1(%rdi)
___
$code.=<<___	if ($win64);
    movdqu 16*0(%rsp), %xmm6
    movdqu 16*1(%rsp), %xmm7
    add \$32, %rsp
___

$code.=<<___;
    ret
.size	ll_u256_select,.-ll_u256_select
___

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
