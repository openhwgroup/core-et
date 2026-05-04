// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//////////////////////////////////////////////////////////////////
// Global simulation file list (only arguments below)
//////////////////////////////////////////////////////////////////

-f $RTLROOT/inc/common_vcs.f

//////////////////////////////////////////////////////////////////
// Standard module arguments (only arguments below)
//////////////////////////////////////////////////////////////////

// Defines for simulation
-f $RTLROOT/cpu_subsystem/cpu_subsystem_top_vcs_defines.f

// Hard macro behavioral models
-f $RTLROOT/cpu_subsystem/cpu_subsystem_top_hm_behav.f

// Primary RTL filelist
-f $RTLROOT/cpu_subsystem/cpu_subsystem_top_rtl.f
