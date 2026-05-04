

`ifndef __EVL_IOSHIRE_MESH_CVG
`define __EVL_IOSHIRE_MESH_CVG

class ioshire_mesh_cvg extends evl_void;

   covergroup io_mesh_parallel_flit_lyrs_in_cg with function sample (logic [7:0] flit_valid_in);

      option.name = "io_mesh_parallel_flit_lyrs_in_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_flit_lyrs_in: coverpoint flit_valid_in {
         bins others[]      = {[1:254]};
         bins all_lyrs      = {8'b11111111};
      }

   endgroup

   covergroup io_mesh_parallel_flit_lyrs_out_cg with function sample (logic [7:0] flit_valid_out);

      option.name = "io_mesh_parallel_flit_lyrs_out_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_flit_lyrs_out: coverpoint flit_valid_out {
         bins others[]      = {[1:254]};
         bins all_lyrs      = {8'b11111111};
      }

   endgroup

   covergroup io_mesh_parallel_flit_accesses_cg with function sample (logic [7:0] [1:0] lyr_valid_in, lyr_valid_out);

      option.name = "io_mesh_parallel_flit_accesses_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_lyr0_accesses_in: coverpoint lyr_valid_in[0] iff (|lyr_valid_in[0] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr1_accesses_in: coverpoint lyr_valid_in[1] iff (|lyr_valid_in[1] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr2_accesses_in: coverpoint lyr_valid_in[2] iff (|lyr_valid_in[2] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr3_accesses_in: coverpoint lyr_valid_in[3] iff (|lyr_valid_in[3] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr4_accesses_in: coverpoint lyr_valid_in[4] iff (|lyr_valid_in[4] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr5_accesses_in: coverpoint lyr_valid_in[5] iff (|lyr_valid_in[5] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr6_accesses_in: coverpoint lyr_valid_in[6] iff (|lyr_valid_in[6] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr7_accesses_in: coverpoint lyr_valid_in[7] iff (|lyr_valid_in[7] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }

      cp_lyr0_accesses_out: coverpoint lyr_valid_out[0] iff (|lyr_valid_out[0] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr1_accesses_out: coverpoint lyr_valid_out[1] iff (|lyr_valid_out[1] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr2_accesses_out: coverpoint lyr_valid_out[2] iff (|lyr_valid_out[2] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr3_accesses_out: coverpoint lyr_valid_out[3] iff (|lyr_valid_out[3] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr4_accesses_out: coverpoint lyr_valid_out[4] iff (|lyr_valid_out[4] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr5_accesses_out: coverpoint lyr_valid_out[5] iff (|lyr_valid_out[5] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr6_accesses_out: coverpoint lyr_valid_out[6] iff (|lyr_valid_out[6] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }
      cp_lyr7_accesses_out: coverpoint lyr_valid_out[7] iff (|lyr_valid_out[7] == 1'b1) {
         ignore_bins no_access = {2'b0};
      }

   endgroup

   covergroup io_mesh_axi_cg with function sample (bit tol3_req, tol3_rsp, tol3_req_rdy, tol3_rsp_rdy, tol3b_req, tol3b_rsp, tol3b_req_rdy, tol3b_rsp_rdy, 
                                                       tosys_req, tosys_rsp, tosys_req_rdy, tosys_rsp_rdy, io_pm_req, io_pm_rsp, io_pm_req_rdy, io_pm_rsp_rdy, 
                                                       io_ps_req, io_ps_rsp, io_ps_req_rdy, io_ps_rsp_rdy);

      option.name = "io_mesh_axi_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      //
      // Master Parallel Reads and Writes
      //
      cp_tol3_parallel_rw: coverpoint (tol3_req) iff (tol3_req_rdy);
      cp_tol3b_parallel_rw: coverpoint (tol3b_req) iff (tol3b_req_rdy);
      cp_tosys_parallel_rw: coverpoint (tosys_req) iff (tosys_req_rdy);
      cp_io_pm_parallel_rw: coverpoint (io_pm_req) iff (io_pm_req_rdy);
      //
      // Slave Parallel Reads and Writes
      //
      cp_io_ps_parallel_rw: coverpoint (io_ps_req) iff (io_ps_req_rdy);

      //
      // Master Parallel Responses
      //
      cp_tol3_parallel_resp: coverpoint (tol3_rsp) iff (tol3_rsp_rdy);
      cp_tol3b_parallel_resp: coverpoint (tol3b_rsp) iff (tol3b_rsp_rdy);
      cp_tosys_parallel_resp: coverpoint (tosys_rsp) iff (tosys_rsp_rdy);
      cp_io_pm_parallel_resp: coverpoint (io_pm_rsp) iff (io_pm_rsp_rdy);

      //
      // Slave Parallel Responses
      //
      cp_io_ps_parallel_resp: coverpoint (io_ps_rsp) iff (io_ps_rsp_rdy);

      //
      // All AXI Iterfaces parallel Read/Writes
      //
      cp_all_axi_parallel_rw: coverpoint {tol3_req, tol3b_req, tosys_req, io_pm_req, io_ps_req} iff (tol3_req_rdy || tol3b_req_rdy || tosys_req_rdy || io_pm_req_rdy || io_ps_req_rdy);

      //
      // All AXI Iterfaces parallel Responses
      //
      cp_all_axi_parallel_resp: coverpoint {tol3_rsp, tol3b_rsp, tosys_rsp, io_pm_rsp, io_ps_rsp} iff (tol3_rsp_rdy || tol3b_rsp_rdy || tosys_rsp_rdy || io_pm_rsp_rdy || io_ps_rsp_rdy);

      //
      // Master Parallel Traffic
      //
      ccp_tol3_parallel_traffic: cross cp_tol3_parallel_rw, cp_tol3_parallel_resp;
      ccp_tol3b_parallel_traffic: cross cp_tol3b_parallel_rw, cp_tol3b_parallel_resp;

      ccp_tosys_parallel_traffic: cross cp_tosys_parallel_rw, cp_tosys_parallel_resp;
      ccp_io_pm_parallel_traffic: cross cp_io_pm_parallel_rw, cp_io_pm_parallel_resp;

      //
      // Slave Parallel Traffic
      //
      ccp_io_ps_parallel_traffic: cross cp_io_ps_parallel_rw, cp_io_ps_parallel_resp;

      //
      // All AXI Interfaces Parallel Traffic
      //
      ccp_all_axi_parallel_traffic: cross cp_all_axi_parallel_rw, cp_all_axi_parallel_resp;

   endgroup

   function void sample_axi (bit tol3_awvalid, tol3_awready, tol3_wvalid, tol3_wready, tol3_bvalid, tol3_bready,
                                 tol3_arvalid, tol3_arready, tol3_rvalid, tol3_rready,
                                 tol3b_awvalid, tol3b_awready, tol3b_wvalid, tol3b_wready, tol3b_bvalid, tol3b_bready,
                                 tol3b_arvalid, tol3b_arready, tol3b_rvalid, tol3b_rready,
                                 tosys_awvalid, tosys_awready, tosys_wvalid, tosys_wready, tosys_bvalid, tosys_bready,
                                 tosys_arvalid, tosys_arready, tosys_rvalid, tosys_rready, 
                                 io_pm_awvalid, io_pm_awready, io_pm_wvalid, io_pm_wready, io_pm_bvalid, io_pm_bready,
                                 io_pm_arvalid, io_pm_arready, io_pm_rvalid, io_pm_rready,
                                 io_ps_awvalid, io_ps_awready, io_ps_wvalid, io_ps_wready, io_ps_bvalid, io_ps_bready,
                                 io_ps_arvalid, io_ps_arready, io_ps_rvalid, io_ps_rready);

      bit tol3_req, tol3b_req, tosys_req, io_pm_req, io_ps_req;
      bit tol3_rsp, tol3b_rsp, tosys_rsp, io_pm_rsp, io_ps_rsp;

      bit tol3_req_rdy, tol3b_req_rdy, tosys_req_rdy, io_pm_req_rdy, io_ps_req_rdy;
      bit tol3_rsp_rdy, tol3b_rsp_rdy, tosys_rsp_rdy, io_pm_rsp_rdy, io_ps_rsp_rdy;

      tol3_req       = tol3_awvalid & tol3_arvalid & tol3_wvalid;
      tol3_rsp       = tol3_rvalid & tol3_bvalid;
      tol3_req_rdy   = tol3_awready & tol3_arready & tol3_wready;
      tol3_rsp_rdy   = tol3_rready & tol3_bready;

      tol3b_req       = tol3b_awvalid & tol3b_arvalid & tol3b_wvalid;
      tol3b_rsp       = tol3b_rvalid & tol3b_bvalid;
      tol3b_req_rdy   = tol3b_awready & tol3b_arready & tol3b_wready;
      tol3b_rsp_rdy   = tol3b_rready & tol3b_bready;

      tosys_req       = tosys_awvalid & tosys_arvalid & tosys_wvalid;
      tosys_rsp       = tosys_rvalid & tosys_bvalid;
      tosys_req_rdy   = tosys_awready & tosys_arready & tosys_wready;
      tosys_rsp_rdy   = tosys_rready & tosys_bready;

      io_pm_req       = io_pm_awvalid & io_pm_arvalid & io_pm_wvalid;
      io_pm_rsp       = io_pm_rvalid & io_pm_bvalid;
      io_pm_req_rdy   = io_pm_awready & io_pm_arready & io_pm_wready;
      io_pm_rsp_rdy   = io_pm_rready & io_pm_bready;

      io_ps_req       = io_ps_awvalid & io_ps_arvalid & io_ps_wvalid;
      io_ps_rsp       = io_ps_rvalid & io_ps_bvalid;
      io_ps_req_rdy   = io_ps_awready & io_ps_arready & io_ps_wready;
      io_ps_rsp_rdy   = io_ps_rready & io_ps_bready;

      io_mesh_axi_cg.sample(tol3_req, tol3_rsp, tol3_req_rdy, tol3_rsp_rdy, tol3b_req, tol3b_rsp, tol3b_req_rdy, tol3b_rsp_rdy, 
                            tosys_req, tosys_rsp, tosys_req_rdy, tosys_rsp_rdy, io_pm_req, io_pm_rsp, io_pm_req_rdy, io_pm_rsp_rdy, 
                            io_ps_req, io_ps_rsp, io_ps_req_rdy, io_ps_rsp_rdy);

   endfunction

   function void sample_flits(input logic [7:0] n_valid_in, e_valid_in, s_valid_out, w_valid_out);
      logic [7:0] [1:0] lyr_valid_in, lyr_valid_out;

      foreach(n_valid_in[ii]) begin
         lyr_valid_in[ii] = '{n_valid_in[ii], e_valid_in[ii]};
         lyr_valid_out[ii] = '{s_valid_out[ii], w_valid_out[ii]};
      end

      //if (|n_valid_in) begin
      //   io_mesh_parallel_flit_lyrs_in_cg.sample(n_valid_in);
      //end
      //if (|e_valid_in) begin
      //   io_mesh_parallel_flit_lyrs_in_cg.sample(e_valid_in);
      //end
      //if (|s_valid_out) begin
      //   io_mesh_parallel_flit_lyrs_out_cg.sample(s_valid_out);
      //end
      //if (|w_valid_out) begin
      //   io_mesh_parallel_flit_lyrs_out_cg.sample(w_valid_out);
      //end

      if ((|n_valid_in) || (|e_valid_in)) begin
         io_mesh_parallel_flit_lyrs_in_cg.sample(n_valid_in | e_valid_in);
      end
      if ((|s_valid_out) || (|w_valid_out)) begin
         io_mesh_parallel_flit_lyrs_out_cg.sample(s_valid_out | w_valid_out);
      end

      io_mesh_parallel_flit_accesses_cg.sample(lyr_valid_in, lyr_valid_out);


   endfunction


   function new();
      super.new();

      io_mesh_parallel_flit_lyrs_in_cg = new;
      io_mesh_parallel_flit_lyrs_out_cg = new;
      io_mesh_parallel_flit_accesses_cg = new;
      io_mesh_axi_cg = new;

   endfunction

endclass


`endif
