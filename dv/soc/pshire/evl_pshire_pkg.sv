`ifndef __EVL_PSHIRE_PKG

   `define __EVL_PSHIRE_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_pshire_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;


      //
      // Basic packet structure.
      //
      typedef struct packed {
            bit [139:0] info;
         } evl_pshire_packet_s;


      //
      // Encapsulate evl_pshire_config_t into a right-sized structure.
      //
//    typedef struct packed {
//          bit [139-$bits(evl_pshire_config_t):0] padding;
//          evl_pshire_config_t                    info;
//       } evl_pshire_config_s;


      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
            evl_pshire_packet_s    packet;
//          evl_pshire_config_s    config_info;
         } evl_pshire_packet_u;


      typedef class evl_pshire_env;
      typedef class evl_pshire_packet;
      typedef class evl_pshire_rtl_port;

      `include "dv/soc/pshire/evl_pshire_packet.sv"
      `include "dv/soc/pshire/evl_pshire_rtl_port.sv"
      `include "dv/soc/pshire/evl_pshire_env.sv"

   endpackage : evl_pshire_pkg

// `ifndef __EVL_PSHIRE_STUB_INTERFACE
//    `include "dv/soc/pshire/evl_pshire_stub_interface.sv"
// `endif

`endif // !__EVL_PSHIRE_PKG
