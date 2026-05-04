`ifndef __EVL_ETLINK_COV_PKG

   `define __EVL_ETLINK_COV_PKG 1

   `include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"


   //-----------------------------------------------------------------------------------------------
   package evl_etlink_cov_pkg;
      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_etlink_pkg::*;
      import evl_etlink_types_pkg::*;

      `include "dv/common/etlink/etlink_vc/evl_etlink_cov.sv"
   endpackage : evl_etlink_cov_pkg

`endif // !__EVL_ETLINK_COV_PKG
