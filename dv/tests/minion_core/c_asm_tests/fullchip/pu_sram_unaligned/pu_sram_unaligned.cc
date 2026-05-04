/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#define WORD_OFFSET 4
#define DWORD_OFFSET 8

int main()
{
    volatile uint64_t shire_id = get_shire_id();
    volatile uint64_t index;

    volatile uintptr_t curr_addr[3];

    volatile uint8_t*  addr_8;
    volatile uint16_t* addr_16;
    volatile uint32_t* addr_32;
    volatile uint64_t* addr_64;

    volatile uint8_t  write_data_8,  read_data_8;
    volatile uint16_t write_data_16, read_data_16;
    volatile uint32_t write_data_32, read_data_32;
    volatile uint64_t write_data_64, read_data_64;    

    volatile uintptr_t baseAddresses[] = {R_PU_SRAM_LO_BASEADDR, 
                                          R_PU_SRAM_MID_BASEADDR,
                                          R_PU_SRAM_HI_BASEADDR};    

    volatile uintptr_t mbox_sizes[] = { R_PU_SRAM_LO_SIZE,  /* -0x1000 because of the preload */
                                        R_PU_SRAM_MID_SIZE,
                                        R_PU_SRAM_HI_SIZE}; /* -0x1000 because of the stack */

    volatile uint16_t testData_16 = 0xFE00;
    volatile uint32_t testData_32 = 0xF1234F00;
    volatile uint64_t testData_64 = 0xFAADEEDBEEFAAF00;

    volatile int i=0;
    volatile int k=0;

    C_TEST_START;
     
    for (i=0; i<3 ; i++) {    

        if (shire_id == 32) {
            curr_addr[0] = (baseAddresses[i] + 128);                      //start of range
            //curr_addr[1] = (baseAddresses[i] + mbox_sizes[i]/2 + 128);    //mid of range
            curr_addr[1] = (baseAddresses[i] + mbox_sizes[i] - 192);      //end of range            
        } else { //shire_id == 0
            curr_addr[0] = (baseAddresses[i]);                            //start of range
            //curr_addr[1] = (baseAddresses[i] + mbox_sizes[i]/2);          //mid of range
            curr_addr[1] = (baseAddresses[i] + mbox_sizes[i] - 64);       //end of range
        } 

        for (k=0; k<2 ; k++) {  

            //8 bit access
            for (int j=0; j<64; j++) {
                addr_8 = (uint8_t*)(curr_addr[k] + j);
                index = shire_id + j;  
                write_data_8  = (uint8_t)(index);
                *addr_8 = write_data_8;
            }
            for (int j=0; j<64; j++) {
                addr_8 = (uint8_t*)(curr_addr[k] + j);
                index = shire_id + j; 
                write_data_8  = (uint8_t)(index);
                read_data_8 = *addr_8;

                if (write_data_8 != read_data_8){
                    C_TEST_FAIL;    
                } 
            }

            //16 bit access
            for (int j=0; j<32; j++) {
                addr_16 = (uint16_t*)(curr_addr[k] + j*2);
                index = shire_id + j + 0x40;  
                write_data_16  = (uint16_t)(testData_16 + index);
                *addr_16 = write_data_16;
            }
            for (int j=0; j<32; j++) {
                addr_16 = (uint16_t*)(curr_addr[k] + j*2);
                index = shire_id + j + 0x40;  
                write_data_16  = (uint16_t)(testData_16 + index);
                read_data_16 = *addr_16;

                if (write_data_16 != read_data_16){
                    C_TEST_FAIL;    
                } 
            }

            //32 bit access
            for (int j=0; j<16; j++) {
                addr_32 = (uint32_t*)(curr_addr[k] + j*4);
                index = shire_id + j + 0x80;  
                write_data_32  = (uint32_t)(testData_32 + index);
                *addr_32 = write_data_32;
            }
            for (int j=0; j<16; j++) {
                addr_32 = (uint32_t*)(curr_addr[k] + j*4);
                index = shire_id + j + 0x80;  
                write_data_32  = (uint32_t)(testData_32 + index);
                read_data_32 = *addr_32;

                if (write_data_32 != read_data_32){
                    C_TEST_FAIL;    
                } 
            }

            //64 bit access
            for (int j=0; j<8; j++) {
                addr_64 = (uint64_t*)(curr_addr[k] + j*8);
                index = shire_id + j + 0xC0;  
                write_data_64  = (uint64_t)(testData_64 + index);
                *addr_64 = write_data_64;
            }
            for (int j=0; j<8; j++) {
                addr_64 = (uint64_t*)(curr_addr[k] + j*8);
                index = shire_id + j + 0xC0; 
                write_data_64  = (uint64_t)(testData_64 + index);
                read_data_64 = *addr_64;

                if (write_data_64 != read_data_64){
                    C_TEST_FAIL;    
                } 
            }   
        }

    }//while 

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

/*****     < EOF >     *****/



                // addr_16 = (uint16_t*)(curr_addr[k]);
                // index = shire_id + 3 + 0x10;  
                // write_data_16  = (uint16_t)(testData_16 + index);
                
                // *addr_16 = write_data_16;
                // read_data_16 = *addr_16;

                // if (write_data_16 != read_data_16){
                //     C_TEST_FAIL;    
                // }


                // addr_32 = (uint32_t*)(curr_addr[k]);
                // index = shire_id + 3 + 0x20;
                // write_data_32  = (uint32_t)(testData_32 + index);

                // *addr_32 = write_data_32;
                // read_data_32 = *addr_32;

                // if (write_data_32 != read_data_32){
                //     C_TEST_FAIL;    
                // }

                // addr_64 = (uint64_t*)(curr_addr[k]);
                // index = shire_id + 3 + 0x50;
                // write_data_64  = (uint64_t)(testData_64 + index);

                // *addr_64 = write_data_64;
                // read_data_64 = *addr_64;

                // if (write_data_64 != read_data_64){
                //     C_TEST_FAIL;    
                // }