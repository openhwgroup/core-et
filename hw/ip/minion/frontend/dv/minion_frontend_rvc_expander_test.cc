// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0
//
// minion_frontend_rvc_expander — Verilator unit test.
//
// Tests RVC expansion against known instruction encodings from
// the RISC-V C extension spec for RV64 compressed encodings. This is not an
// ISA support declaration for the Minion core.

#include "Vminion_frontend_rvc_expander.h"
#include "sim_ctrl.h"

using DUT = Vminion_frontend_rvc_expander;

// Helper: expand and check.
static void expand_check(SimCtrl<DUT>& s, uint32_t in, uint32_t expect,
                         const std::string& name) {
    s.dut->in_bits = in;
    s.dut->eval();
    s.check(s.dut->out_bits == expect,
            name + ": in=0x" + ([&]{
                char buf[16]; snprintf(buf, sizeof(buf), "%04x", in);
                return std::string(buf);
            })() + " expect=0x" + ([&]{
                char buf[16]; snprintf(buf, sizeof(buf), "%08x", expect);
                return std::string(buf);
            })() + " got=0x" + ([&]{
                char buf[16]; snprintf(buf, sizeof(buf), "%08x", s.dut->out_bits);
                return std::string(buf);
            })());
}

int main(int argc, char** argv) {
    SimCtrl<DUT> sim(argc, argv);

    // ── 32-bit pass-through ────────────────────────────
    printf("test: 32-bit pass-through\n");
    expand_check(sim, 0x00000033, 0x00000033, "add x0,x0,x0");
    expand_check(sim, 0xFEDCBA97, 0xFEDCBA97, "arbitrary 32-bit");
    expand_check(sim, 0x00000013, 0x00000013, "nop (addi x0,x0,0)");

    // ── Quadrant 0 ─────────────────────────────────────
    printf("test: Q0 loads and stores\n");
    // C.LW x8, 0(x8) = 0x4000 → lw x8, 0(x8) [CL: rd'=bits[4:2], rs1'=bits[9:7]]
    expand_check(sim, 0x00004000, 0x00042403, "C.LW x8,0(x8)");
    // C.SD x8, 0(x8) = 0xE000 → sd x8, 0(x8) [CS: rs2'=bits[4:2], rs1'=bits[9:7]]
    expand_check(sim, 0x0000E000, 0x00843023, "C.SD x8,0(x8)");

    // ── Quadrant 1: arithmetic ─────────────────────────
    printf("test: Q1 arithmetic\n");
    // C.NOP = 0x0001 → addi x0, x0, 0
    expand_check(sim, 0x00000001, 0x00000013, "C.NOP");
    // C.ADDI x10, 1 = 0x0505 → addi x10, x10, 1
    expand_check(sim, 0x00000505, 0x00150513, "C.ADDI x10,1");
    // C.LI x10, 0 = 0x4501 → addi x10, x0, 0
    expand_check(sim, 0x00004501, 0x00000513, "C.LI x10,0");
    // C.LI x10, -1 = 0x557D → addi x10, x0, -1
    expand_check(sim, 0x0000557D, 0xFFF00513, "C.LI x10,-1");

    // ── Quadrant 1: branches ───────────────────────────
    printf("test: Q1 branches\n");
    // C.J offset=0 = 0xA001 → jal x0, 0
    expand_check(sim, 0x0000A001, 0x0000006F, "C.J 0");
    // C.BEQZ x8, 0 = 0xC001 → beq x8, x0, 0
    expand_check(sim, 0x0000C001, 0x00040063, "C.BEQZ x8,0");
    // C.BNEZ x8, 0 = 0xE001 → bne x8, x0, 0
    expand_check(sim, 0x0000E001, 0x00041063, "C.BNEZ x8,0");

    // ── Quadrant 1: ALU register ───────────────────────
    printf("test: Q1 ALU register\n");
    // C.SUB x8, x8 = 0x8C01 → sub x8, x8, x8
    expand_check(sim, 0x00008C01, 0x40840433, "C.SUB x8,x8");
    // C.XOR x8, x8 = 0x8C21 → xor x8, x8, x8
    expand_check(sim, 0x00008C21, 0x00844433, "C.XOR x8,x8");
    // C.OR x8, x8 = 0x8C41 → or x8, x8, x8
    expand_check(sim, 0x00008C41, 0x00846433, "C.OR x8,x8");
    // C.AND x8, x8 = 0x8C61 → and x8, x8, x8
    expand_check(sim, 0x00008C61, 0x00847433, "C.AND x8,x8");
    // C.SUBW x8, x8 = 0x9C01 → subw x8, x8, x8
    expand_check(sim, 0x00009C01, 0x4084043B, "C.SUBW x8,x8");
    // C.ADDW x8, x8 = 0x9C21 → addw x8, x8, x8
    expand_check(sim, 0x00009C21, 0x0084043B, "C.ADDW x8,x8");

    // ── Quadrant 2 ─────────────────────────────────────
    printf("test: Q2\n");
    // C.SLLI x10, 1 = 0x0506 → slli x10, x10, 1
    expand_check(sim, 0x00000506, 0x00151513, "C.SLLI x10,1");
    // C.LWSP x10, 0 = 0x4502 → lw x10, 0(x2)
    expand_check(sim, 0x00004502, 0x00012503, "C.LWSP x10,0");
    // C.LDSP x10, 0 = 0x6502 → ld x10, 0(x2)
    expand_check(sim, 0x00006502, 0x00013503, "C.LDSP x10,0");
    // C.JR x1 = 0x8082 → jalr x0, x1, 0
    expand_check(sim, 0x00008082, 0x00008067, "C.JR x1");
    // C.MV x10, x11 = 0x852E → add x10, x0, x11
    expand_check(sim, 0x0000852E, 0x00B00533, "C.MV x10,x11");
    // C.EBREAK = 0x9002 → ebreak
    expand_check(sim, 0x00009002, 0x00100073, "C.EBREAK");
    // C.JALR x1 = 0x9082 → jalr x1, x1, 0
    expand_check(sim, 0x00009082, 0x000080E7, "C.JALR x1");
    // C.ADD x10, x11 = 0x952E → add x10, x10, x11
    expand_check(sim, 0x0000952E, 0x00B50533, "C.ADD x10,x11");

    // ── Illegal / reserved ─────────────────────────────
    printf("test: illegal/reserved\n");
    // All-zero = defined illegal
    expand_check(sim, 0x00000000, 0x00000000, "all-zero illegal");
    // C.ADDI4SPN nzuimm=0 = reserved
    expand_check(sim, 0x00000004, 0x0001049F, "C.ADDI4SPN nzuimm=0");
    // C.ADDIW rd=x0 = reserved
    expand_check(sim, 0x00002001, 0x0000001F, "C.ADDIW rd=x0");

    // ── Exhaustive: verify no expansion produces bits[1:0]≠11 ──
    printf("test: all compressed produce bits[1:0]==11\n");
    int bad_opbits = 0;
    for (uint32_t i = 0; i < 65536; i++) {
        if ((i & 3) == 3) continue;
        sim.dut->in_bits = i;
        sim.dut->eval();
        if ((sim.dut->out_bits & 3) != 3 && sim.dut->out_bits != 0) {
            bad_opbits++;
        }
    }
    sim.check(bad_opbits == 0, "all expanded instructions have bits[1:0]==11");

    return sim.finish();
}
