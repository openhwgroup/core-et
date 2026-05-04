//--------------------------------------------------------------------------------------------------
//
// Class: evl_sc_scoreboard_cache_cover
//
class evl_sc_scoreboard_cache_cover extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Cover Group: cg_writearound_partial_qwens
   //
   evl_line_qword_en_t incoming_qwen;
   evl_line_qword_en_t cbuf_qwen;

   covergroup cg_writearound_partial_qwens();
      coverpoint incoming_qwen{ bins valid[] = {[1:15]};}
      coverpoint cbuf_qwen;
      incoming_x_cbuf: cross incoming_qwen, cbuf_qwen;
   endgroup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sample_writearound_partial_qwens
   //
   function void sample_writearound_partial_qwens(input evl_line_qword_en_t incoming_qwen_in, input evl_line_qword_en_t cbuf_qwen_in);
      incoming_qwen = incoming_qwen_in;
      cbuf_qwen     = cbuf_qwen_in;
      cg_writearound_partial_qwens.sample();
   endfunction : sample_writearound_partial_qwens


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      super.new();
      cg_writearound_partial_qwens = new();
   endfunction : new

endclass : evl_sc_scoreboard_cache_cover
