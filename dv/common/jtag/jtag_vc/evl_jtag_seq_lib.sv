
// Sequence library for default sequences

class evl_jtag_master_seq extends evl_sequence_mem;

   `uvm_object_utils(evl_jtag_master_seq)

   `uvm_declare_p_sequencer(evl_jtag_master_sequencer);

   virtual task body();
      evl_req_desc req_desc;

      forever begin
         p_sequencer.m_sa_snd_req_fifo.get(req_desc);
         p_sequencer.req_port.put(req_desc);
      end
   endtask : body

   function new(input string name_in = "evl_jtag_master_seq");
      super.new(name_in);
   endfunction : new

endclass : evl_jtag_master_seq
