/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "tensors.h"
#include "cacheops.h"
#include "et_test_common.h"

#define MRAM_BASE 0x40000000ULL  // MRAM starts at 1GB
#define SMODE_SIZE 11 // Leave some room for code execution 
#define MMODE_SIZE 11 // 2^11 * 4KB = 8MB region 

typedef struct {
   const char* name;
   uint64_t start_addr;
   uint64_t end_addr;
   uint64_t write_allowed; // 1 if write access is allowed, 0 if read-only
   uint64_t amo_allowed;   // 1 if AMO access is allowed, 0 if not
   uint64_t tensor_allowed; // 1 if tensor access is allowed, 0 if not
   uint64_t cacheops_allowed; // 1 if cacheops access is allowed, 0 if not
   uint64_t alignment; // Alignment requirement for accesses (encoded as 2^n)
   uint64_t data_width; // Data width for accesses (encoded as bytes)
   esr_prot_t privilege; // Required privilege level for access (PRV_M, PRV_S, PRV_U)
} mem_region_t;

typedef enum {
   MEM_OP_LOAD,
   MEM_OP_STORE,
   MEM_OP_AMO,
   MEM_OP_CACHEOP
} mem_op_t;

typedef enum {
   UINT8  = 1,
   UINT16 = 2,
   UINT32 = 4,
   UINT64 = 8
} data_width_t;

typedef struct {
   uint64_t   addr;
   uint64_t   data;
   uint64_t   data_width;
   uint64_t   alignment;
   mem_op_t   op;
} mem_req_t;

typedef uint64_t (*mem_op_fn_t)(mem_req_t* req);
// Test counters and state
volatile uint32_t test_fail = 0;
volatile uint32_t exceptions_caught = 0;
volatile esr_prot_t cur_priv = PRV_M; // Start in M-mode
//volatile uint64_t *ipi_pc = (uint64_t*) ESR_NEIGH(0, IPI_REDIRECT_PC);
const char* priv_str[] = {"U", "S", "H", "M"};
const uint64_t size_list[] = {UINT32, UINT8, UINT16, UINT64}; // 
const mem_op_t op_list[] = {MEM_OP_STORE, MEM_OP_LOAD, MEM_OP_AMO};//, MEM_OP_CACHEOP};
const char* op_str[] = {"STORE", "LOAD", "AMO", "CACHEOP"};

uint64_t mem_op_load(mem_req_t* req) {
   uint64_t buf = 0;
   et_printf_long("DEBUG: Performing operation %s from address 0x%lx with data width %lu byte(s)\n", op_str[req->op], req->addr, req->data_width);
   switch (req->data_width) {
      case UINT8:   buf = *(volatile uint8_t *)  req->addr; break;
      case UINT16:  buf = *(volatile uint16_t *) req->addr; break;
      case UINT32:  buf = *(volatile uint32_t *) req->addr; break;
      case UINT64:  buf = *(volatile uint64_t *) req->addr; break;
      default: return 0;
   }
   FENCE;
   return buf;
}

uint64_t mem_op_store(mem_req_t* req) {
   switch (req->data_width) {
      case UINT8:   *(volatile uint8_t *)  req->addr = (uint8_t)  req->data; break;
      case UINT16:  *(volatile uint16_t *) req->addr = (uint16_t) req->data; break;
      case UINT32:  *(volatile uint32_t *) req->addr = (uint32_t) req->data; break;
      case UINT64:  *(volatile uint64_t *) req->addr = (uint64_t) req->data; break;
      default: break;
   }
   return 0;
}

uint64_t mem_op_amo(mem_req_t* req) {
   uint64_t status = 0;
   switch (req->data_width) {
      case UINT8:
      case UINT16:
      case UINT32:
         asm volatile("amoaddg.w %[result], %[data], (%[addr])\n" : [result] "=r" (status) : [data] "r" (req->data), [addr] "r" (req->addr));
         break;
      case UINT64:
         asm volatile("amoaddg.d %[result], %[data], (%[addr])\n" : [result] "=r" (status) : [data] "r" (req->data), [addr] "r" (req->addr));
         break;
      default: break;
   }
   return status;
}

uint64_t mem_op_cacheop(mem_req_t* req) {
   (void) req;
   __asm__ __volatile__ ("csrw prefetch_va, %[val]\n"  :: [val] "r" (req->addr): );
   return 0;
}


