// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// SOC_TYPES - this module should not contain ifndef protection
// because it is required to be read in by DV in multiple locations
// 
// These includes are ifndef protected on the RTL side within soc.vh

`include "axi_types.vh"
`include "esr_types_auto.vh"
`include "minion_types.vh"  // NOTE: dcache/front_end use structs from this types file
`include "et_link_types.vh" // NOTE: debug_types use structs from this types file
`include "cmesh_types.vh"
`include "csr_types.vh"
`include "csr_types_auto.vh"     
`include "dcache_types.vh"
`include "esr_types_legacy.vh" 
`include "fp_types.vh"
`include "vpu_types.vh"
`include "frontend_types.vh"
`include "memshire_types.vh"
`include "mesh_types.vh"
`ifdef GFX_ENABLED
  `include "rbox_types.vh"
`endif
`include "shire_cache_types.vh"
`include "shire_types.vh"
`include "trans_types.vh"
`include "uc_types.vh"
`include "sbm_types.vh"
`include "ioshire_types.vh"
`include "tbox_types.vh"
`include "tbox_usoc_types.vh"
`include "pmu_types.vh"
`include "debug_types.vh"
`include "maxshire_types.vh"
`include "cpu_types.vh"
`include "cpu_clint_types.vh" 
