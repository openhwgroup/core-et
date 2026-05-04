// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Common synthesis file list that should be included by all *_syn.f file lists

+libext+.v+.sv+.vh+.svh+.vg+.svp

+define+SYNTHESIS
// ET_LIBS_USE_STD_CELLS indicates to use manually instantiated standard cells
// not generic behavioral models
// +define+ET_LIBS_USE_STD_CELLS // IGA: standard cells not available
// ET_USE_ASIC_MEMS indicates to use physical SRAM macros not generic behavioral models
//+define+ET_USE_ASIC_MEMS // ANT: Synopsis memories not available
// Netspeed NoC replace latches for CG library cells
+define+GATES

//////////////////////////////////////////////////////////////////
// Common defines (these should be defined in all common_*.f files)
//////////////////////////////////////////////////////////////////

