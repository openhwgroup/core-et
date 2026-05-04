/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#define WORD_OFFSET 4
#define DWORD_OFFSET 8
#define VSNIP_RSV           : : [m_op] "r" (m_op) : "x3", "x4", "x5", "x6", "x7", "x11", "x12", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
  

int main()
{
    volatile uint64_t shire_id = get_shire_id();
    volatile uint64_t index;

    volatile uintptr_t curr_addr[4];

    volatile uint64_t* addr_64;

    volatile uint64_t write_data_64;//, read_data_64;    

    volatile uintptr_t baseAddresses[] = {R_PU_SRAM_LO_BASEADDR, 
                                          R_PU_SRAM_MID_BASEADDR,
                                          R_PU_SRAM_HI_BASEADDR,
                                          0 };    

    volatile uintptr_t mbox_sizes[] = { R_PU_SRAM_LO_SIZE,  /* -0x1000 because of the preload */
                                        R_PU_SRAM_MID_SIZE,
                                        R_PU_SRAM_HI_SIZE}; /* -0x1000 because of the stack */

    volatile uint64_t testData_64 = 0xFAADEEDBEEFA0000;

    volatile uint8_t i=0;
    volatile int k=0;

    volatile uint64_t ret=0;
    

    C_TEST_START;

    //256 bit access
    __asm__ __volatile__ (
        "li x5, 0xffffffffffffffff\n"
        "mova.m.x x5\n" 
        "fence\n"
        :
        : 
        : "x5"
    );    
        
    while (baseAddresses[i] != 0) { 
        if (shire_id == 32) {
            curr_addr[0] = (baseAddresses[i] + 128);                      //start of range
            curr_addr[1] = (baseAddresses[i] + mbox_sizes[i]/2 + 128);    //mid of range
            curr_addr[2] = (baseAddresses[i] + mbox_sizes[i] - 192);      //end of range    
            curr_addr[3] = (baseAddresses[i] + 256);        
        } else { //shire_id == 0
            curr_addr[0] = (baseAddresses[i]);                            //start of range
            curr_addr[1] = (baseAddresses[i] + mbox_sizes[i]/2);          //mid of range
            curr_addr[2] = (baseAddresses[i] + mbox_sizes[i] - 64);       //end of range
            curr_addr[3] = (baseAddresses[i] + 512);
        } 

        //64 bit access
        for (int j=0; j<8; j++) {
            addr_64 = (uint64_t*)(curr_addr[3] + j*8);
            index = shire_id + j;  
            write_data_64  = (uint64_t)(i*0x100 + testData_64 + index);
            *addr_64 = write_data_64;
        }

        for (k=0; k<3; k++) {  
  
            for (int j=0; j<2; j++) {
                addr_64 = (uint64_t*)(curr_addr[k] +  j*32);
  
                //*addr_64 = write_data_64;
                __asm__ __volatile__ ( 
                    "fence\n"
                    "add x5, zero, %[pattern_address]\n"
                    "flw.ps f0,  0 (x5)\n"
                    "fence\n" 
                    "add x5, zero, %[ptr]\n"
                    "fsw.ps f0,  0 (x5)\n"
                    "fence\n"  
                    "flw.ps f1,  0 (x5)\n"
                    "fence\n"
                    : 
                    : [ptr] "r" (addr_64), [pattern_address] "r" (curr_addr[3])
                    : "x5", "f0", "f1"
                );

                __asm__ __volatile__ ( 
                    "feqm.ps m1, f0, f1\n"
                    "maskpopcz x5, m1\n"
                    "add %[ret] ,zero, x5\n"
                    : [ret] "=r" (ret)
                    :
                    : "x5", "f0", "f1"
                );

                if ( ret != 0){
                    C_TEST_FAIL;    
                } 
            }  
        }

        i++; 
    }//while 

    C_TEST_PASS;

} /*pu_sram_rw_Test() */

























// #include "hal_device.h"
// #include "macros.h"
// #include "et_test_common.h"

// #define ldOffset  0x1000

// typedef struct new_memtest_params 
// {
//     uintptr_t startAddress;
//     uint32_t size;
//     uint32_t busDataWidth;
//     uint32_t numberOfAccesses;
//     uint32_t pattern;
    
