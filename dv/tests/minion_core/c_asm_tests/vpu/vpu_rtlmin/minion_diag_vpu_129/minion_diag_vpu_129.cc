/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"


/// Macros

#define VSNIP_RSV           : : [operands] "r" (operands), [m_11] "r" (m_11), [m_12] "r" (m_12), [m_13] "r" (m_13), [m_14] "r" (m_14), [m_15] "r" (m_15) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
#define TSNIP_RSV          : : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x31"
#define RESET_FCSR            __asm__ __volatile__ ( "csrw    fcsr, zero\n" VSNIP_RSV);
#define RESET_TENSOR_COOP     __asm__ __volatile__ ( "csrw    0x804, zero\n" VSNIP_RSV);

#define ECALL                 __asm__ __volatile__ ( "ecall\n" VSNIP_RSV);
#define MRET                  __asm__ __volatile__ ( "mret\n" VSNIP_RSV);

#define TENSOR_WAIT_TL0       __asm__ __volatile__ ( "csrwi   0x830, 0x0\n" VSNIP_RSV);
#define TENSOR_WAIT_TL1       __asm__ __volatile__ ( "csrwi   0x830, 0x1\n" VSNIP_RSV);
#define TENSOR_WAIT_COP       __asm__ __volatile__ ( "csrwi   0x830, 0x6\n" VSNIP_RSV);
#define TENSOR_WAIT_FMA       __asm__ __volatile__ ( "csrwi   0x830, 0x7\n" VSNIP_RSV);
#define TENSOR_WAIT_STR       __asm__ __volatile__ ( "csrwi   0x830, 0x8\n" VSNIP_RSV);
#define TENSOR_WAIT_RED       __asm__ __volatile__ ( "csrwi   0x830, 0x9\n" VSNIP_RSV);
#define TENSOR_WAIT_QNT       __asm__ __volatile__ ( "csrwi   0x830, 0xa\n" VSNIP_RSV);
#define SET_TENSOR_MASK    \
   __asm__ __volatile__ ( \
         "li x5, 0xFFFF\n" \
         "csrrw zero, 0x805, x5\n" \
         VSNIP_RSV);

#define SET_SCP_CTL    \
   __asm__ __volatile__ ( \
         "csrrs	x0, 0x7e0, 3\n" \
         VSNIP_RSV);

