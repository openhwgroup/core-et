/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


// Function that returns shire id
unsigned int get_hart_id()
{
    int ret;
    __asm__ __volatile__ (
        "csrr %[ret], mhartid\n"
      : [ret] "=r" (ret)
      : 
      :
    );
    return ret;
}

// Function that returns shire id
unsigned int get_shire_id()
{
  return get_hart_id() >> 7;
}

// Function that returns minion id
unsigned int get_minion_id()
{
  return get_hart_id() >> 1;
}

// functions to deal with CSR regs
unsigned int get_thread_id()
{
  return get_hart_id() & 1;
}


