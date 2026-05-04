#!/usr/bin/perl
use strict;
use warnings;
use List::Util qw(shuffle);

use constant ITERATIONS => 5000;
use constant FREQ_JUMP  => 4;
use constant FREQ_LOAD  => 4;
use constant FREQ_XCPT  => 4;
use constant FREQ_MULTILOAD => 3;
use constant MAX_TAGS   => 5; # loop using 5 different tags => to force misses in L1


my $seed = 0;
$seed = $ENV{SEED} if exists $ENV{SEED};
srand $seed;

my @mul_insn = qw/mul mulh mulhsu mulhu div divu rem remu mulw divw remw remuw/;
my @insns;
my $load_tag = 0;

foreach my $i (@mul_insn){
  push @insns, $i if !exists $ENV{"MASK_$i"} || $ENV{"MASK_$i"} == 0 
}
die "no non-masked mul div insn" if @insns == 0;

&test_start();
foreach my $it(1..ITERATIONS) {
  my @codes;
  print "\n// iteration $it\n";

  &add_load(\@codes, FREQ_LOAD, FREQ_MULTILOAD);
  &add_jump(\@codes, FREQ_JUMP);
  &add_xcpt(\@codes, FREQ_XCPT);
  &rand_mul_div(\@codes);
  
  @codes = shuffle(@codes);
  print "$_" foreach @codes;
}

&test_end();

sub rand_reg {
  # x1..xMAX_TAGS are reserved
  return "x". (MAX_TAGS +1 + int rand (32 - MAX_TAGS-1));

}
sub rand_mul_div {
  my $code=shift;
  my $rd  = &rand_reg();
  my $rs0 = &rand_reg();
  my $rs1 = &rand_reg();
  my $insn = $insns[rand int scalar @insns];
  push @$code, "$insn $rd, $rs0, $rs1\n";
}

sub add_jump {
  my ($code, $freq)=@_;
  return unless (int rand $freq)  == 0;
  my $str=<<EOF
// jump
j 1f
1:
EOF
    ;
    push @$code, $str;
}

sub add_xcpt {
  my ($code, $freq)=@_;
  return unless (int rand $freq)  == 0;
  my $str= <<EOF
// access to unexisting CSR => xcpt
csrr x0, 0xFFF
EOF
    ;
    push @$code, $str;
}



sub add_load {
  my ($code, @freq)=@_;
  return unless (int rand $freq[0])  == 0;  
  my $n_loads = 1;
  $n_loads = 1+ int rand 4 if  (int rand $freq[1])  == 0;  
  my $str="";
  for ( 1..$n_loads){ 
    $str.="// load from tag $load_tag\n";
    my $offset = int rand (64-8);
    my $rd  = &rand_reg();
    my $rs = "x". ($load_tag+1);
    $str.="ld $rd, $offset ($rs)\n";
    $load_tag++;
    $load_tag%=MAX_TAGS;
  }
  push @$code, $str;
}

sub test_end{
  print <<EOF
  ASM_TEST_PASS
end_loop:
    j end_loop

.align 12
mtrap_vector:
    // do nothing, just return
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0
    mret


.data
load_destination:
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
EOF

}


sub test_start{
  print <<EOF
#include "macros.h"

.text
.global _start
_start: 
    ASM_TEST_START

    //configure mtvec and stvec
    la t0, mtrap_vector
    csrw mtvec, t0

   // setup load addresses
   la x1, load_destination
EOF
    ;
  foreach my $tag(1..MAX_TAGS-1){
    my $next_tag = $tag+1;
    print "  addi x$next_tag, x$tag, 0x400 \n";
  }
}
