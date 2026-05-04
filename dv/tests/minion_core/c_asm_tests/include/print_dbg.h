/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif



#include "serial_device.h"

EXTERN_C int SERIAL_init(volatile Uart* const uartRegs);
EXTERN_C void print_dbg_init(void);

EXTERN_C void SERIAL_putc(volatile Uart* const uartRegs, char c);
EXTERN_C int print_dbg(char *format, ...);
