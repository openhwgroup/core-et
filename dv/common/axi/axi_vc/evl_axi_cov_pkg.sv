`ifndef __EVL_AXI_COV_PKG

   `define __EVL_AXI_COV_PKG 1

   `include "dv/common/axi/axi_vc/evl_axi_pkg.sv"


   //-----------------------------------------------------------------------------------------------
   package evl_axi_cov_pkg;
      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_axi_pkg::*;
      import evl_axi_types_pkg::*;

      `include "dv/common/axi/axi_vc/evl_axi_cov.sv"
   endpackage : evl_axi_cov_pkg

`endif // !__EVL_AXI_COV_PKG
