/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdint.h>

#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

#define __feed_fp(insn,i) \
	__asm__ __volatile__ ( \
	"	" insn " f%0, 0(t0)\n" : :"n" (i)\
	);

#define __computational(ins,j,k,m) \
	__asm__ __volatile__ ( \
	"	" ins " f%0, f%1, f%2\n" : :"n" (j),"n" (k),"n" (m)\
	);

#define __computational4(ins,j,k,m,n) \
	__asm__ __volatile__ ( \
	"	" ins " f%0, f%1, f%2, f%3\n" : :"n" (j),"n" (k),"n" (m),"n" (n)\
	);
