/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _RAND_TABLES_H_
#define _RAND_TABLES_H_

#include <inttypes.h>
#include <array>
////////////////////////////////////////////////////////////////////////////////
// random seed
////////////////////////////////////////////////////////////////////////////////

// if RAND_TABLES_SEED is defined, use it.. otherwise take it from current compilation time
#ifndef RAND_TABLES_SEED
constexpr char inits[] = __TIME__;
const int defaultseed = (inits[0]-'0')*100000+(inits[1]-'0')*10000 +
  (inits[3]-'0')*1000+(inits[4]-'0')*100+
  (inits[6]-'0')*10+inits[7]-'0';
#else
#define defaultseed RAND_TABLES_SEED
#endif

////////////////////////////////////////////////////////////////////////////////
// random engines
// they need to have a val (to return the next value) and next (to return a
// type to the next value
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// simple linear congruential engine
// constexpr val and typedef next must be defined
////////////////////////////////////////////////////////////
template <uint32_t state = defaultseed>
struct linear_congruential_engine{
  
  static constexpr uint32_t next_state() { return (state * 1103515245 + 12345) & 0x7fffffff; }
  static constexpr uint32_t next_val() { return next_state();}
  
  static constexpr uint32_t val = next_val();
  typedef linear_congruential_engine<next_state()>  next;

};

typedef linear_congruential_engine<> default_rand_engine;
typedef uint32_t default_rand_t;

////////////////////////////////////////////////////////////
// bag engine
////////////////////////////////////////////////////////////
template<typename val_t, val_t value, unsigned weight>
struct bag_bin {
  // the bins
  template<unsigned count, val_t val, unsigned idx=count-1, val_t...elements>
  struct bin : bin<count, val, idx-1, val, elements...> {};
  
  template<unsigned count, val_t val, val_t...elements>
  struct bin<count, val, 0, elements...>{
    static constexpr std::array<val_t, count> data_ = {val, elements...};
  };

  static constexpr std::array<val_t, weight> data_ = bin<weight, value>::data_;
};


template <uint32_t state, typename val_t, class first_bin,  class ...bins>
struct bag_engine : bag_engine <state, val_t, bins...> {

  template <unsigned...Is> struct seq {};
  template <unsigned N, unsigned...Is> struct gen_seq : gen_seq <N-1, N-1, Is...>{};
  template <unsigned...Is> struct gen_seq<0, Is...> : seq<Is...> {};
  
  //concat 2 arrays
  template<size_t N1, size_t N2>
  static constexpr std::array<val_t, N1 + N2> concat( std::array<val_t, N1> a1, std::array<val_t, N2> a2){
    return concat(a1, a2, gen_seq<N1>{}, gen_seq<N2>{});
  }
  template<size_t N1, unsigned... I1, size_t N2, unsigned... I2>
  static constexpr std::array<val_t, N1 + N2> concat( std::array<val_t, N1> a1, std::array<val_t, N2> a2, seq<I1...>, seq<I2...>){
    return { std::get<I1>(a1)..., std::get<I2>(a2)... };
  }

  static constexpr auto data_ = concat(first_bin::data_, bag_engine<state, val_t, bins...>::data_);

  static constexpr val_t next_val() { return data_[next_state() % data_.size()]; }
  static constexpr uint32_t next_state() { return linear_congruential_engine<state>::next_state();}

  static constexpr val_t val = next_val();
  typedef bag_engine<next_state(), val_t, first_bin, bins...>  next;

};


template <uint32_t state, typename val_t, class last_bin>
struct bag_engine<state, val_t, last_bin> {
  static constexpr auto data_ = last_bin::data_;
  
  static constexpr val_t next_val() { return data_[next_state() % data_.size()]; }
  static constexpr uint32_t next_state() { return linear_congruential_engine<state>::next_state();}
  
  static constexpr val_t val = next_val();
  typedef bag_engine<next_state(), val_t, last_bin>  next;

};
  


////////////////////////////////////////////////////////////////////////////////
// generic table
////////////////////////////////////////////////////////////////////////////////
template<int TABLE_SIZE, typename generator = default_rand_engine, typename val_t = default_rand_t >
class rand_table {
public:
  rand_table() : idx_(0) {}
  
  virtual val_t next(){
    val_t ret = table_<generator>::get(idx_);
    idx_=(idx_+1) % TABLE_SIZE;
    return ret;
  }

  static constexpr val_t get(const unsigned idx) {
    return table_<generator>::get(idx%TABLE_SIZE);
  }
  
protected:
  unsigned idx_;
private:
  // generic table
  template<typename generator_, unsigned IDX=0, val_t ...elements>
  struct table_ : table_ <typename generator_::next, IDX+1, elements..., generator_::val> { };

  //table specialization, on the last element
  template<typename generator_, val_t ...elements>
  struct table_<generator_, TABLE_SIZE, elements...> {
    constexpr static val_t get(const unsigned idx) {
      constexpr std::array<val_t, TABLE_SIZE> data_ = { elements...};
      return data_[idx%TABLE_SIZE];
    }
  };

};
  
////////////////////////////////////////////////////////////////////////////////
// table specializations
////////////////////////////////////////////////////////////////////////////////
template <int TABLE_SIZE, uint32_t min, uint32_t max, uint32_t seed = defaultseed>
class rand_table_range : public rand_table<TABLE_SIZE, linear_congruential_engine<seed> > {
public:
  typedef rand_table<TABLE_SIZE, linear_congruential_engine<seed> > table;
  uint32_t next() override{
    return table::next() % (max-min+1)+min;
  }
  static constexpr uint32_t get(const unsigned idx) {
    return table::get(idx)  % (max-min+1)+min;
  }

  
};

template <int TABLE_SIZE, unsigned mask_size, typename mask_t = uint32_t, uint32_t seed = defaultseed,   class ...bins>
class rand_mask_bag : public rand_table<TABLE_SIZE, bag_engine<seed, uint32_t, bins...>, mask_t> {
public:
  typedef rand_table<TABLE_SIZE, bag_engine<seed, uint32_t, bins...>, mask_t> table;

  mask_t next() override{
    return create_mask(table::next(), table::idx_);
  }

  static constexpr mask_t get(const unsigned idx) {
    return create_mask(table::get(idx%TABLE_SIZE), idx);
  }
private:
  static constexpr mask_t create_mask(uint32_t n_on, unsigned idx){
    if ( n_on >= mask_size) return static_cast<mask_t>(-1) & ((1<<mask_size)-1);
    mask_t mask = (1ULL << n_on)-1;
    for ( uint32_t i = mask_size-1 ; i > 0; i--) {
      uint32_t j = rand_table<mask_size,linear_congruential_engine<seed> >::get(idx + i) % (i+1);
      mask = (mask & ~( (1ULL<<i) | (1ULL<<j) )) |
        (((mask >> i) & 1) << j) |
        (((mask >> j) & 1) << i);
    }
    return mask;
  }
};




#endif
