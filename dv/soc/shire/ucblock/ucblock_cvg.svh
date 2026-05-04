//`ifdef EVL_COVERAGE_ON

`ifndef __UCBLOCK_CVG
`define __UCBLOCK_CVG

`ifndef walking_one_4
`define walking_one_4  \
    wildcard bins bit_0_as_0 = {4'b???0};\
    wildcard bins bit_0_as_1 = {4'b???1};\
    wildcard bins bit_1_as_0 = {4'b??0?};\
    wildcard bins bit_1_as_1 = {4'b??1?};\
    wildcard bins bit_2_as_0 = {4'b?0??};\
    wildcard bins bit_2_as_1 = {4'b?1??};\
    wildcard bins bit_3_as_0 = {4'b0???};\
    wildcard bins bit_3_as_1 = {4'b1???};
`endif // ifndef walking_one_4

`ifndef walking_one_32
`define walking_one_32  \
    wildcard bins bit_00_as_0 = {32'b???????????????????????????????0};\
    wildcard bins bit_00_as_1 = {32'b???????????????????????????????1};\
    wildcard bins bit_01_as_0 = {32'b??????????????????????????????0?};\
    wildcard bins bit_01_as_1 = {32'b??????????????????????????????1?};\
    wildcard bins bit_02_as_0 = {32'b?????????????????????????????0??};\
    wildcard bins bit_02_as_1 = {32'b?????????????????????????????1??};\
    wildcard bins bit_03_as_0 = {32'b????????????????????????????0???};\
    wildcard bins bit_03_as_1 = {32'b????????????????????????????1???};\
    wildcard bins bit_04_as_0 = {32'b???????????????????????????0????};\
    wildcard bins bit_04_as_1 = {32'b???????????????????????????1????};\
    wildcard bins bit_05_as_0 = {32'b??????????????????????????0?????};\
    wildcard bins bit_05_as_1 = {32'b??????????????????????????1?????};\
    wildcard bins bit_06_as_0 = {32'b?????????????????????????0??????};\
    wildcard bins bit_06_as_1 = {32'b?????????????????????????1??????};\
    wildcard bins bit_07_as_0 = {32'b????????????????????????0???????};\
    wildcard bins bit_07_as_1 = {32'b????????????????????????1???????};\
    wildcard bins bit_08_as_0 = {32'b???????????????????????0????????};\
    wildcard bins bit_08_as_1 = {32'b???????????????????????1????????};\
    wildcard bins bit_09_as_0 = {32'b??????????????????????0?????????};\
    wildcard bins bit_09_as_1 = {32'b??????????????????????1?????????};\
    wildcard bins bit_10_as_0 = {32'b?????????????????????0??????????};\
    wildcard bins bit_10_as_1 = {32'b?????????????????????1??????????};\
    wildcard bins bit_11_as_0 = {32'b????????????????????0???????????};\
    wildcard bins bit_11_as_1 = {32'b????????????????????1???????????};\
    wildcard bins bit_12_as_0 = {32'b???????????????????0????????????};\
    wildcard bins bit_12_as_1 = {32'b???????????????????1????????????};\
    wildcard bins bit_13_as_0 = {32'b??????????????????0?????????????};\
    wildcard bins bit_13_as_1 = {32'b??????????????????1?????????????};\
    wildcard bins bit_14_as_0 = {32'b?????????????????0??????????????};\
    wildcard bins bit_14_as_1 = {32'b?????????????????1??????????????};\
    wildcard bins bit_15_as_0 = {32'b????????????????0???????????????};\
    wildcard bins bit_15_as_1 = {32'b????????????????1???????????????};\
    wildcard bins bit_16_as_0 = {32'b???????????????0????????????????};\
    wildcard bins bit_16_as_1 = {32'b???????????????1????????????????};\
    wildcard bins bit_17_as_0 = {32'b??????????????0?????????????????};\
    wildcard bins bit_17_as_1 = {32'b??????????????1?????????????????};\
    wildcard bins bit_18_as_0 = {32'b?????????????0??????????????????};\
    wildcard bins bit_18_as_1 = {32'b?????????????1??????????????????};\
    wildcard bins bit_19_as_0 = {32'b????????????0???????????????????};\
    wildcard bins bit_19_as_1 = {32'b????????????1???????????????????};\
    wildcard bins bit_20_as_0 = {32'b???????????0????????????????????};\
    wildcard bins bit_20_as_1 = {32'b???????????1????????????????????};\
    wildcard bins bit_21_as_0 = {32'b??????????0?????????????????????};\
    wildcard bins bit_21_as_1 = {32'b??????????1?????????????????????};\
    wildcard bins bit_22_as_0 = {32'b?????????0??????????????????????};\
    wildcard bins bit_22_as_1 = {32'b?????????1??????????????????????};\
    wildcard bins bit_23_as_0 = {32'b????????0???????????????????????};\
    wildcard bins bit_23_as_1 = {32'b????????1???????????????????????};\
    wildcard bins bit_24_as_0 = {32'b???????0????????????????????????};\
    wildcard bins bit_24_as_1 = {32'b???????1????????????????????????};\
    wildcard bins bit_25_as_0 = {32'b??????0?????????????????????????};\
    wildcard bins bit_25_as_1 = {32'b??????1?????????????????????????};\
    wildcard bins bit_26_as_0 = {32'b?????0??????????????????????????};\
    wildcard bins bit_26_as_1 = {32'b?????1??????????????????????????};\
    wildcard bins bit_27_as_0 = {32'b????0???????????????????????????};\
    wildcard bins bit_27_as_1 = {32'b????1???????????????????????????};\
    wildcard bins bit_28_as_0 = {32'b???0????????????????????????????};\
    wildcard bins bit_28_as_1 = {32'b???1????????????????????????????};\
    wildcard bins bit_29_as_0 = {32'b??0?????????????????????????????};\
    wildcard bins bit_29_as_1 = {32'b??1?????????????????????????????};\
    wildcard bins bit_30_as_0 = {32'b?0??????????????????????????????};\
    wildcard bins bit_30_as_1 = {32'b?1??????????????????????????????};\
    wildcard bins bit_31_as_0 = {32'b0???????????????????????????????};\
    wildcard bins bit_31_as_1 = {32'b1???????????????????????????????};
`endif // ifndef walking_one_32


