

logic ex_core_req__valid ;
logic tg_core_req__valid ;
logic me_core_req__valid ;
logic wb_core_req__valid ;

always @ (posedge clock) begin
    wb_core_req__valid <= me_core_req__valid ;
    me_core_req__valid <= tg_core_req__valid ;
    tg_core_req__valid <= ex_core_req__valid ;
    ex_core_req__valid <= `MIN_VPU.id_core_req.valid ;
end

logic ex_core_ctrl__tensorfma_start ;
logic tg_core_ctrl__tensorfma_start ;
logic me_core_ctrl__tensorfma_start ;
logic wb_core_ctrl__tensorfma_start ;

always @ (posedge clock) begin
    wb_core_ctrl__tensorfma_start <= me_core_ctrl__tensorfma_start ;
    me_core_ctrl__tensorfma_start <= tg_core_ctrl__tensorfma_start ;
    tg_core_ctrl__tensorfma_start <= ex_core_ctrl__tensorfma_start ;
    ex_core_ctrl__tensorfma_start <= `MIN_VPU.f0_core_ctrl.tensorfma_start ;
end

logic ex_core_ctrl__reduce_start ;
logic tg_core_ctrl__reduce_start ;
logic me_core_ctrl__reduce_start ;
logic wb_core_ctrl__reduce_start ;

always @ (posedge clock) begin
    wb_core_ctrl__reduce_start <= me_core_ctrl__reduce_start ;
    me_core_ctrl__reduce_start <= tg_core_ctrl__reduce_start ;
    tg_core_ctrl__reduce_start <= ex_core_ctrl__reduce_start ;
    ex_core_ctrl__reduce_start <= `MIN_VPU.f0_core_ctrl.reduce_start ;
end

logic ex_core_ctrl__tensorstore_start ;
logic tg_core_ctrl__tensorstore_start ;
logic me_core_ctrl__tensorstore_start ;
logic wb_core_ctrl__tensorstore_start ;

always @ (posedge clock) begin
    wb_core_ctrl__tensorstore_start <= me_core_ctrl__tensorstore_start ;
    me_core_ctrl__tensorstore_start <= tg_core_ctrl__tensorstore_start ;
    tg_core_ctrl__tensorstore_start <= ex_core_ctrl__tensorstore_start ;
    ex_core_ctrl__tensorstore_start <= `MIN_VPU.f0_core_ctrl.tensorstore_start ;
end

logic ex_core_ctrl__tensorquant_start ;
logic tg_core_ctrl__tensorquant_start ;
logic me_core_ctrl__tensorquant_start ;
logic wb_core_ctrl__tensorquant_start ;

always @ (posedge clock) begin
    wb_core_ctrl__tensorquant_start <= me_core_ctrl__tensorquant_start ;
    me_core_ctrl__tensorquant_start <= tg_core_ctrl__tensorquant_start ;
    tg_core_ctrl__tensorquant_start <= ex_core_ctrl__tensorquant_start ;
    ex_core_ctrl__tensorquant_start <= `MIN_VPU.f0_core_ctrl.tensorquant_start ;
end

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_REQ__THREAD_ID , clock, reset, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_req.thread_id )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_REQ__FCSR_RM , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_req.fcsr_rm )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_REQ__INST_BITS , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_req.inst_bits )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__CMD , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.cmd )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__TXFMA , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.txfma )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__SHSW , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.shsw )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__ROM , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.rom )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__SCATTER , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.scatter )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__CVT , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.cvt )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__TRANS , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.trans )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__GCVT , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.gcvt )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__FROMINTF , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.fromintf )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__TOINTM , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.tointm )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__TOINTF , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.tointf )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__MASKOP , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.maskop )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__LDST , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.ldst )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__M0REN , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.m0ren )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__MALLREN , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.mallren )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__MREN1 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.mren1 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__MREN2 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.mren2 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__WEN , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.wen )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__REN1 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.ren1 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__REN2 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.ren2 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__REN3 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.ren3 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__REND , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.rend )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__SWAP12 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.swap12 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__SWAP23 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.swap23 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__SWAP13 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.swap13 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__VECTOR , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.vector )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__DTYPE , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.dtype )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__FROMINT , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.fromint )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__TOINT , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.toint )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__FMA , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.fma )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__FMS , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.fms )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__ADD , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.add )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__SUB , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.sub )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__MUL , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.mul )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__ROUND , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.round )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_VPU_DECODER_SIGS__WFLAGS , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_vpu_decoder_sigs.wflags )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_REQ__GSCING , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_req.gscing )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_REQ__GSC_SRC , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_req.gsc_src )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_REQ__KILL , clock, reset, ex_core_req__valid , `MIN_VPU.ex_core_req.kill )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_REQ__FROMINT_DATA , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_req.fromint_data )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_REQ__THREAD_ID , clock, reset, ex_core_req__valid , `MIN_VPU.ex_core_req.thread_id )

`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN00 , clock, reset, `MIN_VPU.VPU_LANES[0].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN01 , clock, reset, `MIN_VPU.VPU_LANES[1].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN02 , clock, reset, `MIN_VPU.VPU_LANES[2].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN03 , clock, reset, `MIN_VPU.VPU_LANES[3].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN04 , clock, reset, `MIN_VPU.VPU_LANES[4].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN05 , clock, reset, `MIN_VPU.VPU_LANES[5].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN06 , clock, reset, `MIN_VPU.VPU_LANES[6].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN07 , clock, reset, `MIN_VPU.VPU_LANES[7].lane.f3_tima_rf_wen[0] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[0])

