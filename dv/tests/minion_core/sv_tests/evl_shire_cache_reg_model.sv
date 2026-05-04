//--------------------------------------------------------------------------------------------------
//
// Class: evl_shire_cache_reg_model
//
class evl_shire_cache_reg_model extends evl_reg_block;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_object_utils(evl_shire_cache_reg_model)

   int m_built = 0;


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
      if ((block_num_in >= 0) && (block_num_in < `SC_BANKS) && (reg_block != null)) begin
         m_evl_reg_blocks[block_num_in] = reg_block;
         reg_block.set_abstract_name($sformatf("%0s(RegBank%0d)", m_agent.get_root_abstract_name(), block_num_in));
         reg_block.set_agent(get_agent());
         reg_block.set_agent_id(block_num_in);
         reg_block.configure(this, hdl_path_in);
      end
   endfunction : add_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_map
   //
   virtual function void add_evl_map(input uvm_reg_map reg_map);
      foreach (m_evl_reg_blocks[ii]) begin
         if (m_evl_reg_blocks[ii] != null) begin
            if (reg_map.get_name() == "AXI") begin
               add_evl_subblock_map(reg_map, ii, (`EVL_SHIRE_CACHE_BASE_ESR & evl_paddr_t'(40'h00003fffff)) | (evl_paddr_t'(ii) << 13));
            end
         end
      end
   endfunction : add_evl_map


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build
   //
   // This function calls the build() function in each of the per-bank register blocks.
   //
   virtual function void build(input evl_paddr_t base_addr = evl_paddr_t'(0), input evl_paddr_t offset_addr = evl_paddr_t'(0));
      if (m_built++ != 0) begin
         return;
      end
      void'(create_map("AXI", uvm_reg_addr_t'(base_addr), 8, UVM_LITTLE_ENDIAN, 0));
      for (int ii = 0; ii < `SC_BANKS; ii++) begin
         if (m_evl_reg_blocks.exists(ii) && (m_evl_reg_blocks[ii] != null)) begin
            m_evl_reg_blocks[ii].build();
         end
      end
   endfunction : build


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_shire_cache_reg_model", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction: new

endclass
