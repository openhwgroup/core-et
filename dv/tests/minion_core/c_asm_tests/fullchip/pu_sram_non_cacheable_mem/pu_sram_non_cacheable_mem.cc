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

    volatile uint32_t* pAddr[6];
    volatile uintptr_t baseAddresses[] = {R_PU_SRAM_LO_BASEADDR, 
                                          R_PU_SRAM_MID_BASEADDR,
                                          R_PU_SRAM_HI_BASEADDR,
                                          0 };                             
     
    volatile uintptr_t mbox_sizes[] = { R_PU_SRAM_LO_SIZE, 
                                        R_PU_SRAM_MID_SIZE,
                                        R_PU_SRAM_HI_SIZE  }; 

    volatile uint32_t readData[6];

    volatile uint32_t testData[] = {0xdeadbeef, 0xabcdef01, 0x12345678, 0xd4ad4ee4, 0x6b6de602, 0xf3345a7a};
    
    volatile uint8_t i=0;

    C_TEST_START;

    fc_send_mbox_sync_to_sp(); //defined in fc_utils.h
    if (shire_id == 32) {
        wait_for_credit(0);
    } else { //if (shire_id == 0) {
        wait_for_credit(0); 
    }

    while (baseAddresses[i] != 0)
    {
        pAddr[0] = (uint32_t*)(baseAddresses[i]);                                     //start of range
        pAddr[1] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i]/2);                   //mid of range
        pAddr[2] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i] - WORD_OFFSET);       //end of range
        pAddr[3] = (uint32_t*)(baseAddresses[i]);                                     //start of range
        pAddr[4] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i]/2);                   //mid of range
        pAddr[5] = (uint32_t*)(baseAddresses[i] + mbox_sizes[i] - WORD_OFFSET);       //end of range


        if (shire_id == 32) {
            wait_for_credit(0);

            readData[3] = *pAddr[3]; 
            readData[4] = *pAddr[4]; 
            readData[5] = *pAddr[5];    

            for (int k=0; k<3; k++) {
                if (readData[k+3] != testData[k]){
                  C_TEST_FAIL;  
                } 
            }

            *pAddr[3] = testData[3];
            *pAddr[4] = testData[4];    
            *pAddr[5] = testData[5];  

            give_credit(0, 0, 0, 1);  //int shire_id, int minion_id, int thread_id, int credit_counter_num
            //write_esr(PP_USER, 0, REGION_OTHER, (1 << 15) | CREDINC_BASE | (0 << 1) | 0, 1);
        } else { //if (shire_id == 0) {
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

            give_credit(32, 0, 0, 0);  //int shire_id, int minion_id, int thread_id, int credit_counter_num
            //write_esr(PP_USER, 32, REGION_OTHER, (1 << 15) | CREDINC_BASE | (0 << 1) | 0, 1);
            wait_for_credit(1);

            readData[0] = *pAddr[0]; 
            readData[1] = *pAddr[1]; 
            readData[2] = *pAddr[2];

            for (int k=0; k<3; k++) {
                if (readData[k] != testData[k+3]){
                  C_TEST_FAIL;  
                } 
            }  
        }



        i++; 
    }//while 

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

/*****     < EOF >     *****/
