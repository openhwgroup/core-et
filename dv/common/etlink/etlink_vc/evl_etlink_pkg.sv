
`ifndef __EVL_ETLINK_PKG

   `define __EVL_ETLINK_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "rtl/inc/et_link_defines.vh"


   //-----------------------------------------------------------------------------------------------
   package evl_etlink_pkg;
      `ifdef ET_USE_UVM
         import uvm_pkg::*;
      `endif
      import evl_base_pkg::*;
      import evl_etlink_types_pkg::*;

      typedef struct packed {
            bit [3:0]               bank;
            evl_trans_id_t          trans_id;
            evl_bus_req_id_t        id;
            evl_bus_req_source_t    source;
            evl_etlink_req_opcode_t opcode;
            bit                     cached;
            evl_paddr_t             address;
            bit                     wdata;
            evl_xword_t             data;
            evl_bus_req_size_t      data_size;
            evl_line_qword_en_t     qwen;
            evl_bus_subopcode_t     subopcode;
         } evl_etlink_bus_req_s;

      typedef struct packed {
            evl_bus_req_id_t        id;
            evl_bus_req_source_t    dest;
            bit                     wdata;
            evl_etlink_rsp_opcode_t opcode;
            evl_xword_t             data;
            evl_etlink_size_t       data_size;
            evl_line_qword_en_t     qwen;
            bit [1:0]               cached;
         } evl_etlink_bus_rsp_s;


      `ifdef EVL_SIMULATION
         typedef class evl_etlink_bus_parser;
         typedef class evl_etlink_bus_req;
         typedef class evl_etlink_bus_req_queue;
         typedef class evl_etlink_bus_rsp;
         typedef class evl_etlink_master_agent;
         typedef class evl_etlink_master_driver;
         typedef class evl_etlink_master_sequencer;
         typedef class evl_etlink_monitor;
         typedef class evl_etlink_rtl_port;
         typedef class evl_etlink_slave_agent;
         typedef class evl_etlink_slave_driver;
         typedef class evl_etlink_slave_sequencer;
         typedef class evl_etlink_verif_comp;

         `ifdef ET_INCLUDE_USR_PORT
            `include "dv/common/etlink/etlink_vc/usr_etlink_rtl_port.sv"
         `endif // ifdef ET_INCLUDE_USR_PORT
         `include "dv/common/etlink/etlink_vc/evl_etlink_bus_req.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_bus_rsp.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_bus_req_queue.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_rtl_port.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_bus_parser.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_seq_lib.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_master_sequencer.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_slave_agent.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_slave_driver.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_slave_sequencer.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_master_driver.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_master_agent.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_monitor.sv"
         `include "dv/common/etlink/etlink_vc/evl_etlink_verif_comp.sv"
      `else // ifdef EVL_SIMULATION
         `include "dv/common/etlink/etlink_vc/usr_etlink_rtl_port.sv"
      `endif // !ifdef EVL_SIMULATION
   endpackage : evl_etlink_pkg

   `ifdef EVL_SIMULATION
      `include "dv/common/etlink/etlink_vc/evl_etlink_cov_pkg.sv"
   `endif // ifdef EVL_SIMULATION

`endif // !__EVL_ETLINK_PKG
