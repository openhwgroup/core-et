
`ifndef __EVL_AXI_PKG

   `define __EVL_AXI_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "rtl/inc/axi_defines.vh"
   `include "rtl/inc/shire_cache_defines.vh"

   `define EVL_AXI_REQQ_ID_WIDTH   (`SC_REQQ_ID_SIZE)
   `define EVL_AXI_BANK_ID_WIDTH   (`SC_MESH_MASTER_AXI_ID_SIZE - `SC_REQQ_ID_SIZE)
   `define EVL_AXI_BRIDGE_ID_WIDTH (`MESH_SOURCE_BRIDGE_ID_SIZE)

   `define EVL_AXI_REQQ_ID_MSB     (`EVL_AXI_REQQ_ID_WIDTH - 1)
   `define EVL_AXI_BANK_ID_MSB     (`EVL_AXI_BANK_ID_WIDTH - 1)
   `define EVL_AXI_BRIDGE_ID_MSB   (`EVL_AXI_BRIDGE_ID_WIDTH - 1)

   `define EVL_AXI_WRITE           {{`EVL_BUS_CMD_MSB{1'b0}}, 1'b0}
   `define EVL_AXI_READ            {{`EVL_BUS_CMD_MSB{1'b0}}, 1'b1}

   `ifndef EVL_ISSUE_DUT_ERROR_ON_AXI_ERR_RSP_DEFAULT
      `define EVL_ISSUE_DUT_ERROR_ON_AXI_ERR_RSP_DEFAULT 0
   `endif


   //-----------------------------------------------------------------------------------------------
   //
   // Although this may seem a bit redundant and confusing, the RTL uses types.  These types are
   // defined in $root, which is not accessible to packages.  As a result, we need to create a
   // package with the same type definitions.  To prevent types from being defined twice in the
   // interface files (which have access to $root), we define new EVL types based off of RTL types
   // on an as-needed basis.  These new EVL types can be imported by the interface files as needed.
   //
   package evl_axi_types_pkg;
      `include "rtl/inc/axi_types.vh"

      typedef axi_resp_t evl_axi_resp_t;

      typedef enum logic [`EVL_BUS_CMD_MSB:0] {
            AXI_REQ_WRITE = `EVL_AXI_WRITE,
            AXI_REQ_READ  = `EVL_AXI_READ
         } evl_axi_req_type_t;
   endpackage : evl_axi_types_pkg


   //-----------------------------------------------------------------------------------------------
   package evl_axi_pkg;
      `ifdef ET_USE_UVM
         import uvm_pkg::*;
      `endif
      import evl_base_pkg::*;
      import evl_axi_types_pkg::*;

      typedef logic [`EVL_AXI_REQQ_ID_MSB:0]   evl_axi_reqq_id_t;
      typedef logic [`EVL_AXI_BANK_ID_MSB:0]   evl_axi_bank_id_t;
      typedef logic [`EVL_AXI_BRIDGE_ID_MSB:0] evl_axi_bridge_id_t;

      typedef struct packed unsigned {
            bit                  ax_is_read;
            evl_bus_req_id_t     ax_id;
            evl_paddr_t          ax_addr;
            evl_bus_req_len_t    ax_len;
            evl_bus_req_size_t   ax_size;
            evl_bus_req_burst_t  ax_burst;
            evl_bus_lock_t       ax_lock;
            evl_bus_cache_attr_t ax_cache;
            evl_bus_prot_t       ax_prot;
            evl_bus_qos_t        ax_qos;
            evl_bus_region_t     ax_region;
            evl_bus_req_user_t   ax_user;
            evl_bus_req_vc_t     ax_vc;
            evl_trans_id_t       trans_id;
         } evl_axi_req_info_t;

      typedef struct packed unsigned {
            evl_xword_t          w_data;
            evl_xword_byte_en_t  w_strb;
            evl_bus_req_user_t   w_user;
            bit                  w_last;
            evl_trans_id_t       trans_id;
         } evl_axi_w_data_info_t;

      typedef struct packed unsigned {
            bit                  x_is_read;
            evl_bus_req_id_t     x_id;
            evl_axi_resp_t       x_resp;
            evl_xword_t          x_data;
            evl_bus_req_user_t   x_user;
            bit                  x_last;
            evl_trans_id_t       trans_id;
         } evl_axi_rsp_info_t;


      `ifdef EVL_SIMULATION
         typedef class evl_axi_bus_req;
         typedef class evl_axi_bus_rsp;
         typedef class evl_axi_bus_parser;
         typedef class evl_axi_cov_base;
         typedef class evl_axi_master_agent;
         typedef class evl_axi_master_driver;
         typedef class evl_axi_master_seq;
         typedef class evl_axi_master_sequencer;
         typedef class evl_axi_monitor;
         typedef class evl_axi_rtl_port_base;
         typedef class evl_axi_slave_agent;
         typedef class evl_axi_slave_driver;
         typedef class evl_axi_slave_seq;
         typedef class evl_axi_slave_sequencer;
         typedef class evl_axi_verif_comp;

         `ifdef ET_INCLUDE_USR_PORT
            `include "dv/common/axi/axi_vc/usr_axi_rtl_port_base.sv"
         `endif // ifdef ET_INCLUDE_USR_PORT
         `include "dv/common/axi/axi_vc/evl_axi_bus_req.sv"
         `include "dv/common/axi/axi_vc/evl_axi_bus_rsp.sv"
         `include "dv/common/axi/axi_vc/evl_axi_bus_parser.sv"
         `include "dv/common/axi/axi_vc/evl_axi_cov_base.sv"
         `include "dv/common/axi/axi_vc/evl_axi_master_agent.sv"
         `include "dv/common/axi/axi_vc/evl_axi_master_driver.sv"
         `include "dv/common/axi/axi_vc/evl_axi_master_sequencer.sv"
         `include "dv/common/axi/axi_vc/evl_axi_monitor.sv"
         `include "dv/common/axi/axi_vc/evl_axi_rtl_port_base.sv"
         `include "dv/common/axi/axi_vc/evl_axi_seq_lib.sv"
         `include "dv/common/axi/axi_vc/evl_axi_slave_agent.sv"
         `include "dv/common/axi/axi_vc/evl_axi_slave_driver.sv"
         `include "dv/common/axi/axi_vc/evl_axi_slave_sequencer.sv"
         `include "dv/common/axi/axi_vc/evl_axi_verif_comp.sv"
      `else // !EVL_SIMULATION
         typedef class evl_axi_bus_req;
         typedef class evl_axi_bus_rsp;
         typedef class usr_axi_rtl_port_base;

         `include "dv/common/axi/axi_vc/evl_axi_bus_req.sv"
         `include "dv/common/axi/axi_vc/evl_axi_bus_rsp.sv"
         `include "dv/common/axi/axi_vc/usr_axi_rtl_port_base.sv"
      `endif // EVL_SIMULATION
   endpackage : evl_axi_pkg

   `ifdef EVL_SIMULATION
      `include "dv/common/axi/axi_vc/evl_axi_cov_pkg.sv"
   `endif // EVL_SIMULATION

`endif // !__EVL_AXI_PKG
