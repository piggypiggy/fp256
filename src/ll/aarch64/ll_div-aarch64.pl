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
.align	1
.Llookup_table:
.2byte 0x7fd,0x7f5,0x7ed,0x7e5,0x7dd,0x7d5,0x7ce,0x7c6,0x7bf,0x7b7,0x7b0,0x7a8,0x7a1,0x79a,0x792,0x78b
.2byte 0x784,0x77d,0x776,0x76f,0x768,0x761,0x75b,0x754,0x74d,0x747,0x740,0x739,0x733,0x72c,0x726,0x720
.2byte 0x719,0x713,0x70d,0x707,0x700,0x6fa,0x6f4,0x6ee,0x6e8,0x6e2,0x6dc,0x6d6,0x6d1,0x6cb,0x6c5,0x6bf
.2byte 0x6ba,0x6b4,0x6ae,0x6a9,0x6a3,0x69e,0x698,0x693,0x68d,0x688,0x683,0x67d,0x678,0x673,0x66e,0x669
.2byte 0x664,0x65e,0x659,0x654,0x64f,0x64a,0x645,0x640,0x63c,0x637,0x632,0x62d,0x628,0x624,0x61f,0x61a
.2byte 0x616,0x611,0x60c,0x608,0x603,0x5ff,0x5fa,0x5f6,0x5f1,0x5ed,0x5e9,0x5e4,0x5e0,0x5dc,0x5d7,0x5d3
.2byte 0x5cf,0x5cb,0x5c6,0x5c2,0x5be,0x5ba,0x5b6,0x5b2,0x5ae,0x5aa,0x5a6,0x5a2,0x59e,0x59a,0x596,0x592
.2byte 0x58e,0x58a,0x586,0x583,0x57f,0x57b,0x577,0x574,0x570,0x56c,0x568,0x565,0x561,0x55e,0x55a,0x556
.2byte 0x553,0x54f,0x54c,0x548,0x545,0x541,0x53e,0x53a,0x537,0x534,0x530,0x52d,0x52a,0x526,0x523,0x520
.2byte 0x51c,0x519,0x516,0x513,0x50f,0x50c,0x509,0x506,0x503,0x500,0x4fc,0x4f9,0x4f6,0x4f3,0x4f0,0x4ed
.2byte 0x4ea,0x4e7,0x4e4,0x4e1,0x4de,0x4db,0x4d8,0x4d5,0x4d2,0x4cf,0x4cc,0x4ca,0x4c7,0x4c4,0x4c1,0x4be
.2byte 0x4bb,0x4b9,0x4b6,0x4b3,0x4b0,0x4ad,0x4ab,0x4a8,0x4a5,0x4a3,0x4a0,0x49d,0x49b,0x498,0x495,0x493
.2byte 0x490,0x48d,0x48b,0x488,0x486,0x483,0x481,0x47e,0x47c,0x479,0x477,0x474,0x472,0x46f,0x46d,0x46a
.2byte 0x468,0x465,0x463,0x461,0x45e,0x45c,0x459,0x457,0x455,0x452,0x450,0x44e,0x44b,0x449,0x447,0x444
.2byte 0x442,0x440,0x43e,0x43b,0x439,0x437,0x435,0x432,0x430,0x42e,0x42c,0x42a,0x428,0x425,0x423,0x421
.2byte 0x41f,0x41d,0x41b,0x419,0x417,0x414,0x412,0x410,0x40e,0x40c,0x40a,0x408,0x406,0x404,0x402,0x400
___


$code.=<<___;
# u64 ll_reciprocal1(u64 d)
.globl	ll_reciprocal1
.type	ll_reciprocal1,%function
.align	5
ll_reciprocal1:
    lsr x2,x0,#55                // d >> 55
    lsr x3,x0,#24                // d >> 24
    add x2,x2,#-256              // (d >> 55) - 256
    adr x4,.Llookup_table
    lsl x2,x2,#1
    add x3,x3,#1                 // (d >> 24) + 1
    ldrh w5,[x4,x2]              // v0
    // v1 = (v0 << 11) - ((v0^2 * d40) >> 40) - 1
    lsr x6,x0,#1                 // d >> 1
    lsl x2,x5,#11                // v0 << 11
    umull x5,w5,w5               // v0 * v0
    sub x2,x2,#1                 // (v0 << 11) - 1
    mul x5,x5,x3                 // v0 * v0 * ((d >> 24) + 1)
    mov x4,#0x1000000000000000   // 2^60
    sub x2,x2,x5,lsr#40          // v1
    // v2 = (v1 << 13) - ((v1 * (2^60 - v1 * d40)) >> 47)
    lsl x5,x2,#13                // v1 << 13
    msub x4,x2,x3,x4             // v0 -= (v1 * d40)
    and x3,x0,#1                 // d & 1
    mul x2,x2,x4                 // v1 *= v0
    // e = 2^96 - v2 * d63 + (v2 >> 1) * d0
    add x6,x6,x3                 // d63
    add x5,x5,x2,lsr#47          // v2
    neg x3,x3                    // -d0
    and x4,x3,x5,lsr#1           // (v2 >> 1) & (-d0)
    // v3 = ((v2 << 31) + ((v2 * e) >> 65)) mod 2^64
    msub x4,x5,x6,x4             // e
    lsl x2,x5,#31                // v2 << 31
    umulh x3,x5,x4
    add x2,x2,x3,lsr#1           // v3
    // v4 = (v3 - (((v3 + 2^64 + 1) * d) >> 64)) mod 2^64
    mul x4,x2,x0
    umulh x5,x2,x0
    adds x4,x4,x0
    adc x5,x5,x0
    sub x0,x2,x5
    ret
