/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_GSC_H
#define _ET_GSC_H

#include "cpu_resource.h"

unum_t snp_get_gsc_global_alignment(CpuResource * cpu_resource, unum_t op_type);
unum_t snp_get_gsc_offset_base(CpuResource * cpu_resource, unum_t op_alignment, unum_t must_trap, unum_t gsc_mask, unum_t lane, unum_t base_align);
unum_t snp_get_gsc_offset(CpuResource * cpu_resource, unum_t op_type, unum_t gsc_mask, unum_t lane, unum_t base_align);
unum_t snp_get_gsc_offset_notrap(CpuResource * cpu_resource, unum_t op_type, unum_t gsc_mask, unum_t lane, unum_t base_align);

#endif
