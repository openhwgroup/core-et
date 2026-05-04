`ifndef __EVL_AXI_STIM_PKG

   `define __EVL_AXI_STIM_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/axi/axi_vc/evl_axi_pkg.sv"

   `define EVL_AXI_REQ_NONE    (`EVL_Bus_Idle)
   `define EVL_AXI_REQ_READ    (evl_bus_cmd_t'(AXI_REQ_READ))
   `define EVL_AXI_REQ_WRITE   (evl_bus_cmd_t'(AXI_REQ_WRITE))

   `define EVL_AXI_RSP_NONE    (`EVL_Bus_Idle)
   `define EVL_AXI_RSP_RRESP   (evl_bus_cmd_t'(AXI_RSP_RRESP))
   `define EVL_AXI_RSP_BRESP   (evl_bus_cmd_t'(AXI_RSP_BRESP))


   package evl_axi_stim_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_axi_types_pkg::*;
      import evl_axi_pkg::*;
      import evl_etlink_types_pkg::*;

      typedef enum byte {
            AXQ_None,
            AXQ_Remote,
            AXQ_Local,
            AXQ_AtomicRemote,
            AXQ_AtomicLocal,
            AXQ_AtomicComplete,
            AXQ_MsgRemote,
            AXQ_MsgLocal
         } evl_axi_qual_t;

      typedef enum byte {
            AXI_NoSrc,
            AXI_L2_to_L3,
            AXI_L3_to_Sys,
            AXI_UC_to_L3,
            AXI_UC_to_Sys
         } evl_axi_source_t;

      typedef class evl_atomic_rsp_master_agent_axi;
      typedef class evl_ddr_slave_agent_axi;
      typedef class evl_fromsys_master_agent_axi;
      typedef class evl_generic_master_agent_axi;
      typedef class evl_generic_slave_agent_axi;
      typedef class evl_io_pm_master_agent_axi;
      typedef class evl_io_shire_slave_agent_axi;
      typedef class evl_iotosys_master_agent_axi;
      typedef class evl_l2_master_agent_axi;
      typedef class evl_l2tol3_master_agent_axi;
      typedef class evl_l3_master_agent_axi;
      typedef class evl_mem_master_agent_axi;
      typedef class evl_mesh_slave_agent_axi;
      typedef class evl_minion_shire_slave_agent_axi;
      typedef class evl_maxion_shire_slave_agent_axi;
      typedef class evl_pshire_base_master_agent_axi;
      typedef class evl_pshire_dbg_master_agent_axi;
      typedef class evl_pshire_esr_master_agent_axi;
      typedef class evl_pshire_pcie_master_agent_axi;
      typedef class evl_pshire_tosys_master_agent_axi;
      typedef class evl_pu_dma_am_master_agent_axi;
      typedef class evl_pu_dma_master_agent_axi;
      typedef class evl_pu_emmc_master_agent_axi;
      typedef class evl_pu_usb_master_agent_axi;
      typedef class evl_rbm_master_agent_axi;
      typedef class evl_sbm_master_agent_axi;
      typedef class evl_shire_slave_agent_axi;
      typedef class evl_spio_am_master_agent_axi;
      typedef class evl_spio_dma_master_agent_axi;
      typedef class evl_spio_e2a_master_agent_axi;
      typedef class evl_spio_ram_master_agent_axi;
      typedef class evl_spio_rom_master_agent_axi;
      typedef class evl_spio_vault_master_agent_axi;
      typedef class evl_stim_agent_axi;
      typedef class evl_stim_cache_axi;
      typedef class evl_tosys_master_agent_axi;
      typedef class evl_uctol3_master_agent_axi;
      typedef class evl_uctosys_master_agent_axi;
      typedef class evl_ust_master_agent_axi;
      typedef class evl_ust_slave_agent_axi;

      `include "dv/common/axi/axi_stim/evl_stim_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_stim_cache_axi.sv"
      `include "dv/common/axi/axi_stim/evl_atomic_rsp_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_ddr_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_fromsys_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_generic_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_generic_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_io_pm_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_io_shire_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_iotosys_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_l2_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_l2tol3_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_l3_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_mem_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_mesh_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_minion_shire_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_maxion_shire_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pshire_base_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pshire_tosys_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pshire_pcie_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pshire_esr_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pshire_dbg_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_rbm_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_sbm_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_shire_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_spio_e2a_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_spio_ram_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_spio_rom_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_tosys_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_uctosys_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_uctol3_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_ust_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_ust_slave_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pu_dma_am_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pu_dma_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pu_emmc_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_pu_usb_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_spio_am_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_spio_dma_master_agent_axi.sv"
      `include "dv/common/axi/axi_stim/evl_spio_vault_master_agent_axi.sv"

   endpackage : evl_axi_stim_pkg

`endif // !__EVL_AXI_STIM_PKG

