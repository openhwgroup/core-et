// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace txfma_e_model {

constexpr uint32_t kExpPsW = 8;
constexpr uint32_t kExpPhW = 5;
constexpr uint32_t kExpSextW = 10;
constexpr uint32_t kExpMask = (1u << kExpSextW) - 1u;
constexpr uint32_t kExpPsMask = (1u << kExpPsW) - 1u;
constexpr uint32_t kEtrzProdPhMask = (1u << 5) - 1u;
constexpr uint32_t kEtrzProdPsMask = (1u << 6) - 1u;
constexpr uint32_t kDataMask = 0xffffffffu;

constexpr uint32_t RNE = 0;
constexpr uint32_t RDN = 2;
constexpr uint32_t RUP = 3;
constexpr uint32_t RMM = 4;

constexpr uint32_t VPU_DTYPE_INT = 0;
constexpr uint32_t VPU_DTYPE_F32 = 1;
constexpr uint32_t VPU_DTYPE_F16 = 2;
constexpr uint32_t VPU_DTYPE_F11 = 3;
constexpr uint32_t VPU_DTYPE_F10 = 4;
constexpr uint32_t VPU_DTYPE_F16_F32 = 14;

constexpr uint32_t VPU_FCMD_ADD = 0;
constexpr uint32_t VPU_FCMD_SUB = 1;
constexpr uint32_t VPU_FCMD_NR2_FRCPFXP = 13;
constexpr uint32_t VPU_FCMD_MIN = 15;
constexpr uint32_t VPU_FCMD_MAX = 16;
constexpr uint32_t VPU_FCMD_CVT_F32INT = 18;
constexpr uint32_t VPU_FCMD_CVT_INTF32 = 21;
constexpr uint32_t VPU_FCMD_FEQ = 31;
constexpr uint32_t VPU_FCMD_FLE = 32;
constexpr uint32_t VPU_FCMD_FLT = 33;
constexpr uint32_t VPU_FCMD_CVT_F16F32 = 59;
constexpr uint32_t VPU_FCMD_CVT_F11F32 = 60;
constexpr uint32_t VPU_FCMD_CVT_F10F32 = 61;
constexpr uint32_t VPU_FCMD_CVT_F32RAST = 70;
constexpr uint32_t VPU_FCMD_CVT_RASTF32 = 72;
constexpr uint32_t VPU_FCMD_NR1_FRCPFXP = 73;
constexpr uint32_t VPU_FCMD_FFRC = 76;
constexpr uint32_t VPU_FCMD_FROUND = 77;
constexpr uint32_t VPU_FCMD_FFRC_SIN = 81;
constexpr uint32_t VPU_FCMD_CVT_UINTF32 = 88;
constexpr uint32_t VPU_TRANS_RCP_FMA2 = 100;
constexpr uint32_t VPU_TRANS_RSQRT_FMA2 = 103;
constexpr uint32_t VPU_TRANS_LOG_RR = 113;
constexpr uint32_t VPU_TRANS_LOG_MUL = 116;
constexpr uint32_t VPU_TRANS_SIN_TRANSFORM = 117;
constexpr uint32_t VPU_TRANS_SIN_P1 = 119;
constexpr uint32_t VPU_TRANS_SIN_P2 = 120;
constexpr uint32_t VPU_TRANS_SIN_P3 = 121;
constexpr uint32_t VPU_TRANS_EXP_FRAC = 122;
constexpr uint32_t VPU_TRANS_EXP_FMA2 = 125;

constexpr uint32_t TXFMA_MUL = 7;
constexpr uint32_t TXFMA_CVT = 8;
constexpr uint32_t TXFMA_F32_FX = 12;
constexpr uint32_t TXFMA_FX_F32 = 13;
constexpr uint32_t TXFMA_F32_NORM = 14;
constexpr uint32_t TXFMA_NORM_F32 = 15;
constexpr uint32_t TXFMA_TRANS1 = 16;
constexpr uint32_t TXFMA_TRANS2 = 17;

constexpr uint32_t FLOAT32_E_BIAS_HEX = 0x7f;
constexpr uint32_t FLOAT32_NEG_E_BIAS_HEX = 0x81;
constexpr uint32_t FLOAT16_32_MAX = 142;
constexpr uint32_t TXFMA_BIAS_DIFF = 112;
constexpr uint32_t TXFMA_CVT_F32INT_OFFSET = 31;
constexpr uint32_t TXFMA_CVT_INTF32_OFFSET = 31;
constexpr uint32_t TXFMA_GCVT_F32_NORM_OFFSET = 2;
constexpr uint32_t TXFMA_EXP_FRAC_OFFSET = 7;
constexpr uint32_t TXFMA_CVT_F32_FXP1615_OFFSET = 15;
constexpr uint32_t TXFMA_CVT_FXP1714_F32_OFFSET = 17;
constexpr uint32_t TXFMA_TRANS_RCP_OFFSET = 254;
constexpr uint32_t TXFMA_NORM_F32_BIAS = 158;

constexpr uint32_t TXFMA_F32_A_C_STICKY_DISTANCE = 23;
constexpr uint32_t TXFMA_F32_PROD_STICKY_DISTANCE = 2;
constexpr uint32_t TXFMA_INT_STICKY_DISTANCE = 9;
constexpr uint32_t TXFMA_F16_F32_C_STICKY_DISTANCE = 2;
constexpr uint32_t TXFMA_F16_F32_PROD_STICKY_DISTANCE = 5;
constexpr uint32_t TXFMA_TRANS_EXP_STICKY_DISTANCE = 134;
constexpr uint32_t TXFMA_F16_F32_MAN_DIFF = 0x3f4;
constexpr uint32_t TXFMA_F11_F32_MAN_DIFF = 0x3ef;
constexpr uint32_t TXFMA_F10_F32_MAN_DIFF = 0x3ee;
constexpr uint32_t TXFMA_STICKY_K_F32 = 0x3e8;
constexpr uint32_t TXFMA_STICKY_K_FI = 0x3f0;
constexpr uint32_t TXFMA_STICKY_TO_G_K_F16_L = 0x3e8;
constexpr uint32_t TXFMA_STICKY_TO_R_K_IF = 0x3e8;

constexpr uint32_t TXFMA_SEL_PH = 0;
constexpr uint32_t TXFMA_SEL_PL = 1;
constexpr uint32_t TXFMA_SEL_C = 2;
constexpr uint32_t TXFMA_SEL_A = 3;
constexpr uint32_t TXFMA_SEL_P = 4;
constexpr uint32_t TXFMA_SEL_C_INT = 5;
constexpr uint32_t TXFMA_SEL_CVT_F32INT = 6;
constexpr uint32_t TXFMA_SEL_ZERO = 7;
constexpr uint32_t TXFMA_SEL_P_INT = 9;
constexpr uint32_t TXFMA_SEL_ROUND = 10;
constexpr uint32_t TXFMA_SEL_GCVT_F32_NORM = 11;
constexpr uint32_t TXFMA_SEL_EXP_RES = 12;
constexpr uint32_t TXFMA_SEL_EXP_FRAC = 13;
constexpr uint32_t TXFMA_SEL_CVT_F32_FXP1615 = 14;
constexpr uint32_t TXFMA_SEL_B = 15;

