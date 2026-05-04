/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main() {

	C_TEST_START;

	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out

	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {
                         0xC06300C7,
                         0x5F800001,
                         0xBE7FC00F,
                         0x3FDCC257,
                         0x5FFFFFFF,
                         0xFEBFFFFF,
                         0xDE0FFEFF,
                         0x5FFFFFFE,
                         0xD237FFFE,
                         0x4F214EC3,
                         0x7E800000,
                         0xC16007FF,
                         0xBCF8B267,
                         0x7E800001,
                         0xA9FFEFEF,
                         0x3E8007FD,
                         0x7EFFFFFF,
                         0xBB7C0002,
                         0xB9FBFFEF,
                         0x7EFFFFFE,
                         0x36000001,
                         0xBFFDFFDE,
                         0x7F000000,
                         0x5F000011,
                         0x4102FFFE,
                         0x7F000001,
                         0xBD820100,
                         0x410005FF,
                         0x7F7FFFFF,
                         0xB3FFE002,
                         0x80BEE891,
                         0x7F7FFFFE,
                         0xC17FFA00,
                         0x4FE8403B,
                         0x7F800000,
                         0xDE932098,
                         0x9D800803,
                         0x7F800001,
                         0x7E820006,
                         0xCE7FFBFF,
                         0x7FFFFFFF,
                         0x40F7E298,
                         0xC1800004,
                         0x7FFFFFFE,
                         0xC5FBFFF8,
                         0x1C452E76,
                         0x80000000,
                         0x7FFF000E,
                         0xB5FFBFF7,
                         0x80000001,
                         0x5E8000FD,
                         0x42A33BD3,
                         0x807FFFFF,
                         0x377FF003,
                         0xDF801F00,
                         0x807FFFFE,
                         0xA3600400,
                         0xC0FF2000,
                         0x80800000,
                         0x331C0000,
                         0xC00003FC,
                         0x80800001,
                         0xC4C0FED0,
                         0x627FDF7F,
                         0x80FFFFFF,
                         0x3E7807FE,
                         0xBF000005,
                         0x80FFFFFE,
                         0xC3FFDFFC,
                         0xBF77FFFB,
                         0x81000000,
                         0xBC7FC007,
                         0x3E021FFE,
                         0x81000001,
                         0xCE4727E5,
                         0x4D05280F,
                         0x817FFFFF,
                         0xCF800200,
                         0xABFFFFB6,
                         0x817FFFFE,
                         0xDF8FFFFE,
                         0xBE7FFFDE,
                         0xB3800000,
                         0xCBFFFF3E,
                         0xA67FFFDE,
                         0xB3800001,
                         0xA9F3DF9D,
                         0x3E07FFFA,
                         0xB3FFFFFF,
                         0xC1FFFFBE,
                         0xBE480343,
                         0xB3FFFFFE,
                         0xBFFFF03F,
                         0xC667FFFE,
                         0xBD800000,
                         0x4EF90000,
                         0xFF77FFFB,
                         0xBD800001,
                         0x3FD5CD83,
                         0x3D800017,
                         0xBDFFFFFF,
                         0x4EFFF810,
                         0x6E800083,
                         0xBDFFFFFE,
                         0xB9FF7FDE,
                         0xBF0200FF,
                         0xBE000000,
                         0x8E7F9FFF,
                         0x57FFFFE8,
                         0xBE000001,
                         0x3E7BFF7F,
                         0xE36007FE,
                         0xBE7FFFFF,
                         0xBF0E0000,
                         0xDF800040,
                         0xBE7FFFFE,
                         0xC0FAFCA8,
                         0x5EF162CE,
                         0xBE800000,
                         0x1A040FFF,
                         0xDEFFFF8F,
                         0xBE800001,
                         0x416FFF7F,
                         0xBA7FBFBF,
                         0xBEFFFFFF,
                         0xBFD927B0,
                         0xAA25F8F1,
                         0xBEFFFFFE,
                         0xC03FEFFF,
                         0xCF01FFFA,
                         0xBF000000,
                         0xBD80007D,
                         0x4B8020FF,
                         0xBF000001,
                         0x167FFFFF,
                         0x3ED7E368,
                         0xBF7FFFFF,
                         0x407F00FF,
                         0x33A16981,
                         0xBF7FFFFE,
                         0xBF00003F,
                         0x2A803E00,
                         0xBF800000,
                         0x3F900006,
                         0xCFB6B6A4,
                         0xBF800001,
                         0xDF7F8400,
                         0xC6650556,
                         0xBFFFFFFF,
                         0x3E7DFFDF,
                         0x3DEDFFFE,
                         0xBFFFFFFE,
                         0x3A8800FF,
                         0xC07FFFFF,
                         0xC0000000,
                         0xC2780003,
                         0x80816E31,
                         0xC0000001,
                         0xBE10001F,
                         0xC3780FFE,
                         0xC07FFFFF,
                         0x8008007F,
                         0x0E7EF000,
                         0xC07FFFFE,
                         0x4C000F00,
                         0xDEE6C09E,
                         0xC0800000,
                         0xCF7FF7FF,
                         0xCF7FFE1F,
                         0xC0800001,
                         0x5EFFFE80,
                         0xBE6BD7DA,
                         0xC0FFFFFF,
                         0xC1000FC0,
                         0xF5ECD138,
                         0xC0FFFFFE,
                         0x0100203E,
                         0xB17FBFEF,
                         0xC1000000,
                         0x3E761BE2,
                         0x31801FE0,
                         0xC1000001,
                         0x3F804000,
                         0x010043FF,
                         0xC17FFFFF,
                         0xA413C634,
                         0xAABB1FF8,
                         0xC17FFFFE,
                         0xBFF7FF7F,
                         0x600FFBFF,
                         0xC1800000,
                         0x80703FFF,
                         0xB3FBFFFF,
                         0xC1800001,
                         0xC17FBFFC,
                         0x3C801FC0,
                         0xC1FFFFFF,
                         0xBFBAE7CE,
                         0x4100FFFD,
                         0xC1FFFFFE,
                         0xBE820002,
                         0x40E0000E,
                         0xCB800000,
                         0x2003FFEE,
                         0xC1FFEFFF,
                         0xCB800001,
                         0x4F7FFCFF,
                         0xDF476AB5,
                         0xCBFFFFFF,
                         0x47FFF7F7,
                         0xBF7FFBFD,
                         0xCBFFFFFE,
                         0xCFFFC020,
                         0x0FA91FEA,
                         0xCE000000,
                         0xC00021FF,
                         0x01020800,
                         0xCE000001,
                         0x80E7FFFF,
                         0xBD0007F6,
                         0xCE7FFFFF,
                         0x00810008,
                         0xC0A69B11,
                         0xCE7FFFFE,
                         0x40100040,
                         0xC1F2A0E2,
                         0xCE800000,
                         0xC78800FF,
                         0xBC5BFFFE,
                         0xCE800001,
                         0x3380AAB4,
                         0xCE087FFE,
                         0xCEFFFFFF,
                         0xFF6CA45C,
                         0x4EDFEFFF,
                         0xCEFFFFFE,
                         0x803FFFFE,
                         0xBC0A8EE7,
                         0xCF000000,
                         0xB97FD000,
                         0x4104003F,
                         0xCF000001,
                         0xC17FFFFF,
                         0xC0F80FFF,
                         0xCF7FFFFF,
                         0x33FF0002,
                         0xBB7F9000,
                         0xCF7FFFFE,
                         0x41780007,
                         0xB88043FE,
                         0xCF800000,
                         0x4EA0007F,
                         0x3F1FFF7E,
                         0xCF800001,
                         0x98FC0003,
                         0xBA08003F

                
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


        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n" 

        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n" 

        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n" 

        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n" 

        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n" 

        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n" 

        "fmadd.ps f0,f1,f2,f3\n"
        "fmadd.ps f3,f4,f5,f6\n"
        "fmadd.ps f6,f7,f8,f9\n"
        "fmadd.ps f9,f10,f11,f12\n"
        "fmadd.ps f12,f13,f14,f15\n"
        "fmadd.ps f15,f16,f17,f18\n"
        "fmadd.ps f18,f19,f20,f21\n"
        "fmadd.ps f21,f22,f23,f24\n"
        "fmadd.ps f24,f25,f26,f27\n"
        "fmadd.ps f27,f28,f29,f30\n"
        "fmadd.ps f30,f31,f22,f3\n"
        "fnmadd.ps f1, f0, f13, f12, dyn\n"
	"fmsub.ps f16, f1, f15, f1, dyn\n"     
	"fnmsub.ps f27, f14, f15, f15, rdn\n"                  
	"fmadd.ps f25, f13, f0, f0, dyn\n" 
	"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
        "fnmadd.ps f24, f1, f2, f16, dyn\n"   
        "fmadd.ps f25, f3, f4, f0, dyn\n" 
        "fnmsub.ps f27, f18, f7, f17, rdn\n"         


         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x5", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
         );

   C_TEST_PASS;
   return 0;
}

