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
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0c780000,                                                  // Leading 1s:                                 /// 00004
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00008
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0000c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00014
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00024
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x4b000000,                                                  // 8388608.0                                   /// 0002c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00034
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00040
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00044
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00048
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0004c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00050
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00054
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00058
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0005c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00064
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0006c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00070
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00078
			 0x55555555,                                                  // 4 random values                             /// 0007c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00080
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00084
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0008c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00094
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00098
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0009c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000a0
			 0x00000000,                                                  // zero                                        /// 000a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000ec
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00100
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00110
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00114
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0011c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00120
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00124
			 0x0e000007,                                                  // Trailing 1s:                                /// 00128
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0012c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00130
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00134
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0013c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00140
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00144
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0014c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00154
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00158
			 0x4b000000,                                                  // 8388608.0                                   /// 0015c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00160
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x33333333,                                                  // 4 random values                             /// 0016c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00170
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00174
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x80011111,                                                  // -9.7958E-41                                 /// 0017c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00180
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00188
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00190
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00198
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0019c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e0
			 0xff800000,                                                  // -inf                                        /// 001e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001f4
			 0xff800000,                                                  // -inf                                        /// 001f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00200
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00204
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0020c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00210
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00214
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00218
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0021c
			 0x33333333,                                                  // 4 random values                             /// 00220
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00224
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00234
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0023c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00240
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00244
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0024c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x0e000001,                                                  // Trailing 1s:                                /// 00254
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x4b000000,                                                  // 8388608.0                                   /// 0025c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00260
			 0xcb000000,                                                  // -8388608.0                                  /// 00264
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0026c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00270
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00278
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00280
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00284
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00288
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00294
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0029c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x7fc00001,                                                  // signaling NaN                               /// 002b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002fc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00304
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0030c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0031c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00320
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00324
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00334
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0033c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00340
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00344
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00348
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0034c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00350
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00354
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00358
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0035c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00360
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00364
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00370
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00378
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0037c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00380
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00390
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00394
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00398
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0039c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00404
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00408
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00410
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0041c
			 0x3f028f5c,                                                  // 0.51                                        /// 00420
			 0x00011111,                                                  // 9.7958E-41                                  /// 00424
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00428
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0042c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00430
			 0xffc00001,                                                  // -signaling NaN                              /// 00434
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00438
			 0x0c700000,                                                  // Leading 1s:                                 /// 0043c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0044c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00454
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00460
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00468
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0046c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0e000007,                                                  // Trailing 1s:                                /// 00474
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0047c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00480
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00484
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00490
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00494
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00498
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0049c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0050c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00510
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00518
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00520
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00524
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00528
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00538
			 0x7fc00001,                                                  // signaling NaN                               /// 0053c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00550
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00554
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00558
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0055c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0xcb000000,                                                  // -8388608.0                                  /// 00564
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00568
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0056c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00570
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00574
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0057c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00580
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00584
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00588
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0058c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00594
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0c700000,                                                  // Leading 1s:                                 /// 0059c
			 0xffc00001,                                                  // -signaling NaN                              /// 005a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0xffc00001,                                                  // -signaling NaN                              /// 005e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x4b000000,                                                  // 8388608.0                                   /// 005f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00604
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00608
			 0x55555555,                                                  // 4 random values                             /// 0060c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00610
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00614
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00618
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0061c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00620
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00630
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0e000007,                                                  // Trailing 1s:                                /// 00638
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0063c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00640
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00644
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00648
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00650
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0xff800000,                                                  // -inf                                        /// 00658
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00660
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00664
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00668
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00670
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0067c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00680
			 0x80000000,                                                  // -zero                                       /// 00684
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00688
			 0x7f800000,                                                  // inf                                         /// 0068c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00690
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00698
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0069c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 006e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e8
			 0x00000000,                                                  // zero                                        /// 006ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00708
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00710
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00714
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00718
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00724
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00728
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00738
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00740
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00744
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00748
			 0x00011111,                                                  // 9.7958E-41                                  /// 0074c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00750
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00754
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00758
			 0x80011111,                                                  // -9.7958E-41                                 /// 0075c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00768
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0076c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00770
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00774
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00778
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0077c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00784
			 0xffc00001,                                                  // -signaling NaN                              /// 00788
			 0xbf028f5c,                                                  // -0.51                                       /// 0078c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00790
			 0x00011111,                                                  // 9.7958E-41                                  /// 00794
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00798
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007dc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007f4
			 0x55555555,                                                  // 4 random values                             /// 007f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00800
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00804
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00808
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0080c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00810
			 0x00000000,                                                  // zero                                        /// 00814
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0081c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00824
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0082c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00838
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0083c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00844
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00850
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00858
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00860
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00864
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00874
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00878
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0087c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00880
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00884
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0088c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00890
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00894
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00898
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000000,                                                  // -zero                                       /// 008c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00900
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00904
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00910
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00914
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00918
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00924
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00000000,                                                  // zero                                        /// 0092c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00930
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0094c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0095c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00960
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00968
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00974
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x3f028f5c,                                                  // 0.51                                        /// 0097c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00980
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00984
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00990
			 0xbf028f5c,                                                  // -0.51                                       /// 00994
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00998
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0099c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00000000,                                                  // zero                                        /// 009fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a04
			 0xcb000000,                                                  // -8388608.0                                  /// 00a08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x00000000,                                                  // zero                                        /// 00a40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7f800000,                                                  // inf                                         /// 00a5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a74
			 0x4b000000,                                                  // 8388608.0                                   /// 00a78
			 0x80000000,                                                  // -zero                                       /// 00a7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a84
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ab4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00abc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ac4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00acc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ad0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad8
			 0x80000000,                                                  // -zero                                       /// 00adc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00af0
			 0xbf028f5c,                                                  // -0.51                                       /// 00af4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00afc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0xbf028f5c,                                                  // -0.51                                       /// 00b08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x3f028f5c,                                                  // 0.51                                        /// 00b18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b30
			 0xcb000000,                                                  // -8388608.0                                  /// 00b34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b40
			 0x7f800000,                                                  // inf                                         /// 00b44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7f800000,                                                  // inf                                         /// 00b4c
			 0xff800000,                                                  // -inf                                        /// 00b50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xcb000000,                                                  // -8388608.0                                  /// 00b64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b94
			 0x3f028f5c,                                                  // 0.51                                        /// 00b98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x55555555,                                                  // 4 random values                             /// 00ba4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ba8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bc0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x3f028f5c,                                                  // 0.51                                        /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bd8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bfc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c68
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c80
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c90
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ca0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ca4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cdc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ce4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cf8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cfc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d08
			 0x00000000,                                                  // zero                                        /// 00d0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d10
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x7f800000,                                                  // inf                                         /// 00d1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d34
			 0x7fc00001,                                                  // signaling NaN                               /// 00d38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00da4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00db8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dc0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dc4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ddc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00de0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00df4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e20
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x55555555,                                                  // 4 random values                             /// 00e3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e50
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00e70
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0xffc00001,                                                  // -signaling NaN                              /// 00e9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ea8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00eb4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eb8
			 0xff800000,                                                  // -inf                                        /// 00ebc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ecc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00edc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ee8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ef0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x55555555,                                                  // 4 random values                             /// 00efc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f0c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f24
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f38
			 0x4b000000,                                                  // 8388608.0                                   /// 00f3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f40
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f44
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f4c
			 0x7f800000,                                                  // inf                                         /// 00f50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f70
			 0x55555555,                                                  // 4 random values                             /// 00f74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f84
			 0xcb000000,                                                  // -8388608.0                                  /// 00f88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fbc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fc0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fcc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fd8
			 0x7f800000,                                                  // inf                                         /// 00fdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fe4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fe8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ff0
			 0x00000000,                                                  // zero                                        /// 00ff4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff8
			 0x00000200                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00000
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00008
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0000c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00010
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00014
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00018
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00020
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00030
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00034
			 0x33333333,                                                  // 4 random values                             /// 00038
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0003c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00040
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00044
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00048
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00054
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00058
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00068
			 0x80011111,                                                  // -9.7958E-41                                 /// 0006c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00078
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00080
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00088
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00090
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00094
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0009c
			 0x0c600000,                                                  // Leading 1s:                                 /// 000a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x33333333,                                                  // 4 random values                             /// 000dc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00100
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00104
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00108
			 0x55555555,                                                  // 4 random values                             /// 0010c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00114
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00118
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0011c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00120
			 0xcb000000,                                                  // -8388608.0                                  /// 00124
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00128
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00130
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00134
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00138
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0013c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00140
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00144
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0014c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00154
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0015c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00160
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00164
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0016c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00170
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00174
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0017c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00190
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00194
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001ac
			 0x3f028f5c,                                                  // 0.51                                        /// 001b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d0
			 0x7f800000,                                                  // inf                                         /// 001d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e4
			 0x4b000000,                                                  // 8388608.0                                   /// 001e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00200
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00204
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00208
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0020c
			 0xcb000000,                                                  // -8388608.0                                  /// 00210
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00218
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0021c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00220
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00224
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00230
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00234
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00238
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00244
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00248
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0024c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00250
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00258
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0025c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00260
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x7fc00001,                                                  // signaling NaN                               /// 00268
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0026c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00270
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00274
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0027c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00280
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00284
			 0x0c700000,                                                  // Leading 1s:                                 /// 00288
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0028c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00290
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00294
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0029c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 002d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x33333333,                                                  // 4 random values                             /// 002e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00300
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00304
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00308
			 0x4b000000,                                                  // 8388608.0                                   /// 0030c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00314
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0031c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00320
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00328
			 0x7f800000,                                                  // inf                                         /// 0032c
			 0x80000000,                                                  // -zero                                       /// 00330
			 0xbf028f5c,                                                  // -0.51                                       /// 00334
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0033c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x3f028f5c,                                                  // 0.51                                        /// 00344
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00348
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0034c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0035c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00360
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00368
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0036c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00374
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00378
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00384
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0038c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00390
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00394
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00398
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0039c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0xcb000000,                                                  // -8388608.0                                  /// 003d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00400
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00408
			 0x0e000001,                                                  // Trailing 1s:                                /// 0040c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0041c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00420
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00424
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00428
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0042c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00430
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00434
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00438
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0043c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00440
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00444
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00448
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0044c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00458
			 0xbf028f5c,                                                  // -0.51                                       /// 0045c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00464
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00468
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00474
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00478
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00480
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00488
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x4b000000,                                                  // 8388608.0                                   /// 004c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x55555555,                                                  // 4 random values                             /// 004d4
			 0x3f028f5c,                                                  // 0.51                                        /// 004d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f8
			 0xffc00001,                                                  // -signaling NaN                              /// 004fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00500
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00504
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00508
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0050c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00510
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00514
			 0x0c700000,                                                  // Leading 1s:                                 /// 00518
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00520
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00524
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00528
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0c600000,                                                  // Leading 1s:                                 /// 00534
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0053c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00540
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00544
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0054c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00550
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00558
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00560
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0xffc00001,                                                  // -signaling NaN                              /// 00568
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0056c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00570
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00578
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0057c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00580
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00584
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00588
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00590
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00594
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00600
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80011111,                                                  // -9.7958E-41                                 /// 00608
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00614
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00618
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0061c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00624
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00628
			 0x0e000003,                                                  // Trailing 1s:                                /// 0062c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00630
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00640
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0064c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00650
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00654
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00658
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00664
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0066c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00670
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00674
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00678
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0067c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00684
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00688
			 0x0e000003,                                                  // Trailing 1s:                                /// 0068c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00690
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00698
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0069c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a8
			 0x80000000,                                                  // -zero                                       /// 006ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x55555555,                                                  // 4 random values                             /// 006ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00700
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0070c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00714
			 0xffc00001,                                                  // -signaling NaN                              /// 00718
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00720
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0072c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00734
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00738
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0073c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00740
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00744
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00754
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00758
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0075c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00760
			 0x7fc00001,                                                  // signaling NaN                               /// 00764
			 0x0e000007,                                                  // Trailing 1s:                                /// 00768
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00774
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00778
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00784
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00788
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0079c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007a4
			 0x7f800000,                                                  // inf                                         /// 007a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007fc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00800
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00804
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00808
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0080c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00814
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00818
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0081c
			 0xff800000,                                                  // -inf                                        /// 00820
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00824
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00828
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0082c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00830
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00838
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00844
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00848
			 0x80011111,                                                  // -9.7958E-41                                 /// 0084c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00850
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00864
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00870
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00874
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00878
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00890
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00894
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00898
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0089c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x33333333,                                                  // 4 random values                             /// 008a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00904
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00908
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0090c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00910
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00914
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00918
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0091c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00920
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00924
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00928
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00930
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00934
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0093c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00940
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00948
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0094c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00950
			 0x7fc00001,                                                  // signaling NaN                               /// 00954
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00958
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xffc00001,                                                  // -signaling NaN                              /// 00960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00968
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00978
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00980
			 0x33333333,                                                  // 4 random values                             /// 00984
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0098c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00990
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00994
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00998
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0099c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a0
			 0x00000000,                                                  // zero                                        /// 009a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x3f028f5c,                                                  // 0.51                                        /// 009bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xcb000000,                                                  // -8388608.0                                  /// 009fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a08
			 0x80000000,                                                  // -zero                                       /// 00a0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a60
			 0x7fc00001,                                                  // signaling NaN                               /// 00a64
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a7c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x4b000000,                                                  // 8388608.0                                   /// 00a84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00aa8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ab8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00abc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ac0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ac4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00acc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00adc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x55555555,                                                  // 4 random values                             /// 00ae4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00af4
			 0xffc00001,                                                  // -signaling NaN                              /// 00af8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b1c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b34
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x3f028f5c,                                                  // 0.51                                        /// 00b3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b58
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b78
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ba0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x80000000,                                                  // -zero                                       /// 00bb4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00be0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00be8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bf0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bf4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c10
			 0x80000000,                                                  // -zero                                       /// 00c14
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c18
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c2c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c30
			 0x33333333,                                                  // 4 random values                             /// 00c34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c40
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c68
			 0x4b000000,                                                  // 8388608.0                                   /// 00c6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c70
			 0x3f028f5c,                                                  // 0.51                                        /// 00c74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c80
			 0x4b000000,                                                  // 8388608.0                                   /// 00c84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c94
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ca4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ca8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cbc
			 0xff800000,                                                  // -inf                                        /// 00cc0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cd0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d20
			 0x4b000000,                                                  // 8388608.0                                   /// 00d24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d88
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00da8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00db0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ddc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00de0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x80000000,                                                  // -zero                                       /// 00e28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x33333333,                                                  // 4 random values                             /// 00e34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e6c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ea0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ebc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ec0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ec4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ecc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ed8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00edc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ee8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ef4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ef8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00efc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x33333333,                                                  // 4 random values                             /// 00f0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f20
			 0x33333333,                                                  // 4 random values                             /// 00f24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f30
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80000000,                                                  // -zero                                       /// 00f98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc4
			 0x00000000,                                                  // zero                                        /// 00fc8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fcc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00000400                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xdcaee97a, /// 0x0
			 0x83625f47, /// 0x4
			 0xa27fef13, /// 0x8
			 0x74414de4, /// 0xc
			 0xac0f5e6b, /// 0x10
			 0xc1cd88b8, /// 0x14
			 0x6ec2b68f, /// 0x18
			 0xb551a4ac, /// 0x1c
			 0x57ce3650, /// 0x20
			 0x914ef465, /// 0x24
			 0x8edf88fc, /// 0x28
			 0x246f1050, /// 0x2c
			 0xe0a440be, /// 0x30
			 0x77a9a129, /// 0x34
			 0x1cdd0d90, /// 0x38
			 0x2cffd44a, /// 0x3c
			 0xafc637fe, /// 0x40
			 0x01a9bdd0, /// 0x44
			 0x9fa43839, /// 0x48
			 0x8043910b, /// 0x4c
			 0x585a4a97, /// 0x50
			 0x063e42c3, /// 0x54
			 0xc8a33a33, /// 0x58
			 0x4ffae0ac, /// 0x5c
			 0x21000c86, /// 0x60
			 0x1439bdaf, /// 0x64
			 0x2c6fc929, /// 0x68
			 0x4c7dadce, /// 0x6c
			 0xfa217ca7, /// 0x70
			 0xc1ff76c9, /// 0x74
			 0x564e1643, /// 0x78
			 0x78acb088, /// 0x7c
			 0x835ddf43, /// 0x80
			 0x8373f323, /// 0x84
			 0x294eda5f, /// 0x88
			 0x163afa55, /// 0x8c
			 0x8f8ac092, /// 0x90
			 0xb49d869f, /// 0x94
			 0x8eb5f9d8, /// 0x98
			 0x83051b97, /// 0x9c
			 0x0d308311, /// 0xa0
			 0xfec0af36, /// 0xa4
			 0x11a2289f, /// 0xa8
			 0x3b991f35, /// 0xac
			 0xf1e1ba15, /// 0xb0
			 0x63b205ed, /// 0xb4
			 0xf2902ab0, /// 0xb8
			 0x36d5c0c4, /// 0xbc
			 0x447f6264, /// 0xc0
			 0xc3f9b980, /// 0xc4
			 0x836146de, /// 0xc8
			 0xde7b8d78, /// 0xcc
			 0xab9449d9, /// 0xd0
			 0xcf4f0590, /// 0xd4
			 0x6c32f655, /// 0xd8
			 0x54a57c34, /// 0xdc
			 0x252d817b, /// 0xe0
			 0xe0b95cf1, /// 0xe4
			 0xecfb0506, /// 0xe8
			 0x9457e3cc, /// 0xec
			 0x86f3983a, /// 0xf0
			 0x81588609, /// 0xf4
			 0xfb3fabba, /// 0xf8
			 0x7368da22, /// 0xfc
			 0x07f46c8b, /// 0x100
			 0x031557d1, /// 0x104
			 0xe60ca8d3, /// 0x108
			 0x8059be37, /// 0x10c
			 0xc3409d10, /// 0x110
			 0xcb35991f, /// 0x114
			 0x5902a7fb, /// 0x118
			 0x509094f7, /// 0x11c
			 0x4bc48219, /// 0x120
			 0x75a59dfe, /// 0x124
			 0x1649a8f7, /// 0x128
			 0x9501209f, /// 0x12c
			 0x0f2d8976, /// 0x130
			 0x426c359e, /// 0x134
			 0x2ee43d1f, /// 0x138
			 0xe1fe61be, /// 0x13c
			 0x438d0742, /// 0x140
			 0x1fcc81eb, /// 0x144
			 0x2ed22210, /// 0x148
			 0x2d84ba6c, /// 0x14c
			 0x85b1ed7a, /// 0x150
			 0x3a139f1f, /// 0x154
			 0x0b63c52d, /// 0x158
			 0xed5ee71d, /// 0x15c
			 0xe4508a5d, /// 0x160
			 0x4076bf29, /// 0x164
			 0x3b719855, /// 0x168
			 0xb66265fd, /// 0x16c
			 0x221017ef, /// 0x170
			 0x32d6b947, /// 0x174
			 0xf1af5adb, /// 0x178
			 0x157bbba3, /// 0x17c
			 0xccbba204, /// 0x180
			 0xdd88e837, /// 0x184
			 0xfcaf91ff, /// 0x188
			 0xaac921f7, /// 0x18c
			 0x82bb9048, /// 0x190
			 0x79825191, /// 0x194
			 0xa02fc50d, /// 0x198
			 0x19a49b1e, /// 0x19c
			 0xae8ab5d3, /// 0x1a0
			 0xaaf2bb85, /// 0x1a4
			 0x7e858143, /// 0x1a8
			 0x92b9a5fc, /// 0x1ac
			 0xa75e4042, /// 0x1b0
			 0x8cda27fd, /// 0x1b4
			 0x22b7de11, /// 0x1b8
			 0xc8751122, /// 0x1bc
			 0x7ea030d0, /// 0x1c0
			 0x7ef968ce, /// 0x1c4
			 0x8776ca6c, /// 0x1c8
			 0xcb2cbc03, /// 0x1cc
			 0xd4b1e42c, /// 0x1d0
			 0x9b9aff1c, /// 0x1d4
			 0x32fe71d1, /// 0x1d8
			 0xf4517747, /// 0x1dc
			 0x4e19a27f, /// 0x1e0
			 0xc897ad05, /// 0x1e4
			 0x68f9489d, /// 0x1e8
			 0xe0658e16, /// 0x1ec
			 0x42f83aa2, /// 0x1f0
			 0xa9f4ceb1, /// 0x1f4
			 0x1ce94614, /// 0x1f8
			 0x00a1e477, /// 0x1fc
			 0xae4a584d, /// 0x200
			 0x7eacb42f, /// 0x204
			 0xbde2323e, /// 0x208
			 0xfe398bb4, /// 0x20c
			 0x7b731e39, /// 0x210
			 0xf0b21a8f, /// 0x214
			 0xcb25e1c5, /// 0x218
			 0x353303d8, /// 0x21c
			 0x44c09000, /// 0x220
			 0x9b9b6c64, /// 0x224
			 0xaece42c7, /// 0x228
			 0x67457af5, /// 0x22c
			 0xce08d32b, /// 0x230
			 0xbb9e86ce, /// 0x234
			 0x8b8a8772, /// 0x238
			 0xedbb6a5d, /// 0x23c
			 0xb3f6ff75, /// 0x240
			 0x1d6a5557, /// 0x244
			 0x3c54b173, /// 0x248
			 0x6a65f907, /// 0x24c
			 0x7592941e, /// 0x250
			 0x4358e48e, /// 0x254
			 0x4fb71e5a, /// 0x258
			 0x152c56c5, /// 0x25c
			 0x431dc5e3, /// 0x260
			 0x2fd3f2fe, /// 0x264
			 0x73bb8be8, /// 0x268
			 0xa8e805e7, /// 0x26c
			 0xe3303279, /// 0x270
			 0xc3c8c8ff, /// 0x274
			 0x9aab41b7, /// 0x278
			 0x25571b0f, /// 0x27c
			 0x161c0282, /// 0x280
			 0xbe4b6e3f, /// 0x284
			 0xa266ed54, /// 0x288
			 0x69c77fdf, /// 0x28c
			 0x607f4a93, /// 0x290
			 0xef9e2ef5, /// 0x294
			 0xb4e4c3c6, /// 0x298
			 0xaa87b036, /// 0x29c
			 0xb0910fb4, /// 0x2a0
			 0xe4f3c320, /// 0x2a4
			 0x61e25574, /// 0x2a8
			 0x2f84a503, /// 0x2ac
			 0xc3b92375, /// 0x2b0
			 0x209fba4a, /// 0x2b4
			 0x702ac329, /// 0x2b8
			 0x3770552a, /// 0x2bc
			 0xd4ded0a0, /// 0x2c0
			 0x67a5d07e, /// 0x2c4
			 0xf176d2c2, /// 0x2c8
			 0x741aaf68, /// 0x2cc
			 0x2a878e0c, /// 0x2d0
			 0x64b3ecd9, /// 0x2d4
			 0x3aee52d1, /// 0x2d8
			 0x33296d0e, /// 0x2dc
			 0x1f9b7b31, /// 0x2e0
			 0x934fb474, /// 0x2e4
			 0x4ec28bb8, /// 0x2e8
			 0xe71365e6, /// 0x2ec
			 0x63d3b660, /// 0x2f0
			 0x753210c9, /// 0x2f4
			 0xff23fdb5, /// 0x2f8
			 0x92e6e3b7, /// 0x2fc
			 0xa111e673, /// 0x300
			 0x12bc05b0, /// 0x304
			 0x4996437e, /// 0x308
			 0xb8a99016, /// 0x30c
			 0xbbef52ba, /// 0x310
			 0x6c7987e5, /// 0x314
			 0x7ea312b2, /// 0x318
			 0x062851f8, /// 0x31c
			 0xf4b3d41f, /// 0x320
			 0x0370afa1, /// 0x324
			 0x6203cf74, /// 0x328
			 0x2d600b47, /// 0x32c
			 0xe1861901, /// 0x330
			 0x5c69855a, /// 0x334
			 0x3d49ac30, /// 0x338
			 0x9d1b9fad, /// 0x33c
			 0x243d6596, /// 0x340
			 0xedcdb1f2, /// 0x344
			 0x40f9ee6d, /// 0x348
			 0xf06eeb52, /// 0x34c
			 0xf96d8162, /// 0x350
			 0x97553001, /// 0x354
			 0x54623ea5, /// 0x358
			 0xb2585572, /// 0x35c
			 0x5a325dfd, /// 0x360
			 0x8c94996b, /// 0x364
			 0xfa99e2b0, /// 0x368
			 0xccf5d063, /// 0x36c
			 0x17b9a0eb, /// 0x370
			 0x737f5405, /// 0x374
			 0x958a5fe8, /// 0x378
			 0xf5ed2344, /// 0x37c
			 0x69212160, /// 0x380
			 0x5cd5337f, /// 0x384
			 0x6ca9e4e7, /// 0x388
			 0x31c257f5, /// 0x38c
			 0x6671d786, /// 0x390
			 0x4b56a88f, /// 0x394
			 0xe13f118c, /// 0x398
			 0xf40d884c, /// 0x39c
			 0xd3fd5a03, /// 0x3a0
			 0x15011d6a, /// 0x3a4
			 0x148f5512, /// 0x3a8
			 0x6ee5e7b4, /// 0x3ac
			 0x183edd8d, /// 0x3b0
			 0xf12c8ade, /// 0x3b4
			 0x4d69b8da, /// 0x3b8
			 0xf33e4c58, /// 0x3bc
			 0x78029861, /// 0x3c0
			 0xa9d7658e, /// 0x3c4
			 0x0f063899, /// 0x3c8
			 0xdb4bc8bf, /// 0x3cc
			 0xdee9f73c, /// 0x3d0
			 0xbb6a6f0b, /// 0x3d4
			 0x0732ae07, /// 0x3d8
			 0x346748ee, /// 0x3dc
			 0x8f486ab9, /// 0x3e0
			 0x90c13b23, /// 0x3e4
			 0x2718b5fb, /// 0x3e8
			 0x295b9df8, /// 0x3ec
			 0xf7a1ae85, /// 0x3f0
			 0x98ee15e9, /// 0x3f4
			 0xc2708603, /// 0x3f8
			 0xe415f009, /// 0x3fc
			 0x07b2e61c, /// 0x400
			 0xcfcaf246, /// 0x404
			 0x1fca5664, /// 0x408
			 0xaf9b210f, /// 0x40c
			 0xe53fe0a2, /// 0x410
			 0x2fcbc596, /// 0x414
			 0x64bbe7e5, /// 0x418
			 0xa8e52c9c, /// 0x41c
			 0x3a026267, /// 0x420
			 0xcb58c253, /// 0x424
			 0x8edc1231, /// 0x428
			 0x1fefbf04, /// 0x42c
			 0xc5046590, /// 0x430
			 0x621cab1f, /// 0x434
			 0x5493288b, /// 0x438
			 0x6e2a0b89, /// 0x43c
			 0x62bc662d, /// 0x440
			 0x344d15d4, /// 0x444
			 0x9fcaeb6c, /// 0x448
			 0x38054452, /// 0x44c
			 0x8b14277d, /// 0x450
			 0x6cdbd5c0, /// 0x454
			 0x437915fb, /// 0x458
			 0xc9a2fcfb, /// 0x45c
			 0x1511ce15, /// 0x460
			 0x6a9672ce, /// 0x464
			 0x35258b8a, /// 0x468
			 0x6bc36b78, /// 0x46c
			 0xd4255cd8, /// 0x470
			 0xe88eb351, /// 0x474
			 0x3913aa99, /// 0x478
			 0x3f5363d3, /// 0x47c
			 0x9537c92f, /// 0x480
			 0x722c11f9, /// 0x484
			 0x98475876, /// 0x488
			 0x2bc8a047, /// 0x48c
			 0xd54115fb, /// 0x490
			 0x29f39144, /// 0x494
			 0xcc3c6f11, /// 0x498
			 0xdf0cdea7, /// 0x49c
			 0x856a689d, /// 0x4a0
			 0x451f18b3, /// 0x4a4
			 0x8e4ee4d9, /// 0x4a8
			 0x50b453d1, /// 0x4ac
			 0x7d15f665, /// 0x4b0
			 0xc6748577, /// 0x4b4
			 0xc0817dd4, /// 0x4b8
			 0x7bbfd401, /// 0x4bc
			 0x222673d8, /// 0x4c0
			 0x3297ec0f, /// 0x4c4
			 0xbb52dcf7, /// 0x4c8
			 0x539a0058, /// 0x4cc
			 0xd4ff01e2, /// 0x4d0
			 0xe53ccb44, /// 0x4d4
			 0x49a7436f, /// 0x4d8
			 0xfcd6847a, /// 0x4dc
			 0xb99a79d0, /// 0x4e0
			 0x82a4d428, /// 0x4e4
			 0x6f683387, /// 0x4e8
			 0xc15225a9, /// 0x4ec
			 0xfb876566, /// 0x4f0
			 0xedcc3c93, /// 0x4f4
			 0x151e0a1c, /// 0x4f8
			 0xd774ad70, /// 0x4fc
			 0xfacbe216, /// 0x500
			 0x532f394c, /// 0x504
			 0x844f5e87, /// 0x508
			 0x555d4513, /// 0x50c
			 0xd690c70e, /// 0x510
			 0x88130da8, /// 0x514
			 0xa6eecc6e, /// 0x518
			 0x9ba5807d, /// 0x51c
			 0x066b9fe5, /// 0x520
			 0x666644dc, /// 0x524
			 0xad70366e, /// 0x528
			 0xe8737a3f, /// 0x52c
			 0xc25e5d30, /// 0x530
			 0x2db20b09, /// 0x534
			 0xf58a5596, /// 0x538
			 0xc793262a, /// 0x53c
			 0xcb87c3b6, /// 0x540
			 0x5be74f6f, /// 0x544
			 0x6e5404c7, /// 0x548
			 0xbc206f7d, /// 0x54c
			 0x75e6e234, /// 0x550
			 0x616de837, /// 0x554
			 0xa0b9ca85, /// 0x558
			 0xde19995a, /// 0x55c
			 0x8063e8d7, /// 0x560
			 0x1821096c, /// 0x564
			 0x1f5a6c28, /// 0x568
			 0xb4c31708, /// 0x56c
			 0xd5c4aa30, /// 0x570
			 0x4f069958, /// 0x574
			 0xdc43b15c, /// 0x578
			 0x33e55916, /// 0x57c
			 0xb59fcb12, /// 0x580
			 0x20fa00be, /// 0x584
			 0x381260d9, /// 0x588
			 0x7d555ff9, /// 0x58c
			 0x26184d35, /// 0x590
			 0xc09c0f38, /// 0x594
			 0xae36d0d8, /// 0x598
			 0xc70585ca, /// 0x59c
			 0x7dfc5e30, /// 0x5a0
			 0x69a17071, /// 0x5a4
			 0x6d7a8f0f, /// 0x5a8
			 0xf68b6aed, /// 0x5ac
			 0x6f08bf6c, /// 0x5b0
			 0xf9e57823, /// 0x5b4
			 0x4a619bb2, /// 0x5b8
			 0xcf847dc7, /// 0x5bc
			 0x38fc38d4, /// 0x5c0
			 0xc49f2f64, /// 0x5c4
			 0x37224e28, /// 0x5c8
			 0x959116dd, /// 0x5cc
			 0x8dcf6000, /// 0x5d0
			 0xb3a16d6b, /// 0x5d4
			 0x28e8ba02, /// 0x5d8
			 0x0644b8dd, /// 0x5dc
			 0xc42b0e78, /// 0x5e0
			 0xa3767a4e, /// 0x5e4
			 0xd8324d74, /// 0x5e8
			 0xd143be19, /// 0x5ec
			 0x0a4cc3cb, /// 0x5f0
			 0x63f3a42c, /// 0x5f4
			 0x99b81660, /// 0x5f8
			 0xa9459c7f, /// 0x5fc
			 0x74a47c04, /// 0x600
			 0x30e09283, /// 0x604
			 0x94887e5c, /// 0x608
			 0x7a28f2c5, /// 0x60c
			 0xdb0cae7f, /// 0x610
			 0xaab7758f, /// 0x614
			 0x4088d08a, /// 0x618
			 0x8c87eb1a, /// 0x61c
			 0x3ee16572, /// 0x620
			 0xb00de45b, /// 0x624
			 0x01b97e5d, /// 0x628
			 0x01e41f1c, /// 0x62c
			 0xe18cf603, /// 0x630
			 0xb4ac32c4, /// 0x634
			 0x97a1f7e3, /// 0x638
			 0x504390dc, /// 0x63c
			 0x5ff08782, /// 0x640
			 0xa89822ae, /// 0x644
			 0x2611c831, /// 0x648
			 0x0e2de939, /// 0x64c
			 0x2c6326ce, /// 0x650
			 0xa6f0625e, /// 0x654
			 0x61a219ef, /// 0x658
			 0x1f523be8, /// 0x65c
			 0x31473d8a, /// 0x660
			 0x3386c688, /// 0x664
			 0x1937b423, /// 0x668
			 0x51ef51e0, /// 0x66c
			 0x065cc309, /// 0x670
			 0x65c8129c, /// 0x674
			 0x77541b9a, /// 0x678
			 0x14d37912, /// 0x67c
			 0x81c1fdde, /// 0x680
			 0xa1604f36, /// 0x684
			 0xdfc2b660, /// 0x688
			 0x0fd5360b, /// 0x68c
			 0xc17f97a6, /// 0x690
			 0x7277cbd2, /// 0x694
			 0x0a907abe, /// 0x698
			 0x0ac8563b, /// 0x69c
			 0x8932878b, /// 0x6a0
			 0xe4a8e79a, /// 0x6a4
			 0xaefdb99c, /// 0x6a8
			 0x111079dc, /// 0x6ac
			 0x0f6bb33a, /// 0x6b0
			 0x737636db, /// 0x6b4
			 0xdfe83562, /// 0x6b8
			 0xfd950de6, /// 0x6bc
			 0x9f408a22, /// 0x6c0
			 0xf729a111, /// 0x6c4
			 0x20d073fc, /// 0x6c8
			 0x9f402cfc, /// 0x6cc
			 0x51bf045b, /// 0x6d0
			 0xeaa1d076, /// 0x6d4
			 0x37056c80, /// 0x6d8
			 0xb9bba50f, /// 0x6dc
			 0x0cb2815c, /// 0x6e0
			 0x69d0560a, /// 0x6e4
			 0x61f10bf0, /// 0x6e8
			 0x8a60c5f8, /// 0x6ec
			 0x93793877, /// 0x6f0
			 0x846a9d6e, /// 0x6f4
			 0xb2ccc7c0, /// 0x6f8
			 0x12297924, /// 0x6fc
			 0x4f69250f, /// 0x700
			 0xf69358eb, /// 0x704
			 0x5669e73c, /// 0x708
			 0x026eb397, /// 0x70c
			 0xcc16245a, /// 0x710
			 0x33d8b304, /// 0x714
			 0x269a7269, /// 0x718
			 0xaee1a7b9, /// 0x71c
			 0x10fde72e, /// 0x720
			 0x0c07927f, /// 0x724
			 0xed588e82, /// 0x728
			 0x3261832f, /// 0x72c
			 0x642d61d4, /// 0x730
			 0xcdf3912a, /// 0x734
			 0x51ac286a, /// 0x738
			 0x82cfa1f3, /// 0x73c
			 0x6e4fe109, /// 0x740
			 0x17829f5a, /// 0x744
			 0x27dcf7bf, /// 0x748
			 0x6b34915e, /// 0x74c
			 0x7430f909, /// 0x750
			 0xacd79a27, /// 0x754
			 0xd3857bf0, /// 0x758
			 0x4fc5f4e0, /// 0x75c
			 0xb79ebadb, /// 0x760
			 0x141b2cbb, /// 0x764
			 0xaf003ad5, /// 0x768
			 0x0c207741, /// 0x76c
			 0xf6ca38f8, /// 0x770
			 0x1c05024a, /// 0x774
			 0xfb7f915b, /// 0x778
			 0x9b347fb7, /// 0x77c
			 0x7851a5b9, /// 0x780
			 0x05e59756, /// 0x784
			 0x423490d2, /// 0x788
			 0xd7b1f002, /// 0x78c
			 0xbe52e477, /// 0x790
			 0x1820500b, /// 0x794
			 0x729226b5, /// 0x798
			 0x2f947314, /// 0x79c
			 0xfea15592, /// 0x7a0
			 0x91625fcd, /// 0x7a4
			 0x0cd5174d, /// 0x7a8
			 0x2ab003f1, /// 0x7ac
			 0x1d1ab135, /// 0x7b0
			 0x35e8ebb5, /// 0x7b4
			 0x584ba171, /// 0x7b8
			 0x8a32311c, /// 0x7bc
			 0x74daa539, /// 0x7c0
			 0x472458c1, /// 0x7c4
			 0x6ed25877, /// 0x7c8
			 0xe14011d9, /// 0x7cc
			 0xa53474ed, /// 0x7d0
			 0x1ab20c3f, /// 0x7d4
			 0x5f8955a8, /// 0x7d8
			 0x9ef5b963, /// 0x7dc
			 0xa043e020, /// 0x7e0
			 0x9f6da50c, /// 0x7e4
			 0x223833aa, /// 0x7e8
			 0xa13aec15, /// 0x7ec
			 0xdf86094f, /// 0x7f0
			 0x678ac5d6, /// 0x7f4
			 0xcbbc3c81, /// 0x7f8
			 0x578cfab8, /// 0x7fc
			 0x1b2fa3fb, /// 0x800
			 0x6b4e9d75, /// 0x804
			 0x85eac485, /// 0x808
			 0x78f4941d, /// 0x80c
			 0xf447ee99, /// 0x810
			 0x02cae4ae, /// 0x814
			 0xc2b095a4, /// 0x818
			 0x0a6b51e9, /// 0x81c
			 0xe82a4a47, /// 0x820
			 0x67e89b34, /// 0x824
			 0x71657db5, /// 0x828
			 0x5d44757a, /// 0x82c
			 0x87262487, /// 0x830
			 0x64ca3efc, /// 0x834
			 0xf332e06f, /// 0x838
			 0x45e2db07, /// 0x83c
			 0x2243c87d, /// 0x840
			 0x5869e7ae, /// 0x844
			 0xe34f1aa5, /// 0x848
			 0x6d03b9e6, /// 0x84c
			 0xf9b84c16, /// 0x850
			 0xd4c08cab, /// 0x854
			 0x33f3918b, /// 0x858
			 0x7912e12d, /// 0x85c
			 0xc564c1cc, /// 0x860
			 0xced74ef0, /// 0x864
			 0x0774ccee, /// 0x868
			 0xb8d7dc86, /// 0x86c
			 0x8cdaff2d, /// 0x870
			 0x3920f480, /// 0x874
			 0x8c4ee12d, /// 0x878
			 0x14e915f7, /// 0x87c
			 0x22288265, /// 0x880
			 0xeb52d0d3, /// 0x884
			 0x057e89e3, /// 0x888
			 0xf738dfc0, /// 0x88c
			 0xec1e87c2, /// 0x890
			 0x83e60241, /// 0x894
			 0xe94199cf, /// 0x898
			 0x2d6f976e, /// 0x89c
			 0xdbc10dc6, /// 0x8a0
			 0xbd248267, /// 0x8a4
			 0x8092c67a, /// 0x8a8
			 0xee5a6f48, /// 0x8ac
			 0xe8adccf1, /// 0x8b0
			 0x9a05704a, /// 0x8b4
			 0x77bdaa1f, /// 0x8b8
			 0xca1149b2, /// 0x8bc
			 0xbda205ed, /// 0x8c0
			 0xc50dd6ac, /// 0x8c4
			 0x1d6640cf, /// 0x8c8
			 0x4c9cc6d7, /// 0x8cc
			 0xd40f95bc, /// 0x8d0
			 0xe9983a2b, /// 0x8d4
			 0xf846695d, /// 0x8d8
			 0xd562c89b, /// 0x8dc
			 0xdb0e8d6a, /// 0x8e0
			 0xbd5db1f0, /// 0x8e4
			 0x832ce7aa, /// 0x8e8
			 0xa7f16979, /// 0x8ec
			 0x2a8d2667, /// 0x8f0
			 0xae35754c, /// 0x8f4
			 0x38e3048b, /// 0x8f8
			 0x5f1ed7ea, /// 0x8fc
			 0xbc9c93aa, /// 0x900
			 0xc72ad6c2, /// 0x904
			 0x95b5099e, /// 0x908
			 0x4e1959f5, /// 0x90c
			 0xc0aea51c, /// 0x910
			 0xec25879b, /// 0x914
			 0xba094bb4, /// 0x918
			 0x1625395b, /// 0x91c
			 0xac143900, /// 0x920
			 0x66d8babb, /// 0x924
			 0x69f4c3b8, /// 0x928
			 0x12cd47bd, /// 0x92c
			 0x11c92ee2, /// 0x930
			 0x304a0a49, /// 0x934
			 0xe128502b, /// 0x938
			 0x53265c19, /// 0x93c
			 0x4b797782, /// 0x940
			 0xd12302ab, /// 0x944
			 0x18d2e0fc, /// 0x948
			 0x32e29021, /// 0x94c
			 0x1a51bfb1, /// 0x950
			 0x3adcc662, /// 0x954
			 0x8d009b34, /// 0x958
			 0xb0fdc61d, /// 0x95c
			 0x750fc821, /// 0x960
			 0xa04e7aa2, /// 0x964
			 0x72ec8b0e, /// 0x968
			 0xbf89fa01, /// 0x96c
			 0x38bc9b54, /// 0x970
			 0x96c2056a, /// 0x974
			 0x0450b6ea, /// 0x978
			 0x7b1e0fbe, /// 0x97c
			 0x11137555, /// 0x980
			 0x86092cf8, /// 0x984
			 0xbb09046c, /// 0x988
			 0xd7938f44, /// 0x98c
			 0xca1c984e, /// 0x990
			 0x950f9ee4, /// 0x994
			 0xf3abff22, /// 0x998
			 0x3fb418ba, /// 0x99c
			 0x9a78580c, /// 0x9a0
			 0xded523a9, /// 0x9a4
			 0xb2989fe7, /// 0x9a8
			 0x55941224, /// 0x9ac
			 0xe0f8ae45, /// 0x9b0
			 0x1185927b, /// 0x9b4
			 0x9392afde, /// 0x9b8
			 0x73780929, /// 0x9bc
			 0x2ba1dc65, /// 0x9c0
			 0xee328d80, /// 0x9c4
			 0x04653d81, /// 0x9c8
			 0xe76349d8, /// 0x9cc
			 0x0ada7f88, /// 0x9d0
			 0xc4a9c1b5, /// 0x9d4
			 0x16b496df, /// 0x9d8
			 0x46c4a030, /// 0x9dc
			 0x8207fa98, /// 0x9e0
			 0xf8f1ad88, /// 0x9e4
			 0xcf962b3b, /// 0x9e8
			 0x7cf1c69e, /// 0x9ec
			 0x4d90cdbe, /// 0x9f0
			 0xc1feeb59, /// 0x9f4
			 0xc7d07e91, /// 0x9f8
			 0xbf2281bb, /// 0x9fc
			 0x051c3c75, /// 0xa00
			 0x50c9a776, /// 0xa04
			 0x3912657b, /// 0xa08
			 0x1ba2f9eb, /// 0xa0c
			 0x66705ffe, /// 0xa10
			 0xa4f79f23, /// 0xa14
			 0xb395b937, /// 0xa18
			 0x9641433c, /// 0xa1c
			 0x596586ce, /// 0xa20
			 0x54236170, /// 0xa24
			 0x26722d8a, /// 0xa28
			 0xc2d0e043, /// 0xa2c
			 0x9dd1bfb5, /// 0xa30
			 0xa4958cf9, /// 0xa34
			 0x8f928f90, /// 0xa38
			 0x1eabcd1c, /// 0xa3c
			 0x0bf71364, /// 0xa40
			 0xbe78554a, /// 0xa44
			 0xfa6d48e7, /// 0xa48
			 0x66d75aae, /// 0xa4c
			 0x79034df7, /// 0xa50
			 0x9cdeef64, /// 0xa54
			 0x25c3b54e, /// 0xa58
			 0x20b7ffce, /// 0xa5c
			 0xf75350f7, /// 0xa60
			 0x2a240b53, /// 0xa64
			 0xa0c69dad, /// 0xa68
			 0xc949d2a5, /// 0xa6c
			 0xcf48d1bf, /// 0xa70
			 0x252dd989, /// 0xa74
			 0x0a1975da, /// 0xa78
			 0x8a20bbf7, /// 0xa7c
			 0xc869525f, /// 0xa80
			 0x056cbafd, /// 0xa84
			 0x817490ba, /// 0xa88
			 0x4e667001, /// 0xa8c
			 0x45e81058, /// 0xa90
			 0xd30da4c5, /// 0xa94
			 0x6d816ebe, /// 0xa98
			 0xc00b6d47, /// 0xa9c
			 0x4b703c3f, /// 0xaa0
			 0xdb871182, /// 0xaa4
			 0x6c453fb6, /// 0xaa8
			 0x0bfab1a8, /// 0xaac
			 0x81301f52, /// 0xab0
			 0x33b3226d, /// 0xab4
			 0xd9a0b22e, /// 0xab8
			 0xd0e46581, /// 0xabc
			 0x4974a6bd, /// 0xac0
			 0xd40fea9a, /// 0xac4
			 0xe02dd9f4, /// 0xac8
			 0x037dae4f, /// 0xacc
			 0xbf59b123, /// 0xad0
			 0x12c12172, /// 0xad4
			 0x5b2472cb, /// 0xad8
			 0x3d2928ca, /// 0xadc
			 0x6f7100a0, /// 0xae0
			 0xf5379846, /// 0xae4
			 0x792002a2, /// 0xae8
			 0xf8229fb5, /// 0xaec
			 0x5de92952, /// 0xaf0
			 0x26cd77ee, /// 0xaf4
			 0x0de66cd1, /// 0xaf8
			 0x7172e4c0, /// 0xafc
			 0x71b5a41e, /// 0xb00
			 0x50401b6b, /// 0xb04
			 0x44f95419, /// 0xb08
			 0x1fe62b5b, /// 0xb0c
			 0x2905a1d6, /// 0xb10
			 0xb605af7e, /// 0xb14
			 0x18dac3bc, /// 0xb18
			 0x11df330d, /// 0xb1c
			 0x61116453, /// 0xb20
			 0xacf7ff71, /// 0xb24
			 0xab3d87e5, /// 0xb28
			 0x809da702, /// 0xb2c
			 0x37c50b0b, /// 0xb30
			 0x53cb8494, /// 0xb34
			 0x08613904, /// 0xb38
			 0x27544d4b, /// 0xb3c
			 0x0bdc8a8b, /// 0xb40
			 0x5374588b, /// 0xb44
			 0x4a483613, /// 0xb48
			 0x8ee3f06b, /// 0xb4c
			 0x8cd96220, /// 0xb50
			 0xfc6af985, /// 0xb54
			 0xbe4ca21c, /// 0xb58
			 0x3d561c0d, /// 0xb5c
			 0x24829f35, /// 0xb60
			 0x1b86df3a, /// 0xb64
			 0x9d254a27, /// 0xb68
			 0x60f6862d, /// 0xb6c
			 0x591cd9e0, /// 0xb70
			 0x35b67027, /// 0xb74
			 0x7f94db12, /// 0xb78
			 0x3856ec94, /// 0xb7c
			 0xad7ffbc8, /// 0xb80
			 0xdda650c5, /// 0xb84
			 0x9f65e355, /// 0xb88
			 0x5fb738e6, /// 0xb8c
			 0x93e80d92, /// 0xb90
			 0xd3c56220, /// 0xb94
			 0x4a148c83, /// 0xb98
			 0xf00fdf45, /// 0xb9c
			 0x6b203444, /// 0xba0
			 0x349f1108, /// 0xba4
			 0x7d86ee19, /// 0xba8
			 0xea8deb88, /// 0xbac
			 0xf508c423, /// 0xbb0
			 0x8e2ebbad, /// 0xbb4
			 0x49910297, /// 0xbb8
			 0x627ee315, /// 0xbbc
			 0x38a86e07, /// 0xbc0
			 0xae824fd3, /// 0xbc4
			 0x8d2dfbe0, /// 0xbc8
			 0x054f259f, /// 0xbcc
			 0xb68d35eb, /// 0xbd0
			 0xa7f4cb8c, /// 0xbd4
			 0x88c45af2, /// 0xbd8
			 0xaab93ca1, /// 0xbdc
			 0xf3fb8651, /// 0xbe0
			 0x2c510e97, /// 0xbe4
			 0xc1be3fba, /// 0xbe8
			 0x7fe96ebf, /// 0xbec
			 0x41239556, /// 0xbf0
			 0xaf32b1f6, /// 0xbf4
			 0x4f7d1ade, /// 0xbf8
			 0x1de08cfa, /// 0xbfc
			 0x731d0fe5, /// 0xc00
			 0x39c735d1, /// 0xc04
			 0x0eff38b6, /// 0xc08
			 0xbec768ef, /// 0xc0c
			 0xc842e9c9, /// 0xc10
			 0xcf64adf7, /// 0xc14
			 0xb1bf1f67, /// 0xc18
			 0xda1578d3, /// 0xc1c
			 0x8b4e02f0, /// 0xc20
			 0x0c5365a9, /// 0xc24
			 0xdaa25444, /// 0xc28
			 0xfa00a2a7, /// 0xc2c
			 0x17ff24b3, /// 0xc30
			 0x72a4ba5a, /// 0xc34
			 0xa0eed953, /// 0xc38
			 0x7439618a, /// 0xc3c
			 0x6d57bab5, /// 0xc40
			 0x86bd71cc, /// 0xc44
			 0x00f67046, /// 0xc48
			 0xd7927c59, /// 0xc4c
			 0xe2608a18, /// 0xc50
			 0x8ab421ef, /// 0xc54
			 0xde2bd698, /// 0xc58
			 0x58989ee5, /// 0xc5c
			 0x73197a26, /// 0xc60
			 0xb74aa328, /// 0xc64
			 0xf1dfc7fa, /// 0xc68
			 0xf7a5c7e5, /// 0xc6c
			 0x8816abbd, /// 0xc70
			 0x4000c033, /// 0xc74
			 0x5e093ba7, /// 0xc78
			 0x174586a5, /// 0xc7c
			 0xd7140c2d, /// 0xc80
			 0xc7c4ce1a, /// 0xc84
			 0xcd564d31, /// 0xc88
			 0x8dfba9ad, /// 0xc8c
			 0xab9c96b4, /// 0xc90
			 0xb5fe4af8, /// 0xc94
			 0xd94fc90d, /// 0xc98
			 0xf419fc1a, /// 0xc9c
			 0xadd163e8, /// 0xca0
			 0xce89dc02, /// 0xca4
			 0x6c944e71, /// 0xca8
			 0x16b8c9b1, /// 0xcac
			 0xee9241f3, /// 0xcb0
			 0x0b8d39f1, /// 0xcb4
			 0x261e8cd3, /// 0xcb8
			 0xc406614d, /// 0xcbc
			 0x931160df, /// 0xcc0
			 0x26dde855, /// 0xcc4
			 0xe0390863, /// 0xcc8
			 0x8f63c137, /// 0xccc
			 0x10b7932b, /// 0xcd0
			 0x2ed57e79, /// 0xcd4
			 0x9a2f314a, /// 0xcd8
			 0x9adaf7aa, /// 0xcdc
			 0xb5c59a50, /// 0xce0
			 0x0491975e, /// 0xce4
			 0x1bf19697, /// 0xce8
			 0xac392b5a, /// 0xcec
			 0xbba0a48f, /// 0xcf0
			 0x77dfea0d, /// 0xcf4
			 0x0c7a5054, /// 0xcf8
			 0x4d5bc52b, /// 0xcfc
			 0x6bb16ba6, /// 0xd00
			 0xa28692f4, /// 0xd04
			 0x72ab3d76, /// 0xd08
			 0x715740d3, /// 0xd0c
			 0xabc3746e, /// 0xd10
			 0x132462e2, /// 0xd14
			 0xbf8a07a2, /// 0xd18
			 0x5a243009, /// 0xd1c
			 0x1987ff16, /// 0xd20
			 0x6768761a, /// 0xd24
			 0x745db13c, /// 0xd28
			 0xf77ae864, /// 0xd2c
			 0x08269966, /// 0xd30
			 0x90625a29, /// 0xd34
			 0x7d760e50, /// 0xd38
			 0xcd24b631, /// 0xd3c
			 0xcf30d2ee, /// 0xd40
			 0x2ddec2c2, /// 0xd44
			 0xaaca93d9, /// 0xd48
			 0x28eabf23, /// 0xd4c
			 0xa7c6f7b9, /// 0xd50
			 0x80ea5c40, /// 0xd54
			 0x567037dd, /// 0xd58
			 0xb787d5fb, /// 0xd5c
			 0x68393d4e, /// 0xd60
			 0x938f0d4b, /// 0xd64
			 0x3157e39e, /// 0xd68
			 0xd33722ec, /// 0xd6c
			 0x37fa18c7, /// 0xd70
			 0x30b0d34c, /// 0xd74
			 0x58b9edd2, /// 0xd78
			 0x483991ac, /// 0xd7c
			 0xeffa3a28, /// 0xd80
			 0xf33d67c4, /// 0xd84
			 0x32ef2fb4, /// 0xd88
			 0x9182d2ba, /// 0xd8c
			 0x9dea7799, /// 0xd90
			 0x6ccf1d7f, /// 0xd94
			 0x14019511, /// 0xd98
			 0x8e80a9d2, /// 0xd9c
			 0x52c96fb5, /// 0xda0
			 0x79d8f017, /// 0xda4
			 0x398500b6, /// 0xda8
			 0xcb6d1d15, /// 0xdac
			 0xa41caf3b, /// 0xdb0
			 0x41afbbd3, /// 0xdb4
			 0x359ec9e6, /// 0xdb8
			 0xf5b4a0d7, /// 0xdbc
			 0x0cbe3a1c, /// 0xdc0
			 0xb4c865fb, /// 0xdc4
			 0xff7eb837, /// 0xdc8
			 0x3a239d28, /// 0xdcc
			 0x1abed535, /// 0xdd0
			 0x856136d1, /// 0xdd4
			 0x619b4547, /// 0xdd8
			 0xa7a799bc, /// 0xddc
			 0xa822707b, /// 0xde0
			 0xb91d1b33, /// 0xde4
			 0xf9e3077c, /// 0xde8
			 0x3e566098, /// 0xdec
			 0x99a5b6a0, /// 0xdf0
			 0x487ad2da, /// 0xdf4
			 0xf983d3e6, /// 0xdf8
			 0xdf306a32, /// 0xdfc
			 0xb2cdfe21, /// 0xe00
			 0x79dc09f1, /// 0xe04
			 0xe60a03a1, /// 0xe08
			 0xf93ffe02, /// 0xe0c
			 0xbf33d790, /// 0xe10
			 0x76f61c48, /// 0xe14
			 0x661d3cf0, /// 0xe18
			 0xc3fb0170, /// 0xe1c
			 0x8713dce1, /// 0xe20
			 0x5124f0f0, /// 0xe24
			 0xe7c0dde5, /// 0xe28
			 0x1665cec6, /// 0xe2c
			 0x54f8f7ac, /// 0xe30
			 0xcfc241dd, /// 0xe34
			 0xbc595bfc, /// 0xe38
			 0x37171b5f, /// 0xe3c
			 0xa850a278, /// 0xe40
			 0x47a473f2, /// 0xe44
			 0xb80b2a5c, /// 0xe48
			 0x627e36a2, /// 0xe4c
			 0xfab7f3ee, /// 0xe50
			 0xca7a99b7, /// 0xe54
			 0x5373cc00, /// 0xe58
			 0x3af1feb8, /// 0xe5c
			 0xb6292e17, /// 0xe60
			 0x2d689dd2, /// 0xe64
			 0xb0447ac5, /// 0xe68
			 0xc47f297d, /// 0xe6c
			 0x3b8a9f82, /// 0xe70
			 0xa2962c7e, /// 0xe74
			 0x041ef011, /// 0xe78
			 0xf3fe24e0, /// 0xe7c
			 0xf6b30b39, /// 0xe80
			 0x2924bfd9, /// 0xe84
			 0x496d9f61, /// 0xe88
			 0x9b09e10a, /// 0xe8c
			 0xd75a9fe4, /// 0xe90
			 0x7226a330, /// 0xe94
			 0x1c3ba40b, /// 0xe98
			 0x3bcd236f, /// 0xe9c
			 0x94b4abb2, /// 0xea0
			 0x8a44e611, /// 0xea4
			 0xcf083c54, /// 0xea8
			 0x5225ea0e, /// 0xeac
			 0x4b0aa0e7, /// 0xeb0
			 0x4fb87a86, /// 0xeb4
			 0xe671b9d4, /// 0xeb8
			 0xc3d0c77f, /// 0xebc
			 0xe21b7d78, /// 0xec0
			 0xedef9f18, /// 0xec4
			 0xb492f64f, /// 0xec8
			 0xa4a5ccbb, /// 0xecc
			 0x4184dd54, /// 0xed0
			 0x72bac2fb, /// 0xed4
			 0x77f6f7f6, /// 0xed8
			 0x92177b57, /// 0xedc
			 0x4936902b, /// 0xee0
			 0x6bb9337b, /// 0xee4
			 0xf8663ebe, /// 0xee8
			 0x3e3a5dd5, /// 0xeec
			 0xc6642b43, /// 0xef0
			 0xa16fb230, /// 0xef4
			 0x5a4f7114, /// 0xef8
			 0x4ae223dd, /// 0xefc
			 0xd7714c1f, /// 0xf00
			 0x9a123355, /// 0xf04
			 0x9ee8f128, /// 0xf08
			 0x4579aad7, /// 0xf0c
			 0x54ad753a, /// 0xf10
			 0x5ee48dd9, /// 0xf14
			 0xd6d4bf43, /// 0xf18
			 0xd681c85a, /// 0xf1c
			 0x5c8cd7e6, /// 0xf20
			 0x9904831d, /// 0xf24
			 0xa8053411, /// 0xf28
			 0x93a7d78c, /// 0xf2c
			 0x68b9435f, /// 0xf30
			 0xb72a5d25, /// 0xf34
			 0x4db26045, /// 0xf38
			 0xd5a13c06, /// 0xf3c
			 0xf1d6b22d, /// 0xf40
			 0x8df062f1, /// 0xf44
			 0xc8dead32, /// 0xf48
			 0xfb9352ce, /// 0xf4c
			 0xe2ac54de, /// 0xf50
			 0xd09aba91, /// 0xf54
			 0xd27e3b6b, /// 0xf58
			 0x26108562, /// 0xf5c
			 0xebbec0cc, /// 0xf60
			 0x4fddd7fe, /// 0xf64
			 0x8155c2dc, /// 0xf68
			 0x2d015cd2, /// 0xf6c
			 0x4e8bbb59, /// 0xf70
			 0xb1737960, /// 0xf74
			 0xfa671bc1, /// 0xf78
			 0x2eade151, /// 0xf7c
			 0xc0037e7c, /// 0xf80
			 0x050e4323, /// 0xf84
			 0x625a4fb0, /// 0xf88
			 0x22a195bc, /// 0xf8c
			 0x5a30f529, /// 0xf90
			 0xabbb6a62, /// 0xf94
			 0x1c1e4177, /// 0xf98
			 0x2f411671, /// 0xf9c
			 0x3555a5f7, /// 0xfa0
			 0x92650c78, /// 0xfa4
			 0x2221bab0, /// 0xfa8
			 0xa3d4eb0e, /// 0xfac
			 0x09a3668b, /// 0xfb0
			 0x360fd140, /// 0xfb4
			 0x40e2f142, /// 0xfb8
			 0x9a9b9670, /// 0xfbc
			 0x3ad3233f, /// 0xfc0
			 0x4f8e55c5, /// 0xfc4
			 0x05e8e7e7, /// 0xfc8
			 0xc3475066, /// 0xfcc
			 0x8b67d8d8, /// 0xfd0
			 0xdc3817f3, /// 0xfd4
			 0x2d4e840e, /// 0xfd8
			 0xa29ccb12, /// 0xfdc
			 0xf2bb665b, /// 0xfe0
			 0x2dc88344, /// 0xfe4
			 0xa6debfc2, /// 0xfe8
			 0xcb2c4529, /// 0xfec
			 0xcf081a45, /// 0xff0
			 0x28b723ae, /// 0xff4
			 0xd6176b78, /// 0xff8
			 0xd997b60d /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x32ae0aba, /// 0x0
			 0x95ae243b, /// 0x4
			 0x197b42b4, /// 0x8
			 0x4f79d5b8, /// 0xc
			 0xe784a076, /// 0x10
			 0x7bd4bdaf, /// 0x14
			 0x2667d2a1, /// 0x18
			 0x82fab664, /// 0x1c
			 0x046a4fdd, /// 0x20
			 0xc14d43a8, /// 0x24
			 0xb1769ae4, /// 0x28
			 0x113fb99d, /// 0x2c
			 0x64299533, /// 0x30
			 0x115e7301, /// 0x34
			 0x53a11858, /// 0x38
			 0x8cb3180f, /// 0x3c
			 0xdc6394a0, /// 0x40
			 0x99484cc8, /// 0x44
			 0x825186a2, /// 0x48
			 0xbfd69b47, /// 0x4c
			 0x16bf1489, /// 0x50
			 0xdd151e2f, /// 0x54
			 0x18335302, /// 0x58
			 0x35043735, /// 0x5c
			 0xbd6d2651, /// 0x60
			 0x692c6f9a, /// 0x64
			 0x565fe043, /// 0x68
			 0x93f5ee31, /// 0x6c
			 0xc8cb5d10, /// 0x70
			 0x2b992389, /// 0x74
			 0x9517a062, /// 0x78
			 0x4b217e79, /// 0x7c
			 0x3848f4d0, /// 0x80
			 0x107e06be, /// 0x84
			 0x01255f04, /// 0x88
			 0x5f909f58, /// 0x8c
			 0x24d3f34d, /// 0x90
			 0xb655ed53, /// 0x94
			 0x364c240b, /// 0x98
			 0x99587221, /// 0x9c
			 0x656ad1f8, /// 0xa0
			 0x61b75eab, /// 0xa4
			 0xc109e310, /// 0xa8
			 0xc53b7601, /// 0xac
			 0xd7aab561, /// 0xb0
			 0x877b420f, /// 0xb4
			 0x7a61a0bc, /// 0xb8
			 0x25f02129, /// 0xbc
			 0xc44a9c23, /// 0xc0
			 0x016c72fb, /// 0xc4
			 0x4ec15dea, /// 0xc8
			 0xc7d3e206, /// 0xcc
			 0xd8a791c0, /// 0xd0
			 0xa2d5b456, /// 0xd4
			 0x1d227927, /// 0xd8
			 0xf5952fe1, /// 0xdc
			 0x2d64393f, /// 0xe0
			 0x68602083, /// 0xe4
			 0xe30ee545, /// 0xe8
			 0xdf8e3ac2, /// 0xec
			 0x877dc896, /// 0xf0
			 0x626ae478, /// 0xf4
			 0x104e4d25, /// 0xf8
			 0x3cbce61e, /// 0xfc
			 0x22c9d3a9, /// 0x100
			 0x2f883392, /// 0x104
			 0x027f37e9, /// 0x108
			 0x193c70ee, /// 0x10c
			 0xda794e92, /// 0x110
			 0x712db929, /// 0x114
			 0xb94254df, /// 0x118
			 0xa3291d3a, /// 0x11c
			 0x9c51ae34, /// 0x120
			 0x2bf1928d, /// 0x124
			 0x5a801488, /// 0x128
			 0x37bf04bd, /// 0x12c
			 0x3d4f1ef2, /// 0x130
			 0x85195c22, /// 0x134
			 0x93256d52, /// 0x138
			 0xafe54105, /// 0x13c
			 0x4ac86fad, /// 0x140
			 0x1dcf358f, /// 0x144
			 0x09450d48, /// 0x148
			 0x5fa61859, /// 0x14c
			 0x891a467a, /// 0x150
			 0xe939c0d7, /// 0x154
			 0xb7a70ebb, /// 0x158
			 0x18bf4f49, /// 0x15c
			 0xca1b5972, /// 0x160
			 0xabfd586c, /// 0x164
			 0x8f4f9e41, /// 0x168
			 0xc6fb76a4, /// 0x16c
			 0x1854386c, /// 0x170
			 0x23fb234d, /// 0x174
			 0xe961f32a, /// 0x178
			 0x0c3c5926, /// 0x17c
			 0xc069064e, /// 0x180
			 0xdf3d1b20, /// 0x184
			 0xa7f13d77, /// 0x188
			 0x0efb96ae, /// 0x18c
			 0xe1e35659, /// 0x190
			 0xdeff75e3, /// 0x194
			 0x172ec3b5, /// 0x198
			 0x52df3955, /// 0x19c
			 0x19cb011c, /// 0x1a0
			 0x648e2ded, /// 0x1a4
			 0xbc7f7743, /// 0x1a8
			 0xd92bff5f, /// 0x1ac
			 0x4f9d4bb9, /// 0x1b0
			 0x97446cb6, /// 0x1b4
			 0xb49049a2, /// 0x1b8
			 0xb3a45ab6, /// 0x1bc
			 0xf4b68a52, /// 0x1c0
			 0xaa63a371, /// 0x1c4
			 0x18a9b33d, /// 0x1c8
			 0x76371ae3, /// 0x1cc
			 0x1d86ade6, /// 0x1d0
			 0xe44154a4, /// 0x1d4
			 0x565c0629, /// 0x1d8
			 0x6e1c29c5, /// 0x1dc
			 0x9b4208b5, /// 0x1e0
			 0xd2a8770c, /// 0x1e4
			 0x37b04c36, /// 0x1e8
			 0xe927d6ab, /// 0x1ec
			 0x1191f90c, /// 0x1f0
			 0xa25617db, /// 0x1f4
			 0x3442e496, /// 0x1f8
			 0xeeb9e949, /// 0x1fc
			 0x2aefe887, /// 0x200
			 0xf2cc1d53, /// 0x204
			 0x9df518c9, /// 0x208
			 0x718e0764, /// 0x20c
			 0x9adf88f6, /// 0x210
			 0xe1225012, /// 0x214
			 0x92c02253, /// 0x218
			 0x4942a26c, /// 0x21c
			 0xa0f96d03, /// 0x220
			 0xfff5ea01, /// 0x224
			 0xb99f3b9f, /// 0x228
			 0xf0075cab, /// 0x22c
			 0x160498ad, /// 0x230
			 0xbbf10324, /// 0x234
			 0x8af601ef, /// 0x238
			 0x298bb4ed, /// 0x23c
			 0x7c258919, /// 0x240
			 0x88adb518, /// 0x244
			 0xb38e2502, /// 0x248
			 0x2931a2a4, /// 0x24c
			 0x36e181b9, /// 0x250
			 0x31dea4d6, /// 0x254
			 0x133b56b7, /// 0x258
			 0xeba0d301, /// 0x25c
			 0x1e6dc1d0, /// 0x260
			 0xe5afde70, /// 0x264
			 0xfe71a6aa, /// 0x268
			 0x6f3f1976, /// 0x26c
			 0x9c7414f1, /// 0x270
			 0x007b425f, /// 0x274
			 0xe8df75c7, /// 0x278
			 0x047ceced, /// 0x27c
			 0x12bfeeda, /// 0x280
			 0x57859e29, /// 0x284
			 0x68796283, /// 0x288
			 0xe0c9d89b, /// 0x28c
			 0xa3676932, /// 0x290
			 0xcb6a5f05, /// 0x294
			 0x2de3dc28, /// 0x298
			 0xd092bc72, /// 0x29c
			 0x3f1e9699, /// 0x2a0
			 0x6ac3a063, /// 0x2a4
			 0x82268284, /// 0x2a8
			 0xb21b2b80, /// 0x2ac
			 0x7527a150, /// 0x2b0
			 0xa3df1cce, /// 0x2b4
			 0x82736afa, /// 0x2b8
			 0xeb6753a8, /// 0x2bc
			 0x3c00b5ae, /// 0x2c0
			 0x289b1866, /// 0x2c4
			 0x656bfdf0, /// 0x2c8
			 0x20dc0ee3, /// 0x2cc
			 0x71faf188, /// 0x2d0
			 0x537497e6, /// 0x2d4
			 0x2d5de66c, /// 0x2d8
			 0x47fcda37, /// 0x2dc
			 0x89c5acde, /// 0x2e0
			 0xbeb37581, /// 0x2e4
			 0x642c2fe1, /// 0x2e8
			 0x5a8a3ac7, /// 0x2ec
			 0xaabb0e43, /// 0x2f0
			 0x6b08f04a, /// 0x2f4
			 0x887eeee9, /// 0x2f8
			 0x8ffcc408, /// 0x2fc
			 0x82801946, /// 0x300
			 0x76c119a3, /// 0x304
			 0x9da41f8c, /// 0x308
			 0x30d67953, /// 0x30c
			 0x4c2fe2cb, /// 0x310
			 0x102dc04f, /// 0x314
			 0x51b987a1, /// 0x318
			 0x48bd3940, /// 0x31c
			 0xd9a3312a, /// 0x320
			 0x41730cb4, /// 0x324
			 0xdb9b4d0d, /// 0x328
			 0xa6d009cf, /// 0x32c
			 0xae3ccb5b, /// 0x330
			 0xd17097ea, /// 0x334
			 0xace44e6e, /// 0x338
			 0x5319d5a3, /// 0x33c
			 0xad9474bf, /// 0x340
			 0xd7563bb8, /// 0x344
			 0x1e14ee36, /// 0x348
			 0xeb2ca6fb, /// 0x34c
			 0xdd6d6a85, /// 0x350
			 0x65bebce5, /// 0x354
			 0x9cae9b98, /// 0x358
			 0x6e76c197, /// 0x35c
			 0xb34dc2a6, /// 0x360
			 0x0a56b1f2, /// 0x364
			 0x541c11a7, /// 0x368
			 0xff5a2e67, /// 0x36c
			 0xc15dd077, /// 0x370
			 0xcfd7469d, /// 0x374
			 0x694e4892, /// 0x378
			 0x0eaad360, /// 0x37c
			 0x1ddec4a4, /// 0x380
			 0x08d1bcdb, /// 0x384
			 0x7ae4e4b2, /// 0x388
			 0xa17b0947, /// 0x38c
			 0x5ff7aab9, /// 0x390
			 0x10c5c879, /// 0x394
			 0xf2569698, /// 0x398
			 0xdfc252cf, /// 0x39c
			 0xc750c4ea, /// 0x3a0
			 0xe42175f0, /// 0x3a4
			 0xd805e389, /// 0x3a8
			 0x0a0732ad, /// 0x3ac
			 0xcc55eef8, /// 0x3b0
			 0x04a9801f, /// 0x3b4
			 0xb068c822, /// 0x3b8
			 0x9c1bd4a7, /// 0x3bc
			 0xd0c51646, /// 0x3c0
			 0x707f7bd2, /// 0x3c4
			 0xfea4bf59, /// 0x3c8
			 0xce8e6727, /// 0x3cc
			 0x530aaaa6, /// 0x3d0
			 0x803e374e, /// 0x3d4
			 0x645a2793, /// 0x3d8
			 0xb3757c31, /// 0x3dc
			 0x27f2d687, /// 0x3e0
			 0x26f33937, /// 0x3e4
			 0x9408c691, /// 0x3e8
			 0xf811b2c7, /// 0x3ec
			 0x65eb6695, /// 0x3f0
			 0xb38ba66d, /// 0x3f4
			 0xf0b94be4, /// 0x3f8
			 0xda2212fd, /// 0x3fc
			 0x1ab3f664, /// 0x400
			 0xc0ab8c21, /// 0x404
			 0xee58be42, /// 0x408
			 0xf801320f, /// 0x40c
			 0x80db4c08, /// 0x410
			 0x50005cff, /// 0x414
			 0xbdd3a584, /// 0x418
			 0x82787a8e, /// 0x41c
			 0x07e6246b, /// 0x420
			 0xeceb93c4, /// 0x424
			 0xb339f8e0, /// 0x428
			 0x57c224af, /// 0x42c
			 0x779d4e8b, /// 0x430
			 0x3801e9f3, /// 0x434
			 0xd6d79af8, /// 0x438
			 0xe82b6b36, /// 0x43c
			 0x22ea3cb7, /// 0x440
			 0x25951b8f, /// 0x444
			 0x6990b7e7, /// 0x448
			 0x346e81d5, /// 0x44c
			 0x2d47ab1e, /// 0x450
			 0x2fe753a0, /// 0x454
			 0xf31ae45d, /// 0x458
			 0xa83104a6, /// 0x45c
			 0xb20fffda, /// 0x460
			 0x5303ea23, /// 0x464
			 0x90450184, /// 0x468
			 0x3288caef, /// 0x46c
			 0x6d8d0616, /// 0x470
			 0x6c47956d, /// 0x474
			 0x7a1be808, /// 0x478
			 0xe59816c9, /// 0x47c
			 0xc66bddcb, /// 0x480
			 0x19bcda84, /// 0x484
			 0x436067ec, /// 0x488
			 0x24f6adc0, /// 0x48c
			 0x80cf0239, /// 0x490
			 0x1e136697, /// 0x494
			 0x529195e2, /// 0x498
			 0x2bf89d19, /// 0x49c
			 0x575cc4ed, /// 0x4a0
			 0x9af8b40f, /// 0x4a4
			 0x026295f8, /// 0x4a8
			 0xee409aac, /// 0x4ac
			 0x16bc5c41, /// 0x4b0
			 0x7c7ca5b3, /// 0x4b4
			 0xc5da0fa1, /// 0x4b8
			 0xe7119099, /// 0x4bc
			 0xf46204f0, /// 0x4c0
			 0x99deec6a, /// 0x4c4
			 0xad7dcbd2, /// 0x4c8
			 0x9b6fba9c, /// 0x4cc
			 0x051404ee, /// 0x4d0
			 0x4207b380, /// 0x4d4
			 0xf04ce632, /// 0x4d8
			 0x9191cb39, /// 0x4dc
			 0x1cf0c0f9, /// 0x4e0
			 0x443d2ce4, /// 0x4e4
			 0x08e7d647, /// 0x4e8
			 0x22c96891, /// 0x4ec
			 0xdbc0601b, /// 0x4f0
			 0x393670ce, /// 0x4f4
			 0x8023b5ec, /// 0x4f8
			 0x070a9fc0, /// 0x4fc
			 0xca51d8d2, /// 0x500
			 0x8b49e6c8, /// 0x504
			 0x70112083, /// 0x508
			 0x586f171f, /// 0x50c
			 0x0dd9c527, /// 0x510
			 0x3e4e526d, /// 0x514
			 0x1cec4873, /// 0x518
			 0xb9e94868, /// 0x51c
			 0x5c8360d9, /// 0x520
			 0x3e37137f, /// 0x524
			 0x9454f20a, /// 0x528
			 0xd9e25d97, /// 0x52c
			 0xe23c1304, /// 0x530
			 0x8755cd3a, /// 0x534
			 0xb4400aea, /// 0x538
			 0x3399de15, /// 0x53c
			 0xa247d3de, /// 0x540
			 0x4e08adfd, /// 0x544
			 0xb4ce85ba, /// 0x548
			 0x3d1e9266, /// 0x54c
			 0x3b50d607, /// 0x550
			 0xde91b4be, /// 0x554
			 0x1122173c, /// 0x558
			 0x1cf9381c, /// 0x55c
			 0x2f63e020, /// 0x560
			 0x1b844ce9, /// 0x564
			 0x290b1c31, /// 0x568
			 0xcb0cbffe, /// 0x56c
			 0xd17cae0f, /// 0x570
			 0x2409a82d, /// 0x574
			 0x1d3f592a, /// 0x578
			 0x79a5a705, /// 0x57c
			 0x15a33d25, /// 0x580
			 0x7378091d, /// 0x584
			 0xef716890, /// 0x588
			 0xf5c6c864, /// 0x58c
			 0x42c393d1, /// 0x590
			 0xd749a918, /// 0x594
			 0x5e8e3107, /// 0x598
			 0x4e800cb4, /// 0x59c
			 0x19970bcf, /// 0x5a0
			 0xf5669465, /// 0x5a4
			 0xa3ad3269, /// 0x5a8
			 0x1b388dab, /// 0x5ac
			 0x30a239cd, /// 0x5b0
			 0xbd2e47e3, /// 0x5b4
			 0xb817e504, /// 0x5b8
			 0xc77e70c9, /// 0x5bc
			 0xb928dbce, /// 0x5c0
			 0xac6a730d, /// 0x5c4
			 0xe7e91f74, /// 0x5c8
			 0xd3fea326, /// 0x5cc
			 0xc84ba3e8, /// 0x5d0
			 0x6d5eb072, /// 0x5d4
			 0x0b7b0ceb, /// 0x5d8
			 0x859175a0, /// 0x5dc
			 0x730986e8, /// 0x5e0
			 0x305b3c2c, /// 0x5e4
			 0xd901f494, /// 0x5e8
			 0x33b142ac, /// 0x5ec
			 0x7d399137, /// 0x5f0
			 0x54b39835, /// 0x5f4
			 0x3b93d790, /// 0x5f8
			 0x92a7b223, /// 0x5fc
			 0x8bb63f3f, /// 0x600
			 0xb1e474bd, /// 0x604
			 0xccf8c75e, /// 0x608
			 0x3d9e4b56, /// 0x60c
			 0x077a789a, /// 0x610
			 0x6463914a, /// 0x614
			 0x56d90d66, /// 0x618
			 0x9c390b6f, /// 0x61c
			 0x2c8acc69, /// 0x620
			 0xa0dc7972, /// 0x624
			 0x81dbd4e1, /// 0x628
			 0x5dd913bd, /// 0x62c
			 0x76b0dddf, /// 0x630
			 0xba12011f, /// 0x634
			 0x51a7825b, /// 0x638
			 0xbe260464, /// 0x63c
			 0x5186dab0, /// 0x640
			 0x0a24e086, /// 0x644
			 0x97563074, /// 0x648
			 0xb5260df0, /// 0x64c
			 0xc4d057e2, /// 0x650
			 0xb3295d50, /// 0x654
			 0xc38158b5, /// 0x658
			 0x6964a70d, /// 0x65c
			 0xea2d95ca, /// 0x660
			 0xebe0fc50, /// 0x664
			 0x0d98bdbe, /// 0x668
			 0x8524ccf2, /// 0x66c
			 0x63278c7e, /// 0x670
			 0x2d37b8cf, /// 0x674
			 0x07e41753, /// 0x678
			 0x69067ff0, /// 0x67c
			 0x2e834028, /// 0x680
			 0x5da7149e, /// 0x684
			 0x1ae71b66, /// 0x688
			 0xa9f51f9b, /// 0x68c
			 0xe7d45840, /// 0x690
			 0x69420141, /// 0x694
			 0x06ef7be7, /// 0x698
			 0x56727701, /// 0x69c
			 0xe9d0178a, /// 0x6a0
			 0xf2411455, /// 0x6a4
			 0xf5821774, /// 0x6a8
			 0x095e74b6, /// 0x6ac
			 0x83b3cea6, /// 0x6b0
			 0xf6d5df16, /// 0x6b4
			 0x4b82e199, /// 0x6b8
			 0x8e258d33, /// 0x6bc
			 0x156e5e11, /// 0x6c0
			 0xbc4aaf6e, /// 0x6c4
			 0xaf6af29d, /// 0x6c8
			 0x7a2fe58b, /// 0x6cc
			 0x65327276, /// 0x6d0
			 0x0e7a70b1, /// 0x6d4
			 0x861089b3, /// 0x6d8
			 0x906038f9, /// 0x6dc
			 0x2ab99b16, /// 0x6e0
			 0x58660853, /// 0x6e4
			 0xfa28f3b6, /// 0x6e8
			 0xdc94b619, /// 0x6ec
			 0x65fa3f30, /// 0x6f0
			 0x29ee0c0d, /// 0x6f4
			 0x26fb39f3, /// 0x6f8
			 0x33f61c04, /// 0x6fc
			 0xac99354f, /// 0x700
			 0x775752af, /// 0x704
			 0x70c8d689, /// 0x708
			 0x3ca27192, /// 0x70c
			 0xa9924dfc, /// 0x710
			 0xb4bdbedf, /// 0x714
			 0x211c8431, /// 0x718
			 0x44b0f442, /// 0x71c
			 0xeb2b7040, /// 0x720
			 0x0ad5202d, /// 0x724
			 0xa613c763, /// 0x728
			 0xe44c37f5, /// 0x72c
			 0x5f82e207, /// 0x730
			 0xab5441df, /// 0x734
			 0x1687cb4f, /// 0x738
			 0xcb0326f3, /// 0x73c
			 0xd5f07772, /// 0x740
			 0x5fc54711, /// 0x744
			 0xd9f01af8, /// 0x748
			 0xdabc3ab7, /// 0x74c
			 0x9a74a6bc, /// 0x750
			 0xa83f42de, /// 0x754
			 0x058419c6, /// 0x758
			 0xaeebaaaa, /// 0x75c
			 0xdd47b35b, /// 0x760
			 0x545c9e86, /// 0x764
			 0xef9947ac, /// 0x768
			 0x0094660c, /// 0x76c
			 0x01ed0a29, /// 0x770
			 0x868d8d1e, /// 0x774
			 0x57b7dfde, /// 0x778
			 0xd554dfeb, /// 0x77c
			 0x9044df11, /// 0x780
			 0xa8a35c6a, /// 0x784
			 0x4be4a12c, /// 0x788
			 0xb1a42177, /// 0x78c
			 0xdb37f003, /// 0x790
			 0xc73e54df, /// 0x794
			 0xc3f959ff, /// 0x798
			 0x95bfe84b, /// 0x79c
			 0xd9d04ae9, /// 0x7a0
			 0x45c219cc, /// 0x7a4
			 0x6fcbdd1e, /// 0x7a8
			 0xf3207d5d, /// 0x7ac
			 0x0f7d6fee, /// 0x7b0
			 0xa90d80c7, /// 0x7b4
			 0x559ff1ee, /// 0x7b8
			 0xb6307c18, /// 0x7bc
			 0x1cc2281c, /// 0x7c0
			 0x4b3ff167, /// 0x7c4
			 0x692b50d3, /// 0x7c8
			 0xefae45b3, /// 0x7cc
			 0x06361d6f, /// 0x7d0
			 0x51cf27e8, /// 0x7d4
			 0xe81fe125, /// 0x7d8
			 0xda4eef65, /// 0x7dc
			 0x7341d730, /// 0x7e0
			 0x3b509a86, /// 0x7e4
			 0xd9ba7a32, /// 0x7e8
			 0xa64fdf38, /// 0x7ec
			 0x54bfcf5b, /// 0x7f0
			 0x6fb28846, /// 0x7f4
			 0x1422fab8, /// 0x7f8
			 0x5172e897, /// 0x7fc
			 0x7cc0404e, /// 0x800
			 0x975d8b51, /// 0x804
			 0x9e52f03c, /// 0x808
			 0x4033d684, /// 0x80c
			 0xeb571e50, /// 0x810
			 0x0020d0ba, /// 0x814
			 0x6faf3298, /// 0x818
			 0x22efb251, /// 0x81c
			 0x537e2aa0, /// 0x820
			 0x4a87e87b, /// 0x824
			 0x7862cdfb, /// 0x828
			 0x9f7ddb7b, /// 0x82c
			 0x932ee7d6, /// 0x830
			 0xb215c1eb, /// 0x834
			 0xf5fcd31c, /// 0x838
			 0xf244f580, /// 0x83c
			 0x8a8580a3, /// 0x840
			 0x205c731b, /// 0x844
			 0x19e174fa, /// 0x848
			 0xe66addf0, /// 0x84c
			 0xc8784eda, /// 0x850
			 0x144a8f9f, /// 0x854
			 0x630fe39e, /// 0x858
			 0xeeeadc7e, /// 0x85c
			 0x511261cc, /// 0x860
			 0x7f27225f, /// 0x864
			 0x02a87659, /// 0x868
			 0x9caafe5c, /// 0x86c
			 0xf83d86c9, /// 0x870
			 0xbabcc9cc, /// 0x874
			 0x5b6a2e10, /// 0x878
			 0x89a40f06, /// 0x87c
			 0x6203fa83, /// 0x880
			 0xa71bee6d, /// 0x884
			 0x1bf9b159, /// 0x888
			 0x95914805, /// 0x88c
			 0x831058bc, /// 0x890
			 0x6994d418, /// 0x894
			 0xe7bc5f75, /// 0x898
			 0x8e4535de, /// 0x89c
			 0xa142dec4, /// 0x8a0
			 0xb471169f, /// 0x8a4
			 0x5cd386ec, /// 0x8a8
			 0xab549167, /// 0x8ac
			 0xabdd0df5, /// 0x8b0
			 0x5837f85c, /// 0x8b4
			 0xb592bcd8, /// 0x8b8
			 0x40880108, /// 0x8bc
			 0x5ac7a0b1, /// 0x8c0
			 0xef301df6, /// 0x8c4
			 0x550fcfd2, /// 0x8c8
			 0x4b95d147, /// 0x8cc
			 0x469ff398, /// 0x8d0
			 0xbb47b27e, /// 0x8d4
			 0xd8170c1e, /// 0x8d8
			 0xa9bbf6d5, /// 0x8dc
			 0x9858c054, /// 0x8e0
			 0x02b754fd, /// 0x8e4
			 0xfddf7b48, /// 0x8e8
			 0xb5b20601, /// 0x8ec
			 0xbdcc1407, /// 0x8f0
			 0x45006487, /// 0x8f4
			 0xfc12b3bf, /// 0x8f8
			 0xbb0c5b63, /// 0x8fc
			 0xef498ae1, /// 0x900
			 0xdcf1ce69, /// 0x904
			 0x93ef518f, /// 0x908
			 0xcb0155a2, /// 0x90c
			 0x8f8d8e84, /// 0x910
			 0xdf51a414, /// 0x914
			 0x8af36125, /// 0x918
			 0xb9752b52, /// 0x91c
			 0x105a2975, /// 0x920
			 0x8de51d5f, /// 0x924
			 0x9261fc87, /// 0x928
			 0x549f70b6, /// 0x92c
			 0x73acf57b, /// 0x930
			 0x5471a40a, /// 0x934
			 0x2d083cbd, /// 0x938
			 0xa58981ce, /// 0x93c
			 0x3dfbc3ec, /// 0x940
			 0x75bc5336, /// 0x944
			 0xae3aa21b, /// 0x948
			 0x51afbd30, /// 0x94c
			 0x7e270d16, /// 0x950
			 0xccc07158, /// 0x954
			 0xbc8ea414, /// 0x958
			 0xf3040a09, /// 0x95c
			 0x9c21a8da, /// 0x960
			 0xd4f7e15a, /// 0x964
			 0x727c1107, /// 0x968
			 0x51cbd2f5, /// 0x96c
			 0x7c483342, /// 0x970
			 0xb7ed2700, /// 0x974
			 0xa1b60e54, /// 0x978
			 0x71fab683, /// 0x97c
			 0xff6448a1, /// 0x980
			 0x692d17f8, /// 0x984
			 0xad5677ac, /// 0x988
			 0x72a07215, /// 0x98c
			 0x2a60cbbe, /// 0x990
			 0xbc908bef, /// 0x994
			 0x18ff6988, /// 0x998
			 0xecfc034e, /// 0x99c
			 0x239bb915, /// 0x9a0
			 0xc7e95079, /// 0x9a4
			 0x2b3c6c8e, /// 0x9a8
			 0x1b1d6dc0, /// 0x9ac
			 0x977847fd, /// 0x9b0
			 0x41c4aecd, /// 0x9b4
			 0xab36b87f, /// 0x9b8
			 0x87e0e8d2, /// 0x9bc
			 0x40a4860d, /// 0x9c0
			 0x76523625, /// 0x9c4
			 0xb6acfca3, /// 0x9c8
			 0x3385a61b, /// 0x9cc
			 0xc40399b6, /// 0x9d0
			 0x3744b3c1, /// 0x9d4
			 0xde0e7ccd, /// 0x9d8
			 0xf18cb2d1, /// 0x9dc
			 0xfe268cb7, /// 0x9e0
			 0x64744f8a, /// 0x9e4
			 0x45240ab0, /// 0x9e8
			 0x2ab36e63, /// 0x9ec
			 0xde9f1e1a, /// 0x9f0
			 0x44d48b47, /// 0x9f4
			 0x6bf63777, /// 0x9f8
			 0x0817d8b4, /// 0x9fc
			 0xb589bd22, /// 0xa00
			 0x144d82a9, /// 0xa04
			 0x1a648862, /// 0xa08
			 0x8e3cf5c8, /// 0xa0c
			 0xcf315348, /// 0xa10
			 0xcb6ad302, /// 0xa14
			 0x77c8681c, /// 0xa18
			 0x2a26b23b, /// 0xa1c
			 0xf8516f4d, /// 0xa20
			 0xd8e5fabd, /// 0xa24
			 0x760196b7, /// 0xa28
			 0xa4a90fa6, /// 0xa2c
			 0x508931bb, /// 0xa30
			 0x7defdead, /// 0xa34
			 0x26aec8e7, /// 0xa38
			 0x2dcc258f, /// 0xa3c
			 0x2ef8023e, /// 0xa40
			 0x84157751, /// 0xa44
			 0xaaea4941, /// 0xa48
			 0xdd7f349a, /// 0xa4c
			 0x73c13707, /// 0xa50
			 0x59629ddc, /// 0xa54
			 0x1d1839aa, /// 0xa58
			 0x7fcb5d30, /// 0xa5c
			 0x83f0b5d4, /// 0xa60
			 0x5122bb9b, /// 0xa64
			 0xfc471f22, /// 0xa68
			 0x63b27986, /// 0xa6c
			 0xd6ed3207, /// 0xa70
			 0xccfe9c90, /// 0xa74
			 0xd85cf8a1, /// 0xa78
			 0x6ff09c31, /// 0xa7c
			 0xed56556d, /// 0xa80
			 0xceca4561, /// 0xa84
			 0xe88f468d, /// 0xa88
			 0x798171d7, /// 0xa8c
			 0xec99cad2, /// 0xa90
			 0xca306b13, /// 0xa94
			 0xe522be96, /// 0xa98
			 0x672a8a4d, /// 0xa9c
			 0x957d89d3, /// 0xaa0
			 0x82975ffe, /// 0xaa4
			 0x04fc3eae, /// 0xaa8
			 0x2326155e, /// 0xaac
			 0x15b1bcbb, /// 0xab0
			 0x268aaf5e, /// 0xab4
			 0x77adf67d, /// 0xab8
			 0xdf0946b0, /// 0xabc
			 0xb9d1cf3c, /// 0xac0
			 0x9804668a, /// 0xac4
			 0x2f36620a, /// 0xac8
			 0x4ca25e83, /// 0xacc
			 0x6990a976, /// 0xad0
			 0x890de0ac, /// 0xad4
			 0x8fe5713a, /// 0xad8
			 0xb863f5da, /// 0xadc
			 0x4afdea38, /// 0xae0
			 0x383c557c, /// 0xae4
			 0x3084d6a3, /// 0xae8
			 0x5a866fba, /// 0xaec
			 0xd24c248c, /// 0xaf0
			 0x0c739a90, /// 0xaf4
			 0x5e5fb6ca, /// 0xaf8
			 0x18c56b76, /// 0xafc
			 0xf69e856d, /// 0xb00
			 0x0941b713, /// 0xb04
			 0xe922b86a, /// 0xb08
			 0x32a155ab, /// 0xb0c
			 0x87162b37, /// 0xb10
			 0x52dafd5a, /// 0xb14
			 0x7132db76, /// 0xb18
			 0x46964248, /// 0xb1c
			 0xbf5354e1, /// 0xb20
			 0xb324e746, /// 0xb24
			 0x787d2e1a, /// 0xb28
			 0x9a523fa4, /// 0xb2c
			 0xbe4f7eb9, /// 0xb30
			 0xa0098f2b, /// 0xb34
			 0xd460570c, /// 0xb38
			 0x362f2562, /// 0xb3c
			 0x1f46a19c, /// 0xb40
			 0x6cb3b660, /// 0xb44
			 0x5a5e3944, /// 0xb48
			 0x60aaa5ce, /// 0xb4c
			 0xbf742c8c, /// 0xb50
			 0x7571a9b2, /// 0xb54
			 0x0556e894, /// 0xb58
			 0x2ef56ad5, /// 0xb5c
			 0xf6f77d22, /// 0xb60
			 0xc61d705d, /// 0xb64
			 0x9dd19d4f, /// 0xb68
			 0xb56a1001, /// 0xb6c
			 0x9a90c526, /// 0xb70
			 0x16bf131d, /// 0xb74
			 0x1052f1c8, /// 0xb78
			 0x5c8ef5c2, /// 0xb7c
			 0x0e7c87da, /// 0xb80
			 0x8c32c162, /// 0xb84
			 0x129d3a74, /// 0xb88
			 0xb5278372, /// 0xb8c
			 0xbc6bc8fc, /// 0xb90
			 0x8be88976, /// 0xb94
			 0x959e34fd, /// 0xb98
			 0xeb952f83, /// 0xb9c
			 0x45932173, /// 0xba0
			 0x9d5dae86, /// 0xba4
			 0x0575b97d, /// 0xba8
			 0xf1975da1, /// 0xbac
			 0x871ec0da, /// 0xbb0
			 0xb101546a, /// 0xbb4
			 0xd5edf7b8, /// 0xbb8
			 0x34228db5, /// 0xbbc
			 0xd8e83921, /// 0xbc0
			 0x3703bce0, /// 0xbc4
			 0x445c1f9c, /// 0xbc8
			 0x1ddf689a, /// 0xbcc
			 0x39f43b2f, /// 0xbd0
			 0xc3afd137, /// 0xbd4
			 0x6e37c6e2, /// 0xbd8
			 0xe75a8a01, /// 0xbdc
			 0x3bf70027, /// 0xbe0
			 0x48642b39, /// 0xbe4
			 0xcae1d91a, /// 0xbe8
			 0xb291e1f0, /// 0xbec
			 0x7d9035b5, /// 0xbf0
			 0x7b7aa3d8, /// 0xbf4
			 0xbc94a918, /// 0xbf8
			 0xcd126eab, /// 0xbfc
			 0xd8584533, /// 0xc00
			 0xe7b84c4f, /// 0xc04
			 0x36c1b828, /// 0xc08
			 0x77136a8f, /// 0xc0c
			 0x0f872d9d, /// 0xc10
			 0x210f60b5, /// 0xc14
			 0x57c6f31f, /// 0xc18
			 0xb36dde9d, /// 0xc1c
			 0x81051a45, /// 0xc20
			 0x08a9332a, /// 0xc24
			 0xfb16d77d, /// 0xc28
			 0x856cef39, /// 0xc2c
			 0xa1f3deb4, /// 0xc30
			 0xc57113f1, /// 0xc34
			 0xa0657663, /// 0xc38
			 0x1fa167a2, /// 0xc3c
			 0x3936edbc, /// 0xc40
			 0x350f15de, /// 0xc44
			 0x8b87fcec, /// 0xc48
			 0xaccc2015, /// 0xc4c
			 0x37ffb1e4, /// 0xc50
			 0x6261d9c7, /// 0xc54
			 0x7004b587, /// 0xc58
			 0x7e8829a3, /// 0xc5c
			 0xeff085b5, /// 0xc60
			 0x68f3c0cf, /// 0xc64
			 0x162b2e3b, /// 0xc68
			 0x6e28d8f5, /// 0xc6c
			 0x6b88471d, /// 0xc70
			 0x51efc7f1, /// 0xc74
			 0x6e34c024, /// 0xc78
			 0x8b16986b, /// 0xc7c
			 0xbcae736e, /// 0xc80
			 0x6be4e5ce, /// 0xc84
			 0xda1a1c4c, /// 0xc88
			 0xbbc5a9a1, /// 0xc8c
			 0x66932650, /// 0xc90
			 0xf74dfcb9, /// 0xc94
			 0x5b5fab93, /// 0xc98
			 0x2c2f58bf, /// 0xc9c
			 0x404968de, /// 0xca0
			 0x5b05fd71, /// 0xca4
			 0x0a08c242, /// 0xca8
			 0x4d216301, /// 0xcac
			 0xc1099107, /// 0xcb0
			 0xb3e382a8, /// 0xcb4
			 0x2b5ff514, /// 0xcb8
			 0xb42cc4e2, /// 0xcbc
			 0xa68f4052, /// 0xcc0
			 0x28c0f332, /// 0xcc4
			 0x7b8078a9, /// 0xcc8
			 0x61b0c9f0, /// 0xccc
			 0xfdb12a6e, /// 0xcd0
			 0xeff2f2af, /// 0xcd4
			 0x13015a80, /// 0xcd8
			 0x67337acb, /// 0xcdc
			 0x51fb325c, /// 0xce0
			 0x7d94ee50, /// 0xce4
			 0x51f17428, /// 0xce8
			 0x4a542366, /// 0xcec
			 0xf5469292, /// 0xcf0
			 0x324b9e50, /// 0xcf4
			 0x3b71180b, /// 0xcf8
			 0xe18d825a, /// 0xcfc
			 0x40e6b768, /// 0xd00
			 0xb5d12a74, /// 0xd04
			 0xc03d0c92, /// 0xd08
			 0x58bd254f, /// 0xd0c
			 0xcfce8d4b, /// 0xd10
			 0x5f596730, /// 0xd14
			 0x6f56566f, /// 0xd18
			 0x73b91f9d, /// 0xd1c
			 0x4f91d264, /// 0xd20
			 0xea7f02f9, /// 0xd24
			 0x9af62883, /// 0xd28
			 0xb1dd328b, /// 0xd2c
			 0xefcb941b, /// 0xd30
			 0x90a8c21f, /// 0xd34
			 0x90a53cc1, /// 0xd38
			 0xfd859ae9, /// 0xd3c
			 0x2fd230e8, /// 0xd40
			 0xe38607e8, /// 0xd44
			 0xb5a5c7d5, /// 0xd48
			 0x3229c1bd, /// 0xd4c
			 0x24f3e426, /// 0xd50
			 0x78c66234, /// 0xd54
			 0x9b24c8f3, /// 0xd58
			 0xd9722200, /// 0xd5c
			 0x56d4b26a, /// 0xd60
			 0x6d8871dd, /// 0xd64
			 0xfe9ca180, /// 0xd68
			 0x9350c99d, /// 0xd6c
			 0x2a287c08, /// 0xd70
			 0xc6b2c58a, /// 0xd74
			 0xa8ab78c9, /// 0xd78
			 0x3e3a6c2a, /// 0xd7c
			 0x79b8fa4c, /// 0xd80
			 0x4beabcf8, /// 0xd84
			 0x72b1f722, /// 0xd88
			 0x583c5620, /// 0xd8c
			 0xdfffbb97, /// 0xd90
			 0x748e31d9, /// 0xd94
			 0xaaeab960, /// 0xd98
			 0x2eb6daee, /// 0xd9c
			 0x0c08c092, /// 0xda0
			 0xa64c31b5, /// 0xda4
			 0x3cdee3b4, /// 0xda8
			 0x47614e51, /// 0xdac
			 0x58ff95e9, /// 0xdb0
			 0xc1418aff, /// 0xdb4
			 0x6851606e, /// 0xdb8
			 0x993d4ff8, /// 0xdbc
			 0x4245c166, /// 0xdc0
			 0x187029a0, /// 0xdc4
			 0x424f1f99, /// 0xdc8
			 0x427b5758, /// 0xdcc
			 0x385d72a5, /// 0xdd0
			 0xe19bdc01, /// 0xdd4
			 0xb23c584c, /// 0xdd8
			 0x690b3b29, /// 0xddc
			 0x44c09559, /// 0xde0
			 0xcc327d13, /// 0xde4
			 0xb61bb5dc, /// 0xde8
			 0x865de976, /// 0xdec
			 0x0ea3eae5, /// 0xdf0
			 0xe2e6a275, /// 0xdf4
			 0x4f2a999f, /// 0xdf8
			 0x210b058a, /// 0xdfc
			 0xe76901ab, /// 0xe00
			 0x6b8d65af, /// 0xe04
			 0x2a1641ce, /// 0xe08
			 0xefbb09d6, /// 0xe0c
			 0x68dcd629, /// 0xe10
			 0xaba9e33e, /// 0xe14
			 0x6dedbbd6, /// 0xe18
			 0xfd0c5fae, /// 0xe1c
			 0xcdd19f1c, /// 0xe20
			 0x74811c88, /// 0xe24
			 0x7d402788, /// 0xe28
			 0xabb02ad2, /// 0xe2c
			 0x6a90be7c, /// 0xe30
			 0x8b504d51, /// 0xe34
			 0xcfa8fcfc, /// 0xe38
			 0x42210f9d, /// 0xe3c
			 0x6f0d4d01, /// 0xe40
			 0x6ecd77ef, /// 0xe44
			 0xd0ab11ba, /// 0xe48
			 0x8350a60d, /// 0xe4c
			 0x20f5fbf2, /// 0xe50
			 0xd0b43d9b, /// 0xe54
			 0xd11d1791, /// 0xe58
			 0xa54337b5, /// 0xe5c
			 0x57c260ad, /// 0xe60
			 0xfca6cc37, /// 0xe64
			 0x496c938b, /// 0xe68
			 0x0d3d5040, /// 0xe6c
			 0x0e5db44a, /// 0xe70
			 0x69ca7303, /// 0xe74
			 0x08b21080, /// 0xe78
			 0x5bc2c815, /// 0xe7c
			 0xf1af9a1e, /// 0xe80
			 0x80e912a0, /// 0xe84
			 0xc0231869, /// 0xe88
			 0xc3829ea9, /// 0xe8c
			 0xd2070120, /// 0xe90
			 0xf72f19b4, /// 0xe94
			 0xc9546da0, /// 0xe98
			 0x8acb6228, /// 0xe9c
			 0x89638b54, /// 0xea0
			 0x038425ca, /// 0xea4
			 0xf432dbdd, /// 0xea8
			 0x4e636c46, /// 0xeac
			 0xe5e39613, /// 0xeb0
			 0xd64a40e6, /// 0xeb4
			 0xff66d934, /// 0xeb8
			 0xf30be588, /// 0xebc
			 0x23230808, /// 0xec0
			 0x9e327222, /// 0xec4
			 0x38231091, /// 0xec8
			 0x0d73427d, /// 0xecc
			 0xd5c75460, /// 0xed0
			 0xd9b04550, /// 0xed4
			 0xf2c9a579, /// 0xed8
			 0x259d129b, /// 0xedc
			 0xaf1a109d, /// 0xee0
			 0x13b39f2e, /// 0xee4
			 0x009e62c7, /// 0xee8
			 0x504cee7f, /// 0xeec
			 0x6c56fec3, /// 0xef0
			 0xf481b4aa, /// 0xef4
			 0x35bfedab, /// 0xef8
			 0x5064378c, /// 0xefc
			 0x6edce3e3, /// 0xf00
			 0x1f0f6d1a, /// 0xf04
			 0x6f65681d, /// 0xf08
			 0x7cdb35e5, /// 0xf0c
			 0x8baa0439, /// 0xf10
			 0x705c0d45, /// 0xf14
			 0x72bc05df, /// 0xf18
			 0x5449932c, /// 0xf1c
			 0x8b5f06d0, /// 0xf20
			 0x25536a29, /// 0xf24
			 0x520b72dd, /// 0xf28
			 0x4b5b5f6d, /// 0xf2c
			 0x83ed5914, /// 0xf30
			 0x3fefa8c2, /// 0xf34
			 0xfde3e766, /// 0xf38
			 0x580c8a51, /// 0xf3c
			 0xad12a37d, /// 0xf40
			 0xc53d8c4b, /// 0xf44
			 0x88f588ba, /// 0xf48
			 0x3f1a4881, /// 0xf4c
			 0x258508f1, /// 0xf50
			 0x21e2c049, /// 0xf54
			 0x1da8d1ef, /// 0xf58
			 0x85e35c54, /// 0xf5c
			 0x3b7ab4d6, /// 0xf60
			 0xcca6a25b, /// 0xf64
			 0x30e71995, /// 0xf68
			 0x5113100d, /// 0xf6c
			 0x32ceb8a9, /// 0xf70
			 0xebef810b, /// 0xf74
			 0xab6d20d6, /// 0xf78
			 0x21e2a2de, /// 0xf7c
			 0x1f548dec, /// 0xf80
			 0x846bcd5f, /// 0xf84
			 0xee2e69b4, /// 0xf88
			 0x31c53009, /// 0xf8c
			 0x4ea5d754, /// 0xf90
			 0x3889cd74, /// 0xf94
			 0xa3000bab, /// 0xf98
			 0x86739c80, /// 0xf9c
			 0x1853313b, /// 0xfa0
			 0x512560c0, /// 0xfa4
			 0x28620139, /// 0xfa8
			 0xb17901f5, /// 0xfac
			 0x4dd70fbf, /// 0xfb0
			 0x954cb85c, /// 0xfb4
			 0x159e9f04, /// 0xfb8
			 0xf3a2517a, /// 0xfbc
			 0x74d16b5d, /// 0xfc0
			 0xe2ff7b83, /// 0xfc4
			 0xc822334b, /// 0xfc8
			 0x0c0c6613, /// 0xfcc
			 0x7e27d325, /// 0xfd0
			 0x5ea8d542, /// 0xfd4
			 0xb06a2d31, /// 0xfd8
			 0x654dfb5e, /// 0xfdc
			 0xe58f3536, /// 0xfe0
			 0xe248a40f, /// 0xfe4
			 0x7e1f11b7, /// 0xfe8
			 0xa7a36712, /// 0xfec
			 0x30cb9d2f, /// 0xff0
			 0xe0db99b0, /// 0xff4
			 0xdb6e945b, /// 0xff8
			 0xb29275d6 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xee2809a5, /// 0x0
			 0x28a65bd3, /// 0x4
			 0xd7931e3f, /// 0x8
			 0xb027013c, /// 0xc
			 0xda4832c2, /// 0x10
			 0xb32abb0e, /// 0x14
			 0x79304908, /// 0x18
			 0xe7e31fa7, /// 0x1c
			 0x49b6587a, /// 0x20
			 0x67508f22, /// 0x24
			 0x90ea6926, /// 0x28
			 0x35820eca, /// 0x2c
			 0x913469e2, /// 0x30
			 0x580c2adb, /// 0x34
			 0x981acdae, /// 0x38
			 0xcd0f5099, /// 0x3c
			 0x79f70d86, /// 0x40
			 0xd7df285a, /// 0x44
			 0x3de2119b, /// 0x48
			 0x16279b4e, /// 0x4c
			 0xac698210, /// 0x50
			 0xbbc1db04, /// 0x54
			 0x12012605, /// 0x58
			 0x139d90e1, /// 0x5c
			 0x8de1b06d, /// 0x60
			 0xf09b922c, /// 0x64
			 0xee90fb98, /// 0x68
			 0xab30c7f3, /// 0x6c
			 0x07aca606, /// 0x70
			 0x27581a2c, /// 0x74
			 0x3b9cadd6, /// 0x78
			 0x814acc8b, /// 0x7c
			 0xb0873662, /// 0x80
			 0xefb4f0d0, /// 0x84
			 0x0639ebc6, /// 0x88
			 0x0fce0070, /// 0x8c
			 0x8c6fb479, /// 0x90
			 0x692be2f5, /// 0x94
			 0x774b73c8, /// 0x98
			 0x14b01458, /// 0x9c
			 0xab482bb9, /// 0xa0
			 0x928d4bef, /// 0xa4
			 0xbdddad6e, /// 0xa8
			 0xb8bcbbfc, /// 0xac
			 0xa296cd06, /// 0xb0
			 0x0d49110b, /// 0xb4
			 0xa194d68d, /// 0xb8
			 0x920ef28e, /// 0xbc
			 0x2375b7fc, /// 0xc0
			 0x2066c347, /// 0xc4
			 0x6b7d4093, /// 0xc8
			 0xa12ccf8d, /// 0xcc
			 0xe9db9d92, /// 0xd0
			 0x584c0426, /// 0xd4
			 0xfe0e55b0, /// 0xd8
			 0x8778f361, /// 0xdc
			 0x32fb9957, /// 0xe0
			 0x9ca74177, /// 0xe4
			 0xf8d563f6, /// 0xe8
			 0x8f89f8f8, /// 0xec
			 0x4f7367c3, /// 0xf0
			 0xa1e85eb3, /// 0xf4
			 0xe91f8176, /// 0xf8
			 0x24612d29, /// 0xfc
			 0x14adbe84, /// 0x100
			 0xd23096df, /// 0x104
			 0xe7caab24, /// 0x108
			 0x66ce4a42, /// 0x10c
			 0x2819c4c0, /// 0x110
			 0x2c63277b, /// 0x114
			 0xe28fc377, /// 0x118
			 0xa8723f44, /// 0x11c
			 0xc62b5949, /// 0x120
			 0xe9fd5903, /// 0x124
			 0x63062016, /// 0x128
			 0x8dd707f1, /// 0x12c
			 0x6e42a166, /// 0x130
			 0xbd4f401f, /// 0x134
			 0x85966816, /// 0x138
			 0x63f19852, /// 0x13c
			 0xa86aad18, /// 0x140
			 0x6db6ed58, /// 0x144
			 0xabfc7351, /// 0x148
			 0x922220e9, /// 0x14c
			 0xe1be9ff7, /// 0x150
			 0xc58118ad, /// 0x154
			 0x8457bd0d, /// 0x158
			 0xe4259c23, /// 0x15c
			 0x5452dd4f, /// 0x160
			 0x177a6c5e, /// 0x164
			 0x4667f753, /// 0x168
			 0x37f2895f, /// 0x16c
			 0x5b787295, /// 0x170
			 0xfcdada6a, /// 0x174
			 0x14d44791, /// 0x178
			 0x66a8da56, /// 0x17c
			 0xa78faa6c, /// 0x180
			 0x79d83600, /// 0x184
			 0xeca7d6fe, /// 0x188
			 0x813a03bb, /// 0x18c
			 0xb148e2ec, /// 0x190
			 0x11843278, /// 0x194
			 0x14535e9f, /// 0x198
			 0x9310be80, /// 0x19c
			 0x5ed7e3cb, /// 0x1a0
			 0x2c404627, /// 0x1a4
			 0xdde49e43, /// 0x1a8
			 0x86e388db, /// 0x1ac
			 0x39b66b2a, /// 0x1b0
			 0xfd2c3404, /// 0x1b4
			 0xc265ab9e, /// 0x1b8
			 0x154ec85f, /// 0x1bc
			 0xd76fecb7, /// 0x1c0
			 0x6977e17f, /// 0x1c4
			 0x748b0450, /// 0x1c8
			 0xf8050be6, /// 0x1cc
			 0xdbfc7a4f, /// 0x1d0
			 0xfad289b8, /// 0x1d4
			 0xd3b58b79, /// 0x1d8
			 0xf86ce08f, /// 0x1dc
			 0x2b52e7f2, /// 0x1e0
			 0xbd09502e, /// 0x1e4
			 0x7a6e9cb3, /// 0x1e8
			 0x2ac4eb43, /// 0x1ec
			 0x2cab153f, /// 0x1f0
			 0xf2a84454, /// 0x1f4
			 0x9d0b375e, /// 0x1f8
			 0xcd37b4a2, /// 0x1fc
			 0x16c7ca35, /// 0x200
			 0xd38acf4b, /// 0x204
			 0x6dcb2f4c, /// 0x208
			 0xf9421d73, /// 0x20c
			 0xfa98b5d4, /// 0x210
			 0xa363365b, /// 0x214
			 0x63d91fbc, /// 0x218
			 0x4f394a40, /// 0x21c
			 0xdb14dbfa, /// 0x220
			 0x53820e77, /// 0x224
			 0x53081276, /// 0x228
			 0xaec70ede, /// 0x22c
			 0x7a9d7c4e, /// 0x230
			 0x84bfc770, /// 0x234
			 0x98b4f931, /// 0x238
			 0xac9553a1, /// 0x23c
			 0xf9bcbe75, /// 0x240
			 0x0b0aa0a8, /// 0x244
			 0xdb9e8385, /// 0x248
			 0x543fb9ea, /// 0x24c
			 0x36190a6e, /// 0x250
			 0x52528044, /// 0x254
			 0x5156c0b4, /// 0x258
			 0x7e1cd4f8, /// 0x25c
			 0x2017acf9, /// 0x260
			 0x2b17932c, /// 0x264
			 0x3b229ff5, /// 0x268
			 0xf4fa166f, /// 0x26c
			 0x9918b143, /// 0x270
			 0xf9aeb5c8, /// 0x274
			 0xfcb7ea53, /// 0x278
			 0xed05e674, /// 0x27c
			 0x45ea562a, /// 0x280
			 0xb0e32410, /// 0x284
			 0xf7354be7, /// 0x288
			 0x134a44e6, /// 0x28c
			 0x4a4b7402, /// 0x290
			 0x6f547752, /// 0x294
			 0x0959e7b5, /// 0x298
			 0xd6947175, /// 0x29c
			 0x0dc6ebe4, /// 0x2a0
			 0x9ab23bd5, /// 0x2a4
			 0xf5e4d47a, /// 0x2a8
			 0x952bb9c8, /// 0x2ac
			 0xffaa3331, /// 0x2b0
			 0xd65e7f4a, /// 0x2b4
			 0x8dd223e6, /// 0x2b8
			 0xd03a4727, /// 0x2bc
			 0x9adadd7d, /// 0x2c0
			 0x50dc3e00, /// 0x2c4
			 0x1ffcdd47, /// 0x2c8
			 0x5880bdec, /// 0x2cc
			 0xbf722b39, /// 0x2d0
			 0xd4814729, /// 0x2d4
			 0x021447a9, /// 0x2d8
			 0xddd311fc, /// 0x2dc
			 0x48d35999, /// 0x2e0
			 0x65a07855, /// 0x2e4
			 0x77c3330f, /// 0x2e8
			 0x56e5718c, /// 0x2ec
			 0xd0df38ba, /// 0x2f0
			 0xa23bcdcc, /// 0x2f4
			 0x96e684b7, /// 0x2f8
			 0xea7aef87, /// 0x2fc
			 0x02dc3e2e, /// 0x300
			 0xdc66f97c, /// 0x304
			 0x81097dff, /// 0x308
			 0x81ba420e, /// 0x30c
			 0xc07bb56d, /// 0x310
			 0xea84fc32, /// 0x314
			 0xb9956311, /// 0x318
			 0xcd6ea34e, /// 0x31c
			 0xd759fa25, /// 0x320
			 0x8364d6c7, /// 0x324
			 0x5c661a8e, /// 0x328
			 0x2f032c26, /// 0x32c
			 0x1edef100, /// 0x330
			 0xc7ab342d, /// 0x334
			 0x666377c6, /// 0x338
			 0xf793a26e, /// 0x33c
			 0x85ffde89, /// 0x340
			 0xef349630, /// 0x344
			 0x037a9738, /// 0x348
			 0xf8447951, /// 0x34c
			 0xa45057a2, /// 0x350
			 0x6f502479, /// 0x354
			 0x93709871, /// 0x358
			 0x728a5dcd, /// 0x35c
			 0xe8f7ce6a, /// 0x360
			 0x4fd077fd, /// 0x364
			 0x7ba6057e, /// 0x368
			 0x52258f67, /// 0x36c
			 0x1320ed89, /// 0x370
			 0x8e2e3c6e, /// 0x374
			 0x928a729a, /// 0x378
			 0xba9ae8f3, /// 0x37c
			 0x4fa17c82, /// 0x380
			 0xde55362d, /// 0x384
			 0x62c55cbc, /// 0x388
			 0x5b83715b, /// 0x38c
			 0x26d03dd8, /// 0x390
			 0xe538c476, /// 0x394
			 0xb276dbbf, /// 0x398
			 0xdeb40a30, /// 0x39c
			 0xede1c886, /// 0x3a0
			 0x6619b8d2, /// 0x3a4
			 0x9c26b8a6, /// 0x3a8
			 0xc1c506e9, /// 0x3ac
			 0xffed3bb3, /// 0x3b0
			 0xb548812b, /// 0x3b4
			 0x9c2daa72, /// 0x3b8
			 0xf924b616, /// 0x3bc
			 0xba74cd92, /// 0x3c0
			 0x14af7e2f, /// 0x3c4
			 0x37c6cea6, /// 0x3c8
			 0x55d83ea5, /// 0x3cc
			 0x3895d6c1, /// 0x3d0
			 0x8ce4f41e, /// 0x3d4
			 0xe8e92115, /// 0x3d8
			 0x776d4c1a, /// 0x3dc
			 0xf2cf0e01, /// 0x3e0
			 0xe07701e2, /// 0x3e4
			 0x9d89772f, /// 0x3e8
			 0x8500274d, /// 0x3ec
			 0xc8546754, /// 0x3f0
			 0x77573edf, /// 0x3f4
			 0x064f37d1, /// 0x3f8
			 0xf4f12408, /// 0x3fc
			 0xf5f95a58, /// 0x400
			 0xe4ab98b9, /// 0x404
			 0x10022ebc, /// 0x408
			 0xfc3e5b36, /// 0x40c
			 0xfc37a76e, /// 0x410
			 0xee1cae91, /// 0x414
			 0xe1479fcb, /// 0x418
			 0x1fb14e29, /// 0x41c
			 0xadf080c3, /// 0x420
			 0x2200aa01, /// 0x424
			 0x185f3eb6, /// 0x428
			 0x7ca60750, /// 0x42c
			 0x7eb0d379, /// 0x430
			 0x3ac3dba6, /// 0x434
			 0x36c713a3, /// 0x438
			 0xcf6a96fe, /// 0x43c
			 0x67d26ffb, /// 0x440
			 0x1a81192c, /// 0x444
			 0x731033ff, /// 0x448
			 0x4c0cb5cd, /// 0x44c
			 0x3da5cbcd, /// 0x450
			 0xcbf0cc34, /// 0x454
			 0xf80fc01d, /// 0x458
			 0x858d9423, /// 0x45c
			 0x3138e02b, /// 0x460
			 0xd9c08536, /// 0x464
			 0x8252b5e6, /// 0x468
			 0xe438dba6, /// 0x46c
			 0x82da9bf9, /// 0x470
			 0x4d7b3953, /// 0x474
			 0x262841a6, /// 0x478
			 0xe15c7ce8, /// 0x47c
			 0xa7ef7546, /// 0x480
			 0x8c1609b1, /// 0x484
			 0x14595a76, /// 0x488
			 0xb6ed5a57, /// 0x48c
			 0x143538ea, /// 0x490
			 0x3bcd6d93, /// 0x494
			 0x57d3cf63, /// 0x498
			 0x5b14b450, /// 0x49c
			 0x4bb9745b, /// 0x4a0
			 0x92cfb3da, /// 0x4a4
			 0x72cd5e6d, /// 0x4a8
			 0x3e3aa364, /// 0x4ac
			 0xd0581f04, /// 0x4b0
			 0x3c74102a, /// 0x4b4
			 0xf9596f17, /// 0x4b8
			 0x6a8cfbcd, /// 0x4bc
			 0x09426948, /// 0x4c0
			 0xf17afc42, /// 0x4c4
			 0xf2db6b34, /// 0x4c8
			 0x60a16c73, /// 0x4cc
			 0x5a38499f, /// 0x4d0
			 0x811e1363, /// 0x4d4
			 0xb7db803a, /// 0x4d8
			 0x1a0d2d4e, /// 0x4dc
			 0xe65aae7b, /// 0x4e0
			 0x47d89ef6, /// 0x4e4
			 0xefd5a41a, /// 0x4e8
			 0x2926ce84, /// 0x4ec
			 0xfb19d7de, /// 0x4f0
			 0x42a83e0d, /// 0x4f4
			 0x988feeb7, /// 0x4f8
			 0x4f31669b, /// 0x4fc
			 0xa334fb44, /// 0x500
			 0x7f49ef22, /// 0x504
			 0xf74a8ae4, /// 0x508
			 0x750e28b0, /// 0x50c
			 0x63586137, /// 0x510
			 0xc50e73eb, /// 0x514
			 0xb4de97c6, /// 0x518
			 0xfe9b9baa, /// 0x51c
			 0x704423b4, /// 0x520
			 0xc8dbdb58, /// 0x524
			 0xcab0cbab, /// 0x528
			 0x86a5e1d0, /// 0x52c
			 0xe8b8e39b, /// 0x530
			 0x9dddc7d5, /// 0x534
			 0x302ad0e0, /// 0x538
			 0x589b38bf, /// 0x53c
			 0xc2866674, /// 0x540
			 0xec7ee4df, /// 0x544
			 0xfc13cc20, /// 0x548
			 0x1bfacf6c, /// 0x54c
			 0xe7ee3947, /// 0x550
			 0xcac592aa, /// 0x554
			 0x6faa61f3, /// 0x558
			 0x77226300, /// 0x55c
			 0x95f089bc, /// 0x560
			 0xb7734fad, /// 0x564
			 0xd97671b2, /// 0x568
			 0xde07ff00, /// 0x56c
			 0x4f911bff, /// 0x570
			 0x46cb77be, /// 0x574
			 0xc3bc41c5, /// 0x578
			 0x17c7c4b9, /// 0x57c
			 0x47c6a7e2, /// 0x580
			 0xf13e573b, /// 0x584
			 0x4e3b5326, /// 0x588
			 0xe02c9c83, /// 0x58c
			 0x49502a4a, /// 0x590
			 0x799bcc5b, /// 0x594
			 0x35807f5c, /// 0x598
			 0x48941709, /// 0x59c
			 0x5316a358, /// 0x5a0
			 0x83bb7953, /// 0x5a4
			 0x88de156b, /// 0x5a8
			 0xa8134dc5, /// 0x5ac
			 0x9e37d1d2, /// 0x5b0
			 0xb1a2e614, /// 0x5b4
			 0xf355290b, /// 0x5b8
			 0xa5a5d39b, /// 0x5bc
			 0x6cd195f5, /// 0x5c0
			 0x7905323c, /// 0x5c4
			 0x11c83285, /// 0x5c8
			 0x4c8c58a3, /// 0x5cc
			 0x004bcd7a, /// 0x5d0
			 0x3168b312, /// 0x5d4
			 0x45774f58, /// 0x5d8
			 0x6fa4efab, /// 0x5dc
			 0x2f4ad9da, /// 0x5e0
			 0xf2d62bd6, /// 0x5e4
			 0x64271162, /// 0x5e8
			 0x95f8309c, /// 0x5ec
			 0xc6e420b8, /// 0x5f0
			 0x51bd5534, /// 0x5f4
			 0x717b0ad2, /// 0x5f8
			 0x1094bd48, /// 0x5fc
			 0xf2216875, /// 0x600
			 0xf0b299c3, /// 0x604
			 0x5c8db282, /// 0x608
			 0xe5496a6f, /// 0x60c
			 0xcd46d18d, /// 0x610
			 0x1dfaf9de, /// 0x614
			 0x3ad8ddac, /// 0x618
			 0x13e2cc98, /// 0x61c
			 0xd678193d, /// 0x620
			 0xfb00ee18, /// 0x624
			 0x03f6d718, /// 0x628
			 0xbe116405, /// 0x62c
			 0x6971dc3f, /// 0x630
			 0x09bbabd6, /// 0x634
			 0xe914344e, /// 0x638
			 0x41ab142a, /// 0x63c
			 0xb4fcbb19, /// 0x640
			 0xc7af7c42, /// 0x644
			 0xe33781cb, /// 0x648
			 0xe6dbe69c, /// 0x64c
			 0x4fdcdf05, /// 0x650
			 0xc5447361, /// 0x654
			 0xdf52118c, /// 0x658
			 0x6102aee7, /// 0x65c
			 0x6ea1cb29, /// 0x660
			 0x2ec6364b, /// 0x664
			 0x1a16e824, /// 0x668
			 0x92f407e1, /// 0x66c
			 0x4a8fcab0, /// 0x670
			 0xdf0ccb59, /// 0x674
			 0xbe268fa9, /// 0x678
			 0x3c5710da, /// 0x67c
			 0x36525529, /// 0x680
			 0xba96fe9d, /// 0x684
			 0xdefcd73d, /// 0x688
			 0x2a3c563d, /// 0x68c
			 0x3c211b5f, /// 0x690
			 0x5e08f13c, /// 0x694
			 0xdcbc9eaa, /// 0x698
			 0x27089452, /// 0x69c
			 0x3297447a, /// 0x6a0
			 0x215e7919, /// 0x6a4
			 0xbb2c5c65, /// 0x6a8
			 0x3b3aed4d, /// 0x6ac
			 0xfbbcc9da, /// 0x6b0
			 0x7db57505, /// 0x6b4
			 0xbf00f117, /// 0x6b8
			 0x60a95d59, /// 0x6bc
			 0x0760672c, /// 0x6c0
			 0x8f3f8c76, /// 0x6c4
			 0x518a1c94, /// 0x6c8
			 0x626c23ab, /// 0x6cc
			 0xdb482bbf, /// 0x6d0
			 0xf763603c, /// 0x6d4
			 0x03c29f22, /// 0x6d8
			 0x59f751a5, /// 0x6dc
			 0x8b324226, /// 0x6e0
			 0x87a27dc1, /// 0x6e4
			 0xada2e974, /// 0x6e8
			 0x2f00978c, /// 0x6ec
			 0xe3b367c7, /// 0x6f0
			 0xeb4bad1c, /// 0x6f4
			 0xd825c6e1, /// 0x6f8
			 0x9d7d198e, /// 0x6fc
			 0xc99ec908, /// 0x700
			 0x8c58c43c, /// 0x704
			 0x7b74db95, /// 0x708
			 0x915c1bee, /// 0x70c
			 0x4a0aaff5, /// 0x710
			 0xfe7efc16, /// 0x714
			 0x428f6120, /// 0x718
			 0xc16ca7bb, /// 0x71c
			 0xe9ae0320, /// 0x720
			 0x03e35c5d, /// 0x724
			 0x04925b6b, /// 0x728
			 0x42332cc4, /// 0x72c
			 0x3b8f3297, /// 0x730
			 0x89b1df4b, /// 0x734
			 0xebe8f1c8, /// 0x738
			 0x01b70ad4, /// 0x73c
			 0x6c5b5484, /// 0x740
			 0x78ba724f, /// 0x744
			 0x2dc05687, /// 0x748
			 0x6715aa60, /// 0x74c
			 0x8933ccb2, /// 0x750
			 0xf12e3aa9, /// 0x754
			 0x9e057c56, /// 0x758
			 0x2f9d02aa, /// 0x75c
			 0xd50072f7, /// 0x760
			 0x20142e4c, /// 0x764
			 0xf89b56e0, /// 0x768
			 0x3ddf60a2, /// 0x76c
			 0xea2a0f04, /// 0x770
			 0x7510e48f, /// 0x774
			 0x6e52c0cf, /// 0x778
			 0xb4852359, /// 0x77c
			 0xffd4d685, /// 0x780
			 0x5f7f9931, /// 0x784
			 0x3709105d, /// 0x788
			 0x0a1e74fd, /// 0x78c
			 0xbbfa316b, /// 0x790
			 0x8b590400, /// 0x794
			 0xc376bfbf, /// 0x798
			 0xb413006e, /// 0x79c
			 0xa3f8a162, /// 0x7a0
			 0x56033adb, /// 0x7a4
			 0xfa7806bb, /// 0x7a8
			 0x0ff8a56c, /// 0x7ac
			 0x2b7de22a, /// 0x7b0
			 0x0a80b94d, /// 0x7b4
			 0x0624399a, /// 0x7b8
			 0xe9c89c1b, /// 0x7bc
			 0x61ab0516, /// 0x7c0
			 0x1be883ef, /// 0x7c4
			 0x89150830, /// 0x7c8
			 0x612a9842, /// 0x7cc
			 0x73845248, /// 0x7d0
			 0x8e46c544, /// 0x7d4
			 0x7de46e3e, /// 0x7d8
			 0x7b33cb8d, /// 0x7dc
			 0x8e4a32ef, /// 0x7e0
			 0x61e59bbc, /// 0x7e4
			 0x5ad67ad9, /// 0x7e8
			 0xb579905d, /// 0x7ec
			 0x690a5692, /// 0x7f0
			 0x85a0681e, /// 0x7f4
			 0x6a1d308e, /// 0x7f8
			 0x5dffa045, /// 0x7fc
			 0xa6f9abbf, /// 0x800
			 0xb875800b, /// 0x804
			 0xf9479ccf, /// 0x808
			 0x4a025f39, /// 0x80c
			 0x473dd171, /// 0x810
			 0xe8e452f1, /// 0x814
			 0xbdb40456, /// 0x818
			 0x2c589ad0, /// 0x81c
			 0x110aa059, /// 0x820
			 0xa48d2213, /// 0x824
			 0x841b33f4, /// 0x828
			 0xf76e1007, /// 0x82c
			 0x717be8a1, /// 0x830
			 0x8be02ef8, /// 0x834
			 0xc32dea23, /// 0x838
			 0x917916d5, /// 0x83c
			 0x1218749e, /// 0x840
			 0x94357840, /// 0x844
			 0x3b658d39, /// 0x848
			 0x8f32da34, /// 0x84c
			 0xf157eff1, /// 0x850
			 0x39e395a2, /// 0x854
			 0x5576adf2, /// 0x858
			 0x51ff7616, /// 0x85c
			 0x50cfccb4, /// 0x860
			 0x04ee6732, /// 0x864
			 0xfcdbb6b9, /// 0x868
			 0x66be9802, /// 0x86c
			 0xbbe36580, /// 0x870
			 0xc096735c, /// 0x874
			 0x2083c59a, /// 0x878
			 0xf6cd206b, /// 0x87c
			 0xe7e385ad, /// 0x880
			 0xcfe74b17, /// 0x884
			 0xac8fe9eb, /// 0x888
			 0xf5fa6355, /// 0x88c
			 0x6db8018b, /// 0x890
			 0x2e853380, /// 0x894
			 0x12a8923b, /// 0x898
			 0x14023509, /// 0x89c
			 0xa0b4f6e2, /// 0x8a0
			 0xf0e7833f, /// 0x8a4
			 0x8fdb6ee6, /// 0x8a8
			 0x550598b2, /// 0x8ac
			 0x0cf25290, /// 0x8b0
			 0xee3fe547, /// 0x8b4
			 0xe4e670fd, /// 0x8b8
			 0x9fe1b09e, /// 0x8bc
			 0xbaa3963c, /// 0x8c0
			 0xa9798321, /// 0x8c4
			 0xa13eb7a8, /// 0x8c8
			 0xd85ad448, /// 0x8cc
			 0x8eb899d7, /// 0x8d0
			 0xca253484, /// 0x8d4
			 0xc267b2f1, /// 0x8d8
			 0x6d8ee7a7, /// 0x8dc
			 0x268efd3d, /// 0x8e0
			 0x8bf540f3, /// 0x8e4
			 0x7e8c4b4c, /// 0x8e8
			 0x8ecce1ec, /// 0x8ec
			 0x6c9cce78, /// 0x8f0
			 0x0bc90205, /// 0x8f4
			 0x86a42243, /// 0x8f8
			 0xbf561d31, /// 0x8fc
			 0x1c160065, /// 0x900
			 0x744d5cbc, /// 0x904
			 0xd48bd070, /// 0x908
			 0x6d3fa32f, /// 0x90c
			 0x825e28ae, /// 0x910
			 0xa5bdd9a3, /// 0x914
			 0x190ec52b, /// 0x918
			 0x49b6a9d8, /// 0x91c
			 0x62834386, /// 0x920
			 0xb9423083, /// 0x924
			 0x2adc8129, /// 0x928
			 0x2e711e8f, /// 0x92c
			 0x2b254d06, /// 0x930
			 0x3b84156f, /// 0x934
			 0x67362f2a, /// 0x938
			 0xdf17a744, /// 0x93c
			 0xfb6c4e1a, /// 0x940
			 0x124f7ea5, /// 0x944
			 0x6fd732af, /// 0x948
			 0x7597f977, /// 0x94c
			 0xbd162339, /// 0x950
			 0x5d97cacf, /// 0x954
			 0xbf2d912a, /// 0x958
			 0xd1f256f6, /// 0x95c
			 0xdf48ea32, /// 0x960
			 0xf69ce8f8, /// 0x964
			 0x579f6229, /// 0x968
			 0xa9387eba, /// 0x96c
			 0x49304a17, /// 0x970
			 0x3f428d5f, /// 0x974
			 0x50183f61, /// 0x978
			 0x69f5c9f0, /// 0x97c
			 0x1b585a40, /// 0x980
			 0xeec54a55, /// 0x984
			 0xb8070575, /// 0x988
			 0x029d631d, /// 0x98c
			 0xc4b62bcd, /// 0x990
			 0x42b67ae5, /// 0x994
			 0xb272359b, /// 0x998
			 0xb0f52fd4, /// 0x99c
			 0x58282394, /// 0x9a0
			 0xd4354273, /// 0x9a4
			 0x043de828, /// 0x9a8
			 0x0b3a5b32, /// 0x9ac
			 0x2286a36b, /// 0x9b0
			 0x398274f8, /// 0x9b4
			 0x28fa8bec, /// 0x9b8
			 0x3026dfd0, /// 0x9bc
			 0xe472a9e3, /// 0x9c0
			 0xee485def, /// 0x9c4
			 0xe24b76a3, /// 0x9c8
			 0xd3cbe8af, /// 0x9cc
			 0x437fe7c9, /// 0x9d0
			 0xd54b1ed5, /// 0x9d4
			 0xc4dadde5, /// 0x9d8
			 0x3ab05ba7, /// 0x9dc
			 0x488b8cac, /// 0x9e0
			 0xbb70548a, /// 0x9e4
			 0xa3be5b8d, /// 0x9e8
			 0x26baa27e, /// 0x9ec
			 0x6ff3331e, /// 0x9f0
			 0xcef6366c, /// 0x9f4
			 0x6fac597b, /// 0x9f8
			 0xbf71bec2, /// 0x9fc
			 0x918512ba, /// 0xa00
			 0x031bff79, /// 0xa04
			 0x2df297d4, /// 0xa08
			 0x42727e92, /// 0xa0c
			 0xf42c278d, /// 0xa10
			 0x8060dbfb, /// 0xa14
			 0x8d211396, /// 0xa18
			 0x7ea83955, /// 0xa1c
			 0x76d8570c, /// 0xa20
			 0xe87b4052, /// 0xa24
			 0xd9a964a8, /// 0xa28
			 0x2b3cac22, /// 0xa2c
			 0x0403ec92, /// 0xa30
			 0x4f89fc42, /// 0xa34
			 0xa07a1d3a, /// 0xa38
			 0x3aaca6ee, /// 0xa3c
			 0xd9e51d96, /// 0xa40
			 0xc27c6549, /// 0xa44
			 0xd1e1656d, /// 0xa48
			 0x3537c1a9, /// 0xa4c
			 0xe6a7b324, /// 0xa50
			 0xbc9e90f9, /// 0xa54
			 0x702da536, /// 0xa58
			 0x5655d7a8, /// 0xa5c
			 0x3ff794e9, /// 0xa60
			 0xb960cd01, /// 0xa64
			 0xfe4e30c9, /// 0xa68
			 0x3974e84d, /// 0xa6c
			 0xcb55a1ff, /// 0xa70
			 0xc99fdbc9, /// 0xa74
			 0x69fab270, /// 0xa78
			 0x2cfed785, /// 0xa7c
			 0x6424709b, /// 0xa80
			 0x4ed7b3c7, /// 0xa84
			 0x66deb279, /// 0xa88
			 0xcc286dd1, /// 0xa8c
			 0xd34c7550, /// 0xa90
			 0xc7b8e5e9, /// 0xa94
			 0x21f2d4fa, /// 0xa98
			 0x0055748f, /// 0xa9c
			 0x1b008720, /// 0xaa0
			 0x58ad1b56, /// 0xaa4
			 0xd8ef0ec7, /// 0xaa8
			 0x9608e430, /// 0xaac
			 0xd57baaef, /// 0xab0
			 0x4b386830, /// 0xab4
			 0x288fe92d, /// 0xab8
			 0xb7a6d825, /// 0xabc
			 0x4d2ee9d5, /// 0xac0
			 0x58daa592, /// 0xac4
			 0x325de014, /// 0xac8
			 0x02c55416, /// 0xacc
			 0x4399535b, /// 0xad0
			 0x87c91806, /// 0xad4
			 0x918623a8, /// 0xad8
			 0x76a304fd, /// 0xadc
			 0x38a9e1af, /// 0xae0
			 0x92f61108, /// 0xae4
			 0xaa73fdcd, /// 0xae8
			 0x03e28dad, /// 0xaec
			 0x9f764caf, /// 0xaf0
			 0xddbd9aad, /// 0xaf4
			 0xaf509988, /// 0xaf8
			 0x1ce41e4d, /// 0xafc
			 0x58c505e4, /// 0xb00
			 0x957c7d22, /// 0xb04
			 0xac2b34e5, /// 0xb08
			 0x7f53677a, /// 0xb0c
			 0xe71e73ed, /// 0xb10
			 0xe9414023, /// 0xb14
			 0xc2d0ec79, /// 0xb18
			 0xbafa970f, /// 0xb1c
			 0x5b140a95, /// 0xb20
			 0xffc5d1f0, /// 0xb24
			 0xb8d1ab9a, /// 0xb28
			 0x0de06a8d, /// 0xb2c
			 0xc6e0bc9a, /// 0xb30
			 0x1fc9ed40, /// 0xb34
			 0xd1e45e1c, /// 0xb38
			 0x42bb1682, /// 0xb3c
			 0xc9fab8b7, /// 0xb40
			 0x49b93713, /// 0xb44
			 0x6283b5cb, /// 0xb48
			 0xe9f66772, /// 0xb4c
			 0x8756f258, /// 0xb50
			 0x8a07454b, /// 0xb54
			 0x90614d43, /// 0xb58
			 0xc5946a45, /// 0xb5c
			 0xe1cb76b8, /// 0xb60
			 0x517f67ed, /// 0xb64
			 0x40f0946d, /// 0xb68
			 0x294f5003, /// 0xb6c
			 0x45e07536, /// 0xb70
			 0xa00befa4, /// 0xb74
			 0x3ad0a8ca, /// 0xb78
			 0xab6f9774, /// 0xb7c
			 0x23c7f7f9, /// 0xb80
			 0x8695819d, /// 0xb84
			 0x0f591ede, /// 0xb88
			 0x79f78771, /// 0xb8c
			 0x58bb8964, /// 0xb90
			 0xe6d99336, /// 0xb94
			 0xf6cf5759, /// 0xb98
			 0x97a8aa0d, /// 0xb9c
			 0xf6099b62, /// 0xba0
			 0x3cb94c82, /// 0xba4
			 0xda7948ed, /// 0xba8
			 0xb9fbcbad, /// 0xbac
			 0x85567a9a, /// 0xbb0
			 0xac26203c, /// 0xbb4
			 0x614f95be, /// 0xbb8
			 0x79364ff6, /// 0xbbc
			 0xa7b386a2, /// 0xbc0
			 0xe5e3bb59, /// 0xbc4
			 0x36f52264, /// 0xbc8
			 0x5b1e069a, /// 0xbcc
			 0x4092446b, /// 0xbd0
			 0xa07aae8e, /// 0xbd4
			 0x81fb2c72, /// 0xbd8
			 0x8ab42bd3, /// 0xbdc
			 0x08e439d2, /// 0xbe0
			 0xb55fed55, /// 0xbe4
			 0xf2c88a27, /// 0xbe8
			 0xe1ace6d5, /// 0xbec
			 0xbeff6305, /// 0xbf0
			 0xaadf33e1, /// 0xbf4
			 0xa419e525, /// 0xbf8
			 0x07f96545, /// 0xbfc
			 0x2773ad57, /// 0xc00
			 0xaa613aca, /// 0xc04
			 0xf7487ffe, /// 0xc08
			 0xc18017b3, /// 0xc0c
			 0x9a86696d, /// 0xc10
			 0xe052c448, /// 0xc14
			 0x1c9a9d7d, /// 0xc18
			 0xc9aa8e31, /// 0xc1c
			 0x4f234b5f, /// 0xc20
			 0xcf8d7981, /// 0xc24
			 0x0a8ba695, /// 0xc28
			 0x08b3f1b7, /// 0xc2c
			 0x6ab85a42, /// 0xc30
			 0x9c9296e5, /// 0xc34
			 0xdbd142ec, /// 0xc38
			 0x138e62d3, /// 0xc3c
			 0x1f9d8f2e, /// 0xc40
			 0xc00cce29, /// 0xc44
			 0x2873b4e6, /// 0xc48
			 0x29793b6d, /// 0xc4c
			 0x23683ca2, /// 0xc50
			 0xf4640b82, /// 0xc54
			 0x15ea2810, /// 0xc58
			 0xdb2109e1, /// 0xc5c
			 0x2ef6f56e, /// 0xc60
			 0xe31ab697, /// 0xc64
			 0x029fb7aa, /// 0xc68
			 0x2c67add3, /// 0xc6c
			 0xdb10f416, /// 0xc70
			 0x389a84a8, /// 0xc74
			 0x6efa7fc6, /// 0xc78
			 0xf32db31a, /// 0xc7c
			 0xbb911ee2, /// 0xc80
			 0xa269512c, /// 0xc84
			 0xab731b6c, /// 0xc88
			 0xa7251c26, /// 0xc8c
			 0x239dc7c0, /// 0xc90
			 0x642d2ce3, /// 0xc94
			 0x4642aedb, /// 0xc98
			 0x8147a7b0, /// 0xc9c
			 0xe4c974a8, /// 0xca0
			 0x23933fc4, /// 0xca4
			 0x71413f22, /// 0xca8
			 0x63ab2e1c, /// 0xcac
			 0xbb4acb9b, /// 0xcb0
			 0x2e518797, /// 0xcb4
			 0xc2bf79ad, /// 0xcb8
			 0x248ffe95, /// 0xcbc
			 0x54575617, /// 0xcc0
			 0x1a31dfce, /// 0xcc4
			 0x9d1a6d3d, /// 0xcc8
			 0x7b07d2ed, /// 0xccc
			 0x9c936388, /// 0xcd0
			 0x321d666c, /// 0xcd4
			 0x7686efbf, /// 0xcd8
			 0xdcdd671b, /// 0xcdc
			 0x9d927329, /// 0xce0
			 0xbd76e401, /// 0xce4
			 0x5812345d, /// 0xce8
			 0x871ac24f, /// 0xcec
			 0x1559b839, /// 0xcf0
			 0xfd4880ab, /// 0xcf4
			 0x24068c8f, /// 0xcf8
			 0x1a92aa39, /// 0xcfc
			 0xd80f1599, /// 0xd00
			 0xa4ff66d4, /// 0xd04
			 0x979cb8b8, /// 0xd08
			 0xc8ce0c22, /// 0xd0c
			 0xa47ded95, /// 0xd10
			 0xb699063f, /// 0xd14
			 0x5de99f51, /// 0xd18
			 0x85936f25, /// 0xd1c
			 0xd1ffbf27, /// 0xd20
			 0x66d79b52, /// 0xd24
			 0xeed2a65e, /// 0xd28
			 0x55b92be7, /// 0xd2c
			 0x83dc2145, /// 0xd30
			 0x101a8019, /// 0xd34
			 0x12266d97, /// 0xd38
			 0xae023b4f, /// 0xd3c
			 0x192a198e, /// 0xd40
			 0x12dca05a, /// 0xd44
			 0xc131cdf0, /// 0xd48
			 0xba8dc827, /// 0xd4c
			 0xa04593e9, /// 0xd50
			 0xcce480d2, /// 0xd54
			 0x6296e8f5, /// 0xd58
			 0xced9a173, /// 0xd5c
			 0x2e556fab, /// 0xd60
			 0x95b3445a, /// 0xd64
			 0x6e2ca9eb, /// 0xd68
			 0x0189d5ae, /// 0xd6c
			 0x45fcf769, /// 0xd70
			 0xe427862f, /// 0xd74
			 0x0f252e4c, /// 0xd78
			 0xabc683d0, /// 0xd7c
			 0x91228b60, /// 0xd80
			 0xc8067918, /// 0xd84
			 0xe84bb543, /// 0xd88
			 0x760d5a39, /// 0xd8c
			 0xb794300a, /// 0xd90
			 0xb47bc6d0, /// 0xd94
			 0x9225af94, /// 0xd98
			 0x92c04d08, /// 0xd9c
			 0x79e4e175, /// 0xda0
			 0x0306dbc8, /// 0xda4
			 0xa6a56c8d, /// 0xda8
			 0x121d0852, /// 0xdac
			 0x88989470, /// 0xdb0
			 0xb79448ce, /// 0xdb4
			 0xad79fde8, /// 0xdb8
			 0x2526e001, /// 0xdbc
			 0x56d68989, /// 0xdc0
			 0x51716e2e, /// 0xdc4
			 0x5bec4133, /// 0xdc8
			 0xedf42ab1, /// 0xdcc
			 0x1e12db92, /// 0xdd0
			 0x608952c5, /// 0xdd4
			 0x1a0d2c05, /// 0xdd8
			 0xb61fe6f1, /// 0xddc
			 0x5ef97992, /// 0xde0
			 0x83554cab, /// 0xde4
			 0x71ddd25a, /// 0xde8
			 0xb4043f9d, /// 0xdec
			 0xbc8d56d1, /// 0xdf0
			 0x813df3cf, /// 0xdf4
			 0xf82df244, /// 0xdf8
			 0x4cd5581c, /// 0xdfc
			 0x2a4b84f6, /// 0xe00
			 0x41350a92, /// 0xe04
			 0x56c30357, /// 0xe08
			 0x89ae98c2, /// 0xe0c
			 0xa5c03610, /// 0xe10
			 0xb99664ee, /// 0xe14
			 0x2c37dc63, /// 0xe18
			 0x58d03f49, /// 0xe1c
			 0xd289026c, /// 0xe20
			 0xd213e445, /// 0xe24
			 0x3ddfbb11, /// 0xe28
			 0xbe9aab1b, /// 0xe2c
			 0xe588a64a, /// 0xe30
			 0x20d1f000, /// 0xe34
			 0x1afe3697, /// 0xe38
			 0x4f4b7a2d, /// 0xe3c
			 0x2e74a3d8, /// 0xe40
			 0x4dcb3c21, /// 0xe44
			 0xb6b54703, /// 0xe48
			 0x4f4b3611, /// 0xe4c
			 0xf1ab3fb4, /// 0xe50
			 0xa495ccfb, /// 0xe54
			 0x1e388f83, /// 0xe58
			 0xc625625b, /// 0xe5c
			 0xfbd89418, /// 0xe60
			 0x71aee1d4, /// 0xe64
			 0x57e268b3, /// 0xe68
			 0x6df645d1, /// 0xe6c
			 0x3b1055b9, /// 0xe70
			 0x55a24173, /// 0xe74
			 0x9d72c266, /// 0xe78
			 0x1eb71154, /// 0xe7c
			 0x57378de3, /// 0xe80
			 0x451ac50c, /// 0xe84
			 0x93d50cdb, /// 0xe88
			 0x9733b9be, /// 0xe8c
			 0x22f55a4d, /// 0xe90
			 0x3743c8b2, /// 0xe94
			 0x7cf0a90d, /// 0xe98
			 0x55902e3b, /// 0xe9c
			 0x0e7010e2, /// 0xea0
			 0x92eaaeb1, /// 0xea4
			 0xa60adf53, /// 0xea8
			 0x1ace65c0, /// 0xeac
			 0xe29b3cb0, /// 0xeb0
			 0x2350c1fa, /// 0xeb4
			 0xa3a23cbc, /// 0xeb8
			 0xd3092e0c, /// 0xebc
			 0x8069d6c2, /// 0xec0
			 0x3da4ab22, /// 0xec4
			 0xd1ddb76e, /// 0xec8
			 0x0831672a, /// 0xecc
			 0xf7d66f45, /// 0xed0
			 0xf62d45b1, /// 0xed4
			 0x2b879565, /// 0xed8
			 0xece3eb6d, /// 0xedc
			 0x696378c9, /// 0xee0
			 0x5d8fa25b, /// 0xee4
			 0x51f0e649, /// 0xee8
			 0x28ceb213, /// 0xeec
			 0x7917bc51, /// 0xef0
			 0xc1fed3d4, /// 0xef4
			 0x84632afb, /// 0xef8
			 0xa3d37ac1, /// 0xefc
			 0x29ad1bf1, /// 0xf00
			 0x94f10226, /// 0xf04
			 0x3fefc953, /// 0xf08
			 0xe456dbaf, /// 0xf0c
			 0xef3a31aa, /// 0xf10
			 0x43696b6e, /// 0xf14
			 0xe622776c, /// 0xf18
			 0x4304f6f6, /// 0xf1c
			 0x473454f3, /// 0xf20
			 0x4e57ebde, /// 0xf24
			 0xab049331, /// 0xf28
			 0xf810d4ae, /// 0xf2c
			 0x934be90a, /// 0xf30
			 0xaa61253e, /// 0xf34
			 0xcedc2a4b, /// 0xf38
			 0xa4c3114e, /// 0xf3c
			 0x9524bf0d, /// 0xf40
			 0x58dc826f, /// 0xf44
			 0xe3c0d82b, /// 0xf48
			 0x07b45155, /// 0xf4c
			 0x4abe164a, /// 0xf50
			 0x2d90a987, /// 0xf54
			 0xed0ccd11, /// 0xf58
			 0x2ea9cfa2, /// 0xf5c
			 0xd38086db, /// 0xf60
			 0xb5184d55, /// 0xf64
			 0xf2c47299, /// 0xf68
			 0xce528d92, /// 0xf6c
			 0x08df759e, /// 0xf70
			 0xb5d83b8e, /// 0xf74
			 0x12e2a634, /// 0xf78
			 0x193b414e, /// 0xf7c
			 0xd4e7cf4e, /// 0xf80
			 0x1025419e, /// 0xf84
			 0x85fa539a, /// 0xf88
			 0xdc6421c1, /// 0xf8c
			 0x0cb79f4b, /// 0xf90
			 0x453bf315, /// 0xf94
			 0x746a4e4d, /// 0xf98
			 0x5ddbe620, /// 0xf9c
			 0x808a3691, /// 0xfa0
			 0x449a5ca0, /// 0xfa4
			 0x98cdcc37, /// 0xfa8
			 0x6883c60d, /// 0xfac
			 0xd85c5ccd, /// 0xfb0
			 0x1ae2a745, /// 0xfb4
			 0xab49df5f, /// 0xfb8
			 0x9b0b78f4, /// 0xfbc
			 0x14e344fb, /// 0xfc0
			 0xcbc48a53, /// 0xfc4
			 0xa2f2a455, /// 0xfc8
			 0xe72b85e9, /// 0xfcc
			 0x5668c801, /// 0xfd0
			 0x703a9aee, /// 0xfd4
			 0x2af23d65, /// 0xfd8
			 0x8b2edc41, /// 0xfdc
			 0x100a8eae, /// 0xfe0
			 0xcefead97, /// 0xfe4
			 0xe5fc89f1, /// 0xfe8
			 0x13cc9910, /// 0xfec
			 0x0eb1055b, /// 0xff0
			 0x8162be89, /// 0xff4
			 0x54cb0fba, /// 0xff8
			 0xb29f4eef /// last
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
			 0x00000560,
			 0x00000414,
			 0x00000338,
			 0x000000e0,
			 0x000006dc,
			 0x00000358,
			 0x00000254,
			 0x00000228,

			 /// vpu register f2
			 0x40000000,
			 0x41400000,
			 0x41980000,
			 0x41100000,
			 0x41000000,
			 0x40000000,
			 0x42000000,
			 0x41a80000,

			 /// vpu register f3
			 0x41980000,
			 0x41980000,
			 0x41c00000,
			 0x41a00000,
			 0x41c00000,
			 0x41800000,
			 0x40800000,
			 0x41900000,

			 /// vpu register f4
			 0x41800000,
			 0x41d80000,
			 0x41900000,
			 0x40400000,
			 0x41700000,
			 0x41100000,
			 0x41e80000,
			 0x41880000,

			 /// vpu register f5
			 0x41d80000,
			 0x41600000,
			 0x41500000,
			 0x40a00000,
			 0x41400000,
			 0x41700000,
			 0x41c00000,
			 0x41f00000,

			 /// vpu register f6
			 0x41e00000,
			 0x41300000,
			 0x40c00000,
			 0x40c00000,
			 0x40800000,
			 0x41700000,
			 0x41c80000,
			 0x41300000,

			 /// vpu register f7
			 0x41980000,
			 0x40e00000,
			 0x3f800000,
			 0x41900000,
			 0x41200000,
			 0x41a00000,
			 0x42000000,
			 0x42000000,

			 /// vpu register f8
			 0x41200000,
			 0x41e00000,
			 0x40c00000,
			 0x3f800000,
			 0x41500000,
			 0x3f800000,
			 0x41e00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41200000,
			 0x3f800000,
			 0x41e00000,
			 0x41a80000,
			 0x41e00000,
			 0x41f80000,
			 0x3f800000,
			 0x41c00000,

			 /// vpu register f10
			 0x41100000,
			 0x40c00000,
			 0x41300000,
			 0x41d80000,
			 0x42000000,
			 0x41200000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f11
			 0x41980000,
			 0x41d80000,
			 0x41600000,
			 0x41200000,
			 0x40400000,
			 0x40800000,
			 0x41880000,
			 0x41000000,

			 /// vpu register f12
			 0x41a80000,
			 0x41a00000,
			 0x41f80000,
			 0x40e00000,
			 0x41000000,
			 0x41d00000,
			 0x41600000,
			 0x40000000,

			 /// vpu register f13
			 0x41c80000,
			 0x41a80000,
			 0x41b00000,
			 0x41100000,
			 0x40e00000,
			 0x41400000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f14
			 0x41000000,
			 0x41a80000,
			 0x40c00000,
			 0x41f80000,
			 0x41400000,
			 0x41c00000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f15
			 0x40c00000,
			 0x40a00000,
			 0x41d00000,
			 0x41400000,
			 0x40800000,
			 0x40400000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f16
			 0x41980000,
			 0x41f80000,
			 0x41b00000,
			 0x41000000,
			 0x41a00000,
			 0x42000000,
			 0x41600000,
			 0x41400000,

			 /// vpu register f17
			 0x41e00000,
			 0x41c00000,
			 0x3f800000,
			 0x41300000,
			 0x41f80000,
			 0x40a00000,
			 0x41d80000,
			 0x3f800000,

			 /// vpu register f18
			 0x41e00000,
			 0x40e00000,
			 0x41d80000,
			 0x41c80000,
			 0x41b00000,
			 0x41800000,
			 0x41000000,
			 0x40c00000,

			 /// vpu register f19
			 0x41300000,
			 0x41f00000,
			 0x41900000,
			 0x41700000,
			 0x41e80000,
			 0x41900000,
			 0x41200000,
			 0x41700000,

			 /// vpu register f20
			 0x41980000,
			 0x40a00000,
			 0x41500000,
			 0x41880000,
			 0x3f800000,
			 0x41600000,
			 0x40000000,
			 0x41000000,

			 /// vpu register f21
			 0x40400000,
			 0x41e80000,
			 0x3f800000,
			 0x41700000,
			 0x40000000,
			 0x41880000,
			 0x41100000,
			 0x40000000,

			 /// vpu register f22
			 0x41d80000,
			 0x40a00000,
			 0x41a00000,
			 0x40400000,
			 0x41880000,
			 0x40800000,
			 0x40a00000,
			 0x41c80000,

			 /// vpu register f23
			 0x41f80000,
			 0x41700000,
			 0x40000000,
			 0x41000000,
			 0x41300000,
			 0x41700000,
			 0x41a00000,
			 0x41100000,

			 /// vpu register f24
			 0x41a80000,
			 0x41f80000,
			 0x40c00000,
			 0x41b00000,
			 0x41000000,
			 0x40000000,
			 0x41d00000,
			 0x40400000,

			 /// vpu register f25
			 0x41b80000,
			 0x42000000,
			 0x41b80000,
			 0x40e00000,
			 0x41a80000,
			 0x41f00000,
			 0x40c00000,
			 0x41c80000,

			 /// vpu register f26
			 0x41880000,
			 0x41d00000,
			 0x41900000,
			 0x41900000,
			 0x41100000,
			 0x41c80000,
			 0x41000000,
			 0x41b80000,

			 /// vpu register f27
			 0x40400000,
			 0x41500000,
			 0x41100000,
			 0x41700000,
			 0x40e00000,
			 0x41c00000,
			 0x41500000,
			 0x3f800000,

			 /// vpu register f28
			 0x40000000,
			 0x41600000,
			 0x40000000,
			 0x41c80000,
			 0x40c00000,
			 0x41c00000,
			 0x41b80000,
			 0x41e80000,

			 /// vpu register f29
			 0x41c00000,
			 0x42000000,
			 0x41880000,
			 0x41200000,
			 0x41e80000,
			 0x41a80000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f30
			 0x41100000,
			 0x41300000,
			 0x41400000,
			 0x41b80000,
			 0x41a00000,
			 0x41000000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f31
			 0x41900000,
			 0x40800000,
			 0x41400000,
			 0x41700000,
			 0x41e00000,
			 0x41a00000,
			 0x41880000,
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
		"fcvt.f11.ps f26, f12\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f17\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f11, f19\n"                              ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f5, f22\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f19, f14\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f3, f4\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f13, f24\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f27, f3\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f2, f25\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f19, f27\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f23\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f29, f27\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f12, f20\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f2, f24\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f4, f23\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f27, f27\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f21, f23\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f24, f24\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f6, f12\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f18, f16\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f2, f3\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f28, f18\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f16\n"                               ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f15, f27\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f10, f11\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f18\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f19, f0\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f28, f4\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f21, f21\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f20, f25\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f22, f7\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f13\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f27, f9\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f29, f21\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f5, f22\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f9, f8\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f22, f3\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f21, f4\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f10, f6\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f3\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f2, f21\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f30, f15\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f11, f6\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f10, f25\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f23, f5\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f15, f12\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f17\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f26, f23\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f9, f7\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f4, f5\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f16, f13\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f18, f25\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f25, f7\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f4, f28\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f22, f11\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f12, f14\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f11, f16\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f4\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f28, f7\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f5, f25\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f26, f7\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f29, f22\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f10\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f14, f30\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f9, f11\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f29, f13\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f7, f7\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f24, f28\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f4, f15\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f5, f5\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f6, f3\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f29, f27\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f27, f10\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f4, f26\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f27, f0\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f10, f4\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f3, f13\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f22, f8\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f21, f22\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f28, f27\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f17, f29\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f19, f3\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f14, f14\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f15, f16\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f30, f28\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f9, f25\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f3, f4\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f21, f13\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f18, f20\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f4, f26\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f5, f25\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f13, f25\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f3, f3\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f8, f4\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f11, f2\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f26, f4\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f25, f13\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f6, f29\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fcvt.f11.ps f21, f13\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
