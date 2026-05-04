/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "fc_utils.h"


#define ldOffset  0x1000
#define WORD_OFFSET 4

int main()
{
    volatile uint64_t shire_id = get_shire_id();

    volatile uint32_t* pAddr[3];
    volatile uintptr_t baseAddresses[] = {R_PU_SRAM_LO_BASEADDR, 
                                          R_PU_SRAM_MID_BASEADDR,
                                          R_PU_SRAM_HI_BASEADDR,
                                          0 };                             
     
    volatile uintptr_t mbox_sizes[] = { R_PU_SRAM_LO_SIZE, 
                                        R_PU_SRAM_MID_SIZE,
                                        R_PU_SRAM_HI_SIZE  }; 

    volatile uint32_t readData[3];

    const uint32_t testData[] = {0xdeadbeef, 0xabcdef01, 0x12345678};
    
    volatile uint8_t i=0;

    fc_send_mbox_sync_to_sp();

    C_TEST_START;

    while (baseAddresses[i] != 0)
    {
        if (shire_id == 32) {
            pAddr[0] = (uint32_t*)(baseAddresses[i] + 128);                               //start of range
            pAddr[1] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i]/2 + 128);             //mid of range
            pAddr[2] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i] - WORD_OFFSET - 128); //end of range            
        } else { //shire_id == 0
            pAddr[0] = (uint32_t*)(baseAddresses[i]);                                     //start of range
            pAddr[1] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i]/2);                   //mid of range
            pAddr[2] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i] - WORD_OFFSET);       //end of range
        } 
     
        *pAddr[0] = testData[0];
        *pAddr[1] = testData[1];
        *pAddr[2] = testData[2];

        readData[0] = *pAddr[0]; 
        readData[1] = *pAddr[1]; 
        readData[2] = *pAddr[2];

        for (int k=0; k<3; k++) {
            if (readData[k] != testData[k]){
              C_TEST_FAIL;  
            } 
        }  

        i++; 
    }//while 

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

/*****     < EOF >     *****/
