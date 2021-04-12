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

# u64 ll_add_limb(u64 *rd, u64 *ad, u64 b, size_t al)
.globl	ll_add_limb
.align	4
ll_add_limb:
    mv a4, a0        # save rd
    mv a0, a2        # carry
    beq a3, x0, .Ladd_limb_done

.Ladd_limb_loop:
    ld t1, 0(a1)
    addi a1, a1, 8   # ad++
    addi a3, a3, -1  # al--
    add t1, t1, a0
    sltu a0, t1, a0  # carry
    sd t1, 0(a4)
    addi a4, a4, 8   # rd++
    bne a3, x0, .Ladd_limb_loop

.Ladd_limb_done:
    ret
.size ll_add_limb, .-ll_add_limb


# u64 ll_add_limbs(u64 *rd, u64 *ad, u64 *bd, size_t l)
.globl	ll_add_limbs
.align	4
ll_add_limbs:
    mv a4, a0        # save rd
    mv a0, x0        # carry
    beq a3, x0, .Ladd_limbs_done

.Ladd_limbs_loop:
    ld t1, 0(a1)
    ld t2, 0(a2)
    addi a1, a1, 8   # ad++
    addi a2, a2, 8   # bd++
    add t1, t1, a0
    sltu t0, t1, a0
    add t1, t1, t2
    sltu a0, t1, t2
    addi a3, a3, -1  # l--
    sd t1, 0(a4)
    add a0, a0, t0   # carry
    addi a4, a4, 8   # rd++
    bne a3, x0, .Ladd_limbs_loop

.Ladd_limbs_done:
    ret
.size ll_add_limbs, .-ll_add_limbs


# u64 ll_sub_limb(u64 *rd, u64 *ad, u64 b, size_t al)
.globl	ll_sub_limb
.align	4
ll_sub_limb:
    mv a4, a0        # save rd
    mv a0, a2        # borrow
    beq a3, x0, .Lsub_limb_done

.Lsub_limb_loop:
    ld t1, 0(a1)
    addi a1, a1, 8   # ad++
    addi a3, a3, -1  # al--
    sub t2, t1, a0
    sltu a0, t1, a0  # borrow
    sd t2, 0(a4)
    addi a4, a4, 8  # rd++
    bne a3, x0, .Lsub_limb_loop

.Lsub_limb_done:
    ret
.size ll_sub_limb, .-ll_sub_limb


# u64 ll_sub_limbs(u64 *rd, u64 *ad, u64 *bd, size_t l)
.globl	ll_sub_limbs
.align	4
ll_sub_limbs:
    mv a4, a0        # save rd
    mv a0, x0        # borrow
    beq a3, x0, .Lsub_limbs_done

.Lsub_limbs_loop:
    ld t1, 0(a1)
    ld t2, 0(a2)
    addi a1, a1, 8   # ad++
    addi a2, a2, 8   # bd++
    sub t3, t1, a0
    sltu t0, t1, a0
    sub t1, t3, t2
    sltu a0, t3, t2
    addi a3, a3, -1  # l--
    sd t1, 0(a4)
    add a0, a0, t0   # borrow
    addi a4, a4, 8   # rd++
    bne a3, x0, .Lsub_limbs_loop

.Lsub_limbs_done:
    ret
.size ll_sub_limbs, .-ll_sub_limbs


# void ll_mont_cond_sub_limbs(u64 *rd, u64 *ad, u64 *bd, size_t l)
.globl	ll_mont_cond_sub_limbs
.align	4
ll_mont_cond_sub_limbs:
    mv t6, x0        # borrow
    mv t4, a3        # save l
    beq a3, x0, .Lcond_sub_done

.Lcond_sub_loop:
    ld t1, 0(a1)
    ld t2, 0(a2)
    addi a1, a1, 8   # ad++
    addi a2, a2, 8   # bd++
    sub t3, t1, t6
    sltu t0, t1, t3
    sub t1, t3, t2
    sltu t6, t3, t1
    addi a3, a3 ,-1  # l--
    sd t1, 0(a0)
    add t6, t6, t0   # borrow
    addi a0, a0, 8   # rd++
    bne a3, x0, .Lcond_sub_loop

    ld t1, 0(a1)
    addi a1, a1, -8  # ad--
    addi a0, a0, -8  # rd--
    bltu t1, t6, .Lcond_move
    mv a1, a0

.Lcond_move:
    ld t1, 0(a1)
    addi a1, a1, -8
    addi t4, t4, -1  # l--
    sd t1, 0(a0)
    addi a0, a0, -8
    bne t4, x0, .Lcond_move

.Lcond_sub_done:
    ret
.size ll_mont_cond_sub_limbs, .-ll_mont_cond_sub_limbs
