##############################################################################
#                                                                            #
# Copyright 2020 Meng-Shan Jiang                                             #
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
my ($a0,$a1,$a2,$a3)=map("%r$_",(8..11));
my ($t0,$t1,$t2,$t3,$t4,$t5)=("%rax","%rdx","%r14","%r12","%r13","%r15");
my ($r_ptr,$a_ptr,$b_ptr,$p_ptr)=("%rdi","%rsi","%rdx","%rcx");

$code.=<<___;
# void ll_u256_fmod_div_by_2(u64 r[4], u64 a[4], u64 p[4]);
.globl	ll_u256_fmod_div_by_2
.type	ll_u256_fmod_div_by_2,\@function,3
.align	32
ll_u256_fmod_div_by_2:
    push	%r12
    push	%r13
    push	%r14
    push	%r15

    mov	8*0($a_ptr), $a0
    mov	8*1($a_ptr), $a1
    mov	8*2($a_ptr), $a2
     mov	$a0, $t0
    mov	8*3($a_ptr), $a3

     mov	$a1, $t5
    xor	$t4, $t4
    add	8*0($b_ptr), $a0
     mov	$a2, $t2
    adc	8*1($b_ptr), $a1
    adc	8*2($b_ptr), $a2
     mov	$a3, $t3
    adc	8*3($b_ptr), $a3
    adc	\$0, $t4
    xor	$b_ptr, $b_ptr		# borrow $b_ptr
    test	\$1, $t0

    cmovz	$t0, $a0
    cmovz	$t5, $a1
    cmovz	$t2, $a2
    cmovz	$t3, $a3
    cmovz	$b_ptr, $t4

    mov	$a1, $t0		# a0:a3>>1
    shr	\$1, $a0
    shl	\$63, $t0
    mov	$a2, $t5
    shr	\$1, $a1
    or	$t0, $a0
    shl	\$63, $t5
    mov	$a3, $t2
    shr	\$1, $a2
    or	$t5, $a1
    shl	\$63, $t2
    shr	\$1, $a3
    shl	\$63, $t4
    or	$t2, $a2
    or	$t4, $a3

    mov	$a0, 8*0($r_ptr)
    mov	$a1, 8*1($r_ptr)
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop	%r15
    pop	%r14
    pop	%r13
    pop	%r12
    ret
.size	ll_u256_fmod_div_by_2,.-ll_u256_fmod_div_by_2

################################################################################
# void ll_u256_fmod_add(u64 r[4], u64 a[4], u64 b[4], u64 p[4]);
.globl	ll_u256_fmod_add
.type	ll_u256_fmod_add,\@function,4
.align	32
ll_u256_fmod_add:
    push	%r12
    push	%r13
    push	%r14

    mov	8*0($a_ptr), $a0
    xor	$t4, $t4
    mov	8*1($a_ptr), $a1
    mov	8*2($a_ptr), $a2
    mov	8*3($a_ptr), $a3

    add	8*0($b_ptr), $a0
    adc	8*1($b_ptr), $a1
     mov	$a0, $t0
    adc	8*2($b_ptr), $a2
    adc	8*3($b_ptr), $a3
     mov	$a1, $t1
    adc	\$0, $t4

    sub	8*0($p_ptr), $a0
     mov	$a2, $t2
    sbb	8*1($p_ptr), $a1
    sbb	8*2($p_ptr), $a2
     mov	$a3, $t3
    sbb	8*3($p_ptr), $a3
    sbb	\$0, $t4

    cmovc	$t0, $a0
    cmovc	$t1, $a1
    mov	$a0, 8*0($r_ptr)
    cmovc	$t2, $a2
    mov	$a1, 8*1($r_ptr)
    cmovc	$t3, $a3
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop %r14
    pop %r13
    pop %r12
    ret
.size	ll_u256_fmod_add,.-ll_u256_fmod_add