// } NEW_MEMTEST_PARAMS_t;


// void memtest_copyStruct( NEW_MEMTEST_PARAMS_t *src, NEW_MEMTEST_PARAMS_t *dst )
// {
//     dst->startAddress     = src->startAddress;
//     dst->size             = src->size;
//     dst->busDataWidth     = src->busDataWidth;
//     dst->numberOfAccesses = src->numberOfAccesses;
//     dst->pattern          = src->pattern;
// } /* memtest_copyStruct() */

// uint32_t memtest_dataRoll( NEW_MEMTEST_PARAMS_t *pMtParam)
// {

//     uint64_t *addr64;
//     uint64_t patt;
//     uint32_t size;
//     uint32_t i;
//     uint64_t rd64;
   
//     patt  = (uint64_t)( pMtParam -> pattern );
//     size  = (pMtParam -> busDataWidth) / 64; 
    
//     for( i = 0; i < size; i++ ){
//         addr64  = (uint64_t *)(pMtParam->startAddress) + 8 * i; 
//         *addr64 = patt;
//         rd64    = (uint64_t) * addr64;
//         if( rd64 != patt ){
//           return 1;
//         }
//      }    

//      return 0;

// } /* memtest_dataTest() */

// uint32_t memtest_variableWidthAccess( NEW_MEMTEST_PARAMS_t *pMtParam)
// {

//     uint8_t  *addr8;
//     uint16_t *addr16;
//     uint32_t *addr32;
//     uint64_t *addr64;
//     uint32_t i;
//     uint8_t  rd8;
//     uint16_t rd16;
//     uint32_t rd32;
//     uint64_t rd64;
//     uint8_t  val8;
//     uint16_t val16;
//     uint32_t val32;
//     uint64_t val64;

//     /* Start address calculation for variable Size test */ 
//     /* Addresses in use are concatanated */
//     /* numberOfAccesses is max width count */    
    
//     // addr8  = (uint8_t  *)pMtParam->startAddress;
//     // addr16 = (uint16_t *)((uint32_t)addr8  + pMtParam->numberOfAccesses * 8 ); 
//     // addr32 = (uint32_t *)((uint32_t)addr16 + pMtParam->numberOfAccesses * 4 );
//     // addr64 = (uint64_t *)(((uint32_t)addr32 + pMtParam->numberOfAccesses * 2 + 0x8) & ~0x03 );    

//     addr8  = (uint8_t  *)(pMtParam->startAddress);
//     addr16 = (uint16_t *)(pMtParam->startAddress + pMtParam->numberOfAccesses * 8 ); 
//     addr32 = (uint32_t *)(pMtParam->startAddress + pMtParam->numberOfAccesses * 16);
//     addr64 = (uint64_t *)(pMtParam->startAddress + pMtParam->numberOfAccesses * 24);  

//     /* Check 8 Bit Access */ 
//     /* Write Data */
//     val8 = (uint8_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses * 8; i++ ){
//         val8 += 1;
//         addr8[i] = val8;
//     }

//     /* Read & Verify Data */
//     val8 = (uint8_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses * 8; i++ ){
//         val8 += 1;
//         rd8 = addr8[i];
//         if( rd8 != val8 ){
//           return 1;
//         }
//     }    

//     /* Check 16 Bit Access */
//     /* Write Data */
//     val16 = (uint16_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses * 4; i++ ){
//         val16 += 1;
//         addr16[i] = val16;
//     }

//     /* Read & Verify Data */
//     val16 = (uint16_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses * 4; i++ ){
//         val16 += 1;
//         rd16 = addr16[i];
//         if( rd16 != val16 ){
//           return 1;
//         }
//     }
    
//     /* Check 32 Bit Access */
//     /* Write Data */
//     val32 = (uint32_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses * 2; i++ ){
//         val32 += 1;       
//         addr32[i] = val32;
//     }

//     /* Read & Verify Data */
//     val32 = (uint32_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses * 2; i++ ){
//         val32 += 1;
//         rd32 = addr32[i];
//         if( rd32 != val32 ){
//           return 1;
//         }
//     }

