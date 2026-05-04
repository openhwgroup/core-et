/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>

int main()
{

  static uint32_t operands[16]  __attribute__ ((aligned (16)))= {
    0x11111111,
    0xF0F0F0F0,
    0x00000000,
    0x0F0F0F0F,
    0x01010101,
    0xFFFFFFFF,
    0xFFFF0000,
    0x0000FFFF,
    0xFF00FF00,
    0x00FF00FF,
    0x10101010,
    0xFFFFFFFF,
    0x00000000,
    0xFF000000,
    0x000000FF,
    0x000FF000
   };

  C_TEST_START;


for (int i=0; i<12; i++) {
  switch (i) {
     case 0:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0xaa\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 1:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x55\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 2:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0xff\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 3:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0xf0\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 4:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x0f\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 5:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0xcc\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 6:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x33\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 7:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x01\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 8:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x06\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 9:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x60\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 10:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x80\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
     case 11:   
    __asm__ __volatile__ (
    //enable lanes
    "li x11, 0x18\n"
    "mova.m.x x11\n"
    :
    :
    :"x11"
    );
    break;
    }

  __asm__ __volatile__ (
  
    //load operands
    "flw.ps f0, 0(%[operands])\n"
    "flw.ps f1, 32(%[operands])\n"
    "flw.ps f2, 16(%[operands])\n"
    "flw.ps f4, 24(%[operands])\n"
    "flw.ps f6, 4(%[operands])\n"
    "flw.ps f8, 0(%[operands])\n"

    "fandi.pi f3, f0, 0x1ff\n"

    "fandi.pi f3, f0, 0x00\n"

    "fandi.pi f5, f1, 0x155\n"


    "fandi.pi f5, f6, 0x111\n"

    "fandi.pi f5, f8, 0x123\n"


  // Asm output results to C code
  : 
  // Asm input data from C code
  : [operands] "r" (operands)
  // Registers used in Asm so C knows they change
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "x11"
  );
}

C_TEST_PASS;
  return 0;
}
