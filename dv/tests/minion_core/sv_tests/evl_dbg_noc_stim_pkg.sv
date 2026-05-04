`ifndef __EVL_DBG_NOC_STIM_PKG

   `define __EVL_DBG_NOC_STIM_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/dbg_noc/dbg_noc_vc/evl_dbg_noc_pkg.sv"

   package evl_dbg_noc_stim_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_dbg_noc_pkg::*;
      import evl_etlink_types_pkg::*;

      typedef class evl_stim_agent_dbg_noc;

      `include "dv/common/dbg_noc/dbg_noc_stim/evl_stim_agent_dbg_noc.sv"

   endpackage : evl_dbg_noc_stim_pkg

`endif // !__EVL_DBG_NOC_STIM_PKG

