// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// NOTE: All defines should be global and be included in $RTLROOT/inc/common_lint.f
//       Enable here only for testing
//+define+MINION_USE_MMI_TXFMA

//////////////////////////////////////////////////////////////////
// Global lint file list (only arguments below)
//////////////////////////////////////////////////////////////////

-f $RTLROOT/inc/common_lint.f

//////////////////////////////////////////////////////////////////
// Standard module arguments (only arguments below)
//////////////////////////////////////////////////////////////////

// Hard macro behavioral models
-f $RTLROOT/shire/minion/vpu/txfma_hm_behav.f

// Primary RTL filelist
-f $RTLROOT/shire/minion/vpu/txfma_rtl.f

// Top-level module verilog file
$RTLROOT/shire/minion/vpu/txfma_7s/txfma_top.v
