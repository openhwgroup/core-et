`ifndef __EVL_APB_STIM_PKG

   `define __EVL_APB_STIM_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

   `define EVL_APB_REQ_NONE  (`EVL_Bus_Idle)
   `define EVL_APB_REQ_READ  (evl_bus_cmd_t'(APB_REQ_READ))
   `define EVL_APB_REQ_WRITE (evl_bus_cmd_t'(APB_REQ_WRITE))

   package evl_apb_stim_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_apb_types_pkg::*;
      import evl_apb_pkg::*;

      typedef class evl_stim_agent_apb;
      typedef class evl_stim_cache_apb;
      typedef class evl_reg_slave_agent_apb;
      typedef class evl_bpam_master_agent_apb;

      `include "dv/common/apb/apb_stim/evl_stim_agent_apb.sv"
      `include "dv/common/apb/apb_stim/evl_stim_cache_apb.sv"
      `include "dv/common/apb/apb_stim/evl_reg_slave_agent_apb.sv"
      `include "dv/common/apb/apb_stim/evl_bpam_master_agent_apb.sv"

   endpackage : evl_apb_stim_pkg

`endif // !__EVL_APB_STIM_PKG


