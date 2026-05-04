/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __ET_TEST_COMMON_H
#define __ET_TEST_COMMON_H

#include <inttypes.h>
#include "macros.h"
#include "cacheops.h"
#include "msg.h"
#include "esr.h"
#include "csr.h"
#include "fcc.h"
#include <stdarg.h>
#include <stdint.h>

#define assert(x)

#define NUM_MINIONS_PER_NEIGH (8)
#define NUM_NEIGH_PER_SHIRE   (4)
#define NUM_HARTS_PER_MINION  (2)
#define NUM_HARTS_PER_SHIRE   (NUM_MINIONS_PER_NEIGH*NUM_NEIGH_PER_SHIRE*NUM_HARTS_PER_MINION)
#define NUM_HARTS_PER_NEIGH (NUM_MINIONS_PER_NEIGH*NUM_HARTS_PER_MINION)

#define L1_WAYS         4
#define L1_SETS         16
#define L1_SETS_LOG2    4
#define L1_CL_SIZE      64
#define L1_CL_SIZE_LOG2 6

#define ET_DIAG_NOP             (0x0)
#define ET_DIAG_PUTCHAR         (0x1)
#define ET_DIAG_RAND            (0x2)
#define ET_DIAG_RAND_MEM_UPPER  (0x3)
#define ET_DIAG_RAND_MEM_LOWER  (0x4)
#define ET_IRQ_INJ              (0x5)
#define ET_DIAG_ECC_INJ         (0x6)
#define ET_DIAG_CYCLE           (0x7)
#define ET_RSP_DELAY            (0x8)

#define BASE_S0_GATESIM_VALID_ADDR 0x0090000000

#define UNUSED(x) (void)(x)

//
// FUTURE: Emmanuel Marie autogenerate these from the spec (along with the asm code and csr addresses)
//
typedef struct et_tensor_fma_conf
{
  uint64_t use_tmask;
  uint64_t b_num_col;
  uint64_t a_num_rows;
  uint64_t a_num_cols;
  uint64_t offset;
  uint64_t use_tenb;
  uint64_t scp_loc_b;
  uint64_t scp_loc_a;
  uint64_t opcode;
  bool first_pass;
} et_tensor_fma_conf_t;
typedef struct port_cfg
{
   uint8_t port_id;
   uint8_t port_en;
   uint8_t port_oob_en;
   uint8_t umode;
   uint8_t log_msg_size;
   uint8_t max_msg_size;
   uint8_t scp_set;
   uint8_t scp_way;
} port_cfg_t;

typedef struct cl_data
{
    unsigned char data[L1_SETS][L1_CL_SIZE];
} cl_data_t;

typedef struct cl_aligned_data {
    unsigned char data[L1_CL_SIZE];
} cl_aligned_data_t;


//
// Utility functions
//
// The underlying validation1 CSR functionality utilized below exists only in simulation and does not exist in Zebu/Post-Silicon
inline void __attribute__((always_inline)) et_diag_command(uint8_t command, uint64_t payload)
{
#ifdef POSTSI_UST
    UNUSED(command);
    UNUSED(payload);
#else   
   uint64_t csr_enc = ((uint64_t) command << 56) |
                      (payload & 0x00FFFFFFFFFFFFFFULL);
   // Write to validation1
   __asm__ __volatile__ (
         "csrw 0x8d1, %[csr_enc]\n"
         :
         : [csr_enc] "r" (csr_enc)
         :
   );
#endif   
}

