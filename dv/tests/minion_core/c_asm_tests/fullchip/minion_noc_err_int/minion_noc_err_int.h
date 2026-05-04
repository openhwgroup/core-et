/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _MBOX_ACCESS_
#define _MBOX_ACCESS_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void  __attribute__ ((interrupt)) machine_handler();
EXTERN_C void  __attribute__ ((interrupt)) bad_trap();


#endif