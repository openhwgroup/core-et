
`include "timescale.vh"

`ifndef __EVL_BASE_PKG

   `define __EVL_BASE_PKG 1

   `ifdef UVM_VCS_RECORD
      `ifndef EVL_ALLOW_VPD
         `define EVL_ALLOW_VPD 1
      `endif // ifndef EVL_ALLOW_VPD
   `endif // ifdef UVM_VCS_RECORD

   `ifdef UVM_VERDI_VIF_RECORD
      `ifndef EVL_ALLOW_FSDB
         `define EVL_ALLOW_FSDB 1
      `endif // ifndef EVL_ALLOW_FSDB
   `endif // ifdef UVM_VERDI_VIF_RECORD

   `ifndef ET_DPI_PKT_WIDTH
      `define ET_DPI_PKT_WIDTH     256
      `define ET_DPI_PKT_MSB       (`ET_DPI_PKT_WIDTH - 1)
   `endif

   `ifndef ET_FIFO_EVENT_WIDTH
      `define ET_FIFO_EVENT_WIDTH  8
      `define ET_FIFO_EVENT_MSB    (`ET_FIFO_EVENT_WIDTH - 1)

      `define ET_FIFO_NULL_EVENT   `ET_FIFO_EVENT_WIDTH'd0
      `define ET_FIFO_RETIRE_EVENT `ET_FIFO_EVENT_WIDTH'd1
   `endif


   //-----------------------------------------------------------------------------------------------
   // ET-SOC Memory Map
   //-----------------------------------------------------------------------------------------------

   //
   // Main Regions
   //
   `define EVL_SYSTEMREG_REGION_BASE          evl_paddr_t'('h00_0200_0000)
   `define EVL_SYSTEMREG_REGION_TOP           evl_paddr_t'('h00_0200_0fff)
   `define EVL_MRAMREG_REGION_BASE            evl_paddr_t'('h00_0200_1000)
   `define EVL_MRAMREG_REGION_TOP             evl_paddr_t'('h00_0200_1fff)
   `define EVL_IO_REGION_BASE                 evl_paddr_t'('h00_0200_2000)
   `define EVL_IO_REGION_TOP                  evl_paddr_t'('h00_0200_2fff)
   `define EVL_HYPERBUS_REGION_BASE           evl_paddr_t'('h00_0200_3000)
   `define EVL_HYPERBUS_REGION_TOP            evl_paddr_t'('h00_0200_3fff)
   `define EVL_BOOTROM_REGION_BASE            evl_paddr_t'('h00_0200_a000)
   `define EVL_BOOTROM_REGION_TOP             evl_paddr_t'('h00_0200_bfff)
   `define EVL_SRAM_REGION_BASE               evl_paddr_t'('h00_0200_e000)
   `define EVL_SRAM_REGION_TOP                evl_paddr_t'('h00_0200_e7ff)
   `define EVL_MRAM_REGION_BASE               evl_paddr_t'('h00_4000_0000)
   `define EVL_MRAM_REGION_TOP                evl_paddr_t'('h00_40ff_ffff)
   `define EVL_OTP_REGION_BASE                evl_paddr_t'('h00_4100_0000)
   `define EVL_OTP_REGION_TOP                 evl_paddr_t'('h00_4000_ffff) // TODO: set correct size
   `define EVL_RESERVED0_REGION_BASE          evl_paddr_t'(`EVL_OTP_REGION_TOP + 1)
   `define EVL_RESERVED0_REGION_TOP           evl_paddr_t'('h00_7fff_ffff)
   `define EVL_ESR_REGION_BASE                evl_paddr_t'('h00_8000_0000)
   `define EVL_ESR_REGION_TOP                 evl_paddr_t'('h00_bfff_ffff)
   `define EVL_PLIC_REGION_BASE               evl_paddr_t'('h00_c000_0000)
   `define EVL_PLIC_REGION_TOP                evl_paddr_t'('h00_c3ff_ffff)
   `define EVL_SCP_REGION_BASE                evl_paddr_t'('h01_0000_0000)
   `define EVL_SCP_REGION_TOP                 evl_paddr_t'('h01_ffff_ffff)
   `define EVL_RESERVED_REGION_BASE           evl_paddr_t'('h02_0000_0000)
   `define EVL_RESERVED_REGION_TOP            evl_paddr_t'('h3f_ffff_ffff)

   //
   // DRAM Region: Cached subregions
   //

   // new PMA: RTLMIN-5656: when mprot.enable_secure_mem=0:
   `define EVL_C_MCODE_REGION_BASE            evl_paddr_t'('h80_0000_0000)
   `define EVL_C_MCODE_REGION_TOP             evl_paddr_t'('h80_007f_ffff)   // was: 'h80_001f_ffff  // RTLMIN-5656: new PMA
   `define EVL_C_OS_REGION_BASE               evl_paddr_t'('h80_0080_0000)   // was: 'h80_0020_0000  // RTLMIN-5656: new PMA
   `define EVL_C_OS_REGION_TOP                evl_paddr_t'('h80_ffff_ffff)

   // new PMA: RTLMIN-5656: when mprot.enable_secure_mem=1:
   `define EVL_SECURE_C_M_CODE_REGION_BASE    evl_paddr_t'('h80_0000_0000)
   `define EVL_SECURE_C_M_CODE_REGION_TOP     evl_paddr_t'('h80_001f_ffff)
   `define EVL_SECURE_C_M_DATA_REGION_BASE    evl_paddr_t'('h80_0020_0000)
   `define EVL_SECURE_C_M_DATA_REGION_TOP     evl_paddr_t'('h80_007f_ffff)
   `define EVL_SECURE_C_S_CODE_REGION_BASE    evl_paddr_t'('h80_0080_0000)
   `define EVL_SECURE_C_S_CODE_REGION_TOP     evl_paddr_t'('h80_00ff_ffff)
   `define EVL_SECURE_C_S_DATA_REGION_BASE    evl_paddr_t'('h80_0100_0000)
   `define EVL_SECURE_C_S_DATA_REGION_TOP     evl_paddr_t'('h80_03ff_ffff)
   // new PMA: RTLMIN-5656: when mprot.enable_secure_mem=1:
   `define EVL_SECURE_C_OS_REGION_BASE        evl_paddr_t'('h80_0400_0000)
   `define EVL_SECURE_C_OS_REGION_TOP         evl_paddr_t'('h80_ffff_ffff)

   `define EVL_C_MEMORY_REGION_BASE           evl_paddr_t'('h81_0000_0000)
   `define EVL_C_MEMORY_REGION_TOP            evl_paddr_t'('hbf_ffff_ffff)

   //
   // DRAM Region: UnCached subregions
   //
   `define EVL_UC_MCODE_REGION_BASE           evl_paddr_t'('hC0_0000_0000)
   `define EVL_UC_MCODE_REGION_TOP            evl_paddr_t'('hC0_001F_FFFF)
   `define EVL_UC_OS_REGION_BASE              evl_paddr_t'('hC0_0020_0000)
   `define EVL_UC_OS_REGION_TOP               evl_paddr_t'('hC0_FFFF_FFFF)
   `define EVL_UC_MEMORY_REGION_BASE          evl_paddr_t'('hC1_0000_0000)
   `define EVL_UC_MEMORY_REGION_TOP           evl_paddr_t'('hFF_FFFF_FFFF)

   //
   // IO Region subregions
   //
   `define EVL_MAXION_REGION_BASE             evl_paddr_t'('h00_0000_0000)
   `define EVL_MAXION_REGION_TOP              evl_paddr_t'('h00_0fff_ffff)
   `define EVL_PERIPHERAL_REGION_BASE         evl_paddr_t'('h00_1000_0000)
   `define EVL_PERIPHERAL_REGION_TOP          evl_paddr_t'('h00_1fff_ffff)
   `define EVL_MAILBOX_REGION_BASE            evl_paddr_t'('h00_2000_0000)
   `define EVL_MAILBOX_REGION_TOP             evl_paddr_t'('h00_3fff_ffff)

   //
   // Service Processor Region subregions
   //
   `define EVL_SP_ROM_REGION_BASE             evl_paddr_t'('h00_4000_0000)
   `define EVL_SP_ROM_REGION_TOP              evl_paddr_t'('h00_4001_ffff)
   `define EVL_SP_U0ESR_REGION_BASE           evl_paddr_t'('h00_4008_0000)
   `define EVL_SP_U0ESR_REGION_TOP            evl_paddr_t'('h00_4008_0fff)
   `define EVL_SP_U1ESR_REGION_BASE           evl_paddr_t'('h00_4008_1000)
   `define EVL_SP_U1ESR_REGION_TOP            evl_paddr_t'('h00_4008_1fff)
   `define EVL_SP_SPIO_REGBUS_REGION_BASE     evl_paddr_t'('h00_4010_0000)
   `define EVL_SP_SPIO_REGBUS_REGION_TOP      evl_paddr_t'('h00_401f_ffff)
   `define EVL_SP_PU_MAIN_REGBUS_REGION_BASE  evl_paddr_t'('h00_4020_0000)
   `define EVL_SP_PU_MAIN_REGBUS_REGION_TOP   evl_paddr_t'('h00_402f_ffff)
   `define EVL_SP_PSHIRE_REGBUS_REGION_BASE   evl_paddr_t'('h00_4030_0000)
   `define EVL_SP_PSHIRE_REGBUS_REGION_TOP    evl_paddr_t'('h00_403f_ffff)
   `define EVL_SP_SRAM_REGION_BASE            evl_paddr_t'('h00_4040_0000)
   `define EVL_SP_SRAM_REGION_TOP             evl_paddr_t'('h00_404f_ffff)

   // RTLMIN-5656: new PMA: two SP_SRam subregions: lower & upper:
   `define EVL_SP_SRAM_LOW_REGION_BASE        evl_paddr_t'('h00_4040_0000)
   `define EVL_SP_SRAM_LOW_REGION_TOP         evl_paddr_t'('h00_4047_ffff)
   `define EVL_SP_SRAM_UPP_REGION_BASE        evl_paddr_t'('h00_4048_0000)
   `define EVL_SP_SRAM_UPP_REGION_TOP         evl_paddr_t'('h00_404f_ffff)

   `define EVL_SP_MAIN_NOC_REGBUS_REGION_BASE evl_paddr_t'('h00_4200_0000)
   `define EVL_SP_MAIN_NOC_REGBUS_REGION_TOP  evl_paddr_t'('h00_43ff_ffff)
   `define EVL_SP_PLIC_REGION_BASE            evl_paddr_t'('h00_5000_0000)
   `define EVL_SP_PLIC_REGION_TOP             evl_paddr_t'('h00_51ff_ffff)
   `define EVL_SP_VAULT_REGION_BASE           evl_paddr_t'('h00_5200_0000)
   `define EVL_SP_VAULT_REGION_TOP            evl_paddr_t'('h00_5201_ffff)
   `define EVL_SP_I2C0_REGION_BASE            evl_paddr_t'('h00_5202_0000)
   `define EVL_SP_I2C0_REGION_TOP             evl_paddr_t'('h00_5202_0fff)
   `define EVL_SP_SPI0_REGION_BASE            evl_paddr_t'('h00_5202_1000)
   `define EVL_SP_SPI0_REGION_TOP             evl_paddr_t'('h00_5202_1fff)
   `define EVL_SP_UART0_REGION_BASE           evl_paddr_t'('h00_5202_2000)
   `define EVL_SP_UART0_REGION_TOP            evl_paddr_t'('h00_5202_2fff)
   `define EVL_SP_GPIO_REGION_BASE            evl_paddr_t'('h00_5202_3000)
   `define EVL_SP_GPIO_REGION_TOP             evl_paddr_t'('h00_5202_3fff)
   `define EVL_SP_WDT_REGION_BASE             evl_paddr_t'('h00_5202_4000)
   `define EVL_SP_WDT_REGION_TOP              evl_paddr_t'('h00_5202_4fff)
   `define EVL_SP_TIMER_REGION_BASE           evl_paddr_t'('h00_5202_5000)
   `define EVL_SP_TIMER_REGION_TOP            evl_paddr_t'('h00_5202_5fff)
   `define EVL_SP_EFUSE_REGION_BASE           evl_paddr_t'('h00_5202_6000)
   `define EVL_SP_EFUSE_REGION_TOP            evl_paddr_t'('h00_5202_7fff)
   `define EVL_SP_CRU_REGION_BASE             evl_paddr_t'('h00_5202_8000)
   `define EVL_SP_CRU_REGION_TOP              evl_paddr_t'('h00_5202_8fff)
   `define EVL_SP_MISC_REGION_BASE            evl_paddr_t'('h00_5202_9000)
   `define EVL_SP_MISC_REGION_TOP             evl_paddr_t'('h00_5202_9fff)
   `define EVL_SP_RVTIM_REGION_BASE           evl_paddr_t'('h00_5210_0000)
   `define EVL_SP_RVTIM_REGION_TOP            evl_paddr_t'('h00_5210_0fff)
   `define EVL_SP_PVT0_REGION_BASE            evl_paddr_t'('h00_5400_0000)
   `define EVL_SP_PVT0_REGION_TOP             evl_paddr_t'('h00_5400_ffff)
   `define EVL_SP_PVT1_REGION_BASE            evl_paddr_t'('h00_5401_0000)
   `define EVL_SP_PVT1_REGION_TOP             evl_paddr_t'('h00_5401_ffff)
   `define EVL_SP_PVT2_REGION_BASE            evl_paddr_t'('h00_5402_0000)
   `define EVL_SP_PVT2_REGION_TOP             evl_paddr_t'('h00_5402_ffff)
   `define EVL_SP_PVT3_REGION_BASE            evl_paddr_t'('h00_5403_0000)
   `define EVL_SP_PVT3_REGION_TOP             evl_paddr_t'('h00_5403_ffff)
   `define EVL_SP_PVT4_REGION_BASE            evl_paddr_t'('h00_5404_0000)
   `define EVL_SP_PVT4_REGION_TOP             evl_paddr_t'('h00_5404_ffff)
   `define EVL_SP_I2C1_REGION_BASE            evl_paddr_t'('h00_5405_0000)
   `define EVL_SP_I2C1_REGION_TOP             evl_paddr_t'('h00_5405_0fff)
   `define EVL_SP_SPI1_REGION_BASE            evl_paddr_t'('h00_5405_1000)
   `define EVL_SP_SPI1_REGION_TOP             evl_paddr_t'('h00_5405_1fff)
   `define EVL_SP_UART1_REGION_BASE           evl_paddr_t'('h00_5405_2000)
   `define EVL_SP_UART1_REGION_TOP            evl_paddr_t'('h00_5405_2fff)
   `define EVL_SP_PLL0_REGION_BASE            evl_paddr_t'('h00_5405_3000)
   `define EVL_SP_PLL0_REGION_TOP             evl_paddr_t'('h00_5405_3fff)
   `define EVL_SP_PLL1_REGION_BASE            evl_paddr_t'('h00_5405_4000)
   `define EVL_SP_PLL1_REGION_TOP             evl_paddr_t'('h00_5405_4fff)
   `define EVL_SP_PLL2_REGION_BASE            evl_paddr_t'('h00_5405_5000)
   `define EVL_SP_PLL2_REGION_TOP             evl_paddr_t'('h00_5405_5fff)
   `define EVL_SP_PLL3_REGION_BASE            evl_paddr_t'('h00_5405_6000)
   `define EVL_SP_PLL3_REGION_TOP             evl_paddr_t'('h00_5405_6fff)
   `define EVL_SP_PLL4_REGION_BASE            evl_paddr_t'('h00_5405_7000)
   `define EVL_SP_PLL4_REGION_TOP             evl_paddr_t'('h00_5405_7fff)
   `define EVL_SP_DMA_RELOC_REGION_BASE       evl_paddr_t'('h00_5405_8000)
   `define EVL_SP_DMA_RELOC_REGION_TOP        evl_paddr_t'('h00_5405_8fff)
   `define EVL_SP_DMA_REGION_BASE             evl_paddr_t'('h00_5600_0000)
   `define EVL_SP_DMA_REGION_TOP              evl_paddr_t'('h00_5600_0fff)
   `define EVL_SP_AXI_COMM_REGION_BASE        evl_paddr_t'('h00_5610_0000)
   `define EVL_SP_AXI_COMM_REGION_TOP         evl_paddr_t'('h00_561f_ffff)
   `define EVL_SP_PLLMX0_REGION_BASE          evl_paddr_t'('h00_5800_0000)
   `define EVL_SP_PLLMX0_REGION_TOP           evl_paddr_t'('h00_5800_0fff)
   `define EVL_SP_PLLMX1_REGION_BASE          evl_paddr_t'('h00_5800_1000)
   `define EVL_SP_PLLMX1_REGION_TOP           evl_paddr_t'('h00_5800_1fff)
   `define EVL_PCIE_ESR_REGION_BASE           evl_paddr_t'('h00_5820_0000)
   `define EVL_PCIE_ESR_REGION_TOP            evl_paddr_t'('h00_5820_0fff)
   `define EVL_PCIE_PLLP0_REGION_BASE         evl_paddr_t'('h00_5820_1000)
   `define EVL_PCIE_PLLP0_REGION_TOP          evl_paddr_t'('h00_5820_1fff)
   `define EVL_PCIE_APB_SUBSYS_REGION_BASE    evl_paddr_t'('h00_5840_0000)
   `define EVL_PCIE_APB_SUBSYS_REGION_TOP     evl_paddr_t'('h00_585f_ffff)
   `define EVL_SHIRE_LPDDR_REGION_BASE        evl_paddr_t'('h00_6000_0000)
   `define EVL_SHIRE_LPDDR_REGION_TOP         evl_paddr_t'('h00_7fff_ffff)

   //
   // PCIe Region subregions
   //
   `define EVL_PCIE0_SLV_REGION_BASE          evl_paddr_t'('h40_0000_0000)
   `define EVL_PCIE0_SLV_REGION_TOP           evl_paddr_t'('h5f_ffff_ffff)
   `define EVL_PCIE1_SLV_REGION_BASE          evl_paddr_t'('h60_0000_0000)
   `define EVL_PCIE1_SLV_REGION_TOP           evl_paddr_t'('h7e_7fff_ffff)
   `define EVL_PCIE0_DBI_SLV_REGION_BASE      evl_paddr_t'('h7e_8000_0000)
   `define EVL_PCIE0_DBI_SLV_REGION_TOP       evl_paddr_t'('h7e_ffff_ffff)
   `define EVL_PCIE1_DBI_SLV_REGION_BASE      evl_paddr_t'('h7f_0000_0000)
   `define EVL_PCIE1_DBI_SLV_REGION_TOP       evl_paddr_t'('h7f_7fff_ffff)
   `define EVL_PCIE_USRESR_REGION_BASE        evl_paddr_t'('h7f_8000_0000)
   `define EVL_PCIE_USRESR_REGION_TOP         evl_paddr_t'('h7f_8000_0fff)
   `define EVL_PCIE_NOPCIESR_REGION_BASE      evl_paddr_t'('h7f_8000_1000)
   `define EVL_PCIE_NOPCIESR_REGION_TOP       evl_paddr_t'('h7f_8000_1fff)


   `define EVL_NEIGH_PER_SHIRE             4
   `define EVL_MINION_PER_NEIGH            8
   `define EVL_MINION_PER_SHIRE            (`EVL_NEIGH_PER_SHIRE * `EVL_MINION_PER_NEIGH)

   `define EVL_ESR_SHIRE_ID_SHIFT          22
   `define EVL_ESR_SHIRE_ID_MASK           evl_paddr_t'(40'h00fc000000)
   `define EVL_SHIRE_CACHE_BASE_ESR        evl_paddr_t'(40'h0100300000)
   `define EVL_MEMSHIRE_BASE_ESR           evl_paddr_t'(40'h0100000000)
   `define EVL_MEMSHIRE_ATOMIC_CACHE_DEPTH 4
   `define EVL_ESR_PP_BITS                 23:22
   `define EVL_MAX_MINION_SHIRE_ID         34

   `define EVL_MAX_JTAG_LENGTH             512

   //
   // Message Format:
   //
   //                 3333 3333 3322 2222 2222 1111 1111 11
   //                 9876 5432 1098 7654 3210 9876 5432 1098 7654 3210
   //                 ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
   // Hart message:   0000 0001 00ss ssss ss00 mmmm mmmt 1000 pp00 0000 (pp is the port number, ssssssss is the shire)
   // Reduce message: 0000 0001 10ss ssss ss00 mmmm mmmt 1110 00d0 0000 (d is 1 for data, ssssssss is the shire)
   //
   `define EVL_MSG_SHIRE_ID_LSB            22
   `define EVL_MSG_NEIGH_ID_MSB            19
   `define EVL_MSG_NEIGH_ID_LSB            16
   `define EVL_MSG_MINION_ID_LSB           13
   `define EVL_MSG_SHIRE_ID_SEL_BITS       (`EVL_MSG_SHIRE_ID_LSB+7):`EVL_MSG_SHIRE_ID_LSB
   `define EVL_MSG_SHIRE_ID_AND_MASK       evl_paddr_t'({ {(32 - `EVL_MSG_SHIRE_ID_LSB){1'b0}}, 8'hff, {(`EVL_MSG_SHIRE_ID_LSB){1'b0}} })
   `define EVL_MSG_SHIRE_AND_MASK          evl_paddr_t'(~`EVL_MSG_SHIRE_ID_AND_MASK)
   `define EVL_MSG_ADDR_AND_MASK           evl_paddr_t'(40'h003fcffff8)
   `define EVL_MSG_ADDR_OR_MASK            evl_paddr_t'(40'h0180000800)
   `define EVL_MSG_LOCAL_SHIRE_ID          evl_paddr_t'(40'h00000000ff)
   `define EVL_MSG_HART_AND_MASK           evl_paddr_t'(40'h003fcff8c0)
   `define EVL_MSG_HART_OR_MASK            evl_paddr_t'(40'h0100000800)
   `define EVL_MSG_REDUCE_AND_MASK         evl_paddr_t'(40'h003fcffe20)
   `define EVL_MSG_REDUCE_OR_MASK          evl_paddr_t'(40'h0080800e00)
   `define EVL_MSG_REDUCE_CHK_MASK         evl_paddr_t'(40'h0000000e00)
   `define EVL_MSG_PORT_MATCH_MASK         evl_paddr_t'(40'hffc0300f38)
   `define EVL_MSG_PORT_MATCH_VAL          evl_paddr_t'(40'h0100000800)
   `define EVL_MSG_REDUCE_READY_ID         (8'b11000000)
   `define EVL_MSG_REDUCE_DATA_ID          (8'b11000100)
   `define EVL_MSG_TEXTURE_PULL_ID         (8'b11001000)

   `define EVL_ATRSP_SHIRE_ID_LSB          22
   `define EVL_ATRSP_MINION_ID_LSB         12
   `define EVL_ATRSP_MAX_SHIRE_ID          34
   `define EVL_ATRSP_SHIRE_ID_SEL_BITS     (`EVL_ATRSP_SHIRE_ID_LSB+7):`EVL_ATRSP_SHIRE_ID_LSB
   `define EVL_ATRSP_SHIRE_ID_AND_MASK     evl_paddr_t'({ {(32 - `EVL_ATRSP_SHIRE_ID_LSB){1'b0}}, 8'hff, {(`EVL_ATRSP_SHIRE_ID_LSB){1'b0}} })
   `define EVL_ATRSP_SHIRE_AND_MASK        evl_paddr_t'(~`EVL_ATRSP_SHIRE_ID_AND_MASK)
   `define EVL_ATRSP_ADDR_AND_MASK         evl_paddr_t'(40'h000000003f)
   `define EVL_ATRSP_ADDR_OR_MASK          evl_paddr_t'(40'h01803e0000)
   `define EVL_ATRSP_ADDR_CHECK_MASK       evl_paddr_t'(40'hff803fffc0)
   `define EVL_ATRSP_ADDR_CHECK_VALUE      evl_paddr_t'(40'h01803e0000)

   `define EVL_BUS_ATOMIC_OP_MASK        (7'b0001111)
   `define EVL_BUS_ATOMIC_CONF_VEC_OP    (7'b0010000)
   `define EVL_BUS_ATOMIC_CONF_DW_OP     (7'b0100000)
   `define EVL_BUS_ATOMIC_CONF_L3_OP     (7'b1000000)
   `define EVL_AXI_ATOMIC_CONF_MASK      (32'h0000007f)
   `define EVL_AXI_ATOMIC_CONF_SHIFT     (0)
   `define EVL_AXI_ATOMIC_ADDR_MASK      (32'h0000003f)
   `define EVL_AXI_ATOMIC_ADDR_SHIFT     (7)
   `define EVL_AXI_ATOMIC_SOURCE_MASK    (32'h000000ff)
   `define EVL_AXI_ATOMIC_SOURCE_SHIFT   (13)
   `define EVL_AXI_ATOMIC_TRANSID_MASK   (32'h0000001f)
   `define EVL_AXI_ATOMIC_TRANSID_SHIFT  (21)

   `define EVL_GEN_ADDR_L2               1
   `define EVL_GEN_ADDR_L3               2
   `define EVL_GEN_ADDR_SCP              3
   `define EVL_GEN_ADDR_MEM              4

   `define EVL_HWORD_ADDR_LSB            1
   `define EVL_WORD_ADDR_LSB             2
   `define EVL_DWORD_ADDR_LSB            3
   `define EVL_QWORD_ADDR_LSB            4
   `define EVL_OWORD_ADDR_LSB            5
   `define EVL_XWORD_ADDR_LSB            6
   `define EVL_LINE_ADDR_LSB             6
   `define EVL_PAGE_ADDR_LSB             12

   `define EVL_HWORD_BYTES               (1 << `EVL_HWORD_ADDR_LSB)
   `define EVL_WORD_BYTES                (1 << `EVL_WORD_ADDR_LSB)
   `define EVL_DWORD_BYTES               (1 << `EVL_DWORD_ADDR_LSB)
   `define EVL_QWORD_BYTES               (1 << `EVL_QWORD_ADDR_LSB)
   `define EVL_OWORD_BYTES               (1 << `EVL_OWORD_ADDR_LSB)
   `define EVL_XWORD_BYTES               (1 << `EVL_XWORD_ADDR_LSB)
   `define EVL_LINE_BYTES                (1 << `EVL_LINE_ADDR_LSB)

   `define EVL_LINE_HWORDS               (1 << (`EVL_LINE_ADDR_LSB - `EVL_HWORD_ADDR_LSB))
   `define EVL_LINE_WORDS                (1 << (`EVL_LINE_ADDR_LSB - `EVL_WORD_ADDR_LSB))
   `define EVL_LINE_DWORDS               (1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB))
   `define EVL_LINE_QWORDS               (1 << (`EVL_LINE_ADDR_LSB - `EVL_QWORD_ADDR_LSB))
   `define EVL_LINE_OWORDS               (1 << (`EVL_LINE_ADDR_LSB - `EVL_OWORD_ADDR_LSB))
   `define EVL_LINE_XWORDS               (1 << (`EVL_LINE_ADDR_LSB - `EVL_XWORD_ADDR_LSB))

   `define EVL_BYTE_WIDTH                (1)
   `define EVL_HWORD_WIDTH               (`EVL_HWORD_BYTES << 3)
   `define EVL_WORD_WIDTH                (`EVL_WORD_BYTES << 3)
   `define EVL_DWORD_WIDTH               (`EVL_DWORD_BYTES << 3)
   `define EVL_QWORD_WIDTH               (`EVL_QWORD_BYTES << 3)
   `define EVL_OWORD_WIDTH               (`EVL_OWORD_BYTES << 3)
   `define EVL_XWORD_WIDTH               (`EVL_XWORD_BYTES << 3)

   `define EVL_BYTE_MSB                  (`EVL_BYTE_WIDTH - 1)
   `define EVL_HWORD_MSB                 (`EVL_HWORD_WIDTH - 1)
   `define EVL_WORD_MSB                  (`EVL_WORD_WIDTH - 1)
   `define EVL_DWORD_MSB                 (`EVL_DWORD_WIDTH - 1)
   `define EVL_QWORD_MSB                 (`EVL_QWORD_WIDTH - 1)
   `define EVL_OWORD_MSB                 (`EVL_OWORD_WIDTH - 1)
   `define EVL_XWORD_MSB                 (`EVL_XWORD_WIDTH - 1)

   `define EVL_BYTE_MASK                 ((evl_xword_t'(1) << `EVL_BYTE_WIDTH) - 1)
   `define EVL_HWORD_MASK                ((evl_xword_t'(1) << `EVL_HWORD_WIDTH) - 1)
   `define EVL_WORD_MASK                 ((evl_xword_t'(1) << `EVL_WORD_WIDTH) - 1)
   `define EVL_DWORD_MASK                ((evl_xword_t'(1) << `EVL_DWORD_WIDTH) - 1)
   `define EVL_QWORD_MASK                ((evl_xword_t'(1) << `EVL_QWORD_WIDTH) - 1)
   `define EVL_OWORD_MASK                ((evl_xword_t'(1) << `EVL_OWORD_WIDTH) - 1)
   `define EVL_XWORD_MASK                ((evl_xword_t'(1) << `EVL_XWORD_WIDTH) - 1)

   `define EVL_HWORD_BYTE_EN_MASK        ((evl_xword_byte_en_t'(1) << `EVL_HWORD_BYTES) - 1)
   `define EVL_WORD_BYTE_EN_MASK         ((evl_xword_byte_en_t'(1) << `EVL_WORD_BYTES) - 1)
   `define EVL_DWORD_BYTE_EN_MASK        ((evl_xword_byte_en_t'(1) << `EVL_DWORD_BYTES) - 1)
   `define EVL_QWORD_BYTE_EN_MASK        ((evl_xword_byte_en_t'(1) << `EVL_QWORD_BYTES) - 1)
   `define EVL_OWORD_BYTE_EN_MASK        ((evl_xword_byte_en_t'(1) << `EVL_OWORD_BYTES) - 1)
   `define EVL_XWORD_BYTE_EN_MASK        ((evl_xword_byte_en_t'(1) << `EVL_XWORD_BYTES) - 1)
   `define EVL_LINE_BYTE_EN_MASK         ((evl_xword_byte_en_t'(1) << `EVL_LINE_BYTES) - 1)

   `define EVL_HWORD_OFFSET_MASK         ((1 << `EVL_HWORD_ADDR_LSB) - 1)
   `define EVL_WORD_OFFSET_MASK          ((1 << `EVL_WORD_ADDR_LSB)  - 1)
   `define EVL_DWORD_OFFSET_MASK         ((1 << `EVL_DWORD_ADDR_LSB) - 1)
   `define EVL_QWORD_OFFSET_MASK         ((1 << `EVL_QWORD_ADDR_LSB) - 1)
   `define EVL_OWORD_OFFSET_MASK         ((1 << `EVL_OWORD_ADDR_LSB) - 1)
   `define EVL_XWORD_OFFSET_MASK         ((1 << `EVL_XWORD_ADDR_LSB) - 1)
   `define EVL_LINE_OFFSET_MASK          ((1 << `EVL_LINE_ADDR_LSB)  - 1)

   `define EVL_PAGE_OFFSET_MASK          ((1 << `EVL_PAGE_ADDR_LSB)  - 1)

   `define EVL_SHIRE_ID_WIDTH            8
   `define EVL_NEIGHBORHOOD_ID_WIDTH     4
   `define EVL_CORE_ID_WIDTH             4
   `define EVL_THREAD_ID_WIDTH           1
   `define EVL_FULL_CPU_ID_WIDTH         (`EVL_SHIRE_ID_WIDTH + `EVL_NEIGHBORHOOD_ID_WIDTH + `EVL_CORE_ID_WIDTH + `EVL_THREAD_ID_WIDTH)
   `define EVL_PARTIAL_TRANS_ID_WIDTH    24
   `define EVL_SHIRE_POST_CODE_WIDTH     (`EVL_CORE_ID_WIDTH + `EVL_NEIGHBORHOOD_ID_WIDTH)
   `define EVL_POST_CODE_WIDTH           (`EVL_SHIRE_POST_CODE_WIDTH + `EVL_SHIRE_ID_WIDTH)
   `define EVL_TRANS_ID_WIDTH            (`EVL_PARTIAL_TRANS_ID_WIDTH + `EVL_POST_CODE_WIDTH)

   `define EVL_INSTR_WIDTH               32
   `define EVL_CMD_WIDTH                 8
   `define EVL_CMD_LENGTH_WIDTH          16
   `define EVL_ADDR_SP_WIDTH             4
   `define EVL_CACHE_ST_WIDTH            8
   `define EVL_VADDR_WIDTH               48
   `define EVL_PADDR_WIDTH               40
   `define EVL_AGENT_TYPE_WIDTH          8
   `define EVL_AGENT_ID_WIDTH            16
   `define EVL_AGENT_ID_KEY_WIDTH        (`EVL_SHIRE_ID_WIDTH + `EVL_NEIGHBORHOOD_ID_WIDTH + `EVL_CORE_ID_WIDTH + `EVL_AGENT_TYPE_WIDTH)
   `define EVL_AGENT_SEQR_WIDTH          4
   `define EVL_AGENT_KEY_WIDTH           (`EVL_AGENT_ID_WIDTH + `EVL_AGENT_ID_KEY_WIDTH + `EVL_AGENT_SEQR_WIDTH)
   `define EVL_DATA_ACTION_WIDTH         8
   `define EVL_DELAY_WIDTH               16

   `define EVL_BUS_CMD_WIDTH             8
   `define EVL_BUS_REQ_ID_WIDTH          (`MEM_MASTER_AXI_ID_SIZE)
   `define EVL_BUS_REQ_SOURCE_WIDTH      4
   `define EVL_BUS_REQ_SIZE_WIDTH        4
   `define EVL_BUS_REQ_LEN_WIDTH         8
   `define EVL_BUS_SUBOPCODE_WIDTH       (`ET_LINK_SUBOPCODE_SIZE)
   `define EVL_BUS_REQ_BURST_WIDTH       2
   `define EVL_BUS_LOCK_WIDTH            1
   `define EVL_BUS_CACHE_ATTR_WIDTH      4
   `define EVL_BUS_PROT_WIDTH            3
   `define EVL_BUS_QOS_WIDTH             4
   `define EVL_BUS_REGION_WIDTH          4
   `define EVL_BUS_REQ_USER_WIDTH        16
   `define EVL_BUS_REQ_VC_WIDTH          8
   `define EVL_BUS_RSP_USER_WIDTH        (`EVL_BUS_REQ_USER_WIDTH)
   `define EVL_BUS_RSP_TYPE_WIDTH        7
   `define EVL_BUS_RSP_SIZE_WIDTH        3
   `define EVL_BUS_RSP_ID_WIDTH          (`EVL_BUS_REQ_ID_WIDTH)
   `define EVL_BUS_RSP_DEST_WIDTH        (`EVL_BUS_REQ_SOURCE_WIDTH)
   `define EVL_BCST_REGION_ID_WIDTH      1
   `define EVL_NOC_SRC_IFC_WIDTH         1
   `define EVL_NOC_LAYER_WIDTH           1

   `define EVL_BUS_COP_TYPE_WIDTH         3
   `define EVL_BUS_COP_START_LEVEL_WIDTH  2
   `define EVL_BUS_COP_DEST_LEVEL_WIDTH   2
   `define EVL_BUS_ATOMIC_CONF_WIDTH      8
   `define EVL_BUS_ATOMIC_TRANSID_WIDTH   5
   `define EVL_BUS_MESSAGE_CONF_WIDTH     8
   `define EVL_BUS_COOP_TARGET_WIDTH      8
   `define EVL_BUS_COOP_NEIGH_MASK_WIDTH  8
   `define EVL_BUS_COOP_MINION_MASK_WIDTH 8
   `define EVL_BUS_COOP_ID_WIDTH          8
   `define EVL_BUS_COOP_WAY_WIDTH         7

   `define EVL_SHIRE_ID_MSB              (`EVL_SHIRE_ID_WIDTH - 1)
   `define EVL_NEIGHBORHOOD_ID_MSB       (`EVL_NEIGHBORHOOD_ID_WIDTH - 1)
   `define EVL_CORE_ID_MSB               (`EVL_CORE_ID_WIDTH - 1)
   `define EVL_THREAD_ID_MSB             (`EVL_THREAD_ID_WIDTH - 1)
   `define EVL_FULL_CPU_ID_MSB           (`EVL_FULL_CPU_ID_WIDTH - 1)
   `define EVL_SHIRE_POST_CODE_MSB       (`EVL_SHIRE_POST_CODE_WIDTH - 1)
   `define EVL_POST_CODE_MSB             (`EVL_POST_CODE_WIDTH - 1)
   `define EVL_PARTIAL_TRANS_ID_MSB      (`EVL_PARTIAL_TRANS_ID_WIDTH - 1)
   `define EVL_TRANS_ID_MSB              (`EVL_TRANS_ID_WIDTH - 1)

   `define EVL_INSTR_MSB                 (`EVL_INSTR_WIDTH - 1)
   `define EVL_CMD_MSB                   (`EVL_CMD_WIDTH - 1)
   `define EVL_BUS_CMD_MSB               (`EVL_BUS_CMD_WIDTH - 1)
   `define EVL_CMD_LENGTH_MSB            (`EVL_CMD_LENGTH_WIDTH - 1)
   `define EVL_ADDR_SP_MSB               (`EVL_ADDR_SP_WIDTH - 1)
   `define EVL_CACHE_ST_MSB              (`EVL_CACHE_ST_WIDTH - 1)
   `define EVL_VADDR_MSB                 (`EVL_VADDR_WIDTH - 1)
   `define EVL_PADDR_MSB                 (`EVL_PADDR_WIDTH - 1)
   `define EVL_AGENT_TYPE_MSB            (`EVL_AGENT_TYPE_WIDTH - 1)
   `define EVL_AGENT_ID_MSB              (`EVL_AGENT_ID_WIDTH - 1)
   `define EVL_AGENT_ID_KEY_MSB          (`EVL_AGENT_ID_KEY_WIDTH - 1)
   `define EVL_AGENT_SEQR_MSB            (`EVL_AGENT_SEQR_WIDTH - 1)
   `define EVL_AGENT_KEY_MSB             (`EVL_AGENT_KEY_WIDTH - 1)
   `define EVL_DATA_ACTION_MSB           (`EVL_DATA_ACTION_WIDTH - 1)
   `define EVL_DELAY_MSB                 (`EVL_DELAY_WIDTH - 1)

   `define EVL_BUS_CMD_MSB               (`EVL_BUS_CMD_WIDTH - 1)
   `define EVL_BUS_REQ_ID_MSB            (`EVL_BUS_REQ_ID_WIDTH - 1)
   `define EVL_BUS_REQ_SOURCE_MSB        (`EVL_BUS_REQ_SOURCE_WIDTH - 1)
   `define EVL_BUS_REQ_SIZE_MSB          (`EVL_BUS_REQ_SIZE_WIDTH - 1)
   `define EVL_BUS_REQ_LEN_MSB           (`EVL_BUS_REQ_LEN_WIDTH - 1)
   `define EVL_BUS_SUBOPCODE_MSB         (`EVL_BUS_SUBOPCODE_WIDTH - 1)
   `define EVL_BUS_REQ_BURST_MSB         (`EVL_BUS_REQ_BURST_WIDTH - 1)
   `define EVL_BUS_LOCK_MSB              (`EVL_BUS_LOCK_WIDTH - 1)
   `define EVL_BUS_CACHE_ATTR_MSB        (`EVL_BUS_CACHE_ATTR_WIDTH - 1)
   `define EVL_BUS_PROT_MSB              (`EVL_BUS_PROT_WIDTH - 1)
   `define EVL_BUS_QOS_MSB               (`EVL_BUS_QOS_WIDTH - 1)
   `define EVL_BUS_REGION_MSB            (`EVL_BUS_REGION_WIDTH - 1)
   `define EVL_BUS_REQ_USER_MSB          (`EVL_BUS_REQ_USER_WIDTH - 1)
   `define EVL_BUS_REQ_VC_MSB            (`EVL_BUS_REQ_VC_WIDTH - 1)
   `define EVL_BUS_RSP_USER_MSB          (`EVL_BUS_RSP_USER_WIDTH - 1)
   `define EVL_BUS_RSP_TYPE_MSB          (`EVL_BUS_RSP_TYPE_WIDTH - 1)
   `define EVL_BUS_RSP_SIZE_MSB          (`EVL_BUS_RSP_SIZE_WIDTH - 1)
   `define EVL_BUS_RSP_ID_MSB            (`EVL_BUS_RSP_ID_WIDTH - 1)
   `define EVL_BUS_RSP_DEST_MSB          (`EVL_BUS_RSP_DEST_WIDTH - 1)
   `define EVL_BCST_REGION_ID_MSB        (`EVL_BCST_REGION_ID_WIDTH - 1)
   `define EVL_NOC_SRC_IFC_MSB           (`EVL_NOC_SRC_IFC_WIDTH - 1)
   `define EVL_NOC_LAYER_MSB             (`EVL_NOC_LAYER_WIDTH - 1)

   `define EVL_BUS_COP_TYPE_MSB          (`EVL_BUS_COP_TYPE_WIDTH - 1)
   `define EVL_BUS_COP_START_LEVEL_MSB   (`EVL_BUS_COP_START_LEVEL_WIDTH - 1)
   `define EVL_BUS_COP_DEST_LEVEL_MSB    (`EVL_BUS_COP_DEST_LEVEL_WIDTH - 1)
   `define EVL_BUS_ATOMIC_CONF_MSB       (`EVL_BUS_ATOMIC_CONF_WIDTH - 1)
   `define EVL_BUS_ATOMIC_TRANSID_MSB    (`EVL_BUS_ATOMIC_TRANSID_WIDTH - 1)
   `define EVL_BUS_MESSAGE_CONF_MSB      (10)
   `define EVL_BUS_MESSAGE_CONF_LSB      (`EVL_BUS_MESSAGE_CONF_MSB - (`EVL_BUS_MESSAGE_CONF_WIDTH - 1))
   `define EVL_BUS_MSG_DST_NEIGH_MSB     19
   `define EVL_BUS_MSG_DST_NEIGH_LSB     16
   `define EVL_BUS_MSG_DST_MINION_MSB    15
   `define EVL_BUS_MSG_DST_MINION_LSB    13
   `define EVL_BUS_MSG_DST_HART_MSB      12
   `define EVL_BUS_MSG_DST_HART_LSB      12
   `define EVL_BUS_MSG_PORT_MSB          5
   `define EVL_BUS_MSG_PORT_LSB          4
   //`define EVL_BUS_MSG_SHIRE_ID_MSB      29
   //`define EVL_BUS_MSG_SHIRE_ID_LSB      (`EVL_BUS_MSG_SHIRE_ID_MSB - (`EVL_SHIRE_ID_WIDTH-1))

   `define EVL_BUS_MSG_DST_ADDR_WIDTH    (`EVL_NEIGHBORHOOD_ID_WIDTH + `EVL_CORE_ID_WIDTH)

   `define EVL_BUS_COOP_TARGET_MSB       (`EVL_BUS_COOP_TARGET_WIDTH - 1)
   `define EVL_BUS_COOP_TARGET_LSB       ((`EVL_BUS_COOP_TARGET_MSB +1) -`EVL_BUS_COOP_TARGET_WIDTH)

   `define EVL_BUS_COOP_NEIGH_MASK_MSB   (23)
   `define EVL_BUS_COOP_NEIGH_MASK_LSB   ((`EVL_BUS_COOP_NEIGH_MASK_MSB +1) -`EVL_BUS_COOP_NEIGH_MASK_WIDTH)

   `define EVL_BUS_COOP_MINION_MASK_MSB  (`EVL_BUS_COOP_MINION_MASK_WIDTH - 1)
   `define EVL_BUS_COOP_MINION_MASK_LSB  ((`EVL_BUS_COOP_MINION_MASK_MSB +1) -`EVL_BUS_COOP_MINION_MASK_WIDTH)

   `define EVL_BUS_COOP_ID_MSB           (15)
   `define EVL_BUS_COOP_ID_LSB           ((`EVL_BUS_COOP_ID_MSB+1)-`EVL_BUS_COOP_TARGET_WIDTH)

   `define EVL_BUS_L2SCP_SHIRE_ID_LSB    (23)

   `define EVL_SHIRE_POST_CODE_NUM       (1 << `EVL_SHIRE_POST_CODE_WIDTH)

   `define EVL_GLOBAL_DISABLE            1
   `define EVL_MASTER_DISABLE            2
   `define EVL_PHASE_DISABLE             4
   `define EVL_RAND_STIMULUS_DISABLE     8
   `define EVL_ON_ICE_DISABLE            16

   `define EVL_THREAD_BEGIN              32'hDEAD0000
   `define EVL_THREAD_BOOT_BEGIN         32'hDEAD0001
   `define EVL_THREAD_BOOT_END           32'hDEAD0002
   `define EVL_THREAD_PASS               32'h1FEED000
   `define EVL_THREAD_FAIL               32'h50BAD000

   `define EVL_THREAD_BOOT_BEGIN_HINT    32'h7F002013
   `define EVL_THREAD_BOOT_END_HINT      32'h7F102013

   `define EVL_CORE_RESET_W_HINT         32'h7E002013

   `define REG_VALIDATION0               12'h8d0
   `define REG_VALIDATION1               12'h8d1
   `define REG_VALIDATION2               12'h8d2
   `define REG_VALIDATION3               12'h8d3

   `define EVL_MAX_CYCLE_COUNT           10000

   `define EVL_DBGNOC_EVT_IFC_ID         0
   `define EVL_DBGNOC_MSG_IFC_ID         1

   `define EVL_Bus_Idle                  (~evl_bus_cmd_t'(0))
   `define EVL_Rsp_Null                  (~evl_bus_rsp_type_t'(0))

   `include "dv/common/base/evl_base_macros.svh"
   `include "rtl/inc/soc_defines.vh"

   `ifdef VA_SIZE
      `define EVL_VA_SIZE (`VA_SIZE)
   `else
      `define EVL_VA_SIZE 48
   `endif

   `ifdef VA_SIZE_EXT
      `define EVL_VA_SIZE_EXT (`VA_SIZE_EXT)
   `else
      `define EVL_VA_SIZE_EXT (`VA_SIZE + 1)
   `endif

   `ifdef VA_MSB
      `define EVL_VA_MSB (`VA_MSB)
   `else
      `define EVL_VA_MSB (`EVL_VA_SIZE - 1)
   `endif

   `ifdef VA_EXT_MSB
      `define EVL_VA_EXT_MSB (`VA_EXT_MSB)
   `else
      `define EVL_VA_EXT_MSB (`EVL_VA_SIZE_EXT - 1)
   `endif

   `ifdef FE_FETCH_READ_SIZE
      `define EVL_FE_FETCH_READ_SIZE (`FE_FETCH_READ_SIZE)
   `else
      `define EVL_FE_FETCH_READ_SIZE 256
   `endif

   `ifdef FE_FETCH_ADDR_LSB
      `define EVL_FE_ADDR_LSB (`FE_FETCH_READ_ADDR_SIZE)
   `else
      `define EVL_FE_ADDR_LSB ($clog2(`EVL_FE_FETCH_READ_SIZE / 8))
   `endif


   //-----------------------------------------------------------------------------------------------
   //
   // Although this may seem a bit redundant and confusing, the RTL uses types.  These types are
   // defined in $root, which is not accessible to packages.  As a result, we need to create a
   // package with the same type definitions.  To prevent types from being defined twice in the
   // interface files (which have access to $root), we define new EVL types based off of RTL types
   // on an as-needed basis.  These new EVL types can be imported by the interface files as needed.
   //
   package evl_etlink_types_pkg;
      `include "rtl/inc/et_link_types.vh"

      typedef et_link_req_opcode_t   evl_etlink_req_opcode_t;
      typedef et_link_rsp_opcode_t   evl_etlink_rsp_opcode_t;
      typedef et_link_size_t         evl_etlink_size_t;
      typedef et_link_req_info_t     evl_etlink_req_info_t;
      typedef et_link_rsp_info_t     evl_etlink_rsp_info_t;
      typedef et_link_neigh_source_t evl_etlink_neigh_source_t;
   endpackage : evl_etlink_types_pkg;


   package evl_base_pkg;

      `ifdef ET_USE_UVM
         import uvm_pkg::*;
      `endif

      `ifdef EVL_SIMULATION
         `include "dv/common/base/evl_dpi_calls.vh"
      `endif // !ifdef EVL_SIMULATION

      typedef enum byte {
            VMOD_GLOBAL = 0,
            VMOD_APB_DRV,
            VMOD_APB_IFC,
            VMOD_APB_MON,
            VMOD_APB_SB,
            VMOD_APB_STIM,
            VMOD_AXI_DRV,
            VMOD_AXI_IFC,
            VMOD_AXI_MON,
            VMOD_AXI_SB,
            VMOD_AXI_STIM,
            VMOD_CACHE,
            VMOD_CODE_FLOW,
            VMOD_CONFIG,
            VMOD_COSIM,
            VMOD_COVER,
            VMOD_ETLINK_DRV,
            VMOD_ETLINK_IFC,
            VMOD_ETLINK_MON,
            VMOD_ETLINK_SB,
            VMOD_ETLINK_STIM,
            VMOD_TLINK_DRV,
            VMOD_TLINK_IFC,
            VMOD_TLINK_MON,
            VMOD_TLINK_SB,
            VMOD_TLINK_STIM,
            VMOD_NEMI_DRV,
            VMOD_NEMI_IFC,
            VMOD_NEMI_MON,
            VMOD_NEMI_SB,
            VMOD_NEMI_STIM,
            VMOD_FE_SB,
            VMOD_L1_SB,
            VMOD_L1I_DRV,
            VMOD_L1I_IFC,
            VMOD_L1I_MON,
            VMOD_L1I_SB,
            VMOD_L1I_STIM,
            VMOD_L2_SB_WARN,
            VMOD_L3_SB_WARN,
            VMOD_LOAD_MEM,
            VMOD_MESH_MON,
            VMOD_MINION_MON,
            VMOD_MINION_SB,
            VMOD_MS_IFC,
            VMOD_MS_MON,
            VMOD_MS_SB,
            VMOD_MS_SB_WARN,
            VMOD_NEIGH_MON,
            VMOD_NEIGH_SB,
            VMOD_NOC_SB,
            VMOD_PTW_IFC,
            VMOD_PTW_MON,
            VMOD_PMU_MON,
            VMOD_REG,
            VMOD_REPLAY,
            VMOD_REQ_DESC,
            VMOD_SC_CBUF,
            VMOD_SC_IFC,
            VMOD_SC_MON,
            VMOD_SCP_SB_WARN,
            VMOD_SC_SB,
            VMOD_SC_SB_WARN,
            VMOD_TCL,
            VMOD_TEST,
            VMOD_TIMER,
            VMOD_TRANS_DESC,
            VMOD_UC_SB,
            VMOD_UST,
            VMOD_MESH_HIST,
            VMOD_ECC_INJECT,
            VMOD_JTAG,
            VMOD_JTAG_STIM,
            VMOD_DDR_MON,
            VMOD_DBG_NOC_DRV,
            VMOD_DBG_NOC_IFC,
            VMOD_DBG_NOC_MON,
            VMOD_DBG_NOC_SB,
            VMOD_DBG_NOC_STIM,
            VMOD_PMIC,
            VMOD_MAXION_MON,
            VMOD_SBUS_SB,
            VMOD_SC_BRIDGE_SB,
            VMOD_COH_HUB_SB,
            VMOD_COH_HUB_SB_WARN,
            VMOD_MAXION_L1D_SB,
            VMOD_MAXION_L1D_SB_WARN,
            VMOD_MAXION_IFU,
            VMOD_DESC_STATS,
            VMOD_EVENT_FIFO,
            VMOD_HDL_PH,
            VMOD_HDL_MM,
            VMOD_TEMP
         } evl_verbosity_mod_t;

      typedef bit [`EVL_SHIRE_ID_MSB:0]             evl_shire_id_t;
      typedef bit [`EVL_NEIGHBORHOOD_ID_MSB:0]      evl_neighborhood_id_t;
      typedef bit [`EVL_CORE_ID_MSB:0]              evl_core_id_t;
      typedef bit [`EVL_THREAD_ID_MSB:0]            evl_thread_id_t;
      typedef bit [`EVL_SHIRE_POST_CODE_MSB:0]      evl_shire_post_code_t;
      typedef bit [`EVL_PARTIAL_TRANS_ID_MSB:0]     evl_partial_trans_id_t;
      typedef bit [`EVL_TRANS_ID_MSB:0]             evl_trans_id_t;
      typedef bit [`EVL_POST_CODE_MSB:0]            evl_post_code_t;
      typedef bit [`EVL_FULL_CPU_ID_MSB:0]          evl_full_cpu_id_t;

      typedef bit [`EVL_INSTR_MSB:0]                evl_instr_t;
      typedef bit [`EVL_CMD_LENGTH_MSB:0]           evl_cmd_length_t;
      typedef bit [`EVL_VADDR_MSB:0]                evl_vaddr_t;
      typedef bit [`EVL_PADDR_MSB:0]                evl_paddr_t;
      typedef bit [(`EVL_LINE_ADDR_LSB - 1):0]      evl_line_offset_t;

      typedef bit [`EVL_HWORD_MSB:0]                evl_hword_t;
      typedef bit [`EVL_WORD_MSB:0]                 evl_word_t;
      typedef bit [`EVL_DWORD_MSB:0]                evl_dword_t;
      typedef bit [`EVL_QWORD_MSB:0]                evl_qword_t;
      typedef bit [`EVL_OWORD_MSB:0]                evl_oword_t;
      typedef bit [`EVL_XWORD_MSB:0]                evl_xword_t;

      typedef bit [(`EVL_HWORD_BYTES - 1):0]        evl_hword_byte_en_t;
      typedef bit [(`EVL_WORD_BYTES - 1):0]         evl_word_byte_en_t;
      typedef bit [(`EVL_DWORD_BYTES - 1):0]        evl_dword_byte_en_t;
      typedef bit [(`EVL_QWORD_BYTES - 1):0]        evl_qword_byte_en_t;
      typedef bit [(`EVL_OWORD_BYTES - 1):0]        evl_oword_byte_en_t;
      typedef bit [(`EVL_XWORD_BYTES - 1):0]        evl_xword_byte_en_t;
      typedef bit [(`EVL_LINE_DWORDS - 1):0]        evl_line_dword_en_t;
      typedef bit [(`EVL_LINE_QWORDS - 1):0]        evl_line_qword_en_t;

      typedef bit [`EVL_AGENT_ID_MSB:0]             evl_agent_id_t;
      typedef bit [`EVL_AGENT_ID_KEY_MSB:0]         evl_agent_id_key_t;
      typedef bit [`EVL_AGENT_SEQR_MSB:0]           evl_sequencer_id_t;
      typedef bit [`EVL_AGENT_KEY_MSB:0]            evl_agent_key_t;
      typedef bit [`EVL_DELAY_MSB:0]                evl_delay_t;

      typedef bit [`EVL_BUS_CMD_MSB:0]              evl_bus_cmd_t;
      typedef bit [`EVL_BUS_REQ_ID_MSB:0]           evl_bus_req_id_t;
      typedef bit [`EVL_BUS_REQ_SOURCE_MSB:0]       evl_bus_req_source_t;
      typedef bit [`EVL_BUS_REQ_SIZE_MSB:0]         evl_bus_req_size_t;
      typedef bit [`EVL_BUS_REQ_LEN_MSB:0]          evl_bus_req_len_t;
      typedef bit [`EVL_BUS_REQ_BURST_MSB:0]        evl_bus_req_burst_t;
      typedef bit [`EVL_BUS_SUBOPCODE_MSB:0]        evl_bus_subopcode_t;
      typedef bit [`EVL_BUS_LOCK_MSB:0]             evl_bus_lock_t;
      typedef bit [`EVL_BUS_CACHE_ATTR_MSB:0]       evl_bus_cache_attr_t;
      typedef bit [`EVL_BUS_PROT_MSB:0]             evl_bus_prot_t;
      typedef bit [`EVL_BUS_QOS_MSB:0]              evl_bus_qos_t;
      typedef bit [`EVL_BUS_REGION_MSB:0]           evl_bus_region_t;
      typedef bit [`EVL_BUS_REQ_USER_MSB:0]         evl_bus_req_user_t;
      typedef bit [`EVL_BUS_REQ_VC_MSB:0]           evl_bus_req_vc_t;
      typedef bit [`EVL_BUS_RSP_USER_MSB:0]         evl_bus_rsp_user_t;
      typedef bit [`EVL_BUS_RSP_TYPE_MSB:0]         evl_bus_rsp_type_t;
      typedef bit [`EVL_BUS_RSP_SIZE_MSB:0]         evl_bus_rsp_size_t;
      typedef bit [`EVL_BUS_RSP_ID_MSB:0]           evl_bus_rsp_id_t;
      typedef bit [`EVL_BUS_RSP_DEST_MSB:0]         evl_bus_rsp_dest_t;
      typedef bit [`EVL_BCST_REGION_ID_MSB:0]       evl_bus_req_bcast_rgn_t;
      typedef bit [`EVL_NOC_SRC_IFC_MSB:0]          evl_noc_src_ifc_t;
      typedef bit [`EVL_NOC_LAYER_MSB:0]            evl_noc_layer_t;

      typedef bit [`EVL_BUS_COP_TYPE_MSB:0]         evl_bus_cop_type_t;
      typedef bit [`EVL_BUS_COP_START_LEVEL_MSB:0]  evl_bus_cop_start_level_t;
      typedef bit [`EVL_BUS_COP_DEST_LEVEL_MSB:0]   evl_bus_cop_dest_level_t;
      typedef bit [`EVL_BUS_ATOMIC_CONF_MSB:0]      evl_bus_atomic_conf_t;
      typedef bit [`EVL_BUS_ATOMIC_TRANSID_MSB:0]   evl_bus_atomic_transid_t;
      typedef bit [`EVL_BUS_MESSAGE_CONF_WIDTH:0]   evl_bus_message_conf_t;
      typedef bit [`EVL_BUS_COOP_TARGET_MSB:0]      evl_bus_coop_target_t;
      typedef bit [`EVL_BUS_COOP_NEIGH_MASK_MSB:0]  evl_bus_neigh_coop_mask_t;
      typedef bit [`EVL_BUS_COOP_MINION_MASK_MSB:0] evl_bus_minion_coop_mask_t;
      typedef bit [`EVL_BUS_COOP_ID_WIDTH-1:0]      evl_bus_coop_id_t;
      typedef bit [`EVL_BUS_COOP_WAY_WIDTH-1:0]     evl_bus_coop_way_t;

      typedef bit [`SC_BANK_ID_SIZE:0]              evl_sc_bank_id_t;
      typedef bit [`SC_REQQ_ID_SIZE-1:0]            evl_sc_reqq_id_t;

      typedef bit [`EVL_MAX_JTAG_LENGTH-1:0]        evl_jtag_data_t;
      typedef bit [15:0]                            evl_jtag_length_t;
      typedef evl_verbosity_mod_t evl_verbosity_mod_array_t[];
   
   

   typedef enum int {
            BuildPhase         = 0,
            PreResetPhase      = 1,
            ResetPhase         = 2,
            PostResetPhase     = 3,
            PreConfigurePhase  = 4,
            ConfigurePhase     = 5,
            PostConfigurePhase = 6,
            PreMainPhase       = 7,
            MainPhase          = 8,
            PostMainPhase      = 9,
            PreShutdownPhase   = 10,
            ShutdownPhase      = 11,
            PostShutdownPhase  = 12,
            ExtractPhase       = 13,
            CheckPhase         = 14,
            ReportPhase        = 15,
            FinalPhase         = 16
         } evl_phase_type_t;

      typedef enum bit [3:0] {
            EVL_DESC_UNKNOWN,
            EVL_DESC_TABLE,
            EVL_DESC_LINE_BASE,
            EVL_DESC_LINE,
            EVL_PTE_DESC,
            EVL_DESC_WAY,
            EVL_DESC_TRANS,
            EVL_DESC_REQ,
            EVL_DESC_PTW,
            EVL_DESC_VPU,
            EVL_DESC_PMU
         } evl_desc_type_t;

      typedef enum bit [1:0] {
            EVL_CoreUnknown,
            EVL_MinionCore,
            EVL_MinionSPCore,
            EVL_MaxionCore
         } evl_core_type_t;

      typedef enum bit [3:0] {
            EVL_BUS_TYPE_UNKNOWN,
            EVL_BUS_TYPE_ETLINK,
            EVL_BUS_TYPE_TLINK,
            EVL_BUS_TYPE_AXI,
            EVL_BUS_TYPE_APB,
            EVL_BUS_TYPE_JTAG,
            EVL_BUS_TYPE_DBG_NOC,
            EVL_BUS_TYPE_NEMI,
            EVL_BUS_TYPE_FE_MINION
         } evl_bus_type_t;

      typedef enum bit [`EVL_CMD_MSB:0] {
            NOCMD,
            NOP,
            BARRIER,
            LD,
            LD_COOP,
            LD_NO_MOD,
            ST,
            ST_WA,
            ST_NO_MOD,
            ST_DBG_NOC,
            LD_RA,
            EVICT,
            CACHE_LOCK,
            CACHE_LOCK_SW,
            CACHE_UNLOCK,
            CACHE_UNLOCK_SW,
            CACHE_EVICT,
            CACHE_EVICT_SW,
            CACHE_EVICT_TO_MEM,
            CACHE_EVICT_ALL,
            CACHE_FLUSH,
            CACHE_FLUSH_SW,
            CACHE_FLUSH_TO_MEM,
            CACHE_PREFETCH,
            CACHE_SCP_FILL,
            TENSOR_LOAD,
            TENSOR_STORE,
            TENSOR_STORE_SCP,
            TENSOR_REDUCE,
            VPU_SCP_READ,
            MESSAGE,
            ATOMIC,
            JTAG_STEP,
            JTAG_RESET,
            JTAG_RUN_TEST,
            SELECT_CHAIN,
            JTAG_RD,
            JTAG_WR,
            JTAG_RD_PARTIAL,
            JTAG_WR_PARTIAL,
            IFETCH,
            REFETCH,
            CANCEL,
            LR,
            SC,
            CACHE_FLUSH_EA,
            CACHE_FLUSH_ALL
         } evl_cmd_t;

      `define EVL_LD_ST_CMDS                LD, LD_COOP, LD_NO_MOD, ST, ST_WA, ST_NO_MOD, ST_DBG_NOC, LD_RA, LR, SC, IFETCH, REFETCH
      `define EVL_JTAG_CMDS_WITH_READ_DATA  JTAG_STEP, SELECT_CHAIN, JTAG_RD, JTAG_WR, JTAG_RD_PARTIAL, JTAG_WR_PARTIAL
      `define EVL_CMDS_WITH_READ_DATA       LD, LD_NO_MOD, LD_RA, LD_COOP, ATOMIC, `EVL_JTAG_CMDS_WITH_READ_DATA, LR, IFETCH, REFETCH
      `define EVL_JTAG_CMDS_WITH_WRITE_DATA JTAG_STEP, SELECT_CHAIN, JTAG_WR, JTAG_WR_PARTIAL
      `define EVL_CMDS_WITH_WRITE_DATA      LD_COOP, ST, ST_NO_MOD, ST_DBG_NOC, ST_WA, EVICT, MESSAGE, ATOMIC, CACHE_EVICT_TO_MEM, CACHE_FLUSH_TO_MEM, `EVL_JTAG_CMDS_WITH_WRITE_DATA, SC
      `define EVL_COP_CMDS                  CACHE_LOCK_SW, CACHE_UNLOCK_SW, CACHE_LOCK, CACHE_UNLOCK, CACHE_EVICT, CACHE_EVICT_SW, CACHE_EVICT_TO_MEM, CACHE_EVICT_ALL, CACHE_FLUSH, CACHE_FLUSH_SW, CACHE_FLUSH_TO_MEM, CACHE_FLUSH_EA, CACHE_FLUSH_ALL, CACHE_PREFETCH, CACHE_SCP_FILL
      `define EVL_IDX_COP_CMDS              CACHE_EVICT_SW, CACHE_FLUSH_SW, CACHE_LOCK_SW, CACHE_UNLOCK_SW
      `define EVL_TENSOR_CMDS               TENSOR_LOAD, TENSOR_STORE, TENSOR_STORE_SCP, TENSOR_REDUCE
      `define EVL_JTAG_CMDS                 JTAG_STEP, JTAG_RESET, JTAG_RUN_TEST, SELECT_CHAIN, JTAG_RD, JTAG_WR, JTAG_RD_PARTIAL, JTAG_WR_PARTIAL

      typedef enum evl_bus_subopcode_t {
            dest_l2  = 0,
            dest_l3  = 1,
            dest_scp = 2
         } evl_subopcode_dest_t;

      typedef enum bit [3:0] {
            EVL_WAY_0     = 0,
            EVL_WAY_1     = 1,
            EVL_WAY_2     = 2,
            EVL_WAY_3     = 3,
            EVL_WAY_4     = 4,
            EVL_WAY_5     = 5,
            EVL_WAY_6     = 6,
            EVL_WAY_7     = 7,
            EVL_WAY_MULTI = 8,
            EVL_WAY_SBE   = 9,
            EVL_WAY_MBE   = 10,
            EVL_WAY_NONE  = 15
         } evl_way_t;

      typedef enum bit [`EVL_ADDR_SP_MSB:0] {
            Mem,
            L1Cache,
            L2Cache,
            L3Cache
         } evl_addr_sp_t;

      typedef enum bit [`EVL_CACHE_ST_MSB:0] {
            I           = 8'b00000000, // Invalid line
            S           = 8'b00000001, // Shared line
            E           = 8'b00000010, // Exclusive line
            M           = 8'b00000011, // Modified line
            L           = 8'b00000100, // Locked clean line
            D           = 8'b00000101, // Locked dirty line
            C           = 8'b00000110, // Soft Locked Clean line
            T           = 8'b00000111, // Soft Locked Trashed (dirty) line
            W           = 8'b00001000, // Write-around line that is currently coalescing
            St_Stable   = 8'b00001001, // Anything below this is a stable cache state
            St_0xa      = 8'b00001010, // Unused
            St_0xb      = 8'b00001011, // Unused
            St_0xc      = 8'b00001100, // Unused
            St_0xd      = 8'b00001101, // Unused
            St_0xe      = 8'b00001110, // Unused
            St_0xf      = 8'b00001111, // Unused
            St_toT      = 8'b00010000, // TileLink toT cap param (bits 2:0 = 0)
            St_toB      = 8'b00010001, // TileLink toB cap param (bits 2:0 = 1)
            St_toN      = 8'b00010010, // TileLink toN cap param (bits 2:0 = 2)
            St_TtoT     = 8'b00010011, // TileLink TtoT report param (bits 2:0 = 3)
            St_BtoB     = 8'b00010100, // TileLink BtoB report param (bits 2:0 = 4)
            St_NtoN     = 8'b00010101, // TileLink NtoN report param (bits 2:0 = 5)
            St_Conflict = 8'b00010110,
            St_Ignore   = 8'b00010111,
            St_NtoB     = 8'b00011000, // TileLink NtoB grow param (bits 2:0 = 0)
            St_NtoT     = 8'b00011001, // TileLink NtoT grow param (bits 2:0 = 1)
            St_BtoT     = 8'b00011010, // TileLink BtoT grow param (bits 2:0 = 2)
            Stale       = 8'b00011011, // Ignore model state on a response (info in request descriptor)
            Stale_I     = 8'b00011100, // Ignore model state on a response and invalidate (info in request descriptor)
            Stale_S     = 8'b00011101, // Ignore model state on a response and set to shared (info in request descriptor)
            Stale_E     = 8'b00011110, // Ignore model state on a response and set to exclusive (info in request descriptor)
            Stale_M     = 8'b00011111, // Ignore model state on a response and set to modified (info in request descriptor)
            St_TtoB     = 8'b00100000, // TileLink TtoB prune param (bits 2:0 = 0)
            St_TtoN     = 8'b00100001, // TileLink TtoN prune param (bits 2:0 = 1)
            St_BtoN     = 8'b00100010, // TileLink BtoN prune param (bits 2:0 = 2)
            I_Bsy,                     // Line is invalid but a downstream request is outstanding
            I_IE_Bsy,
            E_Bsy,
            L_Bsy,
            M_Bsy,
            D_Bsy,                     // Lock, but a request/response is pending
            W_Bsy,                     // Eviction of a write-around line (unused?)
            IE_Bsy,                    // Read miss in progress (TC req to AXI repsonse)
            IL_Bsy,
            IM_Bsy,
            ID_Bsy,
            IS_Bsy,                    // for Maxion (MSI): Read miss in progress
            SM_Bsy,                    // for Maxion (MSI): Write hitting S
            SE_Bsy,                    // for Maxion (MSI): ?
            SI_Bsy,                    // for Maxion (MSI): eviction of a S line
            EI_Bsy,
            LI_Bsy,
            LD_Bsy,
            CT_Bsy,
            LE_Bsy,
            EM_Bsy,
            EL_Bsy,
            ED_Bsy,
            MI_Bsy,                     // Eviction of a modified line in progress
            MI_I_Bsy,                   // Eviction of a modified line, followed by something else (ex: FlushToMem, EvictToMem)
            MI_M_Bsy,                   // Eviction of a modified line, followed by a write
            MI_D_Bsy,                   // Eviction of a modified line, followed by a lock
            MI_W_Bsy,                   // Eviction of a modified line, followed by a partial write-around
            MI_MI_Bsy,                  // Eviction of a modified line, followed by a write eviction
            MI_WI_Bsy,                  // Eviction of a modified line, followed by a write-around eviction
            MI_IE_Bsy,                  // Eviction of a modified line, followed by a read miss
            MI_MemBsy,                  // Eviction of a modified line, waiting for (Flush/Evict)ToMem to be sent from tag_rsp stage
            A1_MI_Bsy,
            WI_MemBsy,                  // Eviction of a write-around line, waiting for (Flush/Evict)ToMem to be sent from tag_rsp stage
            WI_MI_MemBsy,               // Eviction of a write-around line, waiting for (Flush/Evict)ToMem to be sent from tag_rsp stage
            WI_ME_MemBsy,               // Eviction of a write-around line, followed by a write, waiting for (Flush)ToMem to be sent from tag_rsp stage
            WI_WI_MemBsy,               // Eviction of a write-around line, waiting for (Flush/Evict)ToMem to be sent from tag_rsp stage
            ME_Bsy,                     // Flush in progress (cline will be kept in E)
            MD_Bsy,
            ME_E_Bsy,                   // Flush in progress, followed by something else (ex: FlushToMem)
            ME_MemBsy,                  // Clean of modified line, waiting for (Flush/Evict)ToMem to be sent from tag_rsp stage
            MS_Bsy,                     // Flush in progress (cline will be kept in S)
            DI_Bsy,
            DM_Bsy,
            DL_Bsy,
            WI_Bsy,                     // Eviction of a write-around line in progress
            A1_WI_Bsy,
            A2_WI_Bsy,
            WI_IE_Bsy,                  // Eviction of a write-around line in progress, followed by a pending read
            MI_IL_Bsy,
            MI_ID_Bsy,
            WI_ID_Bsy,
            DI_IE_Bsy,
            DI_IL_Bsy,
            DI_ID_Bsy,
            EI_Pend,
            WM_Bsy,                     // state is going from W to M
            A1_WM_Bsy,
            WD_Bsy,
            WIE_Bsy,
            IEM_Bsy,                    // starting a read-modify-write
            A1_IEM_Bsy,
            A2_IEM_Bsy,
            WEM_Bsy,                    // starting a read-modify-write with write-around data
            A1_I_Bsy,                   // We are waiting for 1 write ack and the cache st is invalid
            A2_I_Bsy,
            A3_I_Bsy,
            A1_E_Bsy,
            A2_E_Bsy,
            A1_M_Bsy,
            A2_M_Bsy,
            A1_D_Bsy,
            A2_D_Bsy,
            A1_L_Bsy,
            A2_L_Bsy,
            A1_W_Bsy,
            A2_W_Bsy,
            A1_IE_Bsy,
            A2_IE_Bsy,
            A1_EI_Bsy,
            A1_ME_Bsy,
            A1_MI_MemBsy,
            A1_ME_MemBsy,
            A1_WI_MemBsy,
            A1_WIE_Bsy,
            A1_EM_Bsy,
            A1_Stale_M_Bsy,
            St_L3ChkI,
            St_L3ChkE,
            St_L3ChkEM,
            St_L3ChkM,
            St_SysChkI,
            St_SysChkE,
            St_SysChkM
         } evl_cache_st_t;


      `define EVL_LINE_DEALLOC_STATES  I, I_Bsy, A1_I_Bsy, A2_I_Bsy, EI_Bsy, A1_EI_Bsy, EI_Pend, LI_Bsy, DI_Bsy, MI_Bsy, A1_MI_Bsy, MI_IE_Bsy, MI_I_Bsy, MI_MI_Bsy, MI_MemBsy, A1_MI_MemBsy, WI_Bsy, A1_WI_Bsy, A2_WI_Bsy, WI_MemBsy, A1_WI_MemBsy, Stale_I, WIE_Bsy, A1_WIE_Bsy, A1_IEM_Bsy, A2_IEM_Bsy, IEM_Bsy

      `define EVL_LINE_VALID_STATES    S, E, L, E_Bsy, ME_Bsy, ME_MemBsy, A1_E_Bsy, A2_E_Bsy, A1_ME_Bsy, A1_ME_MemBsy

      `define EVL_LINE_DIRTY_STATES    M, D, M_Bsy, D_Bsy, MI_M_Bsy, MI_D_Bsy, A1_M_Bsy, A2_M_Bsy, A1_D_Bsy, A2_D_Bsy, WIE_Bsy, A1_WIE_Bsy, WM_Bsy, A1_WM_Bsy, WD_Bsy

      `define EVL_LINE_WA_DIRTY_STATES W, W_Bsy, MI_W_Bsy, A1_W_Bsy, A2_W_Bsy, WEM_Bsy

      `define EVL_LINE_LOCK_STATES     L, D, L_Bsy, D_Bsy, MI_D_Bsy, WD_Bsy, A1_D_Bsy, A2_D_Bsy, A1_L_Bsy

      typedef enum bit [`EVL_DATA_ACTION_MSB:0] {
            Data_None,
            Data_ReqWr,
            Data_ReqWrBack,          // Get writeback data from the cache
            Data_MrgWrBack,          // Merge into the cache and get writeback data from the cache
            Data_ReqWrNull,          // Set the write data to be null
            Data_ReqWrNorm,          // Send normalized data in a new request
            Data_ReqRd,
            Data_ReqRdNorm,
            Data_L3AtomicRd,
            Data_CacheFill,          // Perform cache fill (TC stage) or mark it as a fill (bus response)
            Data_ZeroFill,           // Zero the cache line data (TC stage)
            Data_ZeroFillAlloc,      // Allocate the way and Zero the cache line data (TC stage)
            Data_CascadeFill,        // Cascade the response to the source and mark it as a fill
            Data_CascadeFillZero,    // Cascade the response to the source and mark it as a fill with zeroes
            Data_CascadeRsp,         // Cascade the response
            Data_EnqueueFill,        // Simply enqueue the current request as a fill
            Data_ForwardMsg,         // Forward the message in the response field of a new descriptor
            Data_MemWr,
            Data_MemWrAll,           // Do the write and set all qwen enables
            Data_MemWrInst,          // A write is installing into the cache (previous state invalid)
            Data_MemWrRd,            // Do the write, respond with data
            Data_MemWrRdNorm,        // Do the write, respond with normalized data
            Data_MemRd,
            Data_FwdRd,              // Cascade read response data from one descriptor to its parent
            Data_FwdWr,              // Cascade write data from one descriptor to its child
            Data_FwdAndWr,
            Data_FwdAndCheck,
            Data_FwdAtomicRsp,       // Forward an atomic response
            Data_ZeroRsp,            // Response data is zeros.
            Data_ReqWrBack_MemWr,    // 2 actions in one!
            Data_ReqWrBack_ZeroFill, // 2 actions in one!
            Data_Create,             // trans_desc didn't have data so agent needs to create some
            Data_WrAtomic
         } evl_data_action_t;

      typedef enum evl_bus_rsp_type_t {
            RspAck,
            RspWData,
            RspMsgData,
            RspErr,
            RspDecErr,
            RspNull = `EVL_Rsp_Null
         } evl_abstract_rsp_type_t;

      typedef enum evl_bus_cmd_t {
            BusRead,
            BusReadCoop,
            BusLineRd,
            BusReadParent,
            BusWrite,
            BusWriteAround,
            BusWriteAtomic,
            BusWriteMsg,
            BusLocalMsg,
            BusVictim,
            BusFlush,
            BusFlushToMem,
            BusEvict,
            BusEvictToMem,
            BusPrefetch,
            BusSysPrefetch,
            BusIFetch,
            BusCancel,
            BusRefetch,
            BusIdle = `EVL_Bus_Idle
         } evl_abstract_bus_cmd_t;

      typedef enum bit [1:0] {
            FE_Req_IFetch,
            FE_Req_Cancel,
            FE_Req_Refetch,
            FE_Req_Unknown
         } evl_ifetch_bus_cmd_t;

      typedef enum bit [1:0] {
            FE_Rsp_IFetch,
            FE_Rsp_Cancel,
            FE_Rsp_Refetch,
            FE_Rsp_Unknown
         } evl_ifetch_bus_rsp_t;

      typedef enum int {
            SC_Source_L2,
            SC_Source_Scp,
            SC_Source_L3
         } evl_sc_scoreboard_src;

      typedef enum int {
            EC_None = 0,
            EC_Replay,       // Force request to be replayed
            EC_EnqWA,        // Enqueue the partial write address
            EC_UpdWAMru,     // Update the coalescing buffer MRU for the partial write address
            EC_DeqWA,        // Dequeue the partial write address (clear quadword dirty bits implied)
            EC_Victim,       // Dequeue the partial write address (dequeue implied)
            EC_Victim_ClrQW, // Dequeue the partial write address (dequeue implied, clear quadword dirty bits)
            EC_Victim_SetQW, // Dequeue the partial write address, set new quadword dirty bits
            EC_ClrQW,        // Clear the quadword dirty bits
            EC_SetQW,        // Set all quadword dirty bits
            EC_SetQW_DeqWA,  // Set all quadword dirty bits, dequeue the partial write address
            EC_CheckLock,    // Check for a lock of all ways
            EC_LateRead,     // A read request should be issued after the write response
            EC_DeallocReqq,  // Deallocate the just-allocated request queue entry
            EC_FwdReq,       // Request needs to be forwarded to next level memory
            EC_IncrCount,    // Increment the pending bus request per line count
            EC_DecrCount,    // Decrement the pending bus request per line count
            EC_Lock,         // Line is to be soft locked
            EC_Unlock,       // Line is to be soft unlocked
            EC_Error01,
            EC_Error02,
            EC_Error03,
            EC_Error04,
            EC_Error05,
            EC_Error06,
            EC_Error07,
            EC_Error08,
            EC_Error09,
            EC_Error10,
            EC_Error11,
            EC_Error12,
            EC_Error13,
            EC_Error14,
            EC_Error15,
            EC_MaybeErrRsp,
            EC_ErrRsp,
            EC_Stale,
            EC_LastEC        // This should always be the last EC value ... insert new ones above this.
         } evl_error_code;

      typedef enum bit [`EVL_AGENT_TYPE_MSB:0] {
            NO_AGENT,
            SYSTEM_AGENT,
            SOC_RTL_AGENT,
            SHIRE_RTL_AGENT,
            SHIRE_CHANNEL_RTL_AGENT,
            SHIRE_CACHE_RTL_AGENT,
            MEM_SHIRE_SIDE_RTL_AGENT,
            MEM_SHIRE_RTL_AGENT,
            DDR_SHIRE_RTL_AGENT,
            IO_SHIRE_RTL_AGENT,
            PSHIRE_RTL_AGENT,
            PSHIRE_NOC_RTL_AGENT,
            MAXSHIRE_RTL_AGENT,
            NEIGH_RTL_AGENT,
            MINION_ICACHE_RTL_AGENT,
            STANDALONE_MINION_RTL_AGENT,
            CORE_RTL_AGENT,
            MAXION_RTL_AGENT,
            MAXION_HUB_RTL_AGENT,
            MAXION_SBUS_RTL_AGENT,
            NOC_RTL_AGENT,
            LPDDR_RTL_AGENT,
            VPN_RTL_AGENT,
            REG_RTL_AGENT,
            SHIRE_STIM_AGENT,
            SHIRE_CHANNEL_STIM_AGENT,
            SHIRE_CACHE_STIM_AGENT,
            NEIGH_STIM_AGENT,
            ETLINK_MASTER_STIM_AGENT,
            ETLINK_SLAVE_STIM_AGENT,
            TLINK_MASTER_STIM_AGENT,
            TLINK_SLAVE_STIM_AGENT,
            CORE_STIM_AGENT,
            FE_STIM_AGENT,
            NOC_STIM_AGENT,
            VPN_STIM_AGENT,
            MEM_STIM_AGENT,
            L2_MASTER_STIM_AGENT,
            L3_MASTER_STIM_AGENT,
            SBM_MASTER_STIM_AGENT,
            FROMSYS_MASTER_STIM_AGENT,
            MEM_MASTER_STIM_AGENT,
            UST_MASTER_STIM_AGENT,
            UST_SLAVE_STIM_AGENT,
            SPIO_E2A_MASTER_STIM_AGENT,
            SPIO_RAM_MASTER_STIM_AGENT,
            SPIO_ROM_MASTER_STIM_AGENT,
            MESH_SLAVE_STIM_AGENT,
            MESH_IO_SLAVE_STIM_AGENT,
            DDR_SLAVE_STIM_AGENT,
            REG_SLAVE_STIM_AGENT,
            SHIRE_AXI_SLAVE_STIM_AGENT,
            GENERIC_AXI_MASTER_STIM_AGENT,
            GENERIC_AXI_SLAVE_STIM_AGENT,
            GENERIC_AXI_RTL_AGENT,
            SHIRE_AGENT,
            SHIRE_CACHE_AGENT,
            MEM_SHIRE_AGENT,
            IO_SHIRE_AGENT,
            PSHIRE_AGENT,
            PSHIRE_NOC_AGENT,
            NEIGH_AGENT,
            STANDALONE_MINION_AGENT,
            CORE_AGENT,
            ICACHE_AGENT,
            NOC_AGENT,
            MEM_AGENT,
            AXI_MASTER_STIM_AGENT,
            AXI_SLAVE_STIM_AGENT,
            APB_MASTER_STIM_AGENT,
            APB_SLAVE_STIM_AGENT,
            TOSYS_MASTER_STIM_AGENT,
            TOSYS_MASTER,
            ATOMIC_RSP_MASTER_STIM_AGENT,
            ATOMIC_RSP_MASTER,
            IOTOSYS_MASTER_STIM_AGENT,
            IOTOSYS_MASTER,
            SPIO_VAULT_MASTER_STIM_AGENT,
            SPIO_VAULT_MASTER,
            SPIO_AM_MASTER_STIM_AGENT,
            SPIO_AM_MASTER,
            SPIO_DMA_MASTER_STIM_AGENT,
            SPIO_DMA_MASTER,
            PU_DMA_AM_MASTER_STIM_AGENT,
            PU_DMA_AM_MASTER,
            PU_DMA_MASTER_STIM_AGENT,
            PU_DMA_MASTER,
            PU_EMMC_MASTER_STIM_AGENT,
            PU_EMMC_MASTER,
            PU_USB_MASTER_STIM_AGENT,
            PU_USB_MASTER,
            PSHIRE_TOSYS_MASTER_STIM_AGENT,
            PSHIRE_TOSYS_MASTER,
            PSHIRE_PCIE_MASTER_STIM_AGENT,
            PSHIRE_PCIE_MASTER,
            PSHIRE_ESR_MASTER_STIM_AGENT,
            PSHIRE_ESR_MASTER,
            PSHIRE_DBG_MASTER_STIM_AGENT,
            PSHIRE_DBG_MASTER,
            IO_PM_MASTER_STIM_AGENT,
            IO_PM_MASTER,
            UCTOSYS_MASTER_STIM_AGENT,
            UCTOSYS_MASTER,
            UCTOL3_MASTER_STIM_AGENT,
            UCTOL3_MASTER,
            L2TOL3_MASTER_STIM_AGENT,
            L2TOL3_MASTER,
            RBM_MASTER_STIM_AGENT,
            RBM_MASTER,
            DBG_NOC_MASTER_STIM_AGENT,
            DBG_NOC_MASTER,
            ETLINK_BUS_AGENT,
            TLINK_BUS_AGENT,
            AXI_BUS_AGENT,
            APB_BUS_AGENT,
            NEMI_BUS_AGENT,
            PTW_BUS_AGENT,
            JTAG_BUS_AGENT,
            JTAG_STIM_AGENT,
            DBG_NOC_BUS_AGENT,
            FE_MINION_BUS_AGENT,
            DBG_NOC_STIM_AGENT,
            BPAM_MASTER_STIM_AGENT
         } evl_agent_type_t;

      typedef enum int {
            NO_DPI_SOURCE            = 0,
            AXI_TO_L3MESH            = 1,
            AXI_FROM_L3MESH          = 2,
            AXI_TO_SYSMESH           = 3,
            AXI_FROM_SYSMESH         = 4,
            AXI_FROM_SYSMESH_TO_SBM  = 5,
            AXI_FROM_UST             = 6,
            AXI_UC_TO_L3MESH         = 7,
            AXI_UC_TO_SYSMESH        = 8,
            AXI_TO_DDRC_LO           = 9,
            AXI_TO_DDRC_HI           = 10
         } evl_dpi_source_t;

      typedef enum int {
            NO_SCOREBOARD,
            L2_SCOREBOARD,
            L3_SCOREBOARD,
            SCP_SCOREBOARD
         } evl_scoreboard_type_t;

      `define EVL_BUS_AGENT_TYPES  ETLINK_BUS_AGENT, AXI_BUS_AGENT, APB_BUS_AGENT, PTW_BUS_AGENT, JTAG_BUS_AGENT, DBG_NOC_BUS_AGENT, TLINK_BUS_AGENT, NEMI_BUS_AGENT, FE_MINION_BUS_AGENT
      `define EVL_STIM_AGENT_TYPES SHIRE_STIM_AGENT, SHIRE_CACHE_STIM_AGENT, NEIGH_STIM_AGENT, CORE_STIM_AGENT, NOC_STIM_AGENT, MEM_STIM_AGENT, L2_MASTER_STIM_AGENT, L3_MASTER_STIM_AGENT, UCTOSYS_MASTER_STIM_AGENT, UCTOL3_MASTER_STIM_AGENT, L2TOL3_MASTER_STIM_AGENT, TOSYS_MASTER_STIM_AGENT, ATOMIC_RSP_MASTER_STIM_AGENT, SBM_MASTER_STIM_AGENT, MEM_MASTER_STIM_AGENT, UST_MASTER_STIM_AGENT, SPIO_E2A_MASTER_STIM_AGENT, SPIO_RAM_MASTER_STIM_AGENT, SPIO_ROM_MASTER_STIM_AGENT, MESH_SLAVE_STIM_AGENT, DDR_SLAVE_STIM_AGENT, REG_SLAVE_STIM_AGENT, GENERIC_AXI_MASTER_STIM_AGENT, GENERIC_AXI_SLAVE_STIM_AGENT, JTAG_STIM_AGENT, DBG_NOC_STIM_AGENT, FE_STIM_AGENT
      `define EVL_RTL_AGENT_TYPES  SYSTEM_AGENT, SOC_RTL_AGENT, SHIRE_RTL_AGENT, SHIRE_CHANNEL_RTL_AGENT, SHIRE_CACHE_RTL_AGENT, MEM_SHIRE_SIDE_RTL_AGENT, MEM_SHIRE_RTL_AGENT, DDR_SHIRE_RTL_AGENT, IO_SHIRE_RTL_AGENT, MAXSHIRE_RTL_AGENT, PSHIRE_RTL_AGENT, PSHIRE_NOC_RTL_AGENT, NEIGH_RTL_AGENT, STANDALONE_MINION_RTL_AGENT, CORE_RTL_AGENT, MAXION_RTL_AGENT, MAXION_HUB_RTL_AGENT, MAXION_SBUS_RTL_AGENT, NOC_RTL_AGENT, VPN_RTL_AGENT, MINION_ICACHE_RTL_AGENT

      typedef struct packed unsigned {
            evl_paddr_t                scp_fill_addr;
            evl_bus_cop_start_level_t  cop_start_level;
            evl_bus_cop_dest_level_t   cop_dest_level;
            bit                        cop_final_state;
            evl_shire_id_t             src_shire_id;
            evl_bus_atomic_conf_t      atomic_conf;
            evl_line_offset_t          atomic_offset;
            evl_bus_atomic_transid_t   atomic_transid;
            evl_bus_subopcode_t        subopcode_dest;
            evl_bus_message_conf_t     message_conf;
            evl_bus_coop_target_t      coop_target;
            evl_bus_neigh_coop_mask_t  coop_neigh_mask;
            evl_bus_minion_coop_mask_t coop_minion_mask;
            evl_bus_coop_id_t          coop_id;
            evl_bus_coop_way_t         coop_way;
         } evl_trans_attr_s;

      typedef struct packed unsigned {
            evl_paddr_t scp_base_addr;
            evl_paddr_t scp_top_addr;
            evl_paddr_t scp_index_mask;
            evl_paddr_t scp_alias_mask;
            evl_paddr_t l2_base_addr;
            evl_paddr_t l2_top_addr;
            evl_paddr_t l2_index_mask;
            evl_paddr_t l2_alias_mask;
            evl_paddr_t l3_base_addr;
            evl_paddr_t l3_top_addr;
            evl_paddr_t l3_index_mask;
            evl_paddr_t l3_alias_mask;
            int         scp_size;
            int         l2_size;
            int         l3_size;
            bit         l2_bypass;
            bit         l3_bypass;
            bit         scp_remote_enable;
            bit         l3_remote_enable;
         } evl_sc_config_attr_s;

      typedef enum byte {
            DIAG_NOP            = 0,
            DIAG_PUTCHAR        = 1,
            DIAG_RAND           = 2,
            DIAG_RAND_MEM_UPPER = 3,
            DIAG_RAND_MEM_LOWER = 4,
            DIAG_UEI            = 5,
            DIAG_ECC_INJ        = 6,
            DIAG_CYCLE          = 7,
            DIAG_RSP_DELAY      = 8
         } evl_diag_cmd_t;

      typedef enum bit [3:0] {
            SC_AMOSWAP     = 4'b0000,
            SC_AMOADD      = 4'b0001,
            SC_AMOXOR      = 4'b0010,
            SC_AMOAND      = 4'b0011,
            SC_AMOOR       = 4'b0100,
            SC_AMOMIN      = 4'b0101,
            SC_AMOMAX      = 4'b0110,
            SC_AMOMINU     = 4'b0111,
            SC_AMOMAXU     = 4'b1000,
            SC_AMOMIN_PS   = 4'b1001,
            SC_AMOMAX_PS   = 4'b1010,
            SC_AMOCOMPEXCH = 4'b1011,
            SC_AMO_0C      = 4'b1100,
            SC_AMO_0D      = 4'b1101,
            SC_AMO_0E      = 4'b1110,
            SC_AMO_0F      = 4'b1111
         } evl_atomic_op_t;

      typedef enum bit [2:0] {
            pte_pointer         = 3'b000,
            pte_read_only       = 3'b001,
            pte_reserved_010    = 3'b010,
            pte_read_write      = 3'b011,
            pte_exec_only       = 3'b100,
            pte_read_exec       = 3'b101,
            pte_reserved_110    = 3'b110,
            pte_read_write_exec = 3'b111
         } evl_pte_attr_t;

      typedef enum bit [3:0] {
            vm_mode_bare   = 4'b0000,
            vm_mode_rsvd_1 = 4'b0001,
            vm_mode_rsvd_2 = 4'b0010,
            vm_mode_rsvd_3 = 4'b0011,
            vm_mode_rsvd_4 = 4'b0100,
            vm_mode_rsvd_5 = 4'b0101,
            vm_mode_rsvd_6 = 4'b0110,
            vm_mode_rsvd_7 = 4'b0111,
            vm_mode_sv39   = 4'b1000,
            vm_mode_sv48   = 4'b1001,
            vm_mode_rsvd_a = 4'b1010,
            vm_mode_rsvd_b = 4'b1011,
            vm_mode_rsvd_c = 4'b1100,
            vm_mode_rsvd_d = 4'b1101,
            vm_mode_rsvd_e = 4'b1110,
            vm_mode_rsvd_f = 4'b1111
         } evl_vm_mode_t;

      // refer to RTL: rtl/inc/csr_defines.vh: `CSR_PRV_{U,S,H,M} ... should match;
      typedef enum bit [1:0] {
            prv_user       = 2'b00,
            prv_supervisor = 2'b01,
            prv_hypervisor = 2'b10,
            prv_machine    = 2'b11
         } evl_privilege_t;


      typedef bit [4:0] evl_pmu_cntr_id_t;

      typedef enum bit [4:0] {
            PMU_EVENT_NOCOUNT          = 0,  // No count
            PMU_EVENT_MCYCLES          = 1,  // Mcycles
            PMU_EVENT_RETIRED_INST0    = 2,  // Retired Inst TH0
            PMU_EVENT_RETIRED_INST1    = 3,  // Retired Inst TH1
            PMU_EVENT_BRANCHES0        = 4,  // Branches taken TH0
            PMU_EVENT_BRANCHES1        = 5,  // Branches taken TH1

            PMU_EVENT_DCACHE_ACCESS0   = 6,  // Dcache Accesses TH0
            PMU_EVENT_DCACHE_ACCESS1   = 7,  // Dcache Accesses TH1
            PMU_EVENT_DCACHE_MISSES0   = 8,  // Dcache Misses TH0
            PMU_EVENT_DCACHE_MISSES1   = 9,  // Dcache Misses TH1
            PMU_EVENT_L2_MISS_REQ      = 10, // L2 Miss requests sent
            PMU_EVENT_L2_MISS_REQ_REJ  = 11, // L2 Miss requests rejected
            PMU_EVENT_L2_EVICT_REQ     = 12, // L2 Evict requests sent
            PMU_EVENT_L2_EVICT_REQ_REJ = 13, // L2 Evict requests rejected
            PMU_EVENT_TL_INST          = 14, // Tensor Load Instruction
            PMU_EVENT_TL_OPS           = 15, // Tensor Load Operation
            PMU_EVENT_TS_INST          = 16, // Tensor Store Instruction
            PMU_EVENT_TS_OPS           = 17, // Tensor Store Operation

            PMU_EVENT_TFMA_WAIT_TENB   = 18, // Tensor FMA waiting TENB
            PMU_EVENT_TIMA_OPS         = 19, // TIMA Operation
            PMU_EVENT_TXFMA_OPS16      = 20, // TXFMA Operation 16 bits
            PMU_EVENT_TXFMA_OPS32      = 21, // TXFMA Operation 32 bits
            PMU_EVENT_TRANS_OPS        = 22, // TRANS Operation
            PMU_EVENT_SHORT_OPS        = 23, // SHORT Operation
            PMU_EVENT_TFMA_INST        = 24, // TFMA Instruction
            PMU_EVENT_TFMA_OPS         = 25  // TFMA Operation
         } evl_pmu_event_type_t;


      typedef enum bit [4:0] {
            PMU_NEIGH_EVENT_NOCOUNT         = 0,  // No count
            PMU_NEIGH_EVENT_MIN_ET_REQ      = 1,  // Minion sends ET Link REQ
            PMU_NEIGH_EVENT_MIN_ET_RSP      = 2,  // Minion receives ET Link RSP
            PMU_NEIGH_EVENT_COOP_LD_REQ     = 3,  // Coop Load REQ sent
            PMU_NEIGH_EVENT_IN_COOP_LD_REQ  = 4,  // Inter-Neigh Coop Load REQ sent
            PMU_NEIGH_EVENT_COOP_LD_RSP     = 5,  // Coop Load RSP received
            PMU_NEIGH_EVENT_COOP_ST_REQ     = 6,  // Coop Store REQ sent
            PMU_NEIGH_EVENT_COOP_ST_RSP     = 7,  // Coop Store RSP received

            PMU_NEIGH_EVENT_MIN_ICACHE_REQ  = 8,  // Minion sneds REQ to ICache
            PMU_NEIGH_EVENT_MIN_ICACHE_RSP  = 9,  // Minion receives RSP from ICache
            PMU_NEIGH_EVENT_MIN_PTW_REQ     = 10, // Minion sends REQ to PTW
            PMU_NEIGH_EVENT_MIN_PTW_RSP     = 11, // Minion receives RSP from PTW
            PMU_NEIGH_EVENT_FLN_MSG         = 12, // Message sent between Minions through FLN
            PMU_NEIGH_EVENT_ICACHE_ET_REQ   = 13, // ICache sends ET Link REQ
            PMU_NEIGH_EVENT_ICACHE_ET_RSP   = 14, // ICache receives ET Link RSP
            PMU_NEIGH_EVENT_ICACHE_L1_REQ   = 15, // ICache sends REQ to L1 SRAM
            PMU_NEIGH_EVENT_ICACHE_L1_RSP   = 16, // ICache receives RSP from L1 SRAM
            PMU_NEIGH_EVENT_PTW_ET_REQ      = 17, // PTW sends ET Link REQ
            PMU_NEIGH_EVENT_PTW_ET_RSP      = 18, // PTW receives ET Link RSP

            PMU_NEIGH_EVENT_ET_REQ_FIFO     = 21, // ET Link REQ pushed into the intermediate FIFO
            PMU_NEIGH_EVENT_ET_REQ_UC_FIFO  = 22, // ET Link REQ pushed into any of the BANK/UC FIFOs
            PMU_NEIGH_EVENT_ET_REQ_SC_UC    = 23  // ET Link RSP received from the SC/UC input
         } evl_pmu_neigh_event_type_t;


      typedef enum int {
            PMU_TXN_CNTR_WR,
            PMU_TXN_CNTR_RD,
            PMU_TXN_CNTR_INCR,
            PMU_TXN_EVENT_WR
         } evl_pmu_txn_type_t;


      //
      // The following type is used within the shire environment to apply numbers to specific AXI
      // interfaces.
      //
      typedef enum int {
            SHIRE_L2TOL3_MESH_AXI  = 0,
            SHIRE_L3TOSYS_MESH_AXI = 1,
            SHIRE_MESH_TOL3_AXI    = 2,
            SHIRE_FROMSYS_MESH_AXI = 3,
            SHIRE_UCTOL3_MESH_AXI  = 4,
            SHIRE_UCTOSYS_MESH_AXI = 5,
            SHIRE_NULL_AXI         = 7
         } evl_shire_axi_ifc_type_t;

      //
      // The following type is used within the IOSHIRE environment to apply numbers to specific AXI
      // interfaces.
      //
      typedef enum int {
            SPIO_ROM_AXI,
            SPIO_SCRAM_AXI,
            SPIO_NOC_PU_NOC_AXI,
            SPIO_E2A_AXI,
            SPIO_DBG_COMM_AXI,
            SPIO_AM_AXI,
            SPIO_DMA_AXI,
            SPIO_VAULT_AXI,
            SPIO_PCIE_AXI,
            IO_TOL3_AXI,
            IO_PM_AXI,
            IO_PS_AXI,
            IO_TOSYS_AXI,
            PU_NOC_SRAM_AXI,
            PU_NOC_DMA_AXI,
            PU_NOC_DMA_AM_AXI,
            PU_NOC_EMMC_AXI,
            PU_NOC_USB0_AXI,
            PU_NOC_USB1_AXI,
            PU_NOC_STATIC_AXI,
            MAXION_TOL3_AXI,
            MAXION_UCTOL3_AXI,
            MAXION_TOSYS_AXI,
            MAXION_UCTOSYS_AXI,
            MAXION_L3_SLV_AXI,
            MAXION_SYS_SLV_AXI,
            IO_MAIN_NOC_RBM,
            IO_PU_NOC_RBM,
            IO_PCIE_RBM,
            IO_DBG_NOC_RT_EVT,
            IO_DBG_NOC_RT_MSG,
            IO_DBG_NOC_LF_EVT,
            IO_DBG_NOC_LF_MSG
         } evl_ioshire_axi_ifc_type_t;

      //
      // The following type is used within the PSHIRE environment to apply numbers to specific AXI
      // interfaces.
      //
      typedef enum int {
            PSHIRE_TOL3_AXI,
            PSHIRE_PS_AXI,
            PSHIRE_TOSYS_AXI,
            PSHIRE_SPIO_M_AXI,
            PCIE0_MSTR_AXI,
            PCIE0_DBI_AXI,
            PCIE0_SLV_AXI,
            PCIE1_MSTR_AXI,
            PCIE1_DBI_AXI,
            PCIE1_SLV_AXI,
            PSHIRE_ESR_AXI,
            PSHIRE_DBG_AXI
         } evl_pshire_axi_ifc_type_t;

      // Debug NoC Interface types
      typedef enum int {
            MIN_DBG_DN_EVT,
            MIN_DBG_DN_MSG
         } minshire_dbg_noc_bus_t;

      typedef enum int {
            MEM_DBG_DN_EVT,
            MEM_DBG_DN_MSG,
            MEM_DBG_LF_EVT,
            MEM_DBG_LF_MSG
         } memshire_dbg_noc_bus_t;

      typedef enum int {
            IO_DBG_RT_EVT,
            IO_DBG_RT_MSG,
            IO_DBG_LF_EVT,
            IO_DBG_LF_MSG
         } ioshire_dbg_noc_bus_t;

      typedef enum int {
            PS_DBG_DN_EVT,
            PS_DBG_DN_MSG
         } pshire_dbg_noc_bus_t;

      typedef struct packed unsigned {
            bit [`EVL_SHIRE_ID_MSB:0]        shire_id;    // 8 bits
            bit [`EVL_NEIGHBORHOOD_ID_MSB:0] neigh_id;    // 4 bits
            bit [`EVL_CORE_ID_MSB:0]         core_id;     // 4 bits
            bit [1:0]                        thread_id;   // 2 bits
            bit [5:0]                        agent_id;    // 6 bits
            bit [7:0]                        source;      // 8 bits
            bit [31:0]                       cycle;       // 32 bits
         } et_common_packet_info_t;

      typedef struct packed unsigned {
            bit [19:0]                    pad_3;       // 20 bits
            bit [19:0]                    b_id;        // 20 bits
            bit [1:0]                     pad_2;       // 2 bits
            bit [1:0]                     b_resp;      // 2 bits
            bit [19:0]                    r_id;        // 20 bits
            bit [1:0]                     pad_1;       // 2 bits
            bit [1:0]                     r_resp;      // 2 bits
            bit [`EVL_BUS_QOS_MSB:0]      aw_qos;      // 4 bits
            bit [`EVL_BUS_REQ_USER_MSB:0] aw_user;     // 16 bits
            bit [`EVL_BUS_REQ_SIZE_MSB:0] aw_size;     // 4 bits
            bit [19:0]                    aw_id;       // 20 bits
            bit [`EVL_PADDR_MSB:0]        aw_addr;     // 40 bits
            bit [`EVL_BUS_QOS_MSB:0]      ar_qos;      // 4 bits
            bit [`EVL_BUS_REQ_SIZE_MSB:0] ar_size;     // 4 bits
            bit [19:0]                    ar_id;       // 20 bits
            bit [`EVL_PADDR_MSB:0]        ar_addr;     // 40 bits
            bit [3:0]                     valid;       // 4 bits
         } et_axi_packet_t;

      typedef struct packed unsigned {
            bit [`ET_DPI_PKT_MSB-64:0] raw_packet;
            et_axi_packet_t            axi_packet;
//          et_etlink_packet_t         etlink_packet;
         } et_dpi_packet_u;

      typedef struct packed unsigned {
            et_dpi_packet_u         dpi_packet;
            et_common_packet_info_t packet_info;
         } et_full_dpi_packet_t;

      typedef bit [`ET_DPI_PKT_MSB:0] et_raw_dpi_packet_t;


      //--------------------------------------------------------------------------------------------
      //
      // AXI Interface Types for Coverage Collection
      //
      typedef enum int {
            EVL_COV_AXI_NONE       = 0,
            EVL_COV_AXI_SC_64      = 1,
            EVL_COV_AXI_SYS_MEM_64 = 2,
            EVL_COV_AXI_SYS_ESR_32 = 3,
            EVL_COV_AXI_MEM_SBM_8  = 4,
            EVL_COV_AXI_UC_64      = 5,
            EVL_COV_AXI_UC_32      = 6,
            EVL_COV_AXI_UST_MEM_64 = 7,
            EVL_COV_AXI_DDR_MEM_64 = 8
         } axi_cov_type_t;


      //--------------------------------------------------------------------------------------------
      //
      // ET Link Interface Types for Coverage Collection
      //
      typedef enum int {
            EVL_COV_ETLINK_NONE     = 0,
            EVL_COV_ETLINK_NEIGH_SC = 1,
            EVL_COV_ETLINK_MINION   = 2
         } etlink_cov_type_t;


      //--------------------------------------------------------------------------------------------
      //
      // Interrupt types
      //
      typedef enum int {
            EVL_MEIP = 0,
            EVL_MTIP = 1,
            EVL_SEIP = 2
         } evl_interrupt_t;


      //--------------------------------------------------------------------------------------------
      //
      // Cache Types
      //
      typedef enum int {
            RAM_TYPE_NONE,
            SC_RAM_TYPE_SC,
            SC_RAM_TYPE_L2,
            SC_RAM_TYPE_L3,
            SC_RAM_TYPE_SC_DATA,
            SC_RAM_TYPE_L2_DATA,
            SC_RAM_TYPE_L2_TAG,
            SC_RAM_TYPE_L2_STATE,
            SC_RAM_TYPE_L3_DATA,
            SC_RAM_TYPE_L3_TAG,
            SC_RAM_TYPE_L3_STATE,
            SC_RAM_TYPE_ICACHE_DATA,
            SC_RAM_TYPE_ICACHE_TAG,
            SC_RAM_TYPE_ICACHE0_DATA,
            SC_RAM_TYPE_ICACHE1_DATA,
            SC_RAM_TYPE_ICACHE2_DATA,
            SC_RAM_TYPE_ICACHE3_DATA,
            SC_RAM_TYPE_ICACHE0_TAG,
            SC_RAM_TYPE_ICACHE1_TAG,
            SC_RAM_TYPE_ICACHE2_TAG,
            SC_RAM_TYPE_ICACHE3_TAG
         } evl_cache_ram_type_t;


      //--------------------------------------------------------------------------------------------
      //
      // ECC Error Types
      //
      typedef enum int {
            EVL_ECC_ERR_NONE,
            EVL_ECC_ERR,
            EVL_ECC_ERR_EPHEMERAL,
            EVL_ECC_ERR_STUCK
         } evl_ecc_err_type_t;


      //--------------------------------------------------------------------------------------------
      //
      // Descriptor Statistics
      //
      typedef struct packed unsigned {
            int            level;
            evl_bus_type_t bus_type;
            int            issue_time;
            int            process_time;
            int            done_time;
         } evl_desc_stat_t;


      //--------------------------------------------------------------------------------------------
      //
      // Elaborated Constants
      //
      localparam evl_vaddr_t const_hword_vaddr_mask = ~(evl_vaddr_t'(`EVL_HWORD_OFFSET_MASK));
      localparam evl_vaddr_t const_word_vaddr_mask  = ~(evl_vaddr_t'(`EVL_WORD_OFFSET_MASK));
      localparam evl_vaddr_t const_dword_vaddr_mask = ~(evl_vaddr_t'(`EVL_DWORD_OFFSET_MASK));
      localparam evl_vaddr_t const_qword_vaddr_mask = ~(evl_vaddr_t'(`EVL_QWORD_OFFSET_MASK));
      localparam evl_vaddr_t const_oword_vaddr_mask = ~(evl_vaddr_t'(`EVL_OWORD_OFFSET_MASK));
      localparam evl_vaddr_t const_xword_vaddr_mask = ~(evl_vaddr_t'(`EVL_XWORD_OFFSET_MASK));
      localparam evl_vaddr_t const_line_vaddr_mask  = ~(evl_vaddr_t'(`EVL_LINE_OFFSET_MASK));
      localparam evl_vaddr_t const_page_vaddr_mask  = ~(evl_vaddr_t'(`EVL_PAGE_OFFSET_MASK));

      localparam evl_paddr_t const_hword_paddr_mask = ~(evl_paddr_t'(`EVL_HWORD_OFFSET_MASK));
      localparam evl_paddr_t const_word_paddr_mask  = ~(evl_paddr_t'(`EVL_WORD_OFFSET_MASK));
      localparam evl_paddr_t const_dword_paddr_mask = ~(evl_paddr_t'(`EVL_DWORD_OFFSET_MASK));
      localparam evl_paddr_t const_qword_paddr_mask = ~(evl_paddr_t'(`EVL_QWORD_OFFSET_MASK));
      localparam evl_paddr_t const_oword_paddr_mask = ~(evl_paddr_t'(`EVL_OWORD_OFFSET_MASK));
      localparam evl_paddr_t const_xword_paddr_mask = ~(evl_paddr_t'(`EVL_XWORD_OFFSET_MASK));
      localparam evl_paddr_t const_line_paddr_mask  = ~(evl_paddr_t'(`EVL_LINE_OFFSET_MASK));
      localparam evl_vaddr_t const_page_paddr_mask  = ~(evl_paddr_t'(`EVL_PAGE_OFFSET_MASK));


      //--------------------------------------------------------------------------------------------
      //
      // Forward Declaration of Class Objects
      //
      typedef class evl_log;
      typedef class evl_param_xref;


      //--------------------------------------------------------------------------------------------
      //
      // Function: get_next_trid
      //
      //   This function is used to obtain a temporally-unique transaction ID for an object.  The
      //   transaction ID takes on the following form:
      //
      //   - Bits 39:32: The specified shire ID.
      //   - Bits 31:28: The specified neighborhood ID.
      //   - Bits 27:24: The specified core ID.
      //   - Bits 23-0:  A temporally-unique value.
      //
      //   This function should only be called when a new transaction is first created.  All
      //   subsequently-spawned request and transaction descriptors will use the same TrID.  To
      //   ease the use of this function, a macro is provided that can be used from within both
      //   RTL code and class objects:
      //
      //      `get_next_trid( [<shire_id>[, <neighborhood_id>[, <core_id>]]] )
      //
      // Inputs:
      //
      // - shire_id:        This is the shire ID of the object requesting a TrID.  For objects
      //                    with no specific shire ID, the default value of this input should be
      //                    used.
      // - neighborhood_id: This is the neighborhood ID of the object requesting a TrID.  For
      //                    objects with no specific neighborhood ID, the default value of this
      //                    input should be used.
      // - core_id:         This is the core ID of the object requesting a TrID.  For objects
      //                    with no specific core ID, the default value of this input should be
      //                    used.
      //
      // Return Value:
      //
      // - evl_trans_id_t: This function returns a temporally-unique transaction ID specific to
      //                   shire, neighborhood, and core IDs specified.
      //
      evl_partial_trans_id_t m_next_trans_ids[evl_shire_id_t][`EVL_SHIRE_POST_CODE_NUM];

      function automatic evl_trans_id_t evl_get_next_trid(input evl_shire_id_t        shire_id        = ~(evl_shire_id_t'(0)),
                                                          input evl_neighborhood_id_t neighborhood_id = ~(evl_neighborhood_id_t'(0)),
                                                          input evl_core_id_t         core_id         = ~(evl_core_id_t'(0)));
         evl_shire_post_code_t  post_code = { neighborhood_id, core_id };
         evl_partial_trans_id_t trans_id;

         if (!m_next_trans_ids.exists(shire_id)) begin
            evl_partial_trans_id_t trans_id_da[`EVL_SHIRE_POST_CODE_NUM];

            //trans_id_da = new[`EVL_SHIRE_POST_CODE_NUM];
            for (int ii = 0; ii < `EVL_SHIRE_POST_CODE_NUM; ii++) begin
               trans_id_da[ii] = evl_partial_trans_id_t'(1);
            end
            m_next_trans_ids[shire_id] = trans_id_da;
         end
         trans_id = m_next_trans_ids[shire_id][post_code]++;
         if ((trans_id == evl_partial_trans_id_t'(0)) || (trans_id == ~evl_partial_trans_id_t'(0))) begin
            trans_id = evl_partial_trans_id_t'(1);
            m_next_trans_ids[shire_id][post_code] = evl_partial_trans_id_t'(2);
         end
         return ({ shire_id, post_code, trans_id });
      endfunction : evl_get_next_trid


      //--------------------------------------------------------------------------------------------
      //
      // Function: parse_hier_levels
      //
      //   This function is used to extract information from a hierarchical path. It can be used
      //   to obtain the path of an item's parent, or just the instance name, optionally removing
      //   levels of hierarchy (arbitrary, or generates)
      //
      // Inputs:
      //
      // - full_name:        This string is intended to be the full path name to this object (or
      //                     the full path name to its parent object).
      // - prefix:           This optional string is a prefix value to prepend to the full path
      //                     name before parsing starts.  Hierarchical levels are separated with
      //                     ".", so "top.soc" as a prefix value would imply a module instance
      //                     named "soc" within a module instance named "top".
      // - postfix:          This optional string is a postfix value to append to the full path
      //                     name once parsing is complete.
      // - skip_levels_in[]: This integer array specifies which hierarchical levels to eliminate.
      //                     The hierarchical numbering scheme is backwards: 1 is the current
      //                     level, 2 is the parent level, 3 is the grandparent level, etc.  Note
      //                     that the prefix value is prepended before eliminating hierarchical
      //                     levels.  As an example, consider the following hierarchy:
      //
      //                       top.soc.shire_0.etlink_if_0
      //
      //                     If skip_levels_in[] is { 2, 3 }, then the resulting full name will
      //                     be "top.etlink_if_0".  If skip_levels_in[] is { 2 }, then the
      //                     resulting full name will be "top.soc.etlink_if_0".  If
      //                     skip_levels_in is { 1 }, then the resulting full name will be
      //                     "top.soc.shire_0".
      // - instance_only:    This flag makes the function return only the last level of hierarchy
      //                     (the instance name).
      // - additional_hier:  This input is used to indicate additional levels of hierarchy.
      //
      // Return Value:
      //
      // - string: This function returns a string representing the parsed full path name.
      //
      function automatic string parse_hier_levels(input string full_name_in,
                                                  input string prefix           = "",
                                                  input string postfix          = "",
                                                  input int    skip_levels_in[] = {},
                                                  input bit    instance_only    = 1'b0,
                                                  input int    additional_hier  = 0);
         int    start;
         int    index;
         int    limit;
         int    skip_levels[$];
         string full_name;
         string partial_name;
         string partial_names[$];

         if (prefix != "") begin
            full_name = $sformatf("%0s.%0s", prefix, full_name_in);
         end
         else begin
            full_name = $sformatf("%0s", full_name_in);
         end
         start = 0;
         index = 0;
         limit = full_name.len();
         while (index < limit) begin
            byte cc = full_name.getc(index);

            if (full_name.getc(index) == byte'(".")) begin
               partial_name = full_name.substr(start, index - 1);
               start = index + 1;
               if (partial_name != "") begin
                  partial_names.push_back(partial_name);
               end
            end
            index++;
         end
         partial_name = full_name.substr(start, limit - 1);
         if (partial_name != "") begin
            partial_names.push_back(partial_name);
         end
         limit = partial_names.size();
         if (additional_hier > 0) begin
            for (int ii = 1; ii <= additional_hier; ii++) begin
               skip_levels.push_back(ii);
            end
            foreach (skip_levels_in[ii]) begin
               skip_levels.push_back(skip_levels_in[ii] + additional_hier);
            end
         end
         else begin
            foreach (skip_levels_in[ii]) begin
               skip_levels.push_back(skip_levels_in[ii] + additional_hier);
            end
         end
         foreach (skip_levels[ii]) begin
            if ((skip_levels[ii] > 0) && ((limit - skip_levels[ii]) >= 0)) begin
               partial_names[limit - skip_levels[ii]] = "";
            end
         end
         for (int ii = partial_names.size() - 1; ii >= 0; ii--) begin
            if (partial_names[ii] == "") begin
               partial_names.delete(ii);
            end
         end
         if (partial_names.size() <= 0) begin
            full_name = "";
         end
         else begin
            full_name = partial_names[0];
            partial_names.delete(0);
            foreach (partial_names[ii]) begin
               full_name = $sformatf("%0s.%0s", full_name, partial_names[ii]);
            end
         end
         if (postfix != "") begin
            full_name = $sformatf("%0s.%0s", full_name, postfix);
         end
         if ((instance_only == 1'b1) && (partial_names.size() > 0)) begin
            return partial_names[partial_names.size() - 1];
         end
         return full_name;
      endfunction : parse_hier_levels


      //--------------------------------------------------------------------------------------------
      //
      // Function: extract_parent_list
      //
      //   Blah.
      //
      // Inputs:
      //
      // - parent_names:     Blah.
      // - full_parent_name: Blah.
      // - full_name:        Blah.
      //
      // Return Value:
      //
      // - string: Blah.
      //
      function automatic string extract_parent_list(output string parent_names[$],
                                                    input  string full_parent_name,
                                                    input  string full_name);
         int    start;
         int    index;
         int    limit;
         string child_name;
         string partial_name;

         start = 0;
         index = 0;
         if ((full_parent_name != "") && (full_name.substr(0, full_parent_name.len() - 1) == full_parent_name)) begin
            child_name = full_name.substr(full_parent_name.len() + 1, full_name.len() - 1);
         end
         else begin
            child_name = full_name;
         end

         limit        = child_name.len();
         parent_names = {};
         while (index < limit) begin
            if (child_name.getc(index) == byte'(".")) begin
               partial_name = child_name.substr(start, index - 1);
               if (partial_name != "") begin
                  parent_names.push_back(partial_name);
               end
               start = index + 1;
            end
            index++;
         end
         return child_name.substr(start, limit - 1);
      endfunction : extract_parent_list


      //--------------------------------------------------------------------------------------------
      //
      // Function: strstr
      //
      // This function search the first string provided for substring provided in the second
      // argument, and returns -1 if the substring isn't present, or the index where the
      // substring starts otherwise.
      //
      // Inputs:
      //
      // - str_in:        This is the string in which the pattern will be searched
      // - sub_in:        This is the pattern to search for
      // - ignore_case:   This optional bit specifies whether or not the search should be case
      //                  insensitive. By default it is case sensitive.
      //
      // Return Value:
      //
      // - int: This function returns the index of the found pattern, or -1 if it wasn't found
      //
      function int strstr(input string str_in,
                          input string sub_in,
                          input bit    ignore_case = 1'b0);
         string str, sub;
         str = (ignore_case) ? str_in.toupper() : str_in;
         sub = (ignore_case) ? sub_in.toupper() : sub_in;

         if (sub.len() > str.len()) return -1;
         if (sub.len() == 0)        return -1;

         for (int ii = 0; ii <= (str.len()-sub.len()); ii++) begin
            if (str.substr(ii, ii + sub.len() - 1).compare(sub) == 0) begin
               return ii;
            end
         end
         return -1;
      endfunction: strstr


      //--------------------------------------------------------------------------------------------
      //
      // Quiescent Check Indicator
      //
      static int m_quiescent_check = 0;


      //--------------------------------------------------------------------------------------------
      //
      // End-of-Simulation Indicator (to prevent C code callbacks)
      //
      static bit m_sim_complete = 1'b0;


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_log_create
      //
      static evl_log m_evl_log = evl_log_create();

      function automatic evl_log evl_log_create();
         if (m_evl_log == null) begin
            m_evl_log = evl_log::get_inst();
         end
         return m_evl_log;
      endfunction : evl_log_create


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_check_plusargs
      //
      function automatic int evl_check_plusargs(input string plus_arg, input string file_name, input int line_number);
         if (m_evl_log == null) begin
            void'(evl_log_create());
         end
         m_evl_log.check_plusarg(plus_arg, file_name, line_number);
         return 0;
      endfunction : evl_check_plusargs


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_certify_plusargs
      //
      function automatic void evl_certify_plusargs();
         if (m_evl_log == null) begin
            void'(evl_log_create());
         end
         m_evl_log.certify_plusargs();
      endfunction : evl_certify_plusargs


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_print
      //
      function automatic void evl_print(input string         abstract_name_in = "",
                                        input string         message_in       = "",
                                        input bit [1:0]      use_addr         = 2'b00,
                                        input evl_paddr_t    paddr_in         = ~(evl_paddr_t'(0)),
                                        input evl_trans_id_t trans_id_in      = ~(evl_trans_id_t'(0)),
                                        input int            tag_in           = ~(0));
         m_evl_log.print(abstract_name_in, message_in, use_addr, paddr_in, trans_id_in, tag_in);
      endfunction : evl_print


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_print_no_addr
      //
      function automatic void evl_print_no_addr(input string abstract_name_in = "",
                                                input string message_in       = "");
         m_evl_log.print_no_addr(abstract_name_in, message_in);
      endfunction : evl_print_no_addr


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_print_no_name
      //
      function automatic void evl_print_no_name(input string message_in = "");
         m_evl_log.print_no_name(message_in);
      endfunction : evl_print_no_name


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_print_no_time
      //
      function automatic void evl_print_no_time(input string message_in = "");
         m_evl_log.print_no_time(message_in);
      endfunction : evl_print_no_time


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_set_verbosity
      //
      function automatic void evl_set_verbosity(input int verbosity_in, input evl_verbosity_mod_t verbosity_module_in);
         m_evl_log.set_verbosity(verbosity_in, verbosity_module_in);
      endfunction : evl_set_verbosity


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_check_verbosity
      //
      function automatic bit evl_check_verbosity(input int verbosity_in, input evl_verbosity_mod_t verbosity_module_in);
         return m_evl_log.check_verbosity(verbosity_in, verbosity_module_in);
      endfunction : evl_check_verbosity


      //--------------------------------------------------------------------------------------------
      //
      // Task: evl_wait_for_verbosity_change
      //
      task automatic evl_wait_for_verbosity_change(output int cur_verbosity, input int verbosity_in, input evl_verbosity_mod_t verbosity_module_in);
         #0;
         m_evl_log.wait_for_verbosity_change(cur_verbosity, verbosity_in, verbosity_module_in);
      endtask : evl_wait_for_verbosity_change


      //--------------------------------------------------------------------------------------------
      //
      // Function: get_verbosity_by_name
      //
      function automatic int evl_get_verbosity_by_name(input string verbosity_name_in);
         return m_evl_log.get_verbosity_by_name(verbosity_name_in);
      endfunction : evl_get_verbosity_by_name


      //--------------------------------------------------------------------------------------------
      //
      // Function: get_verbosity_mod_by_name
      //
      function automatic evl_verbosity_mod_t evl_get_verbosity_mod_by_name(input string verbosity_mod_name_in);
         return m_evl_log.get_verbosity_mod_by_name(verbosity_mod_name_in);
      endfunction : evl_get_verbosity_mod_by_name


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_set_error_intensity
      //
      function automatic void evl_set_error_intensity(input int intensity, input int verbosity);
         m_evl_log.set_intensity(intensity, verbosity);
      endfunction : evl_set_error_intensity


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_set_error_intensity_by_name
      //
      function automatic void evl_set_error_intensity_by_name(input string intensity_name, input int verbosity);
         m_evl_log.set_intensity_by_name(intensity_name, verbosity);
      endfunction : evl_set_error_intensity_by_name


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_get_error_intensity
      //
      function automatic int evl_get_error_intensity(input int intensity);
         return m_evl_log.get_intensity(intensity);
      endfunction : evl_get_error_intensity


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_set_error_intensity_by_name
      //
      function automatic int evl_get_error_intensity_by_name(input string intensity_name);
         return m_evl_log.get_intensity_by_name(intensity_name);
      endfunction : evl_get_error_intensity_by_name


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_dut_error
      //
      function automatic void evl_dut_error(input string              abstract_name_in    = "",
                                            input string              message_in          = "",
                                            input int                 intensity_key       = 0,
                                            input evl_verbosity_mod_t verbosity_mods_in[] = {},
                                            input evl_paddr_t         paddr_in            = ~(evl_paddr_t'(0)),
                                            input evl_trans_id_t      trans_id_in         = ~(evl_trans_id_t'(0)),
                                            input int                 tag_in              = ~(0),
                                            input string              rtl_path            = "");
         m_evl_log.dut_error(abstract_name_in, message_in, intensity_key, verbosity_mods_in, paddr_in, trans_id_in, tag_in, rtl_path);
      endfunction : evl_dut_error


      //--------------------------------------------------------------------------------------------
      //
      // Function: evl_dut_warning
      //
      function automatic void evl_dut_warning(input string              abstract_name_in    = "",
                                              input string              message_in          = "",
                                              input int                 intensity_key       = 0,
                                              input evl_verbosity_mod_t verbosity_mods_in[] = {},
                                              input evl_paddr_t         paddr_in            = ~(evl_paddr_t'(0)),
                                              input evl_trans_id_t      trans_id_in         = ~(evl_trans_id_t'(0)),
                                              input int                 tag_in              = ~(0),
                                              input string              rtl_path            = "");
         m_evl_log.dut_warning(abstract_name_in, message_in, intensity_key, verbosity_mods_in, paddr_in, trans_id_in, tag_in, rtl_path);
      endfunction : evl_dut_warning


      //--------------------------------------------------------------------------------------------
      //
      // Function: perform_et_atomic_op
      //
      // This function performs an ET Link type atomic operation.  It can be used for Minion-issued
      // atomic operations on the ET Link interface or for global atomic operations issued on the
      // AXI interface.
      //
      // Outputs:
      //
      // - return_data:    This is the cache-line aligned data to return for the atomic response.
      // - return_mask:    This is the cache-line aligned data mask (for AXI write strobes) for the
      //                   atomic response.
      //
      // Inputs:
      //
      // - atomic_conf:    This is the atomic operation to perform (encoded as an ET Link atomic
      //                   subopcode).
      // - atomic_offset:  This is the cache-line offset for the atomic operation.
      // - mem_data_in:    This is the cache-line-aligned memory data; it is the current value (in
      //                   memory) of the cache line.
      // - atomic_data_in: This is the cache-line-aligned data from the atomic operation.
      // - normalize_data: Normalize the return data (i.e., do not cache-line align it).
      //
      // Return Value:
      //
      // - evl_xword_t:    This is the cache-line-aligned memory data to write back to memory
      //                   after applying the atomic operation.
      //
      function automatic evl_xword_t perform_et_atomic_op(output evl_xword_t           return_data,
                                                          output evl_xword_byte_en_t   return_mask,
                                                          input  evl_bus_atomic_conf_t atomic_conf,
                                                          input  evl_paddr_t           atomic_offset,
                                                          input  evl_xword_t           mem_data_in,
                                                          input  evl_xword_t           atomic_data_in,
                                                          input  bit                   normalize_data = 1'b0);
         evl_atomic_op_t atomic_op;
         bit             atomic_sc_vec;
         bit             atomic_size;
         evl_dword_t     req_operand;
         evl_dword_t     mem_operand;
         evl_dword_t     result;
         evl_xword_t     mem_data_out;
         evl_xword_t     data_mask;
         evl_xword_t     copy_mask;
         int             offset;

         atomic_op     = evl_atomic_op_t'(atomic_conf[3:0]);
         atomic_sc_vec = bit'(atomic_conf[4]);
         atomic_size   = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);
         mem_data_out  = mem_data_in;
         offset        = int'( (atomic_sc_vec == 1'b1) ? (~(`EVL_OWORD_BYTES - 1) & atomic_offset[`EVL_LINE_ADDR_LSB-1:0]) :
                               (atomic_size   == 1'b1) ? (~(`EVL_DWORD_BYTES - 1) & atomic_offset[`EVL_LINE_ADDR_LSB-1:0]) :
                                                         (~(`EVL_WORD_BYTES - 1)  & atomic_offset[`EVL_LINE_ADDR_LSB-1:0]) );

         //
         // Scalar & vector operations are the same; the only difference is the number of pieces of
         // data we are working with.
         //
         return_data = evl_xword_t'(0);
         return_mask = evl_xword_byte_en_t'(0);
         data_mask   = (atomic_size == 1'b1) ? `EVL_DWORD_MASK : `EVL_WORD_MASK;
         copy_mask   = data_mask << (offset << 3);

         for (int ii = (atomic_sc_vec == 1'b1) ? 7 : 0; ii >= 0; ii--) begin
            int data_shift;

            //
            // The request operand is always in the lower quadword of the data, but the memory
            // operand is based on the full address offset.  We gradually process and build up the
            // result of the atomic operation (to be written back to memory) and the value to
            // return.
            //
            if (atomic_size == 1'b1) begin
               data_shift  = (ii << 3);
               copy_mask   = data_mask << ((data_shift + offset) << 3);
               req_operand = evl_dword_t'(`EVL_DWORD_MASK & (atomic_data_in >> (data_shift << 3)));
               mem_operand = evl_dword_t'(`EVL_DWORD_MASK & (mem_data_in >> ((offset + data_shift) << 3)));
               return_mask = return_mask | (`EVL_DWORD_BYTE_EN_MASK << (offset + data_shift));
            end
            else begin
               data_shift  = (ii << 2);
               copy_mask   = data_mask << ((data_shift + offset) << 3);
               req_operand = evl_dword_t'(`EVL_WORD_MASK & (atomic_data_in >> (data_shift << 3)));
               mem_operand = evl_dword_t'(`EVL_WORD_MASK & (mem_data_in >> ((offset + data_shift) << 3)));
               return_mask = return_mask | (`EVL_WORD_BYTE_EN_MASK << (offset + data_shift));
            end
            return_data = (~copy_mask & return_data) | (copy_mask & evl_xword_t'(mem_data_in));

            case (atomic_op)
               SC_AMOSWAP:     begin
                                  result = req_operand;
                               end
               SC_AMOADD:      begin
                                  result = req_operand + mem_operand;
                               end
               SC_AMOXOR:      begin
                                  result = req_operand ^ mem_operand;
                               end
               SC_AMOAND:      begin
                                  result = req_operand & mem_operand;
                               end
               SC_AMOOR:       begin
                                  result = req_operand | mem_operand;
                               end
               SC_AMOMIN:      begin
                                  longint req_val;
                                  longint mem_val;

                                  if (atomic_size == 1'b1) begin
                                     req_val = signed'(req_operand);
                                     mem_val = signed'(mem_operand);
                                  end
                                  else begin
                                     req_val = signed'(evl_word_t'(req_operand));
                                     mem_val = signed'(evl_word_t'(mem_operand));
                                  end
                                  result = (req_val > mem_val) ? mem_val : req_val;
                               end
               SC_AMOMAX:      begin
                                  longint req_val;
                                  longint mem_val;

                                  if (atomic_size == 1'b1) begin
                                     req_val = signed'(req_operand);
                                     mem_val = signed'(mem_operand);
                                  end
                                  else begin
                                     req_val = signed'(evl_word_t'(req_operand));
                                     mem_val = signed'(evl_word_t'(mem_operand));
                                  end
                                  result = (req_val > mem_val) ? req_val : mem_val;
                               end
               SC_AMOMINU:     begin
                                  result = (req_operand > mem_operand) ? mem_operand : req_operand;
                               end
               SC_AMOMAXU:     begin
                                  result = (req_operand > mem_operand) ? req_operand : mem_operand;
                               end
               SC_AMOMAX_PS,
               SC_AMOMIN_PS:   begin
                                  //
                                  // The magic numbers come from IEEE 754 spec for NaNs.  Floating
                                  // point has a different format based on whether it is 32b or 64b.
                                  //
                                  evl_dword_t c_NaN32 = 64'h0000_0000_7fc0_0000;
                                  evl_dword_t c_NaN64 = 64'h7ff8_0000_0000_0000;
                                  shortreal   req_val32;
                                  shortreal   mem_val32;
                                  real        req_val64;
                                  real        mem_val64;
                                  bit         req_val_is_qnan;
                                  bit         mem_val_is_qnan;
                                  bit         req_val_is_snan;
                                  bit         mem_val_is_snan;
                                  bit         req_val_is_zero;
                                  bit         mem_val_is_zero;

                                  req_val32 = $bitstoshortreal(req_operand);
                                  mem_val32 = $bitstoshortreal(mem_operand);
                                  req_val64 = $bitstoreal(req_operand);
                                  mem_val64 = $bitstoreal(mem_operand);
                                  if (atomic_size == 1'b1) begin
                                     req_val_is_qnan = ((req_operand[62:52] == 11'h7ff) && (req_operand[51] == 1'b1)) ? 1'b1 : 1'b0;
                                     mem_val_is_qnan = ((mem_operand[62:52] == 11'h7ff) && (mem_operand[51] == 1'b1)) ? 1'b1 : 1'b0;
                                     req_val_is_snan = ((req_operand[62:52] == 11'h7ff) && (req_operand[51] == 1'b0) && (req_operand[50:0] != 0)) ? 1'b1 : 1'b0;
                                     mem_val_is_snan = ((mem_operand[62:52] == 11'h7ff) && (mem_operand[51] == 1'b0) && (mem_operand[50:0] != 0)) ? 1'b1 : 1'b0;
                                     req_val_is_zero = ((req_operand == 64'h0000000000000000) || (req_operand == 64'h8000000000000000)) ? 1'b1 : 1'b0;
                                     mem_val_is_zero = ((mem_operand == 64'h0000000000000000) || (mem_operand == 64'h8000000000000000)) ? 1'b1 : 1'b0;
                                  end
                                  else begin
                                     req_val_is_qnan = ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b1)) ? 1'b1 : 1'b0;
                                     mem_val_is_qnan = ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b1)) ? 1'b1 : 1'b0;
                                     req_val_is_snan = ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b0) && (req_operand[21:0] != 0)) ? 1'b1 : 1'b0;
                                     mem_val_is_snan = ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b0) && (mem_operand[21:0] != 0)) ? 1'b1 : 1'b0;
                                     req_val_is_zero = ((req_operand == 64'h0000000000000000) || (req_operand == 64'h0000000080000000)) ? 1'b1 : 1'b0;
                                     mem_val_is_zero = ((mem_operand == 64'h0000000000000000) || (mem_operand == 64'h0000000080000000)) ? 1'b1 : 1'b0;
                                  end

                                  // NaN handling
                                  if ((req_val_is_snan == 1'b1) || (mem_val_is_snan == 1'b1)) begin
                                     result = (atomic_size == 1'b1) ? c_NaN64 : c_NaN32;
                                  end
                                  else if ((req_val_is_qnan == 1'b1) && (mem_val_is_qnan == 1'b1)) begin
                                     result = (atomic_size == 1'b1) ? c_NaN64 : c_NaN32;
                                  end
                                  else if (req_val_is_qnan == 1'b1) begin
                                     result = mem_operand;
                                  end
                                  else if (mem_val_is_qnan == 1'b1) begin
                                     result = req_operand;
                                  end
                                  // Neither are NaNs, so do the comparison.  Special check first
                                  // for +0/-0 since they evaluate as equal when using >.
                                  else if ((req_val_is_zero == 1'b1) && (mem_val_is_zero == 1'b1)) begin
                                     if (atomic_op == SC_AMOMAX_PS) begin
                                        result = (req_operand > mem_operand) ? mem_operand : req_operand;
                                     end
                                     else begin
                                        result = (req_operand > mem_operand) ? req_operand : mem_operand;
                                     end
                                  end
                                  else begin
                                     case ({atomic_op, atomic_size})
                                        {SC_AMOMIN_PS, 1'b1}: result = (req_val64 > mem_val64) ? mem_operand : req_operand;
                                        {SC_AMOMAX_PS, 1'b1}: result = (req_val64 > mem_val64) ? req_operand : mem_operand;
                                        {SC_AMOMIN_PS, 1'b0}: result = (req_val32 > mem_val32) ? mem_operand : req_operand;
                                        {SC_AMOMAX_PS, 1'b0}: result = (req_val32 > mem_val32) ? req_operand : mem_operand;
                                     endcase
                                  end
                               end

               //
               // Note about CompExch: it is scalar ONLY (one operation of either 32b/64b data)
               // There are 2 incoming operands: desired (word0/dword0) and expected (word2/dword1)
               // The format is as follows:
               //    size==1 (64b) <------64------><------64------>
               //    size==0 (32b) <---0--><--32--><---0--><--32-->
               //
               SC_AMOCOMPEXCH: begin
                                  // If the vector bit is set then it behaves as a NOP
                                  if (atomic_sc_vec == 1'b1) begin
                                     result = mem_operand;
                                  end
                                  else begin
                                     evl_dword_t desired_operand;
                                     evl_dword_t expected_operand;

                                     // redundant but easier to read/understand code
                                     desired_operand = req_operand;

                                     if (atomic_size == 1'b1) begin
                                        expected_operand = evl_dword_t'(`EVL_DWORD_MASK & (atomic_data_in >> ((data_shift + 8) << 3)));
                                     end
                                     else begin
                                        expected_operand = evl_dword_t'(`EVL_WORD_MASK & (atomic_data_in >> ((data_shift + 8) << 3)));
                                     end

                                     if (mem_operand == expected_operand) begin
                                        result = desired_operand;
                                     end
                                     else begin
                                        result = mem_operand;
                                     end
                                  end
                               end
               default:        begin
                                  result = mem_operand;
                               end
            endcase

            //
            // Generate the result to write to memory.
            //
            mem_data_out = (~copy_mask & mem_data_out) | (copy_mask & (evl_xword_t'(result) << ((offset + data_shift) << 3)));
         end
         if (normalize_data == 1'b1) begin
            return_mask = return_mask >> offset;
            return_data = return_data >> (offset << 3);
         end
         return mem_data_out;
      endfunction : perform_et_atomic_op


      `include "dv/common/base/evl_log.sv"
      `include "dv/common/base/evl_param_xref.sv"


      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         typedef class evl_addr_alloc;
         typedef class evl_addr_region;
         typedef class evl_addr_region_list;
         typedef class evl_agent;
         typedef class evl_bus_parser;
         typedef class evl_bus_req_id_queue;
         typedef class evl_bus_watch;
         typedef class evl_cache;
         typedef class evl_cache_base;
         typedef class evl_cache_index_config;
         typedef class evl_component;
         typedef class evl_delay_list;
         typedef class evl_delay_wrapper;
         typedef class evl_desc;
         typedef class evl_desc_queue;
         typedef class evl_desc_seq_item;
         typedef class evl_env;
         typedef class evl_global_attrs;
         typedef class evl_instr_watch;
         typedef class evl_line_data;
         typedef class evl_line_desc;
         typedef class evl_line_desc_base;
         typedef class evl_line_queue;
         typedef class evl_mem_desc;
         typedef class evl_mem_desc_queue;
         typedef class evl_message_packet;
         typedef class evl_message_queue;
         typedef class evl_message_wrapper;
         typedef class evl_monitor;
         typedef class evl_monitor_packet;
         typedef class evl_monitor_queue;
         typedef class evl_object;
         typedef class evl_objection;
         typedef class evl_param_ping;
         typedef class evl_plus_arg;
         typedef class evl_ptw_desc;
         typedef class evl_pmu_desc;
         typedef class evl_push_driver;
         typedef class evl_push_sequencer;
         typedef class evl_rand_gen;
         typedef class evl_reg_adapter;
         typedef class evl_reg_block;
         typedef class evl_req_desc;
         typedef class evl_req_desc_queue;
         typedef class evl_rtl_instance;
         typedef class evl_rtl_instance_map;
         typedef class evl_rtl_port;
         typedef class evl_scoreboard;
         typedef class evl_scoreboard_mem;
         typedef class evl_semaphore_wrapper;
         typedef class evl_sequence;
         typedef class evl_sequence_item;
         typedef class evl_sequence_mdir;
         typedef class evl_sequence_mdir_tcl;
         typedef class evl_sequence_mem;
         typedef class evl_sequence_mrand;
         typedef class evl_sequence_wrapper;
         typedef class evl_sequence_wrapper_base;
         typedef class evl_sequencer;
         typedef class evl_sequencer_mem;
         typedef class evl_sequencer_wrapper;
         typedef class evl_sequencer_wrapper_base;
         typedef class evl_stimulus;
         typedef class evl_stimulus_map;
         typedef class evl_table_cover;
         typedef class evl_table_cover_base;
         typedef class evl_table_cover_entry;
         typedef class evl_table_desc;
         typedef class evl_table_wrapper;
         typedef class evl_tcl_cmd_parser;
         typedef class evl_tcl_default_parser;
         typedef class evl_tcl_handler;
         typedef class evl_tcl_mem_seq_parser;
         typedef class evl_test;
         typedef class evl_tlb;
         typedef class evl_trans_desc;
         typedef class evl_unique_element_queue;
         typedef class evl_verif_comp;
         typedef class evl_verif_comp_map;
         typedef class evl_void;
         typedef class evl_way_desc;

         typedef evl_verif_comp evl_verif_comp_q[$];


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_dut_error
         //
         function void CoSim_dut_error(input string message_in);
            string message;
            static evl_verbosity_mod_t verbosity_empty_list[] = {}; // Needed by verilator
            message = $sformatf("%0s", message_in);
            m_evl_log.dut_error("EMU(C)", message, 0, verbosity_empty_list, ~(evl_paddr_t'(0)), ~(evl_trans_id_t'(0)), ~(0), "");
         endfunction : CoSim_dut_error

         export "DPI-C" function CoSim_dut_error;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_dut_warning
         //
         function void CoSim_dut_warning(input string message_in);
            string message;
            static evl_verbosity_mod_t verbosity_empty_list[] = {}; // Needed by verilator
            
            message = $sformatf("%0s", message_in);
            m_evl_log.dut_warning("EMU(C)", message, 0, verbosity_empty_list, ~(evl_paddr_t'(0)), ~(evl_trans_id_t'(0)), ~(0), "");
         endfunction : CoSim_dut_warning

         export "DPI-C" function CoSim_dut_warning;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_agent_print
         //
         function void CoSim_agent_print(input string type_in,
                                         input string message_in);
            if (type_in == "") begin
               m_evl_log.print("EMU(C)", message_in, 1'b0, ~(evl_paddr_t'(0)), ~(evl_trans_id_t'(0)), ~(0));
            end
            else begin
               m_evl_log.print("EMU(C)", message_in, 1'b0, ~(evl_paddr_t'(0)), ~(evl_trans_id_t'(0)), ~(0));
            end
         endfunction : CoSim_agent_print

         export "DPI-C" function CoSim_agent_print;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_agent_initialized
         //
         function void CoSim_agent_initialized();
            evl_global_attrs::cosim_start();
         endfunction : CoSim_agent_initialized

         export "DPI-C" function CoSim_agent_initialized;


         //-----------------------------------------------------------------------------------------
         //
         // Function: CoSim_agent_drop_objection
         //
         function void CoSim_agent_drop_objection(input int result, input int minion_id, input int thread_id);
            evl_global_attrs::cosim_end(result, minion_id, thread_id, 1'b0);
         endfunction : CoSim_agent_drop_objection

         export "DPI-C" function CoSim_agent_drop_objection;


         `include "dv/common/base/evl_void.sv"
         `include "dv/common/base/evl_plus_arg.sv"
         `include "dv/common/base/evl_param_ping.sv"
         `include "dv/common/base/evl_rand_gen.sv"
         `include "dv/common/base/evl_bus_req_id_queue.sv"
         `include "dv/common/base/evl_rtl_port.sv"
         `include "dv/common/base/evl_rtl_instance.sv"
         `include "dv/common/base/evl_rtl_instance_map.sv"
         `include "dv/common/base/evl_bus_watch.sv"
         `include "dv/common/base/evl_instr_watch.sv"
         `include "dv/common/base/evl_objection.sv"
         `include "dv/common/base/evl_global_attrs.sv"
         `include "dv/common/base/evl_bus_parser.sv"
         `include "dv/common/base/evl_delay_list.sv"
         `include "dv/common/base/evl_delay_wrapper.sv"
         `include "dv/common/base/evl_object.sv"
         `include "dv/common/base/evl_component.sv"
         `include "dv/common/base/evl_unique_element_queue.sv"
         `include "dv/common/base/evl_semaphore_wrapper.sv"
         `include "dv/common/base/evl_addr_alloc.sv"
         `include "dv/common/base/evl_addr_region.sv"
         `include "dv/common/base/evl_addr_region_list.sv"
         `include "dv/common/base/evl_message_packet.sv"
         `include "dv/common/base/evl_message_queue.sv"
         `include "dv/common/base/evl_message_wrapper.sv"
         `include "dv/common/base/evl_monitor_packet.sv"
         `include "dv/common/base/evl_monitor_queue.sv"
         `include "dv/common/base/evl_agent.sv"
         `include "dv/common/base/evl_verif_comp.sv"
         `include "dv/common/base/evl_verif_comp_map.sv"
         `include "dv/common/base/evl_sequencer.sv"
         `include "dv/common/base/evl_sequencer_mem.sv"
         `include "dv/common/base/evl_sequencer_wrapper.sv"
         `include "dv/common/base/evl_monitor.sv"
         `include "dv/common/base/evl_push_driver.sv"
         `include "dv/common/base/evl_push_sequencer.sv"
         `include "dv/common/base/evl_env.sv"
         `include "dv/common/base/evl_scoreboard.sv"
         `include "dv/common/base/evl_scoreboard_mem.sv"
         `include "dv/common/base/evl_sequence_item.sv"
         `include "dv/common/base/evl_sequence.sv"
         `include "dv/common/base/evl_sequence_mem.sv"
         `include "dv/common/base/evl_sequence_mdir.sv"
         `include "dv/common/base/evl_sequence_mdir_tcl.sv"
         `include "dv/common/base/evl_sequence_mrand.sv"
         `include "dv/common/base/evl_sequence_wrapper.sv"
         `include "dv/common/base/evl_line_data.sv"
         `include "dv/common/base/evl_desc.sv"
         `include "dv/common/base/evl_desc_seq_item.sv"
         `include "dv/common/base/evl_line_desc_base.sv"
         `include "dv/common/base/evl_line_desc.sv"
         `include "dv/common/base/evl_line_queue.sv"
         `include "dv/common/base/evl_page_table_entry.sv"
         `include "dv/common/base/evl_way_desc.sv"
         `include "dv/common/base/evl_mem_desc.sv"
         `include "dv/common/base/evl_mem_desc_queue.sv"
         `include "dv/common/base/evl_trans_desc.sv"
         `include "dv/common/base/evl_req_desc.sv"
         `include "dv/common/base/evl_ptw_desc.sv"
         `include "dv/common/base/evl_pmu_desc.sv"
         `include "dv/common/base/evl_desc_queue.sv"
         `include "dv/common/base/evl_req_desc_queue.sv"
         `include "dv/common/base/evl_trans_desc_queue.sv"
         `include "dv/common/base/evl_table_desc.sv"
         `include "dv/common/base/evl_table_wrapper.sv"
         `include "dv/common/base/evl_table_cover_entry.sv"
         `include "dv/common/base/evl_table_cover.sv"
         `include "dv/common/base/evl_tcl_cmd_parser.sv"
         `include "dv/common/base/evl_tcl_default_parser.sv"
         `include "dv/common/base/evl_tcl_mem_seq_parser.sv"
         `include "dv/common/base/evl_tcl_handler.sv"
         `include "dv/common/base/evl_cache_index_config.sv"
         `include "dv/common/base/evl_cache_base.sv"
         `include "dv/common/base/evl_cache.sv"
         `include "dv/common/base/evl_tlb.sv"
         `include "dv/common/base/evl_test.sv"
         `include "dv/common/base/evl_stimulus.sv"
         `include "dv/common/base/evl_stimulus_map.sv"
         `include "dv/common/base/evl_reg_adapter.sv"
         `include "dv/common/base/evl_reg_block.sv"

      `endif // ifdef EVL_SIMULATION --------------------------------------------------------------}

   endpackage : evl_base_pkg

`endif // !__EVL_BASE_PKG
