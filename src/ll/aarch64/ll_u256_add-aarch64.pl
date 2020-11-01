$flavour = shift;
$output  = shift;

$0 =~ m/(.*[\/\\])[^\/\\]+$/; $dir=$1;
( $xlate="${dir}arm-xlate.pl" and -f $xlate ) or
( $xlate="${dir}../perlasm/arm-xlate.pl" and -f $xlate) or
die "can't locate arm-xlate.pl";

open OUT,"| \"$^X\" $xlate $flavour $output";
*STDOUT=*OUT;

$code.=<<___;
.text
___

{
my($rd,$ad,$bd)=("x15","x1","x2");
my($a0,$a1,$a2,$a3,$b0,$b1,$b2,$b3)=map("x$_",(3..10));
my($r0,$r1,$r2,$r3)=map("x$_",(11..14));

$code.=<<___;
// u64 ll_u256_add(u64 rd[4], const u64 ad[4], const u64 bd[4]);
.globl	ll_u256_add
.type	ll_u256_add,%function
.align	5
ll_u256_add:
    ldp $a0,$a1,[$ad]
    mov $rd,x0
    ldp $b0,$b1,[$bd]
    adds $r0,$a0,$b0      // r0 = a0 + b0
    ldp $a2,$a3,[$ad,#16]
    adcs $r1,$a1,$b1      // r1 = a1 + b1
    ldp $b2,$b3,[$bd,#16]
    adcs $r2,$a2,$b2      // r2 = a2 + b2
    stp $r0,$r1,[$rd]
    adcs $r3,$a3,$b3      // r3 = a3 + b3
    stp $r2,$r3,[$rd,#16]
    adc x0,xzr,xzr        // carry
    ret
.size	ll_u256_add,.-ll_u256_add


// u64 ll_u256_sub(u64 rd[4], const u64 ad[4], const u64 bd[4]);
.globl	ll_u256_sub
.type	ll_u256_sub,%function
.align	5
ll_u256_sub:
    ldp $a0,$a1,[$ad]
    mov $rd,x0
    ldp $b0,$b1,[$bd]
    subs $r0,$a0,$b0      // r0 = a0 - b0
    ldp $a2,$a3,[$ad,#16]
    sbcs $r1,$a1,$b1      // r1 = a1 - b1
    ldp $b2,$b3,[$bd,#16]
    sbcs $r2,$a2,$b2      // r2 = a2 - b2
    stp $r0,$r1,[$rd]
    sbcs $r3,$a3,$b3      // r3 = a3 - b3
    stp $r2,$r3,[$rd,#16]
    adc x0,xzr,xzr        // borrow
    ret
.size	ll_u256_sub,.-ll_u256_sub
___
}

print $code;

close STDOUT;