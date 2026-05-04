//--------------------------------------------------------------------------------------------------
//
// Class: evl_cache
//
class evl_cache extends evl_cache_base#(evl_paddr_t, evl_line_desc);

   `uvm_component_utils(evl_cache)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   bit         m_mem_loaded      = 1'b0;
   evl_paddr_t m_cur_addr_mask   = evl_paddr_t'(0);
   evl_paddr_t m_cur_ignore_mask = evl_paddr_t'(0);
   evl_paddr_t m_cur_match_mask  = evl_paddr_t'(0);
   evl_paddr_t m_cur_match_value = evl_paddr_t'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_addr_template
   //
   virtual function void add_addr_template(input evl_paddr_t addr_mask_in,
                                           input evl_paddr_t addr_value_in,
                                           input bit         addr_ignore,
                                           input bit         clear_first = 1'b0);
      if (clear_first == 1'b1) begin
         m_cur_addr_mask   = evl_paddr_t'(0);
         m_cur_ignore_mask = evl_paddr_t'(0);
         m_cur_match_mask  = evl_paddr_t'(0);
         m_cur_match_value = evl_paddr_t'(0);
      end
      if (addr_mask_in != evl_paddr_t'(0)) begin
         m_cur_addr_mask |= addr_mask_in;
         if (addr_ignore == 1'b1) begin
            m_cur_ignore_mask |= addr_mask_in;
            m_cur_match_mask  &= ~m_cur_ignore_mask;
            m_cur_match_value &= m_cur_match_mask;
         end
         else begin
            m_cur_match_mask  |= (addr_mask_in & ~m_cur_ignore_mask);
            m_cur_match_value |= (addr_mask_in & addr_value_in);
            m_cur_match_value &= m_cur_match_mask;
         end
      end
   endfunction : add_addr_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: perform_load_mem
   //
   virtual function void perform_load_mem();
      int           fd;
      string        full_file_name;
      string        line;
      int           mod_limit;
      int           addr_shifts[$];
      evl_paddr_t   addr_masks[$];
      evl_paddr_t   retain_masks[$];
      evl_paddr_t   paddr;
      evl_paddr_t   last_addr;
      evl_line_desc line_desc;
      evl_line_desc line_desc_q[$];

      full_file_name = evl_tcl_find_file(m_file_name, 0);
      if (full_file_name == "") begin
         int limit = m_file_name.len() - 1;

         if (limit > 3) begin
            if (m_file_name.substr(limit - 3, limit) != ".hex") begin
               full_file_name = evl_tcl_find_file($sformatf("%s.hex", m_file_name), 0);
            end
         end
      end
      if (full_file_name == "") begin
         return;
      end

      addr_shifts  = {};
      addr_masks   = {};
      retain_masks = {};
      if (m_cur_addr_mask != evl_paddr_t'(0)) begin
         evl_paddr_t addr_mask   = m_cur_addr_mask;
         evl_paddr_t retain_mask = evl_paddr_t'(0);
         int         hi_shift    = 0;
         int         cur_shift   = 0;
         int         bit_index   = 0;

         while (addr_mask != 0) begin
            if (addr_mask[0:0] == 1'b1) begin
               while (addr_mask[0:0] == 1'b1) begin
                  bit_index++;
                  cur_shift++;
                  addr_mask >>= evl_paddr_t'(1);
               end
               addr_shifts.push_back(cur_shift);
               retain_masks.push_back(retain_mask);
               addr_masks.push_back(~retain_mask);
            end
            retain_mask = (retain_mask << 1) | evl_paddr_t'(1);
            bit_index++;
            addr_mask >>= evl_paddr_t'(1);
         end
      end
      mod_limit = addr_shifts.size();

      fd = $fopen(full_file_name, "r");
      last_addr = ~evl_paddr_t'(0);
      `evl_log(UVM_MEDIUM, VMOD_LOAD_MEM, get_abstract_name(), $sformatf("loading the memory image file \"%s\"", full_file_name))
      while ($fgets(line, fd) != 0) begin
         evl_paddr_t new_paddr;
         evl_word_t  data;

         if ($sscanf(line, "%x %x", paddr, data) == 2) begin
            if ((data != evl_word_t'(0)) || (m_random_data == 1'b1)) begin
               if (((m_cur_match_value ^ paddr) & m_cur_match_mask) == evl_paddr_t'(0)) begin
                  new_paddr = paddr;
                  if (m_cur_match_mask != evl_paddr_t'(0)) begin
                     for (int ii = 0; ii < mod_limit; ii++) begin
                        new_paddr &= retain_masks[ii];
                        new_paddr |= ((paddr >> addr_shifts[ii]) & addr_masks[ii]);
                     end
                  end
                  if ((new_paddr & const_line_paddr_mask) != last_addr) begin
                     line_desc = get_line_desc(new_paddr);
                     last_addr = new_paddr & const_line_paddr_mask;
                  end
                  // Using set_bytes as memImage.hex isn't necessarily aligned to word when using compressed
                  line_desc.set_bytes(new_paddr & ~const_line_paddr_mask, `EVL_WORD_BYTES, data, ~evl_word_byte_en_t'(0));
                  if (paddr != new_paddr) begin
                     `evl_log_addr(UVM_DEBUG, VMOD_LOAD_MEM, get_abstract_name(), paddr, $sformatf("converted address 0x%x to 0x%x (loaded 0x%x)", paddr, new_paddr, data))
                  end
               end
            end
         end
      end
      $fclose(fd);

      //
      // Now recreate the model memory in cache line order (there is no requirement that the image
      // file is in cache line order).
      //
      foreach (m_mdl_lines[paddr]) begin
         //
         // Validate partial lines with zero data
         //
         m_mdl_lines[paddr].validate_line(1'b0);
         line_desc_q.push_back(m_mdl_lines[paddr]);
      end
      if (line_desc_q.size() == 0) begin
         return;
      end
      line_desc_q.sort with ( item.m_tag );
      m_mdl_lines.delete();
      foreach (line_desc_q[ii]) begin
         m_mdl_lines[line_desc_q[ii].get_paddr()] = line_desc_q[ii];
      end
      line_desc_q.delete();

      //
      // Print the contents if desired.
      //
      if (`evl_log_check_verbosity(UVM_DEBUG, VMOD_LOAD_MEM) != 0) begin
         foreach (m_mdl_lines[paddr]) begin
            evl_line_data line_data;

            line_data = m_mdl_lines[paddr].get_line_data();
            line_data.print_obj(get_abstract_name(), paddr);
         end
      end
   endfunction : perform_load_mem


   //-----------------------------------------------------------------------------------------------
   //
   // Build Phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_cache", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_tag_mask(const_line_paddr_mask);
   endfunction : new

endclass : evl_cache
