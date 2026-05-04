#!/usr/bin/perl
use strict;
use warnings;
my $count=128*64;
print "#include <inttypes.h>\nuint32_t texture[$count] = {";
foreach my $i(0..$count-1) {
  print "\n  " if $i%8 == 0;
  my $v=(int rand 256) | (0xFA<<8) | (0x55<<(16+$i%2)) | (0xF0<<24);
  printf "0x%.8x", $v;
  print ", " unless $i==$count-1;
}
print "};\n;"
