/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {

  *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

	static uint32_t operands[72] __attribute__ ((aligned (16))) = {
		0x3f800000,    // 1.0 
		0x3f028f5c,    // 0.51
		0x3f800000,    // 1.0
		0x3f028f5c,    // 0.51 
		0xbf800000,    // -1.0 
		0xbf028f5c,    // -0.51
		0xbf800000,    // -1.0
		0xbf028f5c,    // -0.51
		0x4b000000,    // 8388608.0
		0x4b8c4b40,    // 18388608.0
		0x4b000000,    // 8388608.0
		0x4b8c4b40,    // 18388608.0
		0xcb000000,    // -8388608.0
		0xcb8c4b40,    // -18388608.0
		0xcb000000,    // -8388608.0
		0xcb8c4b40,    // -18388608.0
		0x00011111, // 9.7958E-41
		0x00000001, // 1.4E-45
		0x00011111, // 9.7958E-41
		0x007fffff, // 1.1754942E-38
		0x80011111, // -9.7958E-41
		0x80000001, // -1.4E-45
		0x80011111, // -9.7958E-41
		0x007fffff, // -1.1754942E-38
		0x7f800000, // inf
		0xff800000, // -inf
		0x7f800000, // inf
		0xff800000, // -inf
		0x0,  // zero
		0x80000000,  // -zero
		0x0,  // zero
		0x80000000,   // -zero
		0x7fc00000, // quiet NaN     
		0xffc00000, // -quiet NaN 
		0x7fc00001, // signaling NaN 
		0xffc00001, // -signaling NaN
		0x7fc00000, // quiet NaN     
		0xffc00000, // -quiet NaN  
		0x7fc00001, // signaling NaN 
		0xffc00001 // -signaling NaN
	};

	__asm__ __volatile__ (
		"li x2, 0xff\n"
		"mova.m.x x2\n"
		"flw.ps f0,  0 (%[operands])\n"
		"flw.ps f1,  4 (%[operands])\n"
		"flw.ps f2,  8 (%[operands])\n"
		"flw.ps f3,  12 (%[operands])\n"
		"fcmovm.ps f4, f0, f1\n"
		"fcmovm.ps f5, f1, f2\n"
		"fcmovm.ps f6, f2, f3\n"
		"fcmovm.ps f7, f3, f4\n"
		"fcmovm.ps f8, f2, f3\n"
		"fcmovm.ps f9, f3, f4\n"
		"fcmovm.ps f10, f4, f5\n"
		"fcmovm.ps f11, f5, f6\n"
		"fcmovm.ps f12, f6, f7\n"
		"fcmovm.ps f13, f7, f8\n"
		"fcmovm.ps f14, f8, f9\n"
		"fcmovm.ps f15, f9, f10\n"
		"fcmovm.ps f16, f3, f4\n"
		"fcmovm.ps f17, f4, f5\n"
		"fcmovm.ps f18, f5, f6\n"
		"fcmovm.ps f19, f6, f7\n"
		"fcmovm.ps f20, f7, f8\n"
		"fcmovm.ps f21, f8, f9\n"
		"fcmovm.ps f22, f9, f10\n"
		"fcmovm.ps f23, f10, f11\n"
		"fcmovm.ps f24, f3, f4\n"
		"fcmovm.ps f25, f4, f5\n"
		"fcmovm.ps f26, f5, f6\n"
		"fcmovm.ps f27, f6, f7\n"
		"fcmovm.ps f28, f7, f8\n"
		"fcmovm.ps f29, f8, f9\n"
		"fcmovm.ps f30, f9, f10\n"
		"fcmovm.ps f31, f10, f11\n"
		"mova.m.x x28\n"                                      // opcode=0xd60e107b
		"li x2, 0xff\n"
		"mova.m.x x2\n"

		"fsgnjx.ps f20, f29, f24\n"                           // opcode=0x218eaa7b
		"fpackreph.pi f24, f26\n"                             // opcode=0x260d1c7b
		"fle.s x30, f31, f26\n"                               // opcode=0xa1af8f53
		"fnot.pi f23, f28\n"                                  // opcode=0x060e2bfb
		"flt.ps f31, f26, f29\n"                              // opcode=0xa1dd1ffb
		"fxor.pi f20, f21, f22\n"                             // opcode=0x076aca7b
		"fle.s x28, f22, f26\n"                               // opcode=0xa1ab0e53
		"fsgnjx.ps f21, f24, f29\n"                           // opcode=0x21dc2afb
		"fsatu8.pi f23, f20\n"                                // opcode=0x061a3bfb
		"fsgnj.ps f21, f24, f28\n"                            // opcode=0x21cc0afb
		"fclass.ps f28, f23\n"                                // opcode=0xe00b9e7b
		"maskand m7, m1, m2\n"                                // opcode=0x6620f3fb
		"fmul.s f26, f23, f28, dyn\n"                         // opcode=0x11cbfd53
		"cubeface.ps f29, f30, f27\n"                         // opcode=0x89bf0efb
		"fsat8.pi f24, f27\n"                                 // opcode=0x060dbc7b
		"fsat8.pi f26, f20\n"                                 // opcode=0x060a3d7b
		"flt.ps f21, f27, f29\n"                              // opcode=0xa1dd9afb
		"fmul.pi f23, f30, f28\n"                             // opcode=0x17cf0bfb
		"fbci.ps f28, 370724\n"                               // opcode=0x5a824e1f
		"fcmov.ps f24, f31, f27, f23\n"                       // opcode=0xbdbfac3f
		"maskxor m4, m7, m1\n"                                // opcode=0x6613c27b
		"maskor m6, m5, m7\n"                                 // opcode=0x6672e37b
		"flt.pi f25, f22, f24\n"                              // opcode=0xa78b1cfb
		"fsgnjn.ps f20, f23, f21\n"                           // opcode=0x215b9a7b
		"flt.ps f28, f30, f26\n"                              // opcode=0xa1af1e7b
		"ffrc.ps f20, f21\n"                                  // opcode=0x582a8a7b
		"fnot.pi f26, f31\n"                                  // opcode=0x060fad7b
		"fmulh.pi f31, f30, f28\n"                            // opcode=0x17cf2ffb
		"fnot.pi f31, f27\n"                                  // opcode=0x060daffb
		"fsgnjn.s f24, f22, f25\n"                            // opcode=0x219b1c53
		"feqm.ps m6, f20, f31\n"                              // opcode=0xa1fa637b
		"maskxor m5, m3, m4\n"                                // opcode=0x6641c2fb
		"fsgnjn.ps f22, f23, f25\n"                           // opcode=0x219b9b7b
		"cubefaceidx.ps f31, f24, f22\n"                      // opcode=0x896c1ffb
		"ffrc.ps f21, f25\n"                                  // opcode=0x582c8afb
		"fsat8.pi f26, f24\n"                                 // opcode=0x060c3d7b
		"feq.pi f27, f29, f25\n"                              // opcode=0xa79eadfb
		"fbci.ps f26, 708366\n"                               // opcode=0xacf0ed1f
		"fsgnj.s f23, f20, f20\n"                             // opcode=0x214a0bd3
		"fmulh.pi f25, f25, f31\n"                            // opcode=0x17fcacfb
		"fmul.pi f30, f30, f20\n"                             // opcode=0x174f0f7b
		"fround.ps f23, f21, dyn\n"                           // opcode=0x581afbfb
		"li x2, 0xff\n"
		"mova.m.x x2\n"
		"fbcx.ps f23, x31\n"                                  // opcode=0x000fbb8b
		"fmul.ps f29, f23, f25, dyn\n"                        // opcode=0x119bfefb
		"maskpopcz x22, m1\n"                                 // opcode=0x54008b7b
		"fsgnjx.ps f31, f28, f28\n"                           // opcode=0x21ce2ffb
		"feqm.ps m5, f26, f25\n"                              // opcode=0xa19d62fb
		"fsgnjx.ps f24, f30, f25\n"                           // opcode=0x219f2c7b
		"feq.pi f25, f21, f21\n"                              // opcode=0xa75aacfb
		"fltu.pi f29, f22, f24\n"                             // opcode=0xa78b3efb
		//
		// vpu_bug_cubeface_intpipe
		//
		"cubesgntc.ps f20, f24, f28\n"                        // opcode=0x89cc3a7b
//		"nop \n"
		"cubeface.ps f20, f22, f27\n"                         // opcode=0x89bb0a7b
		"ffrc.ps f23, f23\n"                                  // opcode=0x582b8bfb
		"ffrc.ps f30, f20\n"                                  // opcode=0x582a0f7b
	// Asm output results to C code
	:
	// Asm input data from C code
	: [operands] "r" (operands)
	// Registers used in Asm so C knows they change
	: "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
	);
         C_TEST_PASS;
	return 0;
}
