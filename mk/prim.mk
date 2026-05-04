# Technology-specific primitive selection.
#
# Set TECH before including to select implementations:
#   generic  — behavioral (simulation, default)
#   ice40    — Lattice iCE40 FPGA
#   ecp5     — Lattice ECP5 FPGA
#   xilinx   — Xilinx 7-series / Ultrascale+
#
# True primitives (technology-dependent) live under:
#   hw/ip/tech_generic/<module>/rtl/   — behavioral (simulation, default)
#   hw/ip/tech_ice40/<module>/rtl/     — iCE40 FPGA
#   hw/ip/tech_xilinx/<module>/rtl/    — Xilinx FPGA
#
# Library IP (pure RTL, no tech variation) stays under:
#   hw/ip/prim_*/rtl/         — same everywhere
#
# Usage in a project's srcs.mk:
#   TECH := xilinx
#   include $(REPO_ROOT)/mk/prim.mk
#   RTL_SRCS := ... $(PRIM_CLK_GATE) $(PRIM_WRITE_PREVIEW_EN) $(PRIM_PHASE_PAIR_LO_HI) $(PRIM_RAM_1P) ...

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)
TECH      ?= generic

PRIM_GENERIC := $(REPO_ROOT)/hw/ip/tech_generic
PRIM_TECH    := $(REPO_ROOT)/hw/ip/tech_$(TECH)
PRIM_LIB     := $(REPO_ROOT)/hw/ip

select_tech_prim = $(firstword $(wildcard $(PRIM_TECH)/$(1)/rtl/$(1).sv) \
                               $(PRIM_GENERIC)/$(1)/rtl/$(1).sv)

# ── True primitives (technology-dependent) ──────────────
# Each primitive falls back to tech_generic when the selected TECH has no
# target-specific implementation yet.

PRIM_CLK_GATE       := $(call select_tech_prim,prim_clk_gate)
PRIM_CLK_GATE_N     := $(call select_tech_prim,prim_clk_gate_n)
PRIM_CLK_MUX        := $(call select_tech_prim,prim_clk_mux)
PRIM_WRITE_PREVIEW_EN := $(call select_tech_prim,prim_write_preview_en)
PRIM_WRITE_COMMIT_EN := $(call select_tech_prim,prim_write_commit_en)
PRIM_WRITE_COMMIT_RST_EN := $(call select_tech_prim,prim_write_commit_rst_en)
PRIM_PHASE_PAIR_LO_HI := $(call select_tech_prim,prim_phase_pair_lo_hi)
PRIM_PHASE_PAIR_HI_LO := $(call select_tech_prim,prim_phase_pair_hi_lo)
PRIM_MUL_DIV         := $(call select_tech_prim,prim_mul_div)
PRIM_MUL_DIV_SRCS    := $(PRIM_MUL_DIV) \
                        $(wildcard $(dir $(PRIM_MUL_DIV))intpipe_mul_div_dp.sv) \
                        $(wildcard $(dir $(PRIM_MUL_DIV))intpipe_mul_div_ctl.sv)
PRIM_RF_1R1W_PREVIEW := $(call select_tech_prim,prim_rf_1r1w_preview)
PRIM_RF_1R1W_DEC_PREVIEW := $(call select_tech_prim,prim_rf_1r1w_dec_preview)
PRIM_RF_1R1W_PAR_PREVIEW := $(call select_tech_prim,prim_rf_1r1w_par_preview)
PRIM_RF_1R1W_DIFF_PREVIEW := $(call select_tech_prim,prim_rf_1r1w_diff_preview)
PRIM_RF_1R1W_REG_PREVIEW := $(call select_tech_prim,prim_rf_1r1w_reg_preview)
PRIM_RF_SINGLE_1R1W_PAR_PREVIEW := $(call select_tech_prim,prim_rf_single_1r1w_par_preview)
PRIM_RF_2R1W_PREVIEW := $(call select_tech_prim,prim_rf_2r1w_preview)
PRIM_RF_3R2W_PREVIEW := $(call select_tech_prim,prim_rf_3r2w_preview)
PRIM_RAM_1P         := $(call select_tech_prim,prim_ram_1p)
PRIM_RAM_2P         := $(call select_tech_prim,prim_ram_2p)
PRIM_RST_SYNC       := $(call select_tech_prim,prim_rst_sync)
PRIM_FIFO_ASYNC_HIV := $(call select_tech_prim,prim_fifo_async_hiv)
PRIM_FIFO_ASYNC_LOV := $(call select_tech_prim,prim_fifo_async_lov)
PRIM_ECO_PORTS      := $(call select_tech_prim,prim_eco_ports)
PRIM_RF_1R1W        := $(call select_tech_prim,prim_rf_1r1w)
PRIM_RF_1R1W_REG    := $(call select_tech_prim,prim_rf_1r1w_reg)
PRIM_RF_1R1W_PAR    := $(call select_tech_prim,prim_rf_1r1w_par)
PRIM_RF_1R1W_DEC    := $(call select_tech_prim,prim_rf_1r1w_dec)
PRIM_RF_1R2W_PAR    := $(call select_tech_prim,prim_rf_1r2w_par)
PRIM_RF_1R1W_DIFF   := $(call select_tech_prim,prim_rf_1r1w_diff)
PRIM_RF_2R1W        := $(call select_tech_prim,prim_rf_2r1w)
PRIM_RF_SINGLE_1R1W_PAR := $(call select_tech_prim,prim_rf_single_1r1w_par)
PRIM_RF_3R2W        := $(call select_tech_prim,prim_rf_3r2w)

# ── Library IP (pure RTL, same on all targets) ─────────

PRIM_ECC_ENC    := $(PRIM_LIB)/prim_ecc/rtl/prim_ecc_enc.sv
PRIM_ECC_DEC    := $(PRIM_LIB)/prim_ecc/rtl/prim_ecc_dec.sv
PRIM_ARB_LRU      := $(PRIM_LIB)/prim_arb_lru/rtl/prim_arb_lru.sv
PRIM_ARB_LRU_2BID := $(PRIM_LIB)/prim_arb_lru/rtl/prim_arb_lru_2bid.sv
PRIM_ARB_RR       := $(PRIM_LIB)/prim_arb_rr/rtl/prim_arb_rr.sv
PRIM_ARB_RR2      := $(PRIM_LIB)/prim_arb_rr2/rtl/prim_arb_rr2.sv
PRIM_ARB_PRIO     := $(PRIM_LIB)/prim_arb_prio/rtl/prim_arb_prio.sv
PRIM_CMP_32       := $(PRIM_LIB)/prim_cmp/rtl/prim_cmp_32.sv
PRIM_CMP_42       := $(PRIM_LIB)/prim_cmp/rtl/prim_cmp_42.sv
PRIM_FIFO_REG     := $(PRIM_LIB)/prim_fifo_reg/rtl/prim_fifo_reg.sv
PRIM_FIFO         := $(PRIM_LIB)/prim_fifo/rtl/prim_fifo.sv
PRIM_FIFO_SYNC    := $(PRIM_LIB)/prim_fifo_sync/rtl/prim_fifo_sync.sv
PRIM_FIFO_SRAM    := $(PRIM_LIB)/prim_fifo_sram/rtl/prim_fifo_sram.sv
PRIM_HOT2BIN      := $(PRIM_LIB)/prim_hot2bin/rtl/prim_hot2bin.sv
