
// Sequence library for default sequences

class evl_dbg_noc_master_seq extends evl_sequence_mem;

   `uvm_object_utils(evl_dbg_noc_master_seq)

   `uvm_declare_p_sequencer(evl_dbg_noc_master_sequencer);

   virtual task body();
      evl_req_desc req_desc;

      forever begin
         p_sequencer.m_sa_snd_req_fifo.get(req_desc);
         p_sequencer.req_port.put(req_desc);
      end
   endtask : body

   function new(input string name_in = "evl_dbg_noc_master_seq");
      super.new(name_in);
   endfunction : new

endclass : evl_dbg_noc_master_seq

class evl_dbg_noc_slave_seq extends evl_sequence_mem;

   `uvm_object_utils(evl_dbg_noc_slave_seq)

   `uvm_declare_p_sequencer(evl_dbg_noc_slave_sequencer);

   virtual task body();
      evl_req_desc req_desc;

      forever begin
         p_sequencer.m_sa_snd_rsp_fifo.get(req_desc);
         `evl_log_addr(UVM_FULL, VMOD_DBG_NOC_DRV, get_abstract_name(), req_desc.get_paddr(), $sformatf("received a request descriptor (0x%x)", req_desc.get_bus_cmd()))
         p_sequencer.req_port.put(req_desc);
      end
   endtask : body

   function new(input string name_in = "evl_dbg_noc_slave_seq");
      super.new(name_in);
   endfunction : new

endclass : evl_dbg_noc_slave_seq
