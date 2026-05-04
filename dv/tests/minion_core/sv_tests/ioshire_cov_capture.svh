ioshire_cvg ioshire_cov = new();

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_slave_axi_AWVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 0, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_slave_axi_ARVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 0, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_esr_master_axi_AWVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 1, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_esr_master_axi_ARVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 1, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_tosys_master_axi_AWVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 2, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_tosys_master_axi_ARVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 2, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_tol3_master_axi_AWVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 3, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_tol3_master_axi_ARVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 3, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_tol3b_master_axi_AWVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 4, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge ioshire_tol3b_master_axi_ARVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 4, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge spio_sp_main0_main_noc_rbm_s_AWVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 5, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge spio_sp_main0_main_noc_rbm_s_ARVALID)
	@(posedge clk__noc);
	first_edge = $time;
	@(posedge clk__noc);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    ioshire_cov.ioshire_noc_clock_freq_cg.sample(freq, 5, 1);
end

int aw_bp_clk_count = 0;
int w_bp_clk_count = 0;
int ar_bp_clk_count = 0;
int etl_rsp_bp_clk_count = 0;

always begin
	@(posedge spio.sp_etl2axi.clk_core);

	if(spio.sp_etl2axi.q2_aw_blocked === 1'b1) aw_bp_clk_count++;
	else aw_bp_clk_count = 0;
	if(aw_bp_clk_count > 30) ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	if(spio.sp_etl2axi.q2_w_blocked === 1'b1) w_bp_clk_count++;
	else w_bp_clk_count = 0;
	if(w_bp_clk_count > 30) ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	if(spio.sp_etl2axi.q2_ar_blocked === 1'b1) ar_bp_clk_count++;
	else ar_bp_clk_count = 0;
	if(ar_bp_clk_count > 30) ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	if(spio.sp_etl2axi.p2_blocked === 1'b1) etl_rsp_bp_clk_count++;
	else etl_rsp_bp_clk_count = 0;
	if(etl_rsp_bp_clk_count > 30) ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 0, 0, spio.sp_etl2axi.q0_wr_cnt, spio.sp_etl2axi.q0_rd_cnt, spio.sp_etl2axi.q0_wr_cnt_full, 0, 0, 0, 0, 0, 0);
end

int wr_stream_length = 0;
int rd_stream_length = 0;
bit[3:0] wr_stream_diff = 0;
bit[3:0] rd_stream_diff = 0;
int wr_stream_diff_num = 0;
int rd_stream_diff_num = 0;
bit[7:0] wr_req_queue[$];
bit[7:0] rd_req_queue[$];
bit wr_ooo_completion = 0;
bit rd_ooo_completion = 0;

always begin
	@(posedge spio.sp_etl2axi.clk_core);
	if(spio.sp_etl2axi.neigh_sc_req_valid[0] === 1 && spio.sp_etl2axi.neigh_sc_req_ready[0] == 1) begin
       if(spio.sp_etl2axi.neigh_sc_req_info.opcode[4:0] == evl_etlink_types_pkg::ET_LINK_REQ_Read) begin
          if(wr_stream_length >= 5) begin
             wr_stream_diff_num = wr_stream_diff[3] + wr_stream_diff[2] + wr_stream_diff[1] + wr_stream_diff[0];
             ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 0, 0, 0, 0, 0, wr_stream_length, 0, wr_stream_diff_num, 0, 0, 0);           
          end
          wr_stream_length = 0;
          wr_stream_diff = 0;

          rd_stream_length++;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:36] == 4'h8) rd_stream_diff[0] = 1;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:36] == 4'hc) rd_stream_diff[1] = 1;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:16] == 24'h002000 || spio.sp_etl2axi.neigh_sc_req_info.address[39:16] == 24'h002001 || spio.sp_etl2axi.neigh_sc_req_info.address[39:16] == 24'h002002) rd_stream_diff[2] = 1;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:20] == 20'h00404) rd_stream_diff[3] = 1;

          rd_req_queue.push_back(spio.sp_etl2axi.neigh_sc_req_info.id[7:0]);
       end
       if(spio.sp_etl2axi.neigh_sc_req_info.opcode[4:0] == evl_etlink_types_pkg::ET_LINK_REQ_Write) begin
          if(rd_stream_length >= 5) begin
             rd_stream_diff_num = rd_stream_diff[3] + rd_stream_diff[2] + rd_stream_diff[1] + rd_stream_diff[0];
             ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 0, 0, 0, 0, 0, 0, rd_stream_length, 0, rd_stream_diff_num, 0, 0);
          end
          rd_stream_length = 0;
          rd_stream_diff = 0;

          wr_stream_length++;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:36] == 4'h8) wr_stream_diff[0] = 1;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:36] == 4'hc) wr_stream_diff[1] = 1;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:16] == 24'h002000 || spio.sp_etl2axi.neigh_sc_req_info.address[39:16] == 24'h002001 || spio.sp_etl2axi.neigh_sc_req_info.address[39:16] == 24'h002002) wr_stream_diff[2] = 1;
          if(spio.sp_etl2axi.neigh_sc_req_info.address[39:20] == 20'h00404) wr_stream_diff[3] = 1;

          wr_req_queue.push_back(spio.sp_etl2axi.neigh_sc_req_info.id[7:0]);
       end
    end

    if(spio.sp_etl2axi.neigh_sc_rsp_valid === 1 && spio.sp_etl2axi.neigh_sc_rsp_ready == 1) begin
       bit[7:0] temp = 0;
       if(spio.sp_etl2axi.neigh_sc_rsp_info.opcode[1:0] == evl_etlink_types_pkg::ET_LINK_RSP_AckData) begin
          if(spio.sp_etl2axi.neigh_sc_rsp_info.id[7:0] == rd_req_queue[0]) temp = rd_req_queue.pop_front();
          else begin
          	 rd_ooo_completion = 1;
             ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
             for(int i=0; i<rd_req_queue.size(); i++) begin
                if(spio.sp_etl2axi.neigh_sc_rsp_info.id[7:0] == rd_req_queue[i]) begin
                   rd_req_queue.delete(i);
                   break;
                end
             end
          end
       end
       if(spio.sp_etl2axi.neigh_sc_rsp_info.opcode[1:0] == evl_etlink_types_pkg::ET_LINK_RSP_Ack) begin
          if(spio.sp_etl2axi.neigh_sc_rsp_info.id[7:0] == wr_req_queue[0]) temp = wr_req_queue.pop_front();
          else begin
          	 wr_ooo_completion = 1;
             ioshire_cov.ioshire_etl2axi_backpressure_cg.sample(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0);
             for(int i=0; i<wr_req_queue.size(); i++) begin
                if(spio.sp_etl2axi.neigh_sc_rsp_info.id[7:0] == wr_req_queue[i]) begin
                   wr_req_queue.delete(i);
                   break;
                end
             end
          end
       end
    end
end
