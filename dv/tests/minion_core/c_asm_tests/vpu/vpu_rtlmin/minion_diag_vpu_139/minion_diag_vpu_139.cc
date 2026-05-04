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
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00000
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0000c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00014
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00024
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00028
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0002c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00030
			 0xff800000,                                                  // -inf                                        /// 00034
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00044
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0004c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00050
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00054
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00058
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00060
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00064
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00068
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00070
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00074
			 0x0c780000,                                                  // Leading 1s:                                 /// 00078
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0007c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00080
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00084
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00090
			 0x0c600000,                                                  // Leading 1s:                                 /// 00094
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00098
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0009c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000c4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000dc
			 0x3f028f5c,                                                  // 0.51                                        /// 000e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000ec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 000f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00104
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0010c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00110
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00114
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00118
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00120
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00124
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00130
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x4b000000,                                                  // 8388608.0                                   /// 00140
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00148
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0015c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00160
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00164
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00168
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00170
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0017c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00180
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00184
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00188
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00194
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00198
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001a4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x3f028f5c,                                                  // 0.51                                        /// 001d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00200
			 0x3f028f5c,                                                  // 0.51                                        /// 00204
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0020c
			 0x4b000000,                                                  // 8388608.0                                   /// 00210
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00214
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00218
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0021c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00220
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00224
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00228
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0022c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00230
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0e000007,                                                  // Trailing 1s:                                /// 0023c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00248
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00254
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0025c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00260
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0026c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00274
			 0x3f028f5c,                                                  // 0.51                                        /// 00278
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0027c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00280
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00284
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00288
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0028c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00290
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00294
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0029c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002a8
			 0xffc00001,                                                  // -signaling NaN                              /// 002ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002b8
			 0x3f028f5c,                                                  // 0.51                                        /// 002bc
			 0x80000000,                                                  // -zero                                       /// 002c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0xff800000,                                                  // -inf                                        /// 002cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00304
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00308
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00310
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00314
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00318
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0031c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00320
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00324
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00328
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00330
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00334
			 0x00000000,                                                  // zero                                        /// 00338
			 0x7f800000,                                                  // inf                                         /// 0033c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00340
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00344
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00348
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00354
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00358
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00364
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0xffc00001,                                                  // -signaling NaN                              /// 0036c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e000001,                                                  // Trailing 1s:                                /// 0037c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00380
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00388
			 0x3f028f5c,                                                  // 0.51                                        /// 0038c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00394
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0xcb000000,                                                  // -8388608.0                                  /// 003a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003b8
			 0x00000000,                                                  // zero                                        /// 003bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003f0
			 0xcb000000,                                                  // -8388608.0                                  /// 003f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x80000000,                                                  // -zero                                       /// 00400
			 0x4b000000,                                                  // 8388608.0                                   /// 00404
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00408
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0040c
			 0x55555555,                                                  // 4 random values                             /// 00410
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00418
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00420
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00424
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0042c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00430
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00434
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0xcb000000,                                                  // -8388608.0                                  /// 0043c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00440
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00444
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00448
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0044c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00458
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00468
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0046c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00470
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00474
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00478
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0047c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00480
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00488
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00494
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0049c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00500
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00508
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0050c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00510
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00514
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00518
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00524
			 0x0c600000,                                                  // Leading 1s:                                 /// 00528
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0052c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00530
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00538
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0053c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00540
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00544
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0054c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00550
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00560
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00564
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0056c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00570
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00574
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00578
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0057c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00588
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0058c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00590
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00598
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0059c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005c8
			 0x7fc00001,                                                  // signaling NaN                               /// 005cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x3f028f5c,                                                  // 0.51                                        /// 005e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00600
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00608
			 0x7fc00001,                                                  // signaling NaN                               /// 0060c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00610
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00614
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0061c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00624
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0062c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00630
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00634
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00640
			 0x3f028f5c,                                                  // 0.51                                        /// 00644
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x7fc00001,                                                  // signaling NaN                               /// 0064c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00650
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0065c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00660
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0066c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00670
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00674
			 0x0c780000,                                                  // Leading 1s:                                 /// 00678
			 0xff800000,                                                  // -inf                                        /// 0067c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00680
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00688
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0068c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00694
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00698
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0069c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006ac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00700
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0070c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00710
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00718
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00720
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x00011111,                                                  // 9.7958E-41                                  /// 00728
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00730
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00734
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00738
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0073c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00740
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0074c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00750
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00754
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0xff800000,                                                  // -inf                                        /// 0075c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00764
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00768
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00770
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00774
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00778
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00780
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00790
			 0xffc00001,                                                  // -signaling NaN                              /// 00794
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007a4
			 0xffc00001,                                                  // -signaling NaN                              /// 007a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007c0
			 0xbf028f5c,                                                  // -0.51                                       /// 007c4
			 0xffc00001,                                                  // -signaling NaN                              /// 007c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00800
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00804
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0080c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00818
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0081c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00828
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0082c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00830
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0083c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00840
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00850
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00854
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00860
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00868
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0086c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00870
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00874
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00878
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00880
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00884
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00888
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0088c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00890
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0089c
			 0xff800000,                                                  // -inf                                        /// 008a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a8
			 0xcb000000,                                                  // -8388608.0                                  /// 008ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 008f0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008fc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00900
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00908
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00910
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0091c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00920
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00924
			 0x0c700000,                                                  // Leading 1s:                                 /// 00928
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0092c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00938
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00944
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00948
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0094c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00950
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00954
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00958
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0095c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00960
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00964
			 0x55555555,                                                  // 4 random values                             /// 00968
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0096c
			 0x4b000000,                                                  // 8388608.0                                   /// 00970
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00974
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00978
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0097c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00980
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00984
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00988
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0098c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x3f028f5c,                                                  // 0.51                                        /// 00994
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 009b0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009d0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009dc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009f4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a30
			 0x00000000,                                                  // zero                                        /// 00a34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a44
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a74
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a90
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a94
			 0x55555555,                                                  // 4 random values                             /// 00a98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ac4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ad8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00adc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ae0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ae4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x7f800000,                                                  // inf                                         /// 00af4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00afc
			 0x4b000000,                                                  // 8388608.0                                   /// 00b00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b0c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b10
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b54
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x4b000000,                                                  // 8388608.0                                   /// 00b78
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b90
			 0x3f028f5c,                                                  // 0.51                                        /// 00b94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x7fc00001,                                                  // signaling NaN                               /// 00b9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ba0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ba4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bb8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x33333333,                                                  // 4 random values                             /// 00be8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c38
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x7f800000,                                                  // inf                                         /// 00c4c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c6c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ca8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x7f800000,                                                  // inf                                         /// 00cb4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ccc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cdc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ce0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ce4
			 0x80000000,                                                  // -zero                                       /// 00ce8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x00000000,                                                  // zero                                        /// 00d0c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d20
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d50
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d54
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d58
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d5c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d68
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d70
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d88
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00da8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00dac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00db0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dbc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dcc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dd0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00de8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00df8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e24
			 0x80000000,                                                  // -zero                                       /// 00e28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e30
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e80
			 0xffc00001,                                                  // -signaling NaN                              /// 00e84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e98
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eb4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00eb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ebc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ecc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00edc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ee4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00eec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ef0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00efc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0xbf028f5c,                                                  // -0.51                                       /// 00f04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f10
			 0xbf028f5c,                                                  // -0.51                                       /// 00f14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x7fc00001,                                                  // signaling NaN                               /// 00f3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f44
			 0x7f800000,                                                  // inf                                         /// 00f48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f88
			 0x55555555,                                                  // 4 random values                             /// 00f8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00fa0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fa8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fcc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x33333333,                                                  // 4 random values                             /// 00fd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fdc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ff4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ff8
			 0x80040000                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00000
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00004
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00008
			 0x0c700000,                                                  // Leading 1s:                                 /// 0000c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00010
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00014
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0002c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x0c600000,                                                  // Leading 1s:                                 /// 00038
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00040
			 0x00000000,                                                  // zero                                        /// 00044
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0004c
			 0xff800000,                                                  // -inf                                        /// 00050
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00054
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0005c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00064
			 0x55555555,                                                  // 4 random values                             /// 00068
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0006c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00070
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00074
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00078
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0007c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00080
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00084
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0008c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00090
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00094
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00098
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0009c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0xcb000000,                                                  // -8388608.0                                  /// 000e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00104
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0c400000,                                                  // Leading 1s:                                 /// 0010c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00114
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c700000,                                                  // Leading 1s:                                 /// 0011c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00120
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00124
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00128
			 0x4b000000,                                                  // 8388608.0                                   /// 0012c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00134
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00138
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x3f028f5c,                                                  // 0.51                                        /// 00144
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00148
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0014c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0c400000,                                                  // Leading 1s:                                 /// 00154
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0015c
			 0x3f028f5c,                                                  // 0.51                                        /// 00160
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00164
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00168
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0016c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00170
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00174
			 0x7fc00001,                                                  // signaling NaN                               /// 00178
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0017c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00188
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x3f028f5c,                                                  // 0.51                                        /// 00194
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00198
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0019c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001a0
			 0x7f800000,                                                  // inf                                         /// 001a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c0
			 0xff800000,                                                  // -inf                                        /// 001c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001e4
			 0xbf028f5c,                                                  // -0.51                                       /// 001e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 001f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00200
			 0x00000000,                                                  // zero                                        /// 00204
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0020c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00210
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00214
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00218
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0021c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00220
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00224
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00230
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00234
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00238
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00240
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00248
			 0xcb000000,                                                  // -8388608.0                                  /// 0024c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00258
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0025c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00264
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0026c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00274
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00278
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00280
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00284
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00288
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0028c
			 0xcb000000,                                                  // -8388608.0                                  /// 00290
			 0x7fc00001,                                                  // signaling NaN                               /// 00294
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0029c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002d8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00300
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00304
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0xcb000000,                                                  // -8388608.0                                  /// 0030c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00310
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00314
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00318
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0031c
			 0x33333333,                                                  // 4 random values                             /// 00320
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00324
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x3f028f5c,                                                  // 0.51                                        /// 0032c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00330
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00334
			 0x00011111,                                                  // 9.7958E-41                                  /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0033c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00340
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00348
			 0x0e000007,                                                  // Trailing 1s:                                /// 0034c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00350
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00354
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00358
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0035c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00360
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00368
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00378
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0037c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00380
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00388
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0038c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00390
			 0x00000000,                                                  // zero                                        /// 00394
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00398
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0039c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c8
			 0xffc00001,                                                  // -signaling NaN                              /// 003cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00400
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00404
			 0x3f028f5c,                                                  // 0.51                                        /// 00408
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0040c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00414
			 0x0c780000,                                                  // Leading 1s:                                 /// 00418
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00420
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00424
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0042c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00434
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0043c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00440
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00444
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00448
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0044c
			 0x4b000000,                                                  // 8388608.0                                   /// 00450
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00454
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0045c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00470
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00474
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00478
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0047c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00480
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0048c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00490
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0049c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d4
			 0xbf028f5c,                                                  // -0.51                                       /// 004d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0xffc00001,                                                  // -signaling NaN                              /// 004fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00508
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0050c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00510
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00518
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x0c780000,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00530
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00538
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0053c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00540
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00544
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0054c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00550
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00554
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00558
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0055c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00560
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00564
			 0x0e000003,                                                  // Trailing 1s:                                /// 00568
			 0x0c400000,                                                  // Leading 1s:                                 /// 0056c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00578
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00580
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00584
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00588
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00590
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00594
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x33333333,                                                  // 4 random values                             /// 005b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 005b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005c0
			 0xffc00001,                                                  // -signaling NaN                              /// 005c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005dc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80000000,                                                  // -zero                                       /// 00600
			 0xffc00001,                                                  // -signaling NaN                              /// 00604
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0061c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00620
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00628
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x4b000000,                                                  // 8388608.0                                   /// 00634
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00638
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0063c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00640
			 0xbf028f5c,                                                  // -0.51                                       /// 00644
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0c700000,                                                  // Leading 1s:                                 /// 0064c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00650
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00660
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00664
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000000,                                                  // zero                                        /// 00670
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00674
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00678
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00694
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0c700000,                                                  // Leading 1s:                                 /// 0069c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xffc00001,                                                  // -signaling NaN                              /// 006d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00700
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00704
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00708
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0070c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00710
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00714
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00718
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x4b000000,                                                  // 8388608.0                                   /// 0072c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00730
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0073c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00740
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00744
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00748
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x3f028f5c,                                                  // 0.51                                        /// 00750
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00758
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00764
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00770
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0077c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00780
			 0x7f800000,                                                  // inf                                         /// 00784
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0078c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00794
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00798
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0079c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007a4
			 0x80000000,                                                  // -zero                                       /// 007a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00800
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00804
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00808
			 0x3f028f5c,                                                  // 0.51                                        /// 0080c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00810
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00818
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0081c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00824
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00830
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0083c
			 0x3f028f5c,                                                  // 0.51                                        /// 00840
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00844
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00848
			 0x0c780000,                                                  // Leading 1s:                                 /// 0084c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00850
			 0x0e000003,                                                  // Trailing 1s:                                /// 00854
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0085c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00860
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00868
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0086c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00870
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00874
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00878
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00884
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x0e000003,                                                  // Trailing 1s:                                /// 0088c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00890
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0089c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 008a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x80000000,                                                  // -zero                                       /// 00900
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00904
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00908
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0090c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00910
			 0x4b000000,                                                  // 8388608.0                                   /// 00914
			 0x00000000,                                                  // zero                                        /// 00918
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00920
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00924
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00928
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0092c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00938
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00940
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00944
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00948
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00950
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0095c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00964
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0096c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00974
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00978
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0097c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00980
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00984
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0098c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00998
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0099c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000000,                                                  // zero                                        /// 009bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009cc
			 0x3f028f5c,                                                  // 0.51                                        /// 009d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x55555555,                                                  // 4 random values                             /// 00a30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a4c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a58
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a6c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a8c
			 0x7f800000,                                                  // inf                                         /// 00a90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00aa4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aa8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ab8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ac4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ac8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00acc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00adc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ae4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00af8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b28
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x3f028f5c,                                                  // 0.51                                        /// 00b64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b78
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b98
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ba0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ba4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ba8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bd0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bd8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00be8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00bec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bf8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c08
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c18
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c20
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c44
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x33333333,                                                  // 4 random values                             /// 00ca0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ccc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cfc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x7f800000,                                                  // inf                                         /// 00d14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x55555555,                                                  // 4 random values                             /// 00d80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d8c
			 0x33333333,                                                  // 4 random values                             /// 00d90
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00db0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00db4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00db8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dc0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dc4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dc8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dd8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00de0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00df0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00df8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e08
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e50
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ea0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00eac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00eb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ebc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ec0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ec4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ed0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00edc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ee4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ee8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ef8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00efc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f08
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f14
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f28
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x7f800000,                                                  // inf                                         /// 00f30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f38
			 0x7fc00001,                                                  // signaling NaN                               /// 00f3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f40
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f44
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f64
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f90
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa4
			 0x80000000,                                                  // -zero                                       /// 00fa8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fcc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fd8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fe4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fe8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xffc00000 // DefaultNan                                    // SP - ve numbers                             /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0084db8a, /// 0x0
			 0xaaf9a9e9, /// 0x4
			 0x97964e9b, /// 0x8
			 0x38138a88, /// 0xc
			 0x977cf7ff, /// 0x10
			 0x330f86f7, /// 0x14
			 0x8966784c, /// 0x18
			 0x96e72c9b, /// 0x1c
			 0xca307855, /// 0x20
			 0x8ece3211, /// 0x24
			 0xf492fc60, /// 0x28
			 0x35d699c6, /// 0x2c
			 0x4cbf4537, /// 0x30
			 0x6f5ddf00, /// 0x34
			 0x6c3c3b3a, /// 0x38
			 0xd401701a, /// 0x3c
			 0x16130fbc, /// 0x40
			 0x00a282ea, /// 0x44
			 0x84019844, /// 0x48
			 0xc13ab37f, /// 0x4c
			 0x64cd468b, /// 0x50
			 0xaecb1211, /// 0x54
			 0xd48b2017, /// 0x58
			 0xdff419f3, /// 0x5c
			 0x1f2f0c7f, /// 0x60
			 0x24545c71, /// 0x64
			 0x600e36b5, /// 0x68
			 0x7b5d0956, /// 0x6c
			 0xe1f1dbef, /// 0x70
			 0x77ae742f, /// 0x74
			 0x85c0a6f5, /// 0x78
			 0xcd2c3290, /// 0x7c
			 0x974fb3ab, /// 0x80
			 0x1eddedc8, /// 0x84
			 0x67057e63, /// 0x88
			 0x774a6525, /// 0x8c
			 0x2683375e, /// 0x90
			 0x7238a0cc, /// 0x94
			 0x3bf23776, /// 0x98
			 0x57c882b0, /// 0x9c
			 0x722e3ea8, /// 0xa0
			 0xd467814c, /// 0xa4
			 0xed9aa2b3, /// 0xa8
			 0x442d46e7, /// 0xac
			 0x735ac55f, /// 0xb0
			 0xc2c326b0, /// 0xb4
			 0xc38ddee7, /// 0xb8
			 0xffc53998, /// 0xbc
			 0x32407faf, /// 0xc0
			 0x010e0824, /// 0xc4
			 0x25091597, /// 0xc8
			 0xb6cf3300, /// 0xcc
			 0x088f2d4a, /// 0xd0
			 0xec954fa1, /// 0xd4
			 0x859ade3c, /// 0xd8
			 0x2fbb373b, /// 0xdc
			 0x3b569800, /// 0xe0
			 0xd24051ee, /// 0xe4
			 0xacc8c096, /// 0xe8
			 0x7a48a1db, /// 0xec
			 0x9f17b377, /// 0xf0
			 0x6e30be22, /// 0xf4
			 0xd3648e47, /// 0xf8
			 0x75352a77, /// 0xfc
			 0xcf07f79c, /// 0x100
			 0xcf0b58e4, /// 0x104
			 0xfdf1a246, /// 0x108
			 0x845deadb, /// 0x10c
			 0xef33148b, /// 0x110
			 0x0ac69a53, /// 0x114
			 0xee17ae50, /// 0x118
			 0x85168e61, /// 0x11c
			 0xada5f32c, /// 0x120
			 0xc4ee992c, /// 0x124
			 0x68663ee7, /// 0x128
			 0x370aea0e, /// 0x12c
			 0x1c6ca53d, /// 0x130
			 0x1143cc47, /// 0x134
			 0xff158ec3, /// 0x138
			 0xac2b15de, /// 0x13c
			 0x66c66bc2, /// 0x140
			 0x56c0588d, /// 0x144
			 0x6feb0971, /// 0x148
			 0x0937ef5a, /// 0x14c
			 0xd04b605e, /// 0x150
			 0x64137b9f, /// 0x154
			 0x0ae36785, /// 0x158
			 0x7f26fbd5, /// 0x15c
			 0x1bb987f0, /// 0x160
			 0x9f7537dd, /// 0x164
			 0xd9fad79c, /// 0x168
			 0xce71555b, /// 0x16c
			 0xe56f6b2d, /// 0x170
			 0xfa05b3d1, /// 0x174
			 0xb5732670, /// 0x178
			 0x7d85bcdf, /// 0x17c
			 0x7cb26981, /// 0x180
			 0xe6c82ae1, /// 0x184
			 0x844cc323, /// 0x188
			 0xcd534f83, /// 0x18c
			 0xc9330e5b, /// 0x190
			 0x2dc6f083, /// 0x194
			 0x16e07157, /// 0x198
			 0xa5a7c823, /// 0x19c
			 0x1cffeca2, /// 0x1a0
			 0xe79423e6, /// 0x1a4
			 0x7e01cf9d, /// 0x1a8
			 0xa2a42ee6, /// 0x1ac
			 0x6e332634, /// 0x1b0
			 0x563011a8, /// 0x1b4
			 0x8a401c99, /// 0x1b8
			 0xfd1042f7, /// 0x1bc
			 0xf8769987, /// 0x1c0
			 0xfbb3e235, /// 0x1c4
			 0x96cebc16, /// 0x1c8
			 0xe1746637, /// 0x1cc
			 0xeb0ae836, /// 0x1d0
			 0x354301bb, /// 0x1d4
			 0x63c3a5de, /// 0x1d8
			 0x36fb01df, /// 0x1dc
			 0x6b513922, /// 0x1e0
			 0x4f0aeb12, /// 0x1e4
			 0x530c9e19, /// 0x1e8
			 0xe0bde27f, /// 0x1ec
			 0x2cc9a067, /// 0x1f0
			 0x3f9c0e44, /// 0x1f4
			 0x04b27ec1, /// 0x1f8
			 0xb2feaf71, /// 0x1fc
			 0xccda10d3, /// 0x200
			 0xcd099cd0, /// 0x204
			 0xa8a22102, /// 0x208
			 0x5330a211, /// 0x20c
			 0x6525eebf, /// 0x210
			 0x85f0163d, /// 0x214
			 0x7c0e2edd, /// 0x218
			 0x47ac37b1, /// 0x21c
			 0xbee21457, /// 0x220
			 0x3875ed93, /// 0x224
			 0xc0260bf1, /// 0x228
			 0x5d7f7952, /// 0x22c
			 0xb628916b, /// 0x230
			 0x526ebe75, /// 0x234
			 0x9dce41f7, /// 0x238
			 0x01d41bac, /// 0x23c
			 0x351e615c, /// 0x240
			 0x75ef24cb, /// 0x244
			 0x56cd3924, /// 0x248
			 0x3cc28afd, /// 0x24c
			 0xa5e2438d, /// 0x250
			 0x6120dafc, /// 0x254
			 0xb3913a1a, /// 0x258
			 0x5a7b4d68, /// 0x25c
			 0xfa8f0907, /// 0x260
			 0xaad7215f, /// 0x264
			 0xb6dbc32a, /// 0x268
			 0xceed8358, /// 0x26c
			 0x049237ad, /// 0x270
			 0x017ba173, /// 0x274
			 0x05eeb799, /// 0x278
			 0xad3ca7c6, /// 0x27c
			 0x06cd447d, /// 0x280
			 0x0d82a147, /// 0x284
			 0x5f849356, /// 0x288
			 0xdaa6f0ed, /// 0x28c
			 0x03d711c4, /// 0x290
			 0xddce6db2, /// 0x294
			 0xd9c99f14, /// 0x298
			 0xee5960e8, /// 0x29c
			 0x735bfcd0, /// 0x2a0
			 0xed70369d, /// 0x2a4
			 0x499b394a, /// 0x2a8
			 0x27338288, /// 0x2ac
			 0x0d1ae19a, /// 0x2b0
			 0x4d04dc1b, /// 0x2b4
			 0x8c66203a, /// 0x2b8
			 0x5a4bb110, /// 0x2bc
			 0xf858c328, /// 0x2c0
			 0x89a6724e, /// 0x2c4
			 0x8514a643, /// 0x2c8
			 0x56896738, /// 0x2cc
			 0x502b07a8, /// 0x2d0
			 0x9230ed13, /// 0x2d4
			 0x473f7c1f, /// 0x2d8
			 0xfcc07def, /// 0x2dc
			 0x6f91503f, /// 0x2e0
			 0xfa357019, /// 0x2e4
			 0xe258bd39, /// 0x2e8
			 0xa15c6956, /// 0x2ec
			 0xdf32d04f, /// 0x2f0
			 0x133770d8, /// 0x2f4
			 0x690a606c, /// 0x2f8
			 0x573337bf, /// 0x2fc
			 0xf823e8e4, /// 0x300
			 0xe7e09788, /// 0x304
			 0xd1260915, /// 0x308
			 0xa1067240, /// 0x30c
			 0xe4c5aadd, /// 0x310
			 0xb47a7eec, /// 0x314
			 0xf9b3b4a2, /// 0x318
			 0x43005f79, /// 0x31c
			 0xe70ecb7a, /// 0x320
			 0xe1cb8424, /// 0x324
			 0x554469a3, /// 0x328
			 0x25018b7c, /// 0x32c
			 0xa838335a, /// 0x330
			 0x9e250d9a, /// 0x334
			 0xbee326f5, /// 0x338
			 0x27ca8a7d, /// 0x33c
			 0xccda3fb2, /// 0x340
			 0x451a9f18, /// 0x344
			 0x1aa7fd7c, /// 0x348
			 0x43aab3de, /// 0x34c
			 0x07bc185e, /// 0x350
			 0x48b73b38, /// 0x354
			 0xfca9cd57, /// 0x358
			 0x76957cc5, /// 0x35c
			 0xa133af1c, /// 0x360
			 0x0d4fe770, /// 0x364
			 0x2f4a11ed, /// 0x368
			 0x8fb24299, /// 0x36c
			 0x6bfedbb4, /// 0x370
			 0x47a4368b, /// 0x374
			 0xc9e3748c, /// 0x378
			 0x56be7fad, /// 0x37c
			 0xbe74b38a, /// 0x380
			 0x691e0ccb, /// 0x384
			 0x0956f88b, /// 0x388
			 0xe7c6f40b, /// 0x38c
			 0x4e5dc42b, /// 0x390
			 0x95665a87, /// 0x394
			 0x98664919, /// 0x398
			 0xcdb1cb8e, /// 0x39c
			 0x6b2da07d, /// 0x3a0
			 0x2101d806, /// 0x3a4
			 0xb743e3bd, /// 0x3a8
			 0xa45d62f9, /// 0x3ac
			 0x228eb0c0, /// 0x3b0
			 0x7790c38e, /// 0x3b4
			 0x5a043115, /// 0x3b8
			 0x025fc504, /// 0x3bc
			 0x58fe0745, /// 0x3c0
			 0xc4f9a4c6, /// 0x3c4
			 0x3f9edd29, /// 0x3c8
			 0xffcef4d8, /// 0x3cc
			 0x251c87d0, /// 0x3d0
			 0x7ad36895, /// 0x3d4
			 0x75de2067, /// 0x3d8
			 0x27f9154e, /// 0x3dc
			 0x06c6cdb1, /// 0x3e0
			 0xa496ff2e, /// 0x3e4
			 0x040c5682, /// 0x3e8
			 0xdd99170a, /// 0x3ec
			 0x919c924b, /// 0x3f0
			 0x5669a6b0, /// 0x3f4
			 0x38e6c827, /// 0x3f8
			 0x31a18ed2, /// 0x3fc
			 0x5b859039, /// 0x400
			 0x74662e45, /// 0x404
			 0x9034cc94, /// 0x408
			 0x2a208946, /// 0x40c
			 0x557ca481, /// 0x410
			 0xf1a71b21, /// 0x414
			 0xab0eb29f, /// 0x418
			 0x2e7e8605, /// 0x41c
			 0x27272faf, /// 0x420
			 0xc8cb89a7, /// 0x424
			 0x4f1865c7, /// 0x428
			 0xd8d337de, /// 0x42c
			 0x7a781958, /// 0x430
			 0xc44311b6, /// 0x434
			 0xb049fbbd, /// 0x438
			 0x8bb3a310, /// 0x43c
			 0x2ecff7f4, /// 0x440
			 0xa8497e07, /// 0x444
			 0xb990544c, /// 0x448
			 0xed3e0ecd, /// 0x44c
			 0x98ed9f87, /// 0x450
			 0x1ce8b614, /// 0x454
			 0x86d009ef, /// 0x458
			 0xda888be5, /// 0x45c
			 0xebc037f1, /// 0x460
			 0xb0d10e07, /// 0x464
			 0xea4a3939, /// 0x468
			 0x748ece9c, /// 0x46c
			 0x17e638f5, /// 0x470
			 0x7613b528, /// 0x474
			 0x4d4e64be, /// 0x478
			 0xc675e067, /// 0x47c
			 0xf02c888a, /// 0x480
			 0xb0f3ef2e, /// 0x484
			 0xa2d1c9a5, /// 0x488
			 0x9a5be56e, /// 0x48c
			 0xf7e51121, /// 0x490
			 0xd884603b, /// 0x494
			 0x2bcad493, /// 0x498
			 0xd1d10210, /// 0x49c
			 0x5aaeeda5, /// 0x4a0
			 0x4f16faf9, /// 0x4a4
			 0xb68cf87c, /// 0x4a8
			 0x9da948b6, /// 0x4ac
			 0x129d89a9, /// 0x4b0
			 0x895fd2e5, /// 0x4b4
			 0x35a1bac6, /// 0x4b8
			 0x37c9b815, /// 0x4bc
			 0x5791fe9e, /// 0x4c0
			 0xb2e28ac1, /// 0x4c4
			 0xf2f70d92, /// 0x4c8
			 0x0480bf3e, /// 0x4cc
			 0x350fcc60, /// 0x4d0
			 0x57322cdc, /// 0x4d4
			 0x3716e489, /// 0x4d8
			 0x5aedf6ad, /// 0x4dc
			 0x82bc4523, /// 0x4e0
			 0x19a44b60, /// 0x4e4
			 0xa1210f80, /// 0x4e8
			 0x4e93b3f5, /// 0x4ec
			 0xd0ad2628, /// 0x4f0
			 0x306bca35, /// 0x4f4
			 0xf971c4ae, /// 0x4f8
			 0x84bdfc48, /// 0x4fc
			 0x08cf5afe, /// 0x500
			 0xb58ef13a, /// 0x504
			 0xb3d6722c, /// 0x508
			 0x319b1a3c, /// 0x50c
			 0xcbfb81dd, /// 0x510
			 0x9d9ae3c1, /// 0x514
			 0x0ccbeb9f, /// 0x518
			 0x63027667, /// 0x51c
			 0xa70d4d6f, /// 0x520
			 0x33de372f, /// 0x524
			 0xa25485dd, /// 0x528
			 0xb667bc42, /// 0x52c
			 0x335bbab3, /// 0x530
			 0xa2088155, /// 0x534
			 0x0ae9390a, /// 0x538
			 0xbc71549c, /// 0x53c
			 0x14b5ef12, /// 0x540
			 0xe18a8e3a, /// 0x544
			 0x9403174f, /// 0x548
			 0xe6fe63ac, /// 0x54c
			 0xe4a4e549, /// 0x550
			 0x17115fec, /// 0x554
			 0xf9706adc, /// 0x558
			 0x7ca06769, /// 0x55c
			 0xd380326b, /// 0x560
			 0x543ab287, /// 0x564
			 0x9beda6b3, /// 0x568
			 0x1dc514b0, /// 0x56c
			 0x8915d4d8, /// 0x570
			 0x956b7ce6, /// 0x574
			 0x596b105e, /// 0x578
			 0x0597f2c5, /// 0x57c
			 0x54fb8072, /// 0x580
			 0x284c3253, /// 0x584
			 0x4cc0b5d1, /// 0x588
			 0xc2be2a46, /// 0x58c
			 0x4a61f4a6, /// 0x590
			 0x0bc81b1f, /// 0x594
			 0x35a45c9f, /// 0x598
			 0xc468c2db, /// 0x59c
			 0xa46025f6, /// 0x5a0
			 0xdcfb3658, /// 0x5a4
			 0x3b077f07, /// 0x5a8
			 0xf90ce072, /// 0x5ac
			 0x8da51fd9, /// 0x5b0
			 0xf536e915, /// 0x5b4
			 0x2844d006, /// 0x5b8
			 0xf3b46b10, /// 0x5bc
			 0xfa807174, /// 0x5c0
			 0x1a59996d, /// 0x5c4
			 0xa39618c3, /// 0x5c8
			 0xd01a7a4c, /// 0x5cc
			 0xda325e98, /// 0x5d0
			 0x8e39d3ae, /// 0x5d4
			 0x35f7914d, /// 0x5d8
			 0xf9795a6b, /// 0x5dc
			 0xe859eac8, /// 0x5e0
			 0x9f37979d, /// 0x5e4
			 0xbcf8939f, /// 0x5e8
			 0x7d79fd85, /// 0x5ec
			 0x41c720ae, /// 0x5f0
			 0x6597d30b, /// 0x5f4
			 0xd5c42f24, /// 0x5f8
			 0x96a682bf, /// 0x5fc
			 0x5eaba9d9, /// 0x600
			 0xa37a9913, /// 0x604
			 0x193d68a0, /// 0x608
			 0x0768c089, /// 0x60c
			 0xe4dc3c85, /// 0x610
			 0x25e716a4, /// 0x614
			 0xaf579b5e, /// 0x618
			 0x0182446d, /// 0x61c
			 0xd16700ef, /// 0x620
			 0xd7e33a22, /// 0x624
			 0xd95643f7, /// 0x628
			 0x7250bd5d, /// 0x62c
			 0x7bc04651, /// 0x630
			 0x6ec73026, /// 0x634
			 0xf3b707b7, /// 0x638
			 0x9ee9fa58, /// 0x63c
			 0xe65a32c3, /// 0x640
			 0x3af14ffd, /// 0x644
			 0x69fce55c, /// 0x648
			 0x9e59f060, /// 0x64c
			 0x82cdb3aa, /// 0x650
			 0xd4632e05, /// 0x654
			 0xc7ab2468, /// 0x658
			 0x34d0017e, /// 0x65c
			 0x78fdcc8a, /// 0x660
			 0x8a8e713c, /// 0x664
			 0xa669a394, /// 0x668
			 0xb4f14591, /// 0x66c
			 0x32c15ea2, /// 0x670
			 0x3e13c5b3, /// 0x674
			 0x21d17576, /// 0x678
			 0x01ad6ce3, /// 0x67c
			 0x6d618ef0, /// 0x680
			 0x891cf1a3, /// 0x684
			 0x7effebf2, /// 0x688
			 0x25ab0f60, /// 0x68c
			 0xf646890e, /// 0x690
			 0x1ba80484, /// 0x694
			 0x99cef231, /// 0x698
			 0xbd7487fa, /// 0x69c
			 0xa7ce3917, /// 0x6a0
			 0xb9368c92, /// 0x6a4
			 0x5d977168, /// 0x6a8
			 0xe37d37cb, /// 0x6ac
			 0x0c4d29e8, /// 0x6b0
			 0x5ba5e271, /// 0x6b4
			 0x6ca98431, /// 0x6b8
			 0x882b8efa, /// 0x6bc
			 0x43b2cf4e, /// 0x6c0
			 0x78a0dfc7, /// 0x6c4
			 0x3105d28c, /// 0x6c8
			 0x7d881a9b, /// 0x6cc
			 0xd80b721f, /// 0x6d0
			 0x388d977f, /// 0x6d4
			 0x625ce8d9, /// 0x6d8
			 0xdd6e0613, /// 0x6dc
			 0xd4689171, /// 0x6e0
			 0x5c3aa20e, /// 0x6e4
			 0xd116df4f, /// 0x6e8
			 0x9e007e5e, /// 0x6ec
			 0x8f1c97cf, /// 0x6f0
			 0x2b702509, /// 0x6f4
			 0x8754219a, /// 0x6f8
			 0x785af872, /// 0x6fc
			 0x1ed905d0, /// 0x700
			 0x376269fb, /// 0x704
			 0xdf6e4824, /// 0x708
			 0x1dbbd0b5, /// 0x70c
			 0xcd41e537, /// 0x710
			 0x54b49a6b, /// 0x714
			 0x67862b30, /// 0x718
			 0x3b788e6d, /// 0x71c
			 0xa3807029, /// 0x720
			 0xaccbe95c, /// 0x724
			 0x5cb7c5f4, /// 0x728
			 0x0bead8fa, /// 0x72c
			 0x1ccd0979, /// 0x730
			 0xb3cf2a78, /// 0x734
			 0x25845ca5, /// 0x738
			 0x4b766b9a, /// 0x73c
			 0x6a54b902, /// 0x740
			 0x0626c2af, /// 0x744
			 0xecf75d6c, /// 0x748
			 0x139fe8bf, /// 0x74c
			 0x15d871bf, /// 0x750
			 0x6fbd55aa, /// 0x754
			 0xf7be9efb, /// 0x758
			 0xec4a01f9, /// 0x75c
			 0x017268c6, /// 0x760
			 0x31643f62, /// 0x764
			 0xf48e2bb6, /// 0x768
			 0x07848e4d, /// 0x76c
			 0x994e61ed, /// 0x770
			 0x846093e0, /// 0x774
			 0x2b51b1ad, /// 0x778
			 0x0e29f909, /// 0x77c
			 0x2206bc44, /// 0x780
			 0x0a956026, /// 0x784
			 0x6a23ea14, /// 0x788
			 0x09393e62, /// 0x78c
			 0xad17f366, /// 0x790
			 0x3c2bfbac, /// 0x794
			 0x5c1c282d, /// 0x798
			 0x7bab896e, /// 0x79c
			 0x5084d1d1, /// 0x7a0
			 0x6c7b7173, /// 0x7a4
			 0x74f4493f, /// 0x7a8
			 0x847a4187, /// 0x7ac
			 0xbf2a423f, /// 0x7b0
			 0xeec56bae, /// 0x7b4
			 0x575ca155, /// 0x7b8
			 0x263ab613, /// 0x7bc
			 0xb5350fc2, /// 0x7c0
			 0x461d67f6, /// 0x7c4
			 0x7dca977c, /// 0x7c8
			 0x746b4da7, /// 0x7cc
			 0x3c56e760, /// 0x7d0
			 0x81af119a, /// 0x7d4
			 0xd4077cc2, /// 0x7d8
			 0x9a475728, /// 0x7dc
			 0x38051a20, /// 0x7e0
			 0xb938a9e3, /// 0x7e4
			 0x714ee649, /// 0x7e8
			 0x703d43f3, /// 0x7ec
			 0xd878ad9f, /// 0x7f0
			 0x4f4e1a2b, /// 0x7f4
			 0x1af86f97, /// 0x7f8
			 0xce8169b8, /// 0x7fc
			 0x4c4d0c97, /// 0x800
			 0x2af9e17d, /// 0x804
			 0xb6f0222e, /// 0x808
			 0x782d7679, /// 0x80c
			 0x50e48126, /// 0x810
			 0xfc0502ef, /// 0x814
			 0x8b703393, /// 0x818
			 0x71571dc0, /// 0x81c
			 0x34755b6b, /// 0x820
			 0xf625ddb3, /// 0x824
			 0x7aa97e8a, /// 0x828
			 0xa9dfd9a6, /// 0x82c
			 0x8d425ebd, /// 0x830
			 0x3e98088c, /// 0x834
			 0x5fff51f7, /// 0x838
			 0x3c4854ae, /// 0x83c
			 0xc15fc150, /// 0x840
			 0x212e2dc0, /// 0x844
			 0xcea35395, /// 0x848
			 0x764d2160, /// 0x84c
			 0x8e2912e0, /// 0x850
			 0x43e010e5, /// 0x854
			 0x378964a4, /// 0x858
			 0xda97a480, /// 0x85c
			 0x9e4554a9, /// 0x860
			 0xc524787f, /// 0x864
			 0xf643056c, /// 0x868
			 0xa3662660, /// 0x86c
			 0x9869a415, /// 0x870
			 0x3cdba5fe, /// 0x874
			 0x769914aa, /// 0x878
			 0x04b8e333, /// 0x87c
			 0xeaf8c0b3, /// 0x880
			 0xf15b73c0, /// 0x884
			 0xaa150ba0, /// 0x888
			 0x91bcc8e8, /// 0x88c
			 0xb470c7c6, /// 0x890
			 0xdc94fab5, /// 0x894
			 0x902d6295, /// 0x898
			 0x18bca9bc, /// 0x89c
			 0xd0d10167, /// 0x8a0
			 0x7233ebed, /// 0x8a4
			 0x3b75d298, /// 0x8a8
			 0x552d3556, /// 0x8ac
			 0x3f094203, /// 0x8b0
			 0x9de415c9, /// 0x8b4
			 0x4769bf7d, /// 0x8b8
			 0xb21cfdb5, /// 0x8bc
			 0xc1245c7c, /// 0x8c0
			 0x9be8ff2b, /// 0x8c4
			 0x6c1eec7b, /// 0x8c8
			 0x95b1c5ed, /// 0x8cc
			 0x215f7bf2, /// 0x8d0
			 0x68033139, /// 0x8d4
			 0x5c9e79a4, /// 0x8d8
			 0xb9042185, /// 0x8dc
			 0x36df997e, /// 0x8e0
			 0x4716eaba, /// 0x8e4
			 0x03b8be05, /// 0x8e8
			 0x9f22797e, /// 0x8ec
			 0x0e21bea6, /// 0x8f0
			 0x673cde18, /// 0x8f4
			 0xe3cbab8b, /// 0x8f8
			 0x9a27d5c8, /// 0x8fc
			 0xfe82d53d, /// 0x900
			 0x90211487, /// 0x904
			 0xbee07783, /// 0x908
			 0x89f6614e, /// 0x90c
			 0x006ff931, /// 0x910
			 0xe359e40c, /// 0x914
			 0xf9265187, /// 0x918
			 0x19e64a01, /// 0x91c
			 0x87aa55ad, /// 0x920
			 0x0a605d00, /// 0x924
			 0xb0297194, /// 0x928
			 0xc8e58412, /// 0x92c
			 0x435b331a, /// 0x930
			 0x90867255, /// 0x934
			 0x5115a3cd, /// 0x938
			 0xa635aa65, /// 0x93c
			 0xebad367f, /// 0x940
			 0x01616709, /// 0x944
			 0x73d42771, /// 0x948
			 0x1d14cbc9, /// 0x94c
			 0x4d067c06, /// 0x950
			 0xbeec39ba, /// 0x954
			 0x34a271eb, /// 0x958
			 0xf2fbb4a6, /// 0x95c
			 0x09fac6f3, /// 0x960
			 0xb0547d9e, /// 0x964
			 0x3cc2ba8a, /// 0x968
			 0xc3377282, /// 0x96c
			 0xbc612ed7, /// 0x970
			 0xc284578f, /// 0x974
			 0x09057103, /// 0x978
			 0xb9369eb5, /// 0x97c
			 0x499282d3, /// 0x980
			 0x1434d3e8, /// 0x984
			 0xb200aa26, /// 0x988
			 0xaacc2d1b, /// 0x98c
			 0x9e15d7b4, /// 0x990
			 0x122778bb, /// 0x994
			 0x09fe5dca, /// 0x998
			 0xa1c21c7a, /// 0x99c
			 0x8551b99d, /// 0x9a0
			 0xfff5dc58, /// 0x9a4
			 0x46df7bd4, /// 0x9a8
			 0x91b7be2e, /// 0x9ac
			 0x15621643, /// 0x9b0
			 0x332971ff, /// 0x9b4
			 0x860e1eff, /// 0x9b8
			 0x223756a3, /// 0x9bc
			 0x2eb0b759, /// 0x9c0
			 0x66906dea, /// 0x9c4
			 0x12407c33, /// 0x9c8
			 0x7b4a1797, /// 0x9cc
			 0x333325af, /// 0x9d0
			 0x313e6c4e, /// 0x9d4
			 0xc354ca5a, /// 0x9d8
			 0x549a9204, /// 0x9dc
			 0xcab6b28a, /// 0x9e0
			 0x4dc67d3d, /// 0x9e4
			 0x8fbdeeb9, /// 0x9e8
			 0x31cb32d2, /// 0x9ec
			 0x60006a99, /// 0x9f0
			 0x7cb34fca, /// 0x9f4
			 0x0fa0d726, /// 0x9f8
			 0xd51ab2ee, /// 0x9fc
			 0xfed61d4b, /// 0xa00
			 0x3f059599, /// 0xa04
			 0x816e0945, /// 0xa08
			 0xb75faf3d, /// 0xa0c
			 0xa0867d5a, /// 0xa10
			 0x3f47cacc, /// 0xa14
			 0x6a10ecc8, /// 0xa18
			 0x89eea98a, /// 0xa1c
			 0xb1d26936, /// 0xa20
			 0xd792a9a4, /// 0xa24
			 0x094b1bd5, /// 0xa28
			 0x6c1a6a82, /// 0xa2c
			 0xcfda56b2, /// 0xa30
			 0x6712d7f2, /// 0xa34
			 0x0f3c4d25, /// 0xa38
			 0x7f995959, /// 0xa3c
			 0x28e9d642, /// 0xa40
			 0x76b83871, /// 0xa44
			 0xb461b60d, /// 0xa48
			 0xe48f5d74, /// 0xa4c
			 0x699ecb4b, /// 0xa50
			 0x41e60ff9, /// 0xa54
			 0xf8a122c0, /// 0xa58
			 0x454e8204, /// 0xa5c
			 0x9cb54a0b, /// 0xa60
			 0xe95c2c86, /// 0xa64
			 0x3b9317d3, /// 0xa68
			 0xcab8f40d, /// 0xa6c
			 0x751b9d7c, /// 0xa70
			 0xca35359c, /// 0xa74
			 0x43bb2db3, /// 0xa78
			 0x66540397, /// 0xa7c
			 0x171246b9, /// 0xa80
			 0xac626668, /// 0xa84
			 0x57fba023, /// 0xa88
			 0x8eb31090, /// 0xa8c
			 0x09a1d980, /// 0xa90
			 0x21133d13, /// 0xa94
			 0x8dea7661, /// 0xa98
			 0x80c1d48a, /// 0xa9c
			 0xe2f267fe, /// 0xaa0
			 0x766f8192, /// 0xaa4
			 0xad538c2d, /// 0xaa8
			 0x123c216a, /// 0xaac
			 0x67c1e591, /// 0xab0
			 0x05bd5a2d, /// 0xab4
			 0x5d49c229, /// 0xab8
			 0x53212cd2, /// 0xabc
			 0xc252f609, /// 0xac0
			 0x39ad5f38, /// 0xac4
			 0x28c6157a, /// 0xac8
			 0x8f5de9e0, /// 0xacc
			 0x68c12682, /// 0xad0
			 0x2c93b9c8, /// 0xad4
			 0x1182f705, /// 0xad8
			 0x7cf75b36, /// 0xadc
			 0xd860ee00, /// 0xae0
			 0x287d86d7, /// 0xae4
			 0xa88bd090, /// 0xae8
			 0x7332d93e, /// 0xaec
			 0x65c38559, /// 0xaf0
			 0x085c0fdc, /// 0xaf4
			 0x8d5d32c3, /// 0xaf8
			 0x4d66e5d4, /// 0xafc
			 0x490b47bd, /// 0xb00
			 0x9ce4e1a4, /// 0xb04
			 0xe0e929a1, /// 0xb08
			 0xae5caf74, /// 0xb0c
			 0x80fc72c7, /// 0xb10
			 0x1f5b4ecc, /// 0xb14
			 0x180e3404, /// 0xb18
			 0xae067edd, /// 0xb1c
			 0x1e2e1df3, /// 0xb20
			 0xa7a2620b, /// 0xb24
			 0xbe6550c3, /// 0xb28
			 0x9c7cc388, /// 0xb2c
			 0x560aeff2, /// 0xb30
			 0x48edd632, /// 0xb34
			 0x477a104c, /// 0xb38
			 0x4f8911ac, /// 0xb3c
			 0xf280b536, /// 0xb40
			 0xc74dc4b9, /// 0xb44
			 0x729a2181, /// 0xb48
			 0x47831f6b, /// 0xb4c
			 0x8fe1bda9, /// 0xb50
			 0x035c92fb, /// 0xb54
			 0x069f4946, /// 0xb58
			 0x3efdb8b7, /// 0xb5c
			 0x0cced707, /// 0xb60
			 0x91e7551c, /// 0xb64
			 0xb22d6a34, /// 0xb68
			 0xf534b639, /// 0xb6c
			 0xc93ff469, /// 0xb70
			 0x9a2e0d43, /// 0xb74
			 0x62fbc880, /// 0xb78
			 0x1c7f9a7e, /// 0xb7c
			 0x2ce9a64f, /// 0xb80
			 0xe6294113, /// 0xb84
			 0x7852e0d3, /// 0xb88
			 0x1204d1ea, /// 0xb8c
			 0x825b3c10, /// 0xb90
			 0x3cdaab0c, /// 0xb94
			 0x7db60946, /// 0xb98
			 0xe75e74f5, /// 0xb9c
			 0xf52df847, /// 0xba0
			 0xeebc49c2, /// 0xba4
			 0x6e717756, /// 0xba8
			 0xb2d41840, /// 0xbac
			 0xcc778c4d, /// 0xbb0
			 0x84003f6d, /// 0xbb4
			 0x37972983, /// 0xbb8
			 0x0798093b, /// 0xbbc
			 0x8f348855, /// 0xbc0
			 0x04ce215e, /// 0xbc4
			 0x65203c49, /// 0xbc8
			 0x27ce76f5, /// 0xbcc
			 0xdcbb4e50, /// 0xbd0
			 0x84894db7, /// 0xbd4
			 0x176adaba, /// 0xbd8
			 0x5ac65963, /// 0xbdc
			 0x4e61f5fd, /// 0xbe0
			 0x2e217059, /// 0xbe4
			 0x62e38199, /// 0xbe8
			 0xcec479d1, /// 0xbec
			 0x79a75e0a, /// 0xbf0
			 0xeb204967, /// 0xbf4
			 0x1f6dc9ce, /// 0xbf8
			 0xaa7383da, /// 0xbfc
			 0x4a618fe3, /// 0xc00
			 0x8a634b08, /// 0xc04
			 0x5fa873ab, /// 0xc08
			 0xce7336d3, /// 0xc0c
			 0x0533ef75, /// 0xc10
			 0x51aa9ad1, /// 0xc14
			 0x8eeb91c5, /// 0xc18
			 0xaa576cf8, /// 0xc1c
			 0x5c653d05, /// 0xc20
			 0xe8607e58, /// 0xc24
			 0x46204957, /// 0xc28
			 0xc7a25c53, /// 0xc2c
			 0xcec6aeee, /// 0xc30
			 0xe724aa3c, /// 0xc34
			 0xe9b99788, /// 0xc38
			 0x69296a0a, /// 0xc3c
			 0x42fe2aa6, /// 0xc40
			 0xc89e2313, /// 0xc44
			 0xd4c22e70, /// 0xc48
			 0x0e4004e1, /// 0xc4c
			 0x0e90767d, /// 0xc50
			 0xaa87a177, /// 0xc54
			 0x95db8e9b, /// 0xc58
			 0xc92191dd, /// 0xc5c
			 0xd30ae44a, /// 0xc60
			 0xc2602fbd, /// 0xc64
			 0x04e5253f, /// 0xc68
			 0x93bcf416, /// 0xc6c
			 0x23225ff1, /// 0xc70
			 0xe4c1bfdd, /// 0xc74
			 0xdc836a41, /// 0xc78
			 0xf080e053, /// 0xc7c
			 0x0cb5aa77, /// 0xc80
			 0xe92d0ba2, /// 0xc84
			 0x1d50a69e, /// 0xc88
			 0x48373bbe, /// 0xc8c
			 0xa4dc5dcf, /// 0xc90
			 0xed7254b2, /// 0xc94
			 0x5e511655, /// 0xc98
			 0x0902b1b0, /// 0xc9c
			 0xd558e4e8, /// 0xca0
			 0x4cec39bb, /// 0xca4
			 0x5f7a27af, /// 0xca8
			 0xd30c2414, /// 0xcac
			 0x27a41523, /// 0xcb0
			 0xd158d276, /// 0xcb4
			 0x3cbb922b, /// 0xcb8
			 0x58e99931, /// 0xcbc
			 0x54ed1d94, /// 0xcc0
			 0x005ff6f8, /// 0xcc4
			 0xbdf284aa, /// 0xcc8
			 0xe847f869, /// 0xccc
			 0x1e596390, /// 0xcd0
			 0x94079904, /// 0xcd4
			 0xd1e2cf2b, /// 0xcd8
			 0xd987ecc5, /// 0xcdc
			 0x37f8eb1d, /// 0xce0
			 0x887aa35b, /// 0xce4
			 0x1d2084ce, /// 0xce8
			 0xf9fb5c45, /// 0xcec
			 0x4fe265b4, /// 0xcf0
			 0xd8c4640b, /// 0xcf4
			 0x7eb4ab1a, /// 0xcf8
			 0x28f95007, /// 0xcfc
			 0x742bce41, /// 0xd00
			 0xdaa3af51, /// 0xd04
			 0x4d27508b, /// 0xd08
			 0xc85403a0, /// 0xd0c
			 0x0f43c146, /// 0xd10
			 0x7b0d807a, /// 0xd14
			 0xd44149b6, /// 0xd18
			 0x5003b4ba, /// 0xd1c
			 0x096c78ff, /// 0xd20
			 0xae66a2cc, /// 0xd24
			 0x8815c5fe, /// 0xd28
			 0x954f8dbd, /// 0xd2c
			 0x24c22885, /// 0xd30
			 0x5ae99597, /// 0xd34
			 0x935967fd, /// 0xd38
			 0x578aa6d4, /// 0xd3c
			 0xdb2be3c3, /// 0xd40
			 0xb7aac062, /// 0xd44
			 0xe8069896, /// 0xd48
			 0xfae4b103, /// 0xd4c
			 0xbcdf5e49, /// 0xd50
			 0x787e35f6, /// 0xd54
			 0xa1767191, /// 0xd58
			 0x19afa50f, /// 0xd5c
			 0xe75e0c20, /// 0xd60
			 0x59ec7cb1, /// 0xd64
			 0xd7e74bba, /// 0xd68
			 0x41cd5d71, /// 0xd6c
			 0x81f444c3, /// 0xd70
			 0x674745ef, /// 0xd74
			 0xe4851d94, /// 0xd78
			 0xa0af3c01, /// 0xd7c
			 0xb65d8f44, /// 0xd80
			 0x2f81c61a, /// 0xd84
			 0x4236afe6, /// 0xd88
			 0xae9c136d, /// 0xd8c
			 0x809b0316, /// 0xd90
			 0x05ee5e7d, /// 0xd94
			 0x7a500f1b, /// 0xd98
			 0xf2419ca0, /// 0xd9c
			 0x8bdbb90f, /// 0xda0
			 0xac893e52, /// 0xda4
			 0x0d3530c5, /// 0xda8
			 0xe81fbcf8, /// 0xdac
			 0x4863ec32, /// 0xdb0
			 0xe5963858, /// 0xdb4
			 0xc0127705, /// 0xdb8
			 0xfdad09a2, /// 0xdbc
			 0x54ac2f4c, /// 0xdc0
			 0x7b54436a, /// 0xdc4
			 0xd7cd3582, /// 0xdc8
			 0xf6f48ce4, /// 0xdcc
			 0xfc0fef9a, /// 0xdd0
			 0x1829f6eb, /// 0xdd4
			 0x0c234a47, /// 0xdd8
			 0x9d3969f6, /// 0xddc
			 0x3298c322, /// 0xde0
			 0xb345449e, /// 0xde4
			 0xba632cce, /// 0xde8
			 0x68514f8f, /// 0xdec
			 0xedcfd829, /// 0xdf0
			 0x6a9db5b0, /// 0xdf4
			 0xb58a2c9d, /// 0xdf8
			 0xb72f5e25, /// 0xdfc
			 0xdeed6b59, /// 0xe00
			 0xfb9c2e90, /// 0xe04
			 0x4c70dfca, /// 0xe08
			 0x376a8e8a, /// 0xe0c
			 0x1552a1de, /// 0xe10
			 0x063ad262, /// 0xe14
			 0x4a99db28, /// 0xe18
			 0x933eac13, /// 0xe1c
			 0x39a4720d, /// 0xe20
			 0x669ca36d, /// 0xe24
			 0x6feceb03, /// 0xe28
			 0xad52e4f6, /// 0xe2c
			 0xd776c3e3, /// 0xe30
			 0x851d5622, /// 0xe34
			 0x3f7b5868, /// 0xe38
			 0x7412f5d9, /// 0xe3c
			 0x68c8e212, /// 0xe40
			 0x091ac2a0, /// 0xe44
			 0x6867d068, /// 0xe48
			 0x0649c58c, /// 0xe4c
			 0x11cc9607, /// 0xe50
			 0xa0194a0b, /// 0xe54
			 0x5593a593, /// 0xe58
			 0xb422c98d, /// 0xe5c
			 0x1c3a3e05, /// 0xe60
			 0x3301045f, /// 0xe64
			 0xa6fc35ba, /// 0xe68
			 0x1d17d66d, /// 0xe6c
			 0xf634fba6, /// 0xe70
			 0xe042dec8, /// 0xe74
			 0x530857f6, /// 0xe78
			 0x4bde7a78, /// 0xe7c
			 0x2bdcf80a, /// 0xe80
			 0x3d7091d5, /// 0xe84
			 0xd647874c, /// 0xe88
			 0x98606148, /// 0xe8c
			 0xc7534e37, /// 0xe90
			 0x000afcb3, /// 0xe94
			 0xefe0501b, /// 0xe98
			 0x2eb49876, /// 0xe9c
			 0x87d3a6b3, /// 0xea0
			 0x153f8185, /// 0xea4
			 0x1857cf32, /// 0xea8
			 0x1302b18a, /// 0xeac
			 0xba8bd12e, /// 0xeb0
			 0x41d745aa, /// 0xeb4
			 0x428dbcd8, /// 0xeb8
			 0x5b9e6bbb, /// 0xebc
			 0x8a046068, /// 0xec0
			 0x7ce2c38d, /// 0xec4
			 0xaa004711, /// 0xec8
			 0xf2e01dc4, /// 0xecc
			 0xa2714004, /// 0xed0
			 0x59510856, /// 0xed4
			 0x86bfdf3e, /// 0xed8
			 0x647bf9fa, /// 0xedc
			 0x3be61a88, /// 0xee0
			 0xe51c6b49, /// 0xee4
			 0x90a26a4f, /// 0xee8
			 0x9acb8921, /// 0xeec
			 0xa016ab6d, /// 0xef0
			 0xa796bbe5, /// 0xef4
			 0x7a38714c, /// 0xef8
			 0x37bcc514, /// 0xefc
			 0xad415837, /// 0xf00
			 0x636f450f, /// 0xf04
			 0x41207f23, /// 0xf08
			 0xe7b84498, /// 0xf0c
			 0xe6d79f6f, /// 0xf10
			 0xaf3997da, /// 0xf14
			 0x54504183, /// 0xf18
			 0x6d67a0f9, /// 0xf1c
			 0xa92ed510, /// 0xf20
			 0xcdc7f979, /// 0xf24
			 0x61aa7103, /// 0xf28
			 0xc0fa68b1, /// 0xf2c
			 0x7b9ff8e8, /// 0xf30
			 0x03f1a8bd, /// 0xf34
			 0xc54667cb, /// 0xf38
			 0xd7a2303e, /// 0xf3c
			 0x264808da, /// 0xf40
			 0x27fea874, /// 0xf44
			 0xf732af93, /// 0xf48
			 0x139565e4, /// 0xf4c
			 0xe2202bc0, /// 0xf50
			 0x8296d95c, /// 0xf54
			 0xec7c7234, /// 0xf58
			 0xa553bf6d, /// 0xf5c
			 0x3e9825f2, /// 0xf60
			 0x7e528d87, /// 0xf64
			 0x398d12cd, /// 0xf68
			 0x43c67045, /// 0xf6c
			 0x912996cf, /// 0xf70
			 0x0185992b, /// 0xf74
			 0x117f0fb8, /// 0xf78
			 0xf56f08f3, /// 0xf7c
			 0xcf66f452, /// 0xf80
			 0x45ea1847, /// 0xf84
			 0xfc9b8277, /// 0xf88
			 0x0749dbe2, /// 0xf8c
			 0x52307852, /// 0xf90
			 0x8d475e6d, /// 0xf94
			 0xc710ba0c, /// 0xf98
			 0xc3c6a213, /// 0xf9c
			 0xe713ada0, /// 0xfa0
			 0x88d2f00b, /// 0xfa4
			 0x61a503b6, /// 0xfa8
			 0x3420e96f, /// 0xfac
			 0xf2ab5daa, /// 0xfb0
			 0x30f7eeaf, /// 0xfb4
			 0xdb90b6df, /// 0xfb8
			 0x48f4a3ab, /// 0xfbc
			 0x1d34a45f, /// 0xfc0
			 0x40200fc9, /// 0xfc4
			 0x748276d4, /// 0xfc8
			 0x23db0aa4, /// 0xfcc
			 0x29fedfbe, /// 0xfd0
			 0xca660e30, /// 0xfd4
			 0x16bd77a1, /// 0xfd8
			 0xbe46c4ac, /// 0xfdc
			 0x44cc1c44, /// 0xfe0
			 0x9346dea5, /// 0xfe4
			 0x7db5fb57, /// 0xfe8
			 0x22892ae9, /// 0xfec
			 0xdcea30d0, /// 0xff0
			 0x269bd25a, /// 0xff4
			 0x24f1073a, /// 0xff8
			 0x917c8550 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00000
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00004
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00008
			 0x00000000,                                                  // zero                                        /// 0000c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00010
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00014
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0001c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00020
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00024
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00028
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0002c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00030
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00034
			 0xffc00001,                                                  // -signaling NaN                              /// 00038
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0003c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00040
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00048
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0004c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00054
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00058
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0005c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00060
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00064
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00068
			 0x0c780000,                                                  // Leading 1s:                                 /// 0006c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00070
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x55555555,                                                  // 4 random values                             /// 00078
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0007c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00084
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x7f800000,                                                  // inf                                         /// 0008c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00094
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00098
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0009c
			 0xff800000,                                                  // -inf                                        /// 000a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 000b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00000000,                                                  // zero                                        /// 000dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x33333333,                                                  // 4 random values                             /// 000ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00100
			 0x0c600000,                                                  // Leading 1s:                                 /// 00104
			 0x00000000,                                                  // zero                                        /// 00108
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0010c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00110
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00118
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0011c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00128
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0012c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00130
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00134
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00138
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0013c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00144
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0014c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00168
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00174
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00178
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0018c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00194
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00198
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0xcb000000,                                                  // -8388608.0                                  /// 001b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x33333333,                                                  // 4 random values                             /// 001dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00200
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00204
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00208
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0020c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0xcb000000,                                                  // -8388608.0                                  /// 00214
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00218
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0021c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00224
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00228
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0022c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00230
			 0x7fc00001,                                                  // signaling NaN                               /// 00234
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00238
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00240
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00248
			 0x7fc00001,                                                  // signaling NaN                               /// 0024c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00250
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00254
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00258
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00264
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00268
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0026c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00278
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00280
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00284
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00288
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00298
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x3f028f5c,                                                  // 0.51                                        /// 002a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002cc
			 0xff800000,                                                  // -inf                                        /// 002d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00300
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00304
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00308
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0030c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00314
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0031c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00000000,                                                  // zero                                        /// 00328
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0032c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00330
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x3f028f5c,                                                  // 0.51                                        /// 0033c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00340
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00344
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00348
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00350
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00354
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00358
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x3f028f5c,                                                  // 0.51                                        /// 00360
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00364
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00368
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0xffc00001,                                                  // -signaling NaN                              /// 00378
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0037c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00380
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00384
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00388
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00390
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00394
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00398
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003a4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003b0
			 0xcb000000,                                                  // -8388608.0                                  /// 003b4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00404
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00408
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00414
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00418
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0041c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00424
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00430
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00434
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00438
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0043c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0044c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00454
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00464
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00468
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00470
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00474
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00478
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00484
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0048c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00490
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00494
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00498
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0049c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004a8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00504
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0050c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00510
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00514
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00518
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0051c
			 0xcb000000,                                                  // -8388608.0                                  /// 00520
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00524
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00530
			 0x0e000001,                                                  // Trailing 1s:                                /// 00534
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00538
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0053c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00540
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00544
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00548
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0054c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00550
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x3f028f5c,                                                  // 0.51                                        /// 00558
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0055c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00560
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00564
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00568
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0056c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00570
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00574
			 0x0c700000,                                                  // Leading 1s:                                 /// 00578
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0057c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00580
			 0x0c780000,                                                  // Leading 1s:                                 /// 00584
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0c700000,                                                  // Leading 1s:                                 /// 0058c
			 0x7f800000,                                                  // inf                                         /// 00590
			 0x7fc00001,                                                  // signaling NaN                               /// 00594
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00598
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0059c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 005dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00604
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0060c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00610
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00614
			 0xbf028f5c,                                                  // -0.51                                       /// 00618
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00620
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0062c
			 0xbf028f5c,                                                  // -0.51                                       /// 00630
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00634
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0063c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00640
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00650
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x0c700000,                                                  // Leading 1s:                                 /// 00658
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0065c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00660
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00664
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0066c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00674
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00678
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00680
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00684
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0068c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00690
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00694
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00698
			 0x55555555,                                                  // 4 random values                             /// 0069c
			 0xff800000,                                                  // -inf                                        /// 006a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006a8
			 0x7fc00001,                                                  // signaling NaN                               /// 006ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00700
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00704
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00708
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00714
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00718
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0071c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00720
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00724
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00728
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00734
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00738
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0073c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00740
			 0x00011111,                                                  // 9.7958E-41                                  /// 00744
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0xbf028f5c,                                                  // -0.51                                       /// 0074c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00754
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00758
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00768
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0076c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00770
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0077c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00784
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00788
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0078c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0c400000,                                                  // Leading 1s:                                 /// 00794
			 0x80011111,                                                  // -9.7958E-41                                 /// 00798
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0079c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007a8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d8
			 0x7fc00001,                                                  // signaling NaN                               /// 007dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00800
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00808
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0080c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00810
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00818
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0081c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00820
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00824
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00828
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x3f028f5c,                                                  // 0.51                                        /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0083c
			 0x80000000,                                                  // -zero                                       /// 00840
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00844
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00848
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0084c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00850
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00854
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00858
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00860
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00864
			 0x0e000003,                                                  // Trailing 1s:                                /// 00868
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0086c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00870
			 0x80000000,                                                  // -zero                                       /// 00874
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0087c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00880
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00888
			 0xbf028f5c,                                                  // -0.51                                       /// 0088c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00890
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0xbf028f5c,                                                  // -0.51                                       /// 008b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c780000,                                                  // Leading 1s:                                 /// 008c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 008f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00900
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00904
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00908
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0090c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00910
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00914
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00918
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0091c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00924
			 0x7fc00001,                                                  // signaling NaN                               /// 00928
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0xffc00001,                                                  // -signaling NaN                              /// 00934
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00938
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00944
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00948
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0094c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0e000007,                                                  // Trailing 1s:                                /// 00958
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00960
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00964
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00968
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0096c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00970
			 0xbf028f5c,                                                  // -0.51                                       /// 00974
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0097c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00980
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00984
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00988
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00990
			 0xcb000000,                                                  // -8388608.0                                  /// 00994
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x7fc00001,                                                  // signaling NaN                               /// 0099c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c400000,                                                  // Leading 1s:                                 /// 009a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 009bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d0
			 0xcb000000,                                                  // -8388608.0                                  /// 009d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a08
			 0x33333333,                                                  // 4 random values                             /// 00a0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a1c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a7c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aa0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00aa4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ab8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00abc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ac4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ac8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00acc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ad0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ad4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00adc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ae4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00af4
			 0xff800000,                                                  // -inf                                        /// 00af8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00afc
			 0x7fc00001,                                                  // signaling NaN                               /// 00b00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b2c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b44
			 0x33333333,                                                  // 4 random values                             /// 00b48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b50
			 0x7fc00001,                                                  // signaling NaN                               /// 00b54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b58
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b90
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b94
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bac
			 0x3f028f5c,                                                  // 0.51                                        /// 00bb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bb4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00be0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bf0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bf4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bfc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c08
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c0c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c18
			 0x3f028f5c,                                                  // 0.51                                        /// 00c1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c28
			 0x3f028f5c,                                                  // 0.51                                        /// 00c2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c5c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c60
			 0x3f028f5c,                                                  // 0.51                                        /// 00c64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c68
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c70
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c8c
			 0x55555555,                                                  // 4 random values                             /// 00c90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c9c
			 0x3f028f5c,                                                  // 0.51                                        /// 00ca0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cb8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cd8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ce0
			 0x80000000,                                                  // -zero                                       /// 00ce4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cf0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cf4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cf8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cfc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d10
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d18
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d58
			 0xff800000,                                                  // -inf                                        /// 00d5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x7fc00001,                                                  // signaling NaN                               /// 00d68
			 0x3f028f5c,                                                  // 0.51                                        /// 00d6c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x3f028f5c,                                                  // 0.51                                        /// 00d74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0xbf028f5c,                                                  // -0.51                                       /// 00d98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00da4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00da8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00db0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ddc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00de0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00de4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e04
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x80000000,                                                  // -zero                                       /// 00e18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e48
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e50
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0xff800000,                                                  // -inf                                        /// 00e5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e64
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e80
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x55555555,                                                  // 4 random values                             /// 00e88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ea4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ea8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00eb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ebc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ecc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00edc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ee0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ee4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00eec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f08
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f10
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f20
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f30
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f38
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f50
			 0x80000000,                                                  // -zero                                       /// 00f54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f80
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fd8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x55555555,                                                  // 4 random values                             /// 00fe0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fe8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ff0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ff4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ff8
			 0x00000001 // min subnorm                                   // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00000
			 0xffc00001,                                                  // -signaling NaN                              /// 00004
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00008
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0000c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00014
			 0x7fc00001,                                                  // signaling NaN                               /// 00018
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00020
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00024
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x7fc00001,                                                  // signaling NaN                               /// 0002c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00030
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00034
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0003c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00044
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0004c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00050
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00054
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0005c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00060
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x0e000007,                                                  // Trailing 1s:                                /// 00068
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00074
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0007c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00090
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00094
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00098
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a0
			 0x00000000,                                                  // zero                                        /// 000a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x7fc00001,                                                  // signaling NaN                               /// 000ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0xff800000,                                                  // -inf                                        /// 000d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00100
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00108
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00114
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0011c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00120
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00124
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x7f800000,                                                  // inf                                         /// 0012c
			 0x7fc00001,                                                  // signaling NaN                               /// 00130
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00138
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0013c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00144
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0014c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00150
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0015c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00160
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00170
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00174
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00178
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00180
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00188
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0018c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00190
			 0x33333333,                                                  // 4 random values                             /// 00194
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001a4
			 0x3f028f5c,                                                  // 0.51                                        /// 001a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e0
			 0x4b000000,                                                  // 8388608.0                                   /// 001e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00200
			 0x0c700000,                                                  // Leading 1s:                                 /// 00204
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00208
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0020c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00218
			 0x55555555,                                                  // 4 random values                             /// 0021c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0022c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00234
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0023c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00244
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00248
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0024c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00250
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0025c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00264
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00268
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0026c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00270
			 0x33333333,                                                  // 4 random values                             /// 00274
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00280
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00284
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0028c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00290
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00294
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00298
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 002fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00300
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00310
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00314
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00318
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0032c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00330
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00334
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00338
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x0c780000,                                                  // Leading 1s:                                 /// 00344
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00348
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0034c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00350
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00354
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x4b000000,                                                  // 8388608.0                                   /// 0035c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00360
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00364
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00368
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0036c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00011111,                                                  // 9.7958E-41                                  /// 00374
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0037c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00380
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0038c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00390
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x0c700000,                                                  // Leading 1s:                                 /// 0039c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0xcb000000,                                                  // -8388608.0                                  /// 003c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003e0
			 0x33333333,                                                  // 4 random values                             /// 003e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00400
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0040c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00414
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00418
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0041c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00424
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00428
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0042c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00430
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x80000000,                                                  // -zero                                       /// 00440
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00448
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0044c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00450
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00458
			 0x0c600000,                                                  // Leading 1s:                                 /// 0045c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00460
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00464
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00468
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0046c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00470
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00474
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00478
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0047c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00484
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00490
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00494
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0049c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 004b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004bc
			 0xcb000000,                                                  // -8388608.0                                  /// 004c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004c4
			 0x4b000000,                                                  // 8388608.0                                   /// 004c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 004d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00500
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00504
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00508
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0050c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00510
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00514
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00518
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0051c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00524
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00528
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0052c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00530
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00534
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00538
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00540
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x55555555,                                                  // 4 random values                             /// 00548
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0054c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00550
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00554
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0055c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00564
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00568
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00578
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0057c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00580
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00584
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00594
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00598
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0059c
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00604
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0e000007,                                                  // Trailing 1s:                                /// 0060c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00610
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00614
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00620
			 0x0c780000,                                                  // Leading 1s:                                 /// 00624
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00628
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0062c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00638
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00640
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00648
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0064c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00650
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00654
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00658
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0065c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00660
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00670
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00674
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00678
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00680
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00684
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00688
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00690
			 0x33333333,                                                  // 4 random values                             /// 00694
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0069c
			 0x00000000,                                                  // zero                                        /// 006a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 006c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c4
			 0x00000000,                                                  // zero                                        /// 006c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00700
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00704
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00708
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0070c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00710
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00718
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0071c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00720
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00724
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0xffc00001,                                                  // -signaling NaN                              /// 0072c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00730
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00738
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0073c
			 0x7f800000,                                                  // inf                                         /// 00740
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00748
			 0x0c400000,                                                  // Leading 1s:                                 /// 0074c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00750
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00754
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00758
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0075c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00780
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00784
			 0x0e000007,                                                  // Trailing 1s:                                /// 00788
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0078c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00790
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007c0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007c4
			 0x00000000,                                                  // zero                                        /// 007c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d4
			 0x3f028f5c,                                                  // 0.51                                        /// 007d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007dc
			 0x3f028f5c,                                                  // 0.51                                        /// 007e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00800
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0080c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00810
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00814
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00818
			 0x80000000,                                                  // -zero                                       /// 0081c
			 0x80000000,                                                  // -zero                                       /// 00820
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0082c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00830
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00838
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0083c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00000000,                                                  // zero                                        /// 00850
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00858
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c780000,                                                  // Leading 1s:                                 /// 00864
			 0x0c780000,                                                  // Leading 1s:                                 /// 00868
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00870
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00874
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00878
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0087c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00888
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00894
			 0x0c780000,                                                  // Leading 1s:                                 /// 00898
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a0
			 0x7f800000,                                                  // inf                                         /// 008a4
			 0xcb000000,                                                  // -8388608.0                                  /// 008a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00900
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0090c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00910
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00914
			 0x7f800000,                                                  // inf                                         /// 00918
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0091c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00924
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0092c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00934
			 0x0c700000,                                                  // Leading 1s:                                 /// 00938
			 0x0e000001,                                                  // Trailing 1s:                                /// 0093c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00940
			 0x0e000003,                                                  // Trailing 1s:                                /// 00944
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00948
			 0x0e000001,                                                  // Trailing 1s:                                /// 0094c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00950
			 0x0c780000,                                                  // Leading 1s:                                 /// 00954
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00958
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00968
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0096c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00970
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00974
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0097c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00980
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0098c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00990
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00994
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00998
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0099c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x00000000,                                                  // zero                                        /// 009b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0xcb000000,                                                  // -8388608.0                                  /// 009c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f0
			 0x4b000000,                                                  // 8388608.0                                   /// 009f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a08
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a20
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a30
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a40
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a44
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a58
			 0x7f800000,                                                  // inf                                         /// 00a5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a70
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a74
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00aa0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ab0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ab4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ad4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00adc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00afc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b00
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b1c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b34
			 0xffc00001,                                                  // -signaling NaN                              /// 00b38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b3c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b48
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b64
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b84
			 0x7f800000,                                                  // inf                                         /// 00b88
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ba4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ba8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bdc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00be8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bf8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c1c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c34
			 0xbf028f5c,                                                  // -0.51                                       /// 00c38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c68
			 0x4b000000,                                                  // 8388608.0                                   /// 00c6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c70
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c9c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cb4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cb8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cc4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cd0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cd8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cdc
			 0xff800000,                                                  // -inf                                        /// 00ce0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ce8
			 0xbf028f5c,                                                  // -0.51                                       /// 00cec
			 0x33333333,                                                  // 4 random values                             /// 00cf0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cfc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d60
			 0x7f800000,                                                  // inf                                         /// 00d64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d70
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x33333333,                                                  // 4 random values                             /// 00d90
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00da8
			 0xcb000000,                                                  // -8388608.0                                  /// 00dac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00db0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000000,                                                  // -zero                                       /// 00dbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dc4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dcc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00df4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dfc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e44
			 0xbf028f5c,                                                  // -0.51                                       /// 00e48
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e50
			 0x33333333,                                                  // 4 random values                             /// 00e54
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e64
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e90
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ea0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ea4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00eb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ebc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ecc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ed8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00edc
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00eec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x7f800000,                                                  // inf                                         /// 00efc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f1c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f20
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f58
			 0xff800000,                                                  // -inf                                        /// 00f5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f80
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f84
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f88
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f8c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fdc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fe0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ff0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ff4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00004000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000250,
			 0x00000548,
			 0x0000051c,
			 0x0000033c,
			 0x000005e0,
			 0x00000054,
			 0x000000a0,
			 0x00000390,

			 /// vpu register f2
			 0x41d00000,
			 0x41100000,
			 0x41600000,
			 0x41800000,
			 0x41d80000,
			 0x41100000,
			 0x40e00000,
			 0x41a80000,

			 /// vpu register f3
			 0x41d80000,
			 0x41400000,
			 0x41b00000,
			 0x41a00000,
			 0x41300000,
			 0x41b00000,
			 0x41100000,
			 0x41400000,

			 /// vpu register f4
			 0x41a80000,
			 0x40800000,
			 0x41700000,
			 0x41d00000,
			 0x41c80000,
			 0x41e00000,
			 0x42000000,
			 0x41e00000,

			 /// vpu register f5
			 0x41f80000,
			 0x40e00000,
			 0x41980000,
			 0x41e80000,
			 0x41900000,
			 0x3f800000,
			 0x41b00000,
			 0x41500000,

			 /// vpu register f6
			 0x40e00000,
			 0x41c00000,
			 0x40e00000,
			 0x41700000,
			 0x41200000,
			 0x41200000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f7
			 0x41d00000,
			 0x40c00000,
			 0x3f800000,
			 0x41e80000,
			 0x41400000,
			 0x41400000,
			 0x41880000,
			 0x41980000,

			 /// vpu register f8
			 0x40000000,
			 0x41900000,
			 0x41d80000,
			 0x41000000,
			 0x41000000,
			 0x41b00000,
			 0x40a00000,
			 0x40c00000,

			 /// vpu register f9
			 0x40400000,
			 0x40a00000,
			 0x41500000,
			 0x41800000,
			 0x41f80000,
			 0x41100000,
			 0x3f800000,
			 0x41600000,

			 /// vpu register f10
			 0x41a00000,
			 0x41a00000,
			 0x42000000,
			 0x40e00000,
			 0x41d80000,
			 0x41f80000,
			 0x41e80000,
			 0x41e80000,

			 /// vpu register f11
			 0x41e80000,
			 0x41f00000,
			 0x3f800000,
			 0x41c80000,
			 0x41a80000,
			 0x41e80000,
			 0x40a00000,
			 0x3f800000,

			 /// vpu register f12
			 0x41d80000,
			 0x41980000,
			 0x41980000,
			 0x41e80000,
			 0x40c00000,
			 0x41980000,
			 0x41400000,
			 0x41a00000,

			 /// vpu register f13
			 0x40400000,
			 0x40800000,
			 0x41e80000,
			 0x41b80000,
			 0x40c00000,
			 0x3f800000,
			 0x41a00000,
			 0x40a00000,

			 /// vpu register f14
			 0x41f80000,
			 0x40400000,
			 0x42000000,
			 0x40000000,
			 0x41f00000,
			 0x40a00000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f15
			 0x41b80000,
			 0x41400000,
			 0x40c00000,
			 0x40000000,
			 0x40e00000,
			 0x41e80000,
			 0x41f00000,
			 0x41f80000,

			 /// vpu register f16
			 0x40a00000,
			 0x41d80000,
			 0x41980000,
			 0x41b80000,
			 0x41980000,
			 0x42000000,
			 0x3f800000,
			 0x41980000,

			 /// vpu register f17
			 0x41f80000,
			 0x41980000,
			 0x41800000,
			 0x41d80000,
			 0x40a00000,
			 0x41100000,
			 0x41600000,
			 0x41000000,

			 /// vpu register f18
			 0x41100000,
			 0x41000000,
			 0x41980000,
			 0x41400000,
			 0x3f800000,
			 0x42000000,
			 0x41a00000,
			 0x41a80000,

			 /// vpu register f19
			 0x41500000,
			 0x40000000,
			 0x40c00000,
			 0x3f800000,
			 0x41000000,
			 0x41a00000,
			 0x41f80000,
			 0x41600000,

			 /// vpu register f20
			 0x40a00000,
			 0x40400000,
			 0x41f00000,
			 0x41e80000,
			 0x41900000,
			 0x41880000,
			 0x41300000,
			 0x41980000,

			 /// vpu register f21
			 0x41e00000,
			 0x41d80000,
			 0x41700000,
			 0x41d00000,
			 0x41d00000,
			 0x41d00000,
			 0x41e00000,
			 0x40e00000,

			 /// vpu register f22
			 0x41b80000,
			 0x41880000,
			 0x41e80000,
			 0x41200000,
			 0x41880000,
			 0x40400000,
			 0x3f800000,
			 0x40800000,

			 /// vpu register f23
			 0x40c00000,
			 0x41800000,
			 0x41000000,
			 0x40e00000,
			 0x41700000,
			 0x41880000,
			 0x41f00000,
			 0x41e80000,

			 /// vpu register f24
			 0x41c80000,
			 0x41a00000,
			 0x41980000,
			 0x41900000,
			 0x41600000,
			 0x40800000,
			 0x40c00000,
			 0x41500000,

			 /// vpu register f25
			 0x42000000,
			 0x40800000,
			 0x40c00000,
			 0x40800000,
			 0x41e80000,
			 0x41d80000,
			 0x41300000,
			 0x40400000,

			 /// vpu register f26
			 0x41500000,
			 0x41000000,
			 0x41000000,
			 0x41b00000,
			 0x41e00000,
			 0x41200000,
			 0x41e00000,
			 0x41300000,

			 /// vpu register f27
			 0x41c00000,
			 0x41f80000,
			 0x41000000,
			 0x41e00000,
			 0x41b80000,
			 0x41880000,
			 0x40800000,
			 0x41400000,

			 /// vpu register f28
			 0x41400000,
			 0x41600000,
			 0x41500000,
			 0x41600000,
			 0x41a80000,
			 0x41a80000,
			 0x41e00000,
			 0x41f00000,

			 /// vpu register f29
			 0x3f800000,
			 0x41980000,
			 0x40800000,
			 0x41100000,
			 0x41d80000,
			 0x41900000,
			 0x41d00000,
			 0x42000000,

			 /// vpu register f30
			 0x40e00000,
			 0x40c00000,
			 0x41b00000,
			 0x41f80000,
			 0x41600000,
			 0x41300000,
			 0x41a80000,
			 0x41400000,

			 /// vpu register f31
			 0x41a00000,
			 0x41300000,
			 0x41a80000,
			 0x41d80000,
			 0x41a80000,
			 0x41f80000,
			 0x41700000,
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
		"fmax.pi f23, f16, f11\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f18, f3, f17\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f16, f26, f25\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f5, f7, f16\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f3, f2, f10\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f12, f9, f1\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f26, f12, f16\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f28, f13, f12\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f1, f0\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f17, f29, f4\n"                              ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f6, f7, f30\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f8, f18, f14\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f11, f20, f17\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f6, f24, f24\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f12, f14, f1\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f23, f11, f19\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f6, f4, f30\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f21, f4, f13\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f11, f11, f21\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f15, f15, f26\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f29, f24, f20\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f20, f27, f19\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f19, f30, f14\n"                             ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f3, f8, f8\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f28, f11\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f24, f3, f17\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f12, f5, f9\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f26, f10, f23\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f8, f21, f21\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f15, f26, f30\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f23, f3, f28\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f3, f11, f19\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f21, f29, f10\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f4, f16, f8\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f21, f26, f19\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f8, f30, f21\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f9, f8, f22\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f25, f8, f10\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f12, f3, f1\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f14, f16, f18\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f0, f25\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f25, f18, f14\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f20, f12, f17\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f4, f27, f7\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f23, f3, f26\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f28, f21, f5\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f9, f7, f8\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f16, f0, f22\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f18, f2, f13\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f17, f29, f2\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f19, f27, f16\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f19, f16, f0\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f25, f7, f7\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f7, f2, f5\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f19, f30, f3\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f21, f5, f28\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f10, f25, f8\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f14, f20, f6\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f26, f24, f15\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f11, f21, f14\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f4, f9, f5\n"                                ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f0, f9\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f5, f16, f14\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f25, f0\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f15, f4\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f5, f30, f9\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f8, f25, f20\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f3, f28, f8\n"                               ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f23, f24, f10\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f18, f12, f20\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f29, f22, f19\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f29, f8, f21\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f27, f18, f2\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f10, f24, f21\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f7, f22, f15\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f10, f5, f26\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f26, f13, f17\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f10, f25, f24\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f11, f2, f10\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f27, f5, f13\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f4, f10, f18\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f8, f20, f5\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f18, f14, f2\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f21, f16, f0\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f7, f19, f1\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f19, f30, f13\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f16, f9, f25\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f22, f29, f0\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f21, f25, f15\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f14, f6, f9\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f22, f7, f16\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f5, f10, f6\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f26, f15\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f10, f25, f21\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f24, f11, f1\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f3, f13, f6\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f11, f11, f22\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f13, f13, f27\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fmax.pi f4, f19, f30\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
