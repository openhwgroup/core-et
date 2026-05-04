/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "minion.h"

#include "et_test_common.h"

#ifdef MINSHIRE_GATESIM_ENABLE
#define TIMEOUT 300
#endif

#define CSR_ADDR_MEPC    0x341
#define CSR_ADDR_SEPC    0x141
#define CSR_ADDR_STVEC   0x105
#define CSR_ADDR_STVAL   0x143
#define CSR_ADDR_MTVEC   0x305
#define CSR_ADDR_MTVAL   0x343
#define CSR_ADDR_DPC     0x7B1

#define CSR_ADDR_MSTATUS 0x300

#define CSR_ADDR_SATP    0x180
#define CSR_ADDR_MATP    0x7c0

#define CSR_ADDR_SIE     0x104
#define CSR_ADDR_SIP     0x144

#define CSR_ADDR_MIP     0x344

#define CSR_ADDR_MEDELEG     0x302
#define CSR_ADDR_MIDELEG     0x303

#define CSR_ADDR_SSTATUS     0x100
#define CSR_ADDR_SCAUSE      0x142 

#define CSR_ADDR_MCAUSE      0x342

#define CSR_ADDR_TDATA1      0x7A1

#define CSR_ADDR_DCSR        0x7B0

#define CSR_RESET_VALUE_ZERO 0x0UL

