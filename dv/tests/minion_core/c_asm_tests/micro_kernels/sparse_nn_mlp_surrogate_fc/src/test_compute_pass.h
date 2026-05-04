/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    // Barrier used (0 to 7)
    // 31 minion barrier, starting at 1st barrier
    uint64_t barrier_o_t = (31 << 5);

    // C matrix is 128 x 1024 elements logically
    // Each shire works with a section of 64 x 64 (rows x cols)
    // A pair of consecutive shires are piled vertically
    // Within a shire, each pair of consecutive minions work with a 16 x 16 tile
    // Next set of pairs work on horizontally adjacent tiles
    // Next group of 4 minions work on vertically adjacent tiles

    // C row offset computation
    // Within a shire, each 8 minion group advances 16 rows
    uint64_t  cRowInitMinion = (minion_id / 8) * 16;
    // Each pair of consecutive shires work with different A section
#if N_SHIRES_COMPUTE == 32
    uint64_t  cRowInitShire  = (shire_id & 1) * 64;
#else
    uint64_t  cRowInitShire  = 0;
#endif
    // Total is offset due shire and offset due minion within shire
    uint64_t  cRowInit       = cRowInitMinion + cRowInitShire;
    // Each A Row pitch is 1056 elements of 2 bytes
    uint64_t  cRowPitch      = 1056 * 2;
    uint64_t  cRowOffset     = cRowInit * cRowPitch;

    // C col offset computation
    // Within a shire, each minion pair advances 16 cols
    uint64_t  cColInitMinion = (minion_id / 2) * 16;
    // Each pair of consecutive shires work with same B section
    // Each pair moves 64 columns
    uint64_t  cColInitShire = (shire_id / 2) * 64;
    // Total is offset due shire and offset due minion within shire
    uint64_t  cColInit      = cColInitMinion + cColInitShire;
    // Total is cols by pitch
    uint64_t  cColOffset    = cColInit * 2;

    // Input activations  are stored in L2 SCP using the "offset" region
    // Shire_id to access local L2 SCP
    // Offset where the L2 activations are prefetched
    // Offset depending where the section of each minion is prefetched
    uint64_t  tensor_a_scp_chunk = (minion_id & 1) + (minion_id / 8) * 2;
    uint64_t  tensor_a_init = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_ACTIVATION_PREF_OFFSET << 6) + tensor_a_scp_chunk * 256 * 64;

    // Weights are stored in L2 SCP using the "offset" region
    // Shire_id to access local L2 SCP
    // Offset where the L2 weights are prefetched
    // Offset depending where the section of each minion is prefetched
    uint64_t  tensor_b_scp_chunk = (minion_id & 1) + ((minion_id & 0x6) >> 1) * 2;
    uint64_t  tensor_b_init = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_WEIGHT_MEM_OFFSET << 6) + tensor_b_scp_chunk * 256 * 64;

    // Result is stored in the L2 SCP using the "linear" region
#if N_SHIRES_COMPUTE == 32
    uint64_t  tensor_c_init = (uint64_t volatile) 0x00C0000000UL + (L2_SCP_RESULT_OFFSET << 11) + cRowOffset + cColOffset;
#else
    uint64_t  tensor_c_init = (uint64_t volatile) 0x0080000000UL + (shire_id << 23) + (L2_SCP_RESULT_OFFSET << 6) + cRowOffset + cColOffset;
