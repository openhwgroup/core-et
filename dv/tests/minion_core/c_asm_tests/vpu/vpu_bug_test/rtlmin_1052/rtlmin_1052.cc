/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
int main() {

   __asm__ __volatile__ (
         "frcp.ps f8, f6\n"            
         "fnot.pi f1, f0\n"          
         "fnot.pi f1, f0\n"      
         "mova.m.x x5\n"
         : : : "f0", "f1", "f6", "f8"
         );
   C_TEST_PASS;
   return 0;
}
