wire req_is_read;
wire req_is_write;
wire req_is_write_around;
wire req_is_coop_read;
wire req_is_message;
wire req_is_atomic;
wire req_is_flush;
wire req_is_flush_to_mem;
wire req_is_evict;
wire req_is_evict_to_mem;
wire req_is_lock;
wire req_is_unlock;
wire req_is_scp_fill;
wire req_is_prefetch;
wire req_is_cacheop;
wire req_is_legal;
wire req_is_wdata;

int start_addr_bit;
int end_addr_bit;

assign req_is_read         = (req_info.opcode === ET_LINK_REQ_Read);
assign req_is_write        = (req_info.opcode === ET_LINK_REQ_Write);
assign req_is_write_around = (req_info.opcode === ET_LINK_REQ_WriteAround);
assign req_is_coop_read    = (req_info.opcode === ET_LINK_REQ_ReadCoop);
assign req_is_message      = (req_info.opcode === ET_LINK_REQ_MsgSendData);
assign req_is_atomic       = (req_info.opcode === ET_LINK_REQ_Atomic);
assign req_is_flush        = (req_info.opcode === ET_LINK_REQ_Flush);
assign req_is_flush_to_mem = (req_info.opcode === ET_LINK_REQ_FlushToMem);
assign req_is_evict        = (req_info.opcode === ET_LINK_REQ_Evict);
assign req_is_evict_to_mem = (req_info.opcode === ET_LINK_REQ_EvictToMem);
assign req_is_lock         = (req_info.opcode === ET_LINK_REQ_Lock);
assign req_is_unlock       = (req_info.opcode === ET_LINK_REQ_Unlock);
assign req_is_scp_fill     = (req_info.opcode === ET_LINK_REQ_ScpFill);
assign req_is_prefetch     = (req_info.opcode === ET_LINK_REQ_Prefetch);

assign req_is_cacheop      = req_is_flush | req_is_flush_to_mem | req_is_evict | req_is_evict_to_mem | req_is_lock | req_is_unlock | req_is_scp_fill | req_is_prefetch;
assign req_is_legal        = req_is_cacheop | req_is_atomic | req_is_message | req_is_coop_read | req_is_write_around | req_is_write | req_is_read;
assign req_is_wdata        = (req_is_cacheop & ~req_is_lock) |
                             req_is_atomic | req_is_message | req_is_coop_read | req_is_write_around | req_is_write;

assign start_addr_bit = (8 * req_info.address[5:0]);

assign end_addr_bit   = (start_addr_bit + 2**(req_info.size+3));




//`ASSERT_ALWAYS_MSG(req_info_stable, m_clock, reset,
//   ((($bits(req_info.data) <= 256) && (req_valid === 1) && (req_ready === 0)) |=> ($stable(req_info) || (req_valid === 0))), "Request info was not stable until ready was asserted");


//`ASSERT_ALWAYS_MSG(rsp_info_stable, m_clock, reset,
//   ((($bits(req_info.data) <= 256) && (rsp_valid === 1) && (rsp_ready === 0)) |=> ($stable(rsp_info) || (rsp_valid === 0))), "Response info was not stable until ready was asserted");


