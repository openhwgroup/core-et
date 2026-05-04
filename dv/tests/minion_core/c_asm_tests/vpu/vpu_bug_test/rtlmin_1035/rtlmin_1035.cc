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
  *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions
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
         "flt.ps f21, f22, f30\n"                              // opcode=0xa1eb1afb
         "li x2, 0xff\n"
         "mova.m.x x5\n"
         "fsrli.pi f20, f26, 15\n"                             // opcode=0x4efd5a7b
         "fsrli.pi f20, f26, 16\n"                             // opcode=0x4f0d5a7b
         "fle.pi f29, f26, f22\n"                              // opcode=0xa76d0efb
         "flt.s x22, f25, f21\n"                               // opcode=0xa15c9b53
         "flt.pi f20, f20, f28\n"                              // opcode=0xa7ca1a7b
         "cubesgntc.ps f20, f28, f28\n"                        // opcode=0x89ce3a7b
         "fpackreph.pi f22, f30\n"                             // opcode=0x260f1b7b
         "fclass.s x24, f31\n"                                 // opcode=0xe00f9c53
         "cubeface.ps f24, f30, f27\n"                         // opcode=0x89bf0c7b
         "fclass.ps f22, f24\n"                                // opcode=0xe00c1b7b
         "fbci.pi f29, 232309\n"                               // opcode=0x38b75edf
         "fround.ps f29, f28, dyn\n"                           // opcode=0x581e7efb
         "masknot m5, m4\n"                                    // opcode=0x660222fb
         "fsgnjn.s f22, f26, f25\n"                            // opcode=0x219d1b53
         "fcmovm.ps f23, f31, f27\n"                           // opcode=0x01bf8bf7
         "fxor.pi f31, f20, f26\n"                             // opcode=0x07aa4ffb
         "flt.s x23, f21, f24\n"                               // opcode=0xa18a9bd3
         "fsrli.pi f29, f22, 21\n"                             // opcode=0x4f5b5efb
         "fxor.pi f28, f26, f30\n"                             // opcode=0x07ed4e7b
         "fle.s x26, f26, f23\n"                               // opcode=0xa17d0d53
         "fmulh.pi f30, f29, f20\n"                            // opcode=0x174eaf7b
         "fmulh.pi f27, f25, f22\n"                            // opcode=0x176cadfb
         "fbci.ps f25, 531042\n"                               // opcode=0x81a62c9f
         "fsgnj.ps f28, f26, f27\n"                            // opcode=0x21bd0e7b
         "fbcx.ps f22, x22\n"                                  // opcode=0x000b3b0b
         "fmin.pi f29, f22, f23\n"                             // opcode=0x2f7b0efb
         "fandi.pi f26, f21, 157\n"                            // opcode=0x25da9d3f
         "fsgnjx.ps f29, f21, f30\n"                           // opcode=0x21eaaefb
         "fmul.ps f28, f31, f27, rdn\n"                        // opcode=0x11bfae7b
         "fsetm.pi m2, f22\n"                                  // opcode=0xe00b517b
         "fbci.ps f26, 436453\n"                               // opcode=0x6a8e5d1f
         "fcmovm.ps f21, f23, f24\n"                           // opcode=0x018b8af7
         "fmin.pi f26, f24, f25\n"                             // opcode=0x2f9c0d7b
         "fandi.pi f21, f28, 4\n"                            // opcode=0xe48e1abf
         "fcmovm.ps f26, f20, f22\n"                           // opcode=0x016a0d77
         "fxor.pi f27, f31, f26\n"                             // opcode=0x07afcdfb
         "cubesgnsc.ps f28, f21, f22\n"                        // opcode=0x896aae7b
         "cubesgntc.ps f21, f31, f22\n"                        // opcode=0x896fbafb
         "fclass.ps f26, f28\n"                                // opcode=0xe00e1d7b
         "fxor.pi f23, f23, f25\n"                             // opcode=0x079bcbfb
         "mova.m.x x26\n"                                      // opcode=0xd60d107b
         "fsrl.pi f25, f28, f25\n"                             // opcode=0x079e5cfb
         "frcp.ps f26, f27\n"                                  // opcode=0x587d8d7b
         "fmaxu.pi f26, f24, f25\n"                            // opcode=0x2f9c3d7b
         "fsatu8.pi f29, f22\n"                                // opcode=0x061b3efb
         "fxor.pi f28, f29, f28\n"                             // opcode=0x07cece7b
         "fclass.ps f30, f23\n"                                // opcode=0xe00b9f7b
         "maskpopcz x24, m3\n"                                 // opcode=0x54018c7b
         "fmul.ps f30, f21, f24, dyn\n"                        // opcode=0x118aff7b
         "fclass.s x24, f28\n"                                 // opcode=0xe00e1c53
         "fsra.pi f26, f22, f28\n"                             // opcode=0x0fcb5d7b
         "for.pi f26, f25, f27\n"                              // opcode=0x07bced7b
         "maskpopcz x30, m2\n"                                 // opcode=0x54010f7b
         "maskpopc x25, m7\n"                                  // opcode=0x52038cfb
         "fmax.s f30, f31, f30\n"                              // opcode=0x29ef9f53
         "fsrai.pi f25, f26, 28\n"                             // opcode=0x4fcd7cfb
         "fbci.ps f31, 663366\n"                               // opcode=0xa1f46f9f
         "fclass.s x20, f20\n"                                 // opcode=0xe00a1a53
         "fsrai.pi f28, f25, 8\n"                              // opcode=0x4e8cfe7b
         "maskxor m3, m7, m3\n"                                // opcode=0x6633c1fb
         "fsrl.pi f27, f24, f27\n"                             // opcode=0x07bc5dfb
         "fmulh.pi f25, f31, f31\n"                            // opcode=0x17ffacfb
         "flt.ps f29, f30, f28\n"                              // opcode=0xa1cf1efb
         "fmul.ps f31, f30, f24, dyn\n"                        // opcode=0x118f7ffb
         "fmaxu.pi f31, f20, f28\n"                            // opcode=0x2fca3ffb
         "feq.pi f31, f24, f28\n"                              // opcode=0xa7cc2ffb
         "maskand m6, m6, m6\n"                                // opcode=0x6663737b
         "fsrli.pi f20, f24, 11\n"                             // opcode=0x4ebc5a7b
         "maskpopc x21, m2\n"                                  // opcode=0x52010afb
         "fcmovm.ps f30, f27, f21\n"                           // opcode=0x015d8f77
         "fsgnjx.s f27, f22, f25\n"                            // opcode=0x219b2dd3
         "fcmov.ps f22, f22, f23, f20\n"                       // opcode=0xa57b2b3f
         "for.pi f26, f26, f22\n"                              // opcode=0x076d6d7b
         "flt.pi f23, f27, f26\n"                              // opcode=0xa7ad9bfb
         "maskpopc x28, m3\n"                                  // opcode=0x52018e7b
         "fmulhu.pi f26, f20, f30\n"                           // opcode=0x17ea1d7b
         "fround.ps f29, f25, rmm\n"                           // opcode=0x581ccefb
         "maskor m2, m7, m7\n"                                 // opcode=0x6673e17b
         "fmax.s f21, f23, f27\n"                              // opcode=0x29bb9ad3
         "fsra.pi f22, f24, f23\n"                             // opcode=0x0f7c5b7b
         "masknot m5, m6\n"                                    // opcode=0x660322fb
         "feqm.ps m5, f25, f26\n"                              // opcode=0xa1ace2fb
         "fsetm.pi m6, f22\n"                                  // opcode=0xe00b537b
         "maskand m2, m0, m0\n"                                // opcode=0x6600717b
         "fsgnjn.ps f30, f31, f23\n"                           // opcode=0x217f9f7b
         "fbci.ps f20, 794756\n"                               // opcode=0xc2084a1f
         "fclass.s x21, f25\n"                                 // opcode=0xe00c9ad3
         "fsetm.pi m5, f20\n"                                  // opcode=0xe00a52fb
         "fmax.pi f29, f29, f25\n"                             // opcode=0x2f9e9efb
         "maskor m6, m1, m4\n"                                 // opcode=0x6640e37b
         "fpackrepb.pi f28, f26\n"                             // opcode=0x260d0e7b
         "maskpopc x31, m3\n"                                  // opcode=0x52018ffb
         "fsgnjn.ps f28, f23, f28\n"                           // opcode=0x21cb9e7b
         "fsgnjx.ps f20, f30, f29\n"                           // opcode=0x21df2a7b
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "cubefaceidx.ps f22, f20, f30\n"                      // opcode=0x89ea1b7b
         "fclass.s x26, f26\n"                                 // opcode=0xe00d1d53
         "masknot m2, m3\n"                                    // opcode=0x6601a17b
         "fpackreph.pi f21, f27\n"                             // opcode=0x260d9afb
         "fsatu8.pi f31, f21\n"                                // opcode=0x061abffb
         "li x2, 0xff\n"
         "mova.m.x x5\n"
         "fandi.pi f28, f24, 54\n"                            // opcode=0xedac1e3f
         "fsgnjn.s f27, f26, f22\n"                            // opcode=0x216d1dd3
         "fpackreph.pi f29, f23\n"                             // opcode=0x260b9efb
         "fle.s x29, f25, f28\n"                               // opcode=0xa1cc8ed3
         "maskpopcz x29, m4\n"                                 // opcode=0x54020efb
         "fsgnjx.s f21, f23, f23\n"                            // opcode=0x217baad3
         "fle.pi f30, f24, f31\n"                              // opcode=0xa7fc0f7b
         "feq.pi f24, f31, f28\n"                              // opcode=0xa7cfac7b
         "fmulhu.pi f26, f24, f28\n"                           // opcode=0x17cc1d7b
         "fmax.ps f28, f22, f23\n"                             // opcode=0x297b1e7b
         "fsra.pi f25, f23, f28\n"                             // opcode=0x0fcbdcfb
         "maskor m2, m1, m5\n"                                 // opcode=0x6650e17b
         //"fbc.ps f31, 68(x31)\n"                               // opcode=0x744f8f8b
         "fclass.ps f30, f27\n"                                // opcode=0xe00d9f7b
         "fand.pi f27, f26, f30\n"                             // opcode=0x07ed7dfb
         "fsrli.pi f26, f30, 19\n"                             // opcode=0x4f3f5d7b
         //"fbc.ps f21, 4(x28)\n"                                // opcode=0x804e0a8b
         "fpackrepb.pi f25, f25\n"                             // opcode=0x260c8cfb
         "fbcx.ps f20, x20\n"                                  // opcode=0x000a3a0b
         "fmulhu.pi f20, f23, f30\n"                           // opcode=0x17eb9a7b
         "feqm.ps m7, f21, f27\n"                              // opcode=0xa1bae3fb
         "flt.s x29, f22, f30\n"                               // opcode=0xa1eb1ed3
         "cubefaceidx.ps f20, f23, f26\n"                      // opcode=0x89ab9a7b
         "fsrl.pi f21, f29, f29\n"                             // opcode=0x07dedafb
         "feq.pi f23, f23, f30\n"                              // opcode=0xa7ebabfb
         "fand.pi f27, f28, f20\n"                             // opcode=0x074e7dfb
         "mova.m.x x23\n"                                      // opcode=0xd60b907b
         "fmulh.pi f28, f20, f26\n"                            // opcode=0x17aa2e7b
         "cubefaceidx.ps f21, f20, f23\n"                      // opcode=0x897a1afb
         "maskor m3, m7, m0\n"                                 // opcode=0x6603e1fb
         "fbcx.ps f21, x20\n"                                  // opcode=0x000a3a8b
         "for.pi f20, f22, f24\n"                              // opcode=0x078b6a7b
         "fsgnjn.ps f22, f28, f28\n"                           // opcode=0x21ce1b7b
         "flt.pi f22, f28, f29\n"                              // opcode=0xa7de1b7b
         "fsrl.pi f26, f29, f29\n"                             // opcode=0x07dedd7b
         "feq.s x23, f26, f29\n"                               // opcode=0xa1dd2bd3
         "fmaxu.pi f20, f23, f20\n"                            // opcode=0x2f4bba7b
         "maskpopcz x22, m4\n"                                 // opcode=0x54020b7b
         "fbci.ps f29, 360675\n"                               // opcode=0x580e3e9f
         "flt.pi f22, f22, f29\n"                              // opcode=0xa7db1b7b
         "fbci.ps f25, 267690\n"                               // opcode=0x415aac9f
         "fmul.pi f25, f26, f28\n"                             // opcode=0x17cd0cfb
         "masknot m6, m0\n"                                    // opcode=0x6600237b
         "maskxor m4, m2, m4\n"                                // opcode=0x6641427b
         "fsll.pi f25, f29, f24\n"                             // opcode=0x078e9cfb
         "cubeface.ps f25, f21, f30\n"                         // opcode=0x89ea8cfb
         "fbci.pi f23, 318329\n"                               // opcode=0x4db79bdf
         "fsgnj.s f23, f30, f28\n"                             // opcode=0x21cf0bd3
         "fmin.s f22, f25, f30\n"                              // opcode=0x29ec8b53
         "fsll.pi f29, f31, f26\n"                             // opcode=0x07af9efb
         "fle.s x23, f27, f21\n"                               // opcode=0xa15d8bd3
         "for.pi f22, f24, f30\n"                              // opcode=0x07ec6b7b
         "feq.s x21, f26, f24\n"                               // opcode=0xa18d2ad3
         "fsgnjn.ps f29, f30, f23\n"                           // opcode=0x217f1efb
         "fbcx.ps f26, x22\n"                                  // opcode=0x000b3d0b
         "fmulhu.pi f28, f29, f23\n"                           // opcode=0x177e9e7b
         "fmaxu.pi f22, f24, f30\n"                            // opcode=0x2fec3b7b
         "fsrli.pi f30, f21, 24\n"                             // opcode=0x4f8adf7b
         "fmulh.pi f23, f25, f25\n"                            // opcode=0x179cabfb
         "fmax.ps f30, f22, f31\n"                             // opcode=0x29fb1f7b
         "fsrl.pi f30, f24, f30\n"                             // opcode=0x07ec5f7b
         "fcmov.ps f24, f27, f31, f26\n"                       // opcode=0xd5fdac3f
         "fmax.ps f22, f31, f27\n"                             // opcode=0x29bf9b7b
         "fsgnjn.s f22, f21, f27\n"                            // opcode=0x21ba9b53
         "fsetm.pi m2, f29\n"                                  // opcode=0xe00ed17b
         "fltu.pi f23, f26, f27\n"                             // opcode=0xa7bd3bfb
         "fmaxu.pi f20, f24, f23\n"                            // opcode=0x2f7c3a7b
         "fsgnjn.ps f20, f30, f31\n"                           // opcode=0x21ff1a7b
         "maskpopc x22, m1\n"                                  // opcode=0x52008b7b
         "fsgnjn.ps f23, f23, f31\n"                           // opcode=0x21fb9bfb
         "fmulh.pi f23, f30, f28\n"                            // opcode=0x17cf2bfb
         "for.pi f31, f23, f29\n"                              // opcode=0x07dbeffb
         "fmin.s f27, f26, f30\n"                              // opcode=0x29ed0dd3
         "flt.s x25, f22, f21\n"                               // opcode=0xa15b1cd3
         "feqm.ps m2, f22, f22\n"                              // opcode=0xa16b617b
         "fsgnj.ps f26, f30, f29\n"                            // opcode=0x21df0d7b
         "fbci.pi f30, 520536\n"                               // opcode=0x7f158f5f
         "flt.pi f28, f20, f23\n"                              // opcode=0xa77a1e7b
         "fsgnj.ps f25, f20, f24\n"                            // opcode=0x218a0cfb
         "fpackrepb.pi f20, f21\n"                             // opcode=0x260a8a7b
         "fsgnjx.ps f30, f23, f22\n"                           // opcode=0x216baf7b
         "frcp.ps f25, f28\n"                                  // opcode=0x587e0cfb
         "flt.s x27, f28, f22\n"                               // opcode=0xa16e1dd3
         "fsgnjn.ps f23, f30, f21\n"                           // opcode=0x215f1bfb
         "fbci.ps f28, 243917\n"                               // opcode=0x3b8cde1f
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "cubefaceidx.ps f25, f30, f23\n"                      // opcode=0x897f1cfb
         "fslli.pi f30, f31, 18\n"                             // opcode=0x4f2f9f7b
         "fmin.ps f23, f26, f26\n"                             // opcode=0x29ad0bfb
         "flt.ps f29, f27, f20\n"                              // opcode=0xa14d9efb
         "fbci.pi f24, 139405\n"                               // opcode=0x2208dc5f
         "cubefaceidx.ps f20, f23, f29\n"                      // opcode=0x89db9a7b
         "cubesgnsc.ps f24, f23, f26\n"                        // opcode=0x89abac7b
         "fxor.pi f28, f29, f25\n"                             // opcode=0x079ece7b
         "fsgnjx.s f20, f26, f24\n"                            // opcode=0x218d2a53
         "fmulhu.pi f22, f27, f25\n"                           // opcode=0x179d9b7b
         "fle.pi f25, f30, f29\n"                              // opcode=0xa7df0cfb
         "fsgnjx.s f28, f23, f23\n"                            // opcode=0x217bae53
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         "cubesgntc.ps f25, f30, f20\n"                        // opcode=0x894f3cfb
         "li x2, 0xff\n"
         "mova.m.x x5\n"
         "cubefaceidx.ps f22, f21, f27\n"                      // opcode=0x89ba9b7b
         "fslli.pi f27, f24, 29\n"                             // opcode=0x4fdc1dfb
         "fand.pi f29, f21, f30\n"                             // opcode=0x07eafefb
         "cubesgnsc.ps f28, f23, f20\n"                        // opcode=0x894bae7b
         "mova.x.m x26\n"                                      // opcode=0xd6000d7b
         "cubefaceidx.ps f23, f27, f31\n"                      // opcode=0x89fd9bfb
         "fsgnj.ps f29, f24, f31\n"                            // opcode=0x21fc0efb
         "fsgnjn.ps f28, f31, f20\n"                           // opcode=0x214f9e7b
         "fltu.pi f31, f24, f20\n"                             // opcode=0xa74c3ffb
         "fslli.pi f21, f27, 0\n"                              // opcode=0x4e0d9afb
         "fsatu8.pi f30, f28\n"                                // opcode=0x061e3f7b
         //"fbc.ps f22, 1(x21)\n"                                // opcode=0xf81a8b0b
         "flt.ps f24, f26, f27\n"                              // opcode=0xa1bd1c7b
         "fmul.ps f21, f24, f20, rne\n"                        // opcode=0x114c0afb
         "fround.ps f26, f27, dyn\n"                           // opcode=0x581dfd7b
         "feq.s x22, f27, f31\n"                               // opcode=0xa1fdab53
         "maskpopcz x25, m0\n"                                 // opcode=0x54000cfb
         "fsrl.pi f29, f30, f28\n"                             // opcode=0x07cf5efb
         "fround.ps f25, f26, dyn\n"                           // opcode=0x581d7cfb
         "fbcx.ps f28, x26\n"                                  // opcode=0x000d3e0b
         "fmulh.pi f28, f20, f21\n"                            // opcode=0x175a2e7b
         "fround.ps f23, f29, rmm\n"                           // opcode=0x581ecbfb
         "fsll.pi f21, f20, f30\n"                             // opcode=0x07ea1afb
         "fsgnj.s f23, f30, f27\n"                             // opcode=0x21bf0bd3
         "fsgnjn.s f21, f23, f30\n"                            // opcode=0x21eb9ad3
         "fmin.ps f27, f25, f23\n"                             // opcode=0x297c8dfb
         "cubefaceidx.ps f24, f24, f31\n"                      // opcode=0x89fc1c7b
         "fsll.pi f25, f29, f20\n"                             // opcode=0x074e9cfb
         "fle.s x24, f21, f20\n"                               // opcode=0xa14a8c53
         "fmax.ps f30, f24, f26\n"                             // opcode=0x29ac1f7b
         "cubesgntc.ps f22, f30, f28\n"                        // opcode=0x89cf3b7b
         "fxor.pi f23, f29, f27\n"                             // opcode=0x07becbfb
         "maskpopc x21, m7\n"                                  // opcode=0x52038afb
         "fsra.pi f30, f28, f28\n"                             // opcode=0x0fce5f7b
         "feq.ps f31, f27, f30\n"                              // opcode=0xa1edaffb
         "fsrl.pi f21, f23, f25\n"                             // opcode=0x079bdafb
         "fmin.pi f31, f30, f21\n"                             // opcode=0x2f5f0ffb
         "fmax.ps f21, f27, f27\n"                             // opcode=0x29bd9afb
         "fmin.s f27, f22, f30\n"                              // opcode=0x29eb0dd3
         "fcmovm.ps f30, f22, f27\n"                           // opcode=0x01bb0f77
         "cubesgntc.ps f31, f26, f23\n"                        // opcode=0x897d3ffb
         "masknot m2, m7\n"                                    // opcode=0x6603a17b
         "fsrli.pi f24, f26, 10\n"                             // opcode=0x4ead5c7b
         "fsatu8.pi f24, f21\n"                                // opcode=0x061abc7b
         "fclass.ps f21, f21\n"                                // opcode=0xe00a9afb
         "fsgnjx.ps f25, f27, f28\n"                           // opcode=0x21cdacfb
         "cubesgnsc.ps f25, f25, f26\n"                        // opcode=0x89acacfb
         "fmax.s f22, f30, f26\n"                              // opcode=0x29af1b53
         "fltu.pi f28, f30, f21\n"                             // opcode=0xa75f3e7b
         "feq.pi f25, f31, f26\n"                              // opcode=0xa7afacfb
         "fsll.pi f28, f22, f31\n"                             // opcode=0x07fb1e7b
         /*		"fsgnj.ps f20, f22, f21\n"                            // opcode=0x215b0a7b
                           "fmul.ps f26, f23, f22, dyn\n"                        // opcode=0x116bfd7b
                           "fslli.pi f22, f31, 5\n"                              // opcode=0x4e5f9b7b
                           "fsgnj.ps f26, f23, f24\n"                            // opcode=0x218b8d7b
                           "mova.m.x x20\n"                                      // opcode=0xd60a107b
                           "flt.s x25, f27, f27\n"                               // opcode=0xa1bd9cd3
                           "mova.m.x x20\n"                                      // opcode=0xd60a107b
                           "fmul.s f27, f27, f26, dyn\n"                         // opcode=0x11adfdd3
                           "mova.x.m x28\n"                                      // opcode=0xd6000e7b
                           "cubeface.ps f24, f31, f25\n"                         // opcode=0x899f8c7b
                           "fpackrepb.pi f28, f20\n"                             // opcode=0x260a0e7b
                           "fpackreph.pi f20, f30\n"                             // opcode=0x260f1a7b
                           "maskor m2, m2, m7\n"                                 // opcode=0x6671617b
                           "feq.pi f22, f28, f31\n"                              // opcode=0xa7fe2b7b
                           "fslli.pi f22, f25, 24\n"                             // opcode=0x4f8c9b7b
                           "fmul.s f21, f24, f20, dyn\n"                         // opcode=0x114c7ad3
                           "fsgnj.s f21, f31, f20\n"                             // opcode=0x214f8ad3
                           "fle.s x21, f28, f23\n"                               // opcode=0xa17e0ad3
                           "fpackreph.pi f20, f27\n"                             // opcode=0x260d9a7b
                           "fslli.pi f24, f27, 12\n"                             // opcode=0x4ecd9c7b
                           "cubeface.ps f26, f27, f25\n"                         // opcode=0x899d8d7b
                           "fbcx.ps f26, x21\n"                                  // opcode=0x000abd0b
                           "fsgnjn.s f31, f29, f31\n"                            // opcode=0x21fe9fd3
                           "fsrli.pi f23, f20, 9\n"                              // opcode=0x4e9a5bfb
                           "fmax.s f25, f26, f23\n"                              // opcode=0x297d1cd3
                           "fmulh.pi f22, f26, f31\n"                            // opcode=0x17fd2b7b
                           "fmul.s f29, f24, f27, dyn\n"                         // opcode=0x11bc7ed3
                           "fcmovm.ps f20, f23, f28\n"                           // opcode=0x01cb8a77
                           "cubesgnsc.ps f20, f22, f26\n"                        // opcode=0x89ab2a7b
                           "fmin.s f27, f23, f21\n"                              // opcode=0x295b8dd3
                           "fbc.ps f27, 44(x29)\n"                               // opcode=0x32ce8d8b
                           "fsgnjn.ps f24, f28, f23\n"                           // opcode=0x217e1c7b
                           "fsgnj.ps f24, f30, f23\n"                            // opcode=0x217f0c7b
                           "flt.ps f29, f27, f31\n"                              // opcode=0xa1fd9efb
                           "cubesgnsc.ps f26, f31, f21\n"                        // opcode=0x895fad7b
                           "maskpopcz x24, m7\n"                                 // opcode=0x54038c7b
                           "masknot m5, m2\n"                                    // opcode=0x660122fb
                           "fsrl.pi f21, f31, f23\n"                             // opcode=0x077fdafb
                           "masknot m6, m1\n"                                    // opcode=0x6600a37b
                           "fmul.pi f20, f23, f28\n"                             // opcode=0x17cb8a7b
                           "fandi.pi f30, f29, 277\n"                            // opcode=0x455e9f3f
                           "fmax.ps f29, f25, f26\n"                             // opcode=0x29ac9efb
                           "maskpopcz x25, m0\n"                                 // opcode=0x54000cfb
                           "fsra.pi f24, f20, f31\n"                             // opcode=0x0ffa5c7b
                           "fsatu8.pi f26, f31\n"                                // opcode=0x061fbd7b
                           "fmul.ps f26, f30, f22, dyn\n"                        // opcode=0x116f7d7b
                           "mova.x.m x23\n"                                      // opcode=0xd6000bfb
                           "flt.pi f23, f31, f29\n"                              // opcode=0xa7df9bfb
                           "fmaxu.pi f27, f30, f31\n"                            // opcode=0x2fff3dfb
                           "li x2, 0xff\n"
                           "mova.m.x x5\n"
                           "flt.pi f22, f21, f31\n"                              // opcode=0xa7fa9b7b
                           "maskpopcz x27, m3\n"                                 // opcode=0x54018dfb
                           "fandi.pi f28, f27, 999\n"                            // opcode=0xfc7d9e3f
                           "mova.x.m x27\n"                                      // opcode=0xd6000dfb
                           "fle.pi f31, f22, f22\n"                              // opcode=0xa76b0ffb
                           "fsgnjn.s f31, f31, f22\n"                            // opcode=0x216f9fd3
                           "flt.ps f28, f27, f29\n"                              // opcode=0xa1dd9e7b
                           "fsgnj.s f20, f24, f20\n"                             // opcode=0x214c0a53
                           "fandi.pi f30, f23, 613\n"                            // opcode=0x9c5b9f3f
                           "fsgnj.ps f28, f21, f25\n"                            // opcode=0x219a8e7b
                           "fsgnjn.ps f24, f24, f23\n"                           // opcode=0x217c1c7b
                           "fsrli.pi f23, f24, 16\n"                             // opcode=0x4f0c5bfb
                           "fxor.pi f29, f25, f27\n"                             // opcode=0x07bccefb
                           "maskxor m3, m2, m4\n"                                // opcode=0x664141fb
                           "fsetm.pi m0, f24\n"                                  // opcode=0xe00c507b
                           "fle.s x22, f23, f22\n"                               // opcode=0xa16b8b53
                           "fandi.pi f23, f28, 120\n"                            // opcode=0x1d8e1bbf
                           "fsgnj.ps f24, f21, f30\n"                            // opcode=0x21ea8c7b
                           "fminu.pi f24, f31, f26\n"                            // opcode=0x2fafac7b
                           "fmaxu.pi f28, f28, f31\n"                            // opcode=0x2ffe3e7b
         "fcmov.ps f29, f29, f27, f23\n"                       // opcode=0xbdbeaebf
         "fsrai.pi f21, f21, 3\n"                              // opcode=0x4e3afafb
         "fsetm.pi m7, f27\n"                                  // opcode=0xe00dd3fb
         "cubeface.ps f26, f29, f27\n"                         // opcode=0x89be8d7b
         "fmax.s f25, f24, f23\n"                              // opcode=0x297c1cd3
         "fsgnjx.ps f26, f30, f30\n"                           // opcode=0x21ef2d7b
         "fmul.pi f25, f29, f28\n"                             // opcode=0x17ce8cfb
         "ffrc.ps f25, f27\n"                                  // opcode=0x582d8cfb
         "fround.ps f31, f29, rup\n"                           // opcode=0x581ebffb
         "fmaxu.pi f23, f21, f23\n"                            // opcode=0x2f7abbfb
         "fle.s x26, f26, f30\n"                               // opcode=0xa1ed0d53
         "fsll.pi f20, f29, f27\n"                             // opcode=0x07be9a7b
         "fpackrepb.pi f22, f23\n"                             // opcode=0x260b8b7b
         "feqm.ps m7, f23, f26\n"                              // opcode=0xa1abe3fb
         "flt.ps f23, f22, f22\n"                              // opcode=0xa16b1bfb
         "fpackrepb.pi f28, f21\n"                             // opcode=0x260a8e7b
         "cubefaceidx.ps f22, f26, f25\n"                      // opcode=0x899d1b7b
         "fsatu8.pi f21, f27\n"                                // opcode=0x061dbafb
         "fpackreph.pi f26, f22\n"                             // opcode=0x260b1d7b
         "fmin.ps f21, f30, f26\n"                             // opcode=0x29af0afb
         "for.pi f31, f24, f25\n"                              // opcode=0x079c6ffb
         "frcp.ps f23, f25\n"                                  // opcode=0x587c8bfb
         "frcp.ps f27, f25\n"                                  // opcode=0x587c8dfb
         "fbci.pi f24, 399737\n"                               // opcode=0x61979c5f
         "fsat8.pi f22, f21\n"                                 // opcode=0x060abb7b
         "fbci.pi f30, 470966\n"                               // opcode=0x72fb6f5f
         "fmin.s f27, f29, f29\n"                              // opcode=0x29de8dd3
         "fpackrepb.pi f20, f27\n"                             // opcode=0x260d8a7b
         "fmul.pi f20, f31, f21\n"                             // opcode=0x175f8a7b
         "fsat8.pi f28, f29\n"                                 // opcode=0x060ebe7b
         "fsgnjn.s f28, f20, f29\n"                            // opcode=0x21da1e53
         "fround.ps f25, f21, dyn\n"                           // opcode=0x581afcfb
         "fsrli.pi f23, f27, 12\n"                             // opcode=0x4ecddbfb
         "feq.s x26, f30, f20\n"                               // opcode=0xa14f2d53
         "cubefaceidx.ps f29, f27, f26\n"                      // opcode=0x89ad9efb
         "feqm.ps m7, f27, f24\n"                              // opcode=0xa18de3fb
         "fbcx.ps f27, x27\n"                                  // opcode=0x000dbd8b
         "fmaxu.pi f31, f24, f30\n"                            // opcode=0x2fec3ffb
         "frcp.ps f31, f26\n"                                  // opcode=0x587d0ffb
         "flt.ps f30, f29, f26\n"                              // opcode=0xa1ae9f7b
         "fsll.pi f30, f21, f27\n"                             // opcode=0x07ba9f7b
         "fsgnjx.s f26, f23, f31\n"                            // opcode=0x21fbad53
         "fmul.pi f25, f20, f21\n"                             // opcode=0x175a0cfb
         "fmax.ps f24, f26, f21\n"                             // opcode=0x295d1c7b
         "cubesgntc.ps f21, f21, f25\n"                        // opcode=0x899abafb
         "fltu.pi f22, f21, f23\n"                             // opcode=0xa77abb7b
         "maskxor m0, m2, m3\n"                                // opcode=0x6631407b
         "fclass.s x25, f31\n"                                 // opcode=0xe00f9cd3
         "fmin.pi f21, f28, f25\n"                             // opcode=0x2f9e0afb
         "fnot.pi f21, f29\n"                                  // opcode=0x060eaafb
         "fmin.ps f23, f22, f24\n"                             // opcode=0x298b0bfb
         "fsetm.pi m2, f21\n"                                  // opcode=0xe00ad17b
         "fsra.pi f28, f26, f29\n"                             // opcode=0x0fdd5e7b
         "feq.s x28, f30, f31\n"                               // opcode=0xa1ff2e53
         "cubefaceidx.ps f22, f28, f25\n"                      // opcode=0x899e1b7b
         "fmulh.pi f24, f23, f30\n"                            // opcode=0x17ebac7b
         "mova.m.x x20\n"                                      // opcode=0xd60a107b
         "fnot.pi f21, f20\n"                                  // opcode=0x060a2afb
         "fandi.pi f27, f22, 370\n"                            // opcode=0x5d2b1dbf
         "fsat8.pi f22, f25\n"                                 // opcode=0x060cbb7b
         "fand.pi f20, f27, f27\n"                             // opcode=0x07bdfa7b
         "fcmov.ps f31, f28, f22, f21\n"                       // opcode=0xad6e2fbf
         "fsgnjx.ps f30, f22, f25\n"                           // opcode=0x219b2f7b
         "fxor.pi f22, f30, f31\n"                             // opcode=0x07ff4b7b
         "flt.pi f29, f22, f23\n"                              // opcode=0xa77b1efb
         "fsatu8.pi f23, f31\n"                                // opcode=0x061fbbfb
         "fsat8.pi f23, f29\n"                                 // opcode=0x060ebbfb
         "fmul.ps f29, f20, f26, dyn\n"                        // opcode=0x11aa7efb
         "ffrc.ps f29, f29\n"                                  // opcode=0x582e8efb
         "fbcx.ps f23, x23\n"                                  // opcode=0x000bbb8b
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         "fsgnjn.s f20, f31, f21\n"                            // opcode=0x215f9a53
         "ffrc.ps f20, f21\n"                                  // opcode=0x582a8a7b
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "frcp.ps f20, f31\n"                                  // opcode=0x587f8a7b
         "fmax.pi f22, f26, f24\n"                             // opcode=0x2f8d1b7b
         "fcmovm.ps f25, f30, f29\n"                           // opcode=0x01df0cf7
         "fbcx.ps f22, x20\n"                                  // opcode=0x000a3b0b
         "fle.ps f31, f25, f29\n"                              // opcode=0xa1dc8ffb
         "fbc.ps f23, 75(x27)\n"                               // opcode=0x2cbd8b8b
         "li x2, 0xff\n"
         "mova.m.x x5\n"
         "fpackrepb.pi f20, f20\n"                             // opcode=0x260a0a7b
         "fsgnj.s f21, f20, f29\n"                             // opcode=0x21da0ad3
         "cubesgntc.ps f24, f26, f24\n"                        // opcode=0x898d3c7b
         "for.pi f31, f23, f28\n"                              // opcode=0x07cbeffb
         "fsrai.pi f23, f25, 1\n"                              // opcode=0x4e1cfbfb
         "cubesgnsc.ps f24, f26, f20\n"                        // opcode=0x894d2c7b
         "fmul.s f31, f26, f21, dyn\n"                         // opcode=0x115d7fd3
         "fnot.pi f29, f28\n"                                  // opcode=0x060e2efb
         "fxor.pi f24, f24, f29\n"                             // opcode=0x07dc4c7b
         "masknot m1, m5\n"                                    // opcode=0x6602a0fb
         "fsatu8.pi f23, f27\n"                                // opcode=0x061dbbfb
         "fxor.pi f30, f24, f27\n"                             // opcode=0x07bc4f7b
         "fround.ps f28, f31, dyn\n"                           // opcode=0x581ffe7b
         "fsatu8.pi f31, f22\n"                                // opcode=0x061b3ffb
         "fnot.pi f27, f30\n"                                  // opcode=0x060f2dfb
         "fsat8.pi f29, f23\n"                                 // opcode=0x060bbefb
         "fsetm.pi m2, f21\n"                                  // opcode=0xe00ad17b
         "cubefaceidx.ps f28, f23, f23\n"                      // opcode=0x897b9e7b
         "fmax.s f25, f25, f21\n"                              // opcode=0x295c9cd3
         "fand.pi f21, f31, f24\n"                             // opcode=0x078ffafb
         "feq.pi f29, f23, f30\n"                              // opcode=0xa7ebaefb
         "cubesgnsc.ps f26, f31, f22\n"                        // opcode=0x896fad7b
         "cubeface.ps f26, f28, f29\n"                         // opcode=0x89de0d7b
         "flt.ps f21, f31, f22\n"                              // opcode=0xa16f9afb
         "fbcx.ps f22, x29\n"                                  // opcode=0x000ebb0b
         "fbcx.ps f30, x28\n"                                  // opcode=0x000e3f0b
         "maskand m3, m0, m2\n"                                // opcode=0x662071fb
         "fsrli.pi f26, f20, 0\n"                              // opcode=0x4e0a5d7b
         "fle.ps f23, f23, f22\n"                              // opcode=0xa16b8bfb
         "fle.ps f26, f30, f31\n"                              // opcode=0xa1ff0d7b
         "fslli.pi f26, f31, 25\n"                             // opcode=0x4f9f9d7b
         "fand.pi f28, f30, f22\n"                             // opcode=0x076f7e7b
         "feqm.ps m1, f24, f26\n"                              // opcode=0xa1ac60fb
         "fsra.pi f29, f29, f26\n"                             // opcode=0x0faedefb
         "fmulh.pi f28, f20, f23\n"                            // opcode=0x177a2e7b
         "maskor m0, m0, m4\n"                                 // opcode=0x6640607b
         "feq.ps f26, f23, f30\n"                              // opcode=0xa1ebad7b
         "fpackreph.pi f24, f29\n"                             // opcode=0x260e9c7b
         "fnot.pi f22, f29\n"                                  // opcode=0x060eab7b
         "fandi.pi f24, f27, 376\n"                            // opcode=0x5d8d9c3f
         "fle.s x27, f26, f26\n"                               // opcode=0xa1ad0dd3
         "fmul.ps f24, f20, f20, dyn\n"                        // opcode=0x114a7c7b
         "maskor m2, m2, m1\n"                                 // opcode=0x6611617b
         "maskand m0, m6, m7\n"                                // opcode=0x6673707b
         "fsrai.pi f22, f25, 1\n"                              // opcode=0x4e1cfb7b
         "fclass.ps f22, f22\n"                                // opcode=0xe00b1b7b
         "maskxor m3, m2, m2\n"                                // opcode=0x662141fb
         "feq.s x23, f27, f23\n"                               // opcode=0xa17dabd3
         "fpackreph.pi f23, f26\n"                             // opcode=0x260d1bfb
         "fpackrepb.pi f27, f29\n"                             // opcode=0x260e8dfb
         "fmax.pi f28, f28, f30\n"                             // opcode=0x2fee1e7b
         "fmulh.pi f22, f20, f23\n"                            // opcode=0x177a2b7b
         "fle.ps f25, f22, f29\n"                              // opcode=0xa1db0cfb
         "fand.pi f27, f23, f22\n"                             // opcode=0x076bfdfb
         "fbci.ps f23, 957685\n"                               // opcode=0xe9cf5b9f
         "feq.s x21, f28, f27\n"                               // opcode=0xa1be2ad3
         "fsgnjx.s f24, f25, f24\n"                            // opcode=0x218cac53
         "maskpopcz x30, m1\n"                                 // opcode=0x54008f7b
         "maskpopcz x26, m3\n"                                 // opcode=0x54018d7b
         "fle.ps f25, f28, f30\n"                              // opcode=0xa1ee0cfb
         "fmul.ps f21, f25, f29, dyn\n"                        // opcode=0x11dcfafb
         "feq.ps f23, f25, f22\n"                              // opcode=0xa16cabfb
         "fmax.pi f25, f30, f29\n"                             // opcode=0x2fdf1cfb
         "fsgnj.s f29, f29, f22\n"                             // opcode=0x216e8ed3
         "maskpopc x24, m0\n"                                  // opcode=0x52000c7b
         "fsgnjn.ps f23, f30, f21\n"                           // opcode=0x215f1bfb
         "cubesgntc.ps f22, f30, f22\n"                        // opcode=0x896f3b7b
         "fround.ps f20, f30, dyn\n"                           // opcode=0x581f7a7b
         "for.pi f23, f23, f22\n"                              // opcode=0x076bebfb
         "maskor m7, m3, m7\n"                                 // opcode=0x6671e3fb
         "for.pi f31, f30, f25\n"                              // opcode=0x079f6ffb
         "fpackreph.pi f24, f23\n"                             // opcode=0x260b9c7b
         "fmax.s f23, f20, f25\n"                              // opcode=0x299a1bd3
         "fsrl.pi f27, f27, f27\n"                             // opcode=0x07bdddfb
         "fsrai.pi f28, f21, 20\n"                             // opcode=0x4f4afe7b
         "mova.x.m x27\n"                                      // opcode=0xd6000dfb
         "fsatu8.pi f31, f30\n"                                // opcode=0x061f3ffb
         "for.pi f30, f29, f28\n"                              // opcode=0x07ceef7b
         "maskor m1, m5, m5\n"                                 // opcode=0x6652e0fb
         "fsrai.pi f20, f23, 24\n"                             // opcode=0x4f8bfa7b
         "fsgnjn.ps f30, f20, f21\n"                           // opcode=0x215a1f7b
         "fcmov.ps f28, f24, f21, f26\n"                       // opcode=0xd55c2e3f
         "flt.ps f25, f31, f20\n"                              // opcode=0xa14f9cfb
         "fsgnjx.s f27, f23, f27\n"                            // opcode=0x21bbadd3
         "fsrai.pi f30, f22, 3\n"                              // opcode=0x4e3b7f7b
         "fclass.ps f24, f27\n"                                // opcode=0xe00d9c7b
         "cubeface.ps f26, f30, f30\n"                         // opcode=0x89ef0d7b
         "maskpopc x29, m5\n"                                  // opcode=0x52028efb
         "fle.ps f23, f27, f25\n"                              // opcode=0xa19d8bfb
         "fsgnjn.ps f21, f23, f24\n"                           // opcode=0x218b9afb
         "maskpopc x24, m6\n"                                  // opcode=0x52030c7b
         "mova.m.x x31\n"                                      // opcode=0xd60f907b
         "fpackrepb.pi f27, f25\n"                             // opcode=0x260c8dfb
         "fsgnjn.ps f23, f30, f28\n"                           // opcode=0x21cf1bfb
         "fsgnjn.ps f26, f26, f21\n"                           // opcode=0x215d1d7b
         "fsgnjx.s f25, f28, f26\n"                            // opcode=0x21ae2cd3
         "fmin.s f25, f25, f28\n"                              // opcode=0x29cc8cd3
         "fclass.ps f27, f22\n"                                // opcode=0xe00b1dfb
         "fsra.pi f25, f20, f29\n"                             // opcode=0x0fda5cfb
         "fandi.pi f29, f24, 219\n"                            // opcode=0x35bc1ebf
         "li x2, 0xff\n"
         "mova.m.x x5\n"
         "fsatu8.pi f23, f20\n"                                // opcode=0x061a3bfb
         "fmin.ps f20, f30, f29\n"                             // opcode=0x29df0a7b
         "maskpopcz x31, m2\n"                                 // opcode=0x54010ffb
         "fcmovm.ps f21, f20, f25\n"                           // opcode=0x019a0af7
         "fpackreph.pi f29, f30\n"                             // opcode=0x260f1efb
         "maskxor m1, m5, m5\n"                                // opcode=0x6652c0fb
         "fbci.ps f22, 315405\n"                               // opcode=0x4d00db1f
         "fsgnjx.s f21, f22, f28\n"                            // opcode=0x21cb2ad3
         "mova.x.m x29\n"                                      // opcode=0xd6000efb
         "maskxor m7, m0, m1\n"                                // opcode=0x661043fb
         "fsrl.pi f28, f28, f27\n"                             // opcode=0x07be5e7b
         "feq.s x30, f29, f30\n"                               // opcode=0xa1eeaf53
         "fmin.s f25, f24, f30\n"                              // opcode=0x29ec0cd3
         "flt.pi f21, f26, f23\n"                              // opcode=0xa77d1afb
         "cubeface.ps f24, f31, f31\n"                         // opcode=0x89ff8c7b
         "fmulh.pi f29, f29, f20\n"                            // opcode=0x174eaefb
         "maskpopc x23, m4\n"                                  // opcode=0x52020bfb
         "fsgnjx.s f28, f20, f24\n"                            // opcode=0x218a2e53
         "fbc.ps f30, 8(x21)\n"                                // opcode=0x508a8f0b
         "feq.pi f20, f29, f30\n"                              // opcode=0xa7eeaa7b
         "fmin.s f25, f29, f25\n"                              // opcode=0x299e8cd3
         "fbci.pi f23, 43358\n"                                // opcode=0x0a95ebdf
         "fsat8.pi f30, f31\n"                                 // opcode=0x060fbf7b
         "masknot m2, m0\n"                                    // opcode=0x6600217b
         "fsra.pi f20, f29, f29\n"                             // opcode=0x0fdeda7b
         "fmaxu.pi f26, f26, f21\n"                            // opcode=0x2f5d3d7b
         */
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