inline void __attribute__((always_inline)) et_write_val2(uint64_t val)
{
    // Please note that in Post-Silicon, val2 is a 1-bit R/W register only.
   __asm__ __volatile__ (
         "csrw 0x8d2, %[csr_enc]\n"
         :
         : [csr_enc] "r" (val)
         :
   );
}
//
// Print to STDOUT
//
inline void __attribute__((always_inline)) et_putchar(char c)
{
   et_diag_command(ET_DIAG_PUTCHAR, (uint64_t) c);
}
inline void __attribute__((always_inline)) et_printf(const char *str)
{
   while (*str) {
      et_putchar(*str);
      str++;
   }
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

inline void et_print_str(char *n) {
    while (*n != '\0') {
        et_putchar(*n++);
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

inline int et_printf_long(const char* fmt, ...) {
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
            case 's':
                et_print_str(va_arg(ap, char *));
                break;
            default:
                et_putchar(c);
            }
        }
    }

    va_end(ap);

    return 0;
}
//
// Random number generation
//
#if 0
// The following is kept for reference in case the LCG method is allowed for Post-Silicon run(s).
inline uint64_t __attribute__((always_inline)) et_postsi_get_rand_dword_from_lcg(void)
{
    // Note rand() is not available (linker error).
    // See Wikpedia: Linear congruential generator
    // https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
    static uint64_t i = 1;
    return (i = (164603309694725029ul * i) % 14738995463583502973ul);
}
#endif

inline uint64_t __attribute__((always_inline)) et_postsi_get_hart_id()
{
    uint64_t ret;
    __asm__ __volatile__ (
        "csrr %[ret], hartid\n" // u-mode hartid shadow
        : [ret] "=r" (ret)
    );
    return ret;
}

inline uint64_t __attribute__((always_inline)) et_postsi_get_rand_dword(void)
{
#ifdef POSTSI_STATIC_ON_HART_ID
    // NOTE: Depending on the Bring-up environment, you maybe able to get a seed passed to the executable and
    // parse the value. Alternately, you can also use a compile time SEED parameter to set the value
    // which can be changed for different iterations of the test run.
    
    // Currently some randomization of return value is done based on HART-ID
    uint64_t hart_id = et_postsi_get_hart_id(); // A 12-bit value for Minions (FYI: only S32 sets the bit[11]) except SP
    return ((hart_id << 48) | (hart_id << 36) | (hart_id << 24) | (hart_id << 12) | hart_id) ^ 0x123456789ABCDEF7ul;
#else
    return 0x123456789ABCDEF7ul;
#endif    
}

inline uint64_t __attribute__((always_inline)) et_postsi_get_rand_dword(uint64_t min, uint64_t max)
{
#ifdef POSTSI_STATIC_ON_HART_ID
    return (min + (et_postsi_get_rand_dword() % (max + 1 - min)));
#else    
    // By default this returns the midpoint of the range. This is helpful to get the typical value
    // to be somewhere in the middle of a wide range of possible values. If you really want
    // the full coverage in directed tests, exhaustively test out all combinations in the test.

    // Alternately turn define POSTSI_STATIC_ON_HART_ID on and see how much it helps.
    if ((min == max) || (min == (max + 1))) {
        return min;
    }
    if (((min + max) % 2) == 0) {
        return (min + max)/2;
    }
    else {
        return (min + max - 1)/2;
    }
#endif
}

inline uint32_t __attribute__((always_inline)) et_get_rand_word()
{
   uint64_t reg;

#ifdef POSTSI_UST
   reg = et_postsi_get_rand_dword();
#else   
   et_diag_command(ET_DIAG_RAND, 0);
   // Read validation1
   __asm__ __volatile__ ( "csrr %[n], 0x8d1\n\t" :[n] "=r" (reg) : :);
#endif   
   return (uint32_t) reg;
}

// Return a random value >= than 'lower' and <= than 'upper'
inline uint32_t __attribute__((always_inline)) et_get_rand_word(uint32_t lower, uint32_t upper)
{
   uint64_t reg;
#ifdef POSTSI_UST
   reg = et_postsi_get_rand_dword(lower, upper);
#else    
   et_write_val2(((uint64_t) upper << 32) | ((uint64_t) lower));
   et_diag_command(ET_DIAG_RAND, 1);
   // Read validation1
   __asm__ __volatile__ ( "csrr %[n], 0x8d1\n\t" :[n] "=r" (reg) : :);
#endif
   return (uint32_t) reg;
}

