//--------------------------------------------------------------------------------------------------
//
// Class: evl_addr_alloc
//
class evl_addr_alloc extends evl_object;

   `uvm_object_utils(evl_addr_alloc)

   typedef int evl_subbank_list_t[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // m_shire_id:             This is the shire ID for this component.  If this component is not
   //                         local to a shire, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_active_mem_descs[]:   This is a list of active descriptors (refer to m_active_desc_count).
   //
   static evl_addr_alloc m_addr_alloc_tmp = new();
   static evl_addr_alloc m_addr_alloc;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_shire_id:             This is the shire ID for this component.  If this component is not
   //                         local to a shire, the value will be all 1s.
   //                         ----------------------------------------------------------------------
   // m_active_mem_descs[]:   This is a list of active descriptors (refer to m_active_desc_count).
   //


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_inst
   //
   static function evl_addr_alloc get_inst();
      if (m_addr_alloc == null) begin
         evl_addr_alloc evl_addr_alloc;

         evl_addr_alloc = new();
      end
      return m_addr_alloc;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_random_addresses
   //
   static function int get_random_addresses(output evl_paddr_t addrs[],
                                            input  int         shire_in,
                                            input  int         addr_type,
                                            input  bit         use_local_in     = 1'b1,
                                            input  bit         use_remote_in    = 1'b0,
                                            input  int         use_shire_in     = -1,
                                            input  int         bank_count_in    = 1,
                                            input  int         subbank_count_in = 1,
                                            input  int         index_count_in   = 1,
                                            input  int         alias_count_in   = 1,
                                            input  int         bank_in          = -1,
                                            input  int         subbank_in       = -1,
                                            input  int         index_in         = -1);
      if (m_addr_alloc == null) begin
         evl_addr_alloc evl_addr_alloc;

         evl_addr_alloc = new();
      end
      return m_addr_alloc.get_random_addresses_local(addrs, shire_in, addr_type, use_local_in, use_remote_in, use_shire_in, bank_count_in, subbank_count_in, index_count_in, alias_count_in, bank_in, subbank_in, index_in);
   endfunction : get_random_addresses


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_random_aliases
   //
   static function int get_random_aliases(output evl_paddr_t addrs_out[],
                                          input  evl_paddr_t addrs_in[],
                                          input  int         shire_in,
                                          input  int         addr_type,
                                          input  int         alias_count_in = 1);
      if (m_addr_alloc == null) begin
         evl_addr_alloc evl_addr_alloc;

         evl_addr_alloc = new();
      end
      return m_addr_alloc.get_random_aliases_local(addrs_out, addrs_in, shire_in, addr_type, alias_count_in);
   endfunction : get_random_aliases


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_random_addresses_local
   //
   function int get_random_addresses_local(output evl_paddr_t addrs[],
                                           input  int         shire_in,
                                           input  int         addr_type,
                                           input  bit         use_local_in     = 1'b1,
                                           input  bit         use_remote_in    = 1'b0,
                                           input  int         use_shire_in     = -1,
                                           input  int         bank_count_in    = 1,
                                           input  int         subbank_count_in = 1,
                                           input  int         index_count_in   = 1,
                                           input  int         alias_count_in   = 1,
                                           input  int         bank_in          = -1,
                                           input  int         subbank_in       = -1,
                                           input  int         index_in         = -1);
      evl_sc_config_attr_s sc_config;
      bit                  use_local     = 1'b0;
      bit                  use_remote    = 1'b0;
      bit                  use_even      = 1'b0;
      bit                  use_odd       = 1'b0;
      int                  shire         = -1;
      int                  alias_count   = (alias_count_in   <= 0) ? 1 : alias_count_in;
      int                  index_count   = (index_count_in   <= 0) ? 1 : (index_in >= 0) ? 1 : index_count_in;
      int                  bank_count    = (bank_count_in    <= 0) ? 1 : (index_in >= 0) ? 1 : bank_count_in;
      int                  subbank_count = (subbank_count_in <= 0) ? 1 : (index_in >= 0) ? 1 : subbank_count_in;
      evl_paddr_t          shire_ids[$];
      evl_paddr_t          shire_line_mask;
      evl_paddr_t          addr_and_mask;
      evl_paddr_t          addr_or_mask;

      if (shire_in < 0) begin
         addrs = new[0];
         return 0;
      end

      if (addr_type == `EVL_GEN_ADDR_MEM) begin
         if (use_shire_in >= 0) begin
            shire = use_shire_in & evl_paddr_t'(7);
         end
         else begin
            shire = shire_in & evl_paddr_t'(7);
         end
         if (use_local_in == 1'b1) begin
            use_even = 1'b1;
            use_odd  = 1'b0;
         end
         else if (use_remote_in == 1'b1) begin
            use_even = 1'b0;
            use_odd  = 1'b1;
         end
         else begin
            use_even = 1'b0;
            use_odd  = 1'b0;
         end
      end
      else begin
         if (use_shire_in >= 0) begin
            use_local  = 1'b0;
            use_remote = 1'b0;
            shire      = use_shire_in % `NUM_SHIRES;
         end
         else if (use_local_in == 1'b1) begin
            use_local  = 1'b1;
            use_remote = 1'b0;
         end
         else if ((use_local_in == 1'b0) && (use_remote_in == 1'b1)) begin
            use_local  = 1'b0;
            use_remote = 1'b1;
         end

         //
         // Get the shire cache configuration for the requesting shire.
         //
         sc_config = m_global_attrs.get_sc_config(shire_in);

         //
         // If allocating local addresses, create a "list" of local shire IDs that will map to this
         // shire.  This list is not necessarily a true list of shire IDs for this shire ... it
         // depends on what type of shire aliasing is being performed (and for that reason we have a
         // list of local shire IDs).
         //
         // If allocating remote addresses, create a "list" of remote shire IDs that will map to
         // anything *but* this shire.  The type of aliasing used needs to be taken into account as
         // well.
         //
         // If allocating a specific shire ID, the list is simply that shire ID.
         //
         // If none of the above, create a complete list of available shire IDs.
         //
         shire_line_mask = m_global_attrs.convert_to_l3_address(0, 0, `SC_L3_SHIRES - 1) >> `EVL_LINE_ADDR_LSB;
         if (use_local == 1'b1) begin
            shire_ids.push_back(shire_in);
         end
         else if (use_remote == 1'b1) begin
            for (int ii = 0; ii < `SC_L3_SHIRES; ii++) begin
               if (ii != int'(shire_in)) begin
                  shire_ids.push_back(ii);
               end
            end
         end
         else if ((shire >= 0) && (shire < `SC_L3_SHIRES)) begin
            shire_ids.push_back(shire);
         end
         else begin
            for (int ii = 0; ii < `SC_L3_SHIRES; ii++) begin
               shire_ids.push_back(ii);
            end
            shire_line_mask = evl_paddr_t'(0);
         end
      end

      //
      // If we have a real NoC or real memory shires, limit the universe of available addresses.
      //
      if ((evl_verif_comp_map::get_verif_comp(NOC_RTL_AGENT) != null) || (evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT) != null)) begin
         addr_and_mask = evl_paddr_t'(40'h03ffffffff);
         addr_or_mask  = evl_paddr_t'(40'h8000000000);
      end
      else begin
         addr_and_mask = evl_paddr_t'(40'h3fffffffff);
         addr_or_mask  = evl_paddr_t'(40'h8000000000);
      end

      if (addr_type == `EVL_GEN_ADDR_L2) begin
         int                count;
         int                bank_min;
         int                bank_max;
         int                subbank_min;
         int                subbank_max;
         int                index_min;
         int                index_max;
         int                addr_index;
         evl_paddr_t        bank_subbank_mask;
         int                index_overlap;
         int                upper_index_mask;
         int                max_subbanks[`SC_BANKS];
         evl_subbank_list_t bank_subbanks[`SC_BANKS];
         int                bank_subbank_ids[$];
         evl_paddr_t        lower_offsets[$];
         evl_paddr_t        avail_upper_offsets[$];
         evl_paddr_t        upper_offsets[$];
         evl_paddr_t        alias_addrs[$];
         evl_paddr_t        shire_line_addrs[$];

         //
         // For the L2, the bank, subbank, and index select bits may overlap the shire ID field.
         // Let's start by finding any overlap.
         //
         // These overlap masks are normalized.  Thus, the bank overlap mask will potentially go
         // from 0 to essentially `SC_BANKS - 1.  Similarly, the subbank overlap mask will
         // potentially go from 0 to essentially `SC_SUB_BANKS - 1, etc.
         //
         // To ease the comparison burden, we also create a bank_subbank_mask, which is a
         // concatention of the subbank mask and the bank mask.
         //
         // Note that upper_index_mask is simply a normalized mask of L2 index bits that lie outside
         // of the fixed bank, subbank, and shire ID fields.
         //
         bank_subbank_mask = evl_paddr_t'((1 << (`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)) - 1) & shire_line_mask;
         upper_index_mask  = int'(sc_config.l2_index_mask >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE));
         index_overlap     = int'(shire_line_mask >> (`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE));

         foreach (shire_ids[ii]) begin
            shire_line_addrs.push_back(m_global_attrs.convert_to_l3_address(0, 0, shire_ids[ii]) >> `EVL_LINE_ADDR_LSB);
         end

         //
         // Paw through the shire ID list to generate a potential list of available banks and
         // subbanks.  At the end of this routine we will have filled out the max_subbanks[] array
         // (which is a list of the number of subbank IDs that can be paired with a specific bank
         // ID) and the bank_subbanks[][$] array (which is a list of queues of the valid subbank IDs
         // that can be paired with a specific bank ID).
         //
         count = 0;
         if ((bank_in >= `SC_BANKS) || (subbank_in >= `SC_SUB_BANKS)) begin
            addrs = new[0];
            return 0;
         end
         if (bank_in >= 0) begin
            bank_min = bank_in;
            bank_max = bank_in + 1;
            for (int ii = 0; ii < `SC_BANKS; ii++) begin
               bank_subbanks[ii] = {};
               max_subbanks[ii]  = 0;
            end
         end
         else begin
            bank_min = 0;
            bank_max = `SC_BANKS;
         end

         if (subbank_in >= 0) begin
            subbank_min = subbank_in;
            subbank_max = subbank_in + 1;
         end
         else begin
            subbank_min = 0;
            subbank_max = `SC_SUB_BANKS;
         end

         for (int ii = bank_min; ii < bank_max; ii++) begin
            int match_count;

            match_count       = 0;
            bank_subbanks[ii] = {};
            for (int jj = subbank_min; jj < subbank_max; jj++) begin
               int bank_subbank = ii | (jj << `SC_BANK_ID_SIZE);

               //
               // Determine if the concatenated { subbank, bank } combination can actually be used
               // given the list of shire IDs.
               //
               foreach (shire_line_addrs[kk]) begin
                  if (((shire_line_addrs[kk] ^ bank_subbank) & bank_subbank_mask) == 0) begin
                     bank_subbanks[ii].push_back(jj);
                     match_count++;
                     break;
                  end
               end
            end

            //
            // If any concatenated { subbank, bank } combination can actually be used, then the
            // number of available banks can be incremented.
            //
            max_subbanks[ii] = match_count;
            if (match_count > 0) begin
               count++;
            end
         end

         //
         // Make sure we only extract as many banks as the number available.  In other words, cap
         // the bank_count value to the maximum number of banks available.
         //
         if (count == 0) begin
            addrs = new[0];
            return 0;
         end
         if (bank_count > count) begin
            bank_count = count;
         end

         //
         // If the bank count is 1, we actually have a relatively easy time generating the desired
         // subblock count.  If not, we punt (and may actually fulfill the desired result).
         //
         repeat (bank_count) begin
            int avail_bank_ids[$];
            int max_subbank;
            int selection;
            int bank_id;

            //
            // Find the maximum number of subbanks from the available list of banks.
            //
            avail_bank_ids = {};
            max_subbank    = 0;
            for (int ii = 0; ii < `SC_BANKS; ii++) begin
               if (max_subbank < max_subbanks[ii]) begin
                  max_subbank = max_subbanks[ii];
               end
            end

            //
            // Now alter the maximum number of subbanks we can use based on the maximum number
            // available.
            //
            max_subbank = (max_subbank > subbank_count) ? subbank_count : max_subbank;

            //
            // Now create a list of banks to use based on the number of available subbanks.  The
            // resulting avail_bank_ids[$] list contains all of the banks IDs that meet the
            // established criteria.
            //
            foreach (max_subbanks[ii]) begin
               if (max_subbanks[ii] >= max_subbank) begin
                  avail_bank_ids.push_back(ii);
               end
            end

            //
            // Now select a bank from the list (if possible).
            //
            if (avail_bank_ids.size() == 0) begin
               break;
            end
            selection = $urandom % avail_bank_ids.size();
            bank_id   = avail_bank_ids[selection];

            //
            // Now that we have selected a bank, prevent it from getting selected again.
            //
            max_subbanks[bank_id] = -1;

            //
            // Now select subbanks from the available list of subbanks for that bank.
            //
            for (int ii = 0; ii < max_subbank; ii++) begin
               int bank_subbank_id = bank_id;

               selection = $urandom % bank_subbanks[bank_id].size();
               bank_subbank_id |= (bank_subbanks[bank_id][selection] << `SC_BANK_ID_SIZE);
               bank_subbank_ids.push_back(bank_subbank_id);
               bank_subbanks[bank_id].delete(selection);
            end
         end

         //
         // Now based on the given banks and subbanks, created a list indices to support them.  L2
         // indices may actually be divided into two parts ... a shire-overlapping part, and a
         // non-overlapping part.
         //
         if ((index_overlap == 0) && (index_in < 0)) begin
            foreach (bank_subbank_ids[ii]) begin
               lower_offsets.push_back(evl_paddr_t'(bank_subbank_ids[ii]) << `EVL_LINE_ADDR_LSB);
            end
         end
         else begin
            int index_selections[$];

            if (index_in >= 0) begin
               index_min = index_in & index_overlap;
               index_max = index_in & index_overlap;
            end
            else begin
               index_min = 0;
               index_max = index_overlap;
            end

            //
            // Create a list of overlapping indices that satisfy all of the selected banks and
            // subbanks.
            //
            for (int ii = index_min; ii <= index_max; ii++) begin
               int index_select = ii << (`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
               int count        = 0;

               for (int jj = bank_subbank_ids.size() - 1; jj >= 0; jj--) begin
                  int full_index = index_select | bank_subbank_ids[jj];

                  foreach (shire_line_addrs[kk]) begin
                     if (((shire_line_addrs[kk] ^ full_index) & shire_line_mask) == 0) begin
                        count++;
                        break;
                     end
                  end
               end
               if (count == bank_subbank_ids.size()) begin
                  index_selections.push_back(index_select);
               end
            end
            //
            // If a number meet the criteria, randomly choose one.  If none of them do, attempt to
            // find matches wherever possible.
            //
            if (index_selections.size() > 0) begin
               int index_select;

               index_select = index_selections[$urandom % index_selections.size()];
               foreach (bank_subbank_ids[ii]) begin
                  lower_offsets.push_back(evl_paddr_t'(bank_subbank_ids[ii] | index_select) << `EVL_LINE_ADDR_LSB);
               end
            end
            else begin
               for (int ii = index_min; ii <= index_max; ii++) begin
                  int index_select = ii << (`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
                  int count        = 0;

                  for (int jj = bank_subbank_ids.size() - 1; jj >= 0; jj--) begin
                     int full_index = index_select | bank_subbank_ids[jj];

                     foreach (shire_line_addrs[kk]) begin
                        if (((shire_line_addrs[kk] ^ full_index) & shire_line_mask) == 0) begin
                           lower_offsets.push_back(evl_paddr_t'(full_index) << `EVL_LINE_ADDR_LSB);
                           bank_subbank_ids.delete(jj);
                           break;
                        end
                     end
                  end
                end
            end
         end

         //
         // Create a list of available upper index offsets to use for randomization.  Upper offsets
         // lie inside the universe of L2 indices and beyond the bank, subbank, and shire ID fields.
         //
         if (index_in >= 0) begin
            int bit_shift;

            bit_shift = `SC_L3_SHIRE_ID_SIZE - (`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
            if (bit_shift < 0) begin
               bit_shift = (`SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE) - `SC_L3_SHIRE_ID_SIZE;
            end
            index_min = int'((index_in >> bit_shift) & (sc_config.l2_index_mask >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE)));
            index_max = int'((index_in >> bit_shift) & (sc_config.l2_index_mask >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE)));
         end
         else begin
            index_min = 0;
            index_max = upper_index_mask;
         end
         for (int ii = index_min; ii <= index_max; ii++) begin
            avail_upper_offsets.push_back(evl_paddr_t'(ii) << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE));
         end

         //
         // Make sure we only extract as many upper indices as the number available.  In other
         // words, cap the index_count value to the maximum number of indices available.
         //
         index_count = (index_count > avail_upper_offsets.size()) ? avail_upper_offsets.size() : index_count;

         //
         // Create a list of random upper index offsets.
         //
         repeat (index_count) begin
            int selection;

            selection = $urandom % avail_upper_offsets.size();
            upper_offsets.push_back(avail_upper_offsets[selection]);
            avail_upper_offsets.delete(selection);
         end

         //
         // Now generate the aliases (which lie outside the L2 index).  All aliases must be from
         // 512G to 1T.
         //
         if (alias_count > 32) begin
            alias_count = 32;
         end
         if (sc_config.l2_alias_mask > sc_config.l2_index_mask) begin
            while (alias_count > 0) begin
               evl_paddr_t addr;

               addr = ((evl_paddr_t'($urandom) | (evl_paddr_t'($urandom) << 32)) & sc_config.l2_alias_mask & addr_and_mask) | addr_or_mask;
               if (!(addr inside { alias_addrs })) begin
                  alias_addrs.push_back(addr);
                  alias_count--;
               end
            end
         end
         if (alias_addrs.size() == 0) begin
            alias_addrs.push_back(addr_or_mask);
         end

         //
         // Now create a list of addresses to use.
         //
         addr_index = upper_offsets.size() * lower_offsets.size() * alias_addrs.size();
         if (addr_index == 0) begin
            addrs = new[0];
         end
         else begin
            addrs = new[addr_index];
            addr_index = 0;
            foreach (lower_offsets[ii]) begin
               foreach (upper_offsets[jj]) begin
                  foreach (alias_addrs[kk]) begin
                     evl_paddr_t addr = alias_addrs[kk] | upper_offsets[jj] | lower_offsets[ii];

                     if (!(addr inside { addrs })) begin
                        addrs[addr_index++] = addr;;
                     end
                  end
               end
            end
         end
         return addr_index;
      end
      else if (addr_type == `EVL_GEN_ADDR_L3) begin
         int         addr_index;
         int         index_mask;
         int         bank_min;
         int         bank_max;
         int         subbank_min;
         int         subbank_max;
         int         index_min;
         int         index_max;
         int         bank_ids[$];
         int         subbank_ids[$];
         evl_paddr_t index_addrs[$];
         evl_paddr_t alias_addrs[$];

         if ((bank_in >= `SC_BANKS) || (subbank_in >= `SC_SUB_BANKS)) begin
            addrs = new[0];
            return 0;
         end

         index_mask = int'(sc_config.l3_index_mask >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE));

         if (bank_in >= 0) begin
            bank_min = bank_in;
            bank_max = bank_in + 1;
         end
         else begin
            bank_min = 0;
            bank_max = `SC_BANKS;
         end

         if (subbank_in >= 0) begin
            subbank_min = subbank_in;
            subbank_max = subbank_in + 1;
         end
         else begin
            subbank_min = 0;
            subbank_max = `SC_SUB_BANKS;
         end

         if (index_in >= 0) begin
            index_min = index_in;
            index_max = index_in;
         end
         else begin
            index_min = 0;
            index_max = index_mask;
         end

         for (int ii = bank_min; ii < bank_max; ii++) begin
            bank_ids.push_back(ii);
         end

         for (int ii = subbank_min; ii < subbank_max; ii++) begin
            subbank_ids.push_back(ii);
         end

         //
         // Make sure we only extract as many banks as the number available.  In other words, cap
         // the bank_count value to the maximum number of banks available.
         //
         if (bank_count > bank_ids.size()) begin
            bank_count = bank_ids.size();
         end
         for (int ii = bank_ids.size() - bank_count; ii > 0; ii--) begin
            bank_ids.delete($urandom % bank_ids.size());
         end

         //
         // Make sure we only extract as many subbanks as the number available.  In other words, cap
         // the subbank_count value to the maximum number of subbanks available.
         //
         if (subbank_count > subbank_ids.size()) begin
            subbank_count = subbank_ids.size();
         end
         for (int ii = subbank_ids.size() - subbank_count; ii > 0; ii--) begin
            subbank_ids.delete($urandom % subbank_ids.size());
         end

         //
         // For the L3, the { bank, subbank, shire ID } and index select bits are dependent.  We
         // will be using the available shire IDs along with all available index masks to create a
         // list of available indices.
         //
         if (index_count > (index_mask + 1 + shire_ids.size())) begin
            index_count = index_mask + 1 + shire_ids.size();
         end

         if ((index_count < ((index_mask + 1 + shire_ids.size()) / 2)) && (shire_ids.size() > 0)) begin
            int shire_limit;

            shire_limit = shire_ids.size();
            while (index_count > 0) begin
               evl_paddr_t index;

               index = evl_paddr_t'($urandom % (index_mask + 1)) << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE) | m_global_attrs.convert_to_l3_address(0, 0, shire_ids[$urandom % shire_limit]);
               if (!(index inside { index_addrs })) begin
                  index_addrs.push_back(index);
                  index_count--;
               end
            end
         end
         else begin
            evl_paddr_t avail_indices[$];

            for (int ii = 0; ii <= index_mask; ii++) begin
               evl_paddr_t index;

               index = evl_paddr_t'(ii) << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE + `SC_L3_SHIRE_ID_SIZE);
               foreach (shire_ids[jj]) begin
                  avail_indices.push_back(index | m_global_attrs.convert_to_l3_address(0, 0, shire_ids[jj]));
               end
            end
            while ((avail_indices.size() > 0) && (index_count > 0)) begin
               int selection;

               selection = $urandom % avail_indices.size();
               index_addrs.push_back(avail_indices[selection]);
               avail_indices.delete(selection);
               index_count--;
            end
         end

         if (index_addrs.size() == 0) begin
            index_addrs.push_back(0);
         end

         //
         // Now generate the aliases (which lie outside the L3 index).  All aliases must be from
         // 512G to 1T.
         //
         if (alias_count > 32) begin
            alias_count = 32;
         end
         if (sc_config.l3_alias_mask > sc_config.l3_index_mask) begin
            while (alias_count > 0) begin
               evl_paddr_t addr;

               addr = ((evl_paddr_t'($urandom) | (evl_paddr_t'($urandom) << 32)) & sc_config.l3_alias_mask & addr_and_mask) | addr_or_mask;
               if (!(addr inside { alias_addrs })) begin
                  alias_addrs.push_back(addr);
                  alias_count--;
               end
            end
         end
         if (alias_addrs.size() == 0) begin
            alias_addrs.push_back(addr_or_mask);
         end

         //
         // Now create a list of addresses to use.
         //
         addr_index = bank_count * subbank_count * index_addrs.size() * alias_addrs.size();
         if (addr_index == 0) begin
            addrs = new[0];
         end
         else begin
            addrs = new[addr_index];
            addr_index = 0;
            foreach (bank_ids[ii]) begin
               foreach (subbank_ids[jj]) begin
                  foreach (index_addrs[kk]) begin
                     foreach (alias_addrs[ll]) begin
                        evl_paddr_t addr = alias_addrs[ll] | index_addrs[kk] | m_global_attrs.convert_to_l3_address(bank_ids[ii], subbank_ids[jj], 0);

                        if (!(addr inside { addrs })) begin
                           addrs[addr_index++] = addr;;
                        end
                     end
                  end
               end
            end
         end
         return addr_index;
      end
      else if (addr_type == `EVL_GEN_ADDR_SCP) begin
         int         addr_index;
         int         index_mask;
         int         bank_min;
         int         bank_max;
         int         subbank_min;
         int         subbank_max;
         int         index_min;
         int         index_max;
         int         bank_ids[$];
         int         subbank_ids[$];
         evl_paddr_t index_addrs[$];
         evl_paddr_t index_limit;
         evl_paddr_t shire_addr;

         if (shire >= 0) begin
            shire_addr = evl_paddr_t'(shire) & ((1 << `SC_SCP_SHIRE_ID_SIZE) - 1);
         end
         else if (use_local == 1'b1) begin
            shire_addr = evl_paddr_t'(shire_in) & ((1 << `SC_SCP_SHIRE_ID_SIZE) - 1);
         end
         else if (use_remote == 1'b1) begin
            shire_addr = evl_paddr_t'($urandom % `SC_SCP_SHIRES) & ((1 << `SC_SCP_SHIRE_ID_SIZE) - 1);
            while (int'(shire_addr) == shire_in) begin
               shire_addr = evl_paddr_t'($urandom % `SC_SCP_SHIRES) & ((1 << `SC_SCP_SHIRE_ID_SIZE) - 1);
            end
         end
         else begin
            shire_addr = evl_paddr_t'($urandom % `SC_SCP_SHIRES) & ((1 << `SC_SCP_SHIRE_ID_SIZE) - 1);
         end
         shire_addr  = (shire_addr << `SC_SCP_SHIRE_LSB) | (evl_paddr_t'(`SC_SCP_REGION_ID) << `SC_SCP_REGION_LSB);
         index_mask  = int'(sc_config.scp_index_mask >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE));
         index_limit = int'(sc_config.scp_top_addr >> (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)) & index_mask;

         if ((bank_in >= `SC_BANKS) || (subbank_in >= `SC_SUB_BANKS)) begin
            addrs = new[0];
            return 0;
         end
         if (bank_in >= 0) begin
            bank_min = bank_in;
            bank_max = bank_in + 1;
         end
         else begin
            bank_min = 0;
            bank_max = `SC_BANKS;
         end

         if (subbank_in >= 0) begin
            subbank_min = subbank_in;
            subbank_max = subbank_in + 1;
         end
         else begin
            subbank_min = 0;
            subbank_max = `SC_SUB_BANKS;
         end

         if (index_in >= 0) begin
            index_min = index_in;
            index_max = index_in;
         end
         else begin
            index_min = 0;
            index_max = index_mask;
         end

         for (int ii = bank_min; ii < bank_max; ii++) begin
            bank_ids.push_back(ii);
         end

         for (int ii = subbank_min; ii < subbank_max; ii++) begin
            subbank_ids.push_back(ii);
         end

         //
         // Make sure we only extract as many banks as the number available.  In other words, cap
         // the bank_count value to the maximum number of banks available.
         //
         if (bank_count > bank_ids.size()) begin
            bank_count = bank_ids.size();
         end
         for (int ii = bank_ids.size() - bank_count; ii > 0; ii--) begin
            bank_ids.delete($urandom % bank_ids.size());
         end

         //
         // Make sure we only extract as many subbanks as the number available.  In other words, cap
         // the subbank_count value to the maximum number of subbanks available.
         //
         if (subbank_count > subbank_ids.size()) begin
            subbank_count = subbank_ids.size();
         end
         for (int ii = subbank_ids.size() - subbank_count; ii > 0; ii--) begin
            subbank_ids.delete($urandom % subbank_ids.size());
         end

         if (index_in >= 0) begin
            index_addrs.push_back((index_in << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE)) & sc_config.scp_index_mask);
         end
         else begin
            if (index_count > (index_mask + 1)) begin
               index_count = index_mask + 1;
            end

            if (index_count < ((index_mask + 1) / 2)) begin
               int not_available = 0;

               while (index_count > 0) begin
                  evl_paddr_t index;

                  index = evl_paddr_t'($urandom % (index_mask + 1));
                  index = index << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
                  if (index > index_limit) begin
                     not_available++;
                  end
                  else if (!(index inside { index_addrs })) begin
                     index_addrs.push_back(index);
                     index_count--;
                  end
                  else begin
                     not_available++;
                  end
                  if (not_available > 10) begin
                     not_available = 0;
                     index_count--;
                  end
               end
            end
            else begin
               evl_paddr_t avail_indices[$];

               for (int ii = 0; ii <= index_limit; ii++) begin
                  evl_paddr_t index;

                  index = evl_paddr_t'(ii) << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE + `SC_SUB_BANK_ID_SIZE);
                  avail_indices.push_back(index);
               end
               while ((avail_indices.size() > 0) && (index_count > 0)) begin
                  int selection;

                  selection = $urandom % avail_indices.size();
                  index_addrs.push_back(avail_indices[selection]);
                  avail_indices.delete(selection);
                  index_count--;
               end
            end
         end

         if (index_addrs.size() == 0) begin
            index_addrs.push_back(0);
         end

         //
         // Now create a list of addresses to use.
         //
         addr_index = bank_count * subbank_count * index_addrs.size();
         if (addr_index == 0) begin
            addrs = new[0];
         end
         else begin
            addrs = new[addr_index];
            addr_index = 0;
            foreach (bank_ids[ii]) begin
               foreach (subbank_ids[jj]) begin
                  evl_paddr_t bank_subbank;

                  bank_subbank = (evl_paddr_t'(bank_ids[ii]) << `EVL_LINE_ADDR_LSB) | (evl_paddr_t'(subbank_ids[jj]) << (`EVL_LINE_ADDR_LSB + `SC_BANK_ID_SIZE));
                  foreach (index_addrs[kk]) begin
                     evl_paddr_t addr = shire_addr | index_addrs[kk] | bank_subbank;

                     if (!(addr inside { addrs })) begin
                        addrs[addr_index++] = addr;;
                     end
                  end
               end
            end
         end
         return addr_index;
      end
      else if (addr_type == `EVL_GEN_ADDR_MEM) begin
         int         addr_index;
         evl_paddr_t index_addrs[$];
         evl_paddr_t shire_addr;

         shire_addr = (evl_paddr_t'(shire) & evl_paddr_t'(7)) << `EVL_LINE_ADDR_LSB;
         if (use_even == 1'b1) begin
            addr_and_mask = ~(evl_paddr_t'(8) << `EVL_LINE_ADDR_LSB) & addr_and_mask;
         end
         else if (use_odd == 1'b1) begin
            addr_or_mask = addr_or_mask | (evl_paddr_t'(8) << `EVL_LINE_ADDR_LSB);
         end

         if (index_in >= 0) begin
            index_addrs.push_back(index_in << (`EVL_LINE_ADDR_LSB + 3));
         end
         else begin
            if (index_count > 64) begin
               index_count = 64;
            end

            while (index_count > 0) begin
               evl_paddr_t index;

               index = ((evl_paddr_t'($urandom) << (`EVL_LINE_ADDR_LSB + 3)) & addr_and_mask) | addr_or_mask;
               if (!(index inside { index_addrs })) begin
                  index_addrs.push_back(index);
                  index_count--;
               end
            end
         end

         if (index_addrs.size() == 0) begin
            index_addrs.push_back(0);
         end

         //
         // Now create a list of addresses to use.
         //
         addr_index = index_addrs.size();
         if (addr_index == 0) begin
            addrs = new[0];
         end
         else begin
            addrs = new[addr_index];
            addr_index = 0;
            foreach (index_addrs[ii]) begin
               evl_paddr_t addr = shire_addr | index_addrs[ii];

               if (!(addr inside { addrs })) begin
                  addrs[addr_index++] = addr;;
               end
            end
         end
         return addr_index;
      end
      addrs = new[0];
      return 0;
   endfunction : get_random_addresses_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_random_aliases_local
   //
   function int get_random_aliases_local(output evl_paddr_t addrs_out[],
                                         input  evl_paddr_t addrs_in[],
                                         input  int         shire_in,
                                         input  int         addr_type,
                                         input  int         alias_count_in = 1);
      evl_sc_config_attr_s sc_config;
      int                  alias_count = (alias_count_in <= 0) ? 1 : alias_count_in;
      evl_paddr_t          index_addrs[$];
      evl_paddr_t          alias_addrs[$];
      evl_paddr_t          new_alias_addrs[$];
      evl_paddr_t          addr_and_mask;
      evl_paddr_t          addr_or_mask;
      int                  addr_index;

      //
      // If we have a real NoC or real memory shires, limit the universe of available addresses.
      //
      if ((evl_verif_comp_map::get_verif_comp(NOC_RTL_AGENT) != null) || (evl_verif_comp_map::get_verif_comp(MEM_SHIRE_RTL_AGENT) != null)) begin
         addr_and_mask = evl_paddr_t'(40'h03ffffffff);
         addr_or_mask  = evl_paddr_t'(40'h8000000000);
      end
      else begin
         addr_and_mask = evl_paddr_t'(40'h3fffffffff);
         addr_or_mask  = evl_paddr_t'(40'h8000000000);
      end

      //
      // Get the shire cache configuration for the requesting shire.
      //
      sc_config = m_global_attrs.get_sc_config(shire_in);

      if (addr_type == `EVL_GEN_ADDR_L2) begin
         //
         // Extract indices and aliases from the incoming list of addresses.
         //
         foreach (addrs_in[ii]) begin
            evl_paddr_t addr;

            addr = addrs_in[ii] & sc_config.l2_index_mask;
            if (!(addr inside { index_addrs })) begin
               index_addrs.push_back(addr);
            end
            addr = (addrs_in[ii] & sc_config.l2_alias_mask & addr_and_mask) | addr_or_mask;
            if (!(addr inside { alias_addrs })) begin
               alias_addrs.push_back(addr);
            end
         end

         if ((index_addrs.size() == 0) || (alias_addrs.size() == 0)) begin
            addrs_out = new[0];
            return 0;
         end

         //
         // Now generate the aliases (which lie outside the L2 index).  All aliases must be from
         // 512G to 1T.
         //
         if (alias_count > 32) begin
            alias_count = 32;
         end
         if (sc_config.l2_alias_mask > sc_config.l2_index_mask) begin
            while (alias_count > 0) begin
               evl_paddr_t addr;

               addr = ((evl_paddr_t'($urandom) | (evl_paddr_t'($urandom) << 32)) & sc_config.l2_alias_mask & addr_and_mask) | addr_or_mask;
               if (!(addr inside { alias_addrs })) begin
                  if (!(addr inside { new_alias_addrs })) begin
                     new_alias_addrs.push_back(addr);
                     alias_count--;
                  end
               end
            end
         end
         if (new_alias_addrs.size() == 0) begin
            addrs_out = new[0];
            return 0;
         end

         //
         // Now create a list of addresses to use.
         //
         addr_index = 0;
         addrs_out  = new[index_addrs.size() * new_alias_addrs.size()];
         foreach (index_addrs[ii]) begin
            foreach (new_alias_addrs[jj]) begin
               addrs_out[addr_index] = new_alias_addrs[jj] | index_addrs[ii];
               addr_index++;
            end
         end
         return addr_index;
      end
      else if (addr_type == `EVL_GEN_ADDR_L3) begin
         //
         // Extract indices and aliases from the incoming list of addresses.
         //
         foreach (addrs_in[ii]) begin
            evl_paddr_t addr;

            addr = addrs_in[ii] & sc_config.l3_index_mask;
            if (!(addr inside { index_addrs })) begin
               index_addrs.push_back(addr);
            end
            addr = (addrs_in[ii] & sc_config.l3_alias_mask & addr_and_mask) | addr_or_mask;
            if (!(addr inside { alias_addrs })) begin
               alias_addrs.push_back(addr);
            end
         end

         if ((index_addrs.size() == 0) || (alias_addrs.size() == 0)) begin
            addrs_out = new[0];
            return 0;
         end

         //
         // Now generate the aliases (which lie outside the L3 index).  All aliases must be from
         // 512G to 1T.
         //
         if (alias_count > 32) begin
            alias_count = 32;
         end
         if (sc_config.l3_alias_mask > sc_config.l3_index_mask) begin
            while (alias_count > 0) begin
               evl_paddr_t addr;

               addr = ((evl_paddr_t'($urandom) | (evl_paddr_t'($urandom) << 32)) & sc_config.l3_alias_mask & addr_and_mask) | addr_or_mask;
               if (!(addr inside { alias_addrs })) begin
                  if (!(addr inside { new_alias_addrs })) begin
                     new_alias_addrs.push_back(addr);
                     alias_count--;
                  end
               end
            end
         end
         if (new_alias_addrs.size() == 0) begin
            addrs_out = new[0];
            return 0;
         end

         //
         // Now create a list of addresses to use.
         //
         addr_index = 0;
         addrs_out  = new[index_addrs.size() * new_alias_addrs.size()];
         foreach (index_addrs[ii]) begin
            foreach (new_alias_addrs[jj]) begin
               addrs_out[addr_index] = new_alias_addrs[jj] | index_addrs[ii];
               addr_index++;
            end
         end
         return addr_index;
      end
      addrs_out = new[0];
      return 0;
   endfunction : get_random_aliases_local


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_addr_alloc");
      super.new(name_in);
      if (m_addr_alloc == null) begin
         m_addr_alloc = this;
      end
   endfunction : new

endclass : evl_addr_alloc
