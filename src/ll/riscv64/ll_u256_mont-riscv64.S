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

#define acc0 t0
#define acc1 t1
#define acc2 t2
#define acc3 t3
#define acc4 t4
#define acc5 t5
#define acc6 s0
#define acc7 s1
#define N0   s2
#define N1   s3
#define N2   s4
#define N3   s5
#define bd   s6
#define b    t6
#define k0   a4
#define rd   a5
#define cy0  a6
#define cy1  a7

# void ll_u256_mont_mul(u64 rd[4], const u64 ad[4], const u64 bd[4], const u64 Nd[4], u64 k0)
.globl  ll_u256_mont_mul
.align  4
ll_u256_mont_mul:
    addi sp, sp, -64
    sd s0, 0(sp)
    sd s1, 8(sp)
    sd s2, 16(sp)
    sd s3, 24(sp)
    sd s4, 32(sp)
    sd s5, 40(sp)
    sd s6, 48(sp)
    # save rd, bd
    mv rd, a0
    mv bd, a2
    # load N0~N3
    ld N0, 0(a3)
    ld N1, 8(a3)
    ld N2, 16(a3)
    ld N3, 24(a3)
    # load a0~a3
    ld a3, 24(a1)
    ld a2, 16(a1)
    ld a0, 0(a1)
    ld a1, 8(a1)

    # load b0
    ld b, 0(bd)
    # a0 * b0
    mulhu acc1, a0, b
    mul acc0, a0, b
    # a1 * b0
    mulhu acc2, a1, b
    mul s0, a1, b
    add acc1, acc1, s0
    sltu cy0, acc1, s0
    # a2 * b0
    mulhu acc3, a2, b
    mul s1, a2, b
    add acc2, acc2, cy0
    add acc2, acc2, s1
    sltu cy0, acc2, s1
    # a3 * b0
    mulhu acc4, a3, b
    mul s0, a3, b
    add acc3, acc3, cy0
    add acc3, acc3, s0
    sltu cy0, acc3, s0
     sd acc0, 0(rd)       # r0
    add acc4, acc4, cy0

    # reduction
    mul t6, acc0, k0
    # acc5,acc4,acc3,acc2,acc1,acc0 += t6 * N
    # N0 * t6
    mulhu s1, N0, t6
    mul s0, N0, t6
    add acc0, acc0, s0
    sltu cy1, acc0, s0
    add s1, s1, cy1
    # N1 * t6
    mulhu t0, N1, t6       # reuse acc0 = t0
    mul s0, N1, t6
    add acc1, acc1, s1
    sltu cy0, acc1, s1
    add acc1, acc1, s0
    sltu cy1, acc1, s0
    add t0, t0, cy0
     add acc2, acc2, cy1
     sltu cy1, acc2, cy1
    # N2 * t6
    mulhu s1, N2, t6
    mul s0, N2, t6
    add acc2, acc2, t0
    sltu cy0, acc2, t0
    add acc2, acc2, s0
    add cy0, cy0, cy1
    sltu cy1, acc2, s0
    add s1, s1, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # N3 * t6
    mulhu t0, N3, t6
    mul s0, N3, t6
    add acc3, acc3, s1
    sltu cy0, acc3, s1
    add acc3, acc3, s0
    add cy0, cy0, cy1
    sltu cy1, acc3, s0
    add t0, t0, cy0
    add acc4, acc4, cy1
    sltu cy1, acc4, cy1
    add acc4, acc4, t0
    sltu cy0, acc4, t0
    add acc5, cy1, cy0

    # acc0,acc5,acc4,acc3,acc2,acc1 += a * b[1]
    # load b1
    ld b, 8(bd)
    # a0 * b1
    mulhu s1, a0, b
    mul s0, a0, b
    add acc1, acc1, s0
    sltu cy1, acc1, s0
    add s1, s1, cy1
    # a1 * b1
    mulhu t0, a1, b
    mul s0, a1, b
    add acc2, acc2, s1
    sltu cy0, acc2, s1
    add acc2, acc2, s0
    sltu cy1, acc2, s0
    add t0, t0, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # a2 * b1
    mulhu s1, a2, b
    mul s0, a2, b
    add acc3, acc3, t0
    sltu cy0, acc3, t0
    add acc3, acc3, s0
    add cy0, cy0, cy1
    sltu cy1, acc3, s0
    add s1, s1, cy0
     add acc4, acc4, cy1
     sltu cy1, acc4, cy1
    # a3 * b1
    mulhu t0, a3, b
    mul s0, a3, b
    add acc4, acc4, s1
    sltu cy0, acc4, s1
    add acc4, acc4, s0
    add cy0, cy0, cy1
    sltu cy1, acc4, s0
    add t0, t0, cy0
    add acc5, acc5, cy1
    sltu cy1, acc5, cy1
    add acc5, acc5, t0
    sltu cy0, acc5, t0
    add acc0, cy1, cy0

    # reduction
    mul t6, acc1, k0
    # acc0,acc5,acc4,acc3,acc2,acc1 += t6 * N
    # N0 * t6
    mulhu s1, N0, t6
    mul s0, N0, t6
    add acc1, acc1, s0
    sltu cy1, acc1, s0
    add s1, s1, cy1
    # N1 * t6
    mulhu t1, N1, t6       # reuse acc1 = t1
    mul s0, N1, t6
    add acc2, acc2, s1
    sltu cy0, acc2, s1
    add acc2, acc2, s0
    sltu cy1, acc2, s0
    add t1, t1, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # N2 * t6
    mulhu s1, N2, t6
    mul s0, N2, t6
    add acc3, acc3, t1
    sltu cy0, acc3, t1
    add acc3, acc3, s0
    add cy0, cy0, cy1
    sltu cy1, acc3, s0
    add s1, s1, cy0
     add acc4, acc4, cy1
     sltu cy1, acc4, cy1
    # N3 * t6
    mulhu t1, N3, t6
    mul s0, N3, t6
    add acc4, acc4, s1
    sltu cy0, acc4, s1
    add acc4, acc4, s0
    add cy0, cy0, cy1
    sltu cy1, acc4, s0
    add t1, t1, cy0
    add acc5, acc5, cy1
    sltu cy1, acc5, cy1
    add acc5, acc5, t1
    sltu cy0, acc5, t1
    add acc0, acc0, cy1
    add acc0, acc0, cy0

    # acc1,acc0,acc5,acc4,acc3,acc2 += a * b[2]
    # load b2
    ld b, 16(bd)
    # a0 * b2
    mulhu s1, a0, b
    mul s0, a0, b
    add acc2, acc2, s0
    sltu cy1, acc2, s0
    add s1, s1, cy1
    # a1 * b2
    mulhu t1, a1, b
    mul s0, a1, b
    add acc3, acc3, s1
    sltu cy0, acc3, s1
    add acc3, acc3, s0
    sltu cy1, acc3, s0
    add t1, t1, cy0
     add acc4, acc4, cy1
     sltu cy1, acc4, cy1
    # a2 * b2
    mulhu s1, a2, b
    mul s0, a2, b
    add acc4, acc4, t1
    sltu cy0, acc4, t1
    add acc4, acc4, s0
    add cy0, cy0, cy1
    sltu cy1, acc4, s0
    add s1, s1, cy0
     add acc5, acc5, cy1
     sltu cy1, acc5, cy1
    # a3 * b2
    mulhu t1, a3, b
    mul s0, a3, b
    add acc5, acc5, s1
    sltu cy0, acc5, s1
    add acc5, acc5, s0
    add cy0, cy0, cy1
    sltu cy1, acc5, s0
    add t1, t1, cy0
    add acc0, acc0, cy1
    sltu cy1, acc0, cy1
    add acc0, acc0, t1
    sltu cy0, acc0, t1
    add acc1, cy1, cy0

    # reduction
    mul t6, acc2, k0
    # acc1,acc0,acc5,acc4,acc3,acc2 += t6 * N
    # N0 * t6
    mulhu s1, N0, t6
    mul s0, N0, t6
    add acc2, acc2, s0
    sltu cy1, acc2, s0
    add s1, s1, cy1
    # N1 * t6
    mulhu t2, N1, t6       # reuse acc2 = t2
    mul s0, N1, t6
    add acc3, acc3, s1
    sltu cy0, acc3, s1
    add acc3, acc3, s0
    sltu cy1, acc3, s0
    add t2, t2, cy0
     add acc4, acc4, cy1
     sltu cy1, acc4, cy1
    # N2 * t6
    mulhu s1, N2, t6
    mul s0, N2, t6
    add acc4, acc4, t2
    sltu cy0, acc4, t2
    add acc4, acc4, s0
    add cy0, cy0, cy1
    sltu cy1, acc4, s0
    add s1, s1, cy0
     add acc5, acc5, cy1
     sltu cy1, acc5, cy1
    # N3 * t6
    mulhu t2, N3, t6
    mul s0, N3, t6
    add acc5, acc5, s1
    sltu cy0, acc5, s1
    add acc5, acc5, s0
    add cy0, cy0, cy1
    sltu cy1, acc5, s0
    add t2, t2, cy0
    add acc0, acc0, cy1
    sltu cy1, acc0, cy1
    add acc0, acc0, t2
    sltu cy0, acc0, t2
    add acc1, acc1, cy1
    add acc1, acc1, cy0

    # acc2,acc1,acc0,acc5,acc4,acc3 += a * b[3]
    # load b3
    ld b, 24(bd)
    # a0 * b3
    mulhu s1, a0, b
    mul s0, a0, b
    add acc3, acc3, s0
    sltu cy1, acc3, s0
    add s1, s1, cy1
    # a1 * b3
    mulhu t2, a1, b
    mul s0, a1, b
    add acc4, acc4, s1
    sltu cy0, acc4, s1
    add acc4, acc4, s0
    sltu cy1, acc4, s0
    add t2, t2, cy0
     add acc5, acc5, cy1
     sltu cy1, acc5, cy1
    # a2 * b3
    mulhu s1, a2, b
    mul s0, a2, b
    add acc5, acc5, t2
    sltu cy0, acc5, t2
    add acc5, acc5, s0
    add cy0, cy0, cy1
    sltu cy1, acc5, s0
    add s1, s1, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # a3 * b3
    mulhu t2, a3, b
    mul s0, a3, b
    add acc0, acc0, s1
    sltu cy0, acc0, s1
    add acc0, acc0, s0
    add cy0, cy0, cy1
    sltu cy1, acc0, s0
    add t2, t2, cy0
    add acc1, acc1, cy1
    sltu cy1, acc1, cy1
    add acc1, acc1, t2
    sltu cy0, acc1, t2
    add acc2, cy1, cy0

    # reduction
    mul t6, acc3, k0
    # acc2,acc1,acc0,acc5,acc4,acc3 += t6 * N
    # N0 * t6
    mulhu s1, N0, t6
    mul s0, N0, t6
    add acc3, acc3, s0
    sltu cy1, acc3, s0
    add s1, s1, cy1
    # N1 * t6
    mulhu t3, N1, t6       # reuse acc3 = t3
    mul s0, N1, t6
    add acc4, acc4, s1
    sltu cy0, acc4, s1
    add acc4, acc4, s0
    sltu cy1, acc4, s0
    add t3, t3, cy0
     add acc5, acc5, cy1
     sltu cy1, acc5, cy1
    # N2 * t6
    mulhu s1, N2, t6
    mul s0, N2, t6
    add acc5, acc5, t3
    sltu cy0, acc5, t3
    add acc5, acc5, s0
    add cy0, cy0, cy1
    sltu cy1, acc5, s0
    add s1, s1, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # N3 * t6
    mulhu t3, N3, t6
    mul s0, N3, t6
    add acc0, acc0, s1
    sltu cy0, acc0, s1
    add acc0, acc0, s0
    add cy0, cy0, cy1
    sltu cy1, acc0, s0
    add t3, t3, cy0
    add acc1, acc1, cy1
    sltu cy1, acc1, cy1
    add acc1, acc1, t3
    sltu cy0, acc1, t3
    add acc2, acc2, cy1
    add acc2, acc2, cy0

    # acc2,acc1,acc0,acc5,acc4 - N3,N2,N1,N0
    sub a0, acc4, N0       # acc4 - N0
    sltu cy0, acc4, N0     # borrow1
     ld s0, 0(sp)
    sub t6, acc5, N1       # acc5 - N1
    sltu cy1, acc5, N1     # borrow2
     ld s1, 8(sp)
    sub a1, t6, cy0        # acc5 - N1 - borrow1
    sltu cy0, t6, cy0      # borrow2
     ld s2, 16(sp)
    add cy0, cy0, cy1      # borrow2
    sub t6, acc0, N2       # acc0 - N2
    sltu cy1, acc0, N2     # borrow3
     ld s3, 24(sp)
    sub a2, t6, cy0        # acc0 - N2 - borrow2
    sltu cy0, t6, cy0      # borrow3
     ld s4, 32(sp)
    add cy0, cy0, cy1      # borrow3
    sub t6, acc1, N3       # acc1 - N3
    sltu cy1, acc1, N3     # borrow4
     ld s5, 40(sp)
    sub a3, t6, cy0        # acc1 - N3 - borrow3
    sltu cy0, t6, cy0      # borrow4
     ld s6, 48(sp)
    add cy0, cy0, cy1      # borrow4
    bgeu acc2, cy0, .Lmulge

    sd acc4, 0(rd)
    sd acc5, 8(rd)
    sd acc0, 16(rd)
    sd acc1, 24(rd)
    addi sp, sp, 64
    ret