constexpr uint32_t TXFMA_SEL_EDIFF_EC_EPH = 0;
constexpr uint32_t TXFMA_SEL_EDIFF_EC_EPL = 1;
constexpr uint32_t TXFMA_SEL_EDIFF_N_EC_EPH = 2;
constexpr uint32_t TXFMA_SEL_EDIFF_N_EC_EPL = 3;
constexpr uint32_t TXFMA_SEL_EDIFF_EPH_EPL = 4;
constexpr uint32_t TXFMA_SEL_EDIFF_N_EPH_EPL = 5;
constexpr uint32_t TXFMA_SEL_EDIFF_ECH_EPH = 6;
constexpr uint32_t TXFMA_SEL_EDIFF_N_ECH_EPH = 7;
constexpr uint32_t TXFMA_SEL_EDIFF_ZERO = 8;
constexpr uint32_t TXFMA_SEL_EDIFF_LOG_MUL = 9;
constexpr uint32_t TXFMA_SEL_EDIFF_LOG_MUL2 = 10;
constexpr uint32_t TXFMA_SEL_EDIFF_NR2_FRCPFXP = 11;

inline uint32_t mask(unsigned width) {
    return width >= 32 ? 0xffffffffu : ((1u << width) - 1u);
}

inline uint32_t bits(uint32_t value, unsigned width) {
    return value & mask(width);
}

inline bool bit(uint32_t value, unsigned index) {
    return ((value >> index) & 1u) != 0u;
}

inline uint32_t all_bits(bool value, unsigned width) {
    return value ? mask(width) : 0u;
}

inline int32_t signed_value(uint32_t value, unsigned width) {
    const uint32_t m = 1u << (width - 1u);
    const uint32_t v = bits(value, width);
    return static_cast<int32_t>((v ^ m) - m);
}

inline uint32_t neg(uint32_t value, unsigned width = kExpSextW) {
    return bits(0u - bits(value, width), width);
}

inline uint32_t not_w(uint32_t value, unsigned width = kExpSextW) {
    return bits(~value, width);
}

inline uint32_t add_w(unsigned width, uint32_t a, uint32_t b, uint32_t cin = 0) {
    return bits(a + b + cin, width);
}

inline uint32_t add3_w(unsigned width, uint32_t a, uint32_t b, uint32_t c, uint32_t cin = 0) {
    return bits(a + b + c + cin, width);
}

inline uint32_t sign_extend(uint32_t value, unsigned from_width) {
    return bits(static_cast<uint32_t>(signed_value(value, from_width)), kExpSextW);
}

inline uint32_t half_to_single_exp(uint32_t exp5, bool zero_aware, bool is_zero) {
    const bool msb = bit(exp5, 4);
    const uint32_t repl = (!msb && !(zero_aware && is_zero)) ? 0x70u : 0u;
    return bits((static_cast<uint32_t>(msb) << 7) | repl | (exp5 & 0x0fu), kExpPsW);
}

inline bool sig(uint32_t sigs, uint32_t idx) {
    return bit(sigs, idx);
}

struct BitCoverage {
    std::string name;
    uint32_t width;
    uint64_t expected_mask;
    uint64_t or_mask = 0;
    uint64_t and_mask;

    BitCoverage(std::string n, uint32_t w)
        : name(std::move(n)), width(w), expected_mask(w >= 64 ? ~0ull : ((1ull << w) - 1ull)),
          and_mask(expected_mask) {}

    void observe(uint64_t value) {
        const uint64_t v = value & expected_mask;
        or_mask |= v;
        and_mask &= v;
    }
};

class Coverage {
public:
    int add(const std::string& name, uint32_t width) {
        ports_.emplace_back(name, width);
        return static_cast<int>(ports_.size() - 1u);
    }

    void observe(int index, uint64_t value) {
        ports_.at(static_cast<size_t>(index)).observe(value);
    }

    template <typename Sim>
    void check(Sim& sim) const {
        for (const BitCoverage& port : ports_) {
            sim.check(port.or_mask == port.expected_mask,
                      port.name + " should drive every input bit high");
            sim.check(port.and_mask == 0ull,
                      port.name + " should drive every input bit low");
        }
    }

private:
    std::vector<BitCoverage> ports_;
};

inline uint32_t xorshift32(uint32_t& state) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

struct E1In {
    uint32_t op_dtype = VPU_DTYPE_F32;
    uint32_t sigs = 0;
    uint32_t cmd = VPU_FCMD_ADD;
    uint32_t ea_hi = 0;
    uint32_t eb_hi = 0;
    uint32_t ea_lo = 0;
    uint32_t eb_lo = 0;
    uint32_t ec = 0;
    bool ec_zero = false;
    bool ec_all1 = false;
    bool ea_hi_zero = false;
    uint32_t etrz_fa_hi_ph = 0;
    uint32_t etrz_fa_lo_ps = 0;
    uint32_t etrz_fb_hi_ph = 0;
    uint32_t etrz_fb_lo_ps = 0;
    bool fc_zero30_8 = false;
};

struct E1Out {
    uint32_t etrz_prod_hi_ph;
    uint32_t etrz_prod_lo_ps;
    uint32_t ediff_ec_eph;
    uint32_t ediff_ec_epl;
    uint32_t ediff_eph_epl;
    uint32_t eprod_hi;
    uint32_t eprod_lo;
    uint32_t ec_out;
};

