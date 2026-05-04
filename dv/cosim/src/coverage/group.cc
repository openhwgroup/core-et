/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "group.h"

#include <set>
#include <vector>
#include <map>

#include "system.h"
#include "decode.h"
#include "esrs.h"
#include "insn.h"
#include "insn_func.h"
#include "memmap.h"

namespace coverage {

#define CSRDEF(num, lower, upper) static constexpr unsigned csr_##lower = num;
#include "csrs.h"
#undef CSRDEF

const char* group_name(coverage::group g)
{
#define GROUPDEF(G) #G,
    static const char* names[] = {
#include "def/group.h"
    };
#undef GROUPDEF
    using int_type = typename std::underlying_type<coverage::group>::type;
    return names[static_cast<int_type>(g)];
}

group get_group(const cosim_insn& insn)
{
    return get_group(insn.state_change.inst_bits, insn.state_change.mem_vaddr[0]);
}

group get_group(uint32_t inst_bits, uint64_t store_addr)
{
    typedef void (*exec_fn_t)(bemu::Hart&);

#define COV(G, I)                                                                                                      \
    {                                                                                                                  \
        bemu::insn_##I, group::G                                                                                       \
    }                                                                                                                  \
    ,
    static const std::map<exec_fn_t, coverage::group> insn_map{
#include "def/insns.h"
    };
#undef COV

#define COV(G, C)                                                                                                      \
    {                                                                                                                  \
        csr_##C, group::G                                                                                              \
    }                                                                                                                  \
    ,
    static const std::map<unsigned, coverage::group> csr_map{
#include "def/csrs.h"
    };
#undef COV

    static const std::set<exec_fn_t> rv_mem{ bemu::insn_sb, bemu::insn_sd, bemu::insn_sh, bemu::insn_sw,
                                             bemu::insn_lb, bemu::insn_ld, bemu::insn_lh, bemu::insn_lw };
    static const std::set<exec_fn_t> rv_csr{ bemu::insn_csrrc,  bemu::insn_csrrci, bemu::insn_csrrs,
                                             bemu::insn_csrrsi, bemu::insn_csrrw,  bemu::insn_csrrwi };

    const auto exec_fn = reinterpret_cast<exec_fn_t>(bemu::decode(inst_bits));
    bemu::Instruction insn{ inst_bits, 0 };

    // Match based on CSR address
    if (rv_csr.count(exec_fn)) {
        const auto csr = insn.csrimm();
        const auto it = csr_map.find(csr);
        return it == csr_map.end() ? group::csr_misc : it->second;
    }

    // Match based on exec function
    const auto it = insn_map.find(exec_fn);
    if (it != insn_map.end()) return it->second;

    // Match based on store address (ESRs) // NOTE We do not have the load_addr
    if (rv_mem.count(exec_fn)) {
        if (bemu::paddr_is_esr_space(store_addr)) {
            return group::esr_misc;
        }
    }

    return group::rv_misc;
}

} // namespace coverage
