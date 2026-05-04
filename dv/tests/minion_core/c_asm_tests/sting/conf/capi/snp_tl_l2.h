/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _ET_TL_L2_H
#define _ET_TL_L2_H

#include "lib_snip_transform.h"
#include "cpu_resource.h"
unum_t snp_set_valid_num_scp_lines(CpuResource * cpu_resource, unum_t csr_value);
unum_t snp_constrain_l2_lines(CpuResource * cpu_resource, unum_t csr_value);
#endif
