/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "et_test_common.h"
#include "macros.h"
#include <inttypes.h>
#include "param_sweep.h"
#include "tensors.h"
#include "minion.h"

#define TEST_WORKAROUND
#include "rtlmin_6469.h"
asm(".include \"rtlmin_6469.s\"\n"); // assembly test macros

#define SC_CMD(OPCODE) ((((OPCODE) & 0xf) << 8) | 1)
#define SC_GET_COP_SM_STATE(ADDR) ((*(ADDR) >> 24) && 0xff)
#define ESR_SC_IDX_COP_SM_CTL_SHIRE0_BCAST 0x80F1E030
#define ESR_SC_IDX_COP_SM_CTL_SHIRE0_BNK0 0x80F00030
#define SC_CB_INV 10
#define SC_COP_SM_CB_INV 16

/* Test to reproduce RTLMIN-6469: implicit x31 depency tracking for tensor ops
 Note that the test is compiled with -ffixed-x31, so that x31 will not be
 allocated by the C++ code (changes within asm blocks will be preserved)
*/

static constexpr size_t NR_ITERS = 3;
static constexpr size_t MAX_STRIDE = 2;
static constexpr size_t MAX_NOPS=7;

extern cl_aligned_data_t _binary_blob_bin_start[];
__asm__(
      ".section .data\n\t"
      "_binary_blob_bin_start:        \n\t"
      "      .align     12         \n\t"
      "      .incbin    \"data.raw\"  \n\t"
);

// tensor_test
// templated class that checks tensor instructions with the following snippet:
//     set_x31
//     nop (N times)
//     instruction under test
//     self-check
//
// The template parameters configure the snippet:
//     OP => one of the tensorops_t enum values
//     nr_nops => number of nops
//     x31_src => how is the x31 written. one value corresponding one of X31_XXX constants below
//     stride=> in number of lines (to configure x31)
//
// How do the checks work?
// * For Tensor Load (L1SCP) nothing is done, we rely on CoSIM/BEMU checks
// * For Tensor Load L2SCP:
//     check: compare the src memory against the data loaded into the L2SCP. At the end evict_va the accessed memory addresses.
// * For Tensor Store:
//     check: flush the shire cache coalescing buffer to make sure future accesses sees the most up to date values
// * For Tensor Store From SCP:
//     check: flush the shire cache coalescing buffer and then read the memory addresses we wrote (to trigger CoSIM/BEMU checking)
//

enum tensorops_t { TL = 0, TL_TENB, TL_INT8, TL_INT16, TL_TRANS8, TL_TRANS16, TL_TRANS32, TL_L2SCP, TS, TS_FROM_SCP, NR_TENSOR_OPS};  // has to match the .equs in the .S

template<size_t OP = 0, size_t nr_nops = 0, size_t x31_src = 0, size_t stride = 1>
class tensorops_test {
public:
    static void run() {
        __asm__ ("csrw validation3, %0" : : "r" (OP | (nr_nops<<8) | (x31_src<<16) | (stride << 24)));
        uint64_t x31, x31_bad;
        for ( size_t it = 0 ; it < NR_ITERS; it++ ) {
            // setup:
            size_t tl_id = it & 0x01;
            x31 = (stride << 6) | (tl_id); // value of x31 we want
            x31_bad = (stride+1) << 6; // an incorrect value of  x31 that would make the test fail

            // run the test (calling the assembly macro)
            __asm__ __volatile__ (
                "tensorops  %[op], %[csr], %[csr_fma], %[nr_nops], %[x31_src], %[x31], %[x31_ptr], %[x31_bad]\n"
                :
                : [op] "i" (OP), [csr] "r" (csr_conf(it)), [csr_fma] "r" (csr_conf_fma(it)),
                  [x31] "r" (x31), [x31_ptr] "r" (&x31), "m" (x31), [x31_bad] "r" (x31_bad),
                  [nr_nops] "i" (nr_nops),
                  [x31_src] "i" (x31_src)
                : "memory", "t0"
            );

            // check
            check(it);
        }
    }

private:
    static void flush_coalescing_buffer(uint64_t shire_id) {
       volatile uint64_t* sc_cops_esr_addr = (uint64_t*) (ESR_SC_IDX_COP_SM_CTL_SHIRE0_BCAST + (shire_id << 22));
       *sc_cops_esr_addr = SC_CMD(SC_CB_INV);
       FENCE;

       // Wait for the Shire Cache State Machine to finish
       uint64_t addr_bnk0 = ESR_SC_IDX_COP_SM_CTL_SHIRE0_BNK0 + (shire_id << 22);
       for (int bnk = 0; bnk < 4; bnk++) {
           volatile uint64_t* addr = (uint64_t*) (addr_bnk0 + (bnk << 13));
           volatile uint64_t cop_sm_state;
           do {
               cop_sm_state = SC_GET_COP_SM_STATE(addr);
           } while (cop_sm_state == SC_COP_SM_CB_INV);
           FENCE;
       }
    }

