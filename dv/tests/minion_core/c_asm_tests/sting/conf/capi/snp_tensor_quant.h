/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_TQUANT_H
#define _ET_TQUANT_H

#include "cpu_resource.h"
#include "lib_snip_transform.h"

unum_t snp_set_ntrans_tquant(CpuResource * cpu_resource, unum_t ntrans, unum_t csr_val, unum_t trans_val);
unum_t snp_set_valid_tquant_op(CpuResource * cpu_resource, unum_t rand_val);
unum_t snp_get_tquant_wrapper(CpuResource * cpu_resource);

#endif
