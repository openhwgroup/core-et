# `ptw_top`

Standalone Minion page-table walker.

This is the neighborhood/shared PTW controller used by the original
`standalone_minion` shell. It accepts translated TLB/PTW requests, walks page
tables through the neighborhood ET-Link memory interface, caches non-leaf PTEs,
and returns the resolved PTE payload back to the requestor.

## Ports

- `clk_i`, `rst_ni`
  Clock and active-low asynchronous reset.
- `ioshire_i`
  Marks the IOShire case, which changes PMA legality checks.
- `req_data_i`, `req_valid_i`, `req_ready_o`
  PTW request handshake from a TLB-side client.
- `invalidate_i`
  Invalidate outstanding PTW work and cached entries.
- `mprot_i`, `vmspagesize_i`
  ESR and page-size controls used by the legality/PMA logic.
- `resp_data_o`, `resp_valid_o`
  PTW response PTE and valid pulse.
- `mem_miss_req_disable_next_i`
  Backpressure/disable hint for issuing a new memory request.
- `mem_miss_req_info_o`, `mem_miss_req_valid_o`, `mem_miss_req_ready_i`
  Neighborhood ET-Link miss request channel.
- `mem_fill_req_info_i`, `mem_fill_req_valid_i`, `mem_fill_req_ready_o`
  ET-Link fill/ack response channel.
- `dbg_sm_signals_o`
  PTW debug/status-monitor payload.

## Notes

- Reset polarity is the intentional repo-wide difference from the original.
- The controller preserves the original small PTW-local L1 line cache and
  non-leaf PTE cache behavior.
- Dirty-bit update states remain in the exported debug enum for parity, even
  though this translated controller currently follows the same non-dirty-update
  execution path as the original source file.
