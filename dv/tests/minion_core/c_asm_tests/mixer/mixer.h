/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

typedef enum
{
    USES_UNKNOWN, 
    USES_ALL_SHIRES,    // virus70: all shires, all minions, t0 and t1 used (through some t1's are free)
    USES_ALL_SHIRES_T0, // sparseNN:  all shires, all minions, t0 used, t1 available
    USES_ONE_SHIRE,     // one shire, all minions, t0 and t1 used
    USES_ONE_SHIRE_T0,  // one shire, all minions, only t0 used, t1 is available
    USES_ONE_MINION,    // one minion, t0 and t1 used
    USES_ONE_THREAD,    // one thread, can ran on either t0 or t1
    USES_ONE_THREAD_T0, // one thread, must run on t0
    USES_ONE_THREAD_T1  // one thread, must run on t1
} kernel_usage_t;

typedef struct
{
    int (*pc)();
    const char *name;
    int type;
} kernel_info_t;

#include "kernels.h"