const mem_op_fn_t op_fn_list[] = {mem_op_store, mem_op_load, mem_op_amo, mem_op_cacheop};
const mem_region_t mem_regions[] = {
// NAME                 START_ADDR     END_ADDR       WRITE   AMO   TENSOR  CACHEOPS   ALIGNMENT   DATA_WIDTH     PRIVILEGE
   {"System registers", 0x02000000ULL, 0x02001000ULL, 1,      0,    0,      0,         UINT64,     UINT32,        PRV_S},
   {"MRAM registers",   0x02001000ULL, 0x02002000ULL, 1,      0,    0,      0,         UINT64,     UINT64,        PRV_M},
   {"Periph registers", 0x02002000ULL, 0x02005000ULL, 1,      0,    0,      0,         UINT64,     UINT32,        PRV_S},
   {"Bootrom",          0x02008000ULL, 0x0200A000ULL, 0,      0,    0,      0,         0,          0,             PRV_U},
   {"SRAM",             0x0200C000ULL, 0x0200D000ULL, 1,      0,    1,      1,         0,          0,             PRV_U},
   {"xSPI",             0x0200F000ULL, 0x02010000ULL, 1,      0,    0,      0,         UINT64,     UINT64,        PRV_M},
   {"NIC config",       0xFE000000ULL, 0xFE001000ULL, 1,      0,    0,      0,         UINT64,     UINT32,        PRV_M},
   {"MRAM",             0x40000000ULL, 0x80000000ULL, 1,      1,    1,      1,         0,          0,             PRV_U},
//   //{"CPU regs",         0x80000000ULL, 0xC0000000ULL, 1,      0,    0,      0,         UINT64,     UINT64       , PRV_U},
   {"PLIC",             0xA0000000ULL, 0xA4000000ULL, 1,      0,    0,      0,         UINT32,     UINT32,        PRV_S}
};


extern "C" void test_mtrap_vector();
extern "C" __attribute__((section(".text"))) uint64_t test_mtrap_handler(uint64_t cause, uint64_t epc, uint64_t tval, uint64_t hart) {
   (void) tval;
   (void) hart;
   if (cause == MINION_XCTP_CAUSE_LOAD_ACCESS_FAULT) {
      exceptions_caught++;
      FENCE;
      //et_printf_long("INFO: Caught exception cause %d, tval %lx and epc %lx\n", cause, tval, epc);
      epc+= 4;
   } else if (cause == MINION_XCTP_CAUSE_STORE_ACCESS_FAULT) {
      exceptions_caught++;
      //et_printf_long("INFO: Caught exception cause %d, tval %lx and epc %lx\n", cause, tval, epc);
      epc+= 4;
   } else if (cause == MINION_XCTP_CAUSE_ENV_CALL_FROM_S_MODE) {
      //et_printf_long("INFO: Caught expected ECALL from S-mode\n");
      epc+= 4;
   } else if(cause == MINION_XCTP_CAUSE_ENV_CALL_FROM_U_MODE) {
      //et_printf_long("INFO: Caught expected ECALL from U-mode\n");
      epc+= 4;
   } else {
      //et_printf_long("FAIL: Caught unexpected exception cause %d\n", cause);
      //et_printf_long("FAIL: tval %lx\n", tval);
      //et_printf_long("FAIL: epc %lx\n", epc);
      epc+= 4;
      test_fail++;
      C_TEST_FAIL;
   } // Unexpected exception
   
   return epc;
}

void set_trap_vector()
{
    void (*mtvec_ptr)(void) = &test_mtrap_vector;
    __asm__ __volatile__ ("csrw mtvec, %[mtvec]\n\t" : :[mtvec] "r" (mtvec_ptr) :);
}

// Naked attribute to ensure no prologue/epilogue is generated, allowing us to control the exact instructions executed
__attribute__((naked)) void drop_to_s_mode(uintptr_t entry) {
   uint64_t status = 0;
   
   __asm__ __volatile__ ("csrr %[result], mstatus\n": [result] "=r" (status));
   status &= ~(0x3ULL << 11);
   status |= (0x1ULL << 11); // Set to S-mode
   __asm__ __volatile__ ("csrw mstatus, %[val]\n"  :: [val] "r" (status): );
   __asm__ __volatile__ ("csrw mepc, %[val]\n"     :: [val] "r" (entry):);
   __asm__ __volatile__ ("mret" ::: "memory");
}

__attribute__((naked)) void drop_to_u_mode(uintptr_t entry) {
   uint64_t status = 0;
   
   __asm__ __volatile__ ("csrr %[result], sstatus\n": [result] "=r" (status));
   status &= ~(0x3ULL << 11);
   status |= (0x0ULL << 11); // Set to U-mode
   __asm__ __volatile__ ("csrw sstatus, %[val]\n"  :: [val] "r" (status): );
   __asm__ __volatile__ ("csrw sepc, %[val]\n"     :: [val] "r" (entry):);
   __asm__ __volatile__ ("sret");
}

/**
 * Check exceptions 
 */
