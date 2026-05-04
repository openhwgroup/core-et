//--------------------------------------------------------------------------------------------------
//
// Class: evl_sys_reg_model
//
class evl_sys_reg_model extends evl_reg_block;

   `uvm_object_utils(evl_sys_reg_model)


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_reg_block
   //
   // This function is used to add a register block for a specific shire.
   //
   // Input Variables:
   //
   // block_num_in: This is the block number for the register block.
   // reg_block:    This is the register block to add.
   // hdl_path:     This is the HDL path for the block.
   //
   virtual function void add_evl_reg_block(input int block_num_in, input evl_reg_block reg_block, input string hdl_path_in = "");
      if (reg_block != null) begin
         m_evl_reg_blocks[reg_block.get_shire_id()] = reg_block;
         reg_block.configure(this, hdl_path_in);
      end
   endfunction : add_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_map
   //
   // This function adds the each of the shire register maps as subblock maps of the root map.
   //
   virtual function void add_evl_map(input uvm_reg_map reg_map);
      if (reg_map.get_name() == "AXI") begin
         foreach (m_evl_reg_blocks[ii]) begin
            add_evl_subblock_map(reg_map, ii, evl_paddr_t'(ii) * evl_paddr_t'(40'h0000400000));
         end
      end
      else begin
         foreach (m_evl_reg_blocks[ii]) begin
            add_evl_subblock_map(reg_map, ii, evl_paddr_t'(ii) * evl_paddr_t'(40'h0000000000));
         end
      end
   endfunction : add_evl_map


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_sys_reg_model", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction: new

endclass : evl_sys_reg_model
