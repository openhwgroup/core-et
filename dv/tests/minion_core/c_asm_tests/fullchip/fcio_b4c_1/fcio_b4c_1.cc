/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "cpu.h" 
#include "tb.h" 
#include "soc.h" 
#include "macros.h" 
#include "plic_api.h"
#include "fcc.h"
#include "esr.h"
#include <inttypes.h>
#include "macros.h"
#include "et_test_common.h"
#include "minion.h"

#define MASTER_SHIRE   32

#define MODE_USER       0
#define MODE_SUPERVISOR 1
#define MODE_MACHINE    3


#define CAUSE_BUS_ERROR           ((1ULL<<63) | 23)
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#define DRAM_16GB 1
#define DRAM_32GB 3

#define MPROT_ADDR 0x004

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void  __attribute__ ((interrupt)) machine_handler();



/* MEM ACCESS TEST */
#include "mem_table.h"
/* REG ACCESS TEST */
#include "regs.h"

uint8_t wasAccessFault = 0;
uint8_t operatingMode = MODE_MACHINE;



void inline __attribute__ ((always_inline)) set_dram_size( int size ){
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
    __asm__ __volatile__ ("fence\n");
}


void machine_handler()
{
  
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  
  if (cause == CAUSE_BUS_ERROR){
    //et_printf("\nTRAP HANDLER - BUS ERROR\n");
    wasAccessFault = 1;
        
  } else if (cause == CAUSE_LOAD_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - LOAD ACCESS FAULT\n");
    wasAccessFault =1;
    
  } else if (cause ==CAUSE_STORE_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - STORE ACCESS FAULT\n");
    wasAccessFault = 1;
    
  } else {
    //et_printf("\nTRAP HANDLER - UNEXPECTED CAUSE\n");
  }
  //et_printf("\nTRAP HANDLER - LEAVE\n");
  
  if (operatingMode == MODE_USER) {
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

  else if (operatingMode == MODE_SUPERVISOR) {
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

  else if (operatingMode == MODE_MACHINE) {
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

void inline  __attribute__((always_inline)) enable_exceptions_and_interrupts()
{
	      __asm__ __volatile__ 
      (
      //set trap vector
      "la t0, trap_vector\n"
      "csrw mtvec, t0\n"
      //enable interrupt
      "li t0, 1\n"
      "slli t0, t0, 23\n"
      "csrs mie, t0\n"
      "csrsi mstatus, 8\n"
      : : : "t0"
      );
}

void inline __attribute__((always_inline, optimize(0))) memory_access_64(uint64_t memoryAddress)
{
	volatile uint64_t* ptr64 = (uint64_t*)memoryAddress;
	volatile uint64_t data64;
	data64 = *ptr64;
	delay(100);
}

void inline __attribute__((always_inline, optimize(0))) mailbox_access(intptr_t address)
{
  volatile uint64_t* ptr64 = (uint64_t*)address;
  volatile uint64_t data64;
  data64 = *ptr64;
  delay(100);
}

void inline __attribute__((always_inline, optimize(0))) pu_sram_access(intptr_t address)
{
  volatile uint64_t* ptr64 = (uint64_t*)address;
  volatile uint64_t data64;
  data64 = *ptr64;
  delay(100);
}

uint8_t inline __attribute__((always_inline)) reg_access(IPREGS_t* regs_table)
{
  IPREGS_t     *pRegs = regs_table;
  uint16_t failures;
  uintptr_t regAddr; 

  volatile uint8_t* addr8;
  volatile uint16_t* addr16;
  volatile uint32_t* addr32;
  volatile uint64_t* addr64;
  
  failures = 0;
  while( pRegs->base_address != REGTEST_END_OF_LIST ) {
      regAddr = pRegs->base_address + pRegs->reg_offset;
      switch(pRegs->data_width) {
        case (0x08):
          addr8 = (uint8_t*)(regAddr);
          *addr8 = 0xde;
          __asm__ __volatile__ ("fence\n");
          delay(100);
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x10):
          addr16 = (uint16_t*)(regAddr);
          *addr16 = 0xdead;
          __asm__ __volatile__ ("fence\n");
          delay(100);
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x20):
          addr32 = (uint32_t*)(regAddr);
          *addr32 = 0xdeadbeef;
          __asm__ __volatile__ ("fence\n");
          delay(100);
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;          

        case (0x40):
          addr64 = (uint64_t*)(regAddr);
          *addr64 = 0xbaaddeadbabebeef;
          __asm__ __volatile__ ("fence\n");
          delay(100);
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;                    
      } //end switch 
    pRegs++;
  } //end while
  return failures;
}



int main()
{
    
    uint64_t shire_id = get_shire_id();
    
    C_TEST_START;
    

    if (MASTER_SHIRE == shire_id) {

    	enable_exceptions_and_interrupts();
  		

      // ----------- MEMORY ACCESS ------------ //
  		/*MEM 16GB*/
  		set_dram_size(DRAM_16GB);
  		//Access inside boundaries
  		wasAccessFault = 0; 
  		memory_access_64(DRAM_LOW_MEMORY_REGION_BASE_ADDR + DRAM_LOW_MEMORY_REGION_12GB_SIZE - sizeof(uint64_t));
        if (wasAccessFault) C_TEST_FAIL;
		  //Access outside boundaries
		  wasAccessFault = 0;
      memory_access_64(DRAM_LOW_MEMORY_REGION_BASE_ADDR + DRAM_LOW_MEMORY_REGION_12GB_SIZE + sizeof(uint64_t)); 		
      if (!wasAccessFault) C_TEST_FAIL;

      /*MEM 32GB*/
      set_dram_size(DRAM_32GB);
      //Access inside boundaries
      wasAccessFault = 0;
      memory_access_64(DRAM_LOW_MEMORY_REGION_BASE_ADDR + DRAM_LOW_MEMORY_REGION_28GB_SIZE - sizeof(uint64_t));
      if (wasAccessFault) C_TEST_FAIL;
      //Access ouside boundaries
      wasAccessFault = 0;
      memory_access_64(DRAM_LOW_MEMORY_REGION_BASE_ADDR + DRAM_LOW_MEMORY_REGION_28GB_SIZE + sizeof(uint64_t));
      if (!wasAccessFault) C_TEST_FAIL;


      // ---------- Mailbox Access ------------- //
      wasAccessFault = 0;
      mailbox_access(R_PU_MBOX_MM_MX_BASEADDR);
      if (wasAccessFault) C_TEST_FAIL;
      wasAccessFault = 0;
      mailbox_access(R_PU_MBOX_PC_MM_BASEADDR);
      if (wasAccessFault) C_TEST_FAIL;
      wasAccessFault = 0;
      mailbox_access(R_PU_MBOX_MM_SP_BASEADDR);
      if (wasAccessFault) C_TEST_FAIL;


      // -------- PU SRAM Access --------------- //
      wasAccessFault = 0;
      pu_sram_access(R_PU_SRAM_LO_BASEADDR);
      if (wasAccessFault) C_TEST_FAIL;
      wasAccessFault = 0;
      pu_sram_access(R_PU_SRAM_MID_BASEADDR);
      if (wasAccessFault) C_TEST_FAIL;
      wasAccessFault = 0;
      pu_sram_access(R_PU_SRAM_HI_BASEADDR);
      if (wasAccessFault) C_TEST_FAIL;


      // --------- PU Reg Access Quick -------- //
      
      uint8_t failures;
      failures = reg_access(arr_ip);
      if (failures) C_TEST_FAIL;






      C_TEST_PASS;

    }
    else {C_TEST_PASS}

    //uart_send_data(0x01234567);

    //C_TEST_PASS;
}
