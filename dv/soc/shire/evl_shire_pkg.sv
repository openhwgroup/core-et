`ifndef __EVL_SHIRE_PKG

   `define __EVL_SHIRE_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   `ifdef EVL_SIMULATION
      `include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"
      `include "dv/common/axi/axi_vc/evl_axi_pkg.sv"
   `endif // EVL_SIMULATION

   package evl_shire_pkg;

      import evl_base_pkg::*;

      `ifdef EVL_SIMULATION
         import uvm_pkg::*;
         import evl_etlink_types_pkg::*;

         `include "dv/soc/shire/evl_shire_reg_model.sv"
         `include "dv/soc/shire/evl_shire_rtl_port.sv"
         `include "dv/soc/shire/evl_shire_env.sv"
         `include "dv/shire_common/sbm/evl_shire_bus_master_cvg.sv"
         `include "dv/soc/shire/shire_cvg.svh"
      `endif // ifdef EVL_SIMULATION

   endpackage : evl_shire_pkg

`endif // !__EVL_SHIRE_PKG
