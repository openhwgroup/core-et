/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


typedef union data_type {
   uint32_t i;
   float f;
} data_type;

inline void write_32b_to_l3(uint32_t* addr, uint32_t data) {

   data_type wr_data;
   wr_data.i = data;
   float offset = 0;

   __asm__ __volatile__ (
      "mov.m.x m0, x0, 0x1\n"
      "fscwg.ps %[wr_data], %[offset](%[addr])\n"
      :
      : [wr_data] "f" (wr_data.f),
        [offset]  "f" (offset),
        [addr]    "r" (addr)
      :
   );
}

inline uint32_t read_32b_from_l3(uint32_t* addr) {

   float offset = 0;
   data_type rd_data;

   __asm__ __volatile__ (
      "mov.m.x m0, x0, 0x1\n"
      "fgwg.ps %[rd_data], %[offset](%[addr])\n"
      : [rd_data] "=f" (rd_data.f)
      : [offset]  "f" (offset),
        [addr]    "r" (addr)
      :
   );

   return rd_data.i;
}
