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
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00004
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0000c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00010
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00014
			 0x0c700000,                                                  // Leading 1s:                                 /// 00018
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0001c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0c780000,                                                  // Leading 1s:                                 /// 00024
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00028
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0002c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00034
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00038
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00040
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00044
			 0x7fc00001,                                                  // signaling NaN                               /// 00048
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0004c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0005c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00060
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00068
			 0x7f800000,                                                  // inf                                         /// 0006c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00070
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00078
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00084
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00088
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0008c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x0c700000,                                                  // Leading 1s:                                 /// 00094
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00098
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000bc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000c8
			 0xbf028f5c,                                                  // -0.51                                       /// 000cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000d0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000e0
			 0x55555555,                                                  // 4 random values                             /// 000e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00100
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x4b000000,                                                  // 8388608.0                                   /// 00110
			 0x3f028f5c,                                                  // 0.51                                        /// 00114
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00118
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0011c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00120
			 0x80011111,                                                  // -9.7958E-41                                 /// 00124
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00128
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0012c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00138
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00140
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00144
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0014c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00150
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00154
			 0xcb000000,                                                  // -8388608.0                                  /// 00158
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00160
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00164
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0xff800000,                                                  // -inf                                        /// 0016c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00170
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x33333333,                                                  // 4 random values                             /// 00180
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0018c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00194
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0019c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001bc
			 0x7f800000,                                                  // inf                                         /// 001c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001d4
			 0xff800000,                                                  // -inf                                        /// 001d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e8
			 0x80000000,                                                  // -zero                                       /// 001ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00200
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00204
			 0x80000000,                                                  // -zero                                       /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00214
			 0xff800000,                                                  // -inf                                        /// 00218
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0021c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00220
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00224
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00228
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0xffc00001,                                                  // -signaling NaN                              /// 00230
			 0x0c400000,                                                  // Leading 1s:                                 /// 00234
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00238
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0023c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00240
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00244
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00248
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80011111,                                                  // -9.7958E-41                                 /// 00254
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00258
			 0xffc00001,                                                  // -signaling NaN                              /// 0025c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00260
			 0x0c600000,                                                  // Leading 1s:                                 /// 00264
			 0x0c400000,                                                  // Leading 1s:                                 /// 00268
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00270
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00278
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0027c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00284
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00290
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0029c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x33333333,                                                  // 4 random values                             /// 002b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002dc
			 0x7fc00001,                                                  // signaling NaN                               /// 002e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002fc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00304
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00308
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00310
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00314
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00318
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x80011111,                                                  // -9.7958E-41                                 /// 00324
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00328
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00334
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0033c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00340
			 0xbf028f5c,                                                  // -0.51                                       /// 00344
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0034c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00350
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00354
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0035c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00360
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00368
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0036c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00370
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00374
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00380
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00384
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00388
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00394
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00398
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003c0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003d0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003f4
			 0x7fc00001,                                                  // signaling NaN                               /// 003f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00400
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00404
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0040c
			 0xbf028f5c,                                                  // -0.51                                       /// 00410
			 0x0c780000,                                                  // Leading 1s:                                 /// 00414
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00418
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0041c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00420
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00424
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00434
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0043c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00440
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00448
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0044c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00450
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00460
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00468
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00470
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00474
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00478
			 0x3f028f5c,                                                  // 0.51                                        /// 0047c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00480
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00484
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0048c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00490
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0049c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0xcb000000,                                                  // -8388608.0                                  /// 004e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004f4
			 0x55555555,                                                  // 4 random values                             /// 004f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00504
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00508
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0050c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00510
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00514
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0051c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00524
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00528
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0052c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00530
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00534
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00538
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0053c
			 0x4b000000,                                                  // 8388608.0                                   /// 00540
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00550
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00558
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00560
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00564
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0056c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00570
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00578
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00580
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00584
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0058c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00590
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00594
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00598
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0059c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005c8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00604
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00608
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0060c
			 0x7f800000,                                                  // inf                                         /// 00610
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00614
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0061c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00624
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0062c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0063c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00640
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00644
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00648
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0064c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00650
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00654
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00658
			 0xbf028f5c,                                                  // -0.51                                       /// 0065c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00664
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00668
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00670
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00674
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00678
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0067c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00680
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00684
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00690
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00694
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00698
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0069c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006b0
			 0xffc00001,                                                  // -signaling NaN                              /// 006b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006cc
			 0x7f800000,                                                  // inf                                         /// 006d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d8
			 0x55555555,                                                  // 4 random values                             /// 006dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006e4
			 0xbf028f5c,                                                  // -0.51                                       /// 006e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x00000000,                                                  // zero                                        /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00700
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00704
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0070c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00710
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00714
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00718
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x55555555,                                                  // 4 random values                             /// 00720
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00724
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00728
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00730
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0xbf028f5c,                                                  // -0.51                                       /// 00738
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0073c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80011111,                                                  // -9.7958E-41                                 /// 0074c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x00000000,                                                  // zero                                        /// 00758
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0075c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00760
			 0x00000000,                                                  // zero                                        /// 00764
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00768
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0076c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00770
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00774
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0077c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00780
			 0x0e000001,                                                  // Trailing 1s:                                /// 00784
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00788
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00790
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00798
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a8
			 0xcb000000,                                                  // -8388608.0                                  /// 007ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xcb000000,                                                  // -8388608.0                                  /// 007c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d8
			 0x00000000,                                                  // zero                                        /// 007dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e0
			 0xbf028f5c,                                                  // -0.51                                       /// 007e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00800
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00804
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00808
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0080c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00810
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x0e000007,                                                  // Trailing 1s:                                /// 00818
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0081c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00820
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00824
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0082c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00830
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00834
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00838
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0083c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00840
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00844
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00848
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0084c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00850
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00858
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x3f028f5c,                                                  // 0.51                                        /// 00864
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00868
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0086c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00874
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0087c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00884
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00888
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0088c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0089c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00904
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00910
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00924
			 0x80000000,                                                  // -zero                                       /// 00928
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0092c
			 0x80000000,                                                  // -zero                                       /// 00930
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00934
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00938
			 0x0e000001,                                                  // Trailing 1s:                                /// 0093c
			 0x55555555,                                                  // 4 random values                             /// 00940
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00944
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0094c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00954
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0095c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00960
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00964
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00968
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0096c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00974
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00978
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00980
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00988
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00994
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0xff800000,                                                  // -inf                                        /// 009cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d4
			 0xbf028f5c,                                                  // -0.51                                       /// 009d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009f8
			 0x33333333,                                                  // 4 random values                             /// 009fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a00
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a38
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aa0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00aa8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ab0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ab8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00abc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ac0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ac8
			 0x7f800000,                                                  // inf                                         /// 00acc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00adc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ae0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ae8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00af0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00af4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00af8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b64
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00b70
			 0xcb000000,                                                  // -8388608.0                                  /// 00b74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b94
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bf4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bf8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bfc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c1c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x55555555,                                                  // 4 random values                             /// 00c30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c44
			 0x7fc00001,                                                  // signaling NaN                               /// 00c48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c60
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ca4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ca8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ce8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cfc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d54
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d68
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d78
			 0xcb000000,                                                  // -8388608.0                                  /// 00d7c
			 0x00000000,                                                  // zero                                        /// 00d80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d98
			 0x4b000000,                                                  // 8388608.0                                   /// 00d9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00db0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00df4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e58
			 0x55555555,                                                  // 4 random values                             /// 00e5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e74
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e7c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e8c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e90
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00eb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ec4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00edc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ee0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00eec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ef4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00efc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f08
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f10
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f28
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f40
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f70
			 0x80000000,                                                  // -zero                                       /// 00f74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f78
			 0x7fc00001,                                                  // signaling NaN                               /// 00f7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f94
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fa0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fdc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80000100                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00000
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00008
			 0x80000000,                                                  // -zero                                       /// 0000c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00010
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00014
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00018
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00020
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00024
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00028
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0002c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00030
			 0x7f800000,                                                  // inf                                         /// 00034
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0003c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00044
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00048
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0004c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00054
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0005c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00060
			 0x4b000000,                                                  // 8388608.0                                   /// 00064
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00068
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0006c
			 0x80000000,                                                  // -zero                                       /// 00070
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00074
			 0x4b000000,                                                  // 8388608.0                                   /// 00078
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0007c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00080
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00084
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00088
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00094
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b0
			 0x4b000000,                                                  // 8388608.0                                   /// 000b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0c700000,                                                  // Leading 1s:                                 /// 00104
			 0x80000000,                                                  // -zero                                       /// 00108
			 0x0c400000,                                                  // Leading 1s:                                 /// 0010c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00110
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0011c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00120
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00124
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0012c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00130
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00134
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00138
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00140
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00144
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00148
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00150
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00154
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x0e000003,                                                  // Trailing 1s:                                /// 0015c
			 0x7f800000,                                                  // inf                                         /// 00160
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00168
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00170
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00174
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0017c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00184
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0018c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00194
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0019c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x4b000000,                                                  // 8388608.0                                   /// 001d4
			 0xffc00001,                                                  // -signaling NaN                              /// 001d8
			 0x4b000000,                                                  // 8388608.0                                   /// 001dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x55555555,                                                  // 4 random values                             /// 001e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x55555555,                                                  // 4 random values                             /// 00204
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00208
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x3f028f5c,                                                  // 0.51                                        /// 00210
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00224
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00228
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0022c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00230
			 0x80011111,                                                  // -9.7958E-41                                 /// 00234
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00238
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00240
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00248
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0024c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00254
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0025c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00264
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00268
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0026c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00270
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00278
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00280
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00284
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00288
			 0x33333333,                                                  // 4 random values                             /// 0028c
			 0x7f800000,                                                  // inf                                         /// 00290
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00298
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002b0
			 0x7f800000,                                                  // inf                                         /// 002b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x00000000,                                                  // zero                                        /// 002d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x7fc00001,                                                  // signaling NaN                               /// 002ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00304
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x33333333,                                                  // 4 random values                             /// 00310
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00314
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00318
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00320
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00324
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00328
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00330
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00334
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00338
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0033c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00340
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x80011111,                                                  // -9.7958E-41                                 /// 00348
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7fc00001,                                                  // signaling NaN                               /// 00350
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0035c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00368
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0036c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00370
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00374
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0037c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00380
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00388
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0038c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00398
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0039c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 003cc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003e0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003e4
			 0x33333333,                                                  // 4 random values                             /// 003e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00400
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00404
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0040c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00410
			 0x0c700000,                                                  // Leading 1s:                                 /// 00414
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00418
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0041c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00420
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00424
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0042c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00430
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00434
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00438
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00440
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00448
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0044c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00450
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00454
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00458
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0045c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00460
			 0x0c600000,                                                  // Leading 1s:                                 /// 00464
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00468
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00470
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00478
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0047c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00480
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00484
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0048c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00494
			 0x33333333,                                                  // 4 random values                             /// 00498
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0xff800000,                                                  // -inf                                        /// 004f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00514
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00518
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0051c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00524
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00528
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0052c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00530
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00534
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00538
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0053c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00550
			 0xff800000,                                                  // -inf                                        /// 00554
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0055c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00560
			 0x7f800000,                                                  // inf                                         /// 00564
			 0x3f028f5c,                                                  // 0.51                                        /// 00568
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00578
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0057c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00580
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00584
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0058c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00590
			 0x00011111,                                                  // 9.7958E-41                                  /// 00594
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x7fc00001,                                                  // signaling NaN                               /// 005a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005b8
			 0x7fc00001,                                                  // signaling NaN                               /// 005bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d0
			 0x7fc00001,                                                  // signaling NaN                               /// 005d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80000000,                                                  // -zero                                       /// 005e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00600
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00604
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x55555555,                                                  // 4 random values                             /// 00610
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00614
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00618
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00620
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00624
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00628
			 0x0e000007,                                                  // Trailing 1s:                                /// 0062c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00630
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00634
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00638
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00648
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0064c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00650
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00658
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0065c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0x0c700000,                                                  // Leading 1s:                                 /// 00664
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00668
			 0x55555555,                                                  // 4 random values                             /// 0066c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00670
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c600000,                                                  // Leading 1s:                                 /// 00688
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00698
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0069c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006ac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00700
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00704
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00708
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00710
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0071c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00720
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00724
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00728
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0072c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00738
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0073c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00744
			 0xbf028f5c,                                                  // -0.51                                       /// 00748
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x7fc00001,                                                  // signaling NaN                               /// 00750
			 0x55555555,                                                  // 4 random values                             /// 00754
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00758
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0075c
			 0x4b000000,                                                  // 8388608.0                                   /// 00760
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00768
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00774
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00778
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0077c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00780
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00784
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0078c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00790
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00798
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0079c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00800
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00804
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00808
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0080c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00810
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00814
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00820
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00824
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0082c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00830
			 0x0c700000,                                                  // Leading 1s:                                 /// 00834
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00838
			 0x0e000007,                                                  // Trailing 1s:                                /// 0083c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00844
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0084c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00850
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00854
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0085c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00860
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x33333333,                                                  // 4 random values                             /// 00870
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00874
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00880
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00884
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0088c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00890
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00894
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00898
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 008b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 008c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0xffc00001,                                                  // -signaling NaN                              /// 008e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 008e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x55555555,                                                  // 4 random values                             /// 008f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x0e000003,                                                  // Trailing 1s:                                /// 00908
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0090c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00910
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00914
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00918
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00920
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00924
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0092c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00934
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x0e000007,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00940
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00944
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0094c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00950
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00958
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00960
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00964
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00968
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0096c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00970
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00978
			 0x3f028f5c,                                                  // 0.51                                        /// 0097c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00980
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00988
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0098c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00990
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00994
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00998
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009f4
			 0x7fc00001,                                                  // signaling NaN                               /// 009f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a00
			 0x80000000,                                                  // -zero                                       /// 00a04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x55555555,                                                  // 4 random values                             /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a90
			 0x3f028f5c,                                                  // 0.51                                        /// 00a94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00aa0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ab8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00abc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ac0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ac8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ad0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ad8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00adc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ae0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ae8
			 0x00000000,                                                  // zero                                        /// 00aec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00af8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b18
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b28
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b34
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b4c
			 0x80000000,                                                  // -zero                                       /// 00b50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b54
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0xbf028f5c,                                                  // -0.51                                       /// 00b7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b8c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ba0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ba4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ba8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00bc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bcc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd4
			 0xcb000000,                                                  // -8388608.0                                  /// 00bd8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00be0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00be4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00be8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bf4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c30
			 0x7fc00001,                                                  // signaling NaN                               /// 00c34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x3f028f5c,                                                  // 0.51                                        /// 00c6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c9c
			 0x00000000,                                                  // zero                                        /// 00ca0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ca4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ca8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cb4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ce4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cf0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cf4
			 0x7f800000,                                                  // inf                                         /// 00cf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x7fc00001,                                                  // signaling NaN                               /// 00d0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d18
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x55555555,                                                  // 4 random values                             /// 00d24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d30
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d38
			 0x3f028f5c,                                                  // 0.51                                        /// 00d3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d40
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d5c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d78
			 0x55555555,                                                  // 4 random values                             /// 00d7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00da0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0xff800000,                                                  // -inf                                        /// 00da8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00db0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00db8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dd8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00de0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00de4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00de8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00df4
			 0x7f800000,                                                  // inf                                         /// 00df8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e04
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e24
			 0xcb000000,                                                  // -8388608.0                                  /// 00e28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x4b000000,                                                  // 8388608.0                                   /// 00e48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ea4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00eb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ec0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ec4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ec8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ed0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ee8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00efc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0xcb000000,                                                  // -8388608.0                                  /// 00f04
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f0c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f70
			 0xbf028f5c,                                                  // -0.51                                       /// 00f74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f90
			 0x55555555,                                                  // 4 random values                             /// 00f94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fa4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fc4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fcc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x33333333,                                                  // 4 random values                             /// 00fec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ff0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x00000001                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xfba03ec3, /// 0x0
			 0x5e407b9a, /// 0x4
			 0x04590fe7, /// 0x8
			 0x7274901b, /// 0xc
			 0x413ebc5b, /// 0x10
			 0x26292328, /// 0x14
			 0x267aff8c, /// 0x18
			 0x5395beae, /// 0x1c
			 0x8b26f0b5, /// 0x20
			 0x7420b118, /// 0x24
			 0x41f66887, /// 0x28
			 0xc1c792b3, /// 0x2c
			 0x698d31c5, /// 0x30
			 0x9c89c53d, /// 0x34
			 0x61ddb7f4, /// 0x38
			 0x895d95bc, /// 0x3c
			 0xba02355a, /// 0x40
			 0xac203cc3, /// 0x44
			 0x79d1ce82, /// 0x48
			 0xf7dc3243, /// 0x4c
			 0x07b18a47, /// 0x50
			 0xb999c084, /// 0x54
			 0xb4daf9f8, /// 0x58
			 0x03232dd1, /// 0x5c
			 0xc40c660f, /// 0x60
			 0x47d7b197, /// 0x64
			 0x57d9c65a, /// 0x68
			 0x060f2db0, /// 0x6c
			 0xa8e33fe1, /// 0x70
			 0x66ca95dd, /// 0x74
			 0x8fa83718, /// 0x78
			 0xb0c19a8d, /// 0x7c
			 0x983ef238, /// 0x80
			 0x7999ecfa, /// 0x84
			 0x05f0d3a7, /// 0x88
			 0x28263b3d, /// 0x8c
			 0x22e7c7b6, /// 0x90
			 0xd7769372, /// 0x94
			 0xe8adba27, /// 0x98
			 0x65daf8d1, /// 0x9c
			 0x001fc33d, /// 0xa0
			 0x8fa8a0ec, /// 0xa4
			 0xed97d5eb, /// 0xa8
			 0x95abf541, /// 0xac
			 0xb2713b7d, /// 0xb0
			 0xcaf5e8ac, /// 0xb4
			 0xf40fd64f, /// 0xb8
			 0xe042c404, /// 0xbc
			 0x1914db32, /// 0xc0
			 0x91503fa3, /// 0xc4
			 0x6df297ac, /// 0xc8
			 0x4631066c, /// 0xcc
			 0x82d10249, /// 0xd0
			 0xa05c7b2c, /// 0xd4
			 0x2768e929, /// 0xd8
			 0x3a1004f7, /// 0xdc
			 0xe9f4ccae, /// 0xe0
			 0x2f5b33b2, /// 0xe4
			 0xa3967caf, /// 0xe8
			 0x72cff817, /// 0xec
			 0x672e0f8a, /// 0xf0
			 0xcd2af696, /// 0xf4
			 0x49e5d2fe, /// 0xf8
			 0x3f6bb575, /// 0xfc
			 0xdd545935, /// 0x100
			 0xdeb980c0, /// 0x104
			 0x3f161472, /// 0x108
			 0xacc78caa, /// 0x10c
			 0x3452e4ad, /// 0x110
			 0x57100ef5, /// 0x114
			 0x8ae7841b, /// 0x118
			 0x6bf61f92, /// 0x11c
			 0x3412a8e1, /// 0x120
			 0xbcbe8160, /// 0x124
			 0x90ac567c, /// 0x128
			 0x0ba579b4, /// 0x12c
			 0xaa0fb676, /// 0x130
			 0x820bdb83, /// 0x134
			 0xf1c9451d, /// 0x138
			 0x0426d8cd, /// 0x13c
			 0x15de1271, /// 0x140
			 0xfd1739e9, /// 0x144
			 0x14ca92ae, /// 0x148
			 0x9648beac, /// 0x14c
			 0xe5782c37, /// 0x150
			 0x49cd0fe5, /// 0x154
			 0x41cdb081, /// 0x158
			 0xb3731183, /// 0x15c
			 0xbab1a591, /// 0x160
			 0x7f3cea70, /// 0x164
			 0xee645a62, /// 0x168
			 0xb3702d4d, /// 0x16c
			 0xe4f61556, /// 0x170
			 0x12b7e430, /// 0x174
			 0x6d43f2c1, /// 0x178
			 0xe6e34b3e, /// 0x17c
			 0x32ddd7b9, /// 0x180
			 0xe5f7ecdd, /// 0x184
			 0xc2dceace, /// 0x188
			 0x8c20c72a, /// 0x18c
			 0x5a87ea91, /// 0x190
			 0x57d4c2b2, /// 0x194
			 0xef021754, /// 0x198
			 0xb70b9922, /// 0x19c
			 0x8431ae08, /// 0x1a0
			 0xeb2a4842, /// 0x1a4
			 0xce6308d0, /// 0x1a8
			 0x82103b05, /// 0x1ac
			 0x2a6f9515, /// 0x1b0
			 0x3c31f5be, /// 0x1b4
			 0x6edc00e7, /// 0x1b8
			 0x28d56da5, /// 0x1bc
			 0x9a98cda1, /// 0x1c0
			 0x06539291, /// 0x1c4
			 0xb69e4a22, /// 0x1c8
			 0xa916ce9c, /// 0x1cc
			 0xe5f30e06, /// 0x1d0
			 0xc25d1ec0, /// 0x1d4
			 0xb17a55bf, /// 0x1d8
			 0x9912146d, /// 0x1dc
			 0xc1440046, /// 0x1e0
			 0x7ea9ced8, /// 0x1e4
			 0x89433392, /// 0x1e8
			 0x6467b250, /// 0x1ec
			 0xf25fe1f6, /// 0x1f0
			 0x5687ef9c, /// 0x1f4
			 0x45d1a2be, /// 0x1f8
			 0x8435ac8e, /// 0x1fc
			 0xaf22fce9, /// 0x200
			 0xd11d52ae, /// 0x204
			 0xbb270f84, /// 0x208
			 0xefe789e6, /// 0x20c
			 0x416ed985, /// 0x210
			 0x273f21e8, /// 0x214
			 0xe6c7a852, /// 0x218
			 0x156eef83, /// 0x21c
			 0x02232af7, /// 0x220
			 0xf9dc4ac6, /// 0x224
			 0xf2c914a6, /// 0x228
			 0x038f9aef, /// 0x22c
			 0xff52d84c, /// 0x230
			 0xa26f3061, /// 0x234
			 0x6df4d16d, /// 0x238
			 0xd49e11d8, /// 0x23c
			 0xe32491f2, /// 0x240
			 0xc25bf51e, /// 0x244
			 0x8aa532f9, /// 0x248
			 0x5b287571, /// 0x24c
			 0xdb586d54, /// 0x250
			 0xfb830117, /// 0x254
			 0x4c6ae4e2, /// 0x258
			 0x25e4857e, /// 0x25c
			 0x3e169b10, /// 0x260
			 0x1ca7e1f9, /// 0x264
			 0x14955056, /// 0x268
			 0x01989976, /// 0x26c
			 0xe8c7e843, /// 0x270
			 0xda4e1ed8, /// 0x274
			 0xd76a832f, /// 0x278
			 0x671b870c, /// 0x27c
			 0xfa7dbdba, /// 0x280
			 0x5f32201c, /// 0x284
			 0x17a36a5b, /// 0x288
			 0xe34da84d, /// 0x28c
			 0x997ec256, /// 0x290
			 0xaf251769, /// 0x294
			 0xd21c5cab, /// 0x298
			 0xad6f88db, /// 0x29c
			 0x9c40e800, /// 0x2a0
			 0xac5211cc, /// 0x2a4
			 0xd56e01b5, /// 0x2a8
			 0xf8946890, /// 0x2ac
			 0x6917c706, /// 0x2b0
			 0x143f5353, /// 0x2b4
			 0x2480fed9, /// 0x2b8
			 0x0de7ee51, /// 0x2bc
			 0x93ec81aa, /// 0x2c0
			 0x790af2ac, /// 0x2c4
			 0x2c514b52, /// 0x2c8
			 0x267f7778, /// 0x2cc
			 0x4d1131f0, /// 0x2d0
			 0x53436e30, /// 0x2d4
			 0x138071a1, /// 0x2d8
			 0xbc42a3b5, /// 0x2dc
			 0x116f6fc1, /// 0x2e0
			 0x316eb0d1, /// 0x2e4
			 0xacf848f4, /// 0x2e8
			 0x94609271, /// 0x2ec
			 0xeb4b8a3e, /// 0x2f0
			 0x62442bc5, /// 0x2f4
			 0xa92f0230, /// 0x2f8
			 0xadbc8e0b, /// 0x2fc
			 0x38e1b428, /// 0x300
			 0x3b5c9da3, /// 0x304
			 0xe6ab14f5, /// 0x308
			 0x1b572279, /// 0x30c
			 0x090bf842, /// 0x310
			 0x705ba9ef, /// 0x314
			 0x5e447a37, /// 0x318
			 0x3c726da2, /// 0x31c
			 0x13e13045, /// 0x320
			 0x9ebb16fe, /// 0x324
			 0xf4e9a84a, /// 0x328
			 0xe9028fc3, /// 0x32c
			 0x4660d1bc, /// 0x330
			 0x97cde2bf, /// 0x334
			 0xcd2afa83, /// 0x338
			 0xb7da4273, /// 0x33c
			 0x8ce2cbdd, /// 0x340
			 0x0d939035, /// 0x344
			 0xae0b937b, /// 0x348
			 0x1d9f21c1, /// 0x34c
			 0x8fe1e17e, /// 0x350
			 0x0a08eb8c, /// 0x354
			 0xf118f10d, /// 0x358
			 0xe16362bc, /// 0x35c
			 0x32c3851f, /// 0x360
			 0xf4473609, /// 0x364
			 0x928ed4fb, /// 0x368
			 0x1d3b0bc2, /// 0x36c
			 0x17b46c85, /// 0x370
			 0x98789127, /// 0x374
			 0x8d8981ee, /// 0x378
			 0x6e0f670b, /// 0x37c
			 0xef8ed19c, /// 0x380
			 0xe4eb5a11, /// 0x384
			 0x3b11a6bb, /// 0x388
			 0x48078c14, /// 0x38c
			 0x60fccba4, /// 0x390
			 0x0c720db7, /// 0x394
			 0x792c8417, /// 0x398
			 0x4508beb7, /// 0x39c
			 0x67e0d4a1, /// 0x3a0
			 0xea6fb8af, /// 0x3a4
			 0xf8f70446, /// 0x3a8
			 0xb63f4fb5, /// 0x3ac
			 0x8789ff15, /// 0x3b0
			 0x98edd1aa, /// 0x3b4
			 0x9d10f568, /// 0x3b8
			 0x67970aa4, /// 0x3bc
			 0xd7d9a5fa, /// 0x3c0
			 0xa9e268c7, /// 0x3c4
			 0x2dc036eb, /// 0x3c8
			 0xba6eda0f, /// 0x3cc
			 0x403b858c, /// 0x3d0
			 0x053d88fc, /// 0x3d4
			 0x2c65c51b, /// 0x3d8
			 0xdab69dda, /// 0x3dc
			 0x7c6617de, /// 0x3e0
			 0x61a96d68, /// 0x3e4
			 0x38e76500, /// 0x3e8
			 0x1fb89b94, /// 0x3ec
			 0x30c8c96f, /// 0x3f0
			 0x47a5d34f, /// 0x3f4
			 0x18f77347, /// 0x3f8
			 0xa3d25bc3, /// 0x3fc
			 0xb084ebb0, /// 0x400
			 0x94449ef8, /// 0x404
			 0x802f2a2d, /// 0x408
			 0x904ea9aa, /// 0x40c
			 0x6058f711, /// 0x410
			 0x8578600e, /// 0x414
			 0xaaad69e6, /// 0x418
			 0x178aa0f2, /// 0x41c
			 0xc6ddaaff, /// 0x420
			 0xc1bd7826, /// 0x424
			 0xa5c788af, /// 0x428
			 0xc0764219, /// 0x42c
			 0xf328c314, /// 0x430
			 0xb4546510, /// 0x434
			 0xbacd730a, /// 0x438
			 0x365e4be9, /// 0x43c
			 0x04e25e93, /// 0x440
			 0xd0b7c13b, /// 0x444
			 0x3a168941, /// 0x448
			 0x163669b8, /// 0x44c
			 0x6a0edebe, /// 0x450
			 0x49fd3d83, /// 0x454
			 0xa4853cc5, /// 0x458
			 0x6205d57b, /// 0x45c
			 0x5068d43c, /// 0x460
			 0x13166a37, /// 0x464
			 0x909be0f1, /// 0x468
			 0x5e20143b, /// 0x46c
			 0x44591ea9, /// 0x470
			 0xdb3524b3, /// 0x474
			 0x448df3fb, /// 0x478
			 0x99f2acfe, /// 0x47c
			 0x6d2cba1f, /// 0x480
			 0x2e2f1664, /// 0x484
			 0xd26d4259, /// 0x488
			 0x3d2c619e, /// 0x48c
			 0x27603420, /// 0x490
			 0x9531314e, /// 0x494
			 0x71d1586f, /// 0x498
			 0xb4e4fa0f, /// 0x49c
			 0xa77811dd, /// 0x4a0
			 0x23025aff, /// 0x4a4
			 0x6e7d9b3d, /// 0x4a8
			 0x2e7f0d3a, /// 0x4ac
			 0xe3b9f6a8, /// 0x4b0
			 0x46721fd8, /// 0x4b4
			 0x44fc5c9d, /// 0x4b8
			 0x5b0611c8, /// 0x4bc
			 0x29cb3d08, /// 0x4c0
			 0x743be402, /// 0x4c4
			 0xe5288dad, /// 0x4c8
			 0xd6a30ff9, /// 0x4cc
			 0x7e3546f8, /// 0x4d0
			 0xc864674f, /// 0x4d4
			 0x3e4788bb, /// 0x4d8
			 0x2e687007, /// 0x4dc
			 0xbc1de711, /// 0x4e0
			 0x0166d058, /// 0x4e4
			 0x617fa3a5, /// 0x4e8
			 0xc0519cd6, /// 0x4ec
			 0x48169e1d, /// 0x4f0
			 0xc1aa65bb, /// 0x4f4
			 0x0b41ef6f, /// 0x4f8
			 0xef1a37c3, /// 0x4fc
			 0x71eda568, /// 0x500
			 0x16dcd242, /// 0x504
			 0xf2fe4a0b, /// 0x508
			 0x6f856dd8, /// 0x50c
			 0x1f06ec1b, /// 0x510
			 0x96c7a338, /// 0x514
			 0xb1369d7f, /// 0x518
			 0x79c4c6f0, /// 0x51c
			 0x6c45b470, /// 0x520
			 0xc8fabb68, /// 0x524
			 0xb1d0ca56, /// 0x528
			 0xfd86ca74, /// 0x52c
			 0x3b48639f, /// 0x530
			 0xb0bce701, /// 0x534
			 0x9e392b7b, /// 0x538
			 0x414d6826, /// 0x53c
			 0x61ddaa9b, /// 0x540
			 0x134353b4, /// 0x544
			 0xc507b228, /// 0x548
			 0x050a9d92, /// 0x54c
			 0xb2eb3129, /// 0x550
			 0x438cf384, /// 0x554
			 0xaa37ef00, /// 0x558
			 0x3710342c, /// 0x55c
			 0xf682227d, /// 0x560
			 0x156a1526, /// 0x564
			 0x2d8a25f8, /// 0x568
			 0xf69ef49d, /// 0x56c
			 0x9fc298e2, /// 0x570
			 0xc9988523, /// 0x574
			 0xecee59e4, /// 0x578
			 0xdbbecad0, /// 0x57c
			 0x20d41871, /// 0x580
			 0xa5cb871f, /// 0x584
			 0x5d60ed7f, /// 0x588
			 0x10c0bf49, /// 0x58c
			 0x10924d6e, /// 0x590
			 0x0c6a0917, /// 0x594
			 0xd9c7c1a6, /// 0x598
			 0xa52c6eda, /// 0x59c
			 0x29472702, /// 0x5a0
			 0x2e68edfa, /// 0x5a4
			 0xa475d600, /// 0x5a8
			 0x9955d529, /// 0x5ac
			 0x90da3b2b, /// 0x5b0
			 0x913cfcd4, /// 0x5b4
			 0x6f38fdc8, /// 0x5b8
			 0x8357ae46, /// 0x5bc
			 0x21aa3440, /// 0x5c0
			 0x83f8e09e, /// 0x5c4
			 0x6c7fd782, /// 0x5c8
			 0x92679588, /// 0x5cc
			 0x52d01cc1, /// 0x5d0
			 0xcb0f1fc8, /// 0x5d4
			 0xe89df44b, /// 0x5d8
			 0x81f3371b, /// 0x5dc
			 0xdff6c022, /// 0x5e0
			 0x1745cdc5, /// 0x5e4
			 0x768aa388, /// 0x5e8
			 0x9e6f6fb5, /// 0x5ec
			 0xef1dc504, /// 0x5f0
			 0x98fcf1c6, /// 0x5f4
			 0xa46c9222, /// 0x5f8
			 0x18929fdc, /// 0x5fc
			 0xe7c788da, /// 0x600
			 0x0b2906e2, /// 0x604
			 0xe40dd71a, /// 0x608
			 0xc5859dd7, /// 0x60c
			 0xc4403a62, /// 0x610
			 0x5dde8d51, /// 0x614
			 0x3bf3f1f7, /// 0x618
			 0xbf5e430f, /// 0x61c
			 0xcee090cc, /// 0x620
			 0xb52a823d, /// 0x624
			 0xf5b367cc, /// 0x628
			 0x4db7f3d0, /// 0x62c
			 0xb69f252a, /// 0x630
			 0x1cede62f, /// 0x634
			 0xb843500a, /// 0x638
			 0x47b7e340, /// 0x63c
			 0xb97f9bd9, /// 0x640
			 0x9492895c, /// 0x644
			 0x99e238d9, /// 0x648
			 0xef4ecdda, /// 0x64c
			 0xcab66806, /// 0x650
			 0xcf9f7e8a, /// 0x654
			 0x3add38f4, /// 0x658
			 0x34d48cac, /// 0x65c
			 0x031349f8, /// 0x660
			 0x2db2d9ae, /// 0x664
			 0x693456f1, /// 0x668
			 0x44a154c8, /// 0x66c
			 0x0123277f, /// 0x670
			 0x2a14bab3, /// 0x674
			 0x6d3e8569, /// 0x678
			 0x27de73e6, /// 0x67c
			 0x3c74c326, /// 0x680
			 0x64684421, /// 0x684
			 0x987c8439, /// 0x688
			 0x9eece549, /// 0x68c
			 0xae40ccf1, /// 0x690
			 0xb5243204, /// 0x694
			 0xb96ddd1a, /// 0x698
			 0x478026c4, /// 0x69c
			 0x76547ec6, /// 0x6a0
			 0x94b414f3, /// 0x6a4
			 0xfa4197c1, /// 0x6a8
			 0x70a3f458, /// 0x6ac
			 0x4f25a788, /// 0x6b0
			 0x47a36cb0, /// 0x6b4
			 0xb8cecc03, /// 0x6b8
			 0xb3f178ad, /// 0x6bc
			 0xe58089d6, /// 0x6c0
			 0x45b18ed5, /// 0x6c4
			 0xa8f5547f, /// 0x6c8
			 0x2c088ac1, /// 0x6cc
			 0xfa0da59f, /// 0x6d0
			 0x51fbf78e, /// 0x6d4
			 0x21e10827, /// 0x6d8
			 0xf9ae1d17, /// 0x6dc
			 0x5d9b519f, /// 0x6e0
			 0xca1d1bee, /// 0x6e4
			 0x18ae411d, /// 0x6e8
			 0x9075f722, /// 0x6ec
			 0x71ff040f, /// 0x6f0
			 0x75170437, /// 0x6f4
			 0xfc4789e8, /// 0x6f8
			 0xa8cae6f4, /// 0x6fc
			 0xc88504cc, /// 0x700
			 0x71a20393, /// 0x704
			 0xa67f1bf8, /// 0x708
			 0x5a884fec, /// 0x70c
			 0xa8093f19, /// 0x710
			 0x531acfcf, /// 0x714
			 0x171cfe3f, /// 0x718
			 0x3fc597bc, /// 0x71c
			 0x984ead8f, /// 0x720
			 0x95a80c74, /// 0x724
			 0xe17a6a6d, /// 0x728
			 0x1a1d9757, /// 0x72c
			 0xe4f43f8b, /// 0x730
			 0x94974337, /// 0x734
			 0x5a534aa3, /// 0x738
			 0xfdc51389, /// 0x73c
			 0xd5c7e99d, /// 0x740
			 0x62b8e16c, /// 0x744
			 0xae3731c8, /// 0x748
			 0xd7a18a99, /// 0x74c
			 0x6f2d98d9, /// 0x750
			 0xf4e5142b, /// 0x754
			 0x1d39b47d, /// 0x758
			 0x59fabae3, /// 0x75c
			 0x90c7ff8e, /// 0x760
			 0xd3ee8bf6, /// 0x764
			 0xdfe5163a, /// 0x768
			 0xeca88436, /// 0x76c
			 0x86d57cf6, /// 0x770
			 0x2b671c88, /// 0x774
			 0x6b30cd56, /// 0x778
			 0x3f79633c, /// 0x77c
			 0x7b4ba79f, /// 0x780
			 0xc3e22916, /// 0x784
			 0x7a743240, /// 0x788
			 0x3a3252e2, /// 0x78c
			 0x1f8f673b, /// 0x790
			 0x9cd75921, /// 0x794
			 0x18362992, /// 0x798
			 0x24c0a378, /// 0x79c
			 0x5371386a, /// 0x7a0
			 0x4be75487, /// 0x7a4
			 0xbdbf69c1, /// 0x7a8
			 0x3cb952b8, /// 0x7ac
			 0xb15f6b1f, /// 0x7b0
			 0xadc7b06c, /// 0x7b4
			 0x4aebaae9, /// 0x7b8
			 0x6456fd38, /// 0x7bc
			 0x7f18d9ad, /// 0x7c0
			 0x37ba4317, /// 0x7c4
			 0xa8c7cad7, /// 0x7c8
			 0x1f0b0190, /// 0x7cc
			 0xd152ff7a, /// 0x7d0
			 0x25f5112b, /// 0x7d4
			 0x1e696efc, /// 0x7d8
			 0xd3e870d3, /// 0x7dc
			 0xc4a19cc9, /// 0x7e0
			 0x4893e1d5, /// 0x7e4
			 0x48e4524f, /// 0x7e8
			 0x3b9bdd11, /// 0x7ec
			 0x723b9633, /// 0x7f0
			 0xe3482e71, /// 0x7f4
			 0x5eaac28e, /// 0x7f8
			 0x74453d24, /// 0x7fc
			 0x0ba16eb0, /// 0x800
			 0xfe533ace, /// 0x804
			 0xcfe48759, /// 0x808
			 0x79769d9a, /// 0x80c
			 0xc7d843b7, /// 0x810
			 0x46e6763f, /// 0x814
			 0xd30e3bbe, /// 0x818
			 0x77e5fb71, /// 0x81c
			 0x766cc634, /// 0x820
			 0xb85f6ab1, /// 0x824
			 0xcb05ea11, /// 0x828
			 0x2cd72e9f, /// 0x82c
			 0x7768c9fc, /// 0x830
			 0xe2ac748a, /// 0x834
			 0x1a3c8a53, /// 0x838
			 0xf489794b, /// 0x83c
			 0xb8fe5dc8, /// 0x840
			 0xf18ee515, /// 0x844
			 0x4daef4e7, /// 0x848
			 0x44dbf6b6, /// 0x84c
			 0xd8ade0d9, /// 0x850
			 0x2055f4f9, /// 0x854
			 0xeed93f59, /// 0x858
			 0x6db5eb5a, /// 0x85c
			 0xa866f4e2, /// 0x860
			 0xad2e4730, /// 0x864
			 0x0e0368d2, /// 0x868
			 0x1405c2fd, /// 0x86c
			 0x34c0d3df, /// 0x870
			 0x741cde21, /// 0x874
			 0xf69b75e8, /// 0x878
			 0x6e44d54f, /// 0x87c
			 0x3e15bab2, /// 0x880
			 0x4a068107, /// 0x884
			 0xb88f2be6, /// 0x888
			 0xa5519350, /// 0x88c
			 0xbebe007b, /// 0x890
			 0x22827ff8, /// 0x894
			 0x0a2bb3e8, /// 0x898
			 0x5d2399af, /// 0x89c
			 0x146c5a61, /// 0x8a0
			 0x3ff33da6, /// 0x8a4
			 0xc919d438, /// 0x8a8
			 0x45805bce, /// 0x8ac
			 0xad4ea581, /// 0x8b0
			 0xe4af9daa, /// 0x8b4
			 0x006abb56, /// 0x8b8
			 0x7162d4f2, /// 0x8bc
			 0xa83df63a, /// 0x8c0
			 0x6f1edd64, /// 0x8c4
			 0xb5f41e06, /// 0x8c8
			 0x1b4bd704, /// 0x8cc
			 0x7c1ad74e, /// 0x8d0
			 0xd35fadd9, /// 0x8d4
			 0xebc72d32, /// 0x8d8
			 0x75a09a0c, /// 0x8dc
			 0x5e73108c, /// 0x8e0
			 0xd71c6f15, /// 0x8e4
			 0xadf9a91b, /// 0x8e8
			 0xb70b48fe, /// 0x8ec
			 0x02d5228d, /// 0x8f0
			 0x9d5e16c4, /// 0x8f4
			 0x73081b6a, /// 0x8f8
			 0x327cb843, /// 0x8fc
			 0x04034b68, /// 0x900
			 0x7eddaa0f, /// 0x904
			 0x8181693d, /// 0x908
			 0x4fe37ad0, /// 0x90c
			 0x3e988e8c, /// 0x910
			 0xce34f478, /// 0x914
			 0x03b70d5c, /// 0x918
			 0xdaecea90, /// 0x91c
			 0x08e1e4f0, /// 0x920
			 0x4e218d93, /// 0x924
			 0xf27b8524, /// 0x928
			 0xe49bf7ee, /// 0x92c
			 0xca794924, /// 0x930
			 0x8848000b, /// 0x934
			 0x6c8a204f, /// 0x938
			 0x241e6747, /// 0x93c
			 0x5910079f, /// 0x940
			 0x3bf1f308, /// 0x944
			 0x23fcef03, /// 0x948
			 0x8f98786f, /// 0x94c
			 0x3a89cb6a, /// 0x950
			 0xcfdabc36, /// 0x954
			 0x8773d215, /// 0x958
			 0xc685d8a3, /// 0x95c
			 0xa445919c, /// 0x960
			 0xc69b6a4e, /// 0x964
			 0x6fce1a64, /// 0x968
			 0x9406f9ef, /// 0x96c
			 0xcce292cb, /// 0x970
			 0x688525a9, /// 0x974
			 0x428c177b, /// 0x978
			 0xc6a051f3, /// 0x97c
			 0x36c77906, /// 0x980
			 0x518a4445, /// 0x984
			 0xeb626424, /// 0x988
			 0xe5918d2f, /// 0x98c
			 0xd81abd02, /// 0x990
			 0x03d7e863, /// 0x994
			 0x3941f02e, /// 0x998
			 0xc7e84d0d, /// 0x99c
			 0x47e9ee48, /// 0x9a0
			 0xc859a08f, /// 0x9a4
			 0x326f3e08, /// 0x9a8
			 0x9ed2cbf4, /// 0x9ac
			 0x8c44f612, /// 0x9b0
			 0x11666b30, /// 0x9b4
			 0x7290db30, /// 0x9b8
			 0x340fb798, /// 0x9bc
			 0xf0c62b64, /// 0x9c0
			 0x0169382a, /// 0x9c4
			 0x8e4259ed, /// 0x9c8
			 0xe02bf681, /// 0x9cc
			 0x4923a5f7, /// 0x9d0
			 0xa664f890, /// 0x9d4
			 0x33cc443c, /// 0x9d8
			 0xd4393034, /// 0x9dc
			 0xb7ea5df0, /// 0x9e0
			 0x014b56ed, /// 0x9e4
			 0x3c713641, /// 0x9e8
			 0x04da1848, /// 0x9ec
			 0x534ba571, /// 0x9f0
			 0x486f944f, /// 0x9f4
			 0xb556762c, /// 0x9f8
			 0xdc2eb5f1, /// 0x9fc
			 0x953fd84c, /// 0xa00
			 0xfc1cc94d, /// 0xa04
			 0xaaaff219, /// 0xa08
			 0xeb7be510, /// 0xa0c
			 0x369ef34f, /// 0xa10
			 0xd6d2a245, /// 0xa14
			 0x974fc7e2, /// 0xa18
			 0x1ac0e5b4, /// 0xa1c
			 0xe05ceb87, /// 0xa20
			 0xdaa30300, /// 0xa24
			 0x379eaf7b, /// 0xa28
			 0xfb8cb637, /// 0xa2c
			 0x921834e5, /// 0xa30
			 0xc48fc6be, /// 0xa34
			 0x3506ca8f, /// 0xa38
			 0x48327df5, /// 0xa3c
			 0x127b5bca, /// 0xa40
			 0x5d9068d7, /// 0xa44
			 0x536e804d, /// 0xa48
			 0x46014b49, /// 0xa4c
			 0xe9ef553f, /// 0xa50
			 0xc477662d, /// 0xa54
			 0x8a72fb80, /// 0xa58
			 0xc11239e0, /// 0xa5c
			 0x9ec88e38, /// 0xa60
			 0x8ffb0a52, /// 0xa64
			 0x45632cdc, /// 0xa68
			 0xab063e9d, /// 0xa6c
			 0xc8ea5e36, /// 0xa70
			 0xff11fba3, /// 0xa74
			 0xa1ca51ca, /// 0xa78
			 0xbb572a7f, /// 0xa7c
			 0x0f972c22, /// 0xa80
			 0x4c941c2d, /// 0xa84
			 0x499e9899, /// 0xa88
			 0x071ed657, /// 0xa8c
			 0x59942e74, /// 0xa90
			 0xfe64a820, /// 0xa94
			 0x54085c78, /// 0xa98
			 0xfca61c1c, /// 0xa9c
			 0x11db03d8, /// 0xaa0
			 0x83610e55, /// 0xaa4
			 0x05246293, /// 0xaa8
			 0x8344856f, /// 0xaac
			 0x6e4853ea, /// 0xab0
			 0x679429cd, /// 0xab4
			 0x0d7696c7, /// 0xab8
			 0xb650468a, /// 0xabc
			 0x8dc4209e, /// 0xac0
			 0xf1f4588f, /// 0xac4
			 0x9457aa14, /// 0xac8
			 0xc0e1ca1e, /// 0xacc
			 0xc25236e7, /// 0xad0
			 0x30a73163, /// 0xad4
			 0xe65815af, /// 0xad8
			 0xaa55d59d, /// 0xadc
			 0x03a41c7f, /// 0xae0
			 0x626685c6, /// 0xae4
			 0x2e155f39, /// 0xae8
			 0x19b5a8ec, /// 0xaec
			 0xcd1ae70e, /// 0xaf0
			 0x3bc2c50f, /// 0xaf4
			 0x493db9a4, /// 0xaf8
			 0xf679203a, /// 0xafc
			 0x09496390, /// 0xb00
			 0x05bef684, /// 0xb04
			 0xb462fb1b, /// 0xb08
			 0x7dd60d52, /// 0xb0c
			 0xf4f20f7b, /// 0xb10
			 0x86a8d6b6, /// 0xb14
			 0x0f8c9107, /// 0xb18
			 0x0ff064c5, /// 0xb1c
			 0x196c314c, /// 0xb20
			 0xcc5712d4, /// 0xb24
			 0x851cedb9, /// 0xb28
			 0x25c5a7d5, /// 0xb2c
			 0x74e5f70a, /// 0xb30
			 0xd232382c, /// 0xb34
			 0x5a25d254, /// 0xb38
			 0x26817ca8, /// 0xb3c
			 0x9750c4e8, /// 0xb40
			 0xbf5fcc95, /// 0xb44
			 0xdba9302d, /// 0xb48
			 0xdbc42635, /// 0xb4c
			 0xa7874320, /// 0xb50
			 0xf70ce6aa, /// 0xb54
			 0x9831ff7e, /// 0xb58
			 0x1a41643e, /// 0xb5c
			 0x5f0a9f49, /// 0xb60
			 0x29f783e8, /// 0xb64
			 0xb0e0e8cd, /// 0xb68
			 0x2c068f9c, /// 0xb6c
			 0x6b996bd2, /// 0xb70
			 0x90e51115, /// 0xb74
			 0x55e9e39f, /// 0xb78
			 0xbbfa3860, /// 0xb7c
			 0x7ef21012, /// 0xb80
			 0x311f29d2, /// 0xb84
			 0x404c8637, /// 0xb88
			 0x8d032cbc, /// 0xb8c
			 0xb7530b22, /// 0xb90
			 0x9440ab17, /// 0xb94
			 0x80ebc5e3, /// 0xb98
			 0x236ecad4, /// 0xb9c
			 0x68f1341e, /// 0xba0
			 0x3ebfa879, /// 0xba4
			 0x3ad6df51, /// 0xba8
			 0x86d67195, /// 0xbac
			 0x5af0031f, /// 0xbb0
			 0x5ce8822e, /// 0xbb4
			 0x662d139c, /// 0xbb8
			 0x42f00618, /// 0xbbc
			 0xddb26acb, /// 0xbc0
			 0xa9ade385, /// 0xbc4
			 0xa761752c, /// 0xbc8
			 0xaf7c3175, /// 0xbcc
			 0xf8228c05, /// 0xbd0
			 0xfdc4bbd3, /// 0xbd4
			 0x7f72c939, /// 0xbd8
			 0x11485da7, /// 0xbdc
			 0xb04e6400, /// 0xbe0
			 0xfc994189, /// 0xbe4
			 0x62e05886, /// 0xbe8
			 0xde5db272, /// 0xbec
			 0x5a584022, /// 0xbf0
			 0x269708eb, /// 0xbf4
			 0x6478d2b1, /// 0xbf8
			 0xf657af57, /// 0xbfc
			 0x852bc6c8, /// 0xc00
			 0x27a2076e, /// 0xc04
			 0x38badd84, /// 0xc08
			 0x7e2ee7dd, /// 0xc0c
			 0x6d15e7d7, /// 0xc10
			 0x3395ac35, /// 0xc14
			 0x8e897580, /// 0xc18
			 0x8463f9fa, /// 0xc1c
			 0x190ab462, /// 0xc20
			 0x17f2845e, /// 0xc24
			 0x5e279d17, /// 0xc28
			 0x75e9f82c, /// 0xc2c
			 0xeaac6a58, /// 0xc30
			 0x92bfa90e, /// 0xc34
			 0x2c91b0af, /// 0xc38
			 0xb4b6c514, /// 0xc3c
			 0xf88eae72, /// 0xc40
			 0x29975c7b, /// 0xc44
			 0xa976bcf2, /// 0xc48
			 0xe048962d, /// 0xc4c
			 0x66115696, /// 0xc50
			 0x09bdb506, /// 0xc54
			 0xaafd80cf, /// 0xc58
			 0x8ee3dfff, /// 0xc5c
			 0x3800682e, /// 0xc60
			 0xfb4dfa01, /// 0xc64
			 0x33710181, /// 0xc68
			 0x8dddf90c, /// 0xc6c
			 0xa88eef89, /// 0xc70
			 0x26a8e59b, /// 0xc74
			 0x243c7b9f, /// 0xc78
			 0xebfd914d, /// 0xc7c
			 0x0716036a, /// 0xc80
			 0x9019e194, /// 0xc84
			 0x2dc92a66, /// 0xc88
			 0xe5ea77ee, /// 0xc8c
			 0x3e20435b, /// 0xc90
			 0x17684d80, /// 0xc94
			 0xe5bea588, /// 0xc98
			 0x366f1e7d, /// 0xc9c
			 0x5291029d, /// 0xca0
			 0x0d3ec42f, /// 0xca4
			 0xa785c7ad, /// 0xca8
			 0x33c4fcf3, /// 0xcac
			 0x96515f73, /// 0xcb0
			 0xf477066d, /// 0xcb4
			 0x7886e4a6, /// 0xcb8
			 0x2bffbb2c, /// 0xcbc
			 0x676eb033, /// 0xcc0
			 0xb447f7df, /// 0xcc4
			 0x3fcdd0f1, /// 0xcc8
			 0x236cb528, /// 0xccc
			 0xf2cbc393, /// 0xcd0
			 0x524fe0d5, /// 0xcd4
			 0xa7da543b, /// 0xcd8
			 0xafff9899, /// 0xcdc
			 0xf3d9f9a5, /// 0xce0
			 0x06ef1da6, /// 0xce4
			 0x24d49d26, /// 0xce8
			 0xd1dd05ef, /// 0xcec
			 0xe33cccf4, /// 0xcf0
			 0x6365755f, /// 0xcf4
			 0xb2b796cd, /// 0xcf8
			 0xd201427d, /// 0xcfc
			 0x90f849c4, /// 0xd00
			 0xaa321a1d, /// 0xd04
			 0xd3c3843f, /// 0xd08
			 0x3f784678, /// 0xd0c
			 0x28a3144b, /// 0xd10
			 0xfc49935b, /// 0xd14
			 0xdfaeecc2, /// 0xd18
			 0x9dbc2b75, /// 0xd1c
			 0x2cc69509, /// 0xd20
			 0xdc57dd26, /// 0xd24
			 0x630bd49a, /// 0xd28
			 0x5a9a4b19, /// 0xd2c
			 0x8621b662, /// 0xd30
			 0xee8a4338, /// 0xd34
			 0x296a52c4, /// 0xd38
			 0x8aa3339a, /// 0xd3c
			 0x50da9c46, /// 0xd40
			 0x9720a313, /// 0xd44
			 0x1869e92e, /// 0xd48
			 0xb5e8bc7f, /// 0xd4c
			 0x4b8f75c9, /// 0xd50
			 0x3789e113, /// 0xd54
			 0xc9071494, /// 0xd58
			 0x55b7c09c, /// 0xd5c
			 0x02f4746c, /// 0xd60
			 0x891da6d2, /// 0xd64
			 0x5196b81a, /// 0xd68
			 0x4a4c984e, /// 0xd6c
			 0x3fb17835, /// 0xd70
			 0x44253a1e, /// 0xd74
			 0xddec5ce0, /// 0xd78
			 0x1a5a3c52, /// 0xd7c
			 0xfaff1ccf, /// 0xd80
			 0xfa004725, /// 0xd84
			 0xe592e0d9, /// 0xd88
			 0x2130f108, /// 0xd8c
			 0xbba87add, /// 0xd90
			 0x08c1cdf7, /// 0xd94
			 0x6895aabf, /// 0xd98
			 0xb76342d6, /// 0xd9c
			 0x8b57638b, /// 0xda0
			 0x6ec2cfbe, /// 0xda4
			 0xcd6c06b5, /// 0xda8
			 0xdf20cfd0, /// 0xdac
			 0xb8cdda81, /// 0xdb0
			 0x5abefb46, /// 0xdb4
			 0x1e08acf7, /// 0xdb8
			 0xb3793433, /// 0xdbc
			 0xec369241, /// 0xdc0
			 0x4a0b2653, /// 0xdc4
			 0x632036dd, /// 0xdc8
			 0xfe0ed182, /// 0xdcc
			 0x677eb256, /// 0xdd0
			 0x2a34d7a3, /// 0xdd4
			 0x9cff6708, /// 0xdd8
			 0x3e7ebe34, /// 0xddc
			 0x7b039834, /// 0xde0
			 0x119de0a2, /// 0xde4
			 0x73599967, /// 0xde8
			 0xb2867090, /// 0xdec
			 0x72f2f230, /// 0xdf0
			 0xfad14183, /// 0xdf4
			 0x68319d0f, /// 0xdf8
			 0x601d1fae, /// 0xdfc
			 0x35465c74, /// 0xe00
			 0x12372a85, /// 0xe04
			 0x47ec7ffd, /// 0xe08
			 0x870aa4bc, /// 0xe0c
			 0x71913995, /// 0xe10
			 0x7eac9640, /// 0xe14
			 0xd134141a, /// 0xe18
			 0x7796e0ad, /// 0xe1c
			 0x6c8fd25c, /// 0xe20
			 0xd661f295, /// 0xe24
			 0xdfc767cd, /// 0xe28
			 0xf9426010, /// 0xe2c
			 0x9c273e51, /// 0xe30
			 0xd61d2be9, /// 0xe34
			 0xd0935149, /// 0xe38
			 0x994d3581, /// 0xe3c
			 0xfe77e4d6, /// 0xe40
			 0x11c599a2, /// 0xe44
			 0x9efbc816, /// 0xe48
			 0x8c8dc709, /// 0xe4c
			 0xd7209313, /// 0xe50
			 0x7e03e425, /// 0xe54
			 0xc8dcad6e, /// 0xe58
			 0x81d5d555, /// 0xe5c
			 0x06596da5, /// 0xe60
			 0x58519147, /// 0xe64
			 0x851c1346, /// 0xe68
			 0xe6348d31, /// 0xe6c
			 0x45e8936d, /// 0xe70
			 0x7683d713, /// 0xe74
			 0x9a0a0cfb, /// 0xe78
			 0x156b10dc, /// 0xe7c
			 0xc5c882ce, /// 0xe80
			 0x21b4cda4, /// 0xe84
			 0xfaceb25d, /// 0xe88
			 0xc65ca464, /// 0xe8c
			 0x84d3352e, /// 0xe90
			 0x658648fe, /// 0xe94
			 0xc8497c23, /// 0xe98
			 0x842a583c, /// 0xe9c
			 0x2bd1ccdb, /// 0xea0
			 0x93b86bf3, /// 0xea4
			 0x292b3c72, /// 0xea8
			 0xc190eefe, /// 0xeac
			 0x3eeee8d5, /// 0xeb0
			 0x57417748, /// 0xeb4
			 0x1aebe429, /// 0xeb8
			 0x3e43d532, /// 0xebc
			 0xd91e34a9, /// 0xec0
			 0xd43fa1de, /// 0xec4
			 0x504d6bd3, /// 0xec8
			 0xb4304e1a, /// 0xecc
			 0x5d1f55ad, /// 0xed0
			 0xe6385e88, /// 0xed4
			 0x634506f6, /// 0xed8
			 0x1d86a9d9, /// 0xedc
			 0x587ab88c, /// 0xee0
			 0x422b2404, /// 0xee4
			 0x8643f3c9, /// 0xee8
			 0xac8dd987, /// 0xeec
			 0x6c55485a, /// 0xef0
			 0x4efb4419, /// 0xef4
			 0xc85bf438, /// 0xef8
			 0x3dc7bbaa, /// 0xefc
			 0xcdd316d5, /// 0xf00
			 0x6bd19f79, /// 0xf04
			 0x7fc6f80d, /// 0xf08
			 0xd58203f6, /// 0xf0c
			 0x9cc7d95a, /// 0xf10
			 0xdc37f4be, /// 0xf14
			 0x56acf4f7, /// 0xf18
			 0x9ce1ba20, /// 0xf1c
			 0x29a55031, /// 0xf20
			 0x9985f987, /// 0xf24
			 0x7b10c85c, /// 0xf28
			 0x8c719122, /// 0xf2c
			 0x01f1f3b7, /// 0xf30
			 0x06ada0ed, /// 0xf34
			 0x67a3b14d, /// 0xf38
			 0xb07cfc2c, /// 0xf3c
			 0xbfa300da, /// 0xf40
			 0xc3f009c5, /// 0xf44
			 0xc6a81d69, /// 0xf48
			 0xbc7dd301, /// 0xf4c
			 0x8308d7d4, /// 0xf50
			 0x26983078, /// 0xf54
			 0x1dbe8fae, /// 0xf58
			 0xf8985a53, /// 0xf5c
			 0x21e54891, /// 0xf60
			 0x59af5a30, /// 0xf64
			 0x011165d4, /// 0xf68
			 0xb4e2eec8, /// 0xf6c
			 0xa6d6987d, /// 0xf70
			 0xf8d27937, /// 0xf74
			 0xd9f89f71, /// 0xf78
			 0x82c2bfb0, /// 0xf7c
			 0x83449d43, /// 0xf80
			 0x5a32b2d2, /// 0xf84
			 0x27c3b4c2, /// 0xf88
			 0x739bd35d, /// 0xf8c
			 0xf35782de, /// 0xf90
			 0xb78a8000, /// 0xf94
			 0xfc5fd9e1, /// 0xf98
			 0xd983d0df, /// 0xf9c
			 0xc2f0d36d, /// 0xfa0
			 0x74825c69, /// 0xfa4
			 0xf2c71545, /// 0xfa8
			 0xa657c9cb, /// 0xfac
			 0xb019c779, /// 0xfb0
			 0xaf8453bc, /// 0xfb4
			 0x20822a2d, /// 0xfb8
			 0xd752b60e, /// 0xfbc
			 0xac64a090, /// 0xfc0
			 0x93c602e0, /// 0xfc4
			 0xcfbd1f1e, /// 0xfc8
			 0xf801244c, /// 0xfcc
			 0xbcd6c96b, /// 0xfd0
			 0x0604c6fc, /// 0xfd4
			 0xaaac644d, /// 0xfd8
			 0x38a46b22, /// 0xfdc
			 0x9a2caa06, /// 0xfe0
			 0x3e4ffe3c, /// 0xfe4
			 0x2d694d71, /// 0xfe8
			 0xd7d1340f, /// 0xfec
			 0xd156be49, /// 0xff0
			 0x2abcaac2, /// 0xff4
			 0xd95d5a01, /// 0xff8
			 0x3bd2c8c1 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x4762f37c, /// 0x0
			 0x74f403e1, /// 0x4
			 0x6166a045, /// 0x8
			 0x818534b0, /// 0xc
			 0xfe188492, /// 0x10
			 0xd4472b8d, /// 0x14
			 0x5ddbf105, /// 0x18
			 0x5a322932, /// 0x1c
			 0x11d3b226, /// 0x20
			 0x29327888, /// 0x24
			 0x23de8bde, /// 0x28
			 0x66b3e6a5, /// 0x2c
			 0x87916ae6, /// 0x30
			 0x55ca851a, /// 0x34
			 0xba69dad1, /// 0x38
			 0xce200321, /// 0x3c
			 0xf39dac41, /// 0x40
			 0x9a02a995, /// 0x44
			 0x8d7a0ab6, /// 0x48
			 0x32552002, /// 0x4c
			 0x5f732802, /// 0x50
			 0x8ec3bfc6, /// 0x54
			 0xa634c1ff, /// 0x58
			 0xe55c14f0, /// 0x5c
			 0x6b48814e, /// 0x60
			 0x224d81a2, /// 0x64
			 0xed92688e, /// 0x68
			 0x29dcce15, /// 0x6c
			 0xa4e6cd7b, /// 0x70
			 0xa78cab9c, /// 0x74
			 0x062c4b98, /// 0x78
			 0x52d03852, /// 0x7c
			 0xf85f98a7, /// 0x80
			 0x2c71599b, /// 0x84
			 0x19c14955, /// 0x88
			 0x6ab60549, /// 0x8c
			 0xaefc9050, /// 0x90
			 0x8cfe7488, /// 0x94
			 0x21286745, /// 0x98
			 0x9270bf3e, /// 0x9c
			 0x2c0b66c0, /// 0xa0
			 0x0c6a2ba4, /// 0xa4
			 0xe438d312, /// 0xa8
			 0xcaba7e8f, /// 0xac
			 0x423dc412, /// 0xb0
			 0xf142ce4e, /// 0xb4
			 0x2d0d9121, /// 0xb8
			 0xabde92c3, /// 0xbc
			 0xd4c1f0cf, /// 0xc0
			 0x38537f6b, /// 0xc4
			 0xe03ea84d, /// 0xc8
			 0x48dbddb4, /// 0xcc
			 0x5c666b8f, /// 0xd0
			 0x0083e333, /// 0xd4
			 0x31dab3f0, /// 0xd8
			 0x536dc60c, /// 0xdc
			 0x5fffddb0, /// 0xe0
			 0xae8598fd, /// 0xe4
			 0xda075cae, /// 0xe8
			 0xf46f7af9, /// 0xec
			 0x95b07657, /// 0xf0
			 0xdacdae7f, /// 0xf4
			 0xc7aa0411, /// 0xf8
			 0xd5ccc57e, /// 0xfc
			 0xc081d104, /// 0x100
			 0xbf9f6634, /// 0x104
			 0x9957f109, /// 0x108
			 0xfd8e1165, /// 0x10c
			 0x86d68f44, /// 0x110
			 0x3845ca90, /// 0x114
			 0x0c1a29c6, /// 0x118
			 0x33b242d6, /// 0x11c
			 0xee6af735, /// 0x120
			 0x18276ebb, /// 0x124
			 0x4cea2051, /// 0x128
			 0x861c4ada, /// 0x12c
			 0xff73804f, /// 0x130
			 0x32488453, /// 0x134
			 0xc70e34f6, /// 0x138
			 0xb829b45e, /// 0x13c
			 0x233b3a35, /// 0x140
			 0x2e701dda, /// 0x144
			 0xea7b513b, /// 0x148
			 0x9b31c82a, /// 0x14c
			 0x6bb96b46, /// 0x150
			 0xf93a05a7, /// 0x154
			 0xc3a89400, /// 0x158
			 0x116e80c9, /// 0x15c
			 0x60762fdc, /// 0x160
			 0x81e61e73, /// 0x164
			 0x79982f31, /// 0x168
			 0x2a474c51, /// 0x16c
			 0x986df9f0, /// 0x170
			 0x018834c1, /// 0x174
			 0x7e4a7faf, /// 0x178
			 0xa3bef9b6, /// 0x17c
			 0xfa650dc1, /// 0x180
			 0xf1443110, /// 0x184
			 0x89a55c24, /// 0x188
			 0x744dc22c, /// 0x18c
			 0x8cabd188, /// 0x190
			 0x124b18af, /// 0x194
			 0x74b27396, /// 0x198
			 0xc5d8977f, /// 0x19c
			 0x1f81006e, /// 0x1a0
			 0x439ea921, /// 0x1a4
			 0x1f262436, /// 0x1a8
			 0x9350317e, /// 0x1ac
			 0xb9e3c3ec, /// 0x1b0
			 0xabd08fdf, /// 0x1b4
			 0x6148f708, /// 0x1b8
			 0x2f375bca, /// 0x1bc
			 0xe093838c, /// 0x1c0
			 0x663baf30, /// 0x1c4
			 0x90aaf716, /// 0x1c8
			 0x880d8996, /// 0x1cc
			 0x86e91d25, /// 0x1d0
			 0xf724b326, /// 0x1d4
			 0x2da4c19a, /// 0x1d8
			 0xba30abbd, /// 0x1dc
			 0xabe0bea6, /// 0x1e0
			 0x3a3eb903, /// 0x1e4
			 0x30f80c22, /// 0x1e8
			 0xbaac5b39, /// 0x1ec
			 0x84a3b758, /// 0x1f0
			 0x41b772d2, /// 0x1f4
			 0xc379e296, /// 0x1f8
			 0x2bc10778, /// 0x1fc
			 0xd58cc52b, /// 0x200
			 0x17f68254, /// 0x204
			 0x15d31bfc, /// 0x208
			 0xe33c962e, /// 0x20c
			 0x0db9061f, /// 0x210
			 0x4833114b, /// 0x214
			 0x0aabf1d9, /// 0x218
			 0x390d6129, /// 0x21c
			 0x658f070e, /// 0x220
			 0x8ad7400f, /// 0x224
			 0xe81ca7d4, /// 0x228
			 0xee2fa15f, /// 0x22c
			 0x2cbfe0e6, /// 0x230
			 0xb00ae05b, /// 0x234
			 0x993192ec, /// 0x238
			 0x2e2ddd76, /// 0x23c
			 0xb996e577, /// 0x240
			 0xaa632603, /// 0x244
			 0xaf1950d1, /// 0x248
			 0x9f3721a5, /// 0x24c
			 0xf030558a, /// 0x250
			 0xc03fb4c3, /// 0x254
			 0x8e254e2b, /// 0x258
			 0xa7fc580d, /// 0x25c
			 0x65b57047, /// 0x260
			 0x2936671b, /// 0x264
			 0xe2f79140, /// 0x268
			 0x1ecb9734, /// 0x26c
			 0x1784bf48, /// 0x270
			 0x87838008, /// 0x274
			 0x249d0c9b, /// 0x278
			 0xada321bc, /// 0x27c
			 0x3635dad8, /// 0x280
			 0x6fd06678, /// 0x284
			 0xb05d9146, /// 0x288
			 0x1786b552, /// 0x28c
			 0xd44962a0, /// 0x290
			 0xc389073f, /// 0x294
			 0x44fc6a5d, /// 0x298
			 0x3c73b3b1, /// 0x29c
			 0x6db0e325, /// 0x2a0
			 0x0aac5577, /// 0x2a4
			 0x38676717, /// 0x2a8
			 0x143384d9, /// 0x2ac
			 0xa1d2c079, /// 0x2b0
			 0x03fba516, /// 0x2b4
			 0x5d993924, /// 0x2b8
			 0x5b9222e0, /// 0x2bc
			 0x2b5ebeec, /// 0x2c0
			 0xcabd67a0, /// 0x2c4
			 0xa15a64e4, /// 0x2c8
			 0xbcf9e5f6, /// 0x2cc
			 0x7e7a65b7, /// 0x2d0
			 0xd7952ec8, /// 0x2d4
			 0xf4366356, /// 0x2d8
			 0x66286ac5, /// 0x2dc
			 0x0baf0f57, /// 0x2e0
			 0x05cb5cea, /// 0x2e4
			 0x9919a7e5, /// 0x2e8
			 0x0d87d94c, /// 0x2ec
			 0x4af3a391, /// 0x2f0
			 0x1eec818a, /// 0x2f4
			 0x0210eacd, /// 0x2f8
			 0xec0ed251, /// 0x2fc
			 0x77a3a79c, /// 0x300
			 0x0dd8fe7f, /// 0x304
			 0x9249fd8e, /// 0x308
			 0x3983b287, /// 0x30c
			 0x548a9f99, /// 0x310
			 0x383b89d7, /// 0x314
			 0xab0dd608, /// 0x318
			 0x428a1277, /// 0x31c
			 0x8e1862e7, /// 0x320
			 0xfe348de1, /// 0x324
			 0x0987313b, /// 0x328
			 0x1e750f98, /// 0x32c
			 0xee9869bb, /// 0x330
			 0xdb1bb02e, /// 0x334
			 0xcb5ab7c8, /// 0x338
			 0xd16d7776, /// 0x33c
			 0xb00c51ec, /// 0x340
			 0x908572cd, /// 0x344
			 0x1e626c1d, /// 0x348
			 0x6b659021, /// 0x34c
			 0x5040bb55, /// 0x350
			 0x18446576, /// 0x354
			 0x7d65b566, /// 0x358
			 0x704901c5, /// 0x35c
			 0x9777480c, /// 0x360
			 0xab81e8c1, /// 0x364
			 0x3f38c4e2, /// 0x368
			 0xdf08c835, /// 0x36c
			 0x2b0d4fe2, /// 0x370
			 0x91b0b712, /// 0x374
			 0xd491feb4, /// 0x378
			 0xf8f1063b, /// 0x37c
			 0xed7b5c31, /// 0x380
			 0x4e07ce30, /// 0x384
			 0xb58ac3d2, /// 0x388
			 0xe87f2d50, /// 0x38c
			 0x9ab5bf32, /// 0x390
			 0x66effc2a, /// 0x394
			 0x5228f836, /// 0x398
			 0x493fff92, /// 0x39c
			 0x97e42dc2, /// 0x3a0
			 0x4141f71a, /// 0x3a4
			 0x9ce93295, /// 0x3a8
			 0x61a22cf9, /// 0x3ac
			 0xdff24241, /// 0x3b0
			 0xeb19287b, /// 0x3b4
			 0x14e3dee5, /// 0x3b8
			 0xac1be5b6, /// 0x3bc
			 0xc11ed7c8, /// 0x3c0
			 0x5461d8a6, /// 0x3c4
			 0xffcdf0de, /// 0x3c8
			 0xf3f2f286, /// 0x3cc
			 0xdbddf6cd, /// 0x3d0
			 0xb86e16a1, /// 0x3d4
			 0xc3825189, /// 0x3d8
			 0x1313fff7, /// 0x3dc
			 0x1fe39656, /// 0x3e0
			 0x790a08f1, /// 0x3e4
			 0x49712649, /// 0x3e8
			 0x079e98b6, /// 0x3ec
			 0x12fe1a18, /// 0x3f0
			 0xca84f982, /// 0x3f4
			 0x157f65dc, /// 0x3f8
			 0xb9395010, /// 0x3fc
			 0xd39c0184, /// 0x400
			 0xef5fe65a, /// 0x404
			 0x6e7405d5, /// 0x408
			 0xa9041179, /// 0x40c
			 0x127c1b0c, /// 0x410
			 0x051666e5, /// 0x414
			 0x38e88be5, /// 0x418
			 0x917c1a05, /// 0x41c
			 0x8d2336d3, /// 0x420
			 0x556f79a2, /// 0x424
			 0x77c18dc4, /// 0x428
			 0xd0ae9309, /// 0x42c
			 0x5b897737, /// 0x430
			 0x5ec42113, /// 0x434
			 0xd9a365e0, /// 0x438
			 0x03a4cc99, /// 0x43c
			 0xbc181794, /// 0x440
			 0xd110f513, /// 0x444
			 0x8ac30166, /// 0x448
			 0x19a0c034, /// 0x44c
			 0x6b217ad4, /// 0x450
			 0xc28887fe, /// 0x454
			 0xa51ed65a, /// 0x458
			 0x5911c552, /// 0x45c
			 0xf8880953, /// 0x460
			 0xab2403cb, /// 0x464
			 0xf2a4440c, /// 0x468
			 0xa9433e61, /// 0x46c
			 0x2b2f2ecf, /// 0x470
			 0xe548bced, /// 0x474
			 0x9e7a2da9, /// 0x478
			 0xf6590507, /// 0x47c
			 0x34dfa8ce, /// 0x480
			 0x289db32f, /// 0x484
			 0xde86d32a, /// 0x488
			 0x4498dda3, /// 0x48c
			 0x60f59e44, /// 0x490
			 0x48be2216, /// 0x494
			 0x25a3c5be, /// 0x498
			 0xe992c09a, /// 0x49c
			 0x54a43287, /// 0x4a0
			 0x8cbb66b3, /// 0x4a4
			 0xb162d992, /// 0x4a8
			 0x1ffda832, /// 0x4ac
			 0xb58f4e11, /// 0x4b0
			 0xda8d09af, /// 0x4b4
			 0x0ec7c2e0, /// 0x4b8
			 0x3c2374a8, /// 0x4bc
			 0x404c10d9, /// 0x4c0
			 0xfe4e9724, /// 0x4c4
			 0x37dab9b5, /// 0x4c8
			 0xd182de1f, /// 0x4cc
			 0x15f65678, /// 0x4d0
			 0xdcf1c592, /// 0x4d4
			 0x9a28f917, /// 0x4d8
			 0x1edc4137, /// 0x4dc
			 0xc0b9f1f4, /// 0x4e0
			 0xe73010d8, /// 0x4e4
			 0x16c5b025, /// 0x4e8
			 0x1ac7389b, /// 0x4ec
			 0xa0884aac, /// 0x4f0
			 0xf073a639, /// 0x4f4
			 0x63e615fc, /// 0x4f8
			 0xdc567503, /// 0x4fc
			 0x7b6167dc, /// 0x500
			 0xbf94d981, /// 0x504
			 0x8e00bc05, /// 0x508
			 0x19147bb9, /// 0x50c
			 0xdffb87ce, /// 0x510
			 0x21425772, /// 0x514
			 0xe2323445, /// 0x518
			 0x943eaf4a, /// 0x51c
			 0xe5123275, /// 0x520
			 0x62944c30, /// 0x524
			 0x9a37991e, /// 0x528
			 0xb7f5f25f, /// 0x52c
			 0x47ae1aa4, /// 0x530
			 0x6a000f2f, /// 0x534
			 0xa807ff23, /// 0x538
			 0xa41283f8, /// 0x53c
			 0x45e1fbfc, /// 0x540
			 0x5708e158, /// 0x544
			 0x1e9ef143, /// 0x548
			 0x8182eb4c, /// 0x54c
			 0x52e7268e, /// 0x550
			 0x52486413, /// 0x554
			 0xabe4bedc, /// 0x558
			 0x1339ebcc, /// 0x55c
			 0x4af75d50, /// 0x560
			 0x490baa33, /// 0x564
			 0x2b1ee5a8, /// 0x568
			 0x096e1c6f, /// 0x56c
			 0x3bc67543, /// 0x570
			 0xe98d607c, /// 0x574
			 0x02a26546, /// 0x578
			 0x907f3f2d, /// 0x57c
			 0xdad168cc, /// 0x580
			 0x6a4455b4, /// 0x584
			 0x6fb3a283, /// 0x588
			 0xa4da7fc6, /// 0x58c
			 0x5e4335b4, /// 0x590
			 0x1a06857e, /// 0x594
			 0x781631de, /// 0x598
			 0xdf91b1cf, /// 0x59c
			 0x71135737, /// 0x5a0
			 0x700a71bd, /// 0x5a4
			 0x2dfb0cf9, /// 0x5a8
			 0xc51b5840, /// 0x5ac
			 0x5b027764, /// 0x5b0
			 0x5b27871d, /// 0x5b4
			 0xaeaa74c5, /// 0x5b8
			 0xe06ba371, /// 0x5bc
			 0x9e86e095, /// 0x5c0
			 0x3d722106, /// 0x5c4
			 0x4f070937, /// 0x5c8
			 0xfc500d4c, /// 0x5cc
			 0x86a8f9ae, /// 0x5d0
			 0xf2b82cce, /// 0x5d4
			 0x9539e643, /// 0x5d8
			 0x23cd597f, /// 0x5dc
			 0x2653bd31, /// 0x5e0
			 0x85b8e575, /// 0x5e4
			 0x0cda0e34, /// 0x5e8
			 0xcb505f94, /// 0x5ec
			 0x4db8fd31, /// 0x5f0
			 0xfd1fefa1, /// 0x5f4
			 0x278f5b18, /// 0x5f8
			 0x00ce7f2e, /// 0x5fc
			 0x28030313, /// 0x600
			 0xf55faaa9, /// 0x604
			 0xc0a0cf72, /// 0x608
			 0x727aaf40, /// 0x60c
			 0x5349e536, /// 0x610
			 0x659b7935, /// 0x614
			 0xf9b4c836, /// 0x618
			 0x89c3317c, /// 0x61c
			 0xbf8f2209, /// 0x620
			 0xe06ec7c5, /// 0x624
			 0x3ffa0da0, /// 0x628
			 0x1750c666, /// 0x62c
			 0xd4270992, /// 0x630
			 0xc3608fe8, /// 0x634
			 0x57c9a72e, /// 0x638
			 0xd865b61a, /// 0x63c
			 0x4f50f699, /// 0x640
			 0xaaacde2a, /// 0x644
			 0x4895e449, /// 0x648
			 0xf90ccfc0, /// 0x64c
			 0xc4384086, /// 0x650
			 0x661fe0c7, /// 0x654
			 0xb9f5a5e3, /// 0x658
			 0x1ebc7745, /// 0x65c
			 0x4fee1dbd, /// 0x660
			 0xe239578b, /// 0x664
			 0x854a9d1a, /// 0x668
			 0x8228020f, /// 0x66c
			 0x65c14f3d, /// 0x670
			 0x9cb35f17, /// 0x674
			 0xb54345c4, /// 0x678
			 0x34089e17, /// 0x67c
			 0x484518cb, /// 0x680
			 0x470ccb90, /// 0x684
			 0x0961de3d, /// 0x688
			 0x3f9df8ed, /// 0x68c
			 0x24b8ce5e, /// 0x690
			 0x921bf34a, /// 0x694
			 0x88f4376b, /// 0x698
			 0xd8d09dd6, /// 0x69c
			 0x18b49633, /// 0x6a0
			 0xae16cf86, /// 0x6a4
			 0x8221eeb5, /// 0x6a8
			 0x39e4b93a, /// 0x6ac
			 0xb93c9cec, /// 0x6b0
			 0xc9b0c54e, /// 0x6b4
			 0xccbd4a72, /// 0x6b8
			 0x981309bc, /// 0x6bc
			 0x983545e8, /// 0x6c0
			 0x780b5ffa, /// 0x6c4
			 0xabf35d8a, /// 0x6c8
			 0x59a5f73e, /// 0x6cc
			 0x2f7c3b41, /// 0x6d0
			 0x39df7c3c, /// 0x6d4
			 0x3963e32f, /// 0x6d8
			 0x1573dd27, /// 0x6dc
			 0x89e779d3, /// 0x6e0
			 0xb2c420f3, /// 0x6e4
			 0x9373a0cb, /// 0x6e8
			 0x8efc6f1b, /// 0x6ec
			 0xe14749f6, /// 0x6f0
			 0xafdd024f, /// 0x6f4
			 0xe8c067df, /// 0x6f8
			 0x756fba3d, /// 0x6fc
			 0x7e4cbbd1, /// 0x700
			 0xc0e62dc5, /// 0x704
			 0x39281bdc, /// 0x708
			 0x5f2c436c, /// 0x70c
			 0xe9d0a14f, /// 0x710
			 0x92146ccd, /// 0x714
			 0xebe730ee, /// 0x718
			 0xd46578b2, /// 0x71c
			 0x4a1a764a, /// 0x720
			 0x74ce8d50, /// 0x724
			 0xa682cfe5, /// 0x728
			 0xed7653b4, /// 0x72c
			 0xf0a67360, /// 0x730
			 0x8359d031, /// 0x734
			 0x8625445d, /// 0x738
			 0xcdd9c7b2, /// 0x73c
			 0x57e5c049, /// 0x740
			 0xbd672e2c, /// 0x744
			 0x86b79b96, /// 0x748
			 0xd0f84af4, /// 0x74c
			 0xc915d629, /// 0x750
			 0x4475aabc, /// 0x754
			 0xd93b0fd1, /// 0x758
			 0x8190183d, /// 0x75c
			 0x217d9e4b, /// 0x760
			 0x13d679f4, /// 0x764
			 0x0402a0bf, /// 0x768
			 0xa3c9f0cb, /// 0x76c
			 0xcd28e046, /// 0x770
			 0x146a396b, /// 0x774
			 0x1479e18e, /// 0x778
			 0x6d542c42, /// 0x77c
			 0xa3d96fec, /// 0x780
			 0xe51624a7, /// 0x784
			 0x2caaec8b, /// 0x788
			 0xa847c06f, /// 0x78c
			 0xe73046b0, /// 0x790
			 0x201314ab, /// 0x794
			 0xd4cbacd8, /// 0x798
			 0x29858f28, /// 0x79c
			 0x1f9a699a, /// 0x7a0
			 0x3feba5bf, /// 0x7a4
			 0xebd9134c, /// 0x7a8
			 0xe014fe6c, /// 0x7ac
			 0x198d0abd, /// 0x7b0
			 0xb3902e10, /// 0x7b4
			 0xc8fe26c1, /// 0x7b8
			 0x7a3e7349, /// 0x7bc
			 0x09c548f6, /// 0x7c0
			 0x349d56b5, /// 0x7c4
			 0x5d53ca31, /// 0x7c8
			 0xbc6be170, /// 0x7cc
			 0x11c0e15b, /// 0x7d0
			 0x8f0bc138, /// 0x7d4
			 0x3bd7cabf, /// 0x7d8
			 0x3e92bfac, /// 0x7dc
			 0x5ad97025, /// 0x7e0
			 0x8977a612, /// 0x7e4
			 0xe06dee25, /// 0x7e8
			 0x826334fb, /// 0x7ec
			 0x90812835, /// 0x7f0
			 0x8016ec2b, /// 0x7f4
			 0x043087a0, /// 0x7f8
			 0x69da940f, /// 0x7fc
			 0x8417f8a9, /// 0x800
			 0x8b04632c, /// 0x804
			 0x564b0556, /// 0x808
			 0x3d5d093b, /// 0x80c
			 0x14852763, /// 0x810
			 0x057bed1f, /// 0x814
			 0xee6cffaf, /// 0x818
			 0x44a2f981, /// 0x81c
			 0x10d23dc0, /// 0x820
			 0xdf9fd0cb, /// 0x824
			 0xf649ed61, /// 0x828
			 0xaf8d0b2b, /// 0x82c
			 0x21c30e03, /// 0x830
			 0xa543f505, /// 0x834
			 0x30afb684, /// 0x838
			 0x6e8b5d94, /// 0x83c
			 0xf0ddfe08, /// 0x840
			 0x816eea7b, /// 0x844
			 0x939e4994, /// 0x848
			 0xe8887e4d, /// 0x84c
			 0x46352fd8, /// 0x850
			 0xe5a3bb49, /// 0x854
			 0x3dcdedba, /// 0x858
			 0xe34d91d1, /// 0x85c
			 0xd5e536d7, /// 0x860
			 0x074b41d1, /// 0x864
			 0x11061d19, /// 0x868
			 0x363449c3, /// 0x86c
			 0xe026a3dc, /// 0x870
			 0x8b679809, /// 0x874
			 0x9b5bf1c9, /// 0x878
			 0xbb977374, /// 0x87c
			 0x4a0ea273, /// 0x880
			 0x7fb3dfbc, /// 0x884
			 0x767d1ca9, /// 0x888
			 0x05034310, /// 0x88c
			 0xa4bca8be, /// 0x890
			 0x4b2d5949, /// 0x894
			 0x5d7a7468, /// 0x898
			 0x8820b055, /// 0x89c
			 0x8b877f3a, /// 0x8a0
			 0x5a702eb7, /// 0x8a4
			 0xbbfd845c, /// 0x8a8
			 0x2dd66f47, /// 0x8ac
			 0x02e5c435, /// 0x8b0
			 0xbde9079c, /// 0x8b4
			 0xb810ed22, /// 0x8b8
			 0x8b6d7378, /// 0x8bc
			 0x6c864c8d, /// 0x8c0
			 0x35caf052, /// 0x8c4
			 0x847cbff4, /// 0x8c8
			 0x83fcb95a, /// 0x8cc
			 0x4ff62383, /// 0x8d0
			 0x95359a37, /// 0x8d4
			 0x30ebff24, /// 0x8d8
			 0xfea42d54, /// 0x8dc
			 0x7068febd, /// 0x8e0
			 0x541ecdec, /// 0x8e4
			 0x4f9620c0, /// 0x8e8
			 0x100ba38c, /// 0x8ec
			 0x6f8e0faf, /// 0x8f0
			 0x32311334, /// 0x8f4
			 0xe63ff2f0, /// 0x8f8
			 0xdba0d74a, /// 0x8fc
			 0x618deb2c, /// 0x900
			 0x2ab742ab, /// 0x904
			 0x44fb68a3, /// 0x908
			 0x1c19e15f, /// 0x90c
			 0xe17129c4, /// 0x910
			 0x5669a54d, /// 0x914
			 0x13361ef5, /// 0x918
			 0x90ab4532, /// 0x91c
			 0xaa02a52c, /// 0x920
			 0xa97abf7c, /// 0x924
			 0x5f8d6d2e, /// 0x928
			 0x374bedef, /// 0x92c
			 0xbd6cf6ad, /// 0x930
			 0xe8e1bc72, /// 0x934
			 0x01fa7fd3, /// 0x938
			 0xaf0b71eb, /// 0x93c
			 0x973d791b, /// 0x940
			 0x10edb9df, /// 0x944
			 0x6365319a, /// 0x948
			 0xd85dbcb7, /// 0x94c
			 0x40f16798, /// 0x950
			 0x047b7964, /// 0x954
			 0xe10718ba, /// 0x958
			 0xc2369860, /// 0x95c
			 0xf0c03701, /// 0x960
			 0x911a2991, /// 0x964
			 0x4da67df2, /// 0x968
			 0x6bb5d5ec, /// 0x96c
			 0xa307c4c9, /// 0x970
			 0x3c34642d, /// 0x974
			 0x5cc84e49, /// 0x978
			 0xdcdd6878, /// 0x97c
			 0xb2aeac56, /// 0x980
			 0x503a0ca8, /// 0x984
			 0x2dde6416, /// 0x988
			 0xf6533f93, /// 0x98c
			 0xa0c6803b, /// 0x990
			 0x7fe52b1a, /// 0x994
			 0x67abac67, /// 0x998
			 0xa072f7e0, /// 0x99c
			 0x8a898f9a, /// 0x9a0
			 0xaad53bb9, /// 0x9a4
			 0xc50d69c3, /// 0x9a8
			 0x359f6df4, /// 0x9ac
			 0xe629de0c, /// 0x9b0
			 0x9525d611, /// 0x9b4
			 0xc5e14992, /// 0x9b8
			 0x1c153b04, /// 0x9bc
			 0x05e8e6bd, /// 0x9c0
			 0x9d89cbff, /// 0x9c4
			 0x9db9cde5, /// 0x9c8
			 0x4c9c800b, /// 0x9cc
			 0xc246f87b, /// 0x9d0
			 0xa94e09f3, /// 0x9d4
			 0xd06a1613, /// 0x9d8
			 0xe4fa1b76, /// 0x9dc
			 0x808dfa61, /// 0x9e0
			 0x3c1cdcf9, /// 0x9e4
			 0x31223374, /// 0x9e8
			 0xc2a2588e, /// 0x9ec
			 0xd29ce7f3, /// 0x9f0
			 0xcc911868, /// 0x9f4
			 0x0a65ac7c, /// 0x9f8
			 0x6c48abce, /// 0x9fc
			 0x74d25d73, /// 0xa00
			 0x11ed5314, /// 0xa04
			 0x55151953, /// 0xa08
			 0x2c838f9e, /// 0xa0c
			 0x584b3e26, /// 0xa10
			 0x4307606c, /// 0xa14
			 0x364dd763, /// 0xa18
			 0xd7f8929d, /// 0xa1c
			 0x9f5e6576, /// 0xa20
			 0xd6541205, /// 0xa24
			 0x540c7b0a, /// 0xa28
			 0xdd896515, /// 0xa2c
			 0x9a455696, /// 0xa30
			 0x09bf85df, /// 0xa34
			 0x64b10b97, /// 0xa38
			 0x8e5dae44, /// 0xa3c
			 0xb3866c63, /// 0xa40
			 0xa54b114c, /// 0xa44
			 0x28d2051f, /// 0xa48
			 0x294ed967, /// 0xa4c
			 0x2f37f72c, /// 0xa50
			 0x4f48f133, /// 0xa54
			 0x15b5591d, /// 0xa58
			 0xfe098c6b, /// 0xa5c
			 0x78984afa, /// 0xa60
			 0xf0281191, /// 0xa64
			 0x4482ce9f, /// 0xa68
			 0xd85a76fa, /// 0xa6c
			 0x89bf9d9f, /// 0xa70
			 0x3cc05def, /// 0xa74
			 0x4d6d8aaf, /// 0xa78
			 0x5ae827c0, /// 0xa7c
			 0xe3d92fee, /// 0xa80
			 0x0b9a4b66, /// 0xa84
			 0x34816248, /// 0xa88
			 0x97995e62, /// 0xa8c
			 0xa08593a2, /// 0xa90
			 0xd3d05e26, /// 0xa94
			 0x6bdf22c0, /// 0xa98
			 0x21d8cd8b, /// 0xa9c
			 0x703f4abf, /// 0xaa0
			 0x4557cacf, /// 0xaa4
			 0x9166e78f, /// 0xaa8
			 0x03f66157, /// 0xaac
			 0xa10ab082, /// 0xab0
			 0xbd0f6d2e, /// 0xab4
			 0x44bc582c, /// 0xab8
			 0x0ccdf058, /// 0xabc
			 0x02a393c9, /// 0xac0
			 0x4a714940, /// 0xac4
			 0x04b52252, /// 0xac8
			 0x8bc3e691, /// 0xacc
			 0x68e7edb8, /// 0xad0
			 0xbc8a8586, /// 0xad4
			 0xca792489, /// 0xad8
			 0x9eddada8, /// 0xadc
			 0x3da3ced2, /// 0xae0
			 0x6071f94a, /// 0xae4
			 0x089603f9, /// 0xae8
			 0x5e192dfc, /// 0xaec
			 0x27222b23, /// 0xaf0
			 0x1cf92637, /// 0xaf4
			 0x0989a34a, /// 0xaf8
			 0x7c712d33, /// 0xafc
			 0x667641cd, /// 0xb00
			 0xdc42e8a7, /// 0xb04
			 0xa6a6c3fb, /// 0xb08
			 0xd397b49d, /// 0xb0c
			 0x61ca9349, /// 0xb10
			 0x57d87842, /// 0xb14
			 0x69d8da21, /// 0xb18
			 0x6c13afbe, /// 0xb1c
			 0x27fd13cd, /// 0xb20
			 0x9f7ca56d, /// 0xb24
			 0xab260fb6, /// 0xb28
			 0x194eefe7, /// 0xb2c
			 0xcc39dc91, /// 0xb30
			 0x2925d081, /// 0xb34
			 0x946bbe25, /// 0xb38
			 0xa38146df, /// 0xb3c
			 0x9409e205, /// 0xb40
			 0x8c29b0f7, /// 0xb44
			 0xcd308067, /// 0xb48
			 0xd26c1b59, /// 0xb4c
			 0x229bff68, /// 0xb50
			 0x228744e2, /// 0xb54
			 0x573371c4, /// 0xb58
			 0xcf2bcc1e, /// 0xb5c
			 0x178e8ded, /// 0xb60
			 0x115286af, /// 0xb64
			 0xdddacec4, /// 0xb68
			 0x06c0c046, /// 0xb6c
			 0x7d5dd916, /// 0xb70
			 0x158d2664, /// 0xb74
			 0x70ca5b43, /// 0xb78
			 0x62e13376, /// 0xb7c
			 0x81a851d7, /// 0xb80
			 0x8d8ae900, /// 0xb84
			 0x43e7a864, /// 0xb88
			 0x7bf533ad, /// 0xb8c
			 0x9de27c9a, /// 0xb90
			 0x25d4f7d5, /// 0xb94
			 0xa03c8a12, /// 0xb98
			 0x204136e3, /// 0xb9c
			 0xc81fb4ee, /// 0xba0
			 0x47b637da, /// 0xba4
			 0xc9db5987, /// 0xba8
			 0x434168e9, /// 0xbac
			 0x7a3470ea, /// 0xbb0
			 0xef7306d9, /// 0xbb4
			 0x26dac604, /// 0xbb8
			 0x85a986fe, /// 0xbbc
			 0x7c44179a, /// 0xbc0
			 0xf6f66512, /// 0xbc4
			 0xae4cc27f, /// 0xbc8
			 0x60b0c6a6, /// 0xbcc
			 0x5001f6a6, /// 0xbd0
			 0xc9a84604, /// 0xbd4
			 0x2c806b83, /// 0xbd8
			 0x168dd8f7, /// 0xbdc
			 0x7f2fb308, /// 0xbe0
			 0x05d268cb, /// 0xbe4
			 0xd454a30b, /// 0xbe8
			 0x031a5010, /// 0xbec
			 0x7880572d, /// 0xbf0
			 0x62906f13, /// 0xbf4
			 0xff3a1452, /// 0xbf8
			 0x4bf87b29, /// 0xbfc
			 0xc8df47d0, /// 0xc00
			 0x07a33534, /// 0xc04
			 0xd833e04b, /// 0xc08
			 0x8a5a6d2f, /// 0xc0c
			 0x559a93f2, /// 0xc10
			 0xebc9f001, /// 0xc14
			 0x482bb98a, /// 0xc18
			 0xd9f08d67, /// 0xc1c
			 0x0512912f, /// 0xc20
			 0x03625385, /// 0xc24
			 0xad3f4a6f, /// 0xc28
			 0xb55c4dc5, /// 0xc2c
			 0x4eb59968, /// 0xc30
			 0x19e2ca9a, /// 0xc34
			 0xcd85f5b0, /// 0xc38
			 0xbef7cdef, /// 0xc3c
			 0xd1549e76, /// 0xc40
			 0xb1cdefa9, /// 0xc44
			 0x013924e4, /// 0xc48
			 0x42ae43df, /// 0xc4c
			 0x9b8a5e6e, /// 0xc50
			 0x1c767750, /// 0xc54
			 0xea849c76, /// 0xc58
			 0x605c4ca4, /// 0xc5c
			 0x38cb7006, /// 0xc60
			 0x31c3511d, /// 0xc64
			 0xf5ce0c5b, /// 0xc68
			 0x55546a06, /// 0xc6c
			 0xf56c869a, /// 0xc70
			 0xad77a28c, /// 0xc74
			 0x495121cc, /// 0xc78
			 0x7adaf184, /// 0xc7c
			 0x68459d2b, /// 0xc80
			 0xcd12cce6, /// 0xc84
			 0xc9253789, /// 0xc88
			 0x4e9823c2, /// 0xc8c
			 0xeba4e22e, /// 0xc90
			 0xf4dc5ae5, /// 0xc94
			 0xe409bf0e, /// 0xc98
			 0xf7075424, /// 0xc9c
			 0xbeb04ede, /// 0xca0
			 0xcc543424, /// 0xca4
			 0x8a9ef488, /// 0xca8
			 0xb6800662, /// 0xcac
			 0x1c4eb879, /// 0xcb0
			 0xd8805299, /// 0xcb4
			 0xf939cb00, /// 0xcb8
			 0x6bf184f2, /// 0xcbc
			 0xf968f28f, /// 0xcc0
			 0x9a4a2fdd, /// 0xcc4
			 0xe9c9d9f5, /// 0xcc8
			 0x86f1a2d4, /// 0xccc
			 0x6856538a, /// 0xcd0
			 0x1516d0c1, /// 0xcd4
			 0xdd317314, /// 0xcd8
			 0x7814adfa, /// 0xcdc
			 0xd12012de, /// 0xce0
			 0xb2bb87e7, /// 0xce4
			 0x5cd40d20, /// 0xce8
			 0xa9539052, /// 0xcec
			 0xb2b1bd47, /// 0xcf0
			 0x552739e4, /// 0xcf4
			 0x481248ee, /// 0xcf8
			 0xc8e3790a, /// 0xcfc
			 0xac2586ce, /// 0xd00
			 0x8d32bf9d, /// 0xd04
			 0xdc35ad5c, /// 0xd08
			 0xc0aa6136, /// 0xd0c
			 0x834c790f, /// 0xd10
			 0x2c1782f7, /// 0xd14
			 0xa17f3570, /// 0xd18
			 0xc034ba6e, /// 0xd1c
			 0xb453ec55, /// 0xd20
			 0x26c48d11, /// 0xd24
			 0x55bff4ff, /// 0xd28
			 0x7fa51947, /// 0xd2c
			 0x54a5fd25, /// 0xd30
			 0x1da08ee9, /// 0xd34
			 0xf246efad, /// 0xd38
			 0xc8a46354, /// 0xd3c
			 0x26f41d94, /// 0xd40
			 0xee62a197, /// 0xd44
			 0x67c6e1e9, /// 0xd48
			 0x18eff8ef, /// 0xd4c
			 0xe888818d, /// 0xd50
			 0xfaefe629, /// 0xd54
			 0x41756a06, /// 0xd58
			 0x8d8c7cd3, /// 0xd5c
			 0xeb789591, /// 0xd60
			 0xee0776ef, /// 0xd64
			 0x479a59d7, /// 0xd68
			 0x2dfbc2b1, /// 0xd6c
			 0xb04045fb, /// 0xd70
			 0x0bcd5c13, /// 0xd74
			 0xe1d779fe, /// 0xd78
			 0xfcf05728, /// 0xd7c
			 0xbe5470f6, /// 0xd80
			 0x12a5e16f, /// 0xd84
			 0x29eb8077, /// 0xd88
			 0x499776c3, /// 0xd8c
			 0xe3a6b764, /// 0xd90
			 0x5eb4f9bd, /// 0xd94
			 0xb21c6202, /// 0xd98
			 0x04d7301a, /// 0xd9c
			 0x47b86904, /// 0xda0
			 0x890f2990, /// 0xda4
			 0xa403fc06, /// 0xda8
			 0xffa54ad3, /// 0xdac
			 0xc3c64439, /// 0xdb0
			 0x6627310c, /// 0xdb4
			 0xe9fcfcc2, /// 0xdb8
			 0xe36365c1, /// 0xdbc
			 0x269073c1, /// 0xdc0
			 0x398ebd53, /// 0xdc4
			 0xb46387bd, /// 0xdc8
			 0xc380dcf1, /// 0xdcc
			 0xb3f206a7, /// 0xdd0
			 0x29924cd8, /// 0xdd4
			 0x0087850a, /// 0xdd8
			 0xd082911e, /// 0xddc
			 0x600a52c4, /// 0xde0
			 0x70cb1732, /// 0xde4
			 0xbd7eadb8, /// 0xde8
			 0x9d5c6928, /// 0xdec
			 0xa0303691, /// 0xdf0
			 0x12292e4b, /// 0xdf4
			 0x14171d2f, /// 0xdf8
			 0xb7429508, /// 0xdfc
			 0x76503895, /// 0xe00
			 0xdcbd7580, /// 0xe04
			 0x356433ac, /// 0xe08
			 0x46727720, /// 0xe0c
			 0x2582a41d, /// 0xe10
			 0x28c5c746, /// 0xe14
			 0xb687a61e, /// 0xe18
			 0x1278ddb5, /// 0xe1c
			 0x3836e987, /// 0xe20
			 0x6c5244f5, /// 0xe24
			 0x417ed347, /// 0xe28
			 0x5453a2a4, /// 0xe2c
			 0xbe409ab8, /// 0xe30
			 0x1d33bdc6, /// 0xe34
			 0x71bb3081, /// 0xe38
			 0xe54fa1e3, /// 0xe3c
			 0x34c816cf, /// 0xe40
			 0x8763f144, /// 0xe44
			 0xd4c8a60b, /// 0xe48
			 0xb5956632, /// 0xe4c
			 0x3a1bcb21, /// 0xe50
			 0x003b7de5, /// 0xe54
			 0xb51e980b, /// 0xe58
			 0x02e65b8e, /// 0xe5c
			 0xa357ad9a, /// 0xe60
			 0x95131ef9, /// 0xe64
			 0x9d2c26c5, /// 0xe68
			 0x01c62bce, /// 0xe6c
			 0xc9ce3cf7, /// 0xe70
			 0xa8839161, /// 0xe74
			 0x0faf5c41, /// 0xe78
			 0xc3f488a6, /// 0xe7c
			 0x9be13728, /// 0xe80
			 0x582c5011, /// 0xe84
			 0x7970a5f3, /// 0xe88
			 0x6c7745e4, /// 0xe8c
			 0x7ef0dd4b, /// 0xe90
			 0xb1cb585a, /// 0xe94
			 0xbef1f468, /// 0xe98
			 0xbbf853bf, /// 0xe9c
			 0x641092f9, /// 0xea0
			 0x23a82f59, /// 0xea4
			 0xe32eda71, /// 0xea8
			 0x7c4cd0bf, /// 0xeac
			 0x2a156a82, /// 0xeb0
			 0xf12c0b2d, /// 0xeb4
			 0x631c0743, /// 0xeb8
			 0x649a9e62, /// 0xebc
			 0xcc104745, /// 0xec0
			 0x46acac78, /// 0xec4
			 0xc9eb6e06, /// 0xec8
			 0xe46df999, /// 0xecc
			 0xc492f7f3, /// 0xed0
			 0x5495ee95, /// 0xed4
			 0x1f1b998b, /// 0xed8
			 0xf6501adf, /// 0xedc
			 0xb35de5c0, /// 0xee0
			 0x9ce5076a, /// 0xee4
			 0xa76b604b, /// 0xee8
			 0x56944085, /// 0xeec
			 0x26a7fdd4, /// 0xef0
			 0xd8dc2a02, /// 0xef4
			 0x08d8f06a, /// 0xef8
			 0x0834855b, /// 0xefc
			 0x832805a6, /// 0xf00
			 0xd89abd1d, /// 0xf04
			 0x1c7c05d7, /// 0xf08
			 0xc75b591b, /// 0xf0c
			 0x58f72ce5, /// 0xf10
			 0xdfc9a22f, /// 0xf14
			 0xa7d62698, /// 0xf18
			 0x515f962d, /// 0xf1c
			 0x5dece774, /// 0xf20
			 0xa600bd04, /// 0xf24
			 0x7b0fa7ee, /// 0xf28
			 0x03cc761f, /// 0xf2c
			 0x1b56d6fc, /// 0xf30
			 0xc4282616, /// 0xf34
			 0x7fbafee8, /// 0xf38
			 0x487b3b21, /// 0xf3c
			 0x49b7d251, /// 0xf40
			 0x3d15d930, /// 0xf44
			 0xabb1a004, /// 0xf48
			 0x48e656ae, /// 0xf4c
			 0xe39686d5, /// 0xf50
			 0x47891294, /// 0xf54
			 0x2e812999, /// 0xf58
			 0x961f0940, /// 0xf5c
			 0xc8ba0163, /// 0xf60
			 0x7ed69a4b, /// 0xf64
			 0xdf23ce83, /// 0xf68
			 0x16014a7f, /// 0xf6c
			 0xfb1ffebc, /// 0xf70
			 0x9d789857, /// 0xf74
			 0x5562262e, /// 0xf78
			 0x39842752, /// 0xf7c
			 0xa134a39b, /// 0xf80
			 0x2f06d182, /// 0xf84
			 0x0ef8d7c9, /// 0xf88
			 0x23947dea, /// 0xf8c
			 0x66580fcd, /// 0xf90
			 0x29da48fb, /// 0xf94
			 0x0645f4d2, /// 0xf98
			 0x1f5130d4, /// 0xf9c
			 0x38bc3c1a, /// 0xfa0
			 0x16543405, /// 0xfa4
			 0x2a106470, /// 0xfa8
			 0x22d033b5, /// 0xfac
			 0xfd6facd0, /// 0xfb0
			 0xa6015e28, /// 0xfb4
			 0x129fe16d, /// 0xfb8
			 0xab36784d, /// 0xfbc
			 0x07461896, /// 0xfc0
			 0xafae77b0, /// 0xfc4
			 0xc8e73096, /// 0xfc8
			 0xb2526fa0, /// 0xfcc
			 0x6b0bf639, /// 0xfd0
			 0xca133945, /// 0xfd4
			 0xf8ebdae1, /// 0xfd8
			 0xc6e8a48a, /// 0xfdc
			 0xd371b059, /// 0xfe0
			 0xf1fc1345, /// 0xfe4
			 0xcfe78aee, /// 0xfe8
			 0x70adde28, /// 0xfec
			 0xd132f1c9, /// 0xff0
			 0x54c51cbe, /// 0xff4
			 0xdf98620c, /// 0xff8
			 0xd1a88064 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x976a1c9b, /// 0x0
			 0xd19fc51c, /// 0x4
			 0x0d8f5fd2, /// 0x8
			 0xc1339e81, /// 0xc
			 0x66066ec3, /// 0x10
			 0xe31e2245, /// 0x14
			 0x764a0e9f, /// 0x18
			 0x26162f19, /// 0x1c
			 0x0d8f8a46, /// 0x20
			 0xc0f77c74, /// 0x24
			 0xc9e74f49, /// 0x28
			 0x831edb51, /// 0x2c
			 0xf465872b, /// 0x30
			 0x220f1d66, /// 0x34
			 0xe7965848, /// 0x38
			 0xa562e2ff, /// 0x3c
			 0x468de115, /// 0x40
			 0x13c785b8, /// 0x44
			 0x29fd11fa, /// 0x48
			 0xea36ed7e, /// 0x4c
			 0x3172bd36, /// 0x50
			 0x81efe442, /// 0x54
			 0x3ba1e373, /// 0x58
			 0x5700444e, /// 0x5c
			 0xc27c13b5, /// 0x60
			 0xaef4fc42, /// 0x64
			 0xc685ffd1, /// 0x68
			 0x33ac5eca, /// 0x6c
			 0x793db2c1, /// 0x70
			 0x95ae98c2, /// 0x74
			 0x46068977, /// 0x78
			 0xb5dca769, /// 0x7c
			 0xdc34b92a, /// 0x80
			 0xeb4e867f, /// 0x84
			 0xef73e1e6, /// 0x88
			 0x97ed5299, /// 0x8c
			 0x62c4aee6, /// 0x90
			 0x169ab29c, /// 0x94
			 0x570ec9f7, /// 0x98
			 0x95f26829, /// 0x9c
			 0xe02850e1, /// 0xa0
			 0x48c6f09a, /// 0xa4
			 0x966e14e6, /// 0xa8
			 0xf46d32bd, /// 0xac
			 0x0f252734, /// 0xb0
			 0x50e0e9e4, /// 0xb4
			 0x35e1bb2d, /// 0xb8
			 0x22be6296, /// 0xbc
			 0x5c3f7c9b, /// 0xc0
			 0x226529da, /// 0xc4
			 0x6a90f7d3, /// 0xc8
			 0x4ca2d93b, /// 0xcc
			 0x152333b1, /// 0xd0
			 0x3e01cc6a, /// 0xd4
			 0xca17e148, /// 0xd8
			 0xa60b8c85, /// 0xdc
			 0x13b10721, /// 0xe0
			 0xd165cefa, /// 0xe4
			 0x5b3d08e1, /// 0xe8
			 0x2a465268, /// 0xec
			 0x077a0045, /// 0xf0
			 0xbcbe248e, /// 0xf4
			 0xc2ea4794, /// 0xf8
			 0x87c0c251, /// 0xfc
			 0x340538a0, /// 0x100
			 0x50c3f4e6, /// 0x104
			 0xf1a76914, /// 0x108
			 0x6312be04, /// 0x10c
			 0xddcfe370, /// 0x110
			 0xc8b55851, /// 0x114
			 0x886188a2, /// 0x118
			 0x908e68f0, /// 0x11c
			 0x91843378, /// 0x120
			 0xf51b9db7, /// 0x124
			 0x2e6ba4cc, /// 0x128
			 0xb74620d5, /// 0x12c
			 0xe4ff2805, /// 0x130
			 0x682f6df8, /// 0x134
			 0x4f458322, /// 0x138
			 0x0da3401a, /// 0x13c
			 0x96fc3a71, /// 0x140
			 0x8309dbea, /// 0x144
			 0xa06e5a64, /// 0x148
			 0xe50f8917, /// 0x14c
			 0xafb31187, /// 0x150
			 0x7dc66a6e, /// 0x154
			 0x114abf33, /// 0x158
			 0xc884d644, /// 0x15c
			 0xbe944a72, /// 0x160
			 0xb9b40e76, /// 0x164
			 0x1356350a, /// 0x168
			 0x099769fe, /// 0x16c
			 0xf594324e, /// 0x170
			 0x3efe311e, /// 0x174
			 0xbd252de6, /// 0x178
			 0xe71ec24d, /// 0x17c
			 0xddd2623f, /// 0x180
			 0x71c10e22, /// 0x184
			 0x7bfb6aad, /// 0x188
			 0x68af256b, /// 0x18c
			 0x975b55f7, /// 0x190
			 0x7018c4c5, /// 0x194
			 0xab84b42d, /// 0x198
			 0xd16333cb, /// 0x19c
			 0x36db09d7, /// 0x1a0
			 0xf8fabade, /// 0x1a4
			 0xeb069c24, /// 0x1a8
			 0x6928a3c1, /// 0x1ac
			 0x8fb032f0, /// 0x1b0
			 0x99f070be, /// 0x1b4
			 0x9b6d805f, /// 0x1b8
			 0x8624fce8, /// 0x1bc
			 0x7b5d0b82, /// 0x1c0
			 0x3673dd81, /// 0x1c4
			 0xcaf35abd, /// 0x1c8
			 0xd5e55ba7, /// 0x1cc
			 0x07cee283, /// 0x1d0
			 0x9cf9f2cd, /// 0x1d4
			 0x8f816c52, /// 0x1d8
			 0x62241732, /// 0x1dc
			 0x85234243, /// 0x1e0
			 0x057637cb, /// 0x1e4
			 0xe9c67824, /// 0x1e8
			 0x862e4e30, /// 0x1ec
			 0xf95e2b9c, /// 0x1f0
			 0xdaf9965f, /// 0x1f4
			 0xb5e5757c, /// 0x1f8
			 0xdde97c40, /// 0x1fc
			 0x13eec27b, /// 0x200
			 0x3d9a5bb8, /// 0x204
			 0xcc04d8a4, /// 0x208
			 0xb1db33e4, /// 0x20c
			 0x5cf771e7, /// 0x210
			 0x7ebf507f, /// 0x214
			 0x72a49807, /// 0x218
			 0xabc7ca97, /// 0x21c
			 0xc9c5ca97, /// 0x220
			 0xc4955cd9, /// 0x224
			 0xc7a9b02a, /// 0x228
			 0x9ceb59c4, /// 0x22c
			 0x10c47f0e, /// 0x230
			 0x5cf523a9, /// 0x234
			 0x969b525e, /// 0x238
			 0x6a5bb774, /// 0x23c
			 0x17be2783, /// 0x240
			 0x8b284621, /// 0x244
			 0xa0f95d30, /// 0x248
			 0x31e45b7e, /// 0x24c
			 0x13243aa8, /// 0x250
			 0xa0950328, /// 0x254
			 0xb1668f19, /// 0x258
			 0x4d708d7d, /// 0x25c
			 0x1027d0fe, /// 0x260
			 0x6ab083f6, /// 0x264
			 0x493da4b8, /// 0x268
			 0x64db5ff2, /// 0x26c
			 0xfd8c3e91, /// 0x270
			 0xac2277dc, /// 0x274
			 0xc4cc15bb, /// 0x278
			 0xf600404c, /// 0x27c
			 0xfeaae3f0, /// 0x280
			 0xc1ecba44, /// 0x284
			 0xcc3f9e8d, /// 0x288
			 0x39e80749, /// 0x28c
			 0x3b7cef13, /// 0x290
			 0xad6fdb90, /// 0x294
			 0xb59ce306, /// 0x298
			 0x516cfda8, /// 0x29c
			 0xa8f5e03f, /// 0x2a0
			 0x399a8090, /// 0x2a4
			 0x569c1537, /// 0x2a8
			 0x4af2a588, /// 0x2ac
			 0xa7a6679f, /// 0x2b0
			 0x9412d665, /// 0x2b4
			 0xb1ff5099, /// 0x2b8
			 0x7449e737, /// 0x2bc
			 0x26302ae1, /// 0x2c0
			 0x8376706f, /// 0x2c4
			 0x5f0ead10, /// 0x2c8
			 0xf974c802, /// 0x2cc
			 0x0be43d86, /// 0x2d0
			 0x50e29acb, /// 0x2d4
			 0x55c8895c, /// 0x2d8
			 0x84263fc5, /// 0x2dc
			 0x25199a8c, /// 0x2e0
			 0xb87bd2fb, /// 0x2e4
			 0x19d5360d, /// 0x2e8
			 0xac6e0f71, /// 0x2ec
			 0x7cfdb88d, /// 0x2f0
			 0x2d9da5cf, /// 0x2f4
			 0xbd826ab8, /// 0x2f8
			 0x2cbbadf5, /// 0x2fc
			 0x21fe2e0a, /// 0x300
			 0x55b1adf1, /// 0x304
			 0x872c6cf2, /// 0x308
			 0x0343785f, /// 0x30c
			 0x40a86baf, /// 0x310
			 0x757965a8, /// 0x314
			 0x621ccc8c, /// 0x318
			 0xa79190cf, /// 0x31c
			 0xea8d0088, /// 0x320
			 0xcc903360, /// 0x324
			 0xc1cd10db, /// 0x328
			 0x7952385e, /// 0x32c
			 0x07e0d0f2, /// 0x330
			 0x0450ce36, /// 0x334
			 0x860b8141, /// 0x338
			 0x427088c3, /// 0x33c
			 0x78bb8da9, /// 0x340
			 0xa1a30d4c, /// 0x344
			 0x4c2a2607, /// 0x348
			 0xebd823f1, /// 0x34c
			 0x94beb099, /// 0x350
			 0x325d2200, /// 0x354
			 0x32c200c1, /// 0x358
			 0x01242d1b, /// 0x35c
			 0x9fdae496, /// 0x360
			 0xf5980c89, /// 0x364
			 0x57075911, /// 0x368
			 0x3e20f4e8, /// 0x36c
			 0x16e7f281, /// 0x370
			 0x39dea2ee, /// 0x374
			 0x00451047, /// 0x378
			 0xfeab0da0, /// 0x37c
			 0x48ee48c5, /// 0x380
			 0xe43c36c3, /// 0x384
			 0xb112716b, /// 0x388
			 0x071dd38c, /// 0x38c
			 0xa640b307, /// 0x390
			 0xceed063b, /// 0x394
			 0x73ab8d68, /// 0x398
			 0x2bc4faa8, /// 0x39c
			 0x597314b4, /// 0x3a0
			 0x2f328957, /// 0x3a4
			 0xd98ed46d, /// 0x3a8
			 0xae49d1f8, /// 0x3ac
			 0xee5bd74e, /// 0x3b0
			 0xafff3d9d, /// 0x3b4
			 0xb40fc44a, /// 0x3b8
			 0x038045d6, /// 0x3bc
			 0x9818e724, /// 0x3c0
			 0x023be537, /// 0x3c4
			 0x6857dc28, /// 0x3c8
			 0x1bd0a466, /// 0x3cc
			 0x81e769d6, /// 0x3d0
			 0x2c211be6, /// 0x3d4
			 0x462e29ff, /// 0x3d8
			 0x4d39d92e, /// 0x3dc
			 0xde1600c3, /// 0x3e0
			 0x937ea632, /// 0x3e4
			 0xccac7ae6, /// 0x3e8
			 0x343e8d1b, /// 0x3ec
			 0x31fabce7, /// 0x3f0
			 0x5241a02c, /// 0x3f4
			 0x8a25d610, /// 0x3f8
			 0xad9a430c, /// 0x3fc
			 0x9c183d7c, /// 0x400
			 0x23e07efd, /// 0x404
			 0x63ecd433, /// 0x408
			 0x2b5ae5b7, /// 0x40c
			 0xd5154929, /// 0x410
			 0x9b74cb79, /// 0x414
			 0x5dd9b3fd, /// 0x418
			 0xa7f28b59, /// 0x41c
			 0xcc9e61b4, /// 0x420
			 0x50277bed, /// 0x424
			 0x043adcb8, /// 0x428
			 0x449b8447, /// 0x42c
			 0x68052dad, /// 0x430
			 0x79205ec5, /// 0x434
			 0x6e59a018, /// 0x438
			 0xc26195b0, /// 0x43c
			 0x43719b50, /// 0x440
			 0x3839a807, /// 0x444
			 0xeac77958, /// 0x448
			 0xd40418b7, /// 0x44c
			 0x107b85df, /// 0x450
			 0xa87f2970, /// 0x454
			 0x150f1d2d, /// 0x458
			 0x4185a299, /// 0x45c
			 0x7bc1f49f, /// 0x460
			 0xdda3eb28, /// 0x464
			 0xa79a1df3, /// 0x468
			 0x6699a5d9, /// 0x46c
			 0xd119b3a3, /// 0x470
			 0x4a6aedca, /// 0x474
			 0x8a363087, /// 0x478
			 0x1d231963, /// 0x47c
			 0xebe460f0, /// 0x480
			 0x42b04b56, /// 0x484
			 0x45dbb7bf, /// 0x488
			 0x19fa8a2c, /// 0x48c
			 0x4d56101b, /// 0x490
			 0x8c8c0c40, /// 0x494
			 0x8a482673, /// 0x498
			 0x402b80da, /// 0x49c
			 0xdb36f866, /// 0x4a0
			 0x79a8757d, /// 0x4a4
			 0x7778cf96, /// 0x4a8
			 0xe61e639a, /// 0x4ac
			 0x67d05d93, /// 0x4b0
			 0x153683ef, /// 0x4b4
			 0xf65db928, /// 0x4b8
			 0xf27f7fee, /// 0x4bc
			 0x52c1b864, /// 0x4c0
			 0xd3f949b0, /// 0x4c4
			 0xf038c9d1, /// 0x4c8
			 0xd2d2193b, /// 0x4cc
			 0xfe2d6b65, /// 0x4d0
			 0x74eddc45, /// 0x4d4
			 0xe8dd0d13, /// 0x4d8
			 0xbd9df2e8, /// 0x4dc
			 0x1f9e51c7, /// 0x4e0
			 0x805ea755, /// 0x4e4
			 0x7575dc22, /// 0x4e8
			 0xe8c7a732, /// 0x4ec
			 0xe077c552, /// 0x4f0
			 0x763aeb70, /// 0x4f4
			 0x24a65504, /// 0x4f8
			 0x01444cff, /// 0x4fc
			 0xebb974d8, /// 0x500
			 0x0c6221f2, /// 0x504
			 0xad4b77f9, /// 0x508
			 0xf116ab9a, /// 0x50c
			 0xcf4a7d71, /// 0x510
			 0xc5320a87, /// 0x514
			 0x48c7ed52, /// 0x518
			 0x634fec33, /// 0x51c
			 0xc390f5eb, /// 0x520
			 0x98957d69, /// 0x524
			 0x99fee477, /// 0x528
			 0xb75056be, /// 0x52c
			 0x8112df77, /// 0x530
			 0xb4c99d27, /// 0x534
			 0x72aa6b92, /// 0x538
			 0xdb7ce78d, /// 0x53c
			 0x5c5d051b, /// 0x540
			 0x28db9940, /// 0x544
			 0x3a56cee0, /// 0x548
			 0x59cb7466, /// 0x54c
			 0xe57764c6, /// 0x550
			 0x41ff79ed, /// 0x554
			 0xa4ee4398, /// 0x558
			 0x28a3f44b, /// 0x55c
			 0x9145a6a5, /// 0x560
			 0x86a209fb, /// 0x564
			 0x71a748b0, /// 0x568
			 0x66ebadf1, /// 0x56c
			 0x1aac24e5, /// 0x570
			 0x23d4d5d7, /// 0x574
			 0x2ea66c4c, /// 0x578
			 0x655ccf15, /// 0x57c
			 0xc169a46e, /// 0x580
			 0xeb8b6c8e, /// 0x584
			 0x782d1312, /// 0x588
			 0xae5ef167, /// 0x58c
			 0x87f557c9, /// 0x590
			 0xfbbcf7cc, /// 0x594
			 0x73c5932f, /// 0x598
			 0x19530d27, /// 0x59c
			 0x0eebf2cd, /// 0x5a0
			 0xc77bd428, /// 0x5a4
			 0xe6799be5, /// 0x5a8
			 0xeb01538c, /// 0x5ac
			 0x4ed19cd4, /// 0x5b0
			 0xae3b3b8d, /// 0x5b4
			 0xee4298df, /// 0x5b8
			 0x6c1709db, /// 0x5bc
			 0xa7be65e4, /// 0x5c0
			 0x860b39a7, /// 0x5c4
			 0x2ea57996, /// 0x5c8
			 0x2bbbf86a, /// 0x5cc
			 0x0f26f409, /// 0x5d0
			 0x9fb30bcd, /// 0x5d4
			 0x8e0de322, /// 0x5d8
			 0xf3264feb, /// 0x5dc
			 0xd51c33b4, /// 0x5e0
			 0x31ed980f, /// 0x5e4
			 0x54f03518, /// 0x5e8
			 0x5d4627bb, /// 0x5ec
			 0x44e7d699, /// 0x5f0
			 0xa8994e0f, /// 0x5f4
			 0x8c31b5fd, /// 0x5f8
			 0x6b44a05f, /// 0x5fc
			 0x2fdf1922, /// 0x600
			 0xe3cf5cab, /// 0x604
			 0x2ec14939, /// 0x608
			 0x2a7c68b0, /// 0x60c
			 0x6684d121, /// 0x610
			 0x198422ed, /// 0x614
			 0x48c1a7ef, /// 0x618
			 0xc6fe7f33, /// 0x61c
			 0x76d4c1b4, /// 0x620
			 0x81d858c1, /// 0x624
			 0x11bf651a, /// 0x628
			 0x840b9b6a, /// 0x62c
			 0xcc523ca7, /// 0x630
			 0x1f18af1a, /// 0x634
			 0xa7458bcf, /// 0x638
			 0x474f3781, /// 0x63c
			 0xd48f1342, /// 0x640
			 0x674c68eb, /// 0x644
			 0xd65d7514, /// 0x648
			 0x4908875a, /// 0x64c
			 0x0b654e4f, /// 0x650
			 0xc4221d0a, /// 0x654
			 0x7ed3af89, /// 0x658
			 0x11732844, /// 0x65c
			 0xab24af16, /// 0x660
			 0xefbfa6ea, /// 0x664
			 0xf234a325, /// 0x668
			 0x78ae6646, /// 0x66c
			 0x74664498, /// 0x670
			 0x502066d9, /// 0x674
			 0xf31723e7, /// 0x678
			 0x9351abac, /// 0x67c
			 0x6c058a27, /// 0x680
			 0x92afb1e9, /// 0x684
			 0x73ff5a51, /// 0x688
			 0xafa3d766, /// 0x68c
			 0x42954543, /// 0x690
			 0x365ed0d2, /// 0x694
			 0x3a373766, /// 0x698
			 0xd6443202, /// 0x69c
			 0x8cbdbc93, /// 0x6a0
			 0xe80798f2, /// 0x6a4
			 0xfcf879f9, /// 0x6a8
			 0xb7a21e1e, /// 0x6ac
			 0x7c5845aa, /// 0x6b0
			 0x266aefc0, /// 0x6b4
			 0x7ced754a, /// 0x6b8
			 0xc6eb9b3c, /// 0x6bc
			 0xa62651e6, /// 0x6c0
			 0x821a03e6, /// 0x6c4
			 0x6a579439, /// 0x6c8
			 0x4f6a81e8, /// 0x6cc
			 0x15fdff36, /// 0x6d0
			 0x4d4b17d7, /// 0x6d4
			 0xf006f48b, /// 0x6d8
			 0x788266ac, /// 0x6dc
			 0x34692aca, /// 0x6e0
			 0xf4a176c7, /// 0x6e4
			 0x45a405b9, /// 0x6e8
			 0x78953e44, /// 0x6ec
			 0x80392bef, /// 0x6f0
			 0x36020537, /// 0x6f4
			 0x1df83065, /// 0x6f8
			 0x154d1582, /// 0x6fc
			 0xc556ed72, /// 0x700
			 0x86edf5a6, /// 0x704
			 0x68b6a8cf, /// 0x708
			 0x94724ffa, /// 0x70c
			 0x47f0a4f4, /// 0x710
			 0x63ef1373, /// 0x714
			 0x7d632580, /// 0x718
			 0xc2cc4cdd, /// 0x71c
			 0x73226792, /// 0x720
			 0x4d929bc6, /// 0x724
			 0x0018df88, /// 0x728
			 0x2cac9bf0, /// 0x72c
			 0x558f4374, /// 0x730
			 0xcee13413, /// 0x734
			 0x10ee9663, /// 0x738
			 0xfa992371, /// 0x73c
			 0x429d51a7, /// 0x740
			 0x0d3d9e65, /// 0x744
			 0x8f734588, /// 0x748
			 0xc2313beb, /// 0x74c
			 0x64aad06f, /// 0x750
			 0xfcc6538b, /// 0x754
			 0x17bda9d8, /// 0x758
			 0x59223b4a, /// 0x75c
			 0x747463ed, /// 0x760
			 0x9021eba8, /// 0x764
			 0x225e9b6c, /// 0x768
			 0xa4257649, /// 0x76c
			 0x1a078945, /// 0x770
			 0x945f3b88, /// 0x774
			 0xad155988, /// 0x778
			 0xce7b6d22, /// 0x77c
			 0xf42a90db, /// 0x780
			 0x05f58e7e, /// 0x784
			 0x290f951d, /// 0x788
			 0xe62712b8, /// 0x78c
			 0x16ba3821, /// 0x790
			 0x65c23e7b, /// 0x794
			 0x4e25c3ae, /// 0x798
			 0xcef399cf, /// 0x79c
			 0xc2b5ac3e, /// 0x7a0
			 0xa8f79dbc, /// 0x7a4
			 0xfb2bfb1c, /// 0x7a8
			 0xf2d0c705, /// 0x7ac
			 0xdd122465, /// 0x7b0
			 0x81888071, /// 0x7b4
			 0x82e7dbc1, /// 0x7b8
			 0x8ab2f131, /// 0x7bc
			 0xc234e57d, /// 0x7c0
			 0x68cec8cf, /// 0x7c4
			 0x88820d78, /// 0x7c8
			 0x256c1332, /// 0x7cc
			 0x9f824a48, /// 0x7d0
			 0x3fb44177, /// 0x7d4
			 0xcf65492b, /// 0x7d8
			 0xe899868e, /// 0x7dc
			 0x6e467ae1, /// 0x7e0
			 0x3daca0cc, /// 0x7e4
			 0x6f9321d9, /// 0x7e8
			 0x1cd21dfe, /// 0x7ec
			 0x52bb602c, /// 0x7f0
			 0xaf3e2474, /// 0x7f4
			 0x633acb8d, /// 0x7f8
			 0xa2a277b2, /// 0x7fc
			 0x2e90e61b, /// 0x800
			 0xbce47f4e, /// 0x804
			 0x56ae68df, /// 0x808
			 0xa85d6e08, /// 0x80c
			 0x2dd9af33, /// 0x810
			 0xbe7c67b1, /// 0x814
			 0x4b864951, /// 0x818
			 0xc48c5142, /// 0x81c
			 0xe17e7cc7, /// 0x820
			 0xb1938b2a, /// 0x824
			 0x03fbad7e, /// 0x828
			 0xd9a10859, /// 0x82c
			 0xc156d0f5, /// 0x830
			 0x24cd22ae, /// 0x834
			 0x00fa9737, /// 0x838
			 0x83ea924a, /// 0x83c
			 0x21eb6eb7, /// 0x840
			 0xc94dd9e9, /// 0x844
			 0x35b4caba, /// 0x848
			 0x31ebc4e8, /// 0x84c
			 0x17b82a51, /// 0x850
			 0x2f6fff6d, /// 0x854
			 0x6822f8ce, /// 0x858
			 0x7ac6a843, /// 0x85c
			 0xdb45c5d2, /// 0x860
			 0xd255d102, /// 0x864
			 0x23108375, /// 0x868
			 0x7378bc79, /// 0x86c
			 0x6f3c2d85, /// 0x870
			 0x76bdc704, /// 0x874
			 0x2c96ae5d, /// 0x878
			 0x21a15e89, /// 0x87c
			 0xf6dffed9, /// 0x880
			 0x223bbe6b, /// 0x884
			 0xd5009219, /// 0x888
			 0xaa2ae815, /// 0x88c
			 0x295cf953, /// 0x890
			 0x8241a845, /// 0x894
			 0xff5ea806, /// 0x898
			 0x41372443, /// 0x89c
			 0xc30d1680, /// 0x8a0
			 0x0612ecee, /// 0x8a4
			 0x57aa66d1, /// 0x8a8
			 0xa7373036, /// 0x8ac
			 0x788665c4, /// 0x8b0
			 0xff38f834, /// 0x8b4
			 0xd1d1677f, /// 0x8b8
			 0x663862c7, /// 0x8bc
			 0x56de8675, /// 0x8c0
			 0x6e1efdeb, /// 0x8c4
			 0xe0193f4a, /// 0x8c8
			 0x35472230, /// 0x8cc
			 0xcf5828b7, /// 0x8d0
			 0x3a1328a8, /// 0x8d4
			 0xf985c00b, /// 0x8d8
			 0xf1e732a6, /// 0x8dc
			 0x08e64d45, /// 0x8e0
			 0x7c668d69, /// 0x8e4
			 0x18bb7439, /// 0x8e8
			 0xf85792d9, /// 0x8ec
			 0x5126f881, /// 0x8f0
			 0xe885ce47, /// 0x8f4
			 0x3567f1c8, /// 0x8f8
			 0xe37018b4, /// 0x8fc
			 0x00b02fd1, /// 0x900
			 0xa3344097, /// 0x904
			 0x7da5fa3a, /// 0x908
			 0x3e20cde7, /// 0x90c
			 0xcd5f2290, /// 0x910
			 0x0333b5a4, /// 0x914
			 0xb5cdb278, /// 0x918
			 0x2f3fff44, /// 0x91c
			 0x6b450726, /// 0x920
			 0x1692804c, /// 0x924
			 0x5a4a2a95, /// 0x928
			 0xe1d2dde9, /// 0x92c
			 0x9c5e07a1, /// 0x930
			 0x2180f282, /// 0x934
			 0x024cdb8c, /// 0x938
			 0xaeffb652, /// 0x93c
			 0xf580ebbc, /// 0x940
			 0x38a46c6b, /// 0x944
			 0xeb9ccca8, /// 0x948
			 0xda718927, /// 0x94c
			 0xed88158c, /// 0x950
			 0x87db4dfd, /// 0x954
			 0x3c8e9413, /// 0x958
			 0xd41913b9, /// 0x95c
			 0x042d045d, /// 0x960
			 0xd519b835, /// 0x964
			 0x699c7401, /// 0x968
			 0x6ce63d3b, /// 0x96c
			 0x1bac3ea5, /// 0x970
			 0xf4c35a41, /// 0x974
			 0xdf628bbe, /// 0x978
			 0xbc2f8a5c, /// 0x97c
			 0xbc12ea6a, /// 0x980
			 0xd65ce5de, /// 0x984
			 0xb685be05, /// 0x988
			 0x698880d4, /// 0x98c
			 0x613ba9f9, /// 0x990
			 0x91d3d1e3, /// 0x994
			 0xfa425859, /// 0x998
			 0x35f91772, /// 0x99c
			 0xed2d639e, /// 0x9a0
			 0x933d07a4, /// 0x9a4
			 0xcc32e061, /// 0x9a8
			 0xf703b4d9, /// 0x9ac
			 0x69fd62de, /// 0x9b0
			 0xeb21b40a, /// 0x9b4
			 0x20430a74, /// 0x9b8
			 0x1df679cf, /// 0x9bc
			 0xadcdaa23, /// 0x9c0
			 0x222ad8e8, /// 0x9c4
			 0xb2b85630, /// 0x9c8
			 0x67aa8e62, /// 0x9cc
			 0xa3555454, /// 0x9d0
			 0x199d9172, /// 0x9d4
			 0xa97b3b27, /// 0x9d8
			 0xccefbbc1, /// 0x9dc
			 0x6c8b6cd0, /// 0x9e0
			 0xf50e0c6a, /// 0x9e4
			 0x9808d4a6, /// 0x9e8
			 0x2e2ad817, /// 0x9ec
			 0xb2a8d9de, /// 0x9f0
			 0x3e785e69, /// 0x9f4
			 0xa304b6f6, /// 0x9f8
			 0xc2edcd17, /// 0x9fc
			 0x014ad98f, /// 0xa00
			 0x85b3802a, /// 0xa04
			 0xd7f44f9b, /// 0xa08
			 0xa4cd960d, /// 0xa0c
			 0xaa701117, /// 0xa10
			 0xee0845be, /// 0xa14
			 0xc744ff93, /// 0xa18
			 0x1f672bf1, /// 0xa1c
			 0xb2b0e377, /// 0xa20
			 0x083bd222, /// 0xa24
			 0x0f5577cb, /// 0xa28
			 0x831847a8, /// 0xa2c
			 0x1c577447, /// 0xa30
			 0x45513bd5, /// 0xa34
			 0x5892f906, /// 0xa38
			 0xf8b6e832, /// 0xa3c
			 0x464d9220, /// 0xa40
			 0xcd1d14ef, /// 0xa44
			 0x2e21df2b, /// 0xa48
			 0xe3f87dfd, /// 0xa4c
			 0xeca2d078, /// 0xa50
			 0xc0382771, /// 0xa54
			 0xc3472957, /// 0xa58
			 0xbf14e56c, /// 0xa5c
			 0x22ad052b, /// 0xa60
			 0x93f71bc4, /// 0xa64
			 0x068b1932, /// 0xa68
			 0x3bfff9c5, /// 0xa6c
			 0xe2952d5c, /// 0xa70
			 0xc1e9470a, /// 0xa74
			 0x4542e217, /// 0xa78
			 0x95373912, /// 0xa7c
			 0xf2ba880f, /// 0xa80
			 0x21918a73, /// 0xa84
			 0x4e29729c, /// 0xa88
			 0xb480daa5, /// 0xa8c
			 0xb4e18729, /// 0xa90
			 0x4e498736, /// 0xa94
			 0x45bd8b99, /// 0xa98
			 0xfe50982e, /// 0xa9c
			 0x906287dd, /// 0xaa0
			 0x106ce080, /// 0xaa4
			 0x54b1dcb5, /// 0xaa8
			 0x9a14d758, /// 0xaac
			 0x74dac8ec, /// 0xab0
			 0xe18d8845, /// 0xab4
			 0x1c6b849d, /// 0xab8
			 0x7a0c3d0f, /// 0xabc
			 0x80b65373, /// 0xac0
			 0xbdfe49ec, /// 0xac4
			 0xfe77464e, /// 0xac8
			 0x20100a06, /// 0xacc
			 0x224772f0, /// 0xad0
			 0x747a7a41, /// 0xad4
			 0x35635588, /// 0xad8
			 0x5e647c51, /// 0xadc
			 0xbe57c641, /// 0xae0
			 0x7ed23b3d, /// 0xae4
			 0x45979cd8, /// 0xae8
			 0x0e6886d6, /// 0xaec
			 0x45818ac4, /// 0xaf0
			 0xe2b2eba7, /// 0xaf4
			 0x80991fb7, /// 0xaf8
			 0x02acc671, /// 0xafc
			 0xed27dd58, /// 0xb00
			 0x56b21f60, /// 0xb04
			 0xb1f7a99a, /// 0xb08
			 0x85028b5e, /// 0xb0c
			 0x25cfae8e, /// 0xb10
			 0xf577b5f2, /// 0xb14
			 0x431dca81, /// 0xb18
			 0x6a0be906, /// 0xb1c
			 0x11692e3b, /// 0xb20
			 0x9fb81587, /// 0xb24
			 0x93f1ef9e, /// 0xb28
			 0x7533d1b4, /// 0xb2c
			 0x9928f6a3, /// 0xb30
			 0x04e676ff, /// 0xb34
			 0x2493f745, /// 0xb38
			 0xd1a61039, /// 0xb3c
			 0x6b694ecb, /// 0xb40
			 0xfe461a73, /// 0xb44
			 0x631298a5, /// 0xb48
			 0x2b4b3964, /// 0xb4c
			 0x3d9882ef, /// 0xb50
			 0xf97cf92e, /// 0xb54
			 0x6894e41d, /// 0xb58
			 0x3330ab22, /// 0xb5c
			 0x3914febb, /// 0xb60
			 0xfa2bc815, /// 0xb64
			 0x2f38d6d5, /// 0xb68
			 0x610a8f1f, /// 0xb6c
			 0x1b29a667, /// 0xb70
			 0xd0708564, /// 0xb74
			 0xa3f5837d, /// 0xb78
			 0xb8c84466, /// 0xb7c
			 0x7d566c51, /// 0xb80
			 0xfd66cca2, /// 0xb84
			 0x90ac0bf6, /// 0xb88
			 0xb3d03393, /// 0xb8c
			 0x95888c87, /// 0xb90
			 0x9c154e10, /// 0xb94
			 0x1de03a0a, /// 0xb98
			 0x0d0a198a, /// 0xb9c
			 0x745558b8, /// 0xba0
			 0x28756644, /// 0xba4
			 0x04c1e721, /// 0xba8
			 0xe58b8c96, /// 0xbac
			 0x5168c4ac, /// 0xbb0
			 0x0621da14, /// 0xbb4
			 0x45c36014, /// 0xbb8
			 0xe7e79f1a, /// 0xbbc
			 0xe7d95289, /// 0xbc0
			 0xf4c9a464, /// 0xbc4
			 0xadc16d4f, /// 0xbc8
			 0xf8d832e1, /// 0xbcc
			 0x8268f337, /// 0xbd0
			 0x462f6bb1, /// 0xbd4
			 0xbf85f636, /// 0xbd8
			 0xba9f443d, /// 0xbdc
			 0x09890dd2, /// 0xbe0
			 0xbb998c2c, /// 0xbe4
			 0x03d40cbc, /// 0xbe8
			 0xb705257b, /// 0xbec
			 0xa3408d10, /// 0xbf0
			 0x709b3c71, /// 0xbf4
			 0x5b1ca23c, /// 0xbf8
			 0x42836e9a, /// 0xbfc
			 0x5b6007be, /// 0xc00
			 0x3177e630, /// 0xc04
			 0x060b20cd, /// 0xc08
			 0xc78dad04, /// 0xc0c
			 0x26ab1acd, /// 0xc10
			 0x77abdc5a, /// 0xc14
			 0x3afd92ab, /// 0xc18
			 0xc25a071b, /// 0xc1c
			 0xe05c3d26, /// 0xc20
			 0xdf290c71, /// 0xc24
			 0x513cdf76, /// 0xc28
			 0xaab10d78, /// 0xc2c
			 0x60443e9c, /// 0xc30
			 0xac3b721c, /// 0xc34
			 0x251ee254, /// 0xc38
			 0xb96a5e60, /// 0xc3c
			 0x4715bef6, /// 0xc40
			 0x3e6acb43, /// 0xc44
			 0xb8c6de26, /// 0xc48
			 0x3b89c7b0, /// 0xc4c
			 0x77a2b0ac, /// 0xc50
			 0xe2beb304, /// 0xc54
			 0x79a62a55, /// 0xc58
			 0x760d80ee, /// 0xc5c
			 0x6e635974, /// 0xc60
			 0x3f7e08a5, /// 0xc64
			 0xad1fec0c, /// 0xc68
			 0x6b266657, /// 0xc6c
			 0x681fd26c, /// 0xc70
			 0x869f5947, /// 0xc74
			 0xa9b7a00a, /// 0xc78
			 0x7d0a6372, /// 0xc7c
			 0xbb8f3335, /// 0xc80
			 0x901155e3, /// 0xc84
			 0x46139034, /// 0xc88
			 0x6e3b4eca, /// 0xc8c
			 0x753a1bde, /// 0xc90
			 0xed60c589, /// 0xc94
			 0x9a3225ca, /// 0xc98
			 0x8127e235, /// 0xc9c
			 0xe3e20f20, /// 0xca0
			 0xf04c2204, /// 0xca4
			 0xeef27479, /// 0xca8
			 0xfa70c501, /// 0xcac
			 0x88f519b9, /// 0xcb0
			 0xa25ab69e, /// 0xcb4
			 0x6fe537bd, /// 0xcb8
			 0x09bd8552, /// 0xcbc
			 0x1fbfb939, /// 0xcc0
			 0x0b1784c9, /// 0xcc4
			 0x847bd1f9, /// 0xcc8
			 0x17b6c238, /// 0xccc
			 0x56e57f38, /// 0xcd0
			 0xb09328bc, /// 0xcd4
			 0x8c613d2c, /// 0xcd8
			 0xd4c773d4, /// 0xcdc
			 0x0796896b, /// 0xce0
			 0x9668f3cc, /// 0xce4
			 0x47c1e7ca, /// 0xce8
			 0xcc435fd0, /// 0xcec
			 0xc8e65eba, /// 0xcf0
			 0x7562dcc1, /// 0xcf4
			 0x93621fe1, /// 0xcf8
			 0x30b7aade, /// 0xcfc
			 0x84a53a05, /// 0xd00
			 0x59899ac1, /// 0xd04
			 0xcd370bdb, /// 0xd08
			 0x5c0ce8ad, /// 0xd0c
			 0x3717db22, /// 0xd10
			 0xe82f8e1d, /// 0xd14
			 0x2998124b, /// 0xd18
			 0xbf76eb3d, /// 0xd1c
			 0x06a9de7b, /// 0xd20
			 0x0f171270, /// 0xd24
			 0x845a0631, /// 0xd28
			 0x78db3ec7, /// 0xd2c
			 0x4542f31e, /// 0xd30
			 0x9a57a531, /// 0xd34
			 0x6d30983d, /// 0xd38
			 0x4ed9ddac, /// 0xd3c
			 0xf7bee1c5, /// 0xd40
			 0x20746a50, /// 0xd44
			 0xf228040c, /// 0xd48
			 0x24688de5, /// 0xd4c
			 0x44d808fc, /// 0xd50
			 0x1a0a4d72, /// 0xd54
			 0x7fd9e30c, /// 0xd58
			 0x3aa4a731, /// 0xd5c
			 0xf1cf141e, /// 0xd60
			 0x8c7b3675, /// 0xd64
			 0xcd279388, /// 0xd68
			 0x3b69fe4e, /// 0xd6c
			 0x85107d2f, /// 0xd70
			 0xd0f39345, /// 0xd74
			 0x7279747c, /// 0xd78
			 0x3b395dad, /// 0xd7c
			 0x8797fa73, /// 0xd80
			 0xc384cb2e, /// 0xd84
			 0x5add70b9, /// 0xd88
			 0x1e778805, /// 0xd8c
			 0x311e5377, /// 0xd90
			 0x94178254, /// 0xd94
			 0x1caeec33, /// 0xd98
			 0xc94f283c, /// 0xd9c
			 0xf0d80796, /// 0xda0
			 0xc8889728, /// 0xda4
			 0xf7687bdf, /// 0xda8
			 0xc9df2163, /// 0xdac
			 0xacab2ffd, /// 0xdb0
			 0x2a4de1c2, /// 0xdb4
			 0x5d586593, /// 0xdb8
			 0xe088f36d, /// 0xdbc
			 0x0564303e, /// 0xdc0
			 0xbebc378c, /// 0xdc4
			 0x2baf777c, /// 0xdc8
			 0x68aede5a, /// 0xdcc
			 0xce21d9c4, /// 0xdd0
			 0xef145d52, /// 0xdd4
			 0xaed94a32, /// 0xdd8
			 0x9301787c, /// 0xddc
			 0x9d39cca8, /// 0xde0
			 0x55679f2d, /// 0xde4
			 0xd9609dae, /// 0xde8
			 0x4d9aa303, /// 0xdec
			 0x6063ec65, /// 0xdf0
			 0x6cd693e3, /// 0xdf4
			 0x1611c8b7, /// 0xdf8
			 0x1b2db4b1, /// 0xdfc
			 0xffbb0222, /// 0xe00
			 0x7139fc47, /// 0xe04
			 0x6b8eb529, /// 0xe08
			 0x04d28afe, /// 0xe0c
			 0xb8ef2270, /// 0xe10
			 0x52b5aeda, /// 0xe14
			 0x6dffdb95, /// 0xe18
			 0xdb767bbf, /// 0xe1c
			 0x31cbabd5, /// 0xe20
			 0x0c0b27be, /// 0xe24
			 0x1382c4b8, /// 0xe28
			 0x9d6a2b65, /// 0xe2c
			 0x08ab33c8, /// 0xe30
			 0x6fcf6d17, /// 0xe34
			 0x538158b4, /// 0xe38
			 0x108ba7f7, /// 0xe3c
			 0x6098ece0, /// 0xe40
			 0x4c5ba30d, /// 0xe44
			 0x76272204, /// 0xe48
			 0x59a0419a, /// 0xe4c
			 0x0f121c7e, /// 0xe50
			 0xf010c5ba, /// 0xe54
			 0x445e0d31, /// 0xe58
			 0x81fc2551, /// 0xe5c
			 0xdcc2e73a, /// 0xe60
			 0x2b3de4e6, /// 0xe64
			 0x46a36cc4, /// 0xe68
			 0x30dc4c25, /// 0xe6c
			 0xa2ce9474, /// 0xe70
			 0xc81ff297, /// 0xe74
			 0x390324a7, /// 0xe78
			 0x156bf85d, /// 0xe7c
			 0x91308fc4, /// 0xe80
			 0x4ee4df93, /// 0xe84
			 0xdaa8584f, /// 0xe88
			 0x467a0675, /// 0xe8c
			 0x9ae48f06, /// 0xe90
			 0x8bc39b0e, /// 0xe94
			 0x3dfe9934, /// 0xe98
			 0x8e48da6a, /// 0xe9c
			 0x670f0977, /// 0xea0
			 0xd90abfbf, /// 0xea4
			 0xbae3073d, /// 0xea8
			 0x2dac02a3, /// 0xeac
			 0xc48abb2e, /// 0xeb0
			 0xb349ab18, /// 0xeb4
			 0xfce1c7a7, /// 0xeb8
			 0xb850dfb4, /// 0xebc
			 0x94fca47b, /// 0xec0
			 0xc923762a, /// 0xec4
			 0x5d5f712a, /// 0xec8
			 0xbe35bd12, /// 0xecc
			 0x27a3d971, /// 0xed0
			 0x4273c8df, /// 0xed4
			 0xb94f3113, /// 0xed8
			 0xb3812a12, /// 0xedc
			 0x177ac0b5, /// 0xee0
			 0x6a3b4af7, /// 0xee4
			 0x486b2067, /// 0xee8
			 0xd686b2d9, /// 0xeec
			 0x306b4374, /// 0xef0
			 0x10692d0e, /// 0xef4
			 0x5f501faa, /// 0xef8
			 0xef62054a, /// 0xefc
			 0xf5d616c2, /// 0xf00
			 0xef770e31, /// 0xf04
			 0x267a1065, /// 0xf08
			 0xf06a76a4, /// 0xf0c
			 0x465a90f8, /// 0xf10
			 0x107f47f1, /// 0xf14
			 0x69b23af1, /// 0xf18
			 0x8dff5673, /// 0xf1c
			 0x3c8f0bed, /// 0xf20
			 0x8fe65b75, /// 0xf24
			 0xecba2857, /// 0xf28
			 0x8ccc2995, /// 0xf2c
			 0xa27d65c7, /// 0xf30
			 0x1c793f1a, /// 0xf34
			 0x825ccd25, /// 0xf38
			 0x8754244a, /// 0xf3c
			 0x11aab245, /// 0xf40
			 0x71eee209, /// 0xf44
			 0x28a7308f, /// 0xf48
			 0x7a718a0a, /// 0xf4c
			 0xc42115d5, /// 0xf50
			 0xd6811fef, /// 0xf54
			 0xbfee882f, /// 0xf58
			 0x3fcb60a7, /// 0xf5c
			 0x86b9547d, /// 0xf60
			 0x4952b481, /// 0xf64
			 0x9c131a6d, /// 0xf68
			 0xca0da793, /// 0xf6c
			 0xd2d6cde2, /// 0xf70
			 0xa4c891c4, /// 0xf74
			 0xe21a2ccc, /// 0xf78
			 0x966f35ba, /// 0xf7c
			 0x06ae86c5, /// 0xf80
			 0xcb4e1563, /// 0xf84
			 0xc311d3a4, /// 0xf88
			 0xcc1fc85b, /// 0xf8c
			 0x9d61fc34, /// 0xf90
			 0xff00f7ca, /// 0xf94
			 0xca059316, /// 0xf98
			 0xe082dc6c, /// 0xf9c
			 0xc764c399, /// 0xfa0
			 0x6fe82185, /// 0xfa4
			 0x8573bf87, /// 0xfa8
			 0x81cdbfae, /// 0xfac
			 0xb453fba0, /// 0xfb0
			 0x7d939098, /// 0xfb4
			 0xcc926536, /// 0xfb8
			 0x6e951c2a, /// 0xfbc
			 0xd7b7820e, /// 0xfc0
			 0xc97d02c5, /// 0xfc4
			 0xfe20755f, /// 0xfc8
			 0x0da98d7c, /// 0xfcc
			 0x78ca3345, /// 0xfd0
			 0x18a2c604, /// 0xfd4
			 0x5f9418e1, /// 0xfd8
			 0xa5f81026, /// 0xfdc
			 0xc4711f25, /// 0xfe0
			 0x8d6ad065, /// 0xfe4
			 0x377d9d84, /// 0xfe8
			 0x36080f5a, /// 0xfec
			 0x51e9635d, /// 0xff0
			 0x8ffc1f64, /// 0xff4
			 0xa3d66a33, /// 0xff8
			 0xbfa3dfbb /// last
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
			 0x00000624,
			 0x000003c4,
			 0x000005f8,
			 0x00000478,
			 0x0000025c,
			 0x00000670,
			 0x000005d0,
			 0x000002d0,

			 /// vpu register f2
			 0x3f800000,
			 0x41c80000,
			 0x41c00000,
			 0x41a00000,
			 0x41880000,
			 0x3f800000,
			 0x41500000,
			 0x41980000,

			 /// vpu register f3
			 0x41600000,
			 0x41500000,
			 0x40e00000,
			 0x41a80000,
			 0x41c80000,
			 0x41980000,
			 0x41200000,
			 0x41d00000,

			 /// vpu register f4
			 0x41d80000,
			 0x41e00000,
			 0x41700000,
			 0x41a00000,
			 0x41400000,
			 0x41000000,
			 0x41900000,
			 0x41d80000,

			 /// vpu register f5
			 0x41d00000,
			 0x41b00000,
			 0x41500000,
			 0x3f800000,
			 0x41a00000,
			 0x41900000,
			 0x41880000,
			 0x41300000,

			 /// vpu register f6
			 0x40000000,
			 0x41600000,
			 0x41b00000,
			 0x41d00000,
			 0x41200000,
			 0x41d00000,
			 0x41a80000,
			 0x41800000,

			 /// vpu register f7
			 0x3f800000,
			 0x41200000,
			 0x42000000,
			 0x41e00000,
			 0x41200000,
			 0x41c80000,
			 0x41d80000,
			 0x41600000,

			 /// vpu register f8
			 0x41880000,
			 0x41600000,
			 0x41500000,
			 0x40e00000,
			 0x41e00000,
			 0x41000000,
			 0x41900000,
			 0x41c00000,

			 /// vpu register f9
			 0x41d00000,
			 0x41880000,
			 0x41a80000,
			 0x41d00000,
			 0x40e00000,
			 0x41b80000,
			 0x41700000,
			 0x41300000,

			 /// vpu register f10
			 0x41400000,
			 0x41100000,
			 0x41e00000,
			 0x41900000,
			 0x41400000,
			 0x41880000,
			 0x41700000,
			 0x41d80000,

			 /// vpu register f11
			 0x40e00000,
			 0x41400000,
			 0x40a00000,
			 0x41c00000,
			 0x40400000,
			 0x42000000,
			 0x41900000,
			 0x41c80000,

			 /// vpu register f12
			 0x41880000,
			 0x41880000,
			 0x41f00000,
			 0x40400000,
			 0x41f00000,
			 0x41000000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f13
			 0x41880000,
			 0x40c00000,
			 0x41980000,
			 0x41700000,
			 0x41500000,
			 0x41800000,
			 0x40000000,
			 0x41a80000,

			 /// vpu register f14
			 0x40a00000,
			 0x40c00000,
			 0x40000000,
			 0x41a00000,
			 0x41e80000,
			 0x41100000,
			 0x40c00000,
			 0x41b00000,

			 /// vpu register f15
			 0x41e80000,
			 0x41e80000,
			 0x41600000,
			 0x41880000,
			 0x41a00000,
			 0x40000000,
			 0x41d00000,
			 0x41880000,

			 /// vpu register f16
			 0x41b80000,
			 0x41e80000,
			 0x40800000,
			 0x40400000,
			 0x41900000,
			 0x41e80000,
			 0x41b00000,
			 0x41500000,

			 /// vpu register f17
			 0x41e00000,
			 0x41980000,
			 0x41980000,
			 0x41a00000,
			 0x41a80000,
			 0x41b80000,
			 0x41400000,
			 0x41d00000,

			 /// vpu register f18
			 0x41d00000,
			 0x41d00000,
			 0x41e00000,
			 0x3f800000,
			 0x40e00000,
			 0x41980000,
			 0x41980000,
			 0x41600000,

			 /// vpu register f19
			 0x41000000,
			 0x40a00000,
			 0x41f00000,
			 0x41f80000,
			 0x41100000,
			 0x3f800000,
			 0x40000000,
			 0x41e80000,

			 /// vpu register f20
			 0x41880000,
			 0x41200000,
			 0x3f800000,
			 0x40400000,
			 0x41a80000,
			 0x41300000,
			 0x41f00000,
			 0x41c80000,

			 /// vpu register f21
			 0x40a00000,
			 0x41f80000,
			 0x40a00000,
			 0x41500000,
			 0x41e80000,
			 0x41100000,
			 0x41200000,
			 0x40800000,

			 /// vpu register f22
			 0x41600000,
			 0x41000000,
			 0x41300000,
			 0x41b00000,
			 0x41b80000,
			 0x41e00000,
			 0x41a80000,
			 0x41c80000,

			 /// vpu register f23
			 0x41600000,
			 0x41100000,
			 0x41980000,
			 0x41000000,
			 0x41100000,
			 0x41f80000,
			 0x41f80000,
			 0x41b00000,

			 /// vpu register f24
			 0x40800000,
			 0x41f80000,
			 0x41980000,
			 0x41e80000,
			 0x40800000,
			 0x41300000,
			 0x41e80000,
			 0x41200000,

			 /// vpu register f25
			 0x41f80000,
			 0x40a00000,
			 0x3f800000,
			 0x41b00000,
			 0x41400000,
			 0x41e00000,
			 0x41d00000,
			 0x41a80000,

			 /// vpu register f26
			 0x41e00000,
			 0x41a00000,
			 0x41b00000,
			 0x41e80000,
			 0x41700000,
			 0x40000000,
			 0x41200000,
			 0x41a80000,

			 /// vpu register f27
			 0x41880000,
			 0x41300000,
			 0x41700000,
			 0x42000000,
			 0x42000000,
			 0x41a80000,
			 0x40a00000,
			 0x41e00000,

			 /// vpu register f28
			 0x40e00000,
			 0x40800000,
			 0x41d80000,
			 0x41800000,
			 0x41a80000,
			 0x40a00000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f29
			 0x41900000,
			 0x41b80000,
			 0x3f800000,
			 0x40c00000,
			 0x41500000,
			 0x42000000,
			 0x41b80000,
			 0x40000000,

			 /// vpu register f30
			 0x41e80000,
			 0x41d80000,
			 0x41a80000,
			 0x41a00000,
			 0x41100000,
			 0x41a00000,
			 0x41100000,
			 0x41800000,

			 /// vpu register f31
			 0x41c80000,
			 0x41880000,
			 0x41c00000,
			 0x41b00000,
			 0x41a00000,
			 0x40000000,
			 0x40c00000,
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
		//RTLMIN-5479"fsin.ps f8, f26\n"                                   ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f12, f13\n"                                  ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f17, f14\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f26\n"                                  ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f24, f19\n"                                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f5, f15\n"                                   ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f30, f29\n"                                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f10, f15\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f23, f13\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f6, f27\n"                                   ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f12, f22\n"                                  ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f16, f12\n"                                  ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f12, f4\n"                                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f7, f7\n"                                    ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f18, f16\n"                                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f24, f3\n"                                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f25, f6\n"                                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f30, f21\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f8, f2\n"                                    ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f3, f16\n"                                   ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f12\n"                                  ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f10, f14\n"                                  ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f2, f3\n"                                    ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f19\n"                                  ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f24, f6\n"                                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f8\n"                                   ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f13, f25\n"                                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f25, f22\n"                                  ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f27, f21\n"                                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f26, f23\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f4, f21\n"                                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f19\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f25, f21\n"                                  ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f3, f20\n"                                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f20, f6\n"                                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f18\n"                                  ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f23, f22\n"                                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f25, f2\n"                                   ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f22\n"                                  ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f10, f25\n"                                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f3, f27\n"                                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f7, f10\n"                                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f24, f8\n"                                   ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f18, f2\n"                                   ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f19, f18\n"                                  ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f16, f23\n"                                  ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f28, f26\n"                                  ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f11, f22\n"                                  ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f20, f2\n"                                   ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f3, f10\n"                                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f14, f22\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f4, f29\n"                                   ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f30, f23\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f12, f23\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f25, f11\n"                                  ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f5, f18\n"                                   ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f25, f22\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f29\n"                                  ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f15, f9\n"                                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f11, f16\n"                                  ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f5, f28\n"                                   ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f20, f1\n"                                   ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f14, f2\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f4\n"                                   ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f4, f8\n"                                    ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f17, f27\n"                                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f16, f3\n"                                   ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f0\n"                                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f20\n"                                  ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f15\n"                                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f24, f29\n"                                  ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f21, f11\n"                                  ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f2, f17\n"                                   ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f8, f16\n"                                   ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f12, f15\n"                                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f11, f18\n"                                  ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f29, f24\n"                                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f16, f18\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f5, f19\n"                                   ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f17, f19\n"                                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f13, f19\n"                                  ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f5, f2\n"                                    ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f10, f19\n"                                  ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f8, f21\n"                                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f27, f7\n"                                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f19, f30\n"                                  ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f27, f21\n"                                  ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f7, f25\n"                                   ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f12, f24\n"                                  ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f6, f23\n"                                   ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f28, f13\n"                                  ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f15, f25\n"                                  ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f30, f19\n"                                  ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f11, f30\n"                                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f22, f14\n"                                  ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f15, f3\n"                                   ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f5, f5\n"                                    ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f3, f1\n"                                    ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		//RTLMIN-5479"fsin.ps f28, f12\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
