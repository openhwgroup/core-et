/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

int main()
{
    const uint64_t TEST_IPI_REDIRECT_PC_VALUE = 0xFEDCBA987654;
    C_TEST_START;
    uint64_t shire_id = get_shire_id();
    // TBD: The best ESR for this might be the IPI_REDIRECT_PC
    // Address: 0x80100040 + (Shire# <<22) + (Neigh<<16)
    // Access: R/W
    // Description: User-level software writes into this register the 48 least significant bits of a
    // target virtual PC of a redirect IPI for all the harts in a neighborhood. When a redirect IPI
    // is successfully delivered to a given hart, instruction execution of the hart will be redirected to
    // IPI_REDIRECT_PC. Bits 63:48 of the PC are ignored on writes, so non-canonical addresses will be
    // assumed to be correct. Also, bit 0 of the PC is always 0.

    //            (user,          esr region,   esr_addr(fcc reg) shire_mask, data
    broadcast_req(PP_USER, REGION_NEIGHBOURHOOD, 0x1E008, 1 << shire_id, TEST_IPI_REDIRECT_PC_VALUE);
    // broadcast_req (PP_USER,       REGION_OTHER, 0x8018,           0xffffffff, 0x234567890);
    // broadcast_req (PP_SUPERVISOR, REGION_OTHER, 0x8018,           0xaaaaaaaa, 0x234567890);
    // broadcast_req (PP_MACHINE,    REGION_OTHER, 0x8018,           0x11111111, 0x234567890);

    for (uint32_t neigh_id = 0; neigh_id < 4; neigh_id++) {
        // FYI: The neigh_reg_address will be right shifted by 3 to form the correct ESR address
        uint32_t neigh_reg_address = (neigh_id << 13ul) | 0x8ul;
        uint64_t neigh_ipi_redirect_pc_value = read_esr(PP_USER, 0x0, REGION_NEIGHBOURHOOD, neigh_reg_address);
        if (neigh_ipi_redirect_pc_value != TEST_IPI_REDIRECT_PC_VALUE) {
            C_TEST_FAIL;
        }
    }
    C_TEST_PASS;
}