inline uint64_t __attribute__((always_inline)) et_get_rand_dword()
{
   uint64_t reg;
#ifdef POSTSI_UST
   reg = et_postsi_get_rand_dword();
#else
   et_diag_command(ET_DIAG_RAND, 2);
   // Read validation1
   __asm__ __volatile__ ( "csrr %[n], 0x8d1\n\t" :[n] "=r" (reg) : :);
#endif   
   return reg;
}

//
// Get cycle count
//
inline uint64_t __attribute__((always_inline)) et_get_cycle_count()
{
   uint64_t reg;
#ifdef POSTSI_UST
   reg = 0;
#else   
   et_diag_command(ET_DIAG_CYCLE, 0);
   // Read validation1
   __asm__ __volatile__ ( "csrr %[n], 0x8d1\n\t" :[n] "=r" (reg) : :);
#endif
   return reg;
}

//
// Memory randomization
//
// The following functionality depends on the Validation1 CSR and it will not be available on the Zebu or Post-Silicon platform.
#ifndef POSTSI_UST
inline void __attribute__((always_inline)) et_val_rand_mem(uint64_t addr, uint32_t sub_cmd)
{
   et_diag_command(ET_DIAG_RAND_MEM_LOWER, ((uint64_t) (addr & 0x00000000FFFFFFFF)) | ((uint64_t) sub_cmd << 32));
   et_diag_command(ET_DIAG_RAND_MEM_UPPER, ((uint64_t) (addr & 0xFFFFFFFF00000000)) | ((uint64_t) sub_cmd << 32));
}
#endif

//
// External Interrupt injection
// To be used by tests running on a Shire standalone testbench
//
// The following functionality depends on the Validation1 CSR and it will not be available on the Zebu or Post-Silicon platform.
#ifndef POSTSI_UST
inline void __attribute__((always_inline)) et_raise_meip(uint64_t shire_mask)
{
   et_diag_command(ET_IRQ_INJ, (shire_mask & 0x3FFFFFFFFULL) | (0x1ULL << 55) | (0x0ULL << 53));
}
inline void __attribute__((always_inline)) et_clear_meip(uint64_t shire_mask)
{
   et_diag_command(ET_IRQ_INJ, (shire_mask & 0x3FFFFFFFFULL) | (0x0ULL << 55) | (0x0ULL << 53));
}
inline void __attribute__((always_inline)) et_raise_seip(uint64_t shire_mask)
{
   et_diag_command(ET_IRQ_INJ, (shire_mask & 0x3FFFFFFFFULL) | (0x1ULL << 55) | (0x2ULL << 53));
}
inline void __attribute__((always_inline)) et_clear_seip(uint64_t shire_mask)
{
   et_diag_command(ET_IRQ_INJ, (shire_mask & 0x3FFFFFFFFULL) | (0x0ULL << 55) | (0x2ULL << 53));
}
inline void __attribute__((always_inline)) et_raise_mtip(uint64_t shire_mask)
{
   et_diag_command(ET_IRQ_INJ, (shire_mask & 0x3FFFFFFFFULL) | (0x1ULL << 55) | (0x1ULL << 53));
}
inline void __attribute__((always_inline)) et_clear_mtip(uint64_t shire_mask)
{
   et_diag_command(ET_IRQ_INJ, (shire_mask & 0x3FFFFFFFFULL) | (0x0ULL << 55) | (0x1ULL << 53));
}
#endif

//
// Set delay per memory address
//
// The following functionality depends on the Validation1 CSR and it will not be available on the Zebu or Post-Silicon platform.
#ifndef POSTSI_UST
inline void __attribute__((always_inline)) et_delay_response(uint64_t paddr, uint64_t cycles) {
   // [55:40] delay cycles
   // [39:0]  paddr of request
   et_diag_command(ET_RSP_DELAY, (paddr & 0xFFFFFFFFFFULL) | ((cycles & 0xFFFF) << 40));
}
#endif

