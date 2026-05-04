/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#pragma once

#include "et_test_common.h"

#ifndef LOGLEVEL
#define LOGLEVEL 0
#endif

#define PRINTF et_printf_long

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

#ifdef LOG_NOCOLORS
  #define log_debug(MSG, ...) PRINTF( "DEBUG " MSG " at %s (%s:%d)\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
  #define log_error(MSG, ...) PRINTF( "ERR   " MSG " at %s (%s:%d)\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
  #define log_warn(MSG, ...)  PRINTF( "WARN  " MSG " at %s (%s:%d)\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
  #define log_info(MSG, ...)  PRINTF( "INFO  " MSG " at %s (%s:%d)\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
#else
  #define log_debug(MSG, ...) PRINTF( "\33[34mDEBUG\33[39m " MSG "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
  #define log_error(MSG, ...) PRINTF( "\33[31mERR\33[39m   " MSG "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
  #define log_warn(MSG, ...)  PRINTF( "\33[91mWARN\33[39m  " MSG "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
  #define log_info(MSG, ...)  PRINTF( "\33[32mINFO\33[39m  " MSG "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
#endif

#if LOGLEVEL < 4
#undef log_debug
#define log_debug(MSG, ...)
#endif

#if LOGLEVEL < 3
#undef log_info
#define log_info(MSG, ...)
#endif

#if LOGLEVEL < 2
#undef log_warn
#define log_warn(MSG, ...)
#endif

#if LOGLEVEL < 1
#undef log_error
#define log_error(MSG, ...)
#endif

#pragma GCC diagnostic pop

