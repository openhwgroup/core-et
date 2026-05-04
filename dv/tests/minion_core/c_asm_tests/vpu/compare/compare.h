/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

#define __feed_fp(insn,i) \
	__asm__ __volatile__ ( \
	"	" insn " f%0, 0(t0)\n" : :"n" (i)\
	);

#define __compare(ins,j,k,m) \
	__asm__ __volatile__ ( \
	"	" ins " f%0, f%1, f%2\n" : :"n" (j),"n" (k),"n" (m)\
	);

#define __fcvt(inst,l,p) \
	__asm__ __volatile__ ( \
	"	" inst " f%0, f%1\n" : :"n" (l), "n" (p)\
	);
#define __mask(inst,m) \
	__asm__ __volatile__ ( \
	"	" in " f%0\n" : :"n" (m)\
	);
#define __comparemask(ins,j,k,m) \
	__asm__ __volatile__ ( \
	"	" ins " m%0, f%1, f%2\n" : :"n" (j),"n" (k),"n" (m)\
	);

#define __compareint(ins,j,k,m) \
	__asm__ __volatile__ ( \
	"	" ins " t%0, f%1, f%2\n" : :"n" (j),"n" (k),"n" (m)\
	);
