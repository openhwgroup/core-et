#!/usr/bin/perl
use strict;
use warnings;
use List::Util qw(shuffle);

use constant DEFAULT_SEED => 0;
use constant ITERATIONS => 500;
use constant LANES => 8;
use constant MAX_SETS => 64;
use constant FAULT_PROB => 3;

if ( exists $ENV{SEED} ) {
  srand $ENV{SEED};
}
else {
  srand DEFAULT_SEED;
}

my @offsets;
my @masks;
foreach (1..ITERATIONS) {
  my @of;
  
  foreach (1..LANES){
    my $n = sprintf "0x%x", (int rand MAX_SETS) * 0x1000;
    $n="-$n" if ( (int rand FAULT_PROB) == 0);
    push @of, $n;
  }
  push @masks, sprintf "   0x%x", &random_mask;
  push @offsets, "   { " . join (",", @of) . "}";
}


&output(join(",\n", @offsets), join(",\n", @masks));


sub random_mask {
  my $used_lanes = int rand (LANES +1);
  my @b;
  push @b, 1  for (1..$used_lanes);
  push @b, 0  for ($used_lanes+1..LANES);
  @b = shuffle @b;
  my $m=0;
  foreach (@b) {
    $m<<=1;
    $m|=$_;
  }
  return $m;
}
sub output(){
  my ($offsets, $masks) = @_;
  print<<EOF
#include <inttypes.h>

int32_t test_offsets[][8]  __attribute__ ((aligned (32)))  //aligned to 32B (to avoid RTLMIN-2329) =
  {
$offsets
  };

uint8_t test_masks[] __attribute__ ((aligned (32))) =
  {
$masks
  };


uint64_t NR_TESTS = sizeof(test_masks) / sizeof(uint8_t);
EOF
  
}
