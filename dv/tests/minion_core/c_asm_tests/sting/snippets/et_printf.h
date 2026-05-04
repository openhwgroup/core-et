/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <stdarg.h>
#include <stdint.h>

#define assert(x)

inline int et_putchar(int c) {
    static constexpr uint64_t et_diag_putchar = 0x1ull << 56;
    uint64_t csr_enc = et_diag_putchar | c;
    asm volatile("csrw validation1, %[csr_enc]\n" : : [csr_enc] "r"(csr_enc) :);
    return 0;
}

inline void et_print_hex(unsigned long long n) {
    if (n == 0) {
        et_putchar('0');
        return;
    }
    int i = 0;
    int buf[64];
    while (n > 0) {
        buf[i++] = n % 16;
        n /= 16;
    }
    while (i > 0) {
        --i;
        char o = buf[i] < 10 ? '0' : ('a' - 10);
        et_putchar(o + buf[i]);
    }
}

inline void et_print_uint(unsigned long long n) {
    if (n == 0) {
        et_putchar('0');
        return;
    }
    int i = 0;
    int buf[64];
    while (n > 0) {
        buf[i++] = n % 10;
        n /= 10;
    }
    while (i > 0)
        et_putchar('0' + buf[--i]);
}

inline void et_print_int(long long n) {
    if (n < 0) {
        et_putchar('-');
        n = -n;
    }
    et_print_uint(n);
}

inline int et_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        char c = *fmt++;
        if (c != '%') {
            et_putchar(c);
        } else {
            int l = 0;
            c = *fmt++;
            while (c == 'l') {
                ++l;
                c = *fmt++;
            }
            switch (c) {
            case 'd':
            case 'i':
                switch (l) {
                case 0:
                    et_print_int(va_arg(ap, int));
                    break;
                case 1:
                    et_print_int(va_arg(ap, long int));
                    break;
                case 2:
                    et_print_int(va_arg(ap, long long int));
                    break;
                default:
                    assert(0 && "Invalid number of l's");
                }
                break;
            case 'u':
                switch (l) {
                case 0:
                    et_print_uint(va_arg(ap, unsigned));
                    break;
                case 1:
                    et_print_uint(va_arg(ap, long unsigned));
                    break;
                case 2:
                    et_print_uint(va_arg(ap, long long unsigned));
                    break;
                default:
                    assert(0 && "Invalid number of l's");
                }
                break;
            case 'x':
            case 'X':
                switch (l) {
                case 0:
                    et_print_hex(va_arg(ap, unsigned));
                    break;
                case 1:
                    et_print_hex(va_arg(ap, long unsigned));
                    break;
                case 2:
                    et_print_hex(va_arg(ap, long long unsigned));
                    break;
                default:
                    assert(0 && "Invalid number of l's");
                }
                break;
            default:
                et_putchar(c);
            }
        }
    }

    va_end(ap);

    return 0;
}
