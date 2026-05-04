#!/bin/bash


#
# FUNCTIONS
#

#
# A couple of basic aliases to preprend and append items to $PATH without duplicates or sorting.
#
prepend_path() {
    # prepend
    PATH="$*${PATH:+":$PATH"}"

    #and remove duplicates
    export PATH=$(perl -e 'print join(":", grep { not $seen{$_}++ } split(/:/, $ENV{PATH}))')
}

append_path() {
    # append
    PATH="${PATH:+"$*:"}$ARG"

    #and remove duplicates
    export PATH=$(perl -e 'print join(":", grep { not $seen{$_}++ } split(/:/, $ENV{PATH}))')
}

function path_remove()  { export PATH=`echo -n $PATH | awk -v RS=: -v ORS=: '$0 != "'$1'"' | sed 's/:$//'`; }



# set this file's path, made compatible with bash and zsh
if [ -z $BASH_SOURCE ]; then
    ENV_SH_PATH=$0
else
    ENV_SH_PATH=$BASH_SOURCE
fi

# env vars for directories
export REPOROOT=$(dirname `readlink -f $ENV_SH_PATH`)
export TESTROOT=$REPOROOT/test
export MINION_DIAGS=$REPOROOT/dv/tests/minion_core/c_asm_tests
export MINION_REG=$REPOROOT/dv/tests/minion_core/regress_lists/minion.etdv.py
export TESTMINION=$REPOROOT/test/shire/minion
export TESTSHIRE=$REPOROOT/test/shire/shiretop
export TESTSTUB=$REPOROOT/test/shire/shiretop_stub
export TESTRUN=$REPOROOT/test_run_dir
export RTLROOT=$REPOROOT/rtl
export DVTOOLS=$REPOROOT/dv/tools
export SYSEMU=$REPOROOT/extern/et-platform/sw-sysemu
export BEMU=$SYSEMU # TODO: ANT: remove then references are updated
export PYTHONPATH=/usr/bin/python3
export MIN_BIST_DIR=orig
export IOS_BIST_DIR=orig
export ET_PG_RAM_ROM_DIR=

export RISCV_GNU_TOOLCHAIN=/tools/src/riscv-gnu-toolchain
export RISCV_BFD_BUILD=${RISCV_GNU_TOOLCHAIN}/build-binutils-pic
export RISCV=/opt/et
export VCSMXFLOW=0

# Add DV common scripts to path
prepend_path $REPOROOT/dv/common/scripts

# select riscv tools release
# source $TESTROOT/scripts/select_riscv_tools.sh # TODO: docker?

export COMPLIANCE_TESTS=$REPOROOT/extern/riscv-tests-fork/isa


# set path to get riscv tools
if [ -z "$RISCV" ] ; then
    echo "*** Please set environment variable RISCV" 1>&2
    return 1
else
    if [ ! -d $RISCV/bin ]; then
        echo " \$RISCV/bin does not exist. Set RISCV properly" 1>&2
        return 1
    fi
    if which riscv64-unknown-elf-gcc >/dev/null 2>&1 ; then
        currentRiscv=$( dirname `which riscv64-unknown-elf-gcc`)
        if [ $currentRiscv != "$RISCV/bin" ]; then
            prepend_path $RISCV/bin/
        fi
    else
        prepend_path $RISCV/bin/
    fi
fi


# add rtl test scripts to path, if not already in path
if which b4c >/dev/null 2>&1 ; then
    currentB4c=$( dirname `which b4c`)
    if [ $currentB4c != "$TESTROOT/scripts" ]; then
        prepend_path $TESTROOT/scripts/
    fi
else
    prepend_path $TESTROOT/scripts/
fi


# add performance measurement scripts to path, if not already in path
if which log_to_stats.py >/dev/null 2>&1 ; then
    currentLogToPerf=$( dirname `which log_to_stats.py`)
    if [ $currentLogToPerf != "$REPOROOT/tools/dreams2perf" ]; then
        prepend_path $REPOROOT/tools/dreams2perf/
    fi
else
    prepend_path $REPOROOT/tools/dreams2perf/
fi

# add power measurement scripts to path, if not already in path
if which et-PowerFlow.py >/dev/null 2>&1 ; then
    currentPowerFlow=$( dirname `which et-PowerFlow.py`)
    if [ $currentPowerFlow != "$REPOROOT/tools/powerflow" ]; then
        prepend_path $REPOROOT/tools/powerflow/
    fi
else
    prepend_path $REPOROOT/tools/powerflow/