.size	ll_reciprocal1,.-ll_reciprocal1


# u64 ll_reciprocal2(u64 d1, u64 d0)
.globl	ll_reciprocal2
.type	ll_reciprocal2,%function
.align	5
ll_reciprocal2:
    stp x29,x30,[sp,#-32]!
    add x29,sp,#0
    mov x7,x0                    // save d1
    bl ll_reciprocal1
    madd x2,x0,x7,x1             // d0 + d1 * v
    cmp x2,x1
    bhs .L1
    subs x3,x2,x7                // p - d1
    sub x0,x0,#2
    csel x2,x3,x2,cs
    cinc x0,x0,cc
    sub x2,x2,x7
.L1:
    umulh x5,x0,x1
    mul x4,x0,x1
    adds x2,x2,x5
    bcc .L2
    sub x0,x0,#2
    subs x4,x4,x1
    sbcs x2,x2,x7
    cinc x0,x0,cc
.L2:
    ldp x29,x30,[sp],#32
    ret
.size	ll_reciprocal2,.-ll_reciprocal2


# u64 ll_div2by1_pi1(u64 *r, u64 n[2], u64 d, u64 v)
.globl	ll_div2by1_pi1
.type	ll_div2by1_pi1,%function
.align	5
ll_div2by1_pi1:
    ldp x4,x5,[x1]               // n0,n1
    mul x6,x5,x3                 // q0
    umulh x7,x5,x3               // q1
    add x5,x5,#1
    adds x6,x6,x4
    adc x7,x7,x5
    msub x8,x7,x2,x4             // t = n0 - q1 * d
    sub x3,x7,#1                 // q1--
    add x1,x8,x2                 // t += d
    cmp x8,x6
    csel x7,x3,x7,cs
    csel x8,x1,x8,cs
    add x3,x7,#1                 // q1++
    subs x1,x8,x2                // t - d
    csel x8,x1,x8,cs
    stp x8,xzr,[x0]
    csel x0,x3,x7,cs
    ret
.size	ll_div2by1_pi1,.-ll_div2by1_pi1


# u64 ll_div3by2_pi1(u64 *r, u64 n[3], u64 d[2], u64 v)
.globl	ll_div3by2_pi1
.type	ll_div3by2_pi1,%function
.align	5
ll_div3by2_pi1:
    ldp x5,x6,[x1,#8]             // n1,n2
    ldr x4,[x1]                   // n0
    ldp x7,x8,[x2]                // d0,d1
    mul x9,x3,x6                  // q0
    umulh x10,x3,x6               // q1, (q1,q0) = v * n2
    adds x9,x9,x5
    adc x10,x10,x6                // q1,q0 += n2,n1
    mul x1,x7,x10
    umulh x2,x7,x10               // d0 * q1
    msub x3,x10,x8,x5             // r1 = n1 - q1 * d1
    subs x1,x4,x1
    sbc x2,x3,x2                  // r1,r0 = r1,n0 - t1,t0
    subs x5,x1,x7
    sbc x6,x2,x8                  // r1,r0 - d1,d0
    cmp x6,x9
    cinc x10,x10,cc
    csel x1,x1,x5,cs
    csel x2,x2,x6,cs
    subs x5,x1,x7
    sbcs x6,x2,x8                 // r1,r0 - d1,d0
    cinc x10,x10,cs
    csel x1,x5,x1,cs
    csel x2,x6,x2,cs
    str xzr,[x0,#16]
    stp x1,x2,[x0]
    mov x0,x10
    ret
.size	ll_div3by2_pi1,.-ll_div3by2_pi1
___

print $code;
close STDOUT;
