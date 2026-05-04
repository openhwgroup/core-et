/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "hal_device.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "acc_sp_ms.h"

#define REGTEST_END_OF_LIST    0xEEEEEEEE

//24 base addresses

#define TOTAL_NUMBER_OF_FAULTS 24

#define CAUSE_BUS_ERROR          0x1d
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#ifdef MINSHIRE_GATESIM_ENABLE
#define TIMEOUT 15000
#endif

// Struct for IP
typedef struct ip
  {
    uint32_t base_address;
    uint32_t reg_offset;
    uint32_t data_width;
    uint32_t reset_value;
    uint32_t rw_mask;
    char     *ip_name;

  } IPREGS_t;


//                          Base_address,            reg_offset, data_width, reset_value, rw_mask ip_name
//                          -------------            ----------- ----------- ------------ ------- -------
    IPREGS_t arr_ip[] = {
                         { R_SP_DMA_BASEADDR,       0x58,       0x20,        0x0,         0x08, "SPIO DMA"     },
                         { R_SP_I2C1_BASEADDR,      0x3c,       0x20,        0x0,         0x03, "SPIO I2C1"   },/* I2C_IC_DATA_CMD */
                         { R_SP_SPI1_BASEADDR,      0x14,       0x20,        0x0,         0x10, "SPIO SPI1"   },/* SSIENR SSI ENABLE REGISTER*/
                         { R_SP_UART1_BASEADDR,     0x1c,       0x20,        0x0,         0xff, "SPIO UART1"  },/* SCR - SCRATCHPAD REGISTER   PU_UART0_BASEADDR*/
                         { R_SP_PLL0_BASEADDR,      0x00,       0x10,        0x7f8,       0x0d, "SPIO PLL0"   },/* FREQUENCY MULTIPLIER 0x02*/
                         { R_SP_PLL1_BASEADDR,      0x00,       0x10,        0x7f8,       0x0d, "SPIO PLL1"   },/* FREQUENCY MULTIPLIER*/
                         { R_SP_PLL2_BASEADDR,      0x00,       0x10,        0x7f8,       0x0d, "SPIO PLL2"   },/* FREQUENCY MULTIPLIER*/
                         { R_SP_PLL3_BASEADDR,      0x00,       0x10,        0x7f8,       0x0d, "SPIO PLL3"   },/* FREQUENCY MULTIPLIER*/
                         { R_SP_PLL4_BASEADDR,      0x00,       0x10,        0x7f8,       0x0d, "SPIO PLL4"   },/* FREQUENCY MULTIPLIER*/
                         { R_SP_PLIC_BASEADDR,      0x04,       0x20,        0x0,         0x03, "SPIO PLIC"   },
                         { R_SP_VAULT_BASEADDR,     0x3f10,     0x20,        0x0,         0x01, "SPIO VAULT"  },// Root of Trust
                         { R_SP_I2C0_BASEADDR,      0x3c,       0x20,        0x0,         0x03, "SPIO I2C0"   },/* I2C_IC_DATA_CMD*/
                         { R_SP_SPI0_BASEADDR,      0x14,       0x20,        0x0,         0x10, "SPIO SPI0"   },/* SSIENR SSI ENABLE REGISTER*/
                         { R_SP_GPIO_BASEADDR,      0x00,       0x20,        0x0,         0xff, "SPIO GPIO"   },/* GPIO_SWPORTA_DR  PU_GPIO_BASEADDR*/
                         { R_SP_TIMER_BASEADDR,     0x14,       0x20,        0x0,         0xff, "SPIO TIMER"  },  /* TIMER1 COUNT REGISTER */ //Offset 0x08, value 0x02
                         { R_SP_PVT0_BASEADDR,      0x20,       0x20,        0x0,         0xff, "SPIO PVT0"   }, // Accesible but read value is wrong
                         { R_SP_PVT1_BASEADDR,      0x20,       0x20,        0x0,         0xff, "SPIO PVT1"   },
                         { R_SP_PVT2_BASEADDR,      0x20,       0x20,        0x0,         0xff, "SPIO PVT2"   },
                         { R_SP_PVT3_BASEADDR,      0x20,       0x20,        0x0,         0xff, "SPIO PVT3"   },
                         { R_SP_PVT4_BASEADDR,      0x20,       0x20,        0x0,         0xff, "SPIO PVT4"   },
                         { R_SP_UART0_BASEADDR,     0x1c,       0x20,        0x0,         0xff, "SPIO UART0"  },/* SCR - SCRATCHPAD REGISTER   PU_UART0_BASEADDR*/
                         { R_SP_WDT_BASEADDR,       0x00,       0x20,        0xa,         0x2a, "SPIO WDT"    },/* NO NAME FIELD (5) IN WDT CONTROL REGISTER*/
//                         { S_SPIO_CRU_BASEADDR,       0x00,       0x20,        0x13,         0xff, "SPIO CRU"    },/* FIRST REGISTER IN CRU*/
                         { R_SP_MISC_BASEADDR,      0x10,       0x20,        0x0,         0x06, "SPIO MISC"},
                         { R_SP_SPIO_REGBUS_BASEADDR,  0x30b0,  0x20,        0x0,         0x0f, "SPIO REGBUS"},
                         { REGTEST_END_OF_LIST,     0x00,       0x20,        0x0,         0x0, "DUMMY"}
                        };


