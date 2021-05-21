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
___

{
my ($r_ptr,$a_ptr,$b_ptr,$b,$l,$offset)=("%rdi","%rsi","%rdx","%rdx","%rcx","%rax");
my ($a0,$a1,$a2,$a3)=map("%r$_",(8..11));

$code.=<<___;
# u64 ll_u256_add_limb(u64 r[5], u64 a[4], u64 b);
.globl	ll_u256_add_limb
.export	ll_u256_add_limb
.type	ll_u256_add_limb,\@function,3
.align	32
ll_u256_add_limb:
    mov	8*0($a_ptr), $a0
    mov	8*1($a_ptr), $a1
    xor %rax, %rax
    add    $b, $a0               # a0 + b
    mov	8*2($a_ptr), $a2
    adc    \$0, $a1              # a1 + carry
    mov $a0, 8*0($r_ptr)
    mov	8*3($a_ptr), $a3
    adc	   \$0, $a2              # a2 + carry
    mov $a1, 8*1($r_ptr)
    adc	   \$0, $a3              # a3 + carry
    mov $a2, 8*2($r_ptr)
    mov $a3, 8*3($r_ptr)
    adc \$0, %rax
    ret
.size	ll_u256_add_limb,.-ll_u256_add_limb


# u64 ll_u256_add(u64 r[5], u64 a[4], u64 b[4]);
.globl	ll_u256_add
.export	ll_u256_add
.type	ll_u256_add,\@function,3
.align	32
ll_u256_add:
    mov	8*0($a_ptr), $a0
    mov	8*1($a_ptr), $a1
    xor %rax, %rax
    add    8*0($b_ptr), $a0      # a0 + b0
    mov	8*2($a_ptr), $a2
    adc    8*1($b_ptr), $a1      # a1 + b1 + carry
    mov $a0, 8*0($r_ptr)
    mov	8*3($a_ptr), $a3
    adc	   8*2($b_ptr), $a2      # a2 + b3 + carry
    mov $a1, 8*1($r_ptr)
    adc	   8*3($b_ptr), $a3      # a3 + b3 + carry
    mov $a2, 8*2($r_ptr)
    mov $a3, 8*3($r_ptr)
    adc \$0, %rax
    ret
.size	ll_u256_add,.-ll_u256_add


# u64 ll_u256_sub_limb(u64 r[4], u64 a[4], u64 b);
.globl	ll_u256_sub_limb
.export	ll_u256_sub_limb
.type	ll_u256_sub_limb,\@function,3
.align	32
ll_u256_sub_limb:
    mov	8*0($a_ptr), $a0
    mov	8*1($a_ptr), $a1
    xor %rax, %rax
    sub    $b, $a0               # a0 - b
    mov	8*2($a_ptr), $a2
    sbb    \$0, $a1              # a1 - borrow
    mov $a0, 8*0($r_ptr)
    mov	8*3($a_ptr), $a3
    sbb	   \$0, $a2              # a2 - borrow
    mov $a1, 8*1($r_ptr)
    sbb	   \$0, $a3              # a3 - borrow
    mov $a2, 8*2($r_ptr)
    mov $a3, 8*3($r_ptr)
    adc \$0, %rax 
    ret
.size	ll_u256_sub_limb,.-ll_u256_sub_limb


# u64 ll_u256_sub(u64 r[4], u64 a[4], u64 b[4]);
.globl	ll_u256_sub
.export	ll_u256_sub
.type	ll_u256_sub,\@function,3
.align	32
ll_u256_sub:
    mov	8*0($a_ptr), $a0
    mov	8*1($a_ptr), $a1
    xor %rax, %rax
    sub    8*0($b_ptr), $a0      # a0 - b0
    mov	8*2($a_ptr), $a2
    sbb    8*1($b_ptr), $a1      # a1 - b1 - borrow
    mov $a0, 8*0($r_ptr)
    mov	8*3($a_ptr), $a3
    sbb	   8*2($b_ptr), $a2      # a2 - b2 - borrow
    mov $a1, 8*1($r_ptr)
    sbb	   8*3($b_ptr), $a3      # a3 - b3 - borrow
    mov $a2, 8*2($r_ptr)
    mov $a3, 8*3($r_ptr)
    adc \$0, %rax 
    ret
.size	ll_u256_sub,.-ll_u256_sub
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
