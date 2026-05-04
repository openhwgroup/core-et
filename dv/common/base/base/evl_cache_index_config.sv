//--------------------------------------------------------------------------------------------------
//
// Class: evl_cache_index_config
//
class evl_cache_index_config #(type TAG_T = evl_paddr_t) extends evl_object;

   `uvm_object_param_utils(evl_cache_index_config#(TAG_T))


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables for Personalization
   //
   int   m_num_ways         = 0;
   int   m_num_banks        = 0;
   int   m_num_subbanks     = 0;
   int   m_bank_bit_0       = 6;
   int   m_bank_bit_1       = 7;
   int   m_bank_bit_2       = 7;
   int   m_subbank_bit_0    = 8;
   int   m_subbank_bit_1    = 9;
   int   m_subbank_bit_2    = 9;
   int   m_set_bit_0        = 10;
   int   m_set_size         = 0;
   TAG_T m_set_base         = TAG_T'(0);
   TAG_T m_way_index_mask   = TAG_T'(0);
   TAG_T m_way_select_mask  = TAG_T'(0);
   TAG_T m_base_index       = TAG_T'(0);
   TAG_T m_top_index        = TAG_T'(0);
   TAG_T m_index_alias      = TAG_T'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cache_config
   //
   function void set_cache_config(input int   num_ways_in,
                                  input TAG_T index_mask_in,
                                  input TAG_T base_index_in  = TAG_T'(0),
                                  input TAG_T top_index_in   = TAG_T'(0),
                                  input TAG_T index_alias_in = TAG_T'(0));
      if ((num_ways_in > 0) && (num_ways_in < 9)) begin
         m_num_ways        = num_ways_in;
         m_way_select_mask = TAG_T'((1 << $clog2(m_num_ways)) - 1);
         m_way_index_mask  = index_mask_in;
         m_base_index      = base_index_in;
         m_top_index       = top_index_in;
         m_index_alias     = index_alias_in;
      end
      else begin
         m_num_ways        = 0;
         m_way_select_mask = TAG_T'(0);
         m_way_index_mask  = index_mask_in;
         m_base_index      = TAG_T'(0);
         m_top_index       = TAG_T'(0);
         m_index_alias     = TAG_T'(0);
      end
   endfunction : set_cache_config


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_num_ways
   //
   function int get_num_ways();
      return m_num_ways;
   endfunction : get_num_ways


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_way_cache_index
   //
   function TAG_T gen_way_cache_index(input evl_way_t way_in,
                                      input TAG_T     addr_in,
                                      input bit       notify_on_set_size_0 = 1'b0);
      TAG_T masked_addr = addr_in & m_way_index_mask;

      if ((m_set_size == 0) && (notify_on_set_size_0 == 1'b1)) begin
         return ~(TAG_T'(0));
      end
      if (m_index_alias == TAG_T'(0)) begin
         return (masked_addr | (TAG_T'(way_in) & m_way_select_mask));
      end
      if (m_base_index < m_top_index) begin
         if ((masked_addr >= m_base_index) && (masked_addr < m_top_index)) begin
            return (masked_addr | (TAG_T'(way_in) & m_way_select_mask));
         end
         else begin
            return (((addr_in + m_index_alias) & m_way_index_mask) | (TAG_T'(way_in) & m_way_select_mask));
         end
      end
      else begin
         if ((masked_addr >= m_base_index) || (masked_addr < m_top_index)) begin
            return (masked_addr | (TAG_T'(way_in) & m_way_select_mask));
         end
         else begin
            return (((addr_in + m_index_alias) & m_way_index_mask) | (TAG_T'(way_in) & m_way_select_mask));
         end
      end
   endfunction : gen_way_cache_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_set_bits
   //
   function TAG_T get_set_bits(input TAG_T addr_in);
      return ((addr_in & m_way_index_mask) >> `EVL_LINE_ADDR_LSB);
   endfunction : get_set_bits


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_index_mask
   //
   function TAG_T get_index_mask();
      return m_way_index_mask;
   endfunction : get_index_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag_mask
   //
   function TAG_T get_tag_mask();
      return ~(~m_index_alias & m_way_index_mask);
   endfunction : get_tag_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_set
   //
   function TAG_T get_cache_set_info(input  evl_way_t cway,
                                     input  TAG_T     cindex,
                                     output int       set_out,
                                     output int       bank_out,
                                     output int       subbank_out);
      TAG_T paddr;
 
      set_out     = -1;
      bank_out    = -1;
      subbank_out = -1;
      paddr = gen_way_cache_index(cway, cindex, 1'b1);
      if (paddr == ~(TAG_T'(0))) begin
         return paddr;
      end
      if (m_num_subbanks <= 0) begin
         subbank_out = 0;
      end
      else begin
         subbank_out = ( ((int'(paddr >> m_subbank_bit_2) & 1) << 2) |
                         ((int'(paddr >> m_subbank_bit_1) & 1) << 1) |
                         ((int'(paddr >> m_subbank_bit_0) & 1)) ) % m_num_subbanks;
      end
      if (m_num_banks <= 0) begin
         bank_out = 0;
      end
      else begin
         bank_out = ( ((int'(paddr >> m_bank_bit_2) & 1) << 2) |
                      ((int'(paddr >> m_bank_bit_1) & 1) << 1) |
                      ((int'(paddr >> m_bank_bit_0) & 1)) ) % m_num_banks;
      end
      set_out = (int'(paddr) >> m_set_bit_0) + m_set_base;
      return paddr;
   endfunction : get_cache_set_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bit_info
   //
   function void set_bit_info(input int num_banks,
                              input int num_subbanks,
                              input int bank_bit_0,
                              input int bank_bit_1,
                              input int bank_bit_2,
                              input int subbank_bit_0,
                              input int subbank_bit_1,
                              input int subbank_bit_2,
                              input int set_bit_0);
      m_num_banks     = num_banks;
      m_num_subbanks  = num_subbanks;
      m_bank_bit_0    = bank_bit_0;
      m_bank_bit_1    = bank_bit_1;
      m_bank_bit_2    = bank_bit_2;
      m_subbank_bit_0 = subbank_bit_0;
      m_subbank_bit_1 = subbank_bit_1;
      m_subbank_bit_2 = subbank_bit_2;
      m_set_bit_0     = set_bit_0;
   endfunction : set_bit_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_sc_config
   //
   function void process_sc_config(input int   num_ways_in,
                                   input int   set_size_in,
                                   input int   bank_lsb_in,
                                   input TAG_T set_base_in);
      m_set_size = `EVL_LINE_BYTES * int'(set_size_in) * `SC_BANKS * `SC_SUB_BANKS;
      if (m_set_size == 0) begin
         m_set_base = TAG_T'(0);
         set_cache_config(num_ways_in, evl_paddr_t'(0));
      end
      else begin
         int         index_msb;
         evl_paddr_t index_mask;

         index_msb  = $clog2(m_set_size);
         index_mask = ((evl_paddr_t'(1) << (index_msb - `EVL_LINE_ADDR_LSB)) - 1) << bank_lsb_in;
         m_set_base = TAG_T'(set_base_in);
         if ((1 << index_msb) == m_set_size) begin
            set_cache_config(num_ways_in, index_mask);
         end
         else begin
            evl_paddr_t set_base;
            evl_paddr_t set_top;
            evl_paddr_t index_alias;

            set_base    = `EVL_LINE_BYTES * evl_paddr_t'(set_base_in) * `SC_BANKS * `SC_SUB_BANKS;
            set_top     = index_mask & ((set_base + m_set_size) << (bank_lsb_in - `EVL_LINE_ADDR_LSB));
            set_base    = index_mask & (set_base << (bank_lsb_in - `EVL_LINE_ADDR_LSB));
            index_alias = (evl_paddr_t'(1) << (index_msb - `EVL_LINE_ADDR_LSB - 1)) << bank_lsb_in;
            set_cache_config(num_ways_in, index_mask, set_base, set_top, index_alias);
         end
      end
   endfunction : process_sc_config


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_cache_index_config");
      super.new(name_in);
   endfunction : new

endclass : evl_cache_index_config
