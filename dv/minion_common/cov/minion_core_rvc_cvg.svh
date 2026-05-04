//================================================================================
// Coverage for compressed instructions
//================================================================================
`ifdef EVL_COVERAGE_ON

package minion_frontend_rvc_cov_pkg;
   
   ////////////////////////////////////////////////////////////////////////////////
   // RVC decoding coverage
   ////////////////////////////////////////////////////////////////////////////////


   covergroup rvc_decoding with function sample(logic [15:0] inst_bits);
      option.name = "RVC decoding";
      option.per_instance = 0;
      type_option.merge_instances = 1;
      
      // coverpoints for all the possible fields in the opcode, according to
      // the instruction formats defined in the ISA 
      // (see table 12.1 - Compressed 16-bit RVC formats from isa v2.2)

      // register fields
      r1: coverpoint (inst_bits[11:7]) // used in formats: CR, CI
        { option.weight = 0; type_option.weight = 0; }
      
      r2: coverpoint (inst_bits[6:2]) // used in formats: CR, CSS
        { option.weight = 0; type_option.weight = 0; }
      
      r1_short: coverpoint (inst_bits[9:7]) // used in formats CL,CS,CB
        { option.weight = 0; type_option.weight = 0; }
      
      r2_short: coverpoint (inst_bits[4:2]) // used in formats: CIW, CL, CS
        { option.weight = 0; type_option.weight = 0; }
      
      // immediate fields (max 64 bins)
      CI_imm: coverpoint ({inst_bits[12], inst_bits[6:2]})
        { option.weight = 0; type_option.weight = 0;
         bins zero = {0}; // zero in its own bin, to encode reserved cases
         bins vals[] = {[1:$]};
      }
      
      CSS_imm: coverpoint (inst_bits[12:7])
        { option.weight = 0; type_option.weight = 0; }
      
      CIW_imm: coverpoint (inst_bits[12:5])
        { option.weight = 0; type_option.weight = 0;
         bins zero = {0}; // zero in its own bin, to encode reserved cases
         bins vals[64] = {[1:$]};
      }
      
      CL_CS_imm: coverpoint ({inst_bits[12:10], inst_bits[6:5]} )
        { option.weight = 0; type_option.weight = 0; }
      
      CB1_offset: coverpoint ({inst_bits[12:10], inst_bits[6:2]})
        { option.weight = 0; type_option.weight = 0;  }

      CB2_offset: coverpoint ({inst_bits[12], inst_bits[6:2]})
        { option.weight = 0; type_option.weight = 0;  }
      
      
      CJ_target: coverpoint (inst_bits[12:2])
        { option.weight = 0; type_option.weight = 0; }

      
      // instruction decoding. grouping in coverpoints, one per instruction format
      
      // CR: 'register' format
      decode_CR: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CR (register) format";
         
         wildcard bins JR        = {16'b1000??????????10} iff (inst_bits[6:2] == 5'h0); // iff => JR vs MV
         wildcard bins MV        = {16'b1000??????????10} iff (inst_bits[6:2] != 5'h0); // iff => JR vs MV
         wildcard bins JALR      = {16'b1001??????????10} iff (inst_bits[6:2] == 5'h0 && inst_bits[11:7] != 5'h0);  // iff => ADD vs JALR vs EBREAK
         wildcard bins ADD       = {16'b1001??????????10} iff (inst_bits[6:2] != 5'h0);  // iff => ADD vs JALR vs EBREAK
      }
      
      //CI: 'immediate' format
      decode_CI: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CI (immediate) format";
         
         wildcard bins ADDI       = {16'b000???????????01} iff (inst_bits != 16'h01); // iff => ADDI VS NOP
         wildcard bins ADDIW      = {16'b001???????????01};
         wildcard bins LI         = {16'b010???????????01};
         wildcard bins ADDI16SP   = {16'b011???????????01} iff (inst_bits[11:7] == 5'h2); //iff => ADDI16SP vs LUI
         wildcard bins LUI        = {16'b011???????????01} iff (inst_bits[11:7] != 5'h2); //iff => ADDI16SP vs LUI
         wildcard bins SLLI       = {16'b000???????????10};
         wildcard bins FLDSP      = {16'b001???????????10};
         wildcard bins LWSP       = {16'b010???????????10};
         wildcard bins LDSP       = {16'b011???????????10};
      }

      // CSS: 'stack relative store' format
      decode_CSS: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CSS (stack relative store) format";
         
         wildcard bins  FSDSP     = {16'b101???????????10};
         wildcard bins  SWSP      = {16'b110???????????10};
         wildcard bins  SDSP      = {16'b111???????????10};
      }
      
      //CIW: 'wide immediate' format
      decode_CIW: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CIW (wide immediate) format";
         
         wildcard bins ADDI4SPN   = {16'b000???????????00} iff (inst_bits !=16'h00); // iff => ADDI4SPN vs illegal
      }
      
      //CL, CS: 'load' and 'store' formats
      decode_CL_CS: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CL (load)  and CS (store) formats";

         wildcard bins FLD        = {16'b001???????????00};
         wildcard bins LW         = {16'b010???????????00};
         wildcard bins LD         = {16'b011???????????00};
         wildcard bins FSD        = {16'b101???????????00};
         wildcard bins SW         = {16'b110???????????00};
         wildcard bins SD         = {16'b111???????????00};
       }
      decode_CS_no_imm: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CS (store) format, not using an immediate";

         wildcard bins SUB        = {16'b100011???00???01};
         wildcard bins XOR        = {16'b100011???01???01};
         wildcard bins OR         = {16'b100011???10???01};
         wildcard bins AND        = {16'b100011???11???01};
         wildcard bins SUBW       = {16'b100111???00???01};
         wildcard bins ADDW       = {16'b100111???01???01};               
      }
      
      // CB1: 'branch' format, with imm=8 bits
      decode_CB1: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CB (branch) format, 8b imm";
         wildcard bins BEQZ       = {16'b110???????????01};
         wildcard bins BNEZ       = {16'b111???????????01};
      }

      // CB: 'branch' format, with imm=6
      decode_CB2: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CB (branch) format, 6b imm";

         wildcard bins SRLI       = {16'b100?00????????01};
         wildcard bins SRAI       = {16'b100?01????????01};
         wildcard bins ANDI       = {16'b100?10????????01};         
      }

      
      //CJ: 'jump' format
      decode_CJ: coverpoint inst_bits 
        {
         option.comment = "RVC instructions with CJ (jump) format";

         wildcard bins J         = {16'b101???????????01};
      }
      
      // remaining instructions (these don't use any of their fields)
      decode_other: coverpoint inst_bits 
        {
         option.comment = "RVC instructions that don't use any fields form the opcode";
         
         bins ILLEGAL             = {16'b0000000000000000};
         wildcard bins RESERVED0  = {16'b100???????????00};
         bins NOP                 = {16'b0000000000000001};         
         wildcard bins RESERVED1  = {16'b100111???1????01};         
         bins EBREAK              = {16'b1001000000000010};
      }
      
      ////////////////////////////////////////////////////////////////////////////////
      // cross coverage for each kind of instruction with the fields it uses
      // The 'reserved' values according to the ISA (e.g. some instruction require
      // imm!=0) will have an specific bin
      ////////////////////////////////////////////////////////////////////////////////
      CR:    cross decode_CR, r1, r2
        {
         // JR and MV are distinguised with r2 ( 0 for JR, any non-zero for MV)
         ignore_bins jr_r2z = binsof(decode_CR.JR) && !binsof(r2) intersect {0};
         ignore_bins mv_2nz = binsof(decode_CR.MV) && binsof(r2) intersect {0};
         
         // JALR and ADD differ with r2 => 0 for jalr, non zero for ADD
         // and if both r1 and r2 are 0 => it's an ebreak
         ignore_bins jalr_r2z = binsof(decode_CR.JALR) && !binsof(r2) intersect {0};
         ignore_bins ebreak = binsof(decode_CR.JALR) && binsof(r1) intersect {0}; 
         ignore_bins add_r2nz = binsof(decode_CR.ADD) && binsof(r2) intersect {0};
         
         // JR with r1=0 is reserved
         bins res_jr = binsof(decode_CR.JR) && binsof(r1) intersect {0};
      }
      
      CI:    cross decode_CI, r1, CI_imm
        {
         // addi16sp and lui have the same encoding and they differ on the register: SP and any one not SP respectively
         ignore_bins addi16sp_only_sp = binsof(decode_CI.ADDI16SP) && !binsof(r1) intersect {2};
         ignore_bins lui_nosp = binsof(decode_CI.LUI) && binsof(r1) intersect {2};
         // c.addi with r=x0, imm=0 is a nop 
         ignore_bins nop = binsof(decode_CI.ADDI) && binsof(r1) intersect {0} && binsof(CI_imm.zero);
         
         // reserved encodings
         bins res_addiw = binsof(decode_CI.ADDIW) && binsof(r1) intersect {0};
         bins res_addi16sp = binsof(decode_CI.ADDI16SP) && binsof(CI_imm.zero);
         bins res_lui = binsof(decode_CI.LUI) && binsof(CI_imm.zero);
         bins res_lwsp = binsof(decode_CI.LWSP) && binsof(r1) intersect {0};
         bins res_ldsp = binsof(decode_CI.LDSP) && binsof(r1) intersect {0};
      }
      
      CSS:   cross decode_CSS, r1, CI_imm;
      
      CIW:   cross decode_CIW, r2_short, CIW_imm
        {
         bins res_addi4spn = binsof(decode_CIW.ADDI4SPN) && binsof(CIW_imm.zero);
      }

      CL_CS: cross decode_CL_CS, r1_short, r2_short, CL_CS_imm;
      CS_no_imm: cross decode_CS_no_imm, r1_short, r2_short;
      CB1:   cross decode_CB1, r1_short, CB1_offset;
      CB2:   cross decode_CB2, r1_short, CB2_offset;
      CJ:    cross decode_CJ, CJ_target;
        
   endgroup // RVC
   
endpackage // minion_frontend_rvc_cov_pkg
   

module minion_frontend_rvc_cvg;
   import minion_frontend_rvc_cov_pkg::*;
   
   ////////////////////////////////////////////////////////////////////////////////
   // extract signals from RTL (from frontend_thread_buffer)
   ////////////////////////////////////////////////////////////////////////////////
   wire        clock          = frontend_thread_buffer.clock_aon;
   wire        reset          = frontend_thread_buffer.reset;
   wire [15:0] inst_bits      = frontend_thread_buffer.f6_inst_bits[15:0];
   wire        rvc_inst_valid = frontend_thread_buffer.f6_valid && !frontend_thread_buffer.f6_stall
                                && inst_bits[1:0]!=2'b11 && !reset;
   
   ////////////////////////////////////////////////////////////////////////////////
   // cover group instances
   ////////////////////////////////////////////////////////////////////////////////
   // instances of the covergroups
   rvc_decoding rvc_dec= new;
   
   // and sample
   always@(posedge clock) begin
      if ( rvc_inst_valid ) begin
         rvc_dec.sample(inst_bits);
      end
   end
endmodule // minion_frontend_rvc_cvg


bind frontend_thread_buffer minion_frontend_rvc_cvg rvc_decode_cvg(.*);


   //TODO: combinacions rvc / non-rvc i alinemanet


`endif
