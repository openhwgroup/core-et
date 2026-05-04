/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __PCIE_DMA_CONFIG_H
#define __PCIE_DMA_CONFIG_H

#include "cpu.h"
#include "soc.h"
#include "macros.h"
#include "pshire_common.h"

/*==================== Function Separator =============================*/
et_status_t pcie_dma_config(void )
{
  uint64_t  *pRegs64;
  uint32_t  *pRegs32;
  uint64_t  regVal, regValNew;

  // setup the descriptors for the dma     
  pRegs64 = ( uint64_t * )( R_L3_DRAM_BASEADDR );
  // setup 2 link list Elements for read
  for ( uint32_t i = 0; i < 1; i++ ) {
    // Channel Control = 32'h0000_0001
    // Transfer Size = 32'h0000_0180
    // Transfer Size = 32'h0000_0180
    if (  i == 1 ) {
      pRegs64[0] = 0x316800000001;
    } else {
      pRegs64[0] = 0x018000000001;
    }
    pRegs64 += 0x1;
    // {SAR_HIGH, SAR Low}
    pRegs64[0] = R_PCIE0_SLV_BASEADDR + ( 0x4000 * i );   
    pRegs64 += 0x1;
    // {DAR High, DAR Low}
    if ( i == 0 ) {
      pRegs64[0] = R_L3_DRAM_BASEADDR + 0x0000001000;
    } else {
      pRegs64[0] = R_L3_DRAM_BASEADDR + 0x0000201000;
    }
    pRegs64 += 0x1;
  }
  // Channel Control =  32'h0000_0006
  // Reserved = 32'h0000_0000,
  pRegs64[0] = 0x0000000000000006;
  pRegs64 += 0x1;
  // {LLE Pointer High, LLE Pointer Low}
  pRegs64[0] = R_L3_DRAM_BASEADDR;
  
  __asm__ __volatile__ (
      "mv  a6, %[addr]\n\t" // Base ddr address
      "li x31, 0x40\n\t"
      "li  a7, 0x400000000000005\n\t" // Dest[59:58] = 0x1 == L2, numlines = 6 - 1
      "or a6, a6, a7\n\t"  // 6 lines to evict
      "csrw 0x89F, a6\n\t" // Evict 6 lines
      "fence\n\t"
      :
      : [addr] "r" ( R_L3_DRAM_BASEADDR )
      : "memory", "a5", "a6", "a7"
   );

  pcie_open_pll_handle();
  pcie_ss_power_up();
  pcie_phy_disable_eq();
  if ( pcie_ss_init(0) ) {
    return ET_FAIL;
  } else {
    // write 0 to R_PU_MBOX_PC_MM_BASEADDR for the VIP to wait on.
    // The VIP will wait till this test writes a 1 to R_PU_MBOX_PC_MM_BASEADDR later
    pRegs64 = ( uint64_t * )( R_PU_MBOX_PC_MM_BASEADDR );
    pRegs64[0] = 0x0;
    // PCIe controller DMA Read Engine Enable register (0x2c) = 32'h0001_0001
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_READ_ENGINE_EN_OFF_ADDRESS);
    pRegs32[0] = 0x00010001;
    // PCIe controller DMA Read Interrupt Mask register (0a8x) = 32'h0000_0000
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_READ_INT_MASK_OFF_ADDRESS);
    pRegs32[0] = 0x0;
    // PCIe controller DMA Read Linked List Error Enable register (0xc4) = 32'h0000_0001
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_READ_LINKED_LIST_ERR_EN_OFF_ADDRESS);
    pRegs32[0] = 0x1;
    // PCIe controller DMA Channel Control 1 register (0x300) = 32'h0400_0300
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_CH_CONTROL1_OFF_RDCH_0_ADDRESS);
    pRegs32[0] = 0x00400300;
    // PCIe controller DMA Linked List Pointer Low register
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_LLP_LOW_OFF_RDCH_0_ADDRESS);
    pRegs32[0] = R_L3_DRAM_BASEADDR & 0xffffffff;
    // PCIe controller DMA Linked List Pointer High register (0x320)
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_LLP_HIGH_OFF_RDCH_0_ADDRESS);
    pRegs32[0] = R_L3_DRAM_BASEADDR >> 32;
    // PCIe controller DMA Read Doorbell register (0x030) = 32'h0000_0000
    pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_READ_DOORBELL_OFF_ADDRESS);
    pRegs32[0] = 0x0;
    // write 1 to R_PU_MBOX_PC_MM_BASEADDR to signal VIP that we are done with setup
    pRegs64 = ( uint64_t * )( R_PU_MBOX_PC_MM_BASEADDR );
    pRegs64[0] = 0x0123456789abcdef;
    asm("fence");
    // check the rd dma done count. should be 9
    pRegs32 = ( uint32_t * ) ( R_PCIE_USRESR_BASEADDR + PSHIRE_USR0_DMA_RD_DONE_OFFSET );
    regVal = pRegs32[0];
    while ( regVal != 1 ) {
       regVal += pRegs32[0];
    }
    return ET_OK;
  }
}

#endif /* __PCIE_DMA_CONFIG_H */