inline void __attribute__((always_inline)) cmp_data(uint64_t actual, uint64_t expected)
{
   __asm__ __volatile__ (
         "mv  a6, %[src]\n\t" // Actual RTL value
         "mv  a7, %[dst]\n\t" // Expected value
         "sub a5, a6, a7\n\t"
         "beq a5, zero, %=f\n\t"
         "lui a7, 0x50BAD\n\t"
         "csrw 0x8d0, a7\n\t" // write to validation0
         "wfi\n\t"
         "%=: \n\t"
         :
         :[src] "r" (actual),
          [dst] "r" (expected)
         : "memory", "a5", "a6", "a7"
   );
}

// Machine trap vector
// FUTURE: Emmanuel Marie have to add checks and using defines to either branch to fail
// or increment global exception counter for erro checking
// Note : Not sure whether without the naked attr the function will be 4k aligned
// as it is required
inline __attribute__((naked)) void mtrap_vector(void)
{
   __asm__ __volatile__ (
         ".align 12\n\t"
         "mtrap_vector%=: csrr s1, mtval\n\t"
         "csrr s2, mcause\n\t"
         "csrr s3, mepc\n\t"
         "addi s3, s3, 4\n\t"
         //"csrw mepc, s3\n\t"
         //"mret\n\t"
         :::
   );
   C_TEST_FAIL;
}

// Setting up mtrap vector
// FUTURE: Emmanuel Marie Have to add strap_vector etc.
inline void setup_mtrap_vector(void)
{
    void (*mtvec_ptr)(void) = &mtrap_vector;
    __asm__ __volatile__ (
        "csrw mtval, t0\n\t"
        "csrw stval, t0\n\t"
        "mv t0, %[mtvec]\n\t"
        "csrw mtvec, t0\n\t"
        :
        :[mtvec] "r" (mtvec_ptr)
        :
  );
}

inline __attribute__((naked)) void ignore_trap_mtrap_vector(void)
{
   // This trap vector reads the PC that trap and jumps back to PC+4
   __asm__ __volatile__ (
         ".align 12\n\t"
         "mtrap_vector%=: csrr s1, mtval\n\t"
         "csrr s2, mcause\n\t"
         "csrr s3, mepc\n\t"
         "addi s3, s3, 4\n\t"
         "csrw mepc, s3\n\t"
         "mret\n\t"
         :::
   );
}

inline void setup_tensor_th1_mtrap_vector(void)
{
    void (*mtvec_ptr)(void) = &ignore_trap_mtrap_vector;
    __asm__ __volatile__ (
        "csrw mtval, t0\n\t"
        "csrw stval, t0\n\t"
        "mv t0, %[mtvec]\n\t"
        "csrw mtvec, t0\n\t"
        :
        :[mtvec] "r" (mtvec_ptr)
        :
  );
}

// FUTURE: Emmanuel Marie THIS SHOULD BE IN tensors.h
inline void __attribute__((always_inline)) et_tensor_fma(et_tensor_fma_conf_t *conf) {
   uint64_t csr_enc = (((uint64_t)conf->use_tmask & 1) << 63) |
                      ((conf->b_num_col & 0x3) << 55)         |
                      ((conf->a_num_rows & 0xF) << 51)        |
                      ((conf->a_num_cols & 0xF) << 47)        |
                      ((conf->offset & 0xF) << 43)            |
                      ((conf->use_tenb & 0x1) << 20)          |
                      ((conf->scp_loc_b & 0xFF) << 12)        |
                      ((conf->scp_loc_a & 0xFF) << 4)         |
                      ((conf->opcode & 0x7) << 1)             |
                      ((uint64_t)conf->first_pass & 1);

   __asm__ __volatile__ (
         "csrw 0x801, %[csr_enc]\n"
         :
         : [csr_enc] "r" (csr_enc)
         :
   );
}

