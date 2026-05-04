/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include "hart_id.h"

class insn_desc {
public:
    insn_desc()
        : pc_(0)
        , uniq_(0)
    {
    }

    insn_desc(hart_id hart, uint64_t pc = 0)
        : pc_(pc)
        , uniq_(next_uniq(hart))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const insn_desc& desc);

    inline uint64_t uniq_id() const noexcept
    {
        return uniq_;
    }

    static uint64_t next_uniq(hart_id hart);

private:
    uint64_t pc_;
    uint64_t uniq_;
};
