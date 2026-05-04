/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Device-specific serial setings needed by the serial driver

#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <stdint.h>
#include "DW_apb_uart.h"
#include "hal_device.h"

#define UART0 ((volatile Uart*)R_SP_UART0_BASEADDR)
#define UART1 ((volatile Uart*)R_SP_UART1_BASEADDR)
#define PU_UART0 ((volatile Uart*)R_PU_UART_BASEADDR)
#define PU_UART1 ((volatile Uart*)R_PU_UART1_BASEADDR)

#endif
