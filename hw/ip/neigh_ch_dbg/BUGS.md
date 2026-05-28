// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# neigh_ch_dbg BUGS

## Original repository bug: `neigh_ch_dbg` resume FSM for every minion samples only minion-0 running status bits

- **Original module:** `/home/glguida/ainekko/et-soc/rtl/shire/neigh/neigh_ch_dbg.v`
- **Reimplemented module:** `hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg.sv`
- **Symptom:** A DM or BPAM resume request targeting a hart in minion 1-7 does
  not complete its resume-acknowledge FSM when that targeted minion reports its
  own `debug_minion_status[min_it].running` bit. The FSM transitions out of
  `resume_RESUMING` only when the same thread bit in minion 0 is running. For
  example, a request for minion 3 thread 1 keeps `esr_hastatus0.resumeack[7]`
  low and `debug_minion_req[3].resume[1]` asserted until
  `esr_hastatus0.running[1]` (minion 0 thread 1) is high.
- **Root cause:** Inside the original generate block `MINION_DBG_REG`, the
  resume-state comb logic indexes the flat HASTATUS running vector by local
  thread index only:
  - `if ( esr_hastatus0.running[i] ) begin`
  It omits the minion offset used elsewhere in the same module, such as
  `esr_hastatus0.running[\`CORE_NR_THREADS*min_it +: \`CORE_NR_THREADS] =
  debug_out_struct.running;`. Therefore every generated minion FSM observes the
  minion-0 running bits instead of `min_it`'s running bits.
- **Reachability:** Reachable with legal debug traffic whenever a resume request
  targets a nonzero minion and minion 0 does not report the corresponding thread
  as running. The unit test includes a directed check named
  `original minion-local running bug is preserved`; the standalone cosim drives
  random selected-hart and minion-status combinations and passes with zero
  mismatches only when this behavior is preserved.
- **System impact:** Resume acknowledgements for nonzero minions can be delayed
  or withheld based on unrelated minion-0 running state. Debug software polling
  `HASTATUS0.resumeack` may observe a nonzero minion as still resuming even
  after that minion reports running, or may observe completion when only minion 0
  reports the matching thread running.
- **Reimplementation decision:** The current reimplementation intentionally
  **preserves** the original behavior for cycle-by-cycle equivalence. It uses
  `esr_hastatus0.running[thread_idx]` in the translated resume FSM and documents
  the preserved behavior in `README.md`. Any future correctness fix must be an
  intentional divergence with updated documentation and cosim expectations.
