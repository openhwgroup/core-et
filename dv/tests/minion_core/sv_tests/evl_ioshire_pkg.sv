`ifndef __EVL_IOSHIRE_PKG

   `define __EVL_IOSHIRE_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

`ifdef L1SB_EVL_MINION_ENV
   `include "dv/soc/shire/neigh/minion/evl_minion_pkg.sv"
`else
   `include "dv/soc/ioshire/standalone_minion/evl_standalone_minion_pkg.sv"
`endif


   package evl_ioshire_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

`ifdef L1SB_EVL_MINION_ENV
      import evl_minion_pkg::*;
`else
      import evl_standalone_minion_pkg::*;
`endif

      //
      // Basic packet structure.
      //
      typedef struct packed {
            bit [139:0] info;
         } evl_ioshire_packet_s;


      //
      // Encapsulate evl_ioshire_config_t into a right-sized structure.
      //
//    typedef struct packed {
//          bit [139-$bits(evl_ioshire_config_t):0] padding;
//          evl_ioshire_config_t                    info;
//       } evl_ioshire_config_s;


      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
            evl_ioshire_packet_s    packet;
//          evl_ioshire_config_s    config_info;
         } evl_ioshire_packet_u;


      typedef class evl_ioshire_env;
      typedef class evl_ioshire_packet;
      typedef class evl_ioshire_rtl_port;

      `include "dv/soc/ioshire/evl_ioshire_packet.sv"
      `include "dv/soc/ioshire/evl_ioshire_rtl_port.sv"
      `include "dv/soc/ioshire/evl_ioshire_env.sv"
      `include "dv/soc/ioshire/ioshire_cvg.svh"

   endpackage : evl_ioshire_pkg

`endif // !__EVL_IOSHIRE_PKG