inline E1Out model_e1(const E1In& in) {
    const uint32_t cmd = bits(in.cmd, 7);
    const uint32_t dtype = bits(in.op_dtype, 4);
    const uint32_t sigs = bits(in.sigs, 19);
    const uint32_t ea_hi = bits(in.ea_hi, 8);
    const uint32_t eb_hi = bits(in.eb_hi, 8);
    const uint32_t ea_lo = bits(in.ea_lo, 5);
    const uint32_t eb_lo = bits(in.eb_lo, 5);
    const uint32_t ec = bits(in.ec, 8);

    uint32_t trans_log_mul_inc_dec = 0;
    const bool ec_gt127 = ec >= 127u;
    if (ec_gt127) {
        trans_log_mul_inc_dec = 1;
    } else if (!in.fc_zero30_8) {
        trans_log_mul_inc_dec = kExpMask;
    }
    const uint32_t trans_log_mul_ec = ec_gt127 ? ec : bits((0x7u << 7) | (~ec & 0x7fu), kExpSextW);

    uint32_t ea_hi_mux = 0;
    if (cmd == VPU_FCMD_CVT_INTF32 || cmd == VPU_FCMD_CVT_UINTF32 || cmd == VPU_FCMD_FROUND) {
        ea_hi_mux = TXFMA_CVT_INTF32_OFFSET;
    } else if (cmd == VPU_FCMD_CVT_RASTF32) {
        ea_hi_mux = TXFMA_CVT_FXP1714_F32_OFFSET;
    } else if (cmd == VPU_TRANS_RCP_FMA2) {
        ea_hi_mux = TXFMA_TRANS_RCP_OFFSET;
    } else if (cmd == VPU_TRANS_RSQRT_FMA2) {
        ea_hi_mux = kExpMask;
    } else if (cmd == VPU_TRANS_EXP_FMA2 || cmd == VPU_TRANS_SIN_TRANSFORM) {
        ea_hi_mux = 0;
    } else if (cmd == VPU_TRANS_LOG_MUL) {
        ea_hi_mux = trans_log_mul_ec;
    } else if (dtype == VPU_DTYPE_F32) {
        ea_hi_mux = ea_hi;
    } else {
        ea_hi_mux = half_to_single_exp(ea_hi, true, in.ea_hi_zero);
    }

    uint32_t eb_hi_mux = 0;
    if (cmd == VPU_TRANS_RCP_FMA2) {
        eb_hi_mux = bits((0x3u << 8) | (~eb_hi & 0xffu), kExpSextW);
    } else if (cmd == VPU_TRANS_LOG_MUL) {
        eb_hi_mux = trans_log_mul_inc_dec;
    } else if (cmd == VPU_TRANS_SIN_TRANSFORM) {
        eb_hi_mux = FLOAT32_E_BIAS_HEX;
    } else if (cmd == VPU_TRANS_EXP_FMA2) {
        eb_hi_mux = sign_extend(eb_hi, 8);
    } else if (dtype == VPU_DTYPE_F32 || sig(sigs, TXFMA_F32_FX) || cmd == VPU_TRANS_RSQRT_FMA2) {
        eb_hi_mux = eb_hi;
    } else {
        eb_hi_mux = half_to_single_exp(eb_hi, false, false);
    }

    uint32_t ec_mux = 0;
    if (dtype == VPU_DTYPE_F32 || dtype == VPU_DTYPE_F16_F32) {
        ec_mux = ec;
    } else if (in.ec_zero) {
        ec_mux = 0;
    } else if (in.ec_all1) {
        ec_mux = 0xff;
    } else {
        ec_mux = half_to_single_exp(ec, false, false);
    }
    if (sig(sigs, TXFMA_NORM_F32)) {
        ec_mux = TXFMA_NORM_F32_BIAS;
    }

    const uint32_t ea_lo_mux = (cmd == VPU_TRANS_EXP_FRAC) ? 0x86u : half_to_single_exp(ea_lo, false, false);
    const uint32_t eb_lo_mux = (cmd == VPU_TRANS_EXP_FRAC) ? 0u : half_to_single_exp(eb_lo, false, false);

    const bool comp_op = (cmd == VPU_FCMD_FLE) || (cmd == VPU_FCMD_FLT) || (cmd == VPU_FCMD_FEQ);
    const bool minmax_op = (cmd == VPU_FCMD_MIN) || (cmd == VPU_FCMD_MAX);
    const bool cin_hi_bias_sel = (cmd == VPU_FCMD_CVT_INTF32) || (cmd == VPU_FCMD_CVT_UINTF32) ||
                                 (cmd == VPU_FCMD_CVT_RASTF32) || (cmd == VPU_FCMD_FROUND) ||
                                 (cmd == VPU_FCMD_FFRC) || (cmd == VPU_FCMD_FFRC_SIN) ||
                                 (cmd == VPU_TRANS_EXP_FMA2);
    const bool cin_hi_trans_zero_sel = sig(sigs, TXFMA_TRANS2) || (cmd == VPU_TRANS_SIN_TRANSFORM) ||
                                       (cmd == VPU_TRANS_LOG_MUL) || (cmd == VPU_TRANS_EXP_FRAC);
    const bool cin_hi_zero_sel = cin_hi_trans_zero_sel || (cmd == VPU_FCMD_ADD) || (cmd == VPU_FCMD_SUB) ||
                                 (cmd == VPU_FCMD_CVT_F16F32) || (cmd == VPU_FCMD_CVT_F10F32) ||
                                 (cmd == VPU_FCMD_CVT_F11F32) || comp_op || minmax_op;
    const uint32_t cin_hi_mux = cin_hi_bias_sel ? FLOAT32_E_BIAS_HEX :
                                (cin_hi_zero_sel ? 0u : bits((0x3u << 8) | FLOAT32_NEG_E_BIAS_HEX, kExpSextW));
    const uint32_t cin_lo_mux = (cmd == VPU_TRANS_EXP_FRAC) ? 0u : bits((0x3u << 8) | FLOAT32_NEG_E_BIAS_HEX, kExpSextW);

    E1Out out{};
    out.eprod_hi = add3_w(kExpSextW, ea_hi_mux, eb_hi_mux, cin_hi_mux);
    out.eprod_lo = add3_w(kExpSextW, ea_lo_mux, eb_lo_mux, cin_lo_mux);
    out.ediff_ec_eph = add_w(kExpSextW, ec_mux, not_w(out.eprod_hi), 1);
    out.ediff_ec_epl = add_w(kExpSextW, ec, not_w(out.eprod_lo), 1);
    out.ediff_eph_epl = add_w(kExpSextW, out.eprod_hi, not_w(out.eprod_lo), 1);
    out.etrz_prod_hi_ph = add_w(5, bits(in.etrz_fa_hi_ph, 4), bits(in.etrz_fb_hi_ph, 4));
    out.etrz_prod_lo_ps = add_w(6, bits(in.etrz_fa_lo_ps, 5), bits(in.etrz_fb_lo_ps, 5));
    out.ec_out = bits(ec_mux, 8);
    return out;
}

struct EdiffIn {
    uint32_t cmd = VPU_FCMD_ADD;
    uint32_t op_dtype = VPU_DTYPE_F32;
    uint32_t sigs = 0;
    bool sign_c_ph = false;
    bool sign_c_pl = false;
    bool sign_ph_pl = false;
    bool exp_fma2_exc = false;
    uint32_t exp_res_2f3 = 0;
};

struct EdiffOut {
    uint32_t align1_mux_sel;
    uint32_t align2_mux_sel;
    uint32_t opd3_mux_sel;
    uint32_t ediff1_sel;
    uint32_t ediff2_sel;
    uint32_t res_exp_hi_sel;
    uint32_t res_exp_lo_sel;
};

