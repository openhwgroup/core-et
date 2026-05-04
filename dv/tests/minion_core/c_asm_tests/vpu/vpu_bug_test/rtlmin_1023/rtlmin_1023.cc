/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {
   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

   volatile uint32_t operands[40] __attribute__ ((aligned (4096))) = {
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
   C_TEST_START;
   __asm__ __volatile__ (
         "li x2, 0xff\n"
         "mova.m.x x5\n"
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
         "fnot.pi f27, f21\n"                                  // opcode=0x060aadfb
         "fpackrepb.pi f28, f20\n"                             // opcode=0x260a0e7b
         "fmsub.s f22, f28, f27, f29, dyn\n"                   // opcode=0xe9be7b47
         "fbci.pi f22, 263620\n"                               // opcode=0x405c4b5f
         //"fnmadd.ps f31, f23, f23, f21, dyn\n"                 // opcode=0xaf7bffdb
         "fadd.s f20, f28, f25, dyn\n"                         // opcode=0x019e7a53
         "feqm.ps m1, f28, f20\n"                              // opcode=0xa14e60fb
         "maskpopcz x21, m6\n"                                 // opcode=0x54030afb
         "fmadd.s f29, f31, f27, f31, dyn\n"                   // opcode=0xf9bffec3
         "fsgnjn.s f30, f25, f30\n"                            // opcode=0x21ec9f53
         "flt.s x29, f27, f30\n"                               // opcode=0xa1ed9ed3
         "fmsub.ps f31, f22, f24, f29, dyn\n"                  // opcode=0xeb8b7fdb
         "flt.s x24, f24, f21\n"                               // opcode=0xa15c1c53
         "fmul.ps f21, f21, f29, dyn\n"                        // opcode=0x11dafafb
         "fclass.ps f20, f27\n"                                // opcode=0xe00d9a7b
         "feq.s x31, f23, f20\n"                               // opcode=0xa14bafd3
         "flem.ps m6, f21, f21\n"                              // opcode=0xa15ac37b
         "flt.s x21, f30, f27\n"                               // opcode=0xa1bf1ad3
         "fsatu8.pi f30, f22\n"                                // opcode=0x061b3f7b
         "fnmadd.s f27, f20, f22, f22, dyn\n"                  // opcode=0xb16a7dcf
         "feqm.ps m7, f21, f25\n"                              // opcode=0xa19ae3fb
         "feq.pi f26, f20, f27\n"                              // opcode=0xa7ba2d7b
         //"flq2 f22, 96(x23)\n"                                 // opcode=0xe60bdb07
         "fadd.pi f29, f24, f26\n"                             // opcode=0x07ac0efb
         "fmsub.ps f24, f23, f21, f28, dyn\n"                  // opcode=0xe35bfc5b
         //"fbc.ps f31, 92(x30)\n"                               // opcode=0xb5cf0f8b
         "fadd.pi f21, f25, f25\n"                             // opcode=0x079c8afb
         "fbci.ps f25, 569321\n"                               // opcode=0x8afe9c9f
         "fadd.ps f20, f20, f21, dyn\n"                        // opcode=0x015a7a7b
         "fmin.pi f22, f31, f31\n"                             // opcode=0x2fff8b7b
         "fadd.s f31, f24, f30, dyn\n"                         // opcode=0x01ec7fd3
         "fand.pi f31, f21, f25\n"                             // opcode=0x079afffb
         "fltu.pi f24, f23, f21\n"                             // opcode=0xa75bbc7b
         "fnmadd.s f31, f27, f22, f20, dyn\n"                  // opcode=0xa16dffcf
         "maskpopc x30, m1\n"                                  // opcode=0x52008f7b
         "fround.ps f23, f31, dyn\n"                           // opcode=0x581ffbfb
         "fmadd.s f28, f24, f20, f28, rup\n"                   // opcode=0xe14c3e43
         "flt.s x28, f29, f23\n"                               // opcode=0xa17e9e53
         "fbci.ps f24, 1029223\n"                              // opcode=0xfb467c1f
         "fclass.ps f23, f29\n"                                // opcode=0xe00e9bfb
         "fclass.s x29, f21\n"                                 // opcode=0xe00a9ed3
         "fsub.s f31, f22, f27, dyn\n"                         // opcode=0x09bb7fd3
         "fcmov.ps f20, f25, f21, f30\n"                       // opcode=0xf55caa3f
         "cubesgntc.ps f22, f31, f20\n"                        // opcode=0x894fbb7b
         "maskxor m5, m6, m1\n"                                // opcode=0x661342fb
         "fmulhu.pi f31, f28, f26\n"                           // opcode=0x17ae1ffb
         "flog.ps f31, f28\n"                                  // opcode=0x583e0ffb
         "fsrai.pi f22, f26, 23\n"                             // opcode=0x4f7d7b7b
         "fbci.ps f28, 917659\n"                               // opcode=0xe009be1f
         "fmin.s f25, f27, f22\n"                              // opcode=0x296d8cd3
         "fadd.s f29, f20, f28, dyn\n"                         // opcode=0x01ca7ed3
         "ffrc.ps f25, f24\n"                                  // opcode=0x582c0cfb
         "fsub.pi f29, f31, f28\n"                             // opcode=0x0fcf8efb
         "fcmov.ps f29, f23, f29, f21\n"                       // opcode=0xaddbaebf
         "cubefaceidx.ps f27, f26, f23\n"                      // opcode=0x897d1dfb
         "fle.ps f28, f22, f28\n"                              // opcode=0xa1cb0e7b
         "cubesgntc.ps f27, f21, f22\n"                        // opcode=0x896abdfb
         "fclass.ps f24, f26\n"                                // opcode=0xe00d1c7b
         "fminu.pi f22, f23, f28\n"                            // opcode=0x2fcbab7b
         "fcmov.ps f28, f21, f22, f24\n"                       // opcode=0xc56aae3f
         "fle.s x25, f24, f27\n"                               // opcode=0xa1bc0cd3
         "fmin.ps f24, f26, f29\n"                             // opcode=0x29dd0c7b
         "fcmovm.ps f21, f25, f25\n"                           // opcode=0x019c8af7
         "fsrli.pi f26, f31, 25\n"                             // opcode=0x4f9fdd7b
         "for.pi f24, f23, f25\n"                              // opcode=0x079bec7b
         "fsetm.pi m6, f26\n"                                  // opcode=0xe00d537b
         "fmin.ps f20, f24, f31\n"                             // opcode=0x29fc0a7b
         "fnmadd.s f22, f29, f24, f30, dyn\n"                  // opcode=0xf18efb4f
         "fsgnjx.ps f23, f23, f31\n"                           // opcode=0x21fbabfb
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "fsll.pi f25, f20, f30\n"                             // opcode=0x07ea1cfb
         "fsll.pi f20, f20, f24\n"                             // opcode=0x078a1a7b
         "fsatu8.pi f30, f23\n"                                // opcode=0x061bbf7b
         "flt.ps f30, f20, f22\n"                              // opcode=0xa16a1f7b
         "fle.s x25, f21, f31\n"                               // opcode=0xa1fa8cd3
         "fmulhu.pi f31, f27, f31\n"                           // opcode=0x17fd9ffb
         "fsll.pi f22, f24, f30\n"                             // opcode=0x07ec1b7b
         "fsgnjx.s f31, f31, f29\n"                            // opcode=0x21dfafd3
         //"fnmsub.ps f30, f30, f22, f21, dyn\n"                 // opcode=0xad6f7f5b
         "fpackreph.pi f30, f22\n"                             // opcode=0x260b1f7b
         "fsrai.pi f25, f20, 5\n"                              // opcode=0x4e5a7cfb
         "flt.ps f31, f22, f26\n"                              // opcode=0xa1ab1ffb
         "fsgnj.s f29, f20, f29\n"                             // opcode=0x21da0ed3
         "fbci.pi f22, 176181\n"                               // opcode=0x2b035b5f
         "cubesgntc.ps f30, f27, f20\n"                        // opcode=0x894dbf7b
         "maskpopcz x26, m6\n"                                 // opcode=0x54030d7b
         "fclass.ps f25, f30\n"                                // opcode=0xe00f1cfb
         "fbci.ps f24, 817072\n"                               // opcode=0xc77b0c1f
         "fcmovm.ps f26, f26, f21\n"                           // opcode=0x015d0d77
         "mova.x.m x24\n"                                      // opcode=0xd6000c7b
         "fltm.ps m5, f30, f30\n"                              // opcode=0xa1ef52fb
         "fmul.ps f31, f25, f21, dyn\n"                        // opcode=0x115cfffb
         "maskpopcz x30, m1\n"                                 // opcode=0x54008f7b
         "fsub.pi f21, f25, f29\n"                             // opcode=0x0fdc8afb
         "fmulh.pi f24, f27, f26\n"                            // opcode=0x17adac7b
         "fmax.ps f29, f21, f24\n"                             // opcode=0x298a9efb
         "fnot.pi f26, f28\n"                                  // opcode=0x060e2d7b
         "fle.pi f21, f26, f27\n"                              // opcode=0xa7bd0afb
         "maskpopcz x29, m6\n"                                 // opcode=0x54030efb
         "frcp.ps f23, f25\n"                                  // opcode=0x587c8bfb
         "fle.pi f20, f24, f30\n"                              // opcode=0xa7ec0a7b
         "fsgnjx.s f26, f21, f24\n"                            // opcode=0x218aad53
         "maskor m7, m0, m2\n"                                 // opcode=0x662063fb
         "masknot m7, m3\n"                                    // opcode=0x6601a3fb
         "fxor.pi f27, f22, f24\n"                             // opcode=0x078b4dfb
         "maskpopc x24, m6\n"                                  // opcode=0x52030c7b
         "fmul.ps f22, f28, f28, dyn\n"                        // opcode=0x11ce7b7b
         ////RTLMIN-5479"fsin.ps f25, f20\n"                                  // opcode=0x586a0cfb
         "masknot m0, m5\n"                                    // opcode=0x6602a07b
         //"fbc.ps f26, 64(x30)\n"                               // opcode=0x5c0f0d0b
         "fadd.ps f20, f30, f23, rdn\n"                        // opcode=0x017f2a7b
         "fsgnjx.s f22, f29, f26\n"                            // opcode=0x21aeab53
         //"flq2 f23, 37(x30)\n"                                 // opcode=0x025f5b87
         "fltu.pi f29, f23, f30\n"                             // opcode=0xa7ebbefb
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "feq.ps f25, f20, f22\n"                              // opcode=0xa16a2cfb
         "fminu.pi f28, f21, f27\n"                            // opcode=0x2fbaae7b
         "frcp.ps f20, f26\n"                                  // opcode=0x587d0a7b
         "fmulh.pi f29, f20, f28\n"                            // opcode=0x17ca2efb
         "fmsub.ps f23, f21, f20, f20, rtz\n"                  // opcode=0xa34a9bdb
         "fsgnjx.ps f20, f21, f27\n"                           // opcode=0x21baaa7b
         "fcmovm.ps f31, f25, f24\n"                           // opcode=0x018c8ff7
         "fmsub.s f20, f24, f23, f31, dyn\n"                   // opcode=0xf97c7a47
         "fandi.pi f27, f27, 29\n"                            // opcode=0xb59d9dbf
         "maskpopcz x24, m1\n"                                 // opcode=0x54008c7b
         "fmax.pi f29, f25, f29\n"                             // opcode=0x2fdc9efb
         "flog.ps f23, f30\n"                                  // opcode=0x583f0bfb
         "fmin.ps f22, f25, f23\n"                             // opcode=0x297c8b7b
         "fround.ps f20, f21, dyn\n"                           // opcode=0x581afa7b
         "fsetm.pi m1, f27\n"                                  // opcode=0xe00dd0fb
         "fsgnj.ps f25, f31, f23\n"                            // opcode=0x217f8cfb
         "ffrc.ps f27, f23\n"                                  // opcode=0x582b8dfb
         "fslli.pi f28, f27, 22\n"                             // opcode=0x4f6d9e7b
         "fnmsub.ps f31, f25, f25, f21, dyn\n"                 // opcode=0xad9cffdb
         "fmulh.pi f23, f29, f28\n"                            // opcode=0x17ceabfb
         "fnmsub.ps f21, f31, f31, f20, dyn\n"                 // opcode=0xa5fffadb
         "fnot.pi f27, f25\n"                                  // opcode=0x060cadfb
         "fmsub.ps f31, f22, f23, f24, dyn\n"                  // opcode=0xc37b7fdb
         "fmul.pi f21, f23, f23\n"                             // opcode=0x177b8afb
         "fmaxu.pi f31, f24, f29\n"                            // opcode=0x2fdc3ffb
         "fmul.pi f26, f28, f28\n"                             // opcode=0x17ce0d7b
         "fnmadd.s f26, f23, f22, f28, dyn\n"                  // opcode=0xe16bfd4f
         "fadd.s f28, f29, f20, dyn\n"                         // opcode=0x014efe53
         "fxor.pi f30, f25, f25\n"                             // opcode=0x079ccf7b
         "fnmsub.s f31, f25, f31, f30, dyn\n"                  // opcode=0xf1fcffcb
         "fandi.pi f27, f31, 61\n"                            // opcode=0xbd9f9dbf
         "fsrli.pi f20, f27, 18\n"                             // opcode=0x4f2dda7b
         "faddi.pi f24, f29, 97\n"                            // opcode=0xc5de8c3f
         "fsrai.pi f21, f28, 30\n"                             // opcode=0x4fee7afb
         "feq.pi f28, f23, f25\n"                              // opcode=0xa79bae7b
         "fmin.ps f25, f28, f20\n"                             // opcode=0x294e0cfb
         "fmul.pi f26, f27, f25\n"                             // opcode=0x179d8d7b
         "maskxor m4, m5, m2\n"                                // opcode=0x6622c27b
         "fsgnj.s f23, f28, f27\n"                             // opcode=0x21be0bd3
         "fexp.ps f25, f20\n"                                  // opcode=0x584a0cfb
         "faddi.pi f31, f22, 11\n"                           // opcode=0xfd3b0fbf
         "fmulhu.pi f28, f30, f20\n"                           // opcode=0x174f1e7b
         "fadd.pi f21, f20, f28\n"                             // opcode=0x07ca0afb
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "fmul.s f27, f21, f20, dyn\n"                         // opcode=0x114afdd3
         "fsgnj.s f24, f27, f31\n"                             // opcode=0x21fd8c53
         "fcmovm.ps f31, f26, f29\n"                           // opcode=0x01dd0ff7
         "fsub.pi f22, f29, f21\n"                             // opcode=0x0f5e8b7b
         //"flq2 f20, 22(x31)\n"                                 // opcode=0x296fda07
         "fclass.s x27, f21\n"                                 // opcode=0xe00a9dd3
         "maskpopcz x28, m1\n"                                 // opcode=0x54008e7b
         //"fbc.ps f28, 23(x22)\n"                               // opcode=0x797b0e0b
         "fsetm.pi m1, f21\n"                                  // opcode=0xe00ad0fb
         "fle.pi f28, f20, f24\n"                              // opcode=0xa78a0e7b
         "fmin.ps f23, f22, f29\n"                             // opcode=0x29db0bfb
         //"fbc.ps f28, 91(x22)\n"                               // opcode=0xe5bb0e0b
         "faddi.pi f21, f28, 22\n"                            // opcode=0xb52e0abf
         "fnmadd.ps f25, f21, f20, f25, dyn\n"                 // opcode=0xcf4afcdb
         "fsgnjn.s f22, f27, f26\n"                            // opcode=0x21ad9b53
         "fsat8.pi f27, f24\n"                                 // opcode=0x060c3dfb
         "maskand m6, m1, m6\n"                                // opcode=0x6660f37b
         "fmaxu.pi f25, f31, f22\n"                            // opcode=0x2f6fbcfb
         "fsatu8.pi f20, f24\n"                                // opcode=0x061c3a7b
         "maskpopcz x21, m6\n"                                 // opcode=0x54030afb
         "flt.ps f21, f24, f29\n"                              // opcode=0xa1dc1afb
         "fsgnjx.ps f20, f28, f21\n"                           // opcode=0x215e2a7b
         "fmin.ps f27, f20, f20\n"                             // opcode=0x294a0dfb
         "fandi.pi f23, f25, 17\n"                           // opcode=0xfd9c9bbf
         "fsrli.pi f20, f20, 7\n"                              // opcode=0x4e7a5a7b
         "fmin.s f25, f24, f20\n"                              // opcode=0x294c0cd3
         "fsgnjx.s f23, f24, f26\n"                            // opcode=0x21ac2bd3
         "maskpopc x24, m5\n"                                  // opcode=0x52028c7b
         "fbci.pi f28, 659062\n"                               // opcode=0xa0e76e5f
         "fmax.pi f21, f31, f20\n"                             // opcode=0x2f4f9afb
         "fsll.pi f25, f29, f31\n"                             // opcode=0x07fe9cfb
         "fadd.s f20, f22, f29, dyn\n"                         // opcode=0x01db7a53
         "faddi.pi f28, f27, 89\n"                            // opcode=0xc55d8e3f
         //"flq2 f29, 125(x22)\n"                                // opcode=0x2fdb5e87
         "fnmsub.ps f25, f30, f26, f28, dyn\n"                 // opcode=0xe5af7cdb
         "fmulhu.pi f20, f21, f22\n"                           // opcode=0x176a9a7b
         "fcmovm.ps f23, f26, f20\n"                           // opcode=0x014d0bf7
         "fnmadd.s f25, f20, f21, f29, dyn\n"                  // opcode=0xe95a7ccf
         //RTLMIN-5479"frsq.ps f29, f28\n"                                  // opcode=0x588e0efb
         "cubefaceidx.ps f24, f26, f21\n"                      // opcode=0x895d1c7b
         "fexp.ps f31, f30\n"                                  // opcode=0x584f0ffb
         "fslli.pi f30, f28, 23\n"                             // opcode=0x4f7e1f7b
         "fsub.s f26, f23, f24, dyn\n"                         // opcode=0x098bfd53
         "fltm.pi m2, f22, f30\n"                              // opcode=0x3feb017b
         //"fsq2 f27, 21(x23)\n"                                 // opcode=0xe9bbdaa7
         "fsat8.pi f30, f24\n"                                 // opcode=0x060c3f7b
         "fsrl.pi f20, f29, f20\n"                             // opcode=0x074eda7b
         "feq.pi f22, f24, f29\n"                              // opcode=0xa7dc2b7b
         "maskpopc x22, m1\n"                                  // opcode=0x52008b7b
         "fnmadd.ps f24, f20, f23, f28, dyn\n"                 // opcode=0xe77a7c5b
         "fsgnjn.ps f27, f27, f28\n"                           // opcode=0x21cd9dfb
         "fnmsub.ps f22, f29, f28, f20, dyn\n"                 // opcode=0xa5cefb5b
         "fsgnj.ps f24, f28, f29\n"                            // opcode=0x21de0c7b
         "for.pi f24, f20, f26\n"                              // opcode=0x07aa6c7b
         //RTLMIN-5479"fsin.ps f30, f31\n"                                  // opcode=0x586f8f7b
         "fnmsub.ps f25, f30, f26, f29, dyn\n"                 // opcode=0xedaf7cdb
         "fclass.ps f27, f31\n"                                // opcode=0xe00f9dfb
         "maskpopc x27, m4\n"                                  // opcode=0x52020dfb
         "fxor.pi f30, f25, f30\n"                             // opcode=0x07eccf7b
         "fnmsub.ps f24, f28, f24, f30, dyn\n"                 // opcode=0xf58e7c5b
         "fround.ps f27, f27, dyn\n"                           // opcode=0x581dfdfb
         "fsgnjn.s f24, f27, f28\n"                            // opcode=0x21cd9c53
         "faddi.pi f21, f28, 461\n"                            // opcode=0x74de0abf
         "fsll.pi f23, f25, f23\n"                             // opcode=0x077c9bfb
         "for.pi f22, f29, f28\n"                              // opcode=0x07ceeb7b
         "fpackreph.pi f25, f28\n"                             // opcode=0x260e1cfb
         "fsgnjx.s f21, f22, f27\n"                            // opcode=0x21bb2ad3
         "fmax.ps f20, f28, f21\n"                             // opcode=0x295e1a7b
         "fsub.pi f24, f21, f25\n"                             // opcode=0x0f9a8c7b
         "fnot.pi f28, f29\n"                                  // opcode=0x060eae7b
         "feqm.ps m7, f25, f20\n"                              // opcode=0xa14ce3fb
         "fbcx.ps f27, x24\n"                                  // opcode=0x000c3d8b
         "fslli.pi f28, f31, 27\n"                             // opcode=0x4fbf9e7b
         "cubesgntc.ps f28, f20, f30\n"                        // opcode=0x89ea3e7b
         "fbcx.ps f24, x31\n"                                  // opcode=0x000fbc0b
         "flog.ps f21, f26\n"                                  // opcode=0x583d0afb
         "flt.ps f31, f29, f29\n"                              // opcode=0xa1de9ffb
         "fsat8.pi f22, f22\n"                                 // opcode=0x060b3b7b
         "flem.ps m7, f20, f25\n"                              // opcode=0xa19a43fb
         "fadd.ps f30, f29, f22, rtz\n"                        // opcode=0x016e9f7b
         "fexp.ps f24, f31\n"                                  // opcode=0x584f8c7b
         "fmadd.ps f24, f25, f23, f26, dyn\n"                  // opcode=0xd17cfc5b
         //"fbc.ps f21, 116(x29)\n"                              // opcode=0x5f4e8a8b
         "fslli.pi f29, f26, 10\n"                             // opcode=0x4ead1efb
         "fsra.pi f31, f22, f26\n"                             // opcode=0x0fab5ffb
         "fmulhu.pi f30, f31, f30\n"                           // opcode=0x17ef9f7b
         "maskpopcz x28, m5\n"                                 // opcode=0x54028e7b
         "fsgnjn.s f30, f20, f22\n"                            // opcode=0x216a1f53
         "fexp.ps f30, f29\n"                                  // opcode=0x584e8f7b
         "flog.ps f22, f29\n"                                  // opcode=0x583e8b7b
         "mova.m.x x24\n"                                      // opcode=0xd60c107b
         "maskand m5, m7, m5\n"                                // opcode=0x6653f2fb
         "flem.ps m1, f30, f29\n"                              // opcode=0xa1df40fb
         "fpackrepb.pi f23, f24\n"                             // opcode=0x260c0bfb
         "fsat8.pi f27, f23\n"                                 // opcode=0x060bbdfb
         "fmul.s f31, f31, f29, dyn\n"                         // opcode=0x11dfffd3
         "ffrc.ps f21, f23\n"                                  // opcode=0x582b8afb
         "fpackrepb.pi f20, f28\n"                             // opcode=0x260e0a7b
         "fmadd.ps f30, f27, f23, f24, dyn\n"                  // opcode=0xc17dff5b
         "frcp.ps f23, f28\n"                                  // opcode=0x587e0bfb
         "fslli.pi f26, f20, 13\n"                             // opcode=0x4eda1d7b
         "fsgnjn.s f24, f28, f28\n"                            // opcode=0x21ce1c53
         "maskor m4, m1, m5\n"                                 // opcode=0x6650e27b
         "feq.pi f20, f30, f24\n"                              // opcode=0xa78f2a7b
         "fsgnjx.ps f30, f24, f21\n"                           // opcode=0x215c2f7b
         "fmax.s f28, f27, f28\n"                              // opcode=0x29cd9e53
         "fsgnjx.s f23, f25, f29\n"                            // opcode=0x21dcabd3
         "fsub.pi f21, f25, f20\n"                             // opcode=0x0f4c8afb
         "maskpopc x22, m4\n"                                  // opcode=0x52020b7b
         "fle.pi f20, f26, f27\n"                              // opcode=0xa7bd0a7b
         //"flq2 f20, 81(x23)\n"                                 // opcode=0xa51bda07
         "frcp.ps f23, f25\n"                                  // opcode=0x587c8bfb
         "fmsub.ps f25, f20, f25, f29, rtz\n"                  // opcode=0xeb9a1cdb
         "fmsub.ps f31, f26, f27, f29, dyn\n"                  // opcode=0xebbd7fdb
         "fsub.pi f28, f24, f25\n"                             // opcode=0x0f9c0e7b
         "ffrc.ps f28, f24\n"                                  // opcode=0x582c0e7b
         "flog.ps f25, f24\n"                                  // opcode=0x583c0cfb
         "fpackrepb.pi f30, f29\n"                             // opcode=0x260e8f7b
         "fsra.pi f25, f23, f31\n"                             // opcode=0x0ffbdcfb
         "fround.ps f28, f28, dyn\n"                           // opcode=0x581e7e7b
         "fmsub.ps f27, f20, f27, f27, dyn\n"                  // opcode=0xdbba7ddb
         "fmulhu.pi f30, f27, f23\n"                           // opcode=0x177d9f7b
         //"fbc.ps f30, 4(x23)\n"                                // opcode=0x984b8f0b
         "fexp.ps f30, f22\n"                                  // opcode=0x584b0f7b
         "fmin.pi f23, f23, f31\n"                             // opcode=0x2ffb8bfb
         "fmul.pi f31, f24, f25\n"                             // opcode=0x179c0ffb
         "fle.s x30, f26, f20\n"                               // opcode=0xa14d0f53
         "fmin.s f31, f31, f30\n"                              // opcode=0x29ef8fd3
         //"flq2 f21, 101(x22)\n"                                // opcode=0x865b5a87
         "fxor.pi f26, f25, f24\n"                             // opcode=0x078ccd7b
         "cubesgntc.ps f21, f22, f26\n"                        // opcode=0x89ab3afb
         "maskand m5, m7, m7\n"                                // opcode=0x6673f2fb
         "cubefaceidx.ps f31, f21, f31\n"                      // opcode=0x89fa9ffb
         "fmulhu.pi f23, f31, f28\n"                           // opcode=0x17cf9bfb
         "fmax.ps f25, f27, f22\n"                             // opcode=0x296d9cfb
         "cubesgntc.ps f20, f22, f27\n"                        // opcode=0x89bb3a7b
         "fminu.pi f23, f28, f29\n"                            // opcode=0x2fde2bfb
         "fmul.ps f26, f28, f29, dyn\n"                        // opcode=0x11de7d7b
         "cubesgnsc.ps f22, f22, f24\n"                        // opcode=0x898b2b7b
         "fandi.pi f25, f24, 297\n"                            // opcode=0x4c9c1cbf
         "frcp.ps f30, f22\n"                                  // opcode=0x587b0f7b
         "flem.ps m4, f23, f31\n"                              // opcode=0xa1fbc27b
         "fandi.pi f21, f30, 77\n"                            // opcode=0x941f1abf
         "fmulh.pi f22, f22, f26\n"                            // opcode=0x17ab2b7b
         "fmulh.pi f24, f22, f24\n"                            // opcode=0x178b2c7b
         "fnmsub.ps f31, f31, f30, f22, dyn\n"                 // opcode=0xb5efffdb
         "ffrc.ps f30, f28\n"                                  // opcode=0x582e0f7b
         "fmsub.ps f25, f27, f21, f24, dyn\n"                  // opcode=0xc35dfcdb
         "fmul.s f20, f30, f22, dyn\n"                         // opcode=0x116f7a53
         "fsgnjn.ps f22, f29, f24\n"                           // opcode=0x218e9b7b
         "masknot m5, m4\n"                                    // opcode=0x660222fb
         "fmadd.s f30, f22, f25, f30, dyn\n"                   // opcode=0xf19b7f43
         "fle.pi f25, f24, f29\n"                              // opcode=0xa7dc0cfb
         "flt.pi f23, f22, f22\n"                              // opcode=0xa76b1bfb
         "fsgnjx.s f29, f29, f30\n"                            // opcode=0x21eeaed3
         "fnmsub.s f28, f29, f25, f28, dyn\n"                  // opcode=0xe19efe4b
         "fsub.s f26, f25, f28, dyn\n"                         // opcode=0x09ccfd53
         "fsub.s f23, f23, f31, dyn\n"                         // opcode=0x09fbfbd3
         "feq.pi f23, f26, f28\n"                              // opcode=0xa7cd2bfb
         "fbcx.ps f20, x28\n"                                  // opcode=0x000e3a0b
         "fsgnj.s f20, f22, f20\n"                             // opcode=0x214b0a53
         "flt.pi f25, f22, f24\n"                              // opcode=0xa78b1cfb
         "fsgnj.ps f20, f31, f23\n"                            // opcode=0x217f8a7b
         "ffrc.ps f25, f25\n"                                  // opcode=0x582c8cfb
         "fpackrepb.pi f30, f25\n"                             // opcode=0x260c8f7b
         "fbci.pi f30, 134848\n"                               // opcode=0x20ec0f5f
         "fmsub.s f30, f30, f30, f31, dyn\n"                   // opcode=0xf9ef7f47
         "fsgnjn.ps f21, f23, f31\n"                           // opcode=0x21fb9afb
         "fbcx.ps f23, x20\n"                                  // opcode=0x000a3b8b
         "faddi.pi f22, f20, 366\n"                            // opcode=0x5cea0b3f
         "fsetm.pi m2, f28\n"                                  // opcode=0xe00e517b
         "fnmadd.ps f26, f31, f29, f23, dyn\n"                 // opcode=0xbfdffd5b
         "fnmsub.ps f26, f26, f31, f22, dyn\n"                 // opcode=0xb5fd7d5b
         "fsgnjn.ps f25, f24, f22\n"                           // opcode=0x216c1cfb
         "fsgnjn.s f20, f27, f31\n"                            // opcode=0x21fd9a53
         "fltm.ps m3, f20, f29\n"                              // opcode=0xa1da51fb
         "fpackreph.pi f26, f27\n"                             // opcode=0x260d9d7b
         "fclass.ps f29, f29\n"                                // opcode=0xe00e9efb
         "fmul.pi f27, f24, f21\n"                             // opcode=0x175c0dfb
         "flt.pi f25, f21, f24\n"                              // opcode=0xa78a9cfb
         "fmin.s f22, f30, f21\n"                              // opcode=0x295f0b53
         "cubesgnsc.ps f27, f21, f26\n"                        // opcode=0x89aaadfb
         "fsat8.pi f26, f29\n"                                 // opcode=0x060ebd7b
         "for.pi f30, f28, f30\n"                              // opcode=0x07ee6f7b
         "fnmsub.ps f25, f26, f29, f26, dyn\n"                 // opcode=0xd5dd7cdb
         "cubesgntc.ps f30, f27, f24\n"                        // opcode=0x898dbf7b
         "fsrl.pi f29, f30, f30\n"                             // opcode=0x07ef5efb
         "fnmadd.ps f28, f29, f22, f31, dyn\n"                 // opcode=0xff6efe5b
         "fmul.pi f29, f28, f25\n"                             // opcode=0x179e0efb
         "fsub.pi f24, f22, f29\n"                             // opcode=0x0fdb0c7b
         "fadd.s f31, f28, f28, dyn\n"                         // opcode=0x01ce7fd3
         "fcmovm.ps f23, f21, f28\n"                           // opcode=0x01ca8bf7
         "fsub.s f31, f23, f30, dyn\n"                         // opcode=0x09ebffd3
         "fadd.pi f25, f23, f23\n"                             // opcode=0x077b8cfb
         "fmulh.pi f24, f23, f25\n"                            // opcode=0x179bac7b
         "feq.ps f26, f24, f28\n"                              // opcode=0xa1cc2d7b
         "fsra.pi f31, f27, f30\n"                             // opcode=0x0feddffb
         "fsll.pi f22, f20, f22\n"                             // opcode=0x076a1b7b
         "faddi.pi f22, f31, 84\n"                            // opcode=0xaccf8b3f
         "fmaxu.pi f26, f26, f24\n"                            // opcode=0x2f8d3d7b
         "fsrai.pi f29, f27, 11\n"                             // opcode=0x4ebdfefb
         "feqm.ps m2, f27, f30\n"                              // opcode=0xa1ede17b
         "fadd.s f24, f30, f23, dyn\n"                         // opcode=0x017f7c53
         "flt.pi f30, f25, f20\n"                              // opcode=0xa74c9f7b
         "fsat8.pi f30, f22\n"                                 // opcode=0x060b3f7b
         "fslli.pi f21, f28, 14\n"                             // opcode=0x4eee1afb
         "flt.s x28, f26, f27\n"                               // opcode=0xa1bd1e53
         "fclass.ps f23, f20\n"                                // opcode=0xe00a1bfb
         "fminu.pi f20, f28, f26\n"                            // opcode=0x2fae2a7b
         "fmaxu.pi f26, f31, f31\n"                            // opcode=0x2fffbd7b
         "fpackreph.pi f30, f24\n"                             // opcode=0x260c1f7b
         "fsub.s f23, f31, f26, rne\n"                         // opcode=0x09af8bd3
         "ffrc.ps f28, f20\n"                                  // opcode=0x582a0e7b
         "faddi.pi f22, f30, 51\n"                            // opcode=0xbcff0b3f
         "fltm.pi m0, f24, f31\n"                              // opcode=0x3ffc007b
         "fle.ps f23, f29, f24\n"                              // opcode=0xa18e8bfb
         "fmadd.ps f23, f28, f23, f26, dyn\n"                  // opcode=0xd17e7bdb
         "fexp.ps f24, f24\n"                                  // opcode=0x584c0c7b
         "cubesgntc.ps f25, f26, f28\n"                        // opcode=0x89cd3cfb
         "fcmov.ps f30, f20, f31, f26\n"                       // opcode=0xd5fa2f3f
         "fle.pi f26, f25, f24\n"                              // opcode=0xa78c8d7b
         "fmsub.s f28, f30, f21, f21, dyn\n"                   // opcode=0xa95f7e47
         "fsgnjx.ps f26, f26, f27\n"                           // opcode=0x21bd2d7b
         "flt.ps f28, f21, f26\n"                              // opcode=0xa1aa9e7b
         "feq.pi f26, f20, f20\n"                              // opcode=0xa74a2d7b
         "flt.ps f21, f24, f21\n"                              // opcode=0xa15c1afb
         "fle.ps f20, f21, f27\n"                              // opcode=0xa1ba8a7b
         "fand.pi f25, f24, f30\n"                             // opcode=0x07ec7cfb
         "fsub.pi f20, f21, f31\n"                             // opcode=0x0ffa8a7b
         "masknot m3, m1\n"                                    // opcode=0x6600a1fb
         "fmul.s f20, f31, f24, dyn\n"                         // opcode=0x118ffa53
         "fminu.pi f31, f26, f25\n"                            // opcode=0x2f9d2ffb
         "fmin.ps f24, f26, f25\n"                             // opcode=0x299d0c7b
         "maskand m3, m2, m4\n"                                // opcode=0x664171fb
         "fmax.pi f24, f30, f20\n"                             // opcode=0x2f4f1c7b
         "fsll.pi f28, f24, f26\n"                             // opcode=0x07ac1e7b
         "fnmsub.ps f20, f22, f28, f24, dyn\n"                 // opcode=0xc5cb7a5b
         "fsgnjn.s f26, f27, f23\n"                            // opcode=0x217d9d53
         "flt.pi f21, f27, f27\n"                              // opcode=0xa7bd9afb
         "fmul.pi f24, f31, f25\n"                             // opcode=0x179f8c7b
         "fadd.s f29, f24, f24, dyn\n"                         // opcode=0x018c7ed3
         "frcp.ps f29, f26\n"                                  // opcode=0x587d0efb
         "fcmovm.ps f28, f20, f30\n"                           // opcode=0x01ea0e77
         "fmin.ps f30, f23, f26\n"                             // opcode=0x29ab8f7b
         "fadd.pi f30, f29, f23\n"                             // opcode=0x077e8f7b
         "mova.m.x x27\n"                                      // opcode=0xd60d907b
         "feq.ps f22, f23, f25\n"                              // opcode=0xa19bab7b
         "fltm.pi m5, f30, f25\n"                              // opcode=0x3f9f02fb
         "fround.ps f25, f20, dyn\n"                           // opcode=0x581a7cfb
         "fadd.s f31, f22, f26, dyn\n"                         // opcode=0x01ab7fd3
         "fnmadd.ps f30, f23, f30, f26, dyn\n"                 // opcode=0xd7ebff5b
         "fnot.pi f23, f20\n"                                  // opcode=0x060a2bfb
         "fsgnjn.ps f21, f29, f31\n"                           // opcode=0x21fe9afb
         "cubesgnsc.ps f28, f22, f29\n"                        // opcode=0x89db2e7b
         "fsrli.pi f26, f21, 27\n"                             // opcode=0x4fbadd7b
         "fmin.ps f30, f28, f29\n"                             // opcode=0x29de0f7b
         "fmulhu.pi f25, f22, f31\n"                           // opcode=0x17fb1cfb
         "feq.pi f24, f22, f27\n"                              // opcode=0xa7bb2c7b
         "fxor.pi f29, f29, f31\n"                             // opcode=0x07fecefb
         "fsat8.pi f28, f20\n"                                 // opcode=0x060a3e7b
         "fmadd.ps f25, f22, f26, f23, dyn\n"                  // opcode=0xb9ab7cdb
         "ffrc.ps f22, f28\n"                                  // opcode=0x582e0b7b
         "fround.ps f23, f25, dyn\n"                           // opcode=0x581cfbfb
         //"fbc.ps f22, 7(x25)\n"                                // opcode=0x607c8b0b
         "fmaxu.pi f20, f31, f21\n"                            // opcode=0x2f5fba7b
         "fmax.ps f30, f26, f24\n"                             // opcode=0x298d1f7b
         "maskpopcz x30, m5\n"                                 // opcode=0x54028f7b
         "fsrli.pi f27, f24, 16\n"                             // opcode=0x4f0c5dfb
         "fmsub.s f31, f23, f22, f29, rdn\n"                   // opcode=0xe96bafc7
         "fmin.ps f25, f31, f26\n"                             // opcode=0x29af8cfb
         "fmin.s f30, f31, f20\n"                              // opcode=0x294f8f53
         "fnmsub.ps f23, f21, f28, f22, dyn\n"                 // opcode=0xb5cafbdb
         "fnot.pi f26, f29\n"                                  // opcode=0x060ead7b
         "flt.pi f20, f25, f20\n"                              // opcode=0xa74c9a7b
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "fltu.pi f25, f25, f23\n"                             // opcode=0xa77cbcfb
         "frcp.ps f20, f22\n"                                  // opcode=0x587b0a7b
         "fsatu8.pi f28, f22\n"                                // opcode=0x061b3e7b
         "cubeface.ps f26, f23, f30\n"                         // opcode=0x89eb8d7b
         "fmul.pi f21, f29, f20\n"                             // opcode=0x174e8afb
         "feq.s x26, f22, f30\n"                               // opcode=0xa1eb2d53
         "fsgnjx.s f30, f20, f22\n"                            // opcode=0x216a2f53
         "fsra.pi f21, f29, f23\n"                             // opcode=0x0f7edafb
         "for.pi f31, f25, f25\n"                              // opcode=0x079ceffb
         "fpackrepb.pi f31, f27\n"                             // opcode=0x260d8ffb
         "frcp.ps f24, f30\n"                                  // opcode=0x587f0c7b
         "flt.s x20, f23, f23\n"                               // opcode=0xa17b9a53
         "fand.pi f31, f21, f28\n"                             // opcode=0x07cafffb
         "fmaxu.pi f20, f21, f20\n"                            // opcode=0x2f4aba7b
         "maskpopc x26, m7\n"                                  // opcode=0x52038d7b
         "maskxor m2, m0, m1\n"                                // opcode=0x6610417b
         "maskpopc x28, m5\n"                                  // opcode=0x52028e7b
         "feqm.ps m5, f23, f23\n"                              // opcode=0xa17be2fb
         "flt.s x27, f30, f30\n"                               // opcode=0xa1ef1dd3
         "fmul.s f31, f29, f26, dyn\n"                         // opcode=0x11aeffd3
         "fclass.s x29, f30\n"                                 // opcode=0xe00f1ed3
         "fnmadd.ps f20, f31, f26, f29, dyn\n"                 // opcode=0xefaffa5b
         "fsub.ps f30, f21, f21, dyn\n"                        // opcode=0x095aff7b
         "flt.ps f28, f21, f25\n"                              // opcode=0xa19a9e7b
         "fmaxu.pi f21, f28, f23\n"                            // opcode=0x2f7e3afb
         "fsgnjn.s f24, f24, f26\n"                            // opcode=0x21ac1c53
         "flog.ps f22, f30\n"                                  // opcode=0x583f0b7b
         "fsgnjn.s f27, f23, f20\n"                            // opcode=0x214b9dd3
         "fsub.ps f28, f27, f28, dyn\n"                        // opcode=0x09cdfe7b
         "fnmadd.s f31, f24, f25, f20, dyn\n"                  // opcode=0xa19c7fcf
         "fmax.ps f29, f27, f22\n"                             // opcode=0x296d9efb
         "feqm.ps m4, f21, f28\n"                              // opcode=0xa1cae27b
         "fsgnjn.ps f23, f20, f26\n"                           // opcode=0x21aa1bfb
         "fclass.ps f22, f25\n"                                // opcode=0xe00c9b7b
         "feq.s x31, f20, f29\n"                               // opcode=0xa1da2fd3
         //RTLMIN-5479"fsin.ps f31, f25\n"                                  // opcode=0x586c8ffb
         //"flq2 f22, 54(x26)\n"                                 // opcode=0x2b6d5b07
         "fcmov.ps f23, f20, f28, f26\n"                       // opcode=0xd5ca2bbf
         "fmul.ps f28, f25, f25, dyn\n"                        // opcode=0x119cfe7b
         "fltm.ps m2, f27, f28\n"                              // opcode=0xa1cdd17b
         "fsub.pi f24, f20, f29\n"                             // opcode=0x0fda0c7b
         "fclass.ps f27, f31\n"                                // opcode=0xe00f9dfb
         "fmul.s f24, f27, f23, rdn\n"                         // opcode=0x117dac53
         //"fbc.ps f22, 30(x23)\n"                               // opcode=0x79eb8b0b
         "fmin.pi f28, f20, f22\n"                             // opcode=0x2f6a0e7b
         //RTLMIN-5479"fsin.ps f23, f31\n"                                  // opcode=0x586f8bfb
         "maskor m0, m3, m5\n"                                 // opcode=0x6651e07b
         "fpackreph.pi f27, f28\n"                             // opcode=0x260e1dfb
         "fadd.s f29, f31, f31, dyn\n"                         // opcode=0x01fffed3
         "fmaxu.pi f26, f22, f24\n"                            // opcode=0x2f8b3d7b
         "faddi.pi f23, f28, 10\n"                            // opcode=0xccae0bbf
         "feq.ps f29, f25, f20\n"                              // opcode=0xa14caefb
         "flog.ps f25, f22\n"                                  // opcode=0x583b0cfb
         "fbci.ps f31, 459307\n"                               // opcode=0x7022bf9f
         "fle.pi f20, f28, f20\n"                              // opcode=0xa74e0a7b
         "frcp.ps f27, f27\n"                                  // opcode=0x587d8dfb
         "fmin.s f26, f22, f24\n"                              // opcode=0x298b0d53
         "fcmov.ps f25, f25, f21, f31\n"                       // opcode=0xfd5cacbf
         "fcmovm.ps f27, f31, f24\n"                           // opcode=0x018f8df7
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "fmin.pi f21, f30, f26\n"                             // opcode=0x2faf0afb
         "fcmov.ps f23, f28, f26, f21\n"                       // opcode=0xadae2bbf
         "cubesgntc.ps f30, f27, f25\n"                        // opcode=0x899dbf7b
         "fpackrepb.pi f29, f20\n"                             // opcode=0x260a0efb
         "fmulh.pi f23, f20, f23\n"                            // opcode=0x177a2bfb
         "fslli.pi f28, f21, 2\n"                              // opcode=0x4e2a9e7b
         "maskpopcz x22, m4\n"                                 // opcode=0x54020b7b
         "fcmov.ps f26, f24, f24, f26\n"                       // opcode=0xd58c2d3f
         //"fsq2 f31, 5(x25)\n"                                  // opcode=0xf1fcd2a7
         "fmin.s f27, f22, f25\n"                              // opcode=0x299b0dd3
         "maskor m1, m5, m5\n"                                 // opcode=0x6652e0fb
         "fxor.pi f23, f26, f20\n"                             // opcode=0x074d4bfb
         "fsgnjn.s f24, f25, f31\n"                            // opcode=0x21fc9c53
         "fmin.s f27, f22, f29\n"                              // opcode=0x29db0dd3
         "feq.ps f25, f22, f28\n"                              // opcode=0xa1cb2cfb
         "feq.ps f24, f30, f30\n"                              // opcode=0xa1ef2c7b
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "maskxor m5, m4, m2\n"                                // opcode=0x662242fb
         "frcp.ps f20, f21\n"                                  // opcode=0x587a8a7b
         "fle.pi f26, f22, f29\n"                              // opcode=0xa7db0d7b
         "fround.ps f23, f24, dyn\n"                           // opcode=0x581c7bfb
         "fmul.pi f21, f27, f25\n"                             // opcode=0x179d8afb
         "fmul.ps f30, f21, f29, rup\n"                        // opcode=0x11dabf7b
         "fnmadd.s f23, f31, f30, f22, dyn\n"                  // opcode=0xb1effbcf
         "fbci.pi f30, 350882\n"                               // opcode=0x55aa2f5f
         //RTLMIN-5479"fsin.ps f25, f21\n"                                  // opcode=0x586a8cfb
         "flem.ps m3, f20, f25\n"                              // opcode=0xa19a41fb
         "fcmov.ps f31, f26, f22, f24\n"                       // opcode=0xc56d2fbf
         "fminu.pi f24, f31, f24\n"                            // opcode=0x2f8fac7b
         "fle.s x30, f28, f27\n"                               // opcode=0xa1be0f53
         "maskxor m5, m5, m7\n"                                // opcode=0x6672c2fb
         "fnmadd.ps f20, f22, f25, f25, dyn\n"                 // opcode=0xcf9b7a5b
         "fclass.s x25, f30\n"                                 // opcode=0xe00f1cd3
         "fsgnjx.ps f31, f20, f27\n"                           // opcode=0x21ba2ffb
         "fmadd.ps f31, f22, f30, f24, dyn\n"                  // opcode=0xc1eb7fdb
         "fsetm.pi m5, f29\n"                                  // opcode=0xe00ed2fb
         "fsll.pi f28, f30, f26\n"                             // opcode=0x07af1e7b
         "fpackrepb.pi f27, f27\n"                             // opcode=0x260d8dfb
         "fmsub.ps f30, f26, f29, f24, dyn\n"                  // opcode=0xc3dd7f5b
         "fadd.ps f22, f24, f30, dyn\n"                        // opcode=0x01ec7b7b
         "masknot m6, m3\n"                                    // opcode=0x6601a37b
         "fsgnj.s f25, f21, f28\n"                             // opcode=0x21ca8cd3
         "fclass.s x21, f23\n"                                 // opcode=0xe00b9ad3
         "fmulhu.pi f22, f23, f28\n"                           // opcode=0x17cb9b7b
         "flog.ps f20, f31\n"                                  // opcode=0x583f8a7b
         "fmadd.ps f26, f29, f27, f29, dyn\n"                  // opcode=0xe9befd5b
         "fexp.ps f25, f31\n"                                  // opcode=0x584f8cfb
         "feq.ps f24, f23, f27\n"                              // opcode=0xa1bbac7b
         "fslli.pi f31, f20, 11\n"                             // opcode=0x4eba1ffb
         "fsub.s f28, f20, f29, dyn\n"                         // opcode=0x09da7e53
         "feq.ps f21, f26, f26\n"                              // opcode=0xa1ad2afb
         //RTLMIN-5479"frsq.ps f29, f22\n"                                  // opcode=0x588b0efb
         "fpackreph.pi f21, f26\n"                             // opcode=0x260d1afb
         "feq.s x25, f27, f26\n"                               // opcode=0xa1adacd3
         "feq.s x25, f30, f29\n"                               // opcode=0xa1df2cd3
         "fle.s x21, f22, f28\n"                               // opcode=0xa1cb0ad3
         "fsgnjx.s f25, f24, f27\n"                            // opcode=0x21bc2cd3
         "maskpopc x26, m6\n"                                  // opcode=0x52030d7b
         "cubesgnsc.ps f24, f27, f24\n"                        // opcode=0x898dac7b
         "fadd.pi f24, f20, f28\n"                             // opcode=0x07ca0c7b
         "maskxor m5, m3, m1\n"                                // opcode=0x6611c2fb
         //RTLMIN-5479"frsq.ps f25, f26\n"                                  // opcode=0x588d0cfb
         "feq.s x28, f23, f30\n"                               // opcode=0xa1ebae53
         "fexp.ps f23, f23\n"                                  // opcode=0x584b8bfb
         "fmin.ps f24, f29, f31\n"                             // opcode=0x29fe8c7b
         "fclass.ps f31, f30\n"                                // opcode=0xe00f1ffb
         "fmax.ps f28, f27, f30\n"                             // opcode=0x29ed9e7b
         "fcmovm.ps f25, f29, f29\n"                           // opcode=0x01de8cf7
         "fsatu8.pi f26, f29\n"                                // opcode=0x061ebd7b
         "fmaxu.pi f24, f23, f24\n"                            // opcode=0x2f8bbc7b
         //"fbc.ps f23, 65(x24)\n"                               // opcode=0x641c0b8b
         "fmsub.s f29, f26, f25, f23, dyn\n"                   // opcode=0xb99d7ec7
         "fpackreph.pi f20, f24\n"                             // opcode=0x260c1a7b
         "fsgnj.ps f27, f23, f20\n"                            // opcode=0x214b8dfb
         "cubesgntc.ps f23, f21, f28\n"                        // opcode=0x89cabbfb
         "for.pi f28, f22, f30\n"                              // opcode=0x07eb6e7b
         "fmsub.s f22, f26, f29, f27, dyn\n"                   // opcode=0xd9dd7b47
         "fsgnjn.ps f25, f22, f29\n"                           // opcode=0x21db1cfb
         "fmul.ps f20, f26, f20, dyn\n"                        // opcode=0x114d7a7b
         "fbci.pi f28, 169614\n"                               // opcode=0x2968ee5f
         "fmulh.pi f26, f30, f31\n"                            // opcode=0x17ff2d7b
         "fnmsub.s f22, f24, f24, f23, dyn\n"                  // opcode=0xb98c7b4b
         "fbci.pi f24, 362564\n"                               // opcode=0x58844c5f
         "fsatu8.pi f21, f26\n"                                // opcode=0x061d3afb
         "fle.ps f21, f22, f22\n"                              // opcode=0xa16b0afb
         "fmul.s f25, f31, f29, dyn\n"                         // opcode=0x11dffcd3
         "feq.pi f31, f21, f27\n"                              // opcode=0xa7baaffb
         "fand.pi f25, f26, f25\n"                             // opcode=0x079d7cfb
         "fmul.pi f29, f25, f24\n"                             // opcode=0x178c8efb
         "flt.ps f22, f28, f22\n"                              // opcode=0xa16e1b7b
         "fnmsub.ps f29, f20, f24, f21, dyn\n"                 // opcode=0xad8a7edb
         "fxor.pi f22, f27, f23\n"                             // opcode=0x077dcb7b
         "fnot.pi f24, f25\n"                                  // opcode=0x060cac7b
         "fmsub.ps f23, f21, f24, f21, dyn\n"                  // opcode=0xab8afbdb
         "fmax.s f29, f21, f21\n"                              // opcode=0x295a9ed3
         "fmin.s f24, f25, f29\n"                              // opcode=0x29dc8c53
         "fsatu8.pi f31, f29\n"                                // opcode=0x061ebffb
         //"fsq2 f27, 72(x28)\n"                                 // opcode=0xd5be5427
         "fmadd.s f25, f28, f26, f29, dyn\n"                   // opcode=0xe9ae7cc3
         "cubesgnsc.ps f23, f30, f29\n"                        // opcode=0x89df2bfb
         "for.pi f24, f31, f22\n"                              // opcode=0x076fec7b
         "feq.ps f24, f21, f28\n"                              // opcode=0xa1caac7b
         "fmul.pi f28, f26, f26\n"                             // opcode=0x17ad0e7b
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
