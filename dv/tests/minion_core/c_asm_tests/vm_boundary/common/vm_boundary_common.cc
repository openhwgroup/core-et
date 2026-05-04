/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "vm_boundary_common.h"
#include "macros.h"

////////////////////////////////////////////////////////////////////////////////
// MANAGING VM
////////////////////////////////////////////////////////////////////////////////

// page table configuration
static page1G_setup_t page_conf_sv48[] =
  {
   {LAST_PAGE_VA_BASE, LAST_PAGE_PA_BASE}, // page with VA= last valid VA (MSB are all 1's)
   // some pages with PA=VA, starting with the BASE => this should include all other sections
   // (.data and stack are probably the only ones needed)
   { BASE_PA, BASE_PA},
   { BASE_PA + PAGE1G_SIZE, BASE_PA + PAGE1G_SIZE},
   { BASE_PA + PAGE1G_SIZE*2, BASE_PA + PAGE1G_SIZE*2},
   // this is a 1G page whose PA is not aligned to 1G => page fault expected if  used
   { MISALIGNED_SUPER_PAGE_VA, MISALIGNED_SUPER_PAGE_PA}
  };


// the page tables
static uint64_t table0[512] __attribute__ ((aligned (1<<12))) = {0};
static uint64_t table1[512]  __attribute__ ((aligned (1<<12)))= {0};


void setup_vmem_sv48(){
  for ( long unsigned int i = 0; i < sizeof(page_conf_sv48)/sizeof(page1G_setup_t); i++){
    // root
    uint64_t va = page_conf_sv48[i].va;
    uint64_t pa = page_conf_sv48[i].pa;

    // check VA is valid => bits 63:47 either all ones or all zeros
    if ( (va >> 47) != 0 && (va>>47) != ((1<<17)-1)) {
      C_TEST_FAIL;
    }

    // remove MSB for indexing purposes
    va&= (1ULL<<48)-1;

    // fill entry for table0
    int vpn = va >> (12+9*3);
    uintptr_t table1_ad = (uintptr_t) table1;
    table0[vpn] = 0x1 | ((table1_ad>>12)<<10);

    // fill entry for table1
    vpn = (va >> (12 + 9*2)) & 0x1FF;
    table1[vpn] = 0xDF | ((pa>>12)<<10);
  }
  
  uintptr_t satp = (uintptr_t) table0;
  satp>>=12;
  satp|= ( 9ULL<<60);
  __asm__ __volatile__ ( "csrw satp, %[v]\n" : : [v] "r" (satp));

  __asm__ __volatile__ ( "fence\n");  

  // and flush the tables
   for ( int way=0; way < 4; way ++){
    uint64_t flush =
      (1ULL<<58) | //to L2
      (0<<14) | // starting at set 0
      (way<<6)|
      0xF;   // the whole 16 sets of the way
    __asm__ __volatile__ ( "csrw 0x7fb, %[v]\n" : : [v] "r" (flush)  );
  }

  // and tensor wait
  __asm__ __volatile__ ( "csrwi 0x830, 6\n");
}


////////////////////////////////////////////////////////////////////////////////
// OTHER COMMON TEST ROUTINES
////////////////////////////////////////////////////////////////////////////////

void to_u_mode() {
  __asm__ __volatile__ 
    (
     // configure trap vector and move to user mode
     "la t0, test_mtrap_vector\n"
     "csrw mtvec, t0\n"
     "li t0, 0x1800\n"
     "csrrc x0, mstatus, t0\n"  // clear mstatus.mpp
     "la t0, 1f\n"
     "csrw mepc, t0\n"          // set mepc to user-mode entry point
     "mret\n"                   // go to user mode
     "1:\n"                     // label to referr to user-mode
     : 
     : 
     : "t0"
     );
}



void pass() {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
  C_TEST_PASS;
}


