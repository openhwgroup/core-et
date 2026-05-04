
`ifndef __EVL_TLINK_COV_PKG

   `define __EVL_TLINK_COV_PKG 1

   `include "dv/common/tlink/tlink_vc/evl_tlink_pkg.sv"


   //-----------------------------------------------------------------------------------------------
   package evl_tlink_cov_pkg;
      `ifdef ET_USE_UVM
         import uvm_pkg::*;
      `endif
      import evl_base_pkg::*;
      import evl_tlink_pkg::*;

      `include "dv/common/tlink/tlink_vc/evl_tlink_cov.sv"
   endpackage : evl_tlink_cov_pkg

`endif // !__EVL_TLINK_COV_PKG