class ucblock_cvg extends evl_void;

   covergroup ucblock_req_buffer_valid_cg with function sample (bit [31:0] req_buffer_valid); 
      
      option.name = "ucblock_req_buffer_valid_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_req_buffer_valid : coverpoint req_buffer_valid {`walking_one_32}
      
      cp_ucblock_req_buffer_full  : coverpoint req_buffer_valid
      {
	 bins valid_all = {32'hffffffff};
      }
      
   endgroup : ucblock_req_buffer_valid_cg
   
   
   covergroup ucblock_rsp_valid_cg with function sample (bit [3:0] uc_min_rsp_valid);

      option.name = "ucblock_rsp_valid_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_rsp_valid : coverpoint uc_min_rsp_valid {`walking_one_4}
      
   endgroup : ucblock_rsp_valid_cg
   
   covergroup ucblock_etlink_port_id_cg with function sample (bit [1:0] neigh_uc_req_info_port_id, bit etlink_req_valid, bit etlink_req_ready);

      option.name = "ucblock_etlink_port_id_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_ucblock_etlink_port_id : coverpoint neigh_uc_req_info_port_id iff (etlink_req_valid && etlink_req_ready)
	{
	 bins port_id_0 = {0};
	 bins port_id_1 = {1};
	 bins port_id_2 = {2};
	 bins port_id_3 = {3};
	 }
      
   endgroup : ucblock_etlink_port_id_cg
   
   covergroup ucblock_all_requests_cg with function sample (bit etlink_req_valid, etlink_req_ready, etlink_rsp_valid, etlink_rsp_ready,
							 tosys_awvalid, tosys_awready, tosys_wvalid, tosys_wready, tosys_bvalid, tosys_bready,
							 tosys_arvalid, tosys_arready, tosys_rvalid, tosys_rready, 
							 tol3_awvalid, tol3_awready, tol3_wvalid, tol3_wready, tol3_bvalid, tol3_bready,
							 tol3_arvalid, tol3_arready, tol3_rvalid, tol3_rready, 
							 fromsys_awvalid, fromsys_awready, fromsys_wvalid, fromsys_wready, fromsys_bvalid, fromsys_bready,
							 fromsys_arvalid, fromsys_arready, fromsys_rvalid, fromsys_rready,
							 sbm_awvalid, sbm_awready, sbm_wvalid, sbm_wready, sbm_bvalid, sbm_bready,
							 sbm_arvalid, sbm_arready, sbm_rvalid, sbm_rready);      

      option.name = "ucblock_all_requests_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_etlink_req: coverpoint ( ( etlink_req_valid == 1 ) & ( etlink_req_ready == 1 ) )
	{ bins req_valid = {1}; }
      cp_ucblock_etlink_rsp: coverpoint ( ( etlink_rsp_valid == 1 ) & ( etlink_rsp_ready == 1 ) )
	{ bins rsp_valid = {1}; }
      
      cp_ucblock_tosys_aw: coverpoint ( ( tosys_awvalid == 1 ) & ( tosys_awready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tosys_ar: coverpoint ( ( tosys_arvalid == 1 ) & ( tosys_arready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tosys_w: coverpoint ( ( tosys_wvalid == 1 ) & ( tosys_wready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tosys_r: coverpoint ( ( tosys_rvalid == 1 ) & ( tosys_rready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tosys_b: coverpoint ( ( tosys_bvalid == 1 ) & ( tosys_bready == 1 ) )
	{ bins rsp_valid = {1}; }
      
      cp_ucblock_tol3_aw: coverpoint ( ( tol3_awvalid == 1 ) & ( tol3_awready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tol3_ar: coverpoint ( ( tol3_arvalid == 1 ) & ( tol3_arready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tol3_w: coverpoint ( ( tol3_wvalid == 1 ) & ( tol3_wready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tol3_r: coverpoint ( ( tol3_rvalid == 1 ) & ( tol3_rready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_tol3_b: coverpoint ( ( tol3_bvalid == 1 ) & ( tol3_bready == 1 ) )
	{ bins rsp_valid = {1}; }
      
      cp_ucblock_sbm_aw: coverpoint ( ( sbm_awvalid == 1 ) & ( sbm_awready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_sbm_ar: coverpoint ( ( sbm_arvalid == 1 ) & ( sbm_arready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_sbm_w: coverpoint ( ( sbm_wvalid == 1 ) & ( sbm_wready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_sbm_r: coverpoint ( ( sbm_rvalid == 1 ) & ( sbm_rready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_sbm_b: coverpoint ( ( sbm_bvalid == 1 ) & ( sbm_bready == 1 ) )
	{ bins rsp_valid = {1}; }
      
      cp_ucblock_fromsys_aw: coverpoint ( ( fromsys_awvalid == 1 ) & ( fromsys_awready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_fromsys_ar: coverpoint ( ( fromsys_arvalid == 1 ) & ( fromsys_arready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_fromsys_w: coverpoint ( ( fromsys_wvalid == 1 ) & ( fromsys_wready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_fromsys_r: coverpoint ( ( fromsys_rvalid == 1 ) & ( fromsys_rready == 1 ) )
	{ bins rsp_valid = {1}; }
      cp_ucblock_fromsys_b: coverpoint ( ( fromsys_bvalid == 1 ) & ( fromsys_bready == 1 ) )
	{ bins rsp_valid = {1}; }

      // Cover that the request and response are asserted at the same time
      ccp_etlink_req: cross cp_ucblock_etlink_req, cp_ucblock_etlink_rsp;
      ccp_tosys_wr:   cross cp_ucblock_tosys_aw, cp_ucblock_tosys_b;
      ccp_tosys_rd:   cross cp_ucblock_tosys_ar, cp_ucblock_tosys_r;
      ccp_tol3_wr:    cross cp_ucblock_tol3_aw, cp_ucblock_tol3_b;
      ccp_tol3_rd:    cross cp_ucblock_tol3_ar, cp_ucblock_tol3_r;
      ccp_fromsys_wr:     cross cp_ucblock_fromsys_aw, cp_ucblock_fromsys_b;
      ccp_fromsys_rd:     cross cp_ucblock_fromsys_ar, cp_ucblock_fromsys_r;

      // Cover that the requests and responses are asserted at the same time between AXIs
      ccp_tosys_tol3_aw: cross cp_ucblock_tosys_aw, cp_ucblock_tol3_aw;
      ccp_tosys_tol3_ar: cross cp_ucblock_tosys_ar, cp_ucblock_tol3_ar;
      ccp_tosys_tol3_r: cross cp_ucblock_tosys_r, cp_ucblock_tol3_r;
      ccp_tosys_tol3_b: cross cp_ucblock_tosys_b, cp_ucblock_tol3_b;
      ccp_tosys_tol3_w: cross cp_ucblock_tosys_w, cp_ucblock_tol3_w;

      // Cover that the requests and responses are asserted at the same time between AXIs
      ccp_fromsys_sbm_aw: cross cp_ucblock_fromsys_aw, cp_ucblock_sbm_aw;
      ccp_fromsys_sbm_ar: cross cp_ucblock_fromsys_ar, cp_ucblock_sbm_ar;
      ccp_fromsys_sbm_r: cross cp_ucblock_fromsys_r, cp_ucblock_sbm_r;
      ccp_fromsys_sbm_b: cross cp_ucblock_fromsys_b, cp_ucblock_sbm_b;
      ccp_fromsys_sbm_w: cross cp_ucblock_fromsys_w, cp_ucblock_sbm_w;

      // Cover that the etlink request and axi response are asserted at the same time
      ccp_etlinkreq_tol3_wrsp: cross cp_ucblock_etlink_req, cp_ucblock_tol3_b;
      ccp_etlinkreq_tol3_rrsp: cross cp_ucblock_etlink_req, cp_ucblock_tol3_r;
      
      ccp_etlinkreq_tosys_wrsp: cross cp_ucblock_etlink_req, cp_ucblock_tosys_b;
      ccp_etlinkreq_tosys_rrsp: cross cp_ucblock_etlink_req, cp_ucblock_tosys_r;
      
   endgroup : ucblock_all_requests_cg
   
   covergroup ucblock_valid_with_no_ready_cg with function sample (bit req_valid, bit req_ready);

      option.name = "ucblock_valid_with_no_ready_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_ucblock_valid_with_no_ready : coverpoint (req_valid & !req_ready) { bins slave_not_ready = {1}; }
   endgroup : ucblock_valid_with_no_ready_cg

   covergroup ucblock_flb_req_register_cg with function sample (bit flb_req_valid, bit[12:0] flb_req_data);

      option.name = "ucblock_flb_req_register_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_ucblock_flb_req_register : coverpoint flb_req_data[4:0] iff (flb_req_valid)
	{
	 bins flb_reg [] = {[0:$]};
      }
      
   endgroup : ucblock_flb_req_register_cg

   covergroup ucblock_fcc_regs_cg with function sample (bit [3:0] uc_fcc_esr_enables);

      option.name = "ucblock_fcc_regs_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_fcc_regs : coverpoint uc_fcc_esr_enables {`walking_one_4}
      
   endgroup : ucblock_fcc_regs_cg
   
   covergroup ucblock_opcodes_cg with function sample (bit etlink_req_valid, etlink_req_ready, bit [5:0] etlink_opcode, bit [1:0] etlink_port);
      
      option.name = "ucblock_opcodes_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_opcodes_0 : coverpoint etlink_opcode iff (etlink_req_valid & etlink_req_ready & (etlink_port == 0))
	{ 
	 bins message_req = {ET_LINK_REQ_MsgSendData};
	 bins atomic_req = {ET_LINK_REQ_Atomic};
	 bins read_req = {ET_LINK_REQ_Read};
	 bins write_req = {ET_LINK_REQ_Write};
	 bins bad[] = default;
      }
      cp_ucblock_opcodes_1 : coverpoint etlink_opcode iff (etlink_req_valid & etlink_req_ready & (etlink_port == 1))
	{ 
	 bins message_req = {ET_LINK_REQ_MsgSendData};
	 bins atomic_req = {ET_LINK_REQ_Atomic};
	 bins read_req = {ET_LINK_REQ_Read};
	 bins write_req = {ET_LINK_REQ_Write};
	 bins bad[] = default;
      }
      cp_ucblock_opcodes_2 : coverpoint etlink_opcode iff (etlink_req_valid & etlink_req_ready & (etlink_port == 2))
	{ 
	 bins message_req = {ET_LINK_REQ_MsgSendData};
	 bins atomic_req = {ET_LINK_REQ_Atomic};
	 bins read_req = {ET_LINK_REQ_Read};
	 bins write_req = {ET_LINK_REQ_Write};
	 bins bad[] = default;
      }
      cp_ucblock_opcodes_3 : coverpoint etlink_opcode iff (etlink_req_valid & etlink_req_ready & (etlink_port == 3))
	{ 
	 bins message_req = {ET_LINK_REQ_MsgSendData};
	 bins atomic_req = {ET_LINK_REQ_Atomic};
	 bins read_req = {ET_LINK_REQ_Read};
	 bins write_req = {ET_LINK_REQ_Write};
	 bins bad[] = default;
      }
      
   endgroup : ucblock_opcodes_cg

   covergroup ucblock_message_cg with function sample (bit etlink_req_valid, etlink_req_ready,  bit [5:0] etlink_opcode, bit [39:0] etlink_address);
      
      option.name = "ucblock_message_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_msg_port : coverpoint etlink_address[19:16] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_MsgSendData))
	{
	 bins port_id[] = {[0:3]};
	 bins bad[] = default;
      }
      cp_ucblock_msg_hart : coverpoint etlink_address[15:12] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_MsgSendData))
	{
	 bins hart_id[] = {[0:15]};
	 bins bad[] = default;
      }
      cp_ucblock_msg_shire : coverpoint etlink_address[29:22] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_MsgSendData))
	{
	 bins shire_id[] = {[0:31]};
	 //bins shire_self = {8'hff};
	 bins bad[] = default;
	 }
      
   endgroup : ucblock_message_cg
   
   covergroup ucblock_atomic_cg with function sample (bit etlink_req_valid, etlink_req_ready, bit [5:0] etlink_opcode, bit [6:0] etlink_subopcode, bit [39:0] etlink_address);
      
      option.name = "ucblock_atomic_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_atomic : coverpoint etlink_address[10:6] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins shire_id[] = {[0:31]};
      }
      cp_ucblock_atomic_opcode_scalar : coverpoint etlink_subopcode[3:0] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins opcode[] = {[0:11]};
	 bins bad[] = default;
      }
      cp_ucblock_atomic_opcode_vector : coverpoint etlink_subopcode[3:0] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins opcode[] = {[0:10]};
	 bins bad[] = default;
      }
      cp_ucblock_atomic_type_scalar : coverpoint etlink_subopcode[4] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins optype = {0};
      }
      cp_ucblock_atomic_type_vector : coverpoint etlink_subopcode[4] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins optype = {1};
      }
      cp_ucblock_atomic_size : coverpoint etlink_subopcode[5] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins opsize[] = {[0:1]};
      }
      cp_ucblock_atomic_dest : coverpoint etlink_subopcode[6] iff ((etlink_req_valid & etlink_req_ready) & (etlink_opcode == ET_LINK_REQ_Atomic))
	{
	 bins opdest[] = {[0:1]};
      }
      ccp_atomic_op_scaler: cross cp_ucblock_atomic_opcode_scalar, cp_ucblock_atomic_type_scalar, cp_ucblock_atomic_size, cp_ucblock_atomic_dest;
      ccp_atomic_op_vector: cross cp_ucblock_atomic_opcode_vector, cp_ucblock_atomic_type_vector, cp_ucblock_atomic_dest;

   endgroup : ucblock_atomic_cg
   
   covergroup ucblock_broadcast_cg with function sample (bit etlink_req_valid, etlink_req_ready, bit [5:0] etlink_opcode, bit [39:0] address, bit [39:0] req_data);
      
      option.name = "ucblock_broadcast_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_bc_shire : coverpoint req_data[31:0] iff ((address inside {40'h013FF5FFF8, 40'h017FF5FFF8, 40'h01FFF5FFF8, 40'h01BFF5FFF8}) &
	 etlink_req_valid & etlink_req_ready) {`walking_one_32}
								 
      cp_ucblock_bc_mem_0 : coverpoint req_data[35:32] iff ((address inside {40'h013FF5FFF8, 40'h017FF5FFF8, 40'h01FFF5FFF8, 40'h01BFF5FFF8}) &
	 etlink_req_valid & etlink_req_ready) {`walking_one_4} 
      
      cp_ucblock_bc_mem_1 : coverpoint req_data[39:36] iff ((address inside {40'h013FF5FFF8, 40'h017FF5FFF8, 40'h01FFF5FFF8, 40'h01BFF5FFF8}) &
	 etlink_req_valid & etlink_req_ready) {`walking_one_4} 

   endgroup : ucblock_broadcast_cg
   
   covergroup ucblock_fifo_full_cg with function sample (bit fifo_full, bit push, bit pop);
      
      option.name = "ucblock_fifo_full_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_fifo_full : coverpoint fifo_full
	{ bins hit = {1}; }
      cp_ucblock_push_pop : coverpoint (push & pop)
	{ bins hit = {1}; }
      cp_ucblock_push_pop_fifo_full : coverpoint fifo_full iff (push & pop)
	{ bins hit = {1}; }
      
   endgroup : ucblock_fifo_full_cg
   
   covergroup ucblock_req_size_cg with function sample (bit etlink_req_valid, etlink_req_ready, bit [2:0] etlink_size);
      
      option.name = "ucblock_req_size_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_req_size : coverpoint etlink_size iff (etlink_req_valid & etlink_req_ready)
	{
	 bins req_size[] = {[0:6]};
	 bins bad[] = default;
      }
      
   endgroup : ucblock_req_size_cg
     
   covergroup ucblock_axi_id_cg with function sample (bit valid, bit ready, bit [8:0] axi_id);
	
      option.name = "ucblock_axi_id_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_axi_id : coverpoint axi_id iff (valid & ready)
	{
	 bins axi_id[] = {[0:511]};
	 bins bad[] = default;
      }
      
   endgroup : ucblock_axi_id_cg
       
    /* FUTURE
   covergroup ucblock_opcode_reqbuf_full_cg with function sample (bit [5:0] etlink_opcode, bit [31:0] req_buffer_valid);
      
      option.name = "ucblock_opcode_reqbuf_full_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      cp_ucblock_opcode_reqbuf_full : coverpoint req_buffer_valid iff (etlink_opcode inside {ET_LINK_REQ_Atomic})
	{
	 bins axi_id[] = {[0:512]};
	 bins bad[] = default;
      }
      
   endgroup : ucblock_opcode_reqbuf_full_cg
       */
     
     covergroup ucblock_axi_vc_credits_cg with function sample (bit [3:0] aw_credit_count, bit awvalid, bit awready, bit aw_credit);
	
	option.name = "ucblock_axi_vc_credits_cg";
	option.per_instance         = 0;
	type_option.merge_instances = 1;
	
	cp_ucblock_axi_vc_credits : coverpoint awready iff ( ( aw_credit_count == 8 ) & ( awvalid & aw_credit ) )
	  {
	   bins req_accepted = {1};
	   bins bad[] = default;
	}
	
     endgroup : ucblock_axi_vc_credits_cg
       
     covergroup ucblock_sysmesh_read_write_cg with function sample (bit awvalid, bit awready, bit arvalid, bit arready, bit [1:0] aw_credit);
	
	option.name = "ucblock_sysmesh_read_write_cg";
	option.per_instance         = 0;
	type_option.merge_instances = 1;
	
	cp_ucblock_sysmesh_read_write : coverpoint aw_credit iff ( arvalid & arready & awvalid & awready )
	  {
	   bins sbm_rd_with_uc_wr  = {1};
	   bins sbm_rd_with_sbm_wr = {2};
	   bins bad[] = default;
	}
	
     endgroup : ucblock_sysmesh_read_write_cg
       
   function new();
      super.new();
      ucblock_req_buffer_valid_cg = new();
      ucblock_rsp_valid_cg = new();
      ucblock_etlink_port_id_cg = new();
      ucblock_all_requests_cg = new();
      ucblock_valid_with_no_ready_cg = new();      
      ucblock_flb_req_register_cg = new();
      ucblock_fcc_regs_cg = new();
      ucblock_opcodes_cg = new();
      ucblock_message_cg = new();
      ucblock_atomic_cg = new();      
      ucblock_broadcast_cg = new();
      ucblock_fifo_full_cg = new();   
      ucblock_req_size_cg = new();
      ucblock_axi_id_cg = new();   
      //ucblock_opcode_reqbuf_full_cg = new();
      ucblock_axi_vc_credits_cg = new();
      ucblock_sysmesh_read_write_cg = new();
   endfunction

endclass

`endif
//`endif
