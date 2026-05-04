/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#pragma once

#include <cmath>
#include <cstddef>
#include <limits>

struct stats_t {
    static constexpr double nan = std::nan("");

    void sample(double v);
    void reset();

    size_t count() const { return count_; }
    double sum() const { return sum_; }
    double mean() const;
    double min() const;
    double max() const;
    double stdev() const;

private:
    size_t count_;
    double sum_;
    double mean_;
    double min_ = std::numeric_limits<double>::max();
    double max_ = std::numeric_limits<double>::min();
    double M2_;
};
