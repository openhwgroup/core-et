`ifndef __EVL_SOC_PKG

   `define __EVL_SOC_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/noc/evl_noc_pkg.sv"

   package evl_soc_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_noc_pkg::*;

      `include "dv/soc/evl_soc_rtl_port.sv"
      `include "dv/soc/evl_soc_noc_env.sv"
      `include "dv/soc/evl_soc_env.sv"

   endpackage : evl_soc_pkg

`endif // !__EVL_SOC_PKG
