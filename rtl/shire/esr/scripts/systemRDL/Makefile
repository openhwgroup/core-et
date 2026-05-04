# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0
outputs= verilog/esr.sv doc/esr.md
all:$(outputs)
# ../../verilog/esr/neigh.sv:neigh.rdl
# 	peakrdl regblock --cpuif apb4-flat --default-reset arst_n neigh.rdl -o ../../verilog/esr
verilog/esr.sv:esr.rdl
	peakrdl regblock --cpuif apb4-flat --default-reset arst_n esr.rdl -o verilog/esr
doc/esr.md:esr.rdl
	peakrdl markdown  esr.rdl  -o doc/esr.md
# ../../doc/registers.md:top.rdl system.rdl hyperbus.rdl
# 	peakrdl markdown top.rdl  -o ../../doc/registers.md
# 	peakrdl python --async -o ../../copper_digital/testbench top.rdl
# 	peakrdl cocotb_ralgen top.rdl -o ../../copper_digital/testbench
# 	peakrdl c-header top.rdl -b=ltoh -o ../c/erbium.h

# top.rdl:system.rdl hyperbus.rdl
# html:
# 	peakrdl html top.rdl  -o html
clean::
	rm -rf $(outputs)
	rm -rf verilog doc
.PHONY:all clean test
