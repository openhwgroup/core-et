/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "hal_device.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "acc_io_mx_ms.h"

#define REGTEST_END_OF_LIST    0xEEEEEEEE

//access to CLIN region should not fail
//

#define TOTAL_NUMBER_OF_TESTS   18
#define SHOULD_PASS 4
#define TOTAL_NUMBER_OF_FAULTS TOTAL_NUMBER_OF_TESTS-SHOULD_PASS

#define CAUSE_BUS_ERROR           ((1ULL<<63) | 23)
#define CAUSE_LOAD_ACCESS_FAULT   0x5
#define CAUSE_STORE_ACCESS_FAULT  0x7

#ifdef MINSHIRE_GATESIM_ENABLE
#define TIMEOUT 10000
#endif

// Struct for IP
typedef struct ip
  {
    uintptr_t base_address;
    uintptr_t reg_offset;
    uint32_t data_width;
    uint32_t check_rst;
    uint64_t reset_value;
    uint32_t check_rw;
    uint64_t rw_mask;
    char     *ip_name;
    uint8_t  should_not_hit;

  } IPREGS_t;

//                          Base_address,       reg_offset, data_width, check_rst,reset_value,      check_rw,   rw_mask,         ip_name            should_not_hit        
//                          -------------       ----------- ----------     ---  ------------------    ---  ------------------  ------------------- --------------
    IPREGS_t arr_ip[] = {
//  Maxshire
            // DBG
              //{     R_MX_DBG_CTLR_BASEADDR,           0x0000,      0x20,        1,    0x????????,         0,  0x00000000,        "???"  },
              //{      R_MX_ERR_DEV_BASEADDR,           0x0000,      0x20,        1,    0x????????,         0,  0x00000000,        "???"  },

            // ROM
              //{          R_MX_ROM_BASEADDR,           0x0000,      0x20,        0,  0x18005073,         0,    0x00000000, "ROM 1st instruction"  },
              //{          R_MX_ROM_BASEADDR,           0x0004,      0x20,        0,  0x300022f3,         0,    0x00000000, "ROM 2nd instruction"  },

            // R_MX_feature-CTL0,1,2,3
              { R_MX_FEATURE_CTL0_BASEADDR,           0x0000,      0x08,        1,        0x00,         1,          0x01,   "FCU 0, Field0", 1 },
              { R_MX_FEATURE_CTL0_BASEADDR,           0x0004,      0x08,        1,        0x00,         1,          0x1f,   "FCU 0, Field4", 1  },
              //{ R_MX_FEATURE_CTL0_BASEADDR,           0x0008,      0x08,        1,        0x00,         1,          0x07,   "FCU 0, Field8"  },
              //{ R_MX_FEATURE_CTL0_BASEADDR,           0x000c,      0x08,        1,        0x00,         1,          0x01,   "FCU 0, FieldC"  },
              //{ R_MX_FEATURE_CTL0_BASEADDR,           0x0010,      0x08,        1,        0x07,         1,          0x0f,   "FCU 0, Field10" },

              //{ R_MX_FEATURE_CTL1_BASEADDR,           0x0000,      0x08,        1,        0x00,         1,          0x01,   "FCU 1, Field0"  },
              { R_MX_FEATURE_CTL1_BASEADDR,           0x0004,      0x08,        1,        0x00,         1,          0x1f,   "FCU 1, Field4", 1  },
              { R_MX_FEATURE_CTL1_BASEADDR,           0x0008,      0x08,        1,        0x00,         1,          0x07,   "FCU 1, Field8", 1  },
              //{ R_MX_FEATURE_CTL1_BASEADDR,           0x000c,      0x08,        1,        0x00,         1,          0x01,   "FCU 1, FieldC"  },
              //{ R_MX_FEATURE_CTL1_BASEADDR,           0x0010,      0x08,        1,        0x07,         1,          0x0f,   "FCU 1, Field10" },

              //{ R_MX_FEATURE_CTL2_BASEADDR,           0x0000,      0x08,        1,        0x00,         1,          0x01,   "FCU 2, Field0"  },
              //{ R_MX_FEATURE_CTL2_BASEADDR,           0x0004,      0x08,        1,        0x00,         1,          0x1f,   "FCU 2, Field4"  },
              { R_MX_FEATURE_CTL2_BASEADDR,           0x0008,      0x08,        1,        0x00,         1,          0x07,   "FCU 2, Field8", 1  },
              { R_MX_FEATURE_CTL2_BASEADDR,           0x000c,      0x08,        1,        0x00,         1,          0x01,   "FCU 2, FieldC", 1  },
              //{ R_MX_FEATURE_CTL2_BASEADDR,           0x0010,      0x08,        1,        0x07,         1,          0x0f,   "FCU 2, Field10" },

              //{ R_MX_FEATURE_CTL3_BASEADDR,           0x0000,      0x08,        1,        0x00,         1,          0x01,   "FCU 3, Field0"  },
              //{ R_MX_FEATURE_CTL3_BASEADDR,           0x0004,      0x08,        1,        0x00,         1,          0x1f,   "FCU 3, Field4"  },
              //{ R_MX_FEATURE_CTL3_BASEADDR,           0x0008,      0x08,        1,        0x00,         1,          0x07,   "FCU 3, Field8"  },
              { R_MX_FEATURE_CTL3_BASEADDR,           0x000c,      0x08,        1,        0x00,         1,          0x01,   "FCU 3, FieldC", 1  },
              { R_MX_FEATURE_CTL3_BASEADDR,           0x0010,      0x08,        1,        0x07,         1,          0x0f,   "FCU 3, Field10", 1 },

            // SRAM RM ?
              //{      R_MX_SRAM_RM_BASEADDR,           0x0000,      0x08,        0,        0x00,         0,          0x00,   "???" },

            // R_MX_bus-ERR
              {     R_MX_BUS_ERR0_BASEADDR,           0x0000,      0x40,        0,  0x0000000000000000, 1,  0x00000000000000ff, "BEU 0, cause", 1   },
              //{     R_MX_BUS_ERR0_BASEADDR,           0x0008,      0x40,        1,  0x0000000000000000, 1,  0x000000000000001f, "BEU 0, value"   },
              //{     R_MX_BUS_ERR0_BASEADDR,           0x0010,      0x40,        1,  0x0000000000000000, 1,  0x000000000000000e, "BEU 0, mask"    },

              //{     R_MX_BUS_ERR1_BASEADDR,           0x0000,      0x40,        0,  0x0000000000000000, 1,  0x00000000000000ff, "BEU 1, cause"   },
              {     R_MX_BUS_ERR1_BASEADDR,           0x0008,      0x40,        1,  0x0000000000000000, 1,  0x000000000000001f, "BEU 1, value", 1   },
              //{     R_MX_BUS_ERR1_BASEADDR,           0x0010,      0x40,        1,  0x0000000000000000, 1,  0x000000000000000e, "BEU 1, mask"    },

              //{     R_MX_BUS_ERR2_BASEADDR,           0x0000,      0x40,        0,  0x0000000000000000, 1,  0x00000000000000ff, "BEU 2, cause"   },
              //{     R_MX_BUS_ERR2_BASEADDR,           0x0008,      0x40,        1,  0x0000000000000000, 1,  0x000000000000001f, "BEU 2, value"   },
              {     R_MX_BUS_ERR2_BASEADDR,           0x0010,      0x40,        1,  0x0000000000000000, 1,  0x000000000000000e, "BEU 2, mask", 1    },

              {     R_MX_BUS_ERR3_BASEADDR,           0x0000,      0x40,        0,  0x0000000000000000, 1,  0x00000000000000ff, "BEU 3, cause", 1   },
              {     R_MX_BUS_ERR3_BASEADDR,           0x0008,      0x40,        1,  0x0000000000000000, 1,  0x000000000000001f, "BEU 3, value", 1   },
              {     R_MX_BUS_ERR3_BASEADDR,           0x0010,      0x40,        1,  0x0000000000000000, 1,  0x000000000000000e, "BEU 3, mask", 1    },

            // GAG (don't need to test)

            // CLINT
//            {        R_MX_CLINT_BASEADDR,           0x0000,      0x20,        1,          0x00000000, 1,          0x00000001, "CLINT msip"       },
              {        R_MX_CLINT_BASEADDR,           0x4000,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp0", 0  },
              {        R_MX_CLINT_BASEADDR,           0x4008,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp1", 0  },
              {        R_MX_CLINT_BASEADDR,           0x4010,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp2", 0  },
              {        R_MX_CLINT_BASEADDR,           0x4018,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp3", 0  },
// FAILS!!!   {        R_MX_CLINT_BASEADDR,           0x4000,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp"   },


              /* END OF LIST DELIMITER */
            { REGTEST_END_OF_LIST,     0x0,      0x20,        0,                 0x0,    0,                 0x0, "END_OF_LIST (INVALID)", 0 }
    };


volatile uint8_t wasAccessFault;
volatile uint8_t failures;

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

int main()
{
  IPREGS_t     *pRegs = arr_ip;

  uintptr_t regAddr; 

  volatile uint8_t* addr8;
  volatile uint16_t* addr16;
  volatile uint32_t* addr32;
  volatile uint64_t* addr64;

  uint64_t shire_id = get_shire_id();
  volatile uint64_t*   baseAddr =  (uint64_t*) R_PU_MBOX_MM_SP_BASEADDR;

  C_TEST_START;

  *baseAddr = 0xbabe;

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
      // cycle through each entry in the table
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
      //wait_for_credit(0);
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

    if (failures == TOTAL_NUMBER_OF_FAULTS) {
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
  } // if shireid == 32 
  else  {
    #ifdef MINSHIRE_GATESIM_ENABLE
      if (shire_id == 32 and thread_id == 0) {
        check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + (hart_id%64)*4);        
      }
    #endif       
    C_TEST_PASS;
  }
}


void machine_handler()
{
  
  uint64_t cause;
  __asm__ __volatile__ ("csrr %[cause], mcause\n" : [cause] "=r" (cause));

  
  if (cause == CAUSE_BUS_ERROR){
    //et_printf("\nTRAP HANDLER - BUS ERROR\n");
    //was bus error interrupt, clean the IRQ
    __asm__ __volatile__
    (
      "li t0, 1\n"
      "slli t0, t0, 23\n"
      "csrc mip, t0\n"
       : : : "t0"
    );
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



