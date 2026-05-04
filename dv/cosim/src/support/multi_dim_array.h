/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _MULTI_DIM_ARRAY_H_
#define _MULTI_DIM_ARRAY_H_

#include <array>

template <typename T, std::size_t I, std::size_t... J>
struct multi_dim_array_impl {
    using nested = typename multi_dim_array_impl<T, J...>::type;
    using type = std::array<nested, I>;
};

template <typename T, std::size_t I>
struct multi_dim_array_impl<T, I> {
    using type = std::array<T, I>;
};

template <typename T, std::size_t I, std::size_t... J>
using multi_dim_array = typename multi_dim_array_impl<T, I, J...>::type;

#endif // _MULTI_DIM_ARRAY_H_
