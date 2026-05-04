/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <cstdarg>
#include <cstdio>
// NB: bfd.h needs 'PACKAGE' and 'PACKAGE_VERSION' to be defined
// NB: dis-asm.h needs string.h
#include <cstring>
#ifndef PACKAGE
#define PACKAGE
#endif
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION
#endif
#include <bfd.h>
#include <dis-asm.h>

#include "riscv_disasm.h"

#include <algorithm>

struct dstream_t {
    char* ptr;
    char* end;
};

static thread_local disassembler_ftype func = nullptr;
static thread_local disassemble_info info;
static thread_local dstream_t strm;
static thread_local bfd_byte buf[4] = { 0 };

static void disasm_print_address(bfd_vma addr, disassemble_info* dinfo)
{
    int imm20 = addr & 0xfffff;
    // dinfo->fprintf_func(dinfo->stream, "0x%x", imm20);
    dinfo->fprintf_func(dinfo->stream, "%d", (imm20 & 0x80000) ? ((0xffffffff << 20) | imm20) : imm20);
}

static int ATTRIBUTE_PRINTF_2 disasm_printf(void* stream, const char* fmt, ...)
{
    va_list ap;
    dstream_t* obj = static_cast<dstream_t*>(stream);
    va_start(ap, fmt);
    int len = vsnprintf(obj->ptr, obj->end - obj->ptr, fmt, ap);
    va_end(ap);
    obj->ptr += len;
    return len;
}

// 2025/11/03 ANT: Same function as in disasm_printf to match the new init_dissasemble_info interface
static int  disasm_printf_style(void* stream, enum disassembler_style style,  const char* fmt, ...)
{
    (void) style;
    va_list ap;
    dstream_t* obj = static_cast<dstream_t*>(stream);
    va_start(ap, fmt);
    int len = vsnprintf(obj->ptr, obj->end - obj->ptr, fmt, ap);
    va_end(ap);
    obj->ptr += len;
    return len;
}

void riscv_disasm_init()
{
    init_disassemble_info(&info, &strm, (fprintf_ftype)disasm_printf, disasm_printf_style);
    info.arch = bfd_arch_riscv;
    info.mach = bfd_mach_riscv64;         /* this is ignored currently */
    info.endian_code = BFD_ENDIAN_LITTLE; /* this is ignored currently */
    info.buffer = buf;
    info.buffer_vma = 0;
    info.buffer_length = 4;
    info.print_address_func = disasm_print_address;
    info.disassembler_options = "no-aliases,numeric";
    disassemble_init_for_target(&info);
    func = disassembler(info.arch, 0, info.mach, nullptr);
}

void riscv_disasm(char* str, size_t size, uint32_t bits)
{
    if (!func) {
        riscv_disasm_init();
    }
    buf[0] = (bits >> 0) & 0xff;
    buf[1] = (bits >> 8) & 0xff;
    buf[2] = (bits >> 16) & 0xff;
    buf[3] = (bits >> 24) & 0xff;
    strm.ptr = str;
    strm.end = str + size;
    *strm.ptr = '\0';
    (void)(func)(0, &info);
}

std::string riscv_disasm(uint32_t bits)
{
    static char insn_disasm[128];
    riscv_disasm(insn_disasm, sizeof(insn_disasm), bits);
    std::string s(insn_disasm);
    std::replace(s.begin(), s.end(), '\t', ' ');
    return s;
}
