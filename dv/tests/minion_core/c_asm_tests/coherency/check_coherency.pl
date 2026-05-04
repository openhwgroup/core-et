#!/usr/bin/perl

use warnings;
use strict;

my @write_tests = <write*>;

# For all the write tests
foreach(@write_tests)
{
  # Moves to test dir
  my $test = $_;
  chdir $test or die "Couldn't go to directory $test!!\n";

  # Executes test
  print "Executing test $test...";
  system "make clean > /dev/null";
  my @res = `./run_sys_emu.sh`;

  # Checks if write hazard was found
  my $hazard_found = 0;
  foreach(@res)
  {
    if(m/Coherency Write Hazard/)
    {
      $hazard_found = 1;
      last;
    }
  }

  if($hazard_found == 1)
  {
    print " OK!!\n";
  }
  else
  {
    print " ERROR!!! Hazard not detected!!\n";
    exit -1;
  }

  system "make clean > /dev/null";

  # Goes back to base dir
  chdir "..";
}

exit 0;

