/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#define MPROT_ADDR 0x004
#define MEMORY_ROW_SIZE_BYTES 64


int main()
{
    uintptr_t baseAddress =  R_PU_MBOX_MM_SP_BASEADDR;
    uintptr_t mbox_size =    R_PU_MBOX_MM_SP_SIZE;

    uint8_t number_of_sectors = mbox_size / MEMORY_ROW_SIZE_BYTES;

    uintptr_t segmentsAddr[] = { baseAddress,
                                 baseAddress + number_of_sectors * MEMORY_ROW_SIZE_BYTES /2,
                                 baseAddress + mbox_size - MEMORY_ROW_SIZE_BYTES,
                                 0};

    uint8_t accessType[] = {1, 2, 4, 8, 0};

    volatile uint8_t* addr8;
    volatile uint16_t* addr16;
    volatile uint32_t* addr32;
    volatile uint64_t* addr64;

    const uint8_t testData8[]   = {0xde, 0xab, 0x12, 0x43};
    const uint16_t testData16[] = {0xdead, 0xabcd, 0x1234, 0x4321};
    const uint32_t testData32[] = {0xdeadbeef, 0xabcdef01, 0x12345678, 0x43218765};
    const uint64_t testData64[] = {0xdeadbeefabcdef01, 0x1234567843218765, 0xdeadbeef12345678, 0xabcdef0143218765};
    
    volatile uint8_t readData8;
    volatile uint16_t readData16;
    volatile uint32_t readData32;
    volatile uint64_t readData64;


    uint64_t esrVal = 0x0000000000000001; //set mprot[0]=1
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();

    if (shire_id == 0) {
        neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);

        //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
        neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );

        //write mprot ESR with 0x01 value (mprot[0] = 1 is what we want
        *neighESR = esrVal;
    }

    C_TEST_START;
    
    if (shire_id == 32) 
    {
        uint8_t i = 0;
        while(accessType[i] != 0)
        {  
          uint8_t j = 0;  
          while(segmentsAddr[j] != 0)
          {
            switch (accessType[i])
            {
              case 1:
                for (uint8_t k =0; k < MEMORY_ROW_SIZE_BYTES; k+=sizeof(uint8_t))
                  {
                    addr8 = (uint8_t*)(segmentsAddr[j] + k);
                    *addr8 = testData8[k%4];
                    readData8 = *addr8;
                    if (readData8 != testData8[k%4]) C_TEST_FAIL;   
                  }  
                break;

              case 2:
                for (uint8_t k =0; k < MEMORY_ROW_SIZE_BYTES; k+=sizeof(uint16_t))
                  {
                    addr16 = (uint16_t*)(segmentsAddr[j] + k);
                    *addr16 = testData16[k%4];
                    readData16 = *addr16;
                    if (readData16 != testData16[k%4]) C_TEST_FAIL;   
                  }  
                break;

              case 4:
                for (uint8_t k =0; k < MEMORY_ROW_SIZE_BYTES; k+=sizeof(uint32_t))
                  {
                    addr32 = (uint32_t*)(segmentsAddr[j] + k);
                    *addr32 = testData32[k%4];
                    readData32 = *addr32;
                    if (readData32 != testData32[k%4]) C_TEST_FAIL;   
                  }  
                break;  

              case 8:
                for (uint8_t k =0; k < MEMORY_ROW_SIZE_BYTES; k+=sizeof(uint64_t))
                  {
                    addr64 = (uint64_t*)(segmentsAddr[j] + k);
                    *addr64 = testData64[k%4];
                    readData64 = *addr64;
                    if (readData64 != testData64[k%4]) C_TEST_FAIL;   
                  }  
                break;  
            } //end switch
            j++;
          } //end while(accessType)
          i++;
        } //end while(segmentAddr)   

    } //end if shire==32
    C_TEST_PASS;
} 
