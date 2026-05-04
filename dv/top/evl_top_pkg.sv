`ifndef __EVL_TOP_PKG

   `define __EVL_TOP_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   package evl_top_pkg;

      import evl_base_pkg::*;

      `ifdef EVL_SIMULATION
         import uvm_pkg::*;
         `ifdef USE_HDL_TEST_COMPS
            import clk_gen_agent_pkg::*;
            import sw_tb_manager_pkg::*;
            import py_tb_manager_pkg::*;
            `ifdef USE_E2A_AXI_VIP
               import svt_axi_uvm_pkg::*;
            `endif
            `ifdef USE_APB_VIP
               import svt_apb_uvm_pkg::*;
            `endif
            `ifdef USE_UART_VIP
               import svt_uart_uvm_pkg::*;
            `endif
            `ifdef USE_USB_VIP
               `timescale 1ps/1ps;
               import svt_usb_uvm_pkg::*;
            `endif
            `ifdef USE_I2C_VIP
               import svt_uvm_pkg::*;
               import svt_i2c_uvm_pkg::*;
               import svt_i2c_enum_pkg::*;
               `include "cust_svt_i2c_master_transaction.sv"
               `include "cust_svt_i2c_slave_transaction.sv"
               `include "cust_svt_i2c_system_configuration.sv"
               `include "i2c_basic_env.sv"
               `include "i2c_default_mst_sequence.sv"
               `include "i2c_default_slv_sequence.sv"
               `include "i2c_default_virtual_sequence.sv"
               `include "i2c_simple_reset_sequence.sv"
               `include "i2c_mst_deviceid_sequence.sv"
               `include "i2c_mst_generate_stop_insted_of_repeatedstart_after_10bit_slave_address_deviceid_sequence.sv"
               `include "i2c_mst_generate_stop_insted_of_repeatedstart_after_slave_address_deviceid_sequence.sv"
               `include "i2c_mst_send_nack_after_1st_byte_of_deviceid_sequence.sv"
               `include "i2c_mst_send_nack_after_2nd_byte_of_deviceid_sequence.sv"
               `include "i2c_mst_send_nack_after_3rd_byte_of_deviceid_sequence.sv"
               `include "i2c_mst_send_nack_after_3rd_byte_of_deviceid_with_rollback_iteration_enable_sequence.sv"
               `include "i2c_mst_send_nack_after_5th_byte_of_deviceid_with_rollback_iteration_enable_sequence.sv"
               `include "i2c_mst_send_nack_after_6th_byte_of_deviceid_with_rollback_iteration_enable_sequence.sv"
            `endif
            `ifdef USE_EMMC_VIP
               import svt_uvm_pkg::*;
               import svt_mem_uvm_pkg::*;
               import svt_emmc_uvm_pkg::*;
            `endif
            `include "mst_io_monitor.sv"
            `include "ioshire_comps_env.sv"
            `ifdef USE_PSHIRE_ENV
               `include "svt_pcie.uvm.pkg"  //VIP package
               `include "uvm_macros.svh"
               import svt_uvm_pkg::*;
               import svt_pcie_uvm_pkg::*;
               import svt_axi_uvm_pkg::*;
               `include "svc_util_parms.v"
               `include "svt_pcie_defines.svi"
               `include "svt_pcie_device_configuration.sv"
               `include "svt_pcie_device_agent.sv"
               `include "pcie_scoreboard.sv"
               `include "ss_axi_tlp_passive_env.sv"
               `include "pshire_comps_env.sv"
               `include "pcie_device_sequence_library.sv"
            `endif
         `endif
         `include "dv/top/evl_sys_reg_model.sv"
         `include "dv/top/evl_top_env.sv"
      `endif // ifdef EVL_SIMULATION

   endpackage : evl_top_pkg

`endif // !__EVL_TOP_PKG
