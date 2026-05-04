`ifndef __EVL_JTAG_STIM_PKG

   `define __EVL_JTAG_STIM_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/jtag/jtag_vc/evl_jtag_pkg.sv"


   package evl_jtag_stim_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_jtag_pkg::*;

      typedef class evl_stim_agent_jtag;

      `include "dv/common/jtag/jtag_stim/evl_stim_agent_jtag.sv"

   endpackage : evl_jtag_stim_pkg

`endif // !__EVL_JTAG_STIM_PKG
