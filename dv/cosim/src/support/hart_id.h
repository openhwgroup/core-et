/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <ostream>
#include <cstdint>

#include "emu_defines.h"

class hart_id {
public:
    static constexpr auto threads_per_shire = EMU_THREADS_PER_SHIRE;
    static constexpr auto threads_per_neigh = EMU_THREADS_PER_NEIGH;
    static constexpr auto threads_per_minion = EMU_THREADS_PER_MINION;
    static constexpr auto neigh_per_shire = EMU_NEIGH_PER_SHIRE;
    static constexpr auto minions_per_neigh = EMU_MINIONS_PER_NEIGH;

    // Global ID's
    inline uint32_t shire_id() const noexcept
    {
        return hart_ / threads_per_shire;
    }
    inline uint32_t minion_id() const noexcept
    {
        return hart_ / threads_per_minion;
    }

    // Local ID's
    inline uint32_t neigh_in_shire() const noexcept
    {
        return (hart_ / threads_per_neigh) % neigh_per_shire;
    }
    inline uint32_t minion_in_neigh() const noexcept
    {
        return minion_id() % minions_per_neigh;
    }
    inline uint32_t thread_in_minion() const noexcept
    {
        return hart_ % threads_per_minion;
    }

    // Cycle through harts in minion
    inline hart_id other(uint32_t offset = 1) const noexcept
    {
        const auto thread = (thread_in_minion() + offset) % threads_per_minion;
        return hart_id(minion_id(), thread);
    }

    // Constructors
    hart_id(uint32_t hart = 0) : hart_(hart)
    {
    }
    hart_id(uint32_t minion_id, uint32_t thread_in_minion) : hart_((minion_id * threads_per_minion) | thread_in_minion)
    {
    }

    // Default copy/move constructors/operators
    hart_id(const hart_id&) = default;
    hart_id(hart_id&&) = default;
    hart_id& operator=(const hart_id&) = default;
    hart_id& operator=(hart_id&&) = default;

    // Comparison operators
    bool operator==(const hart_id& rhs) const
    {
        return rhs.hart_ == hart_;
    }
    bool operator!=(const hart_id& rhs) const
    {
        return rhs.hart_ != hart_;
    }
    bool operator==(uint32_t rhs) const
    {
        return hart_ == rhs;
    }
    bool operator!=(uint32_t rhs) const
    {
        return hart_ != rhs;
    }

    // Convert to uint32_t representation (for compatibility)
    inline uint32_t u32() const noexcept
    {
        return hart_;
    }

    bool operator<(const hart_id& rhs) const
    {
        return hart_ < rhs.hart_;
    }

    std::string repr() const;

    friend std::ostream& operator<<(std::ostream& os, const hart_id& hart);

private:
    uint32_t hart_;
};
