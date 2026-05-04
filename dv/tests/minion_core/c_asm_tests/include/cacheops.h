/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __CACHEOPS_H
#define __CACHEOPS_H

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#include <cinttypes>
#else
#include <inttypes.h>
#endif

enum cop_dest {
   to_L1  = 0x0ULL,
   to_L2  = 0x1ULL,
   to_L3  = 0x2ULL,
   to_Mem = 0x3ULL
};

#if !defined(__cplusplus)
typedef
#endif
enum l1d_mode {
   l1d_shared,
   l1d_split,
   l1d_scp
}
#if !defined(__cplusplus)
l1d_mode
#endif
;

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: evict_sw
//
//   This function evicts the specified set and way from the cache hierarchy, up to the provided
//   destination. Optionally, a repeat count can be specified to evict more adjacent cache lines.
//   Optionally, each potential line eviction can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) evict_sw(uint64_t use_tmask, uint64_t dst, uint64_t way, uint64_t set, uint64_t num_lines = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) evict_sw(uint64_t use_tmask, uint64_t dst, uint64_t way, uint64_t set, uint64_t num_lines, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x73FFFFFFFFFC3F30ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((dst       & 0x3                   ) << 58 ) |
                      ((set       & 0xF                   ) << 14 ) |
                      ((way       & 0x3                   ) << 6  ) |
                      ((num_lines & 0xF                   )       ) ;

   __asm__ __volatile__ ( "csrw 0x7f9, %[csr_enc]\n" : : [csr_enc] "r" (csr_enc));

}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: flush_sw
//
//   This function writes back the specified set and way up to the provided cached level, if the
//   line is dirty. Optionally, a repeat count can be specified to flush more adjacent cache lines.
//   Optionally, each potential line flush can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) flush_sw(uint64_t use_tmask, uint64_t dst, uint64_t way, uint64_t set, uint64_t num_lines = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) flush_sw(uint64_t use_tmask, uint64_t dst, uint64_t way, uint64_t set, uint64_t num_lines, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x73FFFFFFFFFC3F30ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((dst       & 0x3                   ) << 58 ) |
                      ((set       & 0xF                   ) << 14 ) |
                      ((way       & 0x3                   ) << 6  ) |
                      ((num_lines & 0xF                   )       ) ;

   __asm__ __volatile__ ( "csrw 0x7fb, %[csr_enc]\n" : : [csr_enc] "r" (csr_enc));
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: lock_sw
//
//   This function locks the provided physical address to the specified way, unless the way is
//   already locked to another line, or 3 ways are already locked for that set, or the provided
//   physical address is already present in a different way.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) lock_sw(uint64_t way, uint64_t paddr, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) lock_sw(uint64_t way, uint64_t paddr, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl  & 0x807FFF000000003FULL )       ) |
                      ((way   & 0x3                   ) << 55 ) |
                      ((paddr & 0xFFFFFFFFC0ULL       )       ) ;

   __asm__ __volatile__ ( "csrw 0x7fd, %[csr_enc]\n" : : [csr_enc] "r" (csr_enc));
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: unlock_sw
//
//   This function unlocks the provided set and way
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) unlock_sw(uint64_t way, uint64_t set, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) unlock_sw(uint64_t way, uint64_t set, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl & 0x807FFFFFFFFFFC3FULL )       ) |
                      ((way  & 0xFF                  ) << 55 ) |
                      ((set  & 0xF                   ) << 6  );

   __asm__ __volatile__ ( "csrw 0x7ff, %[csr_enc]\n" : : [csr_enc] "r" (csr_enc));
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: evict_va
//
//   This function evicts the specified virtual address from the cache hierarchy, up to the provided
//   cache level. Optionally, a repeat count can be specified to evict more lines, whose addresses
//   are calculated using the provided stride.
//   Optionally, each potential line eviction can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) evict_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) evict_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x73FF000000000030ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((dst       & 0x3                   ) << 58 ) | //00=L1, 01=L2, 10=L3, 11=MEM
                      ((addr      & 0xFFFFFFFFFFC0ULL     )       ) |
                      ((num_lines & 0xF                   )       ) ;

   uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      "csrw 0x89f, %[csr_enc]\n"
      :
      : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: flush_va
//
//   This function flushes the specified virtual address from the cache hierarchy, if it is present
//   and dirty, up to the provided cache level. Optionally, a repeat count can be specified to flush
//   more lines, whose addresses are calculated using the provided stride.
//   Optionally, each potential line flush can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) flush_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) flush_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x73FF000000000030ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((dst       & 0x3                   ) << 58 ) |
                      ((addr      & 0xFFFFFFFFFFC0ULL     )       ) |
                      ((num_lines & 0xF                   )       ) ;

   uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      "csrw 0x8bf, %[csr_enc]\n"
      :
      : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: prefetch_va
//
//   This function prefetches the provided virtual address to the specified cache level.
//   Optionally, a repeat count can be provided to pretech more lines, whose addresses are
//   calculated using the provided stride.
//   Optionally, each line prefetch can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) prefetch_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) prefetch_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x73FF000000000030ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((dst       & 0x3                   ) << 58 ) |
                      ((addr      & 0xFFFFFFFFFFC0ULL     )       ) |
                      ((num_lines & 0xF                   )       ) ;

   uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      "csrw 0x81f, %[csr_enc]\n"
      :
      : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: lock_va
