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
.Llookup_table:
.value 0x7fd,0x7f5,0x7ed,0x7e5,0x7dd,0x7d5,0x7ce,0x7c6,0x7bf,0x7b7,0x7b0,0x7a8,0x7a1,0x79a,0x792,0x78b
.value 0x784,0x77d,0x776,0x76f,0x768,0x761,0x75b,0x754,0x74d,0x747,0x740,0x739,0x733,0x72c,0x726,0x720
.value 0x719,0x713,0x70d,0x707,0x700,0x6fa,0x6f4,0x6ee,0x6e8,0x6e2,0x6dc,0x6d6,0x6d1,0x6cb,0x6c5,0x6bf
.value 0x6ba,0x6b4,0x6ae,0x6a9,0x6a3,0x69e,0x698,0x693,0x68d,0x688,0x683,0x67d,0x678,0x673,0x66e,0x669
.value 0x664,0x65e,0x659,0x654,0x64f,0x64a,0x645,0x640,0x63c,0x637,0x632,0x62d,0x628,0x624,0x61f,0x61a
.value 0x616,0x611,0x60c,0x608,0x603,0x5ff,0x5fa,0x5f6,0x5f1,0x5ed,0x5e9,0x5e4,0x5e0,0x5dc,0x5d7,0x5d3
.value 0x5cf,0x5cb,0x5c6,0x5c2,0x5be,0x5ba,0x5b6,0x5b2,0x5ae,0x5aa,0x5a6,0x5a2,0x59e,0x59a,0x596,0x592
.value 0x58e,0x58a,0x586,0x583,0x57f,0x57b,0x577,0x574,0x570,0x56c,0x568,0x565,0x561,0x55e,0x55a,0x556
.value 0x553,0x54f,0x54c,0x548,0x545,0x541,0x53e,0x53a,0x537,0x534,0x530,0x52d,0x52a,0x526,0x523,0x520
.value 0x51c,0x519,0x516,0x513,0x50f,0x50c,0x509,0x506,0x503,0x500,0x4fc,0x4f9,0x4f6,0x4f3,0x4f0,0x4ed
.value 0x4ea,0x4e7,0x4e4,0x4e1,0x4de,0x4db,0x4d8,0x4d5,0x4d2,0x4cf,0x4cc,0x4ca,0x4c7,0x4c4,0x4c1,0x4be
.value 0x4bb,0x4b9,0x4b6,0x4b3,0x4b0,0x4ad,0x4ab,0x4a8,0x4a5,0x4a3,0x4a0,0x49d,0x49b,0x498,0x495,0x493
.value 0x490,0x48d,0x48b,0x488,0x486,0x483,0x481,0x47e,0x47c,0x479,0x477,0x474,0x472,0x46f,0x46d,0x46a
.value 0x468,0x465,0x463,0x461,0x45e,0x45c,0x459,0x457,0x455,0x452,0x450,0x44e,0x44b,0x449,0x447,0x444
.value 0x442,0x440,0x43e,0x43b,0x439,0x437,0x435,0x432,0x430,0x42e,0x42c,0x42a,0x428,0x425,0x423,0x421
.value 0x41f,0x41d,0x41b,0x419,0x417,0x414,0x412,0x410,0x40e,0x40c,0x40a,0x408,0x406,0x404,0x402,0x400
___

