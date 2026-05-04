`ifdef EVL_COVERAGE_ON

`ifndef __MINION_CORE_PROG_BUFFER_CVG
`define __MINION_CORE_PROG_BUFFER_CVG

`include "dv/minion_common/cov/instructions_table.svh"
package minion_core_prog_buffer_cov_pkg;
   covergroup cg_prog_buffer_instructions (ref logic [31:0] prog_buff_inst);
      option.per_instance         = 0;
      type_option.merge_instances = 1;

      cp_instructions: coverpoint prog_buff_inst {
         `instructions_table
      }

   endgroup

endpackage;

module minion_core_prog_buffer_cvg (input clock);
   import minion_core_prog_buffer_cov_pkg::*;
   logic             inst_valid;
   logic [31:0]      prog_buff_inst;
   
   logic [32*4-1:0]  reg_inst;
   
   cg_prog_buffer_instructions cg_prog_buffer_instructions_inst;
   
   initial begin
      cg_prog_buffer_instructions_inst = new(prog_buff_inst);
      forever @ (posedge clock) begin
         if (inst_valid === 1'b1) begin
            cg_prog_buffer_instructions_inst.sample();
         end
         inst_valid      <= thread_buffer.f7_inst_valid & thread_buffer.in_debug_mode;
         prog_buff_inst  <= thread_buffer.f7_inst_data.inst_bits;
      end
   end

endmodule: minion_core_prog_buffer_cvg

   bind frontend_thread_buffer minion_core_prog_buffer_cvg minion_core_prog_buffer_cov(.clock(clock));
`endif // __MINION_CORE_PROG_BUFFER_CVG

`endif // EVL_COVERAGE_ON