inline EdiffOut model_ediff(const EdiffIn& in) {
    const uint32_t cmd = bits(in.cmd, 7);
    const uint32_t dtype = bits(in.op_dtype, 4);
    const uint32_t sigs = bits(in.sigs, 19);
    EdiffOut out{TXFMA_SEL_PH, TXFMA_SEL_PL, TXFMA_SEL_P, TXFMA_SEL_EDIFF_EC_EPH,
                 TXFMA_SEL_EDIFF_EC_EPL, TXFMA_SEL_C, TXFMA_SEL_PH};

    if (sig(sigs, TXFMA_F32_NORM)) {
        out.align1_mux_sel = TXFMA_SEL_P;
        out.opd3_mux_sel = TXFMA_SEL_A;
        out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
        out.res_exp_hi_sel = TXFMA_SEL_GCVT_F32_NORM;
    }

    if (dtype == VPU_DTYPE_INT) {
        if (sig(sigs, TXFMA_CVT)) {
            out.align1_mux_sel = TXFMA_SEL_P;
            out.opd3_mux_sel = TXFMA_SEL_A;
            out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
            out.res_exp_hi_sel = TXFMA_SEL_CVT_F32INT;
            if (cmd == VPU_FCMD_CVT_F32RAST) {
                out.res_exp_hi_sel = TXFMA_SEL_CVT_F32_FXP1615;
            }
        }
        if (sig(sigs, TXFMA_TRANS1) || sig(sigs, TXFMA_TRANS2) || cmd == VPU_FCMD_NR1_FRCPFXP) {
            out.res_exp_hi_sel = TXFMA_SEL_P;
            out.align1_mux_sel = TXFMA_SEL_C_INT;
            out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
            out.opd3_mux_sel = TXFMA_SEL_P_INT;
            if (cmd == VPU_TRANS_EXP_FMA2 && in.exp_fma2_exc) {
                out.res_exp_hi_sel = TXFMA_SEL_B;
                out.align1_mux_sel = TXFMA_SEL_ZERO;
                out.align2_mux_sel = TXFMA_SEL_ZERO;
                out.opd3_mux_sel = TXFMA_SEL_ZERO;
            }
        }
        if (cmd == VPU_FCMD_NR2_FRCPFXP) {
            out.ediff1_sel = TXFMA_SEL_EDIFF_NR2_FRCPFXP;
            out.align1_mux_sel = TXFMA_SEL_C_INT;
            out.opd3_mux_sel = TXFMA_SEL_P;
        }
    }

    if (dtype == VPU_DTYPE_F32) {
        if (sig(sigs, TXFMA_CVT)) {
            out.align1_mux_sel = TXFMA_SEL_C;
            out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
            out.res_exp_hi_sel = TXFMA_SEL_PH;
            out.opd3_mux_sel = TXFMA_SEL_A;
            if (cmd == VPU_FCMD_FROUND) {
                out.opd3_mux_sel = TXFMA_SEL_ROUND;
                out.res_exp_hi_sel = TXFMA_SEL_CVT_F32INT;
            }
        } else if (cmd == VPU_TRANS_LOG_MUL) {
            out.res_exp_hi_sel = TXFMA_SEL_P;
            out.align1_mux_sel = TXFMA_SEL_P;
            out.ediff1_sel = (bits(in.exp_res_2f3, 7) == 0u) ? TXFMA_SEL_EDIFF_LOG_MUL2 : TXFMA_SEL_EDIFF_LOG_MUL;
            out.opd3_mux_sel = TXFMA_SEL_EXP_RES;
        } else if (sig(sigs, TXFMA_MUL)) {
            out.align1_mux_sel = TXFMA_SEL_C;
            out.opd3_mux_sel = TXFMA_SEL_P;
            out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
            out.res_exp_hi_sel = TXFMA_SEL_PH;
        } else if (cmd == VPU_TRANS_LOG_RR) {
            out.align1_mux_sel = TXFMA_SEL_C;
            out.opd3_mux_sel = TXFMA_SEL_ZERO;
            out.res_exp_hi_sel = TXFMA_SEL_C;
            out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
        } else if (cmd == VPU_FCMD_FFRC || cmd == VPU_FCMD_FFRC_SIN) {
            out.align1_mux_sel = TXFMA_SEL_A;
            out.opd3_mux_sel = TXFMA_SEL_C;
            out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
            out.res_exp_hi_sel = TXFMA_SEL_C;
        } else if (cmd == VPU_FCMD_FLE || cmd == VPU_FCMD_FLT || cmd == VPU_FCMD_FEQ) {
            out.opd3_mux_sel = TXFMA_SEL_P;
        } else if (cmd == VPU_TRANS_EXP_FRAC) {
            out.align1_mux_sel = TXFMA_SEL_C;
            out.align2_mux_sel = TXFMA_SEL_ZERO;
            out.opd3_mux_sel = TXFMA_SEL_ZERO;
            out.res_exp_hi_sel = TXFMA_SEL_EXP_FRAC;
            out.res_exp_lo_sel = TXFMA_SEL_EXP_FRAC;
            out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
            out.ediff2_sel = TXFMA_SEL_EDIFF_EC_EPL;
        } else if (cmd == VPU_TRANS_SIN_TRANSFORM) {
            out.align1_mux_sel = TXFMA_SEL_C;
            out.align2_mux_sel = TXFMA_SEL_A;
            out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
            out.ediff2_sel = TXFMA_SEL_ZERO;
            out.opd3_mux_sel = TXFMA_SEL_ZERO;
            out.res_exp_hi_sel = TXFMA_SEL_PH;
        } else if (sig(sigs, TXFMA_NORM_F32)) {
            out.align1_mux_sel = TXFMA_SEL_P;
            out.align2_mux_sel = TXFMA_SEL_ZERO;
            out.opd3_mux_sel = TXFMA_SEL_ZERO;
            out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
            out.ediff2_sel = TXFMA_SEL_EDIFF_ZERO;
            out.res_exp_hi_sel = TXFMA_SEL_ZERO;
            out.res_exp_lo_sel = TXFMA_SEL_ZERO;
        } else if (sig(sigs, TXFMA_FX_F32)) {
            out.res_exp_hi_sel = TXFMA_SEL_PH;
            out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
            out.align1_mux_sel = TXFMA_SEL_A;
        } else {
            if (in.sign_c_ph) {
                out.align1_mux_sel = TXFMA_SEL_C;
                out.opd3_mux_sel = (cmd == VPU_FCMD_ADD || cmd == VPU_FCMD_SUB) ? TXFMA_SEL_A : TXFMA_SEL_P;
                out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
                out.res_exp_hi_sel = (cmd == VPU_FCMD_ADD || cmd == VPU_FCMD_SUB) ? TXFMA_SEL_A : TXFMA_SEL_P;
            } else {
                out.align1_mux_sel = (cmd == VPU_FCMD_ADD || cmd == VPU_FCMD_SUB) ? TXFMA_SEL_A : TXFMA_SEL_P;
                out.opd3_mux_sel = TXFMA_SEL_C;
                out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
                out.res_exp_hi_sel = TXFMA_SEL_C;
            }
        }
    }

    if (dtype == VPU_DTYPE_F16_F32) {
        if (sig(sigs, TXFMA_MUL)) {
            out.align1_mux_sel = TXFMA_SEL_C;
            out.ediff1_sel = TXFMA_SEL_EDIFF_ZERO;
            if (in.sign_ph_pl) {
                out.align2_mux_sel = TXFMA_SEL_PH;
                out.opd3_mux_sel = TXFMA_SEL_PL;
                out.ediff2_sel = TXFMA_SEL_EDIFF_EPH_EPL;
                out.res_exp_hi_sel = TXFMA_SEL_PL;
            } else {
                out.align2_mux_sel = TXFMA_SEL_PL;
                out.opd3_mux_sel = TXFMA_SEL_PH;
                out.ediff2_sel = TXFMA_SEL_EDIFF_EPH_EPL;
                out.res_exp_hi_sel = TXFMA_SEL_PH;
            }
        } else {
            const uint32_t order = (static_cast<uint32_t>(in.sign_c_ph) << 2) |
                                   (static_cast<uint32_t>(in.sign_c_pl) << 1) |
                                   static_cast<uint32_t>(in.sign_ph_pl);
            switch (order) {
                case 0b000:
                case 0b001:
                    out.align1_mux_sel = TXFMA_SEL_PH;
                    out.align2_mux_sel = TXFMA_SEL_PL;
                    out.opd3_mux_sel = TXFMA_SEL_C;
                    out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPH;
                    out.ediff2_sel = TXFMA_SEL_EDIFF_EC_EPL;
                    out.res_exp_hi_sel = TXFMA_SEL_C;
                    break;
                case 0b011:
                    out.align1_mux_sel = TXFMA_SEL_C;
                    out.align2_mux_sel = TXFMA_SEL_PH;
                    out.opd3_mux_sel = TXFMA_SEL_PL;
                    out.ediff1_sel = TXFMA_SEL_EDIFF_EC_EPL;
                    out.ediff2_sel = TXFMA_SEL_EDIFF_N_EPH_EPL;
                    out.res_exp_hi_sel = TXFMA_SEL_PL;
                    break;
                case 0b100:
                case 0b110:
                    out.align1_mux_sel = TXFMA_SEL_C;
                    out.align2_mux_sel = TXFMA_SEL_PL;
                    out.opd3_mux_sel = TXFMA_SEL_PH;
                    out.ediff1_sel = TXFMA_SEL_EDIFF_N_EC_EPH;
                    out.ediff2_sel = TXFMA_SEL_EDIFF_EPH_EPL;
                    out.res_exp_hi_sel = TXFMA_SEL_PH;
                    break;
                case 0b111:
                    out.align1_mux_sel = TXFMA_SEL_C;
                    out.align2_mux_sel = TXFMA_SEL_PH;
                    out.opd3_mux_sel = TXFMA_SEL_PL;
                    out.ediff1_sel = TXFMA_SEL_EDIFF_N_EC_EPL;
                    out.ediff2_sel = TXFMA_SEL_EDIFF_N_EPH_EPL;
                    out.res_exp_hi_sel = TXFMA_SEL_PL;
                    break;
                default:
                    break;
            }
        }
    }

    if (dtype == VPU_DTYPE_F11 || dtype == VPU_DTYPE_F10 || dtype == VPU_DTYPE_F16) {
        out.res_exp_hi_sel = TXFMA_SEL_C;
    }
    return out;
}

