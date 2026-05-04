`ifndef __EVL_JTAG_PKG

   `define __EVL_JTAG_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"


   //-----------------------------------------------------------------------------------------------
   package evl_jtag_pkg;
      `ifdef EVL_SIMULATION
         import uvm_pkg::*;
      `endif // `ifdef EVL_SIMULATION
      import evl_base_pkg::*;

      typedef struct packed {
            evl_cmd_t         cmd;
            evl_jtag_length_t count;
            evl_jtag_data_t   data;
         } evl_jtag_req_info_t;

      typedef struct packed {
            evl_jtag_length_t count;
            evl_jtag_data_t   data;
         } evl_jtag_rsp_info_t;

      typedef enum byte {
            TAP_RESET,
            RUN_TEST,
            SELECT_DR,
            CAPTURE_DR,
            SHIFT_DR,
            EXIT1_DR,
            PAUSE_DR,
            EXIT2_DR,
            UPDATE_DR,
            SELECT_IR,
            CAPTURE_IR,
            SHIFT_IR,
            EXIT1_IR,
            PAUSE_IR,
            EXIT2_IR,
            UPDATE_IR
         } jtag_state_t;

      `ifdef EVL_SIMULATION
         typedef class evl_jtag_bus_parser;
         typedef class evl_jtag_master_agent;
         typedef class evl_jtag_master_driver;
         typedef class evl_jtag_master_sequencer;
         typedef class evl_jtag_monitor;
         typedef class evl_jtag_rtl_port;

         `include "dv/common/jtag/jtag_vc/evl_jtag_bus_parser.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_seq_lib.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_master_sequencer.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_rtl_port.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_master_driver.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_master_agent.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_monitor.sv"
         `include "dv/common/jtag/jtag_vc/evl_jtag_verif_comp.sv"
      `endif // EVL_SIMULATION
   endpackage : evl_jtag_pkg

`endif // !__EVL_JTAG_PKG
