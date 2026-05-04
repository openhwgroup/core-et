/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//----------------------------------------------------------------------------------------------
//
//  This test accesses entire DRAM memory map from COMPUTE MINION and checks whether there was a fault at access
//  and counts them. At the end it compares that fault count with expeceted number of failures
//  and decides about the destiny of the test - PASS or FAIL.
//
//  Expected failures per memory access width (size) and mode are as follows (F - failure, P - pass)
//
//  NOTE: There are two configurations we are testing: M16 and M32. M16 has 16GB and M32 has 32GB of RAM total.
//  
//     Region             M - mode       U - mode       S - mode
//     LOW MCODE            3xP            3xF            3xF
//     LOW OS               3xP            3xP            3xP
//     LOW_MEM_M16          3xP            3xP            3xP <=== Region is sized down to 12GB due to total of 16GB of MEM
//     LOW_MEM_M32    M16:1P 2F M32:3P  M16:1P 2F M32:3P  M16:1P 2F M32:3P   <=== Region is sized down to 28GB due to total of 32GB of MEM
//     HIGH MCODE           3xF            3xF            3xF <=== NOT SUPPORTED IN OUR SOC (on Minions)
//     HI OS                3xF            3xF            3xF <=== NOT SUPPORTED IN OUR SOC (on Minions)
//     HI MEM               3xF            3xF            3xF <=== NOT SUPPORTED IN OUR SOC (on Minions)
//
//     Total expected number of faults per access size:     M16: 12
//     (NOT COUNTING HI REGIONS SINCE WE DO NOT TEST THEM)  M32:  6
//     For 1,2,4,8 bytes access we expect in total:    4*(M16+M32) =  72 failures
//
//-----------------------------------------------------------------------------------------------


#include "hal_device.h"
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "cacheops.h"
#include "mem_table.h"
#include "acc_dram_cs.h"

#define MPROT_ADDR 0x004

//#define CAUSE_BUS_ERROR          0x1d
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#define MODE_USER       0
#define MODE_SUPERVISOR 1
#define MODE_MACHINE    3

#define DRAM_16GB 1
#define DRAM_32GB 3

#define FAILURES_16GB_U_MODE_PER_ACCESS_SIZE 5
#define FAILURES_16GB_S_MODE_PER_ACCESS_SIZE 5
#define FAILURES_16GB_M_MODE_PER_ACCESS_SIZE 2

#define FAILURES_32GB_U_MODE_PER_ACCESS_SIZE 3
#define FAILURES_32GB_S_MODE_PER_ACCESS_SIZE 3
#define FAILURES_32GB_M_MODE_PER_ACCESS_SIZE 0


int16_t caughtFailures = 0;
uint8_t wasAccessFault = 0;
uint8_t mode = MODE_MACHINE;
uint8_t accessType[] = {1,2,4,8,0}; 

uint8_t testData8[]   = {0xde, 0xab, 0x12, 0x43};
uint16_t testData16[] = {0xdead, 0xabcd, 0x1234, 0x4321};
uint32_t testData32[] = {0xdeadbeef, 0xabcdef01, 0x12345678, 0x43218765};
uint64_t testData64[] = {0xdeadbeefabcdef01, 0x1234567843218765, 0xdeadbeef12345678, 0xabcdef0143218765};
 

uint64_t segmentsAddr[3];
volatile uint8_t* addr8;
volatile uint16_t* addr16;
volatile uint32_t* addr32;
volatile uint64_t* addr64;

   
volatile uint8_t readData8;
volatile uint16_t readData16;
volatile uint32_t readData32;
volatile uint64_t readData64;


uint8_t test_status =0;

