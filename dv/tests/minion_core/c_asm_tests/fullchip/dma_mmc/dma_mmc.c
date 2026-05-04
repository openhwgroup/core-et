/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cpu.h"
//#include "soc.h"

#include "macros.h"
#include "ioshire_config.h"
#include "et.h"
#include "minion.h"

//#include "tb.h"
#include "inth.h"

#include "hal_device.h"
#include "tcParam.h"

#include "DW_common.h"
#include "DW_ahb_dmac_public.h"
#include "DW_ahb_dmac_private.h"
#include "DW_common_dbc.h"

#include "mem_test.h"

extern uint32_t __bss_end__;
extern uint32_t __code_end__;

// Declare global variables
static volatile bool tfr_flag;
static volatile bool err_flag;
static volatile bool srcTran_flag;
static volatile bool dstTran_flag;
int volatile irq_counter;

// Function prototypes
void userListener(struct dw_device *dev, int eCode);
void userCallback(struct dw_device *dev, int eCode);
uint8_t check_master(void); 
void ito(int a, char *b);


#define ET_DIAG_PUTCHAR         (0x1)

/* Available memories */
MEMTEST_PARAMS_t memTable[] =
/*   StartAddr                          Size                       BusDataW   NumAccess   PattType    Patt        MemName  */
{ 
  {  R_PU_SRAM_LO_BASEADDR,             R_PU_SRAM_LO_SIZE,         32,         0,         0,          0x1234FEDC, "PURAMLO"  },
  {  R_PU_SRAM_MM_MX_BASEADDR,          R_PU_SRAM_MM_MX_SIZE,      32,         0,         0,          0x1234FEDC, "PURAMMX"  },
  {  R_PU_SRAM_HI_BASEADDR,             R_PU_SRAM_HI_SIZE,         32,         0,         0,          0x1234FEDC, "PURAMHI"  },
  {  R_L3_DRAM_BASEADDR,                R_L3_DRAM_SIZE,            32,         0,         0,          0x1234FEDC, "L3DRAM"   },
  {  R_PU_SRAM_MID_BASEADDR,            R_PU_SRAM_MID_SIZE,        32,         0,         0,          0x1234FEDC, "PURAMMID" },
  {  R_DRCT_MCODE_BASEADDR,             R_DRCT_MCODE_SIZE,          32,         0,         0,          0x1234FEDC, "DRCTDRAM" },
  {  0,                                 0,                         32,         0,         0,          0,          "JUNK"     } 
};

volatile uintptr_t baseAddresses;
struct dw_device dmac;
struct dw_dmac_instance dwDmacInstance;
struct dw_list_head     dwListHead;
struct dw_dmac_param    dwDmacParam;
uint32_t dst_data, exp_data, *dst_addr, *src_addr, *real_src_addr, *real_dst_addr, *address;
uint32_t blockSize, numBlocks, numTr;
struct dw_dmac_channel_config ch_config;
uint32_t error;
enum dw_dmac_channel_number chNum;
int i, j;
volatile int chIndex, errorCode;
int *dma_reloc_addr = R_PU_DMA_RELOC_BASEADDR;

