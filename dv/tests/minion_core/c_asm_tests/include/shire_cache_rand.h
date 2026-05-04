/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __SHIRE_CACHE_RAND_H
#define __SHIRE_CACHE_RAND_H

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#include <cinttypes>
#else
#include <inttypes.h>
#endif

#define ESR_SC_L3_SHIRE_SWIZZLE_CTL       0x0
#define ESR_SC_REQQ_CTL                   0x1
#define ESR_SC_PIPE_CTL                   0x2
#define ESR_SC_L2_CACHE_CTL               0x3
#define ESR_SC_L3_CACHE_CTL               0x4
#define ESR_SC_SCP_CACHE_CTL              0x5
#define ESR_SC_IDX_COP_SM_CTL             0x6
#define ESR_SC_IDX_COP_SM_PHYSICAL_INDEX  0x7
#define ESR_SC_IDX_COP_SM_DATA0           0x8
#define ESR_SC_IDX_COP_SM_DATA1           0x9
#define ESR_SC_IDX_COP_SM_ECC             0xa
#define ESR_SC_ERR_LOG_CTL                0xb
#define ESR_SC_ERR_LOG_INFO               0xc
#define ESR_SC_ERR_LOG_ADDRESS            0xd
#define ESR_SC_SBE_DBE_COUNTS             0xe
#define ESR_SC_REQQ_DEBUG_CTL             0xf
#define ESR_SC_REQQ_DEBUG0                0x10
#define ESR_SC_REQQ_DEBUG1                0x11
#define ESR_SC_REQQ_DEBUG2                0x12
#define ESR_SC_REQQ_DEBUG3                0x13
#define ESR_SC_ECO_CTL                    0x14
#define ESR_SC_PERFMON_CTL_STATUS         0x17
#define ESR_SC_PERFMON_CYC_CNTR           0x18
#define ESR_SC_PERFMON_P0_CNTR            0x19
#define ESR_SC_PERFMON_P1_CNTR            0x1a
#define ESR_SC_PERFMON_P0_QUAL            0x1b
#define ESR_SC_PERFMON_P1_QUAL            0x1c
#define ESR_SC_IDX_COP_SM_CTL_USER        0x20
#define ESR_SC_TRACE_ADDRESS_ENABLE       0x3f0
#define ESR_SC_TRACE_ADDRESS_VALUE        0x3f1
#define ESR_SC_TRACE_CTL                  0x3f2

typedef struct cache_line
{
  bool valid; 
  uint64_t qw0_dw0;
  uint64_t qw0_dw1;
  uint64_t qw1_dw0;
  uint64_t qw1_dw1;
  uint64_t qw2_dw0;
  uint64_t qw2_dw1;
  uint64_t qw3_dw0;
  uint64_t qw3_dw1;
} cache_line_t;


// Floating point register usage
// f0 zero
// f1 golden
// f2 read back
// f3 compare result (1 if equal)
// f4 write temp, compare mask

inline __attribute__((always_inline)) void sc_zero_line_l2(uint64_t my_addr) {
   // Get the line into shire cache
   // Lock and zero the L1 line.  Line is now modified.
   lock_va(0, my_addr); 
   WAIT_CACHEOPS;
   // Unlock and evict it to do a full line write to L2. 
   unlock_va(0, my_addr); 
   WAIT_CACHEOPS;
   evict_va(0, to_L2, my_addr); 
   WAIT_CACHEOPS;
}

inline __attribute__((always_inline)) void sc_zero_line_l3(uint64_t my_addr) {
   // Get the line into shire cache
   // Lock and zero the L1 line.  Line is now modified.
   lock_va(0, my_addr); 
   WAIT_CACHEOPS;
   // Unlock and evict it to do a full line write to L2. 
   unlock_va(0, my_addr); 
   WAIT_CACHEOPS;
   evict_va(0, to_L3, my_addr); 
   WAIT_CACHEOPS;
}

