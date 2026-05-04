`ifndef __EVL_TCL_TEST_PKG

   `define __EVL_TCL_TEST_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_tcl_test_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

      `include "dv/tests/common/tcl_tests/evl_tcl_test.sv"

   endpackage : evl_tcl_test_pkg

`endif // !__EVL_TCL_TEST_PKG
