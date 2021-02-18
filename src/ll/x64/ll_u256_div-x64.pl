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
___

{
my ($r_ptr,$q_ptr,$n_org,$div,$nl,$n_ptr)=("%rdi","%rsi","%rdx","%rcx","%r8","%r9");
my ($d0,$d1)=("%r10","%r11");
my ($quo,$rem)=("%rcx","%rbx");
my ($n0,$n1,$n2)=("%rbp","%r12","%r13");
my ($t0,$t1)=("%r14","%r15");

$code.=<<___;
# void ll_naive_div_1_limb(u64 *rd, u64 *qd, u64 *nd, u64 *dd, int nl);
.globl	ll_naive_div_1_limb
.type	ll_naive_div_1_limb,\@function,5
.align	32
ll_naive_div_1_limb:
    lea -16($n_org,$nl,8), $n_ptr      # n_ptr points to the second last limb of num
    lea -8($q_ptr,$nl,8), $q_ptr

    mov 0($div), $div
    mov 8($n_ptr), %rdx
    cmp $div, %rdx
    jb .ll_naive_div_1_limb_loop             # num[nl-1] >= div
    mov %rdx, %rax
    xor %rdx, %rdx
    divq $div                          # 0:num[nl-1] / div
    mov %rax, 0($q_ptr)

.ll_naive_div_1_limb_loop:
    sub \$8, $q_ptr
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_1_limb_done
    mov 0($n_ptr), %rax
    divq $div
    sub \$8, $n_ptr
    mov %rax, 0($q_ptr)
    jmp .ll_naive_div_1_limb_loop

.ll_naive_div_1_limb_done:
    mov %rdx, 0($r_ptr)
    ret
.size	ll_naive_div_1_limb,.-ll_naive_div_1_limb


# void ll_naive_div_2_limbs(u64 *rd, u64 *qd, u64 *nd, u64 *dd, int nl);
.globl	ll_naive_div_2_limbs
.type	ll_naive_div_2_limbs,\@function,5
.align	32
ll_naive_div_2_limbs:
    push %rbp
    push %rbx
    push %r12
    push %r13

    cmp \$2, $nl
    jl .ll_naive_div_2_limbs_done        # nl < 2
    ja .ll_naive_div_2_limbs_begin       # nl > 2
    inc $nl

.ll_naive_div_2_limbs_begin:
    lea -24($n_org,$nl,8), $n_ptr  # n_ptr points to the third last limb of num
    lea -24($q_ptr,$nl,8), $q_ptr
    mov 8($n_ptr), $n1
    mov 16($n_ptr), $n2
    mov 0($div), $d0
    mov 8($div), $d1

    mov $n1, %rax
    mov $n2, %rdx
    sub \$2, $nl

    # compare the most significant 2 limbs of num and div
    sub $d0, $n1
    sbb $d1, $n2
    jb .ll_naive_div_2_limbs_loop1       # n2:n1 < d1:d0
    movq \$1, 8($q_ptr)            # n2:n1 / d1:d0 = 1
    mov $n1, %rax
    mov $n2, %rdx

.ll_naive_div_2_limbs_loop1:
    cmp $d1, %rdx                  # rdx:rax:n0 / d1:d0 
    je .ll_naive_div_2_limbs_equal       # if most significant limb is equal
    divq $d1
    mov 0($n_ptr), $n0
    mov %rax, $quo                 # quo
    mov %rdx, $rem                 # rem
    mulq $d0                       # quo * d0
    jmp .ll_naive_div_2_limbs_loop2

.ll_naive_div_2_limbs_equal:             # quo can either be 0xffffffffffffffff or 0xfffffffffffffffe
    mov 0($n_ptr), $n0
    xor %rbx, %rbx
    add $d0, $n0
    adc $d1, %rax
    adc \$0, %rbx
    movq \$-1, $quo
    sub $d0, %rax
    sbb \$0, %rbx
    jnc .ll_naive_div_2_limbs_q_is_minus_1
    sub \$1, $quo                  # q is minus 2
    add $d0, $n0
    adc $d1, %rax

.ll_naive_div_2_limbs_q_is_minus_1:
    mov $quo, 0($q_ptr)
    mov %rax, %rdx
    sub \$8, $n_ptr
    sub \$8, $q_ptr
    mov $n0, %rax
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_2_limbs_done
    jmp .ll_naive_div_2_limbs_loop1

.ll_naive_div_2_limbs_loop2:
    cmp %rdx, $rem
    ja .ll_naive_div_2_limbs_got_quo     # if rem > (quo * d0)'s higher part
    jb .ll_naive_div_2_limbs_dec_quo     # if rem < (quo * d0)'s higher part
    cmp %rax, $n0
    jae .ll_naive_div_2_limbs_got_quo    # if num[0] >= (quo * d0)'s lower part

.ll_naive_div_2_limbs_dec_quo:
    dec $quo
    sub $d0, %rax
    sbb \$0, %rdx
    add $d1, $rem
    jb .ll_naive_div_2_limbs_got_quo     # if carry
    jmp .ll_naive_div_2_limbs_loop2

.ll_naive_div_2_limbs_got_quo:
    mov $quo, 0($q_ptr)
    sub %rax, $n0
    sbb %rdx, $rem
    sub \$8, $q_ptr
    sub \$8, $n_ptr
    mov $n0, %rax
    mov $rem, %rdx
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_2_limbs_done
    jmp .ll_naive_div_2_limbs_loop1

.ll_naive_div_2_limbs_done:
    mov %rdx, 8($r_ptr)
    mov %rax, 0($r_ptr)
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_naive_div_2_limbs,.-ll_naive_div_2_limbs


# void ll_naive_div_3_limbs(u64 *rd, u64 *qd, u64 *nd, u64 *dd, int nl);
.globl	ll_naive_div_3_limbs
.type	ll_naive_div_3_limbs,\@function,5
.align	32
ll_naive_div_3_limbs:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15

    cmp \$3, $nl
    jl .ll_naive_div_3_limbs_done        # nl < 3
    ja .ll_naive_div_3_limbs_begin       # nl > 3
    inc $nl

.ll_naive_div_3_limbs_begin:
    lea -24($n_org,$nl,8), $n_ptr  # n_ptr points to the third last limb of num
    lea -32($q_ptr,$nl,8), $q_ptr
    mov 0($n_ptr), $n0
    mov 8($n_ptr), $n1
    mov 16($n_ptr), $n2
    mov 0($div), $t0               # t0 = d_{-1}
    mov 8($div), $d0
    mov 16($div), $d1

    mov $n1, %rax
    mov $n2, %rdx
    sub \$3, $nl

    # compare the most significant 3 limbs of num and div
    sub $t0, $n0
    sbb $d0, $n1
    sbb $d1, $n2
    jb .ll_naive_div_3_limbs_loop1       # n2:n1:n0 < d1:d0:d_{-1}
    movq \$1, 8($q_ptr)            # n2:n1 / d1:d0 = 1
    mov $n0, 0($n_ptr)
    mov $n1, %rax
    mov $n2, %rdx

.ll_naive_div_3_limbs_loop1:
    cmp $d1, %rdx                  # rdx:rax:n0 / d1:d0 
    je .ll_naive_div_3_limbs_equal       # if most significant limb is equal
    divq $d1
    mov 0($n_ptr), $n0
    mov %rax, $quo                 # quo
    mov %rdx, $rem                 # rem
    mulq $d0                       # quo * d0
    jmp .ll_naive_div_3_limbs_loop2

.ll_naive_div_3_limbs_equal:             # quo can either be 0xffffffffffffffff or 0xfffffffffffffffe
    mov -8($n_ptr), $t1            # n_{-1}
    mov 0($n_ptr), $n0
    xor %rbx, %rbx
    add $t0, $t1
    adc $d0, $n0
    adc $d1, %rax
    adc \$0, %rbx
    movq \$-1, $quo
    sub $t0, $n0 
    sbb $d0, %rax
    sbb \$0, %rbx
    jnc .ll_naive_div_3_limbs_q_is_minus_1
    sub \$1, $quo                  # q is minus 2
    add $t0, $t1
    adc $d0, $n0
    adc $d1, %rax

.ll_naive_div_3_limbs_q_is_minus_1:
    mov $quo, 0($q_ptr)
    mov %rax, %rdx
    sub \$8, $n_ptr
    sub \$8, $q_ptr
    mov $n0, %rax
    mov $t1, 0($n_ptr)
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_3_limbs_done
    jmp .ll_naive_div_3_limbs_loop1

.ll_naive_div_3_limbs_loop2:
    cmp %rdx, $rem
    ja .ll_naive_div_3_limbs_got_quo     # if rem > (quo * d0)'s higher part
    jb .ll_naive_div_3_limbs_dec_quo     # if rem < (quo * d0)'s higher part
    cmp %rax, $n0
    jae .ll_naive_div_3_limbs_got_quo    # if num[0] >= (quo * d0)'s lower part

.ll_naive_div_3_limbs_dec_quo:
    dec $quo
    sub $d0, %rax
    sbb \$0, %rdx
    add $d1, $rem
    jb .ll_naive_div_3_limbs_got_quo     # if carry
    jmp .ll_naive_div_3_limbs_loop2

.ll_naive_div_3_limbs_got_quo:           # adjust quo
    sub %rax, $n0
    mov $quo, %rax                 # rem:n0:n_{-1} - quo * d_{-1}
    sbb %rdx, $rem
    mulq $t0                       # quo * d_{-1}
    mov -8($n_ptr), $t1            # n_{-1}
    sub %rax, $t1
    sbb %rdx, $n0
    sbb \$0, $rem
    jnc .ll_naive_div_3_limbs_got_quo_finish
    dec $quo
    add $t0, $t1                   # + d1:d0:d_{-1}
    adc $d0, $n0
    adc $d1, $rem

.ll_naive_div_3_limbs_got_quo_finish:
    mov $t1, -8($n_ptr)
    mov $n0, %rax
    mov $rem, %rdx
    mov $quo, 0($q_ptr)
    sub \$8, $q_ptr
    sub \$8, $n_ptr
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_3_limbs_done
    jmp .ll_naive_div_3_limbs_loop1

.ll_naive_div_3_limbs_done:
    mov %rdx, 16($r_ptr)
    mov %rax, 8($r_ptr)
    mov $t1, 0($r_ptr)
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_naive_div_3_limbs,.-ll_naive_div_3_limbs


# void ll_naive_div_4_limbs(u64 *rd, u64 *qd, u64 *nd, u64 *dd, int nl);
.globl	ll_naive_div_4_limbs
.type	ll_naive_div_4_limbs,\@function,5
.align	32
ll_naive_div_4_limbs:
    push %rbp
    push %rbx
    push %r12
    push %r13
    push %r14
    push %r15

    cmp \$4, $nl
    jl .ll_naive_div_4_limbs_done        # nl < 4
    ja .ll_naive_div_4_limbs_begin       # nl > 4
    inc $nl

.ll_naive_div_4_limbs_begin:
    lea -24($n_org,$nl,8), $n_ptr  # n_ptr points to the third last limb of num
    lea -40($q_ptr,$nl,8), $q_ptr
    mov -8($n_ptr), %rbx           # n_{-1}
    mov 0($n_ptr), $n0
    mov 8($n_ptr), $n1
    mov 16($n_ptr), $n2
    mov 0($div), $t1               # t1 = d_{-2}
    mov 8($div), $t0               # t0 = d_{-1}
    mov 16($div), $d0
    mov 24($div), $d1

    mov $n1, %rax
    mov $n2, %rdx
    sub \$4, $nl

    # compare the most significant 4 limbs of num and div
    sub $t1, %rbx
    sbb $t0, $n0
    sbb $d0, $n1
    sbb $d1, $n2
    jb .ll_naive_div_4_limbs_loop1       # n2:n1:n0:n_{-1} < d1:d0:d_{-1}:d_{-2}
    movq \$1, 8($q_ptr)            # n2:n1:n0:n_{-1} / d1:d0:d_{-1}:d_{-2} = 1
    mov %rbx, -8($n_ptr)
    mov $n0, 0($n_ptr)
    mov $n1, %rax
    mov $n2, %rdx

.ll_naive_div_4_limbs_loop1:
    cmp $d1, %rdx                  # rdx:rax:n0 / d1:d0 
    je .ll_naive_div_4_limbs_equal       # if most significant limb is equal
    divq $d1
    mov 0($n_ptr), $n0
    mov %rax, $quo                 # quo
    mov %rdx, $rem                 # rem
    mulq $d0                       # quo * d0
    jmp .ll_naive_div_4_limbs_loop2

.ll_naive_div_4_limbs_equal:             # quo can either be 0xffffffffffffffff or 0xfffffffffffffffe
    mov -8($n_ptr), $t1            # n_{-1}
    mov 0($n_ptr), $n0
    xor %rbx, %rbx
    add $t0, $t1
    adc $d0, $n0
    adc $d1, %rax
    adc \$0, %rbx
    movq \$-1, $quo
    sub $t0, $n0 
    sbb $d0, %rax
    sbb \$0, %rbx
    jnc .ll_naive_div_4_limbs_q_is_minus_1
    sub \$1, $quo                  # q is minus 2
    add $t0, $t1
    adc $d0, $n0
    adc $d1, %rax

.ll_naive_div_4_limbs_q_is_minus_1:
    mov $quo, 0($q_ptr)
    mov %rax, %rdx
    sub \$8, $n_ptr
    sub \$8, $q_ptr
    mov $n0, %rax
    mov $t1, 0($n_ptr)
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_4_limbs_done
    jmp .ll_naive_div_4_limbs_loop1

.ll_naive_div_4_limbs_loop2:
    cmp %rdx, $rem
    ja .ll_naive_div_4_limbs_got_quo     # if rem > (quo * d0)'s higher part
    jb .ll_naive_div_4_limbs_dec_quo     # if rem < (quo * d0)'s higher part
    cmp %rax, $n0
    jae .ll_naive_div_4_limbs_got_quo    # if num[0] >= (quo * d0)'s lower part

.ll_naive_div_4_limbs_dec_quo:
    dec $quo
    sub $d0, %rax
    sbb \$0, %rdx
    add $d1, $rem
    jb .ll_naive_div_4_limbs_got_quo     # if carry
    jmp .ll_naive_div_4_limbs_loop2

.ll_naive_div_4_limbs_got_quo:           # adjust quo
    sub %rax, $n0
    mov $quo, %rax                 # rem:n0:n_{-1}:n_{-2} - quo * d_{-1}:d_{-2}
    sbb %rdx, $rem
    mulq $t1                       # quo * d_{-2}
    mov %rax, %r12
    mov %rdx, %r13
    mov $quo, %rax
    mulq $t0                       # quo * d_{-1}
    add %rax, %r13
    adc \$0, %rdx                  # rdx:r13:r12 = quo * d_{-1}:d_{-2}
    sub %r12, -16($n_ptr)
    sbb %r13, -8($n_ptr)
    sbb %rdx, $n0
    sbb \$0, $rem
    jnc .ll_naive_div_4_limbs_got_quo_finish
    dec $quo
    add $t1, -16($n_ptr)           # + d1:d0:d_{-1}:d_{-2}
    adc $t0, -8($n_ptr)
    adc $d0, $n0
    adc $d1, $rem

.ll_naive_div_4_limbs_got_quo_finish:
    mov $n0, %rax
    mov $rem, %rdx
    mov $quo, 0($q_ptr)
    sub \$8, $q_ptr
    sub \$8, $n_ptr
    dec $nl
    test $nl, $nl
    jz .ll_naive_div_4_limbs_done
    jmp .ll_naive_div_4_limbs_loop1

.ll_naive_div_4_limbs_done:
    mov 0($n_ptr), $t0
    mov -8($n_ptr), $t1
    mov %rdx, 24($r_ptr)
    mov %rax, 16($r_ptr)
    mov $t0, 8($r_ptr)
    mov $t1, 0($r_ptr)
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rbx
    pop %rbp
    ret
.size	ll_naive_div_4_limbs,.-ll_naive_div_4_limbs
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
