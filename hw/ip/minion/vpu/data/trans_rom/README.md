# VPU Trans ROM Data

Canonical extracted ROM contents for the `trans_unit` helper modules live here.

The intended flow is:

1. Extract packed coefficient images from the original CORE-ET sources:

```bash
python3 hw/ip/minion/vpu/scripts/extract_trans_rom.py \
  --orig-root ~/ainekko/etcore-soc \
  --out-dir hw/ip/minion/vpu/data/trans_rom
```

2. Generate original-equivalent SystemVerilog helper modules from those packed
   images:

```bash
python3 hw/ip/minion/vpu/scripts/gen_trans_rom_sv.py \
  --hex-dir hw/ip/minion/vpu/data/trans_rom \
  --out-dir /tmp/trans_rom_sv
```

The packed image format is:

`{exc, mask[6:0], taylor, c2[16:0], c0[31:0]}`

These images are intentionally derived from the checked-in original Verilog
case tables rather than from a coefficient-regeneration flow that is not
present in the CORE-ET tree.
