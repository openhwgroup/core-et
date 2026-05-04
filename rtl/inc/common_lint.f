// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// Common lint file list that should be included by all *_lint.f file lists

+libext+.v+.sv+.vh+.svh+.vg+.svp

+define+ET_ASSERT_OFF
+define+ET_ASCENT_LINT
// ET_LIBS_USE_STD_CELLS indicates to use manually instantiated standard cells
// not generic behavioral models.  Note this is set for lint and standard cell
// behavioral models should be waived, blackboxed, or linted (if clean)
// +define+ET_LIBS_USE_STD_CELLS // IGA: standard cells not available
// ET_USE_ASIC_MEMS indicates to use physical SRAM macros not generic behavioral models
// SRAMs should be blackboxed by the lint tool
+define+ET_USE_ASIC_MEMS
// Netspeed NoC replace latches for CG library cells
+define+GATES

//////////////////////////////////////////////////////////////////
// Common defines (these should be defined in all common_*.f files)
//////////////////////////////////////////////////////////////////

