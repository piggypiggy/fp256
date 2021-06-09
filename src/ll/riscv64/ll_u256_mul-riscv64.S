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

#define b    t6
#define bd   a4
#define rd   a5
#define acc0 t0
#define acc1 t1
#define acc2 t2
#define acc3 t3
#define acc4 t4
#define acc5 t5
#define acc6 s0
#define acc7 s1
#define cy0  a6
#define cy1  a7

# u64 ll_u256_mul_limb(u64 rd[4], const u64 ad[4], u64 b)
.globl	ll_u256_mul_limb
.align	4
ll_u256_mul_limb:
    # save b, rd
    mv b, a2
    mv rd, a0
    # load a0~a3
    ld a3, 24(a1)
    ld a2, 16(a1)
    ld a0, 0(a1)
    ld a1, 8(a1)

    # a0 * b
    mulhu acc1, a0, b
    mul acc0, a0, b
    # a1 * b
    mulhu acc2, a1, b
    mul t5, a1, b
     sd acc0, 0(rd)        # rd[0]
    add acc1, acc1, t5
    sltu cy0, acc1, t5
    # a2 * b
    mulhu acc3, a2, b
    mul t4, a2, b
    add acc2, acc2, cy0
     sd acc1, 8(rd)        # rd[1]
    add acc2, acc2, t4
    sltu cy0, acc2, t4
    # a3 * b
    mulhu acc4, a3, b
    mul t5, a3, b
    add acc3, acc3, cy0
     sd acc2, 16(rd)       # rd[2]
    add acc3, acc3, t5
    sltu cy0, acc3, t5
     sd acc3, 24(rd)       # rd[3]

    add a0, acc4, cy0      # carry
    ret
.size ll_u256_mul_limb, .-ll_u256_mul_limb


# void ll_u256_mul(u64 rd[8], const u64 ad[4], const u64 bd[4])
.globl	ll_u256_mul
.align	4
ll_u256_mul:
    addi sp, sp, -16
    sd s0, 0(sp)
    sd s1, 8(sp)
    # save bd, rd
    mv bd, a2
    mv rd, a0
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

    # load b1
    ld b, 8(bd)
    # a0 * b1
    mulhu s1, a0, b
    mul s0, a0, b
    add acc1, acc1, s0
    sltu cy1, acc1, s0
    add s1, s1, cy1
    # a1 * b1
    mulhu t0, a1, b       # reuse acc0 = t0
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
    mulhu acc5, a3, b
    mul t0, a3, b
    add acc4, acc4, s1
    sltu cy0, acc4, s1
    add acc4, acc4, t0
    add cy0, cy0, cy1
    sltu cy1, acc4, t0
    add acc5, acc5, cy0
     sd acc1, 8(rd)       # r1
    add acc5, acc5, cy1

    # load b2
    ld b, 16(bd)
    # a0 * b2
    mulhu s1, a0, b
    mul s0, a0, b
    add acc2, acc2, s0
    sltu cy1, acc2, s0
    add s1, s1, cy1
    # a1 * b2
    mulhu t1, a1, b       # reuse acc1 = t1
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
    mulhu acc0, a3, b
    mul t1, a3, b
    add acc5, acc5, s1
    sltu cy0, acc5, s1
    add acc5, acc5, t1
    add cy0, cy0, cy1
    sltu cy1, acc5, t1
    add acc0, acc0, cy0
     sd acc2, 16(rd)      # r2
    add acc0, acc0, cy1

    # load b3
    ld b, 24(bd)
    # a0 * b3
    mulhu s1, a0, b
    mul s0, a0, b
    add acc3, acc3, s0
    sltu cy1, acc3, s0
    add s1, s1, cy1
    # a1 * b3
    mulhu t2, a1, b       # reuse acc2 = t2
    mul s0, a1, b
    add acc4, acc4, s1
     sd acc3, 24(rd)      # r3
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
     sd acc4, 32(rd)      # r4
    sltu cy0, acc5, t2
    add acc5, acc5, s0
    add cy0, cy0, cy1
    sltu cy1, acc5, s0
    add s1, s1, cy0
     add acc0, acc0, cy1
     sltu cy1, acc0, cy1
    # a3 * b3
    mulhu acc1, a3, b
    mul t2, a3, b
    add acc0, acc0, s1
    sd acc5, 40(rd)       # r5
    sltu cy0, acc0, s1
    add acc0, acc0, t2
    add cy0, cy0, cy1
    sltu cy1, acc0, t2
    add acc1, acc1, cy0
     sd acc0, 48(rd)      # r6
    add acc1, acc1, cy1
    sd acc1, 56(rd)       # r7

    ld s0, 0(sp)
    ld s1, 8(sp)
    addi sp, sp, 16
    ret
.size ll_u256_mul, .-ll_u256_mul


