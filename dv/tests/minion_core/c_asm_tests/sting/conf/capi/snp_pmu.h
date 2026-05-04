/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_PMU_H
#define _ET_PMU_H

#include "cpu_resource.h"

unum_t snp_get_pmu_event(CpuResource * cpu_resource, unum_t cntr_id);

#endif
