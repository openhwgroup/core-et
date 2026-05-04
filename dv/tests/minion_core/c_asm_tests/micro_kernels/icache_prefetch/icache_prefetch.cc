/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

#include "common.h"

#ifdef MINSHIRE_GATESIM_ENABLE
#include "et_test_common.h"
#define TIMEOUT 4000
#endif

#ifdef __cplusplus
extern "C"
#endif


/* To set bit 31:30 (in the SHIRE_ESR space) which encodes privilege mode
 * --> 00=User; 01=Supervisor; 11=Machine; 10=Reserved (uArch messaging)*/
#define SUPERVISOR_MODE  PRV_S

#define MINION_0 0x1
#define MINION_1 0x2

#define NEIGH_0 0
#define NEIGH_1 1
#define NEIGH_2 2
#define NEIGH_3 3

/*Sets bit 22 in Shire_ESR map to 0*/
#define SHIRE_0 0UL

/*Sets bit 21:17 to 11010 (Shire_ESR)*/
#define SHIRE_ESR 0x1AUL

/**From the memory map (ESR defintion): Chapter 2
 * for SHIRE_ESR bit placement
 * 32=1
 * 31:30 encodes privilege mode --> 00=User; 01=Supervisor; 11=Machine; 10=Reserved (uArch messaging)
 * 29:22 = encdes Shire ID (0 to 34 minion shire), (232 through 239 mem-shires 0 through 7); (253 PCIe-Shire); (254 io-shire)
 * So always <=34 (minion-shire). (for this test, set to 0 as only shireID 0 being used)
 * 21:20:19:18:17 (11010 or x1A)
 * 16:3 will encode different individual ESRs in this space (this is the unique address identifier)
 * 2:0 (always 000)
 */
#define SHIRE_ESR_MAP ((1UL<<32) | ((SHIRE_0)<<22) | ((SHIRE_ESR)<<17))

#define CREDIT_COUNTER_0 0x18UL
#define CREDIT_COUNTER_1 0x19UL
#define CREDIT_COUNTER_2 0x1AUL
#define CREDIT_COUNTER_3 0x1BUL

#define ICACHE_NEIGH_0 0x1
#define ICACHE_NEIGH_1 0x2
#define ICACHE_NEIGH_2 0x4
#define ICACHE_NEIGH_3 0x8
#define ICACHE_ALL_ENABLE (ICACHE_NEIGH_0 | ICACHE_NEIGH_1 |\
                           ICACHE_NEIGH_2 | ICACHE_NEIGH_3)

/*4-bit mask (bits 51:40) in icacahe_prefetch_service (U/S/M mode)
 * each bit indicates which icache in the shire is available for service*/
#define ICACHE_ENABLE_MASK 0x1UL   // indicates neigh-0 icahce will recieve service


void fullNops(void)
{
  __asm__ __volatile__
    (
     //".rept 3000\n"
     ".rept 3024\n"
     "nop\n"
     ".endr\n"
    );
}

void fullNops1(void)
{
  __asm__ __volatile__
    (
     //".rept 3000\n"
     ".rept 3024\n"
     "nop\n"
     ".endr\n"
    );
}

long vaJump64Lines(long addr,int chunck)
{
  long va_addr =  addr + (((long) chunck) <<6);
  return va_addr;
}

void inst_stream_0(void)
{
  waitFCC(0);
  fullNops();
}

void inst_stream_1(void)
{
  waitFCC(0);
  fullNops1();
}

bool isIcacheAvailable (long currentIcacheAddr)
{

  long icacheStatus = 0;

  icacheStatus = read(currentIcacheAddr);

  if (ICACHE_ALL_ENABLE) {
    if (icacheStatus == 0)
      return false;
    else
      return true;
  }
  else
    return false;
}


