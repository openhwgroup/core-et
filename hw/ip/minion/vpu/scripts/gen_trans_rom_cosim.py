# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""Generate tracked rtlcosim directories for trans ROM helper modules."""

from __future__ import annotations

import json
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[5]
MANIFEST = REPO_ROOT / "hw/ip/minion/vpu/data/trans_rom/manifest.json"
COSIM_ROOT = REPO_ROOT / "dv/rtlcosim"


MAKEFILE_TEMPLATE = """REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := {module_name}

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \\
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \\
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/{module_name}.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/{module_name}.v
ORIG_RENAME := {module_name}

include $(REPO_ROOT)/mk/rtlcosim.mk
"""


TB_TEMPLATE = """// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module cosim_{module_name}_tb
  import minion_pkg::*;
  import vpu_pkg::*;
(
  input  logic                                 clk_i,
  input  logic                                 rst_ni,
  input  trans_rom_index_t                     index_i,
  output logic [$bits(trans_fma_coefficients_t)-1:0] orig_data_o,
  output logic [$bits(trans_fma_coefficients_t)-1:0] new_data_o
);

  /* verilator lint_off UNUSED */  // CosimCtrl drives a dummy clock/reset even though this ROM helper is combinational.
  logic unused_ok;
  assign unused_ok = &{{1'b0, clk_i, rst_ni}};
  /* verilator lint_on UNUSED */

  trans_fma_coefficients_t orig_data;
  trans_fma_coefficients_t new_data;

  {module_name}_orig u_orig (
    .index (index_i),
    .data  (orig_data)
  );

  {module_name} u_new (
    .index_i (index_i),
    .data_o  (new_data)
  );

  assign orig_data_o = orig_data;
  assign new_data_o = new_data;

endmodule
"""


TEST_TEMPLATE = """// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#include "Vcosim_{module_name}_tb.h"
#include "cosim_ctrl.h"

using DUT = Vcosim_{module_name}_tb;

int main(int argc, char** argv) {{
    CosimCtrl<DUT> sim(argc, argv);

    sim.dut->index_i = 0;
    sim.tick();
    sim.compare("data", sim.dut->orig_data_o, sim.dut->new_data_o);

    for (int i = 0; i < 256; ++i) {{
        sim.dut->index_i = i;
        sim.tick();
        sim.compare("data", sim.dut->orig_data_o, sim.dut->new_data_o);
    }}

    sim.check(sim.comparisons >= 257,
              "trans ROM cosim should compare the reset sample plus every 8-bit index");
    return sim.finish();
}}
"""


def write_text(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")


def main() -> int:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    for module_name in sorted(manifest["modules"].keys()):
        cosim_dir = COSIM_ROOT / module_name
        write_text(cosim_dir / "Makefile", MAKEFILE_TEMPLATE.format(module_name=module_name))
        write_text(
            cosim_dir / f"cosim_{module_name}_tb.sv",
            TB_TEMPLATE.format(module_name=module_name),
        )
        write_text(
            cosim_dir / f"cosim_{module_name}_test.cc",
            TEST_TEMPLATE.format(module_name=module_name),
        )

    print(f"Generated {len(manifest['modules'])} trans ROM cosim directorie(s) under {COSIM_ROOT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
