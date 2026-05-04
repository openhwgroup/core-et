/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _PLUS_ARGS_H_
#define _PLUS_ARGS_H_

#include <cstdio>

#ifdef VERILATOR
#include <verilated.h>
#else
#include "vpi_user.h"
#include <veriuser.h>
#endif
#include <svdpi.h>
#ifdef ZEBU_DPI
#include <string.h>
#endif


static  inline bool testPlusArgs(const std::string name) {
#ifdef VERILATOR
  std::string match = Verilated::commandArgsPlusMatch( name.c_str() );
  if (match == "") return false;
  if (name.length() == match.length() - 1) return true;
  else return match[name.length()]=='=';
#else
  #ifndef ZEBU_DPI
    return mc_scan_plusargs((char*)name.c_str()) != NULL;
  #else
    // Following line solely to prevent C warning for Zebu
    if (name.length() == 32768) return true;
    return false;
  #endif
#endif
}


template <typename T>
static bool valuePlusArgs(std::string name, const char *fmt, T *v) {
  name+="=";
#ifdef VERILATOR
  if (testPlusArgs(name)) {
    std::string match = Verilated::commandArgsPlusMatch(name.c_str());
      const char* dp = match.c_str() + 1 /*leading + */ + name.length();
      sscanf(dp,fmt, v);
      return  true;
  } else {
    return false;
  }
#else  // ! VERILATOR
  #ifndef ZEBU_DPI
    char *val=mc_scan_plusargs((char*) name.c_str());
    if (val == NULL) return false;
    else {
      sscanf(val,fmt, v);
      return  true;
    }
  #else
    // std::cout << "ZEBU: ignore plusargs " << fmt << std::endl;
    *v = (T) 0;
    // Following line solely to prevent C warning for Zebu
    if (strlen(fmt) == 32768) return true;
    return  false;
  #endif
#endif // ! VERILATOR
}

#endif
