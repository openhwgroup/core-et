// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//////////////////////////////////////////////////////////////////
// Global synthesis file list (only arguments below)
//////////////////////////////////////////////////////////////////

-f $RTLROOT/inc/common_syn.f

//////////////////////////////////////////////////////////////////
// Standard module arguments (only arguments below)
//////////////////////////////////////////////////////////////////
+define+CDC_ANALYSIS // ANT: Enabling this to allow syntesis with gen_mem1p. Waiting for new memories. soc1 memories are not available
      
// Primary RTL filelist
-f $RTLROOT/cpu_subsystem/cpu_subsystem_top_rtl.f

// Top-level module verilog file
$RTLROOT/cpu_subsystem/cpu_subsystem_top.v
