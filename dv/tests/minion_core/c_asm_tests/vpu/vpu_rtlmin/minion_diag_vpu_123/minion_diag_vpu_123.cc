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
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00004
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00018
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0001c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00020
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00024
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0002c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00034
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00038
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00040
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00050
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00058
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0005c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00060
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00064
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00068
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0006c
			 0xbf028f5c,                                                  // -0.51                                       /// 00070
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0007c
			 0x55555555,                                                  // 4 random values                             /// 00080
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00088
			 0x00011111,                                                  // 9.7958E-41                                  /// 0008c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00090
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00094
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00098
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0009c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000c8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000cc
			 0xffc00001,                                                  // -signaling NaN                              /// 000d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0xbf028f5c,                                                  // -0.51                                       /// 000e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f0
			 0x00000000,                                                  // zero                                        /// 000f4
			 0x3f028f5c,                                                  // 0.51                                        /// 000f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00100
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00104
			 0x33333333,                                                  // 4 random values                             /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00110
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00114
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00118
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00128
			 0x00000000,                                                  // zero                                        /// 0012c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00134
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00138
			 0x33333333,                                                  // 4 random values                             /// 0013c
			 0xffc00001,                                                  // -signaling NaN                              /// 00140
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00148
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0014c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00150
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00154
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00158
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0015c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00160
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00164
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00168
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0016c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00180
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00184
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00190
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00194
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00198
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0019c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0xcb000000,                                                  // -8388608.0                                  /// 001f0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00200
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00204
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0020c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00218
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00220
			 0x55555555,                                                  // 4 random values                             /// 00224
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00234
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00238
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0023c
			 0xff800000,                                                  // -inf                                        /// 00240
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0024c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00258
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00260
			 0x00000000,                                                  // zero                                        /// 00264
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00268
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00270
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00278
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0027c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00280
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00284
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00288
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00290
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00294
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0029c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0xbf028f5c,                                                  // -0.51                                       /// 002b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c8
			 0x55555555,                                                  // 4 random values                             /// 002cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00300
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e000007,                                                  // Trailing 1s:                                /// 00308
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0030c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00310
			 0x0c400000,                                                  // Leading 1s:                                 /// 00314
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00320
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00324
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00328
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00330
			 0x3f028f5c,                                                  // 0.51                                        /// 00334
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00338
			 0x33333333,                                                  // 4 random values                             /// 0033c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00344
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00348
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00350
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00354
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00358
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0035c
			 0x00000000,                                                  // zero                                        /// 00360
			 0x0e000007,                                                  // Trailing 1s:                                /// 00364
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00368
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00370
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00374
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0037c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00384
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0038c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00390
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00394
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c0
			 0x3f028f5c,                                                  // 0.51                                        /// 003c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00400
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00414
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00418
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0041c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00424
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00428
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0042c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00430
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00434
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00438
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00440
			 0x00000000,                                                  // zero                                        /// 00444
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00450
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00454
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00458
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00470
			 0x80011111,                                                  // -9.7958E-41                                 /// 00474
			 0xcb000000,                                                  // -8388608.0                                  /// 00478
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00484
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00488
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0048c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00494
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00498
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x3f028f5c,                                                  // 0.51                                        /// 004ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00500
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00504
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00508
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0050c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00510
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00518
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0051c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00520
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00524
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00530
			 0x0e000003,                                                  // Trailing 1s:                                /// 00534
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00538
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0053c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00544
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00548
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0054c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00554
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0055c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00564
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00568
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0056c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00570
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00574
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00578
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00580
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00588
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x00000000,                                                  // zero                                        /// 00594
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00598
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0059c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a8
			 0xcb000000,                                                  // -8388608.0                                  /// 005ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00604
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0060c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00610
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00618
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0061c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00624
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00634
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0063c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00640
			 0x0e000003,                                                  // Trailing 1s:                                /// 00644
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00654
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00658
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0065c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00660
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00664
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00668
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00674
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00678
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00684
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00688
			 0x0e000007,                                                  // Trailing 1s:                                /// 0068c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00690
			 0x0c600000,                                                  // Leading 1s:                                 /// 00694
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00698
			 0x3f028f5c,                                                  // 0.51                                        /// 0069c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a0
			 0x33333333,                                                  // 4 random values                             /// 006a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 006b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00700
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00708
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00710
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00714
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0071c
			 0x4b000000,                                                  // 8388608.0                                   /// 00720
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00724
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00728
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00730
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0074c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00750
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0075c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00764
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00768
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00770
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00774
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00778
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0077c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00790
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00798
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0079c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x7fc00001,                                                  // signaling NaN                               /// 00804
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0080c
			 0x7f800000,                                                  // inf                                         /// 00810
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00814
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00818
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00820
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00824
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00828
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00830
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0083c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00840
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00844
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0084c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00850
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00858
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00860
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x00011111,                                                  // 9.7958E-41                                  /// 0086c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00870
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00874
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0087c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00880
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00888
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0088c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00894
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00898
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x55555555,                                                  // 4 random values                             /// 008b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008c8
			 0x4b000000,                                                  // 8388608.0                                   /// 008cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008d8
			 0x3f028f5c,                                                  // 0.51                                        /// 008dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00900
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00904
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00914
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c700000,                                                  // Leading 1s:                                 /// 00924
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0092c
			 0x7fc00001,                                                  // signaling NaN                               /// 00930
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00934
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00938
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0093c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00940
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00944
			 0x7f800000,                                                  // inf                                         /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0094c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00954
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0095c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00960
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00968
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0096c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00970
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00978
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00980
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00984
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0e000007,                                                  // Trailing 1s:                                /// 0098c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00990
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00994
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00998
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0099c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009b0
			 0x7fc00001,                                                  // signaling NaN                               /// 009b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x4b000000,                                                  // 8388608.0                                   /// 009c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0xcb000000,                                                  // -8388608.0                                  /// 009d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0xff800000,                                                  // -inf                                        /// 00a0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a18
			 0x80000000,                                                  // -zero                                       /// 00a1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a38
			 0x33333333,                                                  // 4 random values                             /// 00a3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a58
			 0x00000000,                                                  // zero                                        /// 00a5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a68
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aa4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ad0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ad8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00adc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ae8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00af0
			 0xbf028f5c,                                                  // -0.51                                       /// 00af4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00af8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00afc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b00
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b24
			 0x80000000,                                                  // -zero                                       /// 00b28
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7f800000,                                                  // inf                                         /// 00b50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b88
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b94
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ba0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x7f800000,                                                  // inf                                         /// 00ba8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bd8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00be0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00be4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00be8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bf8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c00
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c18
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80000000,                                                  // -zero                                       /// 00c2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0xcb000000,                                                  // -8388608.0                                  /// 00c38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c40
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c50
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c84
			 0x7f800000,                                                  // inf                                         /// 00c88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ca4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ccc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce0
			 0x00000000,                                                  // zero                                        /// 00ce4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cf8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d08
			 0xcb000000,                                                  // -8388608.0                                  /// 00d0c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d14
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d5c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d70
			 0x7fc00001,                                                  // signaling NaN                               /// 00d74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d90
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00db0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db8
			 0x33333333,                                                  // 4 random values                             /// 00dbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00de0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00de4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de8
			 0x3f028f5c,                                                  // 0.51                                        /// 00dec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00df8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dfc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e18
			 0x33333333,                                                  // 4 random values                             /// 00e1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e34
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e50
			 0xff800000,                                                  // -inf                                        /// 00e54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e80
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ea8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eb4
			 0x00000000,                                                  // zero                                        /// 00eb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ec0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ecc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ed4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00edc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ee4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ee8
			 0x80000000,                                                  // -zero                                       /// 00eec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00efc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x33333333,                                                  // 4 random values                             /// 00f2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0xcb000000,                                                  // -8388608.0                                  /// 00f44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f58
			 0x00000000,                                                  // zero                                        /// 00f5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f88
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f94
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fa0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fa8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00fb4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fe8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff8
			 0x0e03ffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x349fb022, /// 0x0
			 0xd73698ea, /// 0x4
			 0xef3d804b, /// 0x8
			 0xab1713c5, /// 0xc
			 0xe62961c4, /// 0x10
			 0xa9962685, /// 0x14
			 0xf38748c9, /// 0x18
			 0x3cea8ff0, /// 0x1c
			 0xd1d42007, /// 0x20
			 0xf7dad3d3, /// 0x24
			 0x19286f98, /// 0x28
			 0x79721534, /// 0x2c
			 0xee0e23b9, /// 0x30
			 0x8f968d1c, /// 0x34
			 0xcb863475, /// 0x38
			 0x0b542147, /// 0x3c
			 0x359717e7, /// 0x40
			 0xa199655e, /// 0x44
			 0x24b1714d, /// 0x48
			 0x1d601b7d, /// 0x4c
			 0xf4a1c5e8, /// 0x50
			 0x8c062ee2, /// 0x54
			 0xa7ce3cc0, /// 0x58
			 0x90ac399a, /// 0x5c
			 0x3adc078d, /// 0x60
			 0xd2709095, /// 0x64
			 0xdbf99739, /// 0x68
			 0x36ac038c, /// 0x6c
			 0x20fb1c15, /// 0x70
			 0x5e246261, /// 0x74
			 0xa9d032ca, /// 0x78
			 0x9504812c, /// 0x7c
			 0x0db0e074, /// 0x80
			 0x7a9b0e9e, /// 0x84
			 0xcb1085e2, /// 0x88
			 0xeed833b8, /// 0x8c
			 0xf443aa1d, /// 0x90
			 0xe9916433, /// 0x94
			 0x0a08515c, /// 0x98
			 0x8666c2fc, /// 0x9c
			 0xfd7ed75b, /// 0xa0
			 0xfa1ee8c9, /// 0xa4
			 0xe92b85d4, /// 0xa8
			 0x9e74ec45, /// 0xac
			 0xcaaa68ce, /// 0xb0
			 0x39fda324, /// 0xb4
			 0xbbde58f4, /// 0xb8
			 0x2b8859a4, /// 0xbc
			 0x2d4997bd, /// 0xc0
			 0xf9635105, /// 0xc4
			 0x15299ae1, /// 0xc8
			 0x00fcabb4, /// 0xcc
			 0xe78c82fe, /// 0xd0
			 0x15fcb8da, /// 0xd4
			 0xf2b25b1b, /// 0xd8
			 0x9287ab36, /// 0xdc
			 0x3d45b840, /// 0xe0
			 0xc335f862, /// 0xe4
			 0x48bae9ab, /// 0xe8
			 0xf7a99cfd, /// 0xec
			 0x424d8884, /// 0xf0
			 0xaf1d3509, /// 0xf4
			 0x093e5438, /// 0xf8
			 0x0fb6bb54, /// 0xfc
			 0x661c43a7, /// 0x100
			 0x38594888, /// 0x104
			 0x35331f1a, /// 0x108
			 0xb0b5fd80, /// 0x10c
			 0xcf9d083b, /// 0x110
			 0x31aa65e6, /// 0x114
			 0x73117fe2, /// 0x118
			 0x6f362158, /// 0x11c
			 0xe318c92a, /// 0x120
			 0xc069fdc0, /// 0x124
			 0x7f72c263, /// 0x128
			 0x289adad1, /// 0x12c
			 0x187e6b2a, /// 0x130
			 0x4e335dec, /// 0x134
			 0x6e298f08, /// 0x138
			 0xd0a40d1c, /// 0x13c
			 0xdcd75aa3, /// 0x140
			 0xd8123403, /// 0x144
			 0x2fe5bda3, /// 0x148
			 0xbf40b5f8, /// 0x14c
			 0x68960ac4, /// 0x150
			 0xfd3a7e8d, /// 0x154
			 0xb796c37d, /// 0x158
			 0x35c6a0de, /// 0x15c
			 0x12673b06, /// 0x160
			 0x80c99d40, /// 0x164
			 0xcb6190b3, /// 0x168
			 0x79e915f0, /// 0x16c
			 0x6681c2b2, /// 0x170
			 0x685efd2b, /// 0x174
			 0xf5731990, /// 0x178
			 0xdaf50051, /// 0x17c
			 0x16513e21, /// 0x180
			 0xd8bd259f, /// 0x184
			 0x4f00f32e, /// 0x188
			 0x317b9bca, /// 0x18c
			 0x002b9b59, /// 0x190
			 0xf2ad5f1b, /// 0x194
			 0x50b4b84c, /// 0x198
			 0x190f2883, /// 0x19c
			 0xeabc3b27, /// 0x1a0
			 0x52dca813, /// 0x1a4
			 0xf6f4eee5, /// 0x1a8
			 0x5f09fcb9, /// 0x1ac
			 0x9853f0ea, /// 0x1b0
			 0xc03e199e, /// 0x1b4
			 0x2d4e684e, /// 0x1b8
			 0xf942895f, /// 0x1bc
			 0xc76939e8, /// 0x1c0
			 0xb4587236, /// 0x1c4
			 0xc59d9ca0, /// 0x1c8
			 0x2e2870d9, /// 0x1cc
			 0xe0475b5c, /// 0x1d0
			 0x81c2eec5, /// 0x1d4
			 0x62782aea, /// 0x1d8
			 0x4409c40f, /// 0x1dc
			 0xd558626f, /// 0x1e0
			 0xaba0f1df, /// 0x1e4
			 0x0c186df2, /// 0x1e8
			 0xfc20786f, /// 0x1ec
			 0x5dbdbe06, /// 0x1f0
			 0x41b9e41e, /// 0x1f4
			 0x06f1c96c, /// 0x1f8
			 0xcc45be4c, /// 0x1fc
			 0xfa09270a, /// 0x200
			 0x8e759b2e, /// 0x204
			 0x9a050832, /// 0x208
			 0xcf9ae279, /// 0x20c
			 0x95a8a569, /// 0x210
			 0x2b7a47c1, /// 0x214
			 0x294525eb, /// 0x218
			 0x036e2952, /// 0x21c
			 0x843beb4c, /// 0x220
			 0x88af5479, /// 0x224
			 0x382c66cf, /// 0x228
			 0x9166b5bf, /// 0x22c
			 0x04eb697b, /// 0x230
			 0xe902559d, /// 0x234
			 0x57aa3139, /// 0x238
			 0xa13d1398, /// 0x23c
			 0x155166fc, /// 0x240
			 0x2bee06c3, /// 0x244
			 0xbd3a4391, /// 0x248
			 0x13311574, /// 0x24c
			 0xcac89985, /// 0x250
			 0x3f82adce, /// 0x254
			 0xae80b23a, /// 0x258
			 0x3f78fe54, /// 0x25c
			 0x36aafe7f, /// 0x260
			 0x29ec13cd, /// 0x264
			 0x86d489d1, /// 0x268
			 0x2c1231b7, /// 0x26c
			 0x05bf8cd7, /// 0x270
			 0x8dfd935a, /// 0x274
			 0x2d9dcaef, /// 0x278
			 0xd93724c9, /// 0x27c
			 0x474a8809, /// 0x280
			 0x0c7ca8a4, /// 0x284
			 0xaa1e078f, /// 0x288
			 0xe71143d5, /// 0x28c
			 0x22d752e8, /// 0x290
			 0x827599f5, /// 0x294
			 0xcc4ea5a1, /// 0x298
			 0x319441fc, /// 0x29c
			 0xd1d1f8af, /// 0x2a0
			 0xd86e7cea, /// 0x2a4
			 0xbb559178, /// 0x2a8
			 0x5ab04b7c, /// 0x2ac
			 0x536f4702, /// 0x2b0
			 0xec8963e4, /// 0x2b4
			 0xa79a43b1, /// 0x2b8
			 0x9077644b, /// 0x2bc
			 0x813cf6be, /// 0x2c0
			 0xdf9303a1, /// 0x2c4
			 0x49c8cf10, /// 0x2c8
			 0xcc0e0572, /// 0x2cc
			 0x62e108fb, /// 0x2d0
			 0xf7dbb2d4, /// 0x2d4
			 0x977a5aa7, /// 0x2d8
			 0xd6cf0e02, /// 0x2dc
			 0x5cdbf683, /// 0x2e0
			 0x5df62c3f, /// 0x2e4
			 0x45ef846b, /// 0x2e8
			 0x320cf127, /// 0x2ec
			 0xde88c881, /// 0x2f0
			 0x16b49f7e, /// 0x2f4
			 0x69d22d9b, /// 0x2f8
			 0x2e8113d9, /// 0x2fc
			 0xb2390ca0, /// 0x300
			 0x71b71f77, /// 0x304
			 0x8edfa740, /// 0x308
			 0xfaa19826, /// 0x30c
			 0x89d3cd8e, /// 0x310
			 0xd42dd186, /// 0x314
			 0xa965a5aa, /// 0x318
			 0x74f447a5, /// 0x31c
			 0x13f3fa0c, /// 0x320
			 0xf4cba107, /// 0x324
			 0xc2407f0d, /// 0x328
			 0xa22d72e5, /// 0x32c
			 0xc80e9f88, /// 0x330
			 0x966e13a4, /// 0x334
			 0x816cb172, /// 0x338
			 0xce3fe932, /// 0x33c
			 0xa45aa3e3, /// 0x340
			 0x0e4dc6d1, /// 0x344
			 0x235d36e3, /// 0x348
			 0x62793121, /// 0x34c
			 0xdb898890, /// 0x350
			 0xe647b5f0, /// 0x354
			 0x2ac38d56, /// 0x358
			 0x3a978975, /// 0x35c
			 0x88541a7c, /// 0x360
			 0x3f031434, /// 0x364
			 0x6227606a, /// 0x368
			 0x6750740d, /// 0x36c
			 0x60ed69a4, /// 0x370
			 0xf94fb12e, /// 0x374
			 0x581f8b59, /// 0x378
			 0xa4631b8f, /// 0x37c
			 0x7ab8c7b0, /// 0x380
			 0x550c4069, /// 0x384
			 0x0e24938c, /// 0x388
			 0x1587308d, /// 0x38c
			 0x9da7629e, /// 0x390
			 0x3b1139a2, /// 0x394
			 0xe726d7ca, /// 0x398
			 0x43fcddab, /// 0x39c
			 0xbb5ff5a2, /// 0x3a0
			 0x0f18568a, /// 0x3a4
			 0xa59451cc, /// 0x3a8
			 0xad92b77d, /// 0x3ac
			 0x96f440bf, /// 0x3b0
			 0x6950d8e7, /// 0x3b4
			 0x9a53ede5, /// 0x3b8
			 0x7992abe4, /// 0x3bc
			 0x36f980a9, /// 0x3c0
			 0x1d7c237c, /// 0x3c4
			 0xc44e3435, /// 0x3c8
			 0xc2e2baae, /// 0x3cc
			 0x89ce6e18, /// 0x3d0
			 0x4091d6c1, /// 0x3d4
			 0xa5521e98, /// 0x3d8
			 0xe5a0afa7, /// 0x3dc
			 0x7af3bdab, /// 0x3e0
			 0x8ed9e765, /// 0x3e4
			 0x2de15e23, /// 0x3e8
			 0x41abfa00, /// 0x3ec
			 0xb4971e3e, /// 0x3f0
			 0x6166542d, /// 0x3f4
			 0x3471854d, /// 0x3f8
			 0x0c211b7e, /// 0x3fc
			 0xeb134bdb, /// 0x400
			 0x08ccb288, /// 0x404
			 0xe86f01b9, /// 0x408
			 0x74bab442, /// 0x40c
			 0xf5f090fc, /// 0x410
			 0x8b29fd83, /// 0x414
			 0xfd581d18, /// 0x418
			 0x5901776f, /// 0x41c
			 0xb341e334, /// 0x420
			 0x7e420769, /// 0x424
			 0xbbbf3d0a, /// 0x428
			 0x946ecebe, /// 0x42c
			 0xae0e5232, /// 0x430
			 0x5f428711, /// 0x434
			 0xdf82d505, /// 0x438
			 0xd44b6a37, /// 0x43c
			 0x5b509e82, /// 0x440
			 0x4f108570, /// 0x444
			 0xc7e16d55, /// 0x448
			 0x7d2a6210, /// 0x44c
			 0xc6f0efb9, /// 0x450
			 0x807a2ccd, /// 0x454
			 0x0297a7ca, /// 0x458
			 0xd2368214, /// 0x45c
			 0x0e5d3db1, /// 0x460
			 0xfdbe929e, /// 0x464
			 0x4cc55009, /// 0x468
			 0x96495e2b, /// 0x46c
			 0xa164cdad, /// 0x470
			 0x825e8925, /// 0x474
			 0x589ca241, /// 0x478
			 0x583d1d59, /// 0x47c
			 0x2672046f, /// 0x480
			 0x54e4e56a, /// 0x484
			 0x82c586ef, /// 0x488
			 0x50fc553c, /// 0x48c
			 0x6d23032f, /// 0x490
			 0xf6da5d67, /// 0x494
			 0xc44a2424, /// 0x498
			 0x1a19baf0, /// 0x49c
			 0xb9cececd, /// 0x4a0
			 0xf065fa5b, /// 0x4a4
			 0xa278f0a6, /// 0x4a8
			 0x4e5d670a, /// 0x4ac
			 0xf939b00d, /// 0x4b0
			 0x792ceefe, /// 0x4b4
			 0xcebfe167, /// 0x4b8
			 0x3963117a, /// 0x4bc
			 0xd73007cd, /// 0x4c0
			 0x6a1e38e3, /// 0x4c4
			 0xdd089686, /// 0x4c8
			 0x3989167a, /// 0x4cc
			 0x18758026, /// 0x4d0
			 0xe2e5f84a, /// 0x4d4
			 0x86573dac, /// 0x4d8
			 0xa5565d4a, /// 0x4dc
			 0xb4295dab, /// 0x4e0
			 0x98e65629, /// 0x4e4
			 0xab8f01a5, /// 0x4e8
			 0xf7cb438d, /// 0x4ec
			 0x222d70fd, /// 0x4f0
			 0x3135c317, /// 0x4f4
			 0x7313b78d, /// 0x4f8
			 0x9ab6dd34, /// 0x4fc
			 0x29ccf56c, /// 0x500
			 0x1b43f167, /// 0x504
			 0xb72bb994, /// 0x508
			 0x2e0121fd, /// 0x50c
			 0x748b670e, /// 0x510
			 0x051d6c26, /// 0x514
			 0xd38f3ab5, /// 0x518
			 0x21c77bf8, /// 0x51c
			 0x68f81ef4, /// 0x520
			 0x3e516317, /// 0x524
			 0x29e65d5a, /// 0x528
			 0xd3ebb369, /// 0x52c
			 0x8ac35c35, /// 0x530
			 0x5fe4f439, /// 0x534
			 0x1b74b263, /// 0x538
			 0xfbbdc406, /// 0x53c
			 0xefea1437, /// 0x540
			 0xc53c7e46, /// 0x544
			 0x338c1463, /// 0x548
			 0x9c8b1efd, /// 0x54c
			 0x69617278, /// 0x550
			 0x087cd36d, /// 0x554
			 0xa39a4d3b, /// 0x558
			 0xb792d902, /// 0x55c
			 0xa2c004bb, /// 0x560
			 0x9e3eeccd, /// 0x564
			 0xcceed208, /// 0x568
			 0x86d44f66, /// 0x56c
			 0xfd0a98e9, /// 0x570
			 0xfadd7367, /// 0x574
			 0x6ff47c3e, /// 0x578
			 0xb6f07b9b, /// 0x57c
			 0xb906ff37, /// 0x580
			 0xdd676bcd, /// 0x584
			 0xe1394783, /// 0x588
			 0x3d3516ba, /// 0x58c
			 0xb387892b, /// 0x590
			 0xbf823ca4, /// 0x594
			 0x2d2dc2ae, /// 0x598
			 0xf106c50c, /// 0x59c
			 0xad66c0ab, /// 0x5a0
			 0xd0fcdcc7, /// 0x5a4
			 0xc6d46c7c, /// 0x5a8
			 0x7959f66d, /// 0x5ac
			 0xca644212, /// 0x5b0
			 0xe6beb6fd, /// 0x5b4
			 0xb7d3c490, /// 0x5b8
			 0x83f2f7fb, /// 0x5bc
			 0x304fc846, /// 0x5c0
			 0x8b23ec8b, /// 0x5c4
			 0x0bbf27ae, /// 0x5c8
			 0xa7e4f690, /// 0x5cc
			 0xd267733a, /// 0x5d0
			 0xdb17a58e, /// 0x5d4
			 0xe476a612, /// 0x5d8
			 0x3bc83579, /// 0x5dc
			 0x2fff1af3, /// 0x5e0
			 0x60f46d59, /// 0x5e4
			 0xfaf33dbd, /// 0x5e8
			 0x156526c3, /// 0x5ec
			 0x2bcc06c9, /// 0x5f0
			 0xf9cf9617, /// 0x5f4
			 0x05853faf, /// 0x5f8
			 0xeb8c15b8, /// 0x5fc
			 0xa720fd15, /// 0x600
			 0xfd723325, /// 0x604
			 0x769e5c76, /// 0x608
			 0x7cbe103f, /// 0x60c
			 0x0413bdb9, /// 0x610
			 0xff3b2996, /// 0x614
			 0x492db900, /// 0x618
			 0x5904d4fd, /// 0x61c
			 0x82ba0661, /// 0x620
			 0xa019c718, /// 0x624
			 0x7357647a, /// 0x628
			 0xdd1178c3, /// 0x62c
			 0xaf3f361e, /// 0x630
			 0x238c8575, /// 0x634
			 0x2bb60186, /// 0x638
			 0x2b8b8e49, /// 0x63c
			 0x3ef922e4, /// 0x640
			 0x40984ce7, /// 0x644
			 0xbe4c5556, /// 0x648
			 0x67634db6, /// 0x64c
			 0x18363f78, /// 0x650
			 0x4747177f, /// 0x654
			 0x1cc13fe6, /// 0x658
			 0x08a72412, /// 0x65c
			 0xd3cc15d4, /// 0x660
			 0x5c1e8264, /// 0x664
			 0x046bbda7, /// 0x668
			 0x631fa2dd, /// 0x66c
			 0xb8bc2f55, /// 0x670
			 0x34f636b2, /// 0x674
			 0xc95db72f, /// 0x678
			 0x41d004a9, /// 0x67c
			 0x9e4ed0e6, /// 0x680
			 0x6699f80d, /// 0x684
			 0x50f8f629, /// 0x688
			 0x3174a29e, /// 0x68c
			 0x0f954440, /// 0x690
			 0xa726a108, /// 0x694
			 0x339da75a, /// 0x698
			 0xfafa6768, /// 0x69c
			 0x48a47ac1, /// 0x6a0
			 0x43e6db3a, /// 0x6a4
			 0x376ac70f, /// 0x6a8
			 0x8a08c403, /// 0x6ac
			 0x610ab419, /// 0x6b0
			 0xc0137f52, /// 0x6b4
			 0xae2c1055, /// 0x6b8
			 0x561f1efb, /// 0x6bc
			 0xadff02aa, /// 0x6c0
			 0xcd1fd280, /// 0x6c4
			 0x7a996fd6, /// 0x6c8
			 0x74a6b2dc, /// 0x6cc
			 0x37e9ebc1, /// 0x6d0
			 0xff34495f, /// 0x6d4
			 0x1ab35c2d, /// 0x6d8
			 0x6599b288, /// 0x6dc
			 0xbe926b66, /// 0x6e0
			 0x2c5bcd0d, /// 0x6e4
			 0x610f73c4, /// 0x6e8
			 0x5573ce21, /// 0x6ec
			 0x35a27507, /// 0x6f0
			 0xe6fdef5e, /// 0x6f4
			 0x8c1c08db, /// 0x6f8
			 0xef55afeb, /// 0x6fc
			 0x0b78d2a9, /// 0x700
			 0x1a751ac9, /// 0x704
			 0xa8f418fa, /// 0x708
			 0x61eb5ea0, /// 0x70c
			 0x404b23c9, /// 0x710
			 0xe3b89b1b, /// 0x714
			 0xab76f919, /// 0x718
			 0x54982ad2, /// 0x71c
			 0x94d1ed56, /// 0x720
			 0xf105081c, /// 0x724
			 0x990059c0, /// 0x728
			 0x6604cc1b, /// 0x72c
			 0x57688a24, /// 0x730
			 0xf8fa5765, /// 0x734
			 0x7fbfebf5, /// 0x738
			 0xc0ac7d87, /// 0x73c
			 0xa2f3fd51, /// 0x740
			 0xf78bfe87, /// 0x744
			 0xa60dd7af, /// 0x748
			 0xf5e8fd46, /// 0x74c
			 0xe1cbaa54, /// 0x750
			 0x8a365c63, /// 0x754
			 0xa2fff5a6, /// 0x758
			 0xa52245ad, /// 0x75c
			 0x0b642395, /// 0x760
			 0xe7c756c9, /// 0x764
			 0xcf000e2e, /// 0x768
			 0x1279514c, /// 0x76c
			 0x245cc5b8, /// 0x770
			 0x805eea3f, /// 0x774
			 0x9955dfb9, /// 0x778
			 0x940229af, /// 0x77c
			 0xce7b14b6, /// 0x780
			 0x7b75000d, /// 0x784
			 0x95027436, /// 0x788
			 0x289496a6, /// 0x78c
			 0xf0ba27e6, /// 0x790
			 0x54b92ff1, /// 0x794
			 0xeb3a3646, /// 0x798
			 0x5197f885, /// 0x79c
			 0xed374fb8, /// 0x7a0
			 0x572de3c7, /// 0x7a4
			 0xb36f6a48, /// 0x7a8
			 0x4eafc9b1, /// 0x7ac
			 0xb0c32eb4, /// 0x7b0
			 0x9b36ea21, /// 0x7b4
			 0xd0324c71, /// 0x7b8
			 0xd44926ad, /// 0x7bc
			 0x137427b1, /// 0x7c0
			 0x91c0a43d, /// 0x7c4
			 0x00414d31, /// 0x7c8
			 0x81e1e8f0, /// 0x7cc
			 0x9f5b20d0, /// 0x7d0
			 0xd18ea126, /// 0x7d4
			 0x210052e0, /// 0x7d8
			 0x02d9f844, /// 0x7dc
			 0x6dfcdeb4, /// 0x7e0
			 0xb08fe40d, /// 0x7e4
			 0x3a052375, /// 0x7e8
			 0x6e029052, /// 0x7ec
			 0x60b6e998, /// 0x7f0
			 0xa5f51a13, /// 0x7f4
			 0x3e68cf96, /// 0x7f8
			 0xd71dbcb6, /// 0x7fc
			 0x62c9fd37, /// 0x800
			 0xefe42eca, /// 0x804
			 0xeb90d60f, /// 0x808
			 0x0b616caa, /// 0x80c
			 0x8d8b63b9, /// 0x810
			 0xae1d72b0, /// 0x814
			 0xc0a7970d, /// 0x818
			 0x727a5686, /// 0x81c
			 0x3647a914, /// 0x820
			 0xcceaa7de, /// 0x824
			 0x0a0a634e, /// 0x828
			 0xeb0b0857, /// 0x82c
			 0xd87b4d92, /// 0x830
			 0x797ba952, /// 0x834
			 0xa6c26d03, /// 0x838
			 0x16341dda, /// 0x83c
			 0x56a4ca13, /// 0x840
			 0xaeb3d181, /// 0x844
			 0x0bc3a45b, /// 0x848
			 0x3270e53a, /// 0x84c
			 0x47a12627, /// 0x850
			 0x7ea35a62, /// 0x854
			 0xed05010d, /// 0x858
			 0x046427c1, /// 0x85c
			 0x7aebe60c, /// 0x860
			 0xf25e2018, /// 0x864
			 0x7470a70b, /// 0x868
			 0x6877b976, /// 0x86c
			 0x3a84c058, /// 0x870
			 0x4be4901c, /// 0x874
			 0x23625810, /// 0x878
			 0x89b077d9, /// 0x87c
			 0x1be37af7, /// 0x880
			 0x69c77f9b, /// 0x884
			 0x56a08f2b, /// 0x888
			 0xb2e7e6fd, /// 0x88c
			 0xc4a36cce, /// 0x890
			 0xb474fadf, /// 0x894
			 0x8d26c9fa, /// 0x898
			 0x2dff519a, /// 0x89c
			 0x208117c1, /// 0x8a0
			 0x9aef9240, /// 0x8a4
			 0xfd091171, /// 0x8a8
			 0x05943041, /// 0x8ac
			 0x8da8fe20, /// 0x8b0
			 0x0e42a474, /// 0x8b4
			 0x6182a9d5, /// 0x8b8
			 0x75df4aab, /// 0x8bc
			 0xa131dd83, /// 0x8c0
			 0x2d271ab0, /// 0x8c4
			 0x9e4fd5a3, /// 0x8c8
			 0xa89e12d6, /// 0x8cc
			 0xb0683af5, /// 0x8d0
			 0x7bdf1952, /// 0x8d4
			 0x96cf540a, /// 0x8d8
			 0xb1ef3e87, /// 0x8dc
			 0x3c0ff302, /// 0x8e0
			 0xb2a11174, /// 0x8e4
			 0xd1aeedb6, /// 0x8e8
			 0x13badfc7, /// 0x8ec
			 0x7c9a737c, /// 0x8f0
			 0x8392cbd0, /// 0x8f4
			 0x2e9cbac4, /// 0x8f8
			 0x0d82cb54, /// 0x8fc
			 0xafc35743, /// 0x900
			 0xf04443af, /// 0x904
			 0x73d97ece, /// 0x908
			 0x8a5d05c1, /// 0x90c
			 0x4c5064f2, /// 0x910
			 0x96de2d22, /// 0x914
			 0x85a04e6b, /// 0x918
			 0xdf03453e, /// 0x91c
			 0x9a26899c, /// 0x920
			 0x6e28a19d, /// 0x924
			 0x8ef08a42, /// 0x928
			 0x1afaca1c, /// 0x92c
			 0x732fea47, /// 0x930
			 0x78a21893, /// 0x934
			 0x249f2005, /// 0x938
			 0x51eae90c, /// 0x93c
			 0x4a0ebac9, /// 0x940
			 0xf48429dc, /// 0x944
			 0x39b23f7f, /// 0x948
			 0x17a0c405, /// 0x94c
			 0x8bc2c789, /// 0x950
			 0xefa4fe62, /// 0x954
			 0x5f21b403, /// 0x958
			 0x676afb46, /// 0x95c
			 0x97a6b273, /// 0x960
			 0x7094cc7d, /// 0x964
			 0xb1d8b3dd, /// 0x968
			 0x6b1fd7f9, /// 0x96c
			 0xc1c245ed, /// 0x970
			 0xb7deb04a, /// 0x974
			 0xdf7149a1, /// 0x978
			 0x14a8d0b4, /// 0x97c
			 0x65396aa2, /// 0x980
			 0x80e7609f, /// 0x984
			 0xbe9aa326, /// 0x988
			 0xb1399e62, /// 0x98c
			 0xfade4e7a, /// 0x990
			 0x66cbbc7f, /// 0x994
			 0x16a1c369, /// 0x998
			 0xa7361711, /// 0x99c
			 0x9fdf9d78, /// 0x9a0
			 0x2fb8edf7, /// 0x9a4
			 0x5b71a7fe, /// 0x9a8
			 0xc2343fab, /// 0x9ac
			 0x4c08c257, /// 0x9b0
			 0x403c6ceb, /// 0x9b4
			 0xf20ab9ca, /// 0x9b8
			 0xf3094eb6, /// 0x9bc
			 0x3cfe9199, /// 0x9c0
			 0xfdaa5f80, /// 0x9c4
			 0x4585a2c0, /// 0x9c8
			 0xa26a5b55, /// 0x9cc
			 0x278ff91c, /// 0x9d0
			 0x966d7ef8, /// 0x9d4
			 0xa1c73adf, /// 0x9d8
			 0xc9fbc74b, /// 0x9dc
			 0x8305e565, /// 0x9e0
			 0xe1973702, /// 0x9e4
			 0xde718bbb, /// 0x9e8
			 0xae74b126, /// 0x9ec
			 0xb57d6974, /// 0x9f0
			 0x78d744a8, /// 0x9f4
			 0x3eef13b5, /// 0x9f8
			 0x4ddf22b4, /// 0x9fc
			 0xb86a36f8, /// 0xa00
			 0x2a54bfee, /// 0xa04
			 0x41db62c5, /// 0xa08
			 0xf17496d4, /// 0xa0c
			 0x64214779, /// 0xa10
			 0xa2f076cb, /// 0xa14
			 0x1f034689, /// 0xa18
			 0x35bde99d, /// 0xa1c
			 0xfe3fbac6, /// 0xa20
			 0xee482049, /// 0xa24
			 0x08112526, /// 0xa28
			 0xd6c541ec, /// 0xa2c
			 0xaf85c1f6, /// 0xa30
			 0x46962415, /// 0xa34
			 0xa8e4d5a2, /// 0xa38
			 0x830fde96, /// 0xa3c
			 0x01e4fa57, /// 0xa40
			 0x36804368, /// 0xa44
			 0x694742d1, /// 0xa48
			 0xffa50fee, /// 0xa4c
			 0x23a48a0a, /// 0xa50
			 0xe6d9d470, /// 0xa54
			 0x2c915969, /// 0xa58
			 0xa718a037, /// 0xa5c
			 0x48b87312, /// 0xa60
			 0x6e61955f, /// 0xa64
			 0x7c3003a4, /// 0xa68
			 0x98e6f53c, /// 0xa6c
			 0x94e91bb9, /// 0xa70
			 0xe8870d37, /// 0xa74
			 0x5595b6ea, /// 0xa78
			 0x4b1dbcae, /// 0xa7c
			 0x0fc576f6, /// 0xa80
			 0x2b972b16, /// 0xa84
			 0x205377e6, /// 0xa88
			 0x344598ba, /// 0xa8c
			 0xec329b35, /// 0xa90
			 0xc44bf7a9, /// 0xa94
			 0x876efe3f, /// 0xa98
			 0x1ca4f2b2, /// 0xa9c
			 0x60715a0d, /// 0xaa0
			 0x49f3e42d, /// 0xaa4
			 0x0cfd97f9, /// 0xaa8
			 0xcea5fc00, /// 0xaac
			 0xa2c00e75, /// 0xab0
			 0xf536adaa, /// 0xab4
			 0xfa78c664, /// 0xab8
			 0xc9217d46, /// 0xabc
			 0xe1308552, /// 0xac0
			 0x9c392e28, /// 0xac4
			 0xe7a4ab70, /// 0xac8
			 0x020febc5, /// 0xacc
			 0x00cc0303, /// 0xad0
			 0x2d8ee164, /// 0xad4
			 0xeb064cfb, /// 0xad8
			 0xba7f067d, /// 0xadc
			 0x186ac96e, /// 0xae0
			 0x776fbf8a, /// 0xae4
			 0xbb682257, /// 0xae8
			 0xebcad976, /// 0xaec
			 0x3da8d42c, /// 0xaf0
			 0xa0be42eb, /// 0xaf4
			 0xafd0ef8b, /// 0xaf8
			 0x3d7aa72c, /// 0xafc
			 0xbafe037e, /// 0xb00
			 0x7e3c7c41, /// 0xb04
			 0x145a0afd, /// 0xb08
			 0x968f0f28, /// 0xb0c
			 0x23ec6921, /// 0xb10
			 0x2276b1c1, /// 0xb14
			 0x76c4a9a4, /// 0xb18
			 0xc63a3d19, /// 0xb1c
			 0x22885a04, /// 0xb20
			 0x814f5885, /// 0xb24
			 0x069001c2, /// 0xb28
			 0xf7a31533, /// 0xb2c
			 0x55972d56, /// 0xb30
			 0x1cadf3b2, /// 0xb34
			 0x1cf5cae1, /// 0xb38
			 0xe32066ec, /// 0xb3c
			 0x59625a36, /// 0xb40
			 0x5d3f0364, /// 0xb44
			 0xdd810b48, /// 0xb48
			 0x6f2b787e, /// 0xb4c
			 0x5989dd30, /// 0xb50
			 0x760ebfe1, /// 0xb54
			 0x63a46260, /// 0xb58
			 0x3f284985, /// 0xb5c
			 0x125fe7fb, /// 0xb60
			 0xb573fc25, /// 0xb64
			 0x600bba19, /// 0xb68
			 0x9698291e, /// 0xb6c
			 0x21e6efbd, /// 0xb70
			 0x8bfeffa8, /// 0xb74
			 0x03b906ec, /// 0xb78
			 0x9ab39e65, /// 0xb7c
			 0x3d67c363, /// 0xb80
			 0x519b16b4, /// 0xb84
			 0x601ca6b8, /// 0xb88
			 0xbb3925eb, /// 0xb8c
			 0x6b0ec5b1, /// 0xb90
			 0xee432b33, /// 0xb94
			 0x5a1e6ed1, /// 0xb98
			 0x4528db87, /// 0xb9c
			 0xfdc7c94d, /// 0xba0
			 0x7f073153, /// 0xba4
			 0x099f2369, /// 0xba8
			 0xe28de668, /// 0xbac
			 0x740ba627, /// 0xbb0
			 0xe3067b2f, /// 0xbb4
			 0xf717b738, /// 0xbb8
			 0x5fd3ea21, /// 0xbbc
			 0xfe4ef593, /// 0xbc0
			 0xd818f122, /// 0xbc4
			 0x757ee2dd, /// 0xbc8
			 0x0b99a485, /// 0xbcc
			 0x238c35d6, /// 0xbd0
			 0x33a1ee79, /// 0xbd4
			 0x1f64fff4, /// 0xbd8
			 0x3ea43855, /// 0xbdc
			 0x75fad9fd, /// 0xbe0
			 0x6458405b, /// 0xbe4
			 0x1ed69a0f, /// 0xbe8
			 0xed55e7ec, /// 0xbec
			 0x3363bfc4, /// 0xbf0
			 0x114c03ac, /// 0xbf4
			 0x0c865b24, /// 0xbf8
			 0x4203b0cd, /// 0xbfc
			 0xfc39806a, /// 0xc00
			 0xe8607ce8, /// 0xc04
			 0x5692d571, /// 0xc08
			 0xe5ebb060, /// 0xc0c
			 0x6bc3771e, /// 0xc10
			 0x6340d15d, /// 0xc14
			 0xc995ffc3, /// 0xc18
			 0x81a4f728, /// 0xc1c
			 0x63334b49, /// 0xc20
			 0x6eaf53da, /// 0xc24
			 0xdfee87dd, /// 0xc28
			 0x397f5e63, /// 0xc2c
			 0x4155d433, /// 0xc30
			 0x45d50a4d, /// 0xc34
			 0x7e3b9cc5, /// 0xc38
			 0x4bc153dc, /// 0xc3c
			 0x7639b901, /// 0xc40
			 0xa5430248, /// 0xc44
			 0x4f1b05db, /// 0xc48
			 0x287ed159, /// 0xc4c
			 0x8699976a, /// 0xc50
			 0xa89c3bc9, /// 0xc54
			 0x52f6e788, /// 0xc58
			 0xdda79c5f, /// 0xc5c
			 0xd1597d99, /// 0xc60
			 0xd58bd5f5, /// 0xc64
			 0x500d2e5f, /// 0xc68
			 0xba4b7043, /// 0xc6c
			 0x52325d96, /// 0xc70
			 0xac059479, /// 0xc74
			 0x694b5af3, /// 0xc78
			 0x0777eee1, /// 0xc7c
			 0x82292704, /// 0xc80
			 0x5f71eb92, /// 0xc84
			 0x73f39660, /// 0xc88
			 0xf5f933d9, /// 0xc8c
			 0x25ccfa9c, /// 0xc90
			 0xac3394b2, /// 0xc94
			 0x2de5672c, /// 0xc98
			 0xdcf1b9bd, /// 0xc9c
			 0x944a484c, /// 0xca0
			 0x01fefa7c, /// 0xca4
			 0x55160d1d, /// 0xca8
			 0x852e75cf, /// 0xcac
			 0x6e86ddef, /// 0xcb0
			 0x2dff7fbf, /// 0xcb4
			 0x189eef73, /// 0xcb8
			 0x4e901ef2, /// 0xcbc
			 0x1ddde139, /// 0xcc0
			 0x46fd927a, /// 0xcc4
			 0xa8278f09, /// 0xcc8
			 0xf2cc6cb5, /// 0xccc
			 0x4b362be4, /// 0xcd0
			 0x14ab2f2e, /// 0xcd4
			 0x345d2802, /// 0xcd8
			 0x19732812, /// 0xcdc
			 0xba11e35c, /// 0xce0
			 0x35f2fae4, /// 0xce4
			 0xbc5693ab, /// 0xce8
			 0x5c3874e8, /// 0xcec
			 0x0641585f, /// 0xcf0
			 0xf445444d, /// 0xcf4
			 0xe24d1a85, /// 0xcf8
			 0x4e1c388e, /// 0xcfc
			 0xe1b72b69, /// 0xd00
			 0x49e8b7de, /// 0xd04
			 0xb48b222e, /// 0xd08
			 0xa4f24b6a, /// 0xd0c
			 0x26c74a18, /// 0xd10
			 0x1d3a9d50, /// 0xd14
			 0x796aa938, /// 0xd18
			 0x89a41d4e, /// 0xd1c
			 0x55061850, /// 0xd20
			 0xe5dd7adc, /// 0xd24
			 0xcb56b1b5, /// 0xd28
			 0x1b1e65a6, /// 0xd2c
			 0x56ca9556, /// 0xd30
			 0xa298eef0, /// 0xd34
			 0x21283588, /// 0xd38
			 0x619a29cc, /// 0xd3c
			 0xf33c429e, /// 0xd40
			 0xcb651a7d, /// 0xd44
			 0x8c7851cd, /// 0xd48
			 0x998920d5, /// 0xd4c
			 0xc016ddcc, /// 0xd50
			 0x22938681, /// 0xd54
			 0x3afe2942, /// 0xd58
			 0x629619fa, /// 0xd5c
			 0x4a601a93, /// 0xd60
			 0x66ca7d28, /// 0xd64
			 0xa3cdceb5, /// 0xd68
			 0xd0d7b501, /// 0xd6c
			 0xcbef0ebe, /// 0xd70
			 0xf0662f88, /// 0xd74
			 0x3f4d2293, /// 0xd78
			 0x4e986f2c, /// 0xd7c
			 0x142f046f, /// 0xd80
			 0x4413dc6c, /// 0xd84
			 0x3cc53f8e, /// 0xd88
			 0x77e9b93f, /// 0xd8c
			 0xb1c8ab5d, /// 0xd90
			 0xfd86400f, /// 0xd94
			 0xbc4da28d, /// 0xd98
			 0x4886aa31, /// 0xd9c
			 0x366000cd, /// 0xda0
			 0x672f8737, /// 0xda4
			 0x34c4241b, /// 0xda8
			 0x9e713209, /// 0xdac
			 0x76811ed2, /// 0xdb0
			 0xf5d7c117, /// 0xdb4
			 0xb6bcf09b, /// 0xdb8
			 0xe3a11a2c, /// 0xdbc
			 0x403872c3, /// 0xdc0
			 0x1abdcfe3, /// 0xdc4
			 0x9c067c61, /// 0xdc8
			 0x5f4524ee, /// 0xdcc
			 0x806bf741, /// 0xdd0
			 0x7d992f46, /// 0xdd4
			 0x63cb23f1, /// 0xdd8
			 0x68c57596, /// 0xddc
			 0xbca7d9ee, /// 0xde0
			 0x1d172838, /// 0xde4
			 0x83fec9ea, /// 0xde8
			 0x98ff0e1c, /// 0xdec
			 0x9c0a4293, /// 0xdf0
			 0xef192fe2, /// 0xdf4
			 0xbd5b3dd5, /// 0xdf8
			 0x5dd63c16, /// 0xdfc
			 0x269a4051, /// 0xe00
			 0x5a87fe0d, /// 0xe04
			 0x12841c96, /// 0xe08
			 0x8596a5d5, /// 0xe0c
			 0x510fb7cf, /// 0xe10
			 0xd1505419, /// 0xe14
			 0x11f3a19d, /// 0xe18
			 0x58db8f7c, /// 0xe1c
			 0x95d3be5e, /// 0xe20
			 0x5df2bf79, /// 0xe24
			 0x98eb587a, /// 0xe28
			 0xa69e9df7, /// 0xe2c
			 0x84ec1f97, /// 0xe30
			 0xe03d6e39, /// 0xe34
			 0x3fbba0cd, /// 0xe38
			 0x8a80115c, /// 0xe3c
			 0x1b892cc7, /// 0xe40
			 0x52e0ce7c, /// 0xe44
			 0x7e567b9a, /// 0xe48
			 0x02272667, /// 0xe4c
			 0x37fb0574, /// 0xe50
			 0x0f4a7017, /// 0xe54
			 0xe41fc69f, /// 0xe58
			 0x99503414, /// 0xe5c
			 0x7bb912be, /// 0xe60
			 0x01f24bd5, /// 0xe64
			 0x4593613f, /// 0xe68
			 0x487ef680, /// 0xe6c
			 0x5f20c873, /// 0xe70
			 0x8a5d09bf, /// 0xe74
			 0xb173bab5, /// 0xe78
			 0x149b0e7f, /// 0xe7c
			 0xa9dce741, /// 0xe80
			 0xe966a1eb, /// 0xe84
			 0x7a1cecde, /// 0xe88
			 0xedd52dd0, /// 0xe8c
			 0x1f519adb, /// 0xe90
			 0x492486ce, /// 0xe94
			 0x91ab80db, /// 0xe98
			 0xdc041aac, /// 0xe9c
			 0x0bd4a82a, /// 0xea0
			 0x04d2c0ec, /// 0xea4
			 0x29ee1de4, /// 0xea8
			 0x809d8dc4, /// 0xeac
			 0xf547bf5b, /// 0xeb0
			 0x664cb25e, /// 0xeb4
			 0x74927a1a, /// 0xeb8
			 0xc23c1dfe, /// 0xebc
			 0xb60c7168, /// 0xec0
			 0x9db806b4, /// 0xec4
			 0xa673e9d7, /// 0xec8
			 0x469294bd, /// 0xecc
			 0xfe59fab7, /// 0xed0
			 0x537f126b, /// 0xed4
			 0x02611b8d, /// 0xed8
			 0x0fddc76c, /// 0xedc
			 0x56959166, /// 0xee0
			 0xecda7e12, /// 0xee4
			 0x4ad89e1d, /// 0xee8
			 0x31162b9e, /// 0xeec
			 0x41484538, /// 0xef0
			 0xf7a585fe, /// 0xef4
			 0xf12a5f7d, /// 0xef8
			 0x56c2e317, /// 0xefc
			 0x97eccaa5, /// 0xf00
			 0x74cc1d32, /// 0xf04
			 0x30784839, /// 0xf08
			 0xf3bfa85b, /// 0xf0c
			 0x0b00b637, /// 0xf10
			 0xe14a3e1e, /// 0xf14
			 0xa651ed0b, /// 0xf18
			 0xbe27ddf0, /// 0xf1c
			 0xdca89b91, /// 0xf20
			 0x4cd06e9c, /// 0xf24
			 0xeb433dc5, /// 0xf28
			 0x2265bf6a, /// 0xf2c
			 0x12f8bcd8, /// 0xf30
			 0x93f3088e, /// 0xf34
			 0x4c23b0b7, /// 0xf38
			 0x4d219ad6, /// 0xf3c
			 0x355a0ac0, /// 0xf40
			 0x0cd1f5b9, /// 0xf44
			 0x54060168, /// 0xf48
			 0xd8ef2dc7, /// 0xf4c
			 0x9aaabac6, /// 0xf50
			 0x365fa087, /// 0xf54
			 0x53146f5f, /// 0xf58
			 0x56af9ecf, /// 0xf5c
			 0xf2790ec9, /// 0xf60
			 0xf0175553, /// 0xf64
			 0x63cd2b58, /// 0xf68
			 0x8bc979ad, /// 0xf6c
			 0x1783d445, /// 0xf70
			 0xf26f22c3, /// 0xf74
			 0xa5cfe238, /// 0xf78
			 0x823cba82, /// 0xf7c
			 0xf485ee24, /// 0xf80
			 0x6611c49a, /// 0xf84
			 0x777e5474, /// 0xf88
			 0x25212666, /// 0xf8c
			 0xf8f6610e, /// 0xf90
			 0xe809440f, /// 0xf94
			 0x122233b1, /// 0xf98
			 0xe21ccf2d, /// 0xf9c
			 0xe11b25f6, /// 0xfa0
			 0xaf751f71, /// 0xfa4
			 0x71b7ea67, /// 0xfa8
			 0x00707dff, /// 0xfac
			 0xefdb262f, /// 0xfb0
			 0x1a8ac2d9, /// 0xfb4
			 0x682511d4, /// 0xfb8
			 0x521eb548, /// 0xfbc
			 0xa30646a9, /// 0xfc0
			 0x89deeefb, /// 0xfc4
			 0xea2e8176, /// 0xfc8
			 0x3a928795, /// 0xfcc
			 0x9122ef80, /// 0xfd0
			 0x7dcc78a7, /// 0xfd4
			 0xf844ab58, /// 0xfd8
			 0xa6515722, /// 0xfdc
			 0xecdc1c10, /// 0xfe0
			 0x41afc325, /// 0xfe4
			 0x7fb4aa51, /// 0xfe8
			 0x145eda50, /// 0xfec
			 0x2889c4de, /// 0xff0
			 0xe9a3dba1, /// 0xff4
			 0x5148e5e0, /// 0xff8
			 0x4bbe640b /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00000
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00004
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0000c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00014
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00018
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00024
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0002c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00030
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0003c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00040
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00044
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00048
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00050
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00054
			 0x0c400000,                                                  // Leading 1s:                                 /// 00058
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0005c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00060
			 0x7f800000,                                                  // inf                                         /// 00064
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00070
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00080
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00084
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00088
			 0x7fc00001,                                                  // signaling NaN                               /// 0008c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00090
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00094
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00098
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000ac
			 0x7f800000,                                                  // inf                                         /// 000b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c8
			 0xcb000000,                                                  // -8388608.0                                  /// 000cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000e0
			 0x4b000000,                                                  // 8388608.0                                   /// 000e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00100
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00108
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0010c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00110
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00114
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00118
			 0x7f800000,                                                  // inf                                         /// 0011c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00120
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00124
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00128
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00130
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00138
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0013c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00148
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00154
			 0xffc00001,                                                  // -signaling NaN                              /// 00158
			 0x80000000,                                                  // -zero                                       /// 0015c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00160
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00164
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00168
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00170
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0e000007,                                                  // Trailing 1s:                                /// 00178
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0017c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00184
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00198
			 0xffc00001,                                                  // -signaling NaN                              /// 0019c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00000000,                                                  // zero                                        /// 001fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00200
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00204
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00208
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00210
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00214
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00218
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00224
			 0x0e000007,                                                  // Trailing 1s:                                /// 00228
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00230
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00238
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0024c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00250
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00254
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00260
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00264
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00270
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00274
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00278
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x55555555,                                                  // 4 random values                             /// 00284
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00288
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00290
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00298
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002a0
			 0xcb000000,                                                  // -8388608.0                                  /// 002a4
			 0x00000000,                                                  // zero                                        /// 002a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x7fc00001,                                                  // signaling NaN                               /// 002b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000000,                                                  // zero                                        /// 002d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00300
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00304
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00310
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00314
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00318
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00320
			 0x3f028f5c,                                                  // 0.51                                        /// 00324
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00328
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0032c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00334
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00338
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0033c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00340
			 0x0e000003,                                                  // Trailing 1s:                                /// 00344
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0034c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00350
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x7f800000,                                                  // inf                                         /// 00358
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0035c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00360
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00364
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0036c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00374
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00378
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00380
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00384
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00388
			 0x7fc00001,                                                  // signaling NaN                               /// 0038c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00394
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0039c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003b8
			 0xbf028f5c,                                                  // -0.51                                       /// 003bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00404
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00418
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0041c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x55555555,                                                  // 4 random values                             /// 00428
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0042c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00430
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00434
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00438
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00440
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00444
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00458
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00460
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00468
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0046c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00474
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00478
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00480
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00484
			 0x7f800000,                                                  // inf                                         /// 00488
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00490
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00494
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00498
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0049c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00504
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00510
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00514
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00524
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x55555555,                                                  // 4 random values                             /// 0052c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00530
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x7f800000,                                                  // inf                                         /// 00538
			 0x0c600000,                                                  // Leading 1s:                                 /// 0053c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00540
			 0x4b000000,                                                  // 8388608.0                                   /// 00544
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00550
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00554
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00558
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0055c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00560
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00568
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00570
			 0x0c400000,                                                  // Leading 1s:                                 /// 00574
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00578
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00580
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00584
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00590
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c780000,                                                  // Leading 1s:                                 /// 00598
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0059c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e4
			 0xcb000000,                                                  // -8388608.0                                  /// 005e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00600
			 0x0e000003,                                                  // Trailing 1s:                                /// 00604
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0060c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00610
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x80000000,                                                  // -zero                                       /// 00618
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0061c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00620
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00624
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00628
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00630
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00634
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0063c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00640
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00644
			 0x4b000000,                                                  // 8388608.0                                   /// 00648
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0064c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00650
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x55555555,                                                  // 4 random values                             /// 00658
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0065c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00660
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00664
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0066c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00670
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00674
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00678
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0067c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00680
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00688
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00690
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00694
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0069c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006dc
			 0x33333333,                                                  // 4 random values                             /// 006e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006fc
			 0x7f800000,                                                  // inf                                         /// 00700
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00704
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0c700000,                                                  // Leading 1s:                                 /// 0070c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00710
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00714
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0071c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00720
			 0x0c700000,                                                  // Leading 1s:                                 /// 00724
			 0xbf028f5c,                                                  // -0.51                                       /// 00728
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0072c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00730
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00738
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0073c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00744
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00754
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x55555555,                                                  // 4 random values                             /// 0075c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00764
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00768
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0077c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x33333333,                                                  // 4 random values                             /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0078c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00790
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00794
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0079c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00800
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00804
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0xffc00001,                                                  // -signaling NaN                              /// 0080c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00810
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00818
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0xff800000,                                                  // -inf                                        /// 00820
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00824
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0082c
			 0x00000000,                                                  // zero                                        /// 00830
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00834
			 0x7fc00001,                                                  // signaling NaN                               /// 00838
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0083c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00840
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00844
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00848
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00850
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00854
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0085c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00860
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00864
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0086c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00870
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00878
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0087c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00884
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00888
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x7f800000,                                                  // inf                                         /// 00898
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0089c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00900
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00910
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00914
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00918
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0091c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00920
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00924
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00928
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0093c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00940
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00948
			 0x4b000000,                                                  // 8388608.0                                   /// 0094c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00950
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00954
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00958
			 0xff800000,                                                  // -inf                                        /// 0095c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00960
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00968
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0096c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00970
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00974
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0097c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00980
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00984
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00988
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0098c
			 0x4b000000,                                                  // 8388608.0                                   /// 00990
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00998
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000000,                                                  // -zero                                       /// 009ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xbf028f5c,                                                  // -0.51                                       /// 00a18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a38
			 0x55555555,                                                  // 4 random values                             /// 00a3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a44
			 0x7f800000,                                                  // inf                                         /// 00a48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a4c
			 0x7f800000,                                                  // inf                                         /// 00a50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000000,                                                  // zero                                        /// 00a88
			 0xffc00001,                                                  // -signaling NaN                              /// 00a8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a9c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00aa8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ac8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00acc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ad4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00adc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00afc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x7f800000,                                                  // inf                                         /// 00b0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x7fc00001,                                                  // signaling NaN                               /// 00b18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b50
			 0xbf028f5c,                                                  // -0.51                                       /// 00b54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b68
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b74
			 0x3f028f5c,                                                  // 0.51                                        /// 00b78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ba4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00be8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c14
			 0x4b000000,                                                  // 8388608.0                                   /// 00c18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c34
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c5c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80000000,                                                  // -zero                                       /// 00c84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c88
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c8c
			 0x80000000,                                                  // -zero                                       /// 00c90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80000000,                                                  // -zero                                       /// 00cb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cdc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ce4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cfc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d94
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00da8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00de0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00de4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00df0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x33333333,                                                  // 4 random values                             /// 00e2c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e48
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7f800000,                                                  // inf                                         /// 00e58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xbf028f5c,                                                  // -0.51                                       /// 00e7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ea0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ea8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ec4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ec8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00edc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ee4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00efc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f00
			 0x33333333,                                                  // 4 random values                             /// 00f04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x4b000000,                                                  // 8388608.0                                   /// 00f44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f50
			 0x3f028f5c,                                                  // 0.51                                        /// 00f54
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f80
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fdc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fe4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fe8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ff0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00000800                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x833871f0, /// 0x0
			 0x9e81f53c, /// 0x4
			 0x9bf01d4b, /// 0x8
			 0x12754d59, /// 0xc
			 0xb62356cd, /// 0x10
			 0xfd65f3d6, /// 0x14
			 0x36e455de, /// 0x18
			 0x6d851186, /// 0x1c
			 0x591d0f60, /// 0x20
			 0x3167ede6, /// 0x24
			 0xc95cb6b6, /// 0x28
			 0x58afa18d, /// 0x2c
			 0x54e25171, /// 0x30
			 0x120fa96d, /// 0x34
			 0xb3a67f74, /// 0x38
			 0x7a204bef, /// 0x3c
			 0xac6b9c9b, /// 0x40
			 0xa1979ce1, /// 0x44
			 0xe6f2c29c, /// 0x48
			 0x41ae750e, /// 0x4c
			 0x5e5914f5, /// 0x50
			 0x1ca3455b, /// 0x54
			 0xd23a11f2, /// 0x58
			 0xbaabe824, /// 0x5c
			 0x81e68195, /// 0x60
			 0x18dcda35, /// 0x64
			 0x975a0690, /// 0x68
			 0xde20141c, /// 0x6c
			 0x92921b62, /// 0x70
			 0xe15c0923, /// 0x74
			 0xa9a00f0c, /// 0x78
			 0x84b6a406, /// 0x7c
			 0x15a89b56, /// 0x80
			 0x0e9c017a, /// 0x84
			 0x8364e6fb, /// 0x88
			 0x2b03c15f, /// 0x8c
			 0x495c7a1e, /// 0x90
			 0x7812cd40, /// 0x94
			 0x222d702c, /// 0x98
			 0x829cd5ea, /// 0x9c
			 0xfd2e5ddd, /// 0xa0
			 0x5a1b8047, /// 0xa4
			 0xf2384dee, /// 0xa8
			 0xba6db331, /// 0xac
			 0xc3977d60, /// 0xb0
			 0xbb541664, /// 0xb4
			 0x59c2edf3, /// 0xb8
			 0x10ee7270, /// 0xbc
			 0xd2ef5620, /// 0xc0
			 0xae7e83dd, /// 0xc4
			 0x11180741, /// 0xc8
			 0xc2e24722, /// 0xcc
			 0x702704b4, /// 0xd0
			 0xc7ccf067, /// 0xd4
			 0x49dc5e2a, /// 0xd8
			 0x1051502c, /// 0xdc
			 0x1376025e, /// 0xe0
			 0xc08926b0, /// 0xe4
			 0xf7dd8898, /// 0xe8
			 0x06360322, /// 0xec
			 0x5e35229b, /// 0xf0
			 0x71d337dd, /// 0xf4
			 0x47fea940, /// 0xf8
			 0x350a5a5c, /// 0xfc
			 0x0b818b82, /// 0x100
			 0xb0c29966, /// 0x104
			 0x019324b5, /// 0x108
			 0x20f938fa, /// 0x10c
			 0x4e4b9a1a, /// 0x110
			 0xbccdbd20, /// 0x114
			 0xf5ab3366, /// 0x118
			 0x2f619c46, /// 0x11c
			 0xba0e2c81, /// 0x120
			 0x4b090fe4, /// 0x124
			 0xed9803a0, /// 0x128
			 0xb288bd49, /// 0x12c
			 0xc775261e, /// 0x130
			 0xb5f7d15a, /// 0x134
			 0xf229fd0e, /// 0x138
			 0xc033c11a, /// 0x13c
			 0x1b7a3f06, /// 0x140
			 0x7e5704e0, /// 0x144
			 0x70405814, /// 0x148
			 0xd6f448db, /// 0x14c
			 0x672f668f, /// 0x150
			 0xf1a85010, /// 0x154
			 0x5e222018, /// 0x158
			 0x00d1a0d3, /// 0x15c
			 0x7a6ff5d9, /// 0x160
			 0x92c6645b, /// 0x164
			 0xb0505fbc, /// 0x168
			 0xd6dfe906, /// 0x16c
			 0x5cb2122a, /// 0x170
			 0xebfca849, /// 0x174
			 0xe897da61, /// 0x178
			 0x87ba57f8, /// 0x17c
			 0x3d8bc999, /// 0x180
			 0x9bc1569f, /// 0x184
			 0x9845a857, /// 0x188
			 0x8f0bb11b, /// 0x18c
			 0xb530e2e8, /// 0x190
			 0x78889cfe, /// 0x194
			 0xbbca9dfa, /// 0x198
			 0xc97b6381, /// 0x19c
			 0x2986c8ee, /// 0x1a0
			 0xa8d092b2, /// 0x1a4
			 0x21549a79, /// 0x1a8
			 0x1f4f81be, /// 0x1ac
			 0x9c87b116, /// 0x1b0
			 0x4dba78cb, /// 0x1b4
			 0x05480ed9, /// 0x1b8
			 0xe9d5dabc, /// 0x1bc
			 0x9367957d, /// 0x1c0
			 0xaaf8175e, /// 0x1c4
			 0x474c6fa1, /// 0x1c8
			 0x01c47dcf, /// 0x1cc
			 0xd9636531, /// 0x1d0
			 0x5571fa62, /// 0x1d4
			 0x50338244, /// 0x1d8
			 0x528ef375, /// 0x1dc
			 0x13b4092d, /// 0x1e0
			 0x2fa0187f, /// 0x1e4
			 0x5271c568, /// 0x1e8
			 0xc8b54bf0, /// 0x1ec
			 0x658465a8, /// 0x1f0
			 0x303db1ab, /// 0x1f4
			 0x54b8ca77, /// 0x1f8
			 0x01478297, /// 0x1fc
			 0x367bb929, /// 0x200
			 0x7f44fe62, /// 0x204
			 0x2b423328, /// 0x208
			 0x331de950, /// 0x20c
			 0xb1ef7b09, /// 0x210
			 0xe769d30a, /// 0x214
			 0x724cdc5a, /// 0x218
			 0x5346d8d0, /// 0x21c
			 0x522e40d8, /// 0x220
			 0x40f2d116, /// 0x224
			 0xaace501f, /// 0x228
			 0x5a266441, /// 0x22c
			 0x94ed5f11, /// 0x230
			 0x8c4b6753, /// 0x234
			 0xf9a8beb4, /// 0x238
			 0xe217bd7b, /// 0x23c
			 0x7eb698e7, /// 0x240
			 0xff45f903, /// 0x244
			 0xca954cff, /// 0x248
			 0x2a0f9a80, /// 0x24c
			 0xe4f2be10, /// 0x250
			 0xbb2e5ca3, /// 0x254
			 0x53471285, /// 0x258
			 0x7aeb318f, /// 0x25c
			 0xdc3ef824, /// 0x260
			 0xad0abdca, /// 0x264
			 0xe24d0f09, /// 0x268
			 0x99b10f29, /// 0x26c
			 0xcf1e3f0a, /// 0x270
			 0xe3bb7f08, /// 0x274
			 0x4a2be398, /// 0x278
			 0x63efc4fe, /// 0x27c
			 0x184c975f, /// 0x280
			 0x82258c56, /// 0x284
			 0xb5bc9696, /// 0x288
			 0xf94ca1f0, /// 0x28c
			 0x79a83580, /// 0x290
			 0x675c6a79, /// 0x294
			 0x944a48aa, /// 0x298
			 0x6156ff18, /// 0x29c
			 0xb4e83e19, /// 0x2a0
			 0x6216831e, /// 0x2a4
			 0xb7e707cc, /// 0x2a8
			 0xbbfbdfa9, /// 0x2ac
			 0x126a1b9a, /// 0x2b0
			 0xd9d7fbf3, /// 0x2b4
			 0x2ffefd37, /// 0x2b8
			 0x4ec4ae74, /// 0x2bc
			 0x90e11456, /// 0x2c0
			 0x215215b3, /// 0x2c4
			 0x49b1e576, /// 0x2c8
			 0x26dfce27, /// 0x2cc
			 0x5d02a753, /// 0x2d0
			 0x785dd0fa, /// 0x2d4
			 0xd39babc8, /// 0x2d8
			 0xac882c9e, /// 0x2dc
			 0xbb8f7073, /// 0x2e0
			 0xb581032e, /// 0x2e4
			 0x518bb8ff, /// 0x2e8
			 0x2d359b1a, /// 0x2ec
			 0x4745a117, /// 0x2f0
			 0xc1b83389, /// 0x2f4
			 0xdc2f48b0, /// 0x2f8
			 0x71c54518, /// 0x2fc
			 0xd0a71693, /// 0x300
			 0x824c63e1, /// 0x304
			 0xb464480a, /// 0x308
			 0xc164b021, /// 0x30c
			 0xa1696ebc, /// 0x310
			 0xa044b407, /// 0x314
			 0x578fd139, /// 0x318
			 0xe3815891, /// 0x31c
			 0x598b5d65, /// 0x320
			 0xef01ef49, /// 0x324
			 0x22bfe60b, /// 0x328
			 0x4392fcc2, /// 0x32c
			 0xda2490a4, /// 0x330
			 0xb1de2679, /// 0x334
			 0x2aed5514, /// 0x338
			 0xad1c6d5c, /// 0x33c
			 0x243104c5, /// 0x340
			 0x0ad4f2a4, /// 0x344
			 0x9b454820, /// 0x348
			 0x6a6b9af5, /// 0x34c
			 0xe4863b48, /// 0x350
			 0x0a82d102, /// 0x354
			 0x97a88cd4, /// 0x358
			 0x0c66eee4, /// 0x35c
			 0x1e61a712, /// 0x360
			 0xf4d0374b, /// 0x364
			 0x06cc5ad3, /// 0x368
			 0x6dd58822, /// 0x36c
			 0xb00320aa, /// 0x370
			 0xa841f0e1, /// 0x374
			 0xbe4d4dc8, /// 0x378
			 0x38b82b2b, /// 0x37c
			 0x65b75b1e, /// 0x380
			 0x08acdcea, /// 0x384
			 0xf7e3fc57, /// 0x388
			 0x9bc83d03, /// 0x38c
			 0x9b0e571f, /// 0x390
			 0x3e17b99c, /// 0x394
			 0x24232bb0, /// 0x398
			 0x560947e5, /// 0x39c
			 0xc9375bb7, /// 0x3a0
			 0xb7225ed1, /// 0x3a4
			 0x3550f965, /// 0x3a8
			 0xbe9d1ab4, /// 0x3ac
			 0x46bfd9e6, /// 0x3b0
			 0xc944cbc5, /// 0x3b4
			 0x3c8bfb7f, /// 0x3b8
			 0xd02c3159, /// 0x3bc
			 0x3abdbd50, /// 0x3c0
			 0x40b8ec9f, /// 0x3c4
			 0x86e3fe41, /// 0x3c8
			 0x7bc7af20, /// 0x3cc
			 0x25f576b7, /// 0x3d0
			 0xa31ef468, /// 0x3d4
			 0x0144cca9, /// 0x3d8
			 0xc6a935fb, /// 0x3dc
			 0xbab47983, /// 0x3e0
			 0x17b85e6d, /// 0x3e4
			 0x95018e28, /// 0x3e8
			 0x6bdaf250, /// 0x3ec
			 0x1b7aa6ac, /// 0x3f0
			 0x193a3fce, /// 0x3f4
			 0x8d03573d, /// 0x3f8
			 0x58770ccc, /// 0x3fc
			 0x72da361b, /// 0x400
			 0x7f0aafd3, /// 0x404
			 0xa6b37647, /// 0x408
			 0x5ee2cbbf, /// 0x40c
			 0x8dcde0f0, /// 0x410
			 0x6d339b88, /// 0x414
			 0x190b8b04, /// 0x418
			 0x15100bfc, /// 0x41c
			 0x9d5ddca5, /// 0x420
			 0xb3013627, /// 0x424
			 0xe4a85e5f, /// 0x428
			 0xecf6a0ea, /// 0x42c
			 0x911a752b, /// 0x430
			 0x0f2c0704, /// 0x434
			 0x95709ba7, /// 0x438
			 0xd9ea29cc, /// 0x43c
			 0xcfa7ba96, /// 0x440
			 0xc6fe5150, /// 0x444
			 0xc0fd0124, /// 0x448
			 0x2c792c95, /// 0x44c
			 0x490c83be, /// 0x450
			 0x7708a6e6, /// 0x454
			 0x26fe65a5, /// 0x458
			 0x4975fc76, /// 0x45c
			 0x956fab47, /// 0x460
			 0xd8321871, /// 0x464
			 0x5ef95cd8, /// 0x468
			 0x321e4f1f, /// 0x46c
			 0x5e2cd51f, /// 0x470
			 0x9ebad989, /// 0x474
			 0x1dcace80, /// 0x478
			 0x9e0fd291, /// 0x47c
			 0x63bf2081, /// 0x480
			 0x09ef893f, /// 0x484
			 0x37789e3a, /// 0x488
			 0xb00f78ad, /// 0x48c
			 0x7be27c5e, /// 0x490
			 0x5d6ec9de, /// 0x494
			 0xb836f522, /// 0x498
			 0x4c751636, /// 0x49c
			 0xc042ca1d, /// 0x4a0
			 0xca593845, /// 0x4a4
			 0x5847d69e, /// 0x4a8
			 0x401177d7, /// 0x4ac
			 0x27b6c1ca, /// 0x4b0
			 0x51f8dd09, /// 0x4b4
			 0xf12f0286, /// 0x4b8
			 0xb4bf8239, /// 0x4bc
			 0x6625e6c8, /// 0x4c0
			 0xad640f8c, /// 0x4c4
			 0x769850e5, /// 0x4c8
			 0xeefa7835, /// 0x4cc
			 0x7b998c0a, /// 0x4d0
			 0x6340feb7, /// 0x4d4
			 0xdcc0f5a0, /// 0x4d8
			 0xd8177624, /// 0x4dc
			 0x2f62dee4, /// 0x4e0
			 0xe61ca612, /// 0x4e4
			 0x8d98a174, /// 0x4e8
			 0xf4f9ba1b, /// 0x4ec
			 0x08c0119d, /// 0x4f0
			 0x855fec4b, /// 0x4f4
			 0x3a4ad54c, /// 0x4f8
			 0x310cf8d7, /// 0x4fc
			 0x4c7c340c, /// 0x500
			 0x4122d621, /// 0x504
			 0xbc769252, /// 0x508
			 0x91a1c3f2, /// 0x50c
			 0x011007d5, /// 0x510
			 0xdf840ead, /// 0x514
			 0x44535797, /// 0x518
			 0xa334243c, /// 0x51c
			 0xa78cfdd5, /// 0x520
			 0x31cea1a0, /// 0x524
			 0x882976b0, /// 0x528
			 0x3be90d4e, /// 0x52c
			 0xd22e27c7, /// 0x530
			 0xde941394, /// 0x534
			 0x0bc388d4, /// 0x538
			 0xcf719f3a, /// 0x53c
			 0x3d234388, /// 0x540
			 0xaabc0b7b, /// 0x544
			 0x58b17463, /// 0x548
			 0x512113c0, /// 0x54c
			 0xee6fda84, /// 0x550
			 0x01b12332, /// 0x554
			 0x8fac2e10, /// 0x558
			 0xbdf236ea, /// 0x55c
			 0x0e890ffa, /// 0x560
			 0x05be4618, /// 0x564
			 0x044bb2d2, /// 0x568
			 0x6ea4552b, /// 0x56c
			 0xa8194103, /// 0x570
			 0x26881f3a, /// 0x574
			 0xf5841729, /// 0x578
			 0x6050e4e6, /// 0x57c
			 0x9570dd41, /// 0x580
			 0x55e984fc, /// 0x584
			 0x3d3c8313, /// 0x588
			 0x9978763f, /// 0x58c
			 0x3c663135, /// 0x590
			 0x431496ea, /// 0x594
			 0xd385f1c7, /// 0x598
			 0x9b71f251, /// 0x59c
			 0xd87748bb, /// 0x5a0
			 0x5e1a3e38, /// 0x5a4
			 0x5076fc4d, /// 0x5a8
			 0x10a97fe3, /// 0x5ac
			 0xef7a4096, /// 0x5b0
			 0x07193d1f, /// 0x5b4
			 0xeb2fa12e, /// 0x5b8
			 0xc407f37b, /// 0x5bc
			 0x2f9f6722, /// 0x5c0
			 0xd248be7a, /// 0x5c4
			 0x6f7002cb, /// 0x5c8
			 0x0b2b22d3, /// 0x5cc
			 0xef55e7c5, /// 0x5d0
			 0xc990f55e, /// 0x5d4
			 0x17a72a88, /// 0x5d8
			 0x3966d188, /// 0x5dc
			 0xcea62c91, /// 0x5e0
			 0x382e50dd, /// 0x5e4
			 0x46771649, /// 0x5e8
			 0x36641fd7, /// 0x5ec
			 0xa73a07e1, /// 0x5f0
			 0x35cc982d, /// 0x5f4
			 0x9177d90a, /// 0x5f8
			 0x59c1294b, /// 0x5fc
			 0x2e3e8171, /// 0x600
			 0x8de2a7bd, /// 0x604
			 0xcbda1d0b, /// 0x608
			 0xd38e6c2b, /// 0x60c
			 0xa07daa3a, /// 0x610
			 0xd95b7fb3, /// 0x614
			 0x6713499b, /// 0x618
			 0x4a5af084, /// 0x61c
			 0x505bf5ac, /// 0x620
			 0x6913aedc, /// 0x624
			 0xaddeed63, /// 0x628
			 0xc7636b09, /// 0x62c
			 0x94880332, /// 0x630
			 0x4e72fbee, /// 0x634
			 0xd81bfd51, /// 0x638
			 0x3e63c370, /// 0x63c
			 0xa10eceec, /// 0x640
			 0x3cb241f6, /// 0x644
			 0xaecc20fb, /// 0x648
			 0x2a3fce07, /// 0x64c
			 0xd8ee7ef5, /// 0x650
			 0xad81782f, /// 0x654
			 0xf1d87df9, /// 0x658
			 0x0f3f6774, /// 0x65c
			 0xab4f365a, /// 0x660
			 0x9ae33199, /// 0x664
			 0x29760df0, /// 0x668
			 0x62b02358, /// 0x66c
			 0x0853af40, /// 0x670
			 0xab3dd677, /// 0x674
			 0xd3d82f30, /// 0x678
			 0x656d0e2c, /// 0x67c
			 0x24eff8e8, /// 0x680
			 0x2284eeb0, /// 0x684
			 0x71f3f507, /// 0x688
			 0xd0108da4, /// 0x68c
			 0xf6d3c160, /// 0x690
			 0xefeadffb, /// 0x694
			 0x1671bb5d, /// 0x698
			 0xb5dff428, /// 0x69c
			 0xfd4f8551, /// 0x6a0
			 0xeb9f4795, /// 0x6a4
			 0xa852d88a, /// 0x6a8
			 0xd3831155, /// 0x6ac
			 0xbe9768e5, /// 0x6b0
			 0x8bf3b7cb, /// 0x6b4
			 0x232cfc94, /// 0x6b8
			 0x715d6fda, /// 0x6bc
			 0xb8d9ce9b, /// 0x6c0
			 0xf9faf79e, /// 0x6c4
			 0x105b3d44, /// 0x6c8
			 0xec42153d, /// 0x6cc
			 0x09acbb47, /// 0x6d0
			 0x48804ff3, /// 0x6d4
			 0x03f427c3, /// 0x6d8
			 0xe2322966, /// 0x6dc
			 0x7533d283, /// 0x6e0
			 0x107eeca4, /// 0x6e4
			 0x3b8c948d, /// 0x6e8
			 0x2d7dac0f, /// 0x6ec
			 0x4a09ba66, /// 0x6f0
			 0xc132f3ac, /// 0x6f4
			 0xc3a2ea39, /// 0x6f8
			 0xcc98e25d, /// 0x6fc
			 0xf3d348c2, /// 0x700
			 0xf553202d, /// 0x704
			 0x42cc6025, /// 0x708
			 0xc6f5f9df, /// 0x70c
			 0xd06ac563, /// 0x710
			 0x191ccc67, /// 0x714
			 0xc382e0ad, /// 0x718
			 0xccb10f8b, /// 0x71c
			 0xeadb660e, /// 0x720
			 0x7ca17770, /// 0x724
			 0x78fc68b5, /// 0x728
			 0x307cbde4, /// 0x72c
			 0x945a037c, /// 0x730
			 0xef192629, /// 0x734
			 0xb477ed0a, /// 0x738
			 0xe9c5a570, /// 0x73c
			 0xf2524e7d, /// 0x740
			 0xfb706400, /// 0x744
			 0x5cc103b4, /// 0x748
			 0xde179e2c, /// 0x74c
			 0xbccd7f6f, /// 0x750
			 0x2bd095fa, /// 0x754
			 0x2863ddb1, /// 0x758
			 0x10b2971a, /// 0x75c
			 0x88fb02dd, /// 0x760
			 0xda52007a, /// 0x764
			 0xca562b0d, /// 0x768
			 0x8aa7fa97, /// 0x76c
			 0xc0c77502, /// 0x770
			 0xef0da960, /// 0x774
			 0x0a2724bc, /// 0x778
			 0x11c983f3, /// 0x77c
			 0xb968425c, /// 0x780
			 0xa906f393, /// 0x784
			 0xda1c6871, /// 0x788
			 0x1c985104, /// 0x78c
			 0x1148a233, /// 0x790
			 0xdd2bac31, /// 0x794
			 0xad6f8bdd, /// 0x798
			 0xc10093d4, /// 0x79c
			 0x157376ba, /// 0x7a0
			 0x9340dea8, /// 0x7a4
			 0xcb249087, /// 0x7a8
			 0x6e4630a6, /// 0x7ac
			 0xce242028, /// 0x7b0
			 0xdf6dd958, /// 0x7b4
			 0xa04d311e, /// 0x7b8
			 0x3e5a0884, /// 0x7bc
			 0x590bbc48, /// 0x7c0
			 0x7a518452, /// 0x7c4
			 0x76037a84, /// 0x7c8
			 0x33494cd1, /// 0x7cc
			 0x407144a8, /// 0x7d0
			 0x69dfbe8a, /// 0x7d4
			 0x2c65d8a7, /// 0x7d8
			 0x36046ed3, /// 0x7dc
			 0x5bd337a0, /// 0x7e0
			 0x1eec5eb3, /// 0x7e4
			 0x203969c3, /// 0x7e8
			 0xc6c3480f, /// 0x7ec
			 0x55b37849, /// 0x7f0
			 0x27341540, /// 0x7f4
			 0x48c08640, /// 0x7f8
			 0x85f4ef35, /// 0x7fc
			 0x50e369ab, /// 0x800
			 0xfd7a8889, /// 0x804
			 0x5ab2f93b, /// 0x808
			 0x203efc54, /// 0x80c
			 0x54c1f7ce, /// 0x810
			 0x06b0c267, /// 0x814
			 0xed5b23a2, /// 0x818
			 0x1ed7eb06, /// 0x81c
			 0x87ee0a2e, /// 0x820
			 0x80441417, /// 0x824
			 0xe9140731, /// 0x828
			 0xbbe874bb, /// 0x82c
			 0x90acd0b7, /// 0x830
			 0x9cc3fd3b, /// 0x834
			 0xf0bdb693, /// 0x838
			 0x88c017cb, /// 0x83c
			 0x6306d3b2, /// 0x840
			 0x52a23e83, /// 0x844
			 0x8563f66d, /// 0x848
			 0x1ee60649, /// 0x84c
			 0xe4f7ec82, /// 0x850
			 0x6fc88dbb, /// 0x854
			 0xdb89b436, /// 0x858
			 0x573d9b25, /// 0x85c
			 0x882be1b1, /// 0x860
			 0xae002e11, /// 0x864
			 0x66b42ca2, /// 0x868
			 0xfe8b9ef2, /// 0x86c
			 0xbfeada35, /// 0x870
			 0x6427030a, /// 0x874
			 0x223dbe83, /// 0x878
			 0xab63d801, /// 0x87c
			 0xd67ca0a5, /// 0x880
			 0x486f62cf, /// 0x884
			 0xf42a2c68, /// 0x888
			 0x851c9a7a, /// 0x88c
			 0xeaeacfa1, /// 0x890
			 0x0366e7cb, /// 0x894
			 0xa7ddbbea, /// 0x898
			 0x97a8a48d, /// 0x89c
			 0xfc5424be, /// 0x8a0
			 0x1f1ea73b, /// 0x8a4
			 0x2bbecdd1, /// 0x8a8
			 0x2d4deea3, /// 0x8ac
			 0x0b6c7810, /// 0x8b0
			 0xcd2bb2cd, /// 0x8b4
			 0xb45f22e2, /// 0x8b8
			 0x4be9b47a, /// 0x8bc
			 0x3c2915ac, /// 0x8c0
			 0x7e86d140, /// 0x8c4
			 0x9a93a4f0, /// 0x8c8
			 0x93721fd9, /// 0x8cc
			 0x5bab3a11, /// 0x8d0
			 0xd6ddebce, /// 0x8d4
			 0x67231691, /// 0x8d8
			 0x86c0ef53, /// 0x8dc
			 0x0b82e314, /// 0x8e0
			 0xc7772754, /// 0x8e4
			 0xeb8ca0e4, /// 0x8e8
			 0xd4148802, /// 0x8ec
			 0xc943e809, /// 0x8f0
			 0x535eb347, /// 0x8f4
			 0x5c674907, /// 0x8f8
			 0xb0ab4ae8, /// 0x8fc
			 0x3492c12f, /// 0x900
			 0x6b70672d, /// 0x904
			 0xf458b53e, /// 0x908
			 0x56df0148, /// 0x90c
			 0xce8232de, /// 0x910
			 0x54a97e56, /// 0x914
			 0x5ede49d7, /// 0x918
			 0x4f53a9a5, /// 0x91c
			 0xe2196225, /// 0x920
			 0x1824767d, /// 0x924
			 0x3915dab3, /// 0x928
			 0x7dda738b, /// 0x92c
			 0x988b309f, /// 0x930
			 0xb2332a28, /// 0x934
			 0xdf5bc214, /// 0x938
			 0x707b7c1d, /// 0x93c
			 0xdda7bc08, /// 0x940
			 0xcfc34943, /// 0x944
			 0x66fdbf6d, /// 0x948
			 0xc3caf27a, /// 0x94c
			 0x9f09e9ca, /// 0x950
			 0x510fa00e, /// 0x954
			 0xbfca652b, /// 0x958
			 0xbac57ab6, /// 0x95c
			 0xea74fd12, /// 0x960
			 0x5fc58160, /// 0x964
			 0x6f38e0b2, /// 0x968
			 0xd3a45360, /// 0x96c
			 0x0844dde2, /// 0x970
			 0x4e5e4899, /// 0x974
			 0x400e5343, /// 0x978
			 0x8e8c3ac6, /// 0x97c
			 0xcd5c841f, /// 0x980
			 0x3c85134d, /// 0x984
			 0x59329063, /// 0x988
			 0x8a1e5e68, /// 0x98c
			 0xee95f315, /// 0x990
			 0x4d694100, /// 0x994
			 0x6cc31c80, /// 0x998
			 0x6f8a6c73, /// 0x99c
			 0x1ed2ffbd, /// 0x9a0
			 0x7d9f9c05, /// 0x9a4
			 0xa5ed807a, /// 0x9a8
			 0xa4a6d301, /// 0x9ac
			 0x03277251, /// 0x9b0
			 0xacf0714a, /// 0x9b4
			 0xa96ad003, /// 0x9b8
			 0x09a43b9e, /// 0x9bc
			 0x51bd6ded, /// 0x9c0
			 0xac00f418, /// 0x9c4
			 0xc0f4ef77, /// 0x9c8
			 0xcc1df473, /// 0x9cc
			 0x27c7646f, /// 0x9d0
			 0xab08511c, /// 0x9d4
			 0x29fbf906, /// 0x9d8
			 0xc74b2c10, /// 0x9dc
			 0x45355fb0, /// 0x9e0
			 0x61b1a1c5, /// 0x9e4
			 0xa4624201, /// 0x9e8
			 0x69572cfc, /// 0x9ec
			 0xaff81833, /// 0x9f0
			 0x8acafd9e, /// 0x9f4
			 0x88bbf3f9, /// 0x9f8
			 0x32338ee9, /// 0x9fc
			 0xf817be69, /// 0xa00
			 0xf571b8c7, /// 0xa04
			 0x2336547b, /// 0xa08
			 0xd54f762a, /// 0xa0c
			 0xa308c986, /// 0xa10
			 0x73d9bb91, /// 0xa14
			 0xf8723bdf, /// 0xa18
			 0x1a5e00c0, /// 0xa1c
			 0x2d465da8, /// 0xa20
			 0x31d3d10e, /// 0xa24
			 0x28abef3b, /// 0xa28
			 0x22c603d4, /// 0xa2c
			 0xcc9ac0df, /// 0xa30
			 0xd468b8f2, /// 0xa34
			 0xca1433d5, /// 0xa38
			 0xdc064d79, /// 0xa3c
			 0x573bded6, /// 0xa40
			 0x22278cb3, /// 0xa44
			 0xf8b76ef0, /// 0xa48
			 0x8a17f19b, /// 0xa4c
			 0x6f9706ac, /// 0xa50
			 0x56a164c5, /// 0xa54
			 0x8f4915e3, /// 0xa58
			 0x20187cd3, /// 0xa5c
			 0xbd9389bb, /// 0xa60
			 0x9b499173, /// 0xa64
			 0xb28aba53, /// 0xa68
			 0x73c3014f, /// 0xa6c
			 0x1bc72284, /// 0xa70
			 0x1907039a, /// 0xa74
			 0x50410f4e, /// 0xa78
			 0x3647ffac, /// 0xa7c
			 0x8abd7321, /// 0xa80
			 0x269b593f, /// 0xa84
			 0x1b653c8b, /// 0xa88
			 0xe5808595, /// 0xa8c
			 0x452cbcc9, /// 0xa90
			 0xf58a6418, /// 0xa94
			 0xb6e1048b, /// 0xa98
			 0x83f12546, /// 0xa9c
			 0xa2da2ad9, /// 0xaa0
			 0x4ef64d84, /// 0xaa4
			 0x8b32a995, /// 0xaa8
			 0x89cf89ee, /// 0xaac
			 0xd2ba625d, /// 0xab0
			 0xee103858, /// 0xab4
			 0x88d5a02d, /// 0xab8
			 0x169cbc83, /// 0xabc
			 0xd83ea40c, /// 0xac0
			 0xb60c6dab, /// 0xac4
			 0x43afe4e6, /// 0xac8
			 0xef800521, /// 0xacc
			 0x08f7a7bb, /// 0xad0
			 0xc648a217, /// 0xad4
			 0xc8ffdc43, /// 0xad8
			 0x9e87b3e7, /// 0xadc
			 0x3baddd18, /// 0xae0
			 0xcc719569, /// 0xae4
			 0xa8e0ed03, /// 0xae8
			 0xa23451df, /// 0xaec
			 0x3cb0f8a6, /// 0xaf0
			 0x520d6cdf, /// 0xaf4
			 0xa6577626, /// 0xaf8
			 0x9ab05a4b, /// 0xafc
			 0xe99c4516, /// 0xb00
			 0x82e64de6, /// 0xb04
			 0x6a61066f, /// 0xb08
			 0x440d0043, /// 0xb0c
			 0x053aaa51, /// 0xb10
			 0x4c25d684, /// 0xb14
			 0xf9c05019, /// 0xb18
			 0x412a9c64, /// 0xb1c
			 0xeb598029, /// 0xb20
			 0xcb55d26e, /// 0xb24
			 0xff69a402, /// 0xb28
			 0x3a017fd8, /// 0xb2c
			 0x4bca8452, /// 0xb30
			 0xa888172b, /// 0xb34
			 0xc569255e, /// 0xb38
			 0x95c73fcf, /// 0xb3c
			 0x87c5f23f, /// 0xb40
			 0xade1fe44, /// 0xb44
			 0xf88a9982, /// 0xb48
			 0xffd6825f, /// 0xb4c
			 0xe0ddbf29, /// 0xb50
			 0x4011a4bf, /// 0xb54
			 0x6790c0e4, /// 0xb58
			 0x5c76d29e, /// 0xb5c
			 0x4e4204d2, /// 0xb60
			 0xfa8c9b1b, /// 0xb64
			 0x64046cca, /// 0xb68
			 0x36a092a5, /// 0xb6c
			 0xd007d90f, /// 0xb70
			 0xbd7616fb, /// 0xb74
			 0x65b65e82, /// 0xb78
			 0xc8254f08, /// 0xb7c
			 0xc92baaed, /// 0xb80
			 0xb9aedebe, /// 0xb84
			 0xbc26e83c, /// 0xb88
			 0x86609123, /// 0xb8c
			 0x82a3317d, /// 0xb90
			 0xbc575b65, /// 0xb94
			 0xf236af70, /// 0xb98
			 0x500fa9fd, /// 0xb9c
			 0x8a515397, /// 0xba0
			 0x4b3a5382, /// 0xba4
			 0x2a442aef, /// 0xba8
			 0x55edce47, /// 0xbac
			 0x2fffa1c0, /// 0xbb0
			 0x1514919a, /// 0xbb4
			 0x305f9550, /// 0xbb8
			 0x8a38b387, /// 0xbbc
			 0xb91d7365, /// 0xbc0
			 0x5e73d781, /// 0xbc4
			 0xd7dfcd2a, /// 0xbc8
			 0x31fef4da, /// 0xbcc
			 0x870fcbbe, /// 0xbd0
			 0xe5aec5f9, /// 0xbd4
			 0x5cf1788c, /// 0xbd8
			 0xe5e9fdfe, /// 0xbdc
			 0x7885416b, /// 0xbe0
			 0x817cc65b, /// 0xbe4
			 0x685f57f9, /// 0xbe8
			 0x5526e8fd, /// 0xbec
			 0x8b5c9abf, /// 0xbf0
			 0x1d65781d, /// 0xbf4
			 0xe5035935, /// 0xbf8
			 0x8cd75f23, /// 0xbfc
			 0xea637771, /// 0xc00
			 0x8ab74333, /// 0xc04
			 0x984893df, /// 0xc08
			 0x7b4143cc, /// 0xc0c
			 0x6e9b7c07, /// 0xc10
			 0x54e8d6f8, /// 0xc14
			 0x95a1a57f, /// 0xc18
			 0xa1423a17, /// 0xc1c
			 0x4676274e, /// 0xc20
			 0x530f6bdb, /// 0xc24
			 0x74b5a5e1, /// 0xc28
			 0x674db975, /// 0xc2c
			 0x9a20dfa8, /// 0xc30
			 0x54bae7ba, /// 0xc34
			 0x644f153d, /// 0xc38
			 0xdac6e879, /// 0xc3c
			 0x60f5342d, /// 0xc40
			 0xc3907899, /// 0xc44
			 0xc511d89b, /// 0xc48
			 0x3e9e96c5, /// 0xc4c
			 0xb059d5f4, /// 0xc50
			 0xc21908df, /// 0xc54
			 0xa82b470a, /// 0xc58
			 0xc34acff0, /// 0xc5c
			 0xf430758d, /// 0xc60
			 0x02fe3421, /// 0xc64
			 0x58b50e81, /// 0xc68
			 0x15dc9582, /// 0xc6c
			 0xc1dc1938, /// 0xc70
			 0x29864728, /// 0xc74
			 0x54bdd766, /// 0xc78
			 0x36bf7c54, /// 0xc7c
			 0x59a3478f, /// 0xc80
			 0x3b87be6b, /// 0xc84
			 0x98562628, /// 0xc88
			 0xb376bf0c, /// 0xc8c
			 0xad687e14, /// 0xc90
			 0x3b1b7f1d, /// 0xc94
			 0x6ec3756b, /// 0xc98
			 0x6849c208, /// 0xc9c
			 0xf3c570d6, /// 0xca0
			 0x260f2edd, /// 0xca4
			 0x15bbc1a0, /// 0xca8
			 0x8ebb257f, /// 0xcac
			 0x70eee995, /// 0xcb0
			 0x9aaa88ac, /// 0xcb4
			 0x075ee359, /// 0xcb8
			 0xe63dd267, /// 0xcbc
			 0xd4e79f6d, /// 0xcc0
			 0xd580b147, /// 0xcc4
			 0x83ccbfd1, /// 0xcc8
			 0x1b387628, /// 0xccc
			 0xff1b7cb9, /// 0xcd0
			 0xcc382274, /// 0xcd4
			 0xd0db51f1, /// 0xcd8
			 0x22cac557, /// 0xcdc
			 0x4386d49e, /// 0xce0
			 0x85a3b89e, /// 0xce4
			 0x48b64303, /// 0xce8
			 0xe860f4dd, /// 0xcec
			 0xa3a51256, /// 0xcf0
			 0x9e4476bc, /// 0xcf4
			 0xa3fba540, /// 0xcf8
			 0xa1d8a4f0, /// 0xcfc
			 0xe90a5e77, /// 0xd00
			 0xdf9f2ef3, /// 0xd04
			 0x8302de67, /// 0xd08
			 0x554146e9, /// 0xd0c
			 0xe358c2d6, /// 0xd10
			 0x1561fdc7, /// 0xd14
			 0x9303d7a2, /// 0xd18
			 0x3d98c004, /// 0xd1c
			 0xf96fa401, /// 0xd20
			 0x2d3029ee, /// 0xd24
			 0x24735b13, /// 0xd28
			 0xa6a80006, /// 0xd2c
			 0x8645efc0, /// 0xd30
			 0xb0e8409c, /// 0xd34
			 0x3cf7c167, /// 0xd38
			 0xe7096ccd, /// 0xd3c
			 0xa7f822bd, /// 0xd40
			 0x3c72ba4a, /// 0xd44
			 0x4621e269, /// 0xd48
			 0x613d846c, /// 0xd4c
			 0xd8a8a446, /// 0xd50
			 0x57b21c35, /// 0xd54
			 0x60a6352d, /// 0xd58
			 0xf611c85b, /// 0xd5c
			 0xbd6dfb0f, /// 0xd60
			 0xdf354340, /// 0xd64
			 0x4e19e25c, /// 0xd68
			 0x79f95dfe, /// 0xd6c
			 0xe9283b6f, /// 0xd70
			 0x8fc20199, /// 0xd74
			 0x3592765c, /// 0xd78
			 0x1d78836c, /// 0xd7c
			 0x819e232a, /// 0xd80
			 0x427abbf2, /// 0xd84
			 0xba96e472, /// 0xd88
			 0x974449e0, /// 0xd8c
			 0x09abafbe, /// 0xd90
			 0xa120d1c6, /// 0xd94
			 0x01bfb63d, /// 0xd98
			 0xe6260fcc, /// 0xd9c
			 0x538806f4, /// 0xda0
			 0xb90c402d, /// 0xda4
			 0x44e9a4fe, /// 0xda8
			 0xff80b8df, /// 0xdac
			 0x7078ddfb, /// 0xdb0
			 0x4ffbdcab, /// 0xdb4
			 0xd5a564f8, /// 0xdb8
			 0x51a58c2b, /// 0xdbc
			 0x959c38da, /// 0xdc0
			 0x5643402a, /// 0xdc4
			 0x902a8b1e, /// 0xdc8
			 0xe8b9be75, /// 0xdcc
			 0xea9583f5, /// 0xdd0
			 0xb061bf6b, /// 0xdd4
			 0x6dcd93b9, /// 0xdd8
			 0x08826405, /// 0xddc
			 0xe81a762f, /// 0xde0
			 0xbca389c3, /// 0xde4
			 0xd7f52c9a, /// 0xde8
			 0x27127252, /// 0xdec
			 0x37dbcb54, /// 0xdf0
			 0x69ecf978, /// 0xdf4
			 0x03e5eda2, /// 0xdf8
			 0x51cabf62, /// 0xdfc
			 0xf31291aa, /// 0xe00
			 0x1e2f9fdc, /// 0xe04
			 0xaf4659f8, /// 0xe08
			 0x295b0c1f, /// 0xe0c
			 0x55875363, /// 0xe10
			 0x6c9e8bea, /// 0xe14
			 0x6677dbdf, /// 0xe18
			 0xf1fed8e3, /// 0xe1c
			 0x6c1dcb92, /// 0xe20
			 0x61e07ed1, /// 0xe24
			 0xb58a99f4, /// 0xe28
			 0xa93f39ae, /// 0xe2c
			 0x4d6eedbf, /// 0xe30
			 0x7cc3d8c0, /// 0xe34
			 0x94756204, /// 0xe38
			 0x2abf3be2, /// 0xe3c
			 0x05f0d8ca, /// 0xe40
			 0xb48ef1d5, /// 0xe44
			 0x67938263, /// 0xe48
			 0x4f7330e9, /// 0xe4c
			 0x85da7b17, /// 0xe50
			 0x1bcf88a2, /// 0xe54
			 0xecf6840d, /// 0xe58
			 0x94d7f88e, /// 0xe5c
			 0xe166342e, /// 0xe60
			 0xdcf77ed0, /// 0xe64
			 0xb23d0b8e, /// 0xe68
			 0xb7bd2273, /// 0xe6c
			 0x68064e0b, /// 0xe70
			 0x31e4f2d4, /// 0xe74
			 0x6d1b8634, /// 0xe78
			 0x33154fae, /// 0xe7c
			 0x9ea9dc3e, /// 0xe80
			 0x02e6941c, /// 0xe84
			 0xd6a2f68a, /// 0xe88
			 0xbc9fc5aa, /// 0xe8c
			 0x049dd265, /// 0xe90
			 0x43c22594, /// 0xe94
			 0x4dd87268, /// 0xe98
			 0x021ce09e, /// 0xe9c
			 0xbc7654b2, /// 0xea0
			 0x1d5c0fb9, /// 0xea4
			 0x281dceba, /// 0xea8
			 0x5d592a50, /// 0xeac
			 0x5920853e, /// 0xeb0
			 0x1d6e84e6, /// 0xeb4
			 0x57fa892c, /// 0xeb8
			 0x99a842e5, /// 0xebc
			 0x7603d708, /// 0xec0
			 0x8ba1a470, /// 0xec4
			 0x343980d0, /// 0xec8
			 0x1b3b4276, /// 0xecc
			 0x6d468472, /// 0xed0
			 0x6aac33c8, /// 0xed4
			 0x907addd8, /// 0xed8
			 0x3f5d05f6, /// 0xedc
			 0x7e959096, /// 0xee0
			 0x6c660d1b, /// 0xee4
			 0x30f362c2, /// 0xee8
			 0xc1cf7a38, /// 0xeec
			 0x4697ec19, /// 0xef0
			 0x04afb0f4, /// 0xef4
			 0xb074e005, /// 0xef8
			 0x395d9726, /// 0xefc
			 0x862f4f3f, /// 0xf00
			 0x9e7f91b6, /// 0xf04
			 0xc4fe3c5f, /// 0xf08
			 0x89b3d6f3, /// 0xf0c
			 0x4404771a, /// 0xf10
			 0xc5b9e440, /// 0xf14
			 0x10da0cc9, /// 0xf18
			 0x811b2594, /// 0xf1c
			 0x614ea7cf, /// 0xf20
			 0xe05696c6, /// 0xf24
			 0xb2e2c62c, /// 0xf28
			 0xa31dc0ff, /// 0xf2c
			 0xe7f8315d, /// 0xf30
			 0x42e59a2a, /// 0xf34
			 0x32df5734, /// 0xf38
			 0x24f1bf78, /// 0xf3c
			 0xa0f9b3fa, /// 0xf40
			 0x76faca1a, /// 0xf44
			 0x8c2a96a9, /// 0xf48
			 0xdbac4c9d, /// 0xf4c
			 0xfd1d653c, /// 0xf50
			 0xc64eea8b, /// 0xf54
			 0x8e815cf8, /// 0xf58
			 0x1b880104, /// 0xf5c
			 0x4b29d36b, /// 0xf60
			 0xf15f0184, /// 0xf64
			 0x60247215, /// 0xf68
			 0x251ccabf, /// 0xf6c
			 0x1a859df3, /// 0xf70
			 0x94818347, /// 0xf74
			 0xbda7d0ec, /// 0xf78
			 0x415f3e0f, /// 0xf7c
			 0x770dcc25, /// 0xf80
			 0x4eb918aa, /// 0xf84
			 0xe3f77c77, /// 0xf88
			 0xdc11e103, /// 0xf8c
			 0x46efb8da, /// 0xf90
			 0x92db0613, /// 0xf94
			 0x7ab37a7e, /// 0xf98
			 0x607ef69f, /// 0xf9c
			 0x0449d5a3, /// 0xfa0
			 0x41dbd7d8, /// 0xfa4
			 0xe0ff3b0f, /// 0xfa8
			 0x3ac9f282, /// 0xfac
			 0xbc4bd8e2, /// 0xfb0
			 0xdea0ac32, /// 0xfb4
			 0x85d80bc9, /// 0xfb8
			 0x8f25ca8f, /// 0xfbc
			 0xd7eca863, /// 0xfc0
			 0x992dad2a, /// 0xfc4
			 0x7d2a4c64, /// 0xfc8
			 0x659acf97, /// 0xfcc
			 0xbc424c38, /// 0xfd0
			 0x56a6e161, /// 0xfd4
			 0x6bacd106, /// 0xfd8
			 0x9951444c, /// 0xfdc
			 0x97662ae8, /// 0xfe0
			 0xb384c386, /// 0xfe4
			 0x60217b0d, /// 0xfe8
			 0x91e27097, /// 0xfec
			 0x9e7a1f4a, /// 0xff0
			 0x3dcb748a, /// 0xff4
			 0x3b91d108, /// 0xff8
			 0x6c0a96ff /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00008
			 0x7f800000,                                                  // inf                                         /// 0000c
			 0x55555555,                                                  // 4 random values                             /// 00010
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0001c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00020
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00030
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00038
			 0xff800000,                                                  // -inf                                        /// 0003c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xbf028f5c,                                                  // -0.51                                       /// 00044
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00048
			 0x3f028f5c,                                                  // 0.51                                        /// 0004c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00050
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00054
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00058
			 0xbf028f5c,                                                  // -0.51                                       /// 0005c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00060
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00064
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00068
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x3f028f5c,                                                  // 0.51                                        /// 00070
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0xcb000000,                                                  // -8388608.0                                  /// 00080
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0xffc00001,                                                  // -signaling NaN                              /// 00088
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0008c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00090
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 000d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000ec
			 0x7fc00001,                                                  // signaling NaN                               /// 000f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00100
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00118
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00120
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00124
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00128
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0012c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00130
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00134
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00138
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0013c
			 0x00000000,                                                  // zero                                        /// 00140
			 0x33333333,                                                  // 4 random values                             /// 00144
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00148
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00150
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00158
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0015c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00160
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00168
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0016c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00170
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00174
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00188
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0018c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00194
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00198
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0019c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x3f028f5c,                                                  // 0.51                                        /// 001b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001c8
			 0x4b000000,                                                  // 8388608.0                                   /// 001cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001dc
			 0xcb000000,                                                  // -8388608.0                                  /// 001e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00200
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00214
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0021c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0xff800000,                                                  // -inf                                        /// 0022c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00230
			 0x3f028f5c,                                                  // 0.51                                        /// 00234
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0023c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0024c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x33333333,                                                  // 4 random values                             /// 00254
			 0x00011111,                                                  // 9.7958E-41                                  /// 00258
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0025c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00260
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00264
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0026c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00288
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0028c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x7f800000,                                                  // inf                                         /// 00294
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00298
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x33333333,                                                  // 4 random values                             /// 002a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c8
			 0xcb000000,                                                  // -8388608.0                                  /// 002cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002dc
			 0x80000000,                                                  // -zero                                       /// 002e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x33333333,                                                  // 4 random values                             /// 002f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00300
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00304
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00314
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00318
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0031c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00320
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0032c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00334
			 0x7fc00001,                                                  // signaling NaN                               /// 00338
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0033c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00340
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00344
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00348
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0034c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00354
			 0x80011111,                                                  // -9.7958E-41                                 /// 00358
			 0xbf028f5c,                                                  // -0.51                                       /// 0035c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00360
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00368
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00370
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00378
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0037c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00380
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00384
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00388
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0038c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00390
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00394
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0039c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003c4
			 0xbf028f5c,                                                  // -0.51                                       /// 003c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d4
			 0xcb000000,                                                  // -8388608.0                                  /// 003d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00404
			 0xbf028f5c,                                                  // -0.51                                       /// 00408
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0040c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00414
			 0x0c780000,                                                  // Leading 1s:                                 /// 00418
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0041c
			 0xffc00001,                                                  // -signaling NaN                              /// 00420
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0042c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00430
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00434
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00438
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00444
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00448
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0044c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x55555555,                                                  // 4 random values                             /// 00454
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00460
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00468
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0046c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00470
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00474
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0047c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00480
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00488
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0049c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x4b000000,                                                  // 8388608.0                                   /// 004e8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004ec
			 0x00000000,                                                  // zero                                        /// 004f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00518
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0051c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00520
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00528
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000000,                                                  // -zero                                       /// 00534
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00538
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0053c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x7fc00001,                                                  // signaling NaN                               /// 00544
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00558
			 0xffc00001,                                                  // -signaling NaN                              /// 0055c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00564
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0x3f028f5c,                                                  // 0.51                                        /// 00570
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00574
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00578
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00580
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00588
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00598
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0059c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 005bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00600
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00604
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0060c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00610
			 0x7fc00001,                                                  // signaling NaN                               /// 00614
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00618
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00628
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00630
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00634
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00638
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0063c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00640
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0064c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0e000003,                                                  // Trailing 1s:                                /// 00658
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c600000,                                                  // Leading 1s:                                 /// 00664
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0066c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00670
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00674
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00678
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00684
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00688
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006d4
			 0xcb000000,                                                  // -8388608.0                                  /// 006d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006f8
			 0x55555555,                                                  // 4 random values                             /// 006fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00700
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0070c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00714
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00718
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00720
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00728
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00730
			 0xcb000000,                                                  // -8388608.0                                  /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x4b000000,                                                  // 8388608.0                                   /// 0073c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00740
			 0x7fc00001,                                                  // signaling NaN                               /// 00744
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0074c
			 0x55555555,                                                  // 4 random values                             /// 00750
			 0x4b000000,                                                  // 8388608.0                                   /// 00754
			 0x0e000003,                                                  // Trailing 1s:                                /// 00758
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0075c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00760
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00768
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x4b000000,                                                  // 8388608.0                                   /// 00770
			 0xbf028f5c,                                                  // -0.51                                       /// 00774
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00780
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00790
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00794
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0079c
			 0x3f028f5c,                                                  // 0.51                                        /// 007a0
			 0xbf028f5c,                                                  // -0.51                                       /// 007a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 007d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00800
			 0x00000000,                                                  // zero                                        /// 00804
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00808
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00814
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x33333333,                                                  // 4 random values                             /// 0081c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00820
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00828
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0082c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00830
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00834
			 0x0c400000,                                                  // Leading 1s:                                 /// 00838
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00840
			 0x80011111,                                                  // -9.7958E-41                                 /// 00844
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0084c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00854
			 0xbf028f5c,                                                  // -0.51                                       /// 00858
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00864
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00868
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0086c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00874
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00878
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0087c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00884
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00888
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x3f028f5c,                                                  // 0.51                                        /// 008b0
			 0x33333333,                                                  // 4 random values                             /// 008b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e8
			 0x80000000,                                                  // -zero                                       /// 008ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 008f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00900
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00904
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00908
			 0x7fc00001,                                                  // signaling NaN                               /// 0090c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00914
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00920
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0092c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00930
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00938
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0093c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00940
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00948
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00960
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00964
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00968
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00974
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0097c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00980
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00984
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00988
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0098c
			 0x4b000000,                                                  // 8388608.0                                   /// 00990
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00994
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00998
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0099c
			 0x7f800000,                                                  // inf                                         /// 009a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009ec
			 0xbf028f5c,                                                  // -0.51                                       /// 009f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a20
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x3f028f5c,                                                  // 0.51                                        /// 00a3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a5c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00aac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ab8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00abc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ac8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00acc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ae4
			 0x80000000,                                                  // -zero                                       /// 00ae8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00af4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00afc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b14
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b24
			 0x7fc00001,                                                  // signaling NaN                               /// 00b28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b48
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b5c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b60
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b70
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b78
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ba4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bdc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bf0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bf4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bfc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c34
			 0x55555555,                                                  // 4 random values                             /// 00c38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x7f800000,                                                  // inf                                         /// 00c40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c48
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c54
			 0x80000000,                                                  // -zero                                       /// 00c58
			 0xcb000000,                                                  // -8388608.0                                  /// 00c5c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c8c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ca4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cc4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ccc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cf4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cf8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d04
			 0xffc00001,                                                  // -signaling NaN                              /// 00d08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x00000000,                                                  // zero                                        /// 00d30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d7c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x55555555,                                                  // 4 random values                             /// 00d88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0xbf028f5c,                                                  // -0.51                                       /// 00da4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00db8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00df4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e1c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e2c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e30
			 0x00000000,                                                  // zero                                        /// 00e34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80000000,                                                  // -zero                                       /// 00e58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e84
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x7f800000,                                                  // inf                                         /// 00ea8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x33333333,                                                  // 4 random values                             /// 00eb8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ebc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ec8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ecc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ed0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00edc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ee0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ee4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ef0
			 0x00000000,                                                  // zero                                        /// 00ef4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00efc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f00
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x33333333,                                                  // 4 random values                             /// 00f1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f8c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f98
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fa4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fc0
			 0x80000000,                                                  // -zero                                       /// 00fc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fe4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ff0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff8
			 0x00800003 // min norm + 3ulp                               // subnormals +ve                              /// last
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
			 0x00000354,
			 0x000005d8,
			 0x00000520,
			 0x00000574,
			 0x00000618,
			 0x00000324,
			 0x000006c4,
			 0x000000bc,

			 /// vpu register f2
			 0x41f00000,
			 0x41f80000,
			 0x40400000,
			 0x41a80000,
			 0x41300000,
			 0x41400000,
			 0x41c80000,
			 0x41400000,

			 /// vpu register f3
			 0x41f80000,
			 0x41800000,
			 0x41800000,
			 0x40e00000,
			 0x41700000,
			 0x40c00000,
			 0x40e00000,
			 0x41e00000,

			 /// vpu register f4
			 0x41a00000,
			 0x41c80000,
			 0x42000000,
			 0x41b00000,
			 0x41900000,
			 0x41f80000,
			 0x41e00000,
			 0x41e00000,

			 /// vpu register f5
			 0x40a00000,
			 0x41700000,
			 0x41a00000,
			 0x41b80000,
			 0x40c00000,
			 0x41980000,
			 0x41d00000,
			 0x40000000,

			 /// vpu register f6
			 0x3f800000,
			 0x41900000,
			 0x41b00000,
			 0x41700000,
			 0x42000000,
			 0x41600000,
			 0x40800000,
			 0x41e80000,

			 /// vpu register f7
			 0x41c00000,
			 0x41200000,
			 0x41900000,
			 0x41200000,
			 0x41e00000,
			 0x41980000,
			 0x41c80000,
			 0x41880000,

			 /// vpu register f8
			 0x41600000,
			 0x40a00000,
			 0x41600000,
			 0x41980000,
			 0x41b80000,
			 0x41f00000,
			 0x41e80000,
			 0x41800000,

			 /// vpu register f9
			 0x41880000,
			 0x41100000,
			 0x40e00000,
			 0x41a00000,
			 0x41f80000,
			 0x41500000,
			 0x41e00000,
			 0x41600000,

			 /// vpu register f10
			 0x41c80000,
			 0x41a80000,
			 0x41c80000,
			 0x41b80000,
			 0x41500000,
			 0x41300000,
			 0x41e80000,
			 0x41d80000,

			 /// vpu register f11
			 0x41c80000,
			 0x41c80000,
			 0x41d00000,
			 0x41e00000,
			 0x41400000,
			 0x41800000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f12
			 0x40400000,
			 0x41000000,
			 0x41c80000,
			 0x41400000,
			 0x41a00000,
			 0x42000000,
			 0x41b80000,
			 0x41800000,

			 /// vpu register f13
			 0x41600000,
			 0x41f80000,
			 0x40e00000,
			 0x41100000,
			 0x41100000,
			 0x41d00000,
			 0x41b00000,
			 0x41a80000,

			 /// vpu register f14
			 0x41a80000,
			 0x41a80000,
			 0x41c00000,
			 0x41f80000,
			 0x41e80000,
			 0x41980000,
			 0x41800000,
			 0x40a00000,

			 /// vpu register f15
			 0x41a80000,
			 0x41000000,
			 0x41800000,
			 0x41b00000,
			 0x41c00000,
			 0x41c80000,
			 0x42000000,
			 0x41980000,

			 /// vpu register f16
			 0x41d00000,
			 0x41c00000,
			 0x41800000,
			 0x41300000,
			 0x41d00000,
			 0x41600000,
			 0x3f800000,
			 0x41b00000,

			 /// vpu register f17
			 0x41600000,
			 0x41e00000,
			 0x40c00000,
			 0x42000000,
			 0x41200000,
			 0x40000000,
			 0x40c00000,
			 0x41700000,

			 /// vpu register f18
			 0x41f00000,
			 0x40a00000,
			 0x41d80000,
			 0x41f00000,
			 0x41a80000,
			 0x41800000,
			 0x3f800000,
			 0x40000000,

			 /// vpu register f19
			 0x41880000,
			 0x41300000,
			 0x41e80000,
			 0x41a00000,
			 0x41d00000,
			 0x3f800000,
			 0x41200000,
			 0x41d00000,

			 /// vpu register f20
			 0x41600000,
			 0x40800000,
			 0x41100000,
			 0x40800000,
			 0x41d80000,
			 0x41d00000,
			 0x40000000,
			 0x40c00000,

			 /// vpu register f21
			 0x41700000,
			 0x41800000,
			 0x41f00000,
			 0x41d00000,
			 0x41e00000,
			 0x41700000,
			 0x41300000,
			 0x40000000,

			 /// vpu register f22
			 0x41b00000,
			 0x41b00000,
			 0x41a80000,
			 0x41f00000,
			 0x41e00000,
			 0x41400000,
			 0x40400000,
			 0x41e00000,

			 /// vpu register f23
			 0x41a00000,
			 0x41e80000,
			 0x40400000,
			 0x41600000,
			 0x41300000,
			 0x41b00000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f24
			 0x41900000,
			 0x41e80000,
			 0x41100000,
			 0x41000000,
			 0x41880000,
			 0x40e00000,
			 0x41b80000,
			 0x40e00000,

			 /// vpu register f25
			 0x40000000,
			 0x41500000,
			 0x41600000,
			 0x41000000,
			 0x40c00000,
			 0x41f00000,
			 0x41d00000,
			 0x41700000,

			 /// vpu register f26
			 0x41b00000,
			 0x40000000,
			 0x40800000,
			 0x41e80000,
			 0x41b00000,
			 0x41a80000,
			 0x41e80000,
			 0x40e00000,

			 /// vpu register f27
			 0x40c00000,
			 0x41f00000,
			 0x41600000,
			 0x41d80000,
			 0x40a00000,
			 0x41880000,
			 0x40400000,
			 0x41100000,

			 /// vpu register f28
			 0x41900000,
			 0x41700000,
			 0x41300000,
			 0x3f800000,
			 0x41d00000,
			 0x41e00000,
			 0x41b00000,
			 0x41d80000,

			 /// vpu register f29
			 0x41c80000,
			 0x41200000,
			 0x41300000,
			 0x41a80000,
			 0x41900000,
			 0x41500000,
			 0x41200000,
			 0x41700000,

			 /// vpu register f30
			 0x40e00000,
			 0x40000000,
			 0x3f800000,
			 0x42000000,
			 0x41f00000,
			 0x41500000,
			 0x41a80000,
			 0x41100000,

			 /// vpu register f31
			 0x41500000,
			 0x41800000,
			 0x41f00000,
			 0x41700000,
			 0x41b00000,
			 0x41500000,
			 0x41000000,
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
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f27, rne\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f4, rmm\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f7, rtz\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f28, dyn\n"                            ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f27, rmm\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f8, rmm\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f30, rup\n"                            ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f25, rtz\n"                            ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f8, rtz\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f28, rup\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f26, rdn\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f30, rtz\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f1, rdn\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f26, rne\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f0, rne\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f28, rup\n"                            ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f27, rne\n"                            ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f21, dyn\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f18, rmm\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f12, rdn\n"                            ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f14, dyn\n"                            ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f26, dyn\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f10, rdn\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f26, rmm\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f21, rmm\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f16, rdn\n"                            ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f1, rup\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f13, dyn\n"                            ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f8, rup\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f12, rup\n"                            ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f30, rtz\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f28, rtz\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f13, rmm\n"                            ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f19, rmm\n"                            ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f15, rmm\n"                            ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f0, rne\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x9, f29, dyn\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f11, rdn\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f19, rup\n"                            ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f27, rtz\n"                            ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f22, dyn\n"                            ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f25, rdn\n"                            ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f3, rdn\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f7, rtz\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f20, rup\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f11, rne\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f9, rmm\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f8, rtz\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f15, rmm\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f26, rmm\n"                            ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f17, rup\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f17, rup\n"                            ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f7, rup\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f19, rmm\n"                            ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f0, dyn\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f8, dyn\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f7, rup\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f5, rmm\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f21, rtz\n"                            ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f14, rne\n"                            ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f4, rmm\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f4, rup\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f8, dyn\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f8, rdn\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f9, rdn\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f27, rdn\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f9, rtz\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x9, f13, rmm\n"                            ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f11, rmm\n"                            ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f26, rdn\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f6, rdn\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x9, f17, dyn\n"                            ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f17, rup\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f25, dyn\n"                            ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f20, rup\n"                            ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f26, rup\n"                            ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f11, rmm\n"                            ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f25, rup\n"                            ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f14, rmm\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f13, rup\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f28, rne\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f28, rup\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x9, f16, rtz\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f29, rtz\n"                            ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f21, rne\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x9, f13, rtz\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f21, rtz\n"                            ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f29, rne\n"                            ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f9, dyn\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x5, f2, rmm\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f23, rmm\n"                            ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f18, rdn\n"                            ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f28, rtz\n"                            ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x9, f6, rup\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x7, f2, rdn\n"                             ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f9, rup\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x4, f8, rmm\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x6, f24, rne\n"                            ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.wu.s x3, f3, rne\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
