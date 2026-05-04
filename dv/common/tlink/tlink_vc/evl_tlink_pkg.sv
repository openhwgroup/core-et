
`ifndef __EVL_TLINK_PKG

   `define __EVL_TLINK_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"

   `define EVL_TL_REQ_OPCODE_MASK   5'b11111
   `define EVL_TL_REQ_CHANNEL_MASK  5'b11000
   `define EVL_TL_REQ_CHANNEL_SHIFT 3
   `define EVL_TL_REQ_CHANNEL_A_VAL 2'b00
   `define EVL_TL_REQ_CHANNEL_B_VAL 2'b01
   `define EVL_TL_REQ_CHANNEL_C_VAL 2'b10
   `define EVL_TL_REQ_CHANNEL_X_VAL 2'b11
   `define EVL_TL_REQ_CHANNEL_A     { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b000 }
   `define EVL_TL_REQ_CHANNEL_B     { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b000 }
   `define EVL_TL_REQ_CHANNEL_C     { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b000 }

   `define EVL_TL_RSP_DENIED        8'b01000000
   `define EVL_TL_RSP_CORRUPT       8'b00100000
   `define EVL_TL_RSP_OPCODE_MASK   5'b11111
   `define EVL_TL_RSP_CHANNEL_MASK  5'b11000
   `define EVL_TL_RSP_CHANNEL_SHIFT 3
   `define EVL_TL_RSP_CHANNEL_C_VAL 2'b01
   `define EVL_TL_RSP_CHANNEL_D_VAL 2'b00
   `define EVL_TL_RSP_CHANNEL_E_VAL 2'b11
   `define EVL_TL_RSP_CHANNEL_X_VAL 2'b10
   `define EVL_TL_RSP_CHANNEL_C     { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b000 }
   `define EVL_TL_RSP_CHANNEL_D     { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b000 }
   `define EVL_TL_RSP_CHANNEL_E     { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b000 }

   `define EVL_TL_PARAM_INTENT       6'b001000
   `define EVL_TL_PARAM_LOGIC_ATOMIC 6'b010000
   `define EVL_TL_PARAM_ARITH_ATOMIC 6'b011000
   `define EVL_TL_PARAM_ACQUIRE      6'b100000
   `define EVL_TL_PARAM_PROBE        6'b101000
   `define EVL_TL_PARAM_GRANT        6'b101000
   `define EVL_TL_PARAM_PROBEACK     6'b110000
   `define EVL_TL_PARAM_RELEASE      6'b110000

   `define EVL_TL_VALID_PARAMS TL_param_none, TL_pref_rd, TL_pref_wr, TL_xor, TL_or, TL_and, TL_swap, TL_min, TL_max, TL_minu, TL_maxu, TL_add, NtoB, NtoT, BtoT, toT, toB, toN, TtoB, TtoN, BtoN, TtoT, BtoB, NtoN


   //-----------------------------------------------------------------------------------------------
   package evl_tlink_pkg;
      `ifdef ET_USE_UVM
         import uvm_pkg::*;
      `endif
      import evl_base_pkg::*;

      typedef enum bit [4:0] {
            TLA_PutFull      = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b000 },
            TLA_PutPartial   = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b001 },
            TLA_AtomicArith  = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b010 },
            TLA_AtomicLogic  = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b011 },
            TLA_Get          = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b100 },
            TLA_Intent       = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b101 },
            TLA_AcquireBlock = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b110 },
            TLA_AcquirePerm  = { `EVL_TL_REQ_CHANNEL_A_VAL, 3'b111 },
            TLB_PutFull      = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b000 },
            TLB_PutPartial   = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b001 },
            TLB_AtomicArith  = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b010 },
            TLB_AtomicLogic  = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b011 },
            TLB_Get          = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b100 },
            TLB_Intent       = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b101 },
            TLB_ProbeBlock   = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b110 },
            TLB_ProbePerm    = { `EVL_TL_REQ_CHANNEL_B_VAL, 3'b111 },
            TLC_Req0x0       = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b000 },
            TLC_Req0x1       = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b001 },
            TLC_Req0x2       = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b010 },
            TLC_Req0x3       = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b011 },
            TLC_Req0x4       = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b100 },
            TLC_Req0x5       = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b101 },
            TLC_Release      = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b110 },
            TLC_ReleaseData  = { `EVL_TL_REQ_CHANNEL_C_VAL, 3'b111 },
            TL_Req0x0        = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b000 },
            TL_Req0x1        = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b001 },
            TL_Req0x2        = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b010 },
            TL_Req0x3        = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b011 },
            TL_Req0x4        = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b100 },
            TL_Req0x5        = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b101 },
            TL_ReqUnknown    = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b110 },
            TL_ReqNull       = { `EVL_TL_REQ_CHANNEL_X_VAL, 3'b111 }
         } evl_tlink_req_opcode_t;

      typedef enum bit [4:0] {
            TLD_AccessAck     = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b000 },
            TLD_AccessAckData = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b001 },
            TLD_HintAck       = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b010 },
            TLD_Rsp0x3        = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b011 },
            TLD_Grant         = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b100 },
            TLD_GrantData     = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b101 },
            TLD_ReleaseAck    = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b110 },
            TLD_Rsp0x7        = { `EVL_TL_RSP_CHANNEL_D_VAL, 3'b111 },
            TLC_AccessAck     = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b000 },
            TLC_AccessAckData = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b001 },
            TLC_HintAck       = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b010 },
            TLC_Rsp0x3        = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b011 },
            TLC_ProbeAck      = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b100 },
            TLC_ProbeAckData  = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b101 },
            TLC_Rsp0x6        = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b110 },
            TLC_Rsp0x7        = { `EVL_TL_RSP_CHANNEL_C_VAL, 3'b111 },
            TL_Rsp0x0         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b000 },
            TL_Rsp0x1         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b001 },
            TL_Rsp0x2         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b010 },
            TL_Rsp0x3         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b011 },
            TL_Rsp0x4         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b100 },
            TL_Rsp0x5         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b101 },
            TL_Rsp0x6         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b110 },
            TL_Rsp0x7         = { `EVL_TL_RSP_CHANNEL_X_VAL, 3'b111 },
            TLE_GrantAck      = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b000 },
            TLE_Rsp0x1        = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b001 },
            TLE_Rsp0x2        = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b010 },
            TLE_Rsp0x3        = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b011 },
            TLE_Rsp0x4        = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b100 },
            TLE_Rsp0x5        = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b101 },
            TL_RspUnknown     = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b110 },
            TL_RspNull        = { `EVL_TL_RSP_CHANNEL_E_VAL, 3'b111 }
         } evl_tlink_rsp_opcode_t;

      typedef enum bit [5:0] {
            TL_param_none = 6'b000000, // reserved param
            TL_param_0x01 = 6'b000001, // reserved param
            TL_param_0x02 = 6'b000010, // reserved param
            TL_param_0x03 = 6'b000011, // reserved param
            TL_param_0x04 = 6'b000100, // reserved param
            TL_param_0x05 = 6'b000101, // reserved param
            TL_param_0x06 = 6'b000110, // reserved param
            TL_param_0x07 = 6'b000111, // reserved param
            TL_pref_rd    = 6'b001000, // Intent prefetch read
            TL_pref_wr    = 6'b001001, // Intent prefetch write
            TL_param_0x0a = 6'b001010, // reserved
            TL_param_0x0b = 6'b001011, // reserved
            TL_param_0x0c = 6'b001100, // reserved
            TL_param_0x0d = 6'b001101, // reserved
            TL_param_0x0e = 6'b001110, // reserved
            TL_param_0x0f = 6'b001111, // reserved
            TL_xor        = 6'b010000, // logical atomic XOR
            TL_or         = 6'b010001, // logical atomic OR
            TL_and        = 6'b010010, // logical atomic AND
            TL_swap       = 6'b010011, // logical atomic swap
            TL_param_0x14 = 6'b010100, // reserved
            TL_param_0x15 = 6'b010101, // reserved
            TL_param_0x16 = 6'b010110, // reserved
            TL_param_0x17 = 6'b010111, // reserved
            TL_min        = 6'b011000, // arithmetic atomic minimum
            TL_max        = 6'b011001, // arithmetic atomic maximum
            TL_minu       = 6'b011010, // arithmetic atomic unsigned minimum
            TL_maxu       = 6'b011011, // arithmetic atomic unsigned maximum
            TL_add        = 6'b011100, // arithmetic atomic add
            TL_param_0x1d = 6'b011101, // reserved
            TL_param_0x1e = 6'b011110, // reserved
            TL_param_0x1f = 6'b011111, // reserved
            TL_NtoB       = 6'b100000, // channel A AcquireBlock/AcquirePerm grow
            TL_NtoT       = 6'b100001, // channel A AcquireBlock/AcquirePerm grow
            TL_BtoT       = 6'b100010, // channel A AcquireBlock/AcquirePerm grow
            TL_param_0x23 = 6'b100011, // reserved
            TL_param_0x24 = 6'b100100, // reserved
            TL_param_0x25 = 6'b100101, // reserved
            TL_param_0x26 = 6'b100110, // reserved
            TL_param_0x27 = 6'b100111, // reserved
            TL_toT        = 6'b101000, // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
            TL_toB        = 6'b101001, // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
            TL_toN        = 6'b101010, // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
            TL_param_0x2b = 6'b101011, // reserved
            TL_param_0x2c = 6'b101100, // reserved
            TL_param_0x2d = 6'b101101, // reserved
            TL_param_0x2e = 6'b101110, // reserved
            TL_param_0x2f = 6'b101111, // reserved
            TL_TtoB       = 6'b110000, // channel C ProbeAck/ProbeAckData/Release/ReleaseData
            TL_TtoN       = 6'b110001, // channel C ProbeAck/ProbeAckData/Release/ReleaseData
            TL_BtoN       = 6'b110010, // channel C ProbeAck/ProbeAckData/Release/ReleaseData
            TL_TtoT       = 6'b110011, // channel C ProbeAck/ProbeAckData/Release/ReleaseData
            TL_BtoB       = 6'b110100, // channel C ProbeAck/ProbeAckData/Release/ReleaseData
            TL_NtoN       = 6'b110101, // channel C ProbeAck/ProbeAckData/Release/ReleaseData
            TL_param_0x36 = 6'b110110, // reserved
            TL_param_0x37 = 6'b110111  // reserved
         } evl_tlink_param_t;

      typedef struct packed {
            evl_trans_id_t         trans_id;
            evl_tlink_req_opcode_t opcode;
            evl_tlink_param_t      param;
            evl_bus_req_size_t     cmd_size;
            evl_bus_req_id_t       source;
            evl_paddr_t            address;
            evl_xword_byte_en_t    mask;
            evl_xword_t            data;
            bit                    corrupt;
         } evl_tlink_bus_req_s;

      typedef struct packed {
            evl_tlink_rsp_opcode_t opcode;
            evl_tlink_param_t      param;
            evl_bus_req_size_t     data_size;
            evl_bus_req_id_t       source;
            evl_bus_req_id_t       sink;
            evl_xword_t            data;
            bit                    denied;
            bit                    corrupt;
         } evl_tlink_bus_rsp_s;

      typedef enum int {
            EVL_COV_TLINK_NONE     = 0,
            EVL_COV_TLINK_COH_CORE = 1,
            EVL_COV_TLINK_MCE_CORE = 2,
            EVL_COV_TLINK_BOOTRAM  = 3,
            EVL_COV_TLINK_MMIO     = 4,
            EVL_COV_TLINK_MEM_HUB  = 5,
            EVL_COV_TLINK_SC_HUB   = 6,
            EVL_COV_TLINK_FCU      = 7,
            EVL_COV_TLINK_GAG      = 8,
            EVL_COV_TLINK_SRAMCFG  = 9,
            EVL_COV_TLINK_CLINT    = 10,
            EVL_COV_TLINK_DEBUG    = 11,
            EVL_COV_TLINK_ERRDEV   = 12,
            EVL_COV_TLINK_SBM      = 13
         } tlink_cov_type_t;


      `ifdef EVL_SIMULATION
         typedef class evl_tlink_bus_parser;
         typedef class evl_tlink_bus_req;
         typedef class evl_tlink_bus_rsp;
         typedef class evl_tlink_master_agent;
         typedef class evl_tlink_master_driver;
         typedef class evl_tlink_master_sequencer;
         typedef class evl_tlink_monitor;
         typedef class evl_tlink_rtl_port;
         typedef class evl_tlink_slave_agent;
         typedef class evl_tlink_slave_driver;
         typedef class evl_tlink_slave_sequencer;
         typedef class evl_tlink_verif_comp;

         `ifdef ET_INCLUDE_USR_PORT
            `include "dv/common/tlink/tlink_vc/usr_tlink_rtl_port.sv"
         `endif
         `include "dv/common/tlink/tlink_vc/evl_tlink_bus_req.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_bus_rsp.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_rtl_port.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_bus_parser.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_seq_lib.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_master_sequencer.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_slave_agent.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_slave_driver.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_slave_sequencer.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_master_driver.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_master_agent.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_monitor.sv"
         `include "dv/common/tlink/tlink_vc/evl_tlink_verif_comp.sv"
      `else // ifdef EVL_SIMULATION
         `include "dv/common/tlink/tlink_vc/usr_tlink_rtl_port.sv"
      `endif // !ifdef EVL_SIMULATION
   endpackage : evl_tlink_pkg

   `ifdef ET_SIMULATION
      `include "dv/common/tlink/tlink_vc/evl_tlink_cov_pkg.sv"
   `endif // ifdef ET_SIMULATION

`endif // !__EVL_TLINK_PKG
