/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "maxion_bringup_aux.h"

void maxion_msg_sp_function_call(uint16_t function_id)
{
    volatile uint8_t *mbox2_addr =  (uint8_t*) R_PU_MBOX_MX_SP_BASEADDR;
    uint64_t baseAddrMboxIntTrg = R_PU_TRG_MAX_BASEADDR;
    Mbox_intr* mbox_int_regs;
    mbox_int_regs = ( Mbox_intr* ) baseAddrMboxIntTrg;

    while(*mbox2_addr !=0) asm volatile ("nop\n");

    *(mbox2_addr + MX_SP_MSG_MAX_ID_OFSET) = (uint8_t) CSRregRead( mhartid );
    *(mbox2_addr + MX_SP_MSG_TEST_ID_OFSET) = (uint8_t) function_id;
    *(mbox2_addr + MX_SP_MSG_TEST_ID_OFSET + 1) = (uint8_t) (function_id >> 8); 
    *(mbox2_addr + MX_SP_MSG_TYPE_OFSET)  = MX_SP_FUNCTION_CALL;

    asm volatile ("fence\n");

    // trigger interrupt to SP (via mbox IPI)
    mbox_int_regs->ipi_trigger_set = 0xf;
}


uint8_t maxion_msg_sp_wait_to_continue(void)
{
   if (*((uint8_t*)(R_PU_MBOX_MX_SP_BASEADDR + R_PU_MBOX_MX_SP_SIZE - 1)) != 0xA5) return 1;
   else return 0;


}

void maxion_msg_sp_wait_to_continue_clear(void)
{

   *(uint8_t*)(R_PU_MBOX_MX_SP_BASEADDR + R_PU_MBOX_MX_SP_SIZE - 1) = 0;
}


uint8_t maxion_msg_sp_get_test_result(void)
{
    return *(uint8_t*)(R_PU_MBOX_MX_SP_BASEADDR + R_PU_MBOX_MX_SP_SIZE - 2);
}

void maxion_msg_sp_send_mhartid(uint8_t hartid)
{
    *(uint8_t*)(R_PU_MBOX_MX_SP_BASEADDR + R_PU_MBOX_MX_SP_SIZE - 3) = hartid;

}

