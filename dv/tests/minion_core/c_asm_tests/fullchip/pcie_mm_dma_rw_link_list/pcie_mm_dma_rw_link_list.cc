/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "minion.h"
#include "cpu.h"
#include "soc.h"
//#include "tb.h"
#include "ioshire_config.h"
#include <stdbool.h>
#include "pshire_common.h"
//#include "spy.h"


int main()
{
  volatile uint64_t              *pRegs64;
  volatile uint64_t     regVal;
  volatile uint32_t              *pRegs32;

  uint64_t shire_id = get_shire_id();

  
  C_TEST_START;  
  if (shire_id == 0) {
      C_TEST_PASS;
  }
  
  else { //it is master shire

 // RAJO commented 
 // Wait for the SP to bringup pshire and other logic
  //pRegs64 = ( uint64_t * )( R_PU_MBOX_MM_SP_BASEADDR );
  //regVal = pRegs64[0];
  // wait for the value to change
  //while ( regVal != 0x0123456789abcdef ) {
  //  regVal = pRegs64[0];
  //}

  wait_for_credit(0);


  // setup the descriptors for the dma     
  pRegs64 = ( uint64_t * )( R_L3_DRAM_BASEADDR );
  // setup 10 link list Elements for read
  for ( uint32_t i = 0; i < 9; i++ ) {
    // Channel Control = 32'h0000_0001
    // Transfer Size = 32'h0000_4000
    if ( ( i == 5 ) | (i == 8 ) ) {
      pRegs64[0] = 0x400000000009;
    } else {
      pRegs64[0] = 0x400000000001;
    }
    pRegs64 += 0x1;
    // {SAR_HIGH, SAR Low}
    pRegs64[0] = R_PCIE0_SLV_BASEADDR + ( 0x8000 * i );   
    pRegs64 += 0x1;
    // {DAR High, DAR Low}
    pRegs64[0] = R_L3_DRAM_BASEADDR + 0x1000 + ( 0x8000 * i );
    pRegs64 += 0x1;
  }
  // Channel Control =  32'h0000_0006
  // Reserved = 32'h0000_0000,
  pRegs64[0] = 0x0000000000000006;
  pRegs64 += 0x1;
  // {LLE Pointer High, LLE Pointer Low}
  pRegs64[0] = R_L3_DRAM_BASEADDR;
  
  pRegs64 = ( uint64_t * )( R_L3_DRAM_BASEADDR + 0x800);
  // setup 9 link list Elements for writes
  for ( uint32_t i = 0; i < 9; i++ ) {
    // Channel Control = 32'h0000_0001
    // Transfer Size = 32'h0000_4000
    if ( ( i == 5 ) | (i == 8 ) ) {
      pRegs64[0] = 0x400000000009;
    } else {
      pRegs64[0] = 0x400000000001;
    }
    pRegs64 += 0x1;
    // {SAR_HIGH, SAR Low}  
    pRegs64[0] = R_L3_DRAM_BASEADDR + 0x50000 + ( 0x8000 * i );  
    pRegs64 += 0x1;
    // {DAR High, DAR Low}
    pRegs64[0] = R_PCIE0_SLV_BASEADDR + 0x50000 + ( 0x8000 * i );
    pRegs64 += 0x1;
  }
  // Channel Control =  32'h0000_0006
  // Reserved = 32'h0000_0000,
  pRegs64[0] = 0x0000000000000006;
  pRegs64 += 0x1;
  
  // {LLE Pointer High, LLE Pointer Low}
  pRegs64[0] = R_L3_DRAM_BASEADDR + 0x800;
  
  __asm__ __volatile__ (
			"mv  a6, %[addr]\n\t" // Base ddr address
			"li x31, 0x40\n\t" // stride
			"li  a7, 0x800000000000005\n\t" // Dest[59:58] = 0x2 == L3, numlines = 6 - 1
			"or a6, a6, a7\n\t"  // 6 lines to evict
			"csrw 0x89F, a6\n\t" // Evict 6 lines
			"fence\n\t"
			:
			: [addr] "r" ( R_L3_DRAM_BASEADDR )
			: "memory", "a5", "a6", "a7"
			);
  __asm__ __volatile__ (
			"mv  a6, %[addr]\n\t" // Base ddr address
			"li x31, 0x40\n\t" // stride
			"li  a7, 0x800000000000005\n\t" // Dest[59:58] = 0x2 == L3, numlines = 6 - 1
			"or a6, a6, a7\n\t"  // 6 lines to evict
			"csrw 0x89F, a6\n\t" // Evict 6 lines
			"fence\n\t"
			:
			: [addr] "r" ( R_L3_DRAM_BASEADDR + 0x800 )
			: "memory", "a5", "a6", "a7"
			);
  
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
  
  
  // PCIe controller DMA Write Engine Enable register 32'h0001_0001
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_WRITE_ENGINE_EN_OFF_ADDRESS);
  pRegs32[0] = 0x00010001;
  // PCIe controller DMA Write Interrupt Mask register 32'h0000_0000
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_WRITE_INT_MASK_OFF_ADDRESS);
  pRegs32[0] = 0x0;
  // PCIe controller DMA Write Linked List Error Enable register = 32'h0000_0001
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_WRITE_LINKED_LIST_ERR_EN_OFF_ADDRESS);
  pRegs32[0] = 0x1;
  // PCIe controller DMA Channel Control 1 register 32'h0040_0300
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_CH_CONTROL1_OFF_WRCH_0_ADDRESS);
  pRegs32[0] = 0x00400300;
  // PCIe controller DMA Linked List Pointer Low register
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_LLP_LOW_OFF_WRCH_0_ADDRESS);
  pRegs32[0] = ( R_L3_DRAM_BASEADDR + 0x800 ) & 0xffffffff;
  // PCIe controller DMA Linked List Pointer High register
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_LLP_HIGH_OFF_WRCH_0_ADDRESS);
  pRegs32[0] = ( R_L3_DRAM_BASEADDR + 0x800 ) >> 32;
  // PCIe controller DMA Write Doorbell register = 32'h0000_0000
  pRegs32 = (uint32_t *)(R_PCIE0_DBI_SLV_BASEADDR + PE0_DWC_PCIE_CTL_AXI_SLAVE_PF0_DMA_CAP_DMA_WRITE_DOORBELL_OFF_ADDRESS);
  pRegs32[0] = 0x0;
  
  
  // write 1 to R_PU_MBOX_PC_MM_BASEADDR to signal VIP that we are done with setup
  pRegs64 = ( uint64_t * )( R_PU_MBOX_PC_MM_BASEADDR );
  pRegs64[0] = 0x0123456789abcdef;
  asm("fence");
  
  pRegs32 = (uint32_t *)(R_PCIE_USRESR_BASEADDR + PSHIRE_USR0_DMA_WR_XFER_OFFSET);
  // trigger the DMA write 9 times.
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  pRegs32[0] = 1;
  
  // Expect two interrups on DMA wr and DMA rd channels
  for ( int i = 0; i < 1; i++ ) {
    // check interrupt
    pcie_ss_wait_dma_int(0);
    // clear interrupt
    pcie_ss_clr_dma_int(0);
    // check interrupt
    pcie_ss_wait_dma_int(1);
    // clear interrupt
    pcie_ss_clr_dma_int(1);
  }

  // check the rd dma done count. should be 9
  pRegs32 = ( uint32_t * ) ( R_PCIE_USRESR_BASEADDR + PSHIRE_USR0_DMA_RD_DONE_OFFSET );
  regVal = pRegs32[0];
  while ( regVal != 9 ) {
    regVal += pRegs32[0];
  }
  // check the wr dma done count. should be 9
  pRegs32 = ( uint32_t * ) ( R_PCIE_USRESR_BASEADDR + PSHIRE_USR0_DMA_WR_DONE_OFFSET );
  regVal = pRegs32[0];
  while ( regVal != 9 ) {
    regVal += pRegs32[0];
  }

  pRegs32 = ( uint32_t * ) ( R_PCIE_USRESR_BASEADDR + PSHIRE_USR0_DMA_RD_DONE_OFFSET );
  regVal = pRegs32[0];
  if ( regVal != 0x0 ) {
    C_TEST_FAIL;
  }

  pRegs32 = ( uint32_t * ) ( R_PCIE_USRESR_BASEADDR + PSHIRE_USR0_DMA_WR_DONE_OFFSET );
  regVal = pRegs32[0];
  if ( regVal != 0x0 ) {
    C_TEST_FAIL;
  }

  C_TEST_PASS;
  } //end else master shire
}

/*****     < EOF >     *****/