struct E2In {
    uint32_t cmd = VPU_FCMD_ADD;
    uint32_t op_dtype = VPU_DTYPE_F32;
    uint32_t sigs = 0;
    uint32_t ediff_ec_eph = 0;
    uint32_t ediff_ec_epl = 0;
    uint32_t ediff_eph_epl = 0;
    uint32_t eprod_hi = 0;
    uint32_t eprod_lo = 0;
    uint32_t ec = 0;
    uint32_t eb_hi = 0;
    uint32_t etrz_fa_lo_ps = 0;
    uint32_t etrz_fc = 0;
    uint32_t etrz_prod_hi_ph = 0;
    uint32_t etrz_prod_lo_ps = 0;
    bool sin_ediff2 = false;
    bool eprod_hi_zero = false;
    bool eprod_lo_zero = false;
    bool ec_zero = false;
    bool exp_fma2_exc = false;
};

struct E2Out {
    bool e_sticky1;
    bool e_sticky2;
    bool ediff1_zap;
    uint32_t ediff_abs_diff_hi;
    uint32_t align2_rshamt_c3;
    uint32_t exp_res_2f3;
    uint32_t align1_mux_sel;
    uint32_t align2_mux_sel;
    uint32_t opd3_mux_sel;
    uint32_t ediff1;
    uint32_t ediff2;
    uint32_t ediff1_sel;
    uint32_t ediff2_sel;
    uint32_t ediff1_abs;
    uint32_t ediff2_abs;
    uint32_t exp_res;
};

