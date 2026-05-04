#!/usr/bin/perl

# This script gets a Glow IR description file and computes TOPS required per inference

use warnings;
use strict;

# Gets the entry point
my $result = `grep _Z16test_entry_pointv build/net_test.S | grep text`;

$result =~ m/^(\S*)/;
my $pc = hex $1;
printf "PC is %010llx\n", $pc;

# Writes binary data in txt
my $f;
open($f, ">", "net_desc.txt") or die "Couldn't open net_desc.txt for writing\n";

print $f "0000000: 0100 0000 0000 0000 0300 0000 0000 0000  ........ .......\n";
print $f "0000010: 0000 0000 0000 0000 0000 0000 0000 0000  ................\n";
print $f "0000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................\n";
print $f "0000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................\n";
printf $f "0000040: %02x%02x %02x%02x %02x00 0000 0100 0000 0000 0000  .. ....... .....\n", ($pc & 0xFF), ($pc >> 8) & 0xFF, ($pc >> 16) & 0xFF, ($pc >> 24) & 0xFF, ($pc >> 32) & 0xFF;
print $f "0000050: ffff ffff 0000 0000 ffff ffff 0000 0000  ................\n";
print $f "0000060: 0100 0000 0000 0000 0000 0080 8000 0000  ................\n";
print $f "0000070: 0000 8080 8000 0000 0000 b080 8000 0000  ................\n";
print $f "0000080: 0000 a080 8000 0000 0000 9080 8000 0000  ................\n";
print $f "0000090: 0f00 0000 0000 0000 0d00 0000 0000 0000  ................\n";
print $f "00000a0: 2800 0040 8000 0000 0a                   (..@.....\n";

close($f);

# Converts it to mem
system "xxd -r net_desc.txt > net_desc.mem";
system "rm -rf net_desc.txt";

# Done
exit 0;

