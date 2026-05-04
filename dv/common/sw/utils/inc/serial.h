/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include "DW_apb_uart.h"
#include "serial_device.h"


#define DEBUG_WRITE(x) SERIAL_write(UART0, x, sizeof(x) - 1)

int SERIAL_init(volatile Uart* const uartRegs);
void SERIAL_write(volatile Uart* const uartRegs, const char* const string, int length);
void SERIAL_putc(volatile Uart* const uartRegs, char c);                                                  

#include <stdint.h>
#endif // SERIAL_H