inline void __attribute__((always_inline)) evict_sw(uint64_t use_tmask, uint64_t dst, uint64_t way, uint64_t set, uint64_t num_lines, uint64_t warl)
{
   uint64_t csr_enc = ((warl      & 0x73FFFFFFFFFC3F30ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((dst       & 0x3                   ) << 58 ) |
                      ((set       & 0xF                   ) << 14 ) |
                      ((way       & 0x3                   ) << 6  ) |
                      ((num_lines & 0xF                   )       ) ;

   __asm__ __volatile__ ( "csrw 0x7f9, %[csr_enc]\n" : : [csr_enc] "r" (csr_enc));

}

inline void __attribute__((always_inline)) clear_l1d() {

   for (int w = 0; w < 4; ++w) {
      evict_sw(false, 0x3, w, 0, 15, 0);
   }
   WAIT_CACHEOPS;
}


inline void __attribute__((always_inline)) my_diag_command(uint8_t command, uint64_t payload)
{
   uint64_t csr_enc = ((uint64_t) command << 56) |
                      (payload & 0x00FFFFFFFFFFFFFFULL);
   __asm__ __volatile__ (
         "csrw validation1, %[csr_enc]\n"
         :
         : [csr_enc] "r" (csr_enc)
         :
   );
}

//
// Print to STDOUT
//
inline void __attribute__((always_inline)) my_putchar(char c)
{
   my_diag_command(ET_DIAG_PUTCHAR, (uint64_t) c);
}
inline void __attribute__((always_inline)) my_printf(const char *str)
{
   while (*str) {
      my_putchar(*str);
      str++;
   }
}

void onAssert__(char const *file, unsigned line)
{
//    my_printf( "File: %s   line: %d", file, line );
}

void myDmaIsr(void)
{
    /* Call driver irqHandler */
    dw_dmac_irqHandler(&dmac);
    irq_counter++;
   // my_printf("DMA IRQ number: %d\n", irq_counter);

    /* Check DMA completion and restart transfer */
    if( tfr_flag ) {    // transfer is in progress

        if(err_flag == true) {
            // Wasn't expecting this !!
            my_printf("\nERROR: Received an err interrupt\n");
        }

        if(srcTran_flag == true) {
            // This interrupt should not be unmasked when a memory device is on the source !!
            my_printf("\nERROR: Received an srcTran interrupt\n");
        }

        if(dstTran_flag == true) {
            // This interrupt should not be unmasked when a memory device is on the destination !!
            my_printf("\nERROR: Received an dstTran interrupt\n");
        }
    }
    else {
        my_printf("ERROR: Unexpected DMA IRQ!!!\n");
        while(1);
    }

    char src_data[128];
    char rd_data[128];
    char wr_addr[128];
    char rd_addr[128];

    for(i=0; i<numBlocks; i++)
        for(j=0; j<blockSize; j++) {
            exp_data = *(uint32_t *) real_src_addr++;
            dst_data = *(volatile uint32_t *) real_dst_addr++;
            if(exp_data != dst_data) {
                my_printf("\nFATAL: Data Mismatch between SRC and DST addrress! ");                
                ito(exp_data, src_data);
                ito(dst_data, rd_data);
                ito(real_src_addr-1, wr_addr);
                ito(real_dst_addr-1, rd_addr);              
                my_printf(" Expected data: ");
                my_printf(src_data);
                my_printf(" at SRAM address: ");
                my_printf(wr_addr);
                my_printf(" , but reported data: ");
                my_printf(rd_data);
                my_printf(" at DRAM address: ");
                my_printf(rd_addr);
                my_printf("\n");
                //my_printf("\ndst_addr = %x : dst_data = %x : exp_data = %x", real_dst_addr-1, dst_data, exp_data);
                error = 1;
            }
        }
} /* dma_Handler() */

/* mem_map Test Function */
int main (void)
{
    et_status_t est = ET_OK;
    //tcParam_t   *pTcParams;
    //uint64_t channel, blockSize, src_index, dst_index, src_offset, dst_offset, num_of_trans;  
    int errCode;

    enum dw_dmac_transfer_type transType;

    my_printf("/ DMA interrupt driven transfer from memory to memory");
    my_printf("\n/ -------\n\n");

    if (check_master()){
        inth_init(); 

    irq_counter = 0;
    error = 0;

    inth_setupInterrupt(PU_PLIC_DMA_INTR_ID, myDmaIsr, 7);             
    inth_enableInterrupt(PU_PLIC_DMA_INTR_ID, 10); 

    //dmac = (struct dw_device *)malloc(sizeof(struct dw_device));
    dmac.name = "DMA Device";
    dmac.data_width = 32;
    dmac.base_address = (void *)R_PU_DMA_CFG_BASEADDR;
    dmac.instance = &dwDmacInstance;
    dmac.comp_param = &dwDmacParam;
    dmac.comp_type = Dw_ahb_dmac;
    dmac.list = dwListHead;

    // Initialize the DMA controller
    dwDmacParam.num_master_int = 1;
    dwDmacParam.num_channels = 4;
    dwDmacParam.num_hs_int = 0;
    dwDmacParam.intr_io = 2;
    dwDmacParam.mabrst = 1;
    dwDmacParam.big_endian = 0;
    dwDmacParam.s_hdata_width = 32;
    dwDmacParam.m_hdata_width[0] = 32;
    dwDmacParam.m_hdata_width[1] = 32;
    dwDmacParam.m_hdata_width[2] = 32;
    dwDmacParam.m_hdata_width[3] = 32;
    dwDmacParam.encoded_params = 0;
    for (i=0;i<8;i++) {
       dwDmacParam.ch_max_blk_size[i] = 31;
       dwDmacParam.ch_dtw[i] = 32;
       dwDmacParam.ch_stw[i] = 32;
       dwDmacParam.ch_stat_dst[i] = 0;
       dwDmacParam.ch_stat_src[i] = 0;
       dwDmacParam.ch_dst_sca_en[i] = 0;
       dwDmacParam.ch_src_gat_en[i] = 0;
       dwDmacParam.ch_lock_en[i] = 0;
       dwDmacParam.ch_multi_blk_en[i] = 1;
       dwDmacParam.ch_ctl_wb_en[i] = 0;
       dwDmacParam.ch_hc_llp[i] = 0;
       dwDmacParam.ch_fc[i] = 0;
       dwDmacParam.ch_max_mult_size[i] = 8;
       dwDmacParam.ch_dms[i] = 0;
       dwDmacParam.ch_lms[i] = 0;
       dwDmacParam.ch_sms[i] = 0;
       dwDmacParam.ch_fifo_depth[i] = 16;
   }
    
    // Initialize the DMA controller
    errCode = dw_dmac_init(&dmac);
//    my_printf("dw_dmac_init returned: %d\n", errCode);

    numTr = 2;

    chNum = 0x0101 << 0; //Dmac_channel0; // Select a DMA channel
    transType = Dmac_transfer_row1; // Select a transfer type
    blockSize = 1; // Select the size of a block
    numBlocks = 1;  // Select the number of blocks

    baseAddresses = (memTable[3].startAddress + 0x8f0) & 0xFFFFFFFFFC;
    real_src_addr = (uint64_t *)baseAddresses;

    baseAddresses = (memTable[0].startAddress + 0x3c0) & 0xFFFFFFFFFC;
    real_dst_addr = (uint64_t *)baseAddresses;

    // Source memory data is initialized
    address = real_src_addr;
    
    // Initialize source memory
    while(address <= (real_src_addr + blockSize)) {
        *(address) = (uint64_t) address;
        address++;
    }
    
    clear_l1d();
    WAIT_CACHEOPS;

    *(dma_reloc_addr) = ((uint64_t)real_src_addr & 0xFFF0000000) >> 28;
    src_addr = (uint64_t)real_src_addr & 0x000FFFFFFF;

    // Destination memory data is initialized to zero
    address = real_dst_addr;

    while(address <= (real_dst_addr + blockSize)) {
        *(address) = (uint64_t) 0x0;
        address++;
    }

    *(dma_reloc_addr + 1) = ((uint64_t)real_dst_addr & 0xFFF0000000) >> 28;
    dst_addr = (uint64_t)real_dst_addr & 0x000FFFFFFF;
    dst_addr = (uint64_t)dst_addr | 0x0010000000;

    dw_dmac_enable(&dmac);

    // Get the channel index from the enumerated type
    chIndex = dw_dmac_getChannelIndex(chNum);

    // -------
    // START : Channel configuration
    // -------
    
    // Use a configuration API function to set the transfer type for this example.
    errorCode = dw_dmac_setTransferType(&dmac, chNum, transType);
    if(errorCode == 0) {
        my_printf("\nINFO: Success to set the transfer type row \n");
    }
    else {
        my_printf("\nERROR: Failed to set the transfer type row\n");
    }
    
    // Enable the interrupts on Channel zero
    errorCode = dw_dmac_enableChannelIrq(&dmac, chNum);
    if(errorCode == 0) {
        my_printf("\nINFO: Success to enable channel interrupts\n");
    }
    else {
        my_printf("\nERROR: Failed to enable channel interrupts\n");
    }

    // Now we can intialise the channel configuration structure for DMA channel 0 
    // by loading the structure members with the contents of the DMAC's channel 0 registers.
    errorCode = dw_dmac_getChannelConfig(&dmac, chNum, &ch_config);
    if(errorCode == 0) {
        my_printf("\nINFO: Success to initialise configuration structure from the DMA registers on channel\n");
    }
    else {
        my_printf("\nERROR: Failed to initialise configuration structure from the DMA registers on channel\n");
    }

    // -------
    // Change the configuration structure members to initialise the DMA channel for the chosen transfer.
    // -------

    // Set the Source and destination addresses
    ch_config.sar = (uint64_t) src_addr;
    ch_config.dar = (uint64_t) dst_addr;

    // Set the source and destination burst transaction length
    ch_config.ctl_src_msize = Dmac_msize_4;
    ch_config.ctl_dst_msize = Dmac_msize_4;

    // Set the Address increment type for the source and destination
    ch_config.ctl_sinc = Dmac_addr_increment;
    ch_config.ctl_dinc = Dmac_addr_increment;

    // Set the source and destination transfer width
    ch_config.ctl_src_tr_width = Dmac_trans_width_32;
    ch_config.ctl_dst_tr_width = Dmac_trans_width_32;

    // Set the block size for the DMA transfer
    // Block size is the number of words of size Dmac_trans_width (see transfer width above).
    ch_config.ctl_block_ts = blockSize;

    // Set the transfer device type and flow controller
    ch_config.ctl_tt_fc = Dmac_mem2mem_dma;

    // -------
    // For all other configuration parameters in the DMA controller, we are going to use the default (reset) values.
    // -------

    // -------
    // Now we can write the new configuration setting into the DMA Controller device.
    // -------
    errorCode = dw_dmac_setChannelConfig(&dmac, chNum, &ch_config);
    if(errorCode == 0) {
        my_printf("\nINFO: Success to write configuration structure into the DMA controller registers on channel\n");
    }
    else {
        my_printf("\nERROR: Failed to write configuration structure into the DMA controller registers on channel\n");
    }

    // -------
    // END : Channel configuration
    // -------

    // -------
    // Before beginning an interrupt driven transfer the user must register a listener function in the DMA driver. 
    // This function handles interrupts from the DMA controller that the driver has no knowledge of how to handle. 
    // The listener function for this example simply sets global flags to inform the example code of an interrupt event.
    // -------
    dw_dmac_setListener(&dmac, chNum, userListener);
    
    // Initialise global flags to false
    tfr_flag = false;
    err_flag = false;
    srcTran_flag = false;
    dstTran_flag = false;

    // -------
    // Now we just need to enable the DMA controller and begin the interrupt driven DMA transfer.
    //
    // The userCallback function is called by the DMA driver IRQ Handler when the transfer has completed. 
    // So the example callback function that is being used here simply sets a global flag, so that this code knows when the transfer is complete.
    // -------
    //dw_dmac_enable(dmac);
    errorCode = dw_dmac_startTransfer(&dmac, chNum, numBlocks, userCallback);
    if(errorCode == 0) {
        my_printf("\nINFO: Success to begin the interrupt transfer\n");
    }
    else {
        my_printf("\nERROR: Failed to begin the interrupt transfer\n");
    }

    // -------
    // Now we monitor the transfers progress and wait for completion
    // -------

    while(tfr_flag == false) { // transfer is in progress
        // Just checking here that no error condition interrupts fire during the data transfer.

        if(err_flag == true) {
            // Wasn't expecting this !!
            my_printf("\nERROR: Recieved an err interrupt\n");
        }

        if(srcTran_flag == true) {
            // This interrupt should not be unmasked when a memory device is on the source !!
            my_printf("\nERROR: Received an srcTran interrupt\n");
        }

        if(dstTran_flag == true) {
            // This interrupt should not be unmasked when a memory device is on the destination !!
            my_printf("\nERROR: Received an dstTran interrupt\n");
        }
    }
}
    if( error )
        est = ET_FAIL;

    return( est );

} /* dmac_pu_async_Test() */

void userCallback(struct dw_device *dev, int eCode)
{
    // This function is called by the IRQ handler.
    // Here we are going to set a global flag to inform the application that the DMA transfer has completed.
    tfr_flag = true;
}

void userListener(struct dw_device *dev, int eCode)
{
    // This function is called by the IRQ handler.
    // We use this function to deal with interrupts that the DMA driver is unable to cater for (these are application specific). 
    // All of the interrupts handled by this function are cleared by the DMA driver, so there's no need to clear here.

    // Check the source of the interrupt
    if(eCode == Dmac_irq_err) {        
        err_flag = true;     // Just inform the application that this interrupt has occured.
    }
    if(eCode == Dmac_irq_srctran) {        
        srcTran_flag = true; // Just inform the application that this interrupt has occured.
    }
    if(eCode == Dmac_irq_dsttran) {
        dstTran_flag = true; // Just inform the application that this interrupt has occured.        
    }
    if(eCode == Dmac_irq_block) {
        // We fully expect this interrupt to fire each time the DMA controller has completed the transfer of a block of data.
        // For this example we are not concerned that a block has completed, only that the transfer completes. So we do nothing here.
    }
}


uint8_t check_master(void)
{ 
  uint64_t hart_id = get_hart_id();
          
  if( (hart_id & 1) == 0) {
     uint64_t minion_id = ((hart_id>>1) & 0x1F);
     uint64_t shire_id  = ((hart_id>>6) & 0x3F);
     if (shire_id == 32 && minion_id == 0) return 1;  
     else return 0;
  }   
  return 0;
}

void ito(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}