// Write partial byte
inline __attribute__((always_inline)) void sc_write_byte(uint64_t my_addr, uint64_t my_data) {
    __asm__ __volatile__
        (

         "sbl      %[my_data], (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Write partial byte to l3
inline __attribute__((always_inline)) void sc_write_global_byte(uint64_t my_addr, uint64_t my_data) {
    __asm__ __volatile__
        (

         "sbg      %[my_data], (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Set byte in golden
inline __attribute__((always_inline)) void sc_set_golden_byte(uint64_t my_addr, uint64_t golden) {

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    golden = (golden & 0xff) << ((my_addr % 4) * 8);

    __asm__ __volatile__
        (
         // set one int
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f1, %[golden]\n"

         :
         : [ps_mask] "r" (ps_mask), [golden] "r" (golden)
         :
        );
}

// Write partial word
inline __attribute__((always_inline)) void sc_write_word(uint64_t my_addr, uint64_t my_data) {

    my_addr = my_addr & ~0x3; 

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    __asm__ __volatile__
        (

         // floating store word local from f4
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f4, %[my_data]\n"

         // fswl has a bug in A0 that forces all mask bits to be on. RTLMIN-2840
         //"fswl.ps f4, (%[my_addr])\n"
         // Use scatter instead
         "fscwl.ps f4, f0(%[my_addr])\n"

         :
         : [ps_mask] "r" (ps_mask), [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Write partial word in l3
inline __attribute__((always_inline)) void sc_write_global_word(uint64_t my_addr, uint64_t my_data) {

    my_addr = my_addr & ~0x3; 

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    __asm__ __volatile__
        (

         // floating store word local from f4
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f4, %[my_data]\n"

         // fswl has a bug in A0 that forces all mask bits to be on. RTLMIN-2840
         //"fswl.ps f4, (%[my_addr])\n"
         // Use scatter instead
         "fscwg.ps f4, f0(%[my_addr])\n"

         :
         : [ps_mask] "r" (ps_mask), [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Write Around
inline __attribute__((always_inline)) void sc_write_around(uint64_t my_addr, uint64_t my_data) {

    my_addr = my_addr & ~0xf; 

    uint64_t ps_mask; 
    //ps_mask = 0xf << ((my_addr % 32) >> 2);
    ps_mask = 0xff;

    // replicate the data 4 times across f4.
    __asm__ __volatile__
        (

         // set one int
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f4, %[my_data]\n"
         :
         : [ps_mask] "r" (ps_mask), [my_data] "r" (my_data)
         :  "memory"
        );

    uint64_t reg_stride = 0;
    uint64_t start_reg = 4; // data in f4
    uint64_t row_size = 0;
    uint64_t num_rows = 0;
    uint64_t coop_minions = 0;
    uint64_t addr = (uint64_t) my_addr;	
    uint64_t stride = 0x0;

    tensor_store(reg_stride, start_reg, row_size, num_rows, addr, coop_minions, stride);
}

// Set word in golden
inline __attribute__((always_inline)) void sc_set_golden_word(uint64_t my_addr, uint64_t golden) {

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);
    __asm__ __volatile__
        (
         // set one int
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f1, %[golden]\n"

         :
         : [ps_mask] "r" (ps_mask), [golden] "r" (golden)
         :
        );
}

// set qw in golden
inline __attribute__((always_inline)) void sc_set_golden_qw(uint64_t my_addr, uint64_t golden) {
    my_addr = my_addr & ~0xf; 

    uint64_t ps_mask; 
    ps_mask = 0xf << ((my_addr % 32) >> 2);
    __asm__ __volatile__
        (
         // set one int
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f1, %[golden]\n"

         :
         : [ps_mask] "r" (ps_mask), [golden] "r" (golden)
         :
        );
}

// Write atomic word
inline __attribute__((always_inline)) void sc_atomic_word(uint64_t my_addr) {

    my_addr = my_addr & ~0x3; 

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    // value to add
    uint64_t my_data = 1; 
    // return value
    uint64_t atomic_ret; 

    __asm__ __volatile__
        (

         // set the mask
         "mov.m.x m0, %[ps_mask], 0x0\n"

         // atomic returns data to integer register
         "amoaddl.w %[atomic_ret], %[my_data], (%[my_addr])\n"

         // move the returned data to the read fp register
         "fbcx.ps f2, %[atomic_ret]\n"

         : [atomic_ret] "=r" (atomic_ret)
         : [ps_mask] "r" (ps_mask), [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Write atomic word
inline __attribute__((always_inline)) void sc_atomic_global_word(uint64_t my_addr) {

    my_addr = my_addr & ~0x3; 

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    // value to add
    uint64_t my_data = 1; 
    // return value
    uint64_t atomic_ret; 

    __asm__ __volatile__
        (

         // set the mask
         "mov.m.x m0, %[ps_mask], 0x0\n"

         // atomic returns data to integer register
         "amoaddg.w %[atomic_ret], %[my_data], (%[my_addr])\n"

         // move the returned data to the read fp register
         "fbcx.ps f2, %[atomic_ret]\n"

         : [atomic_ret] "=r" (atomic_ret)
         : [ps_mask] "r" (ps_mask), [my_addr] "r" (my_addr), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Write atomic word
inline __attribute__((always_inline)) void sc_add1_golden(uint64_t my_addr) {

    my_addr = my_addr & ~0x3; 

    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    // value to add
    uint64_t my_data = 1; 

    __asm__ __volatile__
        (

         // set one int
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f4, %[my_data]\n"

         // atomic returns data to integer register
         "fadd.pi f1, f1, f4\n"

         :
         : [ps_mask] "r" (ps_mask), [my_data] "r" (my_data)
         :  "memory"
        );
}

// Cache line read
inline __attribute__((always_inline)) void sc_read(uint64_t my_addr) {
    // half-line address.
    my_addr = my_addr & ~0x1F; 
    __asm__ __volatile__
        (

         "mov.m.x m0, zero, 0xff\n"
         "flwl.ps f2, (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr)
         :  "memory"
        );
}

// Cache line read forward
inline __attribute__((always_inline)) void sc_read_global(uint64_t my_addr) {
    // half-line address.
    my_addr = my_addr & ~0x1F; 
    __asm__ __volatile__
        (

         "mov.m.x m0, zero, 0xff\n"
         "flwg.ps f2, (%[my_addr])\n"

         :
         : [my_addr] "r" (my_addr)
         :  "memory"
        );
}

// Check a byte in read value vs golden
inline __attribute__((always_inline)) uint64_t sc_check_byte(uint64_t my_addr) {
    uint64_t word_match_cnt;
    uint64_t ps_mask; 
    uint64_t byte_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    byte_mask = 0xff << ((my_addr % 4) * 8);

    __asm__ __volatile__
        (

         // clear f3
         "mov.m.x m0, zero, 0xff\n"
         "fbcx.ps f4, zero\n"
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fbcx.ps f4, %[byte_mask]\n"

         // mask all by the byte we care about in f2
         "mov.m.x m0, zero, 0xff\n"
         "fand.pi f2, f2, f4\n"

         // compare results stored in f3.
         "feq.pi f3, f1, f2\n"

         // set matching word bits in mask and count how many words matched. 
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "fsetm.pi m0, f3\n"
         "maskpopc %[word_match_cnt], m0\n"

         : [word_match_cnt] "=r" (word_match_cnt)
         : [ps_mask] "r" (ps_mask), [byte_mask] "r" (byte_mask)
         :
        );
    et_printf_long("TEST: word_match_cnt %lx\n", word_match_cnt);
    if (word_match_cnt != 1) {
       return 1;   // fails
    }  else {
       return 0;   // pass
    }

}

// Check a word in read value vs golden
inline __attribute__((always_inline)) uint64_t sc_check_word(uint64_t my_addr) {
    uint64_t word_match_cnt;
    uint64_t ps_mask; 
    ps_mask = 1 << ((my_addr % 32) >> 2);

    __asm__ __volatile__
        (

         // clear f3
         "mov.m.x m0, zero, 0xff\n"
         "fbcx.ps f3, zero\n"

         // compare results stored in f3.
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "feq.pi f3, f1, f2\n"

         // set matching word bits in mask and count how many words matched. 
         "fsetm.pi m0, f3\n"
         "maskpopc %[word_match_cnt], m0\n"

         : [word_match_cnt] "=r" (word_match_cnt)
         : [ps_mask] "r" (ps_mask)
         :
        );
    if (word_match_cnt != 1) {
       return 1;   // fails
    }  else {
       return 0;   // pass
    }
}

// Check a qw in read value vs golden
inline __attribute__((always_inline)) uint64_t sc_check_qw(uint64_t my_addr) {
    uint64_t word_match_cnt;
    uint64_t ps_mask; 

    my_addr = my_addr & ~0xf; 
    ps_mask = 0xf << ((my_addr % 32) >> 2);

    __asm__ __volatile__
        (

         // clear f3
         "mov.m.x m0, zero, 0xff\n"
         "fbcx.ps f3, zero\n"

         // compare results stored in f3.
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "feq.pi f3, f1, f2\n"

         // set matching word bits in mask and count how many words matched. 
         "fsetm.pi m0, f3\n"
         "maskpopc %[word_match_cnt], m0\n"

         : [word_match_cnt] "=r" (word_match_cnt)
         : [ps_mask] "r" (ps_mask)
         :
        );
    if (word_match_cnt != 4) {
       return 1;   // fails
    }  else {
       return 0;   // pass
    }
}

// Check a qw in read value vs golden
inline __attribute__((always_inline)) uint64_t sc_check_half_line(uint64_t my_addr) {
    uint64_t word_match_cnt;
    uint64_t ps_mask; 

    my_addr = my_addr & ~0xf; 
    ps_mask = 0xff;

    __asm__ __volatile__
        (

         // clear f3
         "mov.m.x m0, zero, 0xff\n"
         "fbcx.ps f3, zero\n"

         // compare results stored in f3.
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "feq.pi f3, f1, f2\n"

         // set matching word bits in mask and count how many words matched. 
         "fsetm.pi m0, f3\n"
         "maskpopc %[word_match_cnt], m0\n"

         : [word_match_cnt] "=r" (word_match_cnt)
         : [ps_mask] "r" (ps_mask)
         :
        );
    if (word_match_cnt != 8) {
       return 1;   // fails
    }  else {
       return 0;   // pass
    }
}

// Check a line in read value vs golden
inline __attribute__((always_inline)) uint64_t sc_check_line(uint64_t my_addr) {
    uint64_t word_match_cnt;
    uint64_t ps_mask; 

    my_addr = my_addr & ~0xf; 
    ps_mask = 0xf << ((my_addr % 32) >> 2);

    __asm__ __volatile__
        (

         // clear f3
         "mov.m.x m0, zero, 0xff\n"
         "fbcx.ps f3, zero\n"

         // compare results stored in f3.
         "mov.m.x m0, %[ps_mask], 0x0\n"
         "feq.pi f3, f1, f2\n"

         // set matching word bits in mask and count how many words matched. 
         "fsetm.pi m0, f3\n"
         "maskpopc %[word_match_cnt], m0\n"

         : [word_match_cnt] "=r" (word_match_cnt)
         : [ps_mask] "r" (ps_mask)
         :
        );
    if (word_match_cnt != 8) {
       return 1;   // fails
    }  else {
       return 0;   // pass
    }
}

#define TAG_STATE_RAM 0x0
#define TAG_RAM       0x1
#define DATA_RAM      0x2

inline __attribute__((always_inline)) void sc_print_line(cache_line_t line) {
    et_printf_long("TEST: line.valid: 0x%lx \n", line.valid);
    et_printf_long("TEST: line.qw3_dw1: 0x%lx \n", line.qw3_dw1);
    et_printf_long("TEST: line.qw3_dw0: 0x%lx \n", line.qw3_dw0);
    et_printf_long("TEST: line.qw2_dw1: 0x%lx \n", line.qw2_dw1);
    et_printf_long("TEST: line.qw2_dw0: 0x%lx \n", line.qw2_dw0);
    et_printf_long("TEST: line.qw1_dw1: 0x%lx \n", line.qw1_dw1);
    et_printf_long("TEST: line.qw1_dw0: 0x%lx \n", line.qw1_dw0);
    et_printf_long("TEST: line.qw0_dw1: 0x%lx \n", line.qw0_dw1);
    et_printf_long("TEST: line.qw0_dw0: 0x%lx \n", line.qw0_dw0);
}

inline __attribute__((always_inline)) uint64_t sc_compare_line(cache_line_t golden, cache_line_t line, uint8_t qwen = 0xf ) {
    uint64_t failed = 0;
    
    if (golden.valid != line.valid)        failed++; 

    if ((qwen >> 3) && 0x1) {
       if (golden.qw3_dw1 != line.qw3_dw1) failed++; 
       if (golden.qw3_dw0 != line.qw3_dw0) failed++; 
    }
    if ((qwen >> 2) && 0x1) {
       if (golden.qw2_dw1 != line.qw2_dw1) failed++; 
       if (golden.qw2_dw0 != line.qw2_dw0) failed++; 
    }
    if ((qwen >> 1) && 0x1) {
       if (golden.qw1_dw1 != line.qw1_dw1) failed++; 
       if (golden.qw1_dw0 != line.qw1_dw0) failed++; 
    }
    if ((qwen >> 0) && 0x1) {
       if (golden.qw0_dw1 != line.qw0_dw1) failed++; 
       if (golden.qw0_dw0 != line.qw0_dw0) failed++; 
    }

    if (failed != 0) {
       et_printf_long("TEST: %lx mismatches :\n", failed);
       et_printf_long("TEST: golden:\n");
       sc_print_line(golden); 
       et_printf_long("TEST: line:\n");
       sc_print_line(line); 
    }

    return failed;
}

inline __attribute__((always_inline)) void sc_idx_cop_write_go(uint64_t shire_id, uint64_t bank, uint64_t which_ram) {

      uint32_t  esr_addr;  
      //uint64_t  esr_rd_data;
      uint64_t  esr_wr_data;
      uint8_t   shr_cach_sub_region = 0;    // Shire_Cache ESR ([19:17]=3'b000)

      esr_addr = ESR_SC_IDX_COP_SM_CTL; 
      esr_wr_data = 
         (0  << 24)         |  // idx_cop_sm_state
         (which_ram  << 14) |  // ram
         (0  << 13)         |  // hw generated ecc
         (8  <<  8)         |  // debug write
         (0  <<  1)         |  // abort
         (1  <<  0)         ;  // go
      write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

}

inline __attribute__((always_inline)) void sc_idx_cop_read_go(uint64_t shire_id, uint64_t bank, uint64_t which_ram) {

      uint32_t  esr_addr;  
      //uint64_t  esr_rd_data;
      uint64_t  esr_wr_data;
      uint8_t   shr_cach_sub_region = 0;    // Shire_Cache ESR ([19:17]=3'b000)

      esr_addr = ESR_SC_IDX_COP_SM_CTL; 
      esr_wr_data = 
         (0  << 24)         |  // idx_cop_sm_state
         (which_ram  << 14) |  // ram
         (0  << 13)         |  // hw generated ecc
         (7  <<  8)         |  // debug read
         (0  <<  1)         |  // abort
         (1  <<  0)         ;  // go
      write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

}

#define IDX_ALL_INV   0x0
#define IDX_L2_INV    0x1
#define IDX_L2_FLUSH  0x2
#define IDX_L2_EVICT  0x3
#define IDX_L3_INV    0x4
#define IDX_L3_FLUSH  0x5
#define IDX_L3_EVICT  0x6
#define IDX_SCP_ZERO  0x9
#define IDX_CB_INV    0xa

inline __attribute__((always_inline)) void sc_idx_cop_go(uint64_t shire_id, uint64_t bank, uint64_t op) {

      uint32_t  esr_addr;  
      //uint64_t  esr_rd_data;
      uint64_t  esr_wr_data;
      uint8_t   shr_cach_sub_region = 0x00;    // Shire_Cache ESR ([19:17]=3'b000)

      esr_addr = ESR_SC_IDX_COP_SM_CTL; 
      esr_wr_data = 
         (0  << 24)         |  // idx_cop_sm_state
         (0  << 14)         |  // ram
         (0  << 13)         |  // hw generated ecc
         (op <<  8)         |  // debug read
         (0  <<  1)         |  // abort
         (1  <<  0)         ;  // go
      write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

}

#define IDX_L2     0x0
#define IDX_L3     0x1
#define IDX_SCP    0x2

// DBG Write
inline __attribute__((always_inline)) void sc_idx_cop_write_dbg(uint64_t shire_id, uint64_t my_addr, cache_line_t line, uint32_t region = IDX_L2, uint32_t lock = 0) {
   uint32_t  esr_addr;  
   uint64_t  esr_rd_data;
   uint64_t  esr_wr_data;

   uint32_t  bank;
   uint8_t   shr_cach_sub_region = 0;    // Shire_Cache ESR ([19:17]=3'b000)

   if (region == IDX_L2) {
      bank = (my_addr >> 6) & 0x3;
   }
   if (region == IDX_L3) {
      bank = (my_addr >> 11) & 0x3;
   }
   if (region == IDX_SCP) {
      bank = (my_addr >> 6) & 0x3;
   } 
   esr_addr = ESR_SC_L3_SHIRE_SWIZZLE_CTL; 
   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
   uint64_t all_shire_aliasing = (esr_rd_data >> 48) & 0x1; 
   uint64_t two_shire_aliasing_use_shire_lsb = (esr_rd_data >> 49) & 0x1; 

   if (region == IDX_L2) {
      esr_addr = ESR_SC_L2_CACHE_CTL; 
   }
   if (region == IDX_L3) {
      esr_addr = ESR_SC_L3_CACHE_CTL; 
   }
   if (region == IDX_SCP) {
      esr_addr = ESR_SC_SCP_CACHE_CTL; 
   } 

   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
   uint64_t set_base = (esr_rd_data >> 48) & 0x3ff; 
   uint64_t set_mask = (esr_rd_data >> 16) & 0x3ff; 
   //et_printf_long("TEST: set_base: 0x%lx \n", set_base);
   //et_printf_long("TEST: set_mask: 0x%lx \n", set_mask);
   uint64_t tag_mask = ~((esr_rd_data >> 0) & 0xfff); 
   //et_printf_long("TEST: tag_mask: 0x%lx \n", tag_mask);

   et_printf_long("TEST: addr: 0x%lx \n", my_addr);
   uint32_t  set;
   uint32_t  tag;
   uint32_t  way = 0;
   uint32_t  sbank;

   if (region == IDX_L2) {
      set = (my_addr >> 10) & set_mask;          // lop off top tag bits
      tag = ((my_addr >> 10) & tag_mask) >> 7;   // add top set bits to tag lsbs
      sbank = (my_addr >> 8) & 0x3;
   }
   if (region == IDX_L3) {
      set = (my_addr >> 15) & set_mask;
      tag = ((my_addr >> 15) & tag_mask) >> 2;  
      if (all_shire_aliasing) {
         tag = tag | shire_id;
      } else {
         if (two_shire_aliasing_use_shire_lsb) {
            tag = tag | (shire_id & 0x1);
         } else {
            tag = tag | (shire_id >> 4);  // msb of shire id
         }
      }
      sbank = (my_addr >> 13) & 0x3;
   }
   if (region == IDX_SCP) {
      set = (my_addr >> 12) & set_mask;
      tag = 0;
      way = (my_addr >> 10) & 0x3; 
      sbank = (my_addr >> 8) & 0x3;
   }

   et_printf_long("TEST: tag: 0x%lx \n", tag);
   et_printf_long("TEST: set: 0x%lx \n", set);

   // ADDRESS
   esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
   esr_wr_data =
      ((set + set_base)  << 32) |   // set
      (way << 16) |    // way
      (sbank  << 8) |  // sbank
      (0  <<  0) ;     // qw
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   // TAG ram
   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_wr_data = 
      (tag << 0) ;  // tag
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_wr_data = 0;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   sc_idx_cop_write_go(shire_id, bank, TAG_RAM); 

   // TAG STATE ram
   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_wr_data = 
      (0xf << 5)    |  // qwen
      (1 << 11)     |  // valid
      (lock << 10)  |  // lock
      (0x19 <<  0)  ;  // lru  set way 0 to mru
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_wr_data = 0;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   sc_idx_cop_write_go(shire_id, bank, TAG_STATE_RAM); 

   // DATA ram qw 0
   esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
   esr_wr_data =
      ((set + set_base)  << 32) |   // set
      (way << 16) |     // way
      (sbank  << 8) |  // sbank
      (0  <<  0) ;    // qw
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_wr_data = line.qw0_dw0;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_wr_data = line.qw0_dw1;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);
   sc_idx_cop_write_go(shire_id, bank, DATA_RAM); 

   // DATA ram qw 1
   esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
   esr_wr_data =
      ((set + set_base)  << 32) |   // set
      (way  << 16) |     // way
      (sbank  << 8) |  // sbank
      (1  <<  0) ;    // qw
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_wr_data = line.qw1_dw0;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_wr_data = line.qw1_dw1;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);
   sc_idx_cop_write_go(shire_id, bank, DATA_RAM); 

   // DATA ram qw 2
   esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
   esr_wr_data =
      ((set + set_base)  << 32) |   // set
      (way  << 16) |     // way
      (sbank  << 8) |  // sbank
      (2  <<  0) ;    // qw
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_wr_data = line.qw2_dw0;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_wr_data = line.qw2_dw1;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);
   sc_idx_cop_write_go(shire_id, bank, DATA_RAM); 

   // DATA ram qw 3
   esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
   esr_wr_data =
      ((set + set_base)  << 32) |   // set
      (way  << 16) |     // way
      (sbank  << 8) |  // sbank
      (3  <<  0) ;    // qw
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_wr_data = line.qw3_dw0;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_wr_data = line.qw3_dw1;
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);
   sc_idx_cop_write_go(shire_id, bank, DATA_RAM); 

}

// DBG Read
inline __attribute__((always_inline)) cache_line_t sc_idx_cop_read_dbg(uint64_t shire_id, uint64_t my_addr, uint32_t region = IDX_L2) {
   cache_line_t line; 
   uint32_t  esr_addr;  
   uint64_t  esr_rd_data;
   uint64_t  esr_wr_data;

   uint32_t  bank;
   uint8_t   shr_cach_sub_region = 0;    // Shire_Cache ESR ([19:17]=3'b000)

   if (region == IDX_L2) {
      bank = (my_addr >> 6) & 0x3;
   }
   if (region == IDX_L3) {
      bank = (my_addr >> 11) & 0x3;
   }
   if (region == IDX_SCP) {
      bank = (my_addr >> 6) & 0x3;
   } 
   esr_addr = ESR_SC_L3_SHIRE_SWIZZLE_CTL; 
   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
   uint64_t all_shire_aliasing = (esr_rd_data >> 48) & 0x1; 
   uint64_t two_shire_aliasing_use_shire_lsb = (esr_rd_data >> 49) & 0x1; 

   if (region == IDX_L2) {
      esr_addr = ESR_SC_L2_CACHE_CTL; 
   }
   if (region == IDX_L3) {
      esr_addr = ESR_SC_L3_CACHE_CTL; 
   }
   if (region == IDX_SCP) {
      esr_addr = ESR_SC_SCP_CACHE_CTL; 
   } 

   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
   uint64_t set_base = (esr_rd_data >> 48) & 0x3ff; 
   uint64_t set_mask = (esr_rd_data >> 16) & 0x3ff; 
   //et_printf_long("TEST: set_base: 0x%lx \n", set_base);
   //et_printf_long("TEST: set_mask: 0x%lx \n", set_mask);
   uint64_t tag_mask = ~((esr_rd_data >> 0) & 0xfff); 
   //et_printf_long("TEST: tag_mask: 0x%lx \n", tag_mask);

   et_printf_long("TEST: addr: 0x%lx \n", my_addr);
   uint32_t  set;
   uint32_t  tag;
   uint32_t  way = 0;
   uint32_t  sbank;

   if (region == IDX_L2) {
      set = (my_addr >> 10) & set_mask;          // lop off top tag bits
      tag = ((my_addr >> 10) & tag_mask) >> 7;   // add top set bits to tag lsbs
      sbank = (my_addr >> 8) & 0x3;
   }
   if (region == IDX_L3) {
      set = (my_addr >> 15) & set_mask;
      tag = ((my_addr >> 15) & tag_mask) >> 2;  
      if (all_shire_aliasing) {
         tag = tag | shire_id;
      } else {
         if (two_shire_aliasing_use_shire_lsb) {
            tag = tag | (shire_id & 0x1);
         } else {
            tag = tag | (shire_id >> 4);  // msb of shire id
         }
      }
      sbank = (my_addr >> 13) & 0x3;
   }
   if (region == IDX_SCP) {
      set = (my_addr >> 12) & set_mask;
      tag = 0;
      way = (my_addr >> 10) & 0x3; 
      sbank = (my_addr >> 8) & 0x3;
   }

   et_printf_long("TEST: tag: 0x%lx \n", tag);
   et_printf_long("TEST: set: 0x%lx \n", set);

   line.valid = 0; 

   // TAG STATE ram
   esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
   esr_wr_data =
      ((set + set_base)  << 32) |   // set
      (0 << 16) |     // way
      (sbank  << 8) |  // sbank
      (0  <<  0) ;    // qw
   write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

   sc_idx_cop_read_go(shire_id, bank, TAG_STATE_RAM); 
   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);

   uint8_t way_valid[4];
   uint8_t way_zero[4];
   way_valid[0] = (esr_rd_data >> 11) & 1; 
   way_valid[1] = (esr_rd_data >> 18) & 1; 
   way_valid[2] = (esr_rd_data >> 25) & 1; 
   way_valid[3] = (esr_rd_data >> 32) & 1; 
   way_zero[0] = (esr_rd_data >> 9) & 1; 
   way_zero[1] = (esr_rd_data >> 16) & 1; 
   way_zero[2] = (esr_rd_data >> 23) & 1; 
   way_zero[3] = (esr_rd_data >> 30) & 1; 
   et_printf_long("TEST: sc_idx_cop_read_dbg valid ways: %lx %lx %lx %lx\n", way_valid[3], way_valid[2], way_valid[1], way_valid[0]);

   // TAG ram
   sc_idx_cop_read_go(shire_id, bank, TAG_RAM); 
   esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
   uint64_t tag_way;
   if (way_valid[0] == 1) {
      tag_way = (esr_rd_data >>0) & 0x7fffff;
      et_printf_long("TEST: sc_idx_cop_read_dbg: 0x%lx in way 0, matching against tag 0x%lx\n", tag_way, tag);
      if (tag_way == tag) {
         line.valid = 1; 
         way = 0; 
         et_printf_long("TEST: sc_idx_cop_read_dbg found: 0x%lx in way 0x%lx\n", my_addr, way);
      }
   }
   if (way_valid[1] == 1) {
      tag_way = (esr_rd_data >>32) & 0x7fffff;
      et_printf_long("TEST: sc_idx_cop_read_dbg: 0x%lx in way 1, matching against tag 0x%lx\n", tag_way, tag);
      if (tag_way == tag) {
         line.valid = 1; 
         way = 1; 
         et_printf_long("TEST: sc_idx_cop_read_dbg found: 0x%lx in way 0x%lx\n", my_addr, way);
      }
   }
   esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
   esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
   if (way_valid[2] == 1) {
      tag_way = (esr_rd_data >>0) & 0x7fffff;
      et_printf_long("TEST: sc_idx_cop_read_dbg: 0x%lx in way 2, matching against tag 0x%lx\n", tag_way, tag);
      if (tag_way == tag) {
         line.valid = 1; 
         way = 2; 
         et_printf_long("TEST: sc_idx_cop_read_dbg found: 0x%lx in way 0x%lx\n", my_addr, way);
      }
   }
   if (way_valid[3] == 1) {
      tag_way = (esr_rd_data >>32) & 0x7fffff;
      et_printf_long("TEST: sc_idx_cop_read_dbg: 0x%lx in way 3, matching against tag 0x%lx\n", tag_way, tag);
      if (tag_way == tag) {
         line.valid = 1; 
         way = 3; 
         et_printf_long("TEST: sc_idx_cop_read_dbg found: 0x%lx in way 0x%lx\n", my_addr, way);
      }
   }

   if (region == IDX_SCP) {
      line.valid = 1; 
      way = (my_addr >> 10) & 0x3; 
   }

   if (line.valid == 1) {  // valid line, read the data
      if (way_zero[way]) {
         line.qw0_dw0 = 0; 
         line.qw0_dw1 = 0; 
         line.qw1_dw0 = 0; 
         line.qw1_dw1 = 0; 
         line.qw2_dw0 = 0; 
         line.qw2_dw1 = 0; 
         line.qw3_dw0 = 0; 
         line.qw3_dw1 = 0; 
      } else {
         esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
         esr_wr_data =
            ((set + set_base)  << 32) |   // set
            (way << 16) |     // way
            (sbank  << 8) |  // sbank
            (0  <<  0) ;    // qw
         write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

         sc_idx_cop_read_go(shire_id, bank, DATA_RAM); 
         esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw0_dw0 = esr_rd_data; 
         esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw0_dw1 = esr_rd_data; 

         esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
         esr_wr_data =
            ((set + set_base)  << 32) |   // set
            (way << 16) |     // way
            (sbank  << 8) |  // sbank
            (1  <<  0) ;    // qw
         write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

         sc_idx_cop_read_go(shire_id, bank, DATA_RAM); 
         esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw1_dw0 = esr_rd_data; 
         esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw1_dw1 = esr_rd_data; 

         esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
         esr_wr_data =
            ((set + set_base)  << 32) |   // set
            (way << 16) |     // way
            (sbank  << 8) |  // sbank
            (2  <<  0) ;    // qw
         write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

         sc_idx_cop_read_go(shire_id, bank, DATA_RAM); 
         esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw2_dw0 = esr_rd_data; 
         esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw2_dw1 = esr_rd_data; 

         esr_addr = ESR_SC_IDX_COP_SM_PHYSICAL_INDEX; 
         esr_wr_data =
            ((set + set_base)  << 32) |   // set
            (way << 16) |     // way
            (sbank  << 8) |  // sbank
            (3  <<  0) ;    // qw
         write_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, esr_wr_data, bank);

         sc_idx_cop_read_go(shire_id, bank, DATA_RAM); 
         esr_addr = ESR_SC_IDX_COP_SM_DATA0; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw3_dw0 = esr_rd_data; 
         esr_addr = ESR_SC_IDX_COP_SM_DATA1; 
         esr_rd_data = read_esr_new(PP_MACHINE, shire_id, REGION_OTHER, shr_cach_sub_region, esr_addr, bank);
         line.qw3_dw1 = esr_rd_data; 
      }
   }

   //sc_print_line(line);

   return line; 
}

inline __attribute__((always_inline)) uint64_t sc_read_and_check_line(uint64_t my_addr, cache_line_t golden) {

    uint64_t failed = 0;
    uint64_t half_line_addr;

    half_line_addr = my_addr & ~0x3f; 
    sc_read(half_line_addr); 
    sc_set_golden_word(my_addr | (0 << 2), golden.qw0_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (1 << 2), golden.qw0_dw0 >> 32); 
    sc_set_golden_word(my_addr | (2 << 2), golden.qw0_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (3 << 2), golden.qw0_dw1 >> 32); 
    sc_set_golden_word(my_addr | (4 << 2), golden.qw1_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (5 << 2), golden.qw1_dw0 >> 32); 
    sc_set_golden_word(my_addr | (6 << 2), golden.qw1_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (7 << 2), golden.qw1_dw1 >> 32); 
    failed += sc_check_half_line(half_line_addr); 

    half_line_addr = half_line_addr | 0x20; 
    sc_read(half_line_addr); 
    sc_set_golden_word(my_addr | (0 << 2), golden.qw2_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (1 << 2), golden.qw2_dw0 >> 32); 
    sc_set_golden_word(my_addr | (2 << 2), golden.qw2_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (3 << 2), golden.qw2_dw1 >> 32); 
    sc_set_golden_word(my_addr | (4 << 2), golden.qw3_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (5 << 2), golden.qw3_dw0 >> 32); 
    sc_set_golden_word(my_addr | (6 << 2), golden.qw3_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (7 << 2), golden.qw3_dw1 >> 32); 
    failed += sc_check_half_line(half_line_addr); 

    return failed;
}

inline __attribute__((always_inline)) uint64_t sc_read_global_and_check_line(uint64_t my_addr, cache_line_t golden) {

    uint64_t failed = 0;
    uint64_t half_line_addr;

    half_line_addr = my_addr & ~0x3f; 
    sc_read_global(half_line_addr); 
    sc_set_golden_word(my_addr | (0 << 2), golden.qw0_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (1 << 2), golden.qw0_dw0 >> 32); 
    sc_set_golden_word(my_addr | (2 << 2), golden.qw0_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (3 << 2), golden.qw0_dw1 >> 32); 
    sc_set_golden_word(my_addr | (4 << 2), golden.qw1_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (5 << 2), golden.qw1_dw0 >> 32); 
    sc_set_golden_word(my_addr | (6 << 2), golden.qw1_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (7 << 2), golden.qw1_dw1 >> 32); 
    failed += sc_check_half_line(half_line_addr); 

    half_line_addr = half_line_addr | 0x20; 
    sc_read_global(half_line_addr); 
    sc_set_golden_word(my_addr | (0 << 2), golden.qw2_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (1 << 2), golden.qw2_dw0 >> 32); 
    sc_set_golden_word(my_addr | (2 << 2), golden.qw2_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (3 << 2), golden.qw2_dw1 >> 32); 
    sc_set_golden_word(my_addr | (4 << 2), golden.qw3_dw0 & 0xffff); 
    sc_set_golden_word(my_addr | (5 << 2), golden.qw3_dw0 >> 32); 
    sc_set_golden_word(my_addr | (6 << 2), golden.qw3_dw1 & 0xffff); 
    sc_set_golden_word(my_addr | (7 << 2), golden.qw3_dw1 >> 32); 
    failed += sc_check_half_line(half_line_addr); 

    return failed;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Functions for rand tests
// Create a list of ops of pointers to these functions. 
typedef struct sc_op
   {
       uint64_t     (*op) (uint64_t x, uint64_t y); 
       const char   *opName;
} SC_OP_t;

#define UNUSED(x) (void)(x)

inline __attribute__((always_inline)) uint64_t sc_rand_nop(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    UNUSED(my_addr);  
    for ( int k = 0; k < 20; k++) __asm__ __volatile__ ("nop");

    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_write_byte(uint64_t my_addr, uint64_t my_data) {
    sc_write_byte(my_addr, my_data); 
    sc_set_golden_byte(my_addr, my_data); 
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_write_word(uint64_t my_addr, uint64_t my_data) {
    sc_write_word(my_addr, my_data); 
    sc_set_golden_word(my_addr, my_data); 
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_write_global_byte(uint64_t my_addr, uint64_t my_data) {
    sc_write_global_byte(my_addr, my_data); 
    sc_set_golden_byte(my_addr, my_data); 
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_write_global_word(uint64_t my_addr, uint64_t my_data) {
    sc_write_global_word(my_addr, my_data); 
    sc_set_golden_word(my_addr, my_data); 
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_evict_L2(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    evict_va(0, to_L3, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_evict_L3(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    evict_va(0, to_Mem, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_flush_L2(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    flush_va(0, to_L3, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_flush_L3(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    flush_va(0, to_Mem, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_prefetch_L1(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    prefetch_va(0, to_L1, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_prefetch_L2(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    prefetch_va(0, to_L2, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_prefetch_L3(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    prefetch_va(0, to_L3, my_addr);
    WAIT_CACHEOPS;
    return 0; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_read_and_check_byte(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_read(my_addr); 
    return sc_check_byte(my_addr); 
}
inline __attribute__((always_inline)) uint64_t sc_rand_read_and_check_word(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_read(my_addr); 
    return sc_check_word(my_addr); 
}
inline __attribute__((always_inline)) uint64_t sc_rand_read_and_check_qw(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_read(my_addr); 
    return sc_check_qw(my_addr); 
}
inline __attribute__((always_inline)) uint64_t sc_rand_read_global_and_check_byte(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_read_global(my_addr); 
    return sc_check_byte(my_addr); 
}
inline __attribute__((always_inline)) uint64_t sc_rand_read_global_and_check_word(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_read_global(my_addr); 
    return sc_check_word(my_addr); 
}
inline __attribute__((always_inline)) uint64_t sc_rand_read_global_and_check_qw(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_read_global(my_addr); 
    return sc_check_qw(my_addr); 
}
inline __attribute__((always_inline)) uint64_t sc_rand_atomic_and_check_word(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_atomic_word(my_addr); 
    uint64_t compare_result =  sc_check_word(my_addr); 
    sc_add1_golden(my_addr); 
    return compare_result; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_atomic_global_and_check_word(uint64_t my_addr, uint64_t my_data) {
    UNUSED(my_data);  
    sc_atomic_global_word(my_addr); 
    uint64_t compare_result =  sc_check_word(my_addr); 
    sc_add1_golden(my_addr); 
    return compare_result; 
}
inline __attribute__((always_inline)) uint64_t sc_rand_write_around(uint64_t my_addr, uint64_t my_data) {
    sc_write_around(my_addr, my_data); 
    sc_set_golden_qw(my_addr, my_data); 
    return 0; 
}

#endif // ! __SHIRE_CACHE_RAND_H
