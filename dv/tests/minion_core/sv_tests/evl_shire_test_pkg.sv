`ifndef __EVL_SHIRE_TEST_PKG

   `define __EVL_SHIRE_TEST_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_shire_test_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

      // Tests
      `include "dv/tests/minion_core/sv_tests/evl_minion_shire_config_test.sv"

   endpackage : evl_shire_test_pkg

`endif // !__EVL_SHIRE_TEST_PKG