inline E2Out model_e2(const E2In& in) {
    const uint32_t cmd = bits(in.cmd, 7);
    const uint32_t dtype = bits(in.op_dtype, 4);
    const uint32_t sigs = bits(in.sigs, 19);
    const uint32_t ediff_ec_eph = bits(in.ediff_ec_eph, 10);
    const uint32_t ediff_ec_epl = bits(in.ediff_ec_epl, 10);
    const uint32_t ediff_eph_epl = bits(in.ediff_eph_epl, 10);
    const uint32_t eprod_hi = bits(in.eprod_hi, 10);
    const uint32_t eprod_lo = bits(in.eprod_lo, 10);
    const uint32_t ec = bits(in.ec, 8);
    const uint32_t eb_hi = bits(in.eb_hi, 8);

    const bool sign_c_ph = (bit(ediff_ec_eph, 9) && !in.eprod_hi_zero) || in.ec_zero;
    const bool sign_c_pl = (bit(ediff_ec_epl, 9) && !in.eprod_lo_zero) ||
                           (in.ec_zero && dtype == VPU_DTYPE_F16_F32);
    const bool sign_ph_pl = (bit(ediff_eph_epl, 9) && !in.eprod_lo_zero) || in.eprod_hi_zero;

    const EdiffOut order = model_ediff(EdiffIn{cmd, dtype, sigs, sign_c_ph, sign_c_pl, sign_ph_pl,
                                               in.exp_fma2_exc, eprod_hi & 0x7fu});

    uint32_t ediff1_mux = 0;
    switch (order.ediff1_sel) {
        case TXFMA_SEL_EDIFF_EC_EPH:
        case TXFMA_SEL_EDIFF_N_EC_EPH:
            ediff1_mux = ediff_ec_eph;
            break;
        case TXFMA_SEL_EDIFF_N_EC_EPL:
        case TXFMA_SEL_EDIFF_EC_EPL:
            ediff1_mux = ediff_ec_epl;
            break;
        case TXFMA_SEL_EDIFF_ZERO:
            ediff1_mux = 0;
            break;
        case TXFMA_SEL_EDIFF_LOG_MUL:
            ediff1_mux = 0x7;
            break;
        case TXFMA_SEL_EDIFF_LOG_MUL2:
        case TXFMA_SEL_EDIFF_NR2_FRCPFXP:
        default:
            ediff1_mux = 0;
            break;
    }
    const bool ediff_gt_31 = signed_value(ediff1_mux, 10) > 31;
    const bool ediff_gte_0 = signed_value(ediff1_mux, 10) >= 0;
    const bool ediff1_zap = (ediff_gt_31 || ediff_gte_0) && cmd == VPU_FCMD_FROUND;
    const uint32_t ediff1 = ediff1_zap ? 0u : ediff1_mux;
    const uint32_t ediff1_abs_int = bit(ediff1, 9) ? neg(ediff1) : ediff1;

    uint32_t ediff2_mux = 0;
    switch (order.ediff2_sel) {
        case TXFMA_SEL_EDIFF_ECH_EPH:
        case TXFMA_SEL_EDIFF_N_ECH_EPH:
            ediff2_mux = ediff_ec_eph;
            break;
        case TXFMA_SEL_EDIFF_EC_EPL:
        case TXFMA_SEL_EDIFF_N_EC_EPL:
            ediff2_mux = ediff_ec_epl;
            break;
        case TXFMA_SEL_EDIFF_EPH_EPL:
        case TXFMA_SEL_EDIFF_N_EPH_EPL:
            ediff2_mux = ediff_eph_epl;
            break;
        default:
            ediff2_mux = 0;
            break;
    }
    const uint32_t ediff2 = ediff2_mux;
    const uint32_t ediff2_abs_int = bit(ediff2_mux, 9) ? neg(ediff2_mux) : ediff2_mux;

    uint32_t etrz_1 = 0;
    switch (order.align1_mux_sel) {
        case TXFMA_SEL_PH:
            etrz_1 = bits(in.etrz_prod_hi_ph, 5);
            break;
        case TXFMA_SEL_P:
        case TXFMA_SEL_PL:
            etrz_1 = bits(in.etrz_prod_lo_ps, 6);
            break;
        case TXFMA_SEL_C:
            etrz_1 = bits(in.etrz_fc, 5);
            break;
        case TXFMA_SEL_A:
            etrz_1 = bits(in.etrz_fa_lo_ps, 5);
            break;
        default:
            etrz_1 = 0;
            break;
    }

    uint32_t etrz_2 = 0;
    switch (order.align2_mux_sel) {
        case TXFMA_SEL_PH:
            etrz_2 = bits(in.etrz_prod_hi_ph, 5);
            break;
        case TXFMA_SEL_P:
        case TXFMA_SEL_PL:
            etrz_2 = bits(in.etrz_prod_lo_ps, 6);
            break;
        case TXFMA_SEL_C:
            etrz_2 = bits(in.etrz_fc, 5);
            break;
        case TXFMA_SEL_A:
            etrz_2 = bits(in.etrz_prod_hi_ph, 5);
            break;
        default:
            etrz_2 = 0;
            break;
    }

    uint32_t e_1_tk = 0;
    switch (dtype) {
        case VPU_DTYPE_INT:
            e_1_tk = TXFMA_F32_A_C_STICKY_DISTANCE;
            break;
        case VPU_DTYPE_F32:
            if (order.align1_mux_sel == TXFMA_SEL_P && !sig(sigs, TXFMA_NORM_F32)) {
                e_1_tk = TXFMA_F32_PROD_STICKY_DISTANCE;
            } else {
                e_1_tk = TXFMA_F32_A_C_STICKY_DISTANCE;
            }
            break;
        case VPU_DTYPE_F16_F32:
            e_1_tk = (order.align1_mux_sel == TXFMA_SEL_C) ? TXFMA_F16_F32_C_STICKY_DISTANCE
                                                           : TXFMA_F16_F32_PROD_STICKY_DISTANCE;
            break;
        default:
            e_1_tk = 0;
            break;
    }
    if (cmd == VPU_FCMD_CVT_F16F32) {
        e_1_tk = TXFMA_F16_F32_MAN_DIFF;
    }
    if (cmd == VPU_FCMD_CVT_F11F32) {
        e_1_tk = TXFMA_F11_F32_MAN_DIFF;
    }
    if (cmd == VPU_FCMD_CVT_F10F32) {
        e_1_tk = TXFMA_F10_F32_MAN_DIFF;
    }
    if (cmd == VPU_FCMD_FROUND) {
        e_1_tk = TXFMA_INT_STICKY_DISTANCE;
    }

    uint32_t e_2_tk = TXFMA_F16_F32_PROD_STICKY_DISTANCE;
    if (cmd == VPU_TRANS_EXP_FRAC) {
        e_2_tk = TXFMA_TRANS_EXP_STICKY_DISTANCE;
    }

    const uint32_t e_sticky1_add = add3_w(10, e_1_tk, not_w(ediff1_abs_int), etrz_1, 1);
    const uint32_t sticky2_inb = (cmd == VPU_TRANS_EXP_FRAC) ? not_w(ec) : not_w(ediff2_abs_int);
    const uint32_t sticky2_cin = (cmd == VPU_TRANS_EXP_FRAC) ? 0u : etrz_2;
    const uint32_t e_sticky2_add = add3_w(10, e_2_tk, sticky2_inb, sticky2_cin, 1);

    uint32_t exp_res = 0;
    switch (order.res_exp_hi_sel) {
        case TXFMA_SEL_PH:
        case TXFMA_SEL_P:
        case TXFMA_SEL_A:
            exp_res = eprod_hi;
            break;
        case TXFMA_SEL_PL:
            exp_res = eprod_lo;
            break;
        case TXFMA_SEL_C:
            exp_res = ec;
            break;
        case TXFMA_SEL_B:
            exp_res = eb_hi;
            break;
        case TXFMA_SEL_CVT_F32INT:
            exp_res = FLOAT32_E_BIAS_HEX + TXFMA_CVT_F32INT_OFFSET;
            break;
        case TXFMA_SEL_CVT_F32_FXP1615:
            exp_res = FLOAT32_E_BIAS_HEX + TXFMA_CVT_F32_FXP1615_OFFSET;
            break;
        case TXFMA_SEL_GCVT_F32_NORM:
            exp_res = FLOAT32_E_BIAS_HEX - TXFMA_GCVT_F32_NORM_OFFSET;
            break;
        case TXFMA_SEL_EXP_FRAC:
            exp_res = FLOAT32_E_BIAS_HEX + TXFMA_EXP_FRAC_OFFSET;
            break;
        default:
            exp_res = 0;
            break;
    }
    exp_res = bits(exp_res, 10);
    const uint32_t exp_res_2f3 = exp_res & 0x7fu;
    const uint32_t exp_frac_e_shift = (order.res_exp_lo_sel == TXFMA_SEL_EXP_FRAC) ? e_sticky2_add : 0u;

    bool sticky1_clear = true;
    bool sticky2_clear = true;
    if (dtype == VPU_DTYPE_F32) {
        sticky1_clear = in.eprod_hi_zero || (in.ec_zero && !(sig(sigs, TXFMA_NORM_F32) || sig(sigs, TXFMA_FX_F32))) || ediff1_zap;
    }
    if (dtype == VPU_DTYPE_F16_F32) {
        sticky1_clear = (in.eprod_hi_zero && order.align1_mux_sel == TXFMA_SEL_PH) ||
                        (in.ec_zero && order.align1_mux_sel == TXFMA_SEL_C);
        sticky2_clear = (in.eprod_lo_zero && order.align2_mux_sel == TXFMA_SEL_PL) ||
                        (in.eprod_hi_zero && order.align2_mux_sel == TXFMA_SEL_PH);
    }

    const bool e_sticky1 = bit(e_sticky1_add, 9) && !sticky1_clear;
    const bool e_sticky2 = bit(e_sticky2_add, 9) && !sticky2_clear;
    const uint32_t ediff1_abs = (cmd == VPU_TRANS_EXP_FRAC) ? exp_frac_e_shift : ediff1_abs_int;
    const uint32_t ediff2_abs = (cmd == VPU_TRANS_SIN_TRANSFORM) ? static_cast<uint32_t>(in.sin_ediff2) : ediff2_abs_int;

    return E2Out{e_sticky1, e_sticky2, ediff1_zap, ediff1_abs & 0x3fu, ediff2_abs & 0x1fu,
                 exp_res_2f3, order.align1_mux_sel, order.align2_mux_sel, order.opd3_mux_sel,
                 ediff1, ediff2, order.ediff1_sel, order.ediff2_sel, bits(ediff1_abs, 10),
                 bits(ediff2_abs, 10), exp_res};
}

inline bool model_e4(bool e_sticky1, bool e_sticky2) {
    return e_sticky1 || e_sticky2;
}

struct E5In {
    uint32_t cmd = VPU_FCMD_ADD;
    uint32_t rm = RNE;
    uint32_t op_dtype = VPU_DTYPE_F32;
    uint32_t sigs = 0;
    uint32_t exp_res_in = 0;
    uint32_t ea_hi = 0;
    uint32_t ec = 0;
    bool s_res = false;
    bool comp_op = false;
    bool comp_op_true = false;
    bool nshc = false;
    bool int_minmax_mux_sel_a = false;
    bool int_minmax_mux_sel_c = false;
    bool e_force_to_zero = false;
    bool ediff1_zap = false;
    bool e_sticky1 = false;
    bool e_sticky2 = false;
    uint32_t elxd = 0;
    uint32_t etrz50 = 0;
    bool use_elxd = false;
    bool elxd_add_in2_mux_sel = false;
    bool fb_hi_zero = false;
    bool eb_hi_lsb = false;
};