`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN10 , clock, reset, `MIN_VPU.VPU_LANES[0].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[0].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN11 , clock, reset, `MIN_VPU.VPU_LANES[1].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[1].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN12 , clock, reset, `MIN_VPU.VPU_LANES[2].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[2].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN13 , clock, reset, `MIN_VPU.VPU_LANES[3].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[3].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN14 , clock, reset, `MIN_VPU.VPU_LANES[4].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[4].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN15 , clock, reset, `MIN_VPU.VPU_LANES[5].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[5].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN16 , clock, reset, `MIN_VPU.VPU_LANES[6].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[6].lane.f8_rf_wdata[1])
`ASSERT_VALID_PORTS ( NO_X_TIMA_RF_WR_EN17 , clock, reset, `MIN_VPU.VPU_LANES[7].lane.f3_tima_rf_wen[1] , `MIN_VPU.VPU_LANES[7].lane.f8_rf_wdata[1])

logic [31:0] t0_id_inst_bits;
logic [31:0] t1_id_inst_bits;

always @* begin
   if (`MIN_IFC_INTPIPE.id_thread_id == 1'b0) begin
      t0_id_inst_bits = `MIN_IFC_INTPIPE.id_inst_bits;
      t1_id_inst_bits = t1_id_inst_bits;
   end
   else if (`MIN_IFC_INTPIPE.id_thread_id == 1'b1) begin
      t0_id_inst_bits = t0_id_inst_bits;
      t1_id_inst_bits = `MIN_IFC_INTPIPE.id_inst_bits;
   end
   else begin
      t0_id_inst_bits = 32'b0;
      t1_id_inst_bits = 32'b0;
   end
end

reg vpu_rf_read_and_write [`VPU_LANE_NUM-1:0][2:0][1:0];
reg vpu_rf_addr_match [`VPU_LANE_NUM-1:0][2:0][1:0];
reg fmv_workaround [1:0];
reg vpu_rf_rw_exceptions [`VPU_LANE_NUM-1:0][2:0][1:0];

