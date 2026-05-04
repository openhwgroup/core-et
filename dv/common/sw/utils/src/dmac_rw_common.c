/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$ 
* @version $Release$ 
* @date $Date$ 
* @author 
*
* @brief spi_api.c   
*/ 
/** 
 *  @Component    DMA 
 * 
 *  @Filename     dmac_rw_common.c 
 * 
 *  @Description  DMA functions for easy read and write
 * 
 *//*======================================================================== */ 
 
/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */ 
 
#include "cpu.h"
#include "soc.h"
#include "et.h"
#include "api.h"
#include "inth.h"
#include "print.h"

#include "DW_common.h"
#include "DW_ahb_dmac_public.h"
#include "DW_ahb_dmac_private.h"

#include "dmac_rw_common.h"

DW_DEFINE_THIS_FILE;

/* =============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * =============================================================================
 */
static volatile bool tfr_flag;
static volatile bool err_flag;
static volatile bool srcTran_flag;
static volatile bool dstTran_flag;
int volatile irq_counter;
 
/* =============================================================================
 * LOCAL TYPES AND DEFINITIONS
 * =============================================================================
 */

/* =============================================================================
 * LOCAL VARIABLES DECLARATIONS
 * =============================================================================
 */
struct dw_device *dmac;
struct dw_dmac_instance dwDmacInstance;
struct dw_list_head     dwListHead;
struct dw_dmac_param    dwDmacParam;
uint32_t dmaInProgress;
uint32_t dst_data, exp_data, *dst_addr, *src_addr, *real_src_addr, *real_dst_addr, *address;
uint32_t blockSize, numBlocks, numTr;
struct dw_dmac_channel_config ch_config;
uint32_t error;
enum dw_dmac_channel_number chNum;
int i, j, chIndex, errorCode;
int *dma_reloc_addr;

 /* ============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */
 
/*==================== Function Separator =============================*/

void dw_dmac_common_handler_wrapper(void)
{
  dw_dmac_common_handler(dmac);
    
  irq_counter++;
  dmaInProgress=0;
  printDbg("DMA IRQ number: %d\n", irq_counter);

  /* Check DMA completion */
  if( tfr_flag ) {

      if(err_flag == true) {

          // Wasn't expecting this !!
          printDbg("\nERROR: Received an err interrupt\n");
      }

      if(srcTran_flag == true) {

          // This interrupt should not be unmasked when
          // a memory device is on the source !!
          printDbg("\nERROR: Received an srcTran interrupt\n");
      }

      if(dstTran_flag == true) {

          // This interrupt should not be unmasked when
          // a memory device is on the destination !!
          printDbg("\nERROR: Received an dstTran interrupt\n");
      }
  }
  else {
      printDbg("FATAL: Unexpected DMA IRQ!!!\n");
      while(1);
  }
}

int dw_dmac_common_handler(struct dw_device *dev)
{
    struct dw_dmac_param *param;
    struct dw_dmac_portmap *portmap;
    dw_callback userCallback;
    struct dw_dmac_instance *instance;
    uint32_t reg;
    int retVal= true, chanIndex, callbackArg;
    uint32_t activeIntReq, mask;

    DMAC_COMMON_REQUIREMENTS(dev);

    param = (struct dw_dmac_param  *) dev->comp_param;
    instance = (struct dw_dmac_instance *) dev->instance;
    portmap = (struct dw_dmac_portmap *) dev->base_address;

    activeIntReq = DMAC_INP(portmap->status_int_l);
    
    if(DW_BIT_GET(activeIntReq, DMAC_STATUSINT_L_TFR)) {
        reg = DMAC_INP(portmap->status_tfr_l);
        
        for(int i=0; i<param->num_channels; i++) {
            mask = 1 << instance->ch_order[i];
            if(reg & mask) {
                chanIndex = instance->ch_order[i];
                break;
            }
        }

        instance->ch[chanIndex].block_cnt++;

        if(instance->ch[chanIndex].userCallback != NULL) {
            callbackArg = instance->ch[chanIndex].block_cnt;
            userCallback = instance->ch[chanIndex].userCallback;
            userCallback(dev, callbackArg);
        }

        dw_dmac_disableChannelIrq(dev, instance->ch[chanIndex].ch_num);
        instance->ch[chanIndex].src_byte_cnt = 0;
        instance->ch[chanIndex].dst_byte_cnt = 0;
        instance->ch[chanIndex].src_state = Dmac_idle;
        instance->ch[chanIndex].dst_state = Dmac_idle;
        DMAC_OUTP(mask, portmap->clear_srctran_l);
        DMAC_OUTP(mask, portmap->clear_dsttran_l);
        DMAC_OUTP(mask, portmap->clear_block_l);
        
        dw_dmac_maskIrq(dev, instance->ch[chanIndex].ch_num, Dmac_irq_all);

        DMAC_OUTP(mask, portmap->clear_tfr_l);
    }
    else {
        printDbg("ERROR: Wrong DMA interrupt type");
	retVal = false;
    }

    return retVal;
}

