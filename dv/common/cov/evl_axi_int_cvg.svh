`ifndef __EVL_AXI_INT_CVG
`define __EVL_AXI_INT_CVG


   covergroup evl_axi_cg () @(posedge clk);

      option.name = "evl_axi_cg";
      option.per_instance         = 0;
      type_option.merge_instances = 1;


//not used      aw_len_cvg: coverpoint aw_len iff ((aw_valid == 1) && (aw_ready == 1));

//not used      ar_len_cvg: coverpoint ar_len iff ((ar_valid == 1) && (ar_ready == 1));


      aw_size_cvg: coverpoint aw_size iff ((aw_valid == 1) && (aw_ready == 1)) {

         bins B1_8b      = {3'b000};
         bins B2_16b     = {3'b001};
         bins B4_32b     = {3'b010};
         bins B8_64b     = {3'b011};
         bins B16_128b   = {3'b100};
         bins B32_256b   = {3'b101};
         bins B64_512b   = {3'b110};
      }

      ar_size_cvg: coverpoint ar_size iff ((ar_valid == 1) && (ar_ready == 1)) {

         bins B1_8b      = {3'b000};
         bins B2_16b     = {3'b001};
         bins B4_32b     = {3'b010};
         bins B8_64b     = {3'b011};
         bins B16_128b   = {3'b100};
         bins B32_256b   = {3'b101};
         bins B64_512b   = {3'b110};
      }


//not used      aw_burst_cvg: coverpoint aw_burst iff ((aw_valid == 1) && (aw_ready == 1)) {

//         bins FIXED    = {2'b00};
//         bins INCR     = {2'b01};
//         bins WRAP     = {2'b10};
//         bins Reserved = {2'b11};
//      }

//not used      ar_burst_cvg: coverpoint ar_burst iff ((ar_valid == 1) && (ar_ready == 1)) {

//         bins FIXED    = {2'b00};
//         bins INCR     = {2'b01};
//         bins WRAP     = {2'b10};
//         bins Reserved = {2'b11};
//      }


//not used      aw_lock_cvg: coverpoint aw_lock iff ((aw_valid == 1) && (aw_ready == 1)) {

//         bins Normal_access    = {1'b0};
//         bins Exclusive_access = {1'b1};
//      }


//not used      ar_lock_cvg: coverpoint ar_lock iff ((ar_valid == 1) && (ar_ready == 1)) {

//         bins Normal_access    = {1'b0};
//         bins Exclusive_access = {1'b1};
//      }


//not used      aw_cache_cvg: coverpoint aw_cache iff ((aw_valid == 1) && (aw_ready == 1)) {

//         bins device_non_bufferable             = {4'b0000};
//         bins device_bufferable                 = {4'b0001};
//         bins normal_non_bufferable             = {4'b0010};
//         bins normal_bufferable                 = {4'b0011};
//         bins write_through_no_allocate         = {4'b0110};
//         bins write_through_write_allocate      = {4'b1110};
//         bins write_through_write_allocate_axi3 = {4'b1010};
//         bins write_back_no_allocate            = {4'b0111};
//         bins write_back_write_allocate         = {4'b1111};
//         bins write_back_write_allocate_axi3    = {4'b1011};
//      }

//not used      ar_cache_cvg: coverpoint ar_cache iff ((ar_valid == 1) && (ar_ready == 1)) {

//         bins device_non_bufferable            = {4'b0000};
//         bins device_bufferable                = {4'b0001};
//         bins normal_non_bufferable            = {4'b0010};
//         bins normal_bufferable                = {4'b0011};
//         bins write_through_no_allocate        = {4'b1010};
//         bins write_through_read_allocate      = {4'b1110};
//         bins write_through_read_allocate_axi3 = {4'b0110};
//         bins write_back_no_allocate           = {4'b1011};
//         bins write_back_read_allocate         = {4'b1111};
//         bins write_back_read_allocate_axi3    = {4'b0111};
//      }


//not used      aw_prot_cvg: coverpoint aw_prot iff ((aw_valid == 1) && (aw_ready == 1)) {

//         wildcard bins unprivileged_access = {3'b??0};
//         wildcard bins privileged_access   = {3'b??1};
//         wildcard bins secure_access       = {3'b?0?};
//         wildcard bins non_secure_access   = {3'b?1?};
//         wildcard bins data_access         = {3'b0??};
//         wildcard bins instruction_access  = {3'b1??};
//      }

//not used      ar_prot_cvg: coverpoint ar_prot iff ((ar_valid == 1) && (ar_ready == 1)) {

//         wildcard bins unprivileged_access = {3'b??0};
//         wildcard bins privileged_access   = {3'b??1};
//         wildcard bins secure_access       = {3'b?0?};
//         wildcard bins non_secure_access   = {3'b?1?};
//         wildcard bins data_access         = {3'b0??};
//         wildcard bins instruction_access  = {3'b1??};
//      }


      w_strb_cvg: coverpoint w_strb iff ((w_valid == 1) && (w_ready == 1));


      b_resp_cvg: coverpoint b_resp iff ((b_valid == 1) && (b_ready == 1)) {

         bins OKAY   = {2'b00};
         bins EXOKAY = {2'b01};
         bins SLVERR = {2'b10};
         bins DECERR = {2'b11};
      }

      r_resp_cvg: coverpoint r_resp iff ((r_valid == 1) && (r_ready == 1)) {

         bins OKAY   = {2'b00};
         bins EXOKAY = {2'b01};
         bins SLVERR = {2'b10};
         bins DECERR = {2'b11};
      }


   endgroup

   evl_axi_cg evl_axi_cg_inst = new();






`endif