################################################################################
# void ll_u256_fmod_add_limb(u64 r[4], u64 a[4], u64 w, u64 p[4]);
.globl	ll_u256_fmod_add_limb
.type	ll_u256_fmod_add_limb,\@function,4
.align	32
ll_u256_fmod_add_limb:
    push	%r12
    push	%r13
    push	%r14

    mov	8*0($a_ptr), $a0
    xor	$t4, $t4
    mov	8*1($a_ptr), $a1
    mov	8*2($a_ptr), $a2
    mov	8*3($a_ptr), $a3

    add	8*0($b_ptr), $a0
    adc	\$0, $a1
     mov	$a0, $t0
    adc	\$0, $a2
    adc	\$0, $a3
     mov	$a1, $t1
    adc	\$0, $t4

    sub	8*0($p_ptr), $a0
     mov	$a2, $t2
    sbb	8*1($p_ptr), $a1
    sbb	8*2($p_ptr), $a2
     mov	$a3, $t3
    sbb	8*3($p_ptr), $a3
    sbb	\$0, $t4

    cmovc	$t0, $a0
    cmovc	$t1, $a1
    mov	$a0, 8*0($r_ptr)
    cmovc	$t2, $a2
    mov	$a1, 8*1($r_ptr)
    cmovc	$t3, $a3
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop %r14
    pop %r13
    pop %r12
    ret
.size	ll_u256_fmod_add_limb,.-ll_u256_fmod_add_limb

################################################################################
# void ll_u256_fmod_sub(u64 r[4], u64 a[4], u64 b[4], u64 p[4]);
.globl	ll_u256_fmod_sub
.type	ll_u256_fmod_sub,\@function,4
.align	32
ll_u256_fmod_sub:
    push	%r12
    push	%r13
    push	%r14

    mov	8*0($a_ptr), $a0
    xor	$t4, $t4
    mov	8*1($a_ptr), $a1
    mov	8*2($a_ptr), $a2
    mov	8*3($a_ptr), $a3


    sub	8*0($b_ptr), $a0
    sbb	8*1($b_ptr), $a1
     mov	$a0, $t0
    sbb	8*2($b_ptr), $a2
    sbb	8*3($b_ptr), $a3
     mov	$a1, $t1
    sbb	\$0, $t4

    add	8*0($p_ptr), $a0
     mov	$a2, $t2
    adc	8*1($p_ptr), $a1
    adc	8*2($p_ptr), $a2
     mov	$a3, $t3
    adc	8*3($p_ptr), $a3
    test	$t4, $t4

    cmovz	$t0, $a0
    cmovz	$t1, $a1
    mov	$a0, 8*0($r_ptr)
    cmovz	$t2, $a2
    mov	$a1, 8*1($r_ptr)
    cmovz	$t3, $a3
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop %r14
    pop %r13
    pop %r12
    ret
.size	ll_u256_fmod_sub,.-ll_u256_fmod_sub

################################################################################
# void ll_u256_fmod_sub_limb(u64 r[4], u64 a[4], u64 w, u64 p[4]);
.globl	ll_u256_fmod_sub_limb
.type	ll_u256_fmod_sub_limb,\@function,4
.align	32
ll_u256_fmod_sub_limb:
    push	%r12
    push	%r13
    push	%r14

    mov	8*0($a_ptr), $a0
    xor	$t4, $t4
    mov	8*1($a_ptr), $a1
    mov	8*2($a_ptr), $a2
    mov	8*3($a_ptr), $a3


    sub	8*0($b_ptr), $a0
    sbb	\$0, $a1
     mov	$a0, $t0
    sbb	\$0, $a2
    sbb	\$0, $a3
     mov	$a1, $t1
    sbb	\$0, $t4

    add	8*0($p_ptr), $a0
     mov	$a2, $t2
    adc	8*1($p_ptr), $a1
    adc	8*2($p_ptr), $a2
     mov	$a3, $t3
    adc	8*3($p_ptr), $a3
    test	$t4, $t4

    cmovz	$t0, $a0
    cmovz	$t1, $a1
    mov	$a0, 8*0($r_ptr)
    cmovz	$t2, $a2
    mov	$a1, 8*1($r_ptr)
    cmovz	$t3, $a3
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop %r14
    pop %r13
    pop %r12
    ret
.size	ll_u256_fmod_sub_limb,.-ll_u256_fmod_sub_limb

