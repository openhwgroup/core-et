/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "cacheops.h"

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#include <cinttypes>
#else
#include <inttypes.h>
#endif

#define ET_PORT0_SCSR   0x9cc
#define ET_PORT0_UCSR   0x8cc
#define ET_PORT0_HEAD   0xcc8

#define DIV_CEIL(a,b) (( (a)/(b) ) + ((a) % (b) > 0 ? 1 : 0 ))

inline __attribute__((always_inline)) void setup_rx_port(unsigned port_id, bool user_mode, unsigned log2size, unsigned maxMsgs, void *buf, unsigned way, unsigned oob_en)
{
   unsigned nr_lines = DIV_CEIL((1 << log2size) * maxMsgs, 64);

   // Evict from L1 then lock the line
   for (unsigned i = 0; i < nr_lines; i++) {
      evict_va(false, to_L2, ((uint64_t)buf) + 64*i);
      lock_sw((uint64_t)way, ((uint64_t)buf) + 64*i);
   }

   uint64_t scp_set = ((uint64_t) buf >> 6) & 0xF;

   uint64_t port_conf =
      ((user_mode   & 0x1 ) << 4  ) |
      ((log2size    & 0x7 ) << 5  ) |
      (((maxMsgs-1) & 0xF ) << 8  ) |
      (1                    << 15 ) | // useScp
      ((scp_set     & 0xFF) << 16 ) |
      ((way         & 0xFF) << 24 ) |
      ((oob_en      & 0x1 ) << 1  ) |
      (0x1);

   switch(port_id){
      case 0:
         __asm__ __volatile__ ( "csrw %[csr], %[port_conf]" : : [port_conf] "r" (port_conf), [csr] "i" (ET_PORT0_SCSR + 0) : "t0" );
         break;
      case 1:
         __asm__ __volatile__ ( "csrw %[csr], %[port_conf]" : : [port_conf] "r" (port_conf), [csr] "i" (ET_PORT0_SCSR + 1) : "t0" );
         break;
      case 2:
         __asm__ __volatile__ ( "csrw %[csr], %[port_conf]" : : [port_conf] "r" (port_conf), [csr] "i" (ET_PORT0_SCSR + 2)  : "t0");
         break;
      default:
         __asm__ __volatile__ ( "csrw %[csr], %[port_conf]" : : [port_conf] "r" (port_conf), [csr] "i" (ET_PORT0_SCSR + 3)  : "t0");
         break;
   }
}

inline __attribute__((always_inline)) uint64_t read_dword(uint32_t offset, void *ptr, uint8_t ld_size)
{
   uint64_t rcv_data = 0;
   switch (ld_size)
   {
      case 0:
         __asm__ __volatile__ (
               "add a5, %[off], %[buff]\n\t"
               "ld  %[result], 0(a5)\n\t"
               "bne %[result], zero, %=f\n\t"
               "lui a7, 0x50bad\n\t"
               "csrw 0x8d0, a7\n\t" // write to validation0
               "wfi\n\t"
               "%=: \n\t"
               : [result] "=r" (rcv_data)
               : [buff] "r" (ptr), [off] "r" (offset)
               : "a5", "a7"
               );
         break;
      case 1:
         __asm__ __volatile__ (
               "add a5, %[off], %[buff]\n\t"
               "lw  %[result], 0(a5)\n\t"
               "lw  a7, 4(a5)\n\t"
               "slli a5, a7, 32\n\t"
               : [result] "=r" (rcv_data)
               : [buff] "r" (ptr), [off] "r" (offset)
               : "a5", "a7"
               );
         break;
   }

   return rcv_data;
}

inline __attribute__((always_inline)) uint32_t read_port_status(unsigned port_id) {

   uint32_t offset = 0;
   switch (port_id)
   {
      case 0:
         __asm__ __volatile__ ("csrr %[n], %[csr]\n" : [n] "=r" (offset) : [csr] "i" (ET_PORT0_HEAD + 0) ); break;
      case 1:
         __asm__ __volatile__ ("csrr %[n], %[csr]\n" : [n] "=r" (offset) : [csr] "i" (ET_PORT0_HEAD + 1) ); break;
      case 2:
         __asm__ __volatile__ ("csrr %[n], %[csr]\n" : [n] "=r" (offset) : [csr] "i" (ET_PORT0_HEAD + 2) ); break;
      case 3:
         __asm__ __volatile__ ("csrr %[n], %[csr]\n" : [n] "=r" (offset) : [csr] "i" (ET_PORT0_HEAD + 3) ); break;
   }

   return offset;
}

inline __attribute__((always_inline)) void send_dword(uint64_t shire, uint64_t mid, uint64_t hid, uint64_t nid, uint64_t pid, uint64_t buff, uint8_t st_size)
{
   volatile uint64_t temp;

   temp = 0x80000800  |
          (shire << 22) |
          (mid   << 13) |
          (nid   << 16) |
          (hid   << 12) |
          (pid   << 6 ) ;

   switch (st_size)
   {
      case 0: __asm__ __volatile__ ( "mv  a5, %[port_addr] \n\t"
                    "sd  %[data], 0(a5)\n\t"
                    :
                    : [port_addr] "r" (temp),
                      [data] "r" (buff)
                    : "memory", "a5"
                    );
              break;
      case 1:
              __asm__ __volatile__ ( "mv   a5, %[port_addr] \n\t"
                    "srai a6, %[data], 32\n\t"
                    "sw   %[data], 0(a5)\n\t"
                    "sw   a6, 0(a5)\n\t"
                    :
                    : [port_addr] "r" (temp),
                    [data] "r" (buff)
                    : "memory", "a5", "a6"
                    );
              break;
   }
}

