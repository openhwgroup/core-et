/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "recorder.h"

#define IMPL_CALLBACK(EVT)                                                                                             \
    void recorder::on(const cosim_evt::EVT##_t* evt)                                                                   \
    {                                                                                                                  \
        save_(cereal::make_nvp("event_id", evt->event_id()));                                                          \
        save_(cereal::make_nvp(#EVT, *evt));                                                                           \
    }
COSIM_EVT_FOREACH(IMPL_CALLBACK)
#undef IMPL_CALLBACK
