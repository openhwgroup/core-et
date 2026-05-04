/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "test_defines.hpp"

void randomize_args(args_t & args)
{
    args.start_reg = et_get_rand_dword()%32;
    args.num_regs  = et_get_rand_dword()%32;
    constexpr uint64_t num_ops = FGET + 1;
    args.operation = static_cast<reduce_transform_t>(et_get_rand_dword()%num_ops);
}

// tensor_error[9] is expected to be set
// clear the CSR after ever check
inline void assert_terror_reduce()
{
    uint64_t v;
    asm volatile ("csrrw %[o], tensor_error, x0"
                  : [o] "=r" (v)
                  :
                  :);
    bool e = (v & (1 << TENSOR_ERROR_REDUCE)) != 0;
    if (!e) C_TEST_FAIL;
}

// Local wrapper to type less
template <uint64_t START, uint64_t END, typename Func>
inline void test_range(Func&& f) {
    ::test_range<START, END, Func, randomize_args, assert_terror_reduce>(std::forward<Func>(f));
}


void test_reduce_invalid()
{
    constexpr auto reduce_auto      =[](auto& args) {log_info("ReduceAuto Lvl %d", args.range_val); tensor_reduce_auto(args.start_reg, args.operation, args.num_regs, args.range_val);};
    constexpr auto reduce_broadcast =[](auto& args) {log_info("ReduceBroadcast Lvl %d", args.range_val); tensor_broadcast(args.start_reg, args.operation, args.num_regs, args.range_val);};
    constexpr auto reduce_send      =[](auto& args) {log_info("ReduceSend to %d", args.range_val); tensor_reduce_send(args.start_reg, args.num_regs, args.range_val);};
    constexpr auto reduce_recv      =[](auto& args) {log_info("ReduceRecv to %d", args.range_val); tensor_reduce_recv(args.start_reg, args.operation, args.num_regs, args.range_val);};
    constexpr auto reduce_send_recv =[reduce_send, reduce_recv](auto& args) {reduce_send(args); reduce_recv(args);};
    constexpr auto reduce_auto_broadcast =[reduce_auto, reduce_broadcast](auto& args) {reduce_auto(args); reduce_broadcast(args);};

    log_info("Starting Test reduce invalid combinations H0");
    test_range<START_INVALID_TREE_DEPTH, END_INVALID_TREE_DEPTH>(reduce_auto_broadcast);

#ifdef SHORT
    constexpr uint64_t END_INVALID_PARTNER_ID_SHORT = (1 << 6)-1; // 0x3F
    test_range<START_INVALID_PARTNER_ID, END_INVALID_PARTNER_ID_SHORT>(reduce_send_recv);
#else
    test_range<START_INVALID_PARTNER_ID, END_INVALID_PARTNER_ID>(reduce_send_recv);
#endif
}
