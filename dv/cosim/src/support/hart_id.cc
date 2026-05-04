/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "hart_id.h"

#include <spdlog/fmt/ostr.h>

std::ostream& operator<<(std::ostream& os, const hart_id& h)
{
    fmt::print(os, "[H{}:S{}:N{}:C{}:T{}]", h.u32(), h.shire_id(), h.neigh_in_shire(), h.minion_in_neigh(),
               h.thread_in_minion());
    return os;
}