#endif

    uint64_t  store_results = ((minion_id & 1) == 0);
    uint64_t  tensor_c_stride = 1056 * 2;

    // Cooperative masks for TLoads
    uint64_t  coopMaskA = (/*CoopID*/         minion_id & 0x1)                  // 2 IDs in total per neigh for A (one for even, one for odds)
                        | (/*coopMaskMinion*/ (0x55 << (minion_id & 0x1)) << 8) // Alternate minions share A (evens and odds)
                        | (/*coopMaskNeigh*/  (0x1 << (minion_id >> 3)) << 16); // No A cooperation between neighs, generate mask of minion's neigh
    uint64_t  coopMaskB = (/*CoopID*/         (minion_id & 0x7) + 2)            // One ID different for each B (2..9)
                        | (/*coopMaskMinion*/ (0x1 << (minion_id & 0x7)) << 8)  // No cooperation of B data within a neigh, generate a 1 hot mask
                        | (/*coopMaskNeigh*/  0xF << 16);                       // Same minion in each neigh uses same B data

    __asm__ __volatile__ (
        "li     x4, 0x400000000000000f\n"
        "li     x5, 0x421000000000000f\n"
        "add    x5, x5, %[tensor_b_init]\n"
        "add    x4, x4, %[tensor_a_init]\n"
        "li     x6, 0x07ff800000100003\n"
        // Open loop for dimension 1
        // Reset Loop counter of Dim:  1
        "li     x7, 16\n"
        "loop_dim_1:\n"
            // Switches to another barrier to prevent races between threads reusing
            // same barrier before the last one is done
            "addi   %[barrier_o_t], %[barrier_o_t], 1\n"
            "andi   %[barrier_o_t], %[barrier_o_t], -9\n"

            // Set A TensorLoad Pitch and coop mask. Read consecutive cachelines from L2 scp
            // Waits for the other thread to send the prefetch signal
            // and execute A TensorLoad
            // Set A TensorLoad Pitch and coop
            "li     x31, 64\n"
            "csrw   0x804, %[coopMaskA]\n"
            "csrw   0x821, zero\n"
            "csrw   0x83F, x4\n"

            // Set B TensorLoad Pitch (64, same as previous) and coop mask. Read consecutive cachelines from L2 scp
            // Waits for the other thread to send the prefetch signal.
            // Execute B TensorLoad
            "csrw   0x804, %[coopMaskB]\n"
            "csrwi  0x821, 1\n"
            "csrw   0x83F, x5\n"

            // TODO: we could potentially move it further up to not have a ~20 cycle delay
            // to get FLB result
            // As now there are cycles where we wait for the TLoad A to finish, it is the
            // best place to send the FCC to helper act thread. Now we are consuming the data, so
            // it is as well from producer/consumer point of view
            "csrrw  x31, 0x820, %[barrier_o_t]\n"
            "beq    x31, zero, skip_ipi_ot_helper_main \n"
                "li     x31, %[helper_activation_threads]\n"
                "li     x30, 0x803400D0\n"
                "sd     x31, 0(x30)\n"
            "skip_ipi_ot_helper_main:\n"

            // Wait for Tload A to be done and start TensorFMA
            "csrw   0x830, 0\n"
            "csrw   0x801, x6\n"
            // Switch L1Scp dest for tensorload A
            "li     x31, 0x200000000000000\n"
            "xor    x4, x4, x31\n"
            // Switch L1Scp src for tensorFMA
            "li     x31, 0x100\n"
            "xor    x6, x6, x31\n"
            // Clear the start flag
            "andi   x6, x6, -2\n"
            // End of loop for dimension 2
            "addi   x7, x7, -1\n"
            "beq    x7, zero, exit_loop_dim_1\n"
                // Advance the same amount as the activation prefetch
                "addi   x4, x4, 16 * 64\n"
                // Advance the same amount as the weight prefetch
                "addi   x5, x5, 16 * 64\n"
                "j      loop_dim_1\n"
        "exit_loop_dim_1:\n"

        //
        // Result write section
        //

        // Reduce data with pairing minion
        //  - Start f0, 32 registers, fadd op, tree depth 0, auto op
        "li     x30, 0x200003\n"
        "csrw   0x800, x30\n"
        "beq    %[store_results], zero, skip_write\n"

	          // Downconvert to FP16
	          "fcvt.f16.ps  f0, f0\n"
	          "fcvt.f16.ps  f1, f1\n"
	          "fcvt.f16.ps  f2, f2\n"
	          "fcvt.f16.ps  f3, f3\n"
	          "fcvt.f16.ps  f4, f4\n"
	          "fcvt.f16.ps  f5, f5\n"
	          "fcvt.f16.ps  f6, f6\n"
	          "fcvt.f16.ps  f7, f7\n"
	          "fcvt.f16.ps  f8, f8\n"
	          "fcvt.f16.ps  f9, f9\n"
	          "fcvt.f16.ps  f10, f10\n"
	          "fcvt.f16.ps  f11, f11\n"
	          "fcvt.f16.ps  f12, f12\n"
	          "fcvt.f16.ps  f13, f13\n"
	          "fcvt.f16.ps  f14, f14\n"
	          "fcvt.f16.ps  f15, f15\n"
	          "fcvt.f16.ps  f16, f16\n"
	          "fcvt.f16.ps  f17, f17\n"
	          "fcvt.f16.ps  f18, f18\n"
	          "fcvt.f16.ps  f19, f19\n"
	          "fcvt.f16.ps  f20, f20\n"
	          "fcvt.f16.ps  f21, f21\n"
	          "fcvt.f16.ps  f22, f22\n"
	          "fcvt.f16.ps  f23, f23\n"
	          "fcvt.f16.ps  f24, f24\n"
	          "fcvt.f16.ps  f25, f25\n"
	          "fcvt.f16.ps  f26, f26\n"
	          "fcvt.f16.ps  f27, f27\n"
	          "fcvt.f16.ps  f28, f28\n"
	          "fcvt.f16.ps  f29, f29\n"
	          "fcvt.f16.ps  f30, f30\n"
	          "fcvt.f16.ps  f31, f31\n"

	          // Compact and put 8 results in 1 vectors
	          // First compute the even registers
	          "mov.m.x       m0, zero, 0xf\n"
	          "fpackreph.pi  f0, f0\n"
	          "fpackreph.pi  f2, f2\n"
	          "fpackreph.pi  f4, f4\n"
	          "fpackreph.pi  f6, f6\n"
	          "fpackreph.pi  f8, f8\n"
	          "fpackreph.pi  f10, f10\n"
	          "fpackreph.pi  f12, f12\n"
	          "fpackreph.pi  f14, f14\n"
	          "fpackreph.pi  f16, f16\n"
	          "fpackreph.pi  f18, f18\n"
	          "fpackreph.pi  f20, f20\n"
	          "fpackreph.pi  f22, f22\n"
	          "fpackreph.pi  f24, f24\n"
	          "fpackreph.pi  f26, f26\n"
	          "fpackreph.pi  f28, f28\n"
	          "fpackreph.pi  f30, f30\n"
            // Then the odd registers
	          "mov.m.x       m0, zero, 0xf0\n"
	          "fpackreph.pi  f0, f1\n"
	          "fpackreph.pi  f2, f3\n"
	          "fpackreph.pi  f4, f5\n"
	          "fpackreph.pi  f6, f7\n"
	          "fpackreph.pi  f8, f9\n"
	          "fpackreph.pi  f10, f11\n"
	          "fpackreph.pi  f12, f13\n"
	          "fpackreph.pi  f14, f15\n"
	          "fpackreph.pi  f16, f17\n"
	          "fpackreph.pi  f18, f19\n"
	          "fpackreph.pi  f20, f21\n"
	          "fpackreph.pi  f22, f23\n"
	          "fpackreph.pi  f24, f25\n"
	          "fpackreph.pi  f26, f27\n"
	          "fpackreph.pi  f28, f29\n"
	          "fpackreph.pi  f30, f31\n"

            //
            // Tensor Store
            //  - start f0, row size of 32 bytes, 
            "li     x30, 0x40f8000000000000\n"
            "add    x30, x30, %[tensor_c_init]\n"
            "li     x31, %[tensor_c_stride]\n"
            "csrw   0x87F, x30\n"

            // Restores mask
            "mov.m.x m0, zero, 0xff\n"

        "skip_write:\n"
      :
        [coopMaskA] "+&r" (coopMaskA),
        [coopMaskB] "+&r" (coopMaskB)
      :
        [helper_activation_threads] "i" (HELPER_ACTIVATION_THREADS),
        [tensor_c_stride] "i" (tensor_c_stride),
        [barrier_o_t] "r" (barrier_o_t),
        [tensor_a_init] "r" (tensor_a_init),
        [tensor_b_init] "r" (tensor_b_init),
        [tensor_c_init] "r" (tensor_c_init),
        [store_results] "r" (store_results)
      :
       "x31",
       "x30",
       "x7",
       "x3",
       "x4",
       "x5",
       "x6"
    );

    // We need a separate barrier for the completely done barrier, as some
    // minions might be done at some point before the other ones. If we use
    // the regular 2 thread 0 barriers, the minion will get boggus data

    // This fence waits for Write Arounds to finish!!
    tensorwait(8);

    // We also fence in case we have other stores
    riscv_fence();

    // FLB at barrier 27 and with 32 threads (FLBs 28-31 are reserved for the FW)
    if(flbarrier(27, 32 - 1))
    {
        // The last one reaching the barrier sends FCC0 to the Coalescing Buffer Drainer threads (24-27)
        // User, own shire, credit0 on thread1
        fcc_send(PRV_U, SHIRE_OWN, THREAD_1, FCC_0, HELPER_DRAIN_THREADS);
    }

