#!/usr/bin/perl
use strict;
use warnings;
use bigint;
my %rec;
print "### Checking that all test functions are exactly 512B:\n";
while(<>){
  chomp;
  my ($ad, $t, $name)=split;
  $rec{$name} = hex $ad;
}

my $err = 0;
foreach my $n (keys %rec){
  if (exists $rec{"end_of_$n"}){
    my $size = $rec{"end_of_$n"}- $rec{$n};
    print "$n => size = $size";
    if ($size == 512) { print "\n"}
    else{
      print " ERROR!\n";
      $err = 1;
    }
  }
}

exit $err;
