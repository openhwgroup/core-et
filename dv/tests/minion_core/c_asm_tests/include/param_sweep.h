/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _PARAM_SWEEP_H_
#define _PARAM_SWEEP_H_

#include <inttypes.h> 
#include <cstddef>  
#include <array>
////////////////////////////////////////////////////////////////////////////////
// Helper class to generate list of tests from a templatized class
// It creates an array of pointers to the function class<params>::run
// by generating all combinations of params as specified with the ranges
////////////////////////////////////////////////////////////////////////////////

template<size_t mxP, size_t mnP = 0, size_t incP = 1>
struct range {
  static constexpr size_t mn = mnP;
  static constexpr size_t mx = mxP;
  static constexpr size_t inc = incP;
  static_assert(mxP >= mnP, "max < min when defining range");
  static_assert(incP != 0, "cannot have 0 inc when defining range");
};



template <uint64_t seed, size_t idx>
struct randVal {
  
  static constexpr uint32_t val() {
    uint64_t state = seed;
    for ( size_t i = 0 ; i < idx; i++ )
      state = state * 1103515245 + 12345;
    return((unsigned)(state/65536) % 32768);    
  }
};



template<template <size_t ...testParams> class testClass, typename func_t, class ...ranges>
class make_tests {
private:
  static constexpr size_t ndims_ = sizeof...(ranges);

  template<class T> static constexpr size_t getDim() { 
    constexpr auto n = (T::mx - T::mn) / T::inc + 1;
    static_assert( T::mx >= T::mn, "bad range: max < min!");
    return n;
  }
  static constexpr size_t getStride(const size_t dimIdx) {
    return dimIdx == ndims_ -1 ? 1 : dims_[dimIdx + 1] * getStride(dimIdx + 1);
  }
  template<std::size_t ... dim> 
  static constexpr std::array<size_t, ndims_> getStrides(std::index_sequence<dim...>) {
    return { getStride(dim)... };
  }
  
  static constexpr std::array<size_t, ndims_> dims_ = {getDim<ranges>()... };
  static constexpr std::array<size_t, ndims_> strides_ = getStrides(std::make_index_sequence<ndims_> {});
  static constexpr std::array<size_t, ndims_> mn_ = {ranges::mn ... };
  static constexpr std::array<size_t, ndims_> mx_ = {ranges::mx ... };  
  static constexpr size_t count = dims_[0] * strides_[0];

  static constexpr size_t getParam(const size_t idx, const size_t dim) {
      return mn_[dim] +  (idx / strides_[dim]) % dims_[dim];
  }

  template<uint32_t seed, size_t idx, size_t dim>
  static constexpr size_t getRandParam() {
    constexpr size_t  randIdx = idx * ndims_ + dim;
    return randVal<seed, randIdx>::val() % (mx_[dim] - mn_[dim] +1) + mn_[dim];
  }
  
  template<std::size_t... idx>
  static constexpr std::array<func_t, count>  genList(std::index_sequence<idx...>)
  {
    return { genTest<idx>( std::make_index_sequence<ndims_>{})... };
  }
 
  template<size_t idx, std::size_t ...dims>
  static constexpr func_t genTest(std::index_sequence<dims...>)
  {
    return testClass<getParam(idx,dims)... >::run;
  }

  template<uint32_t seed, size_t N, std::size_t... idx>
  static constexpr std::array<func_t, N>  genRandList(std::index_sequence<idx...>)
  {
    return { genRandTest<seed, idx>( std::make_index_sequence<ndims_>{})... };
  }
  
  template<uint32_t seed, size_t idx, std::size_t ...dims>
  static constexpr func_t genRandTest(std::index_sequence<dims...>)
  {
    return testClass<getRandParam<seed, idx, dims>()...>::run;
  }


public:

  static constexpr std::array<func_t, count> sweep(){
    return genList(std::make_index_sequence<count>{});
  }

  template<size_t rand_count, uint32_t seed>
  static constexpr std::array<func_t, rand_count> random(){
    return genRandList<seed, rand_count>(std::make_index_sequence<rand_count>{});
  }

};
#endif
