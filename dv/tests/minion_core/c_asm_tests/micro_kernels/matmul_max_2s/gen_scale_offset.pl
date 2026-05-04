#!/usr/bin/perl

use warnings;
use strict;
use POSIX;

open (my $fh,  '>:raw', "scales.mem");
for(my $i = 0; $i < 1024; $i++)
{
    my $data0 = 0x81;
    my $data1 = 0x96;
    my $data2 = 0xae;
    my $data3 = 0x39;
    print $fh  map chr, $data0, $data1, $data2, $data3;
}
close $fh;

open (my $fh2, '>:raw', "offsets.mem");
for(my $i = 0; $i < 1024; $i++)
{
    my $data0 = 0x24;
    my $data1 = 0x01;
    my $data2 = 0x0;
    my $data3 = 0x0;
    print $fh2 map chr, $data0, $data1, $data2, $data3;
}
close $fh2;

