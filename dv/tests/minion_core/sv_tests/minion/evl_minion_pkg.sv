`ifndef __EVL_MINION_PKG

   `define __EVL_MINION_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/minion_common/evl_minion_vc_pkg.sv"

   package evl_minion_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_minion_vc_pkg::*;

      `include "dv/soc/shire/neigh/minion/evl_minion_env.sv"

   endpackage : evl_minion_pkg

`endif // !__EVL_MINION_PKG
