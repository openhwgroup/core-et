#!/usr/bin/perl
use strict;
use warnings;
use POSIX qw/floor ceil/;
use List::Util qw[min max];
use Getopt::Long;

###################################################################################
##
## Options
##
###################################################################################
my $debug    = 0;
GetOptions ( 

              "dbg=i"  => \$debug,   
           ) or die("Error in command line arguments\n");


my $REPOROOT = $ENV{REPOROOT};
$REPOROOT || die "need the REPOROOT environment variable set";

my $CASM = "$REPOROOT/dv/tests/minion_core/c_asm_tests";
-d $CASM || die "Directory $CASM does not seem to exist";

my @kernels  = ( 
    { name => "l3_mixed",          dir => "$CASM/micro_kernels", uses => "ONE_MINION" },
    { name => "l3_read_miss",      dir => "$CASM/micro_kernels", uses => "ONE_THREAD" },
    { name => "gfx_ops_disabled",  dir => "$CASM/minion_feature_esr", uses => "ONE_THREAD_T0", rename => ['err'] },
    #{ name => "meip",              dir => "$CASM",                    rename => [ 'mtrap_vector', 'irq_cnt', 'irq_handler'] },
);

my @kernels_fc  = ( 
    { name => "sparse_nn",         dir => "$CASM/micro_kernels", uses => "ALL_SHIRES" },
    { name => "matmul_max_fc",     dir => "$CASM/micro_kernels", uses =>  "ALL_SHIRES_T0" },
);

sub domake
{
    my ($k) = @_;

    # Build the full directory name
    my $dir = "$k->{dir}/$k->{name}";
    -d $dir || die "Sorry, directory $dir does not seem to exist: $!";

    # Extract the kernel name
    my $name = $k->{name};

    # For every rename word, build a -D<oldname>=<oldname>_<name> that we will pass to the 
    # makefile through EXTRA_FLAGS. Notice we ALWAYS unconditionally rename the "main" function
    my $extra_cflags = "";
    foreach my $r ( "main", @{$k->{rename}} )
    {
        $extra_cflags .= "-D$r=${r}_$name ";
    }
    $extra_cflags = "EXTRA_CFLAGS=\"$extra_cflags\"" if ( $extra_cflags );

    # Invoke remote makefile
    system("make -C $dir $name.o $extra_cflags");
    
    # Copy all objects produced and remember them
    system ("mv $dir/*.o .");

    # Clean up :remote directory
    system("make -C $dir clean");
}

my $fake = 1;

sub create_func
{
    my ( $name ) = @_;
    if ( $fake )
    {
        print "int main_$name() { printf(\"hi from main_$name\"); return 0; }\n";
    }
    else
    {
        print "extern int $name();\n"
    }
}

sub create_table
{
    my ( $tname, $dname, @karray ) = @_;

    my $count = $#karray + 1;

    print "#define $dname $count\n";
    print "const kernel_info_t  $tname\[$dname+1\] = {\n";
    foreach my $k ( @karray )
    {
        my $uses = $k->{uses};
        $uses = $uses ? "USES_" . $uses : "USES_UNKNOWN";
        print "  { main_$k->{name}, \"main_$k->{name}\", $uses },\n";
    }
    print "  { 0, \"\", 0 }\n";
    print "};\n";
}


open(KH, ">kernels.h") || die "Can't write kernels.h";
select KH;

    print "#include <stdio.h>\n" if ( $fake );
    foreach my $k ( @kernels )
    {
        domake($k) if ( $fake == 0 );
    }

    # One external declaration per kernel
    foreach my $k ( @kernels, @kernels_fc )
    {
        create_func($k->{name});
    }

    create_table("kernels", "NKERNELS", @kernels);
    create_table("kernels_fc", "NKERNELS_FC", @kernels_fc);
close KH;
