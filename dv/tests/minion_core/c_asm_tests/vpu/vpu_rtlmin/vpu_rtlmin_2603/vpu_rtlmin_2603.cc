/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//////////////////////////////////////////////////////////////////////// 
////// VPU FOCUS TEST: seed = 0x464327b  = 73675387
////// VPU FOCUS TEST: isa_focus = vpu_trans
////// VPU FOCUS TEST: uarch_focus = uarch_stress
////// VPU FOCUS TEST: cvg_choice = 0
//////////////////////////////////////////////////////////////////////// 
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
			 0xff800000,                                                  // -inf                                        /// 00000
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00008
			 0x80000000,                                                  // -zero                                       /// 0000c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00014
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00018
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0001c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00024
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00028
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0002c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00030
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00038
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0003c
			 0x7f800000,                                                  // inf                                         /// 00040
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00044
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x80011111,                                                  // -9.7958E-41                                 /// 0004c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00050
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00054
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00058
			 0x00011111,                                                  // 9.7958E-41                                  /// 0005c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00060
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00064
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00068
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0006c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0e000003,                                                  // Trailing 1s:                                /// 00078
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0007c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00084
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00088
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0008c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x80000000,                                                  // -zero                                       /// 000a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 000c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x7fc00001,                                                  // signaling NaN                               /// 000d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x4b000000,                                                  // 8388608.0                                   /// 000f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000f4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00100
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00104
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00110
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00114
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00118
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0011c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00120
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00124
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00128
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0012c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00130
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00144
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00148
			 0x00011111,                                                  // 9.7958E-41                                  /// 0014c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00150
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00154
			 0x00011111,                                                  // 9.7958E-41                                  /// 00158
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00160
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00168
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0016c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00170
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00174
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00188
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0018c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00190
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001b4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001c8
			 0xcb000000,                                                  // -8388608.0                                  /// 001cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00208
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00218
			 0x55555555,                                                  // 4 random values                             /// 0021c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00224
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0022c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00230
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00234
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00238
			 0x55555555,                                                  // 4 random values                             /// 0023c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00244
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00248
			 0x00000000,                                                  // zero                                        /// 0024c
			 0x55555555,                                                  // 4 random values                             /// 00250
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00254
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00258
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0025c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00260
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x0e000001,                                                  // Trailing 1s:                                /// 0026c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00274
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00278
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0027c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00280
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00284
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00288
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00290
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00294
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00298
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0029c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002b0
			 0x3f028f5c,                                                  // 0.51                                        /// 002b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0xbf028f5c,                                                  // -0.51                                       /// 002e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00300
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00304
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0030c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00318
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0031c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00324
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0032c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00330
			 0x55555555,                                                  // 4 random values                             /// 00334
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0033c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00340
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00348
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00350
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0035c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00360
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00364
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00368
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0036c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00370
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00374
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00378
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00380
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00388
			 0xffc00001,                                                  // -signaling NaN                              /// 0038c
			 0x00000000,                                                  // zero                                        /// 00390
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0039c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 003b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00400
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00408
			 0x0e000003,                                                  // Trailing 1s:                                /// 0040c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00410
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00418
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0041c
			 0xffc00001,                                                  // -signaling NaN                              /// 00420
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00428
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00430
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0043c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00440
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00448
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00454
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00470
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00478
			 0x55555555,                                                  // 4 random values                             /// 0047c
			 0x80000000,                                                  // -zero                                       /// 00480
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00494
			 0x0e000001,                                                  // Trailing 1s:                                /// 00498
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0049c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0xbf028f5c,                                                  // -0.51                                       /// 004f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00500
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00504
			 0xbf028f5c,                                                  // -0.51                                       /// 00508
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0050c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00514
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0xff800000,                                                  // -inf                                        /// 00520
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00524
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00528
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00534
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00538
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0053c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00548
			 0x0c600000,                                                  // Leading 1s:                                 /// 0054c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00550
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0055c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00560
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00564
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00568
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0056c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00570
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00574
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00578
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00580
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00590
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0059c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x55555555,                                                  // 4 random values                             /// 005d8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005e4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00600
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80011111,                                                  // -9.7958E-41                                 /// 00608
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00614
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00618
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0061c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00620
			 0xffc00001,                                                  // -signaling NaN                              /// 00624
			 0x33333333,                                                  // 4 random values                             /// 00628
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00630
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0063c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00640
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x0c600000,                                                  // Leading 1s:                                 /// 00648
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0064c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00650
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00654
			 0x7f800000,                                                  // inf                                         /// 00658
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0065c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00660
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00664
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00668
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x00000000,                                                  // zero                                        /// 00674
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00680
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00688
			 0xff800000,                                                  // -inf                                        /// 0068c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0069c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006a4
			 0xff800000,                                                  // -inf                                        /// 006a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000000,                                                  // zero                                        /// 006fc
			 0x80000000,                                                  // -zero                                       /// 00700
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00708
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0070c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00710
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00714
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00728
			 0x3f028f5c,                                                  // 0.51                                        /// 0072c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00730
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00734
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00740
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00744
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00748
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0074c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00758
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00764
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00768
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00774
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00778
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0077c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00780
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00784
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0e000001,                                                  // Trailing 1s:                                /// 0078c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00790
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00794
			 0x33333333,                                                  // 4 random values                             /// 00798
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0079c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 007ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 007b4
			 0xbf028f5c,                                                  // -0.51                                       /// 007b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007bc
			 0x00000000,                                                  // zero                                        /// 007c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00804
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0080c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00810
			 0x0c780000,                                                  // Leading 1s:                                 /// 00814
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00818
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00820
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00828
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0082c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x55555555,                                                  // 4 random values                             /// 00840
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00844
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0xffc00001,                                                  // -signaling NaN                              /// 0084c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00850
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00854
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00858
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0085c
			 0x33333333,                                                  // 4 random values                             /// 00860
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00864
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00868
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00874
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0087c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00880
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00884
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0088c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00890
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00894
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00898
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008b4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d4
			 0xbf028f5c,                                                  // -0.51                                       /// 008d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00914
			 0x33333333,                                                  // 4 random values                             /// 00918
			 0x0c600000,                                                  // Leading 1s:                                 /// 0091c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00924
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00928
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0092c
			 0xbf028f5c,                                                  // -0.51                                       /// 00930
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00944
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00948
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0094c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00958
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0095c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00968
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0096c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00970
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00974
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00978
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0097c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00980
			 0xff800000,                                                  // -inf                                        /// 00984
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00988
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0098c
			 0x33333333,                                                  // 4 random values                             /// 00990
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00994
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x7fc00001,                                                  // signaling NaN                               /// 009b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009d8
			 0x00000000,                                                  // zero                                        /// 009dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009f0
			 0xcb000000,                                                  // -8388608.0                                  /// 009f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a00
			 0x33333333,                                                  // 4 random values                             /// 00a04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a18
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a30
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a50
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a58
			 0x00000000,                                                  // zero                                        /// 00a5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a64
			 0x55555555,                                                  // 4 random values                             /// 00a68
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a6c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a84
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a98
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aa0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00aa8
			 0x80000000,                                                  // -zero                                       /// 00aac
			 0xbf028f5c,                                                  // -0.51                                       /// 00ab0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00abc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ac8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00acc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ae0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ae8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00af0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00afc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b88
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b98
			 0x7f800000,                                                  // inf                                         /// 00b9c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ba8
			 0x55555555,                                                  // 4 random values                             /// 00bac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00be8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c00
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c10
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c30
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c34
			 0xff800000,                                                  // -inf                                        /// 00c38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00c50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c60
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c9c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ce0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ce4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d14
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x00000000,                                                  // zero                                        /// 00d24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d30
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d38
			 0x3f028f5c,                                                  // 0.51                                        /// 00d3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d48
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d60
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d64
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d9c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00da8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00db4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dbc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00de0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xcb000000,                                                  // -8388608.0                                  /// 00de8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00df8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e24
			 0x7f800000,                                                  // inf                                         /// 00e28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e30
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e34
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e64
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x00000000,                                                  // zero                                        /// 00e84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ea4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eb8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ed0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ed8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00edc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ee0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00eec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f14
			 0xbf028f5c,                                                  // -0.51                                       /// 00f18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f34
			 0xcb000000,                                                  // -8388608.0                                  /// 00f38
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f58
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f5c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f70
			 0x80000000,                                                  // -zero                                       /// 00f74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f80
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fa0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fa4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fc0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fcc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fdc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ff0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ff4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ff8
			 0x80000100                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00000
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00018
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00024
			 0x3f028f5c,                                                  // 0.51                                        /// 00028
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0002c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00030
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00034
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0003c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00048
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0004c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x33333333,                                                  // 4 random values                             /// 00054
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00058
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00060
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00064
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00070
			 0x0e000001,                                                  // Trailing 1s:                                /// 00074
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00084
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00088
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0008c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00090
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00094
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00098
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0009c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000b8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000d8
			 0x80000000,                                                  // -zero                                       /// 000dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000f8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00104
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0010c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00110
			 0x7fc00001,                                                  // signaling NaN                               /// 00114
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00118
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00124
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00128
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0012c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00130
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00134
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00138
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0013c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00148
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00150
			 0x7f800000,                                                  // inf                                         /// 00154
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00158
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0015c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00164
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7fc00001,                                                  // signaling NaN                               /// 0016c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00170
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x7f800000,                                                  // inf                                         /// 0017c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00180
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00184
			 0x0e000007,                                                  // Trailing 1s:                                /// 00188
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0018c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00194
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00198
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0019c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x4b000000,                                                  // 8388608.0                                   /// 001b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0x0e000003,                                                  // Trailing 1s:                                /// 001e0
			 0x4b000000,                                                  // 8388608.0                                   /// 001e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00200
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x80011111,                                                  // -9.7958E-41                                 /// 00208
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0020c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00210
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0021c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00220
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00228
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0022c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00234
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00238
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0023c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00240
			 0x0e000003,                                                  // Trailing 1s:                                /// 00244
			 0x0c780000,                                                  // Leading 1s:                                 /// 00248
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0xffc00001,                                                  // -signaling NaN                              /// 00260
			 0x7f800000,                                                  // inf                                         /// 00264
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00268
			 0x0c600000,                                                  // Leading 1s:                                 /// 0026c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00270
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00274
			 0x0e000001,                                                  // Trailing 1s:                                /// 00278
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0027c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00284
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00288
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0028c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00294
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00298
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0029c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002b8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 002d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0xcb000000,                                                  // -8388608.0                                  /// 002e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0030c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00310
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00318
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0031c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00320
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00324
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00328
			 0x00011111,                                                  // 9.7958E-41                                  /// 0032c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00330
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00338
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0033c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00340
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00344
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00348
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0034c
			 0x4b000000,                                                  // 8388608.0                                   /// 00350
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00354
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00360
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00368
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00374
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00378
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0037c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00380
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00384
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00388
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0038c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00390
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00394
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003d8
			 0xcb000000,                                                  // -8388608.0                                  /// 003dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003e0
			 0xcb000000,                                                  // -8388608.0                                  /// 003e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00400
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00404
			 0x7f800000,                                                  // inf                                         /// 00408
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00410
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00414
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00418
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0041c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00420
			 0xbf028f5c,                                                  // -0.51                                       /// 00424
			 0xcb000000,                                                  // -8388608.0                                  /// 00428
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00438
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0043c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00440
			 0x3f028f5c,                                                  // 0.51                                        /// 00444
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00448
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0044c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00460
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00468
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0046c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00470
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00474
			 0xff800000,                                                  // -inf                                        /// 00478
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0047c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00480
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00484
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00488
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00490
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00498
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0049c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b4
			 0xbf028f5c,                                                  // -0.51                                       /// 004b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 004f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004f4
			 0x7f800000,                                                  // inf                                         /// 004f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00500
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00504
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00508
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00510
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00518
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0051c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00520
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00524
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0052c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00538
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00540
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00554
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0055c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00560
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00564
			 0x7f800000,                                                  // inf                                         /// 00568
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0056c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00584
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0058c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00590
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00594
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00598
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0059c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005ac
			 0x55555555,                                                  // 4 random values                             /// 005b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005b4
			 0x4b000000,                                                  // 8388608.0                                   /// 005b8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005ec
			 0xcb000000,                                                  // -8388608.0                                  /// 005f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00600
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00608
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0060c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00610
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00614
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00618
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0061c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00620
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0062c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00634
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00638
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00640
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00644
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00648
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0064c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00650
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00658
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0065c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00660
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00664
			 0xff800000,                                                  // -inf                                        /// 00668
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0066c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00674
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e000007,                                                  // Trailing 1s:                                /// 0067c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00680
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00684
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00688
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0068c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00690
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00694
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00698
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x80000000,                                                  // -zero                                       /// 006e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 006e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00704
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00708
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0070c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00710
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00718
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00728
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0072c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00730
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00734
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00738
			 0x3f028f5c,                                                  // 0.51                                        /// 0073c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00740
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00744
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00754
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00758
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0075c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00760
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00764
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00768
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0076c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00778
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0078c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00790
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00798
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0x4b000000,                                                  // 8388608.0                                   /// 007ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00800
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00804
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0080c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00814
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00828
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0082c
			 0x7f800000,                                                  // inf                                         /// 00830
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00834
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00838
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00840
			 0xbf028f5c,                                                  // -0.51                                       /// 00844
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00848
			 0x4b000000,                                                  // 8388608.0                                   /// 0084c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00854
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00858
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0085c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00860
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00864
			 0x0c700000,                                                  // Leading 1s:                                 /// 00868
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0086c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00870
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00874
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0087c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00880
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00888
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0088c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00890
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00894
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00898
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0089c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008a4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 008dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00900
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00908
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00910
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00914
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00918
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0091c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00924
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00934
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0xffc00001,                                                  // -signaling NaN                              /// 00940
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00948
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00954
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00958
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00960
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00964
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00968
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0096c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00974
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00978
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00984
			 0x0c400000,                                                  // Leading 1s:                                 /// 00988
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00990
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00994
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x80000000,                                                  // -zero                                       /// 009ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x3f028f5c,                                                  // 0.51                                        /// 00a04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a14
			 0x55555555,                                                  // 4 random values                             /// 00a18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a2c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a30
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x4b000000,                                                  // 8388608.0                                   /// 00a6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a98
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a9c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x55555555,                                                  // 4 random values                             /// 00aa4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aa8
			 0x00000000,                                                  // zero                                        /// 00aac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ab0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00abc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ac0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ac4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ac8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ad8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00adc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ae0
			 0x33333333,                                                  // 4 random values                             /// 00ae4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00af0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00afc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b14
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b18
			 0x80000000,                                                  // -zero                                       /// 00b1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b28
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b40
			 0x3f028f5c,                                                  // 0.51                                        /// 00b44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b48
			 0xffc00001,                                                  // -signaling NaN                              /// 00b4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80000000,                                                  // -zero                                       /// 00b64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00be0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000000,                                                  // -zero                                       /// 00bf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c04
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c0c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0xff800000,                                                  // -inf                                        /// 00c34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c70
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c74
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cbc
			 0x80000000,                                                  // -zero                                       /// 00cc0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ccc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ce4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ce8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d24
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x4b000000,                                                  // 8388608.0                                   /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d6c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d78
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d8c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00db4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dc4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dcc
			 0x7f800000,                                                  // inf                                         /// 00dd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ddc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00de0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00de4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00de8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x33333333,                                                  // 4 random values                             /// 00df0
			 0x80000000,                                                  // -zero                                       /// 00df4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00df8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e00
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e30
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e48
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e50
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80000000,                                                  // -zero                                       /// 00e90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ea8
			 0x4b000000,                                                  // 8388608.0                                   /// 00eac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ec4
			 0x80000000,                                                  // -zero                                       /// 00ec8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ecc
			 0x80000000,                                                  // -zero                                       /// 00ed0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00edc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ef8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00efc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f10
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f3c
			 0xff800000,                                                  // -inf                                        /// 00f40
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f50
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f64
			 0xffc00001,                                                  // -signaling NaN                              /// 00f68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f7c
			 0x33333333,                                                  // 4 random values                             /// 00f80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fa4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fa8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fb0
			 0x80000000,                                                  // -zero                                       /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fe4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ff0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff8
			 0x80000008                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xfbdcad1a, /// 0x0
			 0xb19b1901, /// 0x4
			 0xaec539ff, /// 0x8
			 0xe89c536a, /// 0xc
			 0x854c80aa, /// 0x10
			 0x9261c703, /// 0x14
			 0xb7d5c487, /// 0x18
			 0x81fbbee5, /// 0x1c
			 0x37c01e0a, /// 0x20
			 0x31a74d8c, /// 0x24
			 0xf2509996, /// 0x28
			 0x09637b46, /// 0x2c
			 0x875928fe, /// 0x30
			 0xb0043fb6, /// 0x34
			 0xdcffa7dd, /// 0x38
			 0x08a4cdec, /// 0x3c
			 0xe5183d92, /// 0x40
			 0x262f192f, /// 0x44
			 0x608d4e38, /// 0x48
			 0xf4754e8d, /// 0x4c
			 0xf3dd0708, /// 0x50
			 0xf4e8a62b, /// 0x54
			 0x4039b292, /// 0x58
			 0x1a7618a8, /// 0x5c
			 0x42b436f4, /// 0x60
			 0xf1e0040c, /// 0x64
			 0x903b82ed, /// 0x68
			 0xd5fcedf2, /// 0x6c
			 0xdd2f25a9, /// 0x70
			 0x3dcdf1c1, /// 0x74
			 0x0643ab0c, /// 0x78
			 0x499fc369, /// 0x7c
			 0xe845e77a, /// 0x80
			 0x7711a720, /// 0x84
			 0x978d547c, /// 0x88
			 0x648aa0a2, /// 0x8c
			 0xd247d2b5, /// 0x90
			 0xf6403300, /// 0x94
			 0x4b42cbd4, /// 0x98
			 0xc80b5540, /// 0x9c
			 0x686b73d5, /// 0xa0
			 0x31c4ab19, /// 0xa4
			 0x27aacd66, /// 0xa8
			 0xcd64e091, /// 0xac
			 0x3186791d, /// 0xb0
			 0x01edb70d, /// 0xb4
			 0xaabf59e8, /// 0xb8
			 0xcedc3b79, /// 0xbc
			 0x0679d55c, /// 0xc0
			 0x56cf0733, /// 0xc4
			 0x5c6cd68d, /// 0xc8
			 0xc3d7805d, /// 0xcc
			 0xca2c1475, /// 0xd0
			 0x30319383, /// 0xd4
			 0xb1d552e8, /// 0xd8
			 0x38918746, /// 0xdc
			 0xc888bebd, /// 0xe0
			 0xbd0ea1cb, /// 0xe4
			 0x3676a516, /// 0xe8
			 0x0c6d2496, /// 0xec
			 0xab45e87a, /// 0xf0
			 0x746a3090, /// 0xf4
			 0xd7d4396e, /// 0xf8
			 0x47f3100e, /// 0xfc
			 0x12de0cde, /// 0x100
			 0xe4dc66d2, /// 0x104
			 0xcc21bb63, /// 0x108
			 0xeb8787a3, /// 0x10c
			 0xf0417aa1, /// 0x110
			 0x63ee2590, /// 0x114
			 0x12162fd1, /// 0x118
			 0xebac5df1, /// 0x11c
			 0x38773fd1, /// 0x120
			 0xe75657f9, /// 0x124
			 0x2f016449, /// 0x128
			 0x3beca9f6, /// 0x12c
			 0x4ad2d239, /// 0x130
			 0x6456e87f, /// 0x134
			 0x4585237e, /// 0x138
			 0xba375bef, /// 0x13c
			 0xa541b97d, /// 0x140
			 0x0f07c519, /// 0x144
			 0xb89c8cd0, /// 0x148
			 0x7276fafd, /// 0x14c
			 0x3f20db94, /// 0x150
			 0x5843c097, /// 0x154
			 0x62edc5b1, /// 0x158
			 0xeff49dec, /// 0x15c
			 0x0c1eb5f8, /// 0x160
			 0xb4bc0186, /// 0x164
			 0x820cedb1, /// 0x168
			 0x26360144, /// 0x16c
			 0x5728fdfe, /// 0x170
			 0x5bc6eb04, /// 0x174
			 0x06c608bd, /// 0x178
			 0x21fbd163, /// 0x17c
			 0xf3509de0, /// 0x180
			 0xd29291b4, /// 0x184
			 0xee370829, /// 0x188
			 0xe8e84baa, /// 0x18c
			 0x4fc1c7e0, /// 0x190
			 0x3286d775, /// 0x194
			 0xa72ae6ff, /// 0x198
			 0xb630011d, /// 0x19c
			 0xd7c9b562, /// 0x1a0
			 0x893286be, /// 0x1a4
			 0x6ec7d004, /// 0x1a8
			 0xd6cd9f86, /// 0x1ac
			 0x2e13e3b2, /// 0x1b0
			 0x90f29b7e, /// 0x1b4
			 0x2b281007, /// 0x1b8
			 0x36e6ac85, /// 0x1bc
			 0x8a599f54, /// 0x1c0
			 0xd34f72d6, /// 0x1c4
			 0xeb7b1a03, /// 0x1c8
			 0xd54779a7, /// 0x1cc
			 0x33c929df, /// 0x1d0
			 0xbd48c982, /// 0x1d4
			 0x2fbc482c, /// 0x1d8
			 0x4c77ec7a, /// 0x1dc
			 0x48ff78fe, /// 0x1e0
			 0x9cb05775, /// 0x1e4
			 0x97e39aef, /// 0x1e8
			 0xd91c9b32, /// 0x1ec
			 0x98cffcf0, /// 0x1f0
			 0x909336c7, /// 0x1f4
			 0xa34faf1d, /// 0x1f8
			 0xf37814a7, /// 0x1fc
			 0x28b8dc39, /// 0x200
			 0xbe5e30c8, /// 0x204
			 0x26d35a1c, /// 0x208
			 0xc88eae2f, /// 0x20c
			 0x151b43ee, /// 0x210
			 0x684341a2, /// 0x214
			 0x674304d7, /// 0x218
			 0xe38cc6d4, /// 0x21c
			 0xda026cea, /// 0x220
			 0x2bf7f492, /// 0x224
			 0x526173ef, /// 0x228
			 0x1d5c2735, /// 0x22c
			 0xbbe6c9c0, /// 0x230
			 0xfa8a84e2, /// 0x234
			 0x4de76c8e, /// 0x238
			 0xa1fdbec2, /// 0x23c
			 0xd08c7f4c, /// 0x240
			 0x31155331, /// 0x244
			 0x9956aab6, /// 0x248
			 0xd6e32bc3, /// 0x24c
			 0x228b6f43, /// 0x250
			 0xef0941b3, /// 0x254
			 0xc37f525f, /// 0x258
			 0x742b8c09, /// 0x25c
			 0x1194e687, /// 0x260
			 0xbf399e32, /// 0x264
			 0xcdd2df84, /// 0x268
			 0x148a31a8, /// 0x26c
			 0x8e77bf77, /// 0x270
			 0xda1910c5, /// 0x274
			 0xfd872200, /// 0x278
			 0xd9579188, /// 0x27c
			 0xf5e2cfae, /// 0x280
			 0xf988bf0e, /// 0x284
			 0x5b8411ed, /// 0x288
			 0xbeaa9fde, /// 0x28c
			 0x9959d4b6, /// 0x290
			 0x7b0f8861, /// 0x294
			 0x9ceacf92, /// 0x298
			 0x33172da9, /// 0x29c
			 0x2313a7f6, /// 0x2a0
			 0x92bb7bcf, /// 0x2a4
			 0xbb742be1, /// 0x2a8
			 0x07a068b2, /// 0x2ac
			 0x74bafe6b, /// 0x2b0
			 0x98a3ed43, /// 0x2b4
			 0xa1a7c041, /// 0x2b8
			 0x2abd9236, /// 0x2bc
			 0x88705235, /// 0x2c0
			 0x791da70e, /// 0x2c4
			 0x52b64a6f, /// 0x2c8
			 0x39157dd3, /// 0x2cc
			 0xa4a44392, /// 0x2d0
			 0xb1defbb7, /// 0x2d4
			 0x6e45decd, /// 0x2d8
			 0xec6e1ef1, /// 0x2dc
			 0x71b46e05, /// 0x2e0
			 0xe2487f9e, /// 0x2e4
			 0xfb6d112f, /// 0x2e8
			 0xdf1745b9, /// 0x2ec
			 0x95edf5cb, /// 0x2f0
			 0x3f007586, /// 0x2f4
			 0xa465dc0b, /// 0x2f8
			 0xba291743, /// 0x2fc
			 0x9b1afbc6, /// 0x300
			 0x6a5fde86, /// 0x304
			 0xa500a197, /// 0x308
			 0xde7d3424, /// 0x30c
			 0xf6b13eca, /// 0x310
			 0xfd7bc4a0, /// 0x314
			 0xcd46fe8d, /// 0x318
			 0xa66b94b3, /// 0x31c
			 0xb6c548d2, /// 0x320
			 0x30fda3b4, /// 0x324
			 0xdee800f4, /// 0x328
			 0x6144f456, /// 0x32c
			 0x8a54266a, /// 0x330
			 0x0cf79e53, /// 0x334
			 0xc5d8b4f6, /// 0x338
			 0xfc6566cf, /// 0x33c
			 0x138d7856, /// 0x340
			 0xa1a41306, /// 0x344
			 0x26083459, /// 0x348
			 0x3cef8c32, /// 0x34c
			 0xd644c90e, /// 0x350
			 0x2e5e08c6, /// 0x354
			 0x4b991f71, /// 0x358
			 0x4b4efc91, /// 0x35c
			 0xd2f714ea, /// 0x360
			 0x2f516b67, /// 0x364
			 0xeec24781, /// 0x368
			 0x2eee3ba6, /// 0x36c
			 0xaa28ff90, /// 0x370
			 0x6c36fad9, /// 0x374
			 0x098c1b16, /// 0x378
			 0x02dbf84c, /// 0x37c
			 0x276445e6, /// 0x380
			 0xc679d354, /// 0x384
			 0x1e528b2f, /// 0x388
			 0x15741551, /// 0x38c
			 0x74250b74, /// 0x390
			 0xdef7e215, /// 0x394
			 0x76229718, /// 0x398
			 0x7c11dc74, /// 0x39c
			 0x6a6852cd, /// 0x3a0
			 0x17aa3634, /// 0x3a4
			 0xff0fc6ff, /// 0x3a8
			 0x33a34f21, /// 0x3ac
			 0x1f5ca520, /// 0x3b0
			 0x02594d77, /// 0x3b4
			 0x6db034c6, /// 0x3b8
			 0x8489e753, /// 0x3bc
			 0x4bd65f06, /// 0x3c0
			 0xc683f93f, /// 0x3c4
			 0x48533bae, /// 0x3c8
			 0x1eabb293, /// 0x3cc
			 0x2cd614b8, /// 0x3d0
			 0x3e6388dc, /// 0x3d4
			 0x99acb13e, /// 0x3d8
			 0xc700746f, /// 0x3dc
			 0xc95d53f9, /// 0x3e0
			 0x00d65d8c, /// 0x3e4
			 0x2814a620, /// 0x3e8
			 0xfbe726ca, /// 0x3ec
			 0x9648006e, /// 0x3f0
			 0xe397df12, /// 0x3f4
			 0x5196976e, /// 0x3f8
			 0x53d2a3c1, /// 0x3fc
			 0xe5141f7b, /// 0x400
			 0x0f4ef51d, /// 0x404
			 0xc7b9f57a, /// 0x408
			 0x355a6f90, /// 0x40c
			 0x35d8fe96, /// 0x410
			 0x96c76899, /// 0x414
			 0x77a5ad91, /// 0x418
			 0xb3140047, /// 0x41c
			 0x24b4c95b, /// 0x420
			 0xa37df915, /// 0x424
			 0x0d50c32c, /// 0x428
			 0xaa2e0252, /// 0x42c
			 0x21ec785a, /// 0x430
			 0xd9207b51, /// 0x434
			 0xe6b2f247, /// 0x438
			 0xf2438264, /// 0x43c
			 0x209747ac, /// 0x440
			 0x59338e80, /// 0x444
			 0x6075ce80, /// 0x448
			 0x12b896bc, /// 0x44c
			 0x1f557fea, /// 0x450
			 0xe2531a7b, /// 0x454
			 0x8269dcfb, /// 0x458
			 0x73dd7ab1, /// 0x45c
			 0x3bc6402f, /// 0x460
			 0x978984d8, /// 0x464
			 0x004176c8, /// 0x468
			 0x3bafc640, /// 0x46c
			 0xd65e6e82, /// 0x470
			 0x25fa5014, /// 0x474
			 0xcbac6c22, /// 0x478
			 0x131009f7, /// 0x47c
			 0x7f326247, /// 0x480
			 0x6994c3a7, /// 0x484
			 0x30d4efac, /// 0x488
			 0x305fdc97, /// 0x48c
			 0x309acf52, /// 0x490
			 0xde003f56, /// 0x494
			 0x61528ef9, /// 0x498
			 0xd81e6445, /// 0x49c
			 0x6416048c, /// 0x4a0
			 0x522d30ce, /// 0x4a4
			 0x34a3c59b, /// 0x4a8
			 0x5a04147f, /// 0x4ac
			 0x04ff4a04, /// 0x4b0
			 0xf88b151b, /// 0x4b4
			 0xff3bf917, /// 0x4b8
			 0xf4e6d26c, /// 0x4bc
			 0xc9dee1aa, /// 0x4c0
			 0x6e185fd7, /// 0x4c4
			 0x7ffd1785, /// 0x4c8
			 0x0e151601, /// 0x4cc
			 0x3fb95141, /// 0x4d0
			 0x9aa9561d, /// 0x4d4
			 0xe8d414e5, /// 0x4d8
			 0x1a1c8f6c, /// 0x4dc
			 0x613c35c6, /// 0x4e0
			 0x938b2c93, /// 0x4e4
			 0x33ffc828, /// 0x4e8
			 0x0661dabc, /// 0x4ec
			 0xcf03034d, /// 0x4f0
			 0x55cc3f4e, /// 0x4f4
			 0x09e149a9, /// 0x4f8
			 0x2a902936, /// 0x4fc
			 0x029f66df, /// 0x500
			 0x3fcc5998, /// 0x504
			 0x1014e625, /// 0x508
			 0x109c950e, /// 0x50c
			 0xf928ad2d, /// 0x510
			 0x5e411a67, /// 0x514
			 0x014e32ae, /// 0x518
			 0x827a4ae4, /// 0x51c
			 0x55274565, /// 0x520
			 0x7ca66b25, /// 0x524
			 0x6bc871b2, /// 0x528
			 0xcddf6138, /// 0x52c
			 0xcc5c950b, /// 0x530
			 0x5c7a2995, /// 0x534
			 0xc113ef35, /// 0x538
			 0x505d95f1, /// 0x53c
			 0x11d3dedf, /// 0x540
			 0x36af55c2, /// 0x544
			 0xa55aadf9, /// 0x548
			 0x21dab202, /// 0x54c
			 0xd6c05ac7, /// 0x550
			 0xbe54449b, /// 0x554
			 0x80f6f55d, /// 0x558
			 0x03780e1e, /// 0x55c
			 0x6148c312, /// 0x560
			 0x5eebc9f3, /// 0x564
			 0x2e513b89, /// 0x568
			 0x914b8f4e, /// 0x56c
			 0x3563fffb, /// 0x570
			 0x30c692eb, /// 0x574
			 0xc2cbeb90, /// 0x578
			 0xf8f55440, /// 0x57c
			 0xbe4130cf, /// 0x580
			 0xd242cdff, /// 0x584
			 0xaad4254d, /// 0x588
			 0x84f7dc8e, /// 0x58c
			 0xa7032613, /// 0x590
			 0x7981560a, /// 0x594
			 0x54eb77bd, /// 0x598
			 0x597ad1d6, /// 0x59c
			 0xf4084d53, /// 0x5a0
			 0x31609dc3, /// 0x5a4
			 0x897e2c0a, /// 0x5a8
			 0x1900e7d7, /// 0x5ac
			 0xad974fc5, /// 0x5b0
			 0x97885b8a, /// 0x5b4
			 0xef64c1e4, /// 0x5b8
			 0x24575b17, /// 0x5bc
			 0xab7ece7c, /// 0x5c0
			 0xa9e01191, /// 0x5c4
			 0x445ab008, /// 0x5c8
			 0x008cb92c, /// 0x5cc
			 0x46eddfb6, /// 0x5d0
			 0x26426efd, /// 0x5d4
			 0x29cef17f, /// 0x5d8
			 0x4343e66a, /// 0x5dc
			 0x91cb8da0, /// 0x5e0
			 0x6be4c1eb, /// 0x5e4
			 0x5a84923d, /// 0x5e8
			 0xb090324b, /// 0x5ec
			 0xa448358f, /// 0x5f0
			 0x9e35cb30, /// 0x5f4
			 0x9c505113, /// 0x5f8
			 0x7f607de0, /// 0x5fc
			 0xb7045bf9, /// 0x600
			 0x2326492d, /// 0x604
			 0x4a5f140a, /// 0x608
			 0xda5a7504, /// 0x60c
			 0xb45d6b8e, /// 0x610
			 0x00cbfdd9, /// 0x614
			 0x7a217b1b, /// 0x618
			 0xedfb0f45, /// 0x61c
			 0x2492dc42, /// 0x620
			 0x8d1105cb, /// 0x624
			 0x19f63a71, /// 0x628
			 0x4f110c0f, /// 0x62c
			 0xafde82fc, /// 0x630
			 0x857d261c, /// 0x634
			 0xf7710e30, /// 0x638
			 0x3b612a46, /// 0x63c
			 0x7e3c7af4, /// 0x640
			 0xe3ae64a0, /// 0x644
			 0x2e73a107, /// 0x648
			 0x9611e17b, /// 0x64c
			 0x43a6677b, /// 0x650
			 0xc1bd1000, /// 0x654
			 0xd2d3636e, /// 0x658
			 0x216c3645, /// 0x65c
			 0x316dd16e, /// 0x660
			 0x3efc8bbd, /// 0x664
			 0xd9e94d94, /// 0x668
			 0xbe59cf19, /// 0x66c
			 0x00ae48f6, /// 0x670
			 0x96a550cf, /// 0x674
			 0xd03b3e8d, /// 0x678
			 0xfc698f0e, /// 0x67c
			 0xf5683fcc, /// 0x680
			 0x7dc070ac, /// 0x684
			 0xf5fe1b79, /// 0x688
			 0xa3fd6d7a, /// 0x68c
			 0x82bf5b05, /// 0x690
			 0x1664c8fe, /// 0x694
			 0x8fdb7810, /// 0x698
			 0x4a65bd6a, /// 0x69c
			 0x70bc3e8f, /// 0x6a0
			 0x3546ff32, /// 0x6a4
			 0xbdd37d00, /// 0x6a8
			 0x2540f4ab, /// 0x6ac
			 0xa5974bd8, /// 0x6b0
			 0xab406d6b, /// 0x6b4
			 0xe96815dd, /// 0x6b8
			 0x456574d4, /// 0x6bc
			 0xb8df1287, /// 0x6c0
			 0x8a26895c, /// 0x6c4
			 0x5f12bddd, /// 0x6c8
			 0x01dfe815, /// 0x6cc
			 0xe1adc0bd, /// 0x6d0
			 0x1133dfc5, /// 0x6d4
			 0xc8b6dbab, /// 0x6d8
			 0x25ab877d, /// 0x6dc
			 0x8668dea5, /// 0x6e0
			 0xebbcd0ed, /// 0x6e4
			 0x36cd4dc4, /// 0x6e8
			 0xcd12051a, /// 0x6ec
			 0x881779fc, /// 0x6f0
			 0x8c7dc73c, /// 0x6f4
			 0x60b077fd, /// 0x6f8
			 0x4f6e3a55, /// 0x6fc
			 0xdc2973cb, /// 0x700
			 0xe22864f7, /// 0x704
			 0x6305fdd9, /// 0x708
			 0xa40de5b2, /// 0x70c
			 0xbd909cf7, /// 0x710
			 0xdbed3504, /// 0x714
			 0x27d0227b, /// 0x718
			 0x1a9d4371, /// 0x71c
			 0x93d130df, /// 0x720
			 0xac69ad07, /// 0x724
			 0x91fd96b6, /// 0x728
			 0x44dc6063, /// 0x72c
			 0x7a1ec8bd, /// 0x730
			 0xa7205202, /// 0x734
			 0x22aac3cc, /// 0x738
			 0x343ff548, /// 0x73c
			 0x3ce6e54f, /// 0x740
			 0x8521b96c, /// 0x744
			 0xe40edbec, /// 0x748
			 0xbd09973d, /// 0x74c
			 0x6b86a644, /// 0x750
			 0x9dc25ed6, /// 0x754
			 0x04bea927, /// 0x758
			 0xd2632f8c, /// 0x75c
			 0xd3cbd375, /// 0x760
			 0x367eb0e0, /// 0x764
			 0x0eeba632, /// 0x768
			 0xad7f7888, /// 0x76c
			 0x68ab0d3e, /// 0x770
			 0xbf2f9194, /// 0x774
			 0xa8a1bc71, /// 0x778
			 0x3641637a, /// 0x77c
			 0x2d25d772, /// 0x780
			 0x0126d841, /// 0x784
			 0x7fecd707, /// 0x788
			 0x69b84933, /// 0x78c
			 0xf7ee4825, /// 0x790
			 0xbfd3683b, /// 0x794
			 0x9d7c8037, /// 0x798
			 0x7a3821e4, /// 0x79c
			 0x1fe7274f, /// 0x7a0
			 0xe441d575, /// 0x7a4
			 0x2dd105dc, /// 0x7a8
			 0x4a51b811, /// 0x7ac
			 0x7c2ea397, /// 0x7b0
			 0x9a95df46, /// 0x7b4
			 0x6e31e527, /// 0x7b8
			 0x2def837e, /// 0x7bc
			 0x559c80d2, /// 0x7c0
			 0x0d7bb273, /// 0x7c4
			 0x9423d86e, /// 0x7c8
			 0xcf9c7599, /// 0x7cc
			 0xa8f54973, /// 0x7d0
			 0xa3e9fd0a, /// 0x7d4
			 0x351f06fe, /// 0x7d8
			 0x5c8397ac, /// 0x7dc
			 0x3d44cf8d, /// 0x7e0
			 0xada7ad1a, /// 0x7e4
			 0xc539ec78, /// 0x7e8
			 0xbb063ec4, /// 0x7ec
			 0x74adb3e0, /// 0x7f0
			 0x7618583a, /// 0x7f4
			 0xfd533c46, /// 0x7f8
			 0x6964b918, /// 0x7fc
			 0x417ad2d0, /// 0x800
			 0x712ce6db, /// 0x804
			 0x53b031b5, /// 0x808
			 0x4c3f6133, /// 0x80c
			 0xc1dc071d, /// 0x810
			 0x0a7a2522, /// 0x814
			 0xe2e1a921, /// 0x818
			 0x967fbfd2, /// 0x81c
			 0xdc4bd675, /// 0x820
			 0x72db4d28, /// 0x824
			 0x11c5f01a, /// 0x828
			 0xe32d74ac, /// 0x82c
			 0xd2eab7d1, /// 0x830
			 0xf80393d3, /// 0x834
			 0xda75a1d5, /// 0x838
			 0x9eb3a3f6, /// 0x83c
			 0x80d3666c, /// 0x840
			 0xd3e4de5b, /// 0x844
			 0xdd7874f8, /// 0x848
			 0xe251eb83, /// 0x84c
			 0x3c28b77c, /// 0x850
			 0x293730ae, /// 0x854
			 0x7992e571, /// 0x858
			 0x6a7f8e38, /// 0x85c
			 0xfd413e60, /// 0x860
			 0x75ee6c68, /// 0x864
			 0xc9c76b82, /// 0x868
			 0x7447d7a2, /// 0x86c
			 0x6742d1fd, /// 0x870
			 0xada0d0e7, /// 0x874
			 0x13529412, /// 0x878
			 0xbdc6bcce, /// 0x87c
			 0xb3d39c2c, /// 0x880
			 0x113a489c, /// 0x884
			 0xff3e0fa2, /// 0x888
			 0x82ba2383, /// 0x88c
			 0x225cfa41, /// 0x890
			 0x547d42b8, /// 0x894
			 0x15fa3b7b, /// 0x898
			 0xb0afee9b, /// 0x89c
			 0xafa3aeae, /// 0x8a0
			 0xb1d52500, /// 0x8a4
			 0xbd93867b, /// 0x8a8
			 0x9cfcabf0, /// 0x8ac
			 0x3c2a6e70, /// 0x8b0
			 0xc3c11182, /// 0x8b4
			 0x6f157ac3, /// 0x8b8
			 0x96de7e7b, /// 0x8bc
			 0xd58d5237, /// 0x8c0
			 0xd3108fd2, /// 0x8c4
			 0xf4619c3b, /// 0x8c8
			 0x881807f3, /// 0x8cc
			 0x3813aa1f, /// 0x8d0
			 0xf0bf5b20, /// 0x8d4
			 0xcedd7282, /// 0x8d8
			 0x44464624, /// 0x8dc
			 0x7a07a39b, /// 0x8e0
			 0x499fa7ed, /// 0x8e4
			 0x19ea7d08, /// 0x8e8
			 0xd0a3f8d4, /// 0x8ec
			 0xd25c05b0, /// 0x8f0
			 0xdc5e8f7f, /// 0x8f4
			 0xb2c33cd3, /// 0x8f8
			 0x3cce93b9, /// 0x8fc
			 0x5b0483c2, /// 0x900
			 0xf98a8c18, /// 0x904
			 0x911adf2e, /// 0x908
			 0x4b45afad, /// 0x90c
			 0x42cec078, /// 0x910
			 0x0c384d1e, /// 0x914
			 0xdeaa83e6, /// 0x918
			 0x95d370b2, /// 0x91c
			 0x7a5dad19, /// 0x920
			 0x6dbeeee2, /// 0x924
			 0xe2484f07, /// 0x928
			 0x79d56e40, /// 0x92c
			 0xd722a38c, /// 0x930
			 0x56b3ec80, /// 0x934
			 0xfc1cc605, /// 0x938
			 0xcfd5aebe, /// 0x93c
			 0xe7a6b3d4, /// 0x940
			 0x75d17cce, /// 0x944
			 0x1d069565, /// 0x948
			 0x812640be, /// 0x94c
			 0x7b549839, /// 0x950
			 0x1bf40990, /// 0x954
			 0x0c680a53, /// 0x958
			 0x03e71d6e, /// 0x95c
			 0x0a97e5da, /// 0x960
			 0x758329f8, /// 0x964
			 0xdb7e2770, /// 0x968
			 0xcbfb43bc, /// 0x96c
			 0x6561b0b5, /// 0x970
			 0x952db657, /// 0x974
			 0x3484f8a6, /// 0x978
			 0x847c74ed, /// 0x97c
			 0xb47c8183, /// 0x980
			 0x3b61f67c, /// 0x984
			 0x9130c8da, /// 0x988
			 0xde1ebed1, /// 0x98c
			 0x024579e9, /// 0x990
			 0xcee55131, /// 0x994
			 0xd2f8e91c, /// 0x998
			 0x7aa7cbc4, /// 0x99c
			 0xc58e657d, /// 0x9a0
			 0xb1fdc754, /// 0x9a4
			 0xec558fb0, /// 0x9a8
			 0x38a36ba5, /// 0x9ac
			 0xc301b6cd, /// 0x9b0
			 0x973c33b7, /// 0x9b4
			 0x9b0ecbc7, /// 0x9b8
			 0x4568cdfb, /// 0x9bc
			 0xabf108b6, /// 0x9c0
			 0xe95567e5, /// 0x9c4
			 0x806047e5, /// 0x9c8
			 0x717cbc15, /// 0x9cc
			 0x9b1d270e, /// 0x9d0
			 0x8a51c153, /// 0x9d4
			 0xaf232fe5, /// 0x9d8
			 0x2b5617b4, /// 0x9dc
			 0xd6b25a45, /// 0x9e0
			 0x7900f54c, /// 0x9e4
			 0x7e0fba1c, /// 0x9e8
			 0x38c81a63, /// 0x9ec
			 0x994bc65b, /// 0x9f0
			 0x371176dc, /// 0x9f4
			 0xb22bc6bb, /// 0x9f8
			 0x08fcb04b, /// 0x9fc
			 0x1b80d796, /// 0xa00
			 0x7377c55e, /// 0xa04
			 0xcf9772e1, /// 0xa08
			 0xef82d140, /// 0xa0c
			 0x978e4ac6, /// 0xa10
			 0xf93b919d, /// 0xa14
			 0x527a7c97, /// 0xa18
			 0xe82d44f8, /// 0xa1c
			 0xebf9b324, /// 0xa20
			 0x3f5949cc, /// 0xa24
			 0x2d3c5b30, /// 0xa28
			 0xc60f9d6c, /// 0xa2c
			 0x246f9f42, /// 0xa30
			 0xb00ca021, /// 0xa34
			 0x77bff103, /// 0xa38
			 0x48c32703, /// 0xa3c
			 0xac1621db, /// 0xa40
			 0xb47cc1dd, /// 0xa44
			 0xe433a190, /// 0xa48
			 0xbcd2889b, /// 0xa4c
			 0x253e1765, /// 0xa50
			 0x3c4efd97, /// 0xa54
			 0x3aa448f9, /// 0xa58
			 0x7622f70f, /// 0xa5c
			 0x920edcba, /// 0xa60
			 0x6187f2de, /// 0xa64
			 0x83127b99, /// 0xa68
			 0x32f78132, /// 0xa6c
			 0x0e8cdbe9, /// 0xa70
			 0x2c798a2d, /// 0xa74
			 0x223caae9, /// 0xa78
			 0x7d540c17, /// 0xa7c
			 0xa3d033d9, /// 0xa80
			 0xafc28058, /// 0xa84
			 0x8c854dff, /// 0xa88
			 0x30231150, /// 0xa8c
			 0xdf7e5514, /// 0xa90
			 0x1e746e0f, /// 0xa94
			 0xb9ef3b8e, /// 0xa98
			 0xd8bbe30b, /// 0xa9c
			 0xdbcf47cf, /// 0xaa0
			 0x87b6b7da, /// 0xaa4
			 0x7385e756, /// 0xaa8
			 0xd951d69b, /// 0xaac
			 0x377f1db1, /// 0xab0
			 0x785a4acc, /// 0xab4
			 0x7affda6f, /// 0xab8
			 0x37fd92cc, /// 0xabc
			 0x7ff4e50d, /// 0xac0
			 0x37d5ed39, /// 0xac4
			 0xf9a3d9cb, /// 0xac8
			 0xff7d83d3, /// 0xacc
			 0xc4999ddd, /// 0xad0
			 0x036ec629, /// 0xad4
			 0xa4aa60b9, /// 0xad8
			 0x370bcece, /// 0xadc
			 0xc33ee33c, /// 0xae0
			 0xd30ea875, /// 0xae4
			 0x0cb300c5, /// 0xae8
			 0x4dacf5d3, /// 0xaec
			 0xc9c4ca52, /// 0xaf0
			 0x9e901bf7, /// 0xaf4
			 0xb82bc0cb, /// 0xaf8
			 0xa0116710, /// 0xafc
			 0x1eb32fbc, /// 0xb00
			 0xbe3894d3, /// 0xb04
			 0x64407eda, /// 0xb08
			 0x4003b0e2, /// 0xb0c
			 0x7a59f97c, /// 0xb10
			 0x77bb0214, /// 0xb14
			 0xb1b17724, /// 0xb18
			 0xcdb004d8, /// 0xb1c
			 0x66a8bf2b, /// 0xb20
			 0x1c28113a, /// 0xb24
			 0xcbffb9de, /// 0xb28
			 0x250cf99a, /// 0xb2c
			 0x24adefc8, /// 0xb30
			 0x30785d03, /// 0xb34
			 0xeac9b6f2, /// 0xb38
			 0xb81751a8, /// 0xb3c
			 0x1e4cea3b, /// 0xb40
			 0x20380d51, /// 0xb44
			 0xfb13e2bb, /// 0xb48
			 0x8101c93d, /// 0xb4c
			 0x941c2bdf, /// 0xb50
			 0x63d1af8e, /// 0xb54
			 0xd65d597e, /// 0xb58
			 0x8e0f1b43, /// 0xb5c
			 0x4c9abaee, /// 0xb60
			 0x150ae1b0, /// 0xb64
			 0x346127ea, /// 0xb68
			 0x6c718030, /// 0xb6c
			 0xe77d7a1d, /// 0xb70
			 0xa3826403, /// 0xb74
			 0xd273ddd8, /// 0xb78
			 0x4d5dd22e, /// 0xb7c
			 0xc4232a26, /// 0xb80
			 0x9529d1b2, /// 0xb84
			 0x75a379c5, /// 0xb88
			 0x5a15cbab, /// 0xb8c
			 0x1a79c833, /// 0xb90
			 0xf705a2da, /// 0xb94
			 0x1a9ba532, /// 0xb98
			 0x18aed52c, /// 0xb9c
			 0xf6bfd948, /// 0xba0
			 0x715648bc, /// 0xba4
			 0x4ba54a4d, /// 0xba8
			 0xc53d78c8, /// 0xbac
			 0x7bbfaa97, /// 0xbb0
			 0x8784856a, /// 0xbb4
			 0xf440b6ec, /// 0xbb8
			 0xd479060d, /// 0xbbc
			 0x68f573ac, /// 0xbc0
			 0x2b5cc2c0, /// 0xbc4
			 0x69a410fb, /// 0xbc8
			 0x66531dec, /// 0xbcc
			 0x8c02f328, /// 0xbd0
			 0xe7bc8eec, /// 0xbd4
			 0xecb3acd1, /// 0xbd8
			 0xf6b86853, /// 0xbdc
			 0x05b555a9, /// 0xbe0
			 0x057206cc, /// 0xbe4
			 0xf01a143e, /// 0xbe8
			 0x97200d56, /// 0xbec
			 0xc732356d, /// 0xbf0
			 0x87c14f1e, /// 0xbf4
			 0xf4ec5e2e, /// 0xbf8
			 0xc8dd7ca3, /// 0xbfc
			 0x9f64302b, /// 0xc00
			 0x3674f248, /// 0xc04
			 0x212f18e0, /// 0xc08
			 0xc64be7e5, /// 0xc0c
			 0x1b65a32e, /// 0xc10
			 0x3d6e3fbf, /// 0xc14
			 0x2d02a082, /// 0xc18
			 0x3c18768d, /// 0xc1c
			 0xa6f23d8b, /// 0xc20
			 0x09fa6ea1, /// 0xc24
			 0xc45da6d0, /// 0xc28
			 0x9bcb36fc, /// 0xc2c
			 0xfdf2be56, /// 0xc30
			 0x0f250f82, /// 0xc34
			 0x6fa21e05, /// 0xc38
			 0xd17f219f, /// 0xc3c
			 0x58c759f6, /// 0xc40
			 0xd2cb8f01, /// 0xc44
			 0xa6f476ec, /// 0xc48
			 0x6f15ecd5, /// 0xc4c
			 0x99d54f24, /// 0xc50
			 0xfb5da501, /// 0xc54
			 0x99a802bc, /// 0xc58
			 0xa76e2d77, /// 0xc5c
			 0x420fa239, /// 0xc60
			 0xfad2aac0, /// 0xc64
			 0x32a55562, /// 0xc68
			 0xe6a2edd8, /// 0xc6c
			 0xe4e6f750, /// 0xc70
			 0x95ff538f, /// 0xc74
			 0xfa7aec74, /// 0xc78
			 0xd155dfdc, /// 0xc7c
			 0x90d03c20, /// 0xc80
			 0x5703b702, /// 0xc84
			 0x8447cd7f, /// 0xc88
			 0x7cad9e79, /// 0xc8c
			 0xb14e63a6, /// 0xc90
			 0x900af635, /// 0xc94
			 0x6295a967, /// 0xc98
			 0x395b9df8, /// 0xc9c
			 0x669cea1e, /// 0xca0
			 0x2c44d871, /// 0xca4
			 0x3e739411, /// 0xca8
			 0x1610f646, /// 0xcac
			 0x10ed73fb, /// 0xcb0
			 0xe5b8e4aa, /// 0xcb4
			 0xbc8cca84, /// 0xcb8
			 0x76eb6d5a, /// 0xcbc
			 0x08609872, /// 0xcc0
			 0x77081a80, /// 0xcc4
			 0xcb962688, /// 0xcc8
			 0xada3141a, /// 0xccc
			 0x0b07fbf6, /// 0xcd0
			 0xf44b6262, /// 0xcd4
			 0xbbb62a43, /// 0xcd8
			 0x1ea6bede, /// 0xcdc
			 0xbcdb3da9, /// 0xce0
			 0x202b8c15, /// 0xce4
			 0x0282e3bd, /// 0xce8
			 0x7ff9b219, /// 0xcec
			 0xa61a80b8, /// 0xcf0
			 0x572fe774, /// 0xcf4
			 0x926aa4a1, /// 0xcf8
			 0x1a2381e7, /// 0xcfc
			 0xc592199a, /// 0xd00
			 0x8c58c5e9, /// 0xd04
			 0xf73f15c3, /// 0xd08
			 0xd26b3a38, /// 0xd0c
			 0xb3a39fa4, /// 0xd10
			 0xd4e44050, /// 0xd14
			 0x71f4877e, /// 0xd18
			 0x50fd77db, /// 0xd1c
			 0xb2fd94fa, /// 0xd20
			 0xa5c7f057, /// 0xd24
			 0x7d346902, /// 0xd28
			 0x72609bf2, /// 0xd2c
			 0x61d2734e, /// 0xd30
			 0x38ba4ccb, /// 0xd34
			 0xbf28c620, /// 0xd38
			 0xe6404f0f, /// 0xd3c
			 0x0a456e7a, /// 0xd40
			 0x87549422, /// 0xd44
			 0xa2916caf, /// 0xd48
			 0x5508c3ce, /// 0xd4c
			 0x97809140, /// 0xd50
			 0x63085ed7, /// 0xd54
			 0x48ffc68e, /// 0xd58
			 0xb7c7b801, /// 0xd5c
			 0x1c36c560, /// 0xd60
			 0x9d2ce7fa, /// 0xd64
			 0x3aa83684, /// 0xd68
			 0x6812d1d4, /// 0xd6c
			 0xd9187e2d, /// 0xd70
			 0xd55dc0ad, /// 0xd74
			 0xdd8fd733, /// 0xd78
			 0x12de235e, /// 0xd7c
			 0x0d3fb1e4, /// 0xd80
			 0x4e008bdc, /// 0xd84
			 0xd82cb3c6, /// 0xd88
			 0xe509f99c, /// 0xd8c
			 0x4ab26427, /// 0xd90
			 0x212763c4, /// 0xd94
			 0x006a48d5, /// 0xd98
			 0x765f2590, /// 0xd9c
			 0x7390182a, /// 0xda0
			 0x1066a4fe, /// 0xda4
			 0xcd422c2b, /// 0xda8
			 0x81a661a0, /// 0xdac
			 0x1517b991, /// 0xdb0
			 0x27a069b7, /// 0xdb4
			 0xeda4a7ec, /// 0xdb8
			 0xe8d5523a, /// 0xdbc
			 0xe1250689, /// 0xdc0
			 0x4df727fc, /// 0xdc4
			 0xacc77b8a, /// 0xdc8
			 0xde69d1af, /// 0xdcc
			 0xa93f7d15, /// 0xdd0
			 0xed96a72e, /// 0xdd4
			 0xc1e6fff4, /// 0xdd8
			 0xf2ef177a, /// 0xddc
			 0x6742896b, /// 0xde0
			 0x5e0f3d68, /// 0xde4
			 0x76ec9105, /// 0xde8
			 0x2126700f, /// 0xdec
			 0xa1c0140b, /// 0xdf0
			 0xa4f54304, /// 0xdf4
			 0xdc92d181, /// 0xdf8
			 0x807b6651, /// 0xdfc
			 0x3b604b5d, /// 0xe00
			 0xc3d73c1e, /// 0xe04
			 0x884e17c8, /// 0xe08
			 0x0ef7eb5d, /// 0xe0c
			 0xf7b891ee, /// 0xe10
			 0x67c9d9a9, /// 0xe14
			 0x7d71eba6, /// 0xe18
			 0x19c5b495, /// 0xe1c
			 0xe2054a2d, /// 0xe20
			 0x284ef376, /// 0xe24
			 0x6a885a15, /// 0xe28
			 0xfb960316, /// 0xe2c
			 0x0fd7b288, /// 0xe30
			 0x00ddc8e4, /// 0xe34
			 0x53f5ad71, /// 0xe38
			 0xb8bc15b7, /// 0xe3c
			 0x99bc94fc, /// 0xe40
			 0x9294d9a3, /// 0xe44
			 0xceb544ec, /// 0xe48
			 0x1a8fed9f, /// 0xe4c
			 0x68d69d41, /// 0xe50
			 0xf07e5525, /// 0xe54
			 0x59c25903, /// 0xe58
			 0x6018f2d3, /// 0xe5c
			 0x1e6a23f9, /// 0xe60
			 0x736d8a8b, /// 0xe64
			 0x9758733b, /// 0xe68
			 0xd2ed756c, /// 0xe6c
			 0x10aa1262, /// 0xe70
			 0x04843a58, /// 0xe74
			 0x40ce1605, /// 0xe78
			 0x3ba79ce0, /// 0xe7c
			 0xf22678ba, /// 0xe80
			 0x716db357, /// 0xe84
			 0x874b13bf, /// 0xe88
			 0x604d3a97, /// 0xe8c
			 0x35ca88f9, /// 0xe90
			 0x4beb5bd4, /// 0xe94
			 0x62485dd1, /// 0xe98
			 0x6968c4e7, /// 0xe9c
			 0x54fe1df9, /// 0xea0
			 0xfee6f81a, /// 0xea4
			 0x6fe79e87, /// 0xea8
			 0x41605579, /// 0xeac
			 0x4ebf83ab, /// 0xeb0
			 0x54b3b0e9, /// 0xeb4
			 0x107964f6, /// 0xeb8
			 0xbffc074f, /// 0xebc
			 0x7c744446, /// 0xec0
			 0x77259f9d, /// 0xec4
			 0xcf387997, /// 0xec8
			 0xcb342a6f, /// 0xecc
			 0x1c17daf9, /// 0xed0
			 0xd4f2a5da, /// 0xed4
			 0x52c96eed, /// 0xed8
			 0xefc470dd, /// 0xedc
			 0x27eb1651, /// 0xee0
			 0xa01bd614, /// 0xee4
			 0x22a0980d, /// 0xee8
			 0xfbf9033a, /// 0xeec
			 0xce0c40d1, /// 0xef0
			 0x1b098c20, /// 0xef4
			 0x9fc63678, /// 0xef8
			 0x9666c195, /// 0xefc
			 0x13e5f41d, /// 0xf00
			 0x10d1185d, /// 0xf04
			 0x6f474b07, /// 0xf08
			 0xfe0749f0, /// 0xf0c
			 0xba79068e, /// 0xf10
			 0x19114685, /// 0xf14
			 0x033b5012, /// 0xf18
			 0x2c7c4816, /// 0xf1c
			 0x6cdfcc38, /// 0xf20
			 0x9239c03f, /// 0xf24
			 0x5252eb71, /// 0xf28
			 0x3fd06514, /// 0xf2c
			 0x0a459b49, /// 0xf30
			 0x07f480a4, /// 0xf34
			 0xd09e615c, /// 0xf38
			 0x1dadfc02, /// 0xf3c
			 0xaab39916, /// 0xf40
			 0x2be28055, /// 0xf44
			 0x40f149d5, /// 0xf48
			 0x42a58273, /// 0xf4c
			 0x784a28e8, /// 0xf50
			 0x40c5ebd1, /// 0xf54
			 0xc36854b3, /// 0xf58
			 0xbe655fa5, /// 0xf5c
			 0x98d2e5ed, /// 0xf60
			 0xb151caee, /// 0xf64
			 0x4d0c06fb, /// 0xf68
			 0x10e2321b, /// 0xf6c
			 0x83d09ca1, /// 0xf70
			 0xfddcd3d3, /// 0xf74
			 0xb6433c5c, /// 0xf78
			 0xda0d7253, /// 0xf7c
			 0xe42ac427, /// 0xf80
			 0xed11fff2, /// 0xf84
			 0xb6f83114, /// 0xf88
			 0xa504c58e, /// 0xf8c
			 0x73ea9ff4, /// 0xf90
			 0x052f3d88, /// 0xf94
			 0xba21736c, /// 0xf98
			 0x1e776117, /// 0xf9c
			 0x556c179f, /// 0xfa0
			 0xc935c0ef, /// 0xfa4
			 0xa6b8a9a2, /// 0xfa8
			 0xa9f29961, /// 0xfac
			 0x38a8f953, /// 0xfb0
			 0xe0ac24fc, /// 0xfb4
			 0xf8a43a42, /// 0xfb8
			 0x7f8b3228, /// 0xfbc
			 0x120702d8, /// 0xfc0
			 0xc5096420, /// 0xfc4
			 0x3dc2e77b, /// 0xfc8
			 0x05798320, /// 0xfcc
			 0x35c555d6, /// 0xfd0
			 0xdceeaed3, /// 0xfd4
			 0x3f5e0f63, /// 0xfd8
			 0x02e9a205, /// 0xfdc
			 0x4cbb164a, /// 0xfe0
			 0xff104aee, /// 0xfe4
			 0x25fd6385, /// 0xfe8
			 0xc5a30270, /// 0xfec
			 0xabbd7fd2, /// 0xff0
			 0x76381d38, /// 0xff4
			 0x0ec72146, /// 0xff8
			 0x135fabe7 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x0e000007,                                                  // Trailing 1s:                                /// 00014
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00020
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00024
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00028
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00030
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00034
			 0x33333333,                                                  // 4 random values                             /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00040
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00048
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00050
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00054
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00058
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0005c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00064
			 0x0e000003,                                                  // Trailing 1s:                                /// 00068
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00070
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00074
			 0x0c400000,                                                  // Leading 1s:                                 /// 00078
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00080
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00084
			 0x0c600000,                                                  // Leading 1s:                                 /// 00088
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0008c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00090
			 0x80000000,                                                  // -zero                                       /// 00094
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00098
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xff800000,                                                  // -inf                                        /// 000a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000c8
			 0xff800000,                                                  // -inf                                        /// 000cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00100
			 0xcb000000,                                                  // -8388608.0                                  /// 00104
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0e000001,                                                  // Trailing 1s:                                /// 0010c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00118
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0011c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00124
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00128
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0012c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00130
			 0x0e000001,                                                  // Trailing 1s:                                /// 00134
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0013c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00140
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x33333333,                                                  // 4 random values                             /// 00148
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00154
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00158
			 0x0c400000,                                                  // Leading 1s:                                 /// 0015c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00164
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00168
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80000000,                                                  // -zero                                       /// 00170
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00174
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0017c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00184
			 0x55555555,                                                  // 4 random values                             /// 00188
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0018c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00194
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00198
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0019c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00200
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00208
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00210
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00214
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00220
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00224
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00228
			 0x00000000,                                                  // zero                                        /// 0022c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00230
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00234
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00248
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0024c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00250
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00254
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00258
			 0x33333333,                                                  // 4 random values                             /// 0025c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00260
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00264
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00268
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00278
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0027c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00284
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00288
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00294
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00298
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0xcb000000,                                                  // -8388608.0                                  /// 002c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002f8
			 0x7fc00001,                                                  // signaling NaN                               /// 002fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00300
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00304
			 0x0c700000,                                                  // Leading 1s:                                 /// 00308
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00310
			 0xbf028f5c,                                                  // -0.51                                       /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0031c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00320
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00328
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0032c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x0e000003,                                                  // Trailing 1s:                                /// 00334
			 0x33333333,                                                  // 4 random values                             /// 00338
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00340
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00344
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00348
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x33333333,                                                  // 4 random values                             /// 00350
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0xbf028f5c,                                                  // -0.51                                       /// 00358
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0035c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00360
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00364
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00368
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0036c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00374
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0037c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00380
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00388
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0038c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00394
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00398
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x7f800000,                                                  // inf                                         /// 003c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 003e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00400
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00404
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00408
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00414
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00418
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0042c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00430
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00434
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00438
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0043c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00440
			 0x33333333,                                                  // 4 random values                             /// 00444
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00448
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00458
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0045c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00460
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00464
			 0xff800000,                                                  // -inf                                        /// 00468
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0046c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00470
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00474
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0047c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00480
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00484
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00488
			 0x80000000,                                                  // -zero                                       /// 0048c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0c400000,                                                  // Leading 1s:                                 /// 00498
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0049c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004a4
			 0xcb000000,                                                  // -8388608.0                                  /// 004a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x3f028f5c,                                                  // 0.51                                        /// 004bc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x0e000001,                                                  // Trailing 1s:                                /// 00508
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00510
			 0x00000000,                                                  // zero                                        /// 00514
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c700000,                                                  // Leading 1s:                                 /// 0051c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0xff800000,                                                  // -inf                                        /// 0052c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00530
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00534
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00538
			 0x00000000,                                                  // zero                                        /// 0053c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00540
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0054c
			 0x80000000,                                                  // -zero                                       /// 00550
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00558
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0055c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00560
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00564
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00568
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0056c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x3f028f5c,                                                  // 0.51                                        /// 00574
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00578
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0057c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00580
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00584
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00590
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00594
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x7f800000,                                                  // inf                                         /// 005c0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0060c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00610
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00614
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00618
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x4b000000,                                                  // 8388608.0                                   /// 00620
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00628
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0062c
			 0x3f028f5c,                                                  // 0.51                                        /// 00630
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00638
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00648
			 0xbf028f5c,                                                  // -0.51                                       /// 0064c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00650
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00654
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00658
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0065c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00660
			 0x80000000,                                                  // -zero                                       /// 00664
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00668
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00670
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00678
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00688
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0068c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00694
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00698
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x80000000,                                                  // -zero                                       /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0xbf028f5c,                                                  // -0.51                                       /// 006a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c4
			 0x55555555,                                                  // 4 random values                             /// 006c8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x80000000,                                                  // -zero                                       /// 006e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00700
			 0xffc00001,                                                  // -signaling NaN                              /// 00704
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0070c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00714
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00720
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00724
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0072c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00730
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0073c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00740
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00748
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0074c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00750
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00754
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00760
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00764
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00768
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00770
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00774
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00778
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0077c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00784
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00788
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00790
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00794
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0079c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0xffc00001,                                                  // -signaling NaN                              /// 007e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007f0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00800
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00808
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00814
			 0x0e000007,                                                  // Trailing 1s:                                /// 00818
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00828
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0082c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00830
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00834
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0083c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00848
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0084c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00850
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00854
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00858
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0085c
			 0x80000000,                                                  // -zero                                       /// 00860
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00864
			 0x7fc00001,                                                  // signaling NaN                               /// 00868
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0086c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00870
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00878
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00880
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00884
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00888
			 0xbf028f5c,                                                  // -0.51                                       /// 0088c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00894
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00898
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x3f028f5c,                                                  // 0.51                                        /// 008cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00900
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00904
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00908
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0090c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00918
			 0x4b000000,                                                  // 8388608.0                                   /// 0091c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00920
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00924
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00928
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00930
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00934
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00938
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0093c
			 0x80000000,                                                  // -zero                                       /// 00940
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00944
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00948
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0094c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00950
			 0x55555555,                                                  // 4 random values                             /// 00954
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00958
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0095c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00960
			 0x00000000,                                                  // zero                                        /// 00964
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00968
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0096c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00970
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00974
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80000000,                                                  // -zero                                       /// 0097c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00980
			 0xbf028f5c,                                                  // -0.51                                       /// 00984
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00988
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00990
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00994
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0099c
			 0x0c700000,                                                  // Leading 1s:                                 /// 009a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009ac
			 0x4b000000,                                                  // 8388608.0                                   /// 009b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00000000,                                                  // zero                                        /// 009e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x3f028f5c,                                                  // 0.51                                        /// 00a04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a38
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a60
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a74
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a80
			 0x4b000000,                                                  // 8388608.0                                   /// 00a84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a8c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a90
			 0x33333333,                                                  // 4 random values                             /// 00a94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00aa0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aa4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00abc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00adc
			 0x00000000,                                                  // zero                                        /// 00ae0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ae4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00aec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00af0
			 0x33333333,                                                  // 4 random values                             /// 00af4
			 0xcb000000,                                                  // -8388608.0                                  /// 00af8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b14
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b1c
			 0x33333333,                                                  // 4 random values                             /// 00b20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b2c
			 0xff800000,                                                  // -inf                                        /// 00b30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b34
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b68
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b84
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b90
			 0xbf028f5c,                                                  // -0.51                                       /// 00b94
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ba0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bb4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bcc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bd4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00be4
			 0x4b000000,                                                  // 8388608.0                                   /// 00be8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf8
			 0xff800000,                                                  // -inf                                        /// 00bfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c00
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c08
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c34
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x80000000,                                                  // -zero                                       /// 00c48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c50
			 0x80000000,                                                  // -zero                                       /// 00c54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c58
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x55555555,                                                  // 4 random values                             /// 00c70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c80
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0xffc00001,                                                  // -signaling NaN                              /// 00c90
			 0x80000000,                                                  // -zero                                       /// 00c94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cb8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ccc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ce0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cf0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cf4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x3f028f5c,                                                  // 0.51                                        /// 00d18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d44
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d60
			 0x55555555,                                                  // 4 random values                             /// 00d64
			 0x3f028f5c,                                                  // 0.51                                        /// 00d68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d88
			 0xff800000,                                                  // -inf                                        /// 00d8c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x33333333,                                                  // 4 random values                             /// 00d94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00da0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0xbf028f5c,                                                  // -0.51                                       /// 00da8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00db0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db4
			 0x33333333,                                                  // 4 random values                             /// 00db8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00dc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00de4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00df8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dfc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e0c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e14
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e3c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e5c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ea4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ec4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ec8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ecc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ee4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ee8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ef0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f04
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f14
			 0x00000000,                                                  // zero                                        /// 00f18
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f40
			 0x7f800000,                                                  // inf                                         /// 00f44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f48
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f68
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f74
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f78
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0xcb000000,                                                  // -8388608.0                                  /// 00f84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f98
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fb4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fbc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fd8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fe8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fec
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00008000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x486c9adf, /// 0x0
			 0x72fdbb44, /// 0x4
			 0xabe1ad7a, /// 0x8
			 0x4288f304, /// 0xc
			 0xeb619b7e, /// 0x10
			 0xa0addf48, /// 0x14
			 0x7eb1ebe4, /// 0x18
			 0xc64b85bc, /// 0x1c
			 0xc5391306, /// 0x20
			 0xc6375bc5, /// 0x24
			 0xa7d17fc6, /// 0x28
			 0x7da132d7, /// 0x2c
			 0x686e738a, /// 0x30
			 0xbd73bf74, /// 0x34
			 0x16d1bc4c, /// 0x38
			 0x890545d2, /// 0x3c
			 0xf5c96c8e, /// 0x40
			 0x299b6a9b, /// 0x44
			 0x5e1167bd, /// 0x48
			 0xde63494a, /// 0x4c
			 0xdaf78967, /// 0x50
			 0x53e15a67, /// 0x54
			 0xdc6d3fb9, /// 0x58
			 0xc9dd7462, /// 0x5c
			 0x092c6238, /// 0x60
			 0xedef1c72, /// 0x64
			 0xaa8d91eb, /// 0x68
			 0xe345bbd7, /// 0x6c
			 0x7966718f, /// 0x70
			 0xae9ab23f, /// 0x74
			 0xab6f555a, /// 0x78
			 0xf4801822, /// 0x7c
			 0xc080105e, /// 0x80
			 0x8c8f9d82, /// 0x84
			 0x58a825d1, /// 0x88
			 0x438b05ad, /// 0x8c
			 0x34eef601, /// 0x90
			 0xfbcea129, /// 0x94
			 0x5af79f91, /// 0x98
			 0x5f6ea0c6, /// 0x9c
			 0x8e5c3a6c, /// 0xa0
			 0xe360b896, /// 0xa4
			 0x27fdca4a, /// 0xa8
			 0xaf35344b, /// 0xac
			 0xe3f8a101, /// 0xb0
			 0xdbf120d1, /// 0xb4
			 0x279b0b54, /// 0xb8
			 0xd21871e0, /// 0xbc
			 0x2fe15961, /// 0xc0
			 0x80ac0cfd, /// 0xc4
			 0x146f1a7d, /// 0xc8
			 0xc5e45f72, /// 0xcc
			 0x22cd6fa1, /// 0xd0
			 0x087c8367, /// 0xd4
			 0x5785ee14, /// 0xd8
			 0x9999c6a7, /// 0xdc
			 0x55ff399a, /// 0xe0
			 0xb9a8f33a, /// 0xe4
			 0xacaa13f8, /// 0xe8
			 0x8a8837ff, /// 0xec
			 0x04170c39, /// 0xf0
			 0x84113e16, /// 0xf4
			 0x4d2ffb55, /// 0xf8
			 0xc042bad6, /// 0xfc
			 0xe217e5d7, /// 0x100
			 0xf9eae68f, /// 0x104
			 0x66aa287c, /// 0x108
			 0x76231c7a, /// 0x10c
			 0x69547819, /// 0x110
			 0x07ffb868, /// 0x114
			 0x9dfaf986, /// 0x118
			 0x35546b67, /// 0x11c
			 0x53115e03, /// 0x120
			 0xae668660, /// 0x124
			 0xd50a4583, /// 0x128
			 0x83aaa7c8, /// 0x12c
			 0xc207ca76, /// 0x130
			 0x99539afc, /// 0x134
			 0xfb49b82b, /// 0x138
			 0xbbb52b66, /// 0x13c
			 0x2cf82af8, /// 0x140
			 0x39fd2747, /// 0x144
			 0x37a3b926, /// 0x148
			 0x9316ebd9, /// 0x14c
			 0x8f040934, /// 0x150
			 0x8fd5057e, /// 0x154
			 0xe7a5f7a0, /// 0x158
			 0x18287917, /// 0x15c
			 0xf28fb66a, /// 0x160
			 0x1b4f7af7, /// 0x164
			 0xee582a57, /// 0x168
			 0xc7f67c82, /// 0x16c
			 0xfce44216, /// 0x170
			 0x39a0b2e6, /// 0x174
			 0x3cca0170, /// 0x178
			 0x64af5f43, /// 0x17c
			 0x5ee9f40c, /// 0x180
			 0x9369d68f, /// 0x184
			 0xa393f3b2, /// 0x188
			 0x134494c4, /// 0x18c
			 0x17e87d69, /// 0x190
			 0x7d363fc7, /// 0x194
			 0xb4a06b57, /// 0x198
			 0x25d2baad, /// 0x19c
			 0x8c8f4c19, /// 0x1a0
			 0x6c089e2e, /// 0x1a4
			 0x84b1cd14, /// 0x1a8
			 0x13a226d9, /// 0x1ac
			 0x8faa4f3b, /// 0x1b0
			 0x1271959b, /// 0x1b4
			 0x2cb52b9d, /// 0x1b8
			 0xe161a5a9, /// 0x1bc
			 0x864c22d0, /// 0x1c0
			 0x30fe3d25, /// 0x1c4
			 0x3ece0d53, /// 0x1c8
			 0xb2d10ef4, /// 0x1cc
			 0x6244a602, /// 0x1d0
			 0x706549af, /// 0x1d4
			 0x2c543074, /// 0x1d8
			 0xd3575e3a, /// 0x1dc
			 0xcb7036b3, /// 0x1e0
			 0x125d044f, /// 0x1e4
			 0x69bb0424, /// 0x1e8
			 0x05b5dffe, /// 0x1ec
			 0x864c9e49, /// 0x1f0
			 0x9d3b3447, /// 0x1f4
			 0xe38f26eb, /// 0x1f8
			 0x845f3583, /// 0x1fc
			 0x2b53a70a, /// 0x200
			 0x3d34a326, /// 0x204
			 0xe4f3483e, /// 0x208
			 0x1c4b4c28, /// 0x20c
			 0x972d5866, /// 0x210
			 0x3dc79c2d, /// 0x214
			 0x1a0dca2f, /// 0x218
			 0x2f67cef3, /// 0x21c
			 0xa47fde36, /// 0x220
			 0xb686e3bc, /// 0x224
			 0xae07f1a6, /// 0x228
			 0xe46d4037, /// 0x22c
			 0x314ee1e3, /// 0x230
			 0x101facdb, /// 0x234
			 0x90110e37, /// 0x238
			 0xb2146686, /// 0x23c
			 0xaab2dd1a, /// 0x240
			 0x1724019a, /// 0x244
			 0x10e7be99, /// 0x248
			 0x619e480f, /// 0x24c
			 0x629316c9, /// 0x250
			 0x17600756, /// 0x254
			 0x11b14343, /// 0x258
			 0x41a8b913, /// 0x25c
			 0xd4818734, /// 0x260
			 0x31254bff, /// 0x264
			 0x669e61ec, /// 0x268
			 0xdd74e346, /// 0x26c
			 0x9430ef1b, /// 0x270
			 0x3f8ab27b, /// 0x274
			 0x88bc9565, /// 0x278
			 0xe37c8e2c, /// 0x27c
			 0xc055b4fb, /// 0x280
			 0x8f89b8cc, /// 0x284
			 0xcf93f606, /// 0x288
			 0xbeb88a5e, /// 0x28c
			 0x48c8754c, /// 0x290
			 0x233c3e7d, /// 0x294
			 0xd5f75d54, /// 0x298
			 0x9a979317, /// 0x29c
			 0xe106bd65, /// 0x2a0
			 0x32b3ba2b, /// 0x2a4
			 0x7ca412fc, /// 0x2a8
			 0x8fbd43f0, /// 0x2ac
			 0x2be18c28, /// 0x2b0
			 0x41d27094, /// 0x2b4
			 0x95402751, /// 0x2b8
			 0xa254985e, /// 0x2bc
			 0x9988f088, /// 0x2c0
			 0x06159320, /// 0x2c4
			 0x3af517bf, /// 0x2c8
			 0x8a18bf7d, /// 0x2cc
			 0x1c25b0c5, /// 0x2d0
			 0x13202d30, /// 0x2d4
			 0x7ae1e499, /// 0x2d8
			 0x26af36f0, /// 0x2dc
			 0x4134cd02, /// 0x2e0
			 0xd7927dff, /// 0x2e4
			 0xd0b71195, /// 0x2e8
			 0xb8c1d33f, /// 0x2ec
			 0xaf062cea, /// 0x2f0
			 0xce70b08a, /// 0x2f4
			 0xf68c957e, /// 0x2f8
			 0xd7e41486, /// 0x2fc
			 0xcced28c0, /// 0x300
			 0xe647536c, /// 0x304
			 0xcde93ade, /// 0x308
			 0x0706519d, /// 0x30c
			 0x2a9d09be, /// 0x310
			 0x085df58e, /// 0x314
			 0xf0ea3505, /// 0x318
			 0x74f97859, /// 0x31c
			 0x16510491, /// 0x320
			 0x8d15d4de, /// 0x324
			 0x32ede0cf, /// 0x328
			 0x9c67a506, /// 0x32c
			 0xdadff77e, /// 0x330
			 0xa46f64d5, /// 0x334
			 0xfe0e6c80, /// 0x338
			 0x0718eedb, /// 0x33c
			 0xda8b803f, /// 0x340
			 0x54563ba2, /// 0x344
			 0x5313e3d0, /// 0x348
			 0x55c5b1b6, /// 0x34c
			 0x0e710d55, /// 0x350
			 0xfaaf4e9b, /// 0x354
			 0x409b571e, /// 0x358
			 0xbd31f18c, /// 0x35c
			 0xe180783e, /// 0x360
			 0xef3415fa, /// 0x364
			 0x8696242f, /// 0x368
			 0x53aa7451, /// 0x36c
			 0xe573e18b, /// 0x370
			 0x980efa9a, /// 0x374
			 0x44b7e0de, /// 0x378
			 0xe4347f39, /// 0x37c
			 0x20e079fe, /// 0x380
			 0x403117c8, /// 0x384
			 0xbad9190e, /// 0x388
			 0x2d19468f, /// 0x38c
			 0xe952ff07, /// 0x390
			 0xa9830765, /// 0x394
			 0xe8479056, /// 0x398
			 0x313b0484, /// 0x39c
			 0xf2d1627a, /// 0x3a0
			 0xb0510f17, /// 0x3a4
			 0x214ff7d5, /// 0x3a8
			 0x1e933104, /// 0x3ac
			 0x7df955d7, /// 0x3b0
			 0xe1428c40, /// 0x3b4
			 0x389447e1, /// 0x3b8
			 0x026b6e70, /// 0x3bc
			 0x9de2baf0, /// 0x3c0
			 0x58d325fe, /// 0x3c4
			 0x9e7202b8, /// 0x3c8
			 0x058fa620, /// 0x3cc
			 0xcff2ce94, /// 0x3d0
			 0x48caedfd, /// 0x3d4
			 0x6246095b, /// 0x3d8
			 0x2da601e3, /// 0x3dc
			 0xa86f1a3c, /// 0x3e0
			 0xf52aa228, /// 0x3e4
			 0xb4418eed, /// 0x3e8
			 0x945757a6, /// 0x3ec
			 0xff4bf6e1, /// 0x3f0
			 0xb4bef651, /// 0x3f4
			 0xd0930cbc, /// 0x3f8
			 0xa24a23a7, /// 0x3fc
			 0x81db11fa, /// 0x400
			 0x1b0e35a2, /// 0x404
			 0x1fd72a4e, /// 0x408
			 0x1ab966b7, /// 0x40c
			 0x046b6f0b, /// 0x410
			 0x4e17d7e2, /// 0x414
			 0x2f8d03f0, /// 0x418
			 0x58fdf1a6, /// 0x41c
			 0x64dbc6ae, /// 0x420
			 0xd2ebd165, /// 0x424
			 0x22b44f07, /// 0x428
			 0x71505eb3, /// 0x42c
			 0x7893b892, /// 0x430
			 0xebcb1e46, /// 0x434
			 0x55caa47b, /// 0x438
			 0x6f6ed7a0, /// 0x43c
			 0xe51fd6a6, /// 0x440
			 0x31848b18, /// 0x444
			 0xc1d2da21, /// 0x448
			 0x1ff636f9, /// 0x44c
			 0x8f152939, /// 0x450
			 0x6cb5ee3f, /// 0x454
			 0xc6c6d559, /// 0x458
			 0xd19f7a1b, /// 0x45c
			 0x16ea7898, /// 0x460
			 0xe9615039, /// 0x464
			 0x05d806ae, /// 0x468
			 0xc3963f03, /// 0x46c
			 0xa0212480, /// 0x470
			 0xe3f76461, /// 0x474
			 0x14e40e74, /// 0x478
			 0x25aff2a6, /// 0x47c
			 0xa8b39653, /// 0x480
			 0xaba9ac42, /// 0x484
			 0xe794e492, /// 0x488
			 0x9c598b2f, /// 0x48c
			 0x4376d36e, /// 0x490
			 0x1ce846cb, /// 0x494
			 0x7b1bc5cd, /// 0x498
			 0x7c761463, /// 0x49c
			 0x510b795a, /// 0x4a0
			 0xa6367b4a, /// 0x4a4
			 0xde2d4954, /// 0x4a8
			 0x0c73452e, /// 0x4ac
			 0xde91823b, /// 0x4b0
			 0x037cd40c, /// 0x4b4
			 0x5abe491a, /// 0x4b8
			 0x9c60a534, /// 0x4bc
			 0x6a33dd3e, /// 0x4c0
			 0x5257e401, /// 0x4c4
			 0x324fa2d9, /// 0x4c8
			 0x5915b7e4, /// 0x4cc
			 0x01e35e41, /// 0x4d0
			 0x734e5d77, /// 0x4d4
			 0x9d15c709, /// 0x4d8
			 0xa2dc415b, /// 0x4dc
			 0x273eab47, /// 0x4e0
			 0x63754b81, /// 0x4e4
			 0x53728b54, /// 0x4e8
			 0x63ff5ee7, /// 0x4ec
			 0x7c2bbd66, /// 0x4f0
			 0xa5d3e3f3, /// 0x4f4
			 0x7b127671, /// 0x4f8
			 0x52cf1c3c, /// 0x4fc
			 0xc7c266ad, /// 0x500
			 0xbd16b78b, /// 0x504
			 0xfd7b2359, /// 0x508
			 0xb04aebc7, /// 0x50c
			 0x8d33c2e7, /// 0x510
			 0x68f0a805, /// 0x514
			 0x70300650, /// 0x518
			 0xe4d30a69, /// 0x51c
			 0xd011ecce, /// 0x520
			 0x9b5a31ac, /// 0x524
			 0x429d548b, /// 0x528
			 0xa08a3d5a, /// 0x52c
			 0x77ad6a96, /// 0x530
			 0xfd8778cd, /// 0x534
			 0xed407423, /// 0x538
			 0xd8d54230, /// 0x53c
			 0x9f7604d3, /// 0x540
			 0x8685a236, /// 0x544
			 0x82b6e009, /// 0x548
			 0xcd102c95, /// 0x54c
			 0x8ad3e2a6, /// 0x550
			 0xf118e048, /// 0x554
			 0x964cc0d2, /// 0x558
			 0x5ff040df, /// 0x55c
			 0x0f007a1b, /// 0x560
			 0x76a53d95, /// 0x564
			 0x61ba2be2, /// 0x568
			 0x56d310e3, /// 0x56c
			 0x13987d53, /// 0x570
			 0xf330883d, /// 0x574
			 0x72c147c6, /// 0x578
			 0x8d29fd39, /// 0x57c
			 0x43a28b02, /// 0x580
			 0xb43333a4, /// 0x584
			 0x1e9e06e5, /// 0x588
			 0xb34e99a6, /// 0x58c
			 0xeb4ec6b9, /// 0x590
			 0xfbe1cfa7, /// 0x594
			 0x1a887fd7, /// 0x598
			 0xc4e2c31a, /// 0x59c
			 0x5d044a6b, /// 0x5a0
			 0x805c65a3, /// 0x5a4
			 0x8c163264, /// 0x5a8
			 0xdd14037f, /// 0x5ac
			 0x1a798c35, /// 0x5b0
			 0x444c7695, /// 0x5b4
			 0x5f6f095f, /// 0x5b8
			 0xc9f9715c, /// 0x5bc
			 0x8d350be0, /// 0x5c0
			 0x60543ec2, /// 0x5c4
			 0x9ac249da, /// 0x5c8
			 0xa67fb046, /// 0x5cc
			 0x51a21f8d, /// 0x5d0
			 0x1534abb0, /// 0x5d4
			 0x41685ff7, /// 0x5d8
			 0x5cc84499, /// 0x5dc
			 0x6f99b6a9, /// 0x5e0
			 0xb89038b4, /// 0x5e4
			 0x66e7c3ae, /// 0x5e8
			 0x0cbf0bfc, /// 0x5ec
			 0xc1ce7089, /// 0x5f0
			 0x4c9d36b4, /// 0x5f4
			 0xbd18951b, /// 0x5f8
			 0x64f09ecb, /// 0x5fc
			 0x3b0ac177, /// 0x600
			 0xa6cb04ee, /// 0x604
			 0x5e7a02fd, /// 0x608
			 0x90d3c78a, /// 0x60c
			 0x1cafbb76, /// 0x610
			 0x7fea251c, /// 0x614
			 0x0c17b6f7, /// 0x618
			 0xe470a228, /// 0x61c
			 0xd15f9feb, /// 0x620
			 0x8247c11e, /// 0x624
			 0xf5ea131f, /// 0x628
			 0x60639017, /// 0x62c
			 0xd9442f36, /// 0x630
			 0xa99228cd, /// 0x634
			 0x9e338805, /// 0x638
			 0xe27e3b2b, /// 0x63c
			 0x5f0cdb9e, /// 0x640
			 0x66370b7a, /// 0x644
			 0xcd8f154a, /// 0x648
			 0xdfe8e74a, /// 0x64c
			 0xbb45857c, /// 0x650
			 0x153bb591, /// 0x654
			 0x8570624d, /// 0x658
			 0x175bcf6b, /// 0x65c
			 0xc81ac027, /// 0x660
			 0x8ee8f50c, /// 0x664
			 0xee20fb51, /// 0x668
			 0x894ba437, /// 0x66c
			 0xebcb0f19, /// 0x670
			 0x74c56802, /// 0x674
			 0x3cece544, /// 0x678
			 0x21958162, /// 0x67c
			 0x324f779a, /// 0x680
			 0x8852f4ac, /// 0x684
			 0x834802d3, /// 0x688
			 0xc59b1360, /// 0x68c
			 0x2bc43d80, /// 0x690
			 0x8c7d19da, /// 0x694
			 0x3e868f73, /// 0x698
			 0x48646529, /// 0x69c
			 0x47c282ec, /// 0x6a0
			 0x1bd815cf, /// 0x6a4
			 0x58da9d77, /// 0x6a8
			 0xf24060ea, /// 0x6ac
			 0xa34fb69e, /// 0x6b0
			 0x7b9cd898, /// 0x6b4
			 0x7ac85856, /// 0x6b8
			 0xd3f806e8, /// 0x6bc
			 0x61c8969c, /// 0x6c0
			 0xde1526d1, /// 0x6c4
			 0x6be148e9, /// 0x6c8
			 0xaaad911c, /// 0x6cc
			 0xaba6de33, /// 0x6d0
			 0x36b0f3a8, /// 0x6d4
			 0x89384290, /// 0x6d8
			 0x89f35f8c, /// 0x6dc
			 0xcf266c37, /// 0x6e0
			 0xd691ddf9, /// 0x6e4
			 0x68c95c63, /// 0x6e8
			 0x06be30c5, /// 0x6ec
			 0x7c2024e1, /// 0x6f0
			 0x3d507d85, /// 0x6f4
			 0xc03875ca, /// 0x6f8
			 0x3bbd348f, /// 0x6fc
			 0xd72c7f4a, /// 0x700
			 0x518c7c09, /// 0x704
			 0x3ece5400, /// 0x708
			 0x5c03c46c, /// 0x70c
			 0x8348f853, /// 0x710
			 0x580aa84f, /// 0x714
			 0xd82916a3, /// 0x718
			 0xe5fd5d29, /// 0x71c
			 0x6723aabe, /// 0x720
			 0xe317d176, /// 0x724
			 0xc43e65e4, /// 0x728
			 0x69527c25, /// 0x72c
			 0xb3a0071a, /// 0x730
			 0x984b7a4b, /// 0x734
			 0xf360c13b, /// 0x738
			 0x61cb7fbf, /// 0x73c
			 0x041f5a88, /// 0x740
			 0xd05c8d3c, /// 0x744
			 0xe5d3612a, /// 0x748
			 0x7492494b, /// 0x74c
			 0xa6338077, /// 0x750
			 0x397bf293, /// 0x754
			 0xcecb96e9, /// 0x758
			 0xf947188b, /// 0x75c
			 0x4effeaa4, /// 0x760
			 0xa157d5a4, /// 0x764
			 0x1406508d, /// 0x768
			 0xdc9e3a50, /// 0x76c
			 0x6ce1121e, /// 0x770
			 0x54faa5d4, /// 0x774
			 0x81580d80, /// 0x778
			 0xa32a0331, /// 0x77c
			 0xab1795cf, /// 0x780
			 0xd433ea89, /// 0x784
			 0xa3a9a75a, /// 0x788
			 0xdb0b5164, /// 0x78c
			 0x853e4a3d, /// 0x790
			 0xbfefcdd1, /// 0x794
			 0x0fe7cc39, /// 0x798
			 0x54ebd6fb, /// 0x79c
			 0x2c3b139e, /// 0x7a0
			 0x177d92df, /// 0x7a4
			 0x64ebbac9, /// 0x7a8
			 0xb159012b, /// 0x7ac
			 0x982887f4, /// 0x7b0
			 0x8845cbd3, /// 0x7b4
			 0xb993168d, /// 0x7b8
			 0xb73e30a8, /// 0x7bc
			 0x7b742ddc, /// 0x7c0
			 0xb97fc704, /// 0x7c4
			 0x49dc96f9, /// 0x7c8
			 0xc345b0e0, /// 0x7cc
			 0xba724672, /// 0x7d0
			 0x6d2b42b8, /// 0x7d4
			 0xbcf245eb, /// 0x7d8
			 0xf506f6c2, /// 0x7dc
			 0xd8808e5e, /// 0x7e0
			 0x7b631cef, /// 0x7e4
			 0xf447efa4, /// 0x7e8
			 0x83d9c6b9, /// 0x7ec
			 0xdf2e7d67, /// 0x7f0
			 0x5aae448a, /// 0x7f4
			 0x71ac2819, /// 0x7f8
			 0xff3d2ddb, /// 0x7fc
			 0xec849270, /// 0x800
			 0x3739bbe2, /// 0x804
			 0xe6999e4a, /// 0x808
			 0x8052c41b, /// 0x80c
			 0xd3fdc218, /// 0x810
			 0x4e2bd68f, /// 0x814
			 0x2ab8b5bc, /// 0x818
			 0xad9a4e43, /// 0x81c
			 0x0f5f5a1b, /// 0x820
			 0x5805c238, /// 0x824
			 0x58ddb222, /// 0x828
			 0xdee2c401, /// 0x82c
			 0x31cfd1ef, /// 0x830
			 0x003cc250, /// 0x834
			 0x88665c3d, /// 0x838
			 0x3e16ec95, /// 0x83c
			 0xa6345e18, /// 0x840
			 0xf109f160, /// 0x844
			 0x0cfed3b9, /// 0x848
			 0x8679fe14, /// 0x84c
			 0x2576fda8, /// 0x850
			 0x8e6b2590, /// 0x854
			 0x1b13b10f, /// 0x858
			 0x78650b26, /// 0x85c
			 0x3f3cca3d, /// 0x860
			 0x81b05bd1, /// 0x864
			 0x46776ba8, /// 0x868
			 0x7959a697, /// 0x86c
			 0x6a051d0a, /// 0x870
			 0x99445e48, /// 0x874
			 0x50265882, /// 0x878
			 0xc5e02095, /// 0x87c
			 0x5bad4090, /// 0x880
			 0xa7d006fa, /// 0x884
			 0x98dc64ef, /// 0x888
			 0x96463eef, /// 0x88c
			 0x1104182c, /// 0x890
			 0x85b6327f, /// 0x894
			 0x9b71ad6f, /// 0x898
			 0x16106c45, /// 0x89c
			 0x2e2ee994, /// 0x8a0
			 0x7bd87945, /// 0x8a4
			 0x27dc0f15, /// 0x8a8
			 0x778f636f, /// 0x8ac
			 0x196fc25e, /// 0x8b0
			 0xc2a094a9, /// 0x8b4
			 0xa9cbb2f5, /// 0x8b8
			 0xe96e0a8e, /// 0x8bc
			 0x06a39544, /// 0x8c0
			 0x3ca83247, /// 0x8c4
			 0x269c59a0, /// 0x8c8
			 0xe19cee9d, /// 0x8cc
			 0x3460ca0c, /// 0x8d0
			 0x9e8eaa49, /// 0x8d4
			 0xcebec15f, /// 0x8d8
			 0xff2c1a28, /// 0x8dc
			 0xc831e733, /// 0x8e0
			 0x549812e7, /// 0x8e4
			 0xfd357800, /// 0x8e8
			 0x5e3220b3, /// 0x8ec
			 0xa2f5c8db, /// 0x8f0
			 0xd658cbed, /// 0x8f4
			 0xed130241, /// 0x8f8
			 0x12aa55ed, /// 0x8fc
			 0xab76d80f, /// 0x900
			 0xee03a4b5, /// 0x904
			 0x8a7b30b0, /// 0x908
			 0x02212eef, /// 0x90c
			 0x89b92c9d, /// 0x910
			 0xb9ef8490, /// 0x914
			 0xf8c9275f, /// 0x918
			 0x4e861951, /// 0x91c
			 0xe48cae61, /// 0x920
			 0xc6cd5f4b, /// 0x924
			 0x960d3bd1, /// 0x928
			 0x5168e587, /// 0x92c
			 0x00f348a5, /// 0x930
			 0x119c3974, /// 0x934
			 0x63adaae1, /// 0x938
			 0xff4e2eb1, /// 0x93c
			 0x0db7807b, /// 0x940
			 0x773f3762, /// 0x944
			 0xf1e71503, /// 0x948
			 0x3a086feb, /// 0x94c
			 0x16415b27, /// 0x950
			 0x5c59672c, /// 0x954
			 0x5d9b9b80, /// 0x958
			 0x726adeca, /// 0x95c
			 0xed2e52f7, /// 0x960
			 0x8dce9f92, /// 0x964
			 0xb8a12ae5, /// 0x968
			 0xc7a88539, /// 0x96c
			 0x6ba1a790, /// 0x970
			 0xb0712b76, /// 0x974
			 0x6431ce4d, /// 0x978
			 0xf1e7562b, /// 0x97c
			 0x93d8d0a0, /// 0x980
			 0xdf1211c8, /// 0x984
			 0x38f4f243, /// 0x988
			 0x827791d7, /// 0x98c
			 0x341bb676, /// 0x990
			 0xa5497e9d, /// 0x994
			 0x3b17100e, /// 0x998
			 0x025817dc, /// 0x99c
			 0x4db65270, /// 0x9a0
			 0xd4f3d351, /// 0x9a4
			 0x2bf00dbd, /// 0x9a8
			 0x4be9269a, /// 0x9ac
			 0x35ff331c, /// 0x9b0
			 0xce490b7e, /// 0x9b4
			 0x60cc86d3, /// 0x9b8
			 0xa1532c4b, /// 0x9bc
			 0x9c0823ff, /// 0x9c0
			 0x6cd82363, /// 0x9c4
			 0x15aecd0e, /// 0x9c8
			 0xce1b1d42, /// 0x9cc
			 0x307db36c, /// 0x9d0
			 0x0b315c9e, /// 0x9d4
			 0x7a84b20e, /// 0x9d8
			 0xe827c23e, /// 0x9dc
			 0x24e64fa6, /// 0x9e0
			 0xa71b92d5, /// 0x9e4
			 0xacd1353f, /// 0x9e8
			 0x40af53e2, /// 0x9ec
			 0x84c6c6e4, /// 0x9f0
			 0x72faf91c, /// 0x9f4
			 0xc732af79, /// 0x9f8
			 0x6e67787e, /// 0x9fc
			 0x9ecec739, /// 0xa00
			 0xb663eb30, /// 0xa04
			 0x0d07689d, /// 0xa08
			 0x8a992aef, /// 0xa0c
			 0xed3596e3, /// 0xa10
			 0xc4a01a07, /// 0xa14
			 0xd481b46f, /// 0xa18
			 0x346e332c, /// 0xa1c
			 0xb14dd527, /// 0xa20
			 0x7e20ef3f, /// 0xa24
			 0xb94c6f9f, /// 0xa28
			 0x40029f31, /// 0xa2c
			 0x43c0475f, /// 0xa30
			 0xde6d395b, /// 0xa34
			 0xe2b06db4, /// 0xa38
			 0x8601cac0, /// 0xa3c
			 0xd59eea2c, /// 0xa40
			 0x11300f97, /// 0xa44
			 0xddb546f7, /// 0xa48
			 0x67ffa4d8, /// 0xa4c
			 0xfb2d2408, /// 0xa50
			 0xcad1cfd8, /// 0xa54
			 0xde89a7cf, /// 0xa58
			 0x7c0eab5a, /// 0xa5c
			 0xf25e7097, /// 0xa60
			 0x38c71096, /// 0xa64
			 0x6cf6efc7, /// 0xa68
			 0x6c91fe31, /// 0xa6c
			 0x831c720a, /// 0xa70
			 0x6485084a, /// 0xa74
			 0xfb15b2e7, /// 0xa78
			 0xa3174fb3, /// 0xa7c
			 0x9e6fbe6f, /// 0xa80
			 0x23710166, /// 0xa84
			 0xd9933cf2, /// 0xa88
			 0x4656821b, /// 0xa8c
			 0x3dd0c069, /// 0xa90
			 0xd8b07492, /// 0xa94
			 0xc7a6d8b0, /// 0xa98
			 0x21598338, /// 0xa9c
			 0xc3fd8791, /// 0xaa0
			 0x5d868a7d, /// 0xaa4
			 0xe19aedd6, /// 0xaa8
			 0x40fda6b9, /// 0xaac
			 0xa9b6a81b, /// 0xab0
			 0xcffd36f0, /// 0xab4
			 0xd52d9e4a, /// 0xab8
			 0x8f6f9eeb, /// 0xabc
			 0xdfa123e2, /// 0xac0
			 0x939eefb6, /// 0xac4
			 0x0805a408, /// 0xac8
			 0x57a9f37f, /// 0xacc
			 0x23a80aaf, /// 0xad0
			 0x11797ff7, /// 0xad4
			 0x7b854007, /// 0xad8
			 0xcca1c16a, /// 0xadc
			 0xdb64adb5, /// 0xae0
			 0x29f413b4, /// 0xae4
			 0xc191ce2c, /// 0xae8
			 0x1578d9f3, /// 0xaec
			 0x1904f37b, /// 0xaf0
			 0x96a1ec33, /// 0xaf4
			 0xc9fa94ae, /// 0xaf8
			 0x11c45834, /// 0xafc
			 0xd5af7225, /// 0xb00
			 0x799e731f, /// 0xb04
			 0xbc9b79d2, /// 0xb08
			 0x0def1a28, /// 0xb0c
			 0x71478550, /// 0xb10
			 0x5ee2b812, /// 0xb14
			 0x827221f6, /// 0xb18
			 0x67bb4902, /// 0xb1c
			 0xc8e7f51b, /// 0xb20
			 0x6f71858d, /// 0xb24
			 0x6d05c517, /// 0xb28
			 0x3983c4b8, /// 0xb2c
			 0xf7a22d72, /// 0xb30
			 0x1c456fbb, /// 0xb34
			 0x98849c30, /// 0xb38
			 0x750dc9eb, /// 0xb3c
			 0xaf48b8a9, /// 0xb40
			 0x061f4397, /// 0xb44
			 0x8c1c4cbd, /// 0xb48
			 0x66c9910c, /// 0xb4c
			 0xc6475717, /// 0xb50
			 0x9d1802d5, /// 0xb54
			 0xf3e812d0, /// 0xb58
			 0x5387dc1b, /// 0xb5c
			 0xd91db0c9, /// 0xb60
			 0xc98d1fe7, /// 0xb64
			 0xa522b805, /// 0xb68
			 0x93963e7a, /// 0xb6c
			 0xe9236b42, /// 0xb70
			 0xfb661b31, /// 0xb74
			 0x31c85637, /// 0xb78
			 0x7701fc64, /// 0xb7c
			 0xdf9af84f, /// 0xb80
			 0x99858789, /// 0xb84
			 0x01cfe45d, /// 0xb88
			 0xcdbaea81, /// 0xb8c
			 0xa47fccf5, /// 0xb90
			 0x89447dc3, /// 0xb94
			 0x6d0b9a0c, /// 0xb98
			 0xdcaaf337, /// 0xb9c
			 0xfdc3b954, /// 0xba0
			 0x1efa157d, /// 0xba4
			 0x1f7de26a, /// 0xba8
			 0x599ec56c, /// 0xbac
			 0xb2123f91, /// 0xbb0
			 0x1c112f9b, /// 0xbb4
			 0x0bbbc3f3, /// 0xbb8
			 0x051b62d7, /// 0xbbc
			 0x0182da90, /// 0xbc0
			 0x1f3d1e85, /// 0xbc4
			 0x3065aa78, /// 0xbc8
			 0xd406de9f, /// 0xbcc
			 0x9e37ca39, /// 0xbd0
			 0xec0937d2, /// 0xbd4
			 0x7f605b32, /// 0xbd8
			 0xa1bc5198, /// 0xbdc
			 0xb5abb69c, /// 0xbe0
			 0x02c857a2, /// 0xbe4
			 0xad0de5e4, /// 0xbe8
			 0x3830c2e8, /// 0xbec
			 0xf9c0956e, /// 0xbf0
			 0x987e87ff, /// 0xbf4
			 0xc5e9e02d, /// 0xbf8
			 0x76d00a54, /// 0xbfc
			 0xea661572, /// 0xc00
			 0x3960ec5e, /// 0xc04
			 0x4b3ee0d9, /// 0xc08
			 0x68f10168, /// 0xc0c
			 0x59aef044, /// 0xc10
			 0x2b353ea7, /// 0xc14
			 0x22a8e2b5, /// 0xc18
			 0x7085660d, /// 0xc1c
			 0xaf1fa8dc, /// 0xc20
			 0x05f192a6, /// 0xc24
			 0xf3eb7db2, /// 0xc28
			 0x782435bc, /// 0xc2c
			 0xf8804ec5, /// 0xc30
			 0x7bac7416, /// 0xc34
			 0x288c5ca1, /// 0xc38
			 0xa9d12ee1, /// 0xc3c
			 0x948731c5, /// 0xc40
			 0x0407a7ef, /// 0xc44
			 0xac6340c2, /// 0xc48
			 0xae5cd600, /// 0xc4c
			 0x06d51af3, /// 0xc50
			 0xd9e809ec, /// 0xc54
			 0x9a8da4d0, /// 0xc58
			 0xbbea813b, /// 0xc5c
			 0x502b4784, /// 0xc60
			 0xbdfb3cb4, /// 0xc64
			 0x3153f883, /// 0xc68
			 0xddedc549, /// 0xc6c
			 0x4f8cb704, /// 0xc70
			 0xb747616e, /// 0xc74
			 0x17b4fd08, /// 0xc78
			 0x7593463c, /// 0xc7c
			 0xb56e31ff, /// 0xc80
			 0xeb463629, /// 0xc84
			 0xa616d7f7, /// 0xc88
			 0xf306d73f, /// 0xc8c
			 0xf03fe533, /// 0xc90
			 0x2488d205, /// 0xc94
			 0x4eb4e9b9, /// 0xc98
			 0x89d53b88, /// 0xc9c
			 0x21c05de7, /// 0xca0
			 0x9ec7f9c8, /// 0xca4
			 0x89c3f9ec, /// 0xca8
			 0xe8deb510, /// 0xcac
			 0x69d5173c, /// 0xcb0
			 0xfb39fe0f, /// 0xcb4
			 0xa9f35c65, /// 0xcb8
			 0x9cd6a2e5, /// 0xcbc
			 0x34a955bf, /// 0xcc0
			 0x71181d50, /// 0xcc4
			 0x4f0bb08f, /// 0xcc8
			 0x362bfe09, /// 0xccc
			 0x3425157e, /// 0xcd0
			 0x8cc812d0, /// 0xcd4
			 0xdb3793a3, /// 0xcd8
			 0x8c603f37, /// 0xcdc
			 0x0d5ede97, /// 0xce0
			 0x62d67369, /// 0xce4
			 0xbf4536f9, /// 0xce8
			 0x53fcacf5, /// 0xcec
			 0x2622b3a4, /// 0xcf0
			 0x3c51e934, /// 0xcf4
			 0xd0a304aa, /// 0xcf8
			 0x17f79f23, /// 0xcfc
			 0xa7e51864, /// 0xd00
			 0x51e2530f, /// 0xd04
			 0x6838391a, /// 0xd08
			 0x1d4719e1, /// 0xd0c
			 0x51e5c9d0, /// 0xd10
			 0xe57da4b2, /// 0xd14
			 0xa36ff45d, /// 0xd18
			 0x1c6e70b1, /// 0xd1c
			 0x166bd23d, /// 0xd20
			 0x7b3a6559, /// 0xd24
			 0x11ab82a4, /// 0xd28
			 0x13d71487, /// 0xd2c
			 0xe927e19f, /// 0xd30
			 0xb179fb08, /// 0xd34
			 0xee4c24d5, /// 0xd38
			 0x5c569650, /// 0xd3c
			 0x1de2c633, /// 0xd40
			 0xb78b26a6, /// 0xd44
			 0x715dba72, /// 0xd48
			 0x0d1e3e82, /// 0xd4c
			 0xc33a2440, /// 0xd50
			 0x8ad20318, /// 0xd54
			 0x59a804be, /// 0xd58
			 0x008f157c, /// 0xd5c
			 0xe95813db, /// 0xd60
			 0x142cb501, /// 0xd64
			 0x293c043d, /// 0xd68
			 0x84c7e051, /// 0xd6c
			 0x1ebc3698, /// 0xd70
			 0x6ede0fd3, /// 0xd74
			 0x6445c907, /// 0xd78
			 0x9812e72f, /// 0xd7c
			 0xa1347400, /// 0xd80
			 0xb468b03c, /// 0xd84
			 0xd0566843, /// 0xd88
			 0x475ca2fc, /// 0xd8c
			 0x06ea2d04, /// 0xd90
			 0xf3feb8b2, /// 0xd94
			 0x31c49bed, /// 0xd98
			 0x573954de, /// 0xd9c
			 0x3d93e516, /// 0xda0
			 0x8b86a9a1, /// 0xda4
			 0x6301ba0a, /// 0xda8
			 0x0078ac8e, /// 0xdac
			 0x23480f56, /// 0xdb0
			 0x54698684, /// 0xdb4
			 0xce129a6e, /// 0xdb8
			 0xb473c915, /// 0xdbc
			 0x4157cea9, /// 0xdc0
			 0x65514fd0, /// 0xdc4
			 0xc72ca084, /// 0xdc8
			 0xd5b4f0ea, /// 0xdcc
			 0x4ce2f728, /// 0xdd0
			 0x070f274e, /// 0xdd4
			 0x3b483faa, /// 0xdd8
			 0xb1e33832, /// 0xddc
			 0x6818a08d, /// 0xde0
			 0xc8ea7efb, /// 0xde4
			 0x5606b67c, /// 0xde8
			 0x560f4746, /// 0xdec
			 0xbebf7324, /// 0xdf0
			 0x41ed0025, /// 0xdf4
			 0xb1b4b4ea, /// 0xdf8
			 0x8be56f67, /// 0xdfc
			 0x7fc5d203, /// 0xe00
			 0x203b008c, /// 0xe04
			 0x6879f2d1, /// 0xe08
			 0x2a8376ab, /// 0xe0c
			 0xa913deff, /// 0xe10
			 0x69845b8e, /// 0xe14
			 0x688f2071, /// 0xe18
			 0x8de724f8, /// 0xe1c
			 0x8d017145, /// 0xe20
			 0x35d4e2b6, /// 0xe24
			 0xbfe55ec9, /// 0xe28
			 0x1c0303e5, /// 0xe2c
			 0x9063e0eb, /// 0xe30
			 0x483e6391, /// 0xe34
			 0xe93a6559, /// 0xe38
			 0x39404c63, /// 0xe3c
			 0xb2e9cb1a, /// 0xe40
			 0x2156db5e, /// 0xe44
			 0x7d888fc8, /// 0xe48
			 0x08221966, /// 0xe4c
			 0x5e9c7cd6, /// 0xe50
			 0xc11bffef, /// 0xe54
			 0x70363059, /// 0xe58
			 0x1e9ace32, /// 0xe5c
			 0x04a29052, /// 0xe60
			 0x435ea5c4, /// 0xe64
			 0x4496dc98, /// 0xe68
			 0xe2add8e7, /// 0xe6c
			 0xcc02ca14, /// 0xe70
			 0xbc8ebdd9, /// 0xe74
			 0xb99f155e, /// 0xe78
			 0x446f4c33, /// 0xe7c
			 0x4dcd4f06, /// 0xe80
			 0x0e0a0cb0, /// 0xe84
			 0x64ef6751, /// 0xe88
			 0x4fa702fe, /// 0xe8c
			 0x07230b1a, /// 0xe90
			 0x94afa1b4, /// 0xe94
			 0x3af03ac9, /// 0xe98
			 0xe5a234bf, /// 0xe9c
			 0xd02d1f46, /// 0xea0
			 0x7760b81b, /// 0xea4
			 0x0862989e, /// 0xea8
			 0x22a4ebf8, /// 0xeac
			 0x64558b84, /// 0xeb0
			 0xd807b163, /// 0xeb4
			 0x1e05b931, /// 0xeb8
			 0x6cbce5aa, /// 0xebc
			 0x8d45cea6, /// 0xec0
			 0xa22f6cb1, /// 0xec4
			 0xdb1d1599, /// 0xec8
			 0x0cb33efe, /// 0xecc
			 0xb8236c22, /// 0xed0
			 0x10010f52, /// 0xed4
			 0x9e6a71bb, /// 0xed8
			 0x4741df4b, /// 0xedc
			 0x5c2e5f4a, /// 0xee0
			 0x064accb3, /// 0xee4
			 0xd1ce87ad, /// 0xee8
			 0xd9a1e4a7, /// 0xeec
			 0x199e73f4, /// 0xef0
			 0xeb11145a, /// 0xef4
			 0x9b8acfe5, /// 0xef8
			 0x5b91f594, /// 0xefc
			 0x279d02e3, /// 0xf00
			 0xed61ddc2, /// 0xf04
			 0xa158636e, /// 0xf08
			 0x719c6436, /// 0xf0c
			 0x62f53f13, /// 0xf10
			 0x2a33a779, /// 0xf14
			 0x97a5d947, /// 0xf18
			 0xb81f2096, /// 0xf1c
			 0xd14a147e, /// 0xf20
			 0xbf57b488, /// 0xf24
			 0x63ef2a5e, /// 0xf28
			 0x3f0e29cb, /// 0xf2c
			 0x97855a30, /// 0xf30
			 0x6c6b750c, /// 0xf34
			 0x427f5138, /// 0xf38
			 0xc6c55434, /// 0xf3c
			 0x7c22521c, /// 0xf40
			 0x62ad426c, /// 0xf44
			 0xc85ad8db, /// 0xf48
			 0xd69726b5, /// 0xf4c
			 0xf0a62958, /// 0xf50
			 0xc6df6f4a, /// 0xf54
			 0xd22c88e7, /// 0xf58
			 0x71d70240, /// 0xf5c
			 0x9ace36f7, /// 0xf60
			 0xf13ec20d, /// 0xf64
			 0xc17c50fd, /// 0xf68
			 0xa600338e, /// 0xf6c
			 0xf93effcf, /// 0xf70
			 0x449dc2f9, /// 0xf74
			 0x2bf5d6bb, /// 0xf78
			 0x444b9dd5, /// 0xf7c
			 0xd71cbc9c, /// 0xf80
			 0xa7950741, /// 0xf84
			 0xd7c7efbc, /// 0xf88
			 0x66cdf565, /// 0xf8c
			 0x70116d15, /// 0xf90
			 0xe0444f4e, /// 0xf94
			 0x85c3a7a3, /// 0xf98
			 0x4e37acbc, /// 0xf9c
			 0x0f09acff, /// 0xfa0
			 0xd1557ba1, /// 0xfa4
			 0xe48a0962, /// 0xfa8
			 0xd9b02031, /// 0xfac
			 0x931a9e0b, /// 0xfb0
			 0x2fbed480, /// 0xfb4
			 0x2fa45e1e, /// 0xfb8
			 0x0e9aa04e, /// 0xfbc
			 0x84b1433c, /// 0xfc0
			 0xa753e202, /// 0xfc4
			 0x504d831a, /// 0xfc8
			 0x2bffe7ef, /// 0xfcc
			 0x2b879c06, /// 0xfd0
			 0x8751ea1b, /// 0xfd4
			 0xa86b043b, /// 0xfd8
			 0x6963bc4f, /// 0xfdc
			 0x4125ab2f, /// 0xfe0
			 0x4edcfcd9, /// 0xfe4
			 0xd83f1647, /// 0xfe8
			 0x4c94e6d9, /// 0xfec
			 0xa836b3aa, /// 0xff0
			 0xfb14b3b1, /// 0xff4
			 0xb878548e, /// 0xff8
			 0x45dc6e90 /// last
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
			 0x0000032c,
			 0x0000030c,
			 0x000005f0,
			 0x000004b0,
			 0x0000016c,
			 0x00000348,
			 0x00000268,
			 0x0000046c,

			 /// vpu register f2
			 0x41700000,
			 0x41e00000,
			 0x41000000,
			 0x41f00000,
			 0x41c80000,
			 0x41a00000,
			 0x41e80000,
			 0x41b80000,

			 /// vpu register f3
			 0x40000000,
			 0x41f80000,
			 0x41a80000,
			 0x3f800000,
			 0x42000000,
			 0x41e80000,
			 0x41a00000,
			 0x41c00000,

			 /// vpu register f4
			 0x40800000,
			 0x41600000,
			 0x41800000,
			 0x41f00000,
			 0x41100000,
			 0x40000000,
			 0x41d00000,
			 0x41e00000,

			 /// vpu register f5
			 0x41f00000,
			 0x41f80000,
			 0x41e80000,
			 0x40800000,
			 0x41800000,
			 0x41880000,
			 0x41100000,
			 0x41000000,

			 /// vpu register f6
			 0x40400000,
			 0x41600000,
			 0x41100000,
			 0x41500000,
			 0x41c80000,
			 0x41c80000,
			 0x41100000,
			 0x40400000,

			 /// vpu register f7
			 0x41b00000,
			 0x41500000,
			 0x41980000,
			 0x41f00000,
			 0x3f800000,
			 0x40a00000,
			 0x41200000,
			 0x40e00000,

			 /// vpu register f8
			 0x3f000000,
			 0x3e800000,
			 0x3e000000,
			 0x3d800000,
			 0x3d000000,
			 0x3c800000,
			 0x3c000000,
			 0x3b800000,

			 /// vpu register f9
			 0x40400000,
			 0x41a80000,
			 0x41d00000,
			 0x40000000,
			 0x41800000,
			 0x41c00000,
			 0x41d80000,
			 0x41980000,

			 /// vpu register f10
			 0x41000000,
			 0x40800000,
			 0x41d80000,
			 0x41900000,
			 0x42000000,
			 0x41600000,
			 0x41100000,
			 0x41f80000,

			 /// vpu register f11
			 0x41980000,
			 0x41b00000,
			 0x41000000,
			 0x40e00000,
			 0x41900000,
			 0x41300000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f12
			 0x41200000,
			 0x42000000,
			 0x41400000,
			 0x41800000,
			 0x41700000,
			 0x41e80000,
			 0x41000000,
			 0x41c00000,

			 /// vpu register f13
			 0x40c00000,
			 0x41b80000,
			 0x41a80000,
			 0x40a00000,
			 0x41a00000,
			 0x3f800000,
			 0x41a80000,
			 0x41b00000,

			 /// vpu register f14
			 0x41b80000,
			 0x41f80000,
			 0x41900000,
			 0x40a00000,
			 0x41e00000,
			 0x41a00000,
			 0x41a80000,
			 0x40000000,

			 /// vpu register f15
			 0x3f800000,
			 0x40c00000,
			 0x40000000,
			 0x40a00000,
			 0x41e80000,
			 0x41300000,
			 0x41880000,
			 0x41e80000,

			 /// vpu register f16
			 0x41300000,
			 0x41a80000,
			 0x41a00000,
			 0x41700000,
			 0x41100000,
			 0x41500000,
			 0x40000000,
			 0x41d80000,

			 /// vpu register f17
			 0x41a00000,
			 0x41800000,
			 0x40800000,
			 0x3f800000,
			 0x41a00000,
			 0x41980000,
			 0x40800000,
			 0x41980000,

			 /// vpu register f18
			 0x41200000,
			 0x41980000,
			 0x41c80000,
			 0x41000000,
			 0x41200000,
			 0x41e80000,
			 0x41700000,
			 0x41a00000,

			 /// vpu register f19
			 0x41880000,
			 0x41b80000,
			 0x41500000,
			 0x41d80000,
			 0x41a00000,
			 0x41100000,
			 0x41000000,
			 0x41300000,

			 /// vpu register f20
			 0x41200000,
			 0x40c00000,
			 0x41f00000,
			 0x41a80000,
			 0x41980000,
			 0x40e00000,
			 0x3f800000,
			 0x41b80000,

			 /// vpu register f21
			 0x41300000,
			 0x41200000,
			 0x41880000,
			 0x42000000,
			 0x41400000,
			 0x40a00000,
			 0x40e00000,
			 0x40e00000,

			 /// vpu register f22
			 0x40e00000,
			 0x41c80000,
			 0x3f800000,
			 0x41e80000,
			 0x41e00000,
			 0x41d80000,
			 0x41980000,
			 0x41980000,

			 /// vpu register f23
			 0x3f800000,
			 0x41400000,
			 0x41b00000,
			 0x42000000,
			 0x41000000,
			 0x40e00000,
			 0x41b80000,
			 0x41300000,

			 /// vpu register f24
			 0x41300000,
			 0x41c00000,
			 0x3f800000,
			 0x41a00000,
			 0x40e00000,
			 0x41980000,
			 0x41b80000,
			 0x41d00000,

			 /// vpu register f25
			 0x41c80000,
			 0x41f00000,
			 0x41a00000,
			 0x41700000,
			 0x41e80000,
			 0x41600000,
			 0x41b00000,
			 0x42000000,

			 /// vpu register f26
			 0x41e80000,
			 0x41c80000,
			 0x40400000,
			 0x41980000,
			 0x41900000,
			 0x3f800000,
			 0x41980000,
			 0x41900000,

			 /// vpu register f27
			 0x40c00000,
			 0x41a80000,
			 0x41f80000,
			 0x41e00000,
			 0x40400000,
			 0x41e00000,
			 0x41f80000,
			 0x41c00000,

			 /// vpu register f28
			 0x40c00000,
			 0x41f80000,
			 0x41000000,
			 0x41700000,
			 0x40a00000,
			 0x41980000,
			 0x41980000,
			 0x41600000,

			 /// vpu register f29
			 0x41200000,
			 0x41b00000,
			 0x41300000,
			 0x41b80000,
			 0x41600000,
			 0x41d80000,
			 0x41d80000,
			 0x41d80000,

			 /// vpu register f30
			 0x41800000,
			 0x40e00000,
			 0x41100000,
			 0x40000000,
			 0x41b80000,
			 0x41a00000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f31
			 0x41880000,
			 0x41a00000,
			 0x41300000,
			 0x41b80000,
			 0x40800000,
			 0x41100000,
			 0x41e80000,
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
		"flog.ps f0, f0\n"
		"fexp.ps f0, f0\n"
		"frcp.ps f0, f0\n"
		//RTLMIN-5479"frsq.ps f0, f0\n"
		"flog.ps f0, f0\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (

	"LBL_VPU_SEQ_START_0:\n"
		"fexp.ps f11, f9\n"                                   ///  0,     0
		"csrr x5, fcsr\n"                                     // read 
		"fscsr x0\n"                                          // clear fcsr

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs


		"fence\n"

		"flog.ps f9, f8\n"                                    ///  0,    43 --> flog error

		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
	"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
