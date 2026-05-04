`ifndef __EVL_NEIGH_PKG

   `define __EVL_NEIGH_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/neigh_common/evl_neigh_vc_pkg.sv"
`ifdef L1SB_EVL_MINION_ENV
   `include "dv/soc/shire/neigh/minion/evl_minion_pkg.sv"
`endif

   package evl_neigh_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_neigh_vc_pkg::*;
`ifdef L1SB_EVL_MINION_ENV
      import evl_minion_pkg::*;
`endif

      `include "dv/soc/shire/neigh/evl_neigh_env.sv"

   endpackage : evl_neigh_pkg

`endif // !__EVL_NEIGH_PKG
