/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */



#define LONG_CALL(symbol)                           \
  do {                                              \
    static uintptr_t __ad = (uintptr_t)symbol;      \
    static void (*__fn)() = (void (*)()) __ad ;     \
    __fn();                                         \
  }while(0)


#define CAUSE_BREAKPOINT 3

// the following constant have to match linker.ld
#define DRAM_END 0x8400000000
#define BREAKPOINT_AD (DRAM_END - 2)
#define TEST_INSTRUCTIONS  32


#ifdef __cplusplus
extern "C" 
#endif
__attribute__ ((aligned (1<<12))) __attribute__ ((interrupt))  void trap_handler();
__attribute__ ((naked)) __attribute__ ((section (".hi_text"))) void test_code();
