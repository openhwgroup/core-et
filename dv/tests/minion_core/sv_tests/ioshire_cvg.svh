
class ioshire_cvg extends evl_void;

   covergroup ioshire_noc_clock_freq_cg with function sample (int noc_clock_freq, int intf_num, int rw); 
      
      option.name = "noc_clock_freq";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      ccp_noc_clock_freq : coverpoint noc_clock_freq {
         bins f375MHz = {[370:380]};
         bins f400MHz = {[395:405]};
         bins f500MHz = {[495:505]};
         bins others = default;
      }

      ccp_noc_intf_num : coverpoint intf_num {
         bins ioshire_slave = {0};
         bins ioshire_esr_master = {1};
         bins ioshire_tosys_master = {2};
         bins ioshire_tol3_master = {3};
         bins ioshire_tol3b_master = {4};
         bins spio_sp_main0_main_noc_rbm = {5};
         bins others = default;
      }

      ccp_rw : coverpoint rw {
         bins RD_op = {0};
         bins WR_op = {1};
         bins others = default;
      }

      ccp_noc_clock_freq_x_noc_intf_num_x_rw: cross ccp_noc_clock_freq, ccp_noc_intf_num, rw;

   endgroup : ioshire_noc_clock_freq_cg

   covergroup ioshire_etl2axi_backpressure_cg with function sample (bit aw_backpressured, bit w_backpressured, bit ar_backpressured, bit etl_rsp_backpressured, int q0_wr_cnt, int q0_rd_cnt, bit q0_wr_cnt_overflow, int wr_stream_length, int rd_stream_length, int different_addresses_in_wr_stream, int different_addresses_in_rd_stream, bit wr_out_of_order_completion, bit rd_out_of_order_completion); 
      
      option.name = "etl2axi_backpressure";
      option.per_instance         = 0;
      type_option.merge_instances = 1;
      
      ccp_aw_backpressured : coverpoint aw_backpressured {
         bins aw_backpressured = {1};
         bins others = default;
      }

      ccp_w_backpressured : coverpoint w_backpressured {
         bins w_backpressured = {1};
         bins others = default;
      }

      ccp_ar_backpressured : coverpoint ar_backpressured {
         bins ar_backpressured = {1};
         bins others = default;
      }

      ccp_etl_rsp_backpressured : coverpoint etl_rsp_backpressured {
         bins etl_rsp_backpressured = {1};
         bins others = default;
      }

      ccp_q0_wr_cnt : coverpoint q0_wr_cnt {
         bins q0_wr_cnt[] = {[0:16]};
         bins others = default;
      }

      ccp_q0_rd_cnt : coverpoint q0_rd_cnt {
         bins q0_rd_cnt[] = {[0:3]};
         bins others = default;
      }

      ccp_q0_wr_cnt_overflow : coverpoint q0_wr_cnt_overflow {
         bins q0_wr_cnt_overflow = {1};
         bins others = default;
      }

      ccp_wr_stream_length : coverpoint wr_stream_length {
         bins wr_stream_length_low = {[5:10]};
         bins wr_stream_length_mid = {[11:25]};
         bins wr_stream_length_high = {[26:32]};
         bins others = default;
      }

      ccp_rd_stream_length : coverpoint rd_stream_length {
         bins rd_stream_length_low = {[5:10]};
         bins rd_stream_length_mid = {[11:25]};
         bins rd_stream_length_high = {[26:32]};
         bins others = default;
      }

      ccp_different_addresses_in_wr_stream : coverpoint different_addresses_in_wr_stream {
         bins different_addresses_in_wr_stream[] = {[0:3]};
         bins others = default;
      }

      ccp_different_addresses_in_rd_stream : coverpoint different_addresses_in_rd_stream {
         bins different_addresses_in_rd_stream[] = {[0:3]};
         bins others = default;
      }

      ccp_wr_out_of_order_completion : coverpoint wr_out_of_order_completion {
         bins wr_out_of_order_completion = {1};
         bins others = default;
      }

      ccp_rd_out_of_order_completion : coverpoint rd_out_of_order_completion {
         bins rd_out_of_order_completion = {1};
         bins others = default;
      }

   endgroup : ioshire_etl2axi_backpressure_cg
       
   function new();
      super.new();
      ioshire_noc_clock_freq_cg = new();
      ioshire_etl2axi_backpressure_cg = new();
   endfunction

endclass
