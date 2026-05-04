`ifdef EVL_COVERAGE_ON

`ifndef SBM_IFC
   `define SBM_IFC shire_top.shire_channel_wrap_inst.shire_control_gen.shire_control_status_inst.sbm_top
   `define SBM_NOC_IFC `SBM_IFC.sbm_nocs
`endif

evl_shire_bus_master_cvg sbm_cvg_inst = new();

// -----------------------------------
//     Covergroup sampling 
// -----------------------------------

always @(posedge shire_clock) begin

sbm_cvg_inst.sample_sbm_decode(`SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ADDR,
                               `SBM_NOC_IFC.sbm_mesh_slave_axi_AR.ADDR,
                               `SBM_NOC_IFC.sbm_mesh_slave_axi_AWVALID,
                               `SBM_NOC_IFC.sbm_mesh_slave_axi_ARVALID,
                               `SBM_NOC_IFC.sbm_mesh_slave_axi_ARREADY,
                               `SHIRE_IFC.m_dec_err,
                               `SHIRE_IFC.m_dec_err_msb,
                               `SHIRE_IFC.m_dec_err_shire,
                               `SHIRE_IFC.m_dec_err_neigh,
                               `SHIRE_IFC.m_dec_err_minion,
                               `SHIRE_IFC.m_dec_err_sc_bank,
                               `SHIRE_IFC.m_dec_err_rbox,
                               `SBM_IFC.reset_neigh,
                               `SBM_IFC.reset_sc,
                               `SBM_IFC.reset_rbox,
                               `SBM_NOC_IFC.sbm_fifo_req_not_empty,
                               `SBM_NOC_IFC.axi_to_apb_ready,
                               `SBM_NOC_IFC.sbm_fifo_req_push,
                               `SBM_NOC_IFC.counter,
                               `SBM_NOC_IFC.sbm_buffer_count,
                               `SBM_NOC_IFC.reserved_wr_pos,
                               `SHIRE_IFC.m_simult_same_endpoint);

sbm_cvg_inst.sample_sbm_rsp(`SBM_NOC_IFC.writeRsp_fifo_not_empty,
                            `SBM_NOC_IFC.readRsp_fifo_not_empty,
                            `SBM_NOC_IFC.error_return,
                            `SBM_NOC_IFC.error_search,
                            `SBM_NOC_IFC.sbm_mesh_slave_axi_BREADY,
                            `SBM_NOC_IFC.sbm_mesh_slave_axi_RREADY,
                            `SBM_NOC_IFC.mesh_slave_axi_BVALID,
                            `SBM_NOC_IFC.mesh_slave_axi_RVALID,
                            `SBM_NOC_IFC.bcast_control[0].valid,
                            `SBM_NOC_IFC.bcast_control[1].valid);

sbm_cvg_inst.sample_axi_to_apb(`SBM_NOC_IFC.axi_to_apb_ready,
                               `SBM_NOC_IFC.sbm_mesh_slave_axi_AW.ADDR,
                               `SBM_NOC_IFC.sbm_mesh_slave_axi_AWVALID);

for (int i = 0; i < 4; i++) begin
   //Neigh
   if(`SHIRE_IFC.m_neigh_rd_req && (`SHIRE_IFC.m_sbm_rd_req_addr[19:16] == i) && !(`SBM_NOC_IFC.axi_to_apb_ready[i]))
     sbm_cvg_inst.cg_fifo_full_new_read_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_ARREADY);
   if(`SHIRE_IFC.m_neigh_wr_req && (`SHIRE_IFC.m_sbm_wr_req_addr[19:16] == i) && !(`SBM_NOC_IFC.axi_to_apb_ready[i]))
     sbm_cvg_inst.cg_fifo_full_new_write_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_AWREADY);

   //SC
   if(`SHIRE_IFC.m_sc_rd_req && (`SHIRE_IFC.m_sbm_rd_req_addr[16:13] == i) && !(`SBM_NOC_IFC.axi_to_apb_ready[i+4]))
     sbm_cvg_inst.cg_fifo_full_new_read_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_ARREADY);
   if(`SHIRE_IFC.m_sc_wr_req && (`SHIRE_IFC.m_sbm_wr_req_addr[16:13] == i) && !(`SBM_NOC_IFC.axi_to_apb_ready[i+4]))
     sbm_cvg_inst.cg_fifo_full_new_write_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_AWREADY);
end

//Shire
if(`SHIRE_IFC.m_shire_rd_req && !(`SBM_NOC_IFC.axi_to_apb_ready[8]))
	sbm_cvg_inst.cg_fifo_full_new_read_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_ARREADY);
if(`SHIRE_IFC.m_shire_wr_req && !(`SBM_NOC_IFC.axi_to_apb_ready[8]))
	sbm_cvg_inst.cg_fifo_full_new_write_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_AWREADY);

//Rbox
if(`SHIRE_IFC.m_rbox_rd_req && !(`SBM_NOC_IFC.axi_to_apb_ready[9]))
	sbm_cvg_inst.cg_fifo_full_new_read_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_ARREADY);
if(`SHIRE_IFC.m_rbox_wr_req && !(`SBM_NOC_IFC.axi_to_apb_ready[9]))
	sbm_cvg_inst.cg_fifo_full_new_write_req.sample(`SBM_NOC_IFC.sbm_mesh_slave_axi_AWREADY);

end

`endif
