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
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00008
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00010
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00014
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00018
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0001c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00020
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00024
			 0x0c700000,                                                  // Leading 1s:                                 /// 00028
			 0x55555555,                                                  // 4 random values                             /// 0002c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00034
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00038
			 0x0c400000,                                                  // Leading 1s:                                 /// 0003c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00040
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00044
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00048
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0004c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00050
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00054
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00058
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00060
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00068
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0006c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00074
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0007c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00080
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00088
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0008c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00094
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00098
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0009c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b0
			 0x55555555,                                                  // 4 random values                             /// 000b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00100
			 0xffc00001,                                                  // -signaling NaN                              /// 00104
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00110
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00118
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0011c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00124
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0012c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00130
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0013c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00140
			 0x7f800000,                                                  // inf                                         /// 00144
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0014c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00154
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00158
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0015c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00164
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00168
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00170
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00178
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0017c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c700000,                                                  // Leading 1s:                                 /// 00188
			 0x0c700000,                                                  // Leading 1s:                                 /// 0018c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00194
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00198
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0019c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a0
			 0x7f800000,                                                  // inf                                         /// 001a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001d4
			 0xffc00001,                                                  // -signaling NaN                              /// 001d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00200
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00208
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00214
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00218
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0021c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00220
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00224
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00228
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00230
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00238
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0023c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00240
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x0e000003,                                                  // Trailing 1s:                                /// 0024c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00250
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00254
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00258
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00264
			 0xbf028f5c,                                                  // -0.51                                       /// 00268
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0026c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00270
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00274
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00278
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0027c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00280
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00284
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00288
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0028c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00290
			 0x0e000001,                                                  // Trailing 1s:                                /// 00294
			 0xcb000000,                                                  // -8388608.0                                  /// 00298
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0029c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 002f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x55555555,                                                  // 4 random values                             /// 00300
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00304
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00308
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0030c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00310
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00314
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00318
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0031c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00324
			 0x0c780000,                                                  // Leading 1s:                                 /// 00328
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0032c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00330
			 0xbf028f5c,                                                  // -0.51                                       /// 00334
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xbf028f5c,                                                  // -0.51                                       /// 00350
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00354
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0035c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00360
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00364
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00370
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00374
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00378
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0037c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00380
			 0x0c780000,                                                  // Leading 1s:                                 /// 00384
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00388
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0038c
			 0xffc00001,                                                  // -signaling NaN                              /// 00390
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00398
			 0x00000000,                                                  // zero                                        /// 0039c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a0
			 0x4b000000,                                                  // 8388608.0                                   /// 003a4
			 0x7fc00001,                                                  // signaling NaN                               /// 003a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003cc
			 0x7f800000,                                                  // inf                                         /// 003d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003d8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e0
			 0xcb000000,                                                  // -8388608.0                                  /// 003e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00408
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0040c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00424
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00428
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0042c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00430
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x7fc00001,                                                  // signaling NaN                               /// 00438
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0043c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00444
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c700000,                                                  // Leading 1s:                                 /// 0044c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00450
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00458
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00460
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00470
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00474
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00478
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00484
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00488
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0048c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00490
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00494
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0xff800000,                                                  // -inf                                        /// 0049c
			 0x4b000000,                                                  // 8388608.0                                   /// 004a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00500
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00508
			 0x00011111,                                                  // 9.7958E-41                                  /// 0050c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x33333333,                                                  // 4 random values                             /// 00514
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0051c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00520
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00524
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00528
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0052c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00530
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00534
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0053c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00548
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0054c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00554
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0055c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00568
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0057c
			 0x7fc00001,                                                  // signaling NaN                               /// 00580
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0058c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00598
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0059c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x4b000000,                                                  // 8388608.0                                   /// 005a8
			 0x3f028f5c,                                                  // 0.51                                        /// 005ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00600
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00604
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00608
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00610
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00614
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00618
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0061c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00620
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00628
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00630
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00640
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00644
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00648
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0064c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00650
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0065c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00000000,                                                  // zero                                        /// 00664
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00668
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00678
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0067c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00684
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00688
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0068c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00698
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0069c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006a4
			 0x7f800000,                                                  // inf                                         /// 006a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00704
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0070c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00710
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00714
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00718
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0071c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00720
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00724
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00728
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x3f028f5c,                                                  // 0.51                                        /// 00734
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x7fc00001,                                                  // signaling NaN                               /// 0073c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00740
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0074c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00750
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00754
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00758
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0075c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00760
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00768
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00770
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00774
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00780
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00784
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00011111,                                                  // 9.7958E-41                                  /// 0078c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00794
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00798
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00800
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00804
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0080c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00814
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00818
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0081c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00820
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0082c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00834
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0083c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00848
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00850
			 0xff800000,                                                  // -inf                                        /// 00854
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00860
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00864
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0086c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00870
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00874
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00878
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0087c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00880
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0088c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00890
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00898
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0089c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00000000,                                                  // zero                                        /// 008a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c0
			 0x7f800000,                                                  // inf                                         /// 008c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00904
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x3f028f5c,                                                  // 0.51                                        /// 0090c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00910
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x55555555,                                                  // 4 random values                             /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0xbf028f5c,                                                  // -0.51                                       /// 00920
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00924
			 0x00011111,                                                  // 9.7958E-41                                  /// 00928
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0092c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00930
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00938
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00940
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00944
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0094c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x7fc00001,                                                  // signaling NaN                               /// 00954
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x55555555,                                                  // 4 random values                             /// 0095c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00960
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00968
			 0x80011111,                                                  // -9.7958E-41                                 /// 0096c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00970
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0097c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00980
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00984
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00988
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0098c
			 0xcb000000,                                                  // -8388608.0                                  /// 00990
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00994
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00998
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0099c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x33333333,                                                  // 4 random values                             /// 009a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 009e0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0xbf028f5c,                                                  // -0.51                                       /// 009ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009fc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80000000,                                                  // -zero                                       /// 00a10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a40
			 0x7fc00001,                                                  // signaling NaN                               /// 00a44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a9c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aa0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0xff800000,                                                  // -inf                                        /// 00aac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00abc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00acc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ad8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00afc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b48
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b64
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b94
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bb8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bbc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bdc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bf8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c1c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c28
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00000000,                                                  // zero                                        /// 00c40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c80
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c98
			 0xff800000,                                                  // -inf                                        /// 00c9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ca0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ca8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cc4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ce0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ce4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cf0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x4b000000,                                                  // 8388608.0                                   /// 00cf8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cfc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d10
			 0xcb000000,                                                  // -8388608.0                                  /// 00d14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d38
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d48
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d50
			 0x3f028f5c,                                                  // 0.51                                        /// 00d54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d70
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d78
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dbc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dc0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dd4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00de4
			 0xff800000,                                                  // -inf                                        /// 00de8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00df0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00df8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dfc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e08
			 0xcb000000,                                                  // -8388608.0                                  /// 00e0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e14
			 0x00000000,                                                  // zero                                        /// 00e18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e20
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e4c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xcb000000,                                                  // -8388608.0                                  /// 00e74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ec0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x7f800000,                                                  // inf                                         /// 00ecc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ed0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00edc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ef4
			 0x7f800000,                                                  // inf                                         /// 00ef8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f0c
			 0x80000000,                                                  // -zero                                       /// 00f10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f20
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f34
			 0x7fc00001,                                                  // signaling NaN                               /// 00f38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f50
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f88
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fb8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fdc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fe4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fec
			 0x3f028f5c,                                                  // 0.51                                        /// 00ff0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ff4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ff8
			 0x0e00000f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00000
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xcb000000,                                                  // -8388608.0                                  /// 00008
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0000c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00018
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0001c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00020
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00024
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80011111,                                                  // -9.7958E-41                                 /// 0002c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00034
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00040
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00044
			 0x0c780000,                                                  // Leading 1s:                                 /// 00048
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00054
			 0xffc00001,                                                  // -signaling NaN                              /// 00058
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00068
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0006c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00070
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00078
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00080
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00088
			 0x3f028f5c,                                                  // 0.51                                        /// 0008c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00090
			 0x7f800000,                                                  // inf                                         /// 00094
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x7f800000,                                                  // inf                                         /// 0009c
			 0x0c400000,                                                  // Leading 1s:                                 /// 000a0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000f8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00104
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00110
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00118
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0011c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00120
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0xbf028f5c,                                                  // -0.51                                       /// 00128
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0012c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x80000000,                                                  // -zero                                       /// 00134
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00138
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0013c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00140
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00144
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00148
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0014c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00150
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0015c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00160
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00164
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00174
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00178
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0017c
			 0x4b000000,                                                  // 8388608.0                                   /// 00180
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00184
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00188
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0018c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00190
			 0x0c700000,                                                  // Leading 1s:                                 /// 00194
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00198
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c4
			 0x3f028f5c,                                                  // 0.51                                        /// 001c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001ec
			 0xcb000000,                                                  // -8388608.0                                  /// 001f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00200
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00210
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0c780000,                                                  // Leading 1s:                                 /// 00218
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0021c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00220
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00224
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0022c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00238
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0023c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00240
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00248
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00254
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00260
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0026c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x80011111,                                                  // -9.7958E-41                                 /// 0027c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00280
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00284
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00288
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00290
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00298
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0029c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 002e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002f8
			 0x3f028f5c,                                                  // 0.51                                        /// 002fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00300
			 0x80011111,                                                  // -9.7958E-41                                 /// 00304
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0030c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00310
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00314
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00318
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00320
			 0x7fc00001,                                                  // signaling NaN                               /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0xff800000,                                                  // -inf                                        /// 00330
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00334
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00340
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00344
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x0e000003,                                                  // Trailing 1s:                                /// 0034c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00350
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00354
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0035c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00360
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00364
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00368
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0036c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00011111,                                                  // 9.7958E-41                                  /// 00378
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00380
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0038c
			 0x55555555,                                                  // 4 random values                             /// 00390
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00394
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00398
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0039c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00418
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00430
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00434
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00438
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0044c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00450
			 0xff800000,                                                  // -inf                                        /// 00454
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0045c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00460
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00464
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0046c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00474
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00478
			 0x7fc00001,                                                  // signaling NaN                               /// 0047c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00480
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00484
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00488
			 0xbf028f5c,                                                  // -0.51                                       /// 0048c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00490
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00498
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0049c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x3f028f5c,                                                  // 0.51                                        /// 004cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004fc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00500
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00504
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00508
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00510
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00514
			 0x0c780000,                                                  // Leading 1s:                                 /// 00518
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00520
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00528
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0052c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00530
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00538
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00540
			 0x4b000000,                                                  // 8388608.0                                   /// 00544
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00548
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0054c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00550
			 0xbf028f5c,                                                  // -0.51                                       /// 00554
			 0x55555555,                                                  // 4 random values                             /// 00558
			 0x0c780000,                                                  // Leading 1s:                                 /// 0055c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0xffc00001,                                                  // -signaling NaN                              /// 00564
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00568
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00574
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00578
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0057c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0058c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00590
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00598
			 0x0c400000,                                                  // Leading 1s:                                 /// 0059c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005bc
			 0x33333333,                                                  // 4 random values                             /// 005c0
			 0x33333333,                                                  // 4 random values                             /// 005c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x33333333,                                                  // 4 random values                             /// 005f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00604
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00608
			 0x33333333,                                                  // 4 random values                             /// 0060c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00610
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x7f800000,                                                  // inf                                         /// 00624
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x0c600000,                                                  // Leading 1s:                                 /// 0062c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00630
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x00000000,                                                  // zero                                        /// 00638
			 0xbf028f5c,                                                  // -0.51                                       /// 0063c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00640
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00644
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00648
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0064c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00658
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0065c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00664
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0066c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00670
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00678
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00680
			 0x00011111,                                                  // 9.7958E-41                                  /// 00684
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00690
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006cc
			 0x55555555,                                                  // 4 random values                             /// 006d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00704
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00708
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0070c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00710
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0xffc00001,                                                  // -signaling NaN                              /// 00718
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00720
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00728
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0xffc00001,                                                  // -signaling NaN                              /// 00730
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00740
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0xff800000,                                                  // -inf                                        /// 00748
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0074c
			 0x4b000000,                                                  // 8388608.0                                   /// 00750
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0075c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00760
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00764
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00770
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00774
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0077c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00788
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00790
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00794
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00798
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0079c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80000000,                                                  // -zero                                       /// 007c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00800
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00804
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00808
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00810
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00814
			 0x80000000,                                                  // -zero                                       /// 00818
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00000000,                                                  // zero                                        /// 00820
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00824
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00830
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00838
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0083c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00840
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00848
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0084c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x00011111,                                                  // 9.7958E-41                                  /// 00864
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0086c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00870
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00874
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00878
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0087c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00884
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00888
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0088c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00894
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00898
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a4
			 0xffc00001,                                                  // -signaling NaN                              /// 008a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b8
			 0xcb000000,                                                  // -8388608.0                                  /// 008bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x00000000,                                                  // zero                                        /// 008cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e4
			 0x00000000,                                                  // zero                                        /// 008e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00900
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00904
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00910
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00918
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0091c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00920
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00924
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00928
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00934
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00944
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00948
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e000001,                                                  // Trailing 1s:                                /// 00954
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00958
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00960
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00968
			 0xff800000,                                                  // -inf                                        /// 0096c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00970
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00974
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00978
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00990
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0099c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009c4
			 0x80000000,                                                  // -zero                                       /// 009c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009cc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a1c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a28
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a70
			 0x3f028f5c,                                                  // 0.51                                        /// 00a74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a84
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a88
			 0xff800000,                                                  // -inf                                        /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a98
			 0x00000000,                                                  // zero                                        /// 00a9c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00aa0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00aac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ab8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00abc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00af4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af8
			 0xff800000,                                                  // -inf                                        /// 00afc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b18
			 0x33333333,                                                  // 4 random values                             /// 00b1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b34
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b50
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b7c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ba0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ba4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bc4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x00000000,                                                  // zero                                        /// 00bd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00be0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00be4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00be8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bf8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bfc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c0c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c14
			 0x3f028f5c,                                                  // 0.51                                        /// 00c18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c34
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c6c
			 0x7f800000,                                                  // inf                                         /// 00c70
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c98
			 0x80000000,                                                  // -zero                                       /// 00c9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ca0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cb0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cbc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cf8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xcb000000,                                                  // -8388608.0                                  /// 00d64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d78
			 0x80000000,                                                  // -zero                                       /// 00d7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d80
			 0x00000000,                                                  // zero                                        /// 00d84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00da8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7fc00001,                                                  // signaling NaN                               /// 00db4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00db8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dbc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00de4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e40
			 0x00000000,                                                  // zero                                        /// 00e44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e50
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e80
			 0x7f800000,                                                  // inf                                         /// 00e84
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e90
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ea4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00eb0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ec0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ec4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ec8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00edc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ee4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0x7fc00001,                                                  // signaling NaN                               /// 00eec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ef0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ef8
			 0xff800000,                                                  // -inf                                        /// 00efc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x7fc00001,                                                  // signaling NaN                               /// 00f28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f74
			 0xcb000000,                                                  // -8388608.0                                  /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000000,                                                  // zero                                        /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fa4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fbc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0xffc00001,                                                  // -signaling NaN                              /// 00fcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fe0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ff0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ff4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c7f0000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00000
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00004
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00008
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00018
			 0x80011111,                                                  // -9.7958E-41                                 /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0xff800000,                                                  // -inf                                        /// 00024
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00034
			 0x4b000000,                                                  // 8388608.0                                   /// 00038
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00040
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00048
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0004c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00050
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00054
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x0e000001,                                                  // Trailing 1s:                                /// 0005c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00060
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00064
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00070
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00074
			 0x7f800000,                                                  // inf                                         /// 00078
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00080
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00084
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00088
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0008c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00090
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00094
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0009c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 000b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00000000,                                                  // zero                                        /// 000ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00100
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00104
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00108
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0010c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00110
			 0x80011111,                                                  // -9.7958E-41                                 /// 00114
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x00000000,                                                  // zero                                        /// 00124
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00128
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0012c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00130
			 0xcb000000,                                                  // -8388608.0                                  /// 00134
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x0c700000,                                                  // Leading 1s:                                 /// 0013c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0e000003,                                                  // Trailing 1s:                                /// 00144
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00148
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0014c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00150
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00154
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0015c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00160
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00168
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00170
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00178
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0017c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00180
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00188
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0018c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00198
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001a0
			 0x4b000000,                                                  // 8388608.0                                   /// 001a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 001b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b8
			 0xbf028f5c,                                                  // -0.51                                       /// 001bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001cc
			 0x00000000,                                                  // zero                                        /// 001d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001fc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x0c600000,                                                  // Leading 1s:                                 /// 00204
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00208
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00210
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00214
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0021c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00220
			 0x00011111,                                                  // 9.7958E-41                                  /// 00224
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00228
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0022c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00230
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00244
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0024c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00250
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00258
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0025c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00264
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00000000,                                                  // zero                                        /// 0026c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00274
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00278
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0027c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00288
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0028c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x80011111,                                                  // -9.7958E-41                                 /// 0029c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x4b000000,                                                  // 8388608.0                                   /// 002f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00300
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00304
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00318
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00320
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00328
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00330
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00334
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0033c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00340
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00344
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00354
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00358
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00360
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xcb000000,                                                  // -8388608.0                                  /// 0036c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00374
			 0x0c700000,                                                  // Leading 1s:                                 /// 00378
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0037c
			 0x7fc00001,                                                  // signaling NaN                               /// 00380
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00384
			 0xcb000000,                                                  // -8388608.0                                  /// 00388
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00394
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00398
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0039c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003dc
			 0xbf028f5c,                                                  // -0.51                                       /// 003e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x80011111,                                                  // -9.7958E-41                                 /// 00404
			 0x0e000007,                                                  // Trailing 1s:                                /// 00408
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00410
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00418
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0041c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00420
			 0x0c780000,                                                  // Leading 1s:                                 /// 00424
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0042c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00434
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0043c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00440
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xffc00001,                                                  // -signaling NaN                              /// 00448
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00458
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00464
			 0x00011111,                                                  // 9.7958E-41                                  /// 00468
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0046c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00470
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00474
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0047c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00480
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00484
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00490
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x55555555,                                                  // 4 random values                             /// 0049c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x7fc00001,                                                  // signaling NaN                               /// 004c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00500
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00504
			 0x0c700000,                                                  // Leading 1s:                                 /// 00508
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0050c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0xcb000000,                                                  // -8388608.0                                  /// 00514
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00518
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0051c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00520
			 0x0c780000,                                                  // Leading 1s:                                 /// 00524
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00528
			 0x0e000001,                                                  // Trailing 1s:                                /// 0052c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00530
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00538
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00540
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x80000000,                                                  // -zero                                       /// 00548
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00550
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0055c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00560
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00564
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00000000,                                                  // zero                                        /// 00570
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00580
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00584
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00588
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00590
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00594
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00598
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0059c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 005a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 005bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x55555555,                                                  // 4 random values                             /// 005cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00600
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00604
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00608
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0060c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00610
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00618
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0061c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0062c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00634
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00638
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00640
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00648
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x3f028f5c,                                                  // 0.51                                        /// 00654
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x0e000003,                                                  // Trailing 1s:                                /// 00664
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00668
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00670
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00674
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00678
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00680
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00684
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c700000,                                                  // Leading 1s:                                 /// 0068c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00700
			 0x0c700000,                                                  // Leading 1s:                                 /// 00704
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00708
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0070c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0071c
			 0x00000000,                                                  // zero                                        /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00728
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0072c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00730
			 0xcb000000,                                                  // -8388608.0                                  /// 00734
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00738
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0073c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00740
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00744
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00748
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0074c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00750
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00754
			 0x0c700000,                                                  // Leading 1s:                                 /// 00758
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0075c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00760
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00764
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x0e000001,                                                  // Trailing 1s:                                /// 0076c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00770
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00774
			 0x00000000,                                                  // zero                                        /// 00778
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0077c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00780
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00784
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00788
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00790
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00798
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0079c
			 0x0c700000,                                                  // Leading 1s:                                 /// 007a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d0
			 0x55555555,                                                  // 4 random values                             /// 007d4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00800
			 0xffc00001,                                                  // -signaling NaN                              /// 00804
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00808
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00814
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0xffc00001,                                                  // -signaling NaN                              /// 00820
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00824
			 0xcb000000,                                                  // -8388608.0                                  /// 00828
			 0x00011111,                                                  // 9.7958E-41                                  /// 0082c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00830
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00834
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00838
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0084c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00858
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0085c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00870
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00874
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0087c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00880
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00884
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00888
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00894
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008b0
			 0x33333333,                                                  // 4 random values                             /// 008b4
			 0x55555555,                                                  // 4 random values                             /// 008b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00900
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00904
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00908
			 0x00011111,                                                  // 9.7958E-41                                  /// 0090c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00910
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00914
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00918
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0091c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00920
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00930
			 0x00011111,                                                  // 9.7958E-41                                  /// 00934
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00940
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00948
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e000001,                                                  // Trailing 1s:                                /// 00954
			 0x0e000001,                                                  // Trailing 1s:                                /// 00958
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00964
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00968
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0096c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00970
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00974
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00978
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0097c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00980
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00984
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0098c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00990
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00994
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00998
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0099c
			 0x80000000,                                                  // -zero                                       /// 009a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0xffc00001,                                                  // -signaling NaN                              /// 009b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009b4
			 0xffc00001,                                                  // -signaling NaN                              /// 009b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009c4
			 0x3f028f5c,                                                  // 0.51                                        /// 009c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d8
			 0xbf028f5c,                                                  // -0.51                                       /// 009dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009fc
			 0x7f800000,                                                  // inf                                         /// 00a00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a1c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a20
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a70
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00abc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ac8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00acc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00adc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00af0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b04
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b44
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b54
			 0xcb000000,                                                  // -8388608.0                                  /// 00b58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b68
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b74
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b78
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b84
			 0x7f800000,                                                  // inf                                         /// 00b88
			 0xffc00001,                                                  // -signaling NaN                              /// 00b8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ba8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bbc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00be4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00bf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c00
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c24
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c2c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c38
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c3c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0xbf028f5c,                                                  // -0.51                                       /// 00c44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c64
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c68
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c70
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ca8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cac
			 0x3f028f5c,                                                  // 0.51                                        /// 00cb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ce0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d04
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x3f028f5c,                                                  // 0.51                                        /// 00d28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d3c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d60
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d64
			 0x3f028f5c,                                                  // 0.51                                        /// 00d68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d6c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d78
			 0xffc00001,                                                  // -signaling NaN                              /// 00d7c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da0
			 0xffc00001,                                                  // -signaling NaN                              /// 00da4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00da8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00dd8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00df4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e1c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e28
			 0xffc00001,                                                  // -signaling NaN                              /// 00e2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e44
			 0xbf028f5c,                                                  // -0.51                                       /// 00e48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e58
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e80
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e90
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x7fc00001,                                                  // signaling NaN                               /// 00e9c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ea4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00eb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ed8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00edc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ef0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ef8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00efc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f34
			 0xbf028f5c,                                                  // -0.51                                       /// 00f38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f80
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fa0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fd0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fd8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fe0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ff4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80000100                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xdb2424ab, /// 0x0
			 0x5f5d9995, /// 0x4
			 0xdbe4cf81, /// 0x8
			 0x27e3de72, /// 0xc
			 0x43df10a8, /// 0x10
			 0xd63a74f0, /// 0x14
			 0x0bb516d1, /// 0x18
			 0xe25a9bda, /// 0x1c
			 0xdb46f933, /// 0x20
			 0xcfc27f04, /// 0x24
			 0x756ac4b9, /// 0x28
			 0x8a216ebb, /// 0x2c
			 0x570dc58d, /// 0x30
			 0x9bd03e53, /// 0x34
			 0xb29344b4, /// 0x38
			 0xe03cbe61, /// 0x3c
			 0xa0fef1ba, /// 0x40
			 0x0081bfb3, /// 0x44
			 0x436e6c15, /// 0x48
			 0xbd01a66a, /// 0x4c
			 0xed5c1fa5, /// 0x50
			 0x89e217c5, /// 0x54
			 0x09f8767a, /// 0x58
			 0x95e0bdd8, /// 0x5c
			 0xa8e70ec7, /// 0x60
			 0x01d35cf2, /// 0x64
			 0xa8c2e99b, /// 0x68
			 0xc104f93e, /// 0x6c
			 0x3292f0b6, /// 0x70
			 0x24e6fe50, /// 0x74
			 0x8d06a5c5, /// 0x78
			 0x619f9a5f, /// 0x7c
			 0x166b5216, /// 0x80
			 0x834927e1, /// 0x84
			 0x42e3fea4, /// 0x88
			 0x0fee75b1, /// 0x8c
			 0x0b020367, /// 0x90
			 0xca9b63ce, /// 0x94
			 0x8a295ee9, /// 0x98
			 0x6098a1ed, /// 0x9c
			 0x59a957e2, /// 0xa0
			 0x687352e1, /// 0xa4
			 0x539e7902, /// 0xa8
			 0x195a9a3d, /// 0xac
			 0xce6c14b8, /// 0xb0
			 0x15be803c, /// 0xb4
			 0x8032243d, /// 0xb8
			 0xc5c62867, /// 0xbc
			 0x0f7ad000, /// 0xc0
			 0x94683e3d, /// 0xc4
			 0x0fffcdbf, /// 0xc8
			 0xaaa08649, /// 0xcc
			 0x530c1f1b, /// 0xd0
			 0xfb3bb76f, /// 0xd4
			 0x86847c21, /// 0xd8
			 0xfbda222e, /// 0xdc
			 0x25e1ad69, /// 0xe0
			 0xa0521cf2, /// 0xe4
			 0x00517c2d, /// 0xe8
			 0x2bcfd6e0, /// 0xec
			 0xc35427a2, /// 0xf0
			 0x0edc6956, /// 0xf4
			 0xc0c04cf3, /// 0xf8
			 0xfedebfc8, /// 0xfc
			 0x75252d52, /// 0x100
			 0xa7e6ee7d, /// 0x104
			 0x173bd9af, /// 0x108
			 0xdd9257e6, /// 0x10c
			 0xa1b517d1, /// 0x110
			 0xba625c73, /// 0x114
			 0xe13a4f10, /// 0x118
			 0xdc7061b4, /// 0x11c
			 0x4daaedbf, /// 0x120
			 0x2bf74b93, /// 0x124
			 0xc44a70ce, /// 0x128
			 0xd5d4c0ce, /// 0x12c
			 0xb6471c07, /// 0x130
			 0xea4b34bc, /// 0x134
			 0xe6ee3b56, /// 0x138
			 0x170948cc, /// 0x13c
			 0xd407aa83, /// 0x140
			 0xa8f5f94a, /// 0x144
			 0x909b1d94, /// 0x148
			 0x4d7a7798, /// 0x14c
			 0x1055b9a0, /// 0x150
			 0x7a8b401c, /// 0x154
			 0x3d902084, /// 0x158
			 0xe60414a9, /// 0x15c
			 0xc98287ba, /// 0x160
			 0x3403a786, /// 0x164
			 0x1b755e43, /// 0x168
			 0xde51c1c7, /// 0x16c
			 0xe4a124da, /// 0x170
			 0x96d84a11, /// 0x174
			 0x3d806c30, /// 0x178
			 0xcc61f786, /// 0x17c
			 0xa601b57f, /// 0x180
			 0x725181e7, /// 0x184
			 0xa70627b7, /// 0x188
			 0x2eabf109, /// 0x18c
			 0xdec91353, /// 0x190
			 0xb18b0fdf, /// 0x194
			 0x02e42e47, /// 0x198
			 0x4efd9add, /// 0x19c
			 0xb96508b3, /// 0x1a0
			 0xb1887900, /// 0x1a4
			 0xd352d72e, /// 0x1a8
			 0xd388b7cb, /// 0x1ac
			 0x5b9ed930, /// 0x1b0
			 0xa081a779, /// 0x1b4
			 0x099bbb4e, /// 0x1b8
			 0x4720bc80, /// 0x1bc
			 0xbc160129, /// 0x1c0
			 0x19ad7fe7, /// 0x1c4
			 0xbfd71810, /// 0x1c8
			 0xbb5c4b18, /// 0x1cc
			 0xb2f916da, /// 0x1d0
			 0x910872e1, /// 0x1d4
			 0x1af83553, /// 0x1d8
			 0x1a5a073e, /// 0x1dc
			 0x6bc53111, /// 0x1e0
			 0xd7e34413, /// 0x1e4
			 0x70db543f, /// 0x1e8
			 0xa0c4740c, /// 0x1ec
			 0xa32a445b, /// 0x1f0
			 0x55bf1937, /// 0x1f4
			 0xe369f476, /// 0x1f8
			 0xc3799e4a, /// 0x1fc
			 0x7d83131d, /// 0x200
			 0x87feca8b, /// 0x204
			 0x1cbdf0d8, /// 0x208
			 0xc377af5a, /// 0x20c
			 0x17572303, /// 0x210
			 0xa0a850cb, /// 0x214
			 0x6e741643, /// 0x218
			 0xf1a0b5b2, /// 0x21c
			 0x35716135, /// 0x220
			 0x76516880, /// 0x224
			 0xfbfe47c9, /// 0x228
			 0xe7f4bc3a, /// 0x22c
			 0xfc4ca3ae, /// 0x230
			 0xf41bb22c, /// 0x234
			 0x93f69c39, /// 0x238
			 0x4948e1b7, /// 0x23c
			 0x651f078d, /// 0x240
			 0xbf1180e2, /// 0x244
			 0xab4997ea, /// 0x248
			 0x67559561, /// 0x24c
			 0x29dad4f8, /// 0x250
			 0x895f867e, /// 0x254
			 0xc6a88974, /// 0x258
			 0x7fbfd07c, /// 0x25c
			 0xdf857e6f, /// 0x260
			 0x8fdff652, /// 0x264
			 0xbadf8548, /// 0x268
			 0x00258de4, /// 0x26c
			 0x1b37d71f, /// 0x270
			 0x3fc6a928, /// 0x274
			 0x36902949, /// 0x278
			 0x85d99926, /// 0x27c
			 0x216d64df, /// 0x280
			 0x4d21b09e, /// 0x284
			 0xe0638fb6, /// 0x288
			 0xf6f09fd5, /// 0x28c
			 0x3d6701bf, /// 0x290
			 0xa8bd1678, /// 0x294
			 0x21bfd1ef, /// 0x298
			 0x596a0c27, /// 0x29c
			 0xc7788c1a, /// 0x2a0
			 0xad5b85a2, /// 0x2a4
			 0xcc4e62ce, /// 0x2a8
			 0x3f4a15e2, /// 0x2ac
			 0x43df0d2f, /// 0x2b0
			 0xf721795c, /// 0x2b4
			 0x6e73e136, /// 0x2b8
			 0x8d9e6296, /// 0x2bc
			 0x76f05366, /// 0x2c0
			 0x791e4198, /// 0x2c4
			 0xae67e355, /// 0x2c8
			 0xbfb90ba6, /// 0x2cc
			 0x896b949c, /// 0x2d0
			 0x26794f82, /// 0x2d4
			 0x47185736, /// 0x2d8
			 0xb8e9589c, /// 0x2dc
			 0x6a786431, /// 0x2e0
			 0xdd57a561, /// 0x2e4
			 0x10758b39, /// 0x2e8
			 0xf5a3130b, /// 0x2ec
			 0xf8627916, /// 0x2f0
			 0xc14fca59, /// 0x2f4
			 0x4f60b3e0, /// 0x2f8
			 0xb1076723, /// 0x2fc
			 0x7f7f59bd, /// 0x300
			 0x51001e40, /// 0x304
			 0x7351be27, /// 0x308
			 0xc6467e9e, /// 0x30c
			 0x5a83f6e5, /// 0x310
			 0x9ec397cf, /// 0x314
			 0xbb2e97e9, /// 0x318
			 0xe34a0f6b, /// 0x31c
			 0xc81016aa, /// 0x320
			 0x4943f189, /// 0x324
			 0x05c30743, /// 0x328
			 0x942e5a8c, /// 0x32c
			 0x58559565, /// 0x330
			 0x141d7e61, /// 0x334
			 0x8ffc604a, /// 0x338
			 0xb9c7cbd9, /// 0x33c
			 0xfa3bdab0, /// 0x340
			 0x78ce93f8, /// 0x344
			 0xdefed68c, /// 0x348
			 0x3978b743, /// 0x34c
			 0xb9e779cb, /// 0x350
			 0x2fc84d32, /// 0x354
			 0xceb4a17b, /// 0x358
			 0x4eabd559, /// 0x35c
			 0x94ff25a7, /// 0x360
			 0x43016de0, /// 0x364
			 0xd3cab9eb, /// 0x368
			 0xa78fe8bf, /// 0x36c
			 0x9c8a0dec, /// 0x370
			 0xb7267bf0, /// 0x374
			 0x2b61c8f8, /// 0x378
			 0xb78069ff, /// 0x37c
			 0x65180271, /// 0x380
			 0x65ac0e92, /// 0x384
			 0xa8962a6a, /// 0x388
			 0x9c39d866, /// 0x38c
			 0xcef29cdd, /// 0x390
			 0x4d60d621, /// 0x394
			 0xbe6056b8, /// 0x398
			 0xe2f5fd5c, /// 0x39c
			 0xb37af17a, /// 0x3a0
			 0x95739946, /// 0x3a4
			 0xd5ccc007, /// 0x3a8
			 0x1618bc31, /// 0x3ac
			 0xf6ac49e6, /// 0x3b0
			 0x2085defe, /// 0x3b4
			 0x81ce1d03, /// 0x3b8
			 0x8b89f4b2, /// 0x3bc
			 0x4427909b, /// 0x3c0
			 0xc7b796ce, /// 0x3c4
			 0x2a55b944, /// 0x3c8
			 0x885613b5, /// 0x3cc
			 0xe0369308, /// 0x3d0
			 0x3344ecc4, /// 0x3d4
			 0xec6c2511, /// 0x3d8
			 0x5a5c827e, /// 0x3dc
			 0xab181aea, /// 0x3e0
			 0x0c02b336, /// 0x3e4
			 0xea96812d, /// 0x3e8
			 0x85157ac3, /// 0x3ec
			 0x3c11df94, /// 0x3f0
			 0x2a4f9b12, /// 0x3f4
			 0x2e1276db, /// 0x3f8
			 0x62b67dc6, /// 0x3fc
			 0x73839063, /// 0x400
			 0x40b9dfaa, /// 0x404
			 0x8e492374, /// 0x408
			 0xc90a962e, /// 0x40c
			 0x800d7977, /// 0x410
			 0x7425c2e4, /// 0x414
			 0x205ae0db, /// 0x418
			 0x0cf0195e, /// 0x41c
			 0x4a3cad9b, /// 0x420
			 0x437c4864, /// 0x424
			 0x8765c40c, /// 0x428
			 0x02e72a7c, /// 0x42c
			 0x24fa5b82, /// 0x430
			 0x571e351d, /// 0x434
			 0xc0199aa7, /// 0x438
			 0x5f68136c, /// 0x43c
			 0x6ebad870, /// 0x440
			 0xd838eb87, /// 0x444
			 0xe8d7f71c, /// 0x448
			 0x080bae6b, /// 0x44c
			 0xc832983e, /// 0x450
			 0x739d6eb3, /// 0x454
			 0x844b4656, /// 0x458
			 0x33a170e2, /// 0x45c
			 0xa51bc8b3, /// 0x460
			 0xe6ede10d, /// 0x464
			 0x17a4c083, /// 0x468
			 0xea149e81, /// 0x46c
			 0x67892fb2, /// 0x470
			 0x5f5e259f, /// 0x474
			 0xc1409973, /// 0x478
			 0xb29cc65f, /// 0x47c
			 0x85ce1ae9, /// 0x480
			 0x3cb4d06b, /// 0x484
			 0x0705ca75, /// 0x488
			 0xbd493c21, /// 0x48c
			 0x51bed241, /// 0x490
			 0xf194bba0, /// 0x494
			 0x32dfacd6, /// 0x498
			 0x3de914c9, /// 0x49c
			 0x6fd6c037, /// 0x4a0
			 0x75d37a09, /// 0x4a4
			 0x91479fa2, /// 0x4a8
			 0xaef9ded1, /// 0x4ac
			 0x7ea975a0, /// 0x4b0
			 0x95556f1d, /// 0x4b4
			 0xef685dd4, /// 0x4b8
			 0xd51ec29f, /// 0x4bc
			 0x17967f50, /// 0x4c0
			 0xa132738c, /// 0x4c4
			 0x8b5bd285, /// 0x4c8
			 0x0bcf2aa3, /// 0x4cc
			 0x328de28b, /// 0x4d0
			 0x94b80b8a, /// 0x4d4
			 0x48fafea8, /// 0x4d8
			 0xb6255323, /// 0x4dc
			 0xdcfa06b1, /// 0x4e0
			 0x579d6d92, /// 0x4e4
			 0x6ef67e67, /// 0x4e8
			 0x5158572e, /// 0x4ec
			 0xbdaca36a, /// 0x4f0
			 0x04247dd0, /// 0x4f4
			 0x9f820319, /// 0x4f8
			 0x0749ce5b, /// 0x4fc
			 0x01c83e85, /// 0x500
			 0x462fbc90, /// 0x504
			 0x5523716b, /// 0x508
			 0x6afd84de, /// 0x50c
			 0x45b7a050, /// 0x510
			 0xcca2f4f5, /// 0x514
			 0xc5b9fa69, /// 0x518
			 0xc178d667, /// 0x51c
			 0xf0b3dea9, /// 0x520
			 0x54c9b0f0, /// 0x524
			 0x96cc7880, /// 0x528
			 0x8056c99d, /// 0x52c
			 0x8ac139cb, /// 0x530
			 0x679373b9, /// 0x534
			 0x617e1ee3, /// 0x538
			 0xc6363e30, /// 0x53c
			 0x26cbbb25, /// 0x540
			 0x7b5c8ccf, /// 0x544
			 0xc770e3b8, /// 0x548
			 0xc7ac5a3c, /// 0x54c
			 0x778c9e12, /// 0x550
			 0x2748d6b5, /// 0x554
			 0x011f93d6, /// 0x558
			 0x7faaca42, /// 0x55c
			 0x284e178e, /// 0x560
			 0x881ca81b, /// 0x564
			 0x53af4597, /// 0x568
			 0xec61cbc9, /// 0x56c
			 0x39e95480, /// 0x570
			 0x16cca9a4, /// 0x574
			 0x5ae11c04, /// 0x578
			 0xe66659c8, /// 0x57c
			 0xf2cafc58, /// 0x580
			 0x3f215582, /// 0x584
			 0x66356445, /// 0x588
			 0x8506af86, /// 0x58c
			 0x567c8cde, /// 0x590
			 0xe932a461, /// 0x594
			 0x88d28bde, /// 0x598
			 0xfd42c9e7, /// 0x59c
			 0x09f1c772, /// 0x5a0
			 0xa7b2ac5e, /// 0x5a4
			 0x58ea7263, /// 0x5a8
			 0xd4bb6fc3, /// 0x5ac
			 0xcfc55b3f, /// 0x5b0
			 0x26ac70f9, /// 0x5b4
			 0x357ccd30, /// 0x5b8
			 0xdf086ba3, /// 0x5bc
			 0x99ae3c7a, /// 0x5c0
			 0xd7c7c494, /// 0x5c4
			 0x2969e2f7, /// 0x5c8
			 0xf24a5bc6, /// 0x5cc
			 0xcf3e7af7, /// 0x5d0
			 0x5668cded, /// 0x5d4
			 0x7d6a25f5, /// 0x5d8
			 0xe88717e0, /// 0x5dc
			 0xf991bd03, /// 0x5e0
			 0xb0b1c79b, /// 0x5e4
			 0x26057f31, /// 0x5e8
			 0xbcb06847, /// 0x5ec
			 0xb8af6242, /// 0x5f0
			 0x3c8c78de, /// 0x5f4
			 0x0696e4f5, /// 0x5f8
			 0x35d1af9f, /// 0x5fc
			 0x349c7354, /// 0x600
			 0xc6d9ac77, /// 0x604
			 0xbae56a93, /// 0x608
			 0xb2fddc14, /// 0x60c
			 0xf432136d, /// 0x610
			 0xd057fc9f, /// 0x614
			 0x7de591e8, /// 0x618
			 0x342a5568, /// 0x61c
			 0x60f05892, /// 0x620
			 0x8d2286e2, /// 0x624
			 0x3abcb146, /// 0x628
			 0x8f97ef8e, /// 0x62c
			 0xda2b9bc5, /// 0x630
			 0xecd42721, /// 0x634
			 0x9213df60, /// 0x638
			 0xd599dad4, /// 0x63c
			 0x725bbb79, /// 0x640
			 0x5439f96f, /// 0x644
			 0x65ad6f64, /// 0x648
			 0xc5499b90, /// 0x64c
			 0x5099b18b, /// 0x650
			 0x2649ec1c, /// 0x654
			 0x1ccf04d6, /// 0x658
			 0xa247a1e8, /// 0x65c
			 0x84bb6dc4, /// 0x660
			 0xb1a0dbb6, /// 0x664
			 0xfdfdeedc, /// 0x668
			 0x9c360e19, /// 0x66c
			 0x065ff3d2, /// 0x670
			 0x0141e246, /// 0x674
			 0x4486c15c, /// 0x678
			 0x9c6d17f2, /// 0x67c
			 0x5ba3010d, /// 0x680
			 0x97caf8d3, /// 0x684
			 0x5caf7270, /// 0x688
			 0x35b48b55, /// 0x68c
			 0xc01a79a6, /// 0x690
			 0xbe85bd0e, /// 0x694
			 0x85172d20, /// 0x698
			 0x139c0f40, /// 0x69c
			 0xc2650e0b, /// 0x6a0
			 0x02856bfd, /// 0x6a4
			 0xf7523920, /// 0x6a8
			 0x95bb6994, /// 0x6ac
			 0x9b03a755, /// 0x6b0
			 0x7c08f08f, /// 0x6b4
			 0xb96ea7e8, /// 0x6b8
			 0xacb6d3ee, /// 0x6bc
			 0x2697f1bf, /// 0x6c0
			 0x01583b3d, /// 0x6c4
			 0x7a878489, /// 0x6c8
			 0x0c8c6a8b, /// 0x6cc
			 0x474954af, /// 0x6d0
			 0xc9045b36, /// 0x6d4
			 0xef167009, /// 0x6d8
			 0xb2f75784, /// 0x6dc
			 0x83900173, /// 0x6e0
			 0x1a2a84a0, /// 0x6e4
			 0x65fbce41, /// 0x6e8
			 0x3d8259f1, /// 0x6ec
			 0x0636facd, /// 0x6f0
			 0x2b1d5a5d, /// 0x6f4
			 0xbe88a642, /// 0x6f8
			 0x27f0cec3, /// 0x6fc
			 0x97240bc0, /// 0x700
			 0x0e07f1ba, /// 0x704
			 0x16c497d2, /// 0x708
			 0x8ed09e23, /// 0x70c
			 0x29645d16, /// 0x710
			 0x61c0f4b3, /// 0x714
			 0xc1931b94, /// 0x718
			 0x87d948a6, /// 0x71c
			 0xbce70c88, /// 0x720
			 0xe9e3fef0, /// 0x724
			 0xbfe2d306, /// 0x728
			 0x320466f1, /// 0x72c
			 0xfe14af8d, /// 0x730
			 0xb770e612, /// 0x734
			 0x6d841bdc, /// 0x738
			 0x2d4451f1, /// 0x73c
			 0xdea9d33d, /// 0x740
			 0xb592b470, /// 0x744
			 0x83b218d5, /// 0x748
			 0x9409dcf5, /// 0x74c
			 0x3f7c6372, /// 0x750
			 0xd7e43bcc, /// 0x754
			 0x9a17e8be, /// 0x758
			 0x1c982dcd, /// 0x75c
			 0xbe78219d, /// 0x760
			 0x6da68825, /// 0x764
			 0xd8164d1a, /// 0x768
			 0x7b4a3cfa, /// 0x76c
			 0x73971320, /// 0x770
			 0x1a545dad, /// 0x774
			 0x84cc9510, /// 0x778
			 0x558598be, /// 0x77c
			 0x4461ebb6, /// 0x780
			 0x79b277ec, /// 0x784
			 0x35a030a3, /// 0x788
			 0x01895422, /// 0x78c
			 0xe7199b03, /// 0x790
			 0xa465c000, /// 0x794
			 0x8aea0784, /// 0x798
			 0xe11346f5, /// 0x79c
			 0x5b5d40fb, /// 0x7a0
			 0x51258371, /// 0x7a4
			 0x13217131, /// 0x7a8
			 0x69e8ffaa, /// 0x7ac
			 0xd38ed453, /// 0x7b0
			 0x8e511fb0, /// 0x7b4
			 0x7040f5e7, /// 0x7b8
			 0x0f87be07, /// 0x7bc
			 0x03bfa1c7, /// 0x7c0
			 0x3e0e4b1e, /// 0x7c4
			 0x56ec5d94, /// 0x7c8
			 0x9c65df4f, /// 0x7cc
			 0x6bed46c2, /// 0x7d0
			 0x792250b0, /// 0x7d4
			 0x1417622f, /// 0x7d8
			 0x747466cc, /// 0x7dc
			 0x4f58a840, /// 0x7e0
			 0xc8f0b30c, /// 0x7e4
			 0x06c64ec4, /// 0x7e8
			 0xa52038c1, /// 0x7ec
			 0x9dba686b, /// 0x7f0
			 0x0b6468db, /// 0x7f4
			 0x7b904b98, /// 0x7f8
			 0x690ea3df, /// 0x7fc
			 0xab8cc33b, /// 0x800
			 0xdedec8e1, /// 0x804
			 0xc4fb0cb4, /// 0x808
			 0xbd961ed5, /// 0x80c
			 0x8a2ad9f8, /// 0x810
			 0x209ff97c, /// 0x814
			 0x31326e09, /// 0x818
			 0xdd27bd1b, /// 0x81c
			 0xee95dfad, /// 0x820
			 0xb72f1869, /// 0x824
			 0xc19dc3be, /// 0x828
			 0x15c8438e, /// 0x82c
			 0xff230b8b, /// 0x830
			 0xca80b734, /// 0x834
			 0xdc993cec, /// 0x838
			 0xd38676cc, /// 0x83c
			 0x691810ff, /// 0x840
			 0x6d6d7866, /// 0x844
			 0xc70d96bf, /// 0x848
			 0xa8f34604, /// 0x84c
			 0xf26c2518, /// 0x850
			 0xb45ebf24, /// 0x854
			 0xc5433cf9, /// 0x858
			 0xde0df7d3, /// 0x85c
			 0x6907ff8a, /// 0x860
			 0x37359f6e, /// 0x864
			 0x38a2f6c5, /// 0x868
			 0x28243093, /// 0x86c
			 0x47beaa91, /// 0x870
			 0x056e241d, /// 0x874
			 0x72c1121d, /// 0x878
			 0xdf662cef, /// 0x87c
			 0x44ae7459, /// 0x880
			 0xc52bd41a, /// 0x884
			 0xc552432b, /// 0x888
			 0x0c618aa5, /// 0x88c
			 0x74f6d731, /// 0x890
			 0x2d290e87, /// 0x894
			 0x112b5476, /// 0x898
			 0xf1d70ac0, /// 0x89c
			 0x34928b93, /// 0x8a0
			 0x31564a6d, /// 0x8a4
			 0x29c03db8, /// 0x8a8
			 0xe7e83d26, /// 0x8ac
			 0xae0d1e90, /// 0x8b0
			 0x3710aa05, /// 0x8b4
			 0x0a21a811, /// 0x8b8
			 0x232767d8, /// 0x8bc
			 0xac2cbce0, /// 0x8c0
			 0x6427b0eb, /// 0x8c4
			 0x15108277, /// 0x8c8
			 0xad99d354, /// 0x8cc
			 0xc5b73fbc, /// 0x8d0
			 0xf555dbb0, /// 0x8d4
			 0x7fffb495, /// 0x8d8
			 0x9a103361, /// 0x8dc
			 0x28112d49, /// 0x8e0
			 0x525689b5, /// 0x8e4
			 0xaa3cb82d, /// 0x8e8
			 0x08ef4cbf, /// 0x8ec
			 0x2eefef3e, /// 0x8f0
			 0xebd26bec, /// 0x8f4
			 0x115f2c44, /// 0x8f8
			 0xbd8c608f, /// 0x8fc
			 0xd4136991, /// 0x900
			 0xdcb63d97, /// 0x904
			 0xa32f576a, /// 0x908
			 0x991f2b25, /// 0x90c
			 0xbe7d600c, /// 0x910
			 0x33d3f91a, /// 0x914
			 0xa0d6eac9, /// 0x918
			 0x47e986f3, /// 0x91c
			 0x7e60262d, /// 0x920
			 0x6b4bb241, /// 0x924
			 0x1f5ad646, /// 0x928
			 0x53669639, /// 0x92c
			 0xde488d5e, /// 0x930
			 0x3795db00, /// 0x934
			 0xa3456557, /// 0x938
			 0xa108c84a, /// 0x93c
			 0xf2058317, /// 0x940
			 0x2e5eb8ec, /// 0x944
			 0x71bd8a0d, /// 0x948
			 0xf9e247dc, /// 0x94c
			 0x12adb531, /// 0x950
			 0x1e8a4896, /// 0x954
			 0x707021d0, /// 0x958
			 0x605b2d49, /// 0x95c
			 0x6db0ca9d, /// 0x960
			 0x052193eb, /// 0x964
			 0xc59f8468, /// 0x968
			 0x549ed285, /// 0x96c
			 0xf735e73a, /// 0x970
			 0xb7f3a091, /// 0x974
			 0x2a6b5577, /// 0x978
			 0x9491c4f4, /// 0x97c
			 0x9654f672, /// 0x980
			 0xa558c73f, /// 0x984
			 0x8702eae9, /// 0x988
			 0xf702b960, /// 0x98c
			 0xbbbed3d5, /// 0x990
			 0x65d6ae41, /// 0x994
			 0x14cda3aa, /// 0x998
			 0x8339ed0e, /// 0x99c
			 0x71bd9150, /// 0x9a0
			 0xfded8736, /// 0x9a4
			 0x0436b980, /// 0x9a8
			 0xd2bcbc83, /// 0x9ac
			 0xbc6b34a2, /// 0x9b0
			 0xb86881ef, /// 0x9b4
			 0xdfbdd2fc, /// 0x9b8
			 0x41d8a23e, /// 0x9bc
			 0x88b93b5c, /// 0x9c0
			 0xd232373c, /// 0x9c4
			 0xdc63211a, /// 0x9c8
			 0xd9458139, /// 0x9cc
			 0x1d423174, /// 0x9d0
			 0xd8e31f75, /// 0x9d4
			 0x3cb93599, /// 0x9d8
			 0x1b62632d, /// 0x9dc
			 0xf20e35a3, /// 0x9e0
			 0xccd745d3, /// 0x9e4
			 0x45d985a4, /// 0x9e8
			 0x24ccf053, /// 0x9ec
			 0x9c98ad1f, /// 0x9f0
			 0xf0c9a087, /// 0x9f4
			 0x1ba1d759, /// 0x9f8
			 0x9392ec84, /// 0x9fc
			 0x93f0fb32, /// 0xa00
			 0x10a61e62, /// 0xa04
			 0x317076be, /// 0xa08
			 0xa072633c, /// 0xa0c
			 0xf1e39c88, /// 0xa10
			 0x722e866e, /// 0xa14
			 0x54acbd53, /// 0xa18
			 0xc9377c53, /// 0xa1c
			 0x04117847, /// 0xa20
			 0xb098523c, /// 0xa24
			 0x02ff1fd8, /// 0xa28
			 0x51eaf0b9, /// 0xa2c
			 0xd4bc5032, /// 0xa30
			 0x465b3c2f, /// 0xa34
			 0x129c9525, /// 0xa38
			 0x6bca19c1, /// 0xa3c
			 0x728ee858, /// 0xa40
			 0x5f170b3d, /// 0xa44
			 0x462b8405, /// 0xa48
			 0x3dfef5d7, /// 0xa4c
			 0x4fc64d46, /// 0xa50
			 0x37275951, /// 0xa54
			 0x0277b704, /// 0xa58
			 0xbdc3c0d8, /// 0xa5c
			 0xcf248e75, /// 0xa60
			 0xc7199049, /// 0xa64
			 0x2461ae14, /// 0xa68
			 0x5104f027, /// 0xa6c
			 0x100aef99, /// 0xa70
			 0x4982c9b5, /// 0xa74
			 0x4461e4c3, /// 0xa78
			 0x8e673c25, /// 0xa7c
			 0xa9520e9a, /// 0xa80
			 0xf75fe6c7, /// 0xa84
			 0x26e84507, /// 0xa88
			 0x0e908275, /// 0xa8c
			 0xcf8cd775, /// 0xa90
			 0xb9996119, /// 0xa94
			 0xc8dcdd03, /// 0xa98
			 0x27158c89, /// 0xa9c
			 0x77ea1707, /// 0xaa0
			 0xe5cf4263, /// 0xaa4
			 0x264754e0, /// 0xaa8
			 0xd3a97817, /// 0xaac
			 0x693a8384, /// 0xab0
			 0x8edbcbb1, /// 0xab4
			 0xfe4f01f1, /// 0xab8
			 0x1b340328, /// 0xabc
			 0xf4730da2, /// 0xac0
			 0x7802a0cc, /// 0xac4
			 0xf99f068e, /// 0xac8
			 0xdec509a8, /// 0xacc
			 0x99444c7c, /// 0xad0
			 0xc23753fb, /// 0xad4
			 0x200c8d4f, /// 0xad8
			 0x4fa59a2f, /// 0xadc
			 0x9ba0b9ed, /// 0xae0
			 0xb7464a13, /// 0xae4
			 0xe4508693, /// 0xae8
			 0x2f4a0e6b, /// 0xaec
			 0x26f85203, /// 0xaf0
			 0x50b96df6, /// 0xaf4
			 0x637dca05, /// 0xaf8
			 0x393b2195, /// 0xafc
			 0x4f350c8c, /// 0xb00
			 0xee0beae9, /// 0xb04
			 0xe7fc8baf, /// 0xb08
			 0x165bfac1, /// 0xb0c
			 0xab2c4ec5, /// 0xb10
			 0xd7c2ae60, /// 0xb14
			 0x29025a90, /// 0xb18
			 0xf497db23, /// 0xb1c
			 0x2d3b9d96, /// 0xb20
			 0x3f211623, /// 0xb24
			 0x4279ff31, /// 0xb28
			 0xe10c1f86, /// 0xb2c
			 0x491297a3, /// 0xb30
			 0x403fe2b6, /// 0xb34
			 0x6f268435, /// 0xb38
			 0x5fc8d29b, /// 0xb3c
			 0x0b998a6e, /// 0xb40
			 0xfbcd3950, /// 0xb44
			 0x683e0a48, /// 0xb48
			 0x07b04226, /// 0xb4c
			 0x90080ff8, /// 0xb50
			 0x4a1568dc, /// 0xb54
			 0xbe79df6c, /// 0xb58
			 0xec6c5439, /// 0xb5c
			 0xf56b3d3e, /// 0xb60
			 0x979f6976, /// 0xb64
			 0x89c3f807, /// 0xb68
			 0xf5ad0dec, /// 0xb6c
			 0xb4245ed1, /// 0xb70
			 0xc8c2e27a, /// 0xb74
			 0xc891e039, /// 0xb78
			 0x262a9780, /// 0xb7c
			 0x0386cb29, /// 0xb80
			 0x01d29685, /// 0xb84
			 0xcfa5b320, /// 0xb88
			 0x3a51ff29, /// 0xb8c
			 0x04bd53e5, /// 0xb90
			 0xb2b06d06, /// 0xb94
			 0x775e4928, /// 0xb98
			 0x98e7d0c8, /// 0xb9c
			 0x33528589, /// 0xba0
			 0xaf5f075f, /// 0xba4
			 0x6097f978, /// 0xba8
			 0xda415b3a, /// 0xbac
			 0xe728bf99, /// 0xbb0
			 0x2eb2e80c, /// 0xbb4
			 0x8574064e, /// 0xbb8
			 0xa7efd62e, /// 0xbbc
			 0xdec6e21b, /// 0xbc0
			 0xcc384049, /// 0xbc4
			 0x4f8f620a, /// 0xbc8
			 0x0a151d51, /// 0xbcc
			 0xb7c594fe, /// 0xbd0
			 0xd548d77e, /// 0xbd4
			 0x94d51a0d, /// 0xbd8
			 0xef99d016, /// 0xbdc
			 0xebd2bbd5, /// 0xbe0
			 0x1266933d, /// 0xbe4
			 0xd58ddbf6, /// 0xbe8
			 0xff27caf2, /// 0xbec
			 0xeae19a17, /// 0xbf0
			 0xe66a2b38, /// 0xbf4
			 0xd1eff3e1, /// 0xbf8
			 0xad3d4e47, /// 0xbfc
			 0x8ff35b1d, /// 0xc00
			 0xa8dc4709, /// 0xc04
			 0x654a66e0, /// 0xc08
			 0x489f301a, /// 0xc0c
			 0x0efeb5d1, /// 0xc10
			 0x24aac16f, /// 0xc14
			 0xd06fca7a, /// 0xc18
			 0x4705203a, /// 0xc1c
			 0xe2b541dd, /// 0xc20
			 0x24ca5db4, /// 0xc24
			 0x2836442b, /// 0xc28
			 0x5912b5e4, /// 0xc2c
			 0x5aeb45cb, /// 0xc30
			 0x3ab0ee43, /// 0xc34
			 0x74d26faf, /// 0xc38
			 0x5495752a, /// 0xc3c
			 0x158fc4bc, /// 0xc40
			 0x633f4763, /// 0xc44
			 0x5f599368, /// 0xc48
			 0x068f6ef8, /// 0xc4c
			 0x491ad460, /// 0xc50
			 0xa9e28425, /// 0xc54
			 0xacdfb996, /// 0xc58
			 0x32e47a7b, /// 0xc5c
			 0xee08a7fe, /// 0xc60
			 0xe08bbb8a, /// 0xc64
			 0xb646d058, /// 0xc68
			 0x3f4e6ee7, /// 0xc6c
			 0x0157d589, /// 0xc70
			 0xd9bf1d44, /// 0xc74
			 0x2cef1d72, /// 0xc78
			 0x064879bc, /// 0xc7c
			 0xd64eb884, /// 0xc80
			 0x44fdd24f, /// 0xc84
			 0xcdfdb0f7, /// 0xc88
			 0x8b06c925, /// 0xc8c
			 0x128a1ac4, /// 0xc90
			 0xdf7c40ff, /// 0xc94
			 0x1cfef91f, /// 0xc98
			 0x18932684, /// 0xc9c
			 0x56380d2c, /// 0xca0
			 0x553341f7, /// 0xca4
			 0x5c7dbaf3, /// 0xca8
			 0xb9af2517, /// 0xcac
			 0x4da4c61d, /// 0xcb0
			 0x7f3b5151, /// 0xcb4
			 0x49ff6eb9, /// 0xcb8
			 0x78fb3bec, /// 0xcbc
			 0x08d28c31, /// 0xcc0
			 0xbbbb473b, /// 0xcc4
			 0x0fea3ad4, /// 0xcc8
			 0xb9a6e188, /// 0xccc
			 0x264fc7eb, /// 0xcd0
			 0x5284d05a, /// 0xcd4
			 0xc99d47d3, /// 0xcd8
			 0xdd5b5068, /// 0xcdc
			 0xdb95ffbf, /// 0xce0
			 0xc387cbd8, /// 0xce4
			 0x0f5156c4, /// 0xce8
			 0xb74203d3, /// 0xcec
			 0x2f3f5a98, /// 0xcf0
			 0x32328162, /// 0xcf4
			 0x25e50198, /// 0xcf8
			 0x05e1921c, /// 0xcfc
			 0xa454e0f4, /// 0xd00
			 0x13c5a7be, /// 0xd04
			 0xf9257f24, /// 0xd08
			 0xaefdb472, /// 0xd0c
			 0x3aa17dd6, /// 0xd10
			 0xb51fc913, /// 0xd14
			 0x035cac17, /// 0xd18
			 0xa1162be5, /// 0xd1c
			 0x7ce9cc51, /// 0xd20
			 0x7dea2219, /// 0xd24
			 0x6bc28d40, /// 0xd28
			 0x81fd97fd, /// 0xd2c
			 0x338481fa, /// 0xd30
			 0xca74bf1c, /// 0xd34
			 0xa7e9a811, /// 0xd38
			 0xf6806de1, /// 0xd3c
			 0x4f0ad152, /// 0xd40
			 0xc7c15a64, /// 0xd44
			 0x6f562556, /// 0xd48
			 0xb6908c6c, /// 0xd4c
			 0x567c4066, /// 0xd50
			 0xf97601da, /// 0xd54
			 0xd242865e, /// 0xd58
			 0x275d0539, /// 0xd5c
			 0xc761b04d, /// 0xd60
			 0x46ec8535, /// 0xd64
			 0x629397eb, /// 0xd68
			 0x36c40f33, /// 0xd6c
			 0x1435919e, /// 0xd70
			 0x89f0b2db, /// 0xd74
			 0xf31290bd, /// 0xd78
			 0x15a8cdb9, /// 0xd7c
			 0x20d0f927, /// 0xd80
			 0xc1b8dcae, /// 0xd84
			 0x87e612d4, /// 0xd88
			 0x1596de2b, /// 0xd8c
			 0x8b95736a, /// 0xd90
			 0x0a348302, /// 0xd94
			 0x676cc370, /// 0xd98
			 0x149dfc34, /// 0xd9c
			 0x494398fd, /// 0xda0
			 0x04e0d309, /// 0xda4
			 0x25142c87, /// 0xda8
			 0xc0b7af47, /// 0xdac
			 0x2215b2e3, /// 0xdb0
			 0xbb5d2047, /// 0xdb4
			 0xec299b17, /// 0xdb8
			 0x53857a9a, /// 0xdbc
			 0xcf894740, /// 0xdc0
			 0x7aa6927d, /// 0xdc4
			 0xab2ed616, /// 0xdc8
			 0xcba30cb1, /// 0xdcc
			 0xde78849d, /// 0xdd0
			 0x3c5d7ded, /// 0xdd4
			 0xdfac0ae6, /// 0xdd8
			 0xbec7796e, /// 0xddc
			 0x87f33975, /// 0xde0
			 0x22143308, /// 0xde4
			 0xe4209fa8, /// 0xde8
			 0xa0b90e65, /// 0xdec
			 0x960b11e3, /// 0xdf0
			 0x125a5b99, /// 0xdf4
			 0xc0d1257c, /// 0xdf8
			 0xbd968da2, /// 0xdfc
			 0xedb4afc6, /// 0xe00
			 0x91e8c04b, /// 0xe04
			 0x498262f9, /// 0xe08
			 0x0115333c, /// 0xe0c
			 0xe64f9e70, /// 0xe10
			 0x22e0e251, /// 0xe14
			 0xcd087c5d, /// 0xe18
			 0x98500c0a, /// 0xe1c
			 0x5dde3043, /// 0xe20
			 0xd62f8cf0, /// 0xe24
			 0xdaaca2ac, /// 0xe28
			 0x24096a64, /// 0xe2c
			 0x203ad54f, /// 0xe30
			 0x2d6c3ca7, /// 0xe34
			 0xce3403c9, /// 0xe38
			 0xa9be72c1, /// 0xe3c
			 0x6fc82ce3, /// 0xe40
			 0x35c49628, /// 0xe44
			 0x4eb03f69, /// 0xe48
			 0x9c09d260, /// 0xe4c
			 0x868eba78, /// 0xe50
			 0x4c37c150, /// 0xe54
			 0x8f2c4383, /// 0xe58
			 0x2f12beab, /// 0xe5c
			 0x2c182df5, /// 0xe60
			 0x32c69e81, /// 0xe64
			 0xd4879c31, /// 0xe68
			 0xa8fd921b, /// 0xe6c
			 0x5be8f283, /// 0xe70
			 0x82cea13c, /// 0xe74
			 0xef6d0380, /// 0xe78
			 0xa2bf071a, /// 0xe7c
			 0x46128646, /// 0xe80
			 0xd86ada5d, /// 0xe84
			 0xb76283a1, /// 0xe88
			 0x697d72ed, /// 0xe8c
			 0xb2af0cb3, /// 0xe90
			 0xbcc6f589, /// 0xe94
			 0x52b125e4, /// 0xe98
			 0x16fdedbc, /// 0xe9c
			 0xfd6fadc0, /// 0xea0
			 0x1d4e5950, /// 0xea4
			 0xe62982fa, /// 0xea8
			 0x796dec14, /// 0xeac
			 0x69b0a5c3, /// 0xeb0
			 0x22e767aa, /// 0xeb4
			 0x725a8bd9, /// 0xeb8
			 0x7f5488a7, /// 0xebc
			 0xaa6640c3, /// 0xec0
			 0x50fa074c, /// 0xec4
			 0x3cc28dfc, /// 0xec8
			 0xd0e16ad8, /// 0xecc
			 0xa9ee3dd1, /// 0xed0
			 0x46ce1430, /// 0xed4
			 0x68b7b033, /// 0xed8
			 0x88a919ab, /// 0xedc
			 0xfc620237, /// 0xee0
			 0x08f2da3d, /// 0xee4
			 0x0765020b, /// 0xee8
			 0xf552a4b1, /// 0xeec
			 0x6184e784, /// 0xef0
			 0x050aa8df, /// 0xef4
			 0xe0efdddf, /// 0xef8
			 0xe9bb1f62, /// 0xefc
			 0x8fba4fa8, /// 0xf00
			 0xe091d1ba, /// 0xf04
			 0xedee64ef, /// 0xf08
			 0x9d6a6649, /// 0xf0c
			 0x33b83df9, /// 0xf10
			 0x40712ca5, /// 0xf14
			 0x00c6b3ee, /// 0xf18
			 0x5d171809, /// 0xf1c
			 0xad40b792, /// 0xf20
			 0x7f0ef43d, /// 0xf24
			 0xb0489860, /// 0xf28
			 0x519cc8e7, /// 0xf2c
			 0x82469fc6, /// 0xf30
			 0xd34cce60, /// 0xf34
			 0xfeddb2ff, /// 0xf38
			 0x84201ae8, /// 0xf3c
			 0x7a8ce862, /// 0xf40
			 0xf52c9389, /// 0xf44
			 0xc7356d51, /// 0xf48
			 0x4b37aa6d, /// 0xf4c
			 0x4f01383d, /// 0xf50
			 0x4d2b7734, /// 0xf54
			 0xbe4466e7, /// 0xf58
			 0x0bd9711b, /// 0xf5c
			 0x0ab004fb, /// 0xf60
			 0xc4e51579, /// 0xf64
			 0xfd5768e8, /// 0xf68
			 0x41b685b1, /// 0xf6c
			 0x3613455f, /// 0xf70
			 0xd7aa9020, /// 0xf74
			 0xab3ea09e, /// 0xf78
			 0x6e2b744f, /// 0xf7c
			 0x8186d5cc, /// 0xf80
			 0xcf89112b, /// 0xf84
			 0xa67141d0, /// 0xf88
			 0x05a20e48, /// 0xf8c
			 0xc0780566, /// 0xf90
			 0x15269bd8, /// 0xf94
			 0xfc062f49, /// 0xf98
			 0x2b25f598, /// 0xf9c
			 0x94da4b71, /// 0xfa0
			 0x41245f84, /// 0xfa4
			 0x6f4d0869, /// 0xfa8
			 0xdb8da86c, /// 0xfac
			 0x09e218d4, /// 0xfb0
			 0xaf122e27, /// 0xfb4
			 0x3f971db0, /// 0xfb8
			 0xf365ee8b, /// 0xfbc
			 0x68e14b37, /// 0xfc0
			 0x8cbafff3, /// 0xfc4
			 0xa57507cc, /// 0xfc8
			 0xe26af7cd, /// 0xfcc
			 0x725fedad, /// 0xfd0
			 0xad892926, /// 0xfd4
			 0x67e7fba6, /// 0xfd8
			 0x41a7a158, /// 0xfdc
			 0x4487a1ad, /// 0xfe0
			 0xef15e078, /// 0xfe4
			 0x0d9d34b7, /// 0xfe8
			 0xe6f44859, /// 0xfec
			 0xbd442115, /// 0xff0
			 0xe17f050f, /// 0xff4
			 0x7e840a8e, /// 0xff8
			 0x37427a8b /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00000
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00004
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00008
			 0x0e000007,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00010
			 0x0c600000,                                                  // Leading 1s:                                 /// 00014
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0001c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00024
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00030
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e000001,                                                  // Trailing 1s:                                /// 0003c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00048
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0004c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00054
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00058
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00060
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00068
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x0c780000,                                                  // Leading 1s:                                 /// 00074
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0007c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80011111,                                                  // -9.7958E-41                                 /// 00088
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00090
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00094
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00098
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0009c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x3f028f5c,                                                  // 0.51                                        /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xcb000000,                                                  // -8388608.0                                  /// 000e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00100
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00104
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00108
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00110
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00114
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00118
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0011c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00120
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00124
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00130
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00134
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00138
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00144
			 0x80011111,                                                  // -9.7958E-41                                 /// 00148
			 0xbf028f5c,                                                  // -0.51                                       /// 0014c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00158
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0015c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00160
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xffc00001,                                                  // -signaling NaN                              /// 00168
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0016c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00170
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00174
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0017c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0018c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0019c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 001b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00208
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0020c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00210
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00220
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00228
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0022c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00230
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00234
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00238
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x80000000,                                                  // -zero                                       /// 00240
			 0x0e000007,                                                  // Trailing 1s:                                /// 00244
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00248
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00250
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00268
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0026c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0027c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00280
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00284
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00288
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0028c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00290
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00294
			 0xbf028f5c,                                                  // -0.51                                       /// 00298
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0029c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002a0
			 0xff800000,                                                  // -inf                                        /// 002a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002cc
			 0x3f028f5c,                                                  // 0.51                                        /// 002d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00300
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00304
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00308
			 0x80011111,                                                  // -9.7958E-41                                 /// 0030c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00314
			 0x0c700000,                                                  // Leading 1s:                                 /// 00318
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0031c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00320
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00328
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0032c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00330
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00334
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x55555555,                                                  // 4 random values                             /// 00340
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00344
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00350
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00358
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0035c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00360
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00364
			 0xcb000000,                                                  // -8388608.0                                  /// 00368
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0036c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00370
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00374
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00378
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0037c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00380
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00384
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00388
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0038c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00390
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00394
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003d8
			 0x7fc00001,                                                  // signaling NaN                               /// 003dc
			 0x00000000,                                                  // zero                                        /// 003e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00404
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00408
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80011111,                                                  // -9.7958E-41                                 /// 00414
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0041c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00424
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00428
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0042c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00430
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00434
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00438
			 0x3f028f5c,                                                  // 0.51                                        /// 0043c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00440
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00444
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00450
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00454
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0045c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00460
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00464
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00468
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0046c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00474
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00478
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0047c
			 0xbf028f5c,                                                  // -0.51                                       /// 00480
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00484
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00488
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0x0e000003,                                                  // Trailing 1s:                                /// 00494
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00498
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00500
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00508
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0050c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00514
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00518
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0051c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00524
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00528
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00530
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00534
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00538
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0053c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00540
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00544
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00548
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00558
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00568
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0056c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00570
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00574
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00580
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00584
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00588
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0058c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00594
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005a0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005cc
			 0x55555555,                                                  // 4 random values                             /// 005d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d8
			 0xbf028f5c,                                                  // -0.51                                       /// 005dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00610
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00618
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0061c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00620
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0062c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00630
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0063c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00640
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00644
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00648
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0064c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00654
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0065c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00660
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00664
			 0x0c780000,                                                  // Leading 1s:                                 /// 00668
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00670
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00680
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00684
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0xff800000,                                                  // -inf                                        /// 0068c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00698
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0xffc00001,                                                  // -signaling NaN                              /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00708
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00710
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00720
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00728
			 0xff800000,                                                  // -inf                                        /// 0072c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00730
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00734
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00738
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0073c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00740
			 0x0c600000,                                                  // Leading 1s:                                 /// 00744
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00750
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00754
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00758
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0075c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00764
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00768
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0076c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00770
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00774
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00780
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00784
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00788
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0078c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00790
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00798
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007f4
			 0xbf028f5c,                                                  // -0.51                                       /// 007f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00800
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00804
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00808
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00810
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00814
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0081c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00820
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00824
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00828
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0082c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00830
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00834
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0083c
			 0x55555555,                                                  // 4 random values                             /// 00840
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00844
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00854
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00858
			 0xffc00001,                                                  // -signaling NaN                              /// 0085c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00860
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00864
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00868
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00870
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x0c700000,                                                  // Leading 1s:                                 /// 00878
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0087c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00880
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00884
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00888
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0088c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00890
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00894
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x7fc00001,                                                  // signaling NaN                               /// 008b4
			 0xcb000000,                                                  // -8388608.0                                  /// 008b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008bc
			 0x3f028f5c,                                                  // 0.51                                        /// 008c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c4
			 0x7f800000,                                                  // inf                                         /// 008c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008cc
			 0xbf028f5c,                                                  // -0.51                                       /// 008d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00900
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x33333333,                                                  // 4 random values                             /// 00908
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00914
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00918
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00924
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00928
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00938
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00940
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00944
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00948
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0094c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00954
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0095c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00960
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00964
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00970
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00980
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00988
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00990
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00994
			 0xbf028f5c,                                                  // -0.51                                       /// 00998
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0099c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a3c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a40
			 0x3f028f5c,                                                  // 0.51                                        /// 00a44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x7f800000,                                                  // inf                                         /// 00a58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a74
			 0xff800000,                                                  // -inf                                        /// 00a78
			 0xcb000000,                                                  // -8388608.0                                  /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a90
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a94
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aa8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ab4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ab8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00abc
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ac4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ac8
			 0xcb000000,                                                  // -8388608.0                                  /// 00acc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00adc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ae8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00af8
			 0xbf028f5c,                                                  // -0.51                                       /// 00afc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b00
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b10
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b38
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ba0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba4
			 0xff800000,                                                  // -inf                                        /// 00ba8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bfc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c00
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c1c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c30
			 0xbf028f5c,                                                  // -0.51                                       /// 00c34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x00000000,                                                  // zero                                        /// 00c70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ca0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cc8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ccc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x33333333,                                                  // 4 random values                             /// 00cd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cdc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ce8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x80000000,                                                  // -zero                                       /// 00cf8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cfc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d14
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d20
			 0x7fc00001,                                                  // signaling NaN                               /// 00d24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d4c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d50
			 0xff800000,                                                  // -inf                                        /// 00d54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d74
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d80
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d84
			 0xcb000000,                                                  // -8388608.0                                  /// 00d88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00da4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00db4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00dc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dd0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ddc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00de8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00df0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00df4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e04
			 0x33333333,                                                  // 4 random values                             /// 00e08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e18
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e5c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e8c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ea0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eb0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00eb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ec0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ec8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ecc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ed4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00edc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ef0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ef8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000000,                                                  // -zero                                       /// 00f98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0xff800000,                                                  // -inf                                        /// 00fa8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fb8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fc4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fec
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00000080                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x000006a4,
			 0x0000044c,
			 0x0000047c,
			 0x00000780,
			 0x00000208,
			 0x0000028c,
			 0x000001d8,
			 0x0000030c,

			 /// vpu register f2
			 0x41f00000,
			 0x41e80000,
			 0x41700000,
			 0x41a80000,
			 0x41100000,
			 0x41000000,
			 0x40c00000,
			 0x41c80000,

			 /// vpu register f3
			 0x41c80000,
			 0x41a80000,
			 0x41e80000,
			 0x41000000,
			 0x41a80000,
			 0x41f00000,
			 0x41000000,
			 0x40a00000,

			 /// vpu register f4
			 0x41500000,
			 0x41a80000,
			 0x41f80000,
			 0x41200000,
			 0x41300000,
			 0x42000000,
			 0x3f800000,
			 0x41d00000,

			 /// vpu register f5
			 0x41e00000,
			 0x41300000,
			 0x41300000,
			 0x41900000,
			 0x41d00000,
			 0x41500000,
			 0x40c00000,
			 0x41880000,

			 /// vpu register f6
			 0x40000000,
			 0x41e00000,
			 0x41400000,
			 0x41c80000,
			 0x41980000,
			 0x41200000,
			 0x41a80000,
			 0x40c00000,

			 /// vpu register f7
			 0x3f800000,
			 0x41e00000,
			 0x40e00000,
			 0x41b80000,
			 0x41100000,
			 0x41000000,
			 0x41d00000,
			 0x40800000,

			 /// vpu register f8
			 0x41100000,
			 0x41980000,
			 0x41e00000,
			 0x3f800000,
			 0x40c00000,
			 0x41a00000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f9
			 0x41980000,
			 0x41980000,
			 0x40a00000,
			 0x3f800000,
			 0x40c00000,
			 0x41a00000,
			 0x41e00000,
			 0x41880000,

			 /// vpu register f10
			 0x41880000,
			 0x41c80000,
			 0x41e00000,
			 0x41b00000,
			 0x41e00000,
			 0x41c00000,
			 0x42000000,
			 0x41000000,

			 /// vpu register f11
			 0x41b80000,
			 0x40800000,
			 0x42000000,
			 0x40800000,
			 0x41400000,
			 0x40a00000,
			 0x41d00000,
			 0x40c00000,

			 /// vpu register f12
			 0x41a80000,
			 0x41300000,
			 0x41b80000,
			 0x41b80000,
			 0x40000000,
			 0x41880000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f13
			 0x41b00000,
			 0x41700000,
			 0x41e80000,
			 0x40a00000,
			 0x41900000,
			 0x41c00000,
			 0x41d80000,
			 0x41d80000,

			 /// vpu register f14
			 0x41c00000,
			 0x40000000,
			 0x42000000,
			 0x41880000,
			 0x3f800000,
			 0x41c80000,
			 0x41900000,
			 0x40000000,

			 /// vpu register f15
			 0x41d00000,
			 0x40000000,
			 0x41a80000,
			 0x40800000,
			 0x41b00000,
			 0x41d80000,
			 0x41880000,
			 0x40000000,

			 /// vpu register f16
			 0x41300000,
			 0x41900000,
			 0x41880000,
			 0x41900000,
			 0x41500000,
			 0x41d80000,
			 0x41b80000,
			 0x41980000,

			 /// vpu register f17
			 0x41980000,
			 0x41d00000,
			 0x41600000,
			 0x42000000,
			 0x41000000,
			 0x40c00000,
			 0x41a80000,
			 0x41980000,

			 /// vpu register f18
			 0x41e00000,
			 0x40000000,
			 0x41400000,
			 0x41d80000,
			 0x40400000,
			 0x40800000,
			 0x40e00000,
			 0x41b80000,

			 /// vpu register f19
			 0x40c00000,
			 0x41c00000,
			 0x41500000,
			 0x41980000,
			 0x41f80000,
			 0x41880000,
			 0x41c00000,
			 0x40e00000,

			 /// vpu register f20
			 0x41800000,
			 0x41300000,
			 0x41900000,
			 0x3f800000,
			 0x42000000,
			 0x41300000,
			 0x41500000,
			 0x42000000,

			 /// vpu register f21
			 0x41000000,
			 0x41100000,
			 0x41a00000,
			 0x40a00000,
			 0x40000000,
			 0x41100000,
			 0x41000000,
			 0x40e00000,

			 /// vpu register f22
			 0x41600000,
			 0x41c00000,
			 0x41400000,
			 0x41900000,
			 0x40c00000,
			 0x3f800000,
			 0x41880000,
			 0x41e80000,

			 /// vpu register f23
			 0x41880000,
			 0x41700000,
			 0x41100000,
			 0x41800000,
			 0x42000000,
			 0x41700000,
			 0x41b00000,
			 0x41100000,

			 /// vpu register f24
			 0x41400000,
			 0x41e00000,
			 0x40800000,
			 0x41400000,
			 0x41a00000,
			 0x41900000,
			 0x41d00000,
			 0x41500000,

			 /// vpu register f25
			 0x41400000,
			 0x41600000,
			 0x41c00000,
			 0x41b80000,
			 0x41880000,
			 0x40a00000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f26
			 0x41f00000,
			 0x41800000,
			 0x41b00000,
			 0x41c00000,
			 0x41000000,
			 0x40400000,
			 0x41b00000,
			 0x41c00000,

			 /// vpu register f27
			 0x41e00000,
			 0x40a00000,
			 0x41100000,
			 0x41900000,
			 0x41900000,
			 0x41300000,
			 0x41100000,
			 0x40000000,

			 /// vpu register f28
			 0x41d80000,
			 0x41800000,
			 0x41800000,
			 0x41a00000,
			 0x41a00000,
			 0x40a00000,
			 0x41b00000,
			 0x41c00000,

			 /// vpu register f29
			 0x40e00000,
			 0x41a80000,
			 0x41900000,
			 0x41600000,
			 0x41300000,
			 0x41f00000,
			 0x41d00000,
			 0x41b80000,

			 /// vpu register f30
			 0x41e80000,
			 0x41a00000,
			 0x41600000,
			 0x42000000,
			 0x41880000,
			 0x41a80000,
			 0x41b00000,
			 0x41c00000,

			 /// vpu register f31
			 0x41a80000,
			 0x41e80000,
			 0x41c00000,
			 0x41300000,
			 0x41d80000,
			 0x41b00000,
			 0x42000000,
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
		"fsgnjn.s f13, f18, f12\n"                            ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f8, f23, f0\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f30, f27, f19\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f18, f4, f19\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f2, f5, f28\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f11, f25, f12\n"                            ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f23, f27, f2\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f2, f15, f18\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f20, f30, f1\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f22, f7, f0\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f8, f22, f30\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f13, f29, f13\n"                            ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f12, f4, f8\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f20, f6, f10\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f26, f15, f21\n"                            ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f30, f21, f4\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f22, f10, f2\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f7, f14, f1\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f17, f23, f26\n"                            ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f3, f12, f19\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f28, f5, f7\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f21, f10, f29\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f4, f12, f6\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f28, f23, f10\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f18, f12, f17\n"                            ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f29, f9, f1\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f2, f10, f7\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f14, f12, f2\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f30, f10, f23\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f4, f17, f13\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f23, f2, f23\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f18, f30, f21\n"                            ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f13, f0, f29\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f14, f23, f6\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f14, f3, f13\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f30, f5, f15\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f28, f5, f14\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f2, f23, f30\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f20, f12, f1\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f5, f12, f15\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f13, f14, f4\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f6, f6, f24\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f27, f24, f5\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f29, f17, f13\n"                            ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f25, f29, f15\n"                            ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f18, f26, f15\n"                            ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f26, f30, f11\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f7, f15, f27\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f29, f11, f13\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f7, f15, f23\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f16, f10, f15\n"                            ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f19, f20, f9\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f13, f27, f7\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f23, f15, f5\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f10, f4, f0\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f27, f12, f30\n"                            ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f21, f16, f28\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f21, f27, f8\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f12, f23, f6\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f4, f8, f11\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f26, f27, f10\n"                            ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f3, f21, f13\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f5, f14, f30\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f14, f14, f23\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f24, f0, f8\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f17, f16, f16\n"                            ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f25, f8, f2\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f8, f25, f2\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f2, f11, f19\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f26, f24, f30\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f23, f26, f8\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f8, f2, f10\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f11, f23, f23\n"                            ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f3, f24, f19\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f19, f13, f2\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f6, f21, f21\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f14, f0, f20\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f6, f27, f16\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f25, f18, f20\n"                            ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f30, f14, f25\n"                            ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f10, f26, f20\n"                            ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f3, f11, f0\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f27, f29, f22\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f16, f26, f9\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f30, f0, f4\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f19, f6, f30\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f29, f26, f1\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f16, f16, f2\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f17, f14, f22\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f11, f9, f7\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f20, f9, f29\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f25, f22, f4\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f12, f17, f6\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f20, f22, f13\n"                            ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f13, f22, f20\n"                            ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f14, f28, f22\n"                            ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f28, f5, f2\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f12, f8, f19\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsgnjn.s f2, f6, f12\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
