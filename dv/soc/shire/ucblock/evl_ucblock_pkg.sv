`ifndef __EVL_UC_BLOCK_PKG_

   `define __EVL_UC_BLOCK_PKG_ 1

   `include "dv/common/base/evl_base_pkg.sv"

   `ifdef EVL_SIMULATION
      `include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"
   `endif // EVL_SIMULATION

   package evl_ucblock_pkg;

      import evl_base_pkg::*;

      `ifdef EVL_SIMULATION
         import uvm_pkg::*;
         import evl_etlink_types_pkg::*;

         typedef struct packed {
               int                  etlink_port_id;
               evl_trans_id_t       etlink_trans_id;
               evl_bus_req_id_t     etlink_bus_cmd;
               evl_bus_req_id_t     axi_req_id;
               evl_bus_req_id_t     etlink_req_id;
               evl_bus_req_source_t etlink_req_src;
               bit                  uc_l3_enabled;
               bit                  uc_scp_enabled;
               bit                  uc_mem_qos;
               // FUTURE: Nihar Shah use defines/parameters
               bit [3:0]            uc_fcc_enables_in;
               bit [31:0]           uc_flb_enables_in;
               bit [63:0]           uc_esr_wdata_in;
               bit [3:0]            uc_to_neighen_fcc_out;
               bit [31:0]           uc_to_neighdata_fcc_out;
               bit [3:0]            uc_flb_req_valid;
               bit [3:0][12:0]      uc_flb_req_data;
               bit [3:0]            uc_flb_resp_valid;
               bit [3:0]            uc_flb_resp_data;
            } evl_ucblock_info_s;


         typedef union packed {
               evl_ucblock_info_s ucblock_info;
            } evl_ucblock_packet_u;


         typedef enum int {
               UCBLOCK_FLB_RSP_INFO,
               UCBLOCK_FLB_REQ_INFO,
               UCBLOCK_ESR_REQ_INFO,
               UCBLOCK_ETLINK_REQ_INFO,
               UCBLOCK_FROMSYS_AXI_CREDIT
            } evl_ucblock_event_t;


         `include "dv/soc/shire/ucblock/evl_ucblock_packet.sv"
         `include "dv/soc/shire/ucblock/evl_ucblock_scoreboard.sv"
         `include "dv/soc/shire/ucblock/ucblock_cvg.svh"
      `endif // ifdef EVL_SIMULATION

   endpackage : evl_ucblock_pkg

`endif // !__EVL_UC_BLOCK_PKG_