    INLINE static uint64_t* SCPline_to_addr(uint64_t scp_line, bool is_local_scp, uint64_t sid, uint64_t scp_format_mode) {
        uint64_t scp_shire;
        uint64_t* addr;

        scp_shire = (is_local_scp == true) ? 0xFFFL : sid;

        // PRM-19 - 2.3 The Scratchpad Region (2G to 4G)
        // Bit 30 defines the SCP Format Mode
        scp_format_mode = scp_format_mode & 0x1UL;
        scp_shire = scp_shire & (0x1UL << 30);
        scp_shire = scp_shire | (scp_format_mode << 30);

        addr = (uint64_t*)(0b000000001UL          << 31 | // Fixed for SCP region
                          (scp_shire & 0x0FFUL)   << 23 | //  8 bits
                          (scp_line  & 0x0FFFFUL) <<  6); // 16 bits

        return addr;
    }

    static void tl_l2scp_self_check(uint64_t* pre, uint64_t* post, uint64_t nlines) {
        uint64_t cop_pre = (uint64_t) pre;
        uint64_t cop_post = (uint64_t) post;
        for (uint64_t i = 0; i < nlines; i++) {
            for (uint64_t elem = 0; elem < 8; elem++) {
                if (pre[elem] != post[elem]) {
                    C_TEST_FAIL;
                }
            }
            pre = pre + ((stride*64)/sizeof(uint64_t));
            post = post + 8;
        }
        //       use_tmask,   dst,     addr, num_lines,    stride, id, warl
        evict_va(    false, to_L2,  cop_pre,  nlines-1, stride*64,  0,    0);
        //       use_tmask,   dst,     addr, num_lines,    stride, id, warl
        evict_va(    false, to_L2, cop_post,  nlines-1,        64,  0,    0);
        WAIT_CACHEOPS;
    }

    static uint64_t ts_from_scp_check(uint64_t* addr, uint64_t rows) {
        uint64_t cop_addr = (uint64_t) addr;
        // Read the memory and rely on CoSIM / BEMU to check the result
        volatile uint64_t data;
        for (uint64_t i = 0; i <= rows; i++) {
            for (int j = 0; j < 8; j++) {
                data = addr[j];
            }
            addr = addr + ((stride*64)/sizeof(uint64_t));
        }
        //       use_tmask,   dst,     addr, num_lines,    stride, id, warl
        evict_va(    false, to_L2, cop_addr,      rows, stride*64,  0,    0);
        WAIT_CACHEOPS;
        return data;
    }

    static void check(size_t it) {
        tensor_error_check();
        if (OP == TL_L2SCP) {
            volatile cl_aligned_data_t *buffer = _binary_blob_bin_start;
            uint64_t ptr = (uint64_t) &buffer[it].data[0];
            //                                   scp_line, is_local_scp, sid, scp_format_mode
            uint64_t* scp_addr = SCPline_to_addr(      it,         true,   0,               0);
            tl_l2scp_self_check((uint64_t*)ptr, scp_addr, it+1);
        }
        else if (OP == TS_FROM_SCP) {
            flush_coalescing_buffer(0x0);
            volatile cl_aligned_data_t *buffer = _binary_blob_bin_start;
            uint64_t ptr = (uint64_t) &buffer[it].data[0];
            ts_from_scp_check((uint64_t*)ptr, it);
        }
        else if (OP == TS) {
            flush_coalescing_buffer(0x0);
        }
    }

    INLINE static uint64_t tensor_load_enc(bool use_tmask, bool use_coop, uint64_t dst_start, uint64_t transformation,
                                           bool use_tenb, uint64_t addr, uint64_t offset, uint64_t num_lines) {
        uint64_t csr_enc = (((uint64_t)use_tmask & 1) << 63)  |
                           (((uint64_t)use_coop & 1)  << 62)  |
                           ((transformation & 0x7) << 59)     |
                           ((dst_start & 0x3F) << 53)         |
                           (((uint64_t)use_tenb & 0x1) << 52) |
                           ((addr & 0xFFFFFFFFFFC0ULL))       |
                           ((offset & 0x3) << 4)              |
                           ((num_lines & 0xF));
        return csr_enc;
    }

    INLINE static uint64_t tensor_load_l2_enc(bool use_tmask, uint64_t dst_start, uint64_t addr, uint64_t num_lines) {
        uint64_t csr_enc = (((uint64_t)use_tmask & 1      ) <<     63) |
                           ((dst_start  & 0x1FFFCUL       ) << (48-2)) |
                           ((dst_start  & 0x3UL           ) <<      4) |
                            (addr       & 0xFFFFFFFFFFC0UL)            |
                            (num_lines  & 0x0FUL          );
        return csr_enc;
    }

    INLINE static uint64_t tensor_store_enc(uint64_t reg_stride, uint64_t start_reg, uint64_t cols, uint64_t Arows, uint64_t addr, uint64_t coop_store) {
        uint64_t csr_enc = ((reg_stride     & 0x3 ) << 62) |
                           ((start_reg      & 0x1F) << 57) |
                           ((cols           & 0x3 ) << 55) |
                           ((Arows          & 0xF ) << 51) |
                           ((coop_store     & 0x3 ) << 49) |
                            (addr & 0xFFFFFFFFFFF0);
        return csr_enc;
    }

