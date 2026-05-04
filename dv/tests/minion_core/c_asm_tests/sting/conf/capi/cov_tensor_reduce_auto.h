/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_TR_AUTO_COV_H
#define _ET_TR_AUTO_COV_H

#include "cpu_resource.h"
#include "lib_snip_transform.h"

unum_t cov_tensor_reduce_start_reg(CpuResource * cpu_resource, unum_t csr_value);
unum_t glbl_cov_tensor_reduce_num_reg(CpuResource * cpu_resource, unum_t csr_value);
#endif
