/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#define __feed_fp(insn,i) \
	__asm__ __volatile__ ( \
	"	" insn " f%0, 0(t0)\n" : :"n" (i)\
	);

#define __trans(ins,j,k,round) \
	__asm__ __volatile__ ( \
	"	" ins " f%0, f%1, " round "\n" : :"n" (j),"n" (k)\
	);

#define __trans_no_r(ins,j,k) \
	__asm__ __volatile__ ( \
	"	" ins " f%0, f%1\n" : :"n" (j),"n" (k)\
	);

