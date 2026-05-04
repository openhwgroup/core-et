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
 
#ifndef DW_DMAC_RW_COMMON_H
#define DW_DMAC_RW_COMMON_H

void dw_dmac_user_listener(struct dw_device *dev, int eCode);
void dw_dmac_user_callback(struct dw_device *dev, int eCode);
void dw_dmac_common_handler_wrapper(void);
int  dw_dmac_common_handler(struct dw_device *dev);
void dw_dmac_common_access(uint64_t i_src_addr, uint64_t i_dst_addr, uint32_t i_word_cnt, uint8_t i_dma_sel, uint8_t i_isread);
void dw_dmac_common_read(uint64_t i_src_addr, uint64_t i_dst_addr, uint32_t i_word_cnt, uint8_t i_dma_sel);
void dw_dmac_common_write(uint64_t i_src_addr, uint64_t i_dst_addr, uint32_t i_word_cnt, uint8_t i_dma_sel);

#endif
