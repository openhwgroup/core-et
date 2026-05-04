/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    // Barrier used (8 to 11)
    // 8 minion barrier, starting at 8th barrier
    uint64_t barrier_o_t = (7 << 5) + 8;

    // B matrix is 1024 x 1024 elements logically
    // Each shire works with a section of 1024 x 64 (rows x cols)
    // Within a shire, even minions work with first 512 rows and odd minions second 512 rows
    // We have 4 minions (even prefetch minions) prefetching the first 512 rows of 64 cols
    // and 4 other minions (odd prefetch minions) prefetching the second 512 rows of 64 cols

    // B row offset computation
    // Gets if even/odd and based on that offsets 512 rows
    uint64_t  bRowInit = ((minion_id - 16) & 1) * 512;
    // Each B Row pitch is 1056 elements of 2 bytes (padding of 64 bytes)
    uint64_t  bRowPitch  = 1056 * 2;
    uint64_t  bRowOffset = bRowInit * bRowPitch;

    // B col offset computation
    // Within a shire, each minion pair advances 16 cols
    uint64_t  bColInitMinion = ((minion_id - 16) / 2) * 16;
    // Each pair of consecutive shires work with same B section
    // Each pair moves 64 columns
    uint64_t  bColInitShire = (shire_id / 2) * 64;
    // Total is offset due shire and offset due minion within shire
    uint64_t  bColInit      = bColInitMinion + bColInitShire;
    // Total is cols by pitch
    uint64_t  bColOffset    = bColInit * 2;

    // Weights are in memory, adds row and col offsets
    uint64_t  tensor_b_init = MEM_WEIGHTS + bRowOffset + bColOffset;

    // Weight minions use L2 scp entries to keep prefetched data
    // Each minion prefetches 16 iterations each one of 16 cachelines, total of 256 cachelines
    // Each one is storing data 256 cachelines apart
    uint64_t  l2_scp_dest_entry_logical = ((minion_id - 16) * 256) + L2_SCP_WEIGHT_MEM_OFFSET;
    // Dest entry goes to bits { 62:48, 5:4 } in TensorLoadL2Scp instruction
    uint64_t  l2_scp_dest_entry = ((l2_scp_dest_entry_logical & 0xFFFFFFFCULL) << 46) + ((l2_scp_dest_entry_logical & 3ULL) << 4);

    // What advances on every tensor load L2 Scp iteration
    // Advances 16 entries the l2 scp destination
    uint64_t  l2_scp_dest_adv_logical = 16;
    uint64_t  l2_scp_dest_adv = ((l2_scp_dest_adv_logical & 0xFFFFFFFCULL) << 46) + ((l2_scp_dest_adv_logical & 3ULL) << 4);
    // Advances 16 rows the B pointer
    uint64_t  tensor_b_adv = 16 * bRowPitch;
    uint64_t  tensor_load_l2scp_adv = l2_scp_dest_adv + tensor_b_adv;

    // Stride for TensorLoad (one b row)
    uint64_t  tensor_load_l2scp_stride = bRowPitch;

     __asm__ __volatile__ (
        // Generate TensorLoadL2Scp command for matrix B
        "or     x8, %[l2_scp_dest_entry], %[tensor_b_init]\n" // Merge mem source with l2 scp dest
        "addi   x8, x8, 15\n"                                 // 16 cachelines worth of prefetching
        // TensorWait ID for TensorLoadL2Scp
        "li     x4, 4\n"
        // TensorLoadL2Scp stride
        "addi   x31, %[tensor_load_l2scp_stride], 0x0\n"
        // Prefetches to wait before doing a tensor wait
        "li     x5, 2\n"
        
        // FCC0 addr + data to weight threads
        "li     x27, 0x803400D0\n"
        "li     x28, %[helper_weights_threads]\n"
        // FCC1 addr + data to compute threads
        "li     x30, 0x803400C8\n"
        "li     x26, %[compute_threads]\n"

        // Open loop for dimension 1
        // Reset Loop counter of Dim: 1
        "li     x6, 16\n"
        "thw_loop_dim_1:\n"
            // Checks if tensor wait needed
            "bne    zero, x5, skip_tensor_wait\n"
                //
                // Checks for tensorwait
                //

                // Switches to another barrier to prevent races between threads reusing
                // same barrier before the last one is done. Done here to improve perf
                "addi   %[barrier_o_t], %[barrier_o_t], 1\n"
                "andi   %[barrier_o_t], %[barrier_o_t], -5\n"

                // Actual tensorwait
                "csrw   0x830, x4\n"

                // Barrier and FCC1 to thread0 and FCC0 to itself
                "csrrw  x29, 0x820, %[barrier_o_t]\n"
                "xori   x4, x4, 1\n"
                "beq    x29, zero, thw_skip_ipi\n"
                    "sd     x26, 0(x30)\n"
                    "sd     x28, 0(x27)\n"
                "thw_skip_ipi:\n"

                "j      thw_do_prefetch\n"
            
            // Decrements skip tensor wait counter
            "skip_tensor_wait:\n"
                "addi   x5, x5, -1\n"

            // Execute B Prefetch
            // To prevent weight threads to be skewed and have issues with FLBs
            // consume an FCC0
            "thw_do_prefetch:\n"
            "csrw   0x821, zero\n"
            "csrw   0x85F, x8\n"
            // Swap the B Prefetch ID
            "xori   x31, x31, 1\n"
            // Advance L2 SCP dest and weight source
            "add    x8, x8, %[tensor_load_l2scp_adv]\n"
            // End of loop for dimension 1
            "addi   x6, x6, -1\n"
            "bne    x6, zero, thw_loop_dim_1\n"

        // End of inner loop

        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done. Done here to improve perf
        "addi   %[barrier_o_t], %[barrier_o_t], 1\n"
        "andi   %[barrier_o_t], %[barrier_o_t], -5\n"
        "csrw   0x830, x4\n"
        "csrrw  x29, 0x820, %[barrier_o_t]\n"
        "beq    x29, zero, skip_ipi_ot_helper_last\n"
            "sd     x26, 0x0 (x30)\n"
            "sd     x28, 0x0 (x27)\n"
        "skip_ipi_ot_helper_last:\n"

        // Switches to another barrier to prevent races between threads reusing
        // same barrier before the last one is done. Done here to improve perf
        "xori   x4, x4, 1\n"
        "addi   %[barrier_o_t], %[barrier_o_t], 1\n"
        "andi   %[barrier_o_t], %[barrier_o_t], -5\n"
        "csrw   0x830, x4\n"
        "csrrw  x29, 0x820, %[barrier_o_t]\n"
        "beq    x29, zero, skip_ipi_ot_helper_last2\n"
            "sd     x26, 0(x30)\n"
            "sd     x28, 0(x27)\n"
        "skip_ipi_ot_helper_last2:\n"
      : 
        [tensor_load_l2scp_stride] "+&r" (tensor_load_l2scp_stride)
      : 
        [compute_threads] "i" (COMPUTE_THREADS),
        [helper_weights_threads] "i" (HELPER_WEIGHTS_THREADS),
        [barrier_o_t] "r" (barrier_o_t),
        [l2_scp_dest_entry] "r" (l2_scp_dest_entry),
        [tensor_b_init] "r" (tensor_b_init),
        [tensor_load_l2scp_adv] "r" (tensor_load_l2scp_adv)
      : 
        "x31",
        "x30",
        "x29",
        "x28",
        "x27",
        "x26",
        "x24",
        "x6",
        "x4",
        "x8" 
    );