assign fmv_workaround[0] = ((t0_id_inst_bits & 32'hfff07fff) == 32'he0000053);
assign fmv_workaround[1] = ((t1_id_inst_bits & 32'hfff07fff) == 32'he0000053);

// Assert for bug in FV-260: Check that VPU RF read and write ports never access the same address in the same cycle
bit enable_mmi_bug_check;
initial begin
   enable_mmi_bug_check = 1'b0;
   if (`et_value_plusargs("enable_mmi_bug_check=%0d", enable_mmi_bug_check)) begin
      $write("found enable_mmi_bug_check=%0d\n", enable_mmi_bug_check);
   end
end

genvar vpu_lane;
genvar rd_port;
genvar wr_port;
generate 
  for (vpu_lane = 0; vpu_lane < `VPU_LANE_NUM; vpu_lane++)
  begin : assert_fv_260_lanes
     for (rd_port = 0; rd_port < 3; rd_port++)
     begin
        for (wr_port = 0; wr_port < 2; wr_port++)
        begin
           if (rd_port != 1) begin
              assign vpu_rf_rw_exceptions[vpu_lane][rd_port][wr_port] = fmv_workaround[`MIN_VPU.VPU_LANES[vpu_lane].lane.rf.rd_thrid[rd_port]] || `MIN_VPU.ctrl.id_vpu_ctrl.sigs.maskop || `MIN_VPU.ctrl.ex_core_gscing;
           end
           else begin
              assign vpu_rf_rw_exceptions[vpu_lane][rd_port][wr_port] = fmv_workaround[`MIN_VPU.VPU_LANES[vpu_lane].lane.rf.rd_thrid[rd_port]] || `MIN_VPU.ctrl.id_vpu_ctrl.sigs.maskop;
           end

           assign vpu_rf_read_and_write[vpu_lane][rd_port][wr_port] = enable_mmi_bug_check && ((`MIN_VPU.VPU_LANES[vpu_lane].lane.rf.rd_en[rd_port] & `MIN_VPU.VPU_LANES[vpu_lane].lane.rf.wr_en[wr_port]) && (`MIN_VPU.VPU_LANES[vpu_lane].lane.rf.rd_thrid[rd_port] == `MIN_VPU.VPU_LANES[vpu_lane].lane.rf.wr_thrid[wr_port])) ? 1'b1 : 1'b0;
           assign vpu_rf_addr_match[vpu_lane][rd_port][wr_port] = (`MIN_VPU.VPU_LANES[vpu_lane].lane.rf.rd_addr[rd_port] == `MIN_VPU.VPU_LANES[vpu_lane].lane.rf.wr_addr[wr_port]) ? 1'b1 : 1'b0;

           `ASSERT_ALWAYS_MSG ( NO_SIMULTANEOUS_VPU_RF_R_W, clock, reset, vpu_rf_read_and_write[vpu_lane][rd_port][wr_port] |-> ((!vpu_rf_addr_match[vpu_lane][rd_port][wr_port]) || vpu_rf_rw_exceptions[vpu_lane][rd_port][wr_port]), $sformatf("FV-260: VPU LANE %0d Read Port %0d and Write Port %0d are accessing the same entry at the same time", vpu_lane, rd_port, wr_port))

        end
     end
  end
endgenerate



`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__IS_CONV , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.is_conv )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__COLS_B , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.cols_b )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__ROWS_A , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.rows_a )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRLCOLS_A , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.cols_a )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRLSTART_A , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.start_a )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__TO_VRF , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.to_vrf )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__U_B , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.u_b )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__U_A , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.u_a )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__TEN_B , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.ten_b )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__SCP_B , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.scp_b )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__SCP_A , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.scp_a )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__MODE , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.mode )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CTRL__FIRST_PASS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_ctrl.first_pass )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CONV_BITS_READY , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_conv_bits_ready )

// emizan: Commenting out -- tensor_fma16a32_latency_tenb micro kernel
//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORFMA_CONV_BITS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorfma_start , `MIN_VPU.f0_core_ctrl.tensorfma_conv_bits )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__REDUCE__START_REG , clock, reset, `MIN_VPU.f0_core_ctrl.reduce_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.reduce.start_reg )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__REDUCE__OP , clock, reset, `MIN_VPU.f0_core_ctrl.reduce_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.reduce.op )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__REDUCE__NUM_REGS , clock, reset, `MIN_VPU.f0_core_ctrl.reduce_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.reduce.num_regs )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__REDUCE__PARTNER , clock, reset, `MIN_VPU.f0_core_ctrl.reduce_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.reduce.partner )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__REDUCE__ACTION , clock, reset, `MIN_VPU.f0_core_ctrl.reduce_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.reduce.action )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__SRC_INC , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.src_inc )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__START_REG , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.start_reg )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__COLS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.cols )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__ROWS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.rows )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__COOP , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.coop )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__SCP , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.scp )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__REDUCE_CTRL__ADDR , clock, reset, `MIN_VPU.f0_core_ctrl.tensorstore_start , `MIN_VPU.f0_core_ctrl.reduce_ctrl.addr )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORQUANT_CTRL__START_REG , clock, reset, `MIN_VPU.f0_core_ctrl.tensorquant_start , `MIN_VPU.f0_core_ctrl.tensorquant_ctrl.start_reg )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORQUANT_CTRL__COLS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorquant_start , `MIN_VPU.f0_core_ctrl.tensorquant_ctrl.cols )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORQUANT_CTRL__ROWS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorquant_start , `MIN_VPU.f0_core_ctrl.tensorquant_ctrl.rows )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORQUANT_CTRL__SCP_SRC , clock, reset, `MIN_VPU.f0_core_ctrl.tensorquant_start , `MIN_VPU.f0_core_ctrl.tensorquant_ctrl.scp_src )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__TENSORQUANT_CTRL__TRANS , clock, reset, `MIN_VPU.f0_core_ctrl.tensorquant_start , `MIN_VPU.f0_core_ctrl.tensorquant_ctrl.trans )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F0_CORE_CTRL__PENDING_SCOREBOARD_T0 , clock, 1'b1, (`MIN_VPU.f0_core_ctrl.tensorfma_start|`MIN_VPU.f0_core_ctrl.reduce_start|`MIN_VPU.f0_core_ctrl.tensorstore_start|`MIN_VPU.f0_core_ctrl.tensorquant_start) , `MIN_VPU.f0_core_ctrl.pending_scoreboard_t0 )

