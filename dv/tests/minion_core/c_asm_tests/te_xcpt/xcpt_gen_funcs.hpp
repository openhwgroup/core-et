
#pragma once
#include <functional>
#include "te_xcpt.hpp"

uint64_t hart_return_addr[2048];

#define XCPT_GEN_FUNC(x) xcpt_gen_##x

void xcpt_gen_ILLEGAL_INSTR () {
  __asm__ __volatile__ ("unimp");
  __asm__ __volatile__ ("nop");
}

void xcpt_gen_BREAKPOINT () {
  uint64_t st_mem_pos = 0xDEADBEAF;
  volatile uint64_t *st_mem_addr = &st_mem_pos;
  uint64_t tdata1_mask = 1 << 1 | // match with stores
    1 << 6 | // in m-mode
    1 << 4 | // in s-mode
    1 << 3; // in u-mode

  uint64_t hartid = 0;
  __asm__ __volatile__ 
    ("csrr %[id], hartid\n"
     :
     : [id] "r" (hartid)
     :
     );
  
  priv_lv_t current_priv = hart_priv_mode[hartid];
  
  change_priv_to(hartid, priv_lv_t::MMODE);
  // Setup breakpoint ST to ST_ADDR
  __asm__ __volatile__
    ( "csrw tdata2, %[MEM_ADDR]\n"
      "csrs tdata1, %[MASK]\n"
      "nop\n"
      :
      : [MEM_ADDR] "r" (st_mem_addr), [MASK] "r" (tdata1_mask)
      :
      );
  
  // Back to priv mode
  change_priv_to(hartid, current_priv);
  
  // Do the ST
  __asm__ __volatile__
    (
     "li x31, 0xCAFECAFE\n"
     "sd x31, 0(%[MEM_ADDR])\n"
     "nop\n"
     :
     : [MEM_ADDR] "r" (st_mem_addr), [MASK] "r" (tdata1_mask)
     : "x31"
     );
}

void xcpt_gen_MISALIGNED_LD () {
  volatile uint64_t *unaligned_ESR_addr = (uint64_t*)0x80000001;

  __asm__ __volatile__ 
    ("ld x0, 0(%[addr])\n"
     "fence\n"
     "nop\n"
     :
     : [addr] "r" (unaligned_ESR_addr)
     :
     );


}


void xcpt_gen_MISALIGNED_ST () {
  volatile uint64_t *unaligned_ESR_addr = (uint64_t*)0x80000001;
    __asm__ __volatile__ 
    ("sd x0, 0(%[addr])\n"
     "fence\n"
     "nop\n"
     :
     : [addr] "r" (unaligned_ESR_addr)
     );
}

void xcpt_gen_LD_AF () {
  volatile uint64_t *forbbiden_addr = (uint64_t*)0x0200000000;
  __asm__ __volatile__ 
    ("ld x0, 0(%[addr])\n"
     "fence\n"
     "nop\n"
     :
     : [addr] "r" (forbbiden_addr)
     );

}

void xcpt_gen_ST_AF () {
  volatile uint64_t *forbbiden_addr =(uint64_t*)0x0200000000;
  __asm__ __volatile__ 
    ("sd x0, 0(%[addr])\n"
     "fence\n"
     "nop\n"
     :
     : [addr] "r" (forbbiden_addr)
     );

}

void xcpt_gen_U_ECALL () {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");

}

void xcpt_gen_S_ECALL () {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

void xcpt_gen_M_ECALL () {
  __asm__ __volatile__ ("ecall\n"
                        "nop\n");
}

void xcpt_gen_BUS_ERROR () {
  volatile uint64_t *bus_error_addr = (volatile uint64_t*)0x80000000;
  __asm__ __volatile__ 
    ("sd x0, 0(%[addr])\n"
     "fence\n"
     "nop\n"
     :
     : [addr] "r" (bus_error_addr)
     );
}

void xcpt_gen_M_CODE () {
  __asm__ __volatile__ ("fence.i\n");
  __asm__ __volatile__ ("nop");
}

void xcpt_gen_FETCH_AF() {
  /*
   * Store return address in return_addr[hart_id] before generating the exception
   */
  __asm__ __volatile__
    ("la x31, fetch_af_addr_ret\n"
     "la x30, hart_return_addr\n" // Load store address
     "csrr x29, hartid\n"
     "slli x29, x29, 3\n"    // 8*mhart_id (double word)
     "add x30, x30, x29\n"  // calculate offset
     "sd x31, 0(x30)\n"     // Store the return address
     "fence\n"
     "li x31, 0x200000000\n"
     "jr x31\n"
     "fetch_af_addr_ret:\n"
     "nop\n"
     :
     :
     : "x31", "x30", "x29"
     ); 
}

void xcpt_gen_FETCH_BUS_ERROR() {
  /*
   * Store return address in return_addr[hart_id] before generating the exception
   */
  __asm__ __volatile__ 
    ("la x31, fetch_bus_error_addr_ret\n" // Load return address
     "la x30, hart_return_addr\n" // Load store address
     "csrr x29, hartid\n"
     "slli x29, x29, 3\n"    // 8*mhart_id (double word)
     "add x30, x30, x29\n"  // calculate offset
     "sd x31, 0(x30)\n"     // Store the return address
     "fence\n"
     "li x31, 0x200000000\n"
     "jr x31\n"
     "fetch_bus_error_addr_ret:\n"
     "nop\n"
     :
     :
     : "x31", "x30", "x29"
     );
}

// void xcpt_gen_DEBUG () {
//   // This functions should not be called
//   // Trace encoder is not conected to DCSR.cause (debug xcpt cause)
//   //static_assert(false, "This functions should not be called. Trace encoder is not conected to DCSR.cause (debug xcpt cause)");
//   C_TEST_FAIL;
// }

void (*xcpt_gen_funcs[])() = {
                          XCPT_GEN_FUNC(ILLEGAL_INSTR),
                          XCPT_GEN_FUNC(BREAKPOINT),
                          XCPT_GEN_FUNC(MISALIGNED_LD),
                          XCPT_GEN_FUNC(MISALIGNED_ST),
                          XCPT_GEN_FUNC(LD_AF),
                          XCPT_GEN_FUNC(ST_AF),
                          XCPT_GEN_FUNC(M_ECALL),
                          XCPT_GEN_FUNC(S_ECALL),
                          XCPT_GEN_FUNC(U_ECALL),
                          XCPT_GEN_FUNC(BUS_ERROR),
                          //XCPT_GEN_FUNC(DEBUG),
                          XCPT_GEN_FUNC(M_CODE),
                          XCPT_GEN_FUNC(FETCH_AF),
                          XCPT_GEN_FUNC(FETCH_BUS_ERROR),
};
