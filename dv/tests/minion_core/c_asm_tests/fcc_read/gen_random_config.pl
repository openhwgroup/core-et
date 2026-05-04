#!/usr/bin/perl
use strict;
use warnings;

use constant NR_COUNTERS => 2;
use constant NR_THREADS => 2;
use constant MAX_CREDITS => 16;

die "Usage: $0 NR_ITERS" unless @ARGV==1;

my $seed=0;
$seed = $ENV{SEED} if exists $ENV{SEED};
srand $seed;

print <<HEADER
#ifndef _FCC_READ_H_
#define _FCC_READ_H_
#define NR_COUNTERS 2
#define ITERATIONS sizeof(test_config)/sizeof(iter_config_t)

typedef struct {
  int counter;
  uint64_t producer;
  uint64_t nr_credits;
}iter_config_t;

static iter_config_t test_config [] = {
HEADER
  ;

my @config;
for(1..$ARGV[0]){
  my $cnt = int rand NR_COUNTERS;
  my $producer = int rand NR_THREADS;
  my $nr_credits = 1 + int rand (MAX_CREDITS-1);
  push @config, "{ $cnt, $producer, $nr_credits}";
}

print join",\n", @config;

print <<EOF

};
#endif
EOF
  
