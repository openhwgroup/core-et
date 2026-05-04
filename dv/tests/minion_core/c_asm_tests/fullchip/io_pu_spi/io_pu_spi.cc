/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */



#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#include "regtest.h"
#include "spi_pu_regTest.h"

#include "fc_utils.h"


int main()
{
    uint64_t shire_id = get_shire_id();

 


    uintptr_t baseAddress = R_PU_SPI_BASEADDR;
    REGTEST_t *pRegs;
    pRegs = ssiRegs; //defined in "spi_pu_regTest.h"
     
    C_TEST_START;
    if (shire_id == 32) {
        fc_send_mbox_sync_to_sp(); //defined in fc_utils.h	
        wait_for_credit(0); //wait until SP finishes configuration of everything
        regtest_checkResetValues(baseAddress, pRegs); //inline function from regtest.h
        C_TEST_PASS;
    } //end if shire-id
    else { C_TEST_PASS; } //in case of any other shire

} /* main() end */

/*****     < EOF >     *****/
