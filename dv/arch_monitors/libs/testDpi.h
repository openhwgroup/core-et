/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TEST_DPI_H_
#define  _TEST_DPI_H_

#include <svdpi.h>

// macros to bind c++ methods + objects with SV modules via DPI

#ifdef __cplusplus
#define EXTERN_DPI extern "C"
#else
#define EXTERN_DPI 
#endif

EXTERN_DPI void setTimeOut();
EXTERN_DPI void setEnded();

// macros for dpi import context functions
#define DPI_IMPORT(TYPE, DPI_NAME, DPI_PARAMS, CXX_CLASS, CXX_METHOD, CXX_METHOD_ARGS) \
EXTERN_DPI TYPE DPI_NAME DPI_PARAMS; \
TYPE DPI_NAME DPI_PARAMS { \
  svScope s = svGetScope(); \
  if (s != NULL ) { \
    CXX_CLASS *p = (CXX_CLASS*) testMain::getMod(s); \
    if (p != NULL) return p->CXX_METHOD CXX_METHOD_ARGS; \
    else { \
      testLog log;                              \
      const char *path = svGetNameFromScope(s); \
      log<<LOG_FTL<<__PRETTY_FUNCTION__<<" C++ not bound to module in "<<path<<endm; \
    } \
  } \
  else { \
    testLog log; \
    log<<LOG_FTL<<__PRETTY_FUNCTION__<<"bad svScope"<<endm; \
  } \
  return ((TYPE) 0); \
}

// for non context dpi imports, bound to a singletone class. The class needs to have the ' xxx& xxx::getInstance' method
#define DPI_IMPORT_NOCONTEXT(TYPE, DPI_NAME, DPI_PARAMS, CXX_CLASS, CXX_METHOD, CXX_METHOD_ARGS) \
  EXTERN_DPI TYPE DPI_NAME DPI_PARAMS;					\
  TYPE DPI_NAME DPI_PARAMS {						\
    CXX_CLASS &p = CXX_CLASS::getInstance();				\
    return p.CXX_METHOD CXX_METHOD_ARGS;				\
  }


#define DPI_EXPORT(TYPE, DPI_NAME, CXX_PARAMS, CXX_CLASS, CXX_METHOD, DPI_ARGS) \
  EXTERN_DPI TYPE DPI_NAME CXX_PARAMS;                                  \
TYPE  CXX_CLASS::CXX_METHOD CXX_PARAMS \
{ \
  svSetScope(scope_); \
  return ::DPI_NAME DPI_ARGS;			\
}

// conversion to and from sv dpi vector types
class dpi2cpp {
public:
  dpi2cpp(const svBitVecVal *v, int lsb=0, int width=0) :
    v_(v),
    lsb_(lsb),
    width_( width)
  {   } 
  
  template<typename T>
  operator T( ) {
    T ret = 0;
    const uint32_t wIn = sizeof(svBitVecVal)*8;
    uint32_t width = (width_==0)? sizeof(T)*8: width_;
    uint32_t  i = 0;
    // align to 32bits
    if ( lsb_ % wIn  > 0) {
      uint32_t alignBits = wIn - lsb_%wIn > width ? width : wIn - lsb_%wIn ;
      ret = (v_[ ( i + lsb_ ) / wIn ] >> (lsb_ % wIn) ) &  ((1<<alignBits)-1);
      i = wIn - lsb_ % wIn;
    }
    svBitVecVal *ptr =( (svBitVecVal*) v_) + ( (i+lsb_) / wIn);
    while( i < width) {
      svBitVecVal next = *ptr;
      if ( width - i < wIn) {
        next &= (1<<(width-i))-1;
      }
      ret |= ((T)next)<<i;
      ptr++;
      i+=wIn;
    }
    return ret;
  }
private:
  const svBitVecVal *v_;
  const int lsb_;
  const int width_;
  inline int getBit(int b){
    int w = sizeof(svBitVecVal)*8;
    int bit = b % w;
    int word = b / w;
    return (v_[word] >> bit) & 1;
  }
};

// note: allocated memory if v==NULL
void cpp2dpi(int width, const void* in, svBitVecVal* &v); 

// v must already be allocated
void setBits2dpi(int lsb, int width, const void*in, svBitVecVal* &v);
#endif
