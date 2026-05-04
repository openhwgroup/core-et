`ifndef __EVL_STANDALONE_MINION_VC_PKG

   `define __EVL_STANDALONE_MINION_VC_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "rtl/inc/et_link_defines.vh"
   `include "rtl/inc/minion_defines.vh"      
   `include "rtl/inc/vpu_defines.vh"
   `include "rtl/inc/frontend_defines.vh"

   package evl_standalone_minion_rtl_pkg;
      `include "rtl/inc/et_link_types.vh"
      `include "rtl/inc/esr_types_auto.vh"
      `include "rtl/inc/minion_types.vh"
      `include "rtl/inc/vpu_types.vh"
      `include "rtl/inc/frontend_types.vh"
   endpackage : evl_standalone_minion_rtl_pkg

   package evl_standalone_minion_vc_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_etlink_types_pkg::*;

      `define EVL_STANDALONE_MINION_PACKET_SIZE 128
      `define EVL_FLB_DATA_SIZE     12 

      typedef enum int {
         STANDALONE_MINION_ICACHE_REQ,
         STANDALONE_MINION_ICACHE_RSP,
         STANDALONE_MINION_PTW_REQ,
         STANDALONE_MINION_PTW_RSP,
         STANDALONE_MINION_FLB_REQ,
         STANDALONE_MINION_FLB_RSP
      } evl_standalone_minion_event_t;

      typedef enum int {
         L2_FLB_REQ,
         L2_FLB_RSP, 
         CORE_FLB_REQ,
         CORE_FLB_RSP
      } evl_flb_type_t;


      typedef evl_standalone_minion_rtl_pkg::minion_ptw_req      evl_ptw_req;
      typedef evl_standalone_minion_rtl_pkg::minion_ptw_pte      evl_ptw_rsp;
      typedef evl_standalone_minion_rtl_pkg::frontend_icache_req evl_frontend_icache_req;

      //
      // FLB packet structure
      //
      typedef struct packed {
          bit [`EVL_FLB_DATA_SIZE-1:0] data;
         evl_flb_type_t flb_type;
      } evl_flb_trans;

      //
      // Basic packet structure.
      //
      typedef struct packed {
         bit [`EVL_STANDALONE_MINION_PACKET_SIZE-1:0] info;
      } evl_standalone_minion_packet_s;


      //
      // Encapsulate evl_frontend_icache_req into a right-sized structure.
      //
      typedef struct packed {
         bit [`EVL_STANDALONE_MINION_PACKET_SIZE-1-$bits(evl_frontend_icache_req):0] padding;
         evl_frontend_icache_req info;
      } evl_standalone_minion_icache_req_s;


      //
      // Encapsulate evl_ptw_req into a right-sized structure.
      //
      typedef struct packed {
         bit [`EVL_STANDALONE_MINION_PACKET_SIZE-1-$bits(evl_ptw_req):0] padding;
         evl_ptw_req info;
      } evl_standalone_minion_ptw_req_s;

      //
      // Encapsulate evl_flb_req into a right-sized structure.
      //
      typedef struct packed {
         bit [`EVL_STANDALONE_MINION_PACKET_SIZE-1-$bits(evl_flb_trans):0] padding;
         evl_flb_trans info;
      } evl_standalone_minion_flb_trans_s;
      
      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
         evl_standalone_minion_packet_s     packet;
         evl_standalone_minion_icache_req_s icache_req;
         evl_standalone_minion_ptw_req_s    ptw_req;
         evl_standalone_minion_flb_trans_s  flb;
      } evl_standalone_minion_packet_u;


      typedef class evl_standalone_minion_packet;
      //typedef class evl_standalone_minion_scoreboard;

      //`include "dv/standalone_minion_common/ptw/evl_ptw_rtl_port_base.sv"    // ???
      //`include "dv/standalone_minion_common/ptw/evl_ptw_monitor.sv"          // ???
      //`include "dv/standalone_minion_common/ptw/evl_ptw_verif_comp.sv"       // ???

      `include "dv/soc/ioshire/standalone_minion/evl_standalone_minion_rtl_port.sv"
      `include "dv/soc/ioshire/standalone_minion/evl_standalone_minion_packet.sv"

      //`include "dv/standalone_minion_common/evl_standalone_minion_scoreboard.sv"
      //Neigh coverage class
      //`include "dv/standalone_minion_common/cov/standalone_minion_cvg.svh"

   endpackage : evl_standalone_minion_vc_pkg

`endif // !__EVL_STANDALONE_MINION_VC_PKG
