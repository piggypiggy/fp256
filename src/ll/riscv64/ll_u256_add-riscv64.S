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

# u64 ll_u256_add(u64 rd[4], const u64 ad[4], const u64 bd[4])
.globl	ll_u256_add
.align	4
ll_u256_add:
    ld t0, 0(a1)
    ld t1, 0(a2)
    ld t2, 8(a1)
    add t6, t0, t1        # ad[0] + bd[0]
    ld t3, 8(a2)
    sltu a6, t6, t0       # carry1
    sd t6, 0(a0)          # rd[0]
    add t5, t2, t3        # ad[1] + bd[1]
    ld t0, 16(a1)
    sltu a4, t5, t2       # carry2
    add t6, t5, a6        # ad[1] + bd[1] + carry1
    ld t1, 16(a2)
    sltu a5, t6, t5       # carry2
    sd t6, 8(a0)          # rd[1]
    add a6, a4, a5        # carry2
    add t5, t0, t1        # ad[2] + bd[2]
    ld t2, 24(a1)
    sltu a4, t5, t0       # carry3
    add t6, t5, a6        # ad[2] + bd[2] + carry2
    ld t3, 24(a2)
    sltu a5, t6, t5       # carry3
    sd t6, 16(a0)         # rd[2]
    add a6, a4, a5        # carry3
    add t5, t2, t3        # ad[3] + bd[3]
    sltu a4, t5, t2       # carry4
    add t6, t5, a6        # ad[3] + bd[3] + carry3
    sltu a5, t6, t5       # carry4
    sd t6, 24(a0)         # rd[3]
    add a0, a4, a5        # carry4
    ret
.size ll_u256_add, .-ll_u256_add


# u64 ll_u256_sub(u64 rd[4], const u64 ad[4], const u64 bd[4])
.globl	ll_u256_sub
.align	4
ll_u256_sub:
    ld t0, 0(a1)
    ld t1, 0(a2)
    ld t2, 8(a1)
    sub t6, t0, t1        # ad[0] - bd[0]
    ld t3, 8(a2)
    sltu a6, t0, t1       # borrow1
    sd t6, 0(a0)          # rd[0]
    sub t5, t2, t3        # ad[1] - bd[1]
    ld t0, 16(a1)
    sltu a4, t2, t3       # borrow2
    sub t6, t5, a6        # ad[1] - bd[1] - borrow1
    ld t1, 16(a2)
    sltu a5, t5, a6       # borrow2
    sd t6, 8(a0)          # rd[1]
    add a6, a4, a5        # borrow2
    sub t5, t0, t1        # ad[2] - bd[2]
    ld t2, 24(a1)
    sltu a4, t0, t1       # borrow3
    sub t6, t5, a6        # ad[2] - bd[2] - borrow2
    ld t3, 24(a2)
    sltu a5, t5, a6       # borrow3
    sd t6, 16(a0)         # rd[2]
    add a6, a4, a5        # borrow3
    sub t5, t2, t3        # ad[3] - bd[3]
    sltu a4, t2, t3       # borrow4
    sub t6, t5, a6        # ad[3] - bd[3] - borrow3
    sltu a5, t5, a6       # borrow4
    sd t6, 24(a0)         # rd[3]
    add a0, a4, a5        # borrow4
    ret
.size ll_u256_sub, .-ll_u256_sub