int main() {

	uint64_t mid, tid;
	mid = get_minion_id();
	tid = get_thread_id();
	if (mid != 0 && tid != 0) { C_TEST_FAIL; return -1;};
	volatile uint32_t m_11[1024] __attribute__ ((aligned (4096))) = {
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00000
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x80011111,                                                  // -9.7958E-41                                 /// 00008
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00010
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00018
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00020
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00024
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00028
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0003c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00040
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00058
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0005c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00060
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00068
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0006c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00000000,                                                  // zero                                        /// 00074
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00080
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00084
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00088
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0008c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00090
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00094
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0009c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00100
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00108
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x55555555,                                                  // 4 random values                             /// 00114
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00120
			 0x0e000003,                                                  // Trailing 1s:                                /// 00124
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00128
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0012c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00134
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00138
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0013c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00148
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0014c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0015c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00160
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00164
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00168
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0016c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00170
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00174
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00178
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0017c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00180
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00184
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00188
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0018c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000000,                                                  // -zero                                       /// 00198
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0019c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001b4
			 0x7fc00001,                                                  // signaling NaN                               /// 001b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 001dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x7fc00001,                                                  // signaling NaN                               /// 001ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x3f028f5c,                                                  // 0.51                                        /// 00204
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00208
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0020c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00214
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00218
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00220
			 0x33333333,                                                  // 4 random values                             /// 00224
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00228
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0022c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00230
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00234
			 0x0c400000,                                                  // Leading 1s:                                 /// 00238
			 0xbf028f5c,                                                  // -0.51                                       /// 0023c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00240
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00244
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00248
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0xbf028f5c,                                                  // -0.51                                       /// 00250
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x3f028f5c,                                                  // 0.51                                        /// 0025c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00260
			 0xff800000,                                                  // -inf                                        /// 00264
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c600000,                                                  // Leading 1s:                                 /// 0026c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00270
			 0x0e000007,                                                  // Trailing 1s:                                /// 00274
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00288
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0028c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00290
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00294
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0x33333333,                                                  // 4 random values                             /// 002d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00304
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0030c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00310
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00318
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00328
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0032c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00330
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0033c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00344
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00350
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00354
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00358
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00360
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00368
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0036c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00370
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00378
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0037c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80000000,                                                  // -zero                                       /// 0038c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0c780000,                                                  // Leading 1s:                                 /// 003a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x7f800000,                                                  // inf                                         /// 003a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003cc
			 0x7f800000,                                                  // inf                                         /// 003d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003f8
			 0x7fc00001,                                                  // signaling NaN                               /// 003fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00400
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00404
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00408
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0040c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00410
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00414
			 0x4b000000,                                                  // 8388608.0                                   /// 00418
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00424
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00428
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0042c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00430
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00434
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0043c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00444
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0044c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00450
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00458
			 0xcb000000,                                                  // -8388608.0                                  /// 0045c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00464
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x00011111,                                                  // 9.7958E-41                                  /// 0046c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00470
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00478
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0047c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00480
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00484
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00488
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00494
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00498
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0049c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x33333333,                                                  // 4 random values                             /// 004bc
			 0x80000000,                                                  // -zero                                       /// 004c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x55555555,                                                  // 4 random values                             /// 004d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00504
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00508
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00510
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00514
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00518
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0051c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00520
			 0x7fc00001,                                                  // signaling NaN                               /// 00524
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00528
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00530
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00534
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00538
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0053c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00540
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00544
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00548
			 0x7fc00001,                                                  // signaling NaN                               /// 0054c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00558
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0055c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00560
			 0x7f800000,                                                  // inf                                         /// 00564
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00568
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00574
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00578
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0057c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00580
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00584
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00588
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00594
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x4b000000,                                                  // 8388608.0                                   /// 005a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005b0
			 0xffc00001,                                                  // -signaling NaN                              /// 005b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00600
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0060c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00614
			 0xff800000,                                                  // -inf                                        /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00620
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00624
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0062c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00630
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00634
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00638
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0063c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00640
			 0x7fc00001,                                                  // signaling NaN                               /// 00644
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0064c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00650
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00654
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00658
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00668
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0066c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00674
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e000001,                                                  // Trailing 1s:                                /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00684
			 0x7f800000,                                                  // inf                                         /// 00688
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0068c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00694
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00698
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0069c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00700
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00704
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00708
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x7fc00001,                                                  // signaling NaN                               /// 00718
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e000001,                                                  // Trailing 1s:                                /// 00728
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0072c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00730
			 0x0e000007,                                                  // Trailing 1s:                                /// 00734
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0073c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e000001,                                                  // Trailing 1s:                                /// 00744
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00748
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0074c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00750
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00754
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00758
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0075c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00760
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00764
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0076c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00770
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00778
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0077c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00784
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0078c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00790
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0079c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00804
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00808
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00810
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0081c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00824
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00828
			 0x4b000000,                                                  // 8388608.0                                   /// 0082c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00830
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00838
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0083c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00840
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0084c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00854
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00860
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00864
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0086c
			 0x80000000,                                                  // -zero                                       /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00878
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0087c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00880
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c600000,                                                  // Leading 1s:                                 /// 0088c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00894
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0089c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x33333333,                                                  // 4 random values                             /// 008b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 008bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008e8
			 0x00000000,                                                  // zero                                        /// 008ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00900
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0090c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0xbf028f5c,                                                  // -0.51                                       /// 00918
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00924
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00928
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0092c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00930
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00934
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00938
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0093c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00940
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00948
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00950
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00954
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00958
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0095c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00960
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x55555555,                                                  // 4 random values                             /// 00968
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0096c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x7f800000,                                                  // inf                                         /// 00974
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00978
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00988
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0098c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00990
			 0x80000000,                                                  // -zero                                       /// 00994
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009c4
			 0x00000000,                                                  // zero                                        /// 009c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00a00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a3c
			 0x33333333,                                                  // 4 random values                             /// 00a40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a44
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a48
			 0x55555555,                                                  // 4 random values                             /// 00a4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a70
			 0x7f800000,                                                  // inf                                         /// 00a74
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a90
			 0x80000000,                                                  // -zero                                       /// 00a94
			 0x3f028f5c,                                                  // 0.51                                        /// 00a98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ab0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ab4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ac4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00adc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ae4
			 0x00000000,                                                  // zero                                        /// 00ae8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x33333333,                                                  // 4 random values                             /// 00b00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x33333333,                                                  // 4 random values                             /// 00b1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b64
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b88
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ba8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bf0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c0c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c24
			 0xbf028f5c,                                                  // -0.51                                       /// 00c28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x4b000000,                                                  // 8388608.0                                   /// 00c3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c40
			 0x7fc00001,                                                  // signaling NaN                               /// 00c44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ca0
			 0x00000000,                                                  // zero                                        /// 00ca4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ca8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cbc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ccc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ce0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ce4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da4
			 0xffc00001,                                                  // -signaling NaN                              /// 00da8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ddc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00de4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00df8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dfc
			 0x33333333,                                                  // 4 random values                             /// 00e00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0xcb000000,                                                  // -8388608.0                                  /// 00e08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x55555555,                                                  // 4 random values                             /// 00e1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xffc00001,                                                  // -signaling NaN                              /// 00e2c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x80000000,                                                  // -zero                                       /// 00e58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e74
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e8c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00eb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ebc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ec0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ec8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ecc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x55555555,                                                  // 4 random values                             /// 00edc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ee0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x55555555,                                                  // 4 random values                             /// 00ee8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ef4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ef8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00efc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f18
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f1c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f30
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f64
			 0x33333333,                                                  // 4 random values                             /// 00f68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f78
			 0x33333333,                                                  // 4 random values                             /// 00f7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fa0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fa4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fd4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fd8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff8
			 0x80800002                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x11d3016b, /// 0x0
			 0xa6c027b8, /// 0x4
			 0x2a56655c, /// 0x8
			 0xd35a1a2c, /// 0xc
			 0xae97df01, /// 0x10
			 0x7eca375a, /// 0x14
			 0x8927d208, /// 0x18
			 0x274c3aa9, /// 0x1c
			 0x538b207b, /// 0x20
			 0x723fc482, /// 0x24
			 0xf17e9b80, /// 0x28
			 0x38ce6ee6, /// 0x2c
			 0xfbb14067, /// 0x30
			 0x6c6031e8, /// 0x34
			 0xf04269e3, /// 0x38
			 0x19dfddac, /// 0x3c
			 0xacb00153, /// 0x40
			 0xeb6e6a5b, /// 0x44
			 0x2aaad5eb, /// 0x48
			 0xd4c9abae, /// 0x4c
			 0x9e97535e, /// 0x50
			 0xc4fd07da, /// 0x54
			 0xe03f5c8d, /// 0x58
			 0x76c1cf8e, /// 0x5c
			 0x19de7ee0, /// 0x60
			 0xfdbc279d, /// 0x64
			 0x5394a65f, /// 0x68
			 0x92b15a49, /// 0x6c
			 0x9ee09ac2, /// 0x70
			 0x756515cc, /// 0x74
			 0xb50d685c, /// 0x78
			 0x7ea18516, /// 0x7c
			 0x2c9c6cd3, /// 0x80
			 0xf2a1dbd6, /// 0x84
			 0xc03da5ed, /// 0x88
			 0x30a979db, /// 0x8c
			 0x7909fe95, /// 0x90
			 0x264b2574, /// 0x94
			 0xca2da5f5, /// 0x98
			 0xe8d25e9c, /// 0x9c
			 0x2e4c022b, /// 0xa0
			 0x2189fef0, /// 0xa4
			 0x60093fe6, /// 0xa8
			 0x41d39e2d, /// 0xac
			 0xb4d64037, /// 0xb0
			 0x488c4f26, /// 0xb4
			 0x8f5fd565, /// 0xb8
			 0x2d9f58a2, /// 0xbc
			 0x64874b9f, /// 0xc0
			 0x4736cfcc, /// 0xc4
			 0x8d6803ff, /// 0xc8
			 0x9882b328, /// 0xcc
			 0xeacdf8d1, /// 0xd0
			 0x76f25e6b, /// 0xd4
			 0x8b1c1f1e, /// 0xd8
			 0xca94248f, /// 0xdc
			 0xb4e0f423, /// 0xe0
			 0xd8b1ad64, /// 0xe4
			 0xba0e2bcb, /// 0xe8
			 0x4e082933, /// 0xec
			 0x0bf40ce2, /// 0xf0
			 0xbe1d4e92, /// 0xf4
			 0x77df25e5, /// 0xf8
			 0x3530a86f, /// 0xfc
			 0xd4c77341, /// 0x100
			 0xe8dfc0c0, /// 0x104
			 0x2705c585, /// 0x108
			 0xae3f2fe5, /// 0x10c
			 0x0a602947, /// 0x110
			 0x34905192, /// 0x114
			 0xc91aa702, /// 0x118
			 0xad1e6735, /// 0x11c
			 0xbbb41ebf, /// 0x120
			 0x497e2b5d, /// 0x124
			 0x63425284, /// 0x128
			 0xf7e181ec, /// 0x12c
			 0xbbc5de42, /// 0x130
			 0x8443c6cc, /// 0x134
			 0x1588164c, /// 0x138
			 0x3bfe8c7e, /// 0x13c
			 0x7b56e7c4, /// 0x140
			 0x321be04e, /// 0x144
			 0x6469ef66, /// 0x148
			 0xe77d4a26, /// 0x14c
			 0x23bac84c, /// 0x150
			 0x0673244b, /// 0x154
			 0x77dd9428, /// 0x158
			 0xccc7c77f, /// 0x15c
			 0x051070d8, /// 0x160
			 0xf9f7e9f7, /// 0x164
			 0x20d713c9, /// 0x168
			 0xb3fc15fc, /// 0x16c
			 0x3c3423cd, /// 0x170
			 0xff7559b5, /// 0x174
			 0xe19dd401, /// 0x178
			 0x30b1fb0f, /// 0x17c
			 0x1c418d43, /// 0x180
			 0x827342b5, /// 0x184
			 0x94537825, /// 0x188
			 0x5398d5af, /// 0x18c
			 0x6299f645, /// 0x190
			 0x9cd07036, /// 0x194
			 0x0eb8429f, /// 0x198
			 0x7126056e, /// 0x19c
			 0x74fc37c6, /// 0x1a0
			 0x2a145f38, /// 0x1a4
			 0x769316f5, /// 0x1a8
			 0xfbcf477b, /// 0x1ac
			 0xd8d8caaf, /// 0x1b0
			 0x17e9323e, /// 0x1b4
			 0xee153db8, /// 0x1b8
			 0x75de270d, /// 0x1bc
			 0x97688a59, /// 0x1c0
			 0xc1e286bb, /// 0x1c4
			 0x4ac31a66, /// 0x1c8
			 0x3aab36de, /// 0x1cc
			 0x164b0d55, /// 0x1d0
			 0xb4f198fa, /// 0x1d4
			 0xedf2d96d, /// 0x1d8
			 0x63ebde0b, /// 0x1dc
			 0x90a85c4f, /// 0x1e0
			 0xb2103185, /// 0x1e4
			 0xf19c45d5, /// 0x1e8
			 0x0ce5e527, /// 0x1ec
			 0x82eb0f72, /// 0x1f0
			 0x7db77a3f, /// 0x1f4
			 0xf3c9d64b, /// 0x1f8
			 0x05f43d77, /// 0x1fc
			 0xe86a1be0, /// 0x200
			 0x32a6eff0, /// 0x204
			 0x695c6e80, /// 0x208
			 0xd479d47a, /// 0x20c
			 0x6c6cc44e, /// 0x210
			 0x5e5c0670, /// 0x214
			 0xf5697bd3, /// 0x218
			 0x74f70898, /// 0x21c
			 0x88827df1, /// 0x220
			 0x960dabf4, /// 0x224
			 0x7528ea7f, /// 0x228
			 0xff19da34, /// 0x22c
			 0xb6b4aa05, /// 0x230
			 0xece34ab0, /// 0x234
			 0x30e98a0f, /// 0x238
			 0xc402c348, /// 0x23c
			 0xa78f0d0a, /// 0x240
			 0xb8c2d29f, /// 0x244
			 0xc14fa756, /// 0x248
			 0x680a9b92, /// 0x24c
			 0x5e5b1e2e, /// 0x250
			 0x2c734036, /// 0x254
			 0x1d10f143, /// 0x258
			 0x4993ce96, /// 0x25c
			 0x396435d0, /// 0x260
			 0xcf7fa15b, /// 0x264
			 0x54ee5e80, /// 0x268
			 0x57ef353a, /// 0x26c
			 0x94f91bae, /// 0x270
			 0xf121ef2a, /// 0x274
			 0x3c332cfa, /// 0x278
			 0x8f7078cf, /// 0x27c
			 0x9b2635fa, /// 0x280
			 0x19eba008, /// 0x284
			 0x73bfe6c0, /// 0x288
			 0xd7427bac, /// 0x28c
			 0x76844bda, /// 0x290
			 0x0b21f792, /// 0x294
			 0x3ca0b825, /// 0x298
			 0xb2b6eb1d, /// 0x29c
			 0x2e16584b, /// 0x2a0
			 0x8bc0930a, /// 0x2a4
			 0x412c843e, /// 0x2a8
			 0x1b9c59ca, /// 0x2ac
			 0x047a9fb6, /// 0x2b0
			 0x68cf9248, /// 0x2b4
			 0xff7fdd12, /// 0x2b8
			 0x84c5a169, /// 0x2bc
			 0xc2311d1b, /// 0x2c0
			 0xce4c7b14, /// 0x2c4
			 0x061ba853, /// 0x2c8
			 0x2e43e2ab, /// 0x2cc
			 0x0d3ec59f, /// 0x2d0
			 0xdeb9a827, /// 0x2d4
			 0xc9050b00, /// 0x2d8
			 0xfd33b05f, /// 0x2dc
			 0x8b111c80, /// 0x2e0
			 0x52755b60, /// 0x2e4
			 0x287b0a51, /// 0x2e8
			 0xe14d0aa0, /// 0x2ec
			 0xffb7ae47, /// 0x2f0
			 0xcf5a351c, /// 0x2f4
			 0xb727e840, /// 0x2f8
			 0x9e6d82ca, /// 0x2fc
			 0x2dc8e97e, /// 0x300
			 0x520b5093, /// 0x304
			 0x969241fa, /// 0x308
			 0x363bd552, /// 0x30c
			 0x8ea32a06, /// 0x310
			 0xf6923315, /// 0x314
			 0x545071d9, /// 0x318
			 0xc807da30, /// 0x31c
			 0xde7c6fa4, /// 0x320
			 0xdeff6359, /// 0x324
			 0xe33df3f5, /// 0x328
			 0xa9381f13, /// 0x32c
			 0x116b031b, /// 0x330
			 0xce8be23d, /// 0x334
			 0x6663ef3d, /// 0x338
			 0x482f26df, /// 0x33c
			 0x251c369a, /// 0x340
			 0x0da480a6, /// 0x344
			 0x0a3cbc3c, /// 0x348
			 0x5b528d9f, /// 0x34c
			 0xc9ef8a45, /// 0x350
			 0x5667c6c9, /// 0x354
			 0x2aebcdd0, /// 0x358
			 0x193ae5ae, /// 0x35c
			 0x6cc8f3e8, /// 0x360
			 0x89373b06, /// 0x364
			 0xde9a8544, /// 0x368
			 0xf64aa69a, /// 0x36c
			 0x737571c2, /// 0x370
			 0x19fdefda, /// 0x374
			 0xeadf1443, /// 0x378
			 0xcf17ba03, /// 0x37c
			 0x4e4400c2, /// 0x380
			 0x3370fa4b, /// 0x384
			 0xec4e2a56, /// 0x388
			 0x056fc0ca, /// 0x38c
			 0x68a7a858, /// 0x390
			 0x5f7809f4, /// 0x394
			 0x89044124, /// 0x398
			 0x6e9b8db3, /// 0x39c
			 0x27d27dbb, /// 0x3a0
			 0x2830ceb4, /// 0x3a4
			 0x6f08486c, /// 0x3a8
			 0xdf1401c0, /// 0x3ac
			 0xe63b5713, /// 0x3b0
			 0xed09efab, /// 0x3b4
			 0x4adf48a1, /// 0x3b8
			 0x6e5d580a, /// 0x3bc
			 0xcf38bd2f, /// 0x3c0
			 0x952d1702, /// 0x3c4
			 0xb08ffc1a, /// 0x3c8
			 0x16618ebd, /// 0x3cc
			 0x5cf263e1, /// 0x3d0
			 0x8c9cfbec, /// 0x3d4
			 0x80ceced8, /// 0x3d8
			 0xd10048ef, /// 0x3dc
			 0x453d6ce7, /// 0x3e0
			 0x73910570, /// 0x3e4
			 0xb675ec11, /// 0x3e8
			 0x75e9980d, /// 0x3ec
			 0xf0d8362e, /// 0x3f0
			 0x5644304e, /// 0x3f4
			 0x2f5af476, /// 0x3f8
			 0xf64e59a3, /// 0x3fc
			 0x9460b78b, /// 0x400
			 0x7ec7358b, /// 0x404
			 0x7e816e0a, /// 0x408
			 0x8934ebd5, /// 0x40c
			 0xe56c3652, /// 0x410
			 0x4a1bd4d4, /// 0x414
			 0xb89f4b79, /// 0x418
			 0x765ea655, /// 0x41c
			 0x54e49690, /// 0x420
			 0x78e812af, /// 0x424
			 0x7d8e0328, /// 0x428
			 0x004e4b02, /// 0x42c
			 0x8d1eb8e5, /// 0x430
			 0x2ddb7c10, /// 0x434
			 0x5dd22156, /// 0x438
			 0x2a3dbfdd, /// 0x43c
			 0x7b4fc383, /// 0x440
			 0x750f8150, /// 0x444
			 0x41b29c33, /// 0x448
			 0x14f131b3, /// 0x44c
			 0x48c1fc3f, /// 0x450
			 0x8d5b6264, /// 0x454
			 0x46709082, /// 0x458
			 0xb3ba73ba, /// 0x45c
			 0xcc6a9117, /// 0x460
			 0x821e25d3, /// 0x464
			 0x03754b58, /// 0x468
			 0xb41792b3, /// 0x46c
			 0xa0dd6a2c, /// 0x470
			 0x81614a4e, /// 0x474
			 0x4d219593, /// 0x478
			 0x8911619a, /// 0x47c
			 0x74b2b34a, /// 0x480
			 0x48a7ea01, /// 0x484
			 0xeba66498, /// 0x488
			 0x457055aa, /// 0x48c
			 0xf53e67a6, /// 0x490
			 0xb839da29, /// 0x494
			 0x5ab85c5e, /// 0x498
			 0x6ca9a1c6, /// 0x49c
			 0x7df48cdf, /// 0x4a0
			 0xfc43836a, /// 0x4a4
			 0xd0c7099f, /// 0x4a8
			 0x76831562, /// 0x4ac
			 0xf7833318, /// 0x4b0
			 0xc16aba9a, /// 0x4b4
			 0x273ce14d, /// 0x4b8
			 0x52ce64c5, /// 0x4bc
			 0xfea91a55, /// 0x4c0
			 0x555dfbc8, /// 0x4c4
			 0xaeac5aed, /// 0x4c8
			 0x931e9d4e, /// 0x4cc
			 0x00f46a09, /// 0x4d0
			 0x5b8ce215, /// 0x4d4
			 0x4d2a1007, /// 0x4d8
			 0x9ab5a74f, /// 0x4dc
			 0xda4e64f3, /// 0x4e0
			 0x20535bde, /// 0x4e4
			 0x895d2855, /// 0x4e8
			 0x9676665c, /// 0x4ec
			 0xa29c9c00, /// 0x4f0
			 0x8fcdc6e6, /// 0x4f4
			 0x1d48f50f, /// 0x4f8
			 0xfa5f98ee, /// 0x4fc
			 0xea11285c, /// 0x500
			 0x50034f43, /// 0x504
			 0x7fafddca, /// 0x508
			 0xcbc7dd71, /// 0x50c
			 0xb63f9120, /// 0x510
			 0x0aaf7840, /// 0x514
			 0x8c85ba81, /// 0x518
			 0xaefcaf45, /// 0x51c
			 0xf4b116c4, /// 0x520
			 0x5ad67b30, /// 0x524
			 0x5a864ed2, /// 0x528
			 0x95d60ed6, /// 0x52c
			 0xed4af12f, /// 0x530
			 0xf2e339cc, /// 0x534
			 0xbb6f78b8, /// 0x538
			 0xfade4f08, /// 0x53c
			 0xc9cb148e, /// 0x540
			 0xc53bdcf6, /// 0x544
			 0x92ecbd55, /// 0x548
			 0xb3c89dd7, /// 0x54c
			 0x8bbe63ca, /// 0x550
			 0xf992b848, /// 0x554
			 0xb7187e60, /// 0x558
			 0xd61e33fc, /// 0x55c
			 0xc54e6c1e, /// 0x560
			 0x76755bce, /// 0x564
			 0x0dcef011, /// 0x568
			 0x10791f20, /// 0x56c
			 0xcf4bd4b8, /// 0x570
			 0xf6feb9b7, /// 0x574
			 0x2a4cb171, /// 0x578
			 0x3c570ac0, /// 0x57c
			 0xefdd0a47, /// 0x580
			 0x507b622c, /// 0x584
			 0x043d970a, /// 0x588
			 0x02512bf1, /// 0x58c
			 0x9f662fd9, /// 0x590
			 0xfa50ff0d, /// 0x594
			 0x5b35a962, /// 0x598
			 0xa5aa1019, /// 0x59c
			 0x71895d11, /// 0x5a0
			 0x30656db2, /// 0x5a4
			 0xb04f64b1, /// 0x5a8
			 0x5772d036, /// 0x5ac
			 0xfa7f9a72, /// 0x5b0
			 0x31f0dc3f, /// 0x5b4
			 0xdd92748b, /// 0x5b8
			 0x4aa1da69, /// 0x5bc
			 0x26c532a6, /// 0x5c0
			 0x35cdaa64, /// 0x5c4
			 0xebec56c9, /// 0x5c8
			 0x6b96810b, /// 0x5cc
			 0x4f77ec57, /// 0x5d0
			 0xed6e28fd, /// 0x5d4
			 0x6b6380e9, /// 0x5d8
			 0x89a56b35, /// 0x5dc
			 0x8957148d, /// 0x5e0
			 0x550a1cea, /// 0x5e4
			 0xaa6e1630, /// 0x5e8
			 0x806e14d0, /// 0x5ec
			 0x805b4c7d, /// 0x5f0
			 0x8e3cbcbb, /// 0x5f4
			 0xe0e368d3, /// 0x5f8
			 0xd05fd640, /// 0x5fc
			 0xda3e8ff2, /// 0x600
			 0x692a35e1, /// 0x604
			 0x5d8e0447, /// 0x608
			 0xa4998c4a, /// 0x60c
			 0xb302d17e, /// 0x610
			 0x2f107cc7, /// 0x614
			 0x76798423, /// 0x618
			 0xf215516d, /// 0x61c
			 0xeebc5980, /// 0x620
			 0x2b9efa0d, /// 0x624
			 0x07496bdb, /// 0x628
			 0x5c20d0be, /// 0x62c
			 0xdcdcb83e, /// 0x630
			 0x79fc68c2, /// 0x634
			 0xbd8ba4c0, /// 0x638
			 0x3557c03a, /// 0x63c
			 0x2a76e517, /// 0x640
			 0x11b6e6a7, /// 0x644
			 0x39e0077f, /// 0x648
			 0xd6a3537b, /// 0x64c
			 0x2ca94d1f, /// 0x650
			 0xdaf03731, /// 0x654
			 0x02277da4, /// 0x658
			 0x64438fe7, /// 0x65c
			 0x3ceb0adb, /// 0x660
			 0x75b9eeee, /// 0x664
			 0xf39f20a3, /// 0x668
			 0x4440df50, /// 0x66c
			 0xd61d6ed1, /// 0x670
			 0x7527787f, /// 0x674
			 0x9cba9a25, /// 0x678
			 0x970ae468, /// 0x67c
			 0x86ebd2ba, /// 0x680
			 0x22cf2c7c, /// 0x684
			 0x2a571f89, /// 0x688
			 0x705ed08b, /// 0x68c
			 0x7c2e5758, /// 0x690
			 0x7ba94f3d, /// 0x694
			 0x82623b9d, /// 0x698
			 0x97176fc2, /// 0x69c
			 0xe8800bc7, /// 0x6a0
			 0x112bd52c, /// 0x6a4
			 0x2745e925, /// 0x6a8
			 0xb9b98adc, /// 0x6ac
			 0x631a8740, /// 0x6b0
			 0xbecf5573, /// 0x6b4
			 0x31f4fe5c, /// 0x6b8
			 0x721efe0a, /// 0x6bc
			 0xb72ce378, /// 0x6c0
			 0x470be7bc, /// 0x6c4
			 0x47e9ea23, /// 0x6c8
			 0x99a71cf2, /// 0x6cc
			 0xa7f1d9dd, /// 0x6d0
			 0xf0c0a255, /// 0x6d4
			 0x06b6c858, /// 0x6d8
			 0x56a1aec6, /// 0x6dc
			 0xedbe4d73, /// 0x6e0
			 0x8b4648b8, /// 0x6e4
			 0x529d02a9, /// 0x6e8
			 0xde554f64, /// 0x6ec
			 0xd9b109c8, /// 0x6f0
			 0xd8964930, /// 0x6f4
			 0x0bdafc8d, /// 0x6f8
			 0xbe3b3f64, /// 0x6fc
			 0x579d38d9, /// 0x700
			 0xd107b442, /// 0x704
			 0x13e262c9, /// 0x708
			 0x561240a9, /// 0x70c
			 0x424494c4, /// 0x710
			 0xdd6ad21e, /// 0x714
			 0xe9efb29e, /// 0x718
			 0xeaf9eea1, /// 0x71c
			 0x01126098, /// 0x720
			 0x5ecfe80a, /// 0x724
			 0x63ded587, /// 0x728
			 0x37abd3db, /// 0x72c
			 0xd447a50e, /// 0x730
			 0xd5eca22a, /// 0x734
			 0x0719e11b, /// 0x738
			 0xd3af91ef, /// 0x73c
			 0xdd038568, /// 0x740
			 0x751c9ef4, /// 0x744
			 0x413721de, /// 0x748
			 0x6fadfaf2, /// 0x74c
			 0xb30ffd18, /// 0x750
			 0xabafb0bc, /// 0x754
			 0x3c08625e, /// 0x758
			 0xbafc59a1, /// 0x75c
			 0x4722d3cb, /// 0x760
			 0x01fb102d, /// 0x764
			 0x65b02ae9, /// 0x768
			 0xd302ea96, /// 0x76c
			 0xc10d2db6, /// 0x770
			 0x64febe55, /// 0x774
			 0x36b13d53, /// 0x778
			 0x5ece98ca, /// 0x77c
			 0x004222b1, /// 0x780
			 0xe6f82dd4, /// 0x784
			 0x5d252762, /// 0x788
			 0x7b2b8356, /// 0x78c
			 0x59584659, /// 0x790
			 0xd139c107, /// 0x794
			 0x1d422448, /// 0x798
			 0x63017ad5, /// 0x79c
			 0x2a312e28, /// 0x7a0
			 0x5543c46e, /// 0x7a4
			 0x2c45d886, /// 0x7a8
			 0x95e537b5, /// 0x7ac
			 0x641204fc, /// 0x7b0
			 0xd6ba8363, /// 0x7b4
			 0x3b0d42c7, /// 0x7b8
			 0x0a3e81fc, /// 0x7bc
			 0xeb28a2a0, /// 0x7c0
			 0xf33285a0, /// 0x7c4
			 0x6203fd73, /// 0x7c8
			 0x1665eb17, /// 0x7cc
			 0x2ea1d76f, /// 0x7d0
			 0xc8811c21, /// 0x7d4
			 0x5b721db7, /// 0x7d8
			 0x103b11e5, /// 0x7dc
			 0x2066c03e, /// 0x7e0
			 0x1b11ac86, /// 0x7e4
			 0x746f47ee, /// 0x7e8
			 0xef1e441e, /// 0x7ec
			 0x08cdd1c5, /// 0x7f0
			 0x2e25b27a, /// 0x7f4
			 0x8a23f606, /// 0x7f8
			 0x133e110e, /// 0x7fc
			 0xd448bec8, /// 0x800
			 0xbdecbe57, /// 0x804
			 0x70cd15e9, /// 0x808
			 0x890e867e, /// 0x80c
			 0xdf99740a, /// 0x810
			 0x157e2ed1, /// 0x814
			 0x8a73d402, /// 0x818
			 0x4b5f0c12, /// 0x81c
			 0x9806a095, /// 0x820
			 0xb2dc055c, /// 0x824
			 0xe8b9967c, /// 0x828
			 0xb63e9da0, /// 0x82c
			 0xedb1d4f6, /// 0x830
			 0x9e18b305, /// 0x834
			 0x4a68635d, /// 0x838
			 0xd17784c8, /// 0x83c
			 0x0e68d228, /// 0x840
			 0x68840908, /// 0x844
			 0x38d862c3, /// 0x848
			 0xff14f6fa, /// 0x84c
			 0x009db42f, /// 0x850
			 0x3d214b1f, /// 0x854
			 0xbb33d2fb, /// 0x858
			 0xf5316344, /// 0x85c
			 0x64d8d8ec, /// 0x860
			 0x2bdb50be, /// 0x864
			 0x4408f0c8, /// 0x868
			 0xaa95374f, /// 0x86c
			 0x033b985c, /// 0x870
			 0xdec43f4b, /// 0x874
			 0x2ac01ebd, /// 0x878
			 0xe0ed32dc, /// 0x87c
			 0x2c4e15c1, /// 0x880
			 0xe8e1d59b, /// 0x884
			 0x28eb6b0c, /// 0x888
			 0x52ffeaed, /// 0x88c
			 0x553e413a, /// 0x890
			 0x834e7dd6, /// 0x894
			 0x1fcc1da6, /// 0x898
			 0xa71cf9af, /// 0x89c
			 0x25167e58, /// 0x8a0
			 0xcfebeaaa, /// 0x8a4
			 0xfdca231e, /// 0x8a8
			 0x7e7bcac4, /// 0x8ac
			 0x3242fb5b, /// 0x8b0
			 0x9bf2c9da, /// 0x8b4
			 0x69095e0e, /// 0x8b8
			 0xb095714b, /// 0x8bc
			 0x3edbf90e, /// 0x8c0
			 0x562308e0, /// 0x8c4
			 0xb0bdc61b, /// 0x8c8
			 0xc1eb86eb, /// 0x8cc
			 0x8d03ae89, /// 0x8d0
			 0xcee665d6, /// 0x8d4
			 0x812f5547, /// 0x8d8
			 0xe94d11e4, /// 0x8dc
			 0xbedeea00, /// 0x8e0
			 0x41a957a8, /// 0x8e4
			 0x31bcbd81, /// 0x8e8
			 0x027863a3, /// 0x8ec
			 0x8aa640e9, /// 0x8f0
			 0x64fb32be, /// 0x8f4
			 0x97e6b372, /// 0x8f8
			 0xc8dac35f, /// 0x8fc
			 0xaa15bad6, /// 0x900
			 0xe90524a5, /// 0x904
			 0xd69adebc, /// 0x908
			 0x5a478271, /// 0x90c
			 0x70e8117a, /// 0x910
			 0xacb138ef, /// 0x914
			 0x97bc7294, /// 0x918
			 0xcfe09524, /// 0x91c
			 0xfd7dcebd, /// 0x920
			 0x07c908da, /// 0x924
			 0xa626e908, /// 0x928
			 0x172a03cc, /// 0x92c
			 0xfeabb329, /// 0x930
			 0x5b2bc129, /// 0x934
			 0xe9bc195e, /// 0x938
			 0x7120c83b, /// 0x93c
			 0x28f025b7, /// 0x940
			 0xbbf691f8, /// 0x944
			 0x1a6158d3, /// 0x948
			 0x2f03968e, /// 0x94c
			 0x66a2ab24, /// 0x950
			 0x456b8c92, /// 0x954
			 0x429bc65d, /// 0x958
			 0x6486da9d, /// 0x95c
			 0x93f27010, /// 0x960
			 0xce891988, /// 0x964
			 0xcc5b0f8a, /// 0x968
			 0xa99ae41e, /// 0x96c
			 0x63a2a631, /// 0x970
			 0x1331ced9, /// 0x974
			 0x18f2f780, /// 0x978
			 0x82567f11, /// 0x97c
			 0x38b06e86, /// 0x980
			 0x338ce65f, /// 0x984
			 0x60e8592d, /// 0x988
			 0xc5a56c5e, /// 0x98c
			 0x20c3ec9d, /// 0x990
			 0x929b9595, /// 0x994
			 0x7a450b82, /// 0x998
			 0x599b3e07, /// 0x99c
			 0x3871a972, /// 0x9a0
			 0x790118b5, /// 0x9a4
			 0xb5ba738b, /// 0x9a8
			 0xf8462050, /// 0x9ac
			 0xa59e01ac, /// 0x9b0
			 0xe90fa79a, /// 0x9b4
			 0x5746d948, /// 0x9b8
			 0x80b658da, /// 0x9bc
			 0xc7f21a40, /// 0x9c0
			 0xe413dfce, /// 0x9c4
			 0xe1a47edc, /// 0x9c8
			 0x54b9a5d0, /// 0x9cc
			 0x54a511bc, /// 0x9d0
			 0x1382a4ee, /// 0x9d4
			 0x34ad2044, /// 0x9d8
			 0x4a302175, /// 0x9dc
			 0x9efdd5af, /// 0x9e0
			 0x13492d61, /// 0x9e4
			 0x2d071fe1, /// 0x9e8
			 0x642df6a0, /// 0x9ec
			 0x0b959127, /// 0x9f0
			 0x30d2745e, /// 0x9f4
			 0x0ab44870, /// 0x9f8
			 0x2082eef2, /// 0x9fc
			 0xe8987f31, /// 0xa00
			 0xfd7e3202, /// 0xa04
			 0x3dd0b4b8, /// 0xa08
			 0xaaaf96cf, /// 0xa0c
			 0xcffa73e5, /// 0xa10
			 0xeff729d4, /// 0xa14
			 0x45e3e1c4, /// 0xa18
			 0x56e0f9da, /// 0xa1c
			 0x45913661, /// 0xa20
			 0x97a582f2, /// 0xa24
			 0xaa413213, /// 0xa28
			 0x63c23618, /// 0xa2c
			 0x8790c290, /// 0xa30
			 0x1823912c, /// 0xa34
			 0xff820db2, /// 0xa38
			 0xd273e3e1, /// 0xa3c
			 0x52c626d0, /// 0xa40
			 0xaa3aa2e2, /// 0xa44
			 0xd71ad5e6, /// 0xa48
			 0x427e1a03, /// 0xa4c
			 0xe57c7914, /// 0xa50
			 0xa12f47b9, /// 0xa54
			 0x90927ea6, /// 0xa58
			 0x6d9c3d59, /// 0xa5c
			 0x14f3cb63, /// 0xa60
			 0x3ea3bc44, /// 0xa64
			 0x7b03ce2f, /// 0xa68
			 0x7cfa893f, /// 0xa6c
			 0xb4886d6f, /// 0xa70
			 0x0616d359, /// 0xa74
			 0x2b21d1ae, /// 0xa78
			 0x459a32af, /// 0xa7c
			 0x280855ff, /// 0xa80
			 0x6895a04a, /// 0xa84
			 0xfcb275b4, /// 0xa88
			 0xf6f641be, /// 0xa8c
			 0x84d8ed2b, /// 0xa90
			 0x05708b6d, /// 0xa94
			 0x1469c66c, /// 0xa98
			 0x7aa920c2, /// 0xa9c
			 0x1bdd2962, /// 0xaa0
			 0x2dda3580, /// 0xaa4
			 0xde057277, /// 0xaa8
			 0xea595158, /// 0xaac
			 0xb950ce92, /// 0xab0
			 0xd9d603f3, /// 0xab4
			 0xc76f8439, /// 0xab8
			 0xcdcf702a, /// 0xabc
			 0x462ae23f, /// 0xac0
			 0x9eacef04, /// 0xac4
			 0xdc4a2920, /// 0xac8
			 0x1aed44ff, /// 0xacc
			 0x375d3b4e, /// 0xad0
			 0xa9945191, /// 0xad4
			 0x311c3c12, /// 0xad8
			 0x46b177e8, /// 0xadc
			 0x8e994d14, /// 0xae0
			 0xfc42efab, /// 0xae4
			 0x6601b856, /// 0xae8
			 0x9654e83f, /// 0xaec
			 0x79628031, /// 0xaf0
			 0xdd36aabc, /// 0xaf4
			 0xc1f4a6eb, /// 0xaf8
			 0x47c6ca32, /// 0xafc
			 0xb8064b88, /// 0xb00
			 0x31069741, /// 0xb04
			 0x31865c8a, /// 0xb08
			 0x50ad16bb, /// 0xb0c
			 0x5d814ca8, /// 0xb10
			 0x9d1834e0, /// 0xb14
			 0x923a5cb1, /// 0xb18
			 0x75d0c7f7, /// 0xb1c
			 0x9da42e7d, /// 0xb20
			 0xf1b94940, /// 0xb24
			 0xbab43c9b, /// 0xb28
			 0x987c39c5, /// 0xb2c
			 0x11fdf6b3, /// 0xb30
			 0x1ff9524b, /// 0xb34
			 0x6b6f0ff7, /// 0xb38
			 0x7c39194e, /// 0xb3c
			 0xcc01a7b8, /// 0xb40
			 0x4aceb0f0, /// 0xb44
			 0x65ea846a, /// 0xb48
			 0xa0324296, /// 0xb4c
			 0x0d3974b7, /// 0xb50
			 0x649dd188, /// 0xb54
			 0x6b9edf35, /// 0xb58
			 0xcbb5f2ae, /// 0xb5c
			 0xd9c0949d, /// 0xb60
			 0xf12c3115, /// 0xb64
			 0x87d9f193, /// 0xb68
			 0x2ba0d43e, /// 0xb6c
			 0x1f9b9755, /// 0xb70
			 0xf54f86da, /// 0xb74
			 0x5d3c76a5, /// 0xb78
			 0x491ff358, /// 0xb7c
			 0x74d5d066, /// 0xb80
			 0x77009b14, /// 0xb84
			 0xe15ff241, /// 0xb88
			 0x38a06a43, /// 0xb8c
			 0xb07cb346, /// 0xb90
			 0x95d58e93, /// 0xb94
			 0x9959226d, /// 0xb98
			 0x4baf9aa1, /// 0xb9c
			 0x3d23cfbf, /// 0xba0
			 0x6730aea5, /// 0xba4
			 0xd28fc373, /// 0xba8
			 0x823d98e9, /// 0xbac
			 0x8da21bb5, /// 0xbb0
			 0x018f7b78, /// 0xbb4
			 0x6993e87a, /// 0xbb8
			 0xb3d1ded3, /// 0xbbc
			 0xe6d2c667, /// 0xbc0
			 0x047686b8, /// 0xbc4
			 0x808f80b6, /// 0xbc8
			 0x16f5c821, /// 0xbcc
			 0x203de911, /// 0xbd0
			 0xd935e2ae, /// 0xbd4
			 0x4249b2a5, /// 0xbd8
			 0xf9a352df, /// 0xbdc
			 0x02ea0071, /// 0xbe0
			 0x589acfa5, /// 0xbe4
			 0x9f5f33d5, /// 0xbe8
			 0x598ee9da, /// 0xbec
			 0x8f899fe1, /// 0xbf0
			 0x384b13d1, /// 0xbf4
			 0xbc0ea514, /// 0xbf8
			 0x3724e7e6, /// 0xbfc
			 0x4359e393, /// 0xc00
			 0x1e6bd8d4, /// 0xc04
			 0xb209331b, /// 0xc08
			 0x3aa4115a, /// 0xc0c
			 0x5779fdfe, /// 0xc10
			 0xbc273dcb, /// 0xc14
			 0xbef1e839, /// 0xc18
			 0xf2d5d100, /// 0xc1c
			 0x8e7d65a7, /// 0xc20
			 0xe2147793, /// 0xc24
			 0x0fcf9bad, /// 0xc28
			 0x9082664e, /// 0xc2c
			 0xaa24f9ca, /// 0xc30
			 0x0b312859, /// 0xc34
			 0xef83eaa7, /// 0xc38
			 0xa2d9c7cc, /// 0xc3c
			 0xcf5b4975, /// 0xc40
			 0x90b2b537, /// 0xc44
			 0x72f07770, /// 0xc48
			 0x6955217d, /// 0xc4c
			 0xc6b0ea11, /// 0xc50
			 0x262a40cf, /// 0xc54
			 0x0769b261, /// 0xc58
			 0x05354d22, /// 0xc5c
			 0x1f2b3e67, /// 0xc60
			 0x1a9c515d, /// 0xc64
			 0x0f3c20cc, /// 0xc68
			 0x84a47d86, /// 0xc6c
			 0xb6988047, /// 0xc70
			 0x16994e1a, /// 0xc74
			 0x5b39af6e, /// 0xc78
			 0x15bee246, /// 0xc7c
			 0x9e4c5cf9, /// 0xc80
			 0xc09b2cb4, /// 0xc84
			 0x0bfe698b, /// 0xc88
			 0xb2ae619b, /// 0xc8c
			 0xf57fd67e, /// 0xc90
			 0x9d29ab2c, /// 0xc94
			 0xddda6e6a, /// 0xc98
			 0x5b504dbc, /// 0xc9c
			 0xed11d21d, /// 0xca0
			 0x431d4adb, /// 0xca4
			 0x7f1afaca, /// 0xca8
			 0xef4eb128, /// 0xcac
			 0x6578f441, /// 0xcb0
			 0x4a11702f, /// 0xcb4
			 0xfe44868d, /// 0xcb8
			 0x3128cb7f, /// 0xcbc
			 0x978a9ba1, /// 0xcc0
			 0x56b176f4, /// 0xcc4
			 0xd3b4fe81, /// 0xcc8
			 0x775eef85, /// 0xccc
			 0x01b919d0, /// 0xcd0
			 0x501a5b20, /// 0xcd4
			 0xa378e789, /// 0xcd8
			 0x2f781809, /// 0xcdc
			 0x21725182, /// 0xce0
			 0x74f05af0, /// 0xce4
			 0xbce2ff79, /// 0xce8
			 0x5896acc4, /// 0xcec
			 0x077b132a, /// 0xcf0
			 0xa39f3db7, /// 0xcf4
			 0x98ce72eb, /// 0xcf8
			 0xf28f8980, /// 0xcfc
			 0xe054f3ec, /// 0xd00
			 0x60133e69, /// 0xd04
			 0x7adaec7b, /// 0xd08
			 0xca08a8d4, /// 0xd0c
			 0x7883b716, /// 0xd10
			 0x0d09ade4, /// 0xd14
			 0x7570a8d1, /// 0xd18
			 0xec1b4c2b, /// 0xd1c
			 0x1d94c648, /// 0xd20
			 0xb4bd0ade, /// 0xd24
			 0x87703cf4, /// 0xd28
			 0xc6857a00, /// 0xd2c
			 0x61b88153, /// 0xd30
			 0x58c80a88, /// 0xd34
			 0x9c1baa7a, /// 0xd38
			 0xf698af24, /// 0xd3c
			 0xdcbc14b0, /// 0xd40
			 0x6a9df315, /// 0xd44
			 0xc9c526b6, /// 0xd48
			 0xc7478ed6, /// 0xd4c
			 0x6308ec05, /// 0xd50
			 0x381bb62e, /// 0xd54
			 0x9348410c, /// 0xd58
			 0x145ca78f, /// 0xd5c
			 0x2c3bfe11, /// 0xd60
			 0xbc4df12f, /// 0xd64
			 0x769a528d, /// 0xd68
			 0xa4d64967, /// 0xd6c
			 0x9b63690e, /// 0xd70
			 0x5738ca50, /// 0xd74
			 0x55db5455, /// 0xd78
			 0xae683fa8, /// 0xd7c
			 0xfeda0a42, /// 0xd80
			 0x7ad9fd1d, /// 0xd84
			 0x524ba8b4, /// 0xd88
			 0x4caf22e0, /// 0xd8c
			 0x951ad699, /// 0xd90
			 0xe93ad762, /// 0xd94
			 0x0095094a, /// 0xd98
			 0x33123065, /// 0xd9c
			 0x608f7e98, /// 0xda0
			 0x818bfd26, /// 0xda4
			 0x0671981f, /// 0xda8
			 0x4d8ce960, /// 0xdac
			 0x88c1b54a, /// 0xdb0
			 0xc0473cf0, /// 0xdb4
			 0x3a0f5dbf, /// 0xdb8
			 0x07a168e2, /// 0xdbc
			 0x5bc45385, /// 0xdc0
			 0x300e6210, /// 0xdc4
			 0x6f1fd5d6, /// 0xdc8
			 0xced126bf, /// 0xdcc
			 0x73a0810f, /// 0xdd0
			 0x9ba904bc, /// 0xdd4
			 0x39f97c9b, /// 0xdd8
			 0x0c73543e, /// 0xddc
			 0x99105e74, /// 0xde0
			 0x74713079, /// 0xde4
			 0x343943bb, /// 0xde8
			 0xc0be3b13, /// 0xdec
			 0x0ecd5c64, /// 0xdf0
			 0xf0645bb9, /// 0xdf4
			 0xc2ce05d9, /// 0xdf8
			 0x13c1c085, /// 0xdfc
			 0x9cdce4f6, /// 0xe00
			 0x6746d809, /// 0xe04
			 0x86e7d936, /// 0xe08
			 0xf0f59313, /// 0xe0c
			 0xd6b1ee13, /// 0xe10
			 0x02881473, /// 0xe14
			 0xb35d9012, /// 0xe18
			 0xa0426724, /// 0xe1c
			 0xe23dd6fc, /// 0xe20
			 0x41ca5798, /// 0xe24
			 0x203b2f63, /// 0xe28
			 0x99303d0c, /// 0xe2c
			 0x9a812152, /// 0xe30
			 0x76b893cc, /// 0xe34
			 0x741d9b7b, /// 0xe38
			 0x590f18ed, /// 0xe3c
			 0xe6a4ad61, /// 0xe40
			 0x805ac012, /// 0xe44
			 0x3c6cc86f, /// 0xe48
			 0x0f065e76, /// 0xe4c
			 0x0b0be0ef, /// 0xe50
			 0x68d3939a, /// 0xe54
			 0xc1f1edbe, /// 0xe58
			 0xb8525985, /// 0xe5c
			 0x8e60f8f9, /// 0xe60
			 0x06d842b4, /// 0xe64
			 0xee4b4c2b, /// 0xe68
			 0x795f253c, /// 0xe6c
			 0xc45322e7, /// 0xe70
			 0x07ca3956, /// 0xe74
			 0x2fa67430, /// 0xe78
			 0x5f66b1ad, /// 0xe7c
			 0xb16122da, /// 0xe80
			 0x4363a0d7, /// 0xe84
			 0xebfb222b, /// 0xe88
			 0x577f129f, /// 0xe8c
			 0xe4f676a1, /// 0xe90
			 0xe29e7daa, /// 0xe94
			 0x6d393046, /// 0xe98
			 0x3a958825, /// 0xe9c
			 0x565e8cfc, /// 0xea0
			 0x5d12139b, /// 0xea4
			 0x27017de1, /// 0xea8
			 0x411a6f5e, /// 0xeac
			 0x574719fc, /// 0xeb0
			 0x91257967, /// 0xeb4
			 0xf4a09464, /// 0xeb8
			 0xf27eb326, /// 0xebc
			 0x9a40e516, /// 0xec0
			 0x33630269, /// 0xec4
			 0x0cb3cf1b, /// 0xec8
			 0x236ad21d, /// 0xecc
			 0x82166a31, /// 0xed0
			 0x46b1de32, /// 0xed4
			 0xb107d5d8, /// 0xed8
			 0x8b4c7021, /// 0xedc
			 0x258c35c5, /// 0xee0
			 0x798db3ca, /// 0xee4
			 0xe5d72731, /// 0xee8
			 0x3f7016dd, /// 0xeec
			 0x62b8183a, /// 0xef0
			 0x161279d9, /// 0xef4
			 0x05feff3e, /// 0xef8
			 0x72af9cca, /// 0xefc
			 0x76c97163, /// 0xf00
			 0x3c051c10, /// 0xf04
			 0x5e83487c, /// 0xf08
			 0xd343b409, /// 0xf0c
			 0xfd8c06e1, /// 0xf10
			 0x58bea3fd, /// 0xf14
			 0x267472af, /// 0xf18
			 0xf5bd6636, /// 0xf1c
			 0x030fdebc, /// 0xf20
			 0x689fc732, /// 0xf24
			 0x1a7bad4b, /// 0xf28
			 0x40ffdb81, /// 0xf2c
			 0x39d64721, /// 0xf30
			 0x88feaa02, /// 0xf34
			 0x050697ef, /// 0xf38
			 0x861aad9c, /// 0xf3c
			 0xc3bd6194, /// 0xf40
			 0xbd729c70, /// 0xf44
			 0xd45c10be, /// 0xf48
			 0xb8229533, /// 0xf4c
			 0xfb598923, /// 0xf50
			 0x0a41b6c5, /// 0xf54
			 0x66a1c56f, /// 0xf58
			 0xd6ee015b, /// 0xf5c
			 0x28b0487d, /// 0xf60
			 0x14ac7fc7, /// 0xf64
			 0x0777bdfa, /// 0xf68
			 0xefa2a377, /// 0xf6c
			 0x13864552, /// 0xf70
			 0xdf9f94db, /// 0xf74
			 0xe13d16e8, /// 0xf78
			 0x6b2797f9, /// 0xf7c
			 0x2725fc09, /// 0xf80
			 0xddaec216, /// 0xf84
			 0x112d43a9, /// 0xf88
			 0x36adf672, /// 0xf8c
			 0x3a6ab587, /// 0xf90
			 0xcf4c4c0e, /// 0xf94
			 0xb07f7fa3, /// 0xf98
			 0x5506249a, /// 0xf9c
			 0x18fe6063, /// 0xfa0
			 0xe265e086, /// 0xfa4
			 0x0bd90d33, /// 0xfa8
			 0xa68cf261, /// 0xfac
			 0xc0428cee, /// 0xfb0
			 0x651fc6e9, /// 0xfb4
			 0xaea07280, /// 0xfb8
			 0x285a21d3, /// 0xfbc
			 0x7ac027eb, /// 0xfc0
			 0x60de3967, /// 0xfc4
			 0x43efa133, /// 0xfc8
			 0xd3227bf8, /// 0xfcc
			 0xa0c5a996, /// 0xfd0
			 0xd8f43801, /// 0xfd4
			 0xa1c25827, /// 0xfd8
			 0x61a535e2, /// 0xfdc
			 0x3b12a726, /// 0xfe0
			 0x983d1999, /// 0xfe4
			 0xc27750e9, /// 0xfe8
			 0x97752b3a, /// 0xfec
			 0xca685563, /// 0xff0
			 0x720c215a, /// 0xff4
			 0xfa2a19a4, /// 0xff8
			 0xd6a0eae0 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00000
			 0x7f800000,                                                  // inf                                         /// 00004
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00010
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00014
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00018
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00020
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00024
			 0xffc00001,                                                  // -signaling NaN                              /// 00028
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0002c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00030
			 0x0c780000,                                                  // Leading 1s:                                 /// 00034
			 0x4b000000,                                                  // 8388608.0                                   /// 00038
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0003c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00040
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00044
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00048
			 0xcb000000,                                                  // -8388608.0                                  /// 0004c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00050
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00058
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00068
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0006c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00078
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0008c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00090
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00098
			 0x7f800000,                                                  // inf                                         /// 0009c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x33333333,                                                  // 4 random values                             /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00100
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00108
			 0x00011111,                                                  // 9.7958E-41                                  /// 0010c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00110
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00114
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00118
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00128
			 0x00011111,                                                  // 9.7958E-41                                  /// 0012c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00134
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00138
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0013c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00140
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00144
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00148
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0014c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00150
			 0xcb000000,                                                  // -8388608.0                                  /// 00154
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00158
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0015c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00160
			 0x33333333,                                                  // 4 random values                             /// 00164
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00168
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0016c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x80011111,                                                  // -9.7958E-41                                 /// 00174
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00178
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00188
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0018c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00194
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0019c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001a8
			 0x3f028f5c,                                                  // 0.51                                        /// 001ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00200
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00208
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0020c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00210
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00220
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00228
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00240
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00244
			 0x00011111,                                                  // 9.7958E-41                                  /// 00248
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00250
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00254
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00258
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0025c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00264
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00268
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00278
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0027c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00284
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00290
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0029c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x3f028f5c,                                                  // 0.51                                        /// 002a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00300
			 0xffc00001,                                                  // -signaling NaN                              /// 00304
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00308
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00318
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00320
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00324
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00328
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0032c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0xcb000000,                                                  // -8388608.0                                  /// 00334
			 0xffc00001,                                                  // -signaling NaN                              /// 00338
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00348
			 0x0e000001,                                                  // Trailing 1s:                                /// 0034c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xff800000,                                                  // -inf                                        /// 00354
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00358
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00368
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00370
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00378
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0037c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00384
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00388
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00390
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00398
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0039c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x7fc00001,                                                  // signaling NaN                               /// 003d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x55555555,                                                  // 4 random values                             /// 00400
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00404
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x33333333,                                                  // 4 random values                             /// 0040c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0x0c600000,                                                  // Leading 1s:                                 /// 00418
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0041c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00420
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00424
			 0x7f800000,                                                  // inf                                         /// 00428
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0042c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00430
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00434
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00438
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0043c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00440
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00448
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0044c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00454
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x33333333,                                                  // 4 random values                             /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00460
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0046c
			 0x7fc00001,                                                  // signaling NaN                               /// 00470
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00478
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0047c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00480
			 0xbf028f5c,                                                  // -0.51                                       /// 00484
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0048c
			 0xff800000,                                                  // -inf                                        /// 00490
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004a0
			 0x55555555,                                                  // 4 random values                             /// 004a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d0
			 0x55555555,                                                  // 4 random values                             /// 004d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004e8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00500
			 0x00011111,                                                  // 9.7958E-41                                  /// 00504
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00508
			 0x3f028f5c,                                                  // 0.51                                        /// 0050c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00510
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00514
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00518
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00520
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x80000000,                                                  // -zero                                       /// 0052c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00530
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00538
			 0xffc00001,                                                  // -signaling NaN                              /// 0053c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0xff800000,                                                  // -inf                                        /// 00548
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00550
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00558
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0055c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00560
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00564
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00568
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0056c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00570
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00574
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00578
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0057c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00580
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00584
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00588
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00594
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00598
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0059c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 005b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b4
			 0xffc00001,                                                  // -signaling NaN                              /// 005b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005ec
			 0x0c600000,                                                  // Leading 1s:                                 /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00600
			 0xbf028f5c,                                                  // -0.51                                       /// 00604
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00608
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00610
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00614
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00618
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00620
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00624
			 0x0c400000,                                                  // Leading 1s:                                 /// 00628
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0062c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00630
			 0xffc00001,                                                  // -signaling NaN                              /// 00634
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00638
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0063c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00640
			 0x80011111,                                                  // -9.7958E-41                                 /// 00644
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00648
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0064c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00650
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00658
			 0x0c400000,                                                  // Leading 1s:                                 /// 0065c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x7f800000,                                                  // inf                                         /// 00668
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0066c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00670
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00674
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0068c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00690
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00694
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00698
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0069c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006a0
			 0xffc00001,                                                  // -signaling NaN                              /// 006a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c0
			 0x7fc00001,                                                  // signaling NaN                               /// 006c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c8
			 0x55555555,                                                  // 4 random values                             /// 006cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006e4
			 0x7f800000,                                                  // inf                                         /// 006e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00700
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x7fc00001,                                                  // signaling NaN                               /// 00708
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0071c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00728
			 0x7f800000,                                                  // inf                                         /// 0072c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00730
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00738
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0073c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00740
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00744
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00748
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00754
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00758
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0075c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00760
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00774
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00778
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0077c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00011111,                                                  // 9.7958E-41                                  /// 00784
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00788
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0078c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00790
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x3f028f5c,                                                  // 0.51                                        /// 0079c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007f8
			 0xff800000,                                                  // -inf                                        /// 007fc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00800
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00804
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00808
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00810
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e000001,                                                  // Trailing 1s:                                /// 00818
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00828
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0082c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0083c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00840
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00848
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0084c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7fc00001,                                                  // signaling NaN                               /// 00854
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00858
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0085c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00864
			 0x55555555,                                                  // 4 random values                             /// 00868
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0086c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00870
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00874
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00880
			 0xcb000000,                                                  // -8388608.0                                  /// 00884
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00888
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0088c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00890
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00894
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00898
			 0x00011111,                                                  // 9.7958E-41                                  /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xbf028f5c,                                                  // -0.51                                       /// 008a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00900
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00904
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00908
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0090c
			 0xbf028f5c,                                                  // -0.51                                       /// 00910
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00914
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00918
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0091c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00920
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00930
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xbf028f5c,                                                  // -0.51                                       /// 00938
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00948
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0094c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x55555555,                                                  // 4 random values                             /// 00954
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00958
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00960
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00968
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x7f800000,                                                  // inf                                         /// 00974
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00978
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00980
			 0x33333333,                                                  // 4 random values                             /// 00984
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00988
			 0x0c600000,                                                  // Leading 1s:                                 /// 0098c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00990
			 0x33333333,                                                  // 4 random values                             /// 00994
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00998
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0099c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009c0
			 0x33333333,                                                  // 4 random values                             /// 009c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009d8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a04
			 0x55555555,                                                  // 4 random values                             /// 00a08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0xff800000,                                                  // -inf                                        /// 00a20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a44
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x33333333,                                                  // 4 random values                             /// 00a5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ab4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00abc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00acc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00adc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ae0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ae8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00aec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00afc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b34
			 0x4b000000,                                                  // 8388608.0                                   /// 00b38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b4c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x80000000,                                                  // -zero                                       /// 00b7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bac
			 0xffc00001,                                                  // -signaling NaN                              /// 00bb0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00be8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bf0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x00000000,                                                  // zero                                        /// 00c08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c30
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c38
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c60
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c68
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c7c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ca4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cbc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cdc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ce4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cf8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x33333333,                                                  // 4 random values                             /// 00d10
			 0x00000000,                                                  // zero                                        /// 00d14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d40
			 0x33333333,                                                  // 4 random values                             /// 00d44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d5c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d80
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00db0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00db4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0xcb000000,                                                  // -8388608.0                                  /// 00de0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00de4
			 0x7f800000,                                                  // inf                                         /// 00de8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00df0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df4
			 0x3f028f5c,                                                  // 0.51                                        /// 00df8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dfc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0xff800000,                                                  // -inf                                        /// 00e10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x4b000000,                                                  // 8388608.0                                   /// 00e34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x33333333,                                                  // 4 random values                             /// 00e44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e70
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e78
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ebc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ec4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ecc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ee8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00efc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f00
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f04
			 0x4b000000,                                                  // 8388608.0                                   /// 00f08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00000000,                                                  // zero                                        /// 00f24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f38
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f3c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x7fc00001,                                                  // signaling NaN                               /// 00f7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x7f800000,                                                  // inf                                         /// 00f90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fa0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fb8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fbc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fc8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fdc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fe0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c7fff00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x035e1089, /// 0x0
			 0x3d85f016, /// 0x4
			 0xf26de5e9, /// 0x8
			 0x91585235, /// 0xc
			 0x806c4c5f, /// 0x10
			 0xd4c90929, /// 0x14
			 0xa4684ddd, /// 0x18
			 0x2d51e1d8, /// 0x1c
			 0xcbf06cbd, /// 0x20
			 0xd27b13d5, /// 0x24
			 0xc19b5e85, /// 0x28
			 0x7134d7bd, /// 0x2c
			 0xdf2630de, /// 0x30
			 0x59cff3dd, /// 0x34
			 0x95b5d8aa, /// 0x38
			 0x9dcd5043, /// 0x3c
			 0x5596b85b, /// 0x40
			 0xb1574249, /// 0x44
			 0x9067b093, /// 0x48
			 0x0d8e5cf3, /// 0x4c
			 0xf584cd73, /// 0x50
			 0x814c223c, /// 0x54
			 0xcae83c10, /// 0x58
			 0xfc03100c, /// 0x5c
			 0x275fef29, /// 0x60
			 0xc3501332, /// 0x64
			 0x0c95b306, /// 0x68
			 0xff7563ac, /// 0x6c
			 0x2ceb8652, /// 0x70
			 0x54bd976b, /// 0x74
			 0x952e8c41, /// 0x78
			 0x1b49fa25, /// 0x7c
			 0x2eb5e8a5, /// 0x80
			 0xd8b81873, /// 0x84
			 0x6f40b3e8, /// 0x88
			 0x4703cea1, /// 0x8c
			 0x247b223d, /// 0x90
			 0xc68a3b18, /// 0x94
			 0xbf93f16a, /// 0x98
			 0xfacb4432, /// 0x9c
			 0xfd471b43, /// 0xa0
			 0xd6e7ca28, /// 0xa4
			 0xb7b44d02, /// 0xa8
			 0x43fc58e0, /// 0xac
			 0x8a08ae28, /// 0xb0
			 0xdec1d85d, /// 0xb4
			 0x94cc4c9a, /// 0xb8
			 0x2c88cda8, /// 0xbc
			 0x03ec1e47, /// 0xc0
			 0x8650c495, /// 0xc4
			 0xc3fd0990, /// 0xc8
			 0x453c50a5, /// 0xcc
			 0xd268c4b7, /// 0xd0
			 0x1c1deecc, /// 0xd4
			 0x49f1811e, /// 0xd8
			 0x9870e2ce, /// 0xdc
			 0xff55d013, /// 0xe0
			 0x1ca53017, /// 0xe4
			 0xd0226776, /// 0xe8
			 0x738a6c9e, /// 0xec
			 0x72d5ff6b, /// 0xf0
			 0x4c4de253, /// 0xf4
			 0x8b9aa150, /// 0xf8
			 0xcbb7823c, /// 0xfc
			 0x509267dc, /// 0x100
			 0x81f5ca1d, /// 0x104
			 0x7e5aedc4, /// 0x108
			 0xdba3f6a0, /// 0x10c
			 0xf6e69f1b, /// 0x110
			 0xb67cb3cc, /// 0x114
			 0xd3860423, /// 0x118
			 0x2388015e, /// 0x11c
			 0xcf1d2f96, /// 0x120
			 0xc654de85, /// 0x124
			 0x6415a9b6, /// 0x128
			 0xcf7ec879, /// 0x12c
			 0x4c245878, /// 0x130
			 0x75f84e54, /// 0x134
			 0xfbd5707f, /// 0x138
			 0x0416e7f2, /// 0x13c
			 0x2b997f87, /// 0x140
			 0x189c7581, /// 0x144
			 0xb6ce2aa1, /// 0x148
			 0x3079cf06, /// 0x14c
			 0xf83b8fa4, /// 0x150
			 0xb35f7d3b, /// 0x154
			 0xa2dcf3bb, /// 0x158
			 0x7ddc8ecb, /// 0x15c
			 0xc7752784, /// 0x160
			 0x63ee4d35, /// 0x164
			 0x1122f2f6, /// 0x168
			 0xb6370df6, /// 0x16c
			 0xe24c87c7, /// 0x170
			 0x7650ed4b, /// 0x174
			 0xfe243e22, /// 0x178
			 0x9f627f09, /// 0x17c
			 0xd93bd98e, /// 0x180
			 0x86b923f7, /// 0x184
			 0xe0cda3a9, /// 0x188
			 0xb0028f30, /// 0x18c
			 0x39944346, /// 0x190
			 0x742c409d, /// 0x194
			 0x5e72e61e, /// 0x198
			 0x9ee45eac, /// 0x19c
			 0x5c011336, /// 0x1a0
			 0xc1035cef, /// 0x1a4
			 0xa206c711, /// 0x1a8
			 0xe896a939, /// 0x1ac
			 0x62d5611b, /// 0x1b0
			 0xef8ec698, /// 0x1b4
			 0x8055189e, /// 0x1b8
			 0xe3b3e17b, /// 0x1bc
			 0xb094e777, /// 0x1c0
			 0x0ae2415f, /// 0x1c4
			 0xa44d8f8e, /// 0x1c8
			 0x717e1360, /// 0x1cc
			 0x9d226e98, /// 0x1d0
			 0x7eab36ed, /// 0x1d4
			 0xbc12f114, /// 0x1d8
			 0x2d9c4b8f, /// 0x1dc
			 0x7f2f94a7, /// 0x1e0
			 0x6062d1d2, /// 0x1e4
			 0x88c9829e, /// 0x1e8
			 0xa3b73b03, /// 0x1ec
			 0x5e2a394c, /// 0x1f0
			 0x386917da, /// 0x1f4
			 0x7f0f0b34, /// 0x1f8
			 0x5acf7308, /// 0x1fc
			 0x453d70bb, /// 0x200
			 0x5abc6adb, /// 0x204
			 0xb2dec6f3, /// 0x208
			 0xcbe9178b, /// 0x20c
			 0x1d43d57f, /// 0x210
			 0x520e252f, /// 0x214
			 0xdc1d4e41, /// 0x218
			 0xd77ff039, /// 0x21c
			 0x57c0a0f9, /// 0x220
			 0x429ee94c, /// 0x224
			 0x2a7e5c03, /// 0x228
			 0xc7143bc8, /// 0x22c
			 0x18de47cd, /// 0x230
			 0xc953707c, /// 0x234
			 0xcf6c31eb, /// 0x238
			 0x8a7863ce, /// 0x23c
			 0xf7aa4afa, /// 0x240
			 0x47103e59, /// 0x244
			 0x3bf12705, /// 0x248
			 0xef7f18c7, /// 0x24c
			 0x2f991a26, /// 0x250
			 0x215e1dc7, /// 0x254
			 0xfa5b689b, /// 0x258
			 0x201f5727, /// 0x25c
			 0x2aef40a8, /// 0x260
			 0xd3083f5d, /// 0x264
			 0x9f7cc4f1, /// 0x268
			 0x6fba2168, /// 0x26c
			 0x1b1552d6, /// 0x270
			 0x28716c1e, /// 0x274
			 0xfac835a7, /// 0x278
			 0xa8bb13ea, /// 0x27c
			 0x297556f0, /// 0x280
			 0x88485437, /// 0x284
			 0x8f6c14d7, /// 0x288
			 0x3929f576, /// 0x28c
			 0x18b56ef0, /// 0x290
			 0xeaec14f3, /// 0x294
			 0x56c64aff, /// 0x298
			 0x0dc71ea1, /// 0x29c
			 0x0eb450c0, /// 0x2a0
			 0x32dbea29, /// 0x2a4
			 0x1c718fc2, /// 0x2a8
			 0xc0196492, /// 0x2ac
			 0xfe4ab806, /// 0x2b0
			 0x8d902469, /// 0x2b4
			 0xe33d8e62, /// 0x2b8
			 0xe7389e96, /// 0x2bc
			 0x2700efbc, /// 0x2c0
			 0x338afa18, /// 0x2c4
			 0x8a54c519, /// 0x2c8
			 0x323c7b71, /// 0x2cc
			 0x6ff8c4de, /// 0x2d0
			 0x4e45e9db, /// 0x2d4
			 0xceba7540, /// 0x2d8
			 0x04362cba, /// 0x2dc
			 0xf7ca784b, /// 0x2e0
			 0x7c219066, /// 0x2e4
			 0x53b362b5, /// 0x2e8
			 0x16d89c6c, /// 0x2ec
			 0xe6668f42, /// 0x2f0
			 0x3e8cffc2, /// 0x2f4
			 0x1daa2f67, /// 0x2f8
			 0xa39a08db, /// 0x2fc
			 0xbf904c1c, /// 0x300
			 0xdb5d0aa9, /// 0x304
			 0x6a9a9f99, /// 0x308
			 0x576df12a, /// 0x30c
			 0x0b1d7eb6, /// 0x310
			 0x08808123, /// 0x314
			 0x09100582, /// 0x318
			 0x616ca654, /// 0x31c
			 0xa6a610e7, /// 0x320
			 0xabd4a67c, /// 0x324
			 0xc3f072ab, /// 0x328
			 0xe9a27af3, /// 0x32c
			 0x689033d9, /// 0x330
			 0xa595c948, /// 0x334
			 0x83d55196, /// 0x338
			 0x54a9b37b, /// 0x33c
			 0x594e5e9e, /// 0x340
			 0x6464bc6e, /// 0x344
			 0x12eda710, /// 0x348
			 0x59931ad9, /// 0x34c
			 0xbba75cc7, /// 0x350
			 0x4d8b64fa, /// 0x354
			 0x15a22985, /// 0x358
			 0x4773f49a, /// 0x35c
			 0xa4f2959f, /// 0x360
			 0xc449f20e, /// 0x364
			 0x653503c1, /// 0x368
			 0xf9981e4d, /// 0x36c
			 0x94548812, /// 0x370
			 0xe72b2d83, /// 0x374
			 0x80ebbf4f, /// 0x378
			 0x6806e98d, /// 0x37c
			 0x19bac3ae, /// 0x380
			 0xa8c541d8, /// 0x384
			 0x6eea9024, /// 0x388
			 0x0fd7b0f7, /// 0x38c
			 0xbf10a56c, /// 0x390
			 0xaefc1b17, /// 0x394
			 0x1adcfa2a, /// 0x398
			 0x3f18e709, /// 0x39c
			 0xbe17f582, /// 0x3a0
			 0x1cbfa683, /// 0x3a4
			 0xff1f1e95, /// 0x3a8
			 0x5607f2e8, /// 0x3ac
			 0xc12a7941, /// 0x3b0
			 0x9f23b7a2, /// 0x3b4
			 0xe6cf30bc, /// 0x3b8
			 0x4cfaa59c, /// 0x3bc
			 0x85f3c4f3, /// 0x3c0
			 0x6e020a85, /// 0x3c4
			 0x15aba2c9, /// 0x3c8
			 0x2e7ae658, /// 0x3cc
			 0x6a3d911b, /// 0x3d0
			 0x282e4b93, /// 0x3d4
			 0xad056808, /// 0x3d8
			 0xc9ed9e25, /// 0x3dc
			 0xacabd813, /// 0x3e0
			 0x88229ff5, /// 0x3e4
			 0xaae9e0c0, /// 0x3e8
			 0xe798bb6f, /// 0x3ec
			 0xd92b5382, /// 0x3f0
			 0x0ff4d4ef, /// 0x3f4
			 0x04200dd7, /// 0x3f8
			 0x151f3738, /// 0x3fc
			 0xa14e178f, /// 0x400
			 0xdb724a85, /// 0x404
			 0xd8ceb7d6, /// 0x408
			 0x6512d73a, /// 0x40c
			 0x0d2cc5ad, /// 0x410
			 0x57c63828, /// 0x414
			 0x54f4ef60, /// 0x418
			 0x889ef319, /// 0x41c
			 0x575cbafe, /// 0x420
			 0xcdd2fd33, /// 0x424
			 0x0b6bde67, /// 0x428
			 0x782e0c4b, /// 0x42c
			 0x08deadb7, /// 0x430
			 0xd5c6bbf9, /// 0x434
			 0xe4eeaffa, /// 0x438
			 0x1a7cda18, /// 0x43c
			 0x2ec70368, /// 0x440
			 0xe93d4e42, /// 0x444
			 0x8d5e77ef, /// 0x448
			 0xc3544802, /// 0x44c
			 0x0d3b0c98, /// 0x450
			 0xea31cfa2, /// 0x454
			 0x54fe906e, /// 0x458
			 0x15b84230, /// 0x45c
			 0xae0638a5, /// 0x460
			 0xbb872a48, /// 0x464
			 0x543f8d4a, /// 0x468
			 0x7992bd6d, /// 0x46c
			 0xbb2f664c, /// 0x470
			 0x321b211b, /// 0x474
			 0x1727c58b, /// 0x478
			 0x61675866, /// 0x47c
			 0xf3243a7f, /// 0x480
			 0x826a92b7, /// 0x484
			 0xa5191f06, /// 0x488
			 0xaff477ff, /// 0x48c
			 0x2cb72abd, /// 0x490
			 0xafd206ea, /// 0x494
			 0x9318e6fd, /// 0x498
			 0x93e2872c, /// 0x49c
			 0x1f34cb55, /// 0x4a0
			 0xa375c4a8, /// 0x4a4
			 0x3a265b7f, /// 0x4a8
			 0x8a9bd3d5, /// 0x4ac
			 0x4cb0783a, /// 0x4b0
			 0xb370559b, /// 0x4b4
			 0x035bd9c3, /// 0x4b8
			 0xb9e36c52, /// 0x4bc
			 0xe56ea797, /// 0x4c0
			 0x044ee0ab, /// 0x4c4
			 0xbc905650, /// 0x4c8
			 0xf71c1416, /// 0x4cc
			 0x7c4b4181, /// 0x4d0
			 0x72ed0159, /// 0x4d4
			 0x0c3ef1c1, /// 0x4d8
			 0x60b3a704, /// 0x4dc
			 0x3389e9b7, /// 0x4e0
			 0x88c08b6e, /// 0x4e4
			 0x9b395420, /// 0x4e8
			 0x8e4fd8a8, /// 0x4ec
			 0xd71ef026, /// 0x4f0
			 0x4662731c, /// 0x4f4
			 0xa0d74488, /// 0x4f8
			 0x6b696188, /// 0x4fc
			 0xd1ff652a, /// 0x500
			 0xf60fb91a, /// 0x504
			 0x3873f8d6, /// 0x508
			 0xa64c37f7, /// 0x50c
			 0x26ceb478, /// 0x510
			 0x6d3b73f7, /// 0x514
			 0x9cc35cfe, /// 0x518
			 0xe82d8022, /// 0x51c
			 0x59c74c6c, /// 0x520
			 0xcad304f2, /// 0x524
			 0xf0bbca44, /// 0x528
			 0xda9e8dbd, /// 0x52c
			 0x20dadace, /// 0x530
			 0x77b1816b, /// 0x534
			 0x637978f1, /// 0x538
			 0xb9634ca0, /// 0x53c
			 0x56314d97, /// 0x540
			 0x1fd92ace, /// 0x544
			 0xfff8f0c0, /// 0x548
			 0xcde9495b, /// 0x54c
			 0x66dc103e, /// 0x550
			 0xe5d98ec6, /// 0x554
			 0x0326fd50, /// 0x558
			 0xbe01e137, /// 0x55c
			 0x457557df, /// 0x560
			 0xee5028a4, /// 0x564
			 0xa49fb059, /// 0x568
			 0x9e6111ac, /// 0x56c
			 0xdca0a54e, /// 0x570
			 0x34aaa5ab, /// 0x574
			 0xcb9ec533, /// 0x578
			 0x92e2a1c8, /// 0x57c
			 0xe956a8b2, /// 0x580
			 0x022ea9be, /// 0x584
			 0x2f52408d, /// 0x588
			 0xae62728f, /// 0x58c
			 0xec17238e, /// 0x590
			 0xb5e47186, /// 0x594
			 0x7bcdb6ce, /// 0x598
			 0x7d08b2ef, /// 0x59c
			 0x2ba513e6, /// 0x5a0
			 0xd0b667b7, /// 0x5a4
			 0xf01b7891, /// 0x5a8
			 0xcdb11d1b, /// 0x5ac
			 0x150c4f1c, /// 0x5b0
			 0x6557530a, /// 0x5b4
			 0x66e56151, /// 0x5b8
			 0x00655ed6, /// 0x5bc
			 0x7cdc61fb, /// 0x5c0
			 0x1dd29530, /// 0x5c4
			 0xcfdc8fa5, /// 0x5c8
			 0xd1ebd31e, /// 0x5cc
			 0xf35d42a2, /// 0x5d0
			 0xf3327cad, /// 0x5d4
			 0x5bd29659, /// 0x5d8
			 0xe972a6fa, /// 0x5dc
			 0x67218c94, /// 0x5e0
			 0xc1a0051a, /// 0x5e4
			 0x6e6865a6, /// 0x5e8
			 0x4a59f65a, /// 0x5ec
			 0x0dccf460, /// 0x5f0
			 0xdc055d43, /// 0x5f4
			 0x96546800, /// 0x5f8
			 0x4b2cbbc9, /// 0x5fc
			 0x5487ead0, /// 0x600
			 0x0584ef13, /// 0x604
			 0x3c5704a2, /// 0x608
			 0x7c757791, /// 0x60c
			 0xa4872c80, /// 0x610
			 0x9913d476, /// 0x614
			 0xcd747c13, /// 0x618
			 0xc114a814, /// 0x61c
			 0x7b7195fb, /// 0x620
			 0x5624019d, /// 0x624
			 0xb8d21497, /// 0x628
			 0x08545d90, /// 0x62c
			 0x841b85e2, /// 0x630
			 0x4808b176, /// 0x634
			 0xb7e4b93c, /// 0x638
			 0xd4a23f45, /// 0x63c
			 0x74ecd95b, /// 0x640
			 0xdc4af7f1, /// 0x644
			 0x67caf195, /// 0x648
			 0x95fb03b7, /// 0x64c
			 0xe92822db, /// 0x650
			 0x65199fec, /// 0x654
			 0x372b8523, /// 0x658
			 0x0d0aa566, /// 0x65c
			 0x597efc5c, /// 0x660
			 0x0f3b47d2, /// 0x664
			 0x34329407, /// 0x668
			 0x954b863c, /// 0x66c
			 0xf6ed361e, /// 0x670
			 0x0f827da3, /// 0x674
			 0x1c5e851e, /// 0x678
			 0x550e40f7, /// 0x67c
			 0x94880d43, /// 0x680
			 0x8e73cc33, /// 0x684
			 0x29978e10, /// 0x688
			 0x8c64866d, /// 0x68c
			 0x73c1534b, /// 0x690
			 0xdc7b689e, /// 0x694
			 0x782071a3, /// 0x698
			 0x3143b063, /// 0x69c
			 0x3d484ee8, /// 0x6a0
			 0xeb98619f, /// 0x6a4
			 0x4c4ea2b5, /// 0x6a8
			 0xa5705fda, /// 0x6ac
			 0xfa27aa3a, /// 0x6b0
			 0x514034fe, /// 0x6b4
			 0x986ea210, /// 0x6b8
			 0x0d32777c, /// 0x6bc
			 0x82151ecf, /// 0x6c0
			 0x504c90d4, /// 0x6c4
			 0x52f47282, /// 0x6c8
			 0x2798c356, /// 0x6cc
			 0x7e0d2038, /// 0x6d0
			 0x46009a96, /// 0x6d4
			 0xa11234f5, /// 0x6d8
			 0xa0ff8b1a, /// 0x6dc
			 0x572898e4, /// 0x6e0
			 0x09d63cf4, /// 0x6e4
			 0xd6a38aca, /// 0x6e8
			 0xc331b162, /// 0x6ec
			 0xf2309381, /// 0x6f0
			 0x86e84863, /// 0x6f4
			 0xe19d9bbe, /// 0x6f8
			 0x68bc513e, /// 0x6fc
			 0x9e213ce1, /// 0x700
			 0x0b6e875c, /// 0x704
			 0x716425eb, /// 0x708
			 0x152981ad, /// 0x70c
			 0xa38a7596, /// 0x710
			 0x9798c743, /// 0x714
			 0x55c35fec, /// 0x718
			 0xcee91a66, /// 0x71c
			 0x4dac85c1, /// 0x720
			 0x5a260a01, /// 0x724
			 0x7d84e357, /// 0x728
			 0x687d59d6, /// 0x72c
			 0xeffd0603, /// 0x730
			 0x87f1ad1c, /// 0x734
			 0x8a5da8d0, /// 0x738
			 0xe20af432, /// 0x73c
			 0x5515c47c, /// 0x740
			 0x936603ab, /// 0x744
			 0xc95643a4, /// 0x748
			 0x86a02f65, /// 0x74c
			 0x6fea78ac, /// 0x750
			 0xd63505fb, /// 0x754
			 0xc0c9fd80, /// 0x758
			 0xd89ca7d3, /// 0x75c
			 0x38364c1d, /// 0x760
			 0x15f1737a, /// 0x764
			 0xbb6ef905, /// 0x768
			 0xf3684e04, /// 0x76c
			 0x383cffdd, /// 0x770
			 0xc1f06e40, /// 0x774
			 0x86fc4505, /// 0x778
			 0x9a0eae2e, /// 0x77c
			 0xbc371047, /// 0x780
			 0x1dc49cef, /// 0x784
			 0x5576239d, /// 0x788
			 0x500c54bb, /// 0x78c
			 0x4756e325, /// 0x790
			 0x302b2189, /// 0x794
			 0xb7608669, /// 0x798
			 0x865de359, /// 0x79c
			 0x71ba9fe9, /// 0x7a0
			 0xbdb9d860, /// 0x7a4
			 0xe469ce95, /// 0x7a8
			 0xcdcdb29c, /// 0x7ac
			 0xdb3d6763, /// 0x7b0
			 0xd3cf5715, /// 0x7b4
			 0x24b82ad9, /// 0x7b8
			 0x3f54e909, /// 0x7bc
			 0xacfb9bc1, /// 0x7c0
			 0x3d166d58, /// 0x7c4
			 0x24f055f0, /// 0x7c8
			 0x884a3ef3, /// 0x7cc
			 0xb04c9de0, /// 0x7d0
			 0x0eedc4fd, /// 0x7d4
			 0xb1998b94, /// 0x7d8
			 0xe9af594a, /// 0x7dc
			 0x7bccbad2, /// 0x7e0
			 0x5564a3bf, /// 0x7e4
			 0xa2782582, /// 0x7e8
			 0xa13502ed, /// 0x7ec
			 0x53ca868a, /// 0x7f0
			 0x210a1c37, /// 0x7f4
			 0x25e7f951, /// 0x7f8
			 0x0520e04f, /// 0x7fc
			 0xf7cdb1f3, /// 0x800
			 0xbbe94c09, /// 0x804
			 0x25dcce5b, /// 0x808
			 0x2b173786, /// 0x80c
			 0x294b2f42, /// 0x810
			 0x91b3b0a8, /// 0x814
			 0x8a41bbad, /// 0x818
			 0xe46a3e1c, /// 0x81c
			 0x7d424556, /// 0x820
			 0xb9fbb306, /// 0x824
			 0xbb2e24d3, /// 0x828
			 0x161eaf22, /// 0x82c
			 0x614f41c4, /// 0x830
			 0xdffb2e79, /// 0x834
			 0x9b6043ce, /// 0x838
			 0x821ce8f7, /// 0x83c
			 0x9f1ef7f4, /// 0x840
			 0x9e321321, /// 0x844
			 0xd2d90653, /// 0x848
			 0xb96c6952, /// 0x84c
			 0xcd4dd9db, /// 0x850
			 0x1a9509b4, /// 0x854
			 0x434fa452, /// 0x858
			 0xd7e988ca, /// 0x85c
			 0x74479e73, /// 0x860
			 0x53267644, /// 0x864
			 0x7799be1a, /// 0x868
			 0xde073cc6, /// 0x86c
			 0x66b5b1b3, /// 0x870
			 0x916eec24, /// 0x874
			 0x35cc9147, /// 0x878
			 0x17f682d7, /// 0x87c
			 0x9d61aeb5, /// 0x880
			 0xc315a78a, /// 0x884
			 0xd69e7ce7, /// 0x888
			 0x6c78948b, /// 0x88c
			 0x202456ab, /// 0x890
			 0x49f91179, /// 0x894
			 0x48135520, /// 0x898
			 0xbe5adb69, /// 0x89c
			 0x748df4bd, /// 0x8a0
			 0x0bdd9859, /// 0x8a4
			 0xd20d3782, /// 0x8a8
			 0xa90a31b0, /// 0x8ac
			 0xa4da1a9c, /// 0x8b0
			 0x7577a9fd, /// 0x8b4
			 0x757d5efd, /// 0x8b8
			 0x1c183d44, /// 0x8bc
			 0x2ef40ace, /// 0x8c0
			 0xe5072ce0, /// 0x8c4
			 0x9a8e6784, /// 0x8c8
			 0xc0d364f2, /// 0x8cc
			 0xc92382f8, /// 0x8d0
			 0xe9458332, /// 0x8d4
			 0xf94c8d5b, /// 0x8d8
			 0x8d51f5ca, /// 0x8dc
			 0x8fae1372, /// 0x8e0
			 0xd973631b, /// 0x8e4
			 0x8ee54454, /// 0x8e8
			 0x2fafbc5a, /// 0x8ec
			 0xaee5409a, /// 0x8f0
			 0x8dca7a45, /// 0x8f4
			 0x32fbfd25, /// 0x8f8
			 0x991cf95a, /// 0x8fc
			 0x927de337, /// 0x900
			 0xc9985f3a, /// 0x904
			 0x3d11a88c, /// 0x908
			 0x1f3078f6, /// 0x90c
			 0x42ed7366, /// 0x910
			 0x1de19a88, /// 0x914
			 0xbf9896e0, /// 0x918
			 0xb8672798, /// 0x91c
			 0x326d64bf, /// 0x920
			 0xf90f03e7, /// 0x924
			 0xc8b3a435, /// 0x928
			 0x2362ccd3, /// 0x92c
			 0x8d3d673d, /// 0x930
			 0xf46d9b24, /// 0x934
			 0xc269f768, /// 0x938
			 0xfb6ab32b, /// 0x93c
			 0x7a590a58, /// 0x940
			 0x7e018d30, /// 0x944
			 0xd90f5a5e, /// 0x948
			 0xc1adf291, /// 0x94c
			 0x38c5e2f9, /// 0x950
			 0x4062f4ee, /// 0x954
			 0xdc65482b, /// 0x958
			 0xc2fca06d, /// 0x95c
			 0xf459a9b3, /// 0x960
			 0x9f019b01, /// 0x964
			 0x17484cd7, /// 0x968
			 0x25085228, /// 0x96c
			 0xe09b3bd3, /// 0x970
			 0x73306785, /// 0x974
			 0xa2396be1, /// 0x978
			 0xb7d4089c, /// 0x97c
			 0x7482d069, /// 0x980
			 0x93632266, /// 0x984
			 0x758d1259, /// 0x988
			 0x785c9b27, /// 0x98c
			 0x5c2e8528, /// 0x990
			 0x084cd2ef, /// 0x994
			 0x4f470576, /// 0x998
			 0x009eb466, /// 0x99c
			 0x191683a2, /// 0x9a0
			 0x59f85e3f, /// 0x9a4
			 0x0323c396, /// 0x9a8
			 0x2ee78df2, /// 0x9ac
			 0xf2e8dc4a, /// 0x9b0
			 0x830af25e, /// 0x9b4
			 0xdb496c72, /// 0x9b8
			 0x9581692b, /// 0x9bc
			 0xa3bbfc64, /// 0x9c0
			 0x95571c9d, /// 0x9c4
			 0xb54ddc1f, /// 0x9c8
			 0xd2164f25, /// 0x9cc
			 0x91923a89, /// 0x9d0
			 0x88824d13, /// 0x9d4
			 0x3c058c01, /// 0x9d8
			 0x724ca06d, /// 0x9dc
			 0xd54f38eb, /// 0x9e0
			 0x815db6f1, /// 0x9e4
			 0x22d41c29, /// 0x9e8
			 0x2c3c30a2, /// 0x9ec
			 0xba2f36a1, /// 0x9f0
			 0x6410ecb0, /// 0x9f4
			 0x65d9b355, /// 0x9f8
			 0x88d034bd, /// 0x9fc
			 0xa019badc, /// 0xa00
			 0xbae3f31b, /// 0xa04
			 0x52f875c4, /// 0xa08
			 0xa7534daf, /// 0xa0c
			 0xfc84ff95, /// 0xa10
			 0xb0db0a41, /// 0xa14
			 0x66c97800, /// 0xa18
			 0x6cf8c5d3, /// 0xa1c
			 0xac4a80a4, /// 0xa20
			 0xc2540cf2, /// 0xa24
			 0x19f30ad8, /// 0xa28
			 0x0089a7b9, /// 0xa2c
			 0x491e1250, /// 0xa30
			 0xc7d5ad94, /// 0xa34
			 0x5a045d63, /// 0xa38
			 0xfabeaf0c, /// 0xa3c
			 0xd16cf16e, /// 0xa40
			 0x44afaa2e, /// 0xa44
			 0x1b99998f, /// 0xa48
			 0xe8f0a3a8, /// 0xa4c
			 0xcc3e9d19, /// 0xa50
			 0xb17d0565, /// 0xa54
			 0x5fb619cf, /// 0xa58
			 0xd1fd1f6e, /// 0xa5c
			 0xfc759e07, /// 0xa60
			 0xd5b63108, /// 0xa64
			 0xb089a1eb, /// 0xa68
			 0xd2e70a45, /// 0xa6c
			 0xcd3b878d, /// 0xa70
			 0x46f66fe0, /// 0xa74
			 0xcee9e254, /// 0xa78
			 0x39a85a6a, /// 0xa7c
			 0xaa80d0e1, /// 0xa80
			 0xa43f80bf, /// 0xa84
			 0xf2019f7a, /// 0xa88
			 0xaad40802, /// 0xa8c
			 0xa10165b4, /// 0xa90
			 0x64288f19, /// 0xa94
			 0x6c203531, /// 0xa98
			 0xb531eb12, /// 0xa9c
			 0x34b31eea, /// 0xaa0
			 0x8d37f7b1, /// 0xaa4
			 0xac7fa7ed, /// 0xaa8
			 0x666ba048, /// 0xaac
			 0x948b5167, /// 0xab0
			 0x5948e317, /// 0xab4
			 0xce9119cb, /// 0xab8
			 0xcda2e1a7, /// 0xabc
			 0x6446e000, /// 0xac0
			 0x8895fbbb, /// 0xac4
			 0x59d66856, /// 0xac8
			 0x8eaebbc8, /// 0xacc
			 0x44d15bb2, /// 0xad0
			 0x751b9e02, /// 0xad4
			 0x6a03e0fb, /// 0xad8
			 0xfb0ee0bb, /// 0xadc
			 0x7821539d, /// 0xae0
			 0x1256751e, /// 0xae4
			 0xa026942c, /// 0xae8
			 0xcf974027, /// 0xaec
			 0x02fb7256, /// 0xaf0
			 0x4b185ddf, /// 0xaf4
			 0xf782fd2c, /// 0xaf8
			 0x708a28e5, /// 0xafc
			 0x17a35ab1, /// 0xb00
			 0x6d134037, /// 0xb04
			 0x958dab19, /// 0xb08
			 0x30c94f7e, /// 0xb0c
			 0xce5d61b9, /// 0xb10
			 0xe558138d, /// 0xb14
			 0xa96cffb2, /// 0xb18
			 0x4eca9bea, /// 0xb1c
			 0xe9ddad3a, /// 0xb20
			 0xc195489f, /// 0xb24
			 0x3de7a5be, /// 0xb28
			 0x14ce8210, /// 0xb2c
			 0x83a4942f, /// 0xb30
			 0xfc48943c, /// 0xb34
			 0xd0694306, /// 0xb38
			 0x4151208b, /// 0xb3c
			 0x7d046698, /// 0xb40
			 0xe8c83c69, /// 0xb44
			 0x6bba24ad, /// 0xb48
			 0xa64752cc, /// 0xb4c
			 0x7ebae395, /// 0xb50
			 0x678a7f5b, /// 0xb54
			 0xe101dddb, /// 0xb58
			 0x4a0c6aac, /// 0xb5c
			 0x9811f5f4, /// 0xb60
			 0x7e0a76aa, /// 0xb64
			 0x5969b090, /// 0xb68
			 0xcbae2802, /// 0xb6c
			 0xcab454f4, /// 0xb70
			 0xa2c4bcdc, /// 0xb74
			 0x3576a4d7, /// 0xb78
			 0x54c5ed89, /// 0xb7c
			 0x0e2c689c, /// 0xb80
			 0x9f2fbf49, /// 0xb84
			 0x2452973f, /// 0xb88
			 0xfae7038c, /// 0xb8c
			 0x0a8a9c58, /// 0xb90
			 0xb9c5909d, /// 0xb94
			 0x5823b58a, /// 0xb98
			 0x438cb4f9, /// 0xb9c
			 0x63fedb8e, /// 0xba0
			 0x8c90e226, /// 0xba4
			 0xca8659e8, /// 0xba8
			 0x52657f22, /// 0xbac
			 0xd5002047, /// 0xbb0
			 0xdaff09a5, /// 0xbb4
			 0x2baa9879, /// 0xbb8
			 0x879541cb, /// 0xbbc
			 0x99d10114, /// 0xbc0
			 0x031fac18, /// 0xbc4
			 0x150556f4, /// 0xbc8
			 0xd9b03c65, /// 0xbcc
			 0x6491f27f, /// 0xbd0
			 0x7a558b15, /// 0xbd4
			 0xc17db3df, /// 0xbd8
			 0xb392b2ed, /// 0xbdc
			 0x1ddc85e4, /// 0xbe0
			 0xbe2b307c, /// 0xbe4
			 0xeafe6fbd, /// 0xbe8
			 0x120c98cc, /// 0xbec
			 0xf958c0eb, /// 0xbf0
			 0xb786ce1d, /// 0xbf4
			 0x323245fe, /// 0xbf8
			 0xe4f22225, /// 0xbfc
			 0x318e5db0, /// 0xc00
			 0xc6a1fe3e, /// 0xc04
			 0x03361bd7, /// 0xc08
			 0x0dbd06a8, /// 0xc0c
			 0xe41d8a9b, /// 0xc10
			 0xe7827994, /// 0xc14
			 0xe83ee6d1, /// 0xc18
			 0x9ebd92db, /// 0xc1c
			 0xec2f9eb6, /// 0xc20
			 0xa8fea114, /// 0xc24
			 0x867db82d, /// 0xc28
			 0x3df212b7, /// 0xc2c
			 0x813c70f6, /// 0xc30
			 0x2690bc7e, /// 0xc34
			 0xac7a00fd, /// 0xc38
			 0x33b1b45d, /// 0xc3c
			 0x802c2ac6, /// 0xc40
			 0x2c045690, /// 0xc44
			 0x3645a8aa, /// 0xc48
			 0x3b9956ef, /// 0xc4c
			 0xe937f5d0, /// 0xc50
			 0xc5fce57e, /// 0xc54
			 0x89c5f238, /// 0xc58
			 0x9ccbff58, /// 0xc5c
			 0x00276ef0, /// 0xc60
			 0x286eac80, /// 0xc64
			 0x0e192bec, /// 0xc68
			 0x800b2a73, /// 0xc6c
			 0xf0147577, /// 0xc70
			 0x3c525d4f, /// 0xc74
			 0x89589b2e, /// 0xc78
			 0x06e2d36a, /// 0xc7c
			 0x9b898614, /// 0xc80
			 0xa9085045, /// 0xc84
			 0x7bc17cd7, /// 0xc88
			 0xddba14f0, /// 0xc8c
			 0x0c805b15, /// 0xc90
			 0x00e11d64, /// 0xc94
			 0x4c1c3157, /// 0xc98
			 0xa871729f, /// 0xc9c
			 0xee236cdf, /// 0xca0
			 0xb373272f, /// 0xca4
			 0x907f412d, /// 0xca8
			 0x91ae7850, /// 0xcac
			 0x1fe534bb, /// 0xcb0
			 0x6b934af6, /// 0xcb4
			 0x929c8b9e, /// 0xcb8
			 0x0201bb72, /// 0xcbc
			 0xf02a37f6, /// 0xcc0
			 0xfad181f6, /// 0xcc4
			 0xd5c1609c, /// 0xcc8
			 0xdd96b3ca, /// 0xccc
			 0xa0a9feb7, /// 0xcd0
			 0x45c7c0b9, /// 0xcd4
			 0x5e8258be, /// 0xcd8
			 0x9d38e70d, /// 0xcdc
			 0x169548cc, /// 0xce0
			 0x855009c4, /// 0xce4
			 0xd22ce060, /// 0xce8
			 0x8acffb57, /// 0xcec
			 0x80b45702, /// 0xcf0
			 0x40e5a62f, /// 0xcf4
			 0x56191d5b, /// 0xcf8
			 0xda5656d4, /// 0xcfc
			 0x8c846a22, /// 0xd00
			 0xfd6451b5, /// 0xd04
			 0x46d98bd2, /// 0xd08
			 0x2b1467d2, /// 0xd0c
			 0x7b5f272d, /// 0xd10
			 0x12cd0777, /// 0xd14
			 0xeb32f04b, /// 0xd18
			 0xc4db378c, /// 0xd1c
			 0x2398f578, /// 0xd20
			 0x3064ac9d, /// 0xd24
			 0xda510c8b, /// 0xd28
			 0xb92cb3d9, /// 0xd2c
			 0x66f8cec9, /// 0xd30
			 0x69ebab2c, /// 0xd34
			 0x649c30ac, /// 0xd38
			 0x0265b061, /// 0xd3c
			 0x7431ffbe, /// 0xd40
			 0xd78e42a1, /// 0xd44
			 0xe7523a34, /// 0xd48
			 0x1b028b34, /// 0xd4c
			 0x5a1082df, /// 0xd50
			 0x308734cc, /// 0xd54
			 0x68e78c5e, /// 0xd58
			 0x9b81ef32, /// 0xd5c
			 0x5441c1f4, /// 0xd60
			 0x20ed63bd, /// 0xd64
			 0x3aa5a6d2, /// 0xd68
			 0x91a79c27, /// 0xd6c
			 0xa2f20493, /// 0xd70
			 0xaee68774, /// 0xd74
			 0xd4dfd0c0, /// 0xd78
			 0x35344c36, /// 0xd7c
			 0x07e30d2b, /// 0xd80
			 0x665512c3, /// 0xd84
			 0x82181539, /// 0xd88
			 0xacda980c, /// 0xd8c
			 0x1bc37aed, /// 0xd90
			 0x41878024, /// 0xd94
			 0xe1ec7d6e, /// 0xd98
			 0x2402f404, /// 0xd9c
			 0xa62619f3, /// 0xda0
			 0xd98c75fd, /// 0xda4
			 0xb7aec48d, /// 0xda8
			 0xebe04a1f, /// 0xdac
			 0xa27d95ea, /// 0xdb0
			 0x282136be, /// 0xdb4
			 0x5c716092, /// 0xdb8
			 0x82d9092b, /// 0xdbc
			 0xf6f20e4d, /// 0xdc0
			 0x64ff2492, /// 0xdc4
			 0xd4041051, /// 0xdc8
			 0xd4c2ab07, /// 0xdcc
			 0x12ca4795, /// 0xdd0
			 0x548a397a, /// 0xdd4
			 0x8f3a1454, /// 0xdd8
			 0x028e129f, /// 0xddc
			 0x5ea26af0, /// 0xde0
			 0xa3baeb7e, /// 0xde4
			 0x193570f7, /// 0xde8
			 0x32632753, /// 0xdec
			 0x88ab4670, /// 0xdf0
			 0xf562310c, /// 0xdf4
			 0xde8c41b5, /// 0xdf8
			 0x4b07ada4, /// 0xdfc
			 0xeaae8eae, /// 0xe00
			 0xa033097a, /// 0xe04
			 0xd9d30174, /// 0xe08
			 0xb8f0d5ad, /// 0xe0c
			 0x20034fff, /// 0xe10
			 0x9474cbe1, /// 0xe14
			 0xf71d99f5, /// 0xe18
			 0x47de4e14, /// 0xe1c
			 0x5e555b21, /// 0xe20
			 0x76a1ece3, /// 0xe24
			 0xc9f77a74, /// 0xe28
			 0x6dfc2c41, /// 0xe2c
			 0x0b4533ca, /// 0xe30
			 0x219e84f8, /// 0xe34
			 0x28a7603f, /// 0xe38
			 0x29da1a60, /// 0xe3c
			 0x7605923a, /// 0xe40
			 0x89c8f978, /// 0xe44
			 0xba96e28f, /// 0xe48
			 0x64f8e31a, /// 0xe4c
			 0xa280728f, /// 0xe50
			 0x2bd4e6a0, /// 0xe54
			 0x007f697c, /// 0xe58
			 0x11e53421, /// 0xe5c
			 0x750f3615, /// 0xe60
			 0x5ed267f1, /// 0xe64
			 0x03359704, /// 0xe68
			 0x75e908c0, /// 0xe6c
			 0x47cd014d, /// 0xe70
			 0x6a51404e, /// 0xe74
			 0x3d6a0217, /// 0xe78
			 0xaff563a8, /// 0xe7c
			 0x5ce4c991, /// 0xe80
			 0xafaedeee, /// 0xe84
			 0x33ff6acf, /// 0xe88
			 0x911fd7b7, /// 0xe8c
			 0x4c527a34, /// 0xe90
			 0x8754d677, /// 0xe94
			 0x5c762cf7, /// 0xe98
			 0xba956874, /// 0xe9c
			 0xeedfc8a3, /// 0xea0
			 0x3a0f0738, /// 0xea4
			 0x43e9d19a, /// 0xea8
			 0x5f90bd59, /// 0xeac
			 0x916b77d8, /// 0xeb0
			 0xc365e296, /// 0xeb4
			 0x976a7b3f, /// 0xeb8
			 0xd49b2156, /// 0xebc
			 0xcc6efb46, /// 0xec0
			 0xecd79c0f, /// 0xec4
			 0xf04eed40, /// 0xec8
			 0x9205132b, /// 0xecc
			 0xa24c99a0, /// 0xed0
			 0xda733a5a, /// 0xed4
			 0x906e6633, /// 0xed8
			 0x7db31254, /// 0xedc
			 0xf49d5497, /// 0xee0
			 0xd602b487, /// 0xee4
			 0x5871a181, /// 0xee8
			 0xb3135f82, /// 0xeec
			 0x6511bd33, /// 0xef0
			 0x8d1b18ee, /// 0xef4
			 0x41dbf431, /// 0xef8
			 0x67a05571, /// 0xefc
			 0x56b8e387, /// 0xf00
			 0x1884228b, /// 0xf04
			 0x83335d41, /// 0xf08
			 0x8064dd4b, /// 0xf0c
			 0x7dbd3f30, /// 0xf10
			 0x10293597, /// 0xf14
			 0xc66c02a7, /// 0xf18
			 0x6f4f1495, /// 0xf1c
			 0x5d68b128, /// 0xf20
			 0x66208c04, /// 0xf24
			 0x08a6981e, /// 0xf28
			 0x1acecdf0, /// 0xf2c
			 0xd570f8e6, /// 0xf30
			 0x4e4aebcd, /// 0xf34
			 0x10a726dd, /// 0xf38
			 0x5b854edc, /// 0xf3c
			 0xcac16087, /// 0xf40
			 0xbf156884, /// 0xf44
			 0xe81295dd, /// 0xf48
			 0xd863c714, /// 0xf4c
			 0x6b3b91a9, /// 0xf50
			 0x3aa6f472, /// 0xf54
			 0xfa4c5087, /// 0xf58
			 0x32c0d4bd, /// 0xf5c
			 0x82997024, /// 0xf60
			 0x777f9bdb, /// 0xf64
			 0xf15ed7f6, /// 0xf68
			 0x8d36025d, /// 0xf6c
			 0xfbef847e, /// 0xf70
			 0x6e8f03d0, /// 0xf74
			 0xa6ab3f6a, /// 0xf78
			 0xf302dc10, /// 0xf7c
			 0xfcbbc028, /// 0xf80
			 0x63b0e005, /// 0xf84
			 0xcf968e76, /// 0xf88
			 0xfdf5b1bc, /// 0xf8c
			 0x98936272, /// 0xf90
			 0x081d223f, /// 0xf94
			 0x7ddb235f, /// 0xf98
			 0xe8925bf8, /// 0xf9c
			 0xbe6bf8e7, /// 0xfa0
			 0xe31c48d6, /// 0xfa4
			 0xcf76de11, /// 0xfa8
			 0xf0a1f9ae, /// 0xfac
			 0xd6d1965c, /// 0xfb0
			 0x6ca8f2a4, /// 0xfb4
			 0xe5a6b37d, /// 0xfb8
			 0xc43a1a8e, /// 0xfbc
			 0xce8f3623, /// 0xfc0
			 0x16a09245, /// 0xfc4
			 0xda429c58, /// 0xfc8
			 0x9c9f7450, /// 0xfcc
			 0x0a85185b, /// 0xfd0
			 0x9a424cc2, /// 0xfd4
			 0xf385cfff, /// 0xfd8
			 0xd128314a, /// 0xfdc
			 0xe765b953, /// 0xfe0
			 0x09fbb020, /// 0xfe4
			 0x142cd818, /// 0xfe8
			 0xdb2be67f, /// 0xfec
			 0x6bdbbcad, /// 0xff0
			 0x4da32787, /// 0xff4
			 0xbc4947aa, /// 0xff8
			 0x6a6545fa /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00004
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00008
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0000c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00010
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00014
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00018
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00020
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00028
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0002c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00034
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00038
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0003c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00040
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00044
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x33333333,                                                  // 4 random values                             /// 00050
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00054
			 0x80011111,                                                  // -9.7958E-41                                 /// 00058
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0005c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00060
			 0x80000000,                                                  // -zero                                       /// 00064
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00068
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00070
			 0x0e000007,                                                  // Trailing 1s:                                /// 00074
			 0x0c700000,                                                  // Leading 1s:                                 /// 00078
			 0x00000000,                                                  // zero                                        /// 0007c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00080
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00084
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7fc00001,                                                  // signaling NaN                               /// 00090
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00094
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00098
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0009c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000c8
			 0x33333333,                                                  // 4 random values                             /// 000cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00100
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00104
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00110
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00114
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00118
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x4b000000,                                                  // 8388608.0                                   /// 00120
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0e000003,                                                  // Trailing 1s:                                /// 0012c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00130
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00134
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0013c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0014c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00150
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00154
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00158
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00160
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00168
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0016c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00170
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00174
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00178
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00180
			 0x80011111,                                                  // -9.7958E-41                                 /// 00184
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00188
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0018c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00190
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00194
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0019c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00200
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00204
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00208
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00210
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00214
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0021c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00220
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x00000000,                                                  // zero                                        /// 00230
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00234
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00238
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00244
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00248
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00250
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00254
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0025c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00260
			 0x3f028f5c,                                                  // 0.51                                        /// 00264
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00268
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00270
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00274
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0027c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0xcb000000,                                                  // -8388608.0                                  /// 0028c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00290
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00294
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ec
			 0x55555555,                                                  // 4 random values                             /// 002f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00304
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00310
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00318
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0031c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00328
			 0x4b000000,                                                  // 8388608.0                                   /// 0032c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00330
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00338
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0033c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00340
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00348
			 0x3f028f5c,                                                  // 0.51                                        /// 0034c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00354
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0035c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00364
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00368
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0036c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00370
			 0x0c780000,                                                  // Leading 1s:                                 /// 00374
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00378
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0037c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00380
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0038c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00390
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0039c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x7f800000,                                                  // inf                                         /// 003f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00400
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00414
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00418
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0041c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00420
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00428
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00434
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0043c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00444
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00448
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00454
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x7f800000,                                                  // inf                                         /// 00460
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00464
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00468
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0046c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00470
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00474
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0047c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00488
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00490
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00494
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00498
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0049c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004b4
			 0xff800000,                                                  // -inf                                        /// 004b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d8
			 0x7f800000,                                                  // inf                                         /// 004dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00500
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00504
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00508
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x0c700000,                                                  // Leading 1s:                                 /// 00514
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00518
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0051c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00520
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00524
			 0x00000000,                                                  // zero                                        /// 00528
			 0x00011111,                                                  // 9.7958E-41                                  /// 0052c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00530
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00534
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00538
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0053c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00540
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00544
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0054c
			 0xbf028f5c,                                                  // -0.51                                       /// 00550
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00554
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00558
			 0x0e000007,                                                  // Trailing 1s:                                /// 0055c
			 0x55555555,                                                  // 4 random values                             /// 00560
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00564
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0056c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00570
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00574
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00578
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0057c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00580
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80000000,                                                  // -zero                                       /// 00588
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0058c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00594
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00598
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0059c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 005c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00600
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00608
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00610
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00618
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0061c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00620
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00624
			 0x7fc00001,                                                  // signaling NaN                               /// 00628
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0062c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00630
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00638
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0063c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00640
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00644
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00648
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0064c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00650
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00654
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00660
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00664
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00668
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0066c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00674
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0067c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00680
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00684
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00690
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00694
			 0x0c780000,                                                  // Leading 1s:                                 /// 00698
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ac
			 0xff800000,                                                  // -inf                                        /// 006b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x4b000000,                                                  // 8388608.0                                   /// 006c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00704
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00710
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00714
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0071c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00724
			 0x0e000003,                                                  // Trailing 1s:                                /// 00728
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0072c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00730
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00734
			 0x7f800000,                                                  // inf                                         /// 00738
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0073c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00748
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0074c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00754
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00758
			 0x3f028f5c,                                                  // 0.51                                        /// 0075c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e000001,                                                  // Trailing 1s:                                /// 00764
			 0x80011111,                                                  // -9.7958E-41                                 /// 00768
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0076c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00774
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0077c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00780
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00784
			 0xffc00001,                                                  // -signaling NaN                              /// 00788
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0078c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00790
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0079c
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007ac
			 0x4b000000,                                                  // 8388608.0                                   /// 007b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007d4
			 0xbf028f5c,                                                  // -0.51                                       /// 007d8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00804
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x00000000,                                                  // zero                                        /// 0080c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00810
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00818
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00820
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00824
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00828
			 0xbf028f5c,                                                  // -0.51                                       /// 0082c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00830
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00834
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00838
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0083c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0084c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00850
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00854
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00858
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0085c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00860
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00870
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00874
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00878
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00884
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00888
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0088c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00890
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00898
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0089c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff800000,                                                  // -inf                                        /// 008bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c4
			 0x3f028f5c,                                                  // 0.51                                        /// 008c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00900
			 0x00011111,                                                  // 9.7958E-41                                  /// 00904
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00908
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00924
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0092c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00930
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00934
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00940
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00944
			 0x00011111,                                                  // 9.7958E-41                                  /// 00948
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0094c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00958
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0095c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00960
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00964
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00968
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0096c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00970
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00974
			 0x4b000000,                                                  // 8388608.0                                   /// 00978
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0097c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00990
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00994
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0xcb000000,                                                  // -8388608.0                                  /// 009c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x33333333,                                                  // 4 random values                             /// 00a08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a2c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a38
			 0xcb000000,                                                  // -8388608.0                                  /// 00a3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a88
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a98
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aa8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ab0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ab8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ac0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00aec
			 0xbf028f5c,                                                  // -0.51                                       /// 00af0
			 0x80000000,                                                  // -zero                                       /// 00af4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00af8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00afc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b20
			 0x80000000,                                                  // -zero                                       /// 00b24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7fc00001,                                                  // signaling NaN                               /// 00b84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x55555555,                                                  // 4 random values                             /// 00bac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb0
			 0x7f800000,                                                  // inf                                         /// 00bb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bc4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00be0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00be8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bf0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bf4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c28
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c34
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xbf028f5c,                                                  // -0.51                                       /// 00c6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ca0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cb4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ccc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ce4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ce8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cf0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00000000,                                                  // zero                                        /// 00cf8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x55555555,                                                  // 4 random values                             /// 00d1c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d20
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d28
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0xbf028f5c,                                                  // -0.51                                       /// 00d68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00da8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00db0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00db8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dcc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00de4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00de8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e24
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x55555555,                                                  // 4 random values                             /// 00e40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00eac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80000000,                                                  // -zero                                       /// 00ec0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ec4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ec8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ecc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ed0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000000,                                                  // zero                                        /// 00edc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ee8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ef4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00efc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f1c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f2c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f48
			 0xffc00001,                                                  // -signaling NaN                              /// 00f4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f50
			 0x33333333,                                                  // 4 random values                             /// 00f54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f5c
			 0xff800000,                                                  // -inf                                        /// 00f60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fa0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fc0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fdc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fe8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ff0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7fff80                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			 // vpu register f0 is designated 0 register
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,
			 0x00000000,

			 // vpu register f1 is designated offset register for memory access ops
			 0x0000014c,
			 0x00000708,
			 0x00000038,
			 0x000001b0,
			 0x00000034,
			 0x000001e0,
			 0x0000055c,
			 0x0000013c,

			 /// vpu register f2
			 0x40400000,
			 0x41c80000,
			 0x41900000,
			 0x41000000,
			 0x41880000,
			 0x41300000,
			 0x41c80000,
			 0x41900000,

			 /// vpu register f3
			 0x41e00000,
			 0x41a00000,
			 0x41000000,
			 0x41a80000,
			 0x41a80000,
			 0x41880000,
			 0x40a00000,
			 0x40800000,

			 /// vpu register f4
			 0x41a80000,
			 0x41300000,
			 0x41400000,
			 0x41d80000,
			 0x41c80000,
			 0x42000000,
			 0x41d80000,
			 0x41a00000,

			 /// vpu register f5
			 0x3f800000,
			 0x41900000,
			 0x41980000,
			 0x41300000,
			 0x41980000,
			 0x41980000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f6
			 0x41200000,
			 0x41400000,
			 0x41c00000,
			 0x41b80000,
			 0x41b80000,
			 0x40e00000,
			 0x41d80000,
			 0x41b80000,

			 /// vpu register f7
			 0x40800000,
			 0x41200000,
			 0x41c00000,
			 0x42000000,
			 0x42000000,
			 0x41a80000,
			 0x41600000,
			 0x41f80000,

			 /// vpu register f8
			 0x41b00000,
			 0x41300000,
			 0x40c00000,
			 0x41100000,
			 0x41300000,
			 0x40400000,
			 0x40c00000,
			 0x41a00000,

			 /// vpu register f9
			 0x41b00000,
			 0x41500000,
			 0x40e00000,
			 0x40000000,
			 0x41e80000,
			 0x41880000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f10
			 0x41600000,
			 0x41c00000,
			 0x41500000,
			 0x41980000,
			 0x40c00000,
			 0x40c00000,
			 0x41c00000,
			 0x41600000,

			 /// vpu register f11
			 0x40a00000,
			 0x41d80000,
			 0x41800000,
			 0x41d80000,
			 0x3f800000,
			 0x42000000,
			 0x40c00000,
			 0x3f800000,

			 /// vpu register f12
			 0x41a00000,
			 0x40000000,
			 0x3f800000,
			 0x41900000,
			 0x42000000,
			 0x41700000,
			 0x40000000,
			 0x41900000,

			 /// vpu register f13
			 0x41d80000,
			 0x40000000,
			 0x41b00000,
			 0x41400000,
			 0x41e00000,
			 0x41600000,
			 0x41b00000,
			 0x40c00000,

			 /// vpu register f14
			 0x41b80000,
			 0x41600000,
			 0x40a00000,
			 0x41d80000,
			 0x41b80000,
			 0x41400000,
			 0x41980000,
			 0x40400000,

			 /// vpu register f15
			 0x41a80000,
			 0x41c00000,
			 0x41e80000,
			 0x41100000,
			 0x41980000,
			 0x41b00000,
			 0x41a80000,
			 0x42000000,

			 /// vpu register f16
			 0x41700000,
			 0x41900000,
			 0x41100000,
			 0x40000000,
			 0x41900000,
			 0x41f00000,
			 0x41980000,
			 0x40400000,

			 /// vpu register f17
			 0x41200000,
			 0x40a00000,
			 0x41b80000,
			 0x41d00000,
			 0x41300000,
			 0x41d00000,
			 0x41f00000,
			 0x41600000,

			 /// vpu register f18
			 0x41300000,
			 0x41c00000,
			 0x40c00000,
			 0x41b00000,
			 0x42000000,
			 0x41980000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f19
			 0x41880000,
			 0x40400000,
			 0x41300000,
			 0x41b00000,
			 0x41400000,
			 0x3f800000,
			 0x41900000,
			 0x41b80000,

			 /// vpu register f20
			 0x41980000,
			 0x40800000,
			 0x41980000,
			 0x41d00000,
			 0x41900000,
			 0x41a00000,
			 0x41700000,
			 0x41000000,

			 /// vpu register f21
			 0x41800000,
			 0x41b00000,
			 0x41f80000,
			 0x41b80000,
			 0x41a80000,
			 0x40800000,
			 0x41f80000,
			 0x41c00000,

			 /// vpu register f22
			 0x42000000,
			 0x41880000,
			 0x41f00000,
			 0x41b00000,
			 0x41300000,
			 0x41b00000,
			 0x41980000,
			 0x41400000,

			 /// vpu register f23
			 0x41500000,
			 0x41900000,
			 0x41500000,
			 0x41a80000,
			 0x41b80000,
			 0x41b00000,
			 0x41500000,
			 0x41a80000,

			 /// vpu register f24
			 0x41c80000,
			 0x41a00000,
			 0x40800000,
			 0x41400000,
			 0x3f800000,
			 0x40e00000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f25
			 0x40000000,
			 0x41d00000,
			 0x41800000,
			 0x41300000,
			 0x42000000,
			 0x40400000,
			 0x41d80000,
			 0x41e80000,

			 /// vpu register f26
			 0x41400000,
			 0x41d00000,
			 0x41f00000,
			 0x41700000,
			 0x41a80000,
			 0x41980000,
			 0x41600000,
			 0x41980000,

			 /// vpu register f27
			 0x41c80000,
			 0x41f80000,
			 0x41d80000,
			 0x41f80000,
			 0x41b00000,
			 0x41e00000,
			 0x41880000,
			 0x40c00000,

			 /// vpu register f28
			 0x40800000,
			 0x41d80000,
			 0x41f00000,
			 0x40c00000,
			 0x41500000,
			 0x40e00000,
			 0x41e00000,
			 0x40a00000,

			 /// vpu register f29
			 0x41b80000,
			 0x40e00000,
			 0x41a00000,
			 0x41000000,
			 0x41600000,
			 0x3f800000,
			 0x41d00000,
			 0x41d80000,

			 /// vpu register f30
			 0x42000000,
			 0x41c80000,
			 0x40800000,
			 0x41b80000,
			 0x41880000,
			 0x40800000,
			 0x41200000,
			 0x41d80000,

			 /// vpu register f31
			 0x41c80000,
			 0x42000000,
			 0x41600000,
			 0x42000000,
			 0x41e00000,
			 0x41c00000,
			 0x41c00000,
			 0x3f800000     /// 1.0                   
	};


	C_TEST_START;

	__asm__ __volatile__ (
		"LBL_C_TEST_START:\n"
		"j LBL_SETUP_EV\n"
		".align 12\n"
		"M_MODE_E_H:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR\n"
		"LBL_SETUP_SCP:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
		"LBL_RET_TO_CLR:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
		"LBL_S_MODE_E_H:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
		"LBL_SETUP_EV:\n"
		"la x5, M_MODE_E_H\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"mv x11, %[m_11]\n"
		"addi x11, x11, 2047\n"
		"addi x11, x11, 1\n"
		"mv x12, %[m_12]\n"
		"addi x12, x12, 2047\n"
		"addi x12, x12, 1\n"
		"mv x13, %[m_13]\n"
		"addi x13, x13, 2047\n"
		"addi x13, x13, 1\n"
		"mv x14, %[m_14]\n"
		"addi x14, x14, 2047\n"
		"addi x14, x14, 1\n"
		"mv x15, %[m_15]\n"
		"addi x15, x15, 2047\n"
		"addi x15, x15, 1\n"
		"mv x4, %[operands]\n"
		"flw.ps f0,  0 (x4)\n"
		"flw.ps f1,  32 (x4)\n"
		"flw.ps f2,  64 (x4)\n"
		"flw.ps f3,  96 (x4)\n"
		"flw.ps f4,  128 (x4)\n"
		"flw.ps f5,  160 (x4)\n"
		"flw.ps f6,  192 (x4)\n"
		"flw.ps f7,  224 (x4)\n"
		"flw.ps f8,  256 (x4)\n"
		"flw.ps f9,  288 (x4)\n"
		"flw.ps f10,  320 (x4)\n"
		"flw.ps f11,  352 (x4)\n"
		"flw.ps f12,  384 (x4)\n"
		"flw.ps f13,  416 (x4)\n"
		"flw.ps f14,  448 (x4)\n"
		"flw.ps f15,  480 (x4)\n"
		"flw.ps f16,  512 (x4)\n"
		"flw.ps f17,  544 (x4)\n"
		"flw.ps f18,  576 (x4)\n"
		"flw.ps f19,  608 (x4)\n"
		"flw.ps f20,  640 (x4)\n"
		"flw.ps f21,  672 (x4)\n"
		"flw.ps f22,  704 (x4)\n"
		"flw.ps f23,  736 (x4)\n"
		"flw.ps f24,  768 (x4)\n"
		"flw.ps f25,  800 (x4)\n"
		"flw.ps f26,  832 (x4)\n"
		"flw.ps f27,  864 (x4)\n"
		"flw.ps f28,  896 (x4)\n"
		"flw.ps f29,  928 (x4)\n"
		"flw.ps f30,  960 (x4)\n"
		"flw.ps f31,  992 (x4)\n"
		//RTLMIN-5479"fsin.ps f0, f0\n"
		"fexp.ps f0, f0\n"
		"frcp.ps f0, f0\n"
		//RTLMIN-5479"frsq.ps f0, f0\n"
		"flog.ps f0, f0\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_VPU_SEQ_START:\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f6, f25\n"                                 ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f17, f19\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f3, f4\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f6, f10\n"                                 ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f27, f21\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f25, f24\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f19, f28\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f28, f30\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f5, f24\n"                                 ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f16, f7\n"                                 ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f29, f16\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f14, f3\n"                                 ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f25, f10\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f5, f12\n"                                 ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f25, f29\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f15, f23\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f25, f2\n"                                 ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f27, f2\n"                                 ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f27, f15\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f13, f20\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f19, f5\n"                                 ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f10, f16\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f21, f24\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f19, f6\n"                                 ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f23, f1\n"                                 ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f1, f5\n"                                  ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f25, f17\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f9, f19\n"                                 ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f11, f3\n"                                 ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f12, f7\n"                                 ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f25, f6\n"                                 ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f4, f24\n"                                 ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f7, f16\n"                                 ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f28, f1\n"                                 ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f9, f0\n"                                  ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f24, f6\n"                                 ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f26, f30\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f2, f12\n"                                 ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f30, f11\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f21, f26\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f10, f19\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f29, f29\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f28, f0\n"                                 ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f12, f26\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f25, f18\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f21, f29\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f21, f28\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f25, f1\n"                                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f20, f30\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f5, f19\n"                                 ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f17, f30\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f11, f24\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f2, f9\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f7, f25\n"                                 ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f28, f23\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f3, f29\n"                                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f22, f10\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f14, f29\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f22, f2\n"                                 ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f10, f1\n"                                 ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f8, f27\n"                                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f25, f14\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f14, f26\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f14, f12\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f13, f30\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f10, f16\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f4, f30\n"                                 ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f5, f24\n"                                 ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f25, f4\n"                                 ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f10, f28\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f30, f6\n"                                 ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f17, f7\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f7, f11\n"                                 ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f3, f10\n"                                 ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f29, f12\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f1, f28\n"                                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f16, f1\n"                                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f22, f16\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f17, f8\n"                                 ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f6, f0\n"                                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f8, f9\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f20, f1\n"                                 ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f3, f14\n"                                 ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f26, f19\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f4, f29\n"                                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f23, f13\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f18, f19\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f2, f2\n"                                  ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f12, f9\n"                                 ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f28, f6\n"                                 ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x9, f12, f25\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f15, f12\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f22, f21\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x6, f25, f3\n"                                 ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x5, f3, f8\n"                                  ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f8, f7\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x7, f23, f1\n"                                 ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x4, f26, f8\n"                                 ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.s x3, f29, f20\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
