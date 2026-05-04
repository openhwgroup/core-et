/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

#include "computational.h"
#include "compare.h"
#include "trans.h"

int main()
{

   C_TEST_START;


   // Test
   static uint32_t operands[256]  __attribute__ ((aligned (16)))= {
     0xBE7FFDFC,
     0x5F800001,
     0x40005FFF,
     0x007FF7FF,
     0x5FFFFFFF,
     0x41FFF3FE,
     0x397F5FFE,
     0x5FFFFFFE,
     0x7F01FDFF,
     0x0103BFFF,
     0x7E800000,
     0xFD00027F,
     0xDEFFFF20,
     0x7E800001,
     0x40E7FFFF,
     0x817459FF,
     0x7EFFFFFF,
     0x4BBBFFFE,
     0x33801FFC,
     0x7EFFFFFE,
     0xBDC0003F,
     0xBE2CFE4C,
     0x7F000000,
     0xB8400080,
     0x2608001F,
     0x7F000001,
     0x4E8047FE,
     0xB436F03E,
     0x7F7FFFFF,
     0x44FFAFFE,
     0xCF0000FA,
     0x7F7FFFFE,
     0xC58FFFDE,
     0x4041C35D,
     0x7F800000,
     0xAB1C89BB,
     0x3E18EECC,
     0x7F800001,
     0xC6810200,
     0x33800FFC,
     0x7FFFFFFF,
     0x0006274F,
     0xDE7FC1FF,
     0x7FFFFFFE,
     0xFEF80400,
     0xB8FFFEFF,
     0x80000000,
     0xB383FBFF,
     0xBE717FC3,
     0x80000001,
     0x00FFFFCF,
     0xCF000010,
     0x807FFFFF,
     0xDFF384EB,
     0xCF00C000,
     0x807FFFFE,
     0xDF8F0000,
     0xBF80DFFF,
     0x80800000,
     0x41867F7C,
     0x5700002F,
     0x80800001,
     0xBF7F00FF,
     0xBC1D9886,
     0x80FFFFFF,
     0x3EEFFEFE,
     0xDE0001FC,
     0x80FFFFFE,
     0x67FFFFBA,
     0x4B820000,
     0x81000000,
     0x7FFFF9FF,
     0x3381BFFF,
     0x81000001,
     0x91FFB000,
     0xBD4A82A6,
     0x817FFFFF,
     0x3F00000A,
     0x33801FE0,
     0x817FFFFE,
     0x3C808400,
     0x33FFD800,
     0xB3800000,
     0xBEA00FFF,
     0xBEFFFEFB,
     0xB3800001,
     0x3F71F5EF,
     0xBC5FFE00,
     0xB3FFFFFF,
     0xBFF48022,
     0x3AEC60E1,
     0xB3FFFFFE,
     0xCE803FEF,
     0x4B008040,
     0xBD800000,
     0x403FFFDF,
     0x7EFFFFB0,
     0xBD800001,
     0xC7FFEC00,
     0xBE203FFE,
     0xBDFFFFFF,
     0xC1C72FEE,
     0xC0000FDF,
     0xBDFFFFFE,
     0x0036476E,
     0xC0472034,
     0xBE000000,
     0xFE80000F,
     0xCEF7FFFF,
     0xBE000001,
     0xBF87BFFE,
     0x4F40FFFE,
     0xBE7FFFFF,
     0xC2FFFFEC,
     0x8081FFFB,
     0xBE7FFFFE,
     0x4EAB026C,
     0xC8FDFFFB,
     0xBE800000,
     0x3DF77FFF,
     0xCBB08E9D,
     0xBE800001,
     0xBDFFEFEE,
     0xBE900007,
     0xBEFFFFFF,
     0x0167FFFF,
     0xBB6FFFBF,
     0xBEFFFFFE,
     0xBEFFE080,
     0xBFFDFEFE,
     0xBF000000,
     0x5E8000E0,
     0xDEFFFF6F,
     0xBF000001,
     0xDFFFFFFC,
     0xCE9FFFDF,
     0xBF7FFFFF,
     0x7E80FFF0,
     0xB17FFBFB,
     0xBF7FFFFE,
     0xDEFFFF80,
     0xFE804020,
     0xBF800000,
     0x5AFFFC00,
     0x3F8FFF00,
     0xBF800001,
     0xDE220C03,
     0x3F002FFF,
     0xBFFFFFFF,
     0xCE7D5EA5,
     0xBE0536FD,
     0xBFFFFFFE,
     0xA9801BFF,
     0x2A9455AC,
     0xC0000000,
     0x7E803DFF,
     0x3E00FFF6,
     0xC0000001,
     0x1DAA0123,
     0xCBA0FFFF,
     0xC07FFFFF,
     0xF8400000,
     0xDEF7FFF0,
     0xC07FFFFE,
     0x7F007EFF,
     0x7900F800,
     0xC0800000,
     0x80FFFFFE,
     0x407BFFBF,
     0xC0800001,
     0x5F100001,
     0xD2697F4B,
     0xC0FFFFFF,
     0x95FF8040,
     0xBF54EA37,
     0xC0FFFFFE,
     0x7F22C563,
     0x437BFFFE,
     0xC1000000,
     0xFC81007F,
     0x4D8E33CE,
     0xC1000001,
     0xBFF0007F,
     0xFE812000,
     0xC17FFFFF,
     0x409FFF80,
     0xFFDAD633,
     0xC17FFFFE,
     0x3FDFFFBF,
     0xB27FE800,
     0xC1800000,
     0x468000C0,
     0xCBBE639E,
     0xC1800001,
     0xFEFFE03E,
     0xCB80037F,
     0xC1FFFFFF,
     0x481FEFFF,
     0xCE83FFFF,
     0xC1FFFFFE,
     0x5F000028,
     0xC2600003,
     0xCB800000,
     0xC01E1693,
     0x20759558,
     0xCB800001,
     0xBE84003F,
     0x551FFFFE,
     0xCBFFFFFF,
     0xBF2B4CD4,
     0x5ECC6A0F,
     0xCBFFFFFE,
     0x4EF7FF7E,
     0x5E3A71F9,
     0xCE000000,
     0x40BD29B7,
     0xBF00007C,
     0xCE000001,
     0xC17C13AD,
     0xEB77FBFF,
     0xCE7FFFFF,
     0x3D900008,
     0xED23EF33,
     0xCE7FFFFE,
     0xB4FFDE00,
     0xBC805FFE,
     0xCE800000,
     0x3E2BBE4E,
     0x2C82FFFF,
     0xCE800001,
     0x3DFFFF80,
     0x7E87FDFF,
     0xCEFFFFFF,
     0x5E7FFBFB,
     0xBD808003,
     0xCEFFFFFE,
     0x3FBB6EFD,
     0xCFFFFF01,
     0xCF000000,
     0x3D000028,
     0xC1C5E02D,
     0xCF000001,
     0xFFF19F12,
     0x41000076,
     0xCF7FFFFF,
     0x4FE061BA,
     0xFB080004,
     0xCF7FFFFE,
     0xFEFFFF7E,
     0x3C6426F6,
     0xCF800000,
     0x7CFDEFFE,
     0xC21003FE,
     0xCF800001,
     0xC5FFF9FF,
     0xAC00081E

   };

   __asm__ __volatile__ (

         //enable lanes
         "li x5, 0xff\n"
         "mova.m.x x5\n"

         //load operands
         "flw.ps f0,  0 (%[operands])\n"
	 "flw.ps f1,  32 (%[operands])\n"
	 "flw.ps f2,  64 (%[operands])\n"
	 "flw.ps f3,  96 (%[operands])\n"
	 "flw.ps f4,  128 (%[operands])\n"
	 "flw.ps f5,  160 (%[operands])\n"
	 "flw.ps f6,  192 (%[operands])\n"
	 "flw.ps f7,  224 (%[operands])\n"
	 "flw.ps f8,  256 (%[operands])\n"
	 "flw.ps f9,  288 (%[operands])\n"
	 "flw.ps f10,  320 (%[operands])\n"
	 "flw.ps f11,  352 (%[operands])\n"
	 "flw.ps f12,  384 (%[operands])\n"
	 "flw.ps f13,  416 (%[operands])\n"
	 "flw.ps f14,  448 (%[operands])\n"
	 "flw.ps f15,  480 (%[operands])\n"
	 "flw.ps f16,  512 (%[operands])\n"
	 "flw.ps f17,  544 (%[operands])\n"
	 "flw.ps f18,  576 (%[operands])\n"
	 "flw.ps f19,  608 (%[operands])\n"
	 "flw.ps f20,  640 (%[operands])\n"
	 "flw.ps f21,  672 (%[operands])\n"
	 "flw.ps f22,  704 (%[operands])\n"
	 "flw.ps f23,  736 (%[operands])\n"
	 "flw.ps f24,  768 (%[operands])\n"
	 "flw.ps f25,  800 (%[operands])\n"
	 "flw.ps f26,  832 (%[operands])\n"
	 "flw.ps f27,  864 (%[operands])\n"
	 "flw.ps f28,  896 (%[operands])\n"
	 "flw.ps f29,  928 (%[operands])\n"
	 "flw.ps f30,  960 (%[operands])\n"
	 "flw.ps f31,  992 (%[operands])\n"

          

         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x5", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );

          __computational("fadd.ps",2,1,0);
          __computational("fmin.s",31,1,0);
          __computational("fadd.pi",16,15,14);
          __computational4("fmadd.ps",12,11,10,9);
          __computational("fmax.ps",26,11,10);
          __computational4("fmsub.ps",15,14,13,12);
          __computational("fmul.pi",31,3,2);
          __computational("fmul.ps",31,9,8);
          __computational("fmulh.pi",10,9,8);
          __computational("fmulhu.pi",20,19,18);
          __computational4("fnmadd.ps",31,24,23,22);
          __computational4("fnmadd.ps",31,22,21,20);
          __computational("fsub.pi",14,13,12);
          __computational("fsub.ps",6,5,4);
          __compare("feq.ps",20,19,18);
          __compareint("feq.s",3,23,22);
          __comparemask("feqm.ps",1,26,25);
          __compare("fle.ps",28,27,26);
          __compareint("fle.s",6,23,23);
          __comparemask("flem.ps",1,23,22);
          __compare("flt.ps",9,8,7);
          __compareint("flt.s",3,3,2);
          __comparemask("fltm.ps",1,9,8);
          __computational("fadd.ps",2,1,0);
          __computational("fmin.s",31,1,0);
          __computational("fadd.pi",16,15,14);
          __computational4("fmadd.ps",12,11,10,9);
          __computational("fmax.ps",26,11,10);
          __computational4("fmsub.ps",15,14,13,12);
          __computational("fmul.pi",31,3,2);
          __computational("fmul.ps",31,9,8);
          __computational("fmulh.pi",10,9,8);
          __computational("fmulhu.pi",20,19,18);
          __computational4("fnmadd.ps",31,24,23,22);
          __computational4("fnmadd.ps",31,22,21,20);
          __computational("fsub.pi",14,13,12);
          __computational("fsub.ps",6,5,4);
          __compare("feq.ps",20,19,18);
          __compareint("feq.s",3,23,22);
          __comparemask("feqm.ps",1,26,25);
          __compare("fle.ps",28,27,26);
          __compareint("fle.s",6,23,23);
          __comparemask("flem.ps",1,23,22);
          __compare("flt.ps",9,8,7);
          __compareint("flt.s",3,3,2);
          __comparemask("fltm.ps",1,9,8);
          __computational("fadd.ps",2,1,0);
          __computational("fmin.s",31,1,0);
          __computational("fadd.pi",16,15,14);
          __computational4("fmadd.ps",12,11,10,9);
          __computational("fmax.ps",26,11,10);
          __computational4("fmsub.ps",15,14,13,12);
          __computational("fmul.pi",31,3,2);
          __computational("fmul.ps",31,9,8);
          __computational("fmulh.pi",10,9,8);
          __computational("fmulhu.pi",20,19,18);
          __computational4("fnmadd.ps",31,24,23,22);
          __computational4("fnmadd.ps",31,22,21,20);
          __computational("fsub.pi",14,13,12);
          __computational("fsub.ps",6,5,4);
          __compare("feq.ps",20,19,18);
          __compareint("feq.s",3,23,22);
          __comparemask("feqm.ps",1,26,25);
          __compare("fle.ps",28,27,26);
          __compareint("fle.s",6,23,23);
          __comparemask("flem.ps",1,23,22);
          __compare("flt.ps",9,8,7);
          __compareint("flt.s",3,3,2);
          __comparemask("fltm.ps",1,9,8);
          //__trans_no_r("flog.ps",3,4)
          //__trans_no_r("fexp.ps",7,3)
          //__trans_no_r(//RTLMIN-5479"fsin.ps",3,12)
          //__trans_no_r(//RTLMIN-5479"fsin.ps",8,6)
          //__trans_no_r("ffrc.ps",9,10)
          //__trans_no_r(//RTLMIN-5479"frsqrt.ps",11,10)

   C_TEST_PASS;
   return 0;
}
