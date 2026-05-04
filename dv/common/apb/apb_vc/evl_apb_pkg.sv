
`ifndef __EVL_APB_PKG

   `define __EVL_APB_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   `define EVL_APB_REQ_TYPE_BITS   1
   `define EVL_APB_REQ_TYPE_MSB   (`EVL_APB_REQ_TYPE_BITS - 1)
   `define EVL_APB_READ           (`EVL_APB_REQ_TYPE_BITS'd0)
   `define EVL_APB_WRITE          (`EVL_APB_REQ_TYPE_BITS'd1)

   `define EVL_APB_RESP_TYPE_BITS  1
   `define EVL_APB_RESP_TYPE_MSB  (`EVL_APB_RESP_TYPE_BITS - 1)
   `define EVL_APB_RESP_OKAY      (`EVL_APB_RESP_TYPE_BITS'd0)
   `define EVL_APB_RESP_SLVERR    (`EVL_APB_RESP_TYPE_BITS'd1)

   `ifndef EVL_ISSUE_DUT_ERROR_ON_APB_ERR_RSP_DEFAULT
      `define EVL_ISSUE_DUT_ERROR_ON_APB_ERR_RSP_DEFAULT 0
   `endif


   //-----------------------------------------------------------------------------------------------
   //
   // Although this may seem a bit redundant and confusing, the RTL uses types.  These types are
   // defined in $root, which is not accessible to packages.  As a result, we need to create a
   // package with the same type definitions.  To prevent types from being defined twice in the
   // interface files (which have access to $root), we define new EVL types based off of RTL types
   // on an as-needed basis.  These new EVL types can be imported by the interface files as needed.
   //
   package evl_apb_types_pkg;
      `include "rtl/inc/et_link_types.vh"
      `include "rtl/inc/shire_types.vh"

      typedef APB_to_bank_ESRs_t   evl_APB_to_bank_ESRs_t;
      typedef APB_from_bank_ESRs_t evl_APB_from_bank_ESRs_t;

      typedef enum logic [`EVL_APB_REQ_TYPE_MSB:0] {
            APB_REQ_READ  = `EVL_APB_READ,
            APB_REQ_WRITE = `EVL_APB_WRITE
         } evl_apb_req_type_t;
      typedef enum logic [`EVL_APB_REQ_TYPE_MSB:0] {
            APB_RESP_SLVERR  = `EVL_APB_RESP_SLVERR,
            APB_RESP_OKAY    = `EVL_APB_RESP_OKAY
         } evl_apb_resp_type_t;
   endpackage : evl_apb_types_pkg


   //-----------------------------------------------------------------------------------------------
   package evl_apb_pkg;
      `ifdef ET_USE_UVM
         import uvm_pkg::*;
      `endif
      import evl_base_pkg::*;
      import evl_apb_types_pkg::*;

      typedef struct packed {
            bit         valid;
            bit         pwrite;
            evl_paddr_t paddr;
            evl_dword_t pwdata;
         } evl_apb_req_s;

      typedef struct packed {
            bit         valid;
            evl_dword_t prdata;
            bit         pslverr;
         } evl_apb_rsp_s;


      `ifdef EVL_SIMULATION
         typedef class evl_apb_bus_req;
         typedef class evl_apb_bus_rsp;
         typedef class evl_apb_bus_parser;
         typedef class evl_apb_master_agent;
         typedef class evl_apb_master_driver;
         typedef class evl_apb_master_sequencer;
         typedef class evl_apb_slave_agent;
         typedef class evl_apb_slave_driver;
         typedef class evl_apb_slave_sequencer;
         typedef class evl_apb_monitor;
         typedef class evl_apb_rtl_port;

         `ifdef ET_INCLUDE_USR_PORT
            `include "dv/common/apb/apb_vc/usr_apb_rtl_port.sv"
         `endif // ifdef ET_INCLUDE_USR_PORT
         `include "dv/common/apb/apb_vc/evl_apb_bus_req.sv"
         `include "dv/common/apb/apb_vc/evl_apb_bus_rsp.sv"
         `include "dv/common/apb/apb_vc/evl_apb_bus_parser.sv"
         `include "dv/common/apb/apb_vc/evl_apb_seq_lib.sv"
         `include "dv/common/apb/apb_vc/evl_apb_master_sequencer.sv"
         `include "dv/common/apb/apb_vc/evl_apb_slave_agent.sv"
         `include "dv/common/apb/apb_vc/evl_apb_slave_driver.sv"
         `include "dv/common/apb/apb_vc/evl_apb_slave_sequencer.sv"
         `include "dv/common/apb/apb_vc/evl_apb_rtl_port.sv"
         `include "dv/common/apb/apb_vc/evl_apb_master_driver.sv"
         `include "dv/common/apb/apb_vc/evl_apb_master_agent.sv"
         `include "dv/common/apb/apb_vc/evl_apb_monitor.sv"
         `include "dv/common/apb/apb_vc/evl_apb_verif_comp.sv"
      `else // ifdef EVL_SIMULATION
         typedef class evl_apb_bus_req;
         typedef class evl_apb_bus_rsp;

         `include "dv/common/apb/apb_vc/usr_apb_rtl_port.sv"
         `include "dv/common/apb/apb_vc/evl_apb_bus_req.sv"
         `include "dv/common/apb/apb_vc/evl_apb_bus_rsp.sv"
      `endif // !ifdef EVL_SIMULATION

   endpackage : evl_apb_pkg

`endif // !__EVL_APB_PKG
