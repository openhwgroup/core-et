// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _ASSERT_
`define _ASSERT_

  `define ASSERT(NAME, TEST, MSG, RESET) \
      static logic val_assert_``NAME;    \
      initial                            \
      begin                              \
         val_assert_``NAME = 1'b0;       \
      end                                \
      always @ (posedge clock)           \
      begin                              \
         if(!RESET && val_assert_``NAME) \
         begin                           \
            assert(TEST)                 \
            else $error(MSG);            \
         end                             \
         else if (RESET)                 \
         begin                           \
            val_assert_``NAME = 1'b1;    \
         end                             \
      end


   `define KILL(MSG) \
      $display("%s\n", MSG); \
      $fatal;

`endif // _ASSERT_