//     /* Check 64 Bit Access */
//     /* Write Data */
//     val64 = (uint64_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses; i++ ){
//         val64 += 1;
//         addr64[i] = val64;
//     }

//     /* Read & Verify Data */
//     val64 = (uint64_t)pMtParam->pattern;
//     for( i = 0; i < pMtParam->numberOfAccesses; i++ ){
//         val64 += 1;
//         rd64 = addr64[i];
//         if( rd64 != val64 ){
//           return 1;
//         }
//     }

//     return 0;

// } /* memtest_variableWidthAccess() */

// uint32_t memtest_Endian( NEW_MEMTEST_PARAMS_t *pMtParam)
// {
//     uintptr_t addrE32;
//     uint32_t pattInc;
//     uint32_t rd32;
//     uint64_t rd64;
//     uint32_t val32;
//     uint64_t val64;
//     uint8_t  tmp8;
//     uint16_t tmp16;
//     uint32_t tmpE32;
//     uint64_t tmpE64;
//     uint8_t  *ptAddr8;
//     uint32_t *ptAddr32;
//     uint64_t *ptAddr64;
//     uint32_t i;
//     int j;

//     pattInc = 1;

//     rd32  = pMtParam->pattern + pattInc;
//     rd64  =(uint64_t)(pMtParam->pattern + pattInc ) * 0x11111111;
        
//     /* read endian 32 */
//     for( i = 0; i < pMtParam->numberOfAccesses; i += 4 ){
//       val32 = 0;
//       ptAddr32  = (uint32_t *)((pMtParam->startAddress) + i );
//       *ptAddr32 = (uint32_t)( pMtParam->pattern + pattInc );
//       addrE32   = ((pMtParam->startAddress) + i ); 
      
//       /*read endian 4x8*/
//       for( j = 3; j >= 0; j-- ){
//           ptAddr8 = (uint8_t *)( addrE32 + j );
//           tmp8    =  (uint8_t)( *ptAddr8 );
//           tmpE32  = (uint32_t) tmp8;
//           tmpE32 <<= ( 8*j );
//           val32 |= tmpE32;
//       }
        
//       if( rd32 != val32 ){
//         return 1;
//       }
        
//       /*read endian 2x16*/
//       val32 = 0;       
//       for(j = 3; j >= 0 ; j -= 2 ){
//           ptAddr8 = (uint8_t *)( addrE32 + j );
//           tmp16   = (uint16_t)( *ptAddr8 );
//           tmp16 <<= 8;
//           tmp16 |= *( ptAddr8 - 1 );
//           tmpE32 = (uint32_t)tmp16;
//           tmpE32 <<= ( 8 * ( j - 1 ));
//           val32  |= tmpE32;
//       }
//       if( rd32 != val32 ){
//         return 1;
//       }
//     } 
    
//     /* read endian 64 */  
//     for( i = 0; i < pMtParam -> numberOfAccesses; i += 8 ){
//       val64 = 0;
//       ptAddr64  = (uint64_t *)((pMtParam->startAddress) + i );
//       *ptAddr64 = (uint64_t)(pMtParam->pattern + pattInc ) * 0x11111111;
//       addrE32   = ((pMtParam->startAddress) + i ); 
        
//       /*read endian 8x8 */
//       for( j = 7; j >= 0; j-- ){    
//           ptAddr8 =(uint8_t *)( addrE32 + j );
//           tmp8    = (uint8_t)( *ptAddr8 );
//           tmpE64  = (uint64_t)tmp8;
//           tmpE64 <<= ( 8 * j );
//           val64  |= tmpE64;   
//       }
//       if( rd64 != val64 ){
//         return 1;
//       } 
         
//       /*read endian 4x16*/
//       val64 = 0;
//       for( j = 7; j >= 0 ; j -= 2 ){
//           ptAddr8 = (uint8_t *)( addrE32 + j );
//           tmp16   = (uint16_t)( *ptAddr8 );
//           tmp16 <<= 8;
//           tmp16  |= *( ptAddr8 - 1 );
//           tmpE64  = (uint64_t)tmp16;
//           tmpE64 <<= ( 8 * ( j -  1 ) );
//           val64  |=tmpE64;
//       }
//       if( rd64 != val64 ){
//         return 1;
//       }
//     }

//     return 0;
     