{
my ($d,$d1,$d0)=("%rdi","%rdi","%rsi");
my ($t0,$t0d,$t1,$t2,$t3,$t4)=("%rax","%eax","%rcx","%r8","%r9","%rdx");
$code.=<<___;

.type __ll_reciprocal1,\@abi-omnipotent
.align	32
__ll_reciprocal1:
    # v1 = (v0 << 11) - ((v0^2 * d40) >> 40) - 1
    mov %rdi, $t3
    mov %rdi, $t1
    shr \$55, $t3
    lea .Llookup_table(%rip), $t2
    sub \$256, $t3
    shr \$24, $t1
    movzw ($t2,$t3,2), %eax    # t0
    inc $t1
    mov $t0, $t3
    imull $t0d, $t0d
    shl \$11, $t3
    imulq $t1, $t0
    dec $t3
    shr \$40, $t0
    sub $t0, $t3
    # v2 = (v1 << 13) - ((v1 * (2^60 - v1 * d40)) >> 47)
    mov \$0x1000000000000000, $t2
    mov $t3, $t0
    imulq $t3, $t1
    shl \$13, $t0
    sub $t1, $t2
     mov %rdi, $t4
     mov %rdi, $t1
    imulq $t3, $t2
     shr \$1, $t1
     and \$1, $t4
    shr \$47, $t2
     add $t4, $t1
    add $t2, $t0
    # e = 2^96 - v2 * d63 + (v2 >> 1) * d0
    neg $t4
    imulq $t0, $t1
    mov $t0, $t2
    shr \$1, $t0
    and $t4, $t0
    sub $t1, $t0
    # v3 = ((v2 << 31) + ((v2 * e) >> 65)) mod 2^64
    mulq $t2
    shl \$31, $t2
    shr \$1, %rdx
    mov %rdi, %rax
    add %rdx, $t2
    # v4 = (v3 - (((v3 + 2^64 + 1) * d) >> 64)) mod 2^64
    mulq $t2
    add %rdi, %rax
    mov $t2, %rax
    adc %rdi, %rdx
    sub %rdx, %rax

    ret
.size	__ll_reciprocal1,.-__ll_reciprocal1


# u64 ll_reciprocal1(u64 d)
.globl	ll_reciprocal1
.type	ll_reciprocal1,\@function,1
.align	32
ll_reciprocal1:
    call __ll_reciprocal1
    ret
.size	ll_reciprocal1,.-ll_reciprocal1


# u64 ll_reciprocal2(u64 d1, u64 d0)
.globl	ll_reciprocal2
.type	ll_reciprocal2,\@function,2
.align	32
ll_reciprocal2:
    call __ll_reciprocal1
    mov %rax, $t1
    imulq $d1, $t1
    add $d0, $t1
    jnc .L1
    mov $t1, $t2
    sub $d1, $t1
    dec %rax
    cmovc $t2, $t1
    adc \$0, %rax
    sub $d1, $t1
    dec %rax
.L1:
    mov %rax, $t3
    mulq $d0
    mov %rax, $t2
    add %rdx, $t1
    mov $t3, %rax
    jnc .L2
    sub $d0, $t2
    dec %rax
    sbb $d1, $t1
    dec %rax
    adc \$0, %rax
.L2:
    ret
.size	ll_reciprocal2,.-ll_reciprocal2
___
}

{
my ($r_ptr,$n_ptr,$d_ptr,$d,$v)=("%rdi","%rsi","%rdx","%rdx","%rcx");
my ($t0,$t1,$t2,$t3)=("%r8","%r9","%r10","%r11");
$code.=<<___;

# u64 ll_div2by1_pi1(u64 *r, u64 n[2], u64 d, u64 v)
.globl	ll_div2by1_pi1
.type	ll_div2by1_pi1,\@function,4
.align	32
ll_div2by1_pi1:
    mov 0($n_ptr), $t0
    mov 8($n_ptr), $t1
    mov $v, %rax
    mov $d, $t2
    mulq $t1
    inc $t1
    add $t0, %rax
    adc $t1, %rdx
    xor $t3, $t3
    mov %rdx, %rcx
    imulq $t2, %rdx
    sub %rdx, $t0
    cmp %rax, $t0
    cmovae $t2, $t3
    adc \$0, %rcx
    add $t3, $t0
     movq \$0, 8($r_ptr)
    mov $t0, $t1
    sub $t2, $t0
    cmovnc $t0, $t1
    sbb \$0, %rcx

    mov $t1, 0($r_ptr)
    mov %rcx, %rax
    ret
.size	ll_div2by1_pi1,.-ll_div2by1_pi1


# u64 ll_div3by2_pi1(u64 *r, u64 n[3], u64 d[2], u64 v)
.globl	ll_div3by2_pi1
.type	ll_div3by2_pi1,\@function,4
.align	32
ll_div3by2_pi1:
    mov 8($n_ptr), $t1
    mov 16($n_ptr), $t2
    mov $v, %rax
    mov 0($d_ptr), $t3     # d0
    mov $d_ptr, $t0        # d_ptr
    mulq $t2
    add $t1, %rax
    adc $t2, %rdx
    mov 8($t0), $t2        # d1
    mov 0($n_ptr), $t0     # n0
    mov %rdx, %rcx         # q1
    imulq $t2, %rdx
    mov %rax, %rsi         # q0
    mov $t3, %rax
    sub %rdx, $t1
    mulq %rcx
    sub %rax, $t0
    sbb %rdx, $t1
    mov $t0, %rax
    mov $t1, %rdx
    sub $t3, $t0
    sbb $t2, $t1
     movq \$0, 16($r_ptr)
    cmp %rsi, $t1
    cmovbq $t0, %rax
    cmovbq $t1, %rdx
    adc \$1, %rcx
    mov %rax, $t0
    mov %rdx, $t1
    sub $t3, %rax
    sbb $t2, %rdx
    cmovnc %rax, $t0
    cmovnc %rdx, $t1
    sbb \$0, %rcx

    mov $t0, 0($r_ptr)
    mov $t1, 8($r_ptr)
    mov %rcx, %rax
    ret
.size	ll_div3by2_pi1,.-ll_div3by2_pi1
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
