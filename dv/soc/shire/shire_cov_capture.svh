shire_cvg shire_cov = new();

`ifndef SKIP_FAKE_ESR_INIT
`ifndef ET_PTPX_SIM
    generate
        for (neighborhood_idx = 0; neighborhood_idx < NUM_NEIGH; neighborhood_idx = neighborhood_idx + 1) begin
            if ((SC_LITE == 1'b0) && ((SC_STUB == 1'b0) || ((NEIGH_STUB[neighborhood_idx] == 1'b0) && (NEIGH_DISABLE[neighborhood_idx] == 1'b0)))) begin
				always @(posedge shire_clock) begin
					if(`et_test_plusargs("LVDPLL_MODE=") == 1) begin
     					if (reset_cold === 1'b0) begin
        					if(neigh_sc_rsp_valid[neighborhood_idx] === 1'b1 && neigh_sc_rsp_ready[neighborhood_idx] === 1'b1) begin
         						shire_cov.lvdpll_mode_cg.sample(shire_channel_wrap_inst.shire_apb_injector.shire_id, shire_channel_wrap_inst.shire_apb_injector.freq_mode + 1, evl_etlink_req_opcode_t'(neigh_sc_req_info[neighborhood_idx].opcode), shire_channel_wrap_inst.shire_apb_injector.clock_sel);
         						shire_cov.dll_clk_delay_cg.sample(shire_channel_wrap_inst.shire_pll.sel_taps_feedback_shire, shire_channel_wrap_inst.shire_pll.sel_taps_feedback_neigh, shire_channel_wrap_inst.shire_pll.sel_taps_neigh);
        					end
      					end // if (reset_cold === 1'b0)
   					 end
				end // always @ (posedge shire_clock)

				always @(posedge shire_channel_wrap_inst.shire_apb_injector.esr_dll_lock) begin
    				shire_cov.dll_feedback_delay_offset_cg.sample(int'((shire_channel_wrap_inst.shire_pll.CLK_SHIRE_DLY - shire_channel_wrap_inst.shire_pll.CLK_NEIGH_DLY)*1000));
				end // always @ (posedge shire_clock)

            end
        end
    endgenerate
`endif // ifndef ET_PTPX_SIM
`endif // ifndef SKIP_FAKE_ESR_INIT

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge sys_mesh_slave_axi_AWVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 0, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge sys_mesh_slave_axi_ARVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 0, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge to_l3_mesh_master_axi_AWVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 1, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge to_l3_mesh_master_axi_ARVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 1, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge to_sys_mesh_master_axi_AWVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 2, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge to_sys_mesh_master_axi_ARVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 2, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge l3_mesh_slave_axi_AWVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 3, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge l3_mesh_slave_axi_ARVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 3, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge uc_to_l3_mesh_master_axi_AWVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 4, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge uc_to_l3_mesh_master_axi_ARVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 4, 1);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge uc_to_sys_mesh_master_axi_AWVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 5, 0);
end

always begin
	time first_edge;
	time second_edge;
	int freq;
	@(posedge uc_to_sys_mesh_master_axi_ARVALID)
	@(posedge noc_clock);
	first_edge = $time;
	@(posedge noc_clock);
	second_edge = $time;
	freq =  int'(1000/(second_edge - first_edge));
    shire_cov.noc_clock_freq_cg.sample(freq, 5, 1);
end
