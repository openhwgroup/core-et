/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _VM_BOUNDARY_COMMON_H_
#define _VM_BOUNDARY_COMMON_H_
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////
// MANAGING VM
////////////////////////////////////////////////////////////////////////////////
#define BASE_PA 0x8000000000
#define PAGE1G_SIZE (1ULL<<30)

#define LAST_PAGE_VA_BASE 0xFFFFFFFFC0000000ULL
#define LAST_PAGE_PA_BASE 0x80C0000000

#define MISALIGNED_SUPER_PAGE_PA (BASE_PA + PAGE1G_SIZE*15 + (PAGE1G_SIZE>>1))
#define MISALIGNED_SUPER_PAGE_VA 0x40000000ULL


typedef struct {
  uintptr_t va;
  uintptr_t pa;
} page1G_setup_t;

void setup_vmem_sv48();

////////////////////////////////////////////////////////////////////////////////
// OTHER COMMON TEST ROUTINES
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define LONG_CALL(symbol)			    \
  do {						    \
    static uintptr_t __ad = (uintptr_t)symbol;      \
    static void (*__fn)() = (void (*)()) __ad ;	    \
    __fn();					    \
  }while(0)

#define LAST_PG_ATTS  __attribute__((section(".last_pg_data")))
#define LAST_PG_FNC_ATTS __attribute__((section(".last_pg_text")))  __attribute__ ((noinline))

#define LAST_PG_BOUNDARY_ATTS __attribute__((section(".last_pg_data_boundary")))
#define LAST_PG_BOUNDARY_FNC_ATTS __attribute__((section(".last_pg_text_boundary")))  __attribute__ ((noinline))


typedef enum {
              CAUSE_MISALIGNED_FETCH       = 0x0,  // Instruction address misaligned
              CAUSE_FETCH_ACCESS           = 0x1,  // Instruction access fault      
              CAUSE_ILLEGAL_INSTRUCTION    = 0x2,  // Illegal instruction           
              CAUSE_BREAKPOINT             = 0x3,  // Breakpoint                    
              CAUSE_MISALIGNED_LOAD        = 0x4,  // Load address misaligned       
              CAUSE_LOAD_ACCESS            = 0x5,  // Load access fault             
              CAUSE_MISALIGNED_STORE       = 0x6,  // Store/AMO address misaligned  
              CAUSE_STORE_ACCESS           = 0x7,  // Store/AMO access fault        
              CAUSE_USER_ECALL             = 0x8,  // Environment call from U-mode  
              CAUSE_SUPERVISOR_ECALL       = 0x9,  // Environment call from S-mode  
              CAUSE_MACHINE_ECALL          = 0xb,  // Environment call from M-mode  
              CAUSE_FETCH_PAGE_FAULT       = 0xc,  // Instruction page fault        
              CAUSE_LOAD_PAGE_FAULT        = 0xd,  // Load page fault
              CAUSE_DEBUG                  = 0xe,  // debugger
              CAUSE_STORE_PAGE_FAULT       = 0xf,  // Store/AMO page fault          
              CAUSE_FETCH_BUS_ERROR        = 0x19, // l2 err reported by icache
              CAUSE_FETCH_ECC_ERROR        = 0x1a, // double bit error reported by icache
              CAUSE_LOAD_PAGE_SPLIT_FAULT  = 0x1b, // Load page-split fault
              CAUSE_STORE_PAGE_SPLIT_FAULT = 0x1c, // Store page-split fault
              CAUSE_BUS_ERROR              = 0x1d, // Instruction to be implemented in m-code
              CAUSE_MCODE_INSTRUCTION      = 0x1e  // Instruction to be implemented in m-code

} cause_t;

EXTERN_C uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t mtval, uint64_t *regs);
EXTERN_C void test_mtrap_vector();
void to_u_mode();
void  pass();
#endif
