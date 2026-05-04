`ifndef __EVL_DBG_NOC_PKG

   `define __EVL_DBG_NOC_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "rtl/inc/shire_cache_defines.vh"


   //-----------------------------------------------------------------------------------------------
   package evl_dbg_noc_pkg;
      `ifdef EVL_SIMULATION
         import uvm_pkg::*;
      `endif // `ifdef EVL_SIMULATION
      import evl_base_pkg::*;

      typedef struct packed unsigned {
            evl_paddr_t          req_addr;
            evl_bus_req_vc_t     req_dest_int;
            evl_bus_qos_t        req_qos;
            bit                  req_eop;
            evl_xword_t          req_data;
            evl_trans_id_t       trans_id;
         } evl_dbg_noc_req_info_t;

      `ifdef EVL_SIMULATION
         typedef class evl_dbg_noc_bus_req;
         typedef class evl_dbg_noc_bus_parser;
         typedef class evl_dbg_noc_master_agent;
         typedef class evl_dbg_noc_master_driver;
         typedef class evl_dbg_noc_master_sequencer;
         typedef class evl_dbg_noc_slave_agent;
         typedef class evl_dbg_noc_slave_driver;
         typedef class evl_dbg_noc_slave_sequencer;
         typedef class evl_dbg_noc_monitor;
         typedef class evl_dbg_noc_rtl_port_base;

         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_bus_req.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_bus_parser.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_seq_lib.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_master_sequencer.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_slave_agent.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_slave_driver.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_slave_sequencer.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_rtl_port_base.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_master_driver.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_master_agent.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_monitor.sv"
         `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_verif_comp.sv"
      `endif // EVL_SIMULATION
   endpackage : evl_dbg_noc_pkg

`endif // !__EVL_DBG_NOC_PKG