void check_exception(uint64_t is_exception_expected) {
   if (is_exception_expected) {
      if(exceptions_caught>0) {
         //et_printf_long("OK: Caught expected exception\n");
      } else {
         et_printf_long("ERROR: Expected exception but none caught\n");
         test_fail++;
         C_TEST_FAIL;
      }
   } else {
      if(exceptions_caught>0) {
         et_printf_long("ERROR: Caught unexpected exception\n");
         test_fail++;
         C_TEST_FAIL;
      } else {
         //et_printf_long("OK: No exception as expected\n");
      }
   }
   exceptions_caught = 0; // Reset for next test
   FENCE;
}

uint64_t validate_privilege(esr_prot_t required_priv) {
   switch (cur_priv) {
      case PRV_M: return 1; // M-mode can access all regions
      case PRV_S: return required_priv != PRV_M; // S-mode can access S and U regions
      case PRV_U: return required_priv == PRV_U; // U-mode can only access U regions
      default: return 0; // Invalid privilege level
   }
}

uint64_t pma_access_fault(const mem_region_t* region, mem_req_t* req) {
   if (!region->write_allowed && (req->op == MEM_OP_STORE || req->op == MEM_OP_AMO || req->op == MEM_OP_CACHEOP)) 
      return 1; // Write access to read-only region
   if (!validate_privilege(region->privilege)) 
      return 1; // Access denied due to privilege
   if (req->op == MEM_OP_AMO && !region->amo_allowed) 
      return 1; // AMO access not allowed
   if (req->op == MEM_OP_CACHEOP && !region->cacheops_allowed) 
      return 1; // Cacheop access not allowed
   if (region->data_width != 0 && (req->data_width != region->data_width)) 
      return 1; // Invalid data width
   return 0; // Access allowed
}

uint64_t test_region(const mem_region_t* region) {
   mem_req_t req;
   
   // Initialize request for the region
   req.addr = region->start_addr;
   req.data = 0xdeadbeef;
   req.data_width = UINT8;
   req.alignment = 0;
   req.op = MEM_OP_AMO;

   // Test read access 
   for (uint8_t i = 0; i < sizeof(op_list)/sizeof(op_list[0]); i++) {
      req.op = op_list[i];
      for (uint8_t j = 0; j < sizeof(size_list)/sizeof(size_list[0]); j++) {
         req.data_width = size_list[j];
         //et_printf_long("INFO: %s %lu-bit value from %s [%s] with privilege %s\n", op_str[req.op], req.data_width * 8, 
         //   region->name, priv_str[region->privilege], priv_str[cur_priv]);
         FENCE;
         op_fn_list[i](&req);
         FENCE;
         check_exception(pma_access_fault(region, &req));
         FENCE;
      }
   }
   
   return 0;
}

void test_all_regions() {
   for (unsigned int i = 0; i < sizeof(mem_regions)/sizeof(mem_region_t); i++) {
      //et_printf_long("Testing access to %s (0x%lx - 0x%lx) with privilege %s\n", mem_regions[i].name, mem_regions[i].start_addr, mem_regions[i].end_addr, priv_str[cur_priv]);
      FENCE;
      test_region(&mem_regions[i]);
   }
}

int main() {
   C_TEST_START;

   uint64_t tid;
   tid = get_thread_id();

   // Setup machine trap vector
   set_trap_vector();

   // Only thread 0 executes tests
   if (tid == 0) {
      //et_printf_long("\n=== Erbium PMA Specification Tests ===\n\n");
      //for (unsigned int i = 0; i < sizeof(mem_regions)/sizeof(mem_region_t); i++) {
      //   et_printf_long("Defined region: %s (0x%lx - 0x%lx), Write: %d, AMO: %d, Tensor: %d, Cacheops: %d, Alignment: %d, Data Width: %d, Privilege: %s\n",
      //      mem_regions[i].name, mem_regions[i].start_addr, mem_regions[i].end_addr, mem_regions[i].write_allowed,
      //      mem_regions[i].amo_allowed, mem_regions[i].tensor_allowed, mem_regions[i].cacheops_allowed,
      //      mem_regions[i].alignment, mem_regions[i].data_width, priv_str[mem_regions[i].privilege]);
      //}

      //et_printf_long("Current Privilege Level: M-mode\n");
      cur_priv = PRV_M;
      test_all_regions();

      //et_printf_long("Current Privilege Level: S-mode\n");
      cur_priv = PRV_S;
      drop_to_s_mode((uintptr_t)test_all_regions);

      //et_printf_long("Current Privilege Level: U-mode\n");
      cur_priv = PRV_U;
      drop_to_u_mode((uintptr_t)test_all_regions);

      if (test_fail > 0) {
         C_TEST_FAIL;
         et_printf_long("FAIL: %d test(s) failed\n", test_fail);
      }
   }

   C_TEST_PASS;
   return 0;
}
