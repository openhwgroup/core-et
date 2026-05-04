/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

//#define NEIGH_ESRM_ADDR 0x80D00000
//#define NEIGH_ESR_PMU_CTRL (NEIGH_ESRM_ADDR+((0x00D)<<3))
//#define DUMMY_MEM_ADDR 0xFF00000000 // far away region that should not be used
constexpr uint64_t DUMMY_MEM_ADDR = 0x00FF00000000; // far away region that should not be used
uint64_t dummy_memory_space;
// Routine with constant number of instructions



inline void __attribute__((always_inline)) setup_const_instr_routine() {
	uint64_t *addr = (uint64_t*) DUMMY_MEM_ADDR;
	*addr = 12;
}

/*
 * Constant instructions routine
 */
#define CONST_INSTR_ROUTINE_NINST (11)
inline void __attribute__((always_inline)) const_instr_routine() {
  uint64_t addr;
  __asm__ __volatile__ (
      "nop\n"
      "nop\n"
      "la %[addr], dummy_memory_space\n"
      "nop\n"
      "nop\n"
      "ld x15, (%[addr])\n"
      "fence\n"
      "addi x15, x15, 0xDA\n"
      "sd x15, (%[addr])\n"
      "nop\n"
      :[addr] "=r" (addr)
      :[iaddr] "i" (DUMMY_MEM_ADDR)
      : "memory", "x15"
   );
}

/*
 * Constant instructions routine diferent for each thread
 */
#define CONST_INSTR_ROUTINE_TH0_NINST (11+9)
#define CONST_INSTR_ROUTINE_TH1_NINST (11+7)
#define CONST_INSTR_ROUTINE_TH_NINST (CONST_INSTR_ROUTINE_TH1_NINST+CONST_INSTR_ROUTINE_TH0_NINST)

inline void __attribute__((always_inline)) const_instr_routine_th() {
  uint64_t addr;
  __asm__ __volatile__ (
      "nop\n"
      "nop\n"
      "la %[addr], dummy_memory_space\n"
      "nop\n"
      "nop\n"
      "ld x15, (%[addr])\n"
      "fence\n"
      "addi x15, x15, 0xDA\n"
      "sd x15, (%[addr])\n"
      "nop\n"
      "csrr x15, mhartid\n"
      "andi x15, x15, 0x1\n"
      "beqz x15, THREAD0_CODE\n"
      "THREAD1_CODE:\n"
      "nop\n"
      "nop\n"
      "nop\n"
      "j END_ROUTINE\n"
      "THREAD0_CODE:\n"
      "nop\n"
      "nop\n"
      "nop\n"
      "nop\n"
      "nop\n"
      "nop\n"
      "END_ROUTINE:\n"
      :[addr] "=r" (addr)
      :[iaddr] "i" (DUMMY_MEM_ADDR)
      : "memory", "x15"
   );
}
