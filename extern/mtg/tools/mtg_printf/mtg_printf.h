
#include <stdarg.h>
#include <stdint.h>

#ifndef __MTG_PRINTF__
#define __MTG_PRINTF__

#define R_PU_UART1_BASEADDR 0x0012007000
#include "../mtg_printf/DW_apb_uart.h"
#include "../mtg_printf/serial_device.h"
#include "../mtg_printf/serial.h"
#include "amo_lock.h"

inline int mtg_putchar(int c) {
#ifdef TARGET_ERBIUM
    // Erbium: use validation1 CSR for putchar (UART not yet in emulator)
    static constexpr uint64_t et_diag_putchar = 0x1ull << 56;
    uint64_t csr_enc = et_diag_putchar | c;
    asm volatile("csrw validation1, %[csr_enc]\n" : : [csr_enc] "r"(csr_enc) :);
#else
    SERIAL_putc(PU_UART1, c);
#endif
    return 0;
}

inline void mtg_print_hex(unsigned long long n) {
    if (n == 0) {
        mtg_putchar('0');
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
        mtg_putchar(o + buf[i]);
    }
}

inline void mtg_print_uint(unsigned long long n) {
    if (n == 0) {
        mtg_putchar('0');
        return;
    }
    int i = 0;
    int buf[64];
    while (n > 0) {
        buf[i++] = n % 10;
        n /= 10;
    }
    while (i > 0)
        mtg_putchar('0' + buf[--i]);
}

inline void mtg_print_int(long long n) {
    if (n < 0) {
        mtg_putchar('-');
        n = -n;
    }
    mtg_print_uint(n);
}

#ifdef UART_PRINTS
   #define mtg_printf _mtg_printf
#else
   #define mtg_printf(...)
#endif

int __mtg_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        char c = *fmt++;
        if (c != '%') {
            mtg_putchar(c);
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
                    mtg_print_int(va_arg(ap, int));
                    break;
                case 1:
                    mtg_print_int(va_arg(ap, long int));
                    break;
                case 2:
                    mtg_print_int(va_arg(ap, long long int));
                    break;
                default:
                    assert(0 && "Invalid number of l's");
                }
                break;
            case 'u':
                switch (l) {
                case 0:
                    mtg_print_uint(va_arg(ap, unsigned));
                    break;
                case 1:
                    mtg_print_uint(va_arg(ap, long unsigned));
                    break;
                case 2:
                    mtg_print_uint(va_arg(ap, long long unsigned));
                    break;
                default:
                    assert(0 && "Invalid number of l's");
                }
                break;
            case 'x':
            case 'X':
                switch (l) {
                case 0:
                    mtg_print_hex(va_arg(ap, unsigned));
                    break;
                case 1:
                    mtg_print_hex(va_arg(ap, long unsigned));
                    break;
                case 2:
                    mtg_print_hex(va_arg(ap, long long unsigned));
                    break;
                default:
                    assert(0 && "Invalid number of l's");
                }
                break;
            default:
                mtg_putchar(c);
            }
        }
    }

    va_end(ap);

    return 0;
}

#ifdef MIN_AGNOSTIC_MODE
   #define _mtg_printf _mtg_minion_printf
#elif  NEIGH_AGNOSTIC_MODE
   #define _mtg_printf _mtg_neigh_printf
#elif  SHIRE_AGNOSTIC_MODE
   #define _mtg_printf _mtg_shire_printf
#else
   #define _mtg_printf _mtg_hid_printf
#endif

template<typename... Args>
int _mtg_minion_printf(const char* fmt, Args... args) {
   unsigned int tid;
   __asm__ __volatile__ (
       "csrr %[tid], hartid\n"
       "andi %[tid], %[tid], 1\n"
     : [tid] "=r" (tid)
     :
     :
   );
   SetLock();
   __mtg_printf("[T%d] ", tid);
   __mtg_printf(fmt, args...);
   __asm__ __volatile__ ("fence iorw, iorw");
   UnsetLock();
   return 0;
}

template<typename... Args>
int _mtg_neigh_printf(const char* fmt, Args... args) {
   unsigned int hid;
   __asm__ __volatile__ (
       "csrr %[hid], hartid\n"
       "andi %[hid], %[hid], 15\n"
     : [hid] "=r" (hid)
     :
     :
   );
   SetLock();
   __mtg_printf("[H%d] ", hid);
   __mtg_printf(fmt, args...);
   __asm__ __volatile__ ("fence iorw, iorw");
   UnsetLock();
   return 0;
}

template<typename... Args>
int _mtg_shire_printf(const char* fmt, Args... args) {
   unsigned int hid;
   __asm__ __volatile__ (
       "csrr %[hid], hartid\n"
       "andi %[hid], %[hid], 63\n"
     : [hid] "=r" (hid)
     :
     :
   );
   SetLock();
   __mtg_printf("[H%d] ", hid);
   __mtg_printf(fmt, args...);
   __asm__ __volatile__ ("fence iorw, iorw");
   UnsetLock();
   return 0;
}

template<typename... Args>
int _mtg_hid_printf(const char* fmt, Args... args) {
   unsigned int hid;
   __asm__ __volatile__ (
       "csrr %[hid], hartid\n"
     : [hid] "=r" (hid)
     :
     :
   );
   SetLock();
   __mtg_printf("[H%d] ", hid);
   __mtg_printf(fmt, args...);
   __asm__ __volatile__ ("fence iorw, iorw");
   UnsetLock();
   return 0;
}

#endif