inline void init_fp_regs(void *addr)
{

  __asm__ __volatile__ (
  "flq2 f0,  0(%[ptr1])\n"
  "flq2 f1,  32(%[ptr1])\n"
  "flq2 f2,  64(%[ptr1])\n"
  "flq2 f3,  96(%[ptr1])\n"
  "flq2 f4,  128(%[ptr1])\n"
  "flq2 f5,  160(%[ptr1])\n"
  "flq2 f6,  192(%[ptr1])\n"
  "flq2 f7,  224(%[ptr1])\n"
  "flq2 f8,  256(%[ptr1])\n"
  "flq2 f9,  288(%[ptr1])\n"
  "flq2 f10, 320(%[ptr1])\n"
  "flq2 f11, 352(%[ptr1])\n"
  "flq2 f12, 384(%[ptr1])\n"
  "flq2 f13, 416(%[ptr1])\n"
  "flq2 f14, 448(%[ptr1])\n"
  "flq2 f15, 480(%[ptr1])\n"
  "flq2 f16, 512(%[ptr1])\n"
  "flq2 f17, 544(%[ptr1])\n"
  "flq2 f18, 576(%[ptr1])\n"
  "flq2 f19, 608(%[ptr1])\n"
  "flq2 f20, 640(%[ptr1])\n"
  "flq2 f21, 672(%[ptr1])\n"
  "flq2 f22, 704(%[ptr1])\n"
  "flq2 f23, 736(%[ptr1])\n"
  "flq2 f24, 768(%[ptr1])\n"
  "flq2 f25, 800(%[ptr1])\n"
  "flq2 f26, 832(%[ptr1])\n"
  "flq2 f27, 864(%[ptr1])\n"
  "flq2 f28, 896(%[ptr1])\n"
  "flq2 f29, 928(%[ptr1])\n"
  "flq2 f30, 960(%[ptr1])\n"
  "flq2 f31, 992(%[ptr1])\n"
  :
  : [ptr1] "r" (addr)
  : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", \
    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "25", "f26", "f27", "f28", "f29", "f30", "f31"
  );

}

//-------------------------------------------------------------------------------------------------
//
// FUNTION: warm_l1d
//
//   This function writes to each cache line from base addr to fill up the L1, randomly skipping
//   some lines unless specified.
//
inline void __attribute__((always_inline)) warm_l1d (uint8_t *base, /*bool*/ uint64_t fill_rate = 75) {
   l1d_mode mode = get_l1d_mode();
   int num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 40 : 16;

/******* // note: function get_hart_id() sometimes is not declared
   // (CYE) more precise based on different Thread:
   uint64_t hart_id = get_hart_id(); 
   uint64_t thread_id  = (hart_id & 1);

   if (thread_id == 0) {
       num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 32 :     // T0 has 8 sets (0...7) 
                                          8;       // T0 has 2 sets (12, 13)
   } else if (thread_id == 1) {
       num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 8  :     // T1 has 2 sets (14, 15) 
                                          8;       // T1 has 2 sets (14, 15)
   }
*******/

   //
   // For each cache line, store something
   // By default, 75% of the lines are actually accessed
   //
   for (int ii = 0; ii < num_lines; ++ii) {
      if ((et_get_rand_dword() % 100) < fill_rate) {
         * (base + (64*ii)) = ii;            // Store 1 byte (8-bit)
      }
   }
   FENCE;
}

//-------------------------------------------------------------------------------------------------
//
// FUNTION: clear_l1d
//
//   This function evicts all cache lines present in the L1 dcache
//
inline void __attribute__((always_inline)) clear_l1d() {

   for (int w = 0; w < L1_WAYS; ++w) {
      evict_sw(false, to_L2, w, 0, 15);
   }
   WAIT_CACHEOPS;
}


//-------------------------------------------------------------------------------------------------
//
// FUNTION: init_l1d
//
//   This function writes to each cache line from base addr to fill up the L1, randomly skipping
//   some lines unless specified.
//
inline void __attribute__((always_inline)) init_l1d (uint32_t *base) {
   l1d_mode mode = get_l1d_mode();
   int num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 40 : 16;

   //
   // For each cache line, store something
   //
   for (int ii = 0; ii < num_lines; ++ii) {
      * (base + (64*ii)) = (uint32_t)ii;            // Store 1 byte (8-bit)
   }
   FENCE;
}

