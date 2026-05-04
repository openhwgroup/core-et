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
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00000
			 0x0e000003,                                                  // Trailing 1s:                                /// 00004
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00008
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0000c
			 0x4b000000,                                                  // 8388608.0                                   /// 00010
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00018
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0001c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00020
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00034
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0003c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00040
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00044
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x00000000,                                                  // zero                                        /// 00050
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00054
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00058
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0005c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00000000,                                                  // zero                                        /// 00064
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00068
			 0xbf028f5c,                                                  // -0.51                                       /// 0006c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00078
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0007c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00080
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0008c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00090
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00094
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000bc
			 0x33333333,                                                  // 4 random values                             /// 000c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ec
			 0x7f800000,                                                  // inf                                         /// 000f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00100
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00104
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00108
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00114
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0011c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00120
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0012c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00134
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0013c
			 0x33333333,                                                  // 4 random values                             /// 00140
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00148
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0014c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80011111,                                                  // -9.7958E-41                                 /// 00154
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x3f028f5c,                                                  // 0.51                                        /// 0015c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00160
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00164
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00168
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0016c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00174
			 0x0c780000,                                                  // Leading 1s:                                 /// 00178
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c780000,                                                  // Leading 1s:                                 /// 00188
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0018c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00194
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0019c
			 0xffc00001,                                                  // -signaling NaN                              /// 001a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0xbf028f5c,                                                  // -0.51                                       /// 001c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x3f028f5c,                                                  // 0.51                                        /// 001f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0xff800000,                                                  // -inf                                        /// 00204
			 0x80000000,                                                  // -zero                                       /// 00208
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00210
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00214
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00218
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0021c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00220
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00224
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00228
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00230
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00238
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0023c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00240
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00250
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00254
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00260
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00268
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00270
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00274
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00278
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0027c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00280
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00284
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00288
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0028c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00290
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00294
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00298
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0029c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a0
			 0x3f028f5c,                                                  // 0.51                                        /// 002a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002b0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 002c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0xffc00001,                                                  // -signaling NaN                              /// 002cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002ec
			 0xffc00001,                                                  // -signaling NaN                              /// 002f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00300
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00304
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00308
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0030c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00314
			 0x4b000000,                                                  // 8388608.0                                   /// 00318
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0031c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00324
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00328
			 0x80000000,                                                  // -zero                                       /// 0032c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00330
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00340
			 0x80000000,                                                  // -zero                                       /// 00344
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00348
			 0xff800000,                                                  // -inf                                        /// 0034c
			 0xff800000,                                                  // -inf                                        /// 00350
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0xff800000,                                                  // -inf                                        /// 00358
			 0x3f028f5c,                                                  // 0.51                                        /// 0035c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00360
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00368
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0036c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00370
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0037c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00380
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00384
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00388
			 0xbf028f5c,                                                  // -0.51                                       /// 0038c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00390
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0039c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e8
			 0x80000000,                                                  // -zero                                       /// 003ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00404
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00408
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00418
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0041c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00420
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00424
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00428
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0042c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00430
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00438
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0043c
			 0xcb000000,                                                  // -8388608.0                                  /// 00440
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00444
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0044c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00450
			 0x0e000007,                                                  // Trailing 1s:                                /// 00454
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00460
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00464
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0046c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00478
			 0x55555555,                                                  // 4 random values                             /// 0047c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00480
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00484
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00488
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0048c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00498
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 004d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00500
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00504
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00518
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0051c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00520
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00524
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0052c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00530
			 0xbf028f5c,                                                  // -0.51                                       /// 00534
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00538
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0053c
			 0x7fc00001,                                                  // signaling NaN                               /// 00540
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00548
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0054c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00550
			 0x3f028f5c,                                                  // 0.51                                        /// 00554
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00558
			 0xff800000,                                                  // -inf                                        /// 0055c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00560
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00564
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00568
			 0x7f800000,                                                  // inf                                         /// 0056c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00570
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00574
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00578
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0057c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00590
			 0x33333333,                                                  // 4 random values                             /// 00594
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005e8
			 0xcb000000,                                                  // -8388608.0                                  /// 005ec
			 0xcb000000,                                                  // -8388608.0                                  /// 005f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00600
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00604
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00608
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0060c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00610
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00618
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0061c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00620
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0e000007,                                                  // Trailing 1s:                                /// 00628
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00630
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00638
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0063c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00640
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00644
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00648
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00654
			 0x0e000003,                                                  // Trailing 1s:                                /// 00658
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0065c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00660
			 0xff800000,                                                  // -inf                                        /// 00664
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0066c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00674
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00678
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0067c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00680
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00684
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00688
			 0x7fc00001,                                                  // signaling NaN                               /// 0068c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00698
			 0x0e000003,                                                  // Trailing 1s:                                /// 0069c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80000000,                                                  // -zero                                       /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f0
			 0xcb000000,                                                  // -8388608.0                                  /// 006f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00700
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00708
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0070c
			 0x33333333,                                                  // 4 random values                             /// 00710
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00718
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0071c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00720
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00724
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00728
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0072c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00730
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00734
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0073c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00748
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0074c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00750
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0075c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00764
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0076c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00770
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00774
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00778
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0077c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00780
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00784
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00788
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00794
			 0x33333333,                                                  // 4 random values                             /// 00798
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0xffc00001,                                                  // -signaling NaN                              /// 007a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0xcb000000,                                                  // -8388608.0                                  /// 007b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 007e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00800
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00808
			 0x0c780000,                                                  // Leading 1s:                                 /// 0080c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00810
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00814
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0081c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00820
			 0xcb000000,                                                  // -8388608.0                                  /// 00824
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00828
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x7fc00001,                                                  // signaling NaN                               /// 00830
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00838
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0084c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00850
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00854
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00858
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00868
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00870
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00878
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00884
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00888
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0088c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00894
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00898
			 0x7fc00001,                                                  // signaling NaN                               /// 0089c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008e0
			 0xbf028f5c,                                                  // -0.51                                       /// 008e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0c780000,                                                  // Leading 1s:                                 /// 0090c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00910
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00914
			 0x80000000,                                                  // -zero                                       /// 00918
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0091c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00928
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0092c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00934
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00938
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0093c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00940
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00944
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00948
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0094c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00954
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00958
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0095c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00964
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00011111,                                                  // 9.7958E-41                                  /// 0096c
			 0x4b000000,                                                  // 8388608.0                                   /// 00970
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00974
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00984
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00990
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00994
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00998
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0099c
			 0x7f800000,                                                  // inf                                         /// 009a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x3f028f5c,                                                  // 0.51                                        /// 009c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009c8
			 0x4b000000,                                                  // 8388608.0                                   /// 009cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000000,                                                  // zero                                        /// 00a04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a08
			 0xcb000000,                                                  // -8388608.0                                  /// 00a0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a28
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a74
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a80
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x7f800000,                                                  // inf                                         /// 00a90
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aa8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ab4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ac0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ac8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00acc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00adc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ae4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00af0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b08
			 0x80000000,                                                  // -zero                                       /// 00b0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x7f800000,                                                  // inf                                         /// 00b78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b80
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b84
			 0xbf028f5c,                                                  // -0.51                                       /// 00b88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bc4
			 0x00000000,                                                  // zero                                        /// 00bc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bcc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bdc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00be0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00be4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00be8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bf0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c14
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xffc00001,                                                  // -signaling NaN                              /// 00c48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c54
			 0x55555555,                                                  // 4 random values                             /// 00c58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c8c
			 0x33333333,                                                  // 4 random values                             /// 00c90
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0xff800000,                                                  // -inf                                        /// 00cb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cdc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d18
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0x3f028f5c,                                                  // 0.51                                        /// 00d4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d8c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00da0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dc4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ddc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00de0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00de4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e04
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e10
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e14
			 0xffc00001,                                                  // -signaling NaN                              /// 00e18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e7c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e88
			 0xff800000,                                                  // -inf                                        /// 00e8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e94
			 0xcb000000,                                                  // -8388608.0                                  /// 00e98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ea4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ea8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x55555555,                                                  // 4 random values                             /// 00eb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ecc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ed4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00edc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ef0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00efc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x33333333,                                                  // 4 random values                             /// 00f08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f10
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f30
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f4c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f60
			 0x7f800000,                                                  // inf                                         /// 00f64
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f70
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f84
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0xff800000,                                                  // -inf                                        /// 00fa8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fe4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fe8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0x80002000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x32e925f3, /// 0x0
			 0x708e4685, /// 0x4
			 0xc17ab746, /// 0x8
			 0xe9747855, /// 0xc
			 0x09e82ac5, /// 0x10
			 0xd154b7af, /// 0x14
			 0x6b24d0f0, /// 0x18
			 0x4396d6c2, /// 0x1c
			 0x3436e8fd, /// 0x20
			 0x74210b37, /// 0x24
			 0xbf5e4de9, /// 0x28
			 0xb477c8ef, /// 0x2c
			 0x8b4b86bd, /// 0x30
			 0x9b9bbba9, /// 0x34
			 0xa865d6e1, /// 0x38
			 0xb0135071, /// 0x3c
			 0x14b844a7, /// 0x40
			 0xa60c4de5, /// 0x44
			 0xba040ff3, /// 0x48
			 0x825d2740, /// 0x4c
			 0x253bcffa, /// 0x50
			 0x6261be9e, /// 0x54
			 0x64eda799, /// 0x58
			 0x038f64a6, /// 0x5c
			 0x4e783b21, /// 0x60
			 0x83b0e4d5, /// 0x64
			 0xf02e039b, /// 0x68
			 0x8719b67e, /// 0x6c
			 0xb1a37192, /// 0x70
			 0x452e8bdb, /// 0x74
			 0x589c85cc, /// 0x78
			 0xa7df82cf, /// 0x7c
			 0x48e5add8, /// 0x80
			 0xc5fbf667, /// 0x84
			 0x056be48d, /// 0x88
			 0x9d857b7a, /// 0x8c
			 0x81dfd6c2, /// 0x90
			 0x3bc7d507, /// 0x94
			 0x6804582d, /// 0x98
			 0x4358cc81, /// 0x9c
			 0x7c62c884, /// 0xa0
			 0xc4e57aef, /// 0xa4
			 0x99a18a20, /// 0xa8
			 0x0bc1e19c, /// 0xac
			 0xae9adac8, /// 0xb0
			 0x665b9533, /// 0xb4
			 0xd3df68bc, /// 0xb8
			 0xbae8d4d9, /// 0xbc
			 0x38f57755, /// 0xc0
			 0xad526cbf, /// 0xc4
			 0xd8e66172, /// 0xc8
			 0x43f6dc37, /// 0xcc
			 0x85c36860, /// 0xd0
			 0x2f8d37bf, /// 0xd4
			 0x8f4ec79a, /// 0xd8
			 0x8218938e, /// 0xdc
			 0x08bc8b72, /// 0xe0
			 0x62bb8696, /// 0xe4
			 0x66863163, /// 0xe8
			 0xc6bd3177, /// 0xec
			 0x1e5f4444, /// 0xf0
			 0x4b7547b1, /// 0xf4
			 0xd48a0976, /// 0xf8
			 0xaa9cc3cf, /// 0xfc
			 0x7af0304a, /// 0x100
			 0xc8d1bda0, /// 0x104
			 0xc660e216, /// 0x108
			 0xd3727044, /// 0x10c
			 0x87233609, /// 0x110
			 0x0dfbf196, /// 0x114
			 0x34271a00, /// 0x118
			 0xf9a28cc7, /// 0x11c
			 0x91621559, /// 0x120
			 0xe2d928b0, /// 0x124
			 0x4c4710f3, /// 0x128
			 0x66003993, /// 0x12c
			 0x63ab283e, /// 0x130
			 0x5941b559, /// 0x134
			 0xa133c051, /// 0x138
			 0xd1d78695, /// 0x13c
			 0x6a184a56, /// 0x140
			 0x9b0d14c5, /// 0x144
			 0x48de9f0e, /// 0x148
			 0x91ae4710, /// 0x14c
			 0x2e6a41d8, /// 0x150
			 0x6829e8d9, /// 0x154
			 0x41bcf2e6, /// 0x158
			 0xa03503ec, /// 0x15c
			 0xc5b2509f, /// 0x160
			 0x25e6f103, /// 0x164
			 0xcd638669, /// 0x168
			 0x845c1abb, /// 0x16c
			 0xf2a30be0, /// 0x170
			 0x974b7b2e, /// 0x174
			 0x48fa9206, /// 0x178
			 0xe3b8c053, /// 0x17c
			 0x9e692b1b, /// 0x180
			 0xc6229b13, /// 0x184
			 0xbace4907, /// 0x188
			 0x7f685992, /// 0x18c
			 0x911f11f3, /// 0x190
			 0xe180ad0e, /// 0x194
			 0xd17bcc33, /// 0x198
			 0x8141c7e1, /// 0x19c
			 0xb94a48f2, /// 0x1a0
			 0xfdee5496, /// 0x1a4
			 0x0e2c96ad, /// 0x1a8
			 0x62c936d2, /// 0x1ac
			 0x2f9da822, /// 0x1b0
			 0xb0f85ccf, /// 0x1b4
			 0x0e752a5d, /// 0x1b8
			 0x49b0738d, /// 0x1bc
			 0xdd71d0d3, /// 0x1c0
			 0x92d0b716, /// 0x1c4
			 0x26966651, /// 0x1c8
			 0x143dda62, /// 0x1cc
			 0x7fcf199f, /// 0x1d0
			 0x8a2e52c8, /// 0x1d4
			 0x00cc597f, /// 0x1d8
			 0x12ceb17c, /// 0x1dc
			 0x8575a27f, /// 0x1e0
			 0xc0133356, /// 0x1e4
			 0xf43dc61d, /// 0x1e8
			 0x70bec201, /// 0x1ec
			 0x0e6f6ab6, /// 0x1f0
			 0xb2479bdb, /// 0x1f4
			 0x5f44177d, /// 0x1f8
			 0xf5388ce0, /// 0x1fc
			 0x2d2001f7, /// 0x200
			 0x620be1dc, /// 0x204
			 0x1f650c75, /// 0x208
			 0xd31f2790, /// 0x20c
			 0xc3062ec9, /// 0x210
			 0xeee2ecdf, /// 0x214
			 0x00447f09, /// 0x218
			 0xd4442c96, /// 0x21c
			 0x2b8d2182, /// 0x220
			 0x9b3913f4, /// 0x224
			 0xda5b3c11, /// 0x228
			 0x5357fcf4, /// 0x22c
			 0xa27c59dd, /// 0x230
			 0x8c6926dd, /// 0x234
			 0x0d7c8562, /// 0x238
			 0x71c62770, /// 0x23c
			 0x6eed06cf, /// 0x240
			 0x5a7d37ad, /// 0x244
			 0xe69b93b1, /// 0x248
			 0x2fc76e18, /// 0x24c
			 0x52538109, /// 0x250
			 0xa015b68c, /// 0x254
			 0xe026fc86, /// 0x258
			 0x6cfcbd3b, /// 0x25c
			 0xfcb64d09, /// 0x260
			 0x34540050, /// 0x264
			 0x97793e4a, /// 0x268
			 0xc87b0f87, /// 0x26c
			 0x4a51a21b, /// 0x270
			 0x0d0f3197, /// 0x274
			 0x4bb4baab, /// 0x278
			 0x72233d48, /// 0x27c
			 0x2ee7f79a, /// 0x280
			 0xa896d9c7, /// 0x284
			 0x3d88b200, /// 0x288
			 0x30ce65f1, /// 0x28c
			 0x9cba5391, /// 0x290
			 0xea5e2ed0, /// 0x294
			 0x7435a963, /// 0x298
			 0x62ed9d85, /// 0x29c
			 0x61c4c38e, /// 0x2a0
			 0x84394afe, /// 0x2a4
			 0x0b35ceec, /// 0x2a8
			 0x1e2a0b19, /// 0x2ac
			 0x508ed14a, /// 0x2b0
			 0x7f230b16, /// 0x2b4
			 0x555009a5, /// 0x2b8
			 0x42483d84, /// 0x2bc
			 0x66514735, /// 0x2c0
			 0x97360fba, /// 0x2c4
			 0x3d9e7c61, /// 0x2c8
			 0xf482bd02, /// 0x2cc
			 0xc6f777f9, /// 0x2d0
			 0x2b56d06a, /// 0x2d4
			 0xe5615ef3, /// 0x2d8
			 0xf05dfed8, /// 0x2dc
			 0x690c9dd4, /// 0x2e0
			 0xb8f5ab5b, /// 0x2e4
			 0x81ba57a9, /// 0x2e8
			 0xe63945c4, /// 0x2ec
			 0xeeaa06fe, /// 0x2f0
			 0x13883755, /// 0x2f4
			 0x845c6a24, /// 0x2f8
			 0x8f15004b, /// 0x2fc
			 0x3b7583af, /// 0x300
			 0x06e15797, /// 0x304
			 0x1a5ba980, /// 0x308
			 0x50f5a3af, /// 0x30c
			 0x8f584567, /// 0x310
			 0x462687e4, /// 0x314
			 0x84d0236f, /// 0x318
			 0x07a4e68d, /// 0x31c
			 0x7f497c28, /// 0x320
			 0x87cdd180, /// 0x324
			 0x63404dd8, /// 0x328
			 0x0b504d0b, /// 0x32c
			 0x58709567, /// 0x330
			 0xb07057d4, /// 0x334
			 0xb6f7486d, /// 0x338
			 0x3a71d0b4, /// 0x33c
			 0xefae8a19, /// 0x340
			 0xb4ac639e, /// 0x344
			 0x1382e18d, /// 0x348
			 0x65da0e2b, /// 0x34c
			 0x5cb5e295, /// 0x350
			 0xc49cc14f, /// 0x354
			 0x70a83dfb, /// 0x358
			 0x13813f79, /// 0x35c
			 0x1f8e5d58, /// 0x360
			 0x082d9299, /// 0x364
			 0xa58d535b, /// 0x368
			 0x602a45fb, /// 0x36c
			 0x2b1a2497, /// 0x370
			 0x420ed38e, /// 0x374
			 0xed3d892b, /// 0x378
			 0x7dd8b4fa, /// 0x37c
			 0x7f96dce7, /// 0x380
			 0x08d66c65, /// 0x384
			 0xf9848571, /// 0x388
			 0x810b21e5, /// 0x38c
			 0x79658035, /// 0x390
			 0xa3399d9b, /// 0x394
			 0x4e988719, /// 0x398
			 0xed7d140e, /// 0x39c
			 0x49d2aef8, /// 0x3a0
			 0x6fd7763c, /// 0x3a4
			 0xadf78073, /// 0x3a8
			 0x801ac3dc, /// 0x3ac
			 0x9c2e4e15, /// 0x3b0
			 0x3f2fb6e6, /// 0x3b4
			 0x5d76f335, /// 0x3b8
			 0x46218954, /// 0x3bc
			 0xca69708c, /// 0x3c0
			 0x024ccc83, /// 0x3c4
			 0x47f0aba3, /// 0x3c8
			 0x18e04c68, /// 0x3cc
			 0xc985e49d, /// 0x3d0
			 0x29dda3b5, /// 0x3d4
			 0x726af3ec, /// 0x3d8
			 0x95951d1e, /// 0x3dc
			 0x0b330877, /// 0x3e0
			 0x304edc2a, /// 0x3e4
			 0x283c88ea, /// 0x3e8
			 0x590b83f9, /// 0x3ec
			 0x2a970bf9, /// 0x3f0
			 0x3e811c0b, /// 0x3f4
			 0x436373e6, /// 0x3f8
			 0x5a46c40e, /// 0x3fc
			 0x4c53de5b, /// 0x400
			 0x2619209d, /// 0x404
			 0xcf365bbb, /// 0x408
			 0xd231a59a, /// 0x40c
			 0x9815a698, /// 0x410
			 0x10e4839b, /// 0x414
			 0x29c587b1, /// 0x418
			 0x212cfe3f, /// 0x41c
			 0x23b65938, /// 0x420
			 0xb3456134, /// 0x424
			 0xb48972b3, /// 0x428
			 0xac416824, /// 0x42c
			 0x30645758, /// 0x430
			 0xc45958a5, /// 0x434
			 0x6120a9a8, /// 0x438
			 0x6339e457, /// 0x43c
			 0xe032e447, /// 0x440
			 0x14dd65c5, /// 0x444
			 0xbde00c55, /// 0x448
			 0x21910fa7, /// 0x44c
			 0x3a8c1124, /// 0x450
			 0x73e13e6c, /// 0x454
			 0xe2bdff93, /// 0x458
			 0xecfd8693, /// 0x45c
			 0x841a26b8, /// 0x460
			 0x4bee225f, /// 0x464
			 0xfa7e02df, /// 0x468
			 0x40fa4b94, /// 0x46c
			 0xd5bad0bb, /// 0x470
			 0xe3ce1c8f, /// 0x474
			 0xbc03fb80, /// 0x478
			 0xa4089057, /// 0x47c
			 0x74adb13c, /// 0x480
			 0xbf2e2276, /// 0x484
			 0xecc3347a, /// 0x488
			 0x34f52f38, /// 0x48c
			 0x75c5a621, /// 0x490
			 0xff9ddde0, /// 0x494
			 0xe01c0add, /// 0x498
			 0xd25ca1ae, /// 0x49c
			 0x2072a9a5, /// 0x4a0
			 0xca63cf8e, /// 0x4a4
			 0x5fac77c0, /// 0x4a8
			 0x133acff7, /// 0x4ac
			 0xe0f47ba0, /// 0x4b0
			 0xe9bcd7da, /// 0x4b4
			 0x3dd942b5, /// 0x4b8
			 0x498d296e, /// 0x4bc
			 0x03262d7a, /// 0x4c0
			 0xd8dcc632, /// 0x4c4
			 0xf77a024f, /// 0x4c8
			 0x3e750ca2, /// 0x4cc
			 0x985e5fde, /// 0x4d0
			 0x2500a912, /// 0x4d4
			 0xea7cb05a, /// 0x4d8
			 0x0082ed56, /// 0x4dc
			 0x46d2bc6d, /// 0x4e0
			 0xa2e309c5, /// 0x4e4
			 0xb2641b34, /// 0x4e8
			 0x93806355, /// 0x4ec
			 0x12bb3230, /// 0x4f0
			 0x6099b6e1, /// 0x4f4
			 0xaef1d100, /// 0x4f8
			 0x4e82638a, /// 0x4fc
			 0x41cb1a70, /// 0x500
			 0xc139254b, /// 0x504
			 0x5ef533a7, /// 0x508
			 0x027182d8, /// 0x50c
			 0x5666ebea, /// 0x510
			 0x3b73dc3d, /// 0x514
			 0x28aa71fd, /// 0x518
			 0x2396828e, /// 0x51c
			 0x92e146ce, /// 0x520
			 0x500d0324, /// 0x524
			 0x0f0437bf, /// 0x528
			 0x83f06278, /// 0x52c
			 0x69913d88, /// 0x530
			 0x7f35fb20, /// 0x534
			 0x943ca9d2, /// 0x538
			 0xbb41f638, /// 0x53c
			 0x1160cd4e, /// 0x540
			 0x52fd7f40, /// 0x544
			 0x4305d746, /// 0x548
			 0x48b32ff7, /// 0x54c
			 0x6a59157d, /// 0x550
			 0xdd7eb73e, /// 0x554
			 0x08d1d1a7, /// 0x558
			 0xe204a8d3, /// 0x55c
			 0xbf910af4, /// 0x560
			 0x68ab1b3d, /// 0x564
			 0x5b98dd13, /// 0x568
			 0x366c64d7, /// 0x56c
			 0x3a588653, /// 0x570
			 0xf55822b7, /// 0x574
			 0xb6b80166, /// 0x578
			 0xa111fb3d, /// 0x57c
			 0x16592c9d, /// 0x580
			 0x27df7846, /// 0x584
			 0xfdcb1cf6, /// 0x588
			 0x7a263acb, /// 0x58c
			 0xcdf40b90, /// 0x590
			 0x90df6ccb, /// 0x594
			 0x8ed820a5, /// 0x598
			 0xd246942b, /// 0x59c
			 0x9a0734b1, /// 0x5a0
			 0x91bd7020, /// 0x5a4
			 0x80e8dd86, /// 0x5a8
			 0xbe18420b, /// 0x5ac
			 0x0a1447bf, /// 0x5b0
			 0xbd07ac79, /// 0x5b4
			 0xe5f86a35, /// 0x5b8
			 0x7d71cd4d, /// 0x5bc
			 0x61c0830c, /// 0x5c0
			 0x89fc0fd2, /// 0x5c4
			 0x78cbddb1, /// 0x5c8
			 0x64d98b75, /// 0x5cc
			 0x6a38972c, /// 0x5d0
			 0xa5d79bfe, /// 0x5d4
			 0xc68ffd89, /// 0x5d8
			 0x025cb0be, /// 0x5dc
			 0xecfdab44, /// 0x5e0
			 0xb7860412, /// 0x5e4
			 0xbf1e876a, /// 0x5e8
			 0xefe53d0c, /// 0x5ec
			 0x2db919a2, /// 0x5f0
			 0x6c7eac1b, /// 0x5f4
			 0x1cae3712, /// 0x5f8
			 0xfeef234e, /// 0x5fc
			 0xda281e06, /// 0x600
			 0xb46af354, /// 0x604
			 0xaeeac881, /// 0x608
			 0x9f0a1c08, /// 0x60c
			 0xd08ac7e6, /// 0x610
			 0x772a3965, /// 0x614
			 0xbbc38d75, /// 0x618
			 0x12be4c97, /// 0x61c
			 0x325d99a8, /// 0x620
			 0x7725462e, /// 0x624
			 0xc72320cb, /// 0x628
			 0x0e0f24da, /// 0x62c
			 0x75911679, /// 0x630
			 0xd68d0ff5, /// 0x634
			 0x660dff95, /// 0x638
			 0x8f9c12f3, /// 0x63c
			 0xa9a9711d, /// 0x640
			 0x94c4f7c4, /// 0x644
			 0x6f30babf, /// 0x648
			 0xa33ca2ab, /// 0x64c
			 0x17dd3609, /// 0x650
			 0xc4f024cb, /// 0x654
			 0xb39b71e6, /// 0x658
			 0x7f64a0e4, /// 0x65c
			 0xb48013ba, /// 0x660
			 0x0a16e733, /// 0x664
			 0xca60aac0, /// 0x668
			 0x187a0c89, /// 0x66c
			 0x7a47290a, /// 0x670
			 0xf7228d2c, /// 0x674
			 0x3a22156d, /// 0x678
			 0x141f9602, /// 0x67c
			 0xbcfce2c2, /// 0x680
			 0x788cf742, /// 0x684
			 0x24742005, /// 0x688
			 0x6360b2b4, /// 0x68c
			 0x7b31f4c6, /// 0x690
			 0x6aa89bf2, /// 0x694
			 0x5ac96584, /// 0x698
			 0xa4eb796a, /// 0x69c
			 0x9f206144, /// 0x6a0
			 0x116977bc, /// 0x6a4
			 0x1cedc809, /// 0x6a8
			 0xc8f2d4a6, /// 0x6ac
			 0x76097a82, /// 0x6b0
			 0x4676e248, /// 0x6b4
			 0x25707799, /// 0x6b8
			 0x43b20a74, /// 0x6bc
			 0xa0d698fa, /// 0x6c0
			 0xcd39e874, /// 0x6c4
			 0x841ed243, /// 0x6c8
			 0x924a1e47, /// 0x6cc
			 0xbeb559dc, /// 0x6d0
			 0xda11dd5e, /// 0x6d4
			 0x0daf7201, /// 0x6d8
			 0x1e7cf926, /// 0x6dc
			 0x055ac7c9, /// 0x6e0
			 0xd3874c76, /// 0x6e4
			 0xc552c282, /// 0x6e8
			 0xeb00a4f5, /// 0x6ec
			 0x7cd13ded, /// 0x6f0
			 0x78b6dba2, /// 0x6f4
			 0xc1abfaee, /// 0x6f8
			 0x0789f320, /// 0x6fc
			 0x878da453, /// 0x700
			 0x341e3b53, /// 0x704
			 0x106ed74b, /// 0x708
			 0x4d91502e, /// 0x70c
			 0xca4439a4, /// 0x710
			 0xf2ac0a61, /// 0x714
			 0xb1fc122c, /// 0x718
			 0xfee49e30, /// 0x71c
			 0x6e9e8659, /// 0x720
			 0x64031c7c, /// 0x724
			 0xc7ae7c77, /// 0x728
			 0xb510a1d2, /// 0x72c
			 0xb5870719, /// 0x730
			 0xd26178db, /// 0x734
			 0x49d3bd3e, /// 0x738
			 0x0e842c9d, /// 0x73c
			 0xf68f55de, /// 0x740
			 0xc21dc51f, /// 0x744
			 0xea746286, /// 0x748
			 0xc9d4f599, /// 0x74c
			 0x53370344, /// 0x750
			 0x6c474dbf, /// 0x754
			 0x755095f4, /// 0x758
			 0xfe716131, /// 0x75c
			 0x3c5441c8, /// 0x760
			 0xb2bacc00, /// 0x764
			 0x79dd1724, /// 0x768
			 0x278d9d56, /// 0x76c
			 0xd7481339, /// 0x770
			 0x8e79bcb1, /// 0x774
			 0xc84eb97a, /// 0x778
			 0xcb11c0d6, /// 0x77c
			 0x6913afd5, /// 0x780
			 0x07eaf575, /// 0x784
			 0x9a0677c6, /// 0x788
			 0x83d72f4d, /// 0x78c
			 0xb322f3d4, /// 0x790
			 0xf6901a5b, /// 0x794
			 0xf5866c67, /// 0x798
			 0x9b62aca9, /// 0x79c
			 0x8f80d9b3, /// 0x7a0
			 0x00fed4d5, /// 0x7a4
			 0xf7bf2446, /// 0x7a8
			 0x4b85d019, /// 0x7ac
			 0x808b588e, /// 0x7b0
			 0x73d7453b, /// 0x7b4
			 0xa97c645a, /// 0x7b8
			 0x65c7e300, /// 0x7bc
			 0x0463c6eb, /// 0x7c0
			 0x1dcfd815, /// 0x7c4
			 0x2dbd3d7c, /// 0x7c8
			 0xbc918a13, /// 0x7cc
			 0x0f0f1acb, /// 0x7d0
			 0xa2372e78, /// 0x7d4
			 0x8b067c5d, /// 0x7d8
			 0x60d25b1e, /// 0x7dc
			 0xdc096b6f, /// 0x7e0
			 0x6bced318, /// 0x7e4
			 0x627c8263, /// 0x7e8
			 0x4206c762, /// 0x7ec
			 0x3dd11eb6, /// 0x7f0
			 0xe666ad96, /// 0x7f4
			 0xf903f4c8, /// 0x7f8
			 0xe4528f3b, /// 0x7fc
			 0x3c9010c7, /// 0x800
			 0xc3b60c99, /// 0x804
			 0x3f5a31f2, /// 0x808
			 0xfe60741d, /// 0x80c
			 0x32a55db9, /// 0x810
			 0x61cfd357, /// 0x814
			 0x7f9a5699, /// 0x818
			 0x10e9d877, /// 0x81c
			 0x5f3dba98, /// 0x820
			 0x9bc43019, /// 0x824
			 0x7d9d1086, /// 0x828
			 0x31da3911, /// 0x82c
			 0x6f80d3c9, /// 0x830
			 0x806368be, /// 0x834
			 0x63241efb, /// 0x838
			 0x50044f94, /// 0x83c
			 0xb51a981d, /// 0x840
			 0x6319c6ab, /// 0x844
			 0x2b8ba017, /// 0x848
			 0x012db5e0, /// 0x84c
			 0x3abf2240, /// 0x850
			 0x417d83fa, /// 0x854
			 0x2d978f91, /// 0x858
			 0x61d91eec, /// 0x85c
			 0x374dc23d, /// 0x860
			 0x71d13e4c, /// 0x864
			 0x2a948223, /// 0x868
			 0xcaf9f534, /// 0x86c
			 0xbeb6348e, /// 0x870
			 0xbe0610c6, /// 0x874
			 0xea371df3, /// 0x878
			 0xffcd6af3, /// 0x87c
			 0xe8717eea, /// 0x880
			 0x2cc489ca, /// 0x884
			 0x6ad81e4b, /// 0x888
			 0x02c67527, /// 0x88c
			 0x27785fbc, /// 0x890
			 0x00155ba0, /// 0x894
			 0xc59e72b4, /// 0x898
			 0xcfaa1e1c, /// 0x89c
			 0xfa89ee93, /// 0x8a0
			 0x380e6e4a, /// 0x8a4
			 0xb648bb1c, /// 0x8a8
			 0xadb6dd30, /// 0x8ac
			 0x3b4949fd, /// 0x8b0
			 0x65db6123, /// 0x8b4
			 0x6d43f093, /// 0x8b8
			 0x91178577, /// 0x8bc
			 0x730cf6e1, /// 0x8c0
			 0x4a672936, /// 0x8c4
			 0x05b7aa0e, /// 0x8c8
			 0x9cc5cb97, /// 0x8cc
			 0x302cc2c3, /// 0x8d0
			 0x9e4dcef4, /// 0x8d4
			 0xac3f5f47, /// 0x8d8
			 0x1dbcfa5a, /// 0x8dc
			 0xa0db287a, /// 0x8e0
			 0xc0fa874f, /// 0x8e4
			 0xc9e5e1db, /// 0x8e8
			 0xb34e52cd, /// 0x8ec
			 0xeeb8e971, /// 0x8f0
			 0x61eac606, /// 0x8f4
			 0x5c7732f9, /// 0x8f8
			 0x74af5a62, /// 0x8fc
			 0xe7f07dca, /// 0x900
			 0x8a51cafb, /// 0x904
			 0x77e827a8, /// 0x908
			 0xcf875a23, /// 0x90c
			 0xb5f790b0, /// 0x910
			 0xdca82c7f, /// 0x914
			 0xa2207bbc, /// 0x918
			 0xccd243e0, /// 0x91c
			 0xa567a53c, /// 0x920
			 0xb7ef0ef4, /// 0x924
			 0x80768026, /// 0x928
			 0xff953085, /// 0x92c
			 0x3036b618, /// 0x930
			 0x3a553070, /// 0x934
			 0x53fa2c4a, /// 0x938
			 0xfb9f3af0, /// 0x93c
			 0x4546f242, /// 0x940
			 0x66c3cf56, /// 0x944
			 0xb997c46a, /// 0x948
			 0x1da0b6f3, /// 0x94c
			 0xd05f389a, /// 0x950
			 0x7626854b, /// 0x954
			 0xe0e562bb, /// 0x958
			 0x6421b499, /// 0x95c
			 0xd4f5828f, /// 0x960
			 0x47ae66f4, /// 0x964
			 0x8ebf64a2, /// 0x968
			 0x2975c78f, /// 0x96c
			 0xc9cb64b6, /// 0x970
			 0x8bf2c159, /// 0x974
			 0x03c5f04c, /// 0x978
			 0x8784c56e, /// 0x97c
			 0x5915d875, /// 0x980
			 0xdedb6e04, /// 0x984
			 0x4900dff5, /// 0x988
			 0xeab69f1e, /// 0x98c
			 0x39dd0151, /// 0x990
			 0xdb378a15, /// 0x994
			 0x22e685e0, /// 0x998
			 0xe26a64fb, /// 0x99c
			 0x7ffcbf59, /// 0x9a0
			 0x680ab6d8, /// 0x9a4
			 0x4e899a80, /// 0x9a8
			 0x5e2d0fb3, /// 0x9ac
			 0x4e28023e, /// 0x9b0
			 0x9c192ba4, /// 0x9b4
			 0x23f95160, /// 0x9b8
			 0xf3a69922, /// 0x9bc
			 0xad8ad3d6, /// 0x9c0
			 0x5f54240d, /// 0x9c4
			 0xe62a0cee, /// 0x9c8
			 0x7f2cdb2e, /// 0x9cc
			 0xd72fdc7d, /// 0x9d0
			 0x7d013300, /// 0x9d4
			 0x897647aa, /// 0x9d8
			 0x042b0df3, /// 0x9dc
			 0xf8ce676f, /// 0x9e0
			 0x25fa8761, /// 0x9e4
			 0x85e54b09, /// 0x9e8
			 0xef9c1483, /// 0x9ec
			 0x8ee46ced, /// 0x9f0
			 0xdec80b0b, /// 0x9f4
			 0x61c721e8, /// 0x9f8
			 0xfaf17309, /// 0x9fc
			 0x62d43a7b, /// 0xa00
			 0x7b72608d, /// 0xa04
			 0x888440a4, /// 0xa08
			 0x53f8d27b, /// 0xa0c
			 0x19eac634, /// 0xa10
			 0x7828e9a4, /// 0xa14
			 0x70f6e2d7, /// 0xa18
			 0x2db1be5c, /// 0xa1c
			 0xed0810cf, /// 0xa20
			 0x96b7d630, /// 0xa24
			 0xcc19bb38, /// 0xa28
			 0xc903b76e, /// 0xa2c
			 0x7fb506a6, /// 0xa30
			 0x8a53ddab, /// 0xa34
			 0x06caa09d, /// 0xa38
			 0x95ab7e5e, /// 0xa3c
			 0x74a163c1, /// 0xa40
			 0xa3c2550f, /// 0xa44
			 0xc0715e36, /// 0xa48
			 0x4517effd, /// 0xa4c
			 0x7881cd55, /// 0xa50
			 0x1eaeb624, /// 0xa54
			 0x1c09decf, /// 0xa58
			 0x66328fa5, /// 0xa5c
			 0x944b7ae7, /// 0xa60
			 0xcefbd080, /// 0xa64
			 0xdcd82988, /// 0xa68
			 0x580e9219, /// 0xa6c
			 0xd3daf07e, /// 0xa70
			 0x6bca6350, /// 0xa74
			 0x63dd501c, /// 0xa78
			 0x5230f997, /// 0xa7c
			 0x2d4dec41, /// 0xa80
			 0x411c6f8e, /// 0xa84
			 0x48e10610, /// 0xa88
			 0x578289e8, /// 0xa8c
			 0xa0bc0b93, /// 0xa90
			 0x8b3c9e29, /// 0xa94
			 0xb6ba2950, /// 0xa98
			 0x91103916, /// 0xa9c
			 0xd8f0be52, /// 0xaa0
			 0xf46d08a4, /// 0xaa4
			 0xfcc80a7c, /// 0xaa8
			 0xa1f60171, /// 0xaac
			 0xfc4e633f, /// 0xab0
			 0xb90f5391, /// 0xab4
			 0xa5307c6e, /// 0xab8
			 0xde9d20df, /// 0xabc
			 0x1cf39474, /// 0xac0
			 0x738b74d1, /// 0xac4
			 0xba00bd74, /// 0xac8
			 0x9abdc22e, /// 0xacc
			 0x19c53d8d, /// 0xad0
			 0x3ac7f3cd, /// 0xad4
			 0x582382cf, /// 0xad8
			 0xa098d494, /// 0xadc
			 0xb2f228a0, /// 0xae0
			 0x4d83aeb9, /// 0xae4
			 0xbc329384, /// 0xae8
			 0x11667b9a, /// 0xaec
			 0xb0e07e4d, /// 0xaf0
			 0xf0c27c29, /// 0xaf4
			 0xb6dc71e6, /// 0xaf8
			 0x81919612, /// 0xafc
			 0x12135e86, /// 0xb00
			 0x84694eac, /// 0xb04
			 0x77ae6a8c, /// 0xb08
			 0xaa469592, /// 0xb0c
			 0xa4b3dfe4, /// 0xb10
			 0x12207b65, /// 0xb14
			 0x0324a366, /// 0xb18
			 0x4fdb6b34, /// 0xb1c
			 0xbd7b529d, /// 0xb20
			 0x691cdf65, /// 0xb24
			 0x03607345, /// 0xb28
			 0xf66d8bd2, /// 0xb2c
			 0xe3d56913, /// 0xb30
			 0x9f3344dc, /// 0xb34
			 0xfd6cae6a, /// 0xb38
			 0x5c28ef57, /// 0xb3c
			 0x2c877c05, /// 0xb40
			 0x0bfa25e5, /// 0xb44
			 0x466af3f3, /// 0xb48
			 0xc17b827a, /// 0xb4c
			 0xaef1816a, /// 0xb50
			 0x5bc49ef7, /// 0xb54
			 0xf3de4a9f, /// 0xb58
			 0xabb82fe5, /// 0xb5c
			 0x3f9a842c, /// 0xb60
			 0xfee755a6, /// 0xb64
			 0xe92ed6d5, /// 0xb68
			 0xac4db697, /// 0xb6c
			 0xeb245f35, /// 0xb70
			 0x845d63b1, /// 0xb74
			 0x3a1344da, /// 0xb78
			 0x41cc78ec, /// 0xb7c
			 0x7d5873a9, /// 0xb80
			 0xdcdfebb2, /// 0xb84
			 0x3a555bf9, /// 0xb88
			 0x33b5c5a1, /// 0xb8c
			 0x81b4ca15, /// 0xb90
			 0x2ad55e0d, /// 0xb94
			 0xdab70049, /// 0xb98
			 0x509e37b4, /// 0xb9c
			 0xe431f06a, /// 0xba0
			 0x5b10a1ab, /// 0xba4
			 0x21cbd30a, /// 0xba8
			 0x40f8094d, /// 0xbac
			 0xe56b6e54, /// 0xbb0
			 0xaf53e959, /// 0xbb4
			 0xa01b2081, /// 0xbb8
			 0x00707a99, /// 0xbbc
			 0x76a9f525, /// 0xbc0
			 0x50908cf6, /// 0xbc4
			 0x2c07105b, /// 0xbc8
			 0x1dbb3adb, /// 0xbcc
			 0xa7ceeaf1, /// 0xbd0
			 0x070a50cb, /// 0xbd4
			 0xf31f95bd, /// 0xbd8
			 0x33518fc6, /// 0xbdc
			 0x6e412e1f, /// 0xbe0
			 0x01d73757, /// 0xbe4
			 0xc692e60f, /// 0xbe8
			 0x87883148, /// 0xbec
			 0x0426834c, /// 0xbf0
			 0x5e65971b, /// 0xbf4
			 0x6a99f2fd, /// 0xbf8
			 0x74ae701a, /// 0xbfc
			 0x9ba8bd20, /// 0xc00
			 0x9b408740, /// 0xc04
			 0xb937be66, /// 0xc08
			 0xb4ed3395, /// 0xc0c
			 0xfd4a2924, /// 0xc10
			 0x3b6cc0f1, /// 0xc14
			 0xe5bb8c9e, /// 0xc18
			 0x7fffe4fe, /// 0xc1c
			 0xa98ddba5, /// 0xc20
			 0xd3eb7cfd, /// 0xc24
			 0x4d81c904, /// 0xc28
			 0x1f1f32e4, /// 0xc2c
			 0x80075df4, /// 0xc30
			 0xc897d758, /// 0xc34
			 0xb4a73d83, /// 0xc38
			 0x8f4014e1, /// 0xc3c
			 0x031e7b99, /// 0xc40
			 0x2223eecf, /// 0xc44
			 0xf3881a4b, /// 0xc48
			 0xded76ca7, /// 0xc4c
			 0x28c5f823, /// 0xc50
			 0x5148c123, /// 0xc54
			 0x7a9136d4, /// 0xc58
			 0x58683156, /// 0xc5c
			 0xf5b718d2, /// 0xc60
			 0x411dd958, /// 0xc64
			 0xf3ee8dbd, /// 0xc68
			 0xe20c4ed1, /// 0xc6c
			 0xc6eab563, /// 0xc70
			 0xa9dae894, /// 0xc74
			 0x12e55dcb, /// 0xc78
			 0xf1038bad, /// 0xc7c
			 0x268235c0, /// 0xc80
			 0xb337edb6, /// 0xc84
			 0xc8a421fb, /// 0xc88
			 0xc2963d33, /// 0xc8c
			 0x190945ad, /// 0xc90
			 0x8e467ac1, /// 0xc94
			 0x375ed896, /// 0xc98
			 0x71568447, /// 0xc9c
			 0xfec4760d, /// 0xca0
			 0xb04f315b, /// 0xca4
			 0x603b18e6, /// 0xca8
			 0xb8bd028c, /// 0xcac
			 0x21e70366, /// 0xcb0
			 0x49037854, /// 0xcb4
			 0x13722d49, /// 0xcb8
			 0x723e4736, /// 0xcbc
			 0xa218ee19, /// 0xcc0
			 0x36479e77, /// 0xcc4
			 0x63681e2b, /// 0xcc8
			 0x5880dfa9, /// 0xccc
			 0x01c3d7ba, /// 0xcd0
			 0x755a55d9, /// 0xcd4
			 0xdfddfcb1, /// 0xcd8
			 0x6e39d948, /// 0xcdc
			 0x376d13c1, /// 0xce0
			 0xef17d8ab, /// 0xce4
			 0x1a1bf682, /// 0xce8
			 0x5c302c6f, /// 0xcec
			 0x4bd7c310, /// 0xcf0
			 0xb7ce962f, /// 0xcf4
			 0x4d48bb74, /// 0xcf8
			 0x10be9340, /// 0xcfc
			 0x01e3ca20, /// 0xd00
			 0x9ba73ffd, /// 0xd04
			 0xaf14cc71, /// 0xd08
			 0x068a08e5, /// 0xd0c
			 0x677bc2fa, /// 0xd10
			 0x0c84cc65, /// 0xd14
			 0x974bad80, /// 0xd18
			 0x28767518, /// 0xd1c
			 0x72cb1a6d, /// 0xd20
			 0x465e2d85, /// 0xd24
			 0x0678a88c, /// 0xd28
			 0x87af1814, /// 0xd2c
			 0x42fa5899, /// 0xd30
			 0xa9f6ba79, /// 0xd34
			 0xf2cb6812, /// 0xd38
			 0x7790f7cd, /// 0xd3c
			 0xf09e76fa, /// 0xd40
			 0xaa1c5552, /// 0xd44
			 0x4081d11a, /// 0xd48
			 0xcc14f8d7, /// 0xd4c
			 0xb58774db, /// 0xd50
			 0x48d00230, /// 0xd54
			 0x7bf5ed6b, /// 0xd58
			 0xfb846661, /// 0xd5c
			 0x7027c79e, /// 0xd60
			 0x7da67d17, /// 0xd64
			 0x12862af6, /// 0xd68
			 0x4757f09a, /// 0xd6c
			 0xacec2878, /// 0xd70
			 0x081a88a7, /// 0xd74
			 0x60561128, /// 0xd78
			 0xcb93c021, /// 0xd7c
			 0x724d4dff, /// 0xd80
			 0x2b8c8d8a, /// 0xd84
			 0xa6973410, /// 0xd88
			 0x5fd39016, /// 0xd8c
			 0xaa9babc1, /// 0xd90
			 0xb8ca9cb7, /// 0xd94
			 0x579d2c7e, /// 0xd98
			 0x589ff5c6, /// 0xd9c
			 0x03bd806a, /// 0xda0
			 0x54c2fccb, /// 0xda4
			 0xc5fe264d, /// 0xda8
			 0xe91b7b89, /// 0xdac
			 0x1f887eb0, /// 0xdb0
			 0x56bdaa0c, /// 0xdb4
			 0x45134ae8, /// 0xdb8
			 0xfcbccc44, /// 0xdbc
			 0x6b1bfd27, /// 0xdc0
			 0x79734dad, /// 0xdc4
			 0x4cc61e07, /// 0xdc8
			 0xa7c07ffd, /// 0xdcc
			 0x9db9fba4, /// 0xdd0
			 0x1e7d9b5c, /// 0xdd4
			 0xefe88d7c, /// 0xdd8
			 0xdb2e581c, /// 0xddc
			 0xaf8bcc52, /// 0xde0
			 0xc6440a6e, /// 0xde4
			 0x103e2585, /// 0xde8
			 0x5cecdaa3, /// 0xdec
			 0xec68194c, /// 0xdf0
			 0xbdd26773, /// 0xdf4
			 0xd5d5c676, /// 0xdf8
			 0xbdb130fb, /// 0xdfc
			 0xdf736627, /// 0xe00
			 0x6714ccf7, /// 0xe04
			 0xeb1ca304, /// 0xe08
			 0x74a5f1a2, /// 0xe0c
			 0xf57413f1, /// 0xe10
			 0x3de8ecd2, /// 0xe14
			 0x61c5c8d2, /// 0xe18
			 0xff32fdcf, /// 0xe1c
			 0x21c54e5b, /// 0xe20
			 0x60e5e81f, /// 0xe24
			 0xf138ff91, /// 0xe28
			 0xeb147070, /// 0xe2c
			 0x795074cf, /// 0xe30
			 0xdf5fba4a, /// 0xe34
			 0xc97fde1f, /// 0xe38
			 0x8f3f617e, /// 0xe3c
			 0xf8ca5295, /// 0xe40
			 0x5701c44c, /// 0xe44
			 0x37713d00, /// 0xe48
			 0x8cfcb0dd, /// 0xe4c
			 0x68280ad8, /// 0xe50
			 0x90de7b50, /// 0xe54
			 0x1ce533ef, /// 0xe58
			 0xec2a9a2d, /// 0xe5c
			 0xa8f3cd2c, /// 0xe60
			 0x83c33219, /// 0xe64
			 0x3eb78e72, /// 0xe68
			 0x64cd7b21, /// 0xe6c
			 0x90bb83e7, /// 0xe70
			 0x2190cabe, /// 0xe74
			 0x36f43390, /// 0xe78
			 0x04b0f5e7, /// 0xe7c
			 0x4e31c575, /// 0xe80
			 0xb3720ad1, /// 0xe84
			 0x48dccf5d, /// 0xe88
			 0x5f03b663, /// 0xe8c
			 0xf0528587, /// 0xe90
			 0x32616165, /// 0xe94
			 0xa7162036, /// 0xe98
			 0x9a9fa2a8, /// 0xe9c
			 0x25034045, /// 0xea0
			 0x0f6bf811, /// 0xea4
			 0xa80ea21b, /// 0xea8
			 0x078ad194, /// 0xeac
			 0x10e288be, /// 0xeb0
			 0x67d2ab70, /// 0xeb4
			 0x0566ce1c, /// 0xeb8
			 0xfac296a5, /// 0xebc
			 0x123e7f91, /// 0xec0
			 0xbf759735, /// 0xec4
			 0x9656f96b, /// 0xec8
			 0xc41dc633, /// 0xecc
			 0x21f1f5af, /// 0xed0
			 0x7304c107, /// 0xed4
			 0xf94d2211, /// 0xed8
			 0x91fef051, /// 0xedc
			 0x44dc938f, /// 0xee0
			 0xf3b180b6, /// 0xee4
			 0x1579523c, /// 0xee8
			 0xbe33be15, /// 0xeec
			 0xf62364fb, /// 0xef0
			 0x1586693b, /// 0xef4
			 0x8e736e99, /// 0xef8
			 0x113fff35, /// 0xefc
			 0x8a7e431b, /// 0xf00
			 0xd5e65679, /// 0xf04
			 0x44ff256e, /// 0xf08
			 0x7e5941cf, /// 0xf0c
			 0x71a11698, /// 0xf10
			 0x369b96d2, /// 0xf14
			 0xf6295bc2, /// 0xf18
			 0xc839ced7, /// 0xf1c
			 0x0265795b, /// 0xf20
			 0x66062c07, /// 0xf24
			 0x929963d4, /// 0xf28
			 0x32bef908, /// 0xf2c
			 0x8b143cde, /// 0xf30
			 0x3b18829e, /// 0xf34
			 0x1bdd0eeb, /// 0xf38
			 0xb0bef348, /// 0xf3c
			 0x3bb29780, /// 0xf40
			 0x946cabae, /// 0xf44
			 0x7e3f0a7f, /// 0xf48
			 0xead673e7, /// 0xf4c
			 0x5dcc476c, /// 0xf50
			 0x828163f1, /// 0xf54
			 0x85f9164a, /// 0xf58
			 0x61c24bc6, /// 0xf5c
			 0x69455703, /// 0xf60
			 0x15f12657, /// 0xf64
			 0x81a52090, /// 0xf68
			 0x2baa789f, /// 0xf6c
			 0xda623333, /// 0xf70
			 0x78d95351, /// 0xf74
			 0x90155dc2, /// 0xf78
			 0xf3e2bc47, /// 0xf7c
			 0xfd162697, /// 0xf80
			 0xc627a1ef, /// 0xf84
			 0x006b4967, /// 0xf88
			 0x1b94e043, /// 0xf8c
			 0x702a2721, /// 0xf90
			 0x0cb5bea8, /// 0xf94
			 0x9d74d944, /// 0xf98
			 0x70a61426, /// 0xf9c
			 0x1b22ee73, /// 0xfa0
			 0x6e783e2c, /// 0xfa4
			 0x988a859f, /// 0xfa8
			 0xd63a829b, /// 0xfac
			 0x1dde9952, /// 0xfb0
			 0x27949627, /// 0xfb4
			 0x210a762b, /// 0xfb8
			 0x45c3a286, /// 0xfbc
			 0xf1f339c0, /// 0xfc0
			 0x283dcb16, /// 0xfc4
			 0x08a02fe3, /// 0xfc8
			 0xcf5b57dd, /// 0xfcc
			 0xcd5f1294, /// 0xfd0
			 0x98fa7a27, /// 0xfd4
			 0x37461ad1, /// 0xfd8
			 0xd1d6e2d9, /// 0xfdc
			 0xa50d1c6e, /// 0xfe0
			 0xef964a8d, /// 0xfe4
			 0xb8bd24e8, /// 0xfe8
			 0x7776efe2, /// 0xfec
			 0xa27a6551, /// 0xff0
			 0x31d8ad7f, /// 0xff4
			 0xbb6035ab, /// 0xff8
			 0x80500433 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x5da703b9, /// 0x0
			 0x01ace6f6, /// 0x4
			 0x3e4d6837, /// 0x8
			 0xee64cf89, /// 0xc
			 0x4d150ab8, /// 0x10
			 0x65f335d4, /// 0x14
			 0xec61704c, /// 0x18
			 0x135e3adb, /// 0x1c
			 0x83246f04, /// 0x20
			 0x26aa9a0d, /// 0x24
			 0x83331655, /// 0x28
			 0xa7df38db, /// 0x2c
			 0x6464a51f, /// 0x30
			 0x59f68275, /// 0x34
			 0x14af6b51, /// 0x38
			 0x755d59c8, /// 0x3c
			 0xe7d36555, /// 0x40
			 0xe9c953b3, /// 0x44
			 0x04dcbd80, /// 0x48
			 0xe8b2272f, /// 0x4c
			 0xfc58cc56, /// 0x50
			 0xd3860b16, /// 0x54
			 0x6034dcc9, /// 0x58
			 0x8558281b, /// 0x5c
			 0x77334dd4, /// 0x60
			 0x04ff2233, /// 0x64
			 0x2126bf78, /// 0x68
			 0x7f8cb83e, /// 0x6c
			 0xb5f97d49, /// 0x70
			 0x0a8d1b6c, /// 0x74
			 0x60a79db7, /// 0x78
			 0x44898cab, /// 0x7c
			 0xd2424a4c, /// 0x80
			 0x3669f0e5, /// 0x84
			 0x18abaa41, /// 0x88
			 0x1957057d, /// 0x8c
			 0x5cc4de5e, /// 0x90
			 0x27c2d948, /// 0x94
			 0xf49df8bf, /// 0x98
			 0x298ffba9, /// 0x9c
			 0x9482c7ed, /// 0xa0
			 0xcb57bb4f, /// 0xa4
			 0x74b24fee, /// 0xa8
			 0x5a1380d7, /// 0xac
			 0x9689d938, /// 0xb0
			 0xb2131f0d, /// 0xb4
			 0xed05b668, /// 0xb8
			 0xafd7196e, /// 0xbc
			 0x9b83b410, /// 0xc0
			 0xa1b00c9f, /// 0xc4
			 0x0cd14334, /// 0xc8
			 0x8b139f9f, /// 0xcc
			 0x339a16e4, /// 0xd0
			 0xfabf4adc, /// 0xd4
			 0x0165a853, /// 0xd8
			 0x5e7989e3, /// 0xdc
			 0xb4a10224, /// 0xe0
			 0xcd7219b6, /// 0xe4
			 0x21d08f36, /// 0xe8
			 0xc8cfd49f, /// 0xec
			 0x2eb262e3, /// 0xf0
			 0xe54176ff, /// 0xf4
			 0xccf170fc, /// 0xf8
			 0xdcad01c1, /// 0xfc
			 0x9a939c62, /// 0x100
			 0xd079981a, /// 0x104
			 0x9b563073, /// 0x108
			 0x14f0a99a, /// 0x10c
			 0xfaddeae6, /// 0x110
			 0x788c6723, /// 0x114
			 0xbf4dc897, /// 0x118
			 0xf89d9476, /// 0x11c
			 0x9b1b8894, /// 0x120
			 0x0b4cbaf7, /// 0x124
			 0x89bca67c, /// 0x128
			 0x2e6acd45, /// 0x12c
			 0x611a1af7, /// 0x130
			 0xd7880fe8, /// 0x134
			 0xd9e602b1, /// 0x138
			 0x3ecd811b, /// 0x13c
			 0xc896c187, /// 0x140
			 0x4074107c, /// 0x144
			 0xca4753ae, /// 0x148
			 0xc18f2ac4, /// 0x14c
			 0xb49fc1ba, /// 0x150
			 0x286b9da2, /// 0x154
			 0x74f5a561, /// 0x158
			 0x9e0344ae, /// 0x15c
			 0x154a88a0, /// 0x160
			 0xc3c66e8a, /// 0x164
			 0x9d64a55f, /// 0x168
			 0x3d6f6e6c, /// 0x16c
			 0xe71d6bb0, /// 0x170
			 0xe8b2fcb0, /// 0x174
			 0x0549f6ab, /// 0x178
			 0xb9121035, /// 0x17c
			 0xa6ec25a1, /// 0x180
			 0x4e629e90, /// 0x184
			 0xd2b58cb5, /// 0x188
			 0x2b84a4bd, /// 0x18c
			 0x95415ec3, /// 0x190
			 0x9afc2cae, /// 0x194
			 0x33c3c5d4, /// 0x198
			 0xc7b27f45, /// 0x19c
			 0x6f58cbca, /// 0x1a0
			 0xa9502bc8, /// 0x1a4
			 0xf8e1da94, /// 0x1a8
			 0x7f763fd8, /// 0x1ac
			 0x80ec153d, /// 0x1b0
			 0x668fda79, /// 0x1b4
			 0x0694e3d8, /// 0x1b8
			 0x5a6471c1, /// 0x1bc
			 0xb6eb1fc0, /// 0x1c0
			 0x7473d4c8, /// 0x1c4
			 0xa0cd8879, /// 0x1c8
			 0x25e32c74, /// 0x1cc
			 0xbeebb553, /// 0x1d0
			 0x07e74ee0, /// 0x1d4
			 0xf87353f7, /// 0x1d8
			 0x23378fda, /// 0x1dc
			 0xd1daa69b, /// 0x1e0
			 0xd7d86f94, /// 0x1e4
			 0x9218819c, /// 0x1e8
			 0xc7068c46, /// 0x1ec
			 0x650b4322, /// 0x1f0
			 0xf594d215, /// 0x1f4
			 0xd7c2539c, /// 0x1f8
			 0x1b8dd404, /// 0x1fc
			 0x551d2bc1, /// 0x200
			 0xf4d9ec09, /// 0x204
			 0xf14fa9f2, /// 0x208
			 0x1fdbc822, /// 0x20c
			 0xcd9fda09, /// 0x210
			 0xcb527120, /// 0x214
			 0xf258d01b, /// 0x218
			 0x97c43f06, /// 0x21c
			 0x4cd02522, /// 0x220
			 0x75f9565f, /// 0x224
			 0xc4c3115b, /// 0x228
			 0xd544702d, /// 0x22c
			 0x7976bbb0, /// 0x230
			 0xe8914de2, /// 0x234
			 0xa7552bf0, /// 0x238
			 0x9fa02c21, /// 0x23c
			 0x0a585f79, /// 0x240
			 0xa0f038d5, /// 0x244
			 0x7fc0c57b, /// 0x248
			 0x5041e698, /// 0x24c
			 0x0800b45e, /// 0x250
			 0x83379056, /// 0x254
			 0xac63e20f, /// 0x258
			 0x83b3be3a, /// 0x25c
			 0x617e3f70, /// 0x260
			 0xe10b942f, /// 0x264
			 0xc006c623, /// 0x268
			 0x0312a014, /// 0x26c
			 0x30f51c5a, /// 0x270
			 0x1ffbee86, /// 0x274
			 0x9b99ebfd, /// 0x278
			 0x2780d613, /// 0x27c
			 0xb00591bd, /// 0x280
			 0x450629e8, /// 0x284
			 0x642233a6, /// 0x288
			 0xb5682b46, /// 0x28c
			 0xc901b62e, /// 0x290
			 0x9e68a2d8, /// 0x294
			 0xc07fb62e, /// 0x298
			 0xb64bfbff, /// 0x29c
			 0x1fa89dc4, /// 0x2a0
			 0x5c36e983, /// 0x2a4
			 0x12cde955, /// 0x2a8
			 0x35137702, /// 0x2ac
			 0x0ad85104, /// 0x2b0
			 0x7007d399, /// 0x2b4
			 0x4504e776, /// 0x2b8
			 0xb6e57044, /// 0x2bc
			 0x0fd7936a, /// 0x2c0
			 0x4f6e0989, /// 0x2c4
			 0xa32aeca2, /// 0x2c8
			 0x5cfd76d3, /// 0x2cc
			 0x10e25d70, /// 0x2d0
			 0x1362edac, /// 0x2d4
			 0xfc50213b, /// 0x2d8
			 0x6b74b5b0, /// 0x2dc
			 0x9c2e1892, /// 0x2e0
			 0x9787bc03, /// 0x2e4
			 0x7ab5e5cf, /// 0x2e8
			 0x6fd0a5e9, /// 0x2ec
			 0x8230a757, /// 0x2f0
			 0xe1e6eacf, /// 0x2f4
			 0x5f39335b, /// 0x2f8
			 0xb530c082, /// 0x2fc
			 0xd83527e0, /// 0x300
			 0x83a13edf, /// 0x304
			 0xe4cc9aa2, /// 0x308
			 0xc927f431, /// 0x30c
			 0x3a824f73, /// 0x310
			 0xfd0a4254, /// 0x314
			 0x58476958, /// 0x318
			 0x5edd733f, /// 0x31c
			 0xd131ffe8, /// 0x320
			 0x78f1d24f, /// 0x324
			 0x50b2cd61, /// 0x328
			 0xb6f9c16d, /// 0x32c
			 0x5b485239, /// 0x330
			 0x5bb79573, /// 0x334
			 0x44ef5c96, /// 0x338
			 0x61f8b74b, /// 0x33c
			 0x892ea2b7, /// 0x340
			 0x185a6937, /// 0x344
			 0xe9a450e5, /// 0x348
			 0xf6704aa0, /// 0x34c
			 0xec740a81, /// 0x350
			 0x52beebe0, /// 0x354
			 0x9f7b60c5, /// 0x358
			 0x1c4787b7, /// 0x35c
			 0x5d31730b, /// 0x360
			 0x235bd7f0, /// 0x364
			 0x81759046, /// 0x368
			 0xf48b18c5, /// 0x36c
			 0x4625e8ff, /// 0x370
			 0xfeca9ab9, /// 0x374
			 0xf828a8d8, /// 0x378
			 0x09b5acd1, /// 0x37c
			 0x27184d9a, /// 0x380
			 0x814aabd3, /// 0x384
			 0x1321f9a7, /// 0x388
			 0x8c8b9d42, /// 0x38c
			 0x5707bec3, /// 0x390
			 0x3b57e783, /// 0x394
			 0xef4c3189, /// 0x398
			 0x14dfec32, /// 0x39c
			 0x8da16a9a, /// 0x3a0
			 0x46faebd2, /// 0x3a4
			 0x10373e83, /// 0x3a8
			 0x4c3d987a, /// 0x3ac
			 0xa71a8366, /// 0x3b0
			 0x22579090, /// 0x3b4
			 0xe8e919ca, /// 0x3b8
			 0xbf8e572b, /// 0x3bc
			 0x3b84cf18, /// 0x3c0
			 0x6c1172b3, /// 0x3c4
			 0xb268758c, /// 0x3c8
			 0x07d4d0a0, /// 0x3cc
			 0x4e5244ee, /// 0x3d0
			 0x8f949d78, /// 0x3d4
			 0x5c09dc9a, /// 0x3d8
			 0x2669827e, /// 0x3dc
			 0x20605b61, /// 0x3e0
			 0xf3a9c231, /// 0x3e4
			 0x976e5e26, /// 0x3e8
			 0xbf77034d, /// 0x3ec
			 0x79b6d3b4, /// 0x3f0
			 0x460514aa, /// 0x3f4
			 0x4f3a58dd, /// 0x3f8
			 0x3f552924, /// 0x3fc
			 0xe60646d9, /// 0x400
			 0xa1d4f882, /// 0x404
			 0xb14ff7fa, /// 0x408
			 0xe3bf646c, /// 0x40c
			 0xea42d703, /// 0x410
			 0x08ef4ba7, /// 0x414
			 0x54646133, /// 0x418
			 0x02cc282a, /// 0x41c
			 0xf7c3f8cb, /// 0x420
			 0xdcca92aa, /// 0x424
			 0xca30f92d, /// 0x428
			 0x52762ff6, /// 0x42c
			 0x2ddf679b, /// 0x430
			 0x919540ef, /// 0x434
			 0x1e8ced65, /// 0x438
			 0xf9cc05b2, /// 0x43c
			 0x297f7bfd, /// 0x440
			 0xa548c070, /// 0x444
			 0x1a8c57f0, /// 0x448
			 0xc84d4cfc, /// 0x44c
			 0x7c950477, /// 0x450
			 0xf7832185, /// 0x454
			 0x1804d4d9, /// 0x458
			 0x3265d152, /// 0x45c
			 0xed0b3d58, /// 0x460
			 0x1f4120b4, /// 0x464
			 0x84b6680f, /// 0x468
			 0x836823dc, /// 0x46c
			 0x3a1f51a7, /// 0x470
			 0x181478fb, /// 0x474
			 0x0884d902, /// 0x478
			 0xfd48ccfb, /// 0x47c
			 0x781349e4, /// 0x480
			 0x378d17cf, /// 0x484
			 0x76bc38f6, /// 0x488
			 0x48c83ba6, /// 0x48c
			 0xc894ed4e, /// 0x490
			 0xa00dcdfc, /// 0x494
			 0xaa760f57, /// 0x498
			 0xcf414327, /// 0x49c
			 0xef93ded9, /// 0x4a0
			 0x81664d6e, /// 0x4a4
			 0xd9683b01, /// 0x4a8
			 0x330d62c7, /// 0x4ac
			 0x6d693b1b, /// 0x4b0
			 0x79b36594, /// 0x4b4
			 0x56a82270, /// 0x4b8
			 0x0e393a3e, /// 0x4bc
			 0x4f1132d9, /// 0x4c0
			 0x0f5212f4, /// 0x4c4
			 0x81ea07a0, /// 0x4c8
			 0xf0c75970, /// 0x4cc
			 0x25fcc404, /// 0x4d0
			 0x3e872da2, /// 0x4d4
			 0x2b411df9, /// 0x4d8
			 0xfd5fa719, /// 0x4dc
			 0x5eb245a0, /// 0x4e0
			 0x24d140f5, /// 0x4e4
			 0x31b18642, /// 0x4e8
			 0xb1ca370a, /// 0x4ec
			 0xf6ea7d16, /// 0x4f0
			 0xd774bb65, /// 0x4f4
			 0xc1f02754, /// 0x4f8
			 0xebb93dde, /// 0x4fc
			 0x41bbc9d0, /// 0x500
			 0xdafc809a, /// 0x504
			 0x817bf875, /// 0x508
			 0x38420ae6, /// 0x50c
			 0x7eb28d02, /// 0x510
			 0x5ed12552, /// 0x514
			 0x5d6c184e, /// 0x518
			 0xadbe58e2, /// 0x51c
			 0x7e7c9375, /// 0x520
			 0x097aa534, /// 0x524
			 0x6bc1fb18, /// 0x528
			 0x7cff6c98, /// 0x52c
			 0x8790da57, /// 0x530
			 0xd3f346cc, /// 0x534
			 0x5910369a, /// 0x538
			 0x4045907f, /// 0x53c
			 0x2bb07bf0, /// 0x540
			 0xfcff6131, /// 0x544
			 0x0f7c4171, /// 0x548
			 0x9c383db3, /// 0x54c
			 0xd3d72c14, /// 0x550
			 0xe397633e, /// 0x554
			 0x9171841a, /// 0x558
			 0x77f20edc, /// 0x55c
			 0x710d217e, /// 0x560
			 0x71e4dcf1, /// 0x564
			 0xdd3a56e3, /// 0x568
			 0x64cc0614, /// 0x56c
			 0x5b90e193, /// 0x570
			 0xeab042e2, /// 0x574
			 0xcdd2f2c6, /// 0x578
			 0x0c92ba9e, /// 0x57c
			 0x9eef0852, /// 0x580
			 0x45e39b16, /// 0x584
			 0xd7983d57, /// 0x588
			 0x603f4539, /// 0x58c
			 0x4b722fbd, /// 0x590
			 0xc42201dc, /// 0x594
			 0xcfec97e3, /// 0x598
			 0x2181e31c, /// 0x59c
			 0x25754f16, /// 0x5a0
			 0x03dfe336, /// 0x5a4
			 0x166bcc50, /// 0x5a8
			 0x4b3f5c0f, /// 0x5ac
			 0x53473547, /// 0x5b0
			 0x864b6653, /// 0x5b4
			 0x3c92f39a, /// 0x5b8
			 0x814f2ca9, /// 0x5bc
			 0xab47777d, /// 0x5c0
			 0x864c03d2, /// 0x5c4
			 0x351499be, /// 0x5c8
			 0x845bf554, /// 0x5cc
			 0x7dedac54, /// 0x5d0
			 0x9e03d214, /// 0x5d4
			 0xef8d28e5, /// 0x5d8
			 0x98a2f2a6, /// 0x5dc
			 0x6deb9e3d, /// 0x5e0
			 0xe74f0188, /// 0x5e4
			 0x4f847905, /// 0x5e8
			 0xed848d2a, /// 0x5ec
			 0xe1a9c5ab, /// 0x5f0
			 0xb14931ec, /// 0x5f4
			 0x1d033788, /// 0x5f8
			 0x44e3fa82, /// 0x5fc
			 0x258fc576, /// 0x600
			 0x3c0e609a, /// 0x604
			 0x00ef12de, /// 0x608
			 0xcc74b1ee, /// 0x60c
			 0x09c1ba49, /// 0x610
			 0xecca3acb, /// 0x614
			 0x6f6a8ad0, /// 0x618
			 0xcb7f7db4, /// 0x61c
			 0x8396d4cf, /// 0x620
			 0xe78863d3, /// 0x624
			 0x5091cd76, /// 0x628
			 0x68487a0c, /// 0x62c
			 0x90275d25, /// 0x630
			 0x9275b73d, /// 0x634
			 0x770584e3, /// 0x638
			 0xbd528395, /// 0x63c
			 0xdfbb7a4f, /// 0x640
			 0xa6d33ef3, /// 0x644
			 0xb8f546e5, /// 0x648
			 0xfb7c6bbe, /// 0x64c
			 0xee087586, /// 0x650
			 0x346eb7ba, /// 0x654
			 0xd1e067b8, /// 0x658
			 0x4a668daa, /// 0x65c
			 0x7fac91c2, /// 0x660
			 0x9015db89, /// 0x664
			 0xe11ff47d, /// 0x668
			 0x3b960d8e, /// 0x66c
			 0xbcd2f98c, /// 0x670
			 0x600b115f, /// 0x674
			 0x84930597, /// 0x678
			 0xc8e1d4ca, /// 0x67c
			 0x03f41891, /// 0x680
			 0xde8e3a45, /// 0x684
			 0x49c163d9, /// 0x688
			 0x62aabb43, /// 0x68c
			 0xa0c329bc, /// 0x690
			 0xf1357697, /// 0x694
			 0x1fcfe87c, /// 0x698
			 0x7d14f891, /// 0x69c
			 0xe931fbc7, /// 0x6a0
			 0xc3a37006, /// 0x6a4
			 0xa20caba7, /// 0x6a8
			 0xcea54075, /// 0x6ac
			 0x909a5fbf, /// 0x6b0
			 0xb18454a4, /// 0x6b4
			 0xc54bbb30, /// 0x6b8
			 0x44b00f19, /// 0x6bc
			 0x1d971a9d, /// 0x6c0
			 0x0a4019ab, /// 0x6c4
			 0xc5d288b1, /// 0x6c8
			 0x8d5ce7f0, /// 0x6cc
			 0x2eddeaed, /// 0x6d0
			 0x95827a57, /// 0x6d4
			 0x2ce387de, /// 0x6d8
			 0x27e4ad55, /// 0x6dc
			 0x8dcad94e, /// 0x6e0
			 0x64754c9b, /// 0x6e4
			 0xc000ac59, /// 0x6e8
			 0xcc897f10, /// 0x6ec
			 0x6eb81e17, /// 0x6f0
			 0x082d9164, /// 0x6f4
			 0x61d6b6c3, /// 0x6f8
			 0x85cdee21, /// 0x6fc
			 0xf30b5aca, /// 0x700
			 0xe0672a21, /// 0x704
			 0x8f19b604, /// 0x708
			 0xb2be0831, /// 0x70c
			 0x39dfb678, /// 0x710
			 0xba2803a5, /// 0x714
			 0x5348e93e, /// 0x718
			 0x5283927b, /// 0x71c
			 0x1eaa15fd, /// 0x720
			 0x0bfd0948, /// 0x724
			 0x8beb047c, /// 0x728
			 0x7b3823b3, /// 0x72c
			 0xb51d3d03, /// 0x730
			 0x1dec22db, /// 0x734
			 0x19780542, /// 0x738
			 0xd94b0dd9, /// 0x73c
			 0x9f37e0fe, /// 0x740
			 0x3b01d43b, /// 0x744
			 0x775f6bbb, /// 0x748
			 0x531e8329, /// 0x74c
			 0xf6e6c5e6, /// 0x750
			 0x9a3c699c, /// 0x754
			 0xe4cd5f73, /// 0x758
			 0x47fb6518, /// 0x75c
			 0xdc7929bc, /// 0x760
			 0xfac035c1, /// 0x764
			 0x93e9c719, /// 0x768
			 0xcb965085, /// 0x76c
			 0xe6fb12d5, /// 0x770
			 0xd85d421e, /// 0x774
			 0xbd34c39d, /// 0x778
			 0x5a91f9d3, /// 0x77c
			 0x0c7f2947, /// 0x780
			 0x8ac4c142, /// 0x784
			 0x14c43659, /// 0x788
			 0x589f2145, /// 0x78c
			 0x70203a33, /// 0x790
			 0x0d1ff729, /// 0x794
			 0xe2d23ec9, /// 0x798
			 0x3d82bb15, /// 0x79c
			 0x4f123bbc, /// 0x7a0
			 0x2fcf3848, /// 0x7a4
			 0x456884c6, /// 0x7a8
			 0x2aba4f43, /// 0x7ac
			 0xd1bf2c85, /// 0x7b0
			 0x8eec040c, /// 0x7b4
			 0x0454f779, /// 0x7b8
			 0x3a37b3a3, /// 0x7bc
			 0xaa12a3bf, /// 0x7c0
			 0x51260769, /// 0x7c4
			 0xd8dcf4e4, /// 0x7c8
			 0x9f3af9c0, /// 0x7cc
			 0x0e6062ba, /// 0x7d0
			 0x78711383, /// 0x7d4
			 0x06b4ee9d, /// 0x7d8
			 0x1c54f941, /// 0x7dc
			 0xfff3c500, /// 0x7e0
			 0x14195388, /// 0x7e4
			 0x91ae47f8, /// 0x7e8
			 0xc67b56b1, /// 0x7ec
			 0xad04bb1a, /// 0x7f0
			 0x6b8844e6, /// 0x7f4
			 0x09f8f899, /// 0x7f8
			 0x4289f510, /// 0x7fc
			 0x47978c1a, /// 0x800
			 0x5decacf3, /// 0x804
			 0xbf554755, /// 0x808
			 0x76391101, /// 0x80c
			 0x44802dbd, /// 0x810
			 0x68f230fc, /// 0x814
			 0x0e2c6439, /// 0x818
			 0xaacbed26, /// 0x81c
			 0x46a20c9c, /// 0x820
			 0x62165a22, /// 0x824
			 0x53e910df, /// 0x828
			 0xd11b1a55, /// 0x82c
			 0x4b1b8af6, /// 0x830
			 0xf28b136a, /// 0x834
			 0x2e61aa19, /// 0x838
			 0x2ff40d68, /// 0x83c
			 0x8f67fff8, /// 0x840
			 0x98bc51e8, /// 0x844
			 0xc4332b91, /// 0x848
			 0xb055cfce, /// 0x84c
			 0xf06a1a77, /// 0x850
			 0x9afefd4e, /// 0x854
			 0x09302c36, /// 0x858
			 0x0e23bec7, /// 0x85c
			 0xef2c375b, /// 0x860
			 0xfdf53a3c, /// 0x864
			 0x570671f2, /// 0x868
			 0x78e43a44, /// 0x86c
			 0xbfe3e892, /// 0x870
			 0x389edca4, /// 0x874
			 0x3fc4e97e, /// 0x878
			 0xe9706e5a, /// 0x87c
			 0xf7708ee6, /// 0x880
			 0x1037e7bb, /// 0x884
			 0xfe80570d, /// 0x888
			 0x99f8c467, /// 0x88c
			 0xde2aaf1b, /// 0x890
			 0xe0295ff1, /// 0x894
			 0xf92d9132, /// 0x898
			 0x466794ab, /// 0x89c
			 0x1c144361, /// 0x8a0
			 0x1d29c3e6, /// 0x8a4
			 0xce9d0b98, /// 0x8a8
			 0xfb15ac59, /// 0x8ac
			 0x76e9b9e0, /// 0x8b0
			 0x5fba4b4c, /// 0x8b4
			 0xe52364ce, /// 0x8b8
			 0xf3d8697d, /// 0x8bc
			 0x92889c30, /// 0x8c0
			 0x6976b7c9, /// 0x8c4
			 0xec20a277, /// 0x8c8
			 0x4f0e80b5, /// 0x8cc
			 0x4cb3561a, /// 0x8d0
			 0xdd5f5437, /// 0x8d4
			 0x36c858ca, /// 0x8d8
			 0xa8cc9dc6, /// 0x8dc
			 0x6ccd4c97, /// 0x8e0
			 0xb2b1d306, /// 0x8e4
			 0xabee26c3, /// 0x8e8
			 0x285b0358, /// 0x8ec
			 0xff7885af, /// 0x8f0
			 0x8da22cd1, /// 0x8f4
			 0x94afb2f0, /// 0x8f8
			 0x2327a814, /// 0x8fc
			 0xb4692303, /// 0x900
			 0x9a6c56ae, /// 0x904
			 0x1d371355, /// 0x908
			 0x484c8866, /// 0x90c
			 0x4aeb2b56, /// 0x910
			 0xfbb98b2e, /// 0x914
			 0xb4712fe8, /// 0x918
			 0x3297b6db, /// 0x91c
			 0x5491abbf, /// 0x920
			 0xe44ea835, /// 0x924
			 0xabc98337, /// 0x928
			 0xcbf4da8b, /// 0x92c
			 0xecabe99e, /// 0x930
			 0xb48e243e, /// 0x934
			 0x94963422, /// 0x938
			 0xf461c1d2, /// 0x93c
			 0x938fed57, /// 0x940
			 0x026c40f1, /// 0x944
			 0x3d99493f, /// 0x948
			 0xc43a412d, /// 0x94c
			 0x3fc7613b, /// 0x950
			 0x44b2cd63, /// 0x954
			 0x8c861301, /// 0x958
			 0xd7e5558d, /// 0x95c
			 0xae2524b3, /// 0x960
			 0xcc3eac9e, /// 0x964
			 0x6184f6dc, /// 0x968
			 0xcffd52b3, /// 0x96c
			 0xa6660281, /// 0x970
			 0xaff96e0d, /// 0x974
			 0xcee41079, /// 0x978
			 0x20cde420, /// 0x97c
			 0x75b72c13, /// 0x980
			 0xc3dac09b, /// 0x984
			 0x5e73a084, /// 0x988
			 0x052fb107, /// 0x98c
			 0x6a8fab0f, /// 0x990
			 0xf7171bd1, /// 0x994
			 0xbcdf862c, /// 0x998
			 0x16dd9cc7, /// 0x99c
			 0xd257d135, /// 0x9a0
			 0xb682d531, /// 0x9a4
			 0xd6f571aa, /// 0x9a8
			 0x94b0edaf, /// 0x9ac
			 0xf247a54b, /// 0x9b0
			 0x6f36c5f9, /// 0x9b4
			 0xe4b915c5, /// 0x9b8
			 0x919638d3, /// 0x9bc
			 0x5cb1acce, /// 0x9c0
			 0x3fca5677, /// 0x9c4
			 0x05a9e32e, /// 0x9c8
			 0x7d3d090a, /// 0x9cc
			 0x561ad026, /// 0x9d0
			 0xe6c28394, /// 0x9d4
			 0x44324b3c, /// 0x9d8
			 0xcc1f54da, /// 0x9dc
			 0x4c138a85, /// 0x9e0
			 0x4281b7f6, /// 0x9e4
			 0xe2ff8379, /// 0x9e8
			 0xf1bb0f29, /// 0x9ec
			 0x18708d89, /// 0x9f0
			 0xdb03a677, /// 0x9f4
			 0x0b228635, /// 0x9f8
			 0xfd93ab69, /// 0x9fc
			 0xbafa7c69, /// 0xa00
			 0xf2d85cf5, /// 0xa04
			 0xaa6c9f80, /// 0xa08
			 0x6b5886ff, /// 0xa0c
			 0xa3b8eaac, /// 0xa10
			 0xa5daef2d, /// 0xa14
			 0x71c5e5cb, /// 0xa18
			 0x5151e837, /// 0xa1c
			 0xb020449c, /// 0xa20
			 0x6a38acfd, /// 0xa24
			 0xbb169f7f, /// 0xa28
			 0x125df320, /// 0xa2c
			 0x0f555c2e, /// 0xa30
			 0xe65f3579, /// 0xa34
			 0xee27ea21, /// 0xa38
			 0xb0e86ebb, /// 0xa3c
			 0x9da2841b, /// 0xa40
			 0x49e4e9d5, /// 0xa44
			 0xe5836380, /// 0xa48
			 0x4ac7cd10, /// 0xa4c
			 0xd7c82328, /// 0xa50
			 0x97f562ae, /// 0xa54
			 0x7bd60dc9, /// 0xa58
			 0xb9a28287, /// 0xa5c
			 0x15a45e9c, /// 0xa60
			 0xf7623052, /// 0xa64
			 0xca7665bc, /// 0xa68
			 0xae41a1a6, /// 0xa6c
			 0xc9ef2549, /// 0xa70
			 0x68fa26bd, /// 0xa74
			 0xc4660332, /// 0xa78
			 0x12a8b9a7, /// 0xa7c
			 0x8e954776, /// 0xa80
			 0x3b8d3d3b, /// 0xa84
			 0x030d697d, /// 0xa88
			 0x041a7419, /// 0xa8c
			 0x151c1e91, /// 0xa90
			 0x7438751f, /// 0xa94
			 0xca34a7ae, /// 0xa98
			 0x331702f8, /// 0xa9c
			 0x0359e116, /// 0xaa0
			 0xcbde9a21, /// 0xaa4
			 0x6d742f0b, /// 0xaa8
			 0x97877d05, /// 0xaac
			 0xc0437b72, /// 0xab0
			 0xe81638f5, /// 0xab4
			 0x16d16526, /// 0xab8
			 0x1a39521c, /// 0xabc
			 0x88dd7bcc, /// 0xac0
			 0x4ac41910, /// 0xac4
			 0xee051a50, /// 0xac8
			 0x87c5dbcb, /// 0xacc
			 0x6ae867b0, /// 0xad0
			 0xf2654d33, /// 0xad4
			 0xaa0daed0, /// 0xad8
			 0x882437a3, /// 0xadc
			 0x07262430, /// 0xae0
			 0xa8d315a3, /// 0xae4
			 0x571c791b, /// 0xae8
			 0x4bbac5db, /// 0xaec
			 0x72a324f6, /// 0xaf0
			 0xc275410f, /// 0xaf4
			 0x48dccbf8, /// 0xaf8
			 0x16be9c7f, /// 0xafc
			 0x3d12cb03, /// 0xb00
			 0x4ca4106e, /// 0xb04
			 0x1192de7d, /// 0xb08
			 0x26cc97a6, /// 0xb0c
			 0xd5220040, /// 0xb10
			 0x3b907b04, /// 0xb14
			 0xc210a6de, /// 0xb18
			 0xb41620e0, /// 0xb1c
			 0x90a8b6b7, /// 0xb20
			 0x4b05b6ba, /// 0xb24
			 0xc56379ab, /// 0xb28
			 0x9ba078f7, /// 0xb2c
			 0x98650978, /// 0xb30
			 0xced45a56, /// 0xb34
			 0x2579ce81, /// 0xb38
			 0x7dfb9e8b, /// 0xb3c
			 0xd29f0dcb, /// 0xb40
			 0xd517266f, /// 0xb44
			 0xc178281c, /// 0xb48
			 0x58558e75, /// 0xb4c
			 0x42a64cea, /// 0xb50
			 0xa4aa6546, /// 0xb54
			 0xbf60834b, /// 0xb58
			 0x62de95e2, /// 0xb5c
			 0xed02f823, /// 0xb60
			 0xd04bca93, /// 0xb64
			 0xf1ec7b48, /// 0xb68
			 0x977eaed3, /// 0xb6c
			 0xb7d532f0, /// 0xb70
			 0xacb6e53a, /// 0xb74
			 0x6e86d20a, /// 0xb78
			 0x7798fe60, /// 0xb7c
			 0x39843574, /// 0xb80
			 0x912f6cf0, /// 0xb84
			 0x1f821593, /// 0xb88
			 0x06fd7e36, /// 0xb8c
			 0x9561f7a3, /// 0xb90
			 0x768555d0, /// 0xb94
			 0xe2d44255, /// 0xb98
			 0xdde04ad1, /// 0xb9c
			 0xd4569a07, /// 0xba0
			 0xd76dd4ba, /// 0xba4
			 0x02f7063a, /// 0xba8
			 0xd40e65ba, /// 0xbac
			 0x9b97dcd5, /// 0xbb0
			 0xef5e03d3, /// 0xbb4
			 0x0717ba85, /// 0xbb8
			 0xc1318dac, /// 0xbbc
			 0xd3cc7ea9, /// 0xbc0
			 0x5c0e0520, /// 0xbc4
			 0x1b286835, /// 0xbc8
			 0xef390693, /// 0xbcc
			 0x73861cae, /// 0xbd0
			 0x595a2123, /// 0xbd4
			 0xb5c0ee1b, /// 0xbd8
			 0xf2c52a38, /// 0xbdc
			 0x9559bd48, /// 0xbe0
			 0x479818f8, /// 0xbe4
			 0xf1a0c132, /// 0xbe8
			 0xfd16f3d4, /// 0xbec
			 0xcb83fd82, /// 0xbf0
			 0x046c83cb, /// 0xbf4
			 0x37db8e05, /// 0xbf8
			 0x531bbdf7, /// 0xbfc
			 0xe919d173, /// 0xc00
			 0x357595e2, /// 0xc04
			 0x4d39d673, /// 0xc08
			 0x3aed6e75, /// 0xc0c
			 0x2e8a6e73, /// 0xc10
			 0x4e1459d0, /// 0xc14
			 0xe861b1cf, /// 0xc18
			 0x75ad79c3, /// 0xc1c
			 0xfc5d0e35, /// 0xc20
			 0xaa04d610, /// 0xc24
			 0x2376879b, /// 0xc28
			 0x12c6efd3, /// 0xc2c
			 0xe0b2fd30, /// 0xc30
			 0xb11a1695, /// 0xc34
			 0xb67d7e66, /// 0xc38
			 0x2d73535d, /// 0xc3c
			 0x20bf6d37, /// 0xc40
			 0x2570ec13, /// 0xc44
			 0x3f6d00d6, /// 0xc48
			 0x34d1191a, /// 0xc4c
			 0xacf62edd, /// 0xc50
			 0x3982cb33, /// 0xc54
			 0x71dd52c4, /// 0xc58
			 0x41119c4e, /// 0xc5c
			 0xc8808d27, /// 0xc60
			 0x95c3c408, /// 0xc64
			 0x368367b0, /// 0xc68
			 0x472ec604, /// 0xc6c
			 0x5196d130, /// 0xc70
			 0x21e89c2d, /// 0xc74
			 0x7fddc2ac, /// 0xc78
			 0xbe150fe1, /// 0xc7c
			 0xc2b54dcb, /// 0xc80
			 0xf56a2b49, /// 0xc84
			 0xc768793c, /// 0xc88
			 0xcc2b5736, /// 0xc8c
			 0xaef07cf3, /// 0xc90
			 0x23ceb311, /// 0xc94
			 0xf86b6f7c, /// 0xc98
			 0x51902294, /// 0xc9c
			 0xd5ef551b, /// 0xca0
			 0xad6bd724, /// 0xca4
			 0x104536f8, /// 0xca8
			 0x7ce67f9b, /// 0xcac
			 0x406f2d2c, /// 0xcb0
			 0x454ec571, /// 0xcb4
			 0xe556ccb9, /// 0xcb8
			 0x5e6fb57f, /// 0xcbc
			 0xdfef0508, /// 0xcc0
			 0x03f5eb77, /// 0xcc4
			 0x679392db, /// 0xcc8
			 0x80b63be4, /// 0xccc
			 0xc6628554, /// 0xcd0
			 0x415aa37c, /// 0xcd4
			 0x2b07163d, /// 0xcd8
			 0x01b15d22, /// 0xcdc
			 0x8d58bf72, /// 0xce0
			 0x10855195, /// 0xce4
			 0x5f6ee45f, /// 0xce8
			 0x21405cfa, /// 0xcec
			 0x659fc950, /// 0xcf0
			 0xd752575f, /// 0xcf4
			 0xf019afa3, /// 0xcf8
			 0x735e545e, /// 0xcfc
			 0x0852d2ba, /// 0xd00
			 0xb55d3fa9, /// 0xd04
			 0x44a0d9e9, /// 0xd08
			 0x2cd10e14, /// 0xd0c
			 0x181ea1eb, /// 0xd10
			 0x4c1f92c1, /// 0xd14
			 0xa04fe859, /// 0xd18
			 0x523c617e, /// 0xd1c
			 0xad2a1fcf, /// 0xd20
			 0xb6bd6745, /// 0xd24
			 0x6d9eb493, /// 0xd28
			 0xebcbd508, /// 0xd2c
			 0x590726fc, /// 0xd30
			 0x1c0be085, /// 0xd34
			 0x305a2e93, /// 0xd38
			 0xa52ffd2a, /// 0xd3c
			 0x143f25e5, /// 0xd40
			 0xf45bada0, /// 0xd44
			 0xf971a853, /// 0xd48
			 0xabf424a2, /// 0xd4c
			 0xb1d68a95, /// 0xd50
			 0x51c2fe0c, /// 0xd54
			 0xcd5e47ce, /// 0xd58
			 0x427ef238, /// 0xd5c
			 0x8f72b757, /// 0xd60
			 0x9cfa4b91, /// 0xd64
			 0x49cb44c1, /// 0xd68
			 0x47d3fce1, /// 0xd6c
			 0x41fa8f1d, /// 0xd70
			 0x8014944b, /// 0xd74
			 0x1ababd32, /// 0xd78
			 0x7b5bd85d, /// 0xd7c
			 0xa575b79b, /// 0xd80
			 0xbb37ffda, /// 0xd84
			 0xb51cb338, /// 0xd88
			 0xe05b81a0, /// 0xd8c
			 0x44fe489d, /// 0xd90
			 0x17bf12c2, /// 0xd94
			 0x202baf6d, /// 0xd98
			 0x8553df85, /// 0xd9c
			 0x19063b6f, /// 0xda0
			 0x324fec4a, /// 0xda4
			 0x34170bd1, /// 0xda8
			 0x86c537ae, /// 0xdac
			 0x748cac83, /// 0xdb0
			 0x770724e9, /// 0xdb4
			 0x048219cf, /// 0xdb8
			 0x1427fcc4, /// 0xdbc
			 0x278954e2, /// 0xdc0
			 0xea402aae, /// 0xdc4
			 0x65cfc9ad, /// 0xdc8
			 0xa7769245, /// 0xdcc
			 0x29ed80ca, /// 0xdd0
			 0xd2b4d129, /// 0xdd4
			 0x6b7a0ef0, /// 0xdd8
			 0xcb993f5a, /// 0xddc
			 0xb0a06722, /// 0xde0
			 0xd5a43135, /// 0xde4
			 0xe9b7a9b7, /// 0xde8
			 0xcf53edec, /// 0xdec
			 0xf5ee9958, /// 0xdf0
			 0xded2b29c, /// 0xdf4
			 0xcdc13c2b, /// 0xdf8
			 0x92158f27, /// 0xdfc
			 0x534b15a3, /// 0xe00
			 0x9195bccd, /// 0xe04
			 0x5e678985, /// 0xe08
			 0x11570753, /// 0xe0c
			 0x754b6eab, /// 0xe10
			 0x500c289e, /// 0xe14
			 0xba1cdd9a, /// 0xe18
			 0x1c21cf7c, /// 0xe1c
			 0x48459c83, /// 0xe20
			 0x2ad2568f, /// 0xe24
			 0x683e364c, /// 0xe28
			 0x376265e7, /// 0xe2c
			 0xa994dcb3, /// 0xe30
			 0x995a4e0c, /// 0xe34
			 0x273eaaea, /// 0xe38
			 0x56303e45, /// 0xe3c
			 0x2db9aa7d, /// 0xe40
			 0xcddfad93, /// 0xe44
			 0x1da55267, /// 0xe48
			 0xb650082e, /// 0xe4c
			 0x3bc43bd6, /// 0xe50
			 0xa1a59a14, /// 0xe54
			 0xb52eb148, /// 0xe58
			 0x25c0a3b2, /// 0xe5c
			 0xc1773b31, /// 0xe60
			 0x6efe48ae, /// 0xe64
			 0xb5775560, /// 0xe68
			 0xa5fd63cd, /// 0xe6c
			 0x89bc8877, /// 0xe70
			 0xd84ec157, /// 0xe74
			 0x2b6c7a0b, /// 0xe78
			 0x464c2e38, /// 0xe7c
			 0x8bcb343e, /// 0xe80
			 0x19fd667e, /// 0xe84
			 0x4730bf24, /// 0xe88
			 0x7ecca8c9, /// 0xe8c
			 0x7ec31b6a, /// 0xe90
			 0xf53ff75d, /// 0xe94
			 0xa4ee4e5c, /// 0xe98
			 0xc76c991a, /// 0xe9c
			 0x67014d8a, /// 0xea0
			 0x7e239519, /// 0xea4
			 0x135f1a11, /// 0xea8
			 0xc4fd1d0b, /// 0xeac
			 0xfcf67a7a, /// 0xeb0
			 0x43699bc2, /// 0xeb4
			 0xd8afca99, /// 0xeb8
			 0x07347e4e, /// 0xebc
			 0x3e03beda, /// 0xec0
			 0x7b3c5b70, /// 0xec4
			 0x1811ddd3, /// 0xec8
			 0x25178520, /// 0xecc
			 0xeee2f7e8, /// 0xed0
			 0xf2b41eb8, /// 0xed4
			 0x5d69b55a, /// 0xed8
			 0x28a0d21c, /// 0xedc
			 0x49709786, /// 0xee0
			 0x52fce480, /// 0xee4
			 0x96cf43d4, /// 0xee8
			 0xc19e1d5f, /// 0xeec
			 0x2f80458d, /// 0xef0
			 0xb4ac6202, /// 0xef4
			 0xaceba2c0, /// 0xef8
			 0xceeb4dea, /// 0xefc
			 0xbd5f9ac4, /// 0xf00
			 0x6f04e225, /// 0xf04
			 0x3e9d4696, /// 0xf08
			 0x938a2508, /// 0xf0c
			 0x226d4e46, /// 0xf10
			 0x324794b1, /// 0xf14
			 0x28907cee, /// 0xf18
			 0xff53161c, /// 0xf1c
			 0xf4e9f871, /// 0xf20
			 0x4398a077, /// 0xf24
			 0xf7fc4e44, /// 0xf28
			 0x6262f856, /// 0xf2c
			 0xbf7e0042, /// 0xf30
			 0x6a897267, /// 0xf34
			 0x07339661, /// 0xf38
			 0x959ee5d7, /// 0xf3c
			 0x1f05472f, /// 0xf40
			 0x94a36ac8, /// 0xf44
			 0xdd4f00ee, /// 0xf48
			 0x648a4c09, /// 0xf4c
			 0x34ae8169, /// 0xf50
			 0x5440b1e8, /// 0xf54
			 0x6be02cf5, /// 0xf58
			 0x3d921ff8, /// 0xf5c
			 0x16abbd29, /// 0xf60
			 0x190a70f7, /// 0xf64
			 0xb261674b, /// 0xf68
			 0x5aa86496, /// 0xf6c
			 0x81ae811d, /// 0xf70
			 0xd02e3500, /// 0xf74
			 0x8d4e4edc, /// 0xf78
			 0x85be61af, /// 0xf7c
			 0x85419051, /// 0xf80
			 0xfb7a1d57, /// 0xf84
			 0xc3fb05a9, /// 0xf88
			 0xa812db7a, /// 0xf8c
			 0xacd6ecf8, /// 0xf90
			 0x951e4200, /// 0xf94
			 0x98dc5323, /// 0xf98
			 0x9cc643cb, /// 0xf9c
			 0x1cf66024, /// 0xfa0
			 0xb3ad8c50, /// 0xfa4
			 0x3177afdf, /// 0xfa8
			 0x9266449f, /// 0xfac
			 0xcc769272, /// 0xfb0
			 0x06c55cd4, /// 0xfb4
			 0x0d41433a, /// 0xfb8
			 0x2c690119, /// 0xfbc
			 0x14e875e9, /// 0xfc0
			 0x24357fa7, /// 0xfc4
			 0x7db52f28, /// 0xfc8
			 0x262af6d9, /// 0xfcc
			 0x1e967f44, /// 0xfd0
			 0xc76b6896, /// 0xfd4
			 0x3bacc37b, /// 0xfd8
			 0x32621b75, /// 0xfdc
			 0x6ebfa8fe, /// 0xfe0
			 0x410e9502, /// 0xfe4
			 0x9f3c2f62, /// 0xfe8
			 0xd0d9e3e1, /// 0xfec
			 0x81b8a6df, /// 0xff0
			 0x690be781, /// 0xff4
			 0x28918ae2, /// 0xff8
			 0x779e0112 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00000
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00008
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00010
			 0xff800000,                                                  // -inf                                        /// 00014
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00018
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00024
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00028
			 0x3f028f5c,                                                  // 0.51                                        /// 0002c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00030
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00034
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00038
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0003c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00040
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00044
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00050
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00054
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00060
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00064
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0006c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00070
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00074
			 0x33333333,                                                  // 4 random values                             /// 00078
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0007c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00084
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00088
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0008c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00090
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00094
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e8
			 0x33333333,                                                  // 4 random values                             /// 000ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xbf028f5c,                                                  // -0.51                                       /// 00104
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00108
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0010c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00110
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00114
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00118
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x7f800000,                                                  // inf                                         /// 00124
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0012c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00130
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00138
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00144
			 0x0e000003,                                                  // Trailing 1s:                                /// 00148
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0e000001,                                                  // Trailing 1s:                                /// 00154
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00158
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0015c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00164
			 0x00011111,                                                  // 9.7958E-41                                  /// 00168
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0016c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00170
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00174
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00178
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00180
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00184
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00188
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x7f800000,                                                  // inf                                         /// 00190
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00198
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0xbf028f5c,                                                  // -0.51                                       /// 001a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00200
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0020c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00214
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00218
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00220
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00224
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00228
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0022c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00234
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00240
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00248
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00250
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00254
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00258
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0025c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00260
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00268
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x4b000000,                                                  // 8388608.0                                   /// 00270
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00274
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00280
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00284
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00288
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c700000,                                                  // Leading 1s:                                 /// 00294
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00298
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f0
			 0xcb000000,                                                  // -8388608.0                                  /// 002f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00300
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00308
			 0x0c700000,                                                  // Leading 1s:                                 /// 0030c
			 0xbf028f5c,                                                  // -0.51                                       /// 00310
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00314
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00318
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0031c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00324
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00328
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00330
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00338
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0033c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00344
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00354
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00358
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0035c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00364
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00368
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00370
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00374
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00380
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x4b000000,                                                  // 8388608.0                                   /// 00388
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00390
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00394
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0039c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x7fc00001,                                                  // signaling NaN                               /// 003bc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00404
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00418
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0041c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00424
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00438
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0043c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x80000000,                                                  // -zero                                       /// 0044c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00450
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0045c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00460
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00464
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00468
			 0xbf028f5c,                                                  // -0.51                                       /// 0046c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00474
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00478
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0047c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00484
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00488
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0048c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00494
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00498
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c4
			 0xcb000000,                                                  // -8388608.0                                  /// 004c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x7f800000,                                                  // inf                                         /// 004d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004d8
			 0xff800000,                                                  // -inf                                        /// 004dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00504
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00508
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00510
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0051c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00520
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00524
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00534
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00538
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00540
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00544
			 0x00011111,                                                  // 9.7958E-41                                  /// 00548
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00550
			 0xbf028f5c,                                                  // -0.51                                       /// 00554
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0055c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00560
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00568
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0056c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00570
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00580
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00584
			 0x0e000003,                                                  // Trailing 1s:                                /// 00588
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0058c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00590
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00594
			 0x3f028f5c,                                                  // 0.51                                        /// 00598
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0059c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0xff800000,                                                  // -inf                                        /// 005c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0xffc00001,                                                  // -signaling NaN                              /// 005e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00600
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00614
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0061c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00624
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00628
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x7f800000,                                                  // inf                                         /// 00630
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00640
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00644
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00648
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0064c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00654
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00658
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0065c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00668
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0066c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0067c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00680
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00684
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0068c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00694
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00698
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0069c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 006d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f0
			 0xbf028f5c,                                                  // -0.51                                       /// 006f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006f8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00700
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00704
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00708
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0070c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00714
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00718
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0071c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00720
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0072c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x4b000000,                                                  // 8388608.0                                   /// 00734
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00738
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0073c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00740
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00744
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00748
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00750
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00754
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0075c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x00000000,                                                  // zero                                        /// 00768
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0076c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00778
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0077c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00780
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00784
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0078c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00790
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00798
			 0x00000000,                                                  // zero                                        /// 0079c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 007d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 007e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00800
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00810
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00814
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00818
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0081c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00820
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00824
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00828
			 0x0c600000,                                                  // Leading 1s:                                 /// 0082c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00830
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x0e000007,                                                  // Trailing 1s:                                /// 0083c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00840
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00848
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0084c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00854
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00868
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0086c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00870
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00874
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00878
			 0x80000000,                                                  // -zero                                       /// 0087c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00880
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00884
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00888
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0088c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00890
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0xbf028f5c,                                                  // -0.51                                       /// 00898
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00900
			 0x80000000,                                                  // -zero                                       /// 00904
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0090c
			 0x33333333,                                                  // 4 random values                             /// 00910
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00914
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0091c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00920
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0092c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00930
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00934
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00938
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00940
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00944
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00948
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0095c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00960
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00964
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00968
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x0c780000,                                                  // Leading 1s:                                 /// 00974
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00978
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0097c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00980
			 0x00000000,                                                  // zero                                        /// 00984
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0098c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00994
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0099c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009ac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009b8
			 0x55555555,                                                  // 4 random values                             /// 009bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a5c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a60
			 0xbf028f5c,                                                  // -0.51                                       /// 00a64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a90
			 0xcb000000,                                                  // -8388608.0                                  /// 00a94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00aa0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00aa8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ab0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ab4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00abc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ac0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000000,                                                  // zero                                        /// 00ac8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00acc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ad8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ae8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00af8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00afc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b04
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b08
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b18
			 0xffc00001,                                                  // -signaling NaN                              /// 00b1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b24
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b4c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b54
			 0xff800000,                                                  // -inf                                        /// 00b58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b5c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x55555555,                                                  // 4 random values                             /// 00b88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ba8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bb4
			 0x33333333,                                                  // 4 random values                             /// 00bb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00be0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c20
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c34
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c54
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c64
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0xff800000,                                                  // -inf                                        /// 00c9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ca0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ca8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cc4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cd0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cdc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cf8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d20
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d70
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d90
			 0x33333333,                                                  // 4 random values                             /// 00d94
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00db4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dd0
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ddc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00de0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00de4
			 0x80000000,                                                  // -zero                                       /// 00de8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00df8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e38
			 0x80000000,                                                  // -zero                                       /// 00e3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e4c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e58
			 0x4b000000,                                                  // 8388608.0                                   /// 00e5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e78
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ea0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ea8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ebc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ec0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ec4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00edc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ef0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ef4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ef8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00efc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f2c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f30
			 0xffc00001,                                                  // -signaling NaN                              /// 00f34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f68
			 0xcb000000,                                                  // -8388608.0                                  /// 00f6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f74
			 0x7fc00001,                                                  // signaling NaN                               /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x7f800000,                                                  // inf                                         /// 00fa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fa8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fdc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fe4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x55555555,                                                  // 4 random values                             /// 00ff0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ff4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff8
			 0x00800003                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x24fa7c83, /// 0x0
			 0x2ffdc0ad, /// 0x4
			 0x1b1fdcf2, /// 0x8
			 0xb8dfcbd8, /// 0xc
			 0x37c7854c, /// 0x10
			 0xb9086457, /// 0x14
			 0x6774f645, /// 0x18
			 0x21424373, /// 0x1c
			 0x48d2319e, /// 0x20
			 0x29f5a41c, /// 0x24
			 0x6accf046, /// 0x28
			 0x7f3f4e29, /// 0x2c
			 0x7ce765ec, /// 0x30
			 0xea1d3d14, /// 0x34
			 0xcd90ea91, /// 0x38
			 0x5539b565, /// 0x3c
			 0xb1372b90, /// 0x40
			 0x3c514b35, /// 0x44
			 0x371bda0a, /// 0x48
			 0x55551c99, /// 0x4c
			 0xbc239b66, /// 0x50
			 0xcb24fb99, /// 0x54
			 0x61619bc9, /// 0x58
			 0x9ee8232c, /// 0x5c
			 0xf002fed9, /// 0x60
			 0x7e680320, /// 0x64
			 0x46ae9f8b, /// 0x68
			 0xc7441cc8, /// 0x6c
			 0xdc03d04a, /// 0x70
			 0x66d8d59b, /// 0x74
			 0x7e427e2b, /// 0x78
			 0x1d7f2f19, /// 0x7c
			 0x82b30f75, /// 0x80
			 0x1a641a8e, /// 0x84
			 0xbd78884f, /// 0x88
			 0xf8539a43, /// 0x8c
			 0xd539be11, /// 0x90
			 0xda9007cf, /// 0x94
			 0x55f65b24, /// 0x98
			 0xd266096f, /// 0x9c
			 0x54d804da, /// 0xa0
			 0xaeb74b47, /// 0xa4
			 0xe6a48a5c, /// 0xa8
			 0x2b7ca9d8, /// 0xac
			 0xded4ed7d, /// 0xb0
			 0x7d7b482c, /// 0xb4
			 0x73652e71, /// 0xb8
			 0xdba19db2, /// 0xbc
			 0x21b425b0, /// 0xc0
			 0xa92b89ba, /// 0xc4
			 0x6c266366, /// 0xc8
			 0x018e9d1d, /// 0xcc
			 0xff445d76, /// 0xd0
			 0x9da1826b, /// 0xd4
			 0xb0b4e8c1, /// 0xd8
			 0xa500fc31, /// 0xdc
			 0xaf96ae3a, /// 0xe0
			 0x32ce811a, /// 0xe4
			 0x2609500f, /// 0xe8
			 0xa1bd3460, /// 0xec
			 0x50f536ff, /// 0xf0
			 0x33f88557, /// 0xf4
			 0x797f7117, /// 0xf8
			 0x4723e179, /// 0xfc
			 0x19280414, /// 0x100
			 0x11031925, /// 0x104
			 0xbd548b5e, /// 0x108
			 0x9c079c5a, /// 0x10c
			 0xb983b5ce, /// 0x110
			 0xb24c2f7d, /// 0x114
			 0xf2acd059, /// 0x118
			 0x884c7173, /// 0x11c
			 0x45bb57be, /// 0x120
			 0x96573f03, /// 0x124
			 0x6f3838bc, /// 0x128
			 0x544e1c44, /// 0x12c
			 0xec585bcf, /// 0x130
			 0x5730834e, /// 0x134
			 0x981ede1b, /// 0x138
			 0xd09497fa, /// 0x13c
			 0x350e57e1, /// 0x140
			 0x4a803c3c, /// 0x144
			 0xa67e02bb, /// 0x148
			 0xa3251b12, /// 0x14c
			 0xfec65e3b, /// 0x150
			 0x1918e43a, /// 0x154
			 0xcebe9821, /// 0x158
			 0x0a7baa07, /// 0x15c
			 0x730ca236, /// 0x160
			 0x003b6e87, /// 0x164
			 0xd26aceb5, /// 0x168
			 0xce657902, /// 0x16c
			 0x4710c649, /// 0x170
			 0x8e3d415d, /// 0x174
			 0x50ade914, /// 0x178
			 0xa4e611e7, /// 0x17c
			 0x4672e7b0, /// 0x180
			 0xadf53ff4, /// 0x184
			 0xce106217, /// 0x188
			 0xddfe882a, /// 0x18c
			 0x8a59be7e, /// 0x190
			 0x66bc21ce, /// 0x194
			 0xed312a6c, /// 0x198
			 0xb6037832, /// 0x19c
			 0xe6a5148d, /// 0x1a0
			 0xf19183d1, /// 0x1a4
			 0xbae595ca, /// 0x1a8
			 0x07331188, /// 0x1ac
			 0x476c155e, /// 0x1b0
			 0x4e934cbd, /// 0x1b4
			 0xf29130a7, /// 0x1b8
			 0x1ccdf7bc, /// 0x1bc
			 0xc63ca3e3, /// 0x1c0
			 0x7b5e4101, /// 0x1c4
			 0x55e62f34, /// 0x1c8
			 0x7df8e225, /// 0x1cc
			 0x9d6a7591, /// 0x1d0
			 0x3c6869f4, /// 0x1d4
			 0xac7822bd, /// 0x1d8
			 0x355912a6, /// 0x1dc
			 0xaa18d6b8, /// 0x1e0
			 0xf9d88b65, /// 0x1e4
			 0x526ee5db, /// 0x1e8
			 0xb8474574, /// 0x1ec
			 0x176a8b1f, /// 0x1f0
			 0x333396fe, /// 0x1f4
			 0xc664844d, /// 0x1f8
			 0xa0aaf264, /// 0x1fc
			 0xf6010d9d, /// 0x200
			 0x56d811a3, /// 0x204
			 0x3c115b7f, /// 0x208
			 0xf2c2b2ae, /// 0x20c
			 0xec7338e1, /// 0x210
			 0xa35047ee, /// 0x214
			 0xb6d7e279, /// 0x218
			 0xbbba7ee4, /// 0x21c
			 0x18d5160d, /// 0x220
			 0x770a010a, /// 0x224
			 0x7ae55f5b, /// 0x228
			 0xba8eca7c, /// 0x22c
			 0xb004494a, /// 0x230
			 0x24c8d902, /// 0x234
			 0xaba26d60, /// 0x238
			 0x880b7484, /// 0x23c
			 0x21e03ba5, /// 0x240
			 0x1d077347, /// 0x244
			 0xc0227259, /// 0x248
			 0x55b5484f, /// 0x24c
			 0x778bb6fe, /// 0x250
			 0xfa49790a, /// 0x254
			 0x264b9548, /// 0x258
			 0xd8395445, /// 0x25c
			 0x4d6da7ed, /// 0x260
			 0xa74cf5c1, /// 0x264
			 0x26a5aad8, /// 0x268
			 0x9205cd6b, /// 0x26c
			 0x53c6963f, /// 0x270
			 0x07c05f9d, /// 0x274
			 0xa5489244, /// 0x278
			 0x6144d90d, /// 0x27c
			 0x9d4d947d, /// 0x280
			 0xea3f04c5, /// 0x284
			 0xb7804feb, /// 0x288
			 0xad045f0e, /// 0x28c
			 0x5c0bdda8, /// 0x290
			 0x36aa9d47, /// 0x294
			 0xb37c9c9e, /// 0x298
			 0x8f0e5d7b, /// 0x29c
			 0xd14e02a6, /// 0x2a0
			 0x23d5760a, /// 0x2a4
			 0xc2ce5135, /// 0x2a8
			 0x61a2f7a4, /// 0x2ac
			 0x2759c518, /// 0x2b0
			 0xc0299aae, /// 0x2b4
			 0xcfbc1b24, /// 0x2b8
			 0x9e01ea73, /// 0x2bc
			 0x178b62e7, /// 0x2c0
			 0xbcf62b7e, /// 0x2c4
			 0x69151270, /// 0x2c8
			 0x81535b3b, /// 0x2cc
			 0x492a8502, /// 0x2d0
			 0xe7d73738, /// 0x2d4
			 0x698b6048, /// 0x2d8
			 0x9d86887d, /// 0x2dc
			 0x5b75a896, /// 0x2e0
			 0xc1d8a311, /// 0x2e4
			 0xb94b8e32, /// 0x2e8
			 0x02f935dd, /// 0x2ec
			 0x7dbe6e0d, /// 0x2f0
			 0x0c39f524, /// 0x2f4
			 0xd9f5fae4, /// 0x2f8
			 0x86798174, /// 0x2fc
			 0x38618518, /// 0x300
			 0x3eae2c3b, /// 0x304
			 0x34e7539e, /// 0x308
			 0x36c2a07d, /// 0x30c
			 0x4e8f533e, /// 0x310
			 0xb0020456, /// 0x314
			 0xdedce713, /// 0x318
			 0x1c539e8b, /// 0x31c
			 0x6b0315c5, /// 0x320
			 0x603fe2e7, /// 0x324
			 0x797cd0fa, /// 0x328
			 0x7a1417f5, /// 0x32c
			 0x2135b8b9, /// 0x330
			 0x76ed52c3, /// 0x334
			 0x7a1bb87b, /// 0x338
			 0xb4af2a9f, /// 0x33c
			 0xb72dfa4f, /// 0x340
			 0x89ba8920, /// 0x344
			 0x270850f1, /// 0x348
			 0x49f917dc, /// 0x34c
			 0x5c923ae6, /// 0x350
			 0x9d991c2b, /// 0x354
			 0xc1b266c7, /// 0x358
			 0x545a18b1, /// 0x35c
			 0x89899cf2, /// 0x360
			 0xcdb979a0, /// 0x364
			 0x887189bf, /// 0x368
			 0x63d0dbac, /// 0x36c
			 0x757410c7, /// 0x370
			 0x228debbe, /// 0x374
			 0x7b58237d, /// 0x378
			 0xa2c63d0e, /// 0x37c
			 0x7e65d194, /// 0x380
			 0xd3f6a8e0, /// 0x384
			 0xdccd279a, /// 0x388
			 0xd7788ba3, /// 0x38c
			 0x045e5252, /// 0x390
			 0xc47aa399, /// 0x394
			 0x574d2d94, /// 0x398
			 0xba15d640, /// 0x39c
			 0x285973fe, /// 0x3a0
			 0x6977d9a8, /// 0x3a4
			 0x4cce1e51, /// 0x3a8
			 0x917905c5, /// 0x3ac
			 0x04496cd7, /// 0x3b0
			 0xc8c12de0, /// 0x3b4
			 0xdf4836bc, /// 0x3b8
			 0x738def95, /// 0x3bc
			 0x7fcac1dd, /// 0x3c0
			 0xa8304ccd, /// 0x3c4
			 0x34dda180, /// 0x3c8
			 0x1db08194, /// 0x3cc
			 0x3283bdbc, /// 0x3d0
			 0xa2b6d5fc, /// 0x3d4
			 0xdce559fa, /// 0x3d8
			 0xf3118dcf, /// 0x3dc
			 0xe0f89a6d, /// 0x3e0
			 0x361d76c0, /// 0x3e4
			 0xff3611f5, /// 0x3e8
			 0x38874557, /// 0x3ec
			 0x7cc2ad3f, /// 0x3f0
			 0xd4b2568b, /// 0x3f4
			 0x35be0b63, /// 0x3f8
			 0x3df659ad, /// 0x3fc
			 0xa8a88582, /// 0x400
			 0x166bf0bd, /// 0x404
			 0x1d4da7db, /// 0x408
			 0xfd70f3ed, /// 0x40c
			 0x571e6e3f, /// 0x410
			 0x51816682, /// 0x414
			 0x8b5e7ae2, /// 0x418
			 0x4dfdc40e, /// 0x41c
			 0x41d9d559, /// 0x420
			 0x45e0c2e8, /// 0x424
			 0x2f9e5273, /// 0x428
			 0x742ad679, /// 0x42c
			 0x39034b54, /// 0x430
			 0x2a3795ca, /// 0x434
			 0x1840afe9, /// 0x438
			 0x6da3ae66, /// 0x43c
			 0x5d2eb3a0, /// 0x440
			 0xb71680a2, /// 0x444
			 0x00d735a7, /// 0x448
			 0x4736667c, /// 0x44c
			 0xac53415f, /// 0x450
			 0x3dd0b9e3, /// 0x454
			 0x61e80aae, /// 0x458
			 0xe8e4e5c8, /// 0x45c
			 0xc93b8aa3, /// 0x460
			 0x94222d81, /// 0x464
			 0x41307978, /// 0x468
			 0xa61695eb, /// 0x46c
			 0x6a5ce7e7, /// 0x470
			 0xd9371725, /// 0x474
			 0x36a7972e, /// 0x478
			 0xf0c1b14e, /// 0x47c
			 0xcd1062e8, /// 0x480
			 0xce977960, /// 0x484
			 0x7c5edbc7, /// 0x488
			 0xd052676c, /// 0x48c
			 0x55d93048, /// 0x490
			 0x65cb7fb9, /// 0x494
			 0x3947d53b, /// 0x498
			 0xfd44b85d, /// 0x49c
			 0x75a45912, /// 0x4a0
			 0x7262f32e, /// 0x4a4
			 0x5e436004, /// 0x4a8
			 0xf0a9db4b, /// 0x4ac
			 0x3beb23be, /// 0x4b0
			 0xab6a835d, /// 0x4b4
			 0x80c42a1b, /// 0x4b8
			 0xc3617e43, /// 0x4bc
			 0x3ca0a37b, /// 0x4c0
			 0x9fde232a, /// 0x4c4
			 0x5ac1cf65, /// 0x4c8
			 0xb6ac4677, /// 0x4cc
			 0x19ad85a4, /// 0x4d0
			 0x09764ee4, /// 0x4d4
			 0xd2d42005, /// 0x4d8
			 0x95d8c227, /// 0x4dc
			 0x2650bd49, /// 0x4e0
			 0xea4ae29a, /// 0x4e4
			 0xc543f2f8, /// 0x4e8
			 0x584277b2, /// 0x4ec
			 0xc7b083bd, /// 0x4f0
			 0x0cdc36db, /// 0x4f4
			 0x2339181b, /// 0x4f8
			 0x9ef628ac, /// 0x4fc
			 0x400bf61c, /// 0x500
			 0xdecd86d8, /// 0x504
			 0x8c066850, /// 0x508
			 0x43b12a32, /// 0x50c
			 0x82750546, /// 0x510
			 0xb91eef6d, /// 0x514
			 0x5ae38ea7, /// 0x518
			 0xa64cf027, /// 0x51c
			 0xeaf0be3e, /// 0x520
			 0xeb5f8035, /// 0x524
			 0x1872c1b2, /// 0x528
			 0x801e0ffd, /// 0x52c
			 0x4b692901, /// 0x530
			 0x56ce5975, /// 0x534
			 0x68fae122, /// 0x538
			 0xca5edbdb, /// 0x53c
			 0x07f6e2ee, /// 0x540
			 0xb6c0a566, /// 0x544
			 0x8f1f5a90, /// 0x548
			 0x163c90db, /// 0x54c
			 0x08a0540e, /// 0x550
			 0xb24d704f, /// 0x554
			 0x2d2f269f, /// 0x558
			 0x697bb16f, /// 0x55c
			 0xa2f2786a, /// 0x560
			 0x5b6d09ce, /// 0x564
			 0xf8644bb2, /// 0x568
			 0x33b2d7fe, /// 0x56c
			 0xefc3046d, /// 0x570
			 0x7e3aff9b, /// 0x574
			 0x51057240, /// 0x578
			 0x051a484e, /// 0x57c
			 0x9b368e71, /// 0x580
			 0x1431dafd, /// 0x584
			 0x41a78246, /// 0x588
			 0xfe3476ab, /// 0x58c
			 0xd8ba40f5, /// 0x590
			 0xff49c1e3, /// 0x594
			 0xeb21539c, /// 0x598
			 0x38ca283e, /// 0x59c
			 0x2a064982, /// 0x5a0
			 0xae267235, /// 0x5a4
			 0x8983dfc9, /// 0x5a8
			 0x13a0fb83, /// 0x5ac
			 0x3f81f502, /// 0x5b0
			 0xd1566cf3, /// 0x5b4
			 0x1e879abd, /// 0x5b8
			 0x32ec88b6, /// 0x5bc
			 0x86be16ac, /// 0x5c0
			 0x38f858e2, /// 0x5c4
			 0xc618bc22, /// 0x5c8
			 0xb14cb059, /// 0x5cc
			 0xdf263d32, /// 0x5d0
			 0xf40385a2, /// 0x5d4
			 0x52a28b8c, /// 0x5d8
			 0x7411e8ee, /// 0x5dc
			 0x425c6bb3, /// 0x5e0
			 0xa1fd04ae, /// 0x5e4
			 0x400f72c7, /// 0x5e8
			 0x6308e972, /// 0x5ec
			 0xff9653c8, /// 0x5f0
			 0x9ce5fbf3, /// 0x5f4
			 0x9de0ac23, /// 0x5f8
			 0xab17c439, /// 0x5fc
			 0x95f1e1ba, /// 0x600
			 0x4a6248a6, /// 0x604
			 0x6655ade1, /// 0x608
			 0xa8fe2006, /// 0x60c
			 0x26884f4e, /// 0x610
			 0xc993785f, /// 0x614
			 0xba80ac39, /// 0x618
			 0x828e198e, /// 0x61c
			 0x44d701fb, /// 0x620
			 0xd205f71c, /// 0x624
			 0xc13b816d, /// 0x628
			 0xf18c9ca6, /// 0x62c
			 0x5428e7d9, /// 0x630
			 0x4a6faa27, /// 0x634
			 0x927aa871, /// 0x638
			 0x23126e25, /// 0x63c
			 0x0cb3fc40, /// 0x640
			 0x825de0e9, /// 0x644
			 0x6ac225e0, /// 0x648
			 0x261d858a, /// 0x64c
			 0x2c984bf9, /// 0x650
			 0xa36a7ec1, /// 0x654
			 0x7eee93be, /// 0x658
			 0xb77d6d6e, /// 0x65c
			 0xc04138e9, /// 0x660
			 0x2409bd68, /// 0x664
			 0x40ffcf75, /// 0x668
			 0x1ea2fd88, /// 0x66c
			 0x5753d193, /// 0x670
			 0xeb1b498f, /// 0x674
			 0xbd7b925b, /// 0x678
			 0x3abd5da9, /// 0x67c
			 0x3f03a128, /// 0x680
			 0x3ee48464, /// 0x684
			 0xa9994bee, /// 0x688
			 0x4cbc1560, /// 0x68c
			 0x914c3402, /// 0x690
			 0x391f7ffa, /// 0x694
			 0xebe1b9d8, /// 0x698
			 0xa3d7cdd8, /// 0x69c
			 0xf363dae1, /// 0x6a0
			 0xcceadff2, /// 0x6a4
			 0x0446a267, /// 0x6a8
			 0x0822ed35, /// 0x6ac
			 0x75a222ed, /// 0x6b0
			 0x9822830d, /// 0x6b4
			 0xee15f1ef, /// 0x6b8
			 0x7812f34c, /// 0x6bc
			 0xa5443351, /// 0x6c0
			 0x48094294, /// 0x6c4
			 0x01b6ebd7, /// 0x6c8
			 0x85f1f6e8, /// 0x6cc
			 0x95ac4613, /// 0x6d0
			 0xd560361c, /// 0x6d4
			 0xcc599beb, /// 0x6d8
			 0x2a37e5a3, /// 0x6dc
			 0xf26cc2fb, /// 0x6e0
			 0x1d83cbc9, /// 0x6e4
			 0x86dbe58e, /// 0x6e8
			 0x73043f80, /// 0x6ec
			 0x9d5b0d92, /// 0x6f0
			 0x434dc6ba, /// 0x6f4
			 0x6bc9fb8b, /// 0x6f8
			 0x9b79578c, /// 0x6fc
			 0xd2de8c74, /// 0x700
			 0x5a1a2f9b, /// 0x704
			 0x8c8a2e78, /// 0x708
			 0x9ef491f8, /// 0x70c
			 0xadb0d572, /// 0x710
			 0x3bf41c14, /// 0x714
			 0x8c6fe2e8, /// 0x718
			 0x02ba8e3e, /// 0x71c
			 0xf84dd9d3, /// 0x720
			 0xcb9bf7d7, /// 0x724
			 0x55b31b67, /// 0x728
			 0x1ffdebd9, /// 0x72c
			 0x9f4e3633, /// 0x730
			 0xcecba3cd, /// 0x734
			 0xb2930c45, /// 0x738
			 0x56fb682b, /// 0x73c
			 0x399b1b65, /// 0x740
			 0xcc208ec1, /// 0x744
			 0x774ff89c, /// 0x748
			 0xf667b195, /// 0x74c
			 0x348a0383, /// 0x750
			 0x4aca6240, /// 0x754
			 0x3ffb3cae, /// 0x758
			 0x9d47d323, /// 0x75c
			 0x6b5eb3f9, /// 0x760
			 0x6048e31a, /// 0x764
			 0x31b59b4f, /// 0x768
			 0xf6bca3c9, /// 0x76c
			 0x0dbb351d, /// 0x770
			 0xd5a70f12, /// 0x774
			 0xe13bd55a, /// 0x778
			 0x812fca8c, /// 0x77c
			 0x55d1ec93, /// 0x780
			 0x3e9619a6, /// 0x784
			 0x9d02c836, /// 0x788
			 0xeca81ae1, /// 0x78c
			 0xd03ff57d, /// 0x790
			 0x8cfcd0d4, /// 0x794
			 0xd6fc4ad5, /// 0x798
			 0x885cc074, /// 0x79c
			 0x5241ed3f, /// 0x7a0
			 0x0a17eade, /// 0x7a4
			 0x86b94648, /// 0x7a8
			 0x6c3bb1f1, /// 0x7ac
			 0x31349e02, /// 0x7b0
			 0x39d7a2ad, /// 0x7b4
			 0xe291840d, /// 0x7b8
			 0x5ffff3e4, /// 0x7bc
			 0x64c61cfa, /// 0x7c0
			 0x0d49d934, /// 0x7c4
			 0x8e023d44, /// 0x7c8
			 0x0b9a2369, /// 0x7cc
			 0x378b84c2, /// 0x7d0
			 0x65004925, /// 0x7d4
			 0xf3911953, /// 0x7d8
			 0xe8642f81, /// 0x7dc
			 0x4f3a3aff, /// 0x7e0
			 0x73666b1d, /// 0x7e4
			 0x3435c762, /// 0x7e8
			 0xeba4ea63, /// 0x7ec
			 0xdbee05fe, /// 0x7f0
			 0x95126f8d, /// 0x7f4
			 0x30efaf17, /// 0x7f8
			 0x68ab02e3, /// 0x7fc
			 0x63ba89e4, /// 0x800
			 0x1e6077c4, /// 0x804
			 0xeb2e645a, /// 0x808
			 0x6a4eb33e, /// 0x80c
			 0xc7cd1485, /// 0x810
			 0xd5984ccc, /// 0x814
			 0xab9cb208, /// 0x818
			 0x0443d622, /// 0x81c
			 0x1c84d132, /// 0x820
			 0x9530a262, /// 0x824
			 0x694006d2, /// 0x828
			 0x4ce02a18, /// 0x82c
			 0xb58c9a1c, /// 0x830
			 0x22cd5534, /// 0x834
			 0xe356ef8d, /// 0x838
			 0xe1e65d53, /// 0x83c
			 0x68cef0ec, /// 0x840
			 0xae3faa10, /// 0x844
			 0xe9ee0cdf, /// 0x848
			 0x6b4ba19d, /// 0x84c
			 0xc08ede75, /// 0x850
			 0x5092a461, /// 0x854
			 0x064af1af, /// 0x858
			 0xf3a415ca, /// 0x85c
			 0x3b440e29, /// 0x860
			 0xbe3d28a5, /// 0x864
			 0x48aad238, /// 0x868
			 0xe096cb27, /// 0x86c
			 0x9fb5104e, /// 0x870
			 0xd6cf8470, /// 0x874
			 0xf39b97b3, /// 0x878
			 0xef6af15c, /// 0x87c
			 0x2c47a302, /// 0x880
			 0xee9efe78, /// 0x884
			 0x059cc611, /// 0x888
			 0x50b14927, /// 0x88c
			 0xb6251341, /// 0x890
			 0xd5cc1ee4, /// 0x894
			 0x9348f81b, /// 0x898
			 0x8b7b61cc, /// 0x89c
			 0x06dcaf85, /// 0x8a0
			 0x2a4dd771, /// 0x8a4
			 0x82621b80, /// 0x8a8
			 0x40623d62, /// 0x8ac
			 0x143f0fdb, /// 0x8b0
			 0xf91542b2, /// 0x8b4
			 0x26124bbf, /// 0x8b8
			 0xaae0a444, /// 0x8bc
			 0xf1069720, /// 0x8c0
			 0x9423a6ca, /// 0x8c4
			 0xefc1338d, /// 0x8c8
			 0xd2f963b7, /// 0x8cc
			 0x82528dd5, /// 0x8d0
			 0xd3a8b9f1, /// 0x8d4
			 0x5faa87f3, /// 0x8d8
			 0xedcc0f9e, /// 0x8dc
			 0x22bbbfec, /// 0x8e0
			 0x9928e08a, /// 0x8e4
			 0xf7bc1202, /// 0x8e8
			 0x7ab4c307, /// 0x8ec
			 0xbf6e2341, /// 0x8f0
			 0xa55f511b, /// 0x8f4
			 0x6322b4cf, /// 0x8f8
			 0x0fbc9971, /// 0x8fc
			 0xa2271d58, /// 0x900
			 0xf58bfe1f, /// 0x904
			 0xc0f7f635, /// 0x908
			 0x18d94b17, /// 0x90c
			 0x3b06f8a6, /// 0x910
			 0x2f753c53, /// 0x914
			 0x0538eaec, /// 0x918
			 0x179060b3, /// 0x91c
			 0xe3a8f3a0, /// 0x920
			 0x524f5cae, /// 0x924
			 0xc3fe6a0f, /// 0x928
			 0x00fee18a, /// 0x92c
			 0x4fcbd3f4, /// 0x930
			 0x67ae9bed, /// 0x934
			 0xf2a79daa, /// 0x938
			 0x629f3639, /// 0x93c
			 0xd82eea5b, /// 0x940
			 0x32431917, /// 0x944
			 0x1c45c47c, /// 0x948
			 0xc7d13014, /// 0x94c
			 0x69a56a69, /// 0x950
			 0xea9c1729, /// 0x954
			 0xdf984494, /// 0x958
			 0x024e0594, /// 0x95c
			 0x84880849, /// 0x960
			 0xf0c96278, /// 0x964
			 0x1fbc6849, /// 0x968
			 0x843e391d, /// 0x96c
			 0xab688955, /// 0x970
			 0x487d7ff0, /// 0x974
			 0xfb4eb66f, /// 0x978
			 0x02883a45, /// 0x97c
			 0x0302499d, /// 0x980
			 0xfbbbbb85, /// 0x984
			 0x046b6688, /// 0x988
			 0x59ab07a3, /// 0x98c
			 0xf6bd650a, /// 0x990
			 0xf48eab91, /// 0x994
			 0x93995248, /// 0x998
			 0x45bba6c3, /// 0x99c
			 0xdd2649b7, /// 0x9a0
			 0xdc5ed3c5, /// 0x9a4
			 0xca921b07, /// 0x9a8
			 0xf6ba3d28, /// 0x9ac
			 0x4d3e08c6, /// 0x9b0
			 0xcfa86b21, /// 0x9b4
			 0x8ba0eb7b, /// 0x9b8
			 0x5b595ac6, /// 0x9bc
			 0xb4f5d04a, /// 0x9c0
			 0xfdd2e10f, /// 0x9c4
			 0x1d7752e6, /// 0x9c8
			 0xfeeab8d1, /// 0x9cc
			 0xfddf55bf, /// 0x9d0
			 0x0494d103, /// 0x9d4
			 0xa1fbff99, /// 0x9d8
			 0x75e26a15, /// 0x9dc
			 0x9e4dd805, /// 0x9e0
			 0xd06f5d87, /// 0x9e4
			 0x4c995452, /// 0x9e8
			 0x95b52610, /// 0x9ec
			 0xeda8b8eb, /// 0x9f0
			 0xa6e801fe, /// 0x9f4
			 0x7680ca59, /// 0x9f8
			 0xb9a04d9d, /// 0x9fc
			 0xd2b53454, /// 0xa00
			 0x29de276e, /// 0xa04
			 0xa609b664, /// 0xa08
			 0x75c130ea, /// 0xa0c
			 0x1a8a3a01, /// 0xa10
			 0x755d145c, /// 0xa14
			 0x98519a5d, /// 0xa18
			 0x40b899fd, /// 0xa1c
			 0xcba51527, /// 0xa20
			 0x7d9de4d2, /// 0xa24
			 0xb808f91a, /// 0xa28
			 0x2af255a9, /// 0xa2c
			 0xc581f201, /// 0xa30
			 0xe05684c6, /// 0xa34
			 0xb67012d0, /// 0xa38
			 0xe7a15422, /// 0xa3c
			 0x65ec45b5, /// 0xa40
			 0xa024b816, /// 0xa44
			 0x92e31f86, /// 0xa48
			 0x70016fc2, /// 0xa4c
			 0x7251feff, /// 0xa50
			 0x0d52379c, /// 0xa54
			 0x02d36b92, /// 0xa58
			 0x5ea8b44b, /// 0xa5c
			 0x0db0c68b, /// 0xa60
			 0xda2e7a4d, /// 0xa64
			 0x3822b796, /// 0xa68
			 0xca4ffc3e, /// 0xa6c
			 0xd5aa3e76, /// 0xa70
			 0xd701213c, /// 0xa74
			 0xe3a0a436, /// 0xa78
			 0xb035fbb9, /// 0xa7c
			 0x5cf290e6, /// 0xa80
			 0x75bea884, /// 0xa84
			 0xf01b526e, /// 0xa88
			 0x7264aec6, /// 0xa8c
			 0x53ea630c, /// 0xa90
			 0x5c61ba95, /// 0xa94
			 0xa621ceb5, /// 0xa98
			 0x6a83268f, /// 0xa9c
			 0xb1f21574, /// 0xaa0
			 0x032b64a1, /// 0xaa4
			 0x62532dca, /// 0xaa8
			 0xc0fc619e, /// 0xaac
			 0x997afbfd, /// 0xab0
			 0xa6ad4f72, /// 0xab4
			 0x2754e801, /// 0xab8
			 0x5249f09f, /// 0xabc
			 0xee73b8a0, /// 0xac0
			 0x0f4ae42d, /// 0xac4
			 0x565fc4b7, /// 0xac8
			 0x5d2fd2e3, /// 0xacc
			 0x9fef90bf, /// 0xad0
			 0x8d4d6208, /// 0xad4
			 0x71fcfb27, /// 0xad8
			 0xef45ac1c, /// 0xadc
			 0xb9dcba71, /// 0xae0
			 0xf8faf38f, /// 0xae4
			 0x363eaecc, /// 0xae8
			 0xa566388b, /// 0xaec
			 0x5db6ce3f, /// 0xaf0
			 0xe7e71fd1, /// 0xaf4
			 0x04238b99, /// 0xaf8
			 0xa7a011f5, /// 0xafc
			 0x6e040c9e, /// 0xb00
			 0x07fd186a, /// 0xb04
			 0x51bf33e8, /// 0xb08
			 0x43f2372c, /// 0xb0c
			 0x41a0d45b, /// 0xb10
			 0x4556719d, /// 0xb14
			 0xfed211fa, /// 0xb18
			 0xe00d654c, /// 0xb1c
			 0xd65d23dd, /// 0xb20
			 0xfb3d859f, /// 0xb24
			 0x9004c7cb, /// 0xb28
			 0x6502f34e, /// 0xb2c
			 0x3445f9f8, /// 0xb30
			 0x68ef6d3b, /// 0xb34
			 0xb1256a41, /// 0xb38
			 0x1e83a747, /// 0xb3c
			 0x270e25a5, /// 0xb40
			 0x3a1ca11b, /// 0xb44
			 0xd7a46e32, /// 0xb48
			 0xe5ffc79f, /// 0xb4c
			 0xe218d2fd, /// 0xb50
			 0x1c9fe6ae, /// 0xb54
			 0x10f0e377, /// 0xb58
			 0x391b7545, /// 0xb5c
			 0x881db8bc, /// 0xb60
			 0xdd764977, /// 0xb64
			 0x4bf92d96, /// 0xb68
			 0xd02d67fd, /// 0xb6c
			 0xab6a479a, /// 0xb70
			 0x20a40a13, /// 0xb74
			 0xe4230e4d, /// 0xb78
			 0x286c4dc2, /// 0xb7c
			 0x99949cb5, /// 0xb80
			 0x022724b0, /// 0xb84
			 0xfcc31202, /// 0xb88
			 0x0a1c5558, /// 0xb8c
			 0x69c7847f, /// 0xb90
			 0x775f6a3e, /// 0xb94
			 0x3ad87a22, /// 0xb98
			 0x8c63dee3, /// 0xb9c
			 0xb1934c0b, /// 0xba0
			 0x1c0bd1c8, /// 0xba4
			 0x7ea229d2, /// 0xba8
			 0x9f056562, /// 0xbac
			 0x6d7ac056, /// 0xbb0
			 0xd7bd30b5, /// 0xbb4
			 0xb76b90d5, /// 0xbb8
			 0xf3580cf6, /// 0xbbc
			 0xdccfad4c, /// 0xbc0
			 0xf2c98e1d, /// 0xbc4
			 0xff5a19a1, /// 0xbc8
			 0xeba2143c, /// 0xbcc
			 0x5bf02e24, /// 0xbd0
			 0x08d91fb3, /// 0xbd4
			 0xe2138159, /// 0xbd8
			 0x006b9b3c, /// 0xbdc
			 0x9cd769ab, /// 0xbe0
			 0x2de3d7fe, /// 0xbe4
			 0x743d2cd5, /// 0xbe8
			 0x6f3cad4a, /// 0xbec
			 0x55a00bd5, /// 0xbf0
			 0xac42f923, /// 0xbf4
			 0xed31f97d, /// 0xbf8
			 0xe1da48e3, /// 0xbfc
			 0x97e5990b, /// 0xc00
			 0x3c127763, /// 0xc04
			 0x49838efa, /// 0xc08
			 0xd34f00b0, /// 0xc0c
			 0x8f6b74b3, /// 0xc10
			 0x3f4b74c0, /// 0xc14
			 0x5988d8dd, /// 0xc18
			 0xdb408d35, /// 0xc1c
			 0x9977be40, /// 0xc20
			 0x843ceb9d, /// 0xc24
			 0x764fb3ef, /// 0xc28
			 0xa94f6ffa, /// 0xc2c
			 0x2bce98f3, /// 0xc30
			 0x64362580, /// 0xc34
			 0x8539a846, /// 0xc38
			 0x20adfd12, /// 0xc3c
			 0x2f877db1, /// 0xc40
			 0xece9bb2a, /// 0xc44
			 0x1d455447, /// 0xc48
			 0x854931c0, /// 0xc4c
			 0xe5f35d03, /// 0xc50
			 0x67921440, /// 0xc54
			 0x72ff48fd, /// 0xc58
			 0x7187ee1d, /// 0xc5c
			 0x5fd87f8c, /// 0xc60
			 0x27101162, /// 0xc64
			 0xb86e2c5e, /// 0xc68
			 0x491a241a, /// 0xc6c
			 0xb640d744, /// 0xc70
			 0x35e8af0f, /// 0xc74
			 0x9d452297, /// 0xc78
			 0xa28b86e3, /// 0xc7c
			 0xcc88ecad, /// 0xc80
			 0x5988969c, /// 0xc84
			 0xe873e7c0, /// 0xc88
			 0xda1512a8, /// 0xc8c
			 0x3124b43b, /// 0xc90
			 0x236baeb1, /// 0xc94
			 0x21acc09d, /// 0xc98
			 0x9a07fc6c, /// 0xc9c
			 0x2d3715c2, /// 0xca0
			 0xa6a9a29f, /// 0xca4
			 0xc26e6f7d, /// 0xca8
			 0x664ab361, /// 0xcac
			 0x71fd7071, /// 0xcb0
			 0x837e54bf, /// 0xcb4
			 0x28fbc884, /// 0xcb8
			 0x6b75939c, /// 0xcbc
			 0xa18c5501, /// 0xcc0
			 0x2b440298, /// 0xcc4
			 0xebacdda6, /// 0xcc8
			 0xbbf70c21, /// 0xccc
			 0x5eea88c0, /// 0xcd0
			 0x8b47b9f8, /// 0xcd4
			 0x7f2fd99e, /// 0xcd8
			 0x5e5b176d, /// 0xcdc
			 0x3aaeefcd, /// 0xce0
			 0x6b36ad5b, /// 0xce4
			 0xfef400c5, /// 0xce8
			 0x7f124943, /// 0xcec
			 0xa4fc0876, /// 0xcf0
			 0xa79de74a, /// 0xcf4
			 0xe81f9f1a, /// 0xcf8
			 0x83b2ab84, /// 0xcfc
			 0x2c4a5017, /// 0xd00
			 0xf78c9bd1, /// 0xd04
			 0xad6a9f8a, /// 0xd08
			 0x86c2a1b5, /// 0xd0c
			 0xc09d5993, /// 0xd10
			 0xa92b244d, /// 0xd14
			 0xfd101b14, /// 0xd18
			 0x8bab7844, /// 0xd1c
			 0xf609af79, /// 0xd20
			 0x66480da5, /// 0xd24
			 0x70b5de12, /// 0xd28
			 0x83abbe8e, /// 0xd2c
			 0x49698333, /// 0xd30
			 0xa0fcc714, /// 0xd34
			 0xcea6d15f, /// 0xd38
			 0x989c6c10, /// 0xd3c
			 0x5a0f2b07, /// 0xd40
			 0xfcaa28b0, /// 0xd44
			 0x33ecd92f, /// 0xd48
			 0x642bbb09, /// 0xd4c
			 0x75c5348f, /// 0xd50
			 0x2a3f8213, /// 0xd54
			 0x91cdae2a, /// 0xd58
			 0x956e05c4, /// 0xd5c
			 0xca18a322, /// 0xd60
			 0x9bdc9748, /// 0xd64
			 0xf5ed0696, /// 0xd68
			 0x8b129a48, /// 0xd6c
			 0xa0da0590, /// 0xd70
			 0x81bd6061, /// 0xd74
			 0xf9018902, /// 0xd78
			 0x6c42cb45, /// 0xd7c
			 0x1a8f99ed, /// 0xd80
			 0x3c172208, /// 0xd84
			 0xd057952b, /// 0xd88
			 0x8587dcbf, /// 0xd8c
			 0xa9f0bcab, /// 0xd90
			 0xc073511a, /// 0xd94
			 0xaf3e3f11, /// 0xd98
			 0x2d3a794f, /// 0xd9c
			 0x7a9e7e8c, /// 0xda0
			 0x2766ddb1, /// 0xda4
			 0xb9dbb8e5, /// 0xda8
			 0x0bbcafda, /// 0xdac
			 0xc908a547, /// 0xdb0
			 0x453b2094, /// 0xdb4
			 0x16435187, /// 0xdb8
			 0x83321e5f, /// 0xdbc
			 0x38ef67ad, /// 0xdc0
			 0xc64426b3, /// 0xdc4
			 0x6cb43b53, /// 0xdc8
			 0xd75737d9, /// 0xdcc
			 0x0f852ce7, /// 0xdd0
			 0x6130c7b9, /// 0xdd4
			 0x5fc299a6, /// 0xdd8
			 0x6cfd8bc3, /// 0xddc
			 0xb869661a, /// 0xde0
			 0x232c1c66, /// 0xde4
			 0xe2404a0c, /// 0xde8
			 0x6a303dc4, /// 0xdec
			 0xe999b536, /// 0xdf0
			 0x03f6be49, /// 0xdf4
			 0x11e98f4b, /// 0xdf8
			 0x9f10ce8e, /// 0xdfc
			 0x6e0e1445, /// 0xe00
			 0xe3aa9f38, /// 0xe04
			 0xeeeb39c4, /// 0xe08
			 0x751be36e, /// 0xe0c
			 0x1860af3d, /// 0xe10
			 0x089f59c9, /// 0xe14
			 0xee5fb31b, /// 0xe18
			 0xdd6d45da, /// 0xe1c
			 0xf114a6e8, /// 0xe20
			 0x233b1d7e, /// 0xe24
			 0x489c941f, /// 0xe28
			 0x2b8aecf0, /// 0xe2c
			 0xfcd9d85d, /// 0xe30
			 0x03e61eb1, /// 0xe34
			 0x796ddf02, /// 0xe38
			 0x185d56f3, /// 0xe3c
			 0x9885eacb, /// 0xe40
			 0x518a4b49, /// 0xe44
			 0xdaec75fd, /// 0xe48
			 0x4a87d43b, /// 0xe4c
			 0xf6ddf165, /// 0xe50
			 0x45c44090, /// 0xe54
			 0xfec12d9b, /// 0xe58
			 0xecd8a239, /// 0xe5c
			 0xb1a3139d, /// 0xe60
			 0x5e0c64bd, /// 0xe64
			 0xacf65b7e, /// 0xe68
			 0x48838abb, /// 0xe6c
			 0x63dafe4c, /// 0xe70
			 0x73172a42, /// 0xe74
			 0x53124e7b, /// 0xe78
			 0xc2f4c10e, /// 0xe7c
			 0xa05541b5, /// 0xe80
			 0xcf53c955, /// 0xe84
			 0xd894fc46, /// 0xe88
			 0x2725cea4, /// 0xe8c
			 0x1c67655e, /// 0xe90
			 0x8a5120f7, /// 0xe94
			 0x0c889516, /// 0xe98
			 0x77b3f661, /// 0xe9c
			 0x6233c8ea, /// 0xea0
			 0x291b2cf9, /// 0xea4
			 0xe48322d6, /// 0xea8
			 0x342e9cb6, /// 0xeac
			 0x33d900e0, /// 0xeb0
			 0x36b3fd45, /// 0xeb4
			 0x50ae346b, /// 0xeb8
			 0xd95558cb, /// 0xebc
			 0x44a8ad75, /// 0xec0
			 0x96d17962, /// 0xec4
			 0x4fd6fb55, /// 0xec8
			 0xe2f7e541, /// 0xecc
			 0x15e3ce03, /// 0xed0
			 0x2bc37937, /// 0xed4
			 0xeadd41a3, /// 0xed8
			 0x36d1172c, /// 0xedc
			 0xf91ca977, /// 0xee0
			 0xbe6043db, /// 0xee4
			 0xf3ed025f, /// 0xee8
			 0x9fbadfd6, /// 0xeec
			 0x35695c49, /// 0xef0
			 0x75d76c0d, /// 0xef4
			 0xbe7daf4e, /// 0xef8
			 0xf21da09d, /// 0xefc
			 0x339c3959, /// 0xf00
			 0xe8077551, /// 0xf04
			 0x43a7e123, /// 0xf08
			 0x528322b9, /// 0xf0c
			 0xede78ec1, /// 0xf10
			 0x13471470, /// 0xf14
			 0x2c4b3483, /// 0xf18
			 0xd01a96f4, /// 0xf1c
			 0xc8efa074, /// 0xf20
			 0xd3c32db4, /// 0xf24
			 0x06fa4291, /// 0xf28
			 0xc4faa5cd, /// 0xf2c
			 0x64e50212, /// 0xf30
			 0x963a5a33, /// 0xf34
			 0x755745d3, /// 0xf38
			 0x5135e8d2, /// 0xf3c
			 0xcf2f4ac2, /// 0xf40
			 0xe1d51e67, /// 0xf44
			 0x1d169261, /// 0xf48
			 0xfc1f8765, /// 0xf4c
			 0x9bc77551, /// 0xf50
			 0x6fbb3fff, /// 0xf54
			 0x5e13eb4b, /// 0xf58
			 0xc6ab9c7e, /// 0xf5c
			 0x849d977e, /// 0xf60
			 0x1e15738f, /// 0xf64
			 0x99f26106, /// 0xf68
			 0x01c82b7b, /// 0xf6c
			 0x9a423d3d, /// 0xf70
			 0x2008b271, /// 0xf74
			 0xb365d7cd, /// 0xf78
			 0x58cfd6bb, /// 0xf7c
			 0xa64d32cf, /// 0xf80
			 0x8e513d97, /// 0xf84
			 0xa7c0711d, /// 0xf88
			 0x5b70fb3a, /// 0xf8c
			 0x1be0b371, /// 0xf90
			 0x5c483fef, /// 0xf94
			 0x825993c8, /// 0xf98
			 0xaff4d06c, /// 0xf9c
			 0xdae36646, /// 0xfa0
			 0x6e5bb0b3, /// 0xfa4
			 0x016e6261, /// 0xfa8
			 0x15737f1c, /// 0xfac
			 0x1be2ee94, /// 0xfb0
			 0x5f0b891b, /// 0xfb4
			 0x967f32e0, /// 0xfb8
			 0x9718267b, /// 0xfbc
			 0x82a4ca1b, /// 0xfc0
			 0x9217f5b7, /// 0xfc4
			 0x0b5f6561, /// 0xfc8
			 0xe0d1d5cf, /// 0xfcc
			 0x3ca2fb99, /// 0xfd0
			 0x75199354, /// 0xfd4
			 0x1b1fade3, /// 0xfd8
			 0xb1d0f303, /// 0xfdc
			 0x0def596c, /// 0xfe0
			 0xcbe39bae, /// 0xfe4
			 0xb645bb4f, /// 0xfe8
			 0xe84135f1, /// 0xfec
			 0xaebcfc38, /// 0xff0
			 0x6458cde5, /// 0xff4
			 0x161bb2cc, /// 0xff8
			 0x581e00d4 /// last
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
			 0x00000020,
			 0x00000194,
			 0x000002c0,
			 0x000007a0,
			 0x0000072c,
			 0x00000260,
			 0x00000048,
			 0x000005b0,

			 /// vpu register f2
			 0x41300000,
			 0x41400000,
			 0x41d80000,
			 0x41b80000,
			 0x41f80000,
			 0x40a00000,
			 0x41f00000,
			 0x41400000,

			 /// vpu register f3
			 0x41a00000,
			 0x41c00000,
			 0x40a00000,
			 0x40c00000,
			 0x41300000,
			 0x41800000,
			 0x41d00000,
			 0x41000000,

			 /// vpu register f4
			 0x41880000,
			 0x40400000,
			 0x41980000,
			 0x40800000,
			 0x41900000,
			 0x41900000,
			 0x41a80000,
			 0x40800000,

			 /// vpu register f5
			 0x41800000,
			 0x41800000,
			 0x41a00000,
			 0x41800000,
			 0x41500000,
			 0x40c00000,
			 0x41f00000,
			 0x41700000,

			 /// vpu register f6
			 0x41600000,
			 0x40400000,
			 0x42000000,
			 0x41c00000,
			 0x41e80000,
			 0x40400000,
			 0x41a80000,
			 0x41300000,

			 /// vpu register f7
			 0x41800000,
			 0x41e00000,
			 0x41980000,
			 0x41f00000,
			 0x40000000,
			 0x41700000,
			 0x41100000,
			 0x41400000,

			 /// vpu register f8
			 0x41200000,
			 0x41a00000,
			 0x40c00000,
			 0x40a00000,
			 0x41a80000,
			 0x40800000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f9
			 0x41d00000,
			 0x41980000,
			 0x41a80000,
			 0x40c00000,
			 0x41a00000,
			 0x41300000,
			 0x41a00000,
			 0x40000000,

			 /// vpu register f10
			 0x41880000,
			 0x41000000,
			 0x3f800000,
			 0x41a80000,
			 0x40800000,
			 0x41300000,
			 0x41000000,
			 0x41d00000,

			 /// vpu register f11
			 0x41400000,
			 0x41f00000,
			 0x41e80000,
			 0x41a00000,
			 0x41e80000,
			 0x42000000,
			 0x41d80000,
			 0x41d00000,

			 /// vpu register f12
			 0x40000000,
			 0x3f800000,
			 0x41500000,
			 0x40e00000,
			 0x41500000,
			 0x40000000,
			 0x41d00000,
			 0x41b00000,

			 /// vpu register f13
			 0x41f00000,
			 0x41880000,
			 0x41800000,
			 0x41a80000,
			 0x41900000,
			 0x3f800000,
			 0x41a80000,
			 0x41500000,

			 /// vpu register f14
			 0x41200000,
			 0x41100000,
			 0x40e00000,
			 0x41a00000,
			 0x41a00000,
			 0x41e00000,
			 0x40c00000,
			 0x41a80000,

			 /// vpu register f15
			 0x40e00000,
			 0x41900000,
			 0x41700000,
			 0x41a80000,
			 0x41300000,
			 0x41200000,
			 0x41b80000,
			 0x41f00000,

			 /// vpu register f16
			 0x41b80000,
			 0x41c80000,
			 0x41600000,
			 0x42000000,
			 0x40400000,
			 0x41a00000,
			 0x40000000,
			 0x41000000,

			 /// vpu register f17
			 0x41b00000,
			 0x41800000,
			 0x41a80000,
			 0x40c00000,
			 0x41600000,
			 0x41600000,
			 0x41800000,
			 0x41d00000,

			 /// vpu register f18
			 0x41300000,
			 0x41200000,
			 0x42000000,
			 0x40000000,
			 0x41300000,
			 0x41b80000,
			 0x41400000,
			 0x41e00000,

			 /// vpu register f19
			 0x41900000,
			 0x41900000,
			 0x41700000,
			 0x41000000,
			 0x41e80000,
			 0x41a00000,
			 0x41c00000,
			 0x40a00000,

			 /// vpu register f20
			 0x41980000,
			 0x41300000,
			 0x41c00000,
			 0x41c00000,
			 0x41a00000,
			 0x41100000,
			 0x41f00000,
			 0x41900000,

			 /// vpu register f21
			 0x40c00000,
			 0x42000000,
			 0x42000000,
			 0x3f800000,
			 0x3f800000,
			 0x41e00000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f22
			 0x42000000,
			 0x3f800000,
			 0x40e00000,
			 0x41b80000,
			 0x41980000,
			 0x41a80000,
			 0x3f800000,
			 0x41b80000,

			 /// vpu register f23
			 0x41700000,
			 0x41c00000,
			 0x41a00000,
			 0x41a80000,
			 0x40a00000,
			 0x41e80000,
			 0x41b00000,
			 0x40000000,

			 /// vpu register f24
			 0x41300000,
			 0x41400000,
			 0x40800000,
			 0x41b80000,
			 0x40800000,
			 0x41e80000,
			 0x41600000,
			 0x41a00000,

			 /// vpu register f25
			 0x40a00000,
			 0x41400000,
			 0x41100000,
			 0x40400000,
			 0x41c00000,
			 0x41a80000,
			 0x40400000,
			 0x41500000,

			 /// vpu register f26
			 0x41300000,
			 0x41d80000,
			 0x41c80000,
			 0x41880000,
			 0x41a00000,
			 0x41100000,
			 0x41a00000,
			 0x41200000,

			 /// vpu register f27
			 0x41f00000,
			 0x41500000,
			 0x41d80000,
			 0x41d00000,
			 0x41f00000,
			 0x41b80000,
			 0x41a00000,
			 0x41f80000,

			 /// vpu register f28
			 0x40a00000,
			 0x41b80000,
			 0x41d80000,
			 0x40800000,
			 0x41c00000,
			 0x41900000,
			 0x40000000,
			 0x41500000,

			 /// vpu register f29
			 0x3f800000,
			 0x41200000,
			 0x41a80000,
			 0x40800000,
			 0x41d00000,
			 0x41300000,
			 0x40800000,
			 0x40800000,

			 /// vpu register f30
			 0x41d80000,
			 0x41600000,
			 0x41c80000,
			 0x41900000,
			 0x42000000,
			 0x41b00000,
			 0x41b00000,
			 0x40800000,

			 /// vpu register f31
			 0x41e80000,
			 0x41e00000,
			 0x41c80000,
			 0x41400000,
			 0x41900000,
			 0x41d80000,
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
		"fle.ps f30, f15, f0\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f4, f24, f25\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f20, f0, f23\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f8, f29, f10\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f30, f29, f9\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f24, f8\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f21, f19, f0\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f28, f27, f2\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f28, f3, f22\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f9, f19\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f13, f17, f25\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f21, f24, f25\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f14, f27, f6\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f23, f17\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f27, f11, f16\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f28, f27, f29\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f3, f24, f29\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f6, f22, f10\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f2, f29\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f24, f16\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f27, f26, f0\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f3, f27, f22\n"                               ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f3, f5, f0\n"                                 ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f13, f5, f13\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f9, f24\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f11, f1, f11\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f17, f22, f11\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f21, f2, f21\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f18, f30, f17\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f7, f15, f30\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f29, f28\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f0, f20\n"                               ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f9, f1, f4\n"                                 ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f29, f24\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f16, f14\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f22, f16, f20\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f22, f24, f24\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f4, f25, f26\n"                               ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f14, f6, f6\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f9, f25, f16\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f4, f19, f13\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f22, f28, f24\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f13, f0, f26\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f28, f17\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f21, f5, f29\n"                               ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f30, f3, f25\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f19, f9, f14\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f19, f2\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f28, f17, f8\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f25, f29, f23\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f11, f10, f13\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f26, f21, f25\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f4, f8, f8\n"                                 ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f28, f25\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f15, f9, f12\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f15, f29, f11\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f11, f7, f26\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f25, f11, f17\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f2, f4, f27\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f7, f24\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f24, f19, f27\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f19, f12, f16\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f6, f15\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f22, f10, f6\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f17, f25, f20\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f8, f5, f25\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f23, f9, f30\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f7, f24, f0\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f17, f28, f9\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f29, f9\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f28, f1, f29\n"                               ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f29, f30, f2\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f27, f17, f3\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f26, f27\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f22, f30, f27\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f14, f10\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f27, f10, f5\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f4, f12\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f15, f12, f26\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f20, f3\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f28, f7, f2\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f2, f2, f4\n"                                 ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f21, f9, f23\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f18, f10, f18\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f14, f8, f24\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f9, f25, f26\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f12, f24, f4\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f20, f11, f11\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f21, f23, f6\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f11, f17, f10\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f9, f6, f10\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f2, f4, f22\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f10, f27, f13\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f25, f2, f21\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f26, f1, f1\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f13, f14, f15\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f16, f18, f24\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f15, f13, f18\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fle.ps f18, f10, f13\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
