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
my ($r_ptr,$a_ptr,$b_org)=("%rdi","%rsi","%rdx");
my ($t0,$t1,$t2)=("%r9","%r10","%r11");
my ($a0,$a1,$a2,$a3,$a4)=("%rax","%r8","%r9","%r10","%r11");

$code.=<<___;
# void ll_u256_lshift(u64 r[5], u64 a[4], size_t n);
.globl	ll_u256_lshift
.type	ll_u256_lshift,\@function,3
.align	32
ll_u256_lshift:
    mov $b_org, %rcx
    xor $a4, $a4
    mov   8*3($a_ptr), $a3
    mov   8*2($a_ptr), $a2
    shldq %cl, $a3, $a4          # r[4] = a[3] >> (64-n)
    mov   8*1($a_ptr), $a1
    shldq %cl, $a2, $a3          # r[3] = (a[2] >> (64-n)) | (a[3] << n)
    mov $a4, 8*4($r_ptr)
    mov   8*0($a_ptr), $a0
    shldq %cl, $a1, $a2          # r[2] = (a[1] >> (64-n)) | (a[2] << n)
    mov $a3, 8*3($r_ptr)
    mov $a2, 8*2($r_ptr)
    shldq %cl, $a0, $a1          # r[1] = (a[0] >> (64-n)) | (a[1] << n)
    shlq %cl, $a0                # r[0] = a[0] << n
    mov $a1, 8*1($r_ptr)
    mov $a0, 8*0($r_ptr)
    ret
.size	ll_u256_lshift,.-ll_u256_lshift

# void ll_u256_rshift(u64 r[4], u64 a[4], size_t n);
.globl	ll_u256_rshift
.type	ll_u256_rshift,\@function,3
.align	32
ll_u256_rshift:
    mov $b_org, %rcx
    mov   8*0($a_ptr), $a0
    mov   8*1($a_ptr), $a1
    mov   8*2($a_ptr), $a2
    shrdq %cl, $a1, $a0          # r[0] = (a[0] >> n) | (a[1] << (64-n))
    mov   8*3($a_ptr), $a3
    shrdq %cl, $a2, $a1          # r[1] = (a[1] >> n) | (a[2] << (64-n))
    mov $a0, 8*0($r_ptr)
    shrdq %cl, $a3, $a2          # r[2] = (a[2] >> n) | (a[3] << (64-n))
    mov $a1, 8*1($r_ptr)
    shrq %cl, $a3                # r[3] = a[3] >> n
    mov $a2, 8*2($r_ptr)
    mov $a3, 8*3($r_ptr)
    ret
.size	ll_u256_rshift,.-ll_u256_rshift


# void ll_u512_lshift(u64 r[9], u64 a[8], size_t n);
.globl	ll_u512_lshift
.type	ll_u512_lshift,\@function,3
.align	32
ll_u512_lshift:
    mov $b_org, %rcx
    xor $a4, $a4
    mov   8*7($a_ptr), $a3
    mov   8*6($a_ptr), $a2
    shldq %cl, $a3, $a4          # r[8] = a[7] >> (64-n)
    mov   8*5($a_ptr), $a1
    shldq %cl, $a2, $a3          # r[7] = (a[6] >> (64-n)) | (a[7] << n)
    mov $a4, 8*8($r_ptr)
    mov   8*4($a_ptr), $a0
    shldq %cl, $a1, $a2          # r[6] = (a[5] >> (64-n)) | (a[6] << n)
    mov $a3, 8*7($r_ptr)
    mov $a2, 8*6($r_ptr)
    shldq %cl, $a0, $a1          # r[5] = (a[4] >> (64-n)) | (a[5] << n)
    mov   8*3($a_ptr), $a3
    mov $a1, 8*5($r_ptr)
    shldq %cl, $a3, $a0          # r[4] = (a[3] >> (64-n)) | (a[4] << n)
    mov   8*2($a_ptr), $a2
    mov $a0, 8*4($r_ptr)
    shldq %cl, $a2, $a3          # r[3] = (a[2] >> (64-n)) | (a[3] << n)
    mov   8*1($a_ptr), $a1
    mov $a3, 8*3($r_ptr)
    shldq %cl, $a1, $a2          # r[2] = (a[1] >> (64-n)) | (a[2] << n)
    mov   8*0($a_ptr), $a0
    mov $a2, 8*2($r_ptr)
    shldq %cl, $a0, $a1          # r[1] = (a[0] >> (64-n)) | (a[1] << n)
    shlq %cl, $a0                # r[0] = a[0] << n
    mov $a1, 8*1($r_ptr)
    mov $a0, 8*0($r_ptr)
    ret
.size	ll_u512_lshift,.-ll_u512_lshift

# void ll_u512_rshift(u64 r[8], u64 a[8], size_t n);
.globl	ll_u512_rshift
.type	ll_u512_rshift,\@function,3
.align	32
ll_u512_rshift:
    mov $b_org, %rcx
    mov   8*0($a_ptr), $a0
    mov   8*1($a_ptr), $a1
    mov   8*2($a_ptr), $a2
    shrdq %cl, $a1, $a0          # r[0] = (a[0] >> n) | (a[1] << (64-n))
    mov   8*3($a_ptr), $a3
    shrdq %cl, $a2, $a1          # r[1] = (a[1] >> n) | (a[2] << (64-n))
    mov $a0, 8*0($r_ptr)
    shrdq %cl, $a3, $a2          # r[2] = (a[2] >> n) | (a[3] << (64-n))
    mov   8*4($a_ptr), $a0
    mov $a1, 8*1($r_ptr)
    shrdq %cl, $a0, $a3          # r[3] = (a[3] >> n) | (a[4] << (64-n))
    mov   8*5($a_ptr), $a1
    mov $a2, 8*2($r_ptr)
    shrdq %cl, $a1, $a0          # r[4] = (a[4] >> n) | (a[5] << (64-n))
    mov   8*6($a_ptr), $a2
    mov $a3, 8*3($r_ptr)
    shrdq %cl, $a2, $a1          # r[5] = (a[5] >> n) | (a[6] << (64-n))
    mov   8*7($a_ptr), $a3
    mov $a0, 8*4($r_ptr)
    shrdq %cl, $a3, $a2          # r[6] = (a[6] >> n) | (a[7] << (64-n))
    mov $a1, 8*5($r_ptr)
    shrq %cl, $a3                # r[7] = a[7] >> n
    mov $a2, 8*6($r_ptr)
    mov $a3, 8*7($r_ptr)
    ret
.size	ll_u512_rshift,.-ll_u512_rshift
___
}

$code =~ s/\`([^\`]*)\`/eval $1/gem;
print $code;
close STDOUT or die "error closing STDOUT: $!";