//-------------------------------------------------------------------------------------------------
//
// FUNTION: incr_l1d
//
//   This function adds a known value to each cache line from base addr
//
inline void __attribute__((always_inline)) incr_l1d (uint32_t *base, uint64_t wr_value) {
   l1d_mode mode = get_l1d_mode();
   int num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 40 : 16;

   //
   // For each cache line, add know value
   //
   for (int ii = 0; ii < num_lines; ++ii) {
      * (base + (64*ii)) += wr_value;
   }
   FENCE;
}

//-------------------------------------------------------------------------------------------------
//
// FUNTION: check_l1d
//
//   This function checks each cache line from base addr against a know write value
//
inline uint64_t __attribute__((always_inline)) check_l1d (uint32_t *base, uint64_t wr_value, uint64_t loop_count) {
   l1d_mode mode = get_l1d_mode();
   int num_lines = (mode == l1d_shared) ? 64 :
                   (mode == l1d_split)  ? 40 : 16;

   uint64_t golden;
   uint64_t line_val;
   uint8_t result = 0;

   //
   // For each cache line, add know value
   //
   for (int ii = 0; ii < num_lines; ++ii) {
      golden = ii + (loop_count * wr_value);
      line_val = *(base + (64*ii));
      if (golden != line_val) {
         result = 1;
      }
      //* (base + (64*ii)) += wr_value;
   }
   FENCE;
   return result;
}


//-------------------------------------------------------------------------------------------------
//
// FUNTION: delay
//
//   This function delays the hart by some cycles proportional to the argument passed
//
inline void __attribute__((always_inline)) delay(uint64_t cycles) {
   for (uint64_t i = 0; i < cycles; ++i) {
      asm volatile ("nop\n");
   }
}


inline void et_gen_rand_l1_load_miss(void *addr, int num_misses) {

  //uint64_t num_misses = et_gen_rand_word(num_misses);

   __asm__ __volatile__ (

          "add t0, zero, %[cnt]\n\t"
          "add t3, zero, %[ptr]\n\t"

          "%=: \n\t"
          "ld t1,  64(t3)\n\t"

          "addi t2, t2, 1\n\t"
          "addi t3, t3, 64\n\t"

          "blt t2, t0, %=b\n\t"
           :
           : [ptr] "r" (addr), [cnt] "r" (num_misses)
           : "t1","t0","t2", "t3"
  );


}

inline void __attribute__((always_inline)) broadcast_req(esr_protection_t pp, esr_reg_t region, uint32_t address, uint64_t shire_mask, uint64_t value)
{
   volatile uint64_t * BC_ESR_ADDR = (uint64_t *) 0x8035FFF0;
   *BC_ESR_ADDR = value;
   volatile uint64_t * BC_REQ_ADDR = (uint64_t *) (0x8035FFF8 | (uint64_t(pp) << 30));
   *BC_REQ_ADDR =  ((uint64_t(region & 0x03)     << 57) |
                    (uint64_t(address & 0x1ffff) << 40) |
                    (shire_mask & 0xffffffffff));
}

inline void __attribute__((always_inline)) uart_send_data(uint32_t value)
{
   volatile uint32_t * PU_UART_ADDR = (uint32_t *) 0x0012002010;
   *PU_UART_ADDR = value;
}

inline void setup_cache_scp(){
      // PRM-8: Cache Control Extension
      excl_mode(1);
      // Evict the whole L1$
      //       use_tmask, dst, way, set, num_lines, warl
      evict_sw(        0,   1,   0,   0,       0xf,    0);
      evict_sw(        0,   1,   1,   0,       0xf,    0);
      evict_sw(        0,   1,   2,   0,       0xf,    0);
      evict_sw(        0,   1,   3,   0,       0xf,    0);

      // Shared Mode
      mcache_control(0,0,0);
      FENCE;
      WAIT_CACHEOPS;

      // Clear the L1 to avoid following locks to fail
      clear_l1d();
      WAIT_CACHEOPS;

      // D1Split Mode
      mcache_control(1,0,0);
      WAIT_CACHEOPS;
      //NOP;   // VERIF-3295: Xavier suggested an extra NOP before FENCE; (here the above "WAIT_CACHEOPS" takes at least 1 cycle)
      FENCE;   // PRM-8; VERIF-3295 

      // Scratchpad Mode
      mcache_control(1,1,0);
      WAIT_CACHEOPS;

      excl_mode(0);
}


