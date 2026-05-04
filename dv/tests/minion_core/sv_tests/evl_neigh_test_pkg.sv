`ifndef __EVL_NEIGH_TEST_PKG

   `define __EVL_NEIGH_TEST_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_neigh_test_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;

      // Sequences
      `include "dv/tests/minion_core/sv_tests/seq/evl_core_req_seq.sv"
      `include "dv/tests/minion_core/sv_tests/seq/evl_neigh_base_test_seq.sv"

      // Tests
      `include "dv/tests/minion_core/sv_tests/evl_neigh_base_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_icache_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_tensor_load_coop_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_tensor_store_coop_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_tensor_stress_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_rand_traffic_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_messages_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_rand_test.sv"
      //`include "dv/tests/minion_core/sv_tests/evl_neigh_regs_read_write_test.sv"


   endpackage : evl_neigh_test_pkg

`endif // !__EVL_NEIGH_TEST_PKG

