/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "hal_device.h"
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#define REGTEST_END_OF_LIST    0xEEEEEEEE


// Struct for IP
typedef struct ip
  {
    uint32_t base_address;
    uint32_t reg_offset;
    uint32_t data_width;
    uint32_t check_rst;
    uint64_t reset_value;
    uint32_t check_rw;
    uint64_t rw_mask;
    char     *ip_name;

  } IPREGS_t;

//                          Base_address,       reg_offset, data_width, check_rst,reset_value,      check_rw,   rw_mask,         ip_name
//                          -------------       ----------- ----------     ---  ------------------    ---  ------------------  -------------------
    IPREGS_t arr_ip[] = {
            // CLINT
//            {        R_MX_CLINT_BASEADDR,           0x0000,      0x20,        1,          0x00000000, 1,          0x00000001, "CLINT msip"       },
              {        R_MX_CLINT_BASEADDR,           0x4000,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp0"  },
              {        R_MX_CLINT_BASEADDR,           0x4008,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp1"  },
              {        R_MX_CLINT_BASEADDR,           0x4010,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp2"  },
              {        R_MX_CLINT_BASEADDR,           0x4018,      0x40,        0,  0x0000000000000000, 1,  0xffffffffffffffff, "CLINT mtimecmp3"  },

            // PLIC
              {         R_PU_PLIC_BASEADDR,             0x04,      0x20,        1,                 0x0, 1,                0x03, "PU PLIC"  },  // Accesible but readvalue is wrong
            // I3C
              {          R_PU_I3C_BASEADDR,             0x00,      0x20,        1,                 0x0, 1,          0x80000000, "PU I3C"   },
            // SPI
            // UART0
              {         R_PU_UART_BASEADDR,             0x1c,      0x20,        1,                 0x0, 1,                0xff, "PU UART"  },
            // UART1
              {        R_PU_UART1_BASEADDR,             0x1c,      0x20,        1,                 0x0, 1,                0xff, "PU UART1" },
            // GPIO
            // WDT
              {          R_PU_WDT_BASEADDR,             0x00,      0x20,        1,                 0xa, 1,                0x2a, "PU WDT"   },
            // TIMER
            // I2C
            // USB0
            // DMA
//            {     S_PU_DMA_CTRL_BASEADDR,             0x58,      0x20,        1,                 0x0, 1,                0x80, "PU DMA"  },
            // EMMC
//            {     R_PU_EMMC_CFG_BASEADDR,             0x2d,      0x20,        1,                 0x0, 1,                0x00, "PU EMMC"}, // 2d is the intr status bit and is in hclk domain
            //
// Shire
            // ESR

              /* END OF LIST DELIMITER */
            {          REGTEST_END_OF_LIST,              0x0,      0x20,        0,                 0x0, 0,                 0x0, "END_OF_LIST (INVALID)" }
    };




/*#define MPROT_ADDR 0x004
void set_privileges( void ){
    uint64_t esrVal = 0x0000000000000001; //set mprot[0]=1
    volatile uint64_t* neighESR;

    uint64_t shire_id = get_shire_id();
    neighESR = esr_address(PP_MACHINE, shire_id, REGION_NEIGHBOURHOOD, MPROT_ADDR);
    //broadcast to all minions in the neighbourhood - 0x0f << 16 is broadcast
    neighESR = (uint64_t*)( (uint64_t)neighESR | (uint64_t)( 0x0f << 16 ) );
    //write mprot ESR with 0x01 value (mprot[0] = 1 is what we want
    *neighESR = esrVal;
} */
uint64_t reg_read ( IPREGS_t *pRegs ) {
  uint8_t  t8;
  uint16_t t16;
  uint32_t t32;
  uint64_t t64;
  uint64_t regVal;
  uint8_t* pDutReg = (uint8_t *)((uint64_t)(pRegs->base_address + pRegs->reg_offset));
  switch(pRegs->data_width){
    case(0x08): t8  = *((uint8_t  *) pDutReg); regVal=t8;  break;
    case(0x10): t16 = *((uint16_t *) pDutReg); regVal=t16; break;
    case(0x20): t32 = *((uint32_t *) pDutReg); regVal=t32; break;
    case(0x40): t64 = *((uint64_t *) pDutReg); regVal=t64; break;
    default:
      //printDbg("\nIP NAME = %0s\t addr=%0h\t ERROR: INVALID DATA WIDTH!!! (width=%X)",pRegs->ip_name,pRegs->base_address+pRegs->reg_offset,pRegs->data_width);
      regVal = 0xdead0baddead0bad;
  }
  return regVal;
}

uint32_t reg_write ( IPREGS_t *pRegs, uint64_t reg_data ) {
  uint8_t  t8;
  uint16_t t16;
  uint32_t t32;
  uint64_t t64;
  uint32_t err=0;
  uint8_t* pDutReg = (uint8_t *)((uint64_t)(pRegs->base_address + pRegs->reg_offset));
  switch(pRegs->data_width){
    case(0x08): t8  = (uint8_t ) reg_data; *((uint8_t  *) pDutReg)=t8;  break;
    case(0x10): t16 = (uint16_t) reg_data; *((uint16_t *) pDutReg)=t16; break;
    case(0x20): t32 = (uint32_t) reg_data; *((uint32_t *) pDutReg)=t32; break;
    case(0x40): t64 = (uint64_t) reg_data; *((uint64_t *) pDutReg)=t64; break;
    default:
      //printDbg("\nIP NAME = %0s\t addr=%0h\t ERROR: INVALID DATA WIDTH!!! (width=%X)",pRegs->ip_name,pRegs->base_address+pRegs->reg_offset,pRegs->data_width);
      err=3;
  }
  return err;
}



int main()
{

  uint64_t shire_id = get_shire_id();
  
  C_TEST_START;

  if (shire_id == 32) {

  uint64_t     regVal;
  uint64_t     expVal;
  uint32_t     err=0;
  IPREGS_t     *pRegs;

  pRegs = arr_ip; //pRegsTable;

  // cycle through each entry in the table
  while( pRegs->base_address != REGTEST_END_OF_LIST ) {
    err=0;
    regVal = reg_read(pRegs);
    if( pRegs->check_rst && regVal != pRegs->reset_value ) {
      // reset value is bad
      C_TEST_FAIL;
      //printDbg("\nIP NAME = %0s\t addr=%0h\t ERROR: Reset value not correct! resetVal=%0h, expected%0h", pRegs->ip_name, pRegs->base_address+pRegs->reg_offset, regVal, pRegs->reset_value);
    } else {
      if (pRegs->check_rw) {
        //  write the inverse of reset value
        expVal = (~pRegs->reset_value) & pRegs->rw_mask;
        err = reg_write(pRegs,expVal);
        if (err) {
         C_TEST_FAIL;
         //printDbg("\nIP NAME = %0s\t addr=%0h\t ERROR: INVALID DATA WIDTH!!! (width=%0h)",pRegs->ip_name,pRegs->base_address+pRegs->reg_offset,pRegs->data_width);
         //return ET_FAIL;
        } else {
          //  read it back and compare
          regVal = reg_read(pRegs) & pRegs->rw_mask;
          if(regVal != expVal) {
            // read/write failure
            //printDbg("\nIP NAME = %0s\t addr=%0h\t ERROR: write/read failure. regVal=%0h, expected=%0h", pRegs->ip_name, pRegs->base_address+pRegs->reg_offset, regVal, expVal);
            C_TEST_FAIL;
          }
        }
      }
    }
    //if (!err) {
    //  printDbg("\nIP NAME = %0s\t addr=%0h\t IP access successful!", pRegs->ip_name, pRegs->base_address+pRegs->reg_offset);
    //}
    pRegs++;
  } //enf while
  C_TEST_PASS;

  } // if shireid == 32 
  else  {C_TEST_PASS}
}



