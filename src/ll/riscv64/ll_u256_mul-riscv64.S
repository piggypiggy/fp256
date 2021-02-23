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

# void ll_u256_mul(u64 rd[8], const u64 ad[4], const u64 bd[4])
.globl	ll_u256_mul
.align	4
ll_u256_mul:
    addi x2, x2, -16
    sd s0, 0(x2)
    sd s1, 8(x2)
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
     sd acc3, 24(rd)       # r3
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
    sd acc5, 40(rd)      # r5
    sltu cy0, acc0, s1
    add acc0, acc0, t2
    add cy0, cy0, cy1
    sltu cy1, acc0, t2
    add acc1, acc1, cy0
     sd acc0, 48(rd)      # r6
    add acc1, acc1, cy1
    sd acc1, 56(rd)       # r7

    ld s0, 0(x2)
    ld s1, 8(x2)
    addi x2, x2, 16
    ret
.size ll_u256_mul, .-ll_u256_mul


# void ll_u256_sqr(u64 rd[8], const u64 ad[4])
.globl	ll_u256_sqr
.align	4
ll_u256_sqr:
    addi x2, x2, -16
    sd s0, 0(x2)
    sd s1, 8(x2)
    # save rd
    mv rd, a0
    # load a0~a3
    ld a3, 24(a1)
    ld a2, 16(a1)
    ld a0, 0(a1)
    ld a1, 8(a1)

    

    ld s0, 0(x2)
    ld s1, 8(x2)
    addi x2, x2, 16
    ret
.size ll_u256_sqr, .-ll_u256_sqr