`ASSERT_ALWAYS_MSG(data_x_8b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b000) && ($size(req_info.data) >= 8) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> (!($isunknown(req_info.data[start_addr_bit +: 8]))  || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 8 bits");

`ASSERT_ALWAYS_MSG(data_x_16b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b001) && ($size(req_info.data) >= 16) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> (!($isunknown(req_info.data[start_addr_bit +: 16]))  || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 16 bits");

if ($size(req_info.data) >= 32) begin
`ASSERT_ALWAYS_MSG(data_x_32b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b010) && ($size(req_info.data) >= 32) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> (!($isunknown(req_info.data[start_addr_bit +: 32]))  || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 32 bits");
end

if ($size(req_info.data) >= 64) begin
`ASSERT_ALWAYS_MSG(data_x_64b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b011) && ($size(req_info.data) >= 64) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> (!($isunknown(req_info.data[start_addr_bit +: 64]))  || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 64 bits");
end

if ($size(req_info.data) >= 128) begin
`ASSERT_ALWAYS_MSG(data_x_128b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b100) && ($size(req_info.data) >= 128) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> (!($isunknown(req_info.data[start_addr_bit +: 128]))  || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 128 bits");
end

if ($size(req_info.data) >= 256) begin
`ASSERT_ALWAYS_MSG(data_x_256b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b101) && ($size(req_info.data) >= 256) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> (!($isunknown(req_info.data[start_addr_bit +: 256]))  || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 256 bits");
end

if ($size(req_info.data) >= 512) begin
`ASSERT_ALWAYS_MSG(data_x_512b, m_clock, reset,
   ((m_req_select_mon != 0) && (req_info.size === 3'b110) && ($size(req_info.data) == 512) && ((req_info.opcode === 5'b00000) || (req_info.opcode === 5'b00010)) |-> ( !($isunknown(req_info.data[511:0])) || !($stable(req_valid)) )),
   "Valid DATA contain X values for write or write-around operation and size of 512 bits");
end

//
// Legal request opcodes
//
`ASSERT_ALWAYS_MSG(illegal_req_opcode, m_clock, reset,
   ((m_req_select_mon != 0) |-> (req_is_legal === 1'b1)),
   "Illegal request opcode");


//
// Requests that should use wdata = 0
// - Read and Cacheop Lock
//
`ASSERT_ALWAYS_MSG(wdata_0_req, m_clock, reset,
   ((m_req_select_mon != 0) && (req_is_legal === 1'b1) && (req_is_wdata === 1'b0) |-> (req_info.wdata === 0)),
   "wdata should be 0 for this request opcode");


//
// Requests that should use wdata = 1
// - all except Read and Cacheop Lock
//
`ASSERT_ALWAYS_MSG(wdata_1_req, m_clock, reset,
   ((m_req_select_mon != 0) && (req_is_legal === 1'b1) && (req_is_wdata === 1'b1) |-> (req_info.wdata === 1)),
   "wdata should be 1 for this request opcode");


//
// Responses that should use wdata = 0
//
`ASSERT_ALWAYS_MSG(wdata_0_rsp, m_clock, reset,
   (((rsp_valid === 1'b1) && (rsp_ready === 1'b1)) && (rsp_info.opcode === 2'b00 || rsp_info.opcode === 2'b11) |-> (rsp_info.wdata === 0)),
   "wdata should be 0 for this response opcode");


//
// Responses that should use wdata = 1
//
`ASSERT_ALWAYS_MSG(wdata_1_rsp, m_clock, reset,
   (((rsp_valid === 1'b1) && (rsp_ready === 1'b1)) && (rsp_info.opcode === 2'b01 || rsp_info.opcode === 2'b10) |-> (rsp_info.wdata === 1)),
   "wdata should be 1 for this response opcode");


//
// Legal values of Read and Write subopcode
//
`ASSERT_ALWAYS_MSG(subopcode_req_read_write, m_clock, reset,
   ((m_req_select_mon != 0) && (req_is_read === 1'b1 || req_is_write === 1'b1) |-> (req_info.subopcode < 2)),
   "For this opcode, subopcode must be in range [0..1]");


//
// Legal values of WriteAround subopcode
//
`ASSERT_ALWAYS_MSG(subopcode_write_around, m_clock, reset,
   ((m_req_select_mon != 0) && (req_is_write_around === 1'b1) |-> (req_info.subopcode < 3)),
   "For this opcode, subopcode must be in range [0..2]");


//
// Requests that can't have a subopcode
//  - all but Read/Write/writeAround/Atomics
//
`ASSERT_ALWAYS_MSG(subopcode_req, m_clock, reset,
   ((m_req_select_mon != 0) && (req_is_read === 1'b0) && (req_is_write === 1'b0) && (req_is_write_around === 1'b0) && (req_is_atomic === 1'b0) |-> (req_info.subopcode === 0)),
   "For this opcode, subopcode must be 0");


`ASSERT_ALWAYS_MSG(r_w_add_aligned_16b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_info.size === 3'b001) && ((req_is_read === 1'b1) || (req_is_write === 1'b1) || (req_is_write_around === 1'b1) || (req_is_coop_read === 1'b1) || (req_is_atomic === 1'b1))) |-> (req_info.address[0] === 1'b0)),
   "Address is not size-aligned");


`ASSERT_ALWAYS_MSG(r_w_add_aligned_32b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_info.size === 3'b010) && ((req_is_read === 1'b1) || (req_is_write === 1'b1) || (req_is_write_around === 1'b1) || (req_is_coop_read === 1'b1) || (req_is_atomic === 1'b1))) |-> (req_info.address[1:0] === 2'b00)),
   "Address is not size-aligned");


`ASSERT_ALWAYS_MSG(r_w_add_aligned_64b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_info.size === 3'b011) && ((req_is_read === 1'b1) || (req_is_write === 1'b1) || (req_is_write_around === 1'b1) || (req_is_coop_read === 1'b1) || (req_is_atomic === 1'b1))) |-> (req_info.address[2:0] === 3'b000)),
   "Address is not size-aligned");


`ASSERT_ALWAYS_MSG(r_w_add_aligned_128b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_info.size === 3'b100) && ((req_is_read === 1'b1) || (req_is_write === 1'b1) || (req_is_write_around === 1'b1) || (req_is_coop_read === 1'b1) || (req_is_atomic === 1'b1))) |-> (req_info.address[3:0] === 4'b0000)),
   "Address is not size-aligned");


`ASSERT_ALWAYS_MSG(r_w_add_aligned_256b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_info.size === 3'b101) && ((req_is_read === 1'b1) || (req_is_write === 1'b1) || (req_is_write_around === 1'b1) || (req_is_coop_read === 1'b1) || (req_is_atomic === 1'b1))) |-> (req_info.address[4:0] === 5'b00000)),
   "Address is not size-aligned");


`ASSERT_ALWAYS_MSG(r_w_add_aligned_512b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_info.size === 3'b110) && ((req_is_read === 1'b1) || (req_is_write === 1'b1) || (req_is_write_around === 1'b1) || (req_is_coop_read === 1'b1) || (req_is_atomic === 1'b1))) |-> (req_info.address[5:0] === 6'b000000)),
   "Address is not size-aligned");


`ASSERT_ALWAYS_MSG(cacheop_aligned_512b, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_cacheop === 1'b1)) |-> (req_info.address[5:0] === 6'b000000)),
   "Cacheop address is not line-aligned");