.Lmulge:
    sd a0, 0(rd)
    sd a1, 8(rd)
    sd a2, 16(rd)
    sd a3, 24(rd)
    addi sp, sp, 64
    ret
.size ll_u256_mont_mul, .-ll_u256_mont_mul


# void ll_u256_mont_sqr(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
.globl  ll_u256_mont_sqr
.align  4
ll_u256_mont_sqr:
    addi sp, sp, -64
    sd s0, 0(sp)
    sd s1, 8(sp)
    sd s2, 16(sp)
    sd s3, 24(sp)
    sd s4, 32(sp)
    sd s5, 40(sp)
    sd s6, 48(sp)
    sd s7, 56(sp)
    # save rd, K0, Nd
    mv rd, a0
    mv k0, a3
    mv N3, a2
    # load a0~a3
    ld a3, 24(a1)
    ld a2, 16(a1)
    ld a0, 0(a1)
    ld a1, 8(a1)

    # a1 * a0
    mulhu acc2, a1, a0
    mul acc1, a1, a0
    # a2 * a0
    mulhu acc3, a2, a0
    mul s1, a2, a0
    add acc2, acc2, s1
    sltu cy0, acc2, s1
    # a3 * a0
    mulhu acc4, a3, a0
    mul s0, a3, a0
    add acc3, acc3, cy0
    add acc3, acc3, s0
    sltu cy0, acc3, s0
    add acc4, acc4, cy0

    # a2 * a1
    mulhu s1, a2, a1
    mul s0, a2, a1
    add acc3, acc3, s0
    sltu cy0, acc3, s0
    add s1, s1, cy0
    # a3 * a1
    mulhu acc5, a3, a1
    mul s0, a3, a1
    add acc4, acc4, s1
    sltu cy0, acc4, s1
    add acc4, acc4, s0
    sltu cy1, acc4, s0
    add acc5, acc5, cy0

    # a3 * a2
    mulhu acc6, a3, a2
    mul s1, a3, a2
    add acc5, acc5, cy1
    sltu cy0, acc5, cy1
    add acc5, acc5, s1
    sltu cy1, acc5, s1
    add acc6, acc6, cy0
    add acc6, acc6, cy1

    # acc6:acc1 << 1
    srli acc7, acc6, 63
    slli acc6, acc6, 1
    srli t6, acc5, 63
    slli acc5, acc5, 1
    or acc6, acc6, t6
    srli t6, acc4, 63
    slli acc4, acc4, 1
    or acc5, acc5, t6
    srli t6, acc3, 63
    slli acc3, acc3, 1
    or acc4, acc4, t6
    srli t6, acc2, 63
    slli acc2, acc2, 1
    or acc3, acc3, t6
    srli t6, acc1, 63
     mulhu s2, a0, a0
     mul acc0, a0, a0
    slli acc1, acc1, 1
    or acc2, acc2, t6

    # a0^2
    add acc1, acc1, s2
    sltu cy0, acc1, s2
    # a1^2
    mulhu s3, a1, a1
    mul t6, a1, a1
    add acc2, acc2, cy0
    sltu cy1, acc2, cy0
    add acc2, acc2, t6
    sltu cy0, acc2, t6
    add s3, s3, cy1
    add s3, s3, cy0
    add acc3, acc3, s3
    sltu cy0, acc3, s3
    # a2^2
    mulhu s2, a2, a2
    mul t6, a2, a2
    add acc4, acc4, cy0
    sltu cy1, acc4, cy0
    add acc4, acc4, t6
    sltu cy0, acc4, t6
    add s2, s2, cy1
    add s2, s2, cy0
    add acc5, acc5, s2
    sltu cy0, acc5, s2
    # a3^2
    mulhu s3, a3, a3
    mul t6, a3, a3
    add acc6, acc6, cy0
    sltu cy1, acc6, cy0
    add acc6, acc6, t6
    sltu cy0, acc6, t6
    add s3, s3, cy1
    add s3, s3, cy0
    add acc7, acc7, s3

    # load N0~N3
    ld N0, 0(N3)
    ld N1, 8(N3)
    ld N2, 16(N3)
    ld N3, 24(N3)
    # reduction
    mul t6, acc0, k0
    # acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc0, acc0, s6
    sltu cy1, acc0, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t0, N1, t6       # reuse acc0 = t0
    mul s6, N1, t6
    add acc1, acc1, s7
    sltu cy0, acc1, s7
    add acc1, acc1, s6
    sltu cy1, acc1, s6
    add t0, t0, cy0
     add acc2, acc2, cy1
     sltu cy1, acc2, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc2, acc2, t0
    sltu cy0, acc2, t0
    add acc2, acc2, s6
    add cy0, cy0, cy1
    sltu cy1, acc2, s6
    add s7, s7, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # N3 * t6
    mulhu t0, N3, t6
    mul s6, N3, t6
    add acc3, acc3, s7
    sltu cy0, acc3, s7
    add acc3, acc3, s6
    add cy0, cy0, cy1
    sltu cy1, acc3, s6
    add acc0, t0, cy0
    add acc0, acc0, cy1

    mul t6, acc1, k0
    # acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc1, acc1, s6
    sltu cy1, acc1, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t1, N1, t6       # reuse acc1 = t1
    mul s6, N1, t6
    add acc2, acc2, s7
    sltu cy0, acc2, s7
    add acc2, acc2, s6
    sltu cy1, acc2, s6
    add t1, t1, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc3, acc3, t1
    sltu cy0, acc3, t1
    add acc3, acc3, s6
    add cy0, cy0, cy1
    sltu cy1, acc3, s6
    add s7, s7, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # N3 * t6
    mulhu t1, N3, t6
    mul s6, N3, t6
    add acc0, acc0, s7
    sltu cy0, acc0, s7
    add acc0, acc0, s6
    add cy0, cy0, cy1
    sltu cy1, acc0, s6
    add acc1, t1, cy0
    add acc1, acc1, cy1

    mul t6, acc2, k0
    # acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc2, acc2, s6
    sltu cy1, acc2, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t2, N1, t6       # reuse acc2 = t2
    mul s6, N1, t6
    add acc3, acc3, s7
    sltu cy0, acc3, s7
    add acc3, acc3, s6
    sltu cy1, acc3, s6
    add t2, t2, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc0, acc0, t2
    sltu cy0, acc0, t2
    add acc0, acc0, s6
    add cy0, cy0, cy1
    sltu cy1, acc0, s6
    add s7, s7, cy0
     add acc1, acc1, cy1
     sltu cy1, acc1, cy1
    # N3 * t6
    mulhu t2, N3, t6
    mul s6, N3, t6
    add acc1, acc1, s7
    sltu cy0, acc1, s7
    add acc1, acc1, s6
    add cy0, cy0, cy1
    sltu cy1, acc1, s6
    add acc2, t2, cy0
    add acc2, acc2, cy1

    mul t6, acc3, k0
    # acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc3, acc3, s6
    sltu cy1, acc3, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t3, N1, t6       # reuse acc3 = t3
    mul s6, N1, t6
    add acc0, acc0, s7
    sltu cy0, acc0, s7
    add acc0, acc0, s6
    sltu cy1, acc0, s6
    add t3, t3, cy0
     add acc1, acc1, cy1
     sltu cy1, acc1, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc1, acc1, t3
    sltu cy0, acc1, t3
    add acc1, acc1, s6
    add cy0, cy0, cy1
    sltu cy1, acc1, s6
    add s7, s7, cy0
     add acc2, acc2, cy1
     sltu cy1, acc2, cy1
    # N3 * t6
    mulhu t3, N3, t6
    mul s6, N3, t6
    add acc2, acc2, s7
    sltu cy0, acc2, s7
    add acc2, acc2, s6
    add cy0, cy0, cy1
    sltu cy1, acc2, s6
    add acc3, t3, cy0
    add acc3, acc3, cy1

    # acc7,acc6,acc5,acc4 + acc3,acc2,acc1,acc0
    add acc4, acc4, acc0
    sltu cy0, acc4, acc0
    add acc5, acc5, acc1
    sltu cy1, acc5, acc1
    add acc5, acc5, cy0
    sltu cy0, acc5, cy0
    add cy0, cy0, cy1
    add acc6, acc6, acc2
    sltu cy1, acc6, acc2
    add acc6, acc6, cy0
    sltu cy0, acc6, cy0
    add cy0, cy0, cy1
    add acc7, acc7, acc3
    sltu a4, acc7, acc3
    add acc7, acc7, cy0
    sltu cy0, acc7, cy0
    add a4, a4, cy0

    # a4,acc7,acc6,acc5,acc4 - N3,N2,N1,N0
    sub a0, acc4, N0       # acc4 - N0
    sltu cy0, acc4, N0     # borrow1
