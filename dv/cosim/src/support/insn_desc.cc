/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "insn_desc.h"

#include <unordered_map>

#include "spdlog/fmt/ostr.h"

uint64_t insn_desc::next_uniq(hart_id hart)
{
    constexpr auto uniq_id_width = 23; // same as EVL
    static std::unordered_map<uint32_t, uint32_t> next_ids;

    auto res = next_ids.insert(std::make_pair(hart.u32(), 1));
    auto& cnt = *res.first;

    uint64_t uniq = static_cast<uint64_t>(hart.u32()) << uniq_id_width | cnt.second++;
    if (cnt.second == static_cast<uint32_t>(1) << uniq_id_width) cnt.second = 1; // Overflow

    return uniq;
}

std::ostream& operator<<(std::ostream& os, const insn_desc& desc)
{
    fmt::print(os, "{:010x}-{:010x}", desc.pc_, desc.uniq_);
    return os;
}