//
//   This function soft-locks the provided virtual address in the L1, meaning that it will never
//   be chosen for line replacement if other non-locked lines are present in the same set.
//   Optionally, a repeat count can be provided to soft-lock more lines, whose addresses are
//   calculated using the provided stride.
//   Optionally, each line lock can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) lock_va(uint64_t use_tmask, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) lock_va(uint64_t use_tmask, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x7FFF000000000030ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((addr      & 0xFFFFFFFFFFC0ULL     )       ) |
                      ((num_lines & 0xF                   )       ) ;

   uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      "csrw 0x8df, %[csr_enc]\n"
      :
      : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: unlock_va
//
//   This function removes the soft-lock of the provided virtual address.
//   Optionally, a repeat count can be provided to unlock more lines, whose addresses are
//   calculated using the provided stride.
//   Optionally, each unlock can be gated by the value of the TensorMask CSR.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) unlock_va(uint64_t use_tmask, uint64_t addr, uint64_t num_lines = 0, uint64_t stride = 0, uint64_t id = 0, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) unlock_va(uint64_t use_tmask, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0x7FFF000000000030ULL )       ) |
                      ((use_tmask & 1                     ) << 63 ) |
                      ((addr      & 0xFFFFFFFFFFC0ULL     )       ) |
                      ((num_lines & 0xF                   )       ) ;

   uint64_t x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
      "add x31, zero, %[x31_enc]\n"
      "csrw 0x8ff, %[csr_enc]\n"
      :
      : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: cache_invalidate
//
//   This function invalidates various cache structures used by the minion core.
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) cache_invalidate(uint64_t inval_instr_cache, uint64_t inval_TLBs_and_PTW, int64_t warl = 0)
#else
inline void __attribute__((always_inline)) cache_invalidate(uint64_t inval_instr_cache, uint64_t inval_TLBs_and_PTW, int64_t warl)
#endif
{
   uint64_t csr_enc = ((warl      & 0xFFFFFFFFFFFFFFFCULL )       ) |
                      ((inval_TLBs_and_PTW & 1            )       ) |
                      ((inval_instr_cache & 1             ) << 1  ) ;

   __asm__ __volatile__ (
      "csrw 0x7d0, %[csr_enc]\n"
      :
      : [csr_enc] "r" (csr_enc)
      : 
   );
}


//
// READS CACHE INVALIDATE
//
inline uint64_t __attribute__((always_inline)) get_cache_invalidate()
{
   uint64_t csr_enc;
   __asm__ __volatile__ (
      "csrr %[csr_enc], 0x7d0\n"
      : [csr_enc] "=r" (csr_enc)
      :
      :
   );
   return csr_enc;
}


//
// MCACHE_CONTROL
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) mcache_control(uint64_t d1_split, uint64_t scp_en, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) mcache_control(uint64_t d1_split, uint64_t scp_en, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl     & 0xFFFFFFFFFFFFFFFCULL )      ) |
                      ((scp_en   & 0x1                   ) << 1 ) |
                      ((d1_split & 0x1                   ) << 0 ) ;

   __asm__ __volatile__ (
      "csrw 0x7e0, %[csr_enc]\n"
      :
      : [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


//
// UCACHE_CONTROL
//
#if defined( __cplusplus) //THIS GUARD IS INTRODUCED TO HAVE FUNCTIONS ARGUMENTS COMPATIBLE WITH C COMPILER WHICH IS USED FOR IOSHIRE SW (SERVICE PROCESSOR)
inline void __attribute__((always_inline)) ucache_control(uint64_t scp_en, uint64_t cacheop_rate, uint64_t cacheop_max, uint64_t warl = 0)
#else
inline void __attribute__((always_inline)) ucache_control(uint64_t scp_en, uint64_t cacheop_rate, uint64_t cacheop_max, uint64_t warl)
#endif
{
   uint64_t csr_enc = ((warl         & 0xFFFFFFFFFFFFF821ULL )      ) |
                      ((cacheop_max  & 0x1F                  ) << 6 ) |
                      ((cacheop_rate & 0x7                   ) << 2 ) |
                      ((scp_en       & 0x1                   ) << 1 ) ;

   __asm__ __volatile__ (
      "csrw 0x810, %[csr_enc]\n"
      :
      : [csr_enc] "r" (csr_enc)
      : "x31"
   );
}


inline l1d_mode __attribute__((always_inline)) get_l1d_mode()
{
   uint64_t csr_enc;
   __asm__ __volatile__ (
      "csrr %[csr_enc], 0x810\n"
      : [csr_enc] "=r" (csr_enc)
      :
      :
   );
   return ((csr_enc & 0x3) == 0x3) ? l1d_scp   :
          ((csr_enc & 0x3) == 0x1) ? l1d_split :
                                     l1d_shared;
}


inline void __attribute__((always_inline)) excl_mode(uint64_t val)
{
   __asm__ __volatile__ (
      "csrw 0x7d3, %[csr_enc]\n"
      :
      : [csr_enc] "r" (val)
      : "x31"
   );
}


inline void __attribute__((always_inline)) scp(uint64_t warl, uint64_t DEscratchpad)
{

    // Hard partition L1 Data cache between the harts
    //mcache_control(0,0,0);
    FENCE;
    WAIT_CACHEOPS;

    // Enable scratchpad
    uint64_t csr_enc = ((warl & 0x7FFFFFFFFFFFFFFF) << 1) |
                       ((DEscratchpad & 0x1));

   __asm__ __volatile__ (
      "csrw 0x810, %[csr_enc]\n"
      :
      : [csr_enc] "r" (csr_enc)
      : "x31"
   );
}

#endif // ! __CACHEOPS_H