uint16_t mem_sweep() {
                              
     
    #define BOOT_CODE_END_ABSOLUTE 0x8000020000
    #define BOOT_CODE_OFFSET 0x20000 
    #define STACK_TOP_END_ABSOLUTE 0x8004090000
    #define STACK_TOP_OFFSET 0x90000
       
    uint16_t failures = 0;
    uint8_t addrCnt = 0;
    while(memTable[addrCnt].baseAddr != ITER_END_ADDR)
      {
       if (memTable[addrCnt].baseAddr == DRAM_LOW_MCODE_REGION_BASE_ADDR) {
          segmentsAddr[0] = BOOT_CODE_END_ABSOLUTE;
          segmentsAddr[1] = memTable[addrCnt].baseAddr + (memTable[addrCnt].size + (BOOT_CODE_END_ABSOLUTE - memTable[addrCnt].baseAddr))/2;
          
       }
       else if (memTable[addrCnt].baseAddr == DRAM_LOW_OS_REGION_BASE_ADDR) {
          segmentsAddr[0] = STACK_TOP_END_ABSOLUTE;
          segmentsAddr[1] = memTable[addrCnt].baseAddr + (memTable[addrCnt].size + (STACK_TOP_END_ABSOLUTE - memTable[addrCnt].baseAddr))/2;
          
       }
       else {
          segmentsAddr[0] = memTable[addrCnt].baseAddr;
          segmentsAddr[1] = memTable[addrCnt].baseAddr + memTable[addrCnt].size/2;
       }
       uint8_t i = 0;
       while(accessType[i] != 0)
       {  
         segmentsAddr[2] = memTable[addrCnt].baseAddr + memTable[addrCnt].size - accessType[i];
                  
         for (uint8_t j=0; j<3; j++)
         {
           switch (accessType[i])
           {
             case 1:
               addr8 = (uint8_t*)(segmentsAddr[j]);
               *addr8 = testData8[0];
               __asm__ __volatile__ ("fence\n");

               if (!wasAccessFault) {
                   evict_va(0, to_Mem, segmentsAddr[j], 0, 0, 0, 0);
                   __asm__ __volatile__ ("fence\n");
                   readData8 = *addr8;
                   if (readData8 != testData8[0]) failures++;   
               } else  {
                   failures++;
                   wasAccessFault = 0;
               }
               break;

             case 2:
               addr16 = (uint16_t*)(segmentsAddr[j]);
               *addr16 = testData16[0];
               __asm__ __volatile__ ("fence\n");

               if (!wasAccessFault) {
                  evict_va(0, to_Mem, segmentsAddr[j], 0, 0, 0, 0);
                   __asm__ __volatile__ ("fence\n");
                  readData16 = *addr16;
                  if (readData16 != testData16[0]) failures++;   
               } else {
                   failures++;
                   wasAccessFault = 0;
               }
               break;

             case 4:
               addr32 = (uint32_t*)(segmentsAddr[j]);
               *addr32 = testData32[0];
               __asm__ __volatile__ ("fence\n");
               if (!wasAccessFault) {
                  evict_va(0, to_Mem, segmentsAddr[j], 0, 0, 0, 0);
                  __asm__ __volatile__ ("fence\n");
                  readData32 = *addr32;
                  if (readData32 != testData32[0]) failures++;    
               } else {
                  failures++;
                  wasAccessFault = 0;
               }
               
               break;  

             case 8:
               addr64 = (uint64_t*)(segmentsAddr[j]);
               *addr64 = testData64[0];
               __asm__ __volatile__ ("fence\n");
               if (!wasAccessFault) {
                  evict_va(0, to_Mem, segmentsAddr[j], 0, 0, 0, 0);
                  __asm__ __volatile__ ("fence\n");
                  readData64 = *addr64;
                  if (readData64 != testData64[0]) failures++;    
               } else {
                  failures++;
                  wasAccessFault = 0;
               }
               
               break;  
           } //end switch accessType
           
         } //end for
         i++;
       } //end while(accessType)   
       addrCnt++;  
      } //end while(memTable[addrCnt])
    return failures;
}


void set_dram_size( int size ){
    uint64_t esrVal;
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();
    neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);
    //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
    //neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );
    esrVal = *neighESR;
    esrVal = esrVal & 0xFFFFFFFFFFFFFFCF;
    esrVal = esrVal | (size << 4);
    *neighESR = esrVal;
}

void m_to_u_mode() {
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6000\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mod
     "1:\n"                     // label to refer to user-mode
     : 
     : 
     : "t0"
     );
}

void m_to_s_mode() 
{
  __asm__ __volatile__ 
    (
     "li t0, 0xffff\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus 16 lower bits
     "li t0, 0x6800\n"
     "csrrs x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to supervisor-mode entry point
     "mret\n"                   // go to supervisor mod
     "1:\n"                     // label to refer to supervisor-mode
     : 
     : 
     : "t0"
     );
}

void to_m_mode() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

