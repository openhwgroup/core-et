`ifdef EVL_COVERAGE_ON

`ifndef __NEIGH_TXFMA_CVG
`define __NEIGH_TXFMA_CVG

package neigh_txfma_cov_pkg;

//Macros related to verification environment

`ifndef walking_one_32
`define walking_one_32  \
    wildcard bins bit_0_as_0 = {32'b???????????????????????????????0};\
    wildcard bins bit_0_as_1 = {32'b???????????????????????????????1};\
    wildcard bins bit_1_as_0 = {32'b??????????????????????????????0?};\
    wildcard bins bit_1_as_1 = {32'b??????????????????????????????1?};\
    wildcard bins bit_2_as_0 = {32'b?????????????????????????????0??};\
    wildcard bins bit_2_as_1 = {32'b?????????????????????????????1??};\
    wildcard bins bit_3_as_0 = {32'b????????????????????????????0???};\
    wildcard bins bit_3_as_1 = {32'b????????????????????????????1???};\
    wildcard bins bit_4_as_0 = {32'b???????????????????????????0????};\
    wildcard bins bit_4_as_1 = {32'b???????????????????????????1????};\
    wildcard bins bit_5_as_0 = {32'b??????????????????????????0?????};\
    wildcard bins bit_5_as_1 = {32'b??????????????????????????1?????};\
    wildcard bins bit_6_as_0 = {32'b?????????????????????????0??????};\
    wildcard bins bit_6_as_1 = {32'b?????????????????????????1??????};\
    wildcard bins bit_7_as_0 = {32'b????????????????????????0???????};\
    wildcard bins bit_7_as_1 = {32'b????????????????????????1???????};\
    wildcard bins bit_8_as_0 = {32'b???????????????????????0????????};\
    wildcard bins bit_8_as_1 = {32'b???????????????????????1????????};\
    wildcard bins bit_9_as_0 = {32'b??????????????????????0?????????};\
    wildcard bins bit_9_as_1 = {32'b??????????????????????1?????????};\
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



      // ===========================================================
      // Collecting the coverage related to conversion instructions
      // ============================================================

      covergroup txfma_conversion_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3);
         
         option.per_instance = 0;
	 type_option.merge_instances = 1;

         input1_cvg : coverpoint in1 {`walking_one_32}
         input2_cvg : coverpoint in2 {`walking_one_32}
         input3_cvg : coverpoint in3 {`walking_one_32}


         range_in1_cvg : coverpoint in1 {
            bins in1_0 = {0};
            bins in1_low = {[32'h1:32'h000F_FFFF]};
            bins in1_midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_high = {[32'h1000_0000:32'hFFFF_FFFE]};
            bins in1_f = {32'hFFFF_FFFF};
          }
        range_in2_cvg : coverpoint in2 {
            bins in2_0 = {0};
            bins in2_low = {[32'h1:32'h000F_FFFF]};
            bins in2_midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in2_midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in2_high = {[32'h1000_0000:32'hFFFF_FFFE]};
            bins in2_f = {32'hFFFF_FFFF};
          }
        range_in3_cvg : coverpoint in3 {
            bins in3_0 = {0};
            bins in3_low = {[32'h1:32'h000F_FFFF]};
            bins in3_midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in3_midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in3_high = {[32'h1000_0000:32'hFFFF_FFFE]};
            bins in3_f = {32'hFFFF_FFFF};
         }
	num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins one_operand = {18, 21, 87, 88};
          }

	conversion_inst_cvg: coverpoint cmd {
	   bins cvt_if = {18}; // FCMD_CVT_F32INT fcvt_ps_pw
           bins cvt_uif = {87}; 
           bins cvt_fi = {21}; // FCMD_CVT_INTF32
           bins cvt_fui = {88};

          }

      // Cross that determines if instruction has operands from the defined ranges
      CONVERSION_INST_VS_OPERAND1_RANGE:
         cross conversion_inst_cvg, range_in1_cvg;


endgroup


      // ===========================================================
      // Collecting the coverage related to compare instructions
      // ============================================================

      covergroup txfma_compare_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3);
        
         option.per_instance = 0;
	 type_option.merge_instances = 1;

         input1_cvg : coverpoint in1 {`walking_one_32}
         input2_cvg : coverpoint in2 {`walking_one_32}
         input3_cvg : coverpoint in3 {`walking_one_32}


         range_in1_cvg : coverpoint in1 {
            bins in1_0 = {0};
            bins in1_low = {[32'h1:32'h000F_FFFF]};
            bins in1_midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_high = {[32'h1000_0000:32'hFFFF_FFFE]};
            bins in1_f = {32'hFFFF_FFFF};
          }
        range_in2_cvg : coverpoint in2 {
            bins in2_0 = {0};
            bins in2_low = {[32'h1:32'h000F_FFFF]};
            bins in2_midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in2_midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in2_high = {[32'h1000_0000:32'hFFFF_FFFE]};
            bins in2_f = {32'hFFFF_FFFF};
          }
        range_in3_cvg : coverpoint in3 {
            bins in3_0 = {0};
            bins in3_low = {[32'h1:32'h000F_FFFF]};
            bins in3_midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in3_midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in3_high = {[32'h1000_0000:32'hFFFF_FFFE]};
            bins in3_f = {32'hFFFF_FFFF};
         }

	num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins one_operand = {86};
            bins two_operands = {31,32,33};
          }
      compare_inst_cvg: coverpoint cmd {
           bins fclass = {86};
           bins feq = {31};
           bins fle = {32};
           bins flt = {33};
      }
      // Cross that determines if every instruction has operands from the defined ranges
      COMPARE_INST_VS_OPERAND1_RANGE:
         cross compare_inst_cvg, range_in1_cvg{
         ignore_bins operands_not1 = COMPARE_INST_VS_OPERAND1_RANGE  
 	 with (compare_inst_cvg == 31 || compare_inst_cvg == 32 || compare_inst_cvg == 33); 
      }
      
      COMPARE_INST_VS_OPERANDS12_RANGE:
         cross compare_inst_cvg, range_in1_cvg, range_in2_cvg{
         ignore_bins operands_not12 = COMPARE_INST_VS_OPERANDS12_RANGE  
         with (compare_inst_cvg == 86); 
      }
      endgroup
      // ===========================================================
      // Collecting the coverage related to computational instructions
      // ============================================================

      covergroup txfma_computational_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3);
         
         option.per_instance = 0;
	 type_option.merge_instances = 1;
         input1_cvg : coverpoint in1 {`walking_one_32}
         input2_cvg : coverpoint in2 {`walking_one_32}
         input3_cvg : coverpoint in3 {`walking_one_32}

	range_in1_cvg : coverpoint in1 {
            bins in1_0low  = {[0:32'h000F_FFFF]};
            bins in1_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in1_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in1_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in1_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in1_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_in2_cvg : coverpoint in2 {
            bins in2_0low  = {[0:32'h000F_FFFF]};
            bins in2_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in2_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in2_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in2_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in2_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in2_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in2_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_in3_cvg : coverpoint in3 {
            bins in3_0low  = {[0:32'h000F_FFFF]};
            bins in3_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in3_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in3_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in3_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in3_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in3_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in3_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
	  }
        num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins two_operands = {0, 1, 2, 3, 4, 5, 6, 15, 16};
            bins three_operands = {7, 8, 9, 10, 95, 96};
          }

        computational_inst_cvg: coverpoint cmd {
           bins add = {0}; 
           bins sub = {1}; 
           bins mul = {2}; 
           bins mull = {3};
           ignore_bins umul = {4};//not implemented in test
           bins umulh = {5};
           bins mulh = {6};
           bins madd = {7};
           bins msub = {8};
           ignore_bins nmsub = {9};
           bins nmadd = {10};
           bins min = {15};
           bins max = {16};
           ignore_bins tfma_ph = {95};//not implemented
           ignore_bins tfma_ps = {96};//not implemented
           ignore_bins trans_fma = {98};
           ignore_bins trans_rcp_fma = {99};
           ignore_bins trans_rsqrt_fma = {100};//not implemented
           ignore_bins trans_log2_fma = {101};//not implemented
           ignore_bins trans_exp_fma = {102};//not implemented
           ignore_bins trans_sin_fma = {103};//not implemented
      }

      COMPUTATIONAL_INST_VS_OPERANDS12_RANGE:
         cross computational_inst_cvg, range_in1_cvg, range_in2_cvg{
         ignore_bins operands_not12 = COMPUTATIONAL_INST_VS_OPERANDS12_RANGE  
         with (computational_inst_cvg == 7 || computational_inst_cvg ==8 || computational_inst_cvg == 10 );
	}
      COMPUTATIONAL_INST_VS_OPERANDS123_RANGE:
         cross computational_inst_cvg, range_in1_cvg, range_in2_cvg, range_in3_cvg{
         ignore_bins operands_not123 = COMPUTATIONAL_INST_VS_OPERANDS123_RANGE  
         with (computational_inst_cvg == 0 || computational_inst_cvg ==1 || computational_inst_cvg == 2 || computational_inst_cvg == 3 || computational_inst_cvg == 4 || computational_inst_cvg == 5 || computational_inst_cvg == 6 || computational_inst_cvg == 15 || computational_inst_cvg == 16);
	}

endgroup

   // ===========================================================
      // Collecting the coverage related to upconvert graphics instructions
      // ============================================================

      covergroup txfma_down_graphics_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3);
         
         option.per_instance = 0;
	 type_option.merge_instances = 1;

         input1_cvg : coverpoint in1 {`walking_one_32}
         input2_cvg : coverpoint in2 {`walking_one_32}
         input3_cvg : coverpoint in3 {`walking_one_32}

	range_in1_cvg : coverpoint in1 {
            bins in1_0low  = {[0:32'h000F_FFFF]};
            bins in1_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in1_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in1_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in1_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in1_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_in2_cvg : coverpoint in2 {
            bins in2_0low  = {[0:32'h000F_FFFF]};
            bins in2_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in2_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in2_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in2_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in2_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in2_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in2_1high = {[32'h9000_0000:32'hFFFF_FFFF]};

          }

        num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins two_operands = {59, 60, 61, 62, 63, 64, 65, 66, 68, 69};
          }

      downconvert_graphics_inst_cvg: coverpoint cmd { 
           bins f16f32 = {59}; //float32 to float16
           ignore_bins f11f32 = {60};
           ignore_bins f10f32 = {61};
           ignore_bins un24f32 = {62};
           ignore_bins un16f32 = {63};
           ignore_bins un10f32 = {64};
           ignore_bins un8f32 = {65};
           ignore_bins un2f32 = {66};
           ignore_bins sn16f32 = {68};
           ignore_bins sn8f32 = {69};
      }


      DO_GRAPHICS_INST_VS_OPERANDS2_RANGE:
        cross downconvert_graphics_inst_cvg, range_in1_cvg, range_in2_cvg;
	

endgroup

      // ===========================================================
      // Collecting the coverage related to input operands
      // ============================================================

      covergroup txfma_upconvert_graphics_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3);
         
         option.per_instance = 0;
	 type_option.merge_instances = 1;

         input1_cvg : coverpoint in1 {`walking_one_32}
         input2_cvg : coverpoint in2 {`walking_one_32}
         input3_cvg : coverpoint in3 {`walking_one_32}

         range_in1_16bit_cvg : coverpoint in1 {
            bins in1_0 = {0};
            bins in1_low = {[16'h1:16'h000F]};
            bins in1_midl = {[16'h0010:16'h00FF]};
	    bins in1_midh = {[16'h0100:16'h0FFF]};
	    bins in1_high = {[16'h1000:16'hFFFE]};
            bins in1_f = {16'hFFFF};
          }
	range_in1_cvg : coverpoint in1 {
            bins in1_0low  = {[0:32'h000F_FFFF]};
            bins in1_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in1_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in1_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in1_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in1_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins one_operand = {50, 51, 52, 53, 54, 55, 56, 57, 58, 77, 78, 91, 93, 94};
          }
      upconvert_graphics_inst_cvg: coverpoint cmd {
	   bins f32f16 = {50}; //float16 to float32
           ignore_bins f32f11 = {51}; //float11 to float32
           ignore_bins f32f10 = {52};
           ignore_bins f32un24 = {53};
           ignore_bins f32un16 = {54};
           ignore_bins f32un8 = {55};
           ignore_bins f32sn16 = {57};
           ignore_bins f32sn8 = {58};
           ignore_bins f32un2 = {91};
           ignore_bins f32sn2 = {92};
           ignore_bins f32un10 = {93};
           ignore_bins f32sn10 = {94};
           bins fround = {77};
           bins addi = {78};
      }
      UP_GRAPHICS_INST_VS_16BIT_RANGE:
         cross upconvert_graphics_inst_cvg, range_in1_16bit_cvg{
	 ignore_bins operands_not_16bit_range = UP_GRAPHICS_INST_VS_16BIT_RANGE  
         with (upconvert_graphics_inst_cvg == 53 || upconvert_graphics_inst_cvg ==77 || upconvert_graphics_inst_cvg == 78 );
      }

      UP_GRAPHICS_INST_VS_OPERAND1_RANGE:
         cross upconvert_graphics_inst_cvg, range_in1_16bit_cvg{
	 ignore_bins operands_not_op1_range = UP_GRAPHICS_INST_VS_OPERAND1_RANGE  
         with (upconvert_graphics_inst_cvg == 50 || upconvert_graphics_inst_cvg ==51 || upconvert_graphics_inst_cvg == 52 || upconvert_graphics_inst_cvg == 53 || upconvert_graphics_inst_cvg == 54 || upconvert_graphics_inst_cvg == 55 || upconvert_graphics_inst_cvg == 57 || upconvert_graphics_inst_cvg == 58 || upconvert_graphics_inst_cvg == 91 || upconvert_graphics_inst_cvg == 92 || upconvert_graphics_inst_cvg == 93 || upconvert_graphics_inst_cvg == 94 );
      }
endgroup

endpackage;

module neigh_txfma_cvg (input clock);
	import neigh_txfma_cov_pkg::*;
	bit[31:0] in1, in2, in3;
	bit[7:0] cmd;
	bit ren1, ren2, ren3;

        txfma_conversion_cg	txfma_conversion_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3);
	txfma_compare_cg	txfma_compare_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3);
	txfma_computational_cg	txfma_computational_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3);
	txfma_down_graphics_cg	txfma_down_graphics_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3);
	txfma_upconvert_graphics_cg	txfma_upconvert_graphics_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3);

        always @ (posedge clock) begin
		in1=txfma_top.in_data.in1;
		in2=txfma_top.in_data.in2;
		in3=txfma_top.in_data.in3;
		cmd=txfma_top.in_data.sigs.cmd;
		ren1=txfma_top.in_data.sigs.ren1;
		ren2=txfma_top.in_data.sigs.ren2;
		ren3=txfma_top.in_data.sigs.ren3;
		
		txfma_conversion_cg_inst.sample();
		txfma_compare_cg_inst.sample();
		txfma_computational_cg_inst.sample();
		txfma_down_graphics_cg_inst.sample();
		txfma_upconvert_graphics_cg_inst.sample();
	end
endmodule: neigh_txfma_cvg

	bind txfma_top neigh_txfma_cvg neigh_txfma_cov(.clock(clock));
`endif
`endif

   