inline void setup_cache_shared(){
      // PRM-8: Cache Control Extension
      excl_mode(1);
      // Evict the whole L1$
      //       use_tmask, dst, way, set, num_lines, warl
      evict_sw(        0,   1,   0,   0,       0xf,    0);
      evict_sw(        0,   1,   1,   0,       0xf,    0);
      evict_sw(        0,   1,   2,   0,       0xf,    0);
      evict_sw(        0,   1,   3,   0,       0xf,    0);

      // Shared Mode
      mcache_control(0,0,0);
      FENCE;
      WAIT_CACHEOPS;

      // Clear the L1 to avoid following locks to fail
      clear_l1d();
      WAIT_CACHEOPS;

      excl_mode(0);
}

// Used for gatesim validation PASS
inline __attribute__((always_inline)) uint64_t atomic_read_inc_feed(uint64_t address)
{
   uint64_t rcv_data = 0;
   __asm__ __volatile__ (
               "li a5, 0xFEED\n\t"
               "amoaddg.d %[result], a5, 0(%[addr])\n\t"
               : [result] "=r" (rcv_data)
               : [addr] "r" (address)
               : "a5"
   );
   return rcv_data;
}

// Used for gatesim validation FAIL
inline __attribute__((always_inline)) uint64_t atomic_read_inc_bad(uint64_t address)
{
   uint64_t rcv_data = 0;
   __asm__ __volatile__ (
               "li a5, 0xBAD\n\t"
               "amoaddg.d %[result], a5, 0(%[addr])\n\t"
               : [result] "=r" (rcv_data)
               : [addr] "r" (address)
               : "a5"
   );
   return rcv_data;
}

inline __attribute__((always_inline)) uint64_t atomic_read(uint64_t address)
{
   uint64_t rcv_data = 0;
   __asm__ __volatile__ (
               "li a5, 0x0\n\t"
               "amoaddg.d %[result], a5, 0(%[addr])\n\t"
               : [result] "=r" (rcv_data)
               : [addr] "r" (address)
               : "a5"
   );
   return rcv_data;
}

// Force S32 to fail in case S0 (netlist) failed
inline __attribute__((always_inline)) void check_gatesim(uint64_t timeout, uint64_t BASE_ADDR) 
{
  while (atomic_read(BASE_ADDR) != 0xfeed) {
    if (atomic_read(BASE_ADDR) == 0xbad) C_TEST_FAIL
    timeout--;
    if (timeout == 0) C_TEST_FAIL
  }
}

inline __attribute__((always_inline)) void s32_scp_init()
{
  // S32 L2 SCP init
  __asm__ __volatile__ 
  (
    "li t0, 0x00000901\n"

    "li t1, 0x80F00030\n"
    "sd t0, 0(t1)\n"

    "li t1, 0x80F02030\n"
    "sd t0, 0(t1)\n"

    "li t1, 0x80F04030\n"
    "sd t0, 0(t1)\n"

    "li t1, 0x80F06030\n"
    "sd t0, 0(t1)\n"

    "fence iorw, iorw\n"
      : : 
  );  
}

inline void setup_l1dcache_split_or_scp(){
#ifdef L1D_SPLIT_MODE
      mcache_control(1,0,0);
      WAIT_CACHEOPS;
#endif
#ifdef L1D_SCP_MODE
      mcache_control(1,0,0);
      WAIT_CACHEOPS;
      FENCE;          // PRM-8; VERIF-3295
      mcache_control(1,1,0);
      WAIT_CACHEOPS;
#endif
}


#endif // ! __ET_TEST_COMMON_H
