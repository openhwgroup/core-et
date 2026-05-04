/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "test_defines.hpp"

#include <array>

constexpr int MEMBERS = 8;
constexpr int GROUPS = 2;
constexpr int PAIRS_PER_ROUND = MEMBERS/GROUPS;
constexpr int NROUNDS = 2*(MEMBERS-1); // Both directions

struct pair {
    uint8_t sender;
    uint8_t receiver;
    pair() : sender(0), receiver(0) {}
    pair(uint8_t a, uint8_t b) : sender(a), receiver(b) {}

    void print() const
    {
        log_info("(%d,%d)", sender, receiver);
    }
};

// Alias
using round_set = std::array<pair, PAIRS_PER_ROUND>;


// GLOBALS
std::array<round_set, NROUNDS> pairs;

// FUNCTIONS
static void populate_pairs()
{
    uint8_t elements[] = {0, 2, 4, 6, 8, 10, 12, 14};
    (void)elements;
    for (auto round_idx = 0; round_idx < NROUNDS; round_idx+=2)
    {
        auto & round  = pairs[round_idx];
        auto & rev_round = pairs[round_idx+1];
        for (auto grp = 0; grp < PAIRS_PER_ROUND; ++grp)
        {
            // Swaping and manual polulation is doine here to avoid the compiler
            // insert a memmove operation...
            round[grp].sender = elements[grp];
            round[grp].receiver = elements[MEMBERS-1-grp];
            // Generate the same round in oposite direction
            rev_round[grp].sender = round[grp].receiver;
            rev_round[grp].receiver = round[grp].sender;
            (void)rev_round;
            (void)round;
        }

        // Rotate numbers array
        auto tmp = elements[MEMBERS-1];
        // Loop unrolled to avoid the compiler to optimize to a
        // memmove operation
        #pragma GCC unroll MEMBERS-1
        for (auto i = MEMBERS-1; i > 1; --i) elements[i] = elements[i-1];
        elements[1] = tmp;
    }
    // Make available on coherency point
    clear_l1d();
}



inline void static_args(args_t & args)
{
    args.start_reg = 0;
    args.num_regs = 16;
    args.operation = FADD;
}

// tensor_error[9] is expected to be set
// clear the CSR after ever check
inline void assert_no_terror_reduce()
{
    uint64_t v;
    asm volatile ("csrrw %[o], tensor_error, x0"
                  : [o] "=r" (v)
                  :
                  :);
    bool e = (v & (1 << TENSOR_ERROR_REDUCE)) != 0;
    if (e) C_TEST_FAIL;
}

// Local wrapper to type less
template <uint64_t START, uint64_t END, typename Func>
inline void test_range(Func&& f) {
    ::test_range<START, END, Func, static_args, assert_no_terror_reduce>(std::forward<Func>(f));
}

void h0_barrier()
{
    if (get_hart_id() == 0)
    {
        give_credit(0, -1, 0, 0);
    }
    wait_for_credit(0);
}

// Barrier implemented with credits
// Each hart expects has much credits as participants in the
// barrier.
// Last hart arrving will have to consume all the credits
// in the period their credits are delivared to keep the threads
// synchronized. Loop in unrolled to gurantee that.
template <int EXPECTED_CREDITS>
void barrier()
{
    give_credit(0, -1, 0, 0);
    #pragma GCC unroll EXPECTED_CREDITS
    for (int i = 0; i < EXPECTED_CREDITS; ++i)
        wait_for_credit(0);
    
}

void test_reduce_valid()
{
    if (get_hart_id() == 0)
        populate_pairs();

    // Synchronize all the harts. H0 is expected to arraive last.
    h0_barrier();

    log_info("Starting Test reduce valid combinations H:%d", get_hart_id());

    // Test autopair
    constexpr auto reduce_auto      =[](auto& args) {log_info("ReduceAuto lvl: %d from H:%d", args.range_val, get_hart_id()); tensor_reduce_auto(args.start_reg, args.operation, args.num_regs, args.range_val); WAIT_TENSOR_REDUCE};
    constexpr auto reduce_broadcast =[](auto& args) {log_info("ReduceBroadcast lvl: %d from H:%d", args.range_val, get_hart_id()); tensor_broadcast(args.start_reg, args.operation, args.num_regs, args.range_val); WAIT_TENSOR_REDUCE};
    constexpr auto reduce_auto_broadcast =[reduce_auto, reduce_broadcast](auto& args) {reduce_auto(args); reduce_broadcast(args);};
    test_range<START_VALID_TREE_DEPTH, END_VALID_TREE_DEPTH>(reduce_auto_broadcast);

    // Test send/receive
    constexpr auto reduce_send_recv = [](auto& args){
        uint64_t hart_id = get_hart_id();
        // Look for your role in current round
        const auto & round = pairs[args.range_val];
        for (const auto & grp : round)
        {
            if (grp.sender == hart_id)
            {
                log_info("ReduceSend H:%d sending to M:%d", hart_id, grp.receiver);
                auto minion_id =  grp.receiver >> 1;
                tensor_reduce_send(args.start_reg, args.num_regs, minion_id);
                WAIT_TENSOR_REDUCE;
                barrier<MEMBERS>();
                return;
            }
            else if (grp.receiver == hart_id)
            {
                log_info("ReduceRecv  H:%d receving from M:%d", hart_id, grp.sender);
                auto minion_id =  grp.sender >> 1;
                tensor_reduce_recv(args.start_reg, args.operation, args.num_regs, minion_id);
                WAIT_TENSOR_REDUCE;
                barrier<MEMBERS>();
                return;
            }
        }
        log_error("Hart %d could not find work to do in the round!", hart_id);
        C_TEST_FAIL;
    };


    test_range<0, NROUNDS-1>(reduce_send_recv);
}
