/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// This test tries to access a single register of each perpheral in IO_PU region.
//
// The assumption is as follows:
//     * No Bus Error generated (or timeout) means we have accessibility to a particular location
//
// We test three protection modes from MPROT: user, supervisor and machine.
//
// * Since this is Compute Minion test, we do test only total-ban on accesses to this region (mprot[1:0] = b'10 (MODE_NOBODY)) *
//
// We count number of failures i.e. access which is not allowed from a particular mode, for example:
//
//     * mprot[1:0]=b'11 would generate a fault on access from USER or SUPERVISOR mode, and so on.
//
// This is the table of expected failures:
//  mprot[1:0]=    b'00     b'01    b'10   b'11
//
//  mode
//  USER           pass     fail    fail   fail
//  SUPERVISOR     pass     pass    fail   fail
//  MACHINE        pass     pass    fail   pass
//
//

#include "hal_device.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "acc_io_pu_cs.h"

#define REGTEST_END_OF_LIST    0xEEEEEEEE

#define TOTAL_NUMBER_OF_EXPECTED_FAULTS 36

#define CAUSE_BUS_ERROR           ((1ULL<<63) | 23)
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#define MODE_USER       0
#define MODE_SUPERVISOR 1
#define MODE_MACHINE    3
#define MODE_NOBODY     2

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
                          { R_PU_I2C_BASEADDR,       0x3c,       0x20,        0x0,         0x03, "PU I2C"      },
                          { R_PU_SPI_BASEADDR,       0x14,       0x20,        0x0,         0x10, "PU SPI"      },
                          { R_PU_GPIO_BASEADDR,      0x00,       0x20,        0x0,         0xff, "PU GPIO"     },
                          { R_PU_TIMER_BASEADDR,     0x14,       0x20,        0x0,         0xff, "PU TIMER"    },
                          { R_PU_UART_BASEADDR,      0x1c,       0x20,        0x0,         0xff, "PU UART"  },
                          { R_PU_WDT_BASEADDR,       0x00,       0x20,        0xa,         0x2a, "PU WDT"   },
                          { R_PU_I3C_BASEADDR,       0x00,       0x20,        0x0,   0x80000000, "PU I3C"   },
                          { R_PU_UART1_BASEADDR,     0x1c,       0x20,        0x0,         0xff, "PU UART1" },
                          { R_PU_PLIC_BASEADDR,      0x04,       0x20,        0x0,         0x03, "PU PLIC"  },  // Accesible but readvalue is wrong
                          { R_PU_USB0_BASEADDR,      0x08,       0x20,        0x0,       0x07, "PU USB0"  },
                          { R_PU_USB1_BASEADDR,      0x08,       0x20,        0x0,       0x07, "PU USB1"  },
                          { R_PU_EMMC_CFG_BASEADDR,  0x34,       0x20,        0x0,         0x0f, "PU EMMC"}, // 2d is the intr status bit and is in hclk domain

                          { REGTEST_END_OF_LIST,     0x00,       0x20,        0x0,         0x0, "DUMMY"}
                        };


volatile uint8_t wasAccessFault;
volatile uint8_t wasBusError;
volatile uint8_t caughtFailures;
volatile uint8_t mode =  MODE_MACHINE;

#define MPROT_ADDR 0x004
void set_mprot_privileges( int prv_mode ){
    uint64_t esrVal;
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();
    neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);
    //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
    //neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );
    //
    esrVal = *neighESR;
    esrVal = esrVal & 0xFFFFFFFFFFFFFFFC;
    esrVal = esrVal | prv_mode;
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
    wasBusError = 1;

  } else if (cause == CAUSE_LOAD_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - LOAD ACCESS FAULT\n");
    wasAccessFault =1;

  } else if (cause ==CAUSE_STORE_ACCESS_FAULT) {
    //et_printf("\nTRAP HANDLER - STORE ACCESS FAULT\n");
    wasAccessFault = 1;

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

int test_access_all_modes(void)
{
    uint8_t failures=0;
    mode = MODE_USER;
    m_to_u_mode();
    failures += sweep_registers();
    to_m_mode();
    mode=MODE_SUPERVISOR;
    m_to_s_mode();
    failures += sweep_registers();
    mode=MODE_MACHINE;
    to_m_mode();
    failures += sweep_registers();
    return failures;
}


int main()
{


  uint64_t shire_id = get_shire_id();

  C_TEST_START;

   __asm__ __volatile__
    (
     //set trap vector
     "la t0, trap_vector\n"
     "ori t0, t0, 1\n"
     "csrw mtvec, t0\n"
      //enable interrupt
     "li t0, 1\n"
     "slli t0, t0, 23\n"
     "csrs mie, t0\n"
     "csrsi mstatus, 8\n"
     : : : "t0"
    );

  if (shire_id == 0) {
     wasAccessFault = 0;
     wasBusError = 0;
     caughtFailures =0;

     set_mprot_privileges(MODE_NOBODY);
     caughtFailures += test_access_all_modes();

     if (wasBusError) {C_TEST_FAIL}
     if (caughtFailures == TOTAL_NUMBER_OF_EXPECTED_FAULTS) {C_TEST_PASS}
     else {C_TEST_FAIL}
  } // if shireid == 0
  else  {C_TEST_PASS}
}
