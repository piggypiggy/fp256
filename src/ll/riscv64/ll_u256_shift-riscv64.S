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

# void ll_u256_lshift(u64 r[5], u64 a[4], size_t n)
.globl	ll_u256_lshift
.align	4
ll_u256_lshift:
    li a3, 64
    ld t0, 0(a1)
    ld t1, 8(a1)
    mv t4, x0
    sub a3, a3, a2     # 64 - n
    ld t2, 16(a1)
    ld t3, 24(a1)
    beq a2, x0, .Lu256_lshift_done

    srl t4, t3, a3     # r[4] = a[3] >> (64-n)
    sll t3, t3, a2     # a[3] <<= n
    srl t6, t2, a3     # a[2] >> (64-n)
     sll t2, t2, a2    # a[2] <<= n
    or t3, t3, t6      # r[3] = (a[2] >> (64-n)) | (a[3] << n)
    srl t6, t1, a3     # a[1] >> (64-n)
     sll t1, t1, a2    # a[1] <<= n
    or t2, t2, t6      # r[2] = (a[1] >> (64-n)) | (a[2] << n)
    srl t6, t0, a3     # a[0] >> (64-n)
     sll t0, t0, a2    # a[0] <<= n
    or t1, t1, t6      # r[1] = (a[0] >> (64-n)) | (a[1] << n)

.Lu256_lshift_done:
    sd t4, 32(a0)
    sd t3, 24(a0)
    sd t2, 16(a0)
    sd t1, 8(a0)
    sd t0, 0(a0)
    ret
.size ll_u256_lshift, .-ll_u256_lshift


# void ll_u256_rshift(u64 r[5], u64 a[4], size_t n)
.globl	ll_u256_rshift
.align	4
ll_u256_rshift:
    li a3, 64
    ld t0, 0(a1)
    ld t1, 8(a1)
    sub a3, a3, a2     # 64 - n
    ld t2, 16(a1)
    ld t3, 24(a1)
    beq a2, x0, .Lu256_rshift_done

    srl t0, t0, a2     # a[0] >> n
    sll t6, t1, a3     # a[1] << (64-n)
     srl t1, t1, a2    # a[1] >> n
    or t0, t0, t6      # r[0] = (a[1] << (64-n)) | (a[0] >> n)
    sll t6, t2, a3     # a[2] << (64-n)
     srl t2, t2, a2    # a[2] >> n
    or t1, t1, t6      # r[1] = (a[2] << (64-n)) | (a[1] >> n)
    sll t6, t3, a3     # a[3] << (64-n)
     srl t3, t3, a2    # r[3] = a[3] >> n
    or t2, t2, t6      # r[2] = (a[3] << (64-n)) | (a[2] >> n)

.Lu256_rshift_done:
    sd t3, 24(a0)
    sd t2, 16(a0)
    sd t1, 8(a0)
    sd t0, 0(a0)
    ret
.size ll_u256_rshift, .-ll_u256_rshift