void machine_handler()
{
  
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  
  /*if (cause == CAUSE_BUS_ERROR){
    //et_printf("\nTRAP HANDLER - BUS ERROR\n");
    wasAccessFault = 1;
  } */
  
  if (cause == CAUSE_LOAD_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - LOAD ACCESS FAULT\n");
    wasAccessFault =1;
    
  } else if (cause ==CAUSE_STORE_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - STORE ACCESS FAULT\n");
    wasAccessFault = 1;
    
  } else {
    //et_printf("\nTRAP HANDLER - UNEXPECTED CAUSE\n");
  }
  //et_printf("\nTRAP HANDLER - LEAVE\n");
  
  if (mode == MODE_USER) {
      __asm__ __volatile__
      (
       "li t0, 0x0000\n" //return to -U- mode
       "csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" );
  }

  else if (mode == MODE_SUPERVISOR) {
      __asm__ __volatile__
      (
       "li t0, 0x0800\n" //return to -S- mode
       "csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" ); 
  }

  else if (mode == MODE_MACHINE) {
      __asm__ __volatile__
      (
       "li t0, 0x1800\n" //return to -M- mode
       "csrs mstatus, t0\n"
       "csrr t0, mepc\n"
       "addi t0, t0, 4\n"
       "csrw mepc, t0\n"
       :
       :
       : "t0" ); 
  }

}

void bad_trap(){
  et_printf("\nBad Trap\n");
  test_status ++;
}

int main()
{
    uint64_t shire_id = get_shire_id();

    C_TEST_START;
        // Set machine trap handler
    __asm__ __volatile__ 
    (
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     : : : "t0"
    );

    if (shire_id == 0) {
          
      set_dram_size(DRAM_16GB);
      mode=MODE_USER;
      m_to_u_mode();
      caughtFailures = mem_sweep();
      mode=MODE_MACHINE;
      to_m_mode();
      if (caughtFailures != FAILURES_16GB_U_MODE_PER_ACCESS_SIZE * (sizeof(accessType)-1)) {
          //et_printf("Unmatched failures on 16GB U-mode access sweep");
          test_status++;
          goto test_end;
      }
      
      caughtFailures = mem_sweep();
      if (caughtFailures != FAILURES_16GB_M_MODE_PER_ACCESS_SIZE * (sizeof(accessType)-1)) { 
          //et_printf("Unmatched failures on 16GB M-mode access sweep");
          test_status++;
          goto test_end;
      }
     

      mode=MODE_SUPERVISOR;
      m_to_s_mode();
      caughtFailures = mem_sweep();
      mode=MODE_MACHINE;
      to_m_mode();
      if (caughtFailures != FAILURES_16GB_S_MODE_PER_ACCESS_SIZE * (sizeof(accessType)-1)) { 
          //et_printf("Unmatched failures on 16GB S-mode access sweep");
          test_status++;
          goto test_end;
      }


      set_dram_size(DRAM_32GB);
      mode=MODE_USER;
      m_to_u_mode();
      caughtFailures = mem_sweep();
      mode=MODE_MACHINE;
      to_m_mode();
      if (caughtFailures != FAILURES_32GB_U_MODE_PER_ACCESS_SIZE * (sizeof(accessType)-1)) {
          //et_printf("Unmatched failures on 32GB U-mode access sweep");
          test_status++;
          goto test_end;
      }
      
      caughtFailures = mem_sweep();
      if (caughtFailures != FAILURES_32GB_M_MODE_PER_ACCESS_SIZE * (sizeof(accessType)-1)) { 
          //et_printf("Unmatched failures on 32GB M-mode access sweep");
          test_status++;
          goto test_end;
      }
      
      mode=MODE_SUPERVISOR;
      m_to_s_mode();
      caughtFailures = mem_sweep();
      mode=MODE_MACHINE;
      to_m_mode();
      if (caughtFailures != FAILURES_32GB_S_MODE_PER_ACCESS_SIZE * (sizeof(accessType)-1)) { 
          //et_printf("Unmatched failures on 32GB S-mode access sweep");
          test_status++;
          goto test_end;
      }

test_end:
      if (test_status) {
         C_TEST_FAIL;
      
      }     
      
      else C_TEST_PASS;
  
    } else {
      goto test_end;
    }
} 



