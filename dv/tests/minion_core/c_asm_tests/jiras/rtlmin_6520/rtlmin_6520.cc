/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"

int main(){
  C_TEST_START;

  __asm__ __volatile__(
     "li x6, 0xf5ec\n"
     "csrrw x0,tensor_mask,x6\n"

     "li x15, 0x9bae0d096e0a097a\n"
     "csrrw x16, tensor_conv_ctrl, x15 /*0x9bae0d096e0a097a*/\n"
  
     "li x22, 0xeeba17c229800ac0\n"
     "csrrw x22, tensor_conv_size, x22 /*0xeeba17c229800ac0*/\n"
  
     "csrr x10, fflags\n"
  
     "li x8, 0xde83\n"
     "add x0, x0, x0\n"
     "add x0, x0, x0\n"
     "add x0, x0, x0\n"
     "add x0, x0, x0\n"
     "add x0, x0, x0\n"
     "csrrw x31, tensor_mask, x8 /*0xde83*/\n"
  );

  C_TEST_PASS;
  return 0;
}