void prefetchAndSendFcc(uint64_t neigh_id)
{
  /*Defines the icache_prefetch_service ESR address
   *in U-mode= 0x05f; S-mode=0x060; M-mode=0x61
   *Following operates in USER mode*/
  long icachprefTrigaddr = ( SHIRE_ESR_MAP  | (SUPERVISOR_MODE<<30) | (ESR_SHIRE_ICACHE_SPREFETCH_REGNO << 3));
  long fccaddr = (SHIRE_ESR_MAP|(CREDIT_COUNTER_0 << 3));

  //load iCache with 3000 instr
  //Prefectcech instr. for m0-m1
  //1 line = 512b from Mem --> 16 instr -> 1 instr = 32b
  //64 lines --> 1024 instr

  //Prefetch for M0
  long va_plus_size = (vaJump64Lines((unsigned long)fullNops, 0) | (ICACHE_ENABLE_MASK << 48) | 63);
  store(icachprefTrigaddr, va_plus_size);
  while (!isIcacheAvailable(icachprefTrigaddr)) {
    forceDelay();
  }
  va_plus_size = (vaJump64Lines((unsigned long)fullNops, 64) | (ICACHE_ENABLE_MASK << 48) | 63);
  store(icachprefTrigaddr, va_plus_size);
  while (!isIcacheAvailable(icachprefTrigaddr)) {
    forceDelay();
  }
  va_plus_size = (vaJump64Lines((unsigned long)fullNops, 128) | (ICACHE_ENABLE_MASK << 48) | 63);
  store(icachprefTrigaddr, va_plus_size);
  while (!isIcacheAvailable(icachprefTrigaddr)) {
    forceDelay();
  }

  //Prefetch for M1
  va_plus_size = (vaJump64Lines((unsigned long)fullNops1, 0) | (ICACHE_ENABLE_MASK << 48) | 63);
  store(icachprefTrigaddr, va_plus_size);
  while (!isIcacheAvailable(icachprefTrigaddr)) {
    forceDelay();
  }
  va_plus_size = (vaJump64Lines((unsigned long)fullNops1, 64) | (ICACHE_ENABLE_MASK << 48) | 63);
  store(icachprefTrigaddr, va_plus_size);
  while (!isIcacheAvailable(icachprefTrigaddr)) {
    forceDelay();
  }
  va_plus_size = (vaJump64Lines((unsigned long)fullNops1, 128) | (ICACHE_ENABLE_MASK << 48) | 63);
  store(icachprefTrigaddr, va_plus_size);
  while (!isIcacheAvailable(icachprefTrigaddr)) {
    forceDelay();
  }

  //minion is selected by mask on write in this addr. send credit to M0 and M1
  for( neigh_id = 0; neigh_id < 4; neigh_id++)
     store(fccaddr,((MINION_1|MINION_0)<<(neigh_id*8)));

}

void DiscoverTask(uint64_t minion_id) {

  switch ((minion_id & 0x7)) {
  case 0:
    inst_stream_0();
    break;
  case 1:
    inst_stream_1();
    break;
  default:    
    #ifdef MINSHIRE_GATESIM_ENABLE
      atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
    #endif
    C_TEST_PASS;
    break;
  }

}

int main()
{
    uint64_t hart_id = getHartId();

    #ifdef MINSHIRE_GATESIM_ENABLE  
      volatile uint64_t thread_id  = (hart_id & 1);
      volatile uint64_t minion_id = ((hart_id>>1) & 0x1F);      
      volatile uint64_t shire_id  = ((hart_id>>6) & 0x3F);
      volatile uint64_t timeout = TIMEOUT;

      if ((shire_id == 32) && (minion_id == 0) && (thread_id == 0)) {  
        s32_scp_init();
        give_credit(0, -1, -1, 1);
        give_credit(32, -1, -1, 1);
      }
   
      wait_for_credit(1);  

      if (shire_id == 0) {
    #endif

    if( (hart_id & 1) == 0) {
      #ifndef MINSHIRE_GATESIM_ENABLE  
        uint64_t minion_id = ((hart_id>>1) & 0x1F);
        uint64_t shire_id  = ((hart_id>>6) & 0x3F);
      #endif

      if (shire_id == 0) {
        if (minion_id == 7) {

          /*this writes 1 to the shire_coop_mode ESR (address 0x052)
           *this ESR needs to be set to enable icache_prefetch_service
	   *Set to SUPERVISOR mode: This ESR can operate ONLY in S-mode*/
          store((SHIRE_ESR_MAP | (SUPERVISOR_MODE<<30) | (ESR_SHIRE_SHIRE_COOP_MODE_REGNO << 3)), 1);
          prefetchAndSendFcc(((minion_id>>3)&0x3));
        }
        DiscoverTask(minion_id);
      }
    }

    #ifdef MINSHIRE_GATESIM_ENABLE
      }

      if (shire_id == 0 and thread_id == 0) {
        atomic_read_inc_feed(BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary 
      }

      if (shire_id == 32 and thread_id == 0) {
        check_gatesim(timeout, BASE_S0_GATESIM_VALID_ADDR + minion_id*8); //Virtual address (rs1) must be aligned to an 8 byte boundary       
      }
    #endif

    C_TEST_PASS;
    // End marker
    return 0;
}
