//--------------------------------------------------------------------------------------------------
//
// Class: evl_line_desc
//
class evl_line_desc extends evl_line_desc_base #(evl_paddr_t, evl_line_desc);

   `uvm_object_utils(evl_line_desc)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_line_data  m_line_data;
   evl_line_data  m_template_line_data;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_template
   //
   virtual function void set_data_template(input evl_line_data template_line_data_in);
      m_template_line_data = template_line_data_in;
   endfunction : set_data_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_data
   //
   virtual function void create_data(input bit random_data = 1'b0, input evl_line_data template_line_data_in = null);
      if (m_line_data == null) begin
         if (template_line_data_in != null) begin
            m_line_data = template_line_data_in.clone_line_data();
         end
         else if (m_template_line_data != null) begin
            m_line_data = m_template_line_data.clone_line_data();
         end
         else begin
            m_line_data = new(get_abstract_name());
         end
      end
      if (random_data == 1'b0) begin
         m_line_data.create_data(null);
      end
      else begin
         m_line_data.create_data(m_rand_gen);
      end
   endfunction : create_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bytes
   //
   // This function is used to set data of arbitrary length and alignment at the specified offset
   //
   // Input Variables:
   //
   // byte_index_in: This is the offset within the cache line to modify.
   // length_in:     This is the length of the data to modify
   // xword_in:      This is the data to use for the modification
   // byte_en_in:    This is a doubleword byte enable specifying which bytes to modify.
   //
   function void set_bytes(input int                 byte_index_in,
                           input int                 length_in,
                           input evl_xword_t         xword_in,
                           input evl_xword_byte_en_t byte_en_in);
      int index = byte_index_in;
      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask);
      end
      m_line_data.set_bytes(index, length_in, xword_in, byte_en_in);
   endfunction : set_bytes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_byte
   //
   // This function is used to set a byte at the specified offset in the m_line_data object.  If
   // m_line_data is null, a new line data object based on this descriptor's line data template will
   // be created (filled with 0s) and the specified byte will be modified appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the byte offset within the cache line to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_byte(input int byte_index_in, input byte byte_in);
      int index = byte_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask);
      end
      m_line_data.set_byte(index, byte_in);
   endfunction : set_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_byte
   //
   // This function is used to get a hebyte (64 bytes) at the specified hebyte offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified hebyte will be returned
   //
   // Input Variables:
   //
   // byte_index_in: This is the hebyte offset within the cache line to modify (always 0).
   //
   function byte get_byte(input int byte_index_in);
      int index = byte_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask);
      end
      return m_line_data.get_byte(index);
   endfunction : get_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_hword
   //
   // This function is used to set a halfword (2 bytes) at the specified halfword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified halfword will be
   // modified appropriately.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the cache line to modify (address bits
   //                 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_hword(input int hword_index_in, input evl_hword_t hword_in);
      int index = hword_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask) >> `EVL_HWORD_ADDR_LSB;
      end
      m_line_data.set_hword(index, hword_in);
   endfunction : set_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_hword
   //
   // This function is used to get a hehword (64 bytes) at the specified hehword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified hehword will be returned
   //
   // Input Variables:
   //
   // hword_index_in: This is the hehword offset within the cache line to modify (always 0).
   //
   function evl_hword_t get_hword(input int hword_index_in);
      int index = hword_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask) >> `EVL_HWORD_ADDR_LSB;
      end
      return m_line_data.get_hword(index);
   endfunction : get_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_word
   //
   // This function is used to set a word (4 bytes) at the specified word offset in the m_line_data
   // object.  If m_line_data is null, a new line data object based on this descriptor's line data
   // template will be created (filled with 0s) and the specified word will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within the cache line to modify (address bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_word(input int word_index_in, input evl_word_t word_in);
      int index = word_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_word_paddr_mask & ~const_line_paddr_mask) >> `EVL_WORD_ADDR_LSB;
      end
      m_line_data.set_word(index, word_in);
   endfunction : set_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_word
   //
   // This function is used to get a heword (64 bytes) at the specified heword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified heword will be returned
   //
   // Input Variables:
   //
   // word_index_in: This is the heword offset within the cache line to modify (always 0).
   //
   function evl_word_t get_word(input int word_index_in);
      int index = word_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_word_paddr_mask & ~const_line_paddr_mask) >> `EVL_WORD_ADDR_LSB;
      end
      return m_line_data.get_word(index);
   endfunction : get_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_dword
   //
   // This function is used to set a doubleword (8 bytes) at the specified doubleword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified doubleword will be
   // modified appropriately.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the cache line to modify (address bits
   //                 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //                 be a handle to a created object.
   //
   function void set_dword(input int dword_index_in, input evl_dword_t dword_in);
      int index = dword_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_dword_paddr_mask & ~const_line_paddr_mask) >> `EVL_DWORD_ADDR_LSB;
      end
      m_line_data.set_dword(index, dword_in);
   endfunction : set_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword
   //
   // This function is used to get a hedword (64 bytes) at the specified hedword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified hedword will be returned
   //
   // Input Variables:
   //
   // dword_index_in: This is the hedword offset within the cache line to modify (always 0).
   //
   function evl_dword_t get_dword(input int dword_index_in);
      int index = dword_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_dword_paddr_mask & ~const_line_paddr_mask) >> `EVL_DWORD_ADDR_LSB;
      end
      return m_line_data.get_dword(index);
   endfunction : get_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_qword
   //
   // This function is used to set a quadword (16 bytes) at the specified quadword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified quadword will be
   // modified appropriately.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the cache line to modify (address bits
   //                 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_qword(input int qword_index_in, input evl_qword_t qword_in);
      int index = qword_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_qword_paddr_mask & ~const_line_paddr_mask) >> `EVL_QWORD_ADDR_LSB;
      end
      m_line_data.set_qword(index, qword_in);
   endfunction : set_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword
   //
   // This function is used to get a heqword (64 bytes) at the specified heqword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified heqword will be returned
   //
   // Input Variables:
   //
   // qword_index_in: This is the heqword offset within the cache line to modify (always 0).
   //
   function evl_qword_t get_qword(input int qword_index_in);
      int index = qword_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_qword_paddr_mask & ~const_line_paddr_mask) >> `EVL_QWORD_ADDR_LSB;
      end
      return m_line_data.get_qword(index);
   endfunction : get_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_oword
   //
   // This function is used to set an octalword (32 bytes) at the specified octalword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified octalword will be
   // modified appropriately.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the cache line to modify (address bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_oword(input int oword_index_in, input evl_oword_t oword_in);
      int index = oword_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_oword_paddr_mask & ~const_line_paddr_mask) >> `EVL_OWORD_ADDR_LSB;
      end
      m_line_data.set_oword(index, oword_in);
   endfunction : set_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_oword
   //
   // This function is used to get a heoword (64 bytes) at the specified heoword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified heoword will be returned
   //
   // Input Variables:
   //
   // oword_index_in: This is the heoword offset within the cache line to modify (always 0).
   //
   function evl_oword_t get_oword(input int oword_index_in);
      int index = oword_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_oword_paddr_mask & ~const_line_paddr_mask) >> `EVL_OWORD_ADDR_LSB;
      end
      return m_line_data.get_oword(index);
   endfunction : get_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_xword
   //
   // This function is used to set a hexword (64 bytes) at the specified hexword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified hexword will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the cache line to modify (always 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_xword(input int xword_index_in, input evl_xword_t xword_in, input evl_xword_byte_en_t byte_en_in = ~evl_xword_byte_en_t'(0));
      int index = xword_index_in;

      if (m_line_data == null) begin
         create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_xword_paddr_mask & ~const_line_paddr_mask) >> `EVL_XWORD_ADDR_LSB;
      end
      m_line_data.set_xword(index, xword_in, byte_en_in);
   endfunction : set_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_xword
   //
   // This function is used to get a hexword (64 bytes) at the specified hexword offset in the
   // m_line_data object.  If m_line_data is null, a new line data object based on this descriptor's
   // line data template will be created (filled randomly) and the specified hexword will be returned
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the cache line to modify (always 0).
   //
   function evl_xword_t get_xword(input int xword_index_in);
      int index = xword_index_in;

      if (m_line_data == null) begin
         create_data(1);
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_xword_paddr_mask & ~const_line_paddr_mask) >> `EVL_XWORD_ADDR_LSB;
      end
      return m_line_data.get_xword(index);
   endfunction : get_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: validate_line
   //
   // This function is used to validate all bytes in this cache line.
   //
   function void validate_line(input bit random_data = 1'b0);
      if (m_line_data == null) begin
         create_data(random_data);
      end
      m_line_data.validate_line(random_data ? m_rand_gen : null);
   endfunction : validate_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_data
   //
   function evl_line_data get_line_data(input bit random_data = 1'b0, input evl_line_data template_line_data_in = null);
      if (m_line_data == null) begin
         create_data(random_data, template_line_data_in);
         m_line_data.invalidate_line();
      end
      return m_line_data;
   endfunction : get_line_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword_en
   //
   function evl_line_qword_en_t get_qword_en();
      if (m_line_data == null) begin
         return evl_line_qword_en_t'(0);
      end
      return m_line_data.get_qword_en();
   endfunction : get_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_modified_qword_en
   //
   function void clr_modified_qword_en();
      if (m_line_data == null) begin
         return;
      end
      m_line_data.clr_modified_qword_en();
   endfunction : clr_modified_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_modified_qword_en
   //
   function void set_modified_qword_en();
      if (m_line_data == null) begin
         return;
      end
      m_line_data.set_modified_qword_en();
   endfunction : set_modified_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_modified_qword_en
   //
   function evl_line_qword_en_t get_modified_qword_en();
      if (m_line_data == null) begin
         return evl_line_qword_en_t'(0);
      end
      return m_line_data.get_modified_qword_en();
   endfunction : get_modified_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_data_copy
   //
   function evl_line_data get_line_data_copy(input bit random_data = 1'b0, input evl_line_data template_line_data_in = null);
      if (m_line_data == null) begin
         create_data(random_data, template_line_data_in);
         m_line_data.invalidate_line();
      end
      return m_line_data.clone_line_data();
   endfunction : get_line_data_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_all_line_data
   //
   // This function is used to zero all of the data for this cache line.
   //
   virtual function void zero_all_line_data(input evl_line_data template_line_data_in = null);
      if (m_line_data == null) begin
         create_data(1'b0, template_line_data_in);
      end
      m_line_data.zero_all_data();
   endfunction : zero_all_line_data


   //---------------------------------------------------------------------------------------------
   //
   // Function: invalidate_line_data
   //
   //
   virtual function void invalidate_line_data(input evl_line_data template_line_data_in = null);
      if (m_line_data == null) begin
         create_data(1'b0, template_line_data_in);
      end
      m_line_data.invalidate_line();
   endfunction : invalidate_line_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: merge_data_from_line
   //
   // This function is used to merge data from a source line into this cache line.  The two cache
   // lines need not be of the same type, but both must be derived from evl_line_data.
   //
   // Input Variables:
   //
   // source_line: This is the source cache line from which to merge data.
   //
   virtual function void merge_data_from_line(input evl_line_data source_line,
                                              input bit           mark_modified_bytes   = 1'b0,
                                              input bit           random_data           = 1'b0,
                                              input evl_line_data template_line_data_in = null);
      if (m_line_data == null) begin
         create_data(random_data, template_line_data_in);
         m_line_data.invalidate_line();
      end
      m_line_data.merge_data_from_line(source_line, mark_modified_bytes);
   endfunction : merge_data_from_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_paddr
   //
   function void set_paddr(input evl_paddr_t paddr_in);
      set_tag(paddr_in & const_line_paddr_mask);
   endfunction : set_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_paddr
   //
   function evl_paddr_t get_paddr();
      return get_tag();
   endfunction : get_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_bytes_are_valid
   //
   // Blah
   //
   virtual function bit all_bytes_are_valid();
      if (m_line_data == null) begin
         return 1'b0;
      end
      return m_line_data.all_bytes_are_valid();
   endfunction : all_bytes_are_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_bytes_are_modified
   //
   // Blah
   //
   virtual function bit all_bytes_are_modified();
      if (m_line_data == null) begin
         return 1'b0;
      end
      return m_line_data.all_bytes_are_modified();
   endfunction : all_bytes_are_modified


   //-----------------------------------------------------------------------------------------------
   //
   // Function: data_is_all_zero
   //
   // Blah
   //
   virtual function bit data_is_all_zero();
      if (m_line_data == null) begin
         return 1'b0;
      end
      return m_line_data.data_is_all_zero();
   endfunction : data_is_all_zero


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_atomic_return_data
   //
   // Input Variables:
   //
   //   req_desc:       the request descriptor of the atomic request
   //   normalize_data: set to 1 if the return data should be normalized
   //   do_line_write:  set to 0 if you just want the return data and not
   //                   modify the line data
   //
   function evl_line_data get_atomic_return_data(input evl_req_desc req_desc,
                                                 input bit          normalize_data = 1'b1);
      evl_bus_atomic_conf_t atomic_conf;
      evl_atomic_op_t       atomic_op;
      bit                   atomic_sc_vec;
      bit                   atomic_size;
      int unsigned          num_operations;
      evl_dword_t           mem_operand;
      evl_dword_t           desired_operand;
      evl_paddr_t           addr;
      evl_line_data         return_data;
      int unsigned          offset;

      atomic_conf   = req_desc.get_bus_atomic_conf();
      atomic_op     = evl_atomic_op_t'(atomic_conf[3:0]);
      atomic_sc_vec = bit'(atomic_conf[4]);
      atomic_size   = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);
      addr          = req_desc.get_bus_atomic_offset();
      offset        = int'((atomic_size == 1'b1) ? addr[`EVL_LINE_ADDR_LSB-1:`EVL_DWORD_ADDR_LSB] : addr[`EVL_LINE_ADDR_LSB-1:`EVL_WORD_ADDR_LSB]);
      return_data   = new();
      return_data.zero_all_data();
      return_data.invalidate_line();

      // scalar & vector operations are the same, the only difference is the number of pieces of data we are working with.
      num_operations = (atomic_sc_vec == 1'b1) ? 8 : 1;
      `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("getting atomic return data (%p%0s)", atomic_op, (atomic_sc_vec == 1'b1) ? ", size:8x32 bits" : ((atomic_size == 1'b1) ? ", size:64 bits" : ", size:32 bits")))

      for (int ii = 0; ii < num_operations; ii++) begin
         // req operand is always in the lower quadword of the data, but get the mem operand based on the address offset
         if (atomic_size == 1'b1) begin
            mem_operand = get_dword(offset + ii);
            if (normalize_data == 1'b1) begin
               return_data.set_dword(ii, mem_operand);
            end
            else begin
               return_data.set_dword(offset + ii, mem_operand);
            end
         end
         else begin
            mem_operand = evl_dword_t'(get_word(offset + ii));
            if (normalize_data == 1'b1) begin
               return_data.set_word(ii, evl_word_t'(mem_operand));
            end
            else begin
               return_data.set_word(offset + ii, evl_word_t'(mem_operand));
            end
         end
      end
      return return_data;
   endfunction : get_atomic_return_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_atomic
   //
   // Input Variables:
   //
   //   req_desc:       the request descriptor of the atomic request
   //   normalize_data: set to 1 if the return data should be normalized
   //   do_line_write:  set to 0 if you just want the return data and not
   //                   modify the line data
   //
   function evl_line_data process_atomic(input evl_req_desc req_desc,
                                         input bit          normalize_data = 1'b1,
                                         input bit          do_line_write  = 1'b1);
      evl_bus_atomic_conf_t atomic_conf;
      evl_atomic_op_t       atomic_op;
      bit                   atomic_sc_vec;
      bit                   atomic_size;
      int unsigned          num_operations;
      evl_dword_t           req_operand;
      evl_dword_t           mem_operand;
      evl_dword_t           desired_operand;
      evl_dword_t           expected_operand;
      evl_dword_t           result;
      evl_paddr_t           addr;
      evl_line_data         return_data;
      int unsigned          offset;

      `ifdef EVL_CHECK_NEW_ATOMIC_FUNCTION
         evl_xword_t         base_mem_data;
         evl_xword_t         base_return_data;
         evl_xword_byte_en_t base_return_mask;
      `endif

      atomic_conf   = req_desc.get_bus_atomic_conf();
      atomic_op     = evl_atomic_op_t'(atomic_conf[3:0]);
      atomic_sc_vec = bit'(atomic_conf[4]);
      // if it's a vector op, then the size is 32b, otherwise check bit 5 to see if 64b or 32b
      atomic_size   = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);
      addr          = req_desc.get_bus_atomic_offset();
      offset        = int'((atomic_size == 1'b1) ? addr[`EVL_LINE_ADDR_LSB-1:`EVL_DWORD_ADDR_LSB] : addr[`EVL_LINE_ADDR_LSB-1:`EVL_WORD_ADDR_LSB]);

      `ifdef EVL_CHECK_NEW_ATOMIC_FUNCTION
         base_mem_data = evl_base_pkg::perform_et_atomic_op(base_return_data, base_return_mask, atomic_conf, addr[`EVL_LINE_ADDR_LSB-1:0], get_xword(0), req_desc.get_req_xword(0), normalize_data);
      `endif
      return_data = new();
      return_data.zero_all_data();
      return_data.invalidate_line();

      // FUTURE: is this the right place to put an error check to make sure
      // the address is aligned correctly?  E.g. a vector request should be
      // oword-aligned
      // ALSO put a check for conf[6] and L2/L3 destination

      // scalar & vector operations are the same, the only difference is the number of pieces of data we are working with.
      num_operations = (atomic_sc_vec == 1'b1) ? 8 : 1;
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("processing atomic operation (%p%0s)", atomic_op, (atomic_sc_vec == 1'b1) ? ", size:8x32 bits" : ((atomic_size == 1'b1) ? ", size:64 bits" : ", size:32 bits")))

      for (int ii = 0; ii < num_operations; ii++) begin
         // req operand is always in the lower quadword of the data, but get the mem operand based on the address offset
         if (atomic_size == 1'b1) begin
            req_operand = req_desc.get_req_dword(ii);
            mem_operand = get_dword(offset + ii);
            if (normalize_data == 1'b1) begin
               return_data.set_dword(ii, mem_operand);
            end
            else begin
               return_data.set_dword(offset + ii, mem_operand);
            end
         end
         else begin
            req_operand = evl_dword_t'(req_desc.get_req_word(ii));
            mem_operand = evl_dword_t'(get_word(offset + ii));
            if (normalize_data == 1'b1) begin
               return_data.set_word(ii, evl_word_t'(mem_operand));
            end
            else begin
               return_data.set_word(offset + ii, evl_word_t'(mem_operand));
            end
         end

         case (atomic_op)
            SC_AMOSWAP  : begin
               result = req_operand;
            end
            SC_AMOADD   : begin
               result = req_operand + mem_operand;
            end
            SC_AMOXOR   : begin
               result = req_operand ^ mem_operand;
            end
            SC_AMOAND   : begin
               result = req_operand & mem_operand;
            end
            SC_AMOOR    : begin
               result = req_operand | mem_operand;
            end
            SC_AMOMIN   : begin
               longint req_val = (atomic_size == 1'b1) ? signed'(req_operand) : signed'(evl_word_t'(req_operand));
               longint mem_val = (atomic_size == 1'b1) ? signed'(mem_operand) : signed'(evl_word_t'(mem_operand));
               result = (req_val > mem_val) ? mem_val : req_val;
            end
            SC_AMOMAX   : begin
               longint req_val = (atomic_size == 1'b1) ? signed'(req_operand) : signed'(evl_word_t'(req_operand));
               longint mem_val = (atomic_size == 1'b1) ? signed'(mem_operand) : signed'(evl_word_t'(mem_operand));
               result = (req_val > mem_val) ? req_val : mem_val;
            end
            SC_AMOMINU  : begin
               result = (req_operand > mem_operand) ? mem_operand : req_operand;
            end
            SC_AMOMAXU  : begin
               result = (req_operand > mem_operand) ? req_operand : mem_operand;
            end
            SC_AMOMAX_PS,
            SC_AMOMIN_PS: begin
               // floating point has a different format whether it's 32b or 64b
               shortreal  req_val32 = $bitstoshortreal(req_operand);
               shortreal  mem_val32 = $bitstoshortreal(mem_operand);
               real       req_val64 = $bitstoreal(req_operand);
               real       mem_val64 = $bitstoreal(mem_operand);

               // these magic numbers come from ieee 754 spec for NaNs
               evl_dword_t c_NaN32         = 64'h0000_0000_7fc0_0000;
               evl_dword_t c_NaN64         = 64'h7ff8_0000_0000_0000;
               bit         req_val_is_qnan = (atomic_size == 1'b1) ? ((req_operand[62:52] == 11'h7ff) && (req_operand[51] == 1'b1))                             : ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b1));
               bit         mem_val_is_qnan = (atomic_size == 1'b1) ? ((mem_operand[62:52] == 11'h7ff) && (mem_operand[51] == 1'b1))                             : ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b1));
               bit         req_val_is_snan = (atomic_size == 1'b1) ? ((req_operand[62:52] == 11'h7ff) && (req_operand[51] == 1'b0) && (req_operand[50:0] != 0)) : ((req_operand[30:23] == 8'hff) && (req_operand[22] == 1'b0) && (req_operand[21:0] != 0));
               bit         mem_val_is_snan = (atomic_size == 1'b1) ? ((mem_operand[62:52] == 11'h7ff) && (mem_operand[51] == 1'b0) && (mem_operand[50:0] != 0)) : ((mem_operand[30:23] == 8'hff) && (mem_operand[22] == 1'b0) && (mem_operand[21:0] != 0));
               bit         req_val_is_zero = (atomic_size == 1'b1) ? ((req_operand == 64'h0000000000000000) || (req_operand == 64'h8000000000000000))           : ((req_operand == 64'h0000000000000000) || (req_operand == 64'h0000000080000000));
               bit         mem_val_is_zero = (atomic_size == 1'b1) ? ((mem_operand == 64'h0000000000000000) || (mem_operand == 64'h8000000000000000))           : ((mem_operand == 64'h0000000000000000) || (mem_operand == 64'h0000000080000000));

               // NaN handling
               if ((req_val_is_snan == 1'b1) || (mem_val_is_snan == 1'b1)) begin
                  result = (atomic_size == 1'b1) ? c_NaN64 : c_NaN32;
               end
               else if ((req_val_is_qnan == 1'b1) && (mem_val_is_qnan == 1'b1)) begin
                  result = (atomic_size == 1'b1) ? c_NaN64 : c_NaN32;
               end
               else if (req_val_is_qnan == 1'b1) begin
                  result = mem_operand;
               end
               else if (mem_val_is_qnan == 1'b1) begin
                  result = req_operand;
               end
               // neither are NaNs so do the comparison
               // special check first for +0/-0 since they evaluate as equal when using >
               else if ((req_val_is_zero == 1'b1) && (mem_val_is_zero == 1'b1)) begin
                  if (atomic_op == SC_AMOMAX_PS) begin
                     result = (req_operand > mem_operand) ? mem_operand : req_operand;
                  end
                  else begin
                     result = (req_operand > mem_operand) ? req_operand : mem_operand;
                  end
               end
               else begin
                  case ({atomic_op, atomic_size})
                     {SC_AMOMIN_PS, 1'b1}: result = (req_val64 > mem_val64) ? mem_operand : req_operand;
                     {SC_AMOMAX_PS, 1'b1}: result = (req_val64 > mem_val64) ? req_operand : mem_operand;
                     {SC_AMOMIN_PS, 1'b0}: result = (req_val32 > mem_val32) ? mem_operand : req_operand;
                     {SC_AMOMAX_PS, 1'b0}: result = (req_val32 > mem_val32) ? req_operand : mem_operand;
                  endcase
               end
            end
            // Note about CompExch - it's scalar ONLY (one operation of either 32b/64b data)
            // There are TWO incoming operands - desired (word0/dword0) and expected (word2/dword1)
            // Format is:
            //     size==1 (64b)    <------64------><------64------>
            //     size==0 (32b)    <---0--><--32--><---0--><--32-->
            SC_AMOCOMPEXCH : begin
               // If Vector bit is set then it behaves as a NOP
               if (atomic_sc_vec == 1'b1) begin
                  result = mem_operand;
               end
               else begin
                  // redundant but easier to read/understand code
                  desired_operand = req_operand;

                  if (atomic_size == 1'b1) begin
                     expected_operand = req_desc.get_req_dword(ii+1);
                  end
                  else begin
                     expected_operand = evl_dword_t'(req_desc.get_req_word(ii+2));
                  end

                  if (mem_operand == expected_operand) begin
                     result = desired_operand;
                  end
                  else begin
                     result = mem_operand;
                  end
               end
            end
            default: begin
               result = mem_operand;
            end
         endcase

         if ((atomic_op == SC_AMOCOMPEXCH) && (atomic_sc_vec == 1'b0)) begin
            if (atomic_size == 1'b1) begin
               `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("atomic expected_operand:0x%16x   desired_operand:0x%16x   mem_operand:0x%16x   result:0x%16x", expected_operand, desired_operand, mem_operand, result))
            end
            else begin
               `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("atomic expected_operand:0x%8x   desired_operand:0x%8x   mem_operand:0x%8x   result:0x%8x", expected_operand, desired_operand, mem_operand, result))
            end
         end
         else begin
            if (atomic_size == 1'b1) begin
               `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("atomic req_operand:0x%16x   mem_operand:0x%16x   result:0x%16x", req_operand, mem_operand, result))
            end
            else begin
               `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), req_desc, $sformatf("atomic req_operand:0x%8x   mem_operand:0x%8x   result:0x%8x", req_operand, mem_operand, result))
            end
         end

         // write the result to the line
         if (do_line_write == 1'b1) begin
            if (atomic_size == 1'b1) begin
               set_dword(offset + ii, result);
            end
            else begin
               set_word(offset + ii, evl_word_t'(result));
            end
         end
      end

      `ifdef EVL_CHECK_NEW_ATOMIC_FUNCTION
         if ((do_line_write == 1'b1) && (base_mem_data != get_xword(0))) begin
            `dut_error("GLOBAL", $sformatf("evl_base_pkg::perform_et_atomic_op() memory:0x%x", base_mem_data))
            `dut_error("GLOBAL", $sformatf("evl_base_pkg::perform_et_atomic_op() expect:0x%x", get_xword(0)))
         end
         if ((base_return_data != return_data.get_xword(0)) || (base_return_mask != return_data.get_xword_valids(0))) begin
            `dut_error("GLOBAL", $sformatf("evl_base_pkg::perform_et_atomic_op() return:0x%x(0x%x)", base_return_data, base_return_mask))
            `dut_error("GLOBAL", $sformatf("evl_base_pkg::perform_et_atomic_op() expect:0x%x(0x%x)", return_data.get_xword(0), return_data.get_xword_valids(0)))
         end
      `endif
      return return_data;
   endfunction : process_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_atomic_return_data
   //
   // Similar to get_atomic_return_data, only difference is: input evl_trans_desc (instead of
   // evl_req_desc).
   //
   // Input Variables:
   //   trans_desc:     the trans descriptor of the atomic trans
   //   normalize_data: set to 1 if the return data should be normalized
   //   do_line_write:  set to 0 if you just want the return data and not
   //                   modify the line data
   //
   function evl_line_data get_trans_atomic_return_data(input evl_trans_desc trans_desc,
                                                       input bit            normalize_data = 1'b1);
      evl_bus_atomic_conf_t atomic_conf;
      evl_atomic_op_t       atomic_op;
      bit                   atomic_sc_vec;
      bit                   atomic_size;
      int unsigned          num_operations;
      evl_dword_t           mem_operand;
      evl_dword_t           desired_operand;
      evl_paddr_t           addr;
      evl_line_data         return_data;
      int unsigned          offset;

      atomic_conf   = trans_desc.get_bus_atomic_conf();
      atomic_op     = evl_atomic_op_t'(atomic_conf[3:0]);
      atomic_sc_vec = bit'(atomic_conf[4]);
      atomic_size   = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);
      addr          = trans_desc.get_bus_atomic_offset();
      offset        = int'((atomic_size == 1'b1) ? addr[`EVL_LINE_ADDR_LSB-1:`EVL_DWORD_ADDR_LSB] : addr[`EVL_LINE_ADDR_LSB-1:`EVL_WORD_ADDR_LSB]);
      return_data   = new();
      return_data.zero_all_data();
      return_data.invalidate_line();

      // scalar & vector operations are the same, the only difference is the number of pieces of data we are working with.
      num_operations = (atomic_sc_vec == 1'b1) ? 8 : 1;
      `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("getting atomic return data (%p%0s)", atomic_op, (atomic_sc_vec == 1'b1) ? ", size:8x32 bits" : ((atomic_size == 1'b1) ? ", size:64 bits" : ", size:32 bits")))

      for (int ii = 0; ii < num_operations; ii++) begin
         // new operand is always in the lower quadword of the data, but get the mem operand based on the address offset
         if (atomic_size == 1'b1) begin
            mem_operand = get_dword(offset + ii);
            if (normalize_data == 1'b1) begin
               return_data.set_dword(ii, mem_operand);
            end
            else begin
               return_data.set_dword(offset + ii, mem_operand);
            end
         end
         else begin
            mem_operand = evl_dword_t'(get_word(offset + ii));
            if (normalize_data == 1'b1) begin
               return_data.set_word(ii, evl_word_t'(mem_operand));
            end
            else begin
               return_data.set_word(offset + ii, evl_word_t'(mem_operand));
            end
         end
      end
      return return_data;
   endfunction : get_trans_atomic_return_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_trans_atomic
   //
   // Similar to process_atomic, only difference is: input evl_trans_desc (instead of evl_req_desc).
   //
   // Input Variables:
   //   trans_desc:     the trans descriptor of the atomic trans
   //   normalize_data: set to 1 if the return data should be normalized
   //   do_line_write:  set to 0 if you just want the return data and not
   //                   modify the line data
   //
   function evl_line_data process_trans_atomic(input evl_trans_desc trans_desc,
                                               input bit            normalize_data = 1'b1,
                                               input bit            do_line_write  = 1'b1);
      evl_bus_atomic_conf_t atomic_conf;
      evl_atomic_op_t       atomic_op;
      bit                   atomic_sc_vec;
      bit                   atomic_size;
      int unsigned          num_operations;
      evl_dword_t           new_operand;
      evl_dword_t           mem_operand;
      evl_dword_t           desired_operand;
      evl_dword_t           expected_operand;
      evl_dword_t           result;
      evl_paddr_t           addr;
      evl_line_data         return_data;
      int unsigned          offset;

      atomic_conf   = trans_desc.get_bus_atomic_conf();
      atomic_op     = evl_atomic_op_t'(atomic_conf[3:0]);
      atomic_sc_vec = bit'(atomic_conf[4]);
      // if it's a vector op, then the size is 32b, otherwise check bit 5 to see if 64b or 32b
      atomic_size   = (atomic_sc_vec == 1'b1) ? 1'b0 : bit'(atomic_conf[5]);

      addr        = trans_desc.get_bus_atomic_offset();
      offset      = int'((atomic_size == 1'b1) ? addr[`EVL_LINE_ADDR_LSB-1:`EVL_DWORD_ADDR_LSB] : addr[`EVL_LINE_ADDR_LSB-1:`EVL_WORD_ADDR_LSB]);
      return_data = new();
      return_data.zero_all_data();
      return_data.invalidate_line();

      // FUTURE: is this the right place to put an error check to make sure
      // the address is aligned correctly?  E.g. a vector request should be
      // qword-aligned
      // ALSO put a check for conf[6] and L2/L3 destination

      // scalar & vector operations are the same, the only difference is the number of pieces of data we are working with.
      num_operations = (atomic_sc_vec == 1'b1) ? 8 : 1;
      `evl_log_mdesc(UVM_MEDIUM, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("processing atomic operation (%p, addr:0x%h, %0s)", atomic_op, trans_desc.get_paddr(), (atomic_sc_vec == 1'b1) ? "size:8x32 bits" : ((atomic_size == 1'b1) ? "size:64 bits" : "size:32 bits")))

      for (int ii = 0; ii < num_operations; ii++) begin
         // new operand is always in the lower quadword of the data, but get the mem operand based on the address offset
         if (atomic_size == 1'b1) begin
            new_operand = trans_desc.get_write_dword(ii);
            mem_operand = get_dword(offset + ii);
            if (normalize_data == 1'b1) begin
               return_data.set_dword(ii, mem_operand);
            end
            else begin
               return_data.set_dword(offset + ii, mem_operand);
            end
         end
         else begin
            new_operand = evl_dword_t'(trans_desc.get_write_word(ii));
            mem_operand = evl_dword_t'(get_word(offset + ii));
            if (normalize_data == 1'b1) begin
               return_data.set_word(ii, evl_word_t'(mem_operand));
            end
            else begin
               return_data.set_word(offset + ii, evl_word_t'(mem_operand));
            end
         end

         case (atomic_op)
            SC_AMOSWAP  : begin
               result = new_operand;
            end
            SC_AMOADD   : begin
               result = new_operand + mem_operand;
            end
            SC_AMOXOR   : begin
               result = new_operand ^ mem_operand;
            end
            SC_AMOAND   : begin
               result = new_operand & mem_operand;
            end
            SC_AMOOR    : begin
               result = new_operand | mem_operand;
            end
            SC_AMOMIN   : begin
               longint req_val = (atomic_size == 1'b1) ? signed'(new_operand) : signed'(evl_word_t'(new_operand));
               longint mem_val = (atomic_size == 1'b1) ? signed'(mem_operand) : signed'(evl_word_t'(mem_operand));
               result = (req_val > mem_val) ? mem_val : req_val;
            end
            SC_AMOMAX   : begin
               longint req_val = (atomic_size == 1'b1) ? signed'(new_operand) : signed'(evl_word_t'(new_operand));
               longint mem_val = (atomic_size == 1'b1) ? signed'(mem_operand) : signed'(evl_word_t'(mem_operand));
               result = (req_val > mem_val) ? req_val : mem_val;
            end
            SC_AMOMINU  : begin
               result = (new_operand > mem_operand) ? mem_operand : new_operand;
            end
            SC_AMOMAXU  : begin
               result = (new_operand > mem_operand) ? new_operand : mem_operand;
            end
            default: begin
               result = mem_operand;
            end
         endcase

         if (atomic_size == 1'b1) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("atomic new_operand:0x%16x   mem_operand:0x%16x   result:0x%16x", new_operand, mem_operand, result))
         end
         else begin
            `evl_log_mdesc(UVM_HIGH, VMOD_GLOBAL, get_abstract_name(), trans_desc, $sformatf("atomic new_operand:0x%8x   mem_operand:0x%8x   result:0x%8x", new_operand, mem_operand, result))
         end

         // write the result to the line
         if (do_line_write == 1'b1) begin
            if (atomic_size == 1'b1) begin
               set_dword(offset + ii, result);
            end
            else begin
               set_word(offset + ii, evl_word_t'(result));
            end
         end
      end  // for (int ii = 0; ii < num_operations; ii++)

      return return_data;
   endfunction : process_trans_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_obj
   //
   // This function is used to print the contents of the cache line.
   //
   // Input Variables:
   //
   // abstract_name_in: This is the abstract name to use when printing.  If it is "", then the
   //                   abstract name in this object will be used.
   //
   virtual function void print_obj(input string abstract_name_in = "");
      string abstract_name = abstract_name_in;

      if (abstract_name == "") begin
         abstract_name = get_abstract_name();
      end
      super.print_obj(abstract_name);
      if (m_line_data != null) begin
         m_line_data.print_obj(abstract_name, get_paddr());
      end
   endfunction : print_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   virtual function void do_copy(uvm_object rhs);
      evl_line_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_line_desc");
      super.new(name_in);
      m_desc_type = EVL_DESC_LINE;
   endfunction : new

endclass : evl_line_desc