struct E5Out {
    bool stky2g;
    bool rmode;
    uint32_t exp_res;
};

inline E5Out model_e5(const E5In& in) {
    const uint32_t cmd = bits(in.cmd, 7);
    const uint32_t dtype = bits(in.op_dtype, 4);
    const uint32_t sigs = bits(in.sigs, 19);
    const uint32_t exp_res_in = bits(in.exp_res_in, 10);
    const uint32_t elxd = bits(in.elxd, 7);

    uint32_t sticky_in1 = 0;
    switch (dtype) {
        case VPU_DTYPE_F32:
            if (sig(sigs, TXFMA_CVT) || sig(sigs, TXFMA_NORM_F32)) {
                sticky_in1 = TXFMA_STICKY_K_FI;
            } else if (in.use_elxd) {
                sticky_in1 = TXFMA_STICKY_K_F32;
            }
            break;
        case VPU_DTYPE_F16_F32:
            sticky_in1 = TXFMA_STICKY_TO_G_K_F16_L;
            break;
        case VPU_DTYPE_INT:
            sticky_in1 = TXFMA_STICKY_TO_R_K_IF;
            break;
        default:
            sticky_in1 = 0;
            break;
    }
    const uint32_t sticky_in2 = in.use_elxd ? sign_extend(elxd, 7) : 0u;
    const uint32_t sticky_cin2 = in.use_elxd ? 1u : 0u;
    const uint32_t e_sticky1_add = add3_w(10, sticky_in1, sticky_in2, bits(in.etrz50, 6), sticky_cin2);
    const bool stky2g = bit(e_sticky1_add, 9) || in.e_sticky1 ||
                        (in.e_sticky2 && dtype == VPU_DTYPE_F16_F32) || sig(sigs, TXFMA_F32_NORM);

    uint32_t elxd_in1 = exp_res_in;
    if (cmd == VPU_TRANS_RSQRT_FMA2) {
        const uint32_t concat = (bit(exp_res_in, 7) << 7) | ((!bit(exp_res_in, 7)) << 6) |
                                ((exp_res_in >> 1) & 0x3fu);
        elxd_in1 = not_w(concat);
    } else if (cmd == VPU_TRANS_LOG_MUL) {
        elxd_in1 = 133u - ((exp_res_in & 0x7fu) == 0u ? 7u : 0u);
    }
    elxd_in1 = bits(elxd_in1, 10);

    uint32_t elxd_in2 = 0;
    if (in.elxd_add_in2_mux_sel || sig(sigs, TXFMA_F32_FX) || sig(sigs, TXFMA_FX_F32)) {
        elxd_in2 = 0;
    } else if (cmd == VPU_TRANS_RCP_FMA2) {
        elxd_in2 = static_cast<uint32_t>(in.fb_hi_zero);
    } else if (cmd == VPU_TRANS_RSQRT_FMA2) {
        elxd_in2 = all_bits(!in.fb_hi_zero || !in.eb_hi_lsb, 10);
    } else if (cmd == VPU_TRANS_EXP_FRAC) {
        elxd_in2 = not_w(bits(in.ec, 8));
    } else {
        elxd_in2 = not_w(sign_extend(elxd, 7));
    }

    const uint32_t elxd_cin = !(sig(sigs, TXFMA_TRANS2) || cmd == VPU_TRANS_EXP_FRAC || sig(sigs, TXFMA_F32_FX));
    const uint32_t elxd_add = add_w(10, elxd_in1, elxd_in2, elxd_cin);
    const uint32_t elxd_add_m1 = add_w(10, elxd_in1, elxd_in2, 0);
    const uint32_t elxd_res = in.nshc ? elxd_add_m1 : elxd_add;

    const bool exp_res_zero_sel = (in.e_force_to_zero && !sig(sigs, TXFMA_TRANS2)) || (in.comp_op && !in.comp_op_true);
    const bool exp_res_ea_sel = in.int_minmax_mux_sel_a && !sig(sigs, TXFMA_FX_F32);
    const bool exp_res_ec_sel = (in.int_minmax_mux_sel_c && !sig(sigs, TXFMA_F32_FX)) || in.ediff1_zap;
    const bool exp_res_m1_sel = in.comp_op && in.comp_op_true;
    const bool exp_res_in_sel = sig(sigs, TXFMA_FX_F32) || sig(sigs, TXFMA_F32_FX) || cmd == VPU_TRANS_EXP_FMA2;

    uint32_t exp_res = elxd_res;
    if (exp_res_zero_sel) {
        exp_res = 0;
    } else if (exp_res_ea_sel) {
        exp_res = bits(in.ea_hi, 8);
    } else if (exp_res_ec_sel) {
        exp_res = bits(in.ec, 8);
    } else if (exp_res_m1_sel) {
        exp_res = 0x3ff;
    } else if (exp_res_in_sel) {
        exp_res = exp_res_in;
    }

    const bool rne = bits(in.rm, 3) == RNE;
    const bool rup = bits(in.rm, 3) == RUP;
    const bool rdn = bits(in.rm, 3) == RDN;
    const bool rmm = bits(in.rm, 3) == RMM;
    const bool rmode = rne || rmm || (rup && !in.s_res) || (rdn && in.s_res);
    return E5Out{stky2g, rmode, bits(exp_res, 10)};
}

struct E6In {
    uint32_t exp_res_in = 0;
    bool ps_rndovf = false;
    bool ph_rndovf = false;
    uint32_t cmd = VPU_FCMD_ADD;
    uint32_t sigs = 0;
    uint32_t op_dtype = VPU_DTYPE_F32;
};

struct E6Out {
    bool def_ovf_ps;
    bool pot_ovf_ps;
    bool def_unf_ps;
    bool pot_unf_ps;
    bool def_ovf_ph;
    bool pot_ovf_ph;
    bool def_unf_ph;
    bool pot_unf_ph;
    uint32_t exp_res;
    bool exp_res_zero;
};

