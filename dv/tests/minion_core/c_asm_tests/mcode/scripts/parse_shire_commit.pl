#!/usr/bin/perl

use warnings;
use strict;

my $check_next_inst=0;
my $test_fail=0;

# Runs through all the PCs
while(<STDIN>)
{
  if ($check_next_inst eq 1) { 
    if ($_ =~ /(.*)wfi(.*)/) {
      $test_fail=1;
      last;   
    }  
    else {
      $check_next_inst=0;
    } 
  } 
  if ($_ =~ /(.*)ori(.*)/) {
    $check_next_inst=1;
  }
}

if ($test_fail eq 1) {
  print "TEST ERROR: ori+wfi signature\n";
}
