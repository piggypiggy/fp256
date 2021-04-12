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

# u64 ll_mul_limb(u64 *rd, u64 *ad, u64 b, size_t al)
.globl	ll_mul_limb
.align	4
ll_mul_limb:
    mv a4, a0        # save rd
    mv t0, x0
    beq a3, x0, .Lmul_limb_done

.Lmul_limb_loop:
    ld t1, 0(a1)
    addi a1, a1, 8   # ad++
    mulhu a0, t1, a2
    mul t2, t1, a2
    addi a3, a3, -1  # al--
    add t2, t2, t0
    sltu t0, t2, t0  # carry
    sd t2, 0(a4)
    add a0, a0, t0   # MSB
    addi a4, a4, 8   # rd++
    mv t0, a0
    bltu x0, a3, .Lmul_limb_loop

.Lmul_limb_done:
    sd t0, 0(a4)
    ret
.size ll_mul_limb, .-ll_mul_limb


# u64 ll_muladd_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al)
.globl	ll_muladd_limb
.align	4
ll_muladd_limb:
    mv a5, a0        # save rd
    mv a6, x0        # i
    mv a0, x0        # carry

.Lmuladd_limb_mul_loop:  # rd[0, al-1] += ad * b
    beq a6, a4, .Lmuladd_limb_add_loop
    ld t2, 0(a1)
    ld t3, 0(a5)
    addi a1, a1, 8   # ad++
    mulhu t1, t2, a2
    mul t0, t2, a2
    add t3, t3, a0
    sltu a0, t3, a0
    addi a6, a6, 1   # i++
    add a0, a0, t1
    add t3, t3, t0
    sltu t0, t3, t0
    sd t3, 0(a5)
    add a0, a0, t0   # carry
    addi a5, a5, 8   # rd++
    j .Lmuladd_limb_mul_loop

.Lmuladd_limb_add_loop:  # rl > al, rd[al, rl-1] += a0
    bgeu a6, a3, .Lmuladd_limb_done
    ld t2, 0(a5)
    addi a6, a6, 1   # i++
    add t2, t2, a0
    sltu a0, t2, a0  # carry
    sd t2, 0(a5)
    addi a5, a5, 8   # rd++
    j .Lmuladd_limb_add_loop

.Lmuladd_limb_done:
    sd a0, 0(a5)
    ret
.size ll_muladd_limb, .-ll_muladd_limb


# u64 ll_mulsub_limb(u64 *rd, u64 *ad, u64 b, size_t rl, size_t al)
.globl	ll_mulsub_limb
.align	4
ll_mulsub_limb:
    mv a5, a0        # save rd
    mv a6, x0        # i
    mv a0, x0        # borrow

.Lmulsub_limb_mul_loop:  # rd[0, al-1] -= ad * b
    beq a6, a4, .Lmulsub_limb_add_loop
    ld t2, 0(a1)
    ld t3, 0(a5)
    addi a1, a1, 8   # ad++
    mulhu t1, t2, a2
    mul t0, t2, a2
    sub t4, t3, a0
    sltu t5, t3, a0
    addi a6, a6, 1   # i++
    sub t3, t4, t0
    add t5, t5, t1
    sltu t0, t4, t0
    sd t3, 0(a5)
    addi a5, a5, 8   # rd++
    add a0, t5, t0   # borrow
    j .Lmulsub_limb_mul_loop

.Lmulsub_limb_add_loop:  # rl > al, rd[al, rl-1] -= a0
    bgeu a6, a3, .Lmulsub_limb_done
    ld t2, 0(a5)
    addi a6, a6, 1   # i++
    sub t1, t2, a0
    sltu a0, t2, a0  # borrow
    sd t1, 0(a5)
    addi a5, a5, 8   # rd++
    j .Lmulsub_limb_add_loop

.Lmulsub_limb_done:
    ret
.size ll_mulsub_limb, .-ll_mulsub_limb
