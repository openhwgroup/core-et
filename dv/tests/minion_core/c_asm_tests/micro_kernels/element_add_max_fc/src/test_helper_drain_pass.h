/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

    int num_drains = 1;
    uint64_t bank  = minion_id - 24; // Minions 24-27 -> bank 0-3
    uint64_t sync  = 32 + shire_id;  // Sync minions are 16-32 (harts 32-64) of master shire
    uint64_t cb_drain_hart_mask = 1ULL << ((shire_id >> 1) + 16);

    uint64_t cb_drain_params =
        (1ULL)                   << 0  | // before_fcc_consume: true
        (0ULL)                   << 1  | // before_fcc_reg: FCC0
        (0xFFULL)                << 2  | // drain_shire: 0xFF (current shire)
        (bank)                   << 10 | // drain_bank: 1 C.B. drainer per shire cache bank
        (1ULL)                   << 12 | // after_flb: true
        (0ULL)                   << 13 | // after_flb_num: FLB barrier num 0
        (4ULL - 1)               << 18 | // after_flb_match: 4 drainers - 1
        (1ULL)                   << 26 | // after_fcc_send: true
        (32ULL)                  << 27 | // after_fcc_shire: master shire
        (0ULL)                   << 35 | // after_fcc_reg: FCC0
        (uint64_t(shire_id & 1)) << 36;  // after_fcc_thread: depending on shire_id[0]

    for(int i = 0; i < num_drains; i++) {
        // Call the M-mode service that performs CB drain
        //ecall_cb_drain(cb_drain_params, cb_drain_hart_mask);

        int before_fcc_consume = (cb_drain_params >> 0) & 1;
        int before_fcc_reg = (cb_drain_params >> 1) & 1;
        int drain_shire = (cb_drain_params >> 2) & 0xFF;
        int drain_bank = (cb_drain_params >> 10) & 3;
        int after_flb = (cb_drain_params >> 12) & 1;
        int after_flb_num = (cb_drain_params >> 13) & 0x1F;
        int after_flb_match = (cb_drain_params >> 18) & 0xFF;
        int after_fcc_send = (cb_drain_params >> 26) & 1;
        int after_fcc_shire = (cb_drain_params >> 27) & 0xFF;
        int after_fcc_reg = (cb_drain_params >> 35) & 1;
        int after_fcc_thread = (cb_drain_params >> 36) & 1;

        int flb_last = 0;

        // Wait until we receive a FCC credit from the compute minions
        if (before_fcc_consume)
            fcc_consume(before_fcc_reg);

        // Drain the coalescing buffer of shire cache bank
        // 1. Write the CB invalidate (assumes FSM always available)
        volatile uint64_t *sc_idx_cop_sm_ctl_addr = (uint64_t *)
            ESR_CACHE(drain_shire, drain_bank, SC_IDX_COP_SM_CTL_USER);

        *sc_idx_cop_sm_ctl_addr = (1 << 0) | // Go bit = 1
                                  (10 << 8); // Opcode = CB_Inv (Coalescing buffer invalidate)

        // 2. Checks done
        uint64_t state;
        do {
            state = (*sc_idx_cop_sm_ctl_addr >> 24) & 0xFF;
        } while (state != 4);

        // Wait until all the other C.B. drainer threads are done
        if (after_flb)
            flb_last = flbarrier(after_flb_num, after_flb_match);

        // Send the FCC to sync minions so they know the layer is done
        if (after_fcc_send && flb_last)
            fcc_send(PRV_U, after_fcc_shire, after_fcc_thread, after_fcc_reg, cb_drain_hart_mask);
    }

