`define UCBLOCK_HIER shire_channel_wrap_inst.shire_channel_gen.channel_inst.uc_blocks

bit [31:0] req_buf_wait_ack;
bit [31:0] req_buf_wait_from_sys;

bit [31:0] req_buf_wait_ack_st;
bit [31:0] req_buf_wait_from_sys_st;

bit [31:0] req_buf_wait_ack_ld;
bit [31:0] req_buf_wait_from_sys_ld;

bit [31:0] req_buf_wait_ack_atomic;
bit [31:0] req_buf_wait_from_sys_atomic;

bit [31:0] req_buf_wait_ack_msg;
bit [31:0] req_buf_wait_from_sys_msg;

bit [31:0] req_buf_wait_ack_bc;
bit [31:0] req_buf_wait_from_sys_bc;

uc_reqq_entry_t req_buf_entry;

ucblock_cvg ucblock_cov = new();
always @(posedge shire_clock) begin
   if (reset_cold === 1'b0) begin
      for (int ii = 0; ii < 32; ii++) begin
	 req_buf_wait_ack[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack;
	 req_buf_wait_from_sys[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_from_sys;
	 req_buf_entry = `UCBLOCK_HIER.uncacheable_noc.uc_reqq.rf[ii];

	 req_buf_wait_ack_st[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (evl_etlink_req_opcode_t'(req_buf_entry.opcode) == evl_etlink_req_opcode_t'(ET_LINK_REQ_Write));
	 req_buf_wait_from_sys_st[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Write);
	 
	 req_buf_wait_ack_ld[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Read);
	 req_buf_wait_from_sys_ld[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Read);

	 req_buf_wait_ack_atomic[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Atomic);
	 req_buf_wait_from_sys_atomic[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Atomic);
	 
	 req_buf_wait_ack_msg[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_MsgSendData);
	 req_buf_wait_from_sys_msg[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_MsgSendData);
	 
	 req_buf_wait_ack_bc[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Write) & req_buf_entry.bcast;
	 req_buf_wait_from_sys_bc[ii] = `UCBLOCK_HIER.uncacheable_noc.req_buffer_valid[ii].wait_ack & (req_buf_entry.opcode == ET_LINK_REQ_Write) & req_buf_entry.bcast;
      end
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_ack);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_from_sys);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_ack_st);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_from_sys_st);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_ack_ld);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_from_sys_ld);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_ack_atomic);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_from_sys_atomic);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_ack_msg);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_from_sys_msg);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_ack_bc);
      ucblock_cov.ucblock_req_buffer_valid_cg.sample(req_buf_wait_from_sys_bc);


      ucblock_cov.ucblock_rsp_valid_cg.sample(`UCBLOCK_HIER.neigh_uc_min_rsp_valid);
      ucblock_cov.ucblock_etlink_port_id_cg.sample(`UCBLOCK_HIER.neigh_uc_req_info.port_id, 
						   `UCBLOCK_HIER.neigh_uc_req_valid,
						   `UCBLOCK_HIER.neigh_uc_req_ready);
      ucblock_cov.ucblock_all_requests_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid, `UCBLOCK_HIER.neigh_uc_req_ready,
						 `UCBLOCK_HIER.neigh_uc_min_rsp_ready, |`UCBLOCK_HIER.neigh_uc_min_rsp_valid,
						 
						 `UCBLOCK_HIER.to_sys_AWVALID, `UCBLOCK_HIER.to_sys_AWREADY,
						 `UCBLOCK_HIER.to_sys_WVALID, `UCBLOCK_HIER.to_sys_WREADY,
						 `UCBLOCK_HIER.to_sys_BVALID, `UCBLOCK_HIER.to_sys_BREADY,
						 `UCBLOCK_HIER.to_sys_ARVALID, `UCBLOCK_HIER.to_sys_ARREADY,
						 `UCBLOCK_HIER.to_sys_RVALID, `UCBLOCK_HIER.to_sys_RREADY,
						 
						 `UCBLOCK_HIER.to_l3_AWVALID, `UCBLOCK_HIER.to_l3_AWREADY,
						 `UCBLOCK_HIER.to_l3_WVALID, `UCBLOCK_HIER.to_l3_WREADY,
						 `UCBLOCK_HIER.to_l3_BVALID, `UCBLOCK_HIER.to_l3_BREADY,
						 `UCBLOCK_HIER.to_l3_ARVALID, `UCBLOCK_HIER.to_l3_ARREADY,
						 `UCBLOCK_HIER.to_l3_RVALID, `UCBLOCK_HIER.to_l3_RREADY,
						 
						 `UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY,
						 `UCBLOCK_HIER.sys_mesh_slave_axi_WVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_WREADY,
						 `UCBLOCK_HIER.sys_mesh_slave_axi_BVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_BREADY,
						 `UCBLOCK_HIER.sys_mesh_slave_axi_ARVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_ARREADY,
						 `UCBLOCK_HIER.sys_mesh_slave_axi_RVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_RREADY,
						 
						 `UCBLOCK_HIER.sbm_enable_write, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_AWREADY,
						 `UCBLOCK_HIER.sbm_enable_write, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_WREADY,
						 `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_BVALID, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_BREADY,
						 `UCBLOCK_HIER.sbm_enable_read, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_ARREADY,
						 `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_RVALID, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_RREADY);

      for (int ii = 0; ii < 4; ii++) begin
	 ucblock_cov.ucblock_flb_req_register_cg.sample(`UCBLOCK_HIER.flb_neigh_l2_req_valid[ii], `UCBLOCK_HIER.flb_neigh_l2_req_data[ii]);
      end
      ucblock_cov.ucblock_fcc_regs_cg.sample(`UCBLOCK_HIER.esr_enables.fcc);
      ucblock_cov.ucblock_opcodes_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid,
					    `UCBLOCK_HIER.neigh_uc_req_ready,
					    `UCBLOCK_HIER.neigh_uc_req_info.req_info.opcode,
					     `UCBLOCK_HIER.neigh_uc_req_info.port_id);
      ucblock_cov.ucblock_fifo_full_cg.sample(`UCBLOCK_HIER.uncacheable_noc.read_req_index_fifo.full, 
					      `UCBLOCK_HIER.uncacheable_noc.read_req_index_fifo.push,
					      `UCBLOCK_HIER.uncacheable_noc.read_req_index_fifo.pop);
      ucblock_cov.ucblock_fifo_full_cg.sample(`UCBLOCK_HIER.uncacheable_noc.write_req_index_fifo.full, 
					      `UCBLOCK_HIER.uncacheable_noc.write_req_index_fifo.push,
					      `UCBLOCK_HIER.uncacheable_noc.write_req_index_fifo.pop);
      ucblock_cov.ucblock_fifo_full_cg.sample(`UCBLOCK_HIER.uncacheable_noc.decode_rsp_index_fifo.full, 
					      `UCBLOCK_HIER.uncacheable_noc.decode_rsp_index_fifo.push,
					      `UCBLOCK_HIER.uncacheable_noc.decode_rsp_index_fifo.pop);
      ucblock_cov.ucblock_fifo_full_cg.sample(`UCBLOCK_HIER.uncacheable_noc.axi_rsp_handlers.rsp_fifo.full, 
					      `UCBLOCK_HIER.uncacheable_noc.axi_rsp_handlers.rsp_fifo.push,
					      `UCBLOCK_HIER.uncacheable_noc.axi_rsp_handlers.rsp_fifo.pop);
      
      ucblock_cov.ucblock_message_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid, 
					    `UCBLOCK_HIER.neigh_uc_req_ready, 
					    `UCBLOCK_HIER.neigh_uc_req_info.req_info.opcode,
					    `UCBLOCK_HIER.neigh_uc_req_info.req_info.address);
      ucblock_cov.ucblock_atomic_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid, 
					   `UCBLOCK_HIER.neigh_uc_req_ready, 
					   `UCBLOCK_HIER.neigh_uc_req_info.req_info.opcode,
					   `UCBLOCK_HIER.neigh_uc_req_info.req_info.subopcode,
					   `UCBLOCK_HIER.neigh_uc_req_info.req_info.address);
      ucblock_cov.ucblock_broadcast_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid,
					      `UCBLOCK_HIER.neigh_uc_req_ready,
					      `UCBLOCK_HIER.neigh_uc_req_info.req_info.opcode,
					      `UCBLOCK_HIER.neigh_uc_req_info.req_info.address,
					      `UCBLOCK_HIER.neigh_uc_req_info.req_info.data[39:0]);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid, `UCBLOCK_HIER.neigh_uc_req_ready);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(|`UCBLOCK_HIER.neigh_uc_min_rsp_valid, `UCBLOCK_HIER.neigh_uc_min_rsp_ready);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_sys_AWVALID, `UCBLOCK_HIER.to_sys_AWREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_sys_WVALID, `UCBLOCK_HIER.to_sys_WREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_sys_BVALID, `UCBLOCK_HIER.to_sys_BREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_sys_ARVALID, `UCBLOCK_HIER.to_sys_ARREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_sys_RVALID, `UCBLOCK_HIER.to_sys_RREADY);
      
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_l3_AWVALID, `UCBLOCK_HIER.to_l3_AWREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_l3_WVALID, `UCBLOCK_HIER.to_l3_WREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_l3_BVALID, `UCBLOCK_HIER.to_l3_BREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_l3_ARVALID, `UCBLOCK_HIER.to_l3_ARREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.to_l3_RVALID, `UCBLOCK_HIER.to_l3_RREADY);
      
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_WVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_WREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_BVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_BREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_ARVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_ARREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_RVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_RREADY);
      
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sbm_enable_write, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_AWREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sbm_enable_write, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_WREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sbm_sys_mesh_slave_axi_BVALID, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_BREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sbm_enable_read, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_ARREADY);
      ucblock_cov.ucblock_valid_with_no_ready_cg.sample(`UCBLOCK_HIER.sbm_sys_mesh_slave_axi_RVALID, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_RREADY);
      ucblock_cov.ucblock_req_size_cg.sample(`UCBLOCK_HIER.neigh_uc_req_valid,
					     `UCBLOCK_HIER.neigh_uc_req_ready,
					     `UCBLOCK_HIER.neigh_uc_req_info.req_info.size);

      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.to_sys_AWVALID, `UCBLOCK_HIER.to_sys_AWREADY, `UCBLOCK_HIER.to_sys_AW.ID);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.to_sys_ARVALID, `UCBLOCK_HIER.to_sys_ARREADY, `UCBLOCK_HIER.to_sys_AR.ID);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.to_l3_AWVALID, `UCBLOCK_HIER.to_l3_AWREADY, `UCBLOCK_HIER.to_l3_AW.ID);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.to_l3_ARVALID, `UCBLOCK_HIER.to_l3_ARREADY, `UCBLOCK_HIER.to_sys_AR.ID);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY, `UCBLOCK_HIER.sys_mesh_slave_axi_AW.ID);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_ARVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_ARREADY, `UCBLOCK_HIER.sys_mesh_slave_axi_AR.ID);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.sbm_enable_write, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_AWREADY, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_AW.ID[8:0]);
      ucblock_cov.ucblock_axi_id_cg.sample(`UCBLOCK_HIER.sbm_enable_read, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_ARREADY, `UCBLOCK_HIER.sbm_sys_mesh_slave_axi_AW.ID[8:0]);
      // Cover that when vc0 (or vc1) cc reaches the max vc1 (or vc0) can still accept requests
      ucblock_cov.ucblock_axi_vc_credits_cg.sample(aw_credit_in[0], `UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY, `UCBLOCK_HIER.sys_mesh_slave_axi_AWvcvalid[1]);
      ucblock_cov.ucblock_axi_vc_credits_cg.sample(aw_credit_in[1], `UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY, `UCBLOCK_HIER.sys_mesh_slave_axi_AWvcvalid[0]);
      // Cover that there is a read (or write) to UC & write (or read) to SBM in the same cycle on AXI
      ucblock_cov.ucblock_sysmesh_read_write_cg.sample(`UCBLOCK_HIER.sys_mesh_slave_axi_AWVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_AWREADY, `UCBLOCK_HIER.sys_mesh_slave_axi_ARVALID, `UCBLOCK_HIER.sys_mesh_slave_axi_ARREADY, `UCBLOCK_HIER.sys_mesh_slave_axi_AWvcvalid[1:0]);
   end // if (reset_cold === 1'b0)
end // always @ (posedge shire_clock)

//top.soc_top.SHIRE[0].shire_top.shire_top.gen_neigh[0].neigh.neigh.gen_minions[0].min_inst.minion.core.intpipe.csr_file.int_fcc[3:0] top.soc_top.SHIRE[0].shire_top.shire_top.gen_neigh[0].neigh.neigh.gen_minions[0].min_inst.minion.core.intpipe.csr_file.reg_fcc_dec[3:0]
      
   
