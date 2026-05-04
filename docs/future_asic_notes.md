# Future ASIC Notes

## `prim_eco_ports`

`prim_eco_ports` is intentionally introduced as a technology primitive even
though its generic behavior is trivial:

- `eco_o` tied low
- `eco_i` preserved in the logic cone

That is enough for simulation and FPGA builds, but the original CORE-ET
`et_eco_ports` wrapper also had a std-cell specialization that preserved these
spare pins behind dedicated buffers. If and when the ASIC flow needs the same
behavior, add a `tech_asic/prim_eco_ports/rtl/prim_eco_ports.sv` override that
implements the required spare/ECO buffering contract without changing the
module interface.