volatile uint8_t wasAccessFault;
volatile uint8_t caughtFailures;

void send_sync_to_sp(void);
void send_sync_to_sp()
{
    *((uint64_t*)R_PU_MBOX_MM_SP_BASEADDR) = 0xbabe;
    return;
}


#define MPROT_ADDR 0x004
void set_privileges( void ){
    uint64_t esrVal = 0x0000000000000001; //set mprot[0]=1
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();
    neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);
    //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
    neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );
    //write mprot ESR with 0x01 value (mprot[0] = 1 is what we want
    *neighESR = esrVal;
}

uint16_t sweep_registers()
{
  IPREGS_t     *pRegs = arr_ip;
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
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x10):
          addr16 = (uint16_t*)(regAddr);
          *addr16 = 0xdead;
          __asm__ __volatile__ ("fence\n");
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x20):
          addr32 = (uint32_t*)(regAddr);
          *addr32 = 0xdeadbeef;
          __asm__ __volatile__ ("fence\n");
          if (wasAccessFault) {
            wasAccessFault = 0;
            failures++;
          }
          break;

        case (0x40):
          addr64 = (uint64_t*)(regAddr);
          *addr64 = 0xbaaddeadbabebeef;
          __asm__ __volatile__ ("fence\n");
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

  }
  //et_printf("\nTRAP HANDLER - LEAVE\n");

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

void bad_trap(){
  et_printf("\nBad Trap\n");
  #ifdef MINSHIRE_GATESIM_ENABLE
    volatile uint64_t shire_id   = get_shire_id();
    volatile uint64_t hart_id    = get_hart_id();
    volatile uint64_t thread_id  = (hart_id & 1);
    if (shire_id == 0 and thread_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 4*hart_id);
  #endif
  C_TEST_FAIL;
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


int main()
{
  uint64_t shire_id = get_shire_id();


  C_TEST_START;

   __asm__ __volatile__
    (
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
     : : : "t0"
    );

  #ifdef MINSHIRE_GATESIM_ENABLE
    volatile uint64_t hart_id = get_hart_id();
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);
    volatile uint64_t timeout = TIMEOUT;

    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {
      s32_scp_init();
      give_credit(0, -1, -1, 1);
      give_credit(32, -1, -1, 1);
    }

    wait_for_credit(1);

    if (shire_id == 0) {
  #else
    if (shire_id == 32) {
  #endif
      wasAccessFault = 0;
      caughtFailures =0;

      send_sync_to_sp();

      #ifndef MINSHIRE_GATESIM_ENABLE
        wait_for_credit(0);
      #endif
      caughtFailures += sweep_registers();

      if (caughtFailures == TOTAL_NUMBER_OF_FAULTS) {
        #ifdef MINSHIRE_GATESIM_ENABLE
          if (shire_id == 0 and thread_id == 0) atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + hart_id*4);
        #endif
        C_TEST_PASS;
      }
      else {
        #ifdef MINSHIRE_GATESIM_ENABLE
          if (shire_id == 0 and thread_id == 0) atomic_read_inc_bad(BASE_S0_GATESIM_VALID_ADDR + 4*hart_id);
        #endif
        C_TEST_FAIL;
      }
  } // if shireid == 0
  else  {
    #ifdef MINSHIRE_GATESIM_ENABLE
      if (shire_id == 32 and thread_id == 0) {
        check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + (hart_id%64)*4);
      }
    #endif
    C_TEST_PASS;
  }
}