// emizan: Commenting out -- tensor_reduce micro kernel
//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_DCACHE_RESP__THREAD_ID , clock, reset, `MIN_VPU.wb_dcache_resp_valid , `MIN_VPU.wb_dcache_resp.thread_id )

// emizan: Commenting out -- tensor_reduce micro_kernel
// `ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_DCACHE_RESP__GDST , clock, reset, `MIN_VPU.wb_dcache_resp_valid , `MIN_VPU.wb_dcache_resp.gdst )

// emizan: Commenting out -- tensor_reduce micro kernel
// `ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_DCACHE_RESP__TYP , clock, reset, `MIN_VPU.wb_dcache_resp_valid , `MIN_VPU.wb_dcache_resp.typ )

// emizan: Commenting out -- tensor_reduce micro kernel
// `ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_DCACHE_RESP__ADDR , clock, reset, `MIN_VPU.wb_dcache_resp_valid , `MIN_VPU.wb_dcache_resp.addr )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_DCACHE_RESP__DATA , clock, reset, (`MIN_VPU.wb_dcache_resp_valid && `MIN_VPU.wb_dcache_resp.typ == dcache_type_PS) , `MIN_VPU.wb_dcache_resp.data )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_DCACHE_RESP__PS_MASK , clock, reset, `MIN_VPU.wb_dcache_resp_valid , `MIN_VPU.wb_dcache_resp.ps_mask )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__M0REN , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.m0ren )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__MALLREN , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.mallren )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__MREN1 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.mren1 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__MREN2 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.mren2 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__WEN , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.wen )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__REN1 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.ren1 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__REN2 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.ren2 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__REN3 , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.ren3 )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__REND , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.rend )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__IS_TRANS , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.is_trans )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__TRANS_BUSY , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.trans_busy )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__ID_TRANS_INSERT , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.id_trans_insert )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__SCOREBOARD , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.scoreboard )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__TFMA_ENABLED , clock, reset, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.tfma_enabled )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__TFMA_WRRF_ENABLED , clock, reset, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.tfma_wrrf_enabled )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__TQUANT_ENABLED , clock, reset, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.tquant_enabled )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__REDUCE_ENABLED , clock, reset, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.reduce_enabled )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__FFLAGS_STALL , clock, reset, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.fflags_stall )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_ID_CORE_CTRL__FROMINT , clock, 1'b1, `MIN_VPU.id_core_req.valid , `MIN_VPU.id_core_ctrl.fromint )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_CTRL__TRANS_DIV , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_ctrl.trans_div )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_CTRL__TOINTM , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_ctrl.tointm )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_CTRL__ILLEGAL_RM , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_ctrl.illegal_rm )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_CTRL__PS_MASK , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_ctrl.ps_mask )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_EX_CORE_CTRL__GSC_FS , clock, 1'b1, ex_core_req__valid , `MIN_VPU.ex_core_ctrl.gsc_fs )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F2_CORE_CTRL__TRANS_DIV , clock, 1'b1, tg_core_req__valid , `MIN_VPU.f2_core_ctrl.trans_div )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F2_CORE_CTRL__LDST , clock, 1'b1, tg_core_req__valid , `MIN_VPU.f2_core_ctrl.ldst )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F2_CORE_CTRL__FMA , clock, 1'b1, tg_core_req__valid , `MIN_VPU.f2_core_ctrl.fma )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F2_CORE_CTRL__STORE_DATA , clock, 1'b1, tg_core_req__valid , `MIN_VPU.f2_core_ctrl.store_data )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F2_CORE_CTRL__SCATTER_DATA , clock, 1'b1, tg_core_req__valid , `MIN_VPU.f2_core_ctrl.scatter_data )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F2_CORE_CTRL__TOINTM , clock, 1'b1, tg_core_req__valid , `MIN_VPU.f2_core_ctrl.tointm )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F3_CORE_CTRL__TRANS_DIV , clock, 1'b1, me_core_req__valid , `MIN_VPU.f3_core_ctrl.trans_div )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F3_CORE_CTRL__LDST , clock, 1'b1, me_core_req__valid , `MIN_VPU.f3_core_ctrl.ldst )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F3_CORE_CTRL__FMA , clock, 1'b1, me_core_req__valid , `MIN_VPU.f3_core_ctrl.fma )

