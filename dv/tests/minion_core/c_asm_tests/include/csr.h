/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


 

#ifndef __CSR_H
#define __CSR_H

inline void __attribute__((always_inline)) csr_write(uint16_t addr, uint64_t val)
{
    __asm__ __volatile__(
        "csrw %[addr],%[val]\n"
        :
        : [addr] "I" (addr), [val] "r" (val)
        :
    );
}

inline uint64_t __attribute__((always_inline)) csr_read(uint16_t addr)
{
  uint64_t ret;
   __asm__ __volatile__ ("csrr %[ret], %[addr]\n"
                         : [ret] "=r" (ret)
                         : [addr] "I" (addr)
                         :
  );
  return ret;
}






#endif // ! __CSR_H
