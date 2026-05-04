// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0


`ifndef __MACROS_LEGACY__
`define __MACROS_LEGACY__
`define ust_psel(x, y, z) ($signed((x >> (y * z)) & ((1 << z) - 1)))
`define ust_psum4(x, y) (`ust_psel(x, 0, y) + `ust_psel(x, 1, y) + \
                         `ust_psel(x, 2, y) + `ust_psel(x, 3, y))
`endif
