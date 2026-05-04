
`ifndef __EVL_SHIRE_CACHE_TYPES_PKG

`define __EVL_SHIRE_CACHE_TYPES_PKG 1

`include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"
`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"


//-----------------------------------------------------------------------------------------------
//
//
//
package evl_shire_cache_types_pkg;
  import evl_etlink_types_pkg::*;

  `include "rtl/inc/esr_types_auto.vh"
  `include "rtl/inc/esr_types_legacy.vh"
  `include "rtl/inc/shire_cache_types.vh"

  typedef sc_err_rsp_t evl_sc_pipe_err_rsp_t;
  typedef sc_reqq_opcode_t evl_sc_reqq_opcode_t;
  typedef sc_reqq_orig_opcode_t evl_sc_reqq_orig_opcode_t;
  typedef sc_address_space_t evl_sc_address_space_t;

  //
  // ESR structure definitions
  //
  typedef esr_sc_err_log_ctl_t evl_esr_sc_err_log_ctl_t;
  typedef esr_sc_l3_shire_swizzle_ctl_t evl_esr_sc_l3_shire_swizzle_ctl_t;
  typedef esr_sc_pipe_cache_ctl_t evl_esr_sc_pipe_cache_ctl_t;
  typedef esr_sc_pipe_ctl_t evl_esr_sc_pipe_ctl_t;
  typedef esr_sc_reqq_ctl_t evl_esr_sc_reqq_ctl_t;
  typedef esr_sc_trace_ctl_t evl_esr_sc_trace_ctl_t;
endpackage : evl_shire_cache_types_pkg

`endif  // !__EVL_SHIRE_CACHE_TYPES_PKG
