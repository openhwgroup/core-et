`ifdef EVL_COVERAGE_ON

`ifndef __NEIGH_SWIZZ_CVG
`define __NEIGH_SWIZZ_CVG

package neigh_swizz_cov_pkg;

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
      // Collecting the coverage related to input operands
      // ============================================================

      covergroup swizz_packedint_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit [3:0] dtype, ref bit[2:0] rm,ref bit ren1, ren2, ren3, ref bit[31:0] swizz_1, swizz_2, swizz_3, ref bit[2:0] lane);
         option.name = "swizz_packedint_cg";
         option.per_instance = 0;
	 type_option.merge_instances = 1;
         input1_cvg : coverpoint in1 {`walking_one_32}
         input2_cvg : coverpoint in2 {`walking_one_32}

         range_in1_cvg : coverpoint in1 {
            bins in1_0low  = {[32'h0:32'h000F_FFFF]};
            bins in1_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in1_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in1_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in1_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in1_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_in2_cvg : coverpoint in2 {
            bins in2_0low  = {[32'h0:32'h000F_FFFF]};
            bins in2_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in2_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in2_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in2_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in2_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in2_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in2_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
 
       }
        num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins zero_operands = {82} iff (dtype == 0);// there is a packed_integer_instruction with zero operands
	    bins one_operand = {24, 78, 79, 80, 83, 84, 109, 110, 126}; 
            bins two_operands = {15, 16, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
            
          }

        packed_int_inst_cvg: coverpoint cmd {
           bins FCMD_MINU  = {15} iff (rm == 0); //RNE rm
           bins FCMD_MAXU  = {16} iff (rm == 1); //RTZ rm
           bins FCMD_MIN  = {15} iff (rm == 2); //RDN rm
           bins FCMD_MAX  = {16} iff (rm == 3); //RUP rm
	   bins FCMD_NOT  = {24};
           bins FCMD_AND  = {25};
           bins FCMD_OR   = {26};
           bins FCMD_XOR  = {27};
           bins FCMD_SLL  = {28};
           bins FCMD_SRL  = {29};
           bins FCMD_SRA  = {30};
           bins FCMD_FEQ  = {31};
           bins FCMD_FLE  = {32};
           bins FCMD_FLT  = {33};
           bins FCMD_FLTU = {34};
           ignore_bins FCMD_ADDI = {78};//not yet implemented
           bins FCMD_ANDI = {79};
           bins FCMD_SLLI = {80};
           bins FCMD_FBCI = {82} iff (dtype == 0);
           bins FCMD_SRAI = {84};
           ignore_bins FCMD_SLLOI = {81};//won't be implemented
           bins FCMD_SRLI  = {83};//not implemented
	   bins FCMD_PACKREPB  = {109};
	   bins FCMD_PACKREPH  = {110};
	   bins FCMD_FSAT8 ={126};
	   bins FCMD_FSATU8 ={97};
      }
/*
      operand1_cvg: coverpoint ren1 {
         bins true = {1'b1};
         bins false = {1'b0};//it will be hit only when cmd=FCMD_FBCI
      }
      operand2_cvg: coverpoint ren2 {
         bins true = {1'b1};
         bins false = {1'b0};
      }
      operand3_cvg: coverpoint ren3 {
         ignore_bins true = {1'b1}; //it is hardcoded to 0
         bins false = {1'b0};
      } 

      //Cross that determines all operands are used in set of packed int instructions
      PACKED_INT_INST_VS_OPERAND1:
         cross packed_int_inst_cvg, operand1_cvg{
         ignore_bins operand_one_inactiv = PACKED_INT_INST_VS_OPERAND1
         with (packed_int_inst_cvg == 82 || operand1_cvg == 0);
      }

      PACKED_INT_INST_VS_OPERAND2:
         cross packed_int_inst_cvg, operand2_cvg{
         ignore_bins operand_two_inactiv = PACKED_INT_INST_VS_OPERAND2
         with (packed_int_inst_cvg == 24 || packed_int_inst_cvg == 78 || packed_int_inst_cvg == 79 || 
		packed_int_inst_cvg == 80 || packed_int_inst_cvg == 82 || packed_int_inst_cvg == 83 || 
		packed_int_inst_cvg == 84 || operand2_cvg == 0);
      }
      
      PACKED_INT_INST_VS_OPERAND3:
         cross packed_int_inst_cvg, operand3_cvg{
         ignore_bins operand_three_inactiv = PACKED_INT_INST_VS_OPERAND3
         with (packed_int_inst_cvg != 0 || operand3_cvg == 0);  
      }
                            
      PACKED_INT_INST_VS_ONE_ACTIVE_OPERAND:
         cross packed_int_inst_cvg, num_of_active_operands_per_inst_cvg{
         ignore_bins not_one_operand = PACKED_INT_INST_VS_ONE_ACTIVE_OPERAND
         with (packed_int_inst_cvg == 15 || packed_int_inst_cvg == 16 || packed_int_inst_cvg == 25 || packed_int_inst_cvg == 26 ||
	       packed_int_inst_cvg == 27 || packed_int_inst_cvg == 28 || packed_int_inst_cvg == 29 || packed_int_inst_cvg == 30 || 
	       packed_int_inst_cvg == 31 || packed_int_inst_cvg == 32 || packed_int_inst_cvg == 33 || packed_int_inst_cvg == 34 || 
		packed_int_inst_cvg == 82 || num_of_active_operands_per_inst_cvg == 15 || 
		num_of_active_operands_per_inst_cvg == 16 || num_of_active_operands_per_inst_cvg == 25 ||
                num_of_active_operands_per_inst_cvg == 26 || num_of_active_operands_per_inst_cvg == 27 || 
		num_of_active_operands_per_inst_cvg == 28 || num_of_active_operands_per_inst_cvg == 29 ||
	        num_of_active_operands_per_inst_cvg == 30 || num_of_active_operands_per_inst_cvg == 31 ||
                num_of_active_operands_per_inst_cvg == 32 || num_of_active_operands_per_inst_cvg == 33 || 
		num_of_active_operands_per_inst_cvg == 34 || num_of_active_operands_per_inst_cvg == 82);
      }

      PACKED_INT_INST_VS_TWO_ACTIVE_OPERANDS:
         cross packed_int_inst_cvg, num_of_active_operands_per_inst_cvg{
         ignore_bins not_two_operands = PACKED_INT_INST_VS_TWO_ACTIVE_OPERANDS
         with (packed_int_inst_cvg == 24 || packed_int_inst_cvg == 78 || packed_int_inst_cvg == 79 || packed_int_inst_cvg == 80 || 
	       packed_int_inst_cvg == 82 || packed_int_inst_cvg == 83 || packed_int_inst_cvg == 84 ||
                num_of_active_operands_per_inst_cvg == 24 || num_of_active_operands_per_inst_cvg == 78 || num_of_active_operands_per_inst_cvg == 79 ||
                num_of_active_operands_per_inst_cvg == 80 || num_of_active_operands_per_inst_cvg == 82 || 
		num_of_active_operands_per_inst_cvg == 83 || num_of_active_operands_per_inst_cvg == 84);
      }*/


      // Cross that determines if every instruction has operands from the defined ranges
      PACKED_INT_INST_VS_OPERAND1_RANGE:
         cross packed_int_inst_cvg, range_in1_cvg{
         ignore_bins operands_not1 = PACKED_INT_INST_VS_OPERAND1_RANGE  
 	 with (packed_int_inst_cvg == 15 || packed_int_inst_cvg == 16 || packed_int_inst_cvg == 25 || packed_int_inst_cvg == 26 ||
	       packed_int_inst_cvg == 27 || packed_int_inst_cvg == 28 || packed_int_inst_cvg == 29 || packed_int_inst_cvg == 30 || 
	       packed_int_inst_cvg == 31 || packed_int_inst_cvg == 32 || packed_int_inst_cvg == 33 || packed_int_inst_cvg == 34 || 
		packed_int_inst_cvg == 82);
      }

      PACKED_INT_INST_VS_OPERANDS12_RANGE:
         cross packed_int_inst_cvg, range_in1_cvg, range_in2_cvg{
         ignore_bins operands_not12 = PACKED_INT_INST_VS_OPERANDS12_RANGE  
         with (packed_int_inst_cvg == 24 || packed_int_inst_cvg == 78 || packed_int_inst_cvg == 79 || packed_int_inst_cvg == 80 || 
	       packed_int_inst_cvg == 82 || packed_int_inst_cvg == 83 || packed_int_inst_cvg == 84 || packed_int_inst_cvg == 109 || packed_int_inst_cvg == 110 || packed_int_inst_cvg ==126 || packed_int_inst_cvg ==97);
      }



			packed_int_swizz1_b: coverpoint swizz_1[7:0] {			
				bins swizz1b_0= {0};
				bins swizz1b_low = {[8'h01:8'h7F]};
				bins swizz1b_high = {[8'h80:8'hFE]};
				bins swizz1b_1 = {8'hFF};
			}
			packed_int_swizz2_b: coverpoint swizz_2[7:0] {			
				bins swizz2b_0= {0};
				bins swizz2b_low = {[8'h01:8'h7F]};
				bins swizz2b_high = {[8'h80:8'hFE]};
				bins swizz2b_1 = {8'hFF};
			}
			packed_int_swizz3_b: coverpoint swizz_3[7:0] {			
				bins swizz3b_0= {0};
				bins swizz3b_low = {[8'h01:8'h7F]};
				bins swizz3b_high = {[8'h80:8'hFE]};
				bins swizz3b_1 = {8'hFF};
			}
			packed_int_in1_b: coverpoint in1[7:0] {			
				bins in1b_0= {0};
				bins in1b_low = {[8'h01:8'h7F]};
				bins in1b_high = {[8'h80:8'hFE]};
				bins in1b_1 = {8'hFF};
			}

			packed_int_swizz1_h: coverpoint swizz_1[15:0] {			
				bins swizz1h_0= {0};
				bins swizz1h_low = {[16'h0001:16'h7FFF]};
				bins swizz1h_high = {[16'h8000:16'hFFFE]};
				bins swizz1h_1 = {16'hFFFF};
			}
			packed_int_swizz2_h: coverpoint swizz_2[15:0] {			
				bins swizz2h_0= {0};
				bins swizz2h_low = {[16'h0001:16'h7FFF]};
				bins swizz2h_high = {[16'h8000:16'hFFFE]};
				bins swizz2h_1 = {16'hFFFF};
			}
			packed_int_swizz3_h: coverpoint swizz_3[15:0] {			
				bins swizz3h_0= {0};
				bins swizz3h_low = {[16'h0001:16'h7FFF]};
				bins swizz3h_high = {[16'h8000:16'hFFFE]};
				bins swizz3h_1 = {16'hFFFF};
			}
			packed_int_in1_h: coverpoint in1[15:0] {			
				bins in1h_0= {0};
				bins in1h_low = {[16'h0001:16'h7FFF]};
				bins in1h_high = {[16'h8000:16'hFFFE]};
				bins in1h_1 = {16'hFFFF};
			}

      
      
      endgroup

      // ==============================================================
      // Collecting the coverage related to conversion instructions
      // ==============================================================

      covergroup swizz_conversion_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3, ref bit[31:0] swizz_1, swizz_2, swizz_3, ref bit[2:0] lane);
         option.name = "swizz_conversion_cg";
         option.per_instance = 0;
	 type_option.merge_instances = 1;
         //input1_cvg : coverpoint in1 {`walking_one_32}
         //input2_cvg : coverpoint in2 {`walking_one_32}
         //input3_cvg : coverpoint in3 {`walking_one_32}

         range_in1_cvg : coverpoint in1 {
            bins in1_0low  = {[32'h0:32'h000F_FFFF]};
            bins in1_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in1_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in1_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in1_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in1_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in1_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in1_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_in2_cvg : coverpoint in2 {
            bins in2_0low  = {[32'h0:32'h000F_FFFF]};
            bins in2_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in2_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in2_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in2_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in2_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in2_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in2_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_in3_cvg : coverpoint in3 {
            bins in3_0low  = {[32'h0:32'h000F_FFFF]};
            bins in3_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins in3_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins in3_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins in3_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins in3_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins in3_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins in3_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
	  }

         range_swizz1_cvg : coverpoint swizz_1 {
            bins swizz_0low  = {[32'h0:32'h000F_FFFF]};
            bins swizz1_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins swizz1_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins swizz1_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins swizz1_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins swizz1_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins swizz1_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins swizz1_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_swizz2_cvg : coverpoint swizz_2 {
            bins swizz2_0low  = {[32'h0:32'h000F_FFFF]};
            bins swizz2_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins swizz2_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins swizz2_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins swizz2_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins swizz2_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins swizz2_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins swizz2_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
          }
        range_swizz3_cvg : coverpoint swizz_3 {
            bins swizz3_0low  = {[32'h1:32'h000F_FFFF]};
            bins swizz3_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
	    bins swizz3_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
	    bins swizz3_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
            bins swizz3_1low  = {[32'h8000_0000:32'h800F_FFFF]};
            bins swizz3_1midl = {[32'h8010_0000:32'h80FF_FFFF]};
	    bins swizz3_1midh = {[32'h8100_0000:32'h8FFF_FFFF]};
	    bins swizz3_1high = {[32'h9000_0000:32'hFFFF_FFFF]};
	  }

		lane_id_cvg : coverpoint lane {
			bins lane0 = {0} iff (cmd == 47);
			bins lane1 = {1} iff (cmd == 47);
			bins lane2 = {2} iff (cmd == 47);
			bins lane3 = {3} iff (cmd == 47);
			bins lane4 = {4} iff (cmd == 47);
			bins lane5 = {5} iff (cmd == 47);
			bins lane6 = {6} iff (cmd == 47);
			bins lane7 = {7} iff (cmd == 47);
		}
        
        num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins one_operand = {20, 39, 48, 49}; //
            bins two_operands = {14, 46, 47, 89, 90, 97};
	    bins three_operands = {45};// there is a conversion_instruction with three operands
	                
          }

        conversion_inst_cvg: coverpoint cmd {
           bins FCMD_FSGNJ   = {14}; 
           bins FCMD_MV_XF   = {20};
	   bins FCMD_FSETM   = {39}; 
	   bins FCMD_FCMOV   = {45};
           bins FCMD_FCMOVM  = {46};
           bins FCMD_FSWIZZ  = {47};
           bins FCMD_MVZ_FX  = {48};
           bins FCMD_MVS_FX  = {49};
           bins FCMD_FSGNJN  = {89};
           bins FCMD_FSGNJX  = {90};
      }

                         
      // Cross that determines if every instruction has operands from the defined ranges
      CONVERSION_INST_VS_OPERAND1_RANGE:
         cross conversion_inst_cvg, range_in1_cvg{
         ignore_bins operands_not1 = CONVERSION_INST_VS_OPERAND1_RANGE  
 	 with (conversion_inst_cvg == 14 || conversion_inst_cvg == 45 || conversion_inst_cvg == 46 || conversion_inst_cvg == 47 ||
	       conversion_inst_cvg == 89 || conversion_inst_cvg == 90 || conversion_inst_cvg == 97);
      }

      CONVERSION_INST_VS_OPERANDS12_RANGE:
         cross conversion_inst_cvg, range_in1_cvg, range_in2_cvg{
         ignore_bins operands_not12 = CONVERSION_INST_VS_OPERANDS12_RANGE  
         with (conversion_inst_cvg == 20 || conversion_inst_cvg == 39 || conversion_inst_cvg == 45 || conversion_inst_cvg == 46 || conversion_inst_cvg == 48 || conversion_inst_cvg == 49);
      }
      CONVERSION_INST_VS_OPERANDS23_RANGE:
         cross conversion_inst_cvg, range_in2_cvg, range_in3_cvg{
         ignore_bins operands_not23 = CONVERSION_INST_VS_OPERANDS23_RANGE  
         with ( conversion_inst_cvg == 14 || conversion_inst_cvg == 20 || conversion_inst_cvg == 39 || conversion_inst_cvg == 45 || conversion_inst_cvg == 48 || conversion_inst_cvg == 49 ||
								conversion_inst_cvg == 89 || conversion_inst_cvg == 90 || conversion_inst_cvg == 97);
      }
      
    /*  CONVERSION_INST_VS_OPERANDS123_RANGE:
         cross conversion_inst_cvg, range_in1_cvg, range_in2_cvg, range_in3_cvg{
         ignore_bins operands_not123 = CONVERSION_INST_VS_OPERANDS123_RANGE  
 	 with (conversion_inst_cvg == 14 || conversion_inst_cvg == 47 || conversion_inst_cvg == 48 || conversion_inst_cvg == 49 ||
	       conversion_inst_cvg == 89 || conversion_inst_cvg == 90 || conversion_inst_cvg == 97);
      }*/

		lane_id_X_swizz: 
		cross lane_id_cvg, range_swizz1_cvg, range_swizz2_cvg, range_swizz3_cvg, conversion_inst_cvg{
		ignore_bins not_swizz = lane_id_X_swizz with (conversion_inst_cvg != 47);
		}
      endgroup
 
      // ==============================================================
      // Collecting the coverage related to conversion instructions
      // ==============================================================

      covergroup swizz_broadcast_cg (ref bit[31:0] in1, bit[19:0] imm, ref bit[7:0] cmd, ref bit [3:0] dtype);
         option.name = "swizz_broadcast_cg";
         option.per_instance = 0;
	 type_option.merge_instances = 1;
         //imm_cvg : coverpoint imm {`walking_one_20}
         
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

         range_imm_cvg : coverpoint imm {
	    bins imm_0 = {0};
            bins imm_low  = {[20'h1:20'h0_FFFF]};
            bins imm_mid = {[20'h1_0000:20'h7_FFFF]};
	    bins imm_high = {[20'h8_0000:20'hF_FFFF]};
            bins imm_f = {20'hF_FFFF};
          }
        
        
        num_of_active_operands_per_inst_cvg : coverpoint cmd {
            bins zero_operands = {82, 85}; 
                          
          }

        broadcast_inst_cvg: coverpoint cmd {
            bins FCMD_FBCI   = {82} iff (dtype == 1); 
            bins FCMD_FBC    = {85}; 
	  }

                         
      // Cross that determines if every instruction has operands from the defined ranges
      BROADCAST_INST_VS_OPERAND1_RANGE:
         cross broadcast_inst_cvg, range_in1_cvg {
	 ignore_bins operand_notimm = BROADCAST_INST_VS_OPERAND1_RANGE
	 with (broadcast_inst_cvg == 85);
      }	
     
            // Cross that determines if every instruction has operands from the defined ranges
      BROADCAST_INST_VS_IMM_RANGE:
         cross broadcast_inst_cvg, range_imm_cvg {
	   bins BROADCAST_INST_VS_IMM_RANGE0 = binsof (broadcast_inst_cvg) intersect {85} && binsof (range_imm_cvg) intersect {0};
           bins BROADCAST_INST_VS_IMM_RANGE1 = binsof (broadcast_inst_cvg) intersect {85} && binsof (range_imm_cvg) intersect {[20'h1:20'h0_FFFF]};
      }	
 

     endgroup   


      // ==============================================================
      // Collecting the coverage related to graphics instructions
      // ==============================================================

      covergroup swizz_graphics_cg (ref bit[31:0] in1, in2, in3, ref bit[7:0] cmd, ref bit ren1, ren2, ren3);
         option.name = "swizz_graphics_cg";
         option.per_instance = 0;
	 type_option.merge_instances = 1;
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
            bins two_operands = {106,107,108};
	    bins three_operands = {105};
	                
          }

        graphics_inst_cvg: coverpoint cmd {
           ignore_bins FCMD_CUBEFACE    = {105}; 
           ignore_bins FCMD_CUBEFACEIDX = {106}; 
	   ignore_bins FCMD_CUBESGNSC   = {107};
           ignore_bins FCMD_CUBESGNTC   = {108};
         
	}
                         
      // Cross that determines if every instruction has operands from the defined ranges

      range_in1_cross: coverpoint (in1 & 32'h1) {
      bins in1_0 = {0};
      bins in1_1 = {1};
	}
      range_in2_cross: coverpoint (in2 & 32'h1) {
      bins in2_0 = {0};
      bins in2_1 = {1};
	}
      range_in3_cross: coverpoint (in3 & 32'h1) {
      bins in3_0 = {0};
      bins in3_1 = {1};
	} 

      range_in11_cross: coverpoint (in1 & 32'h3) {
      bins in1_0 = {0};
      bins in1_1 = {1};
      bins in1_2 = {2};
      bins in1_3 = {3};
	}
      range_in22_cross: coverpoint (in2 & 32'h8000_0000) {
      bins in2_0 = {0};
      bins in2_1 = {32'h8000_0000};
	}
      range_in12_cross: coverpoint (in1 & 32'h7) {
      bins in1_0 = {0};
      bins in1_1 = {1};
      bins in1_2 = {2};
      bins in1_3 = {3};
      bins in1_4 = {4};
      bins in1_5 = {5};
      bins in1_6 = {6};
      bins in1_7 = {7};
	}
      range_in21_cross : coverpoint in2 {
      bins in2_0 = {0};
      bins in2_0low  = {[32'h1:32'h000F_FFFF]};
      bins in2_0midl = {[32'h0010_0000:32'h00FF_FFFF]};
      bins in2_0midh = {[32'h0100_0000:32'h0FFF_FFFF]};
      bins in2_0high = {[32'h1000_0000:32'h7FFF_FFFF]};
	}
      GRAPHICS_INST_VS_OPERANDS12_RANGE:
         cross graphics_inst_cvg, range_in11_cross, range_in22_cross {
         ignore_bins operands_not12 = GRAPHICS_INST_VS_OPERANDS12_RANGE  
 	 with (graphics_inst_cvg == 105 || graphics_inst_cvg == 107 || graphics_inst_cvg == 108); //use 3 operands
	}
      GRAPHICS_INST_VS_OPERANDS21_RANGE:
         cross graphics_inst_cvg, range_in12_cross, range_in21_cross {
         ignore_bins operands_not12 = GRAPHICS_INST_VS_OPERANDS21_RANGE  
 	 with (graphics_inst_cvg == 105 || graphics_inst_cvg == 106); //use 3 operands
	}
      GRAPHICS_INST_VS_OPERANDS123_RANGE:
         cross graphics_inst_cvg, range_in1_cross, range_in2_cross, range_in3_cross{
         ignore_bins operands_not123 = GRAPHICS_INST_VS_OPERANDS123_RANGE  
 	 with (graphics_inst_cvg == 106 || graphics_inst_cvg == 107 || graphics_inst_cvg == 108 ); //uses 2 operands
	}


      endgroup

endpackage;

module neigh_swizz_cvg (input clock);
	import neigh_swizz_cov_pkg::*;
	bit[31:0] in1, in2, in3;
	bit[7:0] cmd;
	bit ren1, ren2, ren3, valid;
	bit[19:0] imm;
	bit [3:0] dtype;
	bit[31:0] swizz_1, swizz_2, swizz_3;
	bit[2:0] lane;
	bit[2:0] rm;

	swizz_packedint_cg	swizz_packedint_cg_inst = new(in1, in2, in3, cmd, dtype, rm, ren1, ren2, ren3, swizz_1, swizz_2, swizz_3, lane);
        swizz_conversion_cg	swizz_conversion_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3, swizz_1, swizz_2, swizz_3, lane);
	swizz_broadcast_cg	swizz_broadcast_cg_inst = new(in1, imm, cmd, dtype);
	swizz_graphics_cg	swizz_graphics_cg_inst = new(in1, in2, in3, cmd, ren1, ren2, ren3);

        always @ (posedge clock) begin
		in1=vpu_sh_sw.ex_in1;
		in2=vpu_sh_sw.ex_in2;
		in3=vpu_sh_sw.ex_in3;
		cmd=vpu_sh_sw.ex_cmd;
		valid=vpu_sh_sw.ex_in_valid;
		imm=vpu_sh_sw.ex_imm;
		dtype=vpu_sh_sw.ex_dtype;
		swizz_1=vpu_sh_sw.f2_swizz_in1;
		swizz_2=vpu_sh_sw.f2_swizz_in2;
		swizz_3=vpu_sh_sw.f2_swizz_in3;
		lane=vpu_sh_sw.lane_id;
		if (valid == 1) begin	
		   swizz_packedint_cg_inst.sample();
		   swizz_conversion_cg_inst.sample();
		   swizz_broadcast_cg_inst.sample();
		   swizz_graphics_cg_inst.sample();
		end

	end
endmodule: neigh_swizz_cvg

	bind vpu_sh_sw neigh_swizz_cvg neigh_swizz_cov(.clock(clock));

`endif
`endif
