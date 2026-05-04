/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_TCONV_H
#define _ET_TCONV_H

#include "snp_common.h"
#include "cpu_resource.h"
#include "lib_snip_transform.h"

unum_t et_urandrange(CpuResource * cpu_resource, const unum_t is_glbl, const unum_t head, const unum_t tail);
unum_t et_randnum(CpuResource * cpu_resource, const unum_t is_glbl);
unum_t __get_tensor_conv_size(CpuResource * cpu_resource, const unum_t is_glbl);
unum_t __get_tensor_conv_ctrl(CpuResource * cpu_resource, unum_t tensor_conv_size, const unum_t is_glbl);
unum_t glbl_snp_get_tensor_conv_size(CpuResource * cpu_resource);
unum_t snp_get_tensor_conv_size(CpuResource * cpu_resource);
unum_t glbl_snp_get_tensor_conv_ctrl(CpuResource * cpu_resource, unum_t tensor_conv_size);
unum_t snp_get_tensor_conv_ctrl(CpuResource * cpu_resource, unum_t tensor_conv_size);

#endif