// read: source addr non-incr, dest increment
// write: source increment, dest non-incr
// ARGS: i_word_cnt must be a factor of 30
// ARGS: i_dma_sel: 0 for SP, 1 for PU
void dw_dmac_common_read(uint64_t i_src_addr, uint64_t i_dst_addr, uint32_t i_word_cnt, uint8_t i_dma_sel)
{
  dw_dmac_common_access(i_src_addr, i_dst_addr, i_word_cnt, i_dma_sel, 1);
}

void dw_dmac_common_write(uint64_t i_src_addr, uint64_t i_dst_addr, uint32_t i_word_cnt, uint8_t i_dma_sel)
{
  dw_dmac_common_access(i_src_addr, i_dst_addr, i_word_cnt, i_dma_sel, 0);
}

void dw_dmac_common_access(uint64_t i_src_addr, uint64_t i_dst_addr, uint32_t i_word_cnt, uint8_t i_dma_sel, uint8_t i_isread)
{
    int errCode;
    enum dw_dmac_transfer_type transType;
    uint32_t blocksTransferred;
    uint32_t reg;
    
    dmaInProgress = 1;
    
    dma_reloc_addr = i_dma_sel ? (uint32_t*) R_PU_DMA_RELOC_BASEADDR : (uint32_t*) R_SP_DMA_RELOC_BASEADDR;
    
    dmac = (struct dw_device *)malloc(sizeof(struct dw_device));
    dmac->name = "DMA Device";
    dmac->data_width = 32;
    dmac->base_address = i_dma_sel ? (void *)R_PU_DMA_CFG_BASEADDR : (void *)R_SP_DMA_BASEADDR;
    dmac->instance = &dwDmacInstance;
    dmac->comp_param = &dwDmacParam;
    dmac->comp_type = Dw_ahb_dmac;
    dmac->list = dwListHead;

    // Initialize the DMA controller
    //dw_dmac_autoCompParams(dmac);
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
    for (i=0;i<4;i++) {
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
    errCode = dw_dmac_init(dmac);

    chNum = 0x0101; //Dmac_channel0;
    transType = i_isread ? Dmac_transfer_row3 : Dmac_transfer_row2; // Select a transfer type
    blockSize = 30;
    numBlocks = (i_word_cnt / 30);  // Select the number of blocks

    real_src_addr = (uint32_t*)(i_src_addr & ~0x3);
    
    real_dst_addr = (uint32_t*)(i_dst_addr & ~0x3);

    printDbg("Aligned src_addr  : %0x\n", real_src_addr);
    printDbg("Aligned dst_addr  : %0x\n", real_dst_addr);
    
    *(dma_reloc_addr) = ((uint64_t)real_src_addr & 0xFFF0000000) >> 28;
    src_addr = (uint32_t*)((uint64_t)real_src_addr & 0x000FFFFFFF);
    
    *(dma_reloc_addr + 1) = ((uint64_t)real_dst_addr & 0xFFF0000000) >> 28;
    dst_addr = (uint32_t*)((uint64_t)real_dst_addr & 0x000FFFFFFF);
    dst_addr = (uint32_t*)((uint64_t)dst_addr | 0x0010000000);
    
    printDbg("After-reloc src_addr  : %0x\n", src_addr);
    printDbg("After-reloc dst_addr  : %0x\n", dst_addr);

    dw_dmac_enable(dmac);
    // Get the channel index from the enumerated type
    chIndex = dw_dmac_getChannelIndex(chNum);

    // -------
    // START : Channel configuration
    // -------
    
    // -------
    // Use a configuration API function to set the transfer type for
    // this example.
    // -------
    errorCode = dw_dmac_setTransferType(dmac, chNum, transType);
    if(errorCode != 0) {
	printDbg("\nERROR: Failed to set the transfer type row %d\n", transType);
	printDbg("\nERROR: Error code %0x\n", errorCode);
    }
    
    // Enable the interrupts on Channel zero
    errorCode = dw_dmac_enableChannelIrq(dmac, chNum);
    if(errorCode != 0) {
        printDbg("\nERROR: Failed to enable channel %d interrupts\n",
              chIndex);
    }

    // Now we can intialise the channel configuration structure for
    // DMA channel 0 by loading the structure members with the
    // contents of the DMAC's channel 0 registers.
    errorCode = dw_dmac_getChannelConfig(dmac, chNum, &ch_config);
    if(errorCode != 0) {
        printDbg("\nERROR: Failed to initialise configuration\n");
        printDbg("structure from the DMA registers on channel %d\n",
              chIndex);
    }

    // -------
    // Change the configuration structure members to initialise the
    // DMA channel for the chosen transfer.
    // -------

    // Set the Source and destination addresses
    // NOTE : We could also use the dw_dmac_setAddress() API function
    //        to perform this task.

    ch_config.sar = (uint64_t) src_addr;
    ch_config.dar = (uint64_t) dst_addr;

    // Set the source and destination burst transaction length
    // NOTE : We could also use the dw_dmac_setBurstTransLength() API
    //        function to perform this task.
    ch_config.ctl_src_msize = Dmac_msize_4;
    ch_config.ctl_dst_msize = Dmac_msize_4;

    // Set the Address increment type for the source and destination
    // NOTE : We could also use the dw_dmac_setAddressInc() API
    //        function to perform this task.
    ch_config.ctl_sinc = i_isread ? Dmac_addr_nochange  : Dmac_addr_increment;
    ch_config.ctl_dinc = i_isread ? Dmac_addr_increment : Dmac_addr_nochange;

    // Set the source and destination transfer width
    // NOTE : We could also use the dw_dmac_setTransWidth() API
    //        function to perform this task.
    ch_config.ctl_src_tr_width = Dmac_trans_width_32;
    ch_config.ctl_dst_tr_width = Dmac_trans_width_32;

    // Set the block size for the DMA transfer
    // Block size is the number of words of size Dmac_trans_width
    // (see transfer width above).
    // NOTE : We could also use the dw_dmac_setBlockTransSize() API
    //        function to perform this task.
    ch_config.ctl_block_ts = blockSize;

    // Set the transfer device type and flow controller
    // NOTE : We could also use the dw_dmac_setMemPeriphFlowCtl() API
    //        function to perform this task.
    ch_config.ctl_tt_fc = Dmac_mem2mem_dma;

    // -------
    // For all other configuration parameters in the DMA controller, we
    // are going to use the default (reset) values.
    // -------

    // -------
    // Now we can write the new configuration setting into the DMA
    // Controller device.
    // -------
    errorCode = dw_dmac_setChannelConfig(dmac, chNum, &ch_config);
    if(errorCode != 0) {
        printDbg("\nERROR: Failed to write configuration structure\n");
        printDbg("into the DMA controller registers on channel %d\n",
              chIndex);
        printDbg("\nERROR: Error code %0x\n", errorCode);
    }

    // -------
    // END : Channel configuration
    // -------

    // -------
    // Now that the channel is configured for the required DMA transfer
    // the transfer information is displayed to the user.
    // -------
    printDbg("\n/ -----\n");
    printDbg("  Channel number       %d\n", chIndex);
    printDbg("  Transfer type        Row %d\n", transType);
    printDbg("  Block Size           %d\n", blockSize);
    printDbg("  Number of blocks     %d\n", numBlocks);
    printDbg("  Source address       %x\n", ch_config.sar);
    printDbg("  Destination address  %x\n", ch_config.dar);
    printDbg("/ -----\n\n");

    // -------
    // Before beginning an interrupt driven transfer the user must
    // register a listener function in the DMA driver. This function
    // handles interrupts from the DMA controller that the driver
    // has no knowledge of how to handle. The listener function for this
    // example simply sets global flags to inform the example code of
    // an interrupt event.
    // -------
    dw_dmac_setListener(dmac, chNum, dw_dmac_user_listener);
    
    // Initialise global flags to false
    tfr_flag = false;
    err_flag = false;
    srcTran_flag = false;
    dstTran_flag = false;

    // -------
    // Now we just need to enable the DMA controller and begin
    // the interrupt driven DMA transfer.
    //
    // The userCallback function is called by the DMA driver IRQ Handler
    // when the transfer has completed. So the example callback
    // function that is being used here simply sets a global flag,
    // so that this code knows when the transfer is complete.
    // -------
    //dw_dmac_enable(dmac);
    errorCode = dw_dmac_startCommonTransfer(dmac, chNum, numBlocks, dw_dmac_user_callback);
    if(errorCode != 0) {
        printDbg("\nERROR: Failed to begin the interrupt transfer\n");
    }
    
    dw_dmac_waitForLastBlock(dmac, numBlocks, 0, i_isread);
    
}

void dw_dmac_user_callback(struct dw_device *dev, int eCode)
{
    // -------
    // This function is called by the IRQ handler.
    // Here we are going to set a global flag to inform the
    // application that the DMA transfer has completed.
    // -------
    tfr_flag = true;
}

void dw_dmac_user_listener(struct dw_device *dev, int eCode)
{
    // -------
    // This function is called by the IRQ handler.
    // We use this function to deal with interrupts that the DMA
    // driver is unable to cater for (these are application
    // specific). All of the interrupts handled by this function
    // are cleared by the DMA driver, so there's no need to clear here.
    // -------

    // Check the source of the interrupt

    if(eCode == Dmac_irq_err) {

        // Just inform the application that this interrupt has occured.
        err_flag = true;
    }

    if(eCode == Dmac_irq_srctran) {

        // Just inform the application that this interrupt has occured.
        srcTran_flag = true;
    }

    if(eCode == Dmac_irq_dsttran) {

        // Just inform the application that this interrupt has occured.
        dstTran_flag = true;
    }

    if(eCode == Dmac_irq_block) {

        // We fully expect this interrupt to fire each time the DMA
        // controller has completed the transfer of a block of data.
        // For this example we are not concerned that a block has
        // completed, only that the transfer completes. So we do
        // nothing here.
    }
}
