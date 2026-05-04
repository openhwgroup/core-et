#!/usr/bin/perl
use strict;
use warnings;

die "Usage: $0 outfile size\n" unless @ARGV==2;

my $outFile=$ARGV[0];
my $size=parseSize($ARGV[1]);

srand($ENV{SEED}) if exists $ENV{SEED};

open (F,">$outFile") or die "cannot create $outFile";
binmode F;
foreach (1..$size) {
  print F pack 'C', int(rand(256))
}
close F;

sub parseSize{
  my $s=$_[0];
  if ( $s=~/^(\d+)(M|K)?$/) {
    my $res = $1;
    if (defined $2) {
      if ($2 eq "M"){ $res*=1024*1024;}
      else { $res*=1024};
    }
    return $res;
  }
  else {
    die "bad size $s\n";
  }
}
