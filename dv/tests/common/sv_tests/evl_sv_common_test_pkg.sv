`ifndef __EVL_SV_COMMON_TEST_PKG

   `define __EVL_SV_COMMON_TEST_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/shire_common/shire_cache/evl_shire_cache_pkg.sv"

   `ifndef ET_MAXION_STANDALONE
      `include "dv/soc/memshire/evl_memshire_regs_pkg.sv"
   `endif // ifndef ET_MAXION_STANDALONE

   package evl_sv_common_test_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_shire_cache_types_pkg::*;

      `ifndef ET_MAXION_STANDALONE
         import evl_memshire_types_pkg::*;
         import evl_memshire_regs_pkg::*;
      `endif // ifndef ET_MAXION_STANDALONE

      `include "dv/tests/common/sv_tests/evl_agent_delay_config.sv"
      `include "dv/tests/common/sv_tests/evl_sc_config_test.sv"

      `ifndef ET_MAXION_STANDALONE
         `include "dv/tests/common/sv_tests/evl_ms_config_test.sv"
         `include "dv/tests/common/sv_tests/evl_ddr_auto_config_test.sv"
      `endif // ifndef ET_MAXION_STANDALONE

   endpackage : evl_sv_common_test_pkg

`endif // !__EVL_SV_COMMON_TEST_PKG