// }/*  memtest_Endian*/


// int main()
// {
//     /* ET Available memories */
//     NEW_MEMTEST_PARAMS_t memTable[] =
//     /*   StartAddr                               Size                      BusDataW    NumAccess      Patt         MemName  */
//     {
//       {   R_PU_SRAM_LO_BASEADDR,             ldOffset,                       512,         8,       0x1234FEDC},
//       {   R_PU_SRAM_LO_BASEADDR + ldOffset,  R_PU_SRAM_LO_SIZE - ldOffset,   512,         8,       0x1234FEDC}, /* -0x1000 because of the preload */
//       {   R_PU_SRAM_MID_BASEADDR,            R_PU_SRAM_MID_SIZE,             512,         8,       0x1234FEDC},
//       {   R_PU_SRAM_HI_BASEADDR,             R_PU_SRAM_HI_SIZE - ldOffset,   512,         8,       0x1234FEDC}, /* -0x1000 because of the stack */
//       {                   0,                          0,                       0,         0,                0}
//     };

//     NEW_MEMTEST_PARAMS_t *mem;
//     NEW_MEMTEST_PARAMS_t memT;
//     uint32_t memError;

//     mem = memTable;

//     C_TEST_START;

//     while( mem->size ) {

//         memtest_copyStruct( mem, &memT );

//     //START ADDRESS
//         memT.startAddress = mem -> startAddress;    
           
//         //printDbg( "\tDataRoll Test test, Address is 0x%h\n", memT.startAddress);
//         memError = memtest_dataRoll( &memT);/* DataRoll Test */
//         if(memError > 0) C_TEST_FAIL;

//         //printDbg( "\tStart Width Access test, Address is 0x%h, Count: %d\n", memT.startAddress, memT.numberOfAccesses);
//         memError = memtest_variableWidthAccess( &memT);/* Variable Width Access Test */
//         if(memError > 0) C_TEST_FAIL;

//         //printDbg( "\tStart Endian test, Address is 0x%h\n", memT.startAddress);
//         memError = memtest_Endian( &memT);/* Endian Test */
//         if(memError > 0) C_TEST_FAIL;

//     //MIDDLE ADDRESS
//         memT.startAddress = (mem->size/2) & ~0x07;

//         //printDbg( "\tDataRoll Test test, Address is 0x%h\n", memT.startAddress);
//         memError = memtest_dataRoll( &memT);/* DataRoll Test */
//         if(memError > 0) C_TEST_FAIL;

//         //printDbg( "\tStart Width Access test, Address is 0x%h, Count: %d\n", memT.startAddress, memT.numberOfAccesses);
//         memError = memtest_variableWidthAccess( &memT);/* Variable Width Access Test */
//         if(memError > 0) C_TEST_FAIL;

//         //printDbg( "\tStart Endian test, Address is 0x%h\n", memT.startAddress);
//         memError = memtest_Endian( &memT);/* Endian Test */
//         if(memError > 0) C_TEST_FAIL;

//     //END ADDRESS
//         memT.startAddress = ( mem -> startAddress + memT.size ) - ( memT.busDataWidth >> 3 );
//         //printDbg( "\tDataRoll Test test, Address is 0x%h\n", memT.startAddress);
//         memError = memtest_dataRoll( &memT);/* DataRoll Test */
//         if(memError > 0) C_TEST_FAIL;

//         memT.startAddress = ( mem -> startAddress + memT.size ) - 0x100 - ( memT.numberOfAccesses << 4 );
//         //printDbg( "\tStart Width Access test, Address is 0x%h, Count: %d\n", memT.startAddress, memT.numberOfAccesses);
//         memError = memtest_variableWidthAccess( &memT );/* Variable Width Access Test */
//         if(memError > 0) C_TEST_FAIL;

//         memT.startAddress = ( mem -> startAddress + memT.size ) - 0x100;
//         //printDbg( "\tStart Endian test, Address is 0x%h\n", memT.startAddress);
//         memError = memtest_Endian( &memT);/* Endian Test */
//         if(memError > 0) C_TEST_FAIL;

//         mem ++;
//     }

//     C_TEST_PASS;

// } /*pu_sram_rw_Test() */

// /*****     < EOF >     *****/