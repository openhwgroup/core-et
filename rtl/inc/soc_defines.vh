// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _SOC_DEFINES_
`define _SOC_DEFINES_

// soc define container for all define files
// all define files must be included in this file
// 
// NOTE: all RTL files other than lib files should include soc_defines.vh and only soc_defines.vh

`include "assert.vh"
`include "evl_wire.vh"
`include "macros.vh"
`include "macros_legacy.vh"
`include "axi_defines.vh"
`include "cmesh_defines.vh"
`include "shire_assert.vh"
`include "shire_cache_defines.vh"
`include "shire_defines.vh"
`include "et_link_defines.vh"
`include "debug_defines.vh"
`include "debug_pshire_defines.vh"
`include "minion_defines.vh"
`include "csr_defines_auto.vh"
`include "csr_defines.vh"
`include "dcache_defines.vh"
`include "esr_defines_auto.vh"
`include "esr_defines.vh"
`include "esr_rvtimer_types.vh" 
`include "fp_defines.vh"
`include "frontend_defines.vh"
`include "instructions.vh"
`include "libs_defines.vh"
`include "rbox_defines.vh"
`include "tbox_defines.vh"
`include "log2.vh"
`include "memshire_defines.vh"
`include "mesh_defines.vh"
`include "trans_defines.vh"
`include "txfma_defines.vh"
`include "uc_defines.vh"
`include "vpu_defines.vh"
`include "tima_defines.vh"
`include "ioshire_defines.vh"
`include "ioshire_plic_defines.vh"
`include "ioshire_tilelink_defines.vh"
`include "pshire_defines.vh"
`include "soctop_defines.vh"
`include "sbm_defines.vh"
`include "standalone_minion_defines.vh"
`include "pmu_defines.vh"
`include "maxshire_defines.vh"
`include "pvt_defines.vh"
`include "dft_defines.vh"
`include "cpu_defines.vh"
`endif // __SOC_DEFINES__
