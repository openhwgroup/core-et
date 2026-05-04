/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "stats.h"

#include <algorithm>

void stats_t::sample(double v)
{
    count_ += 1;
    const double delta = v - mean_;
    mean_ += delta / count_;
    const double delta2 = v - mean_;
    M2_ += delta * delta2;
    min_ = std::min(v, min_);
    max_ = std::max(v, max_);
    sum_ += v;
}

double stats_t::mean() const
{
    return count_ > 0 ? mean_ : nan;
}

double stats_t::min() const
{
    return count_ > 0 ? min_ : nan;
}

double stats_t::max() const
{
    return count_ > 0 ? max_ : nan;
}

double stats_t::stdev() const
{
    return count_ > 1 ? std::sqrt(M2_ / count_) : std::nan("");
}

void stats_t::reset()
{
    count_ = 0;
    sum_ = 0;
    mean_ = 0;
    min_ = std::numeric_limits<double>::max();
    max_ = std::numeric_limits<double>::min();
    M2_ = 0;
}
