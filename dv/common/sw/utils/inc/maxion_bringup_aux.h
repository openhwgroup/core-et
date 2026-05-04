/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>
#include "hal_device.h"
#include "mbox_intr.h"

#define MX_SP_MSG_TEST_NAME    0x1
#define MX_SP_MSG_TEST_PRINT   0x2
#define MX_SP_MSG_TEST_RESULT  0x3
#define MX_SP_MSG_TESTS_DONE   0x4
#define MX_SP_MSG_PRINT        0x5
#define MX_SP_FUNCTION_CALL    0x6

#define MX_SP_MSG_TYPE_OFSET           0x0
#define MX_SP_MSG_MAX_ID_OFSET         0x1
#define MX_SP_MSG_TEST_ID_OFSET        0x2
#define MX_SP_MSG_TEST_RESULT_OFSET    0x4
#define MX_SP_MSG_TEXT_OFSET           0x5


void maxion_msg_sp_function_call(uint16_t function_id);
uint8_t maxion_msg_sp_wait_to_continue(void);
void maxion_msg_sp_wait_to_continue_clear(void);
uint8_t maxion_msg_sp_get_test_result(void);
void maxion_msg_sp_send_mhartid(uint8_t hartid);


/*-------------------------------------------------------------------------*//**
 * @DEFINITION   CSRregRead                                                                   
 *
 * @BRIEF        Read CSR register
 *
 * @DESCRIPTION  This macro is used to read CSR register.
 *               reg is name from Minion  CSR Register table
 *
 *//*------------------------------------------------------------------------ */
#define REGNAME(x) #x
#define CSR(x) REGNAME(x)
#define CSRregRead( reg ) (        \
{                                  \
    uint64_t ret;                  \
    __asm__ __volatile(            \
        "csrr %0, " CSR(reg) "\n"  \
        :"=r"(ret)                 \
        :                          \
        :);                        \
    ret;                           \
}                                  \
)

