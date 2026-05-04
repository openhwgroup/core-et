`ifndef __EVL_STANDALONE_MINION_PKG

   `define __EVL_STANDALONE_MINION_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/soc/ioshire/standalone_minion/evl_standalone_minion_vc_pkg.sv"

   package evl_standalone_minion_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_standalone_minion_vc_pkg::*;

      `include "dv/soc/ioshire/standalone_minion/evl_standalone_minion_env.sv"

   endpackage : evl_standalone_minion_pkg

`endif // !__EVL_STANDALONE_MINION_PKG