`ASSERT_ALWAYS_MSG(readcoop_aligned, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_coop_read === 1'b1)) |-> (req_info.qwen === 4'b0001)),
   "Field qwen is not 0b0001 for REQ_ReadCoop");


`ASSERT_ALWAYS_MSG(atomic_size, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_atomic === 1'b1)) |-> (((req_info.size[2:1] === 2'b01) && (req_info.qwen === 4'b0001)) || ((req_info.size[2:0] === 3'b101) && (req_info.qwen == 4'b0011)) || ((req_info.size[2:0] === 3'b100) && (req_info.qwen == 4'b0001)))),
   "Size is not valid for REQ_Atomic");


`ASSERT_ALWAYS_MSG(flush_evict, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_flush === 1'b1 || req_is_evict === 1'b1)) |-> ((req_info.data[4:3] < 3) && (req_info.data[6:5] > 0) && (req_info.size === 3'b000) && (req_info.qwen === 4'b0001))),
   "Size or qwen values are not valid for REQ_Flush or REQ_Evict");


`ASSERT_ALWAYS_MSG(flush_evict_to_mem, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_flush_to_mem === 1'b1 || req_is_evict_to_mem === 1'b1)) |-> ((req_info.size === 3'b110) && (req_info.qwen === 4'b1111))),
   "Size or qwen values are not valid for REQ_Flush_to_mem or REQ_Evict_to_mem");


`ASSERT_ALWAYS_MSG(unlock, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_unlock === 1'b1)) |-> ((req_info.size === 3'b000) && (req_info.qwen === 4'b0001))),
   "Size or qwen values are not valid for REQ_Unlock");


`ASSERT_ALWAYS_MSG(scp_fill, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_scp_fill === 1'b1)) |-> ((req_info.data[5:0] === 0) && (req_info.size === 3'b011) && (req_info.qwen === 4'b0001))),
   "Size or qwen values are not valid for REQ_ScpFill");


`ASSERT_ALWAYS_MSG(prefetch, m_clock, reset,
   (((m_req_select_mon != 0) && (req_is_prefetch === 1'b1)) |-> ((req_info.data[6:5] > 0) && (req_info.size === 3'b000) && (req_info.qwen === 4'b0001))),
   "Size or qwen values are not valid for REQ_Prefetch");
