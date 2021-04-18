# ############################################################################
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
# ############################################################################

.text

.align 1
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

# u64 ll_reciprocal1(u64 d)
.globl	ll_reciprocal1
.align	4
ll_reciprocal1:
    srli a2, a0, 55             # d >> 55
    srli a3, a0, 24             # d >> 24
    addi a2, a2, -256           # (d >> 55) - 256
    la a4, .Llookup_table
    slli a2, a2, 1
    addi a3, a3, 1              # (d >> 24) + 1
    add a4, a4, a2
    lhu a5, 0(a4)               # v0
    # v1 = (v0 << 11) - ((v0^2 * d40) >> 40) - 1
    srli a6, a0, 1              # d >> 1
    slli a2, a5, 11             # v0 << 11
    mul a5, a5, a5              # v0 * v0
    addi a2, a2, -1             # (v0 << 11) - 1
    mul a5, a5, a3              # v0 * v0 * ((d >> 24) + 1)
    li a4, 0x1000000000000000   # 2^60
    srli a7, a5, 40             # (v0^2 * d40) >> 40)
    sub a2, a2, a7              # v1
    # v2 = (v1 << 13) - ((v1 * (2^60 - v1 * d40)) >> 47)
    slli a5, a2, 13             # v1 << 13
    mul a7, a2, a3
    andi a3, a0, 1              # d & 1
    sub a4, a4, a7              # v0 -= (v1 * d40)
    add a6, a6, a3              # d63
    mul a2, a2, a4              # v1 *= v0
    # e = 2^96 - v2 * d63 + (v2 >> 1) * d0
    srli a7, a2, 47
    neg a3, a3                  # -d0
    add a5, a5, a7              # v2
    srli a7, a5, 1
    mul a6, a6, a5
    and t0, a3, a7              # (v2 >> 1) & (-d0)
    sub a4, t0, a6              # e
    # v3 = ((v2 << 31) + ((v2 * e) >> 65)) mod 2^64
    slli a2, a5, 31             # v2 << 31
    mulhu a3, a5, a4
    srli a3, a3, 1
    add a2, a2, a3              # v3
    # v4 = (v3 - (((v3 + 2^64 + 1) * d) >> 64)) mod 2^64
    mulhu a5, a2, a0
    mul a4, a2, a0
    add a4, a4, a0
    sub a2, a2, a5
    sltu t0, a4, a0
    sub a2, a2, a0
    add a5, a5, a0
    sub a0, a2, t0
    ret
.size	ll_reciprocal1,.-ll_reciprocal1


# u64 ll_reciprocal2(u64 d1, u64 d0)
.globl	ll_reciprocal2
.align	4
ll_reciprocal2:
    addi sp, sp, -16
    mv t2, a0                   # save d1
    sd ra, 0(sp)
    sd s0, 8(sp)
    addi s0, sp, 16
    call ll_reciprocal1
    mul a2, t2, a0              # d1 * v
    add a2, a2, a1              # p = d1 * v + d0
    bgeu a2, a1, .LRE1
    addi a0, a0, -1             # v--
    bltu a2, t2, .LRE11
    sub a2, a2, t2              # p - d1
    addi a0, a0, -1             # v--
.LRE11:
    sub a2, a2, t2              # p - d1

.LRE1:
    mulhu t1, a0, a1
    mul t0, a0, a1
    add a2, a2, t1
    bgeu a2, t1, .LRE2
    addi a0, a0, -2             # v--
    sltu t0, t0, a1
    sub a3, a2, t2
    sltu t6, a2, t2
    sltu t0, a3, t0
    add a0, a0, t6
    add a0, a0, t0
.LRE2:
    ld ra, 0(sp)
    ld s0, 8(sp)
    addi sp, sp, 16
    ret
.size	ll_reciprocal2,.-ll_reciprocal2


# u64 ll_div2by1_pi1(u64 *r, u64 n[2], u64 d, u64 v)
.globl	ll_div2by1_pi1
.align	4
ll_div2by1_pi1:
    ld a4, 0(a1)
    ld a5, 8(a1)
    mulhu a7, a5, a3            # q1
    mul a6, a5, a3              # q0
    addi a7, a7, 1              # q1++
    add a6, a6, a4
    sltu t0, a6, a4
    add a7, a7, a5
    add a7, a7, t0              # q1,q0 += n1,n0
    mul t1, a7, a2              # q1 * d
    sub t1, a4, t1              # t = n0 - q1 * d
    sltu t0, t1, a6
    addi t0, t0, -1             # t0 = (t < q0) - 1 = -(t >= q0)
    and a3, a2, t0
    add a7, a7, t0              # q1-- if t >= q0
    add t1, t1, a3              # t += d if t >= q0

    sltu t0, t1, a2
    addi t0, t0, -1             # t0 = (t < d) - 1 = -(t >= d)
    and a3, a2, t0
    sub a7, a7, t0              # q1++ if t >= d
    sub t1, t1, a3              # t -= d if t >= d

    mv a0, a7                   # return q1
    sd x0, 8(a1)                # r[1] = 0
    sd t1, 0(a1)                # r[0] = t
    ret
.size	ll_div2by1_pi1,.-ll_div2by1_pi1


# u64 ll_div3by2_pi1(u64 *r, const u64 n[3], const u64 d[2], u64 v)
.globl	ll_div3by2_pi1
.align	4
ll_div3by2_pi1:
    ld a4, 0(a1)                # n0
    ld a5, 8(a1)                # n1
    ld a6, 16(a1)               # n2
    ld t0, 0(a2)                # d0
    ld t1, 8(a2)                # d1
    mulhu t3, a3, a6
    mul t2, a3, a6              # q1,q0 = v * n2
    add t2, t2, a5
    add t3, t3, a6
    sltu t6, t2, a5
    add t3, t3, t6              # q1,q0 += n2,n1
    mulhu a2, t0, t3
    mul a1, t0, t3              # d0 * q1
    mul t6, t1, t3              # d1 * q1
    sltu a7, a4, a1
    sub a3, a5, t6              # r1 = n1 - q1 * d1
    sub a1, a4, a1
    sub a2, a3, a2
    sub a2, a2, a7              # r1,r0 = r1,n0 - t1,t0

    sub a5, a1, t0
    sltu a7, a1, t0
    sub a6, a2, t1
    addi t3, t3, 1              # q1++
    sub a6, a6, a7              # r1,r0 - d1,d0

    bltu a6, t2, .L1
    addi t3, t3, -1             # q1--
    mv a5, a1                   # r0
    mv a6, a2                   # r1

.L1:
    sub a1, a5, t0
    sltu a7, a5, t0
    sub a2, a6, t1
    sltu t4, a6, t1
    sub a3, a2, a7              # r1,r0 - d1,d0
    sltu t5, a2, a7
    add t4, t4, t5              # borrow
    bne t4, x0, .L2
    addi t3, t3, 1              # q1++
    mv a5, a1                   # r0
    mv a6, a3                   # r1

.L2:
    sd a5, 0(a0)
    sd a6, 8(a0)
    sd x0, 16(a0)
    mv a0, t3
    ret
.size	ll_div3by2_pi1,.-ll_div3by2_pi1
