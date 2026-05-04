/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __RISCV_DISASM_H__
#define __RISCV_DISASM_H__

#include <cstddef>
#include <cstdint>

#include <string>

// FUTURE Move to src/support

// disassemble a single RISCV instruction into str
void riscv_disasm(char* str, size_t size, uint32_t bits);

std::string riscv_disasm(uint32_t bits);

#endif // __RISCV_DISASM_H__