inline E6Out model_e6(const E6In& in) {
    const uint32_t exp_res_in = bits(in.exp_res_in, 10);
    const uint32_t dtype = bits(in.op_dtype, 4);
    const uint32_t sigs = bits(in.sigs, 19);
    E6Out out{};
    if (dtype == VPU_DTYPE_F32 && !sig(sigs, TXFMA_FX_F32)) {
        out.pot_ovf_ps = exp_res_in == 0xfe;
        out.def_ovf_ps = signed_value(exp_res_in, 10) > signed_value(0x0fe, 10);
        out.pot_unf_ps = exp_res_in == 0;
        out.def_unf_ps = signed_value(exp_res_in, 10) < signed_value(0, 10);
    } else if (dtype == VPU_DTYPE_F16_F32) {
        out.pot_ovf_ps = (exp_res_in == 0xfe) && !sig(sigs, TXFMA_MUL);
        out.def_ovf_ps = (signed_value(exp_res_in, 10) > signed_value(0x0fe, 10)) && !sig(sigs, TXFMA_MUL);
        out.pot_unf_ps = (exp_res_in == 0) && !sig(sigs, TXFMA_MUL);
        out.def_unf_ps = (signed_value(exp_res_in, 10) < signed_value(0, 10)) && !sig(sigs, TXFMA_MUL);
    } else {
        out.pot_ovf_ph = exp_res_in == FLOAT16_32_MAX;
        out.def_ovf_ph = signed_value(exp_res_in, 10) > signed_value(FLOAT16_32_MAX, 10);
        out.pot_unf_ph = (exp_res_in == TXFMA_BIAS_DIFF) && exp_res_in != 0;
        out.def_unf_ph = (signed_value(exp_res_in, 10) < signed_value(TXFMA_BIAS_DIFF, 10)) && exp_res_in != 0;
    }
    const uint32_t adder_in2 = (bits(in.cmd, 7) == VPU_TRANS_RSQRT_FMA2) ? 0xfeu : 0u;
    const uint32_t exp_res_inc_dec = add_w(8, exp_res_in & 0xffu, adder_in2, 1);
    out.exp_res = (in.ps_rndovf || in.ph_rndovf || bits(in.cmd, 7) == VPU_TRANS_RSQRT_FMA2)
                      ? exp_res_inc_dec
                      : (exp_res_in & 0xffu);
    out.exp_res_zero = out.exp_res == 0;
    return out;
}

struct SpecialIn {
    uint32_t opa = 0;
    uint32_t opb = 0;
    uint32_t opc = 0;
    uint32_t op_dtype = VPU_DTYPE_F32;
    uint32_t sigs = 0;
    uint32_t cmd = VPU_FCMD_ADD;
    bool trans_exp_fma2_exc = false;
    bool trans_taylor = false;
};

struct SpecialOut {
    bool ea_hi_zero;
    bool eb_hi_zero;
    bool ec_zero;
    bool ea_lo_zero;
    bool eb_lo_zero;
    bool ea_hi_all_one;
    bool eb_hi_all_one;
    bool ec_all_one;
    bool ea_lo_all_one;
    bool eb_lo_all_one;
};

inline SpecialOut model_special(const SpecialIn& in) {
    const uint32_t opa = in.opa;
    const uint32_t opb = in.opb;
    const uint32_t opc = in.opc;
    const uint32_t dtype = bits(in.op_dtype, 4);
    const uint32_t sigs = bits(in.sigs, 19);
    const uint32_t cmd = bits(in.cmd, 7);

    const bool ea_hi_zero30_27 = ((opa >> 27) & 0xfu) == 0;
    const bool eb_hi_zero30_27 = ((opb >> 27) & 0xfu) == 0;
    const bool ec_zero30_27 = ((opc >> 27) & 0xfu) == 0;
    const bool ea_hi_zero26_23 = ((opa >> 23) & 0xfu) == 0;
    const bool eb_hi_zero26_23 = ((opb >> 23) & 0xfu) == 0;
    const bool ec_zero26_23 = ((opc >> 23) & 0xfu) == 0;
    const bool ea_hi_zero14_10 = ((opa >> 10) & 0x1fu) == 0;
    const bool eb_hi_zero14_10 = ((opb >> 10) & 0x1fu) == 0;
    const bool ec_zero14_10 = ((opc >> 10) & 0x1fu) == 0;
    const bool ea_one30_27 = ((opa >> 27) & 0xfu) == 0xfu;
    const bool eb_one30_27 = ((opb >> 27) & 0xfu) == 0xfu;
    const bool ec_one30_27 = ((opc >> 27) & 0xfu) == 0xfu;
    const bool ea_one26_23 = ((opa >> 23) & 0xfu) == 0xfu;
    const bool eb_one26_23 = ((opb >> 23) & 0xfu) == 0xfu;
    const bool ec_one26_23 = ((opc >> 23) & 0xfu) == 0xfu;
    const bool ea_one14_10 = ((opa >> 10) & 0x1fu) == 0x1fu;
    const bool eb_one14_10 = ((opb >> 10) & 0x1fu) == 0x1fu;
    const bool ec_one14_10 = ((opc >> 10) & 0x1fu) == 0x1fu;
    const bool sin_rom_op = ((cmd == VPU_TRANS_SIN_P1) && !in.trans_taylor) ||
                            ((cmd == VPU_TRANS_SIN_P2) && !in.trans_taylor);

    SpecialOut out{};
    if (((dtype == VPU_DTYPE_F32) && !sin_rom_op) ||
        (sig(sigs, TXFMA_TRANS2) && !((cmd == VPU_TRANS_EXP_FMA2) && !in.trans_exp_fma2_exc))) {
        out.ea_hi_zero = ea_hi_zero30_27 && ea_hi_zero26_23 &&
                         (cmd != VPU_TRANS_SIN_P2 && !(cmd == VPU_TRANS_SIN_P3 && !in.trans_taylor));
        out.eb_hi_zero = eb_hi_zero30_27 && eb_hi_zero26_23;
        out.ec_zero = ec_zero30_27 && ec_zero26_23 && (cmd != VPU_TRANS_SIN_P2);
        out.ea_hi_all_one = ea_one30_27 && ea_one26_23;
        out.eb_hi_all_one = eb_one30_27 && eb_one26_23;
        out.ec_all_one = ec_one30_27 && ec_one26_23;
    }

    switch (dtype) {
        case VPU_DTYPE_F16:
            out.ec_zero = ((opc >> 10) & 0x1fu) == 0;
            out.ec_all_one = ((opc >> 10) & 0x1fu) == 0x1fu;
            break;
        case VPU_DTYPE_F16_F32:
            out.ea_hi_zero = ea_hi_zero30_27 && !bit(opa, 26);
            out.eb_hi_zero = eb_hi_zero30_27 && !bit(opb, 26);
            out.ea_lo_zero = ea_hi_zero14_10;
            out.eb_lo_zero = eb_hi_zero14_10;
            out.ea_hi_all_one = ea_one30_27 && bit(opa, 26);
            out.eb_hi_all_one = eb_one30_27 && bit(opb, 26);
            out.ea_lo_all_one = ea_one14_10;
            out.eb_lo_all_one = eb_one14_10;
            out.ec_zero = ec_zero30_27 && ec_zero26_23;
            out.ec_all_one = ec_one30_27 && ec_one26_23;
            break;
        case VPU_DTYPE_F11:
            out.ec_zero = ((opc >> 6) & 0x1fu) == 0;
            out.ec_all_one = ((opc >> 6) & 0x1fu) == 0x1fu;
            break;
        case VPU_DTYPE_F10:
            out.ec_zero = ((opc >> 5) & 0x1fu) == 0;
            out.ec_all_one = ((opc >> 5) & 0x1fu) == 0x1fu;
            break;
        default:
            break;
    }

    if (cmd == VPU_TRANS_LOG_MUL) {
        out.ea_hi_zero = false;
        out.eb_hi_zero = false;
    }
    if (sig(sigs, TXFMA_TRANS2)) {
        out.ea_hi_zero = false;
        out.ec_zero = false;
    }
    if (cmd == VPU_TRANS_RCP_FMA2) {
        out.eb_hi_all_one = eb_one30_27 && eb_one26_23;
    }
    return out;
}

}  // namespace txfma_e_model
