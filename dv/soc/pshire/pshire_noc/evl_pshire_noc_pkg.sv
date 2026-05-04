`ifndef __EVL_PSHIRE_NOC_PKG

   `define __EVL_PSHIRE_NOC_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_pshire_noc_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;


      //
      // Basic packet structure.
      //
      typedef struct packed {
            bit [139:0] info;
         } evl_pshire_noc_packet_s;

      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
            evl_pshire_noc_packet_s    packet;
         } evl_pshire_noc_packet_u;


      typedef class evl_pshire_noc_env;
      typedef class evl_pshire_noc_packet;
      typedef class evl_pshire_noc_rtl_port;

      `include "dv/soc/pshire/pshire_noc/evl_pshire_noc_packet.sv"
      `include "dv/soc/pshire/pshire_noc/evl_pshire_noc_rtl_port.sv"
      `include "dv/soc/pshire/pshire_noc/evl_pshire_noc_env.sv"

   endpackage : evl_pshire_noc_pkg


`endif // !__EVL_PSHIRE_NOC_PKG