#     # ld s0, 0(sp)
    sub t6, acc5, N1       # acc5 - N1
    sltu cy1, acc5, N1     # borrow2
#     # ld s1, 8(sp)
    sub a1, t6, cy0        # acc5 - N1 - borrow1
    sltu cy0, t6, cy0      # borrow2
     ld s2, 16(sp)
    add cy0, cy0, cy1      # borrow2
    sub t6, acc6, N2       # acc6 - N2
    sltu cy1, acc6, N2     # borrow3
     ld s3, 24(sp)
    sub a2, t6, cy0        # acc6 - N2 - borrow2
    sltu cy0, t6, cy0      # borrow3
     ld s4, 32(sp)
    add cy0, cy0, cy1      # borrow3
    sub t6, acc7, N3       # acc7 - N3
    sltu cy1, acc7, N3     # borrow4
    sub a3, t6, cy0        # acc7 - N3 - borrow3
    sltu cy0, t6, cy0      # borrow4
     ld s5, 40(sp)
    add cy0, cy0, cy1      # borrow4
     ld s6, 48(sp)
     ld s7, 56(sp)
    bgeu a4, cy0, .Lsqrge

    sd acc4, 0(rd)
    sd acc5, 8(rd)
    sd acc6, 16(rd)
    sd acc7, 24(rd)
    ld s0, 0(sp)
    ld s1, 8(sp)
    addi sp, sp, 64
    ret