fi


# add metrik script to path, if not already in path
if which metrik.pl >/dev/null 2>&1 ; then
    currentMetrik=$( dirname `which metrik.pl`)
    if [ $currentMetrik != $METRIK ]; then
        prepend_path PATH=$METRIK/
    fi
else
    prepend_path $METRIK/
fi

# add dv tools to path, if not already in path
if which err_dec.py >/dev/null 2>&1 ; then
    currentErrDec=$( dirname `which err_dec.py`)
    if [ $currentErrDec != "$DVTOOLS" ]; then
       prepend_path $DVTOOLS
    fi
else
    prepend_path $DVTOOLS
fi


## # set max nr of parallel jobs for make to # of cpus, if not set
## if [ -z "$MAX_PARALLEL_JOBS" ] ; then
##     MAX_PARALLEL_JOBS=`grep processor /proc/cpuinfo  |wc -l`
## fi

# set max nr of parallel jobs for make to 2 by default
if [ -z "$MAX_PARALLEL_JOBS" ] ; then
    MAX_PARALLEL_JOBS=2
fi
export MAX_PARALLEL_JOBS




# Misc SNPS Stuff to make tools work better
#
export NOVAS_IDLE_LICENSE_CHECKBACK=30  # This is important to not allow anyone to hog licenses
export SLI_LOCAL_MODE=1                 # This solves resource issues checking Licenses
export DW_WAIT_LICENSE=1                # Wait for License
export DISABLE_TDC_DELETED_MSG=1        # Fixes the issue with KDB mismatch bug


#
# Setup VCS independent of cad bash setup
#
#path_remove ${VCS_HOME}/bin
#export VCS_HOME=${cadpath}/tools/snps/vcs/R-2020.12-SP1-1
#prepend_path ${VCS_HOME}/bin

#
# Setup Verilator
#

export VERILATOR_HOME=/tools/verilator
prepend_path ${VERILATOR_HOME}/bin

#
# Setup et-runsyn.pl
#
export RUNSYN_HOME=$REPOROOT/tools/runsyn
prepend_path ${RUNSYN_HOME}

#
# Setup et-interfaces.pl
#
export RUNSYN_HOME=$REPOROOT/tools/interfaces
prepend_path ${RUNSYN_HOME}

dir=$REPOROOT/dv/tools/etdv/bin
if [ -d $dir ] ; then
    prepend_path $dir
fi
unset dir
export ETDV_DEFAULT_CONFIG=$REPOROOT/dv/tests/minion_core/regress_lists/minion.etdv.py

export CCACHE_BASEDIR=$REPOROOT
export CCACHE_NOHASHDIR=true

# set locale, otherwise some scripts parsing tools output might fail
if [ "$LANG" != "en_US.UTF-8" ]; then
    #echo "Warning: locale will be set to 'en_US.UTF-8' instead of '$LANG'" >&2
    export LANG="en_US.UTF-8";
fi

if [ "$LC_ALL" != "en_US.UTF-8" ]; then
    #echo "Warning: locale will be set to 'en_US.UTF-8' instead of '$LC_ALL'" >&2
    export LC_ALL="en_US.UTF-8";
fi

# and configure lib location
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$RISCV/lib
# workaround inconsistent caffe2 dependencies installation between GLOW hosts
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib_new/
# add path to Protobuf (required for Glow build
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/

export REPLACE_XRUN=vcs

# enabling compilation in some servers
#source scl_source enable devtoolset-7 >&2

# use ccache if available
for CCACHE_PATH in /usr/lib64/ccache  /usr/lib/ccache; do
  if [ -d $CCACHE_PATH  -a `which gcc` != $CCACHE_PATH/gcc ] ; then
     prepend_path $CCACHE_PATH
      break
  fi
done

for CCACHE_PATH in /usr/lib64/ccache  /usr/lib/ccache; do
 if [ -d $CCACHE_PATH  -a `which riscv64-unknown-elf-g++` != $CCACHE_PATH/riscv64-unknown-elf-g++ ] ; then
      prepend_path $CCACHE_PATH
      break
  fi
done

#
# add git-hooks
#
ln -snf ../../.githooks/pre-commit $REPOROOT/.git/hooks/pre-commit 2>/dev/null
ln -snf ../../.githooks/pre-push   $REPOROOT/.git/hooks/pre-push   2>/dev/null


#######################################################
# Semifore
#######################################################

# Fix for bash CTRL-C bug
trap - INT QUIT
