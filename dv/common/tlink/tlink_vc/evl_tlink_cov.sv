
class evl_tlink_cov #(parameter tlink_cov_type_t TLINK_COV_TYPE = EVL_COV_TLINK_NONE);

   localparam int ADDR_WIDTH =    (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) ? 40 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MCE_CORE) ? 22 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_BOOTRAM)  ? 17 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO)     ? 39 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB)  ? 40 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SC_HUB)   ? 40 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_FCU)      ? 21 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_GAG)      ? 22 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SRAMCFG)  ? 21 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_CLINT)    ? 26 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_DEBUG)    ? 12 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_ERRDEV)   ? 40 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SBM)      ? 40 : 40;

   localparam int DATA_WIDTH =    (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) ? 128 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MCE_CORE) ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_BOOTRAM)  ? 128 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO)     ? 128 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB)  ? 128 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SC_HUB)   ? 128 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_FCU)      ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_GAG)      ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SRAMCFG)  ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_CLINT)    ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_DEBUG)    ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_ERRDEV)   ? 64  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SBM)      ? 128 : 64;

   localparam int DATA_SZ_WIDTH = (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) ? 4 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MCE_CORE) ? 3 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_BOOTRAM)  ? 3 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO)     ? 4 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB)  ? 3 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SC_HUB)   ? 3 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_FCU)      ? 2 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_GAG)      ? 2 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SRAMCFG)  ? 2 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_CLINT)    ? 2 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_DEBUG)    ? 2 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_ERRDEV)   ? 4 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SBM)      ? 4 : 1;

   localparam int ASOURCE_WIDTH = (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) ? 5  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MCE_CORE) ? 8  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_BOOTRAM)  ? 8  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO)     ? 8  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB)  ? 8  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SC_HUB)   ? 10 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_FCU)      ? 12 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_GAG)      ? 12 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SRAMCFG)  ? 12 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_CLINT)    ? 12 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_DEBUG)    ? 12 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_ERRDEV)   ? 8  :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SBM)      ? 1  : 1;

   localparam int BSOURCE_WIDTH = (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) ? 5 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MCE_CORE) ? 8 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_BOOTRAM)  ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO)     ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB)  ? 8 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SC_HUB)   ? 5 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_FCU)      ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_GAG)      ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SRAMCFG)  ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_CLINT)    ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_DEBUG)    ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_ERRDEV)   ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SBM)      ? 1 : 1;

   localparam int SINK_WIDTH =    (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) ? 4 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MCE_CORE) ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_BOOTRAM)  ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO)     ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB)  ? 4 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SC_HUB)   ? 4 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_FCU)      ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_GAG)      ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SRAMCFG)  ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_CLINT)    ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_DEBUG)    ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_ERRDEV)   ? 1 :
                                  (TLINK_COV_TYPE == EVL_COV_TLINK_SBM)      ? 1 : 1;

   localparam int DATA_EN_WIDTH = $clog2(DATA_WIDTH);
   localparam int ASOURCE_LIMIT = (1 << ASOURCE_WIDTH) - 1;
   localparam int BSOURCE_LIMIT = (1 << BSOURCE_WIDTH) - 1;
   localparam int ASOURCE_BINS  = (ASOURCE_WIDTH < 8) ? (1 << ASOURCE_WIDTH) : 128;

   bit                     m_cover_data;


   evl_tlink_req_opcode_t  a_opcode;
   evl_tlink_param_t       a_param;
   bit [4:0]               a_size;       // The absolute maximum a_size width is 4 bits
   bit [12:0]              a_source;     // The absolute maximum a_source width is 12 bits
   bit [40:0]              a_address;    // The absolute maximum a_address width is 40 bits
   bit [16:0]              a_mask;       // The absolute maximum a_mask width is 16 bits
   bit [128:0]             a_data;       // The absolute maximum a_data width is 128 bits
   bit                     a_corrupt;
   bit [39:0]              a_addr_4k;    // watch for address bits 11:0 to be 0x000 and 0xff8
   bit [39:0]              a_addr_32k;   // watch for address bits 14:0 to be 0x0000 and 0x7ff8
   bit [39:0]              a_addr_scp;   // watch for address bits 22:0 to be 0x000000 and 0x7ffff8
   bit [39:0]              a_addr_coh;   // watch for address bits 34:0 to be 0x000000000 and 0x7ffffffc0

   evl_tlink_req_opcode_t  b_opcode;
   evl_tlink_param_t       b_param;
   bit [DATA_SZ_WIDTH-1:0] b_size;
   bit [8:0]               b_source;     // The absolute maximum b_source width is 8 bits
   bit [ADDR_WIDTH-1:0]    b_address;
   bit [DATA_EN_WIDTH-1:0] b_mask;
   bit [DATA_WIDTH-1:0]    b_data;
   bit                     b_corrupt;

   evl_tlink_req_opcode_t  c_req_opcode;
   evl_tlink_param_t       c_req_param;
   bit [DATA_SZ_WIDTH-1:0] c_req_size;
   bit [8:0]               c_req_source; // The absolute maximum c_source width is 8 bits
   bit [ADDR_WIDTH-1:0]    c_req_address;
   bit [DATA_WIDTH-1:0]    c_req_data;
   bit                     c_req_corrupt;

   evl_tlink_rsp_opcode_t  c_rsp_opcode;
   evl_tlink_param_t       c_rsp_param;
   bit [8:0]               c_rsp_source; // The absolute maximum c_source width is 8 bits
   bit [DATA_WIDTH-1:0]    c_rsp_data;
   bit                     c_rsp_corrupt;

   evl_tlink_rsp_opcode_t  d_opcode;
   evl_tlink_param_t       d_param;
   bit [DATA_SZ_WIDTH-1:0] d_size;
   bit [12:0]              d_source;     // The absolute maximum a_source width is 12 bits
   bit [SINK_WIDTH-1:0]    d_sink;
   bit                     d_denied;
   bit [DATA_WIDTH-1:0]    d_data;
   bit                     d_corrupt;

   evl_tlink_rsp_opcode_t  e_opcode;
   bit [SINK_WIDTH-1:0]    e_sink;

   // addr[39:32]: 0xc8-0xff: reserved
   //              0xc0-0xc7: cacheable (system)
   //              0x88-0xbf: Reserved
   //              0x80-0x87: cacheable (L3)
   //              0x40-0x7f: PCIE
   //              0x02-0x03: Reserved
   //              0x01:      ESR
   //              0x00:      Lower
   // Mem + MMIO + CPU: addr[31:28]
   // Mem + MMIO + CPU: addr[27:24]
   // Mem + MMIO + CPU: addr[23:20]
   // Mem + MMIO + CPU: addr[19:16]
   // Mem + MMIO + CPU: addr[15:12]
   // Mem + MMIO + CPU: addr[11:8]
   // Mem + MMIO + CPU: addr[7:6]
   // MMIO + CPU:       addr[5:0]
   // BootRAM:          addr[11:10]
   // BootRAM:          addr[9:8]
   // BootRAM:          addr[7:6]
   // BootRAM:          addr[5:4]
   // BootRAM:          addr[3:0] ?
   // For CPU: transition areas
   //
   // Maxion debug:       0x00_0000_0000 to 0x00_0000_0fff
   // Maxion err dev:     0x00_0000_3000 to 0x00_0000_3fff
   // Maxion boot RAM:    0x00_0001_0000 to 0x00_0001_1fff
   // Maxion FCU-0:       0x00_0010_0000 to 0x00_0010_0fff
   // Maxion FCU-1:       0x00_0010_1000 to 0x00_0010_1fff
   // Maxion FCU-2:       0x00_0010_2000 to 0x00_0010_2fff
   // Maxion FCU-3:       0x00_0010_3000 to 0x00_0010_3fff
   // Maxion SRAM cfg:    0x00_0010_4000 to 0x00_0010_7fff
   // Maxion Err-0:       0x00_0020_0000 to 0x00_0020_0fff
   // Maxion Err-1:       0x00_0020_1000 to 0x00_0020_1fff
   // Maxion Err-2:       0x00_0020_2000 to 0x00_0020_2fff
   // Maxion Err-3:       0x00_0020_3000 to 0x00_0020_3fff
   // Maxion GAG-0:       0x00_0030_0000 to 0x00_0030_0fff
   // Maxion GAG-1:       0x00_0030_1000 to 0x00_0030_1fff
   // Maxion GAG-2:       0x00_0030_2000 to 0x00_0030_2fff
   // Maxion GAG-3:       0x00_0030_3000 to 0x00_0030_3fff
   // Maxion CLINT:       0x00_0200_0000 to 0x00_0200_ffff
   // PU PLIC:            0x00_1000_0000 to 0x00_11ff_ffff
   // PU i2c:             0x00_1200_0000 to 0x00_1200_0fff
   // PU spi:             0x00_1200_1000 to 0x00_1200_1fff
   // PU uart:            0x00_1200_2000 to 0x00_1200_2fff
   // PU gpio:            0x00_1200_3000 to 0x00_1200_3fff
   // PU wdt:             0x00_1200_4000 to 0x00_1200_4fff
   // PU timer:           0x00_1200_5000 to 0x00_1200_5fff
   // PU i3c:             0x00_1200_6000 to 0x00_1200_6fff
   // PU uart1:           0x00_1200_7000 to 0x00_1200_7fff
   // PU USB0 reloc:      0x00_1200_8000 to 0x00_1200_8fff
   // PU USB1 reloc:      0x00_1200_9000 to 0x00_1200_9fff
   // PU DMA reloc:       0x00_1200_a000 to 0x00_1200_afff
   // PU static:          0x00_1400_a000 to 0x00_1403_ffff
   // PU USB0:            0x00_1800_0000 to 0x00_1803_ffff
   // PU USB1:            0x00_1804_0000 to 0x00_1807_ffff
   // PU EMMC cfg:        0x00_1808_0000 to 0x00_1808_1fff
   // PU DMA cfg:         0x00_1808_2000 to 0x00_1808_2fff
   // PU Trigger MM:      0x00_2000_0000 to 0x00_2000_1fff
   // PU Trigger MM/SP:   0x00_2000_2000 to 0x00_2000_3fff
   // PU SRAM MM/Max:     0x00_2000_4000 to 0x00_2000_4fff
   // PU Trigger MM/Max:  0x00_2000_5000 to 0x00_2000_5fff
   // PU Trigger MM/SP:   0x00_2000_6000 to 0x00_2000_6fff
   // PU Trigger PC/MM:   0x00_2000_7000 to 0x00_2000_7fff
   // PU SRAM low:        0x00_2000_8000 to 0x00_2000_ffff
   // PU SRAM mid:        0x00_2001_0000 to 0x00_2001_ffff
   // PU SRAM high:       0x00_2002_0000 to 0x00_2003_ffff
   // PU Trigger Max/SP:  0x00_3000_0000 to 0x00_3000_0fff
   // PU Trigger PC/Max:  0x00_3000_1000 to 0x00_3000_1fff
   // PU Trigger spare:   0x00_3000_2000 to 0x00_3000_2fff
   // PU Trigger PC/SP:   0x00_3000_3000 to 0x00_3000_3fff
   // PU Trigger Max:     0x00_3000_4000 to 0x00_3000_5fff
   // PU Trigger Max/SP:  0x00_3000_6000 to 0x00_3000_7fff
   // PU Trigger PCIE:    0x00_3000_8000 to 0x00_3000_8fff
   // PU Trigger PCIE/SP: 0x00_3000_a000 to 0x00_3000_bfff
   // PU SP only:         0x00_4000_0000 to 0x00_7fff_ffff
   // Scratchpad 0:       0x00_8000_0000 to 0x00_807f_ffff
   // Scratchpad 31:      0x00_8f80_0000 to 0x00_8fff_ffff
   // Scratchpad 32:      0x00_9000_0000 to 0x00_907f_ffff
   //
   // Cross of certain aspects vs targets?
   //
   // Sizes
   // Data


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Maxion TileLink Requests
   //
   // This cover group is used to provide coverage for requests on coherent Maxion TileLink
   // interfaces.
   //
   covergroup cg_tlink_req_core;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_areq_opcode: coverpoint a_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLA_PutFull, TLA_Get, TLA_AtomicArith, TLA_AtomicLogic, TLA_AcquireBlock };
         ignore_bins ignore_values = { TLA_PutPartial,
                                       TLA_Intent,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbeBlock,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TLC_ReleaseData,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_areq_param: coverpoint a_param iff (a_opcode == TLA_AcquireBlock) {
         bins        params[]      = { TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT };     // channel A AcquireBlock/AcquirePerm grow
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                    // TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                    // TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                    // TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                       TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                       TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_areq_size: coverpoint a_size iff (a_opcode != TL_ReqNull) {
         bins sz_1_byte   = { 0 };
         bins sz_2_bytes  = { 1 };
         bins sz_4_bytes  = { 2 };
         bins sz_8_bytes  = { 3 };
         bins sz_16_bytes = { 4 };
         bins sz_32_bytes = { 5 };
         bins sz_64_bytes = { 6 };
      }

      cp_areq_source: coverpoint a_source iff (a_opcode != TL_ReqNull) {
                                 // MSHRs 0-7, 8?, 16?
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h10 };
         illegal_bins bad       = default;
      }

      cp_areq_addr_39_32: coverpoint a_address[39:32] iff (a_opcode != TL_ReqNull) {
         bins addrs[] = { 8'h00, 8'h01, [8'h40:8'h7f],
                          8'h80, 8'h81, 8'h82, 8'h83, 8'h84, 8'h85, 8'h86, 8'h87,
                          8'hc0, 8'hc1, 8'hc2, 8'hc3, 8'hc4, 8'hc5, 8'hc6, 8'hc7 };
      }

      cp_areq_addr_31_28: coverpoint a_address[31:28] iff (a_opcode != TL_ReqNull) {
         bins addrs[16] = { [0:$] };
      }

      cp_areq_addr_27_24: coverpoint a_address[27:24] iff (a_opcode != TL_ReqNull) {
         bins addrs[16] = { [0:$] };
      }

      cp_areq_addr_23_20: coverpoint a_address[23:20] iff (a_opcode != TL_ReqNull) {
         bins addrs[16] = { [0:$] };
      }

      cp_areq_addr_19_16: coverpoint a_address[19:16] iff (a_opcode != TL_ReqNull) {
         bins addrs[16] = { [0:$] };
      }

      cp_areq_addr_15_12: coverpoint a_address[15:12] iff (a_opcode != TL_ReqNull) {
         bins addrs[16] = { [0:$] };
      }

      cp_areq_addr_11_7: coverpoint a_address[11:7] iff (a_opcode != TL_ReqNull) {
         bins addrs[32] = { [0:$] };
      }

      cp_areq_addr_6_0: coverpoint a_address[6:0] iff (a_opcode != TL_ReqNull) {
         bins addrs[64] = { [0:$] };
      }

      cp_areq_edge_32_31: coverpoint a_address[32:31] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_edge_28_27: coverpoint a_address[28:27] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_edge_24_23: coverpoint a_address[24:23] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_edge_20_19: coverpoint a_address[20:19] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_edge_16_15: coverpoint a_address[16:15] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_edge_11_10: coverpoint a_address[11:10] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_edge_7_6: coverpoint a_address[7:6] iff (a_opcode != TL_ReqNull) {
         bins addrs[4] = { [0:$] };
      }

      cp_areq_addr_4k: coverpoint a_addr_4k iff (a_opcode != TL_ReqNull) {
         bins addrs[]       = { 40'h0000010000, 40'h0000010ff8, 40'h0000011000, 40'h0000011ff8,
                                40'h0000100000, 40'h0000100ff8, 40'h0000101000, 40'h0000101ff8,
                                40'h0000102000, 40'h0000102ff8, 40'h0000103000, 40'h0000103ff8,
                                40'h0000200000, 40'h0000200ff8, 40'h0000201000, 40'h0000201ff8,
                                40'h0000202000, 40'h0000202ff8, 40'h0000203000, 40'h0000203ff8,
                                40'h0000300000, 40'h0000300ff8, 40'h0000301000, 40'h0000301ff8,
                                40'h0000302000, 40'h0000302ff8, 40'h0000303000, 40'h0000303ff8,
                                40'h0002000000, 40'h0002000ff8, 40'h0002001000, 40'h0002001ff8,
                                40'h0002002000, 40'h0002002ff8, 40'h0002003000, 40'h0002003ff8,
                                40'h0002004000, 40'h0002004ff8, 40'h0002005000, 40'h0002005ff8,
                                40'h0002006000, 40'h0002006ff8, 40'h0002007000, 40'h0002007ff8,
                                40'h0002008000, 40'h0002008ff8, 40'h0002009000, 40'h0002009ff8,
                                40'h000200a000, 40'h000200aff8, 40'h000200b000, 40'h000200bff8,
                                40'h000200c000, 40'h000200cff8, 40'h000200d000, 40'h000200dff8,
                                40'h000200e000, 40'h000200eff8, 40'h000200f000, 40'h000200fff8,
                                40'h0010000000, 40'h0010000ff8, 40'h0011fff000, 40'h0011fffff8,
                                40'h0012000000, 40'h0012000ff8, 40'h0012001000, 40'h0012001ff8,
                                40'h0012002000, 40'h0012002ff8, 40'h0012003000, 40'h0012003ff8,
                                40'h0012004000, 40'h0012004ff8, 40'h0012005000, 40'h0012005ff8,
                                40'h0012006000, 40'h0012006ff8, 40'h0012007000, 40'h0012007ff8,
                                40'h0012008000, 40'h0012008ff8, 40'h0012009000, 40'h0012009ff8,
                                40'h001200a000, 40'h001200aff8, 40'h001400a000, 40'h001400aff8,
                                40'h001403f000, 40'h001403fff8, 40'h0018000000, 40'h0018000ff8,
                                40'h001803f000, 40'h001803fff8, 40'h0018040000, 40'h0018040ff8,
                                40'h001807f000, 40'h001807fff8, 40'h0018080000, 40'h0018080ff8,
                                40'h0018081000, 40'h0018081ff8, 40'h0018082000, 40'h0018082ff8,
                                40'h0020000000, 40'h0020000ff8, 40'h0020001000, 40'h0020001ff8,
                                40'h0020002000, 40'h0020002ff8, 40'h0020003000, 40'h0020003ff8,
                                40'h0020004000, 40'h0020004ff8, 40'h0020005000, 40'h0020005ff8,
                                40'h0020006000, 40'h0020006ff8, 40'h0020007000, 40'h0020007ff8,
                                40'h0030000000, 40'h0030000ff8, 40'h0030001000, 40'h0030001ff8,
                                40'h0030002000, 40'h0030002ff8, 40'h0030003000, 40'h0030003ff8,
                                40'h0030004000, 40'h0030004ff8, 40'h0030005000, 40'h0030005ff8,
                                40'h0030006000, 40'h0030006ff8, 40'h0030007000, 40'h0030007ff8,
                                40'h0030008000, 40'h0030008ff8, 40'h003000a000, 40'h003000aff8,
                                40'h003000b000, 40'h003000bff8 };
         bins ignore_values = default;
      }

      cp_areq_addr_32k: coverpoint a_addr_32k iff (a_opcode != TL_ReqNull) {
         bins addrs[]       = { 40'h0020008000, 40'h002000fff8,
                                40'h0020010000, 40'h0020017ff8,
                                40'h0020018000, 40'h002001fff8,
                                40'h0020020000, 40'h0020027ff8,
                                40'h0020028000, 40'h002002fff8 };
         bins ignore_values = default;
      }

      cp_areq_addr_scp: coverpoint a_addr_scp iff (a_opcode != TL_ReqNull) {
         bins addrs[]       = { 40'h0080000000, 40'h00807ffff8,   // shire 0
                                40'h0080800000, 40'h0080fffff8,   // shire 1
                                40'h0081000000, 40'h00817ffff8,   // shire 2
                                40'h0081800000, 40'h0081fffff8,   // shire 3
                                40'h0082000000, 40'h00817ffff8,   // shire 4
                                40'h0082800000, 40'h0081fffff8,   // shire 5
                                40'h0083000000, 40'h00817ffff8,   // shire 6
                                40'h0083800000, 40'h0081fffff8,   // shire 7
                                40'h0084000000, 40'h00847ffff8,   // shire 8
                                40'h0084800000, 40'h0084fffff8,   // shire 9
                                40'h0085000000, 40'h00857ffff8,   // shire 10
                                40'h0085800000, 40'h0085fffff8,   // shire 11
                                40'h0086000000, 40'h00867ffff8,   // shire 12
                                40'h0086800000, 40'h0086fffff8,   // shire 13
                                40'h0087000000, 40'h00877ffff8,   // shire 14
                                40'h0087800000, 40'h0087fffff8,   // shire 15
                                40'h0088000000, 40'h00887ffff8,   // shire 16
                                40'h0088800000, 40'h0088fffff8,   // shire 17
                                40'h0089000000, 40'h00897ffff8,   // shire 18
                                40'h0089800000, 40'h0089fffff8,   // shire 19
                                40'h008a000000, 40'h008a7ffff8,   // shire 20
                                40'h008a800000, 40'h008afffff8,   // shire 21
                                40'h008b000000, 40'h008b7ffff8,   // shire 22
                                40'h008b800000, 40'h008bfffff8,   // shire 23
                                40'h008c000000, 40'h008c7ffff8,   // shire 24
                                40'h008c800000, 40'h008cfffff8,   // shire 25
                                40'h008d000000, 40'h008d7ffff8,   // shire 26
                                40'h008d800000, 40'h008dfffff8,   // shire 27
                                40'h008e000000, 40'h008e7ffff8,   // shire 28
                                40'h008e800000, 40'h008efffff8,   // shire 29
                                40'h008f000000, 40'h008f7ffff8,   // shire 30
                                40'h008f800000, 40'h008ffffff8,   // shire 31
                                40'h0090000000, 40'h00907ffff8 }; // shire 32
         bins ignore_values = default;
      }

      cp_breq_opcode: coverpoint b_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLB_ProbeBlock };
         ignore_bins ignore_values = { TLA_PutFull,
                                       TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Get,
                                       TLA_Intent,
                                       TLA_AcquireBlock,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TLC_ReleaseData,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_breq_param: coverpoint b_param iff (b_opcode == TLB_ProbeBlock) {
         bins        params[]      = { TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN };      // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                    // TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                       TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_breq_source: coverpoint b_source iff (b_opcode != TL_ReqNull) {
         bins         sources[] = { 1313'h00, 1313'h01, 1313'h02, 1313'h03, 1313'h04, 1313'h05, 1313'h06, 1313'h07, 1313'h08, 1313'h10,
                                    1313'h20, 1313'h21, 1313'h22, 1313'h23, 1313'h24, 1313'h25, 1313'h26, 1313'h27, 1313'h28, 1313'h30,
                                    1313'h40, 1313'h41, 1313'h42, 1313'h43, 1313'h44, 1313'h45, 1313'h46, 1313'h47, 1313'h48, 1313'h50,
                                    1313'h60, 1313'h61, 1313'h62, 1313'h63, 1313'h64, 1313'h65, 1313'h66, 1313'h67, 1313'h68, 1313'h70 };
         illegal_bins bad       = default;
      }

      cp_creq_opcode: coverpoint c_req_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLC_ReleaseData };
         ignore_bins ignore_values = { TLA_PutFull,
                                       TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Get,
                                       TLA_Intent,
                                       TLA_AcquireBlock,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbeBlock,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_creq_param: coverpoint c_req_param iff (c_req_opcode != TL_ReqNull) {
         bins        params[]      = { TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN };     // channel C ProbeAck/ProbeAckData/Release/ReleaseData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                       TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                    // TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_creq_source: coverpoint c_req_source iff (c_req_opcode != TL_ReqNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h09 };
         illegal_bins bad       = default;
      }
   endgroup : cg_tlink_req_core


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Maxion TileLink Responses
   //
   // This cover group is used to provide coverage for responses on core Maxion TileLink
   // interfaces.
   //
   covergroup cg_tlink_rsp_core;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_crsp_opcode: coverpoint c_rsp_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLC_ProbeAck, TLC_ProbeAckData };
         ignore_bins ignore_values = { TLD_AccessAck,
                                       TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                       TLD_Grant,
                                       TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                    // TLC_ProbeAck,
                                    // TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                       TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_crsp_param: coverpoint c_rsp_param iff (c_rsp_opcode != TL_RspNull) {
         bins        params[]      = { TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN };     // channel C ProbeAck/ProbeAckData/Release/ReleaseData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                       TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                    // TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_crsp_source: coverpoint c_rsp_source iff (c_rsp_opcode != TL_RspNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h10 };
         illegal_bins bad       = default;
      }

      cp_drsp_opcode: coverpoint d_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLD_AccessAckData, TLD_Grant, TLD_GrantData };
         ignore_bins ignore_values = { TLD_AccessAck,
                                    // TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                    // TLD_Grant,
                                    // TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                       TLC_ProbeAck,
                                       TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                       TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_drsp_param: coverpoint d_param iff (d_opcode != TL_RspNull) {
         bins        params[]      = { TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN };      // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                    // TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                       TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_drsp_source: coverpoint d_source iff (d_opcode != TL_RspNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h09, 13'h10 };
         illegal_bins bad       = default;
      }

      cp_ersp_opcode: coverpoint e_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLE_GrantAck };
         ignore_bins ignore_values = { TLD_AccessAck,
                                       TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                       TLD_Grant,
                                       TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                       TLC_ProbeAck,
                                       TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                    // TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_ersp_source: coverpoint e_sink iff (e_opcode != TL_RspNull) {
         bins sources[] = default;
      }
   endgroup : cg_tlink_rsp_core


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Broadcast Hub TileLink Requests
   //
   // This cover group is used to provide coverage for requests on coherent broadcast hub TileLink
   // interfaces.
   //
   covergroup cg_tlink_req_hub;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_areq_opcode: coverpoint a_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLA_Get, TLA_AcquireBlock };
         ignore_bins ignore_values = { TLA_PutFull,
                                       TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Intent,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbeBlock,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TLC_ReleaseData,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_areq_param: coverpoint a_param iff (a_opcode == TLA_AcquireBlock) {
         bins        params[]      = { TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT };     // channel A AcquireBlock/AcquirePerm grow
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                    // TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                    // TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                    // TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                       TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                       TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_areq_source: coverpoint a_source iff (a_opcode != TL_ReqNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h10,
                                    13'h20, 13'h21, 13'h22, 13'h23, 13'h24, 13'h25, 13'h26, 13'h27, 13'h28, 13'h30,
                                    13'h40, 13'h41, 13'h42, 13'h43, 13'h44, 13'h45, 13'h46, 13'h47, 13'h48, 13'h50,
                                    13'h60, 13'h61, 13'h62, 13'h63, 13'h64, 13'h65, 13'h66, 13'h67, 13'h68, 13'h70 };
         illegal_bins bad       = default;
      }

      cp_breq_opcode: coverpoint b_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLB_ProbeBlock };
         ignore_bins ignore_values = { TLA_PutFull,
                                       TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Get,
                                       TLA_Intent,
                                       TLA_AcquireBlock,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TLC_ReleaseData,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_breq_param: coverpoint b_param iff (b_opcode == TLB_ProbeBlock) {
         bins        params[]      = { TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN };      // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                    // TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                       TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_breq_source: coverpoint b_source iff (b_opcode != TL_ReqNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h10,
                                    13'h20, 13'h21, 13'h22, 13'h23, 13'h24, 13'h25, 13'h26, 13'h27, 13'h28, 13'h30,
                                    13'h40, 13'h41, 13'h42, 13'h43, 13'h44, 13'h45, 13'h46, 13'h47, 13'h48, 13'h50,
                                    13'h60, 13'h61, 13'h62, 13'h63, 13'h64, 13'h65, 13'h66, 13'h67, 13'h68, 13'h70 };
         illegal_bins bad       = default;
      }

      cp_creq_opcode: coverpoint c_req_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLC_ReleaseData };
         ignore_bins ignore_values = { TLA_PutFull,
                                       TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Get,
                                       TLA_Intent,
                                       TLA_AcquireBlock,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbeBlock,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_creq_param: coverpoint c_req_param iff (c_req_opcode != TL_ReqNull) {
         bins        params[]      = { TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN };     // channel C ProbeAck/ProbeAckData/Release/ReleaseData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                       TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                    // TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_creq_source: coverpoint c_req_source iff (c_req_opcode != TL_ReqNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h09,
                                    13'h20, 13'h21, 13'h22, 13'h23, 13'h24, 13'h25, 13'h26, 13'h27, 13'h29,
                                    13'h40, 13'h41, 13'h42, 13'h43, 13'h44, 13'h45, 13'h46, 13'h47, 13'h49,
                                    13'h60, 13'h61, 13'h62, 13'h63, 13'h64, 13'h65, 13'h66, 13'h67, 13'h69 };
         illegal_bins bad       = default;
      }
   endgroup : cg_tlink_req_hub


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Broadcast Hub TileLink Responses
   //
   // This cover group is used to provide coverage for responses on coherent broadcast hub TileLink
   // interfaces.
   //
   covergroup cg_tlink_rsp_hub;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_crsp_opcode: coverpoint c_rsp_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLC_ProbeAck, TLC_ProbeAckData };
         ignore_bins ignore_values = { TLD_AccessAck,
                                       TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                       TLD_Grant,
                                       TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                    // TLC_ProbeAck,
                                    // TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                       TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_crsp_param: coverpoint c_rsp_param iff (c_rsp_opcode != TL_RspNull) {
         bins        params[]      = { TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN };     // channel C ProbeAck/ProbeAckData/Release/ReleaseData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                       TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                    // TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                    // TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_crsp_source: coverpoint c_rsp_source iff (c_rsp_opcode != TL_RspNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h10,
                                    13'h20, 13'h21, 13'h22, 13'h23, 13'h24, 13'h25, 13'h26, 13'h27, 13'h28, 13'h30,
                                    13'h40, 13'h41, 13'h42, 13'h43, 13'h44, 13'h45, 13'h46, 13'h47, 13'h48, 13'h50,
                                    13'h60, 13'h61, 13'h62, 13'h63, 13'h64, 13'h65, 13'h66, 13'h67, 13'h68, 13'h70 };
         illegal_bins bad       = default;
      }

      cp_drsp_opcode: coverpoint d_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLD_AccessAckData, TLD_Grant, TLD_GrantData };
         ignore_bins ignore_values = { TLD_AccessAck,
                                    // TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                    // TLD_Grant,
                                    // TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                       TLC_ProbeAck,
                                       TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                       TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_drsp_param: coverpoint d_param iff (d_opcode != TL_RspNull) {
         bins        params[]      = { TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_toN };      // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
         ignore_bins ignore_values = { TL_param_none, // reserved param
                                       TL_param_0x01, // reserved param
                                       TL_param_0x02, // reserved param
                                       TL_param_0x03, // reserved param
                                       TL_param_0x04, // reserved param
                                       TL_param_0x05, // reserved param
                                       TL_param_0x06, // reserved param
                                       TL_param_0x07, // reserved param
                                       TL_pref_rd,    // Intent prefetch read
                                       TL_pref_wr,    // Intent prefetch write
                                       TL_param_0x0a, // reserved
                                       TL_param_0x0b, // reserved
                                       TL_param_0x0c, // reserved
                                       TL_param_0x0d, // reserved
                                       TL_param_0x0e, // reserved
                                       TL_param_0x0f, // reserved
                                       TL_xor,        // logical atomic XOR
                                       TL_or,         // logical atomic OR
                                       TL_and,        // logical atomic AND
                                       TL_swap,       // logical atomic swap
                                       TL_param_0x14, // reserved
                                       TL_param_0x15, // reserved
                                       TL_param_0x16, // reserved
                                       TL_param_0x17, // reserved
                                       TL_min,        // arithmetic atomic minimum
                                       TL_max,        // arithmetic atomic maximum
                                       TL_minu,       // arithmetic atomic unsigned minimum
                                       TL_maxu,       // arithmetic atomic unsigned maximum
                                       TL_add,        // arithmetic atomic add
                                       TL_param_0x1d, // reserved
                                       TL_param_0x1e, // reserved
                                       TL_param_0x1f, // reserved
                                       TL_NtoB,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_NtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_BtoT,       // channel A AcquireBlock/AcquirePerm grow
                                       TL_param_0x23, // reserved
                                       TL_param_0x24, // reserved
                                       TL_param_0x25, // reserved
                                       TL_param_0x26, // reserved
                                       TL_param_0x27, // reserved
                                    // TL_toT,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toB,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                    // TL_toN,        // channel B ProbeBlock/ProbePerm cap, channel D Grant/GrantData
                                       TL_param_0x2b, // reserved
                                       TL_param_0x2c, // reserved
                                       TL_param_0x2d, // reserved
                                       TL_param_0x2e, // reserved
                                       TL_param_0x2f, // reserved
                                       TL_TtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_TtoT,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_BtoB,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_NtoN,       // channel C ProbeAck/ProbeAckData/Release/ReleaseData
                                       TL_param_0x36, // reserved
                                       TL_param_0x37 };
      }

      cp_drsp_source: coverpoint d_source iff (d_opcode != TL_RspNull) {
         bins         sources[] = { 13'h00, 13'h01, 13'h02, 13'h03, 13'h04, 13'h05, 13'h06, 13'h07, 13'h08, 13'h09, 13'h10,
                                    13'h20, 13'h21, 13'h22, 13'h23, 13'h24, 13'h25, 13'h26, 13'h27, 13'h28, 13'h29, 13'h30,
                                    13'h40, 13'h41, 13'h42, 13'h43, 13'h44, 13'h45, 13'h46, 13'h47, 13'h48, 13'h49, 13'h50,
                                    13'h60, 13'h61, 13'h62, 13'h63, 13'h64, 13'h65, 13'h66, 13'h67, 13'h68, 13'h69, 13'h70 };
         illegal_bins bad       = default;
      }

      cp_ersp_opcode: coverpoint e_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLE_GrantAck };
         ignore_bins ignore_values = { TLD_AccessAck,
                                       TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                       TLD_Grant,
                                       TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                       TLC_ProbeAck,
                                       TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                    // TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_ersp_source: coverpoint e_sink iff (e_opcode != TL_RspNull) {
         bins sources[] = default;
      }
   endgroup : cg_tlink_rsp_hub


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: MMIO TileLink Requests
   //
   // This cover group is used to provide coverage for requests on the MMIO TileLink interface.
   //
   covergroup cg_tlink_req_mmio;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_req_opcode: coverpoint a_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLA_PutFull, TLA_Get };
         ignore_bins ignore_values = { TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Intent,
                                       TLA_AcquireBlock,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbeBlock,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TLC_ReleaseData,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_areq_size: coverpoint a_size iff (a_opcode != TL_ReqNull) {
         bins sz_1_byte   = { 0 };
         bins sz_2_bytes  = { 1 };
         bins sz_4_bytes  = { 2 };
         bins sz_8_bytes  = { 3 };
         bins others      = default;
      }

      cp_areq_source: coverpoint a_source iff (a_opcode != TL_ReqNull) {
         bins        sources[ASOURCE_BINS] = { [0:ASOURCE_LIMIT] };
         ignore_bins invalid_bins          = { [ASOURCE_LIMIT+1:13'h1fff] };
      }

   endgroup : cg_tlink_req_mmio


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: MMIO TileLink Responses
   //
   // This cover group is used to provide coverage for responses on the MMIO TileLink interface.
   //
   covergroup cg_tlink_rsp_mmio;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_drsp_opcode: coverpoint d_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLD_Grant, TLD_GrantData };
         ignore_bins ignore_values = { TLD_AccessAck,
                                       TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                    // TLD_Grant,
                                    // TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                       TLC_ProbeAck,
                                       TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                       TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_drsp_source: coverpoint d_source iff (d_opcode != TL_RspNull) {
         bins        sources[ASOURCE_BINS] = { [0:ASOURCE_LIMIT] };
         ignore_bins invalid_bins          = { [ASOURCE_LIMIT+1:13'h1fff] };
      }
   endgroup : cg_tlink_rsp_mmio


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Noncoherent TileLink Requests
   //
   // This cover group is used to provide coverage for requests on non-coherent TileLink interfaces.
   //
   covergroup cg_tlink_req_noncoh;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_req_opcode: coverpoint a_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLA_PutFull, TLA_Get };
         ignore_bins ignore_values = { TLA_PutPartial,
                                       TLA_AtomicArith,
                                       TLA_AtomicLogic,
                                       TLA_Intent,
                                       TLA_AcquireBlock,
                                       TLA_AcquirePerm,
                                       TLB_PutFull,
                                       TLB_PutPartial,
                                       TLB_AtomicArith,
                                       TLB_AtomicLogic,
                                       TLB_Get,
                                       TLB_Intent,
                                       TLB_ProbeBlock,
                                       TLB_ProbePerm,
                                       TLC_Req0x0,
                                       TLC_Req0x1,
                                       TLC_Req0x2,
                                       TLC_Req0x3,
                                       TLC_Req0x4,
                                       TLC_Req0x5,
                                       TLC_Release,
                                       TLC_ReleaseData,
                                       TL_Req0x0,
                                       TL_Req0x1,
                                       TL_Req0x2,
                                       TL_Req0x3,
                                       TL_Req0x4,
                                       TL_Req0x5,
                                       TL_ReqUnknown,
                                       TL_ReqNull };
      }

      cp_areq_source: coverpoint a_source iff (a_opcode != TL_ReqNull) {
         bins        sources[ASOURCE_BINS] = { [0:ASOURCE_LIMIT] };
         ignore_bins invalid_bins          = { [ASOURCE_LIMIT+1:13'h1fff] };
      }

   endgroup : cg_tlink_req_noncoh


   //-----------------------------------------------------------------------------------------------
   //
   // Covergroup: Noncoherent TileLink Responses
   //
   // This cover group is used to provide coverage for responses on non-coherent TileLink
   // interfaces.
   //
   covergroup cg_tlink_rsp_noncoh;
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_drsp_opcode: coverpoint d_opcode {
         option.weight      = 1;
         option.goal        = 100;
         type_option.weight = 1;
         type_option.goal   = 100;

         bins        opcodes[]     = { TLD_Grant, TLD_GrantData };
         ignore_bins ignore_values = { TLD_AccessAck,
                                       TLD_AccessAckData,
                                       TLD_HintAck,
                                       TLD_Rsp0x3,
                                    // TLD_Grant,
                                    // TLD_GrantData,
                                       TLD_ReleaseAck,
                                       TLD_Rsp0x7,
                                       TLC_AccessAck,
                                       TLC_AccessAckData,
                                       TLC_HintAck,
                                       TLC_Rsp0x3,
                                       TLC_ProbeAck,
                                       TLC_ProbeAckData,
                                       TLC_Rsp0x6,
                                       TLC_Rsp0x7,
                                       TL_Rsp0x0,
                                       TL_Rsp0x1,
                                       TL_Rsp0x2,
                                       TL_Rsp0x3,
                                       TL_Rsp0x4,
                                       TL_Rsp0x5,
                                       TL_Rsp0x6,
                                       TL_Rsp0x7,
                                       TLE_GrantAck,
                                       TLE_Rsp0x1,
                                       TLE_Rsp0x2,
                                       TLE_Rsp0x3,
                                       TLE_Rsp0x4,
                                       TLE_Rsp0x5,
                                       TL_RspUnknown,
                                       TL_RspNull };
      }

      cp_drsp_source: coverpoint d_source iff (d_opcode != TL_RspNull) {
         bins        sources[ASOURCE_BINS] = { [0:ASOURCE_LIMIT] };
         ignore_bins invalid_bins          = { [ASOURCE_LIMIT+1:13'h1fff] };
      }
   endgroup : cg_tlink_rsp_noncoh


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_req
   //
   function void sample_req(input evl_tlink_req_opcode_t req_opcode_in,
                            input evl_paddr_t            req_addr_in,
                            input evl_tlink_param_t      req_param_in,
                            input bit [4:0]              req_size_in,
                            input bit [11:0]             req_source_in);
      if ((int'(req_opcode_in) & `EVL_TL_REQ_CHANNEL_MASK) == `EVL_TL_REQ_CHANNEL_A) begin
         a_opcode     = req_opcode_in;
         a_address    = { {(40-ADDR_WIDTH){1'b0}}, req_addr_in[ADDR_WIDTH-1:0] };
         a_addr_4k    = { a_address[39:3], 3'b000 };
         a_addr_32k   = { a_address[39:3], 3'b000 };
         a_addr_scp   = { a_address[39:3], 3'b000 };
         a_addr_coh   = { a_address[39:6], 6'b000000 };
         a_param      = req_param_in;
         a_size       = { {(40-DATA_SZ_WIDTH){1'b0}}, req_size_in[DATA_SZ_WIDTH-1:0] };
         b_source     = { {(13-ASOURCE_WIDTH){1'b0}}, req_source_in[ASOURCE_WIDTH-1:0] };
         b_opcode     = TL_ReqNull;
      end
      else if ((int'(req_opcode_in) & `EVL_TL_REQ_CHANNEL_MASK) == `EVL_TL_REQ_CHANNEL_B) begin
         b_opcode     = req_opcode_in;
         b_address    = req_addr_in[ADDR_WIDTH-1:0];
         b_param      = req_param_in;
         b_source     = { {(9-BSOURCE_WIDTH){1'b0}}, req_source_in[BSOURCE_WIDTH-1:0] };
         a_opcode     = TL_ReqNull;
         c_req_opcode = TL_ReqNull;
      end
      else if ((int'(req_opcode_in) & `EVL_TL_REQ_CHANNEL_MASK) == `EVL_TL_REQ_CHANNEL_C) begin
         c_req_opcode  = req_opcode_in;
         c_req_address = req_addr_in[ADDR_WIDTH-1:0];
         c_req_param   = req_param_in;
         c_req_source  = { {(9-BSOURCE_WIDTH){1'b0}}, req_source_in[BSOURCE_WIDTH-1:0] };
         a_opcode      = TL_ReqNull;
         b_opcode      = TL_ReqNull;
      end
      if (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) begin
         cg_tlink_req_core.sample();
      end
      else if (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB) begin
         cg_tlink_req_hub.sample();
      end
      else if (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO) begin
         cg_tlink_req_mmio.sample();
      end
      else begin
         cg_tlink_req_noncoh.sample();
      end
   endfunction : sample_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_rsp
   //
   function void sample_rsp(input evl_tlink_rsp_opcode_t rsp_opcode_in,
                            input evl_tlink_param_t      rsp_param_in,
                            input bit [11:0]             rsp_source_in);
      if ((int'(rsp_opcode_in) & `EVL_TL_RSP_CHANNEL_MASK) == `EVL_TL_RSP_CHANNEL_C) begin
         c_rsp_opcode = rsp_opcode_in;
         c_rsp_param  = rsp_param_in;
         c_rsp_source = { {(9-BSOURCE_WIDTH){1'b0}}, rsp_source_in[BSOURCE_WIDTH-1:0] };
         d_opcode     = TL_RspNull;
         e_opcode     = TL_RspNull;
      end
      else if ((int'(rsp_opcode_in) & `EVL_TL_RSP_CHANNEL_MASK) == `EVL_TL_RSP_CHANNEL_D) begin
         d_opcode     = rsp_opcode_in;
         d_param      = rsp_param_in;
         d_source     = { {(13-ASOURCE_WIDTH){1'b0}}, rsp_source_in[ASOURCE_WIDTH-1:0] };
         c_rsp_opcode = TL_RspNull;
         e_opcode     = TL_RspNull;
      end
      else if ((int'(rsp_opcode_in) & `EVL_TL_RSP_CHANNEL_MASK) == `EVL_TL_RSP_CHANNEL_E) begin
         e_opcode     = rsp_opcode_in;
         e_sink       = rsp_source_in[SINK_WIDTH-1:0];
         c_rsp_opcode = TL_RspNull;
         d_opcode     = TL_RspNull;
      end
      if (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) begin
         cg_tlink_rsp_core.sample();
      end
      else if (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB) begin
         cg_tlink_rsp_hub.sample();
      end
      else if (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO) begin
         cg_tlink_rsp_mmio.sample();
      end
      else begin
         cg_tlink_rsp_noncoh.sample();
      end
   endfunction : sample_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      int value;

      if (`et_value_plusargs("data_cover_on=%d", value) == 0) begin
         m_cover_data = 1'b0;
      end
      else begin
         m_cover_data = (value == 0) ? 1'b0 : 1'b1;
      end
      if (TLINK_COV_TYPE == EVL_COV_TLINK_COH_CORE) begin
         cg_tlink_req_core = new();
         cg_tlink_rsp_core = new();
      end
      else if (TLINK_COV_TYPE == EVL_COV_TLINK_MEM_HUB) begin
         cg_tlink_req_hub = new();
         cg_tlink_rsp_hub = new();
      end
      else if (TLINK_COV_TYPE == EVL_COV_TLINK_MMIO) begin
         cg_tlink_req_mmio = new();
         cg_tlink_rsp_mmio = new();
      end
      else begin
         cg_tlink_req_noncoh = new();
         cg_tlink_rsp_noncoh = new();
      end
   endfunction

endclass : evl_tlink_cov
