/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "vm_boundary_common.h"
#include "macros.h"


// write into validation3 reg => just to see in the waveforms where the test is
#define PROGRESS(d) __asm__ __volatile__("csrw validation3, %[val]\n" : : [val] "r" (d))

////////////////////////////////////////////////////////////////////////////////
// function prototypes and global vars, specifying attributes to select section
// in order to have a specific VA
////////////////////////////////////////////////////////////////////////////////

void errors() LAST_PG_FNC_ATTS;
void dummy_low_page_fn () __attribute__ ((noinline));
void dummy_hi_page_fn () LAST_PG_FNC_ATTS;

uint8_t buffer_hi[512] LAST_PG_ATTS = {0};
uint8_t buffer_lo[512] LAST_PG_ATTS = {0};

uintptr_t bad_fetch []=
  {
   // bad sign extensions
   // the actual address doesn't matter, as it won't actually jump.. so using errors, dummy_low_page_fn
   0xf000000000000000ULL,
   0x0, // there is no page va=0 => page fault
   ((uintptr_t)errors) & 0x0000FFFFFFFFFFFFULL,
   ((uintptr_t)errors) & 0x0001FFFFFFFFFFFFULL,
   ((uintptr_t)errors) & 0x0003FFFFFFFFFFFFULL,
   ((uintptr_t)errors) & 0x7FFFFFFFFFFFFFFFULL,
   ((uintptr_t)dummy_low_page_fn)   | 0x8000000000000000ULL,
   ((uintptr_t)dummy_low_page_fn)   | 0x8000000000000000ULL,
   ((uintptr_t)dummy_low_page_fn)   | 0x1000000000000000ULL,
   ((uintptr_t)dummy_low_page_fn)   | 0x0001000000000000ULL,
   // access to misaligned superpage
   MISALIGNED_SUPER_PAGE_VA,
   MISALIGNED_SUPER_PAGE_VA | 0x30030f0
  };
// using pointer in last pg to avoid relocation truncation problems
// cannot put the bad_fetch directly in last pg because part of it is initialized in .init_array
// which is done before switching on vmem
static  uintptr_t *bad_fetch_ptr LAST_PG_ATTS = bad_fetch;

// bad load/store
uintptr_t bad_ld_st [] = 
  {
   // bad sign extensions
   // the actual address doesn't matter, as it should fail
   0xf000000000000000ULL,
   0,
   ((uintptr_t) buffer_hi) & 0x0000FFFFFFFFFFFFULL,
   ((uint64_t)buffer_hi) & 0x0001FFFFFFFFFFFFULL,
   ((uint64_t)buffer_hi) & 0x0003FFFFFFFFFFFFULL,
   ((uint64_t)buffer_hi) & 0x7FFFFFFFFFFFFFFFULL,
   ((uint64_t)buffer_hi) & 0x7FFFFFFFFFFFFFFFULL,
   ((uint64_t)buffer_lo)   | 0x8000000000000000ULL,
   ((uint64_t)buffer_lo)   | 0x8000000000000000ULL,
   ((uint64_t)buffer_lo)   | 0x1000000000000000ULL,
   ((uint64_t)buffer_lo)   | 0x0001000000000000ULL,
   // access to misaligned superpage
   MISALIGNED_SUPER_PAGE_VA,
   MISALIGNED_SUPER_PAGE_VA | 0x30030f0
  };
//using pointer in last pg to avoid relocation truncation problems
static  uintptr_t *bad_ld_st_ptr LAST_PG_ATTS = bad_fetch;

////////////////////////////////////////////////////////////////////////////////
// test entry point
////////////////////////////////////////////////////////////////////////////////
int main(){
  C_TEST_START;

  // setup vmem and start u-mode
  setup_vmem_sv48();
  to_u_mode();

  LONG_CALL(errors);
    
  pass();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// test itself
////////////////////////////////////////////////////////////////////////////////

void errors(){
  // produce errors that will end up in traps => in the trap handler, return to mepc +4 to skip the 'faulty' instruction
  // the mcause, mtval, mepc are read from within the trap handler, so the BEMU coSim will check their correct values

  // fetch from invalid VA
  // note that saves in a0 PC after the jump (because mepc is set to the bad fetch address.. we will use a0 (x10) as the return address
  // in the trap handler
  uintptr_t * ptr_ = bad_fetch_ptr;
  for ( unsigned i = 0; i < sizeof(bad_fetch) / sizeof (uintptr_t) ; i++, ptr_++) {
    PROGRESS(i);
      __asm__ __volatile__
      (
       "la a0, 1f\n"
       "jalr x0, %[jal_dest]\n"
       "1: nop\n"
       : : [jal_dest] "r"  (*ptr_) : "a0"
       );
  }
  ptr_ = bad_ld_st_ptr;

  // trap handler will return to mepc+4, no need to save in a0
  for ( unsigned i = 0; i < sizeof(bad_ld_st) / sizeof (uintptr_t) ; i++, ptr_++) {
     PROGRESS(i | (0x80000));
     __asm__ __volatile__
       (
        "ld x0, 0(%[dest])\n"
        "sd x0, 0(%[dest])\n"
        "nop\n"
        : : [dest] "r"  (*ptr_)
       );
  }

  
}



////////////////////////////////////////////////////////////////////////////////
// dummy helper functions
////////////////////////////////////////////////////////////////////////////////
void dummy_low_page_fn () {
  __asm__ __volatile__("nop\n");
}
void dummy_hi_page_fn () {
  __asm__ __volatile__("nop\n");
}

////////////////////////////////////////////////////////////////////////////////
//trap handler
////////////////////////////////////////////////////////////////////////////////

uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t mtval, uint64_t *regs)
{
  
  switch (cause)
    {
    case CAUSE_USER_ECALL:
      // if getting here because of ecall... set epc+=4, mpp=3 and return
      // this will just switch to m-mode => so that we can call C_TEST_PASS
      __asm__ __volatile__
        (
         "li t0, 0x1800\n"
         "csrs mstatus, t0\n"
         :
         :
         : "t0" );
      return epc+4;
      
      // for fetch faults, return to what has been set in a0 (regs[10])
    case CAUSE_FETCH_ACCESS:
    case CAUSE_FETCH_PAGE_FAULT:
      return regs[10];
      
      // for data faults, return to epc + 4 (epc is the faulty ld/st.. returning to epc + 4 will skip it)
    case CAUSE_LOAD_ACCESS:
    case CAUSE_STORE_ACCESS:
    case CAUSE_LOAD_PAGE_FAULT:
    case CAUSE_STORE_PAGE_FAULT:
      return epc + 4;
      
      
      // other causes are not expected => end and fail test if received
    default:
      C_TEST_FAIL;
    }

  regs[0] = mtval; // dummy assignment, because it is compiled with -Werror and if mtval is not used
                   // we get compilation error (won't remove parameter, as trap handler prototype is generic)
  return epc + 4;
}

