

`ifndef __EVL_PSHIRE_MESH_CVG
`define __EVL_PSHIRE_MESH_CVG

class pshire_mesh_cvg extends evl_void;

   covergroup pshire_mesh_parallel_flit_lyrs_in_cg with function sample (logic [7:0] flit_valid_in);

      option.name = "pshire_mesh_parallel_flit_lyrs_in_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_flit_lyrs_in: coverpoint flit_valid_in {
         bins others[]      = {[1:254]};
         bins all_lyrs      = {8'b11111111};
      }

   endgroup

   covergroup pshire_mesh_parallel_flit_lyrs_out_cg with function sample (logic [7:0] flit_valid_out);

      option.name = "pshire_mesh_parallel_flit_lyrs_out_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_flit_lyrs_out: coverpoint flit_valid_out {
         bins others[]      = {[1:254]};
         bins all_lyrs      = {8'b11111111};
      }

   endgroup

   covergroup pshire_mesh_parallel_flit_accesses_cg with function sample (logic [7:0] [2:0] lyr_valid_in, lyr_valid_out);

      option.name = "pshire_mesh_parallel_flit_accesses_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_lyr0_accesses_in: coverpoint lyr_valid_in[0] iff (|lyr_valid_in[0] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr1_accesses_in: coverpoint lyr_valid_in[1] iff (|lyr_valid_in[1] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr2_accesses_in: coverpoint lyr_valid_in[2] iff (|lyr_valid_in[2] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr3_accesses_in: coverpoint lyr_valid_in[3] iff (|lyr_valid_in[3] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr4_accesses_in: coverpoint lyr_valid_in[4] iff (|lyr_valid_in[4] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr5_accesses_in: coverpoint lyr_valid_in[5] iff (|lyr_valid_in[5] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr6_accesses_in: coverpoint lyr_valid_in[6] iff (|lyr_valid_in[6] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr7_accesses_in: coverpoint lyr_valid_in[7] iff (|lyr_valid_in[7] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }

      cp_lyr0_accesses_out: coverpoint lyr_valid_out[0] iff (|lyr_valid_out[0] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr1_accesses_out: coverpoint lyr_valid_out[1] iff (|lyr_valid_out[1] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr2_accesses_out: coverpoint lyr_valid_out[2] iff (|lyr_valid_out[2] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr3_accesses_out: coverpoint lyr_valid_out[3] iff (|lyr_valid_out[3] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr4_accesses_out: coverpoint lyr_valid_out[4] iff (|lyr_valid_out[4] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr5_accesses_out: coverpoint lyr_valid_out[5] iff (|lyr_valid_out[5] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr6_accesses_out: coverpoint lyr_valid_out[6] iff (|lyr_valid_out[6] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }
      cp_lyr7_accesses_out: coverpoint lyr_valid_out[7] iff (|lyr_valid_out[7] == 1'b1) {
         ignore_bins no_access = {3'b0};
      }

   endgroup

   covergroup pshire_mesh_axi_cg with function sample (bit tol3_req, tol3_rsp, tol3_req_rdy, tol3_rsp_rdy, tosys_req, tosys_rsp, tosys_req_rdy, tosys_rsp_rdy, 
                                                           pshire_ps_req, pshire_ps_rsp, pshire_ps_req_rdy, pshire_ps_rsp_rdy);

      option.name = "pshire_mesh_axi_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      //
      // Master Parallel Reads and Writes
      //
      cp_tol3_parallel_rw: coverpoint (tol3_req) iff (tol3_req_rdy);
      cp_tosys_parallel_rw: coverpoint (tosys_req) iff (tosys_req_rdy);

      //
      // Slave Parallel Reads and Writes
      //
      cp_pshire_ps_parallel_rw: coverpoint (pshire_ps_req) iff (pshire_ps_req_rdy);

      //
      // Master Parallel Responses
      //
      cp_tol3_parallel_resp: coverpoint (tol3_rsp) iff (tol3_rsp_rdy);
      cp_tosys_parallel_resp: coverpoint (tosys_rsp) iff (tosys_rsp_rdy);

      //
      // Slave Parallel Responses
      //
      cp_pshire_ps_parallel_resp: coverpoint (pshire_ps_rsp) iff (pshire_ps_rsp_rdy);

      //
      // All AXI Iterfaces parallel Read/Writes
      //
      cp_all_axi_parallel_rw: coverpoint {tol3_req, tosys_req, pshire_ps_req} iff (tol3_req_rdy || tosys_req_rdy || pshire_ps_req_rdy);

      //
      // All AXI Iterfaces parallel Responses
      //
      cp_all_axi_parallel_resp: coverpoint {tol3_rsp, tosys_rsp, pshire_ps_rsp} iff (tol3_rsp_rdy || tosys_rsp_rdy || pshire_ps_rsp_rdy);

      //
      // Master Parallel Traffic
      //
      ccp_tol3_parallel_traffic: cross cp_tol3_parallel_rw, cp_tol3_parallel_resp;
      ccp_tosys_parallel_traffic: cross cp_tosys_parallel_rw, cp_tosys_parallel_resp;

      //
      // Slave Parallel Traffic
      //
      ccp_pshire_ps_parallel_traffic: cross cp_pshire_ps_parallel_rw, cp_pshire_ps_parallel_resp;

      //
      // All AXI Interfaces Parallel Traffic
      //
      ccp_all_axi_parallel_traffic: cross cp_all_axi_parallel_rw, cp_all_axi_parallel_resp;

   endgroup

   function void sample_axi (bit tol3_awvalid, tol3_awready, tol3_wvalid, tol3_wready, tol3_bvalid, tol3_bready,
                                 tol3_arvalid, tol3_arready, tol3_rvalid, tol3_rready,
                                 tosys_awvalid, tosys_awready, tosys_wvalid, tosys_wready, tosys_bvalid, tosys_bready,
                                 tosys_arvalid, tosys_arready, tosys_rvalid, tosys_rready, 
                                 pshire_ps_awvalid, pshire_ps_awready, pshire_ps_wvalid, pshire_ps_wready, pshire_ps_bvalid, pshire_ps_bready,
                                 pshire_ps_arvalid, pshire_ps_arready, pshire_ps_rvalid, pshire_ps_rready);

      bit tol3_req, tosys_req, pshire_ps_req;
      bit tol3_rsp, tosys_rsp, pshire_ps_rsp;

      bit tol3_req_rdy, tosys_req_rdy, pshire_ps_req_rdy;
      bit tol3_rsp_rdy, tosys_rsp_rdy, pshire_ps_rsp_rdy;

      tol3_req       = tol3_awvalid & tol3_arvalid & tol3_wvalid;
      tol3_rsp       = tol3_rvalid & tol3_bvalid;
      tol3_req_rdy   = tol3_awready & tol3_arready & tol3_wready;
      tol3_rsp_rdy   = tol3_rready & tol3_bready;

      tosys_req       = tosys_awvalid & tosys_arvalid & tosys_wvalid;
      tosys_rsp       = tosys_rvalid & tosys_bvalid;
      tosys_req_rdy   = tosys_awready & tosys_arready & tosys_wready;
      tosys_rsp_rdy   = tosys_rready & tosys_bready;

      pshire_ps_req       = pshire_ps_awvalid & pshire_ps_arvalid & pshire_ps_wvalid;
      pshire_ps_rsp       = pshire_ps_rvalid & pshire_ps_bvalid;
      pshire_ps_req_rdy   = pshire_ps_awready & pshire_ps_arready & pshire_ps_wready;
      pshire_ps_rsp_rdy   = pshire_ps_rready & pshire_ps_bready;

      pshire_mesh_axi_cg.sample(tol3_req, tol3_rsp, tol3_req_rdy, tol3_rsp_rdy, tosys_req, tosys_rsp, tosys_req_rdy, tosys_rsp_rdy, 
                                pshire_ps_req, pshire_ps_rsp, pshire_ps_req_rdy, pshire_ps_rsp_rdy);

   endfunction

   function void sample_flits(input logic [7:0] n_valid_in, e_valid_in, e_valid_out, s_valid_out, w_valid_in, w_valid_out);
      logic [7:0] [2:0] lyr_valid_in, lyr_valid_out;

      foreach(n_valid_in[ii]) begin
         lyr_valid_in[ii] = '{n_valid_in[ii], e_valid_in[ii], w_valid_in[ii]};
         lyr_valid_out[ii] = '{e_valid_out[ii], s_valid_out[ii], w_valid_out[ii]};
      end

      //if (|n_valid_in) begin
      //   pshire_mesh_parallel_flit_lyrs_in_cg.sample(n_valid_in);
      //end
      //if (|e_valid_in) begin
      //   pshire_mesh_parallel_flit_lyrs_in_cg.sample(e_valid_in);
      //end
      //if (|w_valid_in) begin
      //   pshire_mesh_parallel_flit_lyrs_in_cg.sample(w_valid_in);
      //end
      //if (|e_valid_out) begin
      //   pshire_mesh_parallel_flit_lyrs_out_cg.sample(e_valid_out);
      //end
      //if (|s_valid_out) begin
      //   pshire_mesh_parallel_flit_lyrs_out_cg.sample(s_valid_out);
      //end
      //if (|w_valid_out) begin
      //   pshire_mesh_parallel_flit_lyrs_out_cg.sample(w_valid_out);
      //end

      if ((|n_valid_in) || (|e_valid_in) || (|w_valid_in)) begin
         pshire_mesh_parallel_flit_lyrs_in_cg.sample(n_valid_in | e_valid_in | w_valid_in);
      end
      if ((|e_valid_out) || (|s_valid_out) || (|w_valid_out)) begin
         pshire_mesh_parallel_flit_lyrs_out_cg.sample(e_valid_out | s_valid_out | w_valid_out);
      end

      pshire_mesh_parallel_flit_accesses_cg.sample(lyr_valid_in, lyr_valid_out);


   endfunction


   function new();
      super.new();

      pshire_mesh_parallel_flit_lyrs_in_cg = new;
      pshire_mesh_parallel_flit_lyrs_out_cg = new;
      pshire_mesh_parallel_flit_accesses_cg = new;
      pshire_mesh_axi_cg = new;

   endfunction

endclass


`endif
