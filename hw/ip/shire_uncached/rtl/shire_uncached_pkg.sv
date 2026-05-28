// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

package shire_uncached_pkg;
  import axi_pkg::*;
  import etlink_pkg::*;
  import shirecache_pkg::*;

  /* verilator lint_off UNUSEDPARAM */  // Uncached constants are consumed incrementally by wrappers/cosims.

  localparam int unsigned NumNeigh        = shirecache_pkg::NumNeigh;
  localparam int unsigned MinPerNeigh     = 8;
  localparam int unsigned RboxPerShire    = shirecache_pkg::RboxPerShire;
  localparam int unsigned NumPorts        = NumNeigh + RboxPerShire;
  localparam int unsigned NumShireIdsBits = $clog2(shirecache_pkg::NumShireIds);

  localparam int unsigned BarrierRegisters      = 32;
  localparam int unsigned BarrierValueWidth     = 8;
  localparam int unsigned FlbElemsPerNeigh      = 4;
  localparam int unsigned CsrFlBarrierSize      = 13;
  localparam int unsigned CsrFlBarrierNumLsb    = 0;
  localparam int unsigned CsrFlBarrierNumWidth  = 5;
  localparam int unsigned CsrFlBarrierLimitLsb  = 5;
  localparam int unsigned CsrFlBarrierLimitWidth = BarrierValueWidth;

  localparam int unsigned UcFccRegisters  = 4;
  localparam int unsigned UcFccTargetSize = $clog2(UcFccRegisters);

  localparam int unsigned UcReqqSize             = MinPerNeigh * NumNeigh;
  localparam int unsigned UcReqBaseDataSize      = 256;
  localparam int unsigned UcReqAtmExtraDataSize  = 26;
  localparam int unsigned UcReqDataSize          = UcReqBaseDataSize + UcReqAtmExtraDataSize;
  localparam int unsigned UcNumCredits           = 8;
  localparam int unsigned UcRspArbClients        = 2;
  localparam int unsigned UcAlignBitsStart       = 0;
  localparam int unsigned UcAlignBitsSize        = 6;
  localparam int unsigned UcAlignBitsEnd         = UcAlignBitsStart + UcAlignBitsSize - 1;

  localparam int unsigned UcReqAtmDataSubopLsb    = 256;
  localparam int unsigned UcReqAtmDataShireIdLsb  = 269;
  localparam int unsigned UcReqAtmDataAlgnBitsLsb = 263;
  localparam int unsigned UcReqAtmDataReqqIdxLsb  = 277;
  localparam int unsigned UcAtomicAdRspLsbStart   = 6;
  localparam int unsigned UcAtomicAdRspLsbSize    = 14;
  localparam logic [UcAtomicAdRspLsbSize-1:0] UcAtomicAdRsp = 14'h3800;

  localparam int unsigned EsrBroadcastDataSize = 64;
  localparam logic [etlink_pkg::PaSize-1:0] BroadcastAddrEsr = 40'h013f_f5ff_f0;
  localparam logic [etlink_pkg::PaSize-1:0] BroadcastAddrUm  = 40'h013f_f5ff_f8;
  localparam logic [etlink_pkg::PaSize-1:0] BroadcastAddrSm  = 40'h017f_f5ff_f8;
  localparam logic [etlink_pkg::PaSize-1:0] BroadcastAddrDm  = 40'h01bf_f5ff_f8;
  localparam logic [etlink_pkg::PaSize-1:0] BroadcastAddrMm  = 40'h01ff_f5ff_f8;

  localparam int unsigned BcastMaskLo      = 0;
  localparam int unsigned BcastMaskHi      = 39;
  localparam int unsigned BcastMaskWidth   = BcastMaskHi - BcastMaskLo + 1;
  localparam int unsigned BcastMaskStoreLo = BcastMaskLo + EsrBroadcastDataSize;
  localparam int unsigned BcastSendsSize   = 6;
  localparam int unsigned BcastAxiIdStart  = 64;
  localparam int unsigned NumAvailableMinShires = 32;
  localparam int unsigned MemshireEsrStartId    = 232;

  localparam int unsigned AdScpRegionStart = 31;
  localparam int unsigned AdScpRegionSize  = etlink_pkg::PaSize - AdScpRegionStart;
  localparam logic [AdScpRegionSize-1:0] AdScpRegion = {{AdScpRegionSize-1{1'b0}}, 1'b1};

  localparam int unsigned AdEsrRegionStart = 32;
  localparam int unsigned AdEsrRegionSize  = etlink_pkg::PaSize - AdEsrRegionStart;
  localparam logic [AdEsrRegionSize-1:0] AdEsrRegion = {{AdEsrRegionSize-1{1'b0}}, 1'b1};
  localparam int unsigned AdEsrPortMsgStart       = 0;
  localparam int unsigned AdEsrPortMsgSize        = 12;
  localparam logic [AdEsrPortMsgSize-1:0] AdEsrPortMsgCommon      = 12'h800;
  localparam logic [AdEsrPortMsgSize-1:0] AdEsrPortMsgReduceReady = 12'he00;
  localparam logic [AdEsrPortMsgSize-1:0] AdEsrPortMsgReduceData  = 12'he20;
  localparam int unsigned AdEsrMessageIdStart     = 3;
  localparam int unsigned AdEsrMessageIdSize      = 8;
  localparam int unsigned AdEsrPortIdStart        = AdEsrMessageIdStart + 3;
  localparam int unsigned AdEsrPortIdSize         = 2;
  localparam int unsigned AdEsrThIdStart          = AdEsrPortMsgStart + AdEsrPortMsgSize;
  localparam int unsigned AdEsrThIdSize           = 1;
  localparam int unsigned AdEsrNeighMinIdStart    = AdEsrThIdStart + AdEsrThIdSize;
  localparam int unsigned AdEsrNeighMinIdSize     = 3;
  localparam int unsigned AdEsrNeighHartIdStart   = AdEsrThIdStart;
  localparam int unsigned AdEsrNeighHartIdSize    = AdEsrNeighMinIdSize + AdEsrThIdSize;
  localparam int unsigned AdEsrNeighIdStart       = AdEsrNeighMinIdStart + AdEsrNeighMinIdSize;
  localparam int unsigned AdEsrNeighIdSize        = 4;
  localparam int unsigned AdEsrAgentTypeStart     = 20;
  localparam int unsigned AdEsrAgentTypeSize      = 2;
  localparam logic [AdEsrAgentTypeSize-1:0] AdEsrAgentTypeMinion = 2'd0;
  localparam logic [AdEsrAgentTypeSize-1:0] AdEsrAgentTypeShire  = 2'd3;
  localparam int unsigned AdEsrShireIdStart       = AdEsrAgentTypeStart + AdEsrAgentTypeSize;
  localparam int unsigned AdEsrShireIdSize        = 8;
  localparam int unsigned AdEsrProtBitsStart      = AdEsrShireIdStart + AdEsrShireIdSize;
  localparam int unsigned AdEsrProtBitsSize       = 2;
  localparam logic [AdEsrProtBitsSize-1:0] AdEsrProtBitsMsg   = 2'd2;
  localparam logic [AdEsrProtBitsSize-1:0] AdEsrProtBitsUmode = 2'd0;

  localparam int unsigned AdDdrRegionStart = 39;
  localparam int unsigned AdDdrRegionSize  = etlink_pkg::PaSize - AdDdrRegionStart;
  localparam logic [AdDdrRegionSize-1:0] AdDdrRegion = {{AdDdrRegionSize-1{1'b0}}, 1'b1};

  localparam logic [NumShireIdsBits-1:0] LocalShireId = {NumShireIdsBits{1'b1}};

  localparam logic [axi_pkg::AxQosSize-1:0] EtAxiQosMessage = 4'b0010;
  localparam logic [axi_pkg::AxQosSize-1:0] EtAxiQosEsr     = 4'b0011;

  typedef logic [BarrierRegisters-1:0][BarrierValueWidth-1:0] uc_esr_barrier_values_t;
  typedef logic [UcFccRegisters-1:0] uc_esr_fcc_enable_sigs_t;
  typedef logic [BarrierRegisters-1:0] uc_esr_flb_enable_sigs_t;

  typedef struct packed {
    uc_esr_flb_enable_sigs_t barriers;
    uc_esr_fcc_enable_sigs_t fcc;
  } uc_esr_enable_sigs_t;

  typedef struct packed {
    uc_esr_barrier_values_t barriers;
  } uc_esr_values_t;

  typedef struct packed {
    logic                            resp;
    logic [etlink_pkg::IdSize-1:0]   id;
    logic [UcReqDataSize-1:0]        data;
    etlink_pkg::size_e               size;
    etlink_pkg::req_opcode_e         opcode;
    logic [etlink_pkg::PaSize-1:0]   address;
    logic [etlink_pkg::SourceSize-1:0] source;
    logic [shirecache_pkg::PortIdSize-1:0] port_id;
    logic [shirecache_pkg::DvTransIdSize-1:0] trans_id;
    logic                            use_l3;
    logic                            bcast;
  } uc_reqq_entry_t;

  typedef struct packed {
    logic wait_ack;
    logic wait_from_sys;
  } uc_reqq_valid_t;

  typedef struct packed {
    logic [axi_pkg::ScMasterDataSize-1:0] data;
    logic [etlink_pkg::PaSize-1:0]        address;
    etlink_pkg::size_e                    size;
    etlink_pkg::req_opcode_e              opcode;
    logic [axi_pkg::ScMasterIdSize-1:0]   axi_id;
    logic                                 l3_en;
  } uc_noc_master_req_t;

  typedef struct packed {
    logic [etlink_pkg::PaSize-1:0]        address;
    etlink_pkg::size_e                    size;
    etlink_pkg::req_opcode_e              opcode;
    logic [axi_pkg::ScMasterIdSize-1:0]   axi_id;
    logic                                 l3_en;
  } uc_noc_master_req_nodata_t;

  typedef struct packed {
    logic [shirecache_pkg::PortIdSize-1:0]  port_id;
    logic [etlink_pkg::SourceSize-1:0]      source;
    logic [EsrBroadcastDataSize-1:0]        data;
  } uc_esr_broadcast_t;

  typedef struct packed {
    logic [etlink_pkg::IdSize-1:0]          id;
    etlink_pkg::size_e                      size;
    logic [etlink_pkg::SourceSize-1:0]      dest;
    etlink_pkg::rsp_opcode_e                opcode;
    logic [shirecache_pkg::PortIdSize-1:0]  port_id;
    logic [shirecache_pkg::DvTransIdSize-1:0] trans_id;
  } uc_decode_rsp_t;

  function automatic logic uc_is_broadcast(input logic [etlink_pkg::PaSize-1:0] addr);
    return (addr == BroadcastAddrUm) || (addr == BroadcastAddrSm) ||
           (addr == BroadcastAddrDm) || (addr == BroadcastAddrMm);
  endfunction

  function automatic logic uc_is_broadcast_esr(input logic [etlink_pkg::PaSize-1:0] addr);
    return addr == BroadcastAddrEsr;
  endfunction

  function automatic logic uc_is_atomic(input etlink_pkg::req_opcode_e opcode);
    return opcode == etlink_pkg::ReqAtomic;
  endfunction

  function automatic logic uc_is_msg(input etlink_pkg::req_opcode_e opcode);
    return opcode == etlink_pkg::ReqMsgSendData;
  endfunction

  function automatic logic uc_is_scp(input logic [etlink_pkg::PaSize-1:0] addr);
    logic unused_addr;
    unused_addr = ^addr[AdScpRegionStart-1:0];
    return (addr[etlink_pkg::PaSize-1:AdScpRegionStart] == AdScpRegion) ||
           (unused_addr && 1'b0);
  endfunction

  function automatic logic [etlink_pkg::QwenSize-1:0] uc_qwen(
    input etlink_pkg::size_e size,
    input logic [UcAlignBitsSize-1:0] addr_lsb
  );
    logic [etlink_pkg::QwenSize-1:0] qwen;
    logic [1:0] offset;
    logic unused_addr_lsb;
    unused_addr_lsb = ^addr_lsb[3:0];
    unique case (size)
      etlink_pkg::SizeByte,
      etlink_pkg::SizeHWord,
      etlink_pkg::SizeWord,
      etlink_pkg::SizeDWord,
      etlink_pkg::SizeQWord: begin
        offset = addr_lsb[5:4];
        qwen = 4'b0001 << offset;
      end
      etlink_pkg::SizeHLine: begin
        offset = addr_lsb[5:4];
        if (offset > 2) offset = 2;
        qwen = 4'b0011 << offset;
      end
      default: qwen = 4'b1111;
    endcase
    return qwen ^ {etlink_pkg::QwenSize{unused_addr_lsb && 1'b0}};
  endfunction

  function automatic logic [etlink_pkg::QwenSize-1:0] uc_qwen_unalign(
    input etlink_pkg::size_e size
  );
    unique case (size)
      etlink_pkg::SizeByte,
      etlink_pkg::SizeHWord,
      etlink_pkg::SizeWord,
      etlink_pkg::SizeDWord,
      etlink_pkg::SizeQWord: return 4'b0001;
      etlink_pkg::SizeHLine: return 4'b0011;
      default: return 4'b1111;
    endcase
  endfunction

  function automatic logic uc_slv_is_msg(input logic [etlink_pkg::PaSize-1:0] addr);
    logic hart_msg;
    logic reduce_msg;
    logic unused_addr;
    unused_addr = ^{addr[29:22], addr[19:12]};
    hart_msg = (addr[etlink_pkg::PaSize-1:AdEsrRegionStart] == AdEsrRegion) &&
               (addr[AdEsrProtBitsStart +: AdEsrProtBitsSize] == AdEsrProtBitsUmode) &&
               (addr[AdEsrAgentTypeStart +: AdEsrAgentTypeSize] == AdEsrAgentTypeMinion) &&
               (addr[AdEsrPortMsgStart +: AdEsrPortMsgSize] ==
                   (AdEsrPortMsgCommon |
                    ({{AdEsrPortMsgSize-AdEsrPortIdSize{1'b0}},
                      addr[AdEsrPortIdStart +: AdEsrPortIdSize]} << AdEsrPortIdStart)));
    reduce_msg = (addr[etlink_pkg::PaSize-1:AdEsrRegionStart] == AdEsrRegion) &&
                 (addr[AdEsrProtBitsStart +: AdEsrProtBitsSize] == AdEsrProtBitsMsg) &&
                 (addr[AdEsrAgentTypeStart +: AdEsrAgentTypeSize] == AdEsrAgentTypeMinion) &&
                 ((addr[AdEsrPortMsgStart +: AdEsrPortMsgSize] == AdEsrPortMsgReduceData) ||
                  (addr[AdEsrPortMsgStart +: AdEsrPortMsgSize] == AdEsrPortMsgReduceReady));
    return hart_msg || reduce_msg || (unused_addr && 1'b0);
  endfunction

  function automatic logic [axi_pkg::ScMasterStrbSize-1:0] uc_axi_strb(
    input etlink_pkg::req_opcode_e opcode,
    input etlink_pkg::size_e size,
    input logic [UcAlignBitsSize-1:0] addr_lsb,
    input logic [3:0] atomic_opcode
  );
    logic [axi_pkg::ScMasterStrbSize-1:0] strb;
    unique case (size)
      etlink_pkg::SizeByte:  strb = 64'h0000_0000_0000_0001;
      etlink_pkg::SizeHWord: strb = 64'h0000_0000_0000_0003;
      etlink_pkg::SizeWord:  strb = 64'h0000_0000_0000_000f;
      etlink_pkg::SizeDWord: strb = 64'h0000_0000_0000_00ff;
      etlink_pkg::SizeQWord: strb = 64'h0000_0000_0000_ffff;
      etlink_pkg::SizeHLine: strb = 64'h0000_0000_ffff_ffff;
      default:               strb = 64'hffff_ffff_ffff_ffff;
    endcase
    strb = strb << addr_lsb;
    if (uc_is_atomic(opcode)) begin
      if (atomic_opcode == 4'b1011) begin
        strb = (size == etlink_pkg::SizeWord) ? 64'h0000_0000_0000_0f0f :
                                                64'h0000_0000_0000_ffff;
      end
      return strb | (64'hf << 32);
    end
    return strb;
  endfunction

  function automatic logic [axi_pkg::ScMasterDataSize-1:0] uc_aligned_data(
    input uc_noc_master_req_t req
  );
    logic [axi_pkg::ScMasterDataSize-1:0] aligned_noatomic_data;
    logic [$clog2(axi_pkg::ScMasterDataSize)-1:0] offset;
    logic unused_req;
    offset = req.address[UcAlignBitsEnd:UcAlignBitsStart] * 8;
    aligned_noatomic_data = req.data << offset;
    unused_req = ^{req.l3_en, req.size, req.axi_id, req.address, req.data};
    return (uc_is_atomic(req.opcode) ? req.data : aligned_noatomic_data) ^
           {axi_pkg::ScMasterDataSize{unused_req && 1'b0}};
  endfunction

  function automatic logic [UcReqDataSize-1:0] uc_unaligned_data(
    input logic [etlink_pkg::DataSize-1:0] req_data,
    input logic [UcAlignBitsSize-1:0] align_bits
  );
    logic [UcReqDataSize-1:0] data;
    logic [etlink_pkg::DataSize-1:0] shifted_data;
    shifted_data = req_data >> (align_bits * 8);
    data[UcReqBaseDataSize-1:0] = shifted_data[UcReqBaseDataSize-1:0] ^
        {UcReqBaseDataSize{(^shifted_data[etlink_pkg::DataSize-1:UcReqBaseDataSize]) && 1'b0}};
    data[UcReqDataSize-1:UcReqBaseDataSize] = '0;
    return data;
  endfunction

  /* verilator lint_on UNUSEDPARAM */
endpackage : shire_uncached_pkg
