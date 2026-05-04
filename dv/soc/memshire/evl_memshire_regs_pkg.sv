`ifndef __EVL_MEMSHIRE_REGS_PKG

   `define __EVL_MEMSHIRE_REGS_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_memshire_regs_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

      typedef class ral_block_ms_regs;
      typedef class evl_memshire_reg_block;

      `include "dv/soc/memshire/ral_ms_regs.sv"
      `include "dv/soc/memshire/evl_memshire_reg_block.sv"
      `include "dv/soc/memshire/evl_memshire_reg_model.sv"

   endpackage : evl_memshire_regs_pkg

`endif // !__EVL_MEMSHIRE_REGS_PKG
