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
                         0x3C140D9D,
                         0xBF5FFF7E,
                         0x00000000,
                         0xC4A00004,
                         0x81800807,
                         0x00000001,
                         0x406F6187,
                         0x3400041F,
                         0x007FFFFF,
                         0xCE838000,
                         0x3FBFFFFE,
                         0x007FFFFE,
                         0x8BE00FFF,
                         0x4C000500,
                         0x00800000,
                         0xBF7FC03F,
                         0x2BD74AD3,
                         0x00800001,
                         0x3D8007FF,
                         0xC01FF7FE,
                         0x00FFFFFF,
                         0xA0FDFFFF,
                         0xBF06AD19,
                         0x00FFFFFE,
                         0xBF7FBF80,
                         0xCF4530C8,
                         0x01000000,
                         0xBE8037FF,
                         0xBE7FE0FF,
                         0x01000001,
                         0xBF535072,
                         0x3F80021E,
                         0x017FFFFF,
                         0x4BBFF9D2,
                         0x4FFFC01F,
                         0x017FFFFE,
                         0xBDA62F2E,
                         0x42020003,
                         0x33800000,
                         0xC1BFFFDF,
                         0x407DFFFF,
                         0x33800001,
                         0xBFFCBA0C,
                         0xC7355EF5,
                         0x33FFFFFF,
                         0x1380010F,
                         0x44800076,
                         0x33FFFFFE,
                         0x4FEFFFFF,
                         0xBDFFAFFE,
                         0x3D800000,
                         0x4170001E,
                         0x5E3A4033,
                         0x3D800001,
                         0xD8F7FF00,
                         0xB9001F00,
                         0x3DFFFFFF,
                         0xCF7F2000,
                         0x801001FF,
                         0x3DFFFFFE,
                         0x010DFFFF,
                         0x5E7FC1FF,
                         0x3E000000,
                         0x5E7FC002,
                         0x7EFFBBFE,
                         0x3E000001,
                         0x3E7D02A2,
                         0x41000207,
                         0x3E7FFFFF,
                         0x0E001FBF,
                         0x3D800201,
                         0x3E7FFFFE,
                         0xC091A4B4,
                         0x438AD436,
                         0x3E800000,
                         0xBF001F00,
                         0x7E88A737,
                         0x3E800001,
                         0xDE0FFF7E,
                         0x457FFFF7,
                         0x3EFFFFFF,
                         0xBD5FFFF7,
                         0x3387FFFC,
                         0x3EFFFFFE,
                         0x007FEF7F,
                         0xDE03E5F7,
                         0x3F000000,
                         0xBF7007FF,
                         0xC40003DF,
                         0x3F000001,
                         0x597F81FE,
                         0xDEFFDFFF,
                         0x3F7FFFFF,
                         0x338040FF,
                         0xBBC00080,
                         0x3F7FFFFE,
                         0x3B90115B,
                         0xB5E100CF,
                         0x3F800000,
                         0xBF0FC000,
                         0xCF003FFA,
                         0x3F800001,
                         0x251AE136,
                         0x55691BE5,
                         0x3FFFFFFF,
                         0xBDDA4C3E,
                         0x3F00207E,
                         0x3FFFFFFE,
                         0xBEF00400,
                         0xBE7FFDFE,
                         0x40000000,
                         0xCF97FFFF,
                         0x3DFFFEDF,
                         0x40000001,
                         0x98AE1AB5,
                         0x3E0CA325,
                         0x407FFFFF,
                         0x45D102AB,
                         0xCEF7F000,
                         0x407FFFFE,
                         0xDF00043E,
                         0xDF802001,
                         0x40800000,
                         0xD5D55C6A,
                         0xBD7FFB7F,
                         0x40800001,
                         0x4ACBD5EE,
                         0x68B4140E,
                         0x40FFFFFF,
                         0xFFFFFFF9,
                         0xBFC4F421,
                         0x40FFFFFE,
                         0x3DFFC01F,
                         0xDEFFFFE6,
                         0x41000000,
                         0xCD9FBA61,
                         0xC1820007,
                         0x41000001,
                         0xB00077FF,
                         0xA66F7FFF,
                         0x417FFFFF,
                         0xBE80003C,
                         0x33FE0002,
                         0x417FFFFE,
                         0xC17FF3FE,
                         0xC17C003F,
                         0x41800000,
                         0x5E83FFFE,
                         0x3C600FFE,
                         0x41800001,
                         0x499895ED,
                         0x45007FDF,
                         0x41FFFFFF,
                         0x27CAD95F,
                         0xEB83F7FF,
                         0x41FFFFFE,
                         0x4F00FDFE,
                         0x40801FFF,
                         0x4B800000,
                         0x5E702000,
                         0xCEFFDFE0,
                         0x4B800001,
                         0xA087FFBF,
                         0x4E4E7F45,
                         0x4BFFFFFF,
                         0xB1BA3FB2,
                         0x00FF0080,
                         0x4BFFFFFE,
                         0xFFF80400,
                         0x4021438F,
                         0x4E000000,
                         0xFF000FC0,
                         0xCF001002,
                         0x4E000001,
                         0x43000140,
                         0x41004003,
                         0x4E7FFFFF,
                         0x77601FFE,
                         0x5B00080F,
                         0x4E7FFFFE,
                         0xBFA9C375,
                         0x3FF7F800,
                         0x4E800000,
                         0x447A6A4B,
                         0x80A0000F,
                         0x4E800001,
                         0x8905FFFF,
                         0x3C800003,
                         0x4EFFFFFF,
                         0x01004D58,
                         0xC00107FF,
                         0x4EFFFFFE,
                         0x4BD1BDCD,
                         0x5FFFEFFD,
                         0x4F000000,
                         0x4E093FC3,
                         0xC0FFB800,
                         0x4F000001,
                         0xC1C2CA84,
                         0xBFD367A9,
                         0x4F7FFFFF,
                         0xBEFFFF40,
                         0xC68007C0,
                         0x4F7FFFFE,
                         0xC0808008,
                         0xC17FFFFF,
                         0x4F800000,
                         0xCEDB4B91,
                         0x3E7FF7EE,
                         0x4F800001,
                         0xB3CDBF66,
                         0xBE7FFFFE,
                         0x4FFFFFFF,
                         0x3C9FF7FE,
                         0xB38001F7,
                         0x4FFFFFFE,
                         0xBDFFFD80,
                         0x35E6BF9B,
                         0x5E000000,
                         0x477FFFEF,
                         0xBE800C00,
                         0x5E000001,
                         0xCFC05B66,
                         0x407FFC00,
                         0x5E7FFFFF,
                         0x807FE03F,
                         0x40E472C8,
                         0x5E7FFFFE,
                         0xC088001F,
                         0x2C7BFDFF,
                         0x5E800000,
                         0x3F569213,
                         0xDC0040FE,
                         0x5E800001,
                         0xB1000007,
                         0x42801DFE,
                         0x5EFFFFFF,
                         0x3AC3FFFF,
                         0xCF77FBFF,
                         0x5EFFFFFE,
                         0xC5E576B7,
                         0x0C0FFFFF,
                         0x5F000000,
                         0xB101DFFF,
                         0x3E0000FC,
                         0x5F000001,
                         0x468FF800,
                         0x18FEFE00,
                         0x5F7FFFFF,
                         0x3CD8F67A,
                         0xC1C0007E,
                         0x5F7FFFFE,
                         0xC09EAF7C,
                         0xA713FFFF,
                         0x5F800000,
                         0x4EBF7FFE

                
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