`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_F3_CORE_CTRL__TOINTM , clock, 1'b1, me_core_req__valid , `MIN_VPU.f3_core_ctrl.tointm )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__TRANS_DIV , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.trans_div )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__LDST , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.ldst )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__FP_TOINT , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.fp_toint )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__FMA , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.fma )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__TOINT_DATA , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.toint_data )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__MOVA_MX , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.mova_mx )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__FCSR_FLAGS_VALID , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.fcsr_flags_valid )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__FCSR_FLAGS , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.fcsr_flags )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__THREAD_ID , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.thread_id )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__FS , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.fs )

//`ASSERT_VALID_PORTS ( NO_X_WHILE_VALID_EN_WB_CORE_CTRL__TOINTM , clock, 1'b1, wb_core_req__valid , `MIN_VPU.wb_core_ctrl.tointm )

//`ASSERT_ALWAYS_MSG ( NEVER_X_F2_CORE_KILL, clock, 1'b1, `MIN_VPU.f2_core_kill, "f2_core_kill should never be X")

//`ASSERT_ALWAYS_MSG ( NEVER_X_F3_CORE_KILL, clock, 1'b1, `MIN_VPU.f3_core_kill, "f3_core_kill should never be X")

//`ASSERT_ALWAYS_MSG ( NEVER_X_F4_CORE_KILL, clock, 1'b1, `MIN_VPU.f4_core_kill, "f4_core_kill should never be X")

//`ASSERT_ALWAYS_MSG ( NEVER_X_F0_CORE_CTRL__TXFMA_27_SLEEPING, clock, 1'b1, !$isunknown(`MIN_VPU.f0_core_ctrl.txfma_27_sleeping), "`MIN_VPU.f0_core_ctrl.txfma_27_sleeping should never be X")
   
`ifndef DFT_SD_DUMP_FLOPS
`ASSERT_ALWAYS_MSG ( NEVER_X_CHICKEN_BIT_VPULANE, clock, reset, !$isunknown(`MIN_VPU.chicken_bit_vpulane), "`MIN_VPU.chicken_bit_vpulane should never be X")

`ASSERT_ALWAYS_MSG ( NEVER_X_CHICKEN_BIT_VPUTIMA, clock, reset, !$isunknown(`MIN_VPU.chicken_bit_vputima), "`MIN_VPU.chicken_bit_vputima should never be X")

`ASSERT_ALWAYS_MSG ( NEVER_X_CHICKEN_BIT_VPUTRANS, clock, reset, !$isunknown(`MIN_VPU.chicken_bit_vputrans), "`MIN_VPU.chicken_bit_vputrans should never be X")
`endif

  //vpu mask
logic multiple_rf_wen_assert;
assign multiple_rf_wen_assert = (((|`MIN_VPU.ctrl.mask.f8_regmask_store) & (|`MIN_VPU.ctrl.mask.f3_regmask_store)) & ({`MIN_VPU.ctrl.mask.f3_thread_id,`MIN_VPU.ctrl.mask.f3_regmask_waddr}=={`MIN_VPU.ctrl.mask.f8_thread_id,`MIN_VPU.ctrl.mask.f8_regmask_waddr}))
                              |  ((|`MIN_VPU.ctrl.mask.f3_regmask_store) & `MIN_VPU.ctrl.mask.f3_regmask_store_all_qual)
                              |  ((|`MIN_VPU.ctrl.mask.f8_regmask_store) & `MIN_VPU.ctrl.mask.f3_regmask_store_all_qual & (`MIN_VPU.ctrl.mask.f3_thread_id==`MIN_VPU.ctrl.mask.f8_thread_id));
`ASSERT_ALWAYS_MSG(ERROR_multipleRFWen, clock, reset_for_assert, ~multiple_rf_wen_assert, "ERROR: multiple writes to mask RF entry in the same cycle")


  //vpu ml
`ASSERT_ALWAYS_MSG(ERROR_multiple_insns, clock, reset_for_assert, !(`MIN_VPU.ctrl.ml.tensorfma_inst_en_next && `MIN_VPU.ctrl.ml.reduce_inst_en_next), "More than one stuffed instruction at same time")
