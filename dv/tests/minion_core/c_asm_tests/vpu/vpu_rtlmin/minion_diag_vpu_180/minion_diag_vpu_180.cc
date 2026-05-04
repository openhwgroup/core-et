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
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7fc00001,                                                  // signaling NaN                               /// 00004
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00008
			 0xff800000,                                                  // -inf                                        /// 0000c
			 0xcb000000,                                                  // -8388608.0                                  /// 00010
			 0x0c600000,                                                  // Leading 1s:                                 /// 00014
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0001c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00030
			 0xffc00001,                                                  // -signaling NaN                              /// 00034
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x00011111,                                                  // 9.7958E-41                                  /// 0003c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00040
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c400000,                                                  // Leading 1s:                                 /// 0004c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00050
			 0x0c700000,                                                  // Leading 1s:                                 /// 00054
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00060
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00068
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0006c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00070
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00074
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00078
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0007c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00084
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00088
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x55555555,                                                  // 4 random values                             /// 00094
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00098
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 000d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000d4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0xff800000,                                                  // -inf                                        /// 000e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e000007,                                                  // Trailing 1s:                                /// 00104
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0010c
			 0xff800000,                                                  // -inf                                        /// 00110
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00114
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0011c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00124
			 0x0e000003,                                                  // Trailing 1s:                                /// 00128
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xffc00001,                                                  // -signaling NaN                              /// 00134
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00138
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0013c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00144
			 0x0c400000,                                                  // Leading 1s:                                 /// 00148
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0014c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x00000000,                                                  // zero                                        /// 00154
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00170
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00174
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00178
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00180
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00184
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00190
			 0xff800000,                                                  // -inf                                        /// 00194
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00198
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001a8
			 0x00000000,                                                  // zero                                        /// 001ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001b4
			 0x00000000,                                                  // zero                                        /// 001b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001c4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0xff800000,                                                  // -inf                                        /// 001dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00208
			 0x0c400000,                                                  // Leading 1s:                                 /// 0020c
			 0x7fc00001,                                                  // signaling NaN                               /// 00210
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00214
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00218
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00220
			 0x0e000007,                                                  // Trailing 1s:                                /// 00224
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00228
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00230
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00234
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00240
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00244
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00248
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00250
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x00011111,                                                  // 9.7958E-41                                  /// 00258
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00260
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00264
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x80011111,                                                  // -9.7958E-41                                 /// 0026c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00270
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00274
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0027c
			 0x55555555,                                                  // 4 random values                             /// 00280
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00288
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00290
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0029c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002c0
			 0x7fc00001,                                                  // signaling NaN                               /// 002c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00300
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00304
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00308
			 0x80011111,                                                  // -9.7958E-41                                 /// 0030c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00310
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00318
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0031c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x55555555,                                                  // 4 random values                             /// 00324
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00328
			 0x00000000,                                                  // zero                                        /// 0032c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00330
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00334
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00340
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00344
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00348
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xffc00001,                                                  // -signaling NaN                              /// 0035c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00360
			 0x80011111,                                                  // -9.7958E-41                                 /// 00364
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00368
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0036c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00378
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0037c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00380
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00384
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00388
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0038c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00390
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00398
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a0
			 0x7fc00001,                                                  // signaling NaN                               /// 003a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 003b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003cc
			 0xffc00001,                                                  // -signaling NaN                              /// 003d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003ec
			 0x7fc00001,                                                  // signaling NaN                               /// 003f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c600000,                                                  // Leading 1s:                                 /// 00404
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00408
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0040c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00410
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00414
			 0x0e000007,                                                  // Trailing 1s:                                /// 00418
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0041c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00420
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00424
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00428
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0042c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00430
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00434
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0xbf028f5c,                                                  // -0.51                                       /// 00448
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0044c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00450
			 0x7fc00001,                                                  // signaling NaN                               /// 00454
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00460
			 0xff800000,                                                  // -inf                                        /// 00464
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0046c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00470
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00474
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00480
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00488
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00490
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00494
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00498
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0049c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x7fc00001,                                                  // signaling NaN                               /// 004a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b0
			 0x33333333,                                                  // 4 random values                             /// 004b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00500
			 0x80000000,                                                  // -zero                                       /// 00504
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00508
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00510
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00514
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x7fc00001,                                                  // signaling NaN                               /// 00520
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00524
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0052c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00530
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00534
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00538
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00540
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00548
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0054c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00550
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00554
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00560
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00564
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00568
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0056c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00570
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00574
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00578
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0057c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00580
			 0x55555555,                                                  // 4 random values                             /// 00584
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00588
			 0x00011111,                                                  // 9.7958E-41                                  /// 0058c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00590
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00594
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00598
			 0xcb000000,                                                  // -8388608.0                                  /// 0059c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x55555555,                                                  // 4 random values                             /// 005d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00600
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00604
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00614
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00618
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0061c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00620
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00624
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00628
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00630
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00650
			 0x3f028f5c,                                                  // 0.51                                        /// 00654
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0065c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00660
			 0x55555555,                                                  // 4 random values                             /// 00664
			 0x33333333,                                                  // 4 random values                             /// 00668
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0066c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00670
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00674
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00678
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0067c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00680
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00684
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0xff800000,                                                  // -inf                                        /// 0068c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00690
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00694
			 0xcb000000,                                                  // -8388608.0                                  /// 00698
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 006e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00704
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000000,                                                  // zero                                        /// 00710
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00718
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0071c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00720
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0072c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00734
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0073c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00740
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00748
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0074c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00758
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0075c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x7fc00001,                                                  // signaling NaN                               /// 00764
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00768
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00774
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00778
			 0x7fc00001,                                                  // signaling NaN                               /// 0077c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00780
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00784
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00794
			 0x0e000007,                                                  // Trailing 1s:                                /// 00798
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0079c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007a0
			 0x3f028f5c,                                                  // 0.51                                        /// 007a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007cc
			 0xff800000,                                                  // -inf                                        /// 007d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007dc
			 0x3f028f5c,                                                  // 0.51                                        /// 007e0
			 0xff800000,                                                  // -inf                                        /// 007e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007f0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x7f800000,                                                  // inf                                         /// 00804
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00808
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00810
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00814
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00818
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0081c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00828
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0082c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00830
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00834
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0083c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00840
			 0x4b000000,                                                  // 8388608.0                                   /// 00844
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0084c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00858
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0085c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00870
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00874
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00878
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0087c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00880
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00888
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0088c
			 0x7fc00001,                                                  // signaling NaN                               /// 00890
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00900
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00904
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00910
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00928
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0092c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00934
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00938
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0093c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00948
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00950
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00954
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0095c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00960
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00968
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0096c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00970
			 0x55555555,                                                  // 4 random values                             /// 00974
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00978
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0097c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00984
			 0x7f800000,                                                  // inf                                         /// 00988
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0098c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00990
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00998
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0099c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x7f800000,                                                  // inf                                         /// 009ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009d0
			 0x80000000,                                                  // -zero                                       /// 009d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00a00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xcb000000,                                                  // -8388608.0                                  /// 00a08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a18
			 0x7fc00001,                                                  // signaling NaN                               /// 00a1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a2c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a38
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a50
			 0xbf028f5c,                                                  // -0.51                                       /// 00a54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a64
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a70
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a7c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a8c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00aac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ab4
			 0xff800000,                                                  // -inf                                        /// 00ab8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ac0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ac4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ac8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00acc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ad8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ae4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00af8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00afc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b10
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b2c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b3c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b40
			 0x3f028f5c,                                                  // 0.51                                        /// 00b44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b58
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b68
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b70
			 0xbf028f5c,                                                  // -0.51                                       /// 00b74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b94
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00ba0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00be0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00be8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bf8
			 0x80000000,                                                  // -zero                                       /// 00bfc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c00
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c30
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c34
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0xff800000,                                                  // -inf                                        /// 00c50
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c74
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cc0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cf0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d0c
			 0x80000000,                                                  // -zero                                       /// 00d10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d14
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d44
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d58
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d64
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x80000000,                                                  // -zero                                       /// 00d6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d78
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00da0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00da4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00db8
			 0xffc00001,                                                  // -signaling NaN                              /// 00dbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x33333333,                                                  // 4 random values                             /// 00dd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00de4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00dec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00df0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00df8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e28
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e48
			 0xcb000000,                                                  // -8388608.0                                  /// 00e4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e78
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e8c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ea8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80000000,                                                  // -zero                                       /// 00eb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ebc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ec4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ecc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ed0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ed8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ee4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ef0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ef8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00efc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x33333333,                                                  // 4 random values                             /// 00f18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f30
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x3f028f5c,                                                  // 0.51                                        /// 00f44
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f84
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fa4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fd0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fd4
			 0x80000000,                                                  // -zero                                       /// 00fd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fdc
			 0x33333333,                                                  // 4 random values                             /// 00fe0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ff8
			 0x3f800001 // 1  + 1ulp                                     // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xd5fcecd7, /// 0x0
			 0x64b1b0b1, /// 0x4
			 0x4dff3781, /// 0x8
			 0x2e576d11, /// 0xc
			 0x3308055b, /// 0x10
			 0x23889683, /// 0x14
			 0x062ef24b, /// 0x18
			 0xc3902077, /// 0x1c
			 0x2cca43b9, /// 0x20
			 0x6037b307, /// 0x24
			 0xe7f90d21, /// 0x28
			 0x71fd1811, /// 0x2c
			 0x919dc166, /// 0x30
			 0x37579b98, /// 0x34
			 0x20b0c3f5, /// 0x38
			 0x0b2775d7, /// 0x3c
			 0x683b651b, /// 0x40
			 0x43991166, /// 0x44
			 0xa76cd9ed, /// 0x48
			 0x615e5547, /// 0x4c
			 0xd0fcb4af, /// 0x50
			 0x42c14afa, /// 0x54
			 0xddbc42d1, /// 0x58
			 0x00ab7f82, /// 0x5c
			 0x6e6fb8c8, /// 0x60
			 0xb5adde0f, /// 0x64
			 0x0c762a39, /// 0x68
			 0xbd8da49d, /// 0x6c
			 0xa9ed2768, /// 0x70
			 0xf0b91355, /// 0x74
			 0x5f6295c4, /// 0x78
			 0x78bed1e7, /// 0x7c
			 0x443a0f0c, /// 0x80
			 0x9e13d622, /// 0x84
			 0x5f1916f6, /// 0x88
			 0xee8cfa6d, /// 0x8c
			 0x42955167, /// 0x90
			 0xd3142530, /// 0x94
			 0xb1470691, /// 0x98
			 0x3c1cfa86, /// 0x9c
			 0x36a091b2, /// 0xa0
			 0xbf2b26b3, /// 0xa4
			 0xa1d11be5, /// 0xa8
			 0xa28508b4, /// 0xac
			 0x9f7d3ace, /// 0xb0
			 0xf8db7af9, /// 0xb4
			 0xfc645696, /// 0xb8
			 0x5908be50, /// 0xbc
			 0xab72078a, /// 0xc0
			 0x76e4641b, /// 0xc4
			 0x08dc0bf6, /// 0xc8
			 0x65238425, /// 0xcc
			 0xc3fcbd36, /// 0xd0
			 0x4a027716, /// 0xd4
			 0xef059f26, /// 0xd8
			 0xa0fc156b, /// 0xdc
			 0x57c333cc, /// 0xe0
			 0xe17b5791, /// 0xe4
			 0x78222cc6, /// 0xe8
			 0x57d18c7c, /// 0xec
			 0x35304563, /// 0xf0
			 0xe226b6c0, /// 0xf4
			 0x3d1bacc4, /// 0xf8
			 0xe66a9950, /// 0xfc
			 0xbc106eaa, /// 0x100
			 0x47cd7bb5, /// 0x104
			 0x1a74e3b0, /// 0x108
			 0xeb67aa23, /// 0x10c
			 0xec4aec5b, /// 0x110
			 0x6c3496ae, /// 0x114
			 0x1d14b462, /// 0x118
			 0xf1e82573, /// 0x11c
			 0xdb0fd042, /// 0x120
			 0x4b14e819, /// 0x124
			 0xcbb62a71, /// 0x128
			 0x40501494, /// 0x12c
			 0xb2ff1f06, /// 0x130
			 0x55554dbc, /// 0x134
			 0x977a16cc, /// 0x138
			 0xad8c9072, /// 0x13c
			 0x5343cba3, /// 0x140
			 0xc942f9f7, /// 0x144
			 0xcca966a5, /// 0x148
			 0x7a602135, /// 0x14c
			 0xa452125f, /// 0x150
			 0xbf07626b, /// 0x154
			 0xafd07134, /// 0x158
			 0x67fa9343, /// 0x15c
			 0x6a94241c, /// 0x160
			 0xa8564b78, /// 0x164
			 0x2b28f6ed, /// 0x168
			 0x0c9e4987, /// 0x16c
			 0xffbaf03d, /// 0x170
			 0x6a0fa00d, /// 0x174
			 0x9eff5f6e, /// 0x178
			 0x1efa4034, /// 0x17c
			 0xe084880e, /// 0x180
			 0xeed40d3d, /// 0x184
			 0xd0505b8a, /// 0x188
			 0x4c35f035, /// 0x18c
			 0xb4e69797, /// 0x190
			 0x97f27354, /// 0x194
			 0x5a575241, /// 0x198
			 0x6e151041, /// 0x19c
			 0x503ed0ad, /// 0x1a0
			 0x7a7cf48d, /// 0x1a4
			 0xf1b599cb, /// 0x1a8
			 0xd8a5f01e, /// 0x1ac
			 0xd61b5c51, /// 0x1b0
			 0x97d86ad5, /// 0x1b4
			 0x36b5cdf7, /// 0x1b8
			 0x8eb5f117, /// 0x1bc
			 0x2abf6f8e, /// 0x1c0
			 0xeb0daf10, /// 0x1c4
			 0x4cb2e225, /// 0x1c8
			 0x5569d85b, /// 0x1cc
			 0xea16fa13, /// 0x1d0
			 0xab3592b6, /// 0x1d4
			 0x9f9deb14, /// 0x1d8
			 0x698d3bc4, /// 0x1dc
			 0xa4be9a0f, /// 0x1e0
			 0xf75f04ae, /// 0x1e4
			 0x1aafc04d, /// 0x1e8
			 0xe2d93abf, /// 0x1ec
			 0xa82fbe15, /// 0x1f0
			 0x1e64786d, /// 0x1f4
			 0xc6e29bd5, /// 0x1f8
			 0x60907ded, /// 0x1fc
			 0x400efcb5, /// 0x200
			 0x6cfe26fc, /// 0x204
			 0x85271183, /// 0x208
			 0x6f9fb668, /// 0x20c
			 0x935ffa0e, /// 0x210
			 0xcdd97eb3, /// 0x214
			 0x9b314369, /// 0x218
			 0x1c263ac0, /// 0x21c
			 0xc3738f95, /// 0x220
			 0x9d67296d, /// 0x224
			 0x7df5478b, /// 0x228
			 0x9a154da0, /// 0x22c
			 0x2a9140c9, /// 0x230
			 0x3253a9da, /// 0x234
			 0x104fe5a7, /// 0x238
			 0xacca6386, /// 0x23c
			 0x7e205b96, /// 0x240
			 0x3fe50d7b, /// 0x244
			 0xcbb7ac7b, /// 0x248
			 0x024bec7a, /// 0x24c
			 0x64d325cf, /// 0x250
			 0x516ed216, /// 0x254
			 0xd717fb85, /// 0x258
			 0x7682086d, /// 0x25c
			 0x28facf3a, /// 0x260
			 0xba473b39, /// 0x264
			 0x3100e756, /// 0x268
			 0x3fabdfdf, /// 0x26c
			 0x3aeeb0a8, /// 0x270
			 0x9eaaa68b, /// 0x274
			 0x04ddde94, /// 0x278
			 0xb93193d9, /// 0x27c
			 0x3942bcf8, /// 0x280
			 0xd12d40a7, /// 0x284
			 0xd7bad7b7, /// 0x288
			 0x4a52e9cf, /// 0x28c
			 0x375392ee, /// 0x290
			 0x1e5f9bf2, /// 0x294
			 0x1234d918, /// 0x298
			 0x7c3fc73b, /// 0x29c
			 0x3733b9f0, /// 0x2a0
			 0x9faaa2e3, /// 0x2a4
			 0xc6f316d6, /// 0x2a8
			 0x1354a206, /// 0x2ac
			 0x3bf9e7d0, /// 0x2b0
			 0xdcdbca04, /// 0x2b4
			 0x8d7e4697, /// 0x2b8
			 0x676ca844, /// 0x2bc
			 0xc883f610, /// 0x2c0
			 0x85ee8b4d, /// 0x2c4
			 0xe584c082, /// 0x2c8
			 0xd87cfc15, /// 0x2cc
			 0xa813a4f1, /// 0x2d0
			 0xedbee869, /// 0x2d4
			 0x2bba63ba, /// 0x2d8
			 0x8bf8b279, /// 0x2dc
			 0xeb45baf3, /// 0x2e0
			 0xb971a180, /// 0x2e4
			 0xd1f863b3, /// 0x2e8
			 0x57161cb3, /// 0x2ec
			 0x6bd10c26, /// 0x2f0
			 0xdd5b196d, /// 0x2f4
			 0xc2c73baf, /// 0x2f8
			 0xe67a2f8e, /// 0x2fc
			 0x752a95c0, /// 0x300
			 0x49017aca, /// 0x304
			 0xebf621c3, /// 0x308
			 0xd3f24e25, /// 0x30c
			 0xece4a15b, /// 0x310
			 0x70a983b5, /// 0x314
			 0x57def039, /// 0x318
			 0x14d9ea75, /// 0x31c
			 0x9413efb5, /// 0x320
			 0x215de12e, /// 0x324
			 0x5c694c8a, /// 0x328
			 0xdeb49bd2, /// 0x32c
			 0x3e0d28ed, /// 0x330
			 0xb716e068, /// 0x334
			 0x528e6317, /// 0x338
			 0x067da2c9, /// 0x33c
			 0x9faa8716, /// 0x340
			 0xb1038580, /// 0x344
			 0x4e35c96e, /// 0x348
			 0xb886715f, /// 0x34c
			 0x0763e25e, /// 0x350
			 0xadc4a07e, /// 0x354
			 0x803165c4, /// 0x358
			 0xfbf0a43a, /// 0x35c
			 0xf56a562a, /// 0x360
			 0x966c4f62, /// 0x364
			 0xb1230c8b, /// 0x368
			 0x8ff15d70, /// 0x36c
			 0xa004a27a, /// 0x370
			 0xe34899b9, /// 0x374
			 0xc9578391, /// 0x378
			 0x4ca7cf65, /// 0x37c
			 0x6ac63bcd, /// 0x380
			 0xe330bba4, /// 0x384
			 0xc4f0bd91, /// 0x388
			 0xdacfb7f5, /// 0x38c
			 0x47aadf13, /// 0x390
			 0x7a6892b8, /// 0x394
			 0x6b692f88, /// 0x398
			 0x205834a0, /// 0x39c
			 0xf127a90d, /// 0x3a0
			 0xa0b33cb5, /// 0x3a4
			 0x705125f2, /// 0x3a8
			 0x0b9ffdca, /// 0x3ac
			 0x2ac81fb8, /// 0x3b0
			 0x2647a788, /// 0x3b4
			 0xf73c4894, /// 0x3b8
			 0xff73675d, /// 0x3bc
			 0xecc98921, /// 0x3c0
			 0xae2facd9, /// 0x3c4
			 0xcfc8d950, /// 0x3c8
			 0xfa1bd57a, /// 0x3cc
			 0xf99a6610, /// 0x3d0
			 0xf34612fa, /// 0x3d4
			 0x3bd5474c, /// 0x3d8
			 0x2e330c3e, /// 0x3dc
			 0x527f8ebc, /// 0x3e0
			 0x15f4b459, /// 0x3e4
			 0xc23f9f86, /// 0x3e8
			 0xec249921, /// 0x3ec
			 0xdb7d061d, /// 0x3f0
			 0x2e28845c, /// 0x3f4
			 0x6161cf28, /// 0x3f8
			 0xce365660, /// 0x3fc
			 0x296ab0f9, /// 0x400
			 0x8c68dd0a, /// 0x404
			 0x835639c7, /// 0x408
			 0x738256e3, /// 0x40c
			 0x24edd181, /// 0x410
			 0x0721946c, /// 0x414
			 0x776209db, /// 0x418
			 0x1cf4d7b5, /// 0x41c
			 0x233a8f2c, /// 0x420
			 0xda6b678a, /// 0x424
			 0xef0a0856, /// 0x428
			 0xdda9d039, /// 0x42c
			 0xb64baead, /// 0x430
			 0x77478c70, /// 0x434
			 0x322b40ea, /// 0x438
			 0xc744ab52, /// 0x43c
			 0xe0815c30, /// 0x440
			 0xccc6932c, /// 0x444
			 0x5f6e3db5, /// 0x448
			 0x94085a31, /// 0x44c
			 0x677fb7b1, /// 0x450
			 0xf9be331e, /// 0x454
			 0xdbe3d131, /// 0x458
			 0xc8d515cd, /// 0x45c
			 0x96af5454, /// 0x460
			 0xcabab357, /// 0x464
			 0x29af4610, /// 0x468
			 0xe2293ea8, /// 0x46c
			 0x8f367d5a, /// 0x470
			 0x60481734, /// 0x474
			 0x692e3f8e, /// 0x478
			 0x97473143, /// 0x47c
			 0xf8cb66ee, /// 0x480
			 0x72b0521d, /// 0x484
			 0x098236cb, /// 0x488
			 0x394fac90, /// 0x48c
			 0x1a33e3ab, /// 0x490
			 0x5775a816, /// 0x494
			 0xdc137531, /// 0x498
			 0x24e64df0, /// 0x49c
			 0x26bfc84c, /// 0x4a0
			 0xf839645b, /// 0x4a4
			 0xf55ee55d, /// 0x4a8
			 0x540ed8c5, /// 0x4ac
			 0x00e07434, /// 0x4b0
			 0x1e8e5f02, /// 0x4b4
			 0x85ff6476, /// 0x4b8
			 0x91f02173, /// 0x4bc
			 0x28b19bda, /// 0x4c0
			 0xe3325af0, /// 0x4c4
			 0xbe03fc84, /// 0x4c8
			 0xe83c4f40, /// 0x4cc
			 0x2776c1bc, /// 0x4d0
			 0x83937a6c, /// 0x4d4
			 0xc90589ea, /// 0x4d8
			 0xfc7f7520, /// 0x4dc
			 0x029ada16, /// 0x4e0
			 0xb0a019fc, /// 0x4e4
			 0x045216e4, /// 0x4e8
			 0x7611fdb9, /// 0x4ec
			 0x12852b7c, /// 0x4f0
			 0x6e2425e8, /// 0x4f4
			 0xe773ebe4, /// 0x4f8
			 0x92b22d0b, /// 0x4fc
			 0x9a3df8b6, /// 0x500
			 0xd54ef858, /// 0x504
			 0x32164e55, /// 0x508
			 0x95c614e9, /// 0x50c
			 0x02c8100d, /// 0x510
			 0x3d921788, /// 0x514
			 0x4a392650, /// 0x518
			 0x63c40f30, /// 0x51c
			 0x5c96c1a9, /// 0x520
			 0xead0f034, /// 0x524
			 0x8b736509, /// 0x528
			 0x0c959bbd, /// 0x52c
			 0xb57d2cbb, /// 0x530
			 0xdb183c02, /// 0x534
			 0xd3f73f95, /// 0x538
			 0x9a2f6172, /// 0x53c
			 0x2a2bf2f5, /// 0x540
			 0xbccc3d6b, /// 0x544
			 0x3a60af5d, /// 0x548
			 0x67e0d8e3, /// 0x54c
			 0x5c46a99d, /// 0x550
			 0x0c0a91c2, /// 0x554
			 0x79fd561e, /// 0x558
			 0x2f72e400, /// 0x55c
			 0xa67948b1, /// 0x560
			 0x8d43d5d3, /// 0x564
			 0x71d388ae, /// 0x568
			 0x7ce1e39c, /// 0x56c
			 0x5bd9a52d, /// 0x570
			 0x81f4248b, /// 0x574
			 0xa1870fde, /// 0x578
			 0x4b08c60d, /// 0x57c
			 0x503ce93d, /// 0x580
			 0x988a34fd, /// 0x584
			 0x2ea606d4, /// 0x588
			 0x36bc318f, /// 0x58c
			 0xd88df596, /// 0x590
			 0xeb0982f5, /// 0x594
			 0x1c965710, /// 0x598
			 0xbcb17875, /// 0x59c
			 0x5d6e8e4f, /// 0x5a0
			 0x0ddd9d52, /// 0x5a4
			 0xec53281e, /// 0x5a8
			 0x3aa4f520, /// 0x5ac
			 0xa36485f9, /// 0x5b0
			 0x18b88722, /// 0x5b4
			 0x67fec6a3, /// 0x5b8
			 0x46b134d4, /// 0x5bc
			 0x105afa18, /// 0x5c0
			 0x800ac094, /// 0x5c4
			 0x7c4a375c, /// 0x5c8
			 0xcf498ca2, /// 0x5cc
			 0xe1b19b3c, /// 0x5d0
			 0x3408b660, /// 0x5d4
			 0x8680f37f, /// 0x5d8
			 0xf49eaef8, /// 0x5dc
			 0xf0657bf9, /// 0x5e0
			 0x03c0e3a0, /// 0x5e4
			 0x91d82624, /// 0x5e8
			 0x766e4dfd, /// 0x5ec
			 0xf4515fae, /// 0x5f0
			 0xc706be23, /// 0x5f4
			 0xf14cd653, /// 0x5f8
			 0x48c082bf, /// 0x5fc
			 0xbd257237, /// 0x600
			 0xb2759c1c, /// 0x604
			 0xfc4cb111, /// 0x608
			 0xc0b800c9, /// 0x60c
			 0x47fa433b, /// 0x610
			 0x2db2b587, /// 0x614
			 0x198b875a, /// 0x618
			 0x5571857c, /// 0x61c
			 0x33a6f4d4, /// 0x620
			 0x495b0d2e, /// 0x624
			 0xa128f326, /// 0x628
			 0x166a6678, /// 0x62c
			 0xcf183438, /// 0x630
			 0xe1d1dd74, /// 0x634
			 0x0457c2d1, /// 0x638
			 0x3e76e2c1, /// 0x63c
			 0x117d0bde, /// 0x640
			 0xa2e8515f, /// 0x644
			 0x2c469b45, /// 0x648
			 0xe17066d1, /// 0x64c
			 0x47072937, /// 0x650
			 0xb91c061d, /// 0x654
			 0xa1d0a953, /// 0x658
			 0xc2aaeeaa, /// 0x65c
			 0xf3581d8a, /// 0x660
			 0x5c60d404, /// 0x664
			 0xe4331c71, /// 0x668
			 0x9ca8a2e1, /// 0x66c
			 0x05e05e32, /// 0x670
			 0x845236f8, /// 0x674
			 0x47af8c16, /// 0x678
			 0x77813938, /// 0x67c
			 0x71732b50, /// 0x680
			 0x05494a5b, /// 0x684
			 0x1e5d9f65, /// 0x688
			 0x278226de, /// 0x68c
			 0xbcf829da, /// 0x690
			 0xbe256f58, /// 0x694
			 0xf42e94cd, /// 0x698
			 0xefdd2304, /// 0x69c
			 0xe4e9726c, /// 0x6a0
			 0x464d4ad4, /// 0x6a4
			 0x04d1c4c6, /// 0x6a8
			 0x539ec331, /// 0x6ac
			 0xaf74d777, /// 0x6b0
			 0x050112ba, /// 0x6b4
			 0xc168e754, /// 0x6b8
			 0x74bcd366, /// 0x6bc
			 0xd02dd931, /// 0x6c0
			 0xbddb78a1, /// 0x6c4
			 0x345038d5, /// 0x6c8
			 0x1551b6b8, /// 0x6cc
			 0x2436fac8, /// 0x6d0
			 0x3b20cd10, /// 0x6d4
			 0x6b041bbc, /// 0x6d8
			 0xcd090d1f, /// 0x6dc
			 0xd4e95007, /// 0x6e0
			 0x61cff645, /// 0x6e4
			 0x78a4df0e, /// 0x6e8
			 0x3d80782c, /// 0x6ec
			 0xf4e38aa9, /// 0x6f0
			 0xd51f4bc8, /// 0x6f4
			 0x52e95e9c, /// 0x6f8
			 0x7f27e395, /// 0x6fc
			 0x8decd6e5, /// 0x700
			 0x410fcb45, /// 0x704
			 0xd565d67e, /// 0x708
			 0x2ee4d01f, /// 0x70c
			 0xc0880867, /// 0x710
			 0x1d0729d9, /// 0x714
			 0xecb79fea, /// 0x718
			 0x6fc580f7, /// 0x71c
			 0x62d9f7a1, /// 0x720
			 0x6bc27a58, /// 0x724
			 0x1c0930b9, /// 0x728
			 0x4f59ea4c, /// 0x72c
			 0x7510db36, /// 0x730
			 0x1fb1bfd8, /// 0x734
			 0x5a6456b5, /// 0x738
			 0x38ac9ad9, /// 0x73c
			 0xdc65b336, /// 0x740
			 0xa5248587, /// 0x744
			 0x29625682, /// 0x748
			 0x9fbbbb03, /// 0x74c
			 0x95c4b3f3, /// 0x750
			 0xf6460f87, /// 0x754
			 0x5d111f89, /// 0x758
			 0x59826f30, /// 0x75c
			 0xf2d97e23, /// 0x760
			 0xb0c02328, /// 0x764
			 0xa6af51ab, /// 0x768
			 0x35dcbb4c, /// 0x76c
			 0x8822a1d1, /// 0x770
			 0xeecc8eef, /// 0x774
			 0x50cee9e0, /// 0x778
			 0xc8bcdb89, /// 0x77c
			 0x7f4901d2, /// 0x780
			 0x9d47710b, /// 0x784
			 0x951d94bf, /// 0x788
			 0x308e32d1, /// 0x78c
			 0x744669d4, /// 0x790
			 0xda71db14, /// 0x794
			 0x8bd41e41, /// 0x798
			 0x2df392be, /// 0x79c
			 0x5a9b8a40, /// 0x7a0
			 0xc3127d59, /// 0x7a4
			 0x2b073ace, /// 0x7a8
			 0x620eb037, /// 0x7ac
			 0x16d9f941, /// 0x7b0
			 0x64b111e5, /// 0x7b4
			 0xb3d9f58c, /// 0x7b8
			 0xf42d27ae, /// 0x7bc
			 0xf2829fa3, /// 0x7c0
			 0xe664e178, /// 0x7c4
			 0x14e75d14, /// 0x7c8
			 0xfedd7339, /// 0x7cc
			 0x4ad54a64, /// 0x7d0
			 0x9025856b, /// 0x7d4
			 0xb1fb7f1a, /// 0x7d8
			 0x20ed1dda, /// 0x7dc
			 0x00b7787d, /// 0x7e0
			 0xb3e11629, /// 0x7e4
			 0x0d2c9851, /// 0x7e8
			 0x8173fd25, /// 0x7ec
			 0x76d69a0a, /// 0x7f0
			 0x28f4a675, /// 0x7f4
			 0xd0451c36, /// 0x7f8
			 0x8ff4920d, /// 0x7fc
			 0xdf95ba5d, /// 0x800
			 0x53e774e8, /// 0x804
			 0x06da91dd, /// 0x808
			 0x20dd0bdb, /// 0x80c
			 0x72fad91f, /// 0x810
			 0xe6fc0f3f, /// 0x814
			 0xb3a722e5, /// 0x818
			 0xe272e180, /// 0x81c
			 0xbea1da81, /// 0x820
			 0x52519afa, /// 0x824
			 0x86976b79, /// 0x828
			 0x85101ac0, /// 0x82c
			 0xd8f272db, /// 0x830
			 0xdc8dc8a7, /// 0x834
			 0x97e93df5, /// 0x838
			 0xa65666a8, /// 0x83c
			 0x4ae641f7, /// 0x840
			 0x66aae37a, /// 0x844
			 0x249c9809, /// 0x848
			 0xc909eb27, /// 0x84c
			 0x759473b4, /// 0x850
			 0xb0c7fbc5, /// 0x854
			 0x43d61d09, /// 0x858
			 0x730d68df, /// 0x85c
			 0xd60c192c, /// 0x860
			 0xbdd72ca8, /// 0x864
			 0x576b5847, /// 0x868
			 0x7ca94e69, /// 0x86c
			 0xeee6459c, /// 0x870
			 0xceed9e38, /// 0x874
			 0x6c43e3f6, /// 0x878
			 0x6ae490a6, /// 0x87c
			 0x17130ac4, /// 0x880
			 0x9a62ab41, /// 0x884
			 0xa4e2d6ec, /// 0x888
			 0x1342fbca, /// 0x88c
			 0xb59bbe20, /// 0x890
			 0x17168f0e, /// 0x894
			 0x37c50369, /// 0x898
			 0x9f7bbae4, /// 0x89c
			 0xba14ea31, /// 0x8a0
			 0x7bd00b33, /// 0x8a4
			 0x30af228f, /// 0x8a8
			 0x9eed993d, /// 0x8ac
			 0x5fc86043, /// 0x8b0
			 0xc0ace2af, /// 0x8b4
			 0x429542d1, /// 0x8b8
			 0x2cb519ff, /// 0x8bc
			 0xd7be0727, /// 0x8c0
			 0x42ca22d4, /// 0x8c4
			 0x54e94436, /// 0x8c8
			 0x0d7b2848, /// 0x8cc
			 0x128943f6, /// 0x8d0
			 0xa13bd8ed, /// 0x8d4
			 0x4c5a4997, /// 0x8d8
			 0xd19667a4, /// 0x8dc
			 0xdcc0ee28, /// 0x8e0
			 0x5d957875, /// 0x8e4
			 0x466db652, /// 0x8e8
			 0x5fe87e78, /// 0x8ec
			 0x024c303c, /// 0x8f0
			 0x3ef8672f, /// 0x8f4
			 0xcc091988, /// 0x8f8
			 0x056a0cbe, /// 0x8fc
			 0xeb9e6dc5, /// 0x900
			 0xb17f3de0, /// 0x904
			 0x834a9a8c, /// 0x908
			 0x1a6790fd, /// 0x90c
			 0xc663524d, /// 0x910
			 0x95fd5986, /// 0x914
			 0x6b442d13, /// 0x918
			 0xa93571e5, /// 0x91c
			 0x369235c6, /// 0x920
			 0xb3208c53, /// 0x924
			 0x11df8c8b, /// 0x928
			 0x9fde8b56, /// 0x92c
			 0x29ac402b, /// 0x930
			 0xa134b108, /// 0x934
			 0xe3c877fc, /// 0x938
			 0x15abbe03, /// 0x93c
			 0xc3a25d3d, /// 0x940
			 0xd7345a13, /// 0x944
			 0x7de2b59c, /// 0x948
			 0xda1adf2f, /// 0x94c
			 0xd6ab4d55, /// 0x950
			 0xc8affaae, /// 0x954
			 0xf8ec1264, /// 0x958
			 0xe50641c1, /// 0x95c
			 0x93bf060e, /// 0x960
			 0x406f3a7b, /// 0x964
			 0xd7132216, /// 0x968
			 0x2b2a572a, /// 0x96c
			 0xa679850d, /// 0x970
			 0x4ad3a837, /// 0x974
			 0x95e545a5, /// 0x978
			 0x97f53e1d, /// 0x97c
			 0x48d08e56, /// 0x980
			 0xfebbdf13, /// 0x984
			 0x2771c53a, /// 0x988
			 0xa363d7b8, /// 0x98c
			 0x7f09df65, /// 0x990
			 0x58991acd, /// 0x994
			 0xb6d1f24d, /// 0x998
			 0x51ae2bf3, /// 0x99c
			 0x103e61dc, /// 0x9a0
			 0x2c01edc8, /// 0x9a4
			 0x492bf21d, /// 0x9a8
			 0x13e975d7, /// 0x9ac
			 0xc279fca2, /// 0x9b0
			 0xd467d608, /// 0x9b4
			 0xea45f72b, /// 0x9b8
			 0x95df1db4, /// 0x9bc
			 0x9b1c44c1, /// 0x9c0
			 0xec993a48, /// 0x9c4
			 0x3b9d30b9, /// 0x9c8
			 0xb5c3d15c, /// 0x9cc
			 0x8a734309, /// 0x9d0
			 0x597436da, /// 0x9d4
			 0x49a5fd37, /// 0x9d8
			 0xf27d398a, /// 0x9dc
			 0x35bb7930, /// 0x9e0
			 0xdf9230b9, /// 0x9e4
			 0x1477ec86, /// 0x9e8
			 0x23d7774f, /// 0x9ec
			 0xf0b153c7, /// 0x9f0
			 0xfc967696, /// 0x9f4
			 0xc646c3bd, /// 0x9f8
			 0x446beff3, /// 0x9fc
			 0x5462367c, /// 0xa00
			 0xdb7a6613, /// 0xa04
			 0xdd0ab0ee, /// 0xa08
			 0xd9aaf5e1, /// 0xa0c
			 0x32b063ca, /// 0xa10
			 0x84748355, /// 0xa14
			 0x97ab3a1d, /// 0xa18
			 0xc7292740, /// 0xa1c
			 0x2fd8309d, /// 0xa20
			 0x35212386, /// 0xa24
			 0x9a3be5a4, /// 0xa28
			 0x8bd426b3, /// 0xa2c
			 0x19be6c88, /// 0xa30
			 0xcbbfa335, /// 0xa34
			 0x75a44f56, /// 0xa38
			 0xdaeb3d82, /// 0xa3c
			 0x4dc2af1c, /// 0xa40
			 0xa6ac2829, /// 0xa44
			 0x98864245, /// 0xa48
			 0x50b54a58, /// 0xa4c
			 0x4f08d9cb, /// 0xa50
			 0x004feb51, /// 0xa54
			 0x224978a4, /// 0xa58
			 0x82caec72, /// 0xa5c
			 0xd2876670, /// 0xa60
			 0xacf1ee9c, /// 0xa64
			 0xe35dcca8, /// 0xa68
			 0x934e857b, /// 0xa6c
			 0x2ddd23bb, /// 0xa70
			 0x1dc96cee, /// 0xa74
			 0x5ea12069, /// 0xa78
			 0x60ed6aea, /// 0xa7c
			 0x48c0ea24, /// 0xa80
			 0x38e899de, /// 0xa84
			 0x879bcbea, /// 0xa88
			 0x7c0d5194, /// 0xa8c
			 0xd07a779f, /// 0xa90
			 0x898116ef, /// 0xa94
			 0x960428cf, /// 0xa98
			 0xbf572847, /// 0xa9c
			 0x88f39513, /// 0xaa0
			 0xc304e677, /// 0xaa4
			 0x67f3df30, /// 0xaa8
			 0xcf04f02d, /// 0xaac
			 0xa49ec9c5, /// 0xab0
			 0x8856062a, /// 0xab4
			 0xa646bcf0, /// 0xab8
			 0x5211de34, /// 0xabc
			 0xffe52090, /// 0xac0
			 0x413ea6f6, /// 0xac4
			 0x40414041, /// 0xac8
			 0x982a84fd, /// 0xacc
			 0x8a572fc0, /// 0xad0
			 0xf8d74154, /// 0xad4
			 0x2d046ada, /// 0xad8
			 0x8cb5f6cf, /// 0xadc
			 0x61062fd4, /// 0xae0
			 0x06d1f065, /// 0xae4
			 0x611b8550, /// 0xae8
			 0xa3036df2, /// 0xaec
			 0x14143887, /// 0xaf0
			 0xcec557b5, /// 0xaf4
			 0xdd3e865b, /// 0xaf8
			 0xaf02b702, /// 0xafc
			 0xfd9a7d26, /// 0xb00
			 0x9946cda0, /// 0xb04
			 0x606d7919, /// 0xb08
			 0xe19c9661, /// 0xb0c
			 0x62ec510c, /// 0xb10
			 0x7aa22b84, /// 0xb14
			 0xa1727ae4, /// 0xb18
			 0xfc633786, /// 0xb1c
			 0x117ad5d7, /// 0xb20
			 0x453c66d0, /// 0xb24
			 0xf2b73449, /// 0xb28
			 0x7fcc6a4c, /// 0xb2c
			 0xff2a7398, /// 0xb30
			 0xb935c5bf, /// 0xb34
			 0x607e032a, /// 0xb38
			 0x743533d5, /// 0xb3c
			 0x1853af21, /// 0xb40
			 0x028a87a9, /// 0xb44
			 0x99e969b7, /// 0xb48
			 0x5d5c252c, /// 0xb4c
			 0x88dd5049, /// 0xb50
			 0xfab3a804, /// 0xb54
			 0xf2d8e48e, /// 0xb58
			 0x4ed6e3d5, /// 0xb5c
			 0x78c610fb, /// 0xb60
			 0x6f12b1e6, /// 0xb64
			 0x9181fc6b, /// 0xb68
			 0xdc9ebe91, /// 0xb6c
			 0xcd0139d2, /// 0xb70
			 0x6f6df75c, /// 0xb74
			 0xa3349940, /// 0xb78
			 0xde6060f7, /// 0xb7c
			 0x374fe80f, /// 0xb80
			 0x319b3c71, /// 0xb84
			 0x7c94c9ca, /// 0xb88
			 0x99c7d71b, /// 0xb8c
			 0xfae11e8f, /// 0xb90
			 0x68d8586f, /// 0xb94
			 0x80ab3b17, /// 0xb98
			 0x003d465a, /// 0xb9c
			 0x9889ae77, /// 0xba0
			 0xf81032df, /// 0xba4
			 0xcc05c6e1, /// 0xba8
			 0x8e22f62e, /// 0xbac
			 0xbb6912c3, /// 0xbb0
			 0x99cb0231, /// 0xbb4
			 0x7aa7d4e5, /// 0xbb8
			 0xff80fd73, /// 0xbbc
			 0x146aaaa0, /// 0xbc0
			 0x48e93520, /// 0xbc4
			 0xd085b594, /// 0xbc8
			 0x32410957, /// 0xbcc
			 0xd4c86528, /// 0xbd0
			 0x5ad1bca6, /// 0xbd4
			 0xe89b73dd, /// 0xbd8
			 0x5885d22c, /// 0xbdc
			 0xc268c949, /// 0xbe0
			 0x2e5c0d80, /// 0xbe4
			 0x36c58994, /// 0xbe8
			 0x425f83ff, /// 0xbec
			 0x422dfa36, /// 0xbf0
			 0x04efa020, /// 0xbf4
			 0x585cc626, /// 0xbf8
			 0xec0450ca, /// 0xbfc
			 0x646ac9b6, /// 0xc00
			 0x7bde9346, /// 0xc04
			 0x4eeaecf3, /// 0xc08
			 0x14551c0e, /// 0xc0c
			 0x14d3048c, /// 0xc10
			 0x755b09fc, /// 0xc14
			 0x9666592b, /// 0xc18
			 0xea0c782a, /// 0xc1c
			 0xf5cec770, /// 0xc20
			 0xe8ffa32d, /// 0xc24
			 0xbb88a731, /// 0xc28
			 0x9f5525eb, /// 0xc2c
			 0xc3327215, /// 0xc30
			 0x7833c950, /// 0xc34
			 0x4a1b8012, /// 0xc38
			 0x6224531e, /// 0xc3c
			 0x1f40a973, /// 0xc40
			 0x8e5fd3ec, /// 0xc44
			 0xf30a9462, /// 0xc48
			 0xaa34d75c, /// 0xc4c
			 0x7173124e, /// 0xc50
			 0x198a3cb5, /// 0xc54
			 0x44355a19, /// 0xc58
			 0x93dcddda, /// 0xc5c
			 0x1a3ffa88, /// 0xc60
			 0x62eaf89b, /// 0xc64
			 0x8fc8fe21, /// 0xc68
			 0xef5ec2c6, /// 0xc6c
			 0x2d4ca6fb, /// 0xc70
			 0x24fd063a, /// 0xc74
			 0xbdf4f0d7, /// 0xc78
			 0xa1a0183a, /// 0xc7c
			 0xc7f2871b, /// 0xc80
			 0x6dd4d8c2, /// 0xc84
			 0x2b2d7774, /// 0xc88
			 0x3c5df0ad, /// 0xc8c
			 0xeae046e1, /// 0xc90
			 0xba6ca75f, /// 0xc94
			 0x4d00b305, /// 0xc98
			 0x71a07692, /// 0xc9c
			 0x0ea74c16, /// 0xca0
			 0x0466acfc, /// 0xca4
			 0x1092d2e0, /// 0xca8
			 0x917d9d08, /// 0xcac
			 0x6db4ac3c, /// 0xcb0
			 0x7e784cfb, /// 0xcb4
			 0xed4589cc, /// 0xcb8
			 0x3ac4458c, /// 0xcbc
			 0x545adda0, /// 0xcc0
			 0xcf26aa83, /// 0xcc4
			 0x88a6bd5e, /// 0xcc8
			 0x645e09e0, /// 0xccc
			 0x6ff526d0, /// 0xcd0
			 0xbcf9952f, /// 0xcd4
			 0xd5561777, /// 0xcd8
			 0xf54a6582, /// 0xcdc
			 0x4289bbfa, /// 0xce0
			 0x07791422, /// 0xce4
			 0x941df3e0, /// 0xce8
			 0x11af1572, /// 0xcec
			 0xf5ade717, /// 0xcf0
			 0xafc7e9d6, /// 0xcf4
			 0xb5f17c49, /// 0xcf8
			 0x9b7a402c, /// 0xcfc
			 0xf823d168, /// 0xd00
			 0x1db8be22, /// 0xd04
			 0x15c800f6, /// 0xd08
			 0x4f85119d, /// 0xd0c
			 0xb945c7ea, /// 0xd10
			 0xc917e0c9, /// 0xd14
			 0x3d376cb1, /// 0xd18
			 0xd8cf2d03, /// 0xd1c
			 0x41def7d8, /// 0xd20
			 0xc357a4bd, /// 0xd24
			 0x38e894ed, /// 0xd28
			 0x73a48316, /// 0xd2c
			 0xca17274d, /// 0xd30
			 0x001054cc, /// 0xd34
			 0x226a0809, /// 0xd38
			 0x76a90c3b, /// 0xd3c
			 0x037f9a93, /// 0xd40
			 0x32e31fd8, /// 0xd44
			 0x4fff61d4, /// 0xd48
			 0xd7e4c938, /// 0xd4c
			 0x70f52adc, /// 0xd50
			 0xa53842a8, /// 0xd54
			 0xf92cee9e, /// 0xd58
			 0xa2a2aaba, /// 0xd5c
			 0xefb3a6e4, /// 0xd60
			 0xaa7faa04, /// 0xd64
			 0xca6c8980, /// 0xd68
			 0x3575022f, /// 0xd6c
			 0xb38f0dde, /// 0xd70
			 0x04a591a1, /// 0xd74
			 0x50e6dbff, /// 0xd78
			 0xdb479cd0, /// 0xd7c
			 0xee00d503, /// 0xd80
			 0x332a1f9d, /// 0xd84
			 0x03670621, /// 0xd88
			 0xb87bd646, /// 0xd8c
			 0x85f3ebf4, /// 0xd90
			 0xe8a96ff7, /// 0xd94
			 0x20a5cb0a, /// 0xd98
			 0x957998a9, /// 0xd9c
			 0x34afb0f2, /// 0xda0
			 0xf57fa527, /// 0xda4
			 0xb0f2fc11, /// 0xda8
			 0xd8197c51, /// 0xdac
			 0x97e7e4b7, /// 0xdb0
			 0x65e749c6, /// 0xdb4
			 0x4f5a9768, /// 0xdb8
			 0xa2250654, /// 0xdbc
			 0x0458c09c, /// 0xdc0
			 0xbbadaec5, /// 0xdc4
			 0x9bd39ce9, /// 0xdc8
			 0xd00f5e2e, /// 0xdcc
			 0x8324d261, /// 0xdd0
			 0x83285599, /// 0xdd4
			 0x34c74e67, /// 0xdd8
			 0x6518da69, /// 0xddc
			 0x068046ab, /// 0xde0
			 0x8bd2f5ed, /// 0xde4
			 0x39908fae, /// 0xde8
			 0x6fa05d41, /// 0xdec
			 0x1bde0a56, /// 0xdf0
			 0xa65d69b7, /// 0xdf4
			 0x07fcc681, /// 0xdf8
			 0x148681c8, /// 0xdfc
			 0x436f060b, /// 0xe00
			 0x235280f6, /// 0xe04
			 0xc58053db, /// 0xe08
			 0x5a5fa96f, /// 0xe0c
			 0xc63eeea6, /// 0xe10
			 0xdea9bd92, /// 0xe14
			 0x163ca0a8, /// 0xe18
			 0xc3293008, /// 0xe1c
			 0x181ddcfe, /// 0xe20
			 0xd9a874d4, /// 0xe24
			 0x6cd3efba, /// 0xe28
			 0xd5b45879, /// 0xe2c
			 0x114c5e43, /// 0xe30
			 0xeda579c1, /// 0xe34
			 0xccb4358a, /// 0xe38
			 0x831bbec9, /// 0xe3c
			 0xb4173f10, /// 0xe40
			 0x43a1a406, /// 0xe44
			 0x2f52f80a, /// 0xe48
			 0x5eb8c8d5, /// 0xe4c
			 0xeee6274c, /// 0xe50
			 0xc2112247, /// 0xe54
			 0xbc9eb049, /// 0xe58
			 0x7ae13534, /// 0xe5c
			 0x772bc10f, /// 0xe60
			 0xe6d70c03, /// 0xe64
			 0xeea4b2db, /// 0xe68
			 0x7b9b8085, /// 0xe6c
			 0x1f12cb6f, /// 0xe70
			 0xb4d87987, /// 0xe74
			 0xe9b009e7, /// 0xe78
			 0xffc1ce18, /// 0xe7c
			 0x1287b29f, /// 0xe80
			 0x7e6b59bc, /// 0xe84
			 0xd98a93c2, /// 0xe88
			 0xa66425d0, /// 0xe8c
			 0xe438b137, /// 0xe90
			 0xf66327e8, /// 0xe94
			 0x45cfc8c2, /// 0xe98
			 0x09e43550, /// 0xe9c
			 0x1589103f, /// 0xea0
			 0x2aa0c2ea, /// 0xea4
			 0x459e7bcf, /// 0xea8
			 0x858b5cb1, /// 0xeac
			 0x8ac3d212, /// 0xeb0
			 0xf27e587e, /// 0xeb4
			 0xb7244e45, /// 0xeb8
			 0x21422951, /// 0xebc
			 0x728aeb53, /// 0xec0
			 0x47d28b07, /// 0xec4
			 0x7db4112a, /// 0xec8
			 0xb5b8c176, /// 0xecc
			 0x36eeb76b, /// 0xed0
			 0x0a96dd42, /// 0xed4
			 0xc1c786c3, /// 0xed8
			 0x995817f9, /// 0xedc
			 0xa0f8cb65, /// 0xee0
			 0x663d6163, /// 0xee4
			 0xb5ca6f22, /// 0xee8
			 0xcf6e89e6, /// 0xeec
			 0xd8b8f895, /// 0xef0
			 0x4ffb56a9, /// 0xef4
			 0xf85d2b1d, /// 0xef8
			 0x2ec55b6b, /// 0xefc
			 0x750b722b, /// 0xf00
			 0xc59f064e, /// 0xf04
			 0x5091e9c0, /// 0xf08
			 0x7a2b4421, /// 0xf0c
			 0x198660a4, /// 0xf10
			 0x79cdf246, /// 0xf14
			 0x76ef13c3, /// 0xf18
			 0xe71f09a5, /// 0xf1c
			 0x5feb6a39, /// 0xf20
			 0xde49a046, /// 0xf24
			 0x707a246f, /// 0xf28
			 0x7e12a52b, /// 0xf2c
			 0xb6604798, /// 0xf30
			 0x53c76dea, /// 0xf34
			 0xd1bcfcae, /// 0xf38
			 0x0d190cb5, /// 0xf3c
			 0xf96dc6b3, /// 0xf40
			 0xe7f5899f, /// 0xf44
			 0x06491b9e, /// 0xf48
			 0x41ecb0a1, /// 0xf4c
			 0xf6ed0b43, /// 0xf50
			 0x065a72aa, /// 0xf54
			 0x4f9cd787, /// 0xf58
			 0xfccf1d5a, /// 0xf5c
			 0xdb4490e3, /// 0xf60
			 0xa8a9f9dd, /// 0xf64
			 0xa2b100d2, /// 0xf68
			 0x27701e33, /// 0xf6c
			 0xe70830ce, /// 0xf70
			 0x1b33adb8, /// 0xf74
			 0xd2860ead, /// 0xf78
			 0x81551cae, /// 0xf7c
			 0xd844c032, /// 0xf80
			 0x33eed594, /// 0xf84
			 0x43ce092d, /// 0xf88
			 0x3e7b1fc8, /// 0xf8c
			 0xe626331e, /// 0xf90
			 0x8b77493c, /// 0xf94
			 0x29a5a52a, /// 0xf98
			 0x625869e6, /// 0xf9c
			 0xf9c8cd24, /// 0xfa0
			 0x795bb00a, /// 0xfa4
			 0x71146cdf, /// 0xfa8
			 0x0136c9c9, /// 0xfac
			 0x766312ab, /// 0xfb0
			 0xf68881d2, /// 0xfb4
			 0xe89b3756, /// 0xfb8
			 0xc151dde4, /// 0xfbc
			 0x5d549d4c, /// 0xfc0
			 0x26abd8a7, /// 0xfc4
			 0x46c59f82, /// 0xfc8
			 0xdae337a3, /// 0xfcc
			 0x33dd2b15, /// 0xfd0
			 0x8ea6992a, /// 0xfd4
			 0x99e46f1f, /// 0xfd8
			 0x876fb6bc, /// 0xfdc
			 0x85de2dd4, /// 0xfe0
			 0xe47f0b66, /// 0xfe4
			 0xd65e9003, /// 0xfe8
			 0xda47f5d3, /// 0xfec
			 0xee9c8b13, /// 0xff0
			 0xacbad05a, /// 0xff4
			 0xaceb997b, /// 0xff8
			 0xe1f4ad16 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xa9413403, /// 0x0
			 0x7c27221b, /// 0x4
			 0x658378e5, /// 0x8
			 0x2a62fdff, /// 0xc
			 0x6a613d02, /// 0x10
			 0x66d6821a, /// 0x14
			 0x91f3f221, /// 0x18
			 0xbb8e5e3b, /// 0x1c
			 0x0cd3f3e6, /// 0x20
			 0xf611530a, /// 0x24
			 0x0071882d, /// 0x28
			 0xf1166a8a, /// 0x2c
			 0x0f6d08a7, /// 0x30
			 0x0ae6c168, /// 0x34
			 0xe51eac6f, /// 0x38
			 0xb803b468, /// 0x3c
			 0x6226518f, /// 0x40
			 0xeb87fb6b, /// 0x44
			 0x422a0f47, /// 0x48
			 0x4b6b0e36, /// 0x4c
			 0xe69f8ce5, /// 0x50
			 0x13d68b65, /// 0x54
			 0xe1800a78, /// 0x58
			 0x03854fe7, /// 0x5c
			 0xc61fbbef, /// 0x60
			 0x98c4fba5, /// 0x64
			 0xa25c2065, /// 0x68
			 0x181e9e00, /// 0x6c
			 0x7dfb7f4c, /// 0x70
			 0x867faaa5, /// 0x74
			 0x662056ec, /// 0x78
			 0xf29379b8, /// 0x7c
			 0xd645561d, /// 0x80
			 0x15e3959d, /// 0x84
			 0x967a64c5, /// 0x88
			 0xe3bc923e, /// 0x8c
			 0xf596e419, /// 0x90
			 0x0df29a05, /// 0x94
			 0xc69e84f9, /// 0x98
			 0x330441f7, /// 0x9c
			 0x25347a8d, /// 0xa0
			 0xe27fa9ff, /// 0xa4
			 0xe2d63122, /// 0xa8
			 0xde6d9eaf, /// 0xac
			 0x61cbcf29, /// 0xb0
			 0xd02fd4f5, /// 0xb4
			 0x5cf07bd5, /// 0xb8
			 0xe19467fb, /// 0xbc
			 0xd9bed165, /// 0xc0
			 0x8bb1a212, /// 0xc4
			 0x72506bde, /// 0xc8
			 0x5e607399, /// 0xcc
			 0x1e569f73, /// 0xd0
			 0xf654fdad, /// 0xd4
			 0xd325fa57, /// 0xd8
			 0xb2bc8f27, /// 0xdc
			 0xe34f49dc, /// 0xe0
			 0xcad5e007, /// 0xe4
			 0xe409f5d3, /// 0xe8
			 0x2c4c4a25, /// 0xec
			 0x85e72b75, /// 0xf0
			 0x9e1a8b9b, /// 0xf4
			 0xbeca69bb, /// 0xf8
			 0xa80009bb, /// 0xfc
			 0x66e1ec82, /// 0x100
			 0xfe0e4eb3, /// 0x104
			 0xee5be747, /// 0x108
			 0xa874e77e, /// 0x10c
			 0xe2dfd3a2, /// 0x110
			 0x0d20c2f4, /// 0x114
			 0x6df279a4, /// 0x118
			 0xe4c5955e, /// 0x11c
			 0x2b57b198, /// 0x120
			 0x6ada1cf1, /// 0x124
			 0xcddac235, /// 0x128
			 0xb7c64ab6, /// 0x12c
			 0xe0e267d5, /// 0x130
			 0x10cde3e8, /// 0x134
			 0xbfeab9ea, /// 0x138
			 0x1ee5b2d3, /// 0x13c
			 0x2ec80b29, /// 0x140
			 0x0e07c4a8, /// 0x144
			 0x86033d91, /// 0x148
			 0xba634bbe, /// 0x14c
			 0x685e6c4d, /// 0x150
			 0x8f3e2908, /// 0x154
			 0x15fcbb85, /// 0x158
			 0x923d3554, /// 0x15c
			 0x37338aa7, /// 0x160
			 0x86ed8058, /// 0x164
			 0x8f6c225f, /// 0x168
			 0x633dcbef, /// 0x16c
			 0x58b93496, /// 0x170
			 0x4ecc40f6, /// 0x174
			 0xa8b82236, /// 0x178
			 0x0135039d, /// 0x17c
			 0x6c405b15, /// 0x180
			 0x435798b8, /// 0x184
			 0x5ff6f438, /// 0x188
			 0x7d2a5f39, /// 0x18c
			 0xf9a24520, /// 0x190
			 0x25cb15c4, /// 0x194
			 0x518360fe, /// 0x198
			 0x6203c4a1, /// 0x19c
			 0xcf01563f, /// 0x1a0
			 0x62a2de35, /// 0x1a4
			 0xbfe34785, /// 0x1a8
			 0x57b38c12, /// 0x1ac
			 0x392e1044, /// 0x1b0
			 0x6861dfdf, /// 0x1b4
			 0x68921137, /// 0x1b8
			 0x52c16366, /// 0x1bc
			 0x0d211190, /// 0x1c0
			 0xb9453039, /// 0x1c4
			 0xe27fde31, /// 0x1c8
			 0xfb038f36, /// 0x1cc
			 0x3fbfea32, /// 0x1d0
			 0x63920573, /// 0x1d4
			 0xfe1d569a, /// 0x1d8
			 0x01fb13ef, /// 0x1dc
			 0x98282874, /// 0x1e0
			 0xf7d03191, /// 0x1e4
			 0xfd1adb14, /// 0x1e8
			 0xc1442039, /// 0x1ec
			 0xed92de2b, /// 0x1f0
			 0xca0fdcbe, /// 0x1f4
			 0x02b4bb8d, /// 0x1f8
			 0x716dd47b, /// 0x1fc
			 0xd57ff1eb, /// 0x200
			 0x98e8152a, /// 0x204
			 0x1b1d620a, /// 0x208
			 0x6dc34ff1, /// 0x20c
			 0x46aa26cb, /// 0x210
			 0xab7a25ac, /// 0x214
			 0x2e5d3ba9, /// 0x218
			 0xb5c22b4f, /// 0x21c
			 0x4900c8f4, /// 0x220
			 0x56c46f90, /// 0x224
			 0xdd7702e7, /// 0x228
			 0x15e2ea17, /// 0x22c
			 0x203e3bd9, /// 0x230
			 0xb7b5d693, /// 0x234
			 0x5bf1cb09, /// 0x238
			 0x809da7c7, /// 0x23c
			 0x2ecc51af, /// 0x240
			 0x68b5c117, /// 0x244
			 0xcc4ec528, /// 0x248
			 0x07e489de, /// 0x24c
			 0x48efd60d, /// 0x250
			 0xf4bb1e7d, /// 0x254
			 0x8313afb8, /// 0x258
			 0xc30eefbe, /// 0x25c
			 0x61d1fc35, /// 0x260
			 0xc1449599, /// 0x264
			 0x940638f7, /// 0x268
			 0x43dde0e5, /// 0x26c
			 0x9856f09d, /// 0x270
			 0x865902d0, /// 0x274
			 0xccb8013d, /// 0x278
			 0xe5e07593, /// 0x27c
			 0x09535898, /// 0x280
			 0x4c1620c4, /// 0x284
			 0x54cfa1bd, /// 0x288
			 0x70c51a1f, /// 0x28c
			 0x3917daed, /// 0x290
			 0x292ec27f, /// 0x294
			 0xefdcda87, /// 0x298
			 0x4939f936, /// 0x29c
			 0xd3479c3a, /// 0x2a0
			 0x391e5c04, /// 0x2a4
			 0x99ad3170, /// 0x2a8
			 0x81eae2cd, /// 0x2ac
			 0xe3a56c23, /// 0x2b0
			 0xc9a29233, /// 0x2b4
			 0xdbfb4a16, /// 0x2b8
			 0x974ca96c, /// 0x2bc
			 0x3c60d267, /// 0x2c0
			 0xf203f4a5, /// 0x2c4
			 0xd538c758, /// 0x2c8
			 0xfed97b5f, /// 0x2cc
			 0xf86cee48, /// 0x2d0
			 0x99745570, /// 0x2d4
			 0x1af014d7, /// 0x2d8
			 0xee78981b, /// 0x2dc
			 0x80a02551, /// 0x2e0
			 0x7fb72a60, /// 0x2e4
			 0x15bdb579, /// 0x2e8
			 0xbef06de0, /// 0x2ec
			 0x580a7f7b, /// 0x2f0
			 0x569316b7, /// 0x2f4
			 0x65db61b8, /// 0x2f8
			 0xed3446ce, /// 0x2fc
			 0x31694321, /// 0x300
			 0x4b2bd6bb, /// 0x304
			 0x321c9b57, /// 0x308
			 0x801a0012, /// 0x30c
			 0x2207f4dd, /// 0x310
			 0x3417c14c, /// 0x314
			 0x829209d8, /// 0x318
			 0x650ee8a3, /// 0x31c
			 0x74efdf30, /// 0x320
			 0xa73ed773, /// 0x324
			 0x5759efda, /// 0x328
			 0x812a9684, /// 0x32c
			 0xf1207665, /// 0x330
			 0xe5230c94, /// 0x334
			 0xcff26f15, /// 0x338
			 0x76250403, /// 0x33c
			 0xa35372f6, /// 0x340
			 0x09d077f0, /// 0x344
			 0xc6767bf7, /// 0x348
			 0xd8f6afe4, /// 0x34c
			 0xc1958b6b, /// 0x350
			 0x7524f303, /// 0x354
			 0x9d04bc71, /// 0x358
			 0xfac18ff6, /// 0x35c
			 0xea31f35c, /// 0x360
			 0x917d8bab, /// 0x364
			 0xebe8d988, /// 0x368
			 0x6d3101e2, /// 0x36c
			 0x42175738, /// 0x370
			 0x9e37dd82, /// 0x374
			 0x37e0f7ad, /// 0x378
			 0xfa3884dd, /// 0x37c
			 0x7d70e353, /// 0x380
			 0x9615b4c1, /// 0x384
			 0x289d80f6, /// 0x388
			 0x5919b428, /// 0x38c
			 0x69e323c2, /// 0x390
			 0x58152f79, /// 0x394
			 0x851bb8a6, /// 0x398
			 0x484dd0b2, /// 0x39c
			 0x6e451083, /// 0x3a0
			 0xd711298f, /// 0x3a4
			 0x679a2b38, /// 0x3a8
			 0xf155db87, /// 0x3ac
			 0xdc7d82b8, /// 0x3b0
			 0x8738e206, /// 0x3b4
			 0x7c00c120, /// 0x3b8
			 0x33ec7dac, /// 0x3bc
			 0x89d8f40a, /// 0x3c0
			 0x4aa083da, /// 0x3c4
			 0x60377256, /// 0x3c8
			 0x4ca183ce, /// 0x3cc
			 0x0a8494c8, /// 0x3d0
			 0x15dcd60c, /// 0x3d4
			 0x56f4335f, /// 0x3d8
			 0xb6c59855, /// 0x3dc
			 0xbc80746a, /// 0x3e0
			 0x31b5dbef, /// 0x3e4
			 0x1a19a3dc, /// 0x3e8
			 0x7d602e00, /// 0x3ec
			 0xd28bd157, /// 0x3f0
			 0xa53452d4, /// 0x3f4
			 0xc72e302e, /// 0x3f8
			 0x4513ad1d, /// 0x3fc
			 0x6b23864d, /// 0x400
			 0x994928f1, /// 0x404
			 0x90c174c8, /// 0x408
			 0x0b39d126, /// 0x40c
			 0x22345a3a, /// 0x410
			 0x5e66509a, /// 0x414
			 0x4387555e, /// 0x418
			 0xb4d1f308, /// 0x41c
			 0x8d05870a, /// 0x420
			 0x7a7f7128, /// 0x424
			 0x3a33ff90, /// 0x428
			 0x2865ede9, /// 0x42c
			 0x207992bb, /// 0x430
			 0xd56923a3, /// 0x434
			 0x29cdaa3b, /// 0x438
			 0x5b41fed0, /// 0x43c
			 0x6095e43a, /// 0x440
			 0x80973667, /// 0x444
			 0xf60ff58e, /// 0x448
			 0xcadcc017, /// 0x44c
			 0x016ec978, /// 0x450
			 0xdb3f4a6b, /// 0x454
			 0x820f2dd8, /// 0x458
			 0x9b06d1ac, /// 0x45c
			 0x3fe77fd8, /// 0x460
			 0x09db47be, /// 0x464
			 0x8af4d016, /// 0x468
			 0xd883b2b2, /// 0x46c
			 0xc8432756, /// 0x470
			 0x798b6666, /// 0x474
			 0x51f91228, /// 0x478
			 0xb2dc02bb, /// 0x47c
			 0xe7e8dbc5, /// 0x480
			 0x595500f9, /// 0x484
			 0xcd1bc235, /// 0x488
			 0x46f3cea9, /// 0x48c
			 0x098b9621, /// 0x490
			 0x86e7e732, /// 0x494
			 0x36138100, /// 0x498
			 0x970c1ced, /// 0x49c
			 0x6e967be0, /// 0x4a0
			 0x3806f30f, /// 0x4a4
			 0x622c43fc, /// 0x4a8
			 0x23cc41f5, /// 0x4ac
			 0x28761e7e, /// 0x4b0
			 0x7dc54d80, /// 0x4b4
			 0xd221ab40, /// 0x4b8
			 0x48a2f7bb, /// 0x4bc
			 0x212d138a, /// 0x4c0
			 0xdb1cdeb4, /// 0x4c4
			 0x32accea0, /// 0x4c8
			 0x0432e825, /// 0x4cc
			 0x1aa7bfc7, /// 0x4d0
			 0xb42d15ca, /// 0x4d4
			 0xcb889de5, /// 0x4d8
			 0xdf00c332, /// 0x4dc
			 0x599bd599, /// 0x4e0
			 0xaf1b7afb, /// 0x4e4
			 0xa9ad4d0c, /// 0x4e8
			 0xe665dadc, /// 0x4ec
			 0xff626c73, /// 0x4f0
			 0x3a398e59, /// 0x4f4
			 0x03d72e4f, /// 0x4f8
			 0x1ee6efd1, /// 0x4fc
			 0x13cd93db, /// 0x500
			 0x605373c2, /// 0x504
			 0x982c8107, /// 0x508
			 0x21827ac6, /// 0x50c
			 0x4b7795c2, /// 0x510
			 0x2c27cd9a, /// 0x514
			 0xd9eeeaab, /// 0x518
			 0x52bc91db, /// 0x51c
			 0xb71103a2, /// 0x520
			 0x3ade5d42, /// 0x524
			 0x60525453, /// 0x528
			 0x8674ecf9, /// 0x52c
			 0xa427e8e8, /// 0x530
			 0x03de7e94, /// 0x534
			 0x76197dcf, /// 0x538
			 0x8c53c958, /// 0x53c
			 0x0e4a5ad4, /// 0x540
			 0xcb2ee1d1, /// 0x544
			 0x4b87e1dc, /// 0x548
			 0xa989fdea, /// 0x54c
			 0x03e9ea46, /// 0x550
			 0x01b1572a, /// 0x554
			 0x6dc368b1, /// 0x558
			 0x7443e4e7, /// 0x55c
			 0xd6375fda, /// 0x560
			 0xeebedaa7, /// 0x564
			 0xdd2a1bfe, /// 0x568
			 0x56cb5aad, /// 0x56c
			 0x9a052801, /// 0x570
			 0xdc83a503, /// 0x574
			 0xe81a6787, /// 0x578
			 0x35a684c6, /// 0x57c
			 0x4b2d418c, /// 0x580
			 0xadde7a28, /// 0x584
			 0x65d2ef39, /// 0x588
			 0x83b8292d, /// 0x58c
			 0x88ce2467, /// 0x590
			 0x5992fd3e, /// 0x594
			 0x3269b9aa, /// 0x598
			 0xe1e1aee4, /// 0x59c
			 0x9ef61843, /// 0x5a0
			 0xbaf00429, /// 0x5a4
			 0xe0855658, /// 0x5a8
			 0x8aa415e9, /// 0x5ac
			 0x9f9ffe3a, /// 0x5b0
			 0x4f7323bb, /// 0x5b4
			 0x7af2234d, /// 0x5b8
			 0x2415a7b7, /// 0x5bc
			 0x8c72402c, /// 0x5c0
			 0x27ecbe4a, /// 0x5c4
			 0x38388408, /// 0x5c8
			 0xcd5a3369, /// 0x5cc
			 0x9dd2e9ae, /// 0x5d0
			 0xe3271d9a, /// 0x5d4
			 0xbf19ec3d, /// 0x5d8
			 0x574706dd, /// 0x5dc
			 0xcf4fbe64, /// 0x5e0
			 0xd1a7866e, /// 0x5e4
			 0x6532c70d, /// 0x5e8
			 0x337cdbd9, /// 0x5ec
			 0xfe4b8029, /// 0x5f0
			 0x54034abe, /// 0x5f4
			 0xf6812ba2, /// 0x5f8
			 0xcce12329, /// 0x5fc
			 0x0b8505c0, /// 0x600
			 0xd03ccc4d, /// 0x604
			 0x6b7283f3, /// 0x608
			 0x583a49a8, /// 0x60c
			 0x87a5db62, /// 0x610
			 0xba6315a0, /// 0x614
			 0xdd1488df, /// 0x618
			 0xa302f720, /// 0x61c
			 0xd6a6f6a8, /// 0x620
			 0x97ac3d70, /// 0x624
			 0x62b3c0a2, /// 0x628
			 0x3142eb38, /// 0x62c
			 0x5878ad4f, /// 0x630
			 0x6f7d51e9, /// 0x634
			 0x02ba2b16, /// 0x638
			 0x332b3cfd, /// 0x63c
			 0x4c0b9a22, /// 0x640
			 0x3bbdfb6c, /// 0x644
			 0x04db7c8d, /// 0x648
			 0xc38caa71, /// 0x64c
			 0xd5fcd506, /// 0x650
			 0x1a4d0980, /// 0x654
			 0x2670719d, /// 0x658
			 0x6874c31c, /// 0x65c
			 0x73b1599d, /// 0x660
			 0x67476850, /// 0x664
			 0x02a80d53, /// 0x668
			 0x50ba95c2, /// 0x66c
			 0x10f050fb, /// 0x670
			 0xf5bf61d8, /// 0x674
			 0x0b42a59b, /// 0x678
			 0x40473ca6, /// 0x67c
			 0xa4bb85a5, /// 0x680
			 0x7a2b4378, /// 0x684
			 0x58f30d54, /// 0x688
			 0x7a725a63, /// 0x68c
			 0xbf5543ef, /// 0x690
			 0x1a089111, /// 0x694
			 0xf8201428, /// 0x698
			 0xf445a357, /// 0x69c
			 0x6f6c0e12, /// 0x6a0
			 0xfe44787e, /// 0x6a4
			 0x31a2b2ef, /// 0x6a8
			 0x2867b35c, /// 0x6ac
			 0x86cdc91a, /// 0x6b0
			 0x3ac6314b, /// 0x6b4
			 0x46c08a60, /// 0x6b8
			 0xf8ee2cf2, /// 0x6bc
			 0xb30c2d9d, /// 0x6c0
			 0xa0ec241e, /// 0x6c4
			 0xfb76fe9f, /// 0x6c8
			 0xd90f81da, /// 0x6cc
			 0xf20a4a70, /// 0x6d0
			 0x156fd90d, /// 0x6d4
			 0x28b8172b, /// 0x6d8
			 0xd1a587fa, /// 0x6dc
			 0xb77babfd, /// 0x6e0
			 0xbf0c0ef0, /// 0x6e4
			 0xecc7ffa3, /// 0x6e8
			 0x502b0eb7, /// 0x6ec
			 0xf9c26707, /// 0x6f0
			 0x64659acc, /// 0x6f4
			 0x70925448, /// 0x6f8
			 0xdbc19d53, /// 0x6fc
			 0x2fbecd02, /// 0x700
			 0x45df252d, /// 0x704
			 0x0ad23e51, /// 0x708
			 0xeca41eb9, /// 0x70c
			 0xbb7891a8, /// 0x710
			 0xde66b80a, /// 0x714
			 0x24ad011d, /// 0x718
			 0xfecd6d3c, /// 0x71c
			 0x29233a2e, /// 0x720
			 0x65f2ca7f, /// 0x724
			 0xee8b5b93, /// 0x728
			 0x1136e221, /// 0x72c
			 0x14f72e78, /// 0x730
			 0xc6bb27a5, /// 0x734
			 0xc1d80f44, /// 0x738
			 0xa98938cc, /// 0x73c
			 0xb8c3e498, /// 0x740
			 0x56deedf2, /// 0x744
			 0x8941d7d0, /// 0x748
			 0x45dc69dd, /// 0x74c
			 0xe0b6aa1f, /// 0x750
			 0xdabe3ea1, /// 0x754
			 0xc8ea8553, /// 0x758
			 0xcb1d46e2, /// 0x75c
			 0x42ccf966, /// 0x760
			 0xf8a28fc9, /// 0x764
			 0x264a0f2f, /// 0x768
			 0xd70c4310, /// 0x76c
			 0xea478309, /// 0x770
			 0x02d4b8de, /// 0x774
			 0x49843c2e, /// 0x778
			 0xbd6214e8, /// 0x77c
			 0x975a6943, /// 0x780
			 0xa8df7dcd, /// 0x784
			 0x741d8ad0, /// 0x788
			 0x8f1369fb, /// 0x78c
			 0x108b95f2, /// 0x790
			 0xd76085bd, /// 0x794
			 0xc44bdaa9, /// 0x798
			 0x7704605b, /// 0x79c
			 0xec691af3, /// 0x7a0
			 0x060bfea8, /// 0x7a4
			 0x9f82d562, /// 0x7a8
			 0x4e47acee, /// 0x7ac
			 0xcb156043, /// 0x7b0
			 0x94fa2f74, /// 0x7b4
			 0x2a16da1c, /// 0x7b8
			 0xf8e22b82, /// 0x7bc
			 0x9afad67f, /// 0x7c0
			 0x61914fae, /// 0x7c4
			 0x5016aa72, /// 0x7c8
			 0xe1e52a5a, /// 0x7cc
			 0x669a277f, /// 0x7d0
			 0x203fdf98, /// 0x7d4
			 0x04a56f2b, /// 0x7d8
			 0xa325dd5f, /// 0x7dc
			 0x66464e00, /// 0x7e0
			 0x0b467862, /// 0x7e4
			 0x285dfa31, /// 0x7e8
			 0x43abcaad, /// 0x7ec
			 0xcecc8785, /// 0x7f0
			 0x1a641d43, /// 0x7f4
			 0xad84e77c, /// 0x7f8
			 0xbf074b89, /// 0x7fc
			 0x1a4342ea, /// 0x800
			 0xd77b01d9, /// 0x804
			 0x1276ddf6, /// 0x808
			 0xbfb44bea, /// 0x80c
			 0x12b4bf19, /// 0x810
			 0xbf8c3669, /// 0x814
			 0x7c8cf295, /// 0x818
			 0xdf71850d, /// 0x81c
			 0x5e95bcb3, /// 0x820
			 0x3d681122, /// 0x824
			 0x3f49d2b9, /// 0x828
			 0x0146ba0b, /// 0x82c
			 0x78f95338, /// 0x830
			 0x21fcf7e9, /// 0x834
			 0xb0c7e71f, /// 0x838
			 0xfe0bb6a3, /// 0x83c
			 0xc3d73af6, /// 0x840
			 0x72dac3ad, /// 0x844
			 0x4d298662, /// 0x848
			 0x2637c537, /// 0x84c
			 0x1637ac5f, /// 0x850
			 0x26da9e74, /// 0x854
			 0x3e531955, /// 0x858
			 0xcb9e246e, /// 0x85c
			 0xaf85cdab, /// 0x860
			 0x27d5ba26, /// 0x864
			 0x3dabe988, /// 0x868
			 0x6155d707, /// 0x86c
			 0xaae2efd6, /// 0x870
			 0xbb240657, /// 0x874
			 0xf91a66d7, /// 0x878
			 0x94a5a6d7, /// 0x87c
			 0x691b838c, /// 0x880
			 0x21dd19f5, /// 0x884
			 0x611a79b1, /// 0x888
			 0x81eb7728, /// 0x88c
			 0xdd048e6d, /// 0x890
			 0x6773e764, /// 0x894
			 0x48684a56, /// 0x898
			 0x90165555, /// 0x89c
			 0x7e079a42, /// 0x8a0
			 0x26c7754c, /// 0x8a4
			 0x33f8da04, /// 0x8a8
			 0xff93aaab, /// 0x8ac
			 0xdc182ddf, /// 0x8b0
			 0xf0636def, /// 0x8b4
			 0x70d51c0b, /// 0x8b8
			 0xf3769fd1, /// 0x8bc
			 0x77ade64b, /// 0x8c0
			 0x7768d891, /// 0x8c4
			 0x88ac4faa, /// 0x8c8
			 0x29b11cce, /// 0x8cc
			 0xa19b9c15, /// 0x8d0
			 0xf11c4f00, /// 0x8d4
			 0xf09c35a1, /// 0x8d8
			 0x3f18acd1, /// 0x8dc
			 0x24b10a80, /// 0x8e0
			 0x594d9727, /// 0x8e4
			 0x632c0961, /// 0x8e8
			 0x570b52ed, /// 0x8ec
			 0xa7d8e152, /// 0x8f0
			 0xfcd08e93, /// 0x8f4
			 0x8a4ed015, /// 0x8f8
			 0x9636d61c, /// 0x8fc
			 0xec05a78e, /// 0x900
			 0x569f3e6a, /// 0x904
			 0x52f39921, /// 0x908
			 0x3628e87b, /// 0x90c
			 0xa23890f0, /// 0x910
			 0x956e3a7a, /// 0x914
			 0x7adf46a0, /// 0x918
			 0x399077a4, /// 0x91c
			 0x5a9f4658, /// 0x920
			 0x05846ca2, /// 0x924
			 0x1b9d08ff, /// 0x928
			 0xb52c893d, /// 0x92c
			 0x6fc6b6be, /// 0x930
			 0x0ce1d3dc, /// 0x934
			 0x54857e0c, /// 0x938
			 0xddbc7754, /// 0x93c
			 0x9e053760, /// 0x940
			 0x1f6ecb13, /// 0x944
			 0xf8c0d52f, /// 0x948
			 0xebd7fb44, /// 0x94c
			 0x912cd6ea, /// 0x950
			 0xdefb3372, /// 0x954
			 0xe47aa4a7, /// 0x958
			 0x86473a3d, /// 0x95c
			 0x04728819, /// 0x960
			 0x4fd1778b, /// 0x964
			 0xe05c2e3b, /// 0x968
			 0x9c12be68, /// 0x96c
			 0x8d64095b, /// 0x970
			 0x56ba38a1, /// 0x974
			 0x26e064f5, /// 0x978
			 0xf559c9f3, /// 0x97c
			 0x146f37de, /// 0x980
			 0xb69bf87c, /// 0x984
			 0x3bb3994d, /// 0x988
			 0x03c594a3, /// 0x98c
			 0x41279160, /// 0x990
			 0xaf1c56e6, /// 0x994
			 0xa9bb345d, /// 0x998
			 0x0aed5191, /// 0x99c
			 0x011f28b6, /// 0x9a0
			 0x6da40c0f, /// 0x9a4
			 0x6324d223, /// 0x9a8
			 0x08e711c5, /// 0x9ac
			 0x5ab4c7b2, /// 0x9b0
			 0x982735c5, /// 0x9b4
			 0x9156a559, /// 0x9b8
			 0x369da50c, /// 0x9bc
			 0x97e4e9e4, /// 0x9c0
			 0x6b2dad01, /// 0x9c4
			 0x8963b612, /// 0x9c8
			 0x1282537e, /// 0x9cc
			 0x2f483c58, /// 0x9d0
			 0x0aaa8d20, /// 0x9d4
			 0xf9f88496, /// 0x9d8
			 0x798db8e6, /// 0x9dc
			 0xb1b8856f, /// 0x9e0
			 0xe6a852ef, /// 0x9e4
			 0x8d2259be, /// 0x9e8
			 0xd3544119, /// 0x9ec
			 0x0000adee, /// 0x9f0
			 0x82badd45, /// 0x9f4
			 0x672004ea, /// 0x9f8
			 0xf0643fb0, /// 0x9fc
			 0x58205ea3, /// 0xa00
			 0x26d1558f, /// 0xa04
			 0x62d5e3b9, /// 0xa08
			 0xadd6a612, /// 0xa0c
			 0xbffff2ee, /// 0xa10
			 0xbff757da, /// 0xa14
			 0x52ded2aa, /// 0xa18
			 0x31197feb, /// 0xa1c
			 0x039ce92f, /// 0xa20
			 0xa2b90708, /// 0xa24
			 0x2078535f, /// 0xa28
			 0x3d1c40c0, /// 0xa2c
			 0x967ada3d, /// 0xa30
			 0xbe3656ff, /// 0xa34
			 0x484cf873, /// 0xa38
			 0xb1ebb619, /// 0xa3c
			 0xb8fa6ed2, /// 0xa40
			 0x5a3497da, /// 0xa44
			 0x78b28b0c, /// 0xa48
			 0xa3d965b5, /// 0xa4c
			 0x0c3c5fb1, /// 0xa50
			 0x26c8b134, /// 0xa54
			 0xf0d34591, /// 0xa58
			 0x0fd28c11, /// 0xa5c
			 0xf7305245, /// 0xa60
			 0x0345bade, /// 0xa64
			 0x9f26c4c3, /// 0xa68
			 0xcf5da268, /// 0xa6c
			 0x0b6b4a54, /// 0xa70
			 0x5b018010, /// 0xa74
			 0x43f9dac7, /// 0xa78
			 0x3f9a2f87, /// 0xa7c
			 0xa004dee1, /// 0xa80
			 0x6376e8ce, /// 0xa84
			 0x5bea232b, /// 0xa88
			 0xdbf14728, /// 0xa8c
			 0x1d979dc6, /// 0xa90
			 0x33de29e2, /// 0xa94
			 0xed88de70, /// 0xa98
			 0xda56e1a4, /// 0xa9c
			 0x28d6366c, /// 0xaa0
			 0x7c95ad19, /// 0xaa4
			 0xa1ac17f6, /// 0xaa8
			 0xd1aa6354, /// 0xaac
			 0x0ed7aea3, /// 0xab0
			 0xdc41c86c, /// 0xab4
			 0x37cd5884, /// 0xab8
			 0x3901a137, /// 0xabc
			 0x6aa39eac, /// 0xac0
			 0xb584a9b1, /// 0xac4
			 0x6cd23745, /// 0xac8
			 0xc63f8fd8, /// 0xacc
			 0x0a716fd5, /// 0xad0
			 0xad49d7f8, /// 0xad4
			 0x2bcf6fce, /// 0xad8
			 0x40915048, /// 0xadc
			 0x0859f51d, /// 0xae0
			 0x0b4c1531, /// 0xae4
			 0x343c1786, /// 0xae8
			 0xf10a4747, /// 0xaec
			 0x1d154236, /// 0xaf0
			 0xc793231a, /// 0xaf4
			 0x7ac4da31, /// 0xaf8
			 0xb1d18a37, /// 0xafc
			 0x6694929a, /// 0xb00
			 0xfb2442ac, /// 0xb04
			 0x5107cf0c, /// 0xb08
			 0xa3713066, /// 0xb0c
			 0xc7dc1a01, /// 0xb10
			 0x95ca7ecc, /// 0xb14
			 0xc678e156, /// 0xb18
			 0xf4c77d9c, /// 0xb1c
			 0x5f49836b, /// 0xb20
			 0x1476ddbd, /// 0xb24
			 0x68dbee7f, /// 0xb28
			 0x2e828f4d, /// 0xb2c
			 0x63f91dce, /// 0xb30
			 0x1e55d7fb, /// 0xb34
			 0xc191774c, /// 0xb38
			 0x235734fe, /// 0xb3c
			 0x7b2c3cac, /// 0xb40
			 0xeff02caa, /// 0xb44
			 0x4727d688, /// 0xb48
			 0x6621213e, /// 0xb4c
			 0x59c2eb43, /// 0xb50
			 0xa0583712, /// 0xb54
			 0x06c21052, /// 0xb58
			 0xca1fb68f, /// 0xb5c
			 0x37a1bd4f, /// 0xb60
			 0xc869c0a6, /// 0xb64
			 0x1fc4d3e7, /// 0xb68
			 0x444c9d2d, /// 0xb6c
			 0xfc9a9342, /// 0xb70
			 0xfb1f5afd, /// 0xb74
			 0x484d17a7, /// 0xb78
			 0x27487cc1, /// 0xb7c
			 0x37e0c15c, /// 0xb80
			 0x03ebfb0e, /// 0xb84
			 0x9d0bf6f9, /// 0xb88
			 0x1aa65c10, /// 0xb8c
			 0xf6b4024f, /// 0xb90
			 0x80e0cac1, /// 0xb94
			 0x16649ab5, /// 0xb98
			 0x50c72f06, /// 0xb9c
			 0x8a5e1928, /// 0xba0
			 0x1f6cccd2, /// 0xba4
			 0x65df0057, /// 0xba8
			 0x77655276, /// 0xbac
			 0x7f6f182c, /// 0xbb0
			 0x2e9737f3, /// 0xbb4
			 0xec44429e, /// 0xbb8
			 0xb8e9cab6, /// 0xbbc
			 0x3ec29bee, /// 0xbc0
			 0xb030b282, /// 0xbc4
			 0x1cc465f3, /// 0xbc8
			 0xf2ec4e7d, /// 0xbcc
			 0x7ef3854d, /// 0xbd0
			 0x89056ec9, /// 0xbd4
			 0x74dc08f8, /// 0xbd8
			 0x6d3e4137, /// 0xbdc
			 0x3f6c8120, /// 0xbe0
			 0xad4d757d, /// 0xbe4
			 0x5684d3ab, /// 0xbe8
			 0xd005e0a0, /// 0xbec
			 0xaf8a635c, /// 0xbf0
			 0x1a7c4654, /// 0xbf4
			 0x6db27026, /// 0xbf8
			 0x8925637d, /// 0xbfc
			 0x5252af71, /// 0xc00
			 0xb8fd57a8, /// 0xc04
			 0x50ded879, /// 0xc08
			 0x5d5efaff, /// 0xc0c
			 0x4b672125, /// 0xc10
			 0xc1c11910, /// 0xc14
			 0x44863fd1, /// 0xc18
			 0x043a9806, /// 0xc1c
			 0x01c1b186, /// 0xc20
			 0xbc096b87, /// 0xc24
			 0xd83dc958, /// 0xc28
			 0x9cde1799, /// 0xc2c
			 0x84fd798f, /// 0xc30
			 0xe77a7395, /// 0xc34
			 0xcfa64ac2, /// 0xc38
			 0x48cf3ec1, /// 0xc3c
			 0x5c5e088f, /// 0xc40
			 0x8589b914, /// 0xc44
			 0x649181ed, /// 0xc48
			 0xa6d6beff, /// 0xc4c
			 0x7773bc8a, /// 0xc50
			 0x047dc0d2, /// 0xc54
			 0x1a23969b, /// 0xc58
			 0x1da59d17, /// 0xc5c
			 0x15eed03e, /// 0xc60
			 0x9348b8e7, /// 0xc64
			 0x2b349a02, /// 0xc68
			 0x8cb4656d, /// 0xc6c
			 0x16202faa, /// 0xc70
			 0x781ae76d, /// 0xc74
			 0xfb45b65f, /// 0xc78
			 0x678bc9ac, /// 0xc7c
			 0x614ac736, /// 0xc80
			 0x62743e98, /// 0xc84
			 0x81914abd, /// 0xc88
			 0x4e994837, /// 0xc8c
			 0xbfdd6ee4, /// 0xc90
			 0x2440ce3d, /// 0xc94
			 0x23001668, /// 0xc98
			 0x6d44893f, /// 0xc9c
			 0xf1824858, /// 0xca0
			 0xcd9f65b7, /// 0xca4
			 0x7ca15323, /// 0xca8
			 0x4480ffea, /// 0xcac
			 0x6b0bcc79, /// 0xcb0
			 0x8c562b9f, /// 0xcb4
			 0x1a50f537, /// 0xcb8
			 0xad001eb2, /// 0xcbc
			 0xaf7d6e90, /// 0xcc0
			 0x6f93407a, /// 0xcc4
			 0x312e0f8a, /// 0xcc8
			 0x28309602, /// 0xccc
			 0x7466a40d, /// 0xcd0
			 0xde281c21, /// 0xcd4
			 0x6b1305ed, /// 0xcd8
			 0x85617310, /// 0xcdc
			 0x8a515bfd, /// 0xce0
			 0x87065d05, /// 0xce4
			 0x668b3283, /// 0xce8
			 0xb7dc00a2, /// 0xcec
			 0x8d8ef50b, /// 0xcf0
			 0xc96f3468, /// 0xcf4
			 0xa5c75e68, /// 0xcf8
			 0xb653d299, /// 0xcfc
			 0xd66e73e5, /// 0xd00
			 0x137bd122, /// 0xd04
			 0x3f5154c9, /// 0xd08
			 0x462e76f0, /// 0xd0c
			 0x652abcc5, /// 0xd10
			 0xc3bbd101, /// 0xd14
			 0x4df4e920, /// 0xd18
			 0x859a8f43, /// 0xd1c
			 0xa5cbb91d, /// 0xd20
			 0x4b92a39c, /// 0xd24
			 0xd6c7f3c4, /// 0xd28
			 0x35fd58da, /// 0xd2c
			 0x9867cb47, /// 0xd30
			 0xce03c8ba, /// 0xd34
			 0x64363cc7, /// 0xd38
			 0x1613f184, /// 0xd3c
			 0x7606bd7c, /// 0xd40
			 0x3ecbbd1f, /// 0xd44
			 0x14ae53b8, /// 0xd48
			 0xd7e5e045, /// 0xd4c
			 0x7c70f301, /// 0xd50
			 0x38b4537f, /// 0xd54
			 0x4490171d, /// 0xd58
			 0xb565fdc0, /// 0xd5c
			 0x65e622a8, /// 0xd60
			 0x3effc1df, /// 0xd64
			 0x002c45fd, /// 0xd68
			 0x3c37d6f1, /// 0xd6c
			 0xe520a6e3, /// 0xd70
			 0x3ac73002, /// 0xd74
			 0x630a6828, /// 0xd78
			 0x6f256895, /// 0xd7c
			 0xc3c00c29, /// 0xd80
			 0x4b048800, /// 0xd84
			 0x5b1a11ba, /// 0xd88
			 0x57f5378b, /// 0xd8c
			 0xaa80b9be, /// 0xd90
			 0xbec1b8e8, /// 0xd94
			 0xdb8a4dea, /// 0xd98
			 0xed820882, /// 0xd9c
			 0x2c121bed, /// 0xda0
			 0x3007ae09, /// 0xda4
			 0xfc0a4fcb, /// 0xda8
			 0xd392721d, /// 0xdac
			 0xbd7495aa, /// 0xdb0
			 0xe9cd1a8f, /// 0xdb4
			 0xa876f5e6, /// 0xdb8
			 0xfc15be56, /// 0xdbc
			 0x911e95f7, /// 0xdc0
			 0x4b2ea822, /// 0xdc4
			 0xc067c2bf, /// 0xdc8
			 0xcdb0e591, /// 0xdcc
			 0x0f42844d, /// 0xdd0
			 0x3b060cae, /// 0xdd4
			 0x411c4297, /// 0xdd8
			 0x46900a98, /// 0xddc
			 0xd1d276ef, /// 0xde0
			 0xf2410c99, /// 0xde4
			 0xfccb919d, /// 0xde8
			 0x6efb68a9, /// 0xdec
			 0x0c19ec24, /// 0xdf0
			 0xda906b19, /// 0xdf4
			 0x7a722034, /// 0xdf8
			 0x983c87dd, /// 0xdfc
			 0x57850977, /// 0xe00
			 0x072177d0, /// 0xe04
			 0x04d53d62, /// 0xe08
			 0xcb79cce5, /// 0xe0c
			 0x72de0685, /// 0xe10
			 0xa82d079a, /// 0xe14
			 0x2cba884d, /// 0xe18
			 0x9d3c1134, /// 0xe1c
			 0x787b6dff, /// 0xe20
			 0x0c95750c, /// 0xe24
			 0xeff9683a, /// 0xe28
			 0x06d7604f, /// 0xe2c
			 0x6fc67d72, /// 0xe30
			 0x651c2f35, /// 0xe34
			 0xf90cc3cf, /// 0xe38
			 0x3404373e, /// 0xe3c
			 0xa5ecc543, /// 0xe40
			 0x8d2617a7, /// 0xe44
			 0x2b006b46, /// 0xe48
			 0x69e1b2b2, /// 0xe4c
			 0xdda5de9c, /// 0xe50
			 0x6ceaa279, /// 0xe54
			 0xa223bd49, /// 0xe58
			 0x8aba9eca, /// 0xe5c
			 0x0d8732d2, /// 0xe60
			 0x43e12cf9, /// 0xe64
			 0xacb2e79a, /// 0xe68
			 0x1f57a598, /// 0xe6c
			 0x55e22bce, /// 0xe70
			 0x8c462564, /// 0xe74
			 0x89111881, /// 0xe78
			 0xafed1867, /// 0xe7c
			 0x1ae3fb5b, /// 0xe80
			 0x20a84e92, /// 0xe84
			 0x1fb3bd07, /// 0xe88
			 0x91bbfd85, /// 0xe8c
			 0xc8d64b0b, /// 0xe90
			 0x27858c5b, /// 0xe94
			 0xb7c8acab, /// 0xe98
			 0xb2b30a93, /// 0xe9c
			 0x2b032cf5, /// 0xea0
			 0x256508fa, /// 0xea4
			 0xa7284be6, /// 0xea8
			 0xabf666e7, /// 0xeac
			 0x3a2c504e, /// 0xeb0
			 0x709ad7ad, /// 0xeb4
			 0x0913708c, /// 0xeb8
			 0x2a69043c, /// 0xebc
			 0x0c5790a0, /// 0xec0
			 0xb167e4ae, /// 0xec4
			 0x14805e33, /// 0xec8
			 0x17f395a6, /// 0xecc
			 0xb6c26ef8, /// 0xed0
			 0xcf7b5493, /// 0xed4
			 0xf99ef5d4, /// 0xed8
			 0xd8d03642, /// 0xedc
			 0x76a6580b, /// 0xee0
			 0xc98660ba, /// 0xee4
			 0x479fa00d, /// 0xee8
			 0x26ee5e4c, /// 0xeec
			 0x95c734e5, /// 0xef0
			 0xd7af50bd, /// 0xef4
			 0x39968db9, /// 0xef8
			 0xba0e0658, /// 0xefc
			 0xfb4cffb8, /// 0xf00
			 0x6396ff5b, /// 0xf04
			 0x1dd39b47, /// 0xf08
			 0x1ce4df11, /// 0xf0c
			 0x2084ec09, /// 0xf10
			 0x1f3b7f95, /// 0xf14
			 0xab7d38c6, /// 0xf18
			 0x13ece356, /// 0xf1c
			 0x648d526c, /// 0xf20
			 0x3096a90e, /// 0xf24
			 0x92e817a8, /// 0xf28
			 0x27d0113b, /// 0xf2c
			 0x0216f60d, /// 0xf30
			 0x00a8332b, /// 0xf34
			 0x5a4d38cd, /// 0xf38
			 0x59b4d9f0, /// 0xf3c
			 0xf32cedf4, /// 0xf40
			 0x791d40e3, /// 0xf44
			 0x19eb870a, /// 0xf48
			 0xc8a5576d, /// 0xf4c
			 0x11e54a24, /// 0xf50
			 0xb6faa018, /// 0xf54
			 0xa023344f, /// 0xf58
			 0xd834cfdf, /// 0xf5c
			 0xb2df2daf, /// 0xf60
			 0x12c4bf0f, /// 0xf64
			 0x89c1be48, /// 0xf68
			 0x0284fa3d, /// 0xf6c
			 0xc0b32b39, /// 0xf70
			 0xc4ed4f9c, /// 0xf74
			 0x2ee5d97f, /// 0xf78
			 0xc122df2e, /// 0xf7c
			 0xee4aaf7e, /// 0xf80
			 0x9d4f1a71, /// 0xf84
			 0xd7f516a9, /// 0xf88
			 0x2f7af85d, /// 0xf8c
			 0xba4121f9, /// 0xf90
			 0x4406506f, /// 0xf94
			 0x0e37882c, /// 0xf98
			 0x8ec1ebc8, /// 0xf9c
			 0x80b201ed, /// 0xfa0
			 0x941dd8fc, /// 0xfa4
			 0xe4777dc8, /// 0xfa8
			 0x3c28d552, /// 0xfac
			 0x69b8172b, /// 0xfb0
			 0xdb8d32eb, /// 0xfb4
			 0x83286e4d, /// 0xfb8
			 0x292be2f8, /// 0xfbc
			 0x37ccd0dc, /// 0xfc0
			 0xe961df02, /// 0xfc4
			 0x4caadc55, /// 0xfc8
			 0x25617c06, /// 0xfcc
			 0xe1931bd0, /// 0xfd0
			 0xc384992a, /// 0xfd4
			 0x9231c48c, /// 0xfd8
			 0x092052cb, /// 0xfdc
			 0x72103ee7, /// 0xfe0
			 0xe72c522c, /// 0xfe4
			 0x0aa80fb7, /// 0xfe8
			 0xe5eda46d, /// 0xfec
			 0x444150ac, /// 0xff0
			 0x71d19e4b, /// 0xff4
			 0x85fec972, /// 0xff8
			 0xc890a73d /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x644974c5, /// 0x0
			 0x0147097b, /// 0x4
			 0x9edbdc4c, /// 0x8
			 0xf34ebdd1, /// 0xc
			 0xc482cf64, /// 0x10
			 0x8bd776fd, /// 0x14
			 0xce89116b, /// 0x18
			 0x3654985b, /// 0x1c
			 0x706d8c40, /// 0x20
			 0x31ab760b, /// 0x24
			 0xb574e45f, /// 0x28
			 0x0abdfd61, /// 0x2c
			 0x66af2077, /// 0x30
			 0x3e7cf28c, /// 0x34
			 0x800ea4a4, /// 0x38
			 0xe09f872e, /// 0x3c
			 0x87382922, /// 0x40
			 0x6ca57f33, /// 0x44
			 0x0865fc98, /// 0x48
			 0xd030d57e, /// 0x4c
			 0x36c0481f, /// 0x50
			 0xffdedd46, /// 0x54
			 0x868b711c, /// 0x58
			 0x46bf69b6, /// 0x5c
			 0x87ded1b0, /// 0x60
			 0xadca124a, /// 0x64
			 0x8251b470, /// 0x68
			 0x5748b813, /// 0x6c
			 0xb2084519, /// 0x70
			 0x025e4773, /// 0x74
			 0x05a41ef8, /// 0x78
			 0x2a25dfef, /// 0x7c
			 0x8cd2e08e, /// 0x80
			 0x6020ffe7, /// 0x84
			 0x1705fc06, /// 0x88
			 0xce1ad254, /// 0x8c
			 0xb22c54c9, /// 0x90
			 0xe03eac93, /// 0x94
			 0x32a65526, /// 0x98
			 0xf7b6965d, /// 0x9c
			 0x4205a36c, /// 0xa0
			 0x48de67fc, /// 0xa4
			 0x3157c483, /// 0xa8
			 0xa140373d, /// 0xac
			 0x88966594, /// 0xb0
			 0xa6d407bb, /// 0xb4
			 0x4211d0c1, /// 0xb8
			 0x4011d75b, /// 0xbc
			 0xe554117f, /// 0xc0
			 0x45b1ca34, /// 0xc4
			 0xee5ff170, /// 0xc8
			 0x8fec7d85, /// 0xcc
			 0xad41f821, /// 0xd0
			 0x818b72d8, /// 0xd4
			 0xa5418388, /// 0xd8
			 0xea7b51b8, /// 0xdc
			 0x19256d80, /// 0xe0
			 0xd9db795b, /// 0xe4
			 0x61d22c76, /// 0xe8
			 0x975595ef, /// 0xec
			 0x036c72fc, /// 0xf0
			 0x113d649f, /// 0xf4
			 0x61db3fb5, /// 0xf8
			 0xf5af66d4, /// 0xfc
			 0xc22366ce, /// 0x100
			 0x9b6da24e, /// 0x104
			 0xaf867c01, /// 0x108
			 0xe1c6ef7d, /// 0x10c
			 0xd3a86e45, /// 0x110
			 0xed95b431, /// 0x114
			 0x668111fa, /// 0x118
			 0x384e4264, /// 0x11c
			 0x9705f9cf, /// 0x120
			 0x89291925, /// 0x124
			 0xf0237d72, /// 0x128
			 0x008488bb, /// 0x12c
			 0x2c18fc80, /// 0x130
			 0xa6958b14, /// 0x134
			 0x86b97fd0, /// 0x138
			 0xceffd5e5, /// 0x13c
			 0x99b34640, /// 0x140
			 0x39f1d856, /// 0x144
			 0xd35c3ccb, /// 0x148
			 0xad7b7b9a, /// 0x14c
			 0x89a7fb2d, /// 0x150
			 0x2528a201, /// 0x154
			 0x5d61fd3b, /// 0x158
			 0x1f4f47cd, /// 0x15c
			 0x0737a4ea, /// 0x160
			 0xe104c606, /// 0x164
			 0x4320caae, /// 0x168
			 0x0464591d, /// 0x16c
			 0xee00ee11, /// 0x170
			 0x4fd373b3, /// 0x174
			 0x8de134f7, /// 0x178
			 0x85f8353a, /// 0x17c
			 0x14c6355c, /// 0x180
			 0x1c770849, /// 0x184
			 0xe0f477bf, /// 0x188
			 0x8739002b, /// 0x18c
			 0xf74e8c4a, /// 0x190
			 0x8a4d63ce, /// 0x194
			 0x68dfb767, /// 0x198
			 0xafe84bb1, /// 0x19c
			 0x056044dd, /// 0x1a0
			 0xcdb18e8a, /// 0x1a4
			 0x4430a40c, /// 0x1a8
			 0x93314495, /// 0x1ac
			 0x3db1e2fe, /// 0x1b0
			 0xa1e64998, /// 0x1b4
			 0xe059ec0b, /// 0x1b8
			 0x6d7f97c7, /// 0x1bc
			 0x55f015a4, /// 0x1c0
			 0x83910c4e, /// 0x1c4
			 0x9829db83, /// 0x1c8
			 0xf98bc388, /// 0x1cc
			 0xc042c197, /// 0x1d0
			 0xeb721df7, /// 0x1d4
			 0x4055936f, /// 0x1d8
			 0x24a2358b, /// 0x1dc
			 0x8aa9f9da, /// 0x1e0
			 0x7264bbe2, /// 0x1e4
			 0x6b2d1c02, /// 0x1e8
			 0x6e78bc79, /// 0x1ec
			 0x80c01531, /// 0x1f0
			 0xa04582cf, /// 0x1f4
			 0x9c649d3c, /// 0x1f8
			 0x5e350859, /// 0x1fc
			 0xb84726e3, /// 0x200
			 0xd28f586b, /// 0x204
			 0xc7538924, /// 0x208
			 0x2efc31d0, /// 0x20c
			 0x307f2228, /// 0x210
			 0x07db947c, /// 0x214
			 0x5b17357b, /// 0x218
			 0xda8afd99, /// 0x21c
			 0xaafed95c, /// 0x220
			 0x969e0d4f, /// 0x224
			 0xee99a640, /// 0x228
			 0xc7b34b15, /// 0x22c
			 0x2c32e189, /// 0x230
			 0x6c7af635, /// 0x234
			 0xfe1bba5f, /// 0x238
			 0xa9d0e237, /// 0x23c
			 0xdf4d191e, /// 0x240
			 0x0f48eab1, /// 0x244
			 0x3ac3f380, /// 0x248
			 0x89aa887e, /// 0x24c
			 0x29766dbd, /// 0x250
			 0xddbfcfb7, /// 0x254
			 0x865fd289, /// 0x258
			 0x8a4f3370, /// 0x25c
			 0x90f00219, /// 0x260
			 0x0505d6c2, /// 0x264
			 0x25f79ff6, /// 0x268
			 0xfb03a957, /// 0x26c
			 0x5d664f86, /// 0x270
			 0x0d7a09fc, /// 0x274
			 0xd253adf7, /// 0x278
			 0x71612c2e, /// 0x27c
			 0x6f146b4e, /// 0x280
			 0x14b5ed56, /// 0x284
			 0x20823e88, /// 0x288
			 0xbd6910d6, /// 0x28c
			 0x1125a80f, /// 0x290
			 0x72a91f03, /// 0x294
			 0xe425963b, /// 0x298
			 0x184126eb, /// 0x29c
			 0xc0c54179, /// 0x2a0
			 0xaef222fb, /// 0x2a4
			 0x69bf0a84, /// 0x2a8
			 0xba38f222, /// 0x2ac
			 0x5f059988, /// 0x2b0
			 0x48cda9fb, /// 0x2b4
			 0xdf644f83, /// 0x2b8
			 0x5dcfb495, /// 0x2bc
			 0xf19496d5, /// 0x2c0
			 0x84a28a6e, /// 0x2c4
			 0x5a4cb3e9, /// 0x2c8
			 0xe73426a3, /// 0x2cc
			 0xb7843ce0, /// 0x2d0
			 0xc5a8849d, /// 0x2d4
			 0x30d7507c, /// 0x2d8
			 0xcaa46b69, /// 0x2dc
			 0xb80160a0, /// 0x2e0
			 0xde0c58fd, /// 0x2e4
			 0xecb51c9d, /// 0x2e8
			 0x20d851a1, /// 0x2ec
			 0x6e2004df, /// 0x2f0
			 0x8f727f74, /// 0x2f4
			 0x4f81a797, /// 0x2f8
			 0x8ddf928b, /// 0x2fc
			 0x8f5c2bf5, /// 0x300
			 0x26846a3c, /// 0x304
			 0xb7a47fd0, /// 0x308
			 0x86d38fb6, /// 0x30c
			 0x4ed2f432, /// 0x310
			 0x973057b5, /// 0x314
			 0xa1c0a3ec, /// 0x318
			 0x4b9ea25e, /// 0x31c
			 0xc4842892, /// 0x320
			 0xac256db0, /// 0x324
			 0x240b61c8, /// 0x328
			 0xf6c4fb7d, /// 0x32c
			 0xc1376bea, /// 0x330
			 0xd3522886, /// 0x334
			 0x8677ee8d, /// 0x338
			 0xdb8bd1e9, /// 0x33c
			 0x5aab9770, /// 0x340
			 0x519ab2f9, /// 0x344
			 0xd2d0836a, /// 0x348
			 0x30e7bd05, /// 0x34c
			 0x28b9adbe, /// 0x350
			 0xefd8284b, /// 0x354
			 0x19e3fbd6, /// 0x358
			 0x0d41c0e8, /// 0x35c
			 0x8ffe1cc1, /// 0x360
			 0x8474b430, /// 0x364
			 0x7b1bf0cb, /// 0x368
			 0xc53ff7f9, /// 0x36c
			 0xc18775ab, /// 0x370
			 0x027bfee6, /// 0x374
			 0x1053bc7d, /// 0x378
			 0x1e1262bb, /// 0x37c
			 0xe7b09ffc, /// 0x380
			 0xf395a1b7, /// 0x384
			 0x367a174f, /// 0x388
			 0x236f1dff, /// 0x38c
			 0x053b7ce0, /// 0x390
			 0xa7a8a880, /// 0x394
			 0x96e0150e, /// 0x398
			 0xb83326a0, /// 0x39c
			 0xc4251de3, /// 0x3a0
			 0x98fafae4, /// 0x3a4
			 0xb1b23717, /// 0x3a8
			 0xfab6125e, /// 0x3ac
			 0x31da898c, /// 0x3b0
			 0x04d261fe, /// 0x3b4
			 0xb7ca278d, /// 0x3b8
			 0xb054f39d, /// 0x3bc
			 0x890848c4, /// 0x3c0
			 0xd0cd255e, /// 0x3c4
			 0xd513a83b, /// 0x3c8
			 0x11138020, /// 0x3cc
			 0x9e489e4e, /// 0x3d0
			 0xf5fcb75c, /// 0x3d4
			 0x5b10423a, /// 0x3d8
			 0x302fde24, /// 0x3dc
			 0x864418d6, /// 0x3e0
			 0xd72c392e, /// 0x3e4
			 0xcf0b5489, /// 0x3e8
			 0xea7a7a1f, /// 0x3ec
			 0xf92801a6, /// 0x3f0
			 0xa76521c8, /// 0x3f4
			 0x535f684a, /// 0x3f8
			 0x855cec98, /// 0x3fc
			 0x51ef5b2a, /// 0x400
			 0x7175fb05, /// 0x404
			 0x6d74d474, /// 0x408
			 0x9c8f885d, /// 0x40c
			 0x4af5b944, /// 0x410
			 0xe5b09c5b, /// 0x414
			 0x76b7b099, /// 0x418
			 0x7c894e7e, /// 0x41c
			 0x4ff2f839, /// 0x420
			 0xe4aab94f, /// 0x424
			 0x20ed6e27, /// 0x428
			 0xfd388667, /// 0x42c
			 0x5a00242d, /// 0x430
			 0xfdd097af, /// 0x434
			 0xa1c5d764, /// 0x438
			 0x2dc550fc, /// 0x43c
			 0x6e0eafbf, /// 0x440
			 0xe74a3d6f, /// 0x444
			 0x73e70fce, /// 0x448
			 0x66047208, /// 0x44c
			 0xfb1f8e49, /// 0x450
			 0x2f210e13, /// 0x454
			 0x95b2b62f, /// 0x458
			 0x63ed1923, /// 0x45c
			 0xf83c0520, /// 0x460
			 0x887a8b25, /// 0x464
			 0x2c711e1f, /// 0x468
			 0x09b670dd, /// 0x46c
			 0xe8efcdf6, /// 0x470
			 0xd2cd0f8c, /// 0x474
			 0xd5eb2ea7, /// 0x478
			 0xab8930a2, /// 0x47c
			 0x565f74b8, /// 0x480
			 0x2e2dea20, /// 0x484
			 0x8a058422, /// 0x488
			 0x8135bde6, /// 0x48c
			 0x889ae4a9, /// 0x490
			 0xafc73a31, /// 0x494
			 0xa732a029, /// 0x498
			 0x7380abf1, /// 0x49c
			 0xcb85de9f, /// 0x4a0
			 0x4bf1bf98, /// 0x4a4
			 0x25f40361, /// 0x4a8
			 0x16552ea6, /// 0x4ac
			 0x76f3691d, /// 0x4b0
			 0x55f74734, /// 0x4b4
			 0xf64b69f2, /// 0x4b8
			 0xb1b9e856, /// 0x4bc
			 0xebe96170, /// 0x4c0
			 0xd9ac8c66, /// 0x4c4
			 0xf51bc9eb, /// 0x4c8
			 0xcbacf0ba, /// 0x4cc
			 0x00d94e5b, /// 0x4d0
			 0xe2917ec6, /// 0x4d4
			 0x21f745c0, /// 0x4d8
			 0x63ee760a, /// 0x4dc
			 0x076b93c3, /// 0x4e0
			 0x19309c41, /// 0x4e4
			 0xb2efbca2, /// 0x4e8
			 0x9826bf35, /// 0x4ec
			 0xe66ec6c6, /// 0x4f0
			 0x16d16629, /// 0x4f4
			 0xe71664f1, /// 0x4f8
			 0x327fa80a, /// 0x4fc
			 0xb7022bd0, /// 0x500
			 0xd8e2a46d, /// 0x504
			 0xef41c1dc, /// 0x508
			 0x8358f7fa, /// 0x50c
			 0x3bdc84ab, /// 0x510
			 0x275e3a09, /// 0x514
			 0xd300e185, /// 0x518
			 0x43b648cc, /// 0x51c
			 0xeea60546, /// 0x520
			 0x446f1604, /// 0x524
			 0x893b3a61, /// 0x528
			 0x5e37cc57, /// 0x52c
			 0x653fa3fa, /// 0x530
			 0xf8816e52, /// 0x534
			 0xea08fb71, /// 0x538
			 0x65583870, /// 0x53c
			 0x844f83f1, /// 0x540
			 0xacce2f01, /// 0x544
			 0xe73ad450, /// 0x548
			 0xfed5ec36, /// 0x54c
			 0x257d62de, /// 0x550
			 0xa6411b3d, /// 0x554
			 0x22c5f5ef, /// 0x558
			 0x712aa8fa, /// 0x55c
			 0x024f2372, /// 0x560
			 0x16a3d053, /// 0x564
			 0x56ca3f3c, /// 0x568
			 0xca331867, /// 0x56c
			 0x37d2477e, /// 0x570
			 0x497a215f, /// 0x574
			 0xa00499c8, /// 0x578
			 0xff2e77e6, /// 0x57c
			 0x95fe7415, /// 0x580
			 0x807d3e4d, /// 0x584
			 0x0d20a3f0, /// 0x588
			 0x66c2d6c6, /// 0x58c
			 0x45e9c22b, /// 0x590
			 0x9acb4c7d, /// 0x594
			 0x4b06dd33, /// 0x598
			 0xd5361b38, /// 0x59c
			 0x489e98f6, /// 0x5a0
			 0xdb346d5f, /// 0x5a4
			 0x46749171, /// 0x5a8
			 0x92a6e03e, /// 0x5ac
			 0x01035074, /// 0x5b0
			 0x979397c7, /// 0x5b4
			 0x59ee330c, /// 0x5b8
			 0x23aad63f, /// 0x5bc
			 0xc25a917b, /// 0x5c0
			 0xc6182288, /// 0x5c4
			 0xce926a54, /// 0x5c8
			 0xd5d89b05, /// 0x5cc
			 0x9b397967, /// 0x5d0
			 0xc2ec076d, /// 0x5d4
			 0xc095d7a2, /// 0x5d8
			 0x36cfd14d, /// 0x5dc
			 0x5c255469, /// 0x5e0
			 0x332e783f, /// 0x5e4
			 0x5a98d9c8, /// 0x5e8
			 0xe0ea503c, /// 0x5ec
			 0xcf6db9b2, /// 0x5f0
			 0x303b806e, /// 0x5f4
			 0x299b12fa, /// 0x5f8
			 0xc15fdb6a, /// 0x5fc
			 0xac04713c, /// 0x600
			 0x9ef21bcf, /// 0x604
			 0x6389d153, /// 0x608
			 0xa9993aaa, /// 0x60c
			 0x5d0704e4, /// 0x610
			 0x67398726, /// 0x614
			 0x609759f6, /// 0x618
			 0xdcbaba37, /// 0x61c
			 0xae2d536d, /// 0x620
			 0x543dd025, /// 0x624
			 0xde92cf95, /// 0x628
			 0xc92d7344, /// 0x62c
			 0x9cceb8f2, /// 0x630
			 0x9ac31a88, /// 0x634
			 0xab011ff1, /// 0x638
			 0x939aa326, /// 0x63c
			 0xda68088d, /// 0x640
			 0xb42762d1, /// 0x644
			 0x274dd972, /// 0x648
			 0xdeea458c, /// 0x64c
			 0xd410a13f, /// 0x650
			 0x167c8127, /// 0x654
			 0x72c47dec, /// 0x658
			 0x134e69bc, /// 0x65c
			 0xa22c9d2d, /// 0x660
			 0xe95d967b, /// 0x664
			 0xbc99e7f6, /// 0x668
			 0xd7eca680, /// 0x66c
			 0x4229117f, /// 0x670
			 0x6f66dce1, /// 0x674
			 0x63b810df, /// 0x678
			 0x51546635, /// 0x67c
			 0x141f8bec, /// 0x680
			 0x84676359, /// 0x684
			 0x2f9a2099, /// 0x688
			 0x834d3435, /// 0x68c
			 0x1c18c139, /// 0x690
			 0x1ebb93c4, /// 0x694
			 0x046f235d, /// 0x698
			 0xded015cf, /// 0x69c
			 0x315f92e5, /// 0x6a0
			 0xddec0139, /// 0x6a4
			 0x5a014604, /// 0x6a8
			 0x8ef83e97, /// 0x6ac
			 0x5c8e847a, /// 0x6b0
			 0x93c2536d, /// 0x6b4
			 0xb6143805, /// 0x6b8
			 0x7c68c10e, /// 0x6bc
			 0x7297bd56, /// 0x6c0
			 0x499f4624, /// 0x6c4
			 0x47757266, /// 0x6c8
			 0xabf61ee5, /// 0x6cc
			 0x8b2628bf, /// 0x6d0
			 0x753c460b, /// 0x6d4
			 0x09077553, /// 0x6d8
			 0xd1166ff9, /// 0x6dc
			 0x6a72f3fa, /// 0x6e0
			 0x27121c37, /// 0x6e4
			 0x3ddba603, /// 0x6e8
			 0x957fbad8, /// 0x6ec
			 0xeddd00e0, /// 0x6f0
			 0x1cf119e4, /// 0x6f4
			 0x811373aa, /// 0x6f8
			 0x35ca8409, /// 0x6fc
			 0x3311ada5, /// 0x700
			 0xf5d22318, /// 0x704
			 0xadeb706c, /// 0x708
			 0x64721952, /// 0x70c
			 0x4ccd05ba, /// 0x710
			 0x420cb075, /// 0x714
			 0xc1c7972f, /// 0x718
			 0x0c1d24be, /// 0x71c
			 0xadc8eb69, /// 0x720
			 0x7b5ef1c2, /// 0x724
			 0x0bf48fe0, /// 0x728
			 0x67c297ff, /// 0x72c
			 0x9b682ce8, /// 0x730
			 0x25ae073c, /// 0x734
			 0xa9ae17a7, /// 0x738
			 0x92d87716, /// 0x73c
			 0xe02c8f68, /// 0x740
			 0x0e4c9f74, /// 0x744
			 0xa58c9db5, /// 0x748
			 0x3502bd38, /// 0x74c
			 0x704b7b39, /// 0x750
			 0x444a1dba, /// 0x754
			 0xf4cfe6cb, /// 0x758
			 0x0655a96b, /// 0x75c
			 0x5eda718a, /// 0x760
			 0x4ca84f35, /// 0x764
			 0x69389d59, /// 0x768
			 0x3cbcf724, /// 0x76c
			 0x5a247676, /// 0x770
			 0xb746debf, /// 0x774
			 0x2b3a6886, /// 0x778
			 0xdb9cd361, /// 0x77c
			 0xc9adfd21, /// 0x780
			 0xa1465d64, /// 0x784
			 0x2720e15b, /// 0x788
			 0xd9bb19c7, /// 0x78c
			 0xd14dc52d, /// 0x790
			 0x2a5698d2, /// 0x794
			 0x6e406806, /// 0x798
			 0xd5dabf2d, /// 0x79c
			 0xa4592ee2, /// 0x7a0
			 0x59d8ce51, /// 0x7a4
			 0x12301908, /// 0x7a8
			 0x72e2f206, /// 0x7ac
			 0x21c73e7f, /// 0x7b0
			 0x0d52c739, /// 0x7b4
			 0x8deaa871, /// 0x7b8
			 0x749d690e, /// 0x7bc
			 0x21110d6d, /// 0x7c0
			 0xe6bf0718, /// 0x7c4
			 0xa1964184, /// 0x7c8
			 0x09c4f63a, /// 0x7cc
			 0x32128323, /// 0x7d0
			 0x0058d1ce, /// 0x7d4
			 0x8bed62ee, /// 0x7d8
			 0xc2e17b3a, /// 0x7dc
			 0x65e44663, /// 0x7e0
			 0xdc283ed5, /// 0x7e4
			 0x0b1b5841, /// 0x7e8
			 0x97fd1e25, /// 0x7ec
			 0x58930130, /// 0x7f0
			 0xd9305145, /// 0x7f4
			 0xc1d98a6c, /// 0x7f8
			 0x5c697107, /// 0x7fc
			 0x73466139, /// 0x800
			 0x3fbba1c8, /// 0x804
			 0x152715dd, /// 0x808
			 0xfd94bdc1, /// 0x80c
			 0xc55f7dc7, /// 0x810
			 0x6ca2607f, /// 0x814
			 0x96512ca1, /// 0x818
			 0xb7eae1b3, /// 0x81c
			 0xabbe8222, /// 0x820
			 0xaf32528a, /// 0x824
			 0x1d78d27a, /// 0x828
			 0xdd618a03, /// 0x82c
			 0xd05dd3ca, /// 0x830
			 0x370c3aa3, /// 0x834
			 0xe1c4b292, /// 0x838
			 0x46270f44, /// 0x83c
			 0x1fbc515c, /// 0x840
			 0xf973ccd7, /// 0x844
			 0x26813ed4, /// 0x848
			 0x63fa33d0, /// 0x84c
			 0x5fc4bae8, /// 0x850
			 0xd623aa41, /// 0x854
			 0x0cdd6680, /// 0x858
			 0x00458895, /// 0x85c
			 0xf5fb3c8e, /// 0x860
			 0xd136f08d, /// 0x864
			 0x6143f897, /// 0x868
			 0xd74b6d99, /// 0x86c
			 0x247e15d0, /// 0x870
			 0x941de53b, /// 0x874
			 0x21a5505d, /// 0x878
			 0xaaa4d71a, /// 0x87c
			 0x1d06d935, /// 0x880
			 0xf410f70c, /// 0x884
			 0xe7a048b1, /// 0x888
			 0xb928e21a, /// 0x88c
			 0xf914b566, /// 0x890
			 0xc68c8285, /// 0x894
			 0x522dbb3c, /// 0x898
			 0x9e20c376, /// 0x89c
			 0xc8d99d07, /// 0x8a0
			 0x5089c4e8, /// 0x8a4
			 0xf13cb755, /// 0x8a8
			 0xf7a50006, /// 0x8ac
			 0x8295af65, /// 0x8b0
			 0x97308103, /// 0x8b4
			 0x9c07a5f0, /// 0x8b8
			 0xf1ecac23, /// 0x8bc
			 0x1f4c96f4, /// 0x8c0
			 0x3f14a32d, /// 0x8c4
			 0x6dc6f8ae, /// 0x8c8
			 0xdce60370, /// 0x8cc
			 0x7cf3f236, /// 0x8d0
			 0x326330b7, /// 0x8d4
			 0x079f564f, /// 0x8d8
			 0x9ce1c6e4, /// 0x8dc
			 0x0bd27664, /// 0x8e0
			 0xa457b343, /// 0x8e4
			 0x0295906b, /// 0x8e8
			 0x3e71f6b8, /// 0x8ec
			 0x84f00faf, /// 0x8f0
			 0xe788abba, /// 0x8f4
			 0x9c968322, /// 0x8f8
			 0x70dc091a, /// 0x8fc
			 0xb131a912, /// 0x900
			 0x6c30a798, /// 0x904
			 0x97527ca1, /// 0x908
			 0x593ff684, /// 0x90c
			 0xb225794f, /// 0x910
			 0x377f6afe, /// 0x914
			 0xc5ed0553, /// 0x918
			 0x367913ac, /// 0x91c
			 0x965300dd, /// 0x920
			 0x043e62f4, /// 0x924
			 0xee8cfb87, /// 0x928
			 0x3526faf2, /// 0x92c
			 0x096325f8, /// 0x930
			 0x1ed4955d, /// 0x934
			 0x1feeb294, /// 0x938
			 0x377bc1e9, /// 0x93c
			 0xad66b61f, /// 0x940
			 0x30c37a36, /// 0x944
			 0x59466d46, /// 0x948
			 0xff52b4a2, /// 0x94c
			 0xa1ed9f65, /// 0x950
			 0x2ffad0ec, /// 0x954
			 0x801c6fa4, /// 0x958
			 0x092a8224, /// 0x95c
			 0xadca5626, /// 0x960
			 0x6628f064, /// 0x964
			 0x0018effb, /// 0x968
			 0xae299e68, /// 0x96c
			 0x72db313c, /// 0x970
			 0x5af418bd, /// 0x974
			 0x4c533c13, /// 0x978
			 0x59c21088, /// 0x97c
			 0xb00e701f, /// 0x980
			 0x56915da6, /// 0x984
			 0x578808fb, /// 0x988
			 0x6444af43, /// 0x98c
			 0xa9ab5a41, /// 0x990
			 0x8f412c8e, /// 0x994
			 0xc899a034, /// 0x998
			 0x2906a44d, /// 0x99c
			 0x2698e829, /// 0x9a0
			 0xf3292051, /// 0x9a4
			 0x487f5369, /// 0x9a8
			 0xf5f0901f, /// 0x9ac
			 0x992d1750, /// 0x9b0
			 0x67bdce9c, /// 0x9b4
			 0x6f1affab, /// 0x9b8
			 0x34394cdd, /// 0x9bc
			 0xfda278f8, /// 0x9c0
			 0xca7c4b09, /// 0x9c4
			 0xb88b05d1, /// 0x9c8
			 0x54ab8310, /// 0x9cc
			 0x05ed8a8b, /// 0x9d0
			 0xcd8c3f0b, /// 0x9d4
			 0x1599ce9d, /// 0x9d8
			 0xce345f9d, /// 0x9dc
			 0x032fa251, /// 0x9e0
			 0x5e1f684f, /// 0x9e4
			 0x5aae3eba, /// 0x9e8
			 0x939014e0, /// 0x9ec
			 0x689fd097, /// 0x9f0
			 0xc3024a4f, /// 0x9f4
			 0x1578ed53, /// 0x9f8
			 0x085e2c77, /// 0x9fc
			 0x5e0adf06, /// 0xa00
			 0x29de3838, /// 0xa04
			 0x5244e310, /// 0xa08
			 0xac1727a6, /// 0xa0c
			 0x9c93d0a1, /// 0xa10
			 0x41eaf094, /// 0xa14
			 0x41b5ec8a, /// 0xa18
			 0xbaf1951d, /// 0xa1c
			 0xb9b9bd50, /// 0xa20
			 0x9bb375bf, /// 0xa24
			 0xc5d696c6, /// 0xa28
			 0xc801fb92, /// 0xa2c
			 0x3ec2c284, /// 0xa30
			 0x037e41c7, /// 0xa34
			 0x9403d9f7, /// 0xa38
			 0xf645c684, /// 0xa3c
			 0x860c90e7, /// 0xa40
			 0x7f2d1b50, /// 0xa44
			 0xfc76f5cf, /// 0xa48
			 0x1fcef049, /// 0xa4c
			 0xb1a90690, /// 0xa50
			 0x6c4b6f04, /// 0xa54
			 0x4298f093, /// 0xa58
			 0xa3699863, /// 0xa5c
			 0xa323349c, /// 0xa60
			 0x1d91a7e5, /// 0xa64
			 0x2f21fe10, /// 0xa68
			 0xa07bad9d, /// 0xa6c
			 0x35270327, /// 0xa70
			 0xb4c4041d, /// 0xa74
			 0xbb0111b1, /// 0xa78
			 0xcf9d1495, /// 0xa7c
			 0xc51a5d6b, /// 0xa80
			 0x838b2988, /// 0xa84
			 0x0343e319, /// 0xa88
			 0x7a387b08, /// 0xa8c
			 0x6afeef9f, /// 0xa90
			 0x43637290, /// 0xa94
			 0xdfccaf6b, /// 0xa98
			 0x6341de54, /// 0xa9c
			 0xa224dbe5, /// 0xaa0
			 0x3ca622fd, /// 0xaa4
			 0xe1845602, /// 0xaa8
			 0x5377ce61, /// 0xaac
			 0xbdb2ab27, /// 0xab0
			 0x1f0a98ba, /// 0xab4
			 0x0d8e9bdf, /// 0xab8
			 0x97207542, /// 0xabc
			 0x6553c8c3, /// 0xac0
			 0x154ee77c, /// 0xac4
			 0xe8442ec8, /// 0xac8
			 0x1fbd8c2c, /// 0xacc
			 0xf2333eff, /// 0xad0
			 0x6c3997d6, /// 0xad4
			 0x9a16a55d, /// 0xad8
			 0xb3328671, /// 0xadc
			 0x3d597dae, /// 0xae0
			 0xbf2a57fb, /// 0xae4
			 0x5753f210, /// 0xae8
			 0xa5809835, /// 0xaec
			 0x9113d923, /// 0xaf0
			 0xe692ad87, /// 0xaf4
			 0x3af82cbd, /// 0xaf8
			 0xcfb4f07b, /// 0xafc
			 0xead4c6ed, /// 0xb00
			 0xff858f41, /// 0xb04
			 0x6d670978, /// 0xb08
			 0xaa3680fe, /// 0xb0c
			 0xbffb9bb0, /// 0xb10
			 0xf382589b, /// 0xb14
			 0x7f3957e3, /// 0xb18
			 0x69f4e78b, /// 0xb1c
			 0x908949cc, /// 0xb20
			 0xa4c25801, /// 0xb24
			 0x86cd5a75, /// 0xb28
			 0xf4a53bab, /// 0xb2c
			 0x4bdf4488, /// 0xb30
			 0x8b5f19a0, /// 0xb34
			 0xf6c884c1, /// 0xb38
			 0xd6383b3d, /// 0xb3c
			 0x63256fcb, /// 0xb40
			 0x71bc1e32, /// 0xb44
			 0x729a1b0c, /// 0xb48
			 0x032337ff, /// 0xb4c
			 0x3f3d48af, /// 0xb50
			 0xa0938c05, /// 0xb54
			 0xaf69f635, /// 0xb58
			 0x64c4ac7c, /// 0xb5c
			 0xd7398f09, /// 0xb60
			 0x0040881e, /// 0xb64
			 0x4a279454, /// 0xb68
			 0xb3fba5b2, /// 0xb6c
			 0x0b13fc51, /// 0xb70
			 0x9493038e, /// 0xb74
			 0x16b140e0, /// 0xb78
			 0xfbf514ed, /// 0xb7c
			 0x230e8adc, /// 0xb80
			 0x714f70e3, /// 0xb84
			 0x76ef744b, /// 0xb88
			 0xafc8e6b1, /// 0xb8c
			 0x08065e27, /// 0xb90
			 0x0c091adb, /// 0xb94
			 0x4eff24ef, /// 0xb98
			 0xa97f9dab, /// 0xb9c
			 0x03dc4496, /// 0xba0
			 0xa88455b9, /// 0xba4
			 0x2531c449, /// 0xba8
			 0x0c823a4e, /// 0xbac
			 0x3e37768c, /// 0xbb0
			 0xf90d72dc, /// 0xbb4
			 0x929df525, /// 0xbb8
			 0x5e1a6477, /// 0xbbc
			 0x1e2cd249, /// 0xbc0
			 0x76f3bfcf, /// 0xbc4
			 0xd79b128e, /// 0xbc8
			 0x62fda3e4, /// 0xbcc
			 0x2aa7668c, /// 0xbd0
			 0x4f2bf921, /// 0xbd4
			 0xb6c3bdb4, /// 0xbd8
			 0x13fe2624, /// 0xbdc
			 0xeeba6c1a, /// 0xbe0
			 0xcc1f06a9, /// 0xbe4
			 0x5729e2c1, /// 0xbe8
			 0x9220b9eb, /// 0xbec
			 0xfaeaa14a, /// 0xbf0
			 0xb6ba9193, /// 0xbf4
			 0x7790376d, /// 0xbf8
			 0x5964f732, /// 0xbfc
			 0xf939bf7d, /// 0xc00
			 0xf0233945, /// 0xc04
			 0xcc665081, /// 0xc08
			 0xa7c2444c, /// 0xc0c
			 0x9e382e81, /// 0xc10
			 0x1c5dd34d, /// 0xc14
			 0x4169913a, /// 0xc18
			 0xaa677f0f, /// 0xc1c
			 0xb033597b, /// 0xc20
			 0x5359be79, /// 0xc24
			 0xe9e5e971, /// 0xc28
			 0xdfef7c89, /// 0xc2c
			 0xd1572257, /// 0xc30
			 0x68d00ce8, /// 0xc34
			 0x447fddd8, /// 0xc38
			 0x8fba2697, /// 0xc3c
			 0xfe0ff2dd, /// 0xc40
			 0x84aa0fc2, /// 0xc44
			 0xe0b37380, /// 0xc48
			 0x40a4413b, /// 0xc4c
			 0x9165a1db, /// 0xc50
			 0x102a8eb7, /// 0xc54
			 0xd48bf680, /// 0xc58
			 0x4984a32c, /// 0xc5c
			 0xb592416b, /// 0xc60
			 0x1823fcbe, /// 0xc64
			 0xf9364c40, /// 0xc68
			 0x8d65f1db, /// 0xc6c
			 0x704802c0, /// 0xc70
			 0x68fbb4c2, /// 0xc74
			 0x3171c599, /// 0xc78
			 0x264f75e8, /// 0xc7c
			 0x95a551fa, /// 0xc80
			 0x9c09962a, /// 0xc84
			 0x8a5ddc4b, /// 0xc88
			 0x72a4f4ac, /// 0xc8c
			 0xb74c586b, /// 0xc90
			 0x8346db9d, /// 0xc94
			 0xb34d36f8, /// 0xc98
			 0x37107d83, /// 0xc9c
			 0xd776163c, /// 0xca0
			 0x978c32ec, /// 0xca4
			 0x49de994f, /// 0xca8
			 0xe317b0d2, /// 0xcac
			 0x1dadfb44, /// 0xcb0
			 0xc90681d2, /// 0xcb4
			 0xae866db6, /// 0xcb8
			 0x0340ce4e, /// 0xcbc
			 0x0c80e711, /// 0xcc0
			 0xd1c6c41d, /// 0xcc4
			 0xb2cfdd9a, /// 0xcc8
			 0x9cb81a2a, /// 0xccc
			 0xb24213a8, /// 0xcd0
			 0x26aa8821, /// 0xcd4
			 0xeda00230, /// 0xcd8
			 0xb2df321d, /// 0xcdc
			 0x48241e13, /// 0xce0
			 0xcbe9e90e, /// 0xce4
			 0x3e66e065, /// 0xce8
			 0xc70438db, /// 0xcec
			 0xdbe003a7, /// 0xcf0
			 0xf5f4bde9, /// 0xcf4
			 0xd17db7a7, /// 0xcf8
			 0xc4db52b6, /// 0xcfc
			 0x7c5319c8, /// 0xd00
			 0xe3df6037, /// 0xd04
			 0xa54ec38a, /// 0xd08
			 0x94377d4f, /// 0xd0c
			 0x3c1964e5, /// 0xd10
			 0x567d2775, /// 0xd14
			 0x3978ca15, /// 0xd18
			 0x6aee4177, /// 0xd1c
			 0xc015413b, /// 0xd20
			 0x847bcb51, /// 0xd24
			 0xa322b71e, /// 0xd28
			 0xb8595725, /// 0xd2c
			 0xb6049528, /// 0xd30
			 0x9b34b104, /// 0xd34
			 0xa6fd3f88, /// 0xd38
			 0x776a621b, /// 0xd3c
			 0x69a34e81, /// 0xd40
			 0xf77963e9, /// 0xd44
			 0x29dd5f33, /// 0xd48
			 0x3f1954d9, /// 0xd4c
			 0x438e9b0f, /// 0xd50
			 0xde073a54, /// 0xd54
			 0x65cddbc4, /// 0xd58
			 0x505c3361, /// 0xd5c
			 0x6dc1a60d, /// 0xd60
			 0xb9a8d17c, /// 0xd64
			 0xfc985251, /// 0xd68
			 0xddb10627, /// 0xd6c
			 0x376a27bd, /// 0xd70
			 0xbe8a6ab6, /// 0xd74
			 0x6ef4d99a, /// 0xd78
			 0xa70efac4, /// 0xd7c
			 0xf28ca691, /// 0xd80
			 0x4a9de0ef, /// 0xd84
			 0x586bfdcd, /// 0xd88
			 0x0d937ed7, /// 0xd8c
			 0x2c2f2904, /// 0xd90
			 0xc850e174, /// 0xd94
			 0x3d96e4e5, /// 0xd98
			 0x2d0660c4, /// 0xd9c
			 0x3edc517b, /// 0xda0
			 0xb3ccbeba, /// 0xda4
			 0x8f31da75, /// 0xda8
			 0x14aaabe2, /// 0xdac
			 0xa84e8b39, /// 0xdb0
			 0x4105814f, /// 0xdb4
			 0xab127bd7, /// 0xdb8
			 0xf50ce964, /// 0xdbc
			 0xdf481b80, /// 0xdc0
			 0x78a8c311, /// 0xdc4
			 0x1cc0bbdc, /// 0xdc8
			 0xa7eb6ab2, /// 0xdcc
			 0x944851db, /// 0xdd0
			 0x81b5107f, /// 0xdd4
			 0x2bbbd94e, /// 0xdd8
			 0xe0df6d80, /// 0xddc
			 0x03358083, /// 0xde0
			 0xf8a26ca3, /// 0xde4
			 0xe83fdfe5, /// 0xde8
			 0x4a77d77e, /// 0xdec
			 0x55e92cb5, /// 0xdf0
			 0x03f534f3, /// 0xdf4
			 0x7041ea82, /// 0xdf8
			 0xa4bc3de2, /// 0xdfc
			 0x8234a603, /// 0xe00
			 0xc1f5dd4f, /// 0xe04
			 0x1937a4a8, /// 0xe08
			 0x85c1cbb6, /// 0xe0c
			 0x74bba0ab, /// 0xe10
			 0x590a122e, /// 0xe14
			 0x56a855a9, /// 0xe18
			 0x7181500e, /// 0xe1c
			 0x030d92f6, /// 0xe20
			 0x0e4128db, /// 0xe24
			 0x32b876b6, /// 0xe28
			 0x6a95ba31, /// 0xe2c
			 0xe340aff8, /// 0xe30
			 0x459ec63a, /// 0xe34
			 0xac3a7ef5, /// 0xe38
			 0x960401fe, /// 0xe3c
			 0x4c5f410b, /// 0xe40
			 0x36780868, /// 0xe44
			 0xd2299a77, /// 0xe48
			 0x9f6476e9, /// 0xe4c
			 0xae9a752c, /// 0xe50
			 0x15d1bfdf, /// 0xe54
			 0x2be0f923, /// 0xe58
			 0xec00001c, /// 0xe5c
			 0xc39b867e, /// 0xe60
			 0xa1eec8ca, /// 0xe64
			 0xa43369fd, /// 0xe68
			 0x22d40045, /// 0xe6c
			 0xa804f3e3, /// 0xe70
			 0x06897bde, /// 0xe74
			 0x79aa78ad, /// 0xe78
			 0xc364ba82, /// 0xe7c
			 0x72454da4, /// 0xe80
			 0x1128b45b, /// 0xe84
			 0x20759dbd, /// 0xe88
			 0x0a31d7f8, /// 0xe8c
			 0xeba87ed5, /// 0xe90
			 0x10602661, /// 0xe94
			 0x251bef8f, /// 0xe98
			 0x17e39daa, /// 0xe9c
			 0xd7f56739, /// 0xea0
			 0xb34d9336, /// 0xea4
			 0x6021c693, /// 0xea8
			 0x90367bdb, /// 0xeac
			 0x3a3130bd, /// 0xeb0
			 0xfe1a3a6d, /// 0xeb4
			 0x0e2a423d, /// 0xeb8
			 0xda0a2080, /// 0xebc
			 0x65be502b, /// 0xec0
			 0x1c98dbaf, /// 0xec4
			 0x84500de2, /// 0xec8
			 0x23bbb7f8, /// 0xecc
			 0xc88a5170, /// 0xed0
			 0xdb475014, /// 0xed4
			 0xeeeb489b, /// 0xed8
			 0x986e102d, /// 0xedc
			 0x96f81836, /// 0xee0
			 0x22728df2, /// 0xee4
			 0x73f03332, /// 0xee8
			 0xe771bdd0, /// 0xeec
			 0xf0d7a1c7, /// 0xef0
			 0x42248470, /// 0xef4
			 0xf7c14065, /// 0xef8
			 0x1e2fd3d8, /// 0xefc
			 0xd05ee90f, /// 0xf00
			 0x129401b0, /// 0xf04
			 0x8bfbca80, /// 0xf08
			 0x3664fc25, /// 0xf0c
			 0xde628539, /// 0xf10
			 0x7b8ade44, /// 0xf14
			 0x2858384c, /// 0xf18
			 0x3020d612, /// 0xf1c
			 0x5960eec5, /// 0xf20
			 0x2b04bde8, /// 0xf24
			 0xf610b67b, /// 0xf28
			 0x93210691, /// 0xf2c
			 0x1b7f5fab, /// 0xf30
			 0x691d4660, /// 0xf34
			 0xd1706522, /// 0xf38
			 0x1895ff0e, /// 0xf3c
			 0x3ea1b3b5, /// 0xf40
			 0xd1396e3c, /// 0xf44
			 0x2537953b, /// 0xf48
			 0x21966f6c, /// 0xf4c
			 0x347a8a34, /// 0xf50
			 0xdd9ac2b7, /// 0xf54
			 0x03de2d45, /// 0xf58
			 0xadf9762b, /// 0xf5c
			 0x02c6fc47, /// 0xf60
			 0xab2ca808, /// 0xf64
			 0x5b8f82c9, /// 0xf68
			 0x35a742ac, /// 0xf6c
			 0x54a026e5, /// 0xf70
			 0xfece9e16, /// 0xf74
			 0x4ae8b573, /// 0xf78
			 0xb6f1222b, /// 0xf7c
			 0x2879b50c, /// 0xf80
			 0x4e9d6d63, /// 0xf84
			 0xc7ae0f04, /// 0xf88
			 0x8121db7b, /// 0xf8c
			 0xfe0660e4, /// 0xf90
			 0xe2cc0c79, /// 0xf94
			 0x1b26eb04, /// 0xf98
			 0xefca9092, /// 0xf9c
			 0xc5c67541, /// 0xfa0
			 0x8875ee4a, /// 0xfa4
			 0x2947146c, /// 0xfa8
			 0x5a6d15be, /// 0xfac
			 0x02bbd597, /// 0xfb0
			 0x5540f5fe, /// 0xfb4
			 0x9fe262ac, /// 0xfb8
			 0x48019aeb, /// 0xfbc
			 0x8a41086e, /// 0xfc0
			 0xd0713f93, /// 0xfc4
			 0xd909ca31, /// 0xfc8
			 0xc0e0b558, /// 0xfcc
			 0x8de9f159, /// 0xfd0
			 0x61ab3752, /// 0xfd4
			 0x53fd9830, /// 0xfd8
			 0xf18c7d3f, /// 0xfdc
			 0x27ab3272, /// 0xfe0
			 0x078f6ffa, /// 0xfe4
			 0x2f005432, /// 0xfe8
			 0xe9470488, /// 0xfec
			 0x5e6ea3d9, /// 0xff0
			 0xfd84e46d, /// 0xff4
			 0x323d35b1, /// 0xff8
			 0x53ea2a47 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00004
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00008
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00010
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00018
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00020
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00024
			 0xffc00001,                                                  // -signaling NaN                              /// 00028
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00030
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00034
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00038
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0003c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00040
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00050
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00058
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0005c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xff800000,                                                  // -inf                                        /// 0006c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00074
			 0x3f028f5c,                                                  // 0.51                                        /// 00078
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0xcb000000,                                                  // -8388608.0                                  /// 00084
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00088
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0008c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00090
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00094
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0009c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000b8
			 0xcb000000,                                                  // -8388608.0                                  /// 000bc
			 0x55555555,                                                  // 4 random values                             /// 000c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00100
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00104
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00108
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00110
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00114
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x33333333,                                                  // 4 random values                             /// 00130
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00134
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00140
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00144
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00160
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00168
			 0x55555555,                                                  // 4 random values                             /// 0016c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00170
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c400000,                                                  // Leading 1s:                                 /// 0017c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00180
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00184
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0018c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00190
			 0xffc00001,                                                  // -signaling NaN                              /// 00194
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00198
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x55555555,                                                  // 4 random values                             /// 001a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001b4
			 0x3f028f5c,                                                  // 0.51                                        /// 001b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001bc
			 0x7f800000,                                                  // inf                                         /// 001c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80000000,                                                  // -zero                                       /// 001fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00200
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00204
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00208
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00214
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00218
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0021c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00220
			 0x00000000,                                                  // zero                                        /// 00224
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0022c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x00011111,                                                  // 9.7958E-41                                  /// 00234
			 0x00011111,                                                  // 9.7958E-41                                  /// 00238
			 0x0e000003,                                                  // Trailing 1s:                                /// 0023c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00240
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00248
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00250
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00254
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00258
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0025c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0026c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00270
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00274
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00278
			 0x55555555,                                                  // 4 random values                             /// 0027c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00288
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0028c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00290
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xffc00001,                                                  // -signaling NaN                              /// 00298
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002ac
			 0x00000000,                                                  // zero                                        /// 002b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 002e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 002f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00300
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00308
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00314
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0031c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00320
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00324
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00328
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0032c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00334
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00338
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0033c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00340
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00344
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00348
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00350
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00354
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00360
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00368
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0036c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0037c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00380
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00384
			 0xffc00001,                                                  // -signaling NaN                              /// 00388
			 0x33333333,                                                  // 4 random values                             /// 0038c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0c600000,                                                  // Leading 1s:                                 /// 00394
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0xffc00001,                                                  // -signaling NaN                              /// 003b4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00400
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00404
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0040c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00410
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00418
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00420
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00424
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00428
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0042c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00430
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00434
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00438
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0043c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00448
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0044c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00450
			 0x00011111,                                                  // 9.7958E-41                                  /// 00454
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00458
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0045c
			 0xff800000,                                                  // -inf                                        /// 00460
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00474
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00478
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00480
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0xffc00001,                                                  // -signaling NaN                              /// 00488
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0048c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00490
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00494
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x7f800000,                                                  // inf                                         /// 004a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004a8
			 0xff800000,                                                  // -inf                                        /// 004ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004b4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00500
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00504
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00508
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00510
			 0x80011111,                                                  // -9.7958E-41                                 /// 00514
			 0x4b000000,                                                  // 8388608.0                                   /// 00518
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00520
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00528
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0052c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00530
			 0x33333333,                                                  // 4 random values                             /// 00534
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00538
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0053c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00540
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00548
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0054c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00554
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00558
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0055c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x7f800000,                                                  // inf                                         /// 00564
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0056c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00574
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00578
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00580
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00588
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00590
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00594
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00598
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x3f028f5c,                                                  // 0.51                                        /// 005ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 005f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005f8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00600
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x7f800000,                                                  // inf                                         /// 00608
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0060c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x80011111,                                                  // -9.7958E-41                                 /// 00614
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00618
			 0x0e000003,                                                  // Trailing 1s:                                /// 0061c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00620
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00624
			 0xffc00001,                                                  // -signaling NaN                              /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00630
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00638
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0063c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00644
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0c600000,                                                  // Leading 1s:                                 /// 0065c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00660
			 0x0c780000,                                                  // Leading 1s:                                 /// 00664
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0066c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00674
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0067c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00680
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00688
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0068c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00698
			 0xcb000000,                                                  // -8388608.0                                  /// 0069c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006b8
			 0x4b000000,                                                  // 8388608.0                                   /// 006bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00700
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00708
			 0x00011111,                                                  // 9.7958E-41                                  /// 0070c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00714
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00720
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00734
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00738
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00740
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00744
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0074c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00750
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00754
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00758
			 0x0c400000,                                                  // Leading 1s:                                 /// 0075c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00760
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00768
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00778
			 0x0c600000,                                                  // Leading 1s:                                 /// 0077c
			 0x33333333,                                                  // 4 random values                             /// 00780
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00784
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00788
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0078c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00790
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0xcb000000,                                                  // -8388608.0                                  /// 00798
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0079c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x55555555,                                                  // 4 random values                             /// 007b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7fc00001,                                                  // signaling NaN                               /// 007c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007d4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00800
			 0x0e000001,                                                  // Trailing 1s:                                /// 00804
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0080c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00810
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00814
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00818
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00824
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00828
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0082c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00834
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00838
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00840
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0084c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00854
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00858
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0085c
			 0xff800000,                                                  // -inf                                        /// 00860
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00868
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0086c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0x33333333,                                                  // 4 random values                             /// 00878
			 0x00011111,                                                  // 9.7958E-41                                  /// 0087c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00880
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00888
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0088c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00890
			 0x7fc00001,                                                  // signaling NaN                               /// 00894
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00898
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0089c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a0
			 0xbf028f5c,                                                  // -0.51                                       /// 008a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x00000000,                                                  // zero                                        /// 008bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0xffc00001,                                                  // -signaling NaN                              /// 008d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00900
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00904
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00908
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00910
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00914
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00920
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00924
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00928
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00000000,                                                  // zero                                        /// 00930
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00934
			 0x7f800000,                                                  // inf                                         /// 00938
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00940
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x4b000000,                                                  // 8388608.0                                   /// 0094c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00950
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00954
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0xffc00001,                                                  // -signaling NaN                              /// 0095c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00960
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00964
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00968
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0096c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00970
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00974
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00978
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00988
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00990
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00998
			 0xff800000,                                                  // -inf                                        /// 0099c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d4
			 0x3f028f5c,                                                  // 0.51                                        /// 009d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009f0
			 0x00000000,                                                  // zero                                        /// 009f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a24
			 0xffc00001,                                                  // -signaling NaN                              /// 00a28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a30
			 0x7f800000,                                                  // inf                                         /// 00a34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a54
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a78
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a84
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00aa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ab0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ab8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ac0
			 0x55555555,                                                  // 4 random values                             /// 00ac4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ad0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00adc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00af0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00afc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b1c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b44
			 0x00000000,                                                  // zero                                        /// 00b48
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b6c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b74
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0xff800000,                                                  // -inf                                        /// 00bb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bc0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bc8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bd8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00be0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bf0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bf4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c2c
			 0x7f800000,                                                  // inf                                         /// 00c30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x4b000000,                                                  // 8388608.0                                   /// 00c44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c54
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c60
			 0x7f800000,                                                  // inf                                         /// 00c64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c7c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c80
			 0x4b000000,                                                  // 8388608.0                                   /// 00c84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cc8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ccc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000000,                                                  // zero                                        /// 00cd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cdc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ce8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cf0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cf8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d0c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x80000000,                                                  // -zero                                       /// 00d20
			 0x4b000000,                                                  // 8388608.0                                   /// 00d24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d44
			 0x80000000,                                                  // -zero                                       /// 00d48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d58
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d68
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d70
			 0x7fc00001,                                                  // signaling NaN                               /// 00d74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x55555555,                                                  // 4 random values                             /// 00da4
			 0x7f800000,                                                  // inf                                         /// 00da8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00db0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xff800000,                                                  // -inf                                        /// 00db8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dc0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dcc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00df0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dfc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e04
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e0c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e40
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e44
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e4c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x55555555,                                                  // 4 random values                             /// 00e70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00e90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e98
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ea4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00eb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ebc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ecc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ed0
			 0x33333333,                                                  // 4 random values                             /// 00ed4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ee4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ee8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f0c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x7f800000,                                                  // inf                                         /// 00f34
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f50
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x33333333,                                                  // 4 random values                             /// 00f60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7fc00001,                                                  // signaling NaN                               /// 00f98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fb4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ff0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0e01ffff                                                  // Trailing 1s:                                /// last
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
			 0x00000318,
			 0x0000048c,
			 0x00000738,
			 0x0000071c,
			 0x0000023c,
			 0x00000154,
			 0x00000370,
			 0x000003c0,

			 /// vpu register f2
			 0x41e80000,
			 0x40c00000,
			 0x41700000,
			 0x41700000,
			 0x41d80000,
			 0x41a80000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f3
			 0x40000000,
			 0x41c80000,
			 0x41a00000,
			 0x41100000,
			 0x41a80000,
			 0x41300000,
			 0x41300000,
			 0x40e00000,

			 /// vpu register f4
			 0x41400000,
			 0x40e00000,
			 0x41200000,
			 0x41f80000,
			 0x41b80000,
			 0x41880000,
			 0x41b80000,
			 0x41b00000,

			 /// vpu register f5
			 0x41880000,
			 0x41d80000,
			 0x41700000,
			 0x41d00000,
			 0x41500000,
			 0x40800000,
			 0x41600000,
			 0x40a00000,

			 /// vpu register f6
			 0x41400000,
			 0x40400000,
			 0x41e80000,
			 0x41d00000,
			 0x40c00000,
			 0x3f800000,
			 0x40e00000,
			 0x41500000,

			 /// vpu register f7
			 0x41f00000,
			 0x41a00000,
			 0x41700000,
			 0x3f800000,
			 0x41880000,
			 0x40c00000,
			 0x41900000,
			 0x41600000,

			 /// vpu register f8
			 0x42000000,
			 0x40e00000,
			 0x41800000,
			 0x40a00000,
			 0x3f800000,
			 0x3f800000,
			 0x41700000,
			 0x41980000,

			 /// vpu register f9
			 0x41880000,
			 0x3f800000,
			 0x41100000,
			 0x41000000,
			 0x41880000,
			 0x41d00000,
			 0x41400000,
			 0x41a80000,

			 /// vpu register f10
			 0x41400000,
			 0x40000000,
			 0x41600000,
			 0x41f80000,
			 0x41e80000,
			 0x41000000,
			 0x40000000,
			 0x41800000,

			 /// vpu register f11
			 0x40a00000,
			 0x41900000,
			 0x41d80000,
			 0x41c80000,
			 0x41e00000,
			 0x41500000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f12
			 0x41b00000,
			 0x41980000,
			 0x41500000,
			 0x41b00000,
			 0x41600000,
			 0x41000000,
			 0x40400000,
			 0x42000000,

			 /// vpu register f13
			 0x41c00000,
			 0x41500000,
			 0x40a00000,
			 0x41d80000,
			 0x40e00000,
			 0x41a80000,
			 0x41980000,
			 0x41b00000,

			 /// vpu register f14
			 0x41d00000,
			 0x41980000,
			 0x41880000,
			 0x41d80000,
			 0x41b80000,
			 0x41b80000,
			 0x40400000,
			 0x42000000,

			 /// vpu register f15
			 0x41300000,
			 0x41a00000,
			 0x41100000,
			 0x41f00000,
			 0x41900000,
			 0x41000000,
			 0x41f80000,
			 0x40a00000,

			 /// vpu register f16
			 0x41f80000,
			 0x42000000,
			 0x40a00000,
			 0x3f800000,
			 0x40800000,
			 0x41300000,
			 0x40000000,
			 0x41500000,

			 /// vpu register f17
			 0x41700000,
			 0x41400000,
			 0x40a00000,
			 0x42000000,
			 0x41300000,
			 0x40a00000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f18
			 0x41800000,
			 0x41980000,
			 0x41200000,
			 0x41a80000,
			 0x41100000,
			 0x42000000,
			 0x41c00000,
			 0x41700000,

			 /// vpu register f19
			 0x41b00000,
			 0x41d00000,
			 0x3f800000,
			 0x41200000,
			 0x40e00000,
			 0x40000000,
			 0x41100000,
			 0x41f00000,

			 /// vpu register f20
			 0x41100000,
			 0x40400000,
			 0x41400000,
			 0x41880000,
			 0x41600000,
			 0x41e00000,
			 0x41600000,
			 0x41700000,

			 /// vpu register f21
			 0x40c00000,
			 0x41a00000,
			 0x41400000,
			 0x41880000,
			 0x41e80000,
			 0x41a00000,
			 0x41a00000,
			 0x41600000,

			 /// vpu register f22
			 0x41100000,
			 0x41400000,
			 0x41e00000,
			 0x41e80000,
			 0x41980000,
			 0x41a00000,
			 0x41c80000,
			 0x41e80000,

			 /// vpu register f23
			 0x41300000,
			 0x41300000,
			 0x41500000,
			 0x41c00000,
			 0x41a00000,
			 0x41700000,
			 0x41d80000,
			 0x41e00000,

			 /// vpu register f24
			 0x41d80000,
			 0x41a80000,
			 0x40c00000,
			 0x41400000,
			 0x40e00000,
			 0x40000000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f25
			 0x41800000,
			 0x41d80000,
			 0x41800000,
			 0x41b80000,
			 0x41500000,
			 0x3f800000,
			 0x40800000,
			 0x41700000,

			 /// vpu register f26
			 0x40400000,
			 0x42000000,
			 0x41400000,
			 0x40400000,
			 0x40000000,
			 0x41300000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f27
			 0x41600000,
			 0x41d00000,
			 0x41b80000,
			 0x41a00000,
			 0x40800000,
			 0x3f800000,
			 0x41a00000,
			 0x40800000,

			 /// vpu register f28
			 0x41600000,
			 0x41b00000,
			 0x41500000,
			 0x41200000,
			 0x41000000,
			 0x41980000,
			 0x41300000,
			 0x3f800000,

			 /// vpu register f29
			 0x40e00000,
			 0x40c00000,
			 0x41900000,
			 0x41b00000,
			 0x41c00000,
			 0x41f80000,
			 0x41e80000,
			 0x41f00000,

			 /// vpu register f30
			 0x41e00000,
			 0x40800000,
			 0x40000000,
			 0x41880000,
			 0x41f80000,
			 0x41a80000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f31
			 0x41b00000,
			 0x41500000,
			 0x41300000,
			 0x41f80000,
			 0x40c00000,
			 0x41c00000,
			 0x41600000,
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
		"famomaxg.pi f24, f1 (x14)\n"                         ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f13, f1 (x12)\n"                         ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x12)\n"                          ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f6, f1 (x12)\n"                          ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f3, f1 (x11)\n"                          ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f28, f1 (x12)\n"                         ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f23, f1 (x11)\n"                         ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f19, f1 (x14)\n"                         ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f14, f1 (x13)\n"                         ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f9, f1 (x11)\n"                          ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f30, f1 (x15)\n"                         ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f14, f1 (x11)\n"                         ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f6, f1 (x13)\n"                          ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f18, f1 (x15)\n"                         ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f14, f1 (x14)\n"                         ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f28, f1 (x15)\n"                         ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f17, f1 (x13)\n"                         ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f21, f1 (x13)\n"                         ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f16, f1 (x14)\n"                         ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f22, f1 (x14)\n"                         ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f16, f1 (x12)\n"                         ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f9, f1 (x13)\n"                          ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f21, f1 (x15)\n"                         ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f4, f1 (x12)\n"                          ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f30, f1 (x13)\n"                         ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f20, f1 (x11)\n"                         ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f27, f1 (x14)\n"                         ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f15, f1 (x11)\n"                         ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f28, f1 (x12)\n"                         ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x14)\n"                          ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f7, f1 (x13)\n"                          ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f10, f1 (x15)\n"                         ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f18, f1 (x14)\n"                         ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f8, f1 (x13)\n"                          ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f16, f1 (x12)\n"                         ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f15, f1 (x14)\n"                         ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f4, f1 (x15)\n"                          ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f16, f1 (x14)\n"                         ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f3, f1 (x13)\n"                          ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f10, f1 (x11)\n"                         ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f28, f1 (x12)\n"                         ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f15, f1 (x14)\n"                         ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f6, f1 (x14)\n"                          ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f27, f1 (x13)\n"                         ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f22, f1 (x15)\n"                         ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x13)\n"                          ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f11, f1 (x14)\n"                         ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f13, f1 (x14)\n"                         ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f17, f1 (x14)\n"                         ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f6, f1 (x13)\n"                          ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f12, f1 (x15)\n"                         ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f2, f1 (x14)\n"                          ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f23, f1 (x11)\n"                         ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f12, f1 (x15)\n"                         ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f6, f1 (x15)\n"                          ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f17, f1 (x11)\n"                         ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f20, f1 (x11)\n"                         ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f16, f1 (x13)\n"                         ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f18, f1 (x14)\n"                         ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f8, f1 (x11)\n"                          ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f7, f1 (x15)\n"                          ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f28, f1 (x11)\n"                         ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x12)\n"                          ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f22, f1 (x14)\n"                         ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f20, f1 (x15)\n"                         ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f21, f1 (x11)\n"                         ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f23, f1 (x12)\n"                         ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x15)\n"                          ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f19, f1 (x12)\n"                         ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f22, f1 (x15)\n"                         ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f19, f1 (x11)\n"                         ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f2, f1 (x13)\n"                          ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f30, f1 (x15)\n"                         ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f22, f1 (x15)\n"                         ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f15, f1 (x15)\n"                         ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f15, f1 (x11)\n"                         ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f23, f1 (x14)\n"                         ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f30, f1 (x11)\n"                         ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f7, f1 (x12)\n"                          ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x15)\n"                          ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f30, f1 (x12)\n"                         ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f14, f1 (x12)\n"                         ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f8, f1 (x12)\n"                          ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f14, f1 (x12)\n"                         ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f29, f1 (x13)\n"                         ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f14, f1 (x13)\n"                         ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f22, f1 (x13)\n"                         ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f19, f1 (x14)\n"                         ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f13, f1 (x15)\n"                         ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f16, f1 (x15)\n"                         ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f12, f1 (x14)\n"                         ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f2, f1 (x12)\n"                          ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x11)\n"                          ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f24, f1 (x12)\n"                         ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f6, f1 (x11)\n"                          ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f27, f1 (x11)\n"                         ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f5, f1 (x14)\n"                          ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f4, f1 (x11)\n"                          ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"famomaxg.pi f23, f1 (x11)\n"                         ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
