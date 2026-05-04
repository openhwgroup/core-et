// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//////////////////////////////////////////////////////////////////
// Global lint file list (only arguments below)
//////////////////////////////////////////////////////////////////

-f $RTLROOT/inc/common_lint.f

//////////////////////////////////////////////////////////////////
// Standard module arguments (only arguments below)
//////////////////////////////////////////////////////////////////

// Hard macro behavioral models
-f $RTLROOT/shire/esr/esr_hm_behav.f

// Primary RTL filelist
-f $RTLROOT/shire/esr/esr_rtl.f

// Top-level module verilog file
// There is not a unique top-level module for all ESRs
// Instead, lint will be run for all of them through a script that
// selects each top-level module 
//$RTLROOT/shire/esr/$mod.v