int main()
{
    C_TEST_START;
   
  #ifdef MINSHIRE_GATESIM_ENABLE
    //#pragma message("MINSHIRE_GATESIM_ENABLE")
    volatile uint64_t hart_id = get_hart_id();    
    volatile uint64_t thread_id  = (hart_id & 1);
    volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
    volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
    volatile uint64_t timeout = TIMEOUT;

    if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
      s32_scp_init();
      give_credit(0, -1, -1, 0);
      give_credit(32, -1, -1, 0);
    }
 
    wait_for_credit(0);  

    if (shire_id == 0) {
  #endif

    //MEPC: cover sign-extend CSRs 
    csr_write(CSR_ADDR_MEPC, CSR_RESET_VALUE_ZERO); // write some reset value;    
    volatile uint64_t mepc_value = csr_read(CSR_ADDR_MEPC);
    csr_write(CSR_ADDR_MEPC, (mepc_value | 0x0FFF800000000000)); //write an invalid value related to sign extension
    mepc_value = csr_read(CSR_ADDR_MEPC); //read after write
    csr_write(CSR_ADDR_MEPC, 0x0000AABBCCDDEEFF);   //write a valid sign extension value
    mepc_value = csr_read(CSR_ADDR_MEPC); //read after write
     
    //SEPC: cover sign-extend CSRs
    csr_write(CSR_ADDR_SEPC, CSR_RESET_VALUE_ZERO); //write some reset value;
    volatile uint64_t sepc_value = csr_read(CSR_ADDR_SEPC);
    csr_write(CSR_ADDR_SEPC, (sepc_value | 0x0FFF800000000000)); //write an invalid value related to sign extension
    sepc_value = csr_read(CSR_ADDR_SEPC); //read after write
    csr_write(CSR_ADDR_MEPC, 0x0000AABBCCDDEEFF);   //write a valid sign extension value
    sepc_value = csr_read(CSR_ADDR_SEPC); //read after write
         
    //STVEC: cover sign-extend CSRs
    csr_write(CSR_ADDR_STVEC, CSR_RESET_VALUE_ZERO); //write some reset value;
    volatile uint64_t stvec_value = csr_read(CSR_ADDR_STVEC);
    csr_write(CSR_ADDR_STVEC, (stvec_value | 0x0FFF800000000000)); //write an invalid value related to sign extension, not this is bit[47] on DUT that gets extended
    stvec_value = csr_read(CSR_ADDR_STVEC); //read after write
    csr_write(CSR_ADDR_STVEC, stvec_value | 0x0000AABBCCDDEEFF);   //write a valid sign extension value
    stvec_value = csr_read(CSR_ADDR_STVEC); //read after write
    
    //STVAL: cover sign-extend CSRs 
    csr_write(CSR_ADDR_STVAL, CSR_RESET_VALUE_ZERO); //write some reset value
    volatile uint64_t stval_value = csr_read(CSR_ADDR_STVAL);
    csr_write(CSR_ADDR_STVAL, (stval_value | 0x0FFF800000000000)); //write an invalid value related to sign extension
    stval_value = csr_read(CSR_ADDR_STVAL); //read after write
    csr_write(CSR_ADDR_STVAL, 0x0000AABBCCDDEEFF); //write a valid sign extension value
    stval_value = csr_read(CSR_ADDR_STVAL); //read after write 


    //MTVEC: cover sign-extend CSRs
    csr_write(CSR_ADDR_MTVEC, CSR_RESET_VALUE_ZERO); //write some reset value
    volatile uint64_t mtvec_value = csr_read(CSR_ADDR_MTVEC);
    csr_write(CSR_ADDR_MTVEC, (mtvec_value | 0x0FFF800000000000)); //write an invalid value related to sign exstension
    mtvec_value = csr_read(CSR_ADDR_MTVEC); //read after write
    csr_write(CSR_ADDR_MTVEC, 0x0000AABBCCDDEEFF); //write a valid sign extension value
    mtvec_value = csr_read(CSR_ADDR_MTVEC);


    //MTVAL: cover sign-extended CSRs
    csr_write(CSR_ADDR_MTVAL, CSR_RESET_VALUE_ZERO); //write some reset value
    volatile uint64_t mtval_value = csr_read(CSR_ADDR_MTVAL);
    csr_write(CSR_ADDR_MTVAL, (mtval_value | 0xFFF7000000000000)); //write an invalid value related to sign extension
    mtval_value = csr_read(CSR_ADDR_MTVAL); //read after write
    csr_write(CSR_ADDR_MTVAL, 0x0000AABBCCDDEEFF); //write a valid sign extension value
    mtval_value = csr_read(CSR_ADDR_MTVAL); //read after write
    

    //-- writing reserved mpp bits to MSTATUS CSR 
    volatile uint64_t mstatus_val = csr_read(CSR_ADDR_MSTATUS);
    mstatus_val |= (1 << 12);
    mstatus_val &=~(1 << 11);
    csr_write(CSR_ADDR_MSTATUS, mstatus_val);
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);


    //-- SATP valid, mode bits equal to CSR_SATP_MODE_BARE, CSR_SATP_MODE_SV39, CSR_SATP_MODE_SV48
    csr_write(CSR_ADDR_SATP, CSR_RESET_VALUE_ZERO);       
    volatile uint64_t satp_value = csr_read(CSR_ADDR_SATP);
    csr_write(CSR_ADDR_SATP, ((0x9UL << 60) | satp_value | 0x0000000000001234)); //using satp value such that compiler does not optimize it out
    satp_value = csr_read(CSR_ADDR_SATP);
    csr_write(CSR_ADDR_SATP, ((0x8UL << 60) | 0x0000000000001234)); 
    satp_value = csr_read(CSR_ADDR_SATP);
    csr_write(CSR_ADDR_SATP, ((0x0UL << 60) | 0x0000000000001234));
    satp_value = csr_read(CSR_ADDR_SATP);
    csr_write(CSR_ADDR_SATP, ((0xFUL << 60) | 0x0000000000001234)); 
    satp_value = csr_read(CSR_ADDR_SATP);

    // -- MATP -- //
    // lock=off modes valid
    //
    volatile uint64_t matp_value = csr_read(CSR_ADDR_MATP);
    csr_write(CSR_ADDR_MATP, (matp_value &=~(0xFFUL << 60))); //write zero mode
    //csr_write(CSR_ADDR_MATP, (matp_value | (0x8UL << 60))); // NOT POSSIBLE TO WRITE THIS VALID MODE
    //csr_write(CSR_ADDR_MATP, ((0x9UL << 60) | 0x0000000000004321)); //NOT POSSIBLE TO WRITE THIS VALID MODE
    matp_value = csr_read(CSR_ADDR_MATP);

    // lock=off mode invalid
    csr_write(CSR_ADDR_MATP, (matp_value | (0xFUL << 60))); //write not supported mode
    matp_value = csr_read(CSR_ADDR_MATP);

    //lock=OFF->ON
    csr_write(CSR_ADDR_MATP, (matp_value | (0x1UL <<59))); //write lock bit
    matp_value = csr_read(CSR_ADDR_MATP);    

    //lock=ON mode valid
    csr_write(CSR_ADDR_MATP, (matp_value | (0x8UL << 60))); //try to write legal mode with lock=1
    matp_value = csr_read(CSR_ADDR_MATP);    

    //lock=ON, mode invalid
    csr_write(CSR_ADDR_MATP, (matp_value | (0xFUL << 60))); //write illegal mode with lock=1
    matp_value = csr_read(CSR_ADDR_MATP);
    // --MATP ENDE

    //-- SIE write ones to zero fields
    csr_write(CSR_ADDR_SIE, CSR_RESET_VALUE_ZERO);
    volatile uint64_t sie_value = csr_read(CSR_ADDR_SIE);
    csr_write(CSR_ADDR_SIE, sie_value); //write valid
    sie_value = csr_read(CSR_ADDR_SIE);
    csr_write(CSR_ADDR_SIE, (sie_value | 0xFFFFFFFFFFFFFDDD)); //write "illegal"
    sie_value = csr_read(CSR_ADDR_SIE);    
     
    //-- SIP write ones to zero fields
    csr_write(CSR_ADDR_SIP, CSR_RESET_VALUE_ZERO);
    volatile uint64_t sip_value = csr_read(CSR_ADDR_SIP);
    csr_write(CSR_ADDR_SIP, sip_value); //write valid
    sip_value = csr_read(CSR_ADDR_SIP);
    csr_write(CSR_ADDR_SIP, (sip_value | 0xFFFFFFFFFFFFFDDD )); //write "illegal"
    sip_value = csr_read(CSR_ADDR_SIP);

    
    // --MIP write ones to zero fields, write illegal regarding interrupt inputs
    volatile uint64_t mip_value = csr_read(CSR_ADDR_MIP);
    csr_write(CSR_ADDR_MIP, mip_value); //write valid regarding to zeroes
    mip_value = csr_read(CSR_ADDR_MIP);
    csr_write(CSR_ADDR_MIP, (mip_value | 0xFFFFFFFFFF76F555)); //write "illegal" regarding to zeroes
    mip_value = csr_read(CSR_ADDR_MIP);
    csr_write(CSR_ADDR_MIP, (mip_value | (1UL << 11))); //write "illegal" regarding interrupt inputs
    mip_value = csr_read(CSR_ADDR_MIP);
    csr_write(CSR_ADDR_MIP, (mip_value | 0xFFFFFFFFFFFFFFFF)); //write all ones
    mip_value = csr_read(CSR_ADDR_MIP);

    // -- mstatus.SD is RO => writing is ignored, reads return 1 if either FS or XS are dirty
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);
    csr_write(CSR_ADDR_MSTATUS, (mstatus_val | (1UL<<63))); //write "0" regarding to zeroes
    mstatus_val = csr_read(CSR_ADDR_MSTATUS); 
    csr_write(CSR_ADDR_MSTATUS, (mstatus_val & ~(1UL<<63))); //write "1" regarding to zeroes
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);     
    // -- the following fields of mstatus are hardwired (writes ignored): SXL=2, UXL=2
    csr_write(CSR_ADDR_MSTATUS, ((mstatus_val | (1UL<<34)) &~ (1UL<<35) )); //write "01" opposite of 0x2
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);     
    csr_write(CSR_ADDR_MSTATUS, ((mstatus_val | (1UL<<35)) &~ (1UL<<34) )); //write "10" legal
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);     

    csr_write(CSR_ADDR_MSTATUS, ((mstatus_val | (1UL<<32)) &~ (1UL<<33) )); //write "01" opposite of 0x2
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);     
    csr_write(CSR_ADDR_MSTATUS, ((mstatus_val | (1UL<<33)) &~ (1UL<<32) )); //write "10" legal
    mstatus_val = csr_read(CSR_ADDR_MSTATUS);     

    // -- mideleg => mieco, mbad_rep, meip, ueip, mtip, utip, msip, usip are non delegable => hardwired to 0
    volatile uint64_t mideleg_val = csr_read(CSR_ADDR_MIDELEG);
    csr_write(CSR_ADDR_MIDELEG, (mideleg_val | 0xFFFFFFFFFFFFFEEE)); //write illegal
    mideleg_val = csr_read(CSR_ADDR_MIDELEG);
    csr_write(CSR_ADDR_MIDELEG, (mideleg_val & ~(0xFFFFFFFFFFFFFEEE))); //write legal
    mideleg_val = csr_read(CSR_ADDR_MIDELEG);


    // -- medeleg => non delegable xcpts are also hardwired to 0 (see fields in green in the spreadsheet for type csr_medeleg_t)
    volatile uint64_t medeleg_val = csr_read(CSR_ADDR_MEDELEG);
    csr_write(CSR_ADDR_MEDELEG, (medeleg_val | 0xFFFFFFFFFFFFA776)); //write illegal
    medeleg_val = csr_read(CSR_ADDR_MEDELEG);
    csr_write(CSR_ADDR_MEDELEG, (medeleg_val & ~(0xFFFFFFFFFFFFA776))); //write legal
    medeleg_val = csr_read(CSR_ADDR_MEDELEG);
    
    // -- dcsr: xdebugver hardwired to 4, stepie, stopcount, stoptime to zero
    //DEBUG ONLY REGISTER impossible to hit
    //

    // -- tdata1: ttype hardwired to 2, maskmax to 6, hit, select to zero. action[5:1] is hardwired to 0 (only action[0] can be set to either 0 or 1), match to 0
    //DEBUG ONLY REGISTER impossible to hit
    //

    
    // -- SSTATUS -- //
    volatile uint64_t sstatus_value = csr_read(CSR_ADDR_SSTATUS);
    csr_write(CSR_ADDR_SSTATUS, (sstatus_value | 0x7FFFFFFCFFF21EDD)); //try to write all ones at hardwired zero bits
    sstatus_value = csr_read(CSR_ADDR_SSTATUS);
    

    // -- STVEC: write ones on zeroed fields
    stvec_value = csr_read(CSR_ADDR_STVEC);
    csr_write(CSR_ADDR_STVEC, (stvec_value | 0xFFEUL));
    stvec_value = csr_read(CSR_ADDR_STVEC);

    //-- SEPC write 1 to bit 0
    sepc_value = csr_read(CSR_ADDR_SEPC);
    csr_write(CSR_ADDR_SEPC, (sepc_value | 0x1UL));
    sepc_value = csr_read(CSR_ADDR_SEPC);
    

    //-- SCAUSE -- not fully implemented in BEMU
    csr_write(CSR_ADDR_SCAUSE, CSR_RESET_VALUE_ZERO);
    volatile uint64_t scause_value = csr_read(CSR_ADDR_SCAUSE);
    csr_write(CSR_ADDR_SCAUSE, (scause_value | 0x7FFFFFFFFFFFFFE0UL));
    scause_value = csr_read(CSR_ADDR_SCAUSE);
    //scause_value |= 0xdeadbeefdeadbeefUL;

    satp_value = csr_read(CSR_ADDR_SATP);
    csr_write(CSR_ADDR_SATP, (satp_value | 0x0FFFFFFFF0000000));
    satp_value = csr_read(CSR_ADDR_SATP);
    

    // -- write one on zero field
    mepc_value = csr_read(CSR_ADDR_MEPC);
    csr_write(CSR_ADDR_MEPC, (mepc_value | 0x1UL));
    mepc_value = csr_read(CSR_ADDR_MEPC);
    
 
    volatile uint64_t mcause_value = csr_read(CSR_ADDR_MCAUSE);
    csr_write(CSR_ADDR_MCAUSE, (mcause_value | 0x7FFFFFFFFFFFFFE0));
    mcause_value = csr_read(CSR_ADDR_MCAUSE);
    

    volatile uint64_t tdata1_value = csr_read(CSR_ADDR_TDATA1);
    csr_write(CSR_ADDR_TDATA1, (tdata1_value | (0x001FFFFFFFFBEF20UL)));
    tdata1_value = csr_read(CSR_ADDR_TDATA1);
    

    // -- cannot enter debug from M-mode
    //volatile uint64_t dcsr_value = csr_read(CSR_ADDR_DCSR);
    //csr_write(CSR_ADDR_DCSR, (dcsr_value | 0xFFFFFFFFFFFF8E20UL));
    //dcsr_value = csr_read(CSR_ADDR_DCSR);
    //dcsr_value &= 0xFFFFFFFFFFFF8E20UL;

    // -- cannot access debug from M-mode
    //volatile uint64_t dpc_value = csr_read(CSR_ADDR_DPC);
    //csr_write(CSR_ADDR_DPC, (dpc_value | 0x1));
    //dpc_value = csr_read(CSR_ADDR_DPC);
    //dpc_value &= 0x1;

    matp_value = csr_read(CSR_ADDR_MATP);
    csr_write(CSR_ADDR_MATP, (matp_value | 0x07FFFFFFF0000000UL));
    matp_value = csr_read(CSR_ADDR_MATP);
    

  #ifdef MINSHIRE_GATESIM_ENABLE
 //     if (get_hart_id() % 2 == 0) wait_for_credit(1); // to ensure that t1 is successfully finished
 //     else give_credit(0, minion_id, 0, 1); 
    }

    if (shire_id == 0 and thread_id == 0) {
      atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*64); //Virtual address (rs1) must be aligned to an 8 byte boundary
      //give_credit(32, minion_id, 0, 1); 
    }

    if (shire_id == 32 and thread_id == 0) {
      //wait_for_credit(1);
      check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*64); //Virtual address (rs1) must be aligned to an 8 byte boundary       
    }
  #endif


    C_TEST_PASS;
}

