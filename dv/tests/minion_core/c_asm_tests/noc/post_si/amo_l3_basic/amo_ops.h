/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _AMO_OPS_
#define _AMO_OPS_

inline void amo_write(uint64_t *addr, uint64_t write_data) {
   __asm__ __volatile__(
      "amoswapg.d x0, %[write_data], (%[addr])\n"
      :
      : [write_data] "r" (write_data),
        [addr] "r" (addr)
      :
   );
}

inline uint64_t amo_read(uint64_t *addr) {
   uint64_t read_data;
   __asm__ __volatile__(
      "amoorg.d %[read_data], x0, (%[addr])\n"
      : [read_data] "=r" (read_data)
      : [addr] "r" (addr)
      :
   );
   return read_data;
}

#endif