################################################################################
# void ll_u256_fmod_neg(u64 r[4], u64 a[4], u64 p[4]);
.globl	ll_u256_fmod_neg
.type	ll_u256_fmod_neg,\@function,3
.align	32
ll_u256_fmod_neg:

    mov	8*0($b_ptr), $a0
    mov	8*1($b_ptr), $a1
    mov	8*2($b_ptr), $a2
    mov	8*3($b_ptr), $a3

    sub	8*0($a_ptr), $a0
    sbb	8*1($a_ptr), $a1
    sbb	8*2($a_ptr), $a2
    sbb	8*3($a_ptr), $a3

    mov	$a0, 8*0($r_ptr)
    mov	$a1, 8*1($r_ptr)
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    ret
.size	ll_u256_fmod_neg,.-ll_u256_fmod_neg

################################################################################
# void ll_u256_fmod_double(u64 r[4], u64 a[4], u64 p[4]);
.globl	ll_u256_fmod_double
.type	ll_u256_fmod_double,\@function,3
.align	32
ll_u256_fmod_double:
    push	%r12
    push	%r13
    push	%r14
    push	%r15

    mov	8*0($a_ptr), $a0
    xor	$t4,$t4
    mov	8*1($a_ptr), $a1
    add	$a0, $a0		# a0:a3+a0:a3
    mov	8*2($a_ptr), $a2
    adc	$a1, $a1
    mov	8*3($a_ptr), $a3
     mov	$a0, $t0
    adc	$a2, $a2
    adc	$a3, $a3
     mov	$a1, $t5
    adc	\$0, $t4

    sub	8*0($b_ptr), $a0
     mov	$a2, $t2
    sbb	8*1($b_ptr), $a1
    sbb	8*2($b_ptr), $a2
     mov	$a3, $t3
    sbb	8*3($b_ptr), $a3
    sbb	\$0, $t4

    cmovc	$t0, $a0
    cmovc	$t5, $a1
    mov	$a0, 8*0($r_ptr)
    cmovc	$t2, $a2
    mov	$a1, 8*1($r_ptr)
    cmovc	$t3, $a3
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop %r15
    pop %r14
    pop	%r13
    pop	%r12
    ret
.size	ll_u256_fmod_double,.-ll_u256_fmod_double

################################################################################
# void ll_u256_fmod_triple(u64 r[4], u64 a[4], u64 p[4]);
.globl	ll_u256_fmod_triple
.type	ll_u256_fmod_triple,\@function,3
.align	32
ll_u256_fmod_triple:
    push	%r12
    push	%r13
    push	%r14
    push	%r15

    mov	8*0($a_ptr), $a0
    xor	$t4, $t4
    mov	8*1($a_ptr), $a1
    add	$a0, $a0		# a0:a3+a0:a3
    mov	8*2($a_ptr), $a2
    adc	$a1, $a1
    mov	8*3($a_ptr), $a3
     mov	$a0, $t0
    adc	$a2, $a2
    adc	$a3, $a3
     mov	$a1, $t5
    adc	\$0, $t4

    sub	8*0($b_ptr), $a0
     mov	$a2, $t2
    sbb	8*1($b_ptr), $a1
    sbb	8*2($b_ptr), $a2
     mov	$a3, $t3
    sbb	8*3($b_ptr), $a3
    sbb	\$0, $t4

    cmovc	$t0, $a0
    cmovc	$t5, $a1
    cmovc	$t2, $a2
    cmovc	$t3, $a3

    xor	$t4, $t4
    add	8*0($a_ptr), $a0	# a0:a3+=a_ptr[0:3]
    adc	8*1($a_ptr), $a1
     mov	$a0, $t0
    adc	8*2($a_ptr), $a2
    adc	8*3($a_ptr), $a3
     mov	$a1, $t5
    adc	\$0, $t4

    sub	8*0($b_ptr), $a0
     mov	$a2, $t2
    sbb	8*1($b_ptr), $a1
    sbb	8*2($b_ptr), $a2
     mov	$a3, $t3
    sbb	8*3($b_ptr), $a3
    sbb	\$0, $t4

    cmovc	$t0, $a0
    cmovc	$t5, $a1
    mov	$a0, 8*0($r_ptr)
    cmovc	$t2, $a2
    mov	$a1, 8*1($r_ptr)
    cmovc	$t3, $a3
    mov	$a2, 8*2($r_ptr)
    mov	$a3, 8*3($r_ptr)

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    ret
.size	ll_u256_fmod_triple,.-ll_u256_fmod_triple
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
