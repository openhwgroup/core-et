`ifndef __EVL_SHIRE_CACHE_REGS_PKG

   `define __EVL_SHIRE_CACHE_REGS_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_shire_cache_regs_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

      static bit m_force_checks    = (`et_test_plusargs("force_sc_reg_checks") != 0) ? 1'b1 : 1'b0;
      static bit m_reg_is_volatile = (`et_test_plusargs("no_volatile_sc_regs") != 0) ? 1'b0 : 1'b1;

      typedef class ral_block_sc_bank_regs;
      typedef class evl_shire_cache_reg_block;
      typedef class evl_shire_cache_reg_model;

      `include "dv/shire_common/shire_cache/ral_sc_bank_regs.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_reg_block.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_reg_model.sv"

   endpackage : evl_shire_cache_regs_pkg

`endif // !__EVL_SHIRE_CACHE_REGS_PKG


