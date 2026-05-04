#!/usr/bin/perl
use strict;
use warnings;

use constant MAX_SETS => 8;
$\="\n";
my $maxSets=MAX_SETS;


my $iterations=10000;
srand($ENV{SEED}) if exists $ENV{SEED};
my $t1_start="lab_". int($iterations/2);
print <<EOM
#define MAX_SETS $maxSets
char data[64*16*MAX_SETS] __attribute__ ((aligned (64)));

int getThreadId() {
    int  thread;
    __asm__ __volatile__
      (
       "  csrr %[tid], mhartid\\n" // software thread
       : [tid] "=r" (thread) );
    return thread & 1;
}

int main(){
  char *ptr[MAX_SETS];
  long long int d;
  for( int i = 0; i< MAX_SETS; i++)  ptr[i] = &(data[64*16*i]);

  while(true) {
    if ( getThreadId() != 0)
      __asm__ __volatile__ ( "jal zero, $t1_start\\n");
EOM
  ;

my %funcs = (
  fptoint_1 => {func=> \&fptoint_1 , weight => 1},
  fptoint_2 => {func=> \&fptoint_2 , weight => 1},
  load      => {func=> \&load      , weight => 1},
  store     => {func=> \&store     , weight => 1},
  add       => {func=> \&add       , weight => 1},
  jump      => {func=> \&jump      , weight => 1}
);

foreach my $x(keys %funcs){
  my $e="WEIGHT_". uc($x);
  $funcs{$x}->{weight} = $ENV{$e} if exists $ENV{$e};
  print STDERR "$e => $funcs{$x}->{weight}";
}
my @bag;

foreach my $x(keys %funcs){
  foreach (0..$funcs{$x}->{weight}-1){
    push @bag, $funcs{$x}->{func};
  }
}

foreach my $current_iter(0.. $iterations -1 ) {
  $bag[rand @bag] -> ($current_iter);
}

print "  }\n}";



sub fptoint_1{
  my $current_iter=shift;
  my $freg = int rand 32;
  print "    __asm__ __volatile__ ( \"lab_$current_iter: fcvt.w.s %[dummy], f$freg\\n\" : [dummy] \"=r\" (d): : );";
}

sub fptoint_2{
  my $current_iter=shift;
  print "    __asm__ __volatile__ ( \"lab_$current_iter: mova.x.m  %[dummy]\\n\" : [dummy] \"=r\" (d));";
}

sub load{
  my $current_iter=shift;
  my $src=int rand MAX_SETS;
  print "    __asm__ __volatile__ ( \"lab_$current_iter: ld %[dummy], 0(%[src])\\n\" :[dummy] \"=r\" (d) : [src] \"r\" (ptr[$src]) :);";
}

sub store{
  my $current_iter=shift;
  my $src=int rand MAX_SETS;
  print "    __asm__ __volatile__ ( \"lab_$current_iter: sd %[dummy], 0(%[src])\\n\" :[dummy] \"=r\" (d) : [src] \"r\" (ptr[$src]) :);";
}


sub add{
  my $current_iter=shift;
    print "    __asm__ __volatile__ ( \"lab_$current_iter:  add  %[dummy], %[dummy], zero\\n\" : [dummy] \"=r\" (d));";
}


sub jump {
  my $current_iter=shift;
  my $pos=int rand $iterations;
  $pos=int rand $iterations while ($current_iter==$pos);
  print "    __asm__ __volatile__ ( \"lab_$current_iter:  jal zero, lab_$pos\\n\");";
}