    INLINE static uint64_t tensor_store_scp_enc(uint64_t entry_stride, uint64_t start_scp_entry, uint64_t Arows, uint64_t addr) {
        uint64_t csr_enc = ((entry_stride & 0x3) << 62)      |
                           ((start_scp_entry & 0x3F) << 56)  |
                           ((addr & 0xFFFFFFFFFFC0ULL))      |
                           ((Arows & 0xF) << 51)             |
                           (((uint64_t)1) << 48);
        return csr_enc;
    }

    INLINE static uint64_t tensor_fma_enc(bool use_tmask, uint64_t b_num_col, uint64_t a_num_rows, uint64_t a_num_cols, uint64_t offset, bool tenc_loc,
                                          bool tenb_unsigned, bool tena_unsigned, bool tenb_loc, uint64_t scp_loc_b, uint64_t scp_loc_a, uint64_t opcode, bool first_pass) {
        uint64_t csr_enc = (((uint64_t)use_tmask & 1) << 63)       |
                           ((b_num_col & 0x3) << 55)               |
                           ((a_num_rows & 0xF) << 51)              |
                           ((a_num_cols & 0xF) << 47)              |
                           ((offset & 0xF) << 43)                  |
                           (((uint64_t) tenc_loc & 1) << 23)       |
                           (((uint64_t) tena_unsigned & 1) << 22)  |
                           (((uint64_t) tenb_unsigned & 1) << 21)  |
                           (((uint64_t) tenb_loc & 1) << 20)       |
                           ((scp_loc_b & 0xFF) << 12)              |
                           ((scp_loc_a & 0xFF) << 4)               |
                           ((opcode & 0x7) << 1)                   |
                           ((uint64_t)first_pass & 1);
        return csr_enc;
    }

    INLINE static uint64_t csr_conf_fma(size_t it) {
        uint64_t tenA_cols = it; // TL TenB num_lines
        uint64_t tenB_cols = 16;

        //                    use_tmask,       b_num_col, a_num_rows, a_num_cols, offset, tenc_loc, tenb_unsigned, tena_unsigned, tenb_loc, scp_loc_b, scp_loc_a, opcode, first_pass
        return tensor_fma_enc(    false, (tenB_cols/4)-1,         it,  tenA_cols,     it,        0,            it,            it,        1,         0,        it,  0b011,          1);
    }

    // returns CSR configuration
    INLINE static uint64_t csr_conf(size_t it) {
        volatile cl_aligned_data_t *buffer = _binary_blob_bin_start;
        uint64_t addr = (uint64_t) &buffer[it].data[0];
        if constexpr (OP >= TL && OP <= TL_TRANS32) {
            constexpr uint64_t trans[] = {0b000, 0b000, 0b001, 0b10, 0b101, 0b110, 0b111};
            //                     use_tmask, use_coop, dst_start, transformation,      use_tenb, addr, offset, num_lines
            return tensor_load_enc(    false,    false,        it,      trans[OP], OP == TL_TENB, addr,     it,        it);
        }
        else if constexpr (OP == TL_L2SCP) {
            //                        use_tmask, dst_start, addr, num_lines
            return tensor_load_l2_enc(    false,        it, addr,        it);
        }
        else if constexpr (OP == TS) {
            constexpr uint64_t cols[] = {0b00, 0b01, 0b11};
            //                      reg_stride, start_reg,       cols, Arows, addr, coop_store
            return tensor_store_enc(        it,        it, cols[it%3],    it, addr,      false);
        }
        else if constexpr (OP == TS_FROM_SCP) {
            //                          entry_stride, start_scp_entry, Arows, addr
            return tensor_store_scp_enc(          it,            it*2,    it, addr);
        }
        return 0;
    }

    static_assert(x31_src < X31_NR_SRCS, "bad x31 src");
    static_assert(stride>0 && stride <= MAX_STRIDE, "bad stride");
    static_assert(OP < size_t(NR_TENSOR_OPS), "bad op");
};


// make a table with all the combinations we want to test
using func_t = decltype( tensorops_test<>::run)*;
constexpr auto cases = make_tests < tensorops_test, func_t,
                                    range<NR_TENSOR_OPS-1, 0>, // tensor operation
                                    range<MAX_NOPS, 0>,        // number of nops
                                    range<X31_NR_SRCS -1, 0>,  // where the x31 is loaded from
                                    range<MAX_STRIDE, 1>       // stride
                                  > :: sweep();

int main(){
    C_TEST_START;
    uint64_t tid;

    // Get minion ID and thread id
    tid = get_thread_id();

    if (tid == 0) {
        // Set DCACHE in SCP mode
        setup_cache_scp();

        // Load random values into the FP RF
        volatile cl_aligned_data_t *buffer = _binary_blob_bin_start;
        uint64_t addr = (uint64_t) &buffer[12].data[0];
        init_fp_regs((void*) addr);

        // run the test cases
        for (auto const  &t: cases) {
            t();
        }
    }
    C_TEST_PASS;
}
