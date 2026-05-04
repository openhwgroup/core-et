/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _MSTATUS_FS_H_
#define _MSTATUS_FS_H_

#define ILLEGAL_CAUSE 2
#define ILLEGAL_INST __asm__ __volatile__(".word 0\n")

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

uint8_t* get_rand_byte_ptr(unsigned &ptr, unsigned alignment = 32);
uint8_t get_rand_byte(unsigned &ptr);
void randomize_fregs(unsigned &ptr);
inline void set_mask(uint8_t m);
inline void delay();
inline void set_debug(uint64_t d);
inline void clear_fs();
void evict_l1();
EXTERN_C void  __attribute__ ((naked)) __attribute__((aligned(4096)))  mtrap_handler();
EXTERN_C void  __attribute__ ((interrupt)) mtrap_handler_func();

#endif
