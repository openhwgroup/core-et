`ifndef __EVL_NEIGH_VC_PKG

   `define __EVL_NEIGH_VC_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "rtl/inc/et_link_defines.vh"
   `include "rtl/inc/minion_defines.vh"      
   `include "rtl/inc/vpu_defines.vh"
   `include "rtl/inc/frontend_defines.vh"

   package evl_neigh_rtl_pkg;
      `include "rtl/inc/et_link_types.vh"
      `include "rtl/inc/esr_types_auto.vh"
      `include "rtl/inc/minion_types.vh"
      `include "rtl/inc/vpu_types.vh"
      `include "rtl/inc/frontend_types.vh"
   endpackage : evl_neigh_rtl_pkg

   package evl_neigh_vc_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_etlink_types_pkg::*;

      `define EVL_NEIGH_PACKET_SIZE 128

      typedef enum int {
         NEIGH_ICACHE_REQ,
         NEIGH_ICACHE_RSP,
         NEIGH_PTW_REQ,
         NEIGH_PTW_RSP,
         NEIGH_FLB_REQ,
         NEIGH_FLB_RSP
      } evl_neigh_event_t;

      typedef enum int {
         L2_FLB_REQ,
         L2_FLB_RSP, 
         CORE_FLB_REQ,
         CORE_FLB_RSP
      } evl_flb_type_t;


      typedef evl_neigh_rtl_pkg::minion_ptw_req      evl_ptw_req;
      typedef evl_neigh_rtl_pkg::minion_ptw_pte      evl_ptw_rsp;
      typedef evl_neigh_rtl_pkg::frontend_icache_req evl_frontend_icache_req;

      //
      // FLB packet structure
      //
      typedef struct packed {
          bit [`CSR_FL_BARRIER_SIZE-1:0] data;
         evl_flb_type_t flb_type;
      } evl_flb_trans;

      //
      // Basic packet structure.
      //
      typedef struct packed {
         bit [`EVL_NEIGH_PACKET_SIZE-1:0] info;
      } evl_neigh_packet_s;


      //
      // Encapsulate evl_frontend_icache_req into a right-sized structure.
      //
      typedef struct packed {
         bit [`EVL_NEIGH_PACKET_SIZE-1-$bits(evl_frontend_icache_req):0] padding;
         evl_frontend_icache_req info;
      } evl_neigh_icache_req_s;


      //
      // Encapsulate evl_ptw_req into a right-sized structure.
      //
      typedef struct packed {
         bit [`EVL_NEIGH_PACKET_SIZE-1-$bits(evl_ptw_req):0] padding;
         evl_ptw_req info;
      } evl_neigh_ptw_req_s;

      //
      // Encapsulate evl_flb_req into a right-sized structure.
      //
      typedef struct packed {
         bit [`EVL_NEIGH_PACKET_SIZE-1-$bits(evl_flb_trans):0] padding;
         evl_flb_trans info;
      } evl_neigh_flb_trans_s;
      
      //
      // Encapsulate evl_pmu_neigh_pkt_t into a right-sized structure.
      //
      typedef struct packed {
         logic                  pmu_thread_id;
         logic            [3:0] pmu_cntr_id;
         logic            [4:0] pmu_event_type;
      } evl_pmu_neigh_pkt_t;

      typedef struct packed {
         bit [`EVL_NEIGH_PACKET_SIZE-1-$bits(evl_pmu_neigh_pkt_t):0] padding;
         evl_pmu_neigh_pkt_t info;
      } evl_neigh_pmu_pkt_s;

      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
         evl_neigh_packet_s     packet;
         evl_neigh_icache_req_s icache_req;
         evl_neigh_ptw_req_s    ptw_req;
         evl_neigh_flb_trans_s  flb;
         evl_neigh_pmu_pkt_s    pmu_event;
      } evl_neigh_packet_u;


      typedef class evl_neigh_packet;
      typedef class evl_neigh_scoreboard;
      typedef class neigh_cvg;
      typedef class neigh_cvg_per_minion;

      `include "dv/neigh_common/ptw/evl_ptw_rtl_port_base.sv"
      `include "dv/neigh_common/ptw/evl_ptw_monitor.sv"
      `include "dv/neigh_common/ptw/evl_ptw_verif_comp.sv"
      `include "dv/neigh_common/evl_neigh_rtl_port.sv"
      `include "dv/neigh_common/evl_neigh_packet.sv"
      `include "dv/neigh_common/evl_neigh_scoreboard.sv"
      //Neigh coverage classes
      `include "dv/neigh_common/cov/neigh_cvg.svh"
      `include "dv/neigh_common/cov/neigh_cvg_per_minion.svh"

   endpackage : evl_neigh_vc_pkg

`endif // !__EVL_NEIGH_VC_PKG
