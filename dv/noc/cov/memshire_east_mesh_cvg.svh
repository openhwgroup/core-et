
`ifndef __EVL_MEMSHIRE_EAST_MESH_CVG
`define __EVL_MEMSHIRE_EAST_MESH_CVG

class memshire_east_mesh_cvg extends evl_void;

   covergroup memshire_east_mesh_parallel_flit_lyrs_in_cg with function sample (logic [7:0] flit_valid_in);

      option.name = "memshire_east_mesh_parallel_flit_lyrs_in_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_flit_lyrs_in: coverpoint flit_valid_in {
         bins lyrs_2        = {8'b00000100};
         bins lyrs_3        = {8'b00001000};
         bins lyrs_2_3      = {8'b00001100};
         bins lyrs_4        = {8'b00010000};
         bins lyrs_4_2      = {8'b00010100};
         bins lyrs_4_3      = {8'b00011000};
         bins lyrs_4_3_2    = {8'b00011100};
         bins lyrs_5        = {8'b00100000};
         bins lyrs_5_2      = {8'b00100100};
         bins lyrs_5_3      = {8'b00101000};
         bins lyrs_5_3_2    = {8'b00101100};
         bins lyrs_5_4      = {8'b00110000};
         bins lyrs_5_4_2    = {8'b00110100};
         bins lyrs_5_4_3    = {8'b00111000};
         bins lyrs_5_4_3_2  = {8'b00111100};
      }

   endgroup

   covergroup memshire_east_mesh_parallel_flit_lyrs_out_cg with function sample (logic [7:0] flit_valid_out);

      option.name = "memshire_east_mesh_parallel_flit_lyrs_out_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_flit_lyrs_out: coverpoint flit_valid_out {
         bins lyrs_2        = {8'b00000100};
         bins lyrs_6        = {8'b01000000};
         bins lyrs_6_2      = {8'b01000100};
         bins lyrs_7        = {8'b10000000};
         bins lyrs_7_2      = {8'b10000100};
         bins lyrs_7_6      = {8'b11000000};
         bins lyrs_7_6_2    = {8'b11000100};
      }

   endgroup

   covergroup memshire_east_mesh_axi_cg with function sample (bit atomic_rsp_req, atomic_rsp_rsp, atomic_rsp_req_rdy, atomic_rsp_rsp_rdy, mem_mesh_req, mem_mesh_rsp, mem_mesh_req_rdy, mem_mesh_rsp_rdy, 
                                                                  sbm_mesh_req, sbm_mesh_rsp, sbm_mesh_req_rdy, sbm_mesh_rsp_rdy);

      option.name = "memshire_east_mesh_axi_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      //
      // Master Parallel Reads and Writes
      //
      cp_atomic_rsp_parallel_rw: coverpoint (atomic_rsp_req) iff (atomic_rsp_req_rdy);

      //
      // Slave Parallel Reads and Writes
      //
      cp_mem_mesh_parallel_rw: coverpoint (mem_mesh_req) iff (mem_mesh_req_rdy);
      cp_sbm_mesh_parallel_rw: coverpoint (sbm_mesh_req) iff (sbm_mesh_req_rdy);

      //
      // Master Parallel Responses
      //
      cp_atomic_rsp_parallel_resp: coverpoint (atomic_rsp_rsp) iff (atomic_rsp_rsp_rdy);

      //
      // Slave Parallel Responses
      //
      cp_mem_mesh_parallel_resp: coverpoint (mem_mesh_rsp) iff (mem_mesh_rsp_rdy);
      cp_sbm_mesh_parallel_resp: coverpoint (sbm_mesh_rsp) iff (sbm_mesh_rsp_rdy);

      //
      // All AXI Iterfaces parallel Read/Writes
      //
      cp_all_axi_parallel_rw: coverpoint {atomic_rsp_req, mem_mesh_req, sbm_mesh_req} iff (atomic_rsp_req_rdy || mem_mesh_req_rdy || sbm_mesh_req_rdy);

      //
      // All AXI Iterfaces parallel Responses
      //
      cp_all_axi_parallel_resp: coverpoint {atomic_rsp_rsp, mem_mesh_rsp, sbm_mesh_rsp} iff (atomic_rsp_rsp_rdy || mem_mesh_rsp_rdy || sbm_mesh_rsp_rdy);

      //
      // Master Parallel Traffic
      //
      ccp_atomic_rsp_parallel_traffic: cross cp_atomic_rsp_parallel_rw, cp_atomic_rsp_parallel_resp;

      //
      // Slave Parallel Traffic
      //
      ccp_mem_mesh_parallel_traffic: cross cp_mem_mesh_parallel_rw, cp_mem_mesh_parallel_resp;
      ccp_sbm_mesh_parallel_traffic: cross cp_sbm_mesh_parallel_rw, cp_sbm_mesh_parallel_resp;

      //
      // All AXI Interfaces Parallel Traffic
      //
      ccp_all_axi_parallel_traffic: cross cp_all_axi_parallel_rw, cp_all_axi_parallel_resp;

   endgroup

   function void sample_axi (bit atomic_rsp_awvalid, atomic_rsp_awready, atomic_rsp_wvalid, atomic_rsp_wready, atomic_rsp_bvalid, atomic_rsp_bready,
                                 atomic_rsp_arvalid, atomic_rsp_arready, atomic_rsp_rvalid, atomic_rsp_rready,
                                 mem_mesh_awvalid, mem_mesh_awready, mem_mesh_wvalid, mem_mesh_wready, mem_mesh_bvalid, mem_mesh_bready,
                                 mem_mesh_arvalid, mem_mesh_arready, mem_mesh_rvalid, mem_mesh_rready,
                                 sbm_mesh_awvalid, sbm_mesh_awready, sbm_mesh_wvalid, sbm_mesh_wready, sbm_mesh_bvalid, sbm_mesh_bready,
                                 sbm_mesh_arvalid, sbm_mesh_arready, sbm_mesh_rvalid, sbm_mesh_rready);

      bit atomic_rsp_req, mem_mesh_req, sbm_mesh_req;
      bit atomic_rsp_rsp, mem_mesh_rsp, sbm_mesh_rsp;

      bit atomic_rsp_req_rdy, mem_mesh_req_rdy, sbm_mesh_req_rdy;
      bit atomic_rsp_rsp_rdy, mem_mesh_rsp_rdy, sbm_mesh_rsp_rdy;

      atomic_rsp_req       = atomic_rsp_awvalid & atomic_rsp_arvalid & atomic_rsp_wvalid;
      atomic_rsp_rsp       = atomic_rsp_rvalid & atomic_rsp_bvalid;
      atomic_rsp_req_rdy   = atomic_rsp_awready & atomic_rsp_arready & atomic_rsp_wready;
      atomic_rsp_rsp_rdy   = atomic_rsp_rready & atomic_rsp_bready;

      mem_mesh_req       = mem_mesh_awvalid & mem_mesh_arvalid & mem_mesh_wvalid;
      mem_mesh_rsp       = mem_mesh_rvalid & mem_mesh_bvalid;
      mem_mesh_req_rdy   = mem_mesh_awready & mem_mesh_arready & mem_mesh_wready;
      mem_mesh_rsp_rdy   = mem_mesh_rready & mem_mesh_bready;

      sbm_mesh_req       = sbm_mesh_awvalid & sbm_mesh_arvalid & sbm_mesh_wvalid;
      sbm_mesh_rsp       = sbm_mesh_rvalid & sbm_mesh_bvalid;
      sbm_mesh_req_rdy   = sbm_mesh_awready & sbm_mesh_arready & sbm_mesh_wready;
      sbm_mesh_rsp_rdy   = sbm_mesh_rready & sbm_mesh_bready;

      memshire_east_mesh_axi_cg.sample(atomic_rsp_req, atomic_rsp_rsp, atomic_rsp_req_rdy, atomic_rsp_rsp_rdy, mem_mesh_req, mem_mesh_rsp, mem_mesh_req_rdy, mem_mesh_rsp_rdy, 
                                       sbm_mesh_req, sbm_mesh_rsp, sbm_mesh_req_rdy, sbm_mesh_rsp_rdy);

   endfunction

   function new();
      super.new();

      memshire_east_mesh_parallel_flit_lyrs_in_cg = new;
      memshire_east_mesh_parallel_flit_lyrs_out_cg = new;
      memshire_east_mesh_axi_cg = new;

   endfunction

endclass


`endif
