//--------------------------------------------------------------------------------------------------
//
// Class: evl_reg_block
//
class evl_reg_block extends uvm_reg_block;

   `include "dv/common/base/evl_base_inc_name.svh"
   `include "dv/common/base/evl_base_inc.svh"

   `uvm_object_utils(evl_reg_block)

   evl_verif_comp m_agent;
   evl_reg_block  m_evl_reg_blocks[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent
   //
   // This function is used to set the shire ID for this component.
   //
   // Input Variables:
   //
   // shire_id_in: This is the shire ID to set for this component.
   //
   virtual function void set_agent(input evl_verif_comp agent_in);
      m_agent = agent_in;
   endfunction : set_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent
   //
   // This function is used to set the shire ID for this component.
   //
   // Input Variables:
   //
   // shire_id_in: This is the shire ID to set for this component.
   //
   virtual function evl_verif_comp get_agent();
      return m_agent;
   endfunction : get_agent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id
   //
   // This function is used to get the shire ID for this component.
   //
   // Return Value:
   //
   // evl_shire_id_t: This is the shire ID of this component.
   //
   virtual function evl_shire_id_t get_shire_id();
      if (m_agent != null) begin
         return m_agent.get_shire_id();
      end
      return ~evl_shire_id_t'(0);
   endfunction : get_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_agent_id
   //
   // This function is used to set the virtual agent ID for this component.
   //
   // Input Variables:
   //
   // agent_id_in: This is the virtual agent ID to use for this component.
   //
   virtual function void set_agent_id(input evl_agent_id_t agent_id_in);
      return;
   endfunction : set_agent_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_agent_id
   //
   // This function is used to get the virtual agent ID for this component.
   //
   // Return Value:
   //
   // evl_agent_id_t: This is the virtual agent ID of this component.
   //
   virtual function evl_agent_id_t get_agent_id();
      return ~evl_agent_id_t'(0);
   endfunction : get_agent_id


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
         int block_num;

         block_num = (block_num_in < 0) ? m_evl_reg_blocks.size() : block_num_in;
         m_evl_reg_blocks[block_num] = reg_block;
         reg_block.configure(this, hdl_path_in);
      end
   endfunction : add_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_evl_reg_block
   //
   // This function is used to Blah.
   //
   // Input Variables:
   //
   // agent_id_in: Blah.
   //
   virtual function evl_reg_block get_evl_reg_block(input int block_num);
      if (m_evl_reg_blocks.exists(block_num)) begin
         return m_evl_reg_blocks[block_num];
      end
      return null;
   endfunction : get_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_evl_reg_block_list
   //
   // This function is used to set the virtual agent ID for this component.
   //
   // Input Variables:
   //
   // agent_id_in: This is the virtual agent ID to use for this component.
   //
   virtual function int get_evl_reg_block_list(output evl_reg_block reg_block_list[]);
      evl_reg_block reg_block_queue[$];

      foreach (m_evl_reg_blocks[ii]) begin
         reg_block_queue.push_back(m_evl_reg_blocks[ii]);
      end
      if (reg_block_queue.size() == 0) begin
         return 0;
      end
      reg_block_list = new[reg_block_queue.size()];
      for (int ii = reg_block_queue.size() - 1; ii >= 0; ii--) begin
         reg_block_list[ii] = reg_block_queue[ii];
      end
      return reg_block_queue.size();
   endfunction : get_evl_reg_block_list


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_subblock_map
   //
   virtual function void add_evl_subblock_map(input uvm_reg_map reg_map, input int block_num, input evl_paddr_t offset_addr);
      if (m_evl_reg_blocks.exists(block_num) && (m_evl_reg_blocks[block_num] != null)) begin
         uvm_reg_map maps[$];
         uvm_reg_map block_map;
         uvm_reg_map subblock_map;

         //
         // Get a handle to the map in this register block.
         //
         get_maps(maps);
         foreach (maps[ii]) begin
            if (reg_map.get_name() == maps[ii].get_name()) begin
               block_map = maps[ii];
               break;
            end
         end
         if (block_map != null) begin
            //
            // Get a handle to the map in the child register block.
            //
            maps = {};
            m_evl_reg_blocks[block_num].get_maps(maps);
            foreach (maps[ii]) begin
               if (reg_map.get_name() == maps[ii].get_name()) begin
                  subblock_map = maps[ii];
                  break;
               end
            end
            if (subblock_map != null) begin
               subblock_map.add_parent_map(block_map, uvm_reg_addr_t'(offset_addr));

               //
               // Continue down the hierarchy ... attach subblock_map as a parent to its children.
               //
               m_evl_reg_blocks[block_num].add_evl_map(subblock_map);
            end
         end
      end
   endfunction : add_evl_subblock_map


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_evl_map
   //
   virtual function void add_evl_map(input uvm_reg_map reg_map);
      return;
   endfunction : add_evl_map


   //-----------------------------------------------------------------------------------------------
   //
   // Function: lock_evl_reg_block
   //
   virtual function void lock_evl_reg_block();
      lock_model();
      foreach (m_evl_reg_blocks[ii]) begin
         m_evl_reg_blocks[ii].lock_evl_reg_block();
      end
   endfunction : lock_evl_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_reg_mask
   //
   virtual function evl_dword_t get_reg_mask(input string name);
      return evl_dword_t'(0);
   endfunction : get_reg_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_reg_test_mask
   //
   virtual function evl_dword_t get_reg_test_mask(input string name);
      return evl_dword_t'(0);
   endfunction : get_reg_test_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: build
   //
   // This function calls the build() function in each of the per-bank register blocks.
   //
   virtual function void build(input evl_paddr_t base_addr = evl_paddr_t'(0), input evl_paddr_t offset_addr = evl_paddr_t'(0));
      return;
   endfunction : build


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_reg_block", input int has_coverage = int'(UVM_NO_COVERAGE));
      super.new(name_in, has_coverage);
   endfunction: new

endclass : evl_reg_block