.Lsqrge:
    sd a0, 0(rd)
    sd a1, 8(rd)
    sd a2, 16(rd)
    sd a3, 24(rd)
    ld s0, 0(sp)
    ld s1, 8(sp)
    addi sp, sp, 64
    ret
.size ll_u256_mont_sqr, .-ll_u256_mont_sqr


# void ll_u256_mont_reduce(u64 rd[4], const u64 ad[4], const u64 Nd[4], u64 k0)
.globl  ll_u256_mont_reduce
.align  4
ll_u256_mont_reduce:
    addi sp, sp, -48
    sd s2, 0(sp)
    sd s3, 8(sp)
    sd s4, 16(sp)
    sd s5, 24(sp)
    sd s6, 32(sp)
    sd s7, 40(sp)
    # save rd, K0, Nd
    mv rd, a0
    mv k0, a3
    mv N3, a2

    # load a0~a3
    ld acc3, 24(a1)
    ld acc2, 16(a1)
    ld acc1, 8(a1)
    ld acc0, 0(a1)

    # load N0~N3
    ld N0, 0(N3)
    ld N1, 8(N3)
    ld N2, 16(N3)
    ld N3, 24(N3)

    # reduction
    mul t6, acc0, k0
    # acc0,acc3,acc2,acc1 = (acc3,acc2,acc1,acc0 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc0, acc0, s6
    sltu cy1, acc0, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t0, N1, t6       # reuse acc0 = t0
    mul s6, N1, t6
    add acc1, acc1, s7
    sltu cy0, acc1, s7
    add acc1, acc1, s6
    sltu cy1, acc1, s6
    add t0, t0, cy0
     add acc2, acc2, cy1
     sltu cy1, acc2, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc2, acc2, t0
    sltu cy0, acc2, t0
    add acc2, acc2, s6
    add cy0, cy0, cy1
    sltu cy1, acc2, s6
    add s7, s7, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # N3 * t6
    mulhu t0, N3, t6
    mul s6, N3, t6
    add acc3, acc3, s7
    sltu cy0, acc3, s7
    add acc3, acc3, s6
    add cy0, cy0, cy1
    sltu cy1, acc3, s6
    add acc0, t0, cy0
    add acc0, acc0, cy1

    mul t6, acc1, k0
    # acc1,acc0,acc3,acc2 = (acc0,acc3,acc2,acc1 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc1, acc1, s6
    sltu cy1, acc1, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t1, N1, t6       # reuse acc1 = t1
    mul s6, N1, t6
    add acc2, acc2, s7
    sltu cy0, acc2, s7
    add acc2, acc2, s6
    sltu cy1, acc2, s6
    add t1, t1, cy0
     add acc3, acc3, cy1
     sltu cy1, acc3, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc3, acc3, t1
    sltu cy0, acc3, t1
    add acc3, acc3, s6
    add cy0, cy0, cy1
    sltu cy1, acc3, s6
    add s7, s7, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # N3 * t6
    mulhu t1, N3, t6
    mul s6, N3, t6
    add acc0, acc0, s7
    sltu cy0, acc0, s7
    add acc0, acc0, s6
    add cy0, cy0, cy1
    sltu cy1, acc0, s6
    add acc1, t1, cy0
    add acc1, acc1, cy1

    mul t6, acc2, k0
    # acc2,acc1,acc0,acc3 = (acc1,acc0,acc3,acc2 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc2, acc2, s6
    sltu cy1, acc2, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t2, N1, t6       # reuse acc2 = t2
    mul s6, N1, t6
    add acc3, acc3, s7
    sltu cy0, acc3, s7
    add acc3, acc3, s6
    sltu cy1, acc3, s6
    add t2, t2, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc0, acc0, t2
    sltu cy0, acc0, t2
    add acc0, acc0, s6
    add cy0, cy0, cy1
    sltu cy1, acc0, s6
    add s7, s7, cy0
     add acc1, acc1, cy1
     sltu cy1, acc1, cy1
    # N3 * t6
    mulhu t2, N3, t6
    mul s6, N3, t6
    add acc1, acc1, s7
    sltu cy0, acc1, s7
    add acc1, acc1, s6
    add cy0, cy0, cy1
    sltu cy1, acc1, s6
    add acc2, t2, cy0
    add acc2, acc2, cy1

    mul t6, acc3, k0
    # acc3,acc2,acc1,acc0 = (acc2,acc1,acc0,acc3 + t6 * N) / 2^64
    # N0 * t6
    mulhu s7, N0, t6
    mul s6, N0, t6
    add acc3, acc3, s6
    sltu cy1, acc3, s6
    add s7, s7, cy1
    # N1 * t6
    mulhu t3, N1, t6       # reuse acc3 = t3
    mul s6, N1, t6
    add acc0, acc0, s7
    sltu cy0, acc0, s7
    add acc0, acc0, s6
    sltu cy1, acc0, s6
    add t3, t3, cy0
     add acc1, acc1, cy1
     sltu cy1, acc1, cy1
    # N2 * t6
    mulhu s7, N2, t6
    mul s6, N2, t6
    add acc1, acc1, t3
    sltu cy0, acc1, t3
    add acc1, acc1, s6
    add cy0, cy0, cy1
    sltu cy1, acc1, s6
    add s7, s7, cy0
     add acc2, acc2, cy1
     sltu cy1, acc2, cy1
    # N3 * t6
    mulhu t3, N3, t6
    mul s6, N3, t6
    add acc2, acc2, s7
    sltu cy0, acc2, s7
    add acc2, acc2, s6
    add cy0, cy0, cy1
    sltu cy1, acc2, s6
    add acc3, t3, cy0
    add acc3, acc3, cy1

    mv a4, x0
    # a4,acc3,acc2,acc1,acc0 - N3,N2,N1,N0
    sub a0, acc0, N0       # acc0 - N0
    sltu cy0, acc0, a0     # borrow1
    sub t6, acc1, N1       # acc1 - N1
    sltu cy1, acc1, t6     # borrow2
    sub a1, t6, cy0        # acc1 - N1 - borrow1
    sltu cy0, t6, a1       # borrow2
     ld s2, 0(sp)
    add cy0, cy0, cy1      # borrow2
    sub t6, acc2, N2       # acc2 - N2
    sltu cy1, acc2, t6     # borrow3
     ld s3, 8(sp)
    sub a2, t6, cy0        # acc2 - N2 - borrow2
    sltu cy0, t6, a2       # borrow3
     ld s4, 16(sp)
    add cy0, cy0, cy1      # borrow3
    sub t6, acc3, N3       # acc3 - N3
    sltu cy1, acc3, t6     # borrow4
    sub a3, t6, cy0        # acc3 - N3 - borrow3
    sltu cy0, t6, a3       # borrow4
     ld s5, 24(sp)
    add cy0, cy0, cy1      # borrow4
     ld s6, 32(sp)
     ld s7, 40(sp)
    bgeu a4, cy0, .Lreducege

    sd acc0, 0(rd)
    sd acc1, 8(rd)
    sd acc2, 16(rd)
    sd acc3, 24(rd)
    addi sp, sp, 48
    ret
.Lreducege:
    sd a0, 0(rd)
    sd a1, 8(rd)
    sd a2, 16(rd)
    sd a3, 24(rd)
    addi sp, sp, 48
    ret
.size ll_u256_mont_reduce, .-ll_u256_mont_reduce
