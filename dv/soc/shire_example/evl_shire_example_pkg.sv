`ifndef __EVL_SHIRE_PKG

   `define __EVL_SHIRE_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_shire_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

      `include "dv/soc/shire_example/evl_shire_example_env.sv"

   endpackage : evl_shire_pkg

`endif // !__EVL_SHIRE_PKG
