`ifndef __EVL_SHIRE_CHANNEL_PKG

   `define __EVL_SHIRE_CHANNEL_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/soc/shire/ucblock/evl_ucblock_pkg.sv"

   package evl_shire_channel_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_ucblock_pkg::*;

      `include "dv/soc/shire/shire_channel/evl_shire_channel_rtl_port.sv"
      `include "dv/soc/shire/shire_channel/evl_shire_channel_env.sv"

   endpackage : evl_shire_channel_pkg

`endif // !__EVL_SHIRE_CHANNEL_PKG
