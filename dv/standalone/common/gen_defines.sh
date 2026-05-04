#!/bin/bash
sed \
       -e 's/`include.*/#include "testBase.h"/' \
       -e 's/`define/#define/g' \
       -e 's/`ifndef/#ifndef/g' \
       -e 's/`ifdef/#ifdef/g' \
       -e 's/`else/#else/g' \
       -e 's/`endif/#endif/g' \
       -e "s/[[:alnum:]_]*\s*'\(b\|d\|h\)//g" \
       -e 's/`//g' \
       -e 's/\$clog2/LOG2/g' \
       -e "/'/d" \
       "$RTLROOT/inc/shire_defines.vh" > vlog_defines.h