# void ll_u256_mullo(u64 rd[4], const u64 ad[4], const u64 bd[4])
.globl	ll_u256_mullo
.align	4
ll_u256_mullo:
    # save bd, rd
    mv bd, a2
    mv rd, a0
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
    mul t4, a1, b
    add acc1, acc1, t4
    sltu cy0, acc1, t4
    # a2 * b0
    mulhu acc3, a2, b
    mul t5, a2, b
    add acc2, acc2, cy0
    add acc2, acc2, t5
    sltu cy0, acc2, t5
    # a3 * b0
    mul t4, a3, b
    add acc3, acc3, cy0
    add acc3, acc3, t4
     sd acc0, 0(rd)       # r0

    # load b1
    ld b, 8(bd)
    # a0 * b1
    mulhu t5, a0, b
    mul t4, a0, b
    add acc1, acc1, t4
    sltu cy1, acc1, t4
    add t5, t5, cy1
    # a1 * b1
    mulhu t0, a1, b       # reuse acc0 = t0
    mul t4, a1, b
    add acc2, acc2, t5
    sltu cy0, acc2, t5
    add acc2, acc2, t4
    sltu cy1, acc2, t4
    add t0, t0, cy0
     add acc3, acc3, cy1
    # a2 * b1
    mul t4, a2, b
    add acc3, acc3, t0
     sd acc1, 8(rd)       # r1
    add acc3, acc3, t4

    # load b2
    ld b, 16(bd)
    # a0 * b2
    mulhu t5, a0, b
    mul t4, a0, b
    add acc2, acc2, t4
    sltu cy1, acc2, t4
    add t5, t5, cy1
    # a1 * b2
    mul t4, a1, b
    add acc3, acc3, t5
     sd acc2, 16(rd)      # r2
    add acc3, acc3, t4

    # load b3
    ld b, 24(bd)
    # a0 * b3
    mul t4, a0, b
    add acc3, acc3, t4

    sd acc3, 24(rd)      # r3
    ret
.size ll_u256_mullo, .-ll_u256_mullo


# void ll_u256_sqr(u64 rd[8], const u64 ad[4])
.globl	ll_u256_sqr
.align	4
ll_u256_sqr:
    addi sp, sp, -16
    sd s0, 0(sp)
    sd s1, 8(sp)
    # save rd
    mv rd, a0
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
     mulhu a4, a0, a0
     mul acc0, a0, a0
    slli acc1, acc1, 1
    or acc2, acc2, t6

    # a0^2
    add acc1, acc1, a4
    sltu cy0, acc1, a4
    # a1^2
    mulhu a4, a1, a1
    mul t6, a1, a1
    add acc2, acc2, cy0
    sltu cy1, acc2, cy0
    add acc2, acc2, t6
    sltu cy0, acc2, t6
    add a4, a4, cy1
    add a4, a4, cy0
     sd acc0, 0(rd)
     sd acc1, 8(rd)
    add acc3, acc3, a4
    sltu cy0, acc3, a4
    # a2^2
    mulhu t0, a2, a2
    mul t6, a2, a2
    add acc4, acc4, cy0
    sltu cy1, acc4, cy0
    add acc4, acc4, t6
    sltu cy0, acc4, t6
    add t0, t0, cy1
    add t0, t0, cy0
     sd acc2, 16(rd)
     sd acc3, 24(rd)
    add acc5, acc5, t0
    sltu cy0, acc5, t0
    # a3^2
    mulhu a4, a3, a3
    mul t6, a3, a3
    add acc6, acc6, cy0
    sltu cy1, acc6, cy0
    add acc6, acc6, t6
    sltu cy0, acc6, t6
    add a4, a4, cy1
    add a4, a4, cy0
     sd acc4, 32(rd)
     sd acc5, 40(rd)
    add acc7, acc7, a4

    sd acc6, 48(rd)
    sd acc7, 56(rd)
    ld s0, 0(sp)
    ld s1, 8(sp)
    addi sp, sp, 16
    ret
.size ll_u256_sqr, .-ll_u256_sqr


# void ll_u256_sqrlo(u64 rd[4], const u64 ad[4])
.globl	ll_u256_sqrlo
.align	4
ll_u256_sqrlo:
    # save rd
    mv rd, a0
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
    mul t5, a2, a0
    add acc2, acc2, t5
    sltu cy0, acc2, t5
    # a3 * a0
    mul t4, a3, a0
    add acc3, acc3, cy0
    add acc3, acc3, t4

    # a2 * a1
    mul t5, a2, a1
    add acc3, acc3, t5

    # acc3:acc1 << 1
    slli acc3, acc3, 1
    srli t6, acc2, 63
    slli acc2, acc2, 1
    or acc3, acc3, t6
    srli t6, acc1, 63
     mulhu a4, a0, a0
     mul acc0, a0, a0
    slli acc1, acc1, 1
    or acc2, acc2, t6

    # a0^2
    add acc1, acc1, a4
    sltu cy0, acc1, a4
    # a1^2
    mulhu a4, a1, a1
    mul t6, a1, a1
    add acc2, acc2, cy0
    sltu cy1, acc2, cy0
    add acc2, acc2, t6
    sltu cy0, acc2, t6
    add a4, a4, cy1
    add a4, a4, cy0
     sd acc0, 0(rd)
     sd acc1, 8(rd)
    add acc3, acc3, a4

    sd acc2, 16(rd)
    sd acc3, 24(rd)
    ret
.size ll_u256_sqrlo, .-ll_u256_sqrlo
