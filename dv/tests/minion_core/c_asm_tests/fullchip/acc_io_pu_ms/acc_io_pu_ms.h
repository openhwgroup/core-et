/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _IO_PU_ACCESS__
#define _IO_PU_ACCESS_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void  __attribute__ ((interrupt)) machine_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();










#endif


