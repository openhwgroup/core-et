//--------------------------------------------------------------------------------------------------
//
// Class: evl_trans_desc
//
class evl_trans_desc extends evl_mem_desc;

   `uvm_object_utils(evl_trans_desc)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_cmd:          This variable is the abstract transaction command to be performed.
   //                 ------------------------------------------------------------------------------
   // m_graduated:    This variable is intended to reflect the graduation state of a transaction
   //                 descriptor.
   //                 ------------------------------------------------------------------------------
   // m_committed:    This variable is intended to reflect the committment state of a store
   //                 transaction.  It should be set when the store is guaranteed to be performed.
   //                 ------------------------------------------------------------------------------
   // m_visible:      This variable is intended to reflect the global visibility of a store
   //                 transaction.  In a multi-threaded coherent system, it is possible for a
   //                 particular store to only be visible by loads from the thread that issued the
   //                 store until that store becomes globally visible.
   //                 ------------------------------------------------------------------------------
   // m_can_drop:     This variable is only used by stimulus agents.  When it is set, it indicates
   //                 that this transaction can simply be dropped in the presence of a resource
   //                 conflict.  Fully random transactions can be fairly dropped when resource
   //                 conflicts occur, but transactions from directed tests should not be dropped
   //                 willy-nilly.
   //                 ------------------------------------------------------------------------------
   // m_req_desc:     This object handle is used to point to a child request descriptor that is
   //                 generated for transactions that require bus operations.  It is used for both
   //                 master stimulus agents and cache scoreboards.  It is included mainly as a
   //                 convenience; the handle to the child request descriptor is also the 0th entry
   //                 in m_child_desc_queues[EVL_DESC_REQ].
   //                 ------------------------------------------------------------------------------
   // m_write_data:   This object handle is used to point to write data for transactions with write
   //                 data (such as stores and atomic operations).  Data in this object should be
   //                 stored in cache line order, even if only some of the data is valid.
   //                 ------------------------------------------------------------------------------
   // m_read_data:    This object handle is used to point to predicted read data for transactions
   //                 that receive data (such as loads).  Data in this object should be stored in
   //                 cache line order, even if only some of the data is valid.
   //                 ------------------------------------------------------------------------------
   // m_obs_data:     This object handle is used to point to observed read data for transactions
   //                 that receive data (such as loads).  Data in this object should be stored in
   //                 cache line order, even if only some of the data is valid.  It is intended to
   //                 be used for a CPU's LSU scoreboard.
   //                 ------------------------------------------------------------------------------
   // m_req_id_queue: This object handle is used solely for stimulus application.  It is a way for a
   //                 test sequence to specify a list of valid bus request IDs.
   //
   //                 ------------------------------------------------------------------------------
   // m_privilege:    This variable holds the privilege level with which this transaction was
   //                 initiated.
   //
   evl_cmd_t            m_cmd          = NOP;
   bit                  m_graduated    = 1'b0;
   bit                  m_committed    = 1'b0;
   bit                  m_visible      = 1'b0;
   bit                  m_can_drop     = 1'b0;
   evl_req_desc         m_req_desc     = null;
   evl_line_data        m_write_data   = null;
   evl_line_data        m_read_data    = null;
   evl_line_data        m_obs_data     = null;
   evl_bus_req_id_queue m_req_id_queue = null;
   evl_privilege_t      m_privilege    = prv_user;
   evl_desc_stat_t      m_desc_stats[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cmd
   //
   // This function is used to set the abstract transaction command for this transaction thread.
   //
   // Input Variables:
   //
   // cmd_in: This is the abstract transaction command to be used.
   //
   virtual function void set_cmd(input evl_cmd_t cmd_in);
      m_cmd = cmd_in;
   endfunction : set_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cmd
   //
   // This function is used to get the abstract transaction command for this transaction thread.
   //
   // Return Value:
   //
   // evl_cmd_t: This is the abstract transaction command of this transcation thread.
   //
   virtual function evl_cmd_t get_cmd();
      return m_cmd;
   endfunction : get_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_privilege
   //
   //   This function is used to set the privilege level of this request
   //
   virtual function void set_privilege(input evl_privilege_t privilege_in);
      m_privilege = privilege_in;
   endfunction : set_privilege


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_privilege
   //
   //   This function is used to get the privilege level of this request
   //
   virtual function evl_privilege_t get_privilege();
      return m_privilege;
   endfunction : get_privilege


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cmd_attrs
   //
   // This function is used to set a group of commonly-required attributes for transaction thread
   // (such as the abstract command, the address space, etc.).  It is included mainly as a
   // convenience so all of these attributes can be set with one function call.
   //
   // Input Variables:
   //
   // cmd_in:        This is the abstract transaction command to be used.
   // addr_sp_in:    This is the abstract address space to be used.
   // cache_attr_in: If this input is 1'b1, the command is cacheable.
   // cmd_length_in: This is the command length of the command.  This should not be confused with
   //                the size of the bus request; a single-byte load to cacheable space will result
   //                in a full cache line read.
   // paddr_in:      This is the physical address for the transaction.  This should not be confused
   //                with the bus address; the ET Link, for example, requires all request addresses
   //                to be properly aligned with the size.  Thus, a 2-byte cacheable load to an
   //                offset of 10 will reault in a full cache line read with an offset of 0.
   // req_source_in: This is the bus request source for transactions that lead to ET Link requests.
   //
   function void set_cmd_attrs(input evl_cmd_t            cmd_in,
                               input evl_addr_sp_t        addr_sp_in,
                               input bit                  cache_attr_in,
                               input evl_cmd_length_t     cmd_length_in,
                               input evl_paddr_t          paddr_in,
                               input evl_bus_req_source_t req_source_in);
      m_cmd                       = cmd_in;
      m_mem_desc_attrs.addr_sp    = addr_sp_in;
      m_mem_desc_attrs.cached     = cache_attr_in;
      m_mem_desc_attrs.cmd_length = cmd_length_in;
      m_mem_desc_attrs.req_source = req_source_in;
      set_paddr(paddr_in);
   endfunction : set_cmd_attrs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_data
   //
   // This function is used to set the write data object.  It is possible to set individual write
   // data bytes with other functions in the descriptor, but the assumption here is that a
   // previously-created line data object has been initialized appropriately.  It is important to
   // note that this is NOT a copy of the object passed in; it is the SAME object.  Thus, if the
   // object is subsequently modified in some manner, the transaction descriptor's write data will
   // also be modified.
   //
   // Input Variables:
   //
   // line_data_in: This is a handle to the line data object to be used as this transaction
   //               descriptor's write data.
   //
   function void set_write_data(input evl_line_data line_data_in);
      m_write_data = line_data_in;
   endfunction : set_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_data
   //
   // This function is used to get a handle to the write data object.  There are a number of input
   // variables to control what to do in the event there is currently no write line data object.  It
   // is important to note that this is NOT a handle to a copy of the write data object; it is the
   // SAME object.  Thus, if the object is subsequently modified in some manner, the transaction
   // descriptor's write data will also be modified.
   //
   // Input Variables:
   //
   // create_data_in:        If no write data object currently exists and this variable is 1'b1, a
   //                        new line data object will be created, m_write_data will be set to point
   //                        to this new object, and a handle to the new object will be returned.
   // random_data_in:        This input variable only has meaning when a new line data object is
   //                        created.  If set to 1'b1, the new line data object will be filled with
   //                        random data rather than 0s.
   // template_line_data_in: This input variable is only used when creating a new line data object.
   //                        If this variable is not null, the incoming template will be used when
   //                        creating the new line data object.  If it is null, the descriptor's
   //                        m_template_line_data object will be used for creation.
   //
   // Return Value:
   //
   // evl_line_data: This is a handle to the descriptor's write data object.  If no write data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_write_data(input bit           create_data_in        = 1'b0,
                                         input bit           random_data_in        = 1'b0,
                                         input evl_line_data template_line_data_in = null);
      if ((m_write_data == null) && (create_data_in == 1'b1)) begin
         m_write_data = create_data(random_data_in, template_line_data_in);
      end
      return m_write_data;
   endfunction : get_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_normalized_write_data
   //
   // This function is used to get a normalized copy of the write data.
   //
   // Input Variables:
   //
   // create_data_in:        If no write data object currently exists and this variable is 1'b1, a
   //                        new line data object will be created, m_write_data will be set to point
   //                        to this new object, and a handle to the normalized data will be
   //                        returned.
   // random_data_in:        This input variable only has meaning when a new line data object is
   //                        created.  If set to 1'b1, the new line data object will be filled with
   //                        random data rather than 0s.
   // template_line_data_in: This input variable is only used when creating a new line data object.
   //                        If this variable is not null, the incoming template will be used when
   //                        creating the new line data object.  If it is null, the descriptor's
   //                        m_template_line_data object will be used for creation.
   //
   // Return Value:
   //
   // evl_line_data: This is a handle to the normalized write data object.  If no write data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_normalized_write_data(input bit           create_data_in        = 1'b0,
                                                    input bit           random_data_in        = 1'b0,
                                                    input evl_line_data template_line_data_in = null);
      if (m_write_data == null) begin
         if (create_data_in == 1'b0) begin
            return null;
         end
         m_write_data = create_data(random_data_in, template_line_data_in);
      end
      return m_write_data.get_normalized_data(get_paddr(), get_cmd_length());
   endfunction : get_normalized_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_bytes
   //
   // This function is used to set data of arbitrary length and alignment in the m_write_data object
   // at the specified byte offset.
   // If the m_write_data handle is null, a new line data object based on this descriptor's line
   // data template will be created (filled with 0s) and the specified byte will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the offset within the cache line to modify.
   // length_in:     This is the length of the data to modify
   // xword_in:      This is the data to use for the modification
   // byte_en_in:    This is a doubleword byte enable specifying which bytes to modify.
   // unknown_in:    This is a doubleword byte enable specifying which bytes to mark as unknown.
   //                Only those bytes being modified can be marked as unknown.
   // modified_in:   This is a doubleword byte enable specifying which bytes to mark as modified.
   //
   function void set_write_bytes(input int                 byte_index_in,
                                 input int                 length_in,
                                 input evl_xword_t         xword_in,
                                 input evl_xword_byte_en_t byte_en_in,
                                 input evl_xword_byte_en_t unknown_in  = evl_xword_byte_en_t'(0),
                                 input evl_xword_byte_en_t modified_in = evl_xword_byte_en_t'(0));
      m_write_data = set_bytes(m_write_data, byte_index_in, length_in, xword_in, byte_en_in, unknown_in, modified_in);
   endfunction : set_write_bytes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_byte
   //
   // This function is used to set a byte in the m_write_data object at the specified byte offset.
   // If the m_write_data handle is null, a new line data object based on this descriptor's line
   // data template will be created (filled with 0s) and the specified byte will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the byte offset within the m_write_data object to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_write_byte(input int byte_index_in, input byte byte_in);
      m_write_data = set_byte(m_write_data, byte_index_in, byte_in);
   endfunction : set_write_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_hword
   //
   // This function is used to set a halfword (2 bytes) in the m_write_data object at the specified
   // halfword offset.  If the m_write_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified halfword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the m_write_data object to modify (address
   //                 bits 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_write_hword(input int hword_index_in, input evl_hword_t hword_in);
      m_write_data = set_hword(m_write_data, hword_index_in, hword_in);
   endfunction : set_write_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_word
   //
   // This function is used to set a word (4 bytes) in the m_write_data object at the specified word
   // offset.  If the m_write_data handle is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified word will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within the m_write_data object to modify (address
   //                bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_write_word(input int word_index_in, input evl_word_t word_in);
      m_write_data = set_word(m_write_data, word_index_in, word_in);
   endfunction : set_write_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_dword
   //
   // This function is used to set a doubleword (8 bytes) in the m_write_data object at the
   // specified doubleword offset.  If the m_write_data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // doubleword will be modified appropriately.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_write_data object to modify
   //                 (address bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   function void set_write_dword(input int dword_index_in, input evl_dword_t dword_in);
      m_write_data = set_dword(m_write_data, dword_index_in, dword_in);
   endfunction : set_write_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_qword
   //
   // This function is used to set a quadword (16 bytes) in the m_write_data object at the specified
   // quadword offset.  If the m_write_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified quadword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the m_write_data object to modify (address
   //                 bits 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_write_qword(input int qword_index_in, input evl_qword_t qword_in);
      m_write_data = set_qword(m_write_data, qword_index_in, qword_in);
   endfunction : set_write_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_oword
   //
   // This function is used to set an octalword (32 bytes) in the m_write_data object at the
   // specified octalword offset.  If the m_write_data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // octalword will be modified appropriately.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the m_write_data object to modify (address
   //                 bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_write_oword(input int oword_index_in, input evl_oword_t oword_in);
      m_write_data = set_oword(m_write_data, oword_index_in, oword_in);
   endfunction : set_write_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_write_xword
   //
   // This function is used to set a hexword (64 bytes) in the m_write_data object at the specified
   // hexword offset.  If the m_write_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified hexword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the m_write_data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_write_xword(input int xword_index_in, input evl_xword_t xword_in);
      m_write_data = set_xword(m_write_data, xword_index_in, xword_in);
   endfunction : set_write_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_byte
   //
   // This function is used to get a byte from the m_write_data object at the specified byte offset.
   // If the m_write_data handle is null and the create_data_in input is 1'b1, a new m_write_data
   // object based on this descriptor's line data template will be created (filled with 0s) and the
   // specified halfword will be returned.
   //
   // Input Variables:
   //
   // byte_index_in:  This is the byte offset within the m_write_data object to read.
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // byte: This is the m_write_data byte at the specified byte offset.
   //
   function byte get_write_byte(input int byte_index_in, input bit create_data_in = 1'b0);
      return get_byte(m_write_data, byte_index_in, create_data_in);
   endfunction : get_write_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_hword
   //
   // This function is used to get a halfword (2 bytes) from the m_write_data object at the
   // specified halfword offset.  If the m_write_data handle is null and the create_data_in input is
   // 1'b1, a new m_write_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified halfword will be returned.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the m_write_data object to read (address
   //                 bits 5-1).
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_hword_t: This is the m_write_data halfword at the specified halfword offset.
   //
   function evl_hword_t get_write_hword(input int hword_index_in, input bit create_data_in = 1'b0);
      return get_hword(m_write_data, hword_index_in, create_data_in);
   endfunction : get_write_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_word
   //
   // This function is used to get a word (4 bytes) from the m_write_data object at the specified
   // word offset.  If the m_write_data handle is null and the create_data_in input is 1'b1, a new
   // m_write_data object based on this descriptor's line data template will be created (filled with
   // 0s) and the specified word will be returned.
   //
   // Input Variables:
   //
   // word_index_in:  This is the word offset within the m_write_data object to read (address
   //                 bits 5-2).
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_word_t: This is the m_write_data word at the specified word offset.
   //
   function evl_word_t get_write_word(input int word_index_in, input bit create_data_in = 1'b0);
      return get_word(m_write_data, word_index_in, create_data_in);
   endfunction : get_write_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_dword
   //
   // This function is used to get a doubleword (8 bytes) from the m_write_data object at the
   // specified doubleword offset.  If the m_write_data handle is null and the create_data_in input
   // is 1'b1, a new m_write_data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified doubleword will be returned.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_write_data object to read (address
   //                 bits 5-3).
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_dword_t: This is the m_write_data doubleword at the specified doubleword offset.
   //
   function evl_dword_t get_write_dword(input int dword_index_in, input bit create_data_in = 1'b0);
      return get_dword(m_write_data, dword_index_in, create_data_in);
   endfunction : get_write_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_qword
   //
   // This function is used to get a quadword (16 bytes) from the m_write_data object at the
   // specified quadword offset.  If the m_write_data handle is null and the create_data_in input is
   // 1'b1, a new m_write_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified quadword will be returned.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the m_write_data object to read (address
   //                 bits 5-4).
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_qword_t: This is the m_write_data quadword at the specified quadword offset.
   //
   function evl_qword_t get_write_qword(input int qword_index_in, input bit create_data_in = 1'b0);
      return get_qword(m_write_data, qword_index_in, create_data_in);
   endfunction : get_write_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_oword
   //
   // This function is used to get an octalword (32 bytes) from the m_write_data object at the
   // specified octalword offset.  If the m_write_data handle is null and the create_data_in input
   // is 1'b1, a new m_write_data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified octalword will be returned.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the m_write_data object to read (address
   //                 bit 5).
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_oword_t: This is the m_write_data octalword at the specified octalword offset.
   //
   function evl_oword_t get_write_oword(input int oword_index_in, input bit create_data_in = 1'b0);
      return get_oword(m_write_data, oword_index_in, create_data_in);
   endfunction : get_write_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_xword
   //
   // This function is used to get a hexword (64 bytes) from the m_write_data object at the
   // specified hexword offset.  If the m_write_data handle is null and the create_data_in input is
   // 1'b1, a new m_write_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified hexword will be returned.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the m_write_data object to read (always 0).
   // create_data_in: If this input is 1'b1, a new m_write_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_xword_t: This is the m_write_data hexword at the specified hexword offset.
   //
   function evl_xword_t get_write_xword(input int xword_index_in, input bit create_data_in = 1'b0);
      return get_xword(m_write_data, xword_index_in, create_data_in);
   endfunction : get_write_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_data
   //
   // This function is used to set the read data object.  It is possible to set individual read data
   // bytes with other functions in the descriptor, but the assumption here is that a
   // previously-created line data object has been initialized appropriately.  It is important to
   // note that this is NOT a copy of the object passed in; it is the SAME object.  Thus, if the
   // object is subsequently modified in some manner, the transaction descriptor's read data will
   // also be modified.
   //
   // Input Variables:
   //
   // line_data_in: This is a handle to the line data object to be used as this transaction
   //               descriptor's read data.
   //
   function void set_read_data(input evl_line_data line_data_in);
      m_read_data = line_data_in;
   endfunction : set_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_data
   //
   // This function is used to get a handle to the read data object.  There are a number of input
   // variables to control what to do in the event there is currently no read line data object.  It
   // is important to note that this is NOT a handle to a copy of the read data object; it is the
   // SAME object.  Thus, if the object is subsequently modified in some manner, the transaction
   // descriptor's read data will also be modified.
   //
   // Input Variables:
   //
   // create_data_in:        If no read data object currently exists and this variable is 1'b1, a
   //                        new line data object will be created, m_read_data will be set to point
   //                        to this new object, and a handle to the new object will be returned.
   // random_data_in:        This input variable only has meaning when a new line data object is
   //                        created.  If set to 1'b1, the new line data object will be filled with
   //                        random data rather than 0s.
   // template_line_data_in: This input variable is only used when creating a new line data object.
   //                        If this variable is not null, the incoming template will be used when
   //                        creating the new line data object.  If it is null, the descriptor's
   //                        m_template_line_data object will be used for creation.
   //
   // Return Value:
   //
   // evl_line_data: This is a handle to the descriptor's read data object.  If no read data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_read_data(input bit           create_data_in        = 1'b0,
                                        input bit           random_data_in        = 1'b0,
                                        input evl_line_data template_line_data_in = null);
      if ((m_read_data == null) && (create_data_in == 1'b1)) begin
         m_read_data = create_data(random_data_in, template_line_data_in);
      end
      return m_read_data;
   endfunction : get_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_normalized_read_data
   //
   // This function is used to get a normalized copy of the read data.
   //
   // Input Variables:
   //
   // create_data_in:        If no read data object currently exists and this variable is 1'b1, a
   //                        new line data object will be created, m_read_data will be set to point
   //                        to this new object, and a handle to the normalized data will be
   //                        returned.
   // random_data_in:        This input variable only has meaning when a new line data object is
   //                        created.  If set to 1'b1, the new line data object will be filled with
   //                        random data rather than 0s.
   // template_line_data_in: This input variable is only used when creating a new line data object.
   //                        If this variable is not null, the incoming template will be used when
   //                        creating the new line data object.  If it is null, the descriptor's
   //                        m_template_line_data object will be used for creation.
   //
   // Return Value:
   //
   // evl_line_data: This is a handle to the normalized read data object.  If no read data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_normalized_read_data(input bit           create_data_in        = 1'b0,
                                                   input bit           random_data_in        = 1'b0,
                                                   input evl_line_data template_line_data_in = null);
      if (m_read_data == null) begin
         if (create_data_in == 1'b0) begin
            return null;
         end
         m_read_data = create_data(random_data_in, template_line_data_in);
      end
      return m_read_data.get_normalized_data(get_paddr(), get_cmd_length());
   endfunction : get_normalized_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_byte
   //
   // This function is used to set a byte in the m_read_data object at the specified byte offset.
   // If the m_read_data handle is null, a new line data object based on this descriptor's line
   // data template will be created (filled with 0s) and the specified byte will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the byte offset within the m_read_data object to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_read_byte(input int byte_index_in, input byte byte_in);
      m_read_data = set_byte(m_read_data, byte_index_in, byte_in);
   endfunction : set_read_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_hword
   //
   // This function is used to set a halfword (2 bytes) in the m_read_data object at the specified
   // halfword offset.  If the m_read_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified halfword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the m_read_data object to modify (address
   //                 bits 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_read_hword(input int hword_index_in, input evl_hword_t hword_in);
      m_read_data = set_hword(m_read_data, hword_index_in, hword_in);
   endfunction : set_read_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_word
   //
   // This function is used to set a word (4 bytes) in the m_read_data object at the specified word
   // offset.  If the m_read_data handle is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified word will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within the m_read_data object to modify (address
   //                bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_read_word(input int word_index_in, input evl_word_t word_in);
      m_read_data = set_word(m_read_data, word_index_in, word_in);
   endfunction : set_read_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_dword
   //
   // This function is used to set a doubleword (8 bytes) in the m_read_data object at the
   // specified doubleword offset.  If the m_read_data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // doubleword will be modified appropriately.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_read_data object to modify (address
   //                 bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   function void set_read_dword(input int dword_index_in, input evl_dword_t dword_in);
      m_read_data = set_dword(m_read_data, dword_index_in, dword_in);
   endfunction : set_read_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_qword
   //
   // This function is used to set a quadword (16 bytes) in the m_read_data object at the specified
   // quadword offset.  If the m_read_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified quadword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the m_read_data object to modify (address
   //                 bits 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_read_qword(input int qword_index_in, input evl_qword_t qword_in);
      m_read_data = set_qword(m_read_data, qword_index_in, qword_in);
   endfunction : set_read_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_oword
   //
   // This function is used to set an octalword (32 bytes) in the m_read_data object at the
   // specified octalword offset.  If the m_read_data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // octalword will be modified appropriately.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the m_read_data object to modify (address
   //                 bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_read_oword(input int oword_index_in, input evl_oword_t oword_in);
      m_read_data = set_oword(m_read_data, oword_index_in, oword_in);
   endfunction : set_read_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_read_xword
   //
   // This function is used to set a hexword (64 bytes) in the m_read_data object at the specified
   // hexword offset.  If the m_read_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified hexword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the m_read_data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_read_xword(input int xword_index_in, input evl_xword_t xword_in);
      m_read_data = set_xword(m_read_data, xword_index_in, xword_in);
   endfunction : set_read_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_byte
   //
   // This function is used to get a byte from the m_read_data object at the specified byte offset.
   // If the m_read_data handle is null and the create_data_in input is 1'b1, a new m_read_data
   // object based on this descriptor's line data template will be created (filled with 0s) and the
   // specified halfword will be returned.
   //
   // Input Variables:
   //
   // byte_index_in:  This is the byte offset within the m_read_data object to read.
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // byte: This is the m_read_data byte at the specified byte offset.
   //
   function byte get_read_byte(input int byte_index_in, input bit create_data_in = 1'b0);
      return get_byte(m_read_data, byte_index_in, create_data_in);
   endfunction : get_read_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_hword
   //
   // This function is used to get a halfword (2 bytes) from the m_read_data object at the
   // specified halfword offset.  If the m_read_data handle is null and the create_data_in input is
   // 1'b1, a new m_read_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified halfword will be returned.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the m_read_data object to read (address
   //                 bits 5-1).
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_hword_t: This is the m_read_data halfword at the specified halfword offset.
   //
   function evl_hword_t get_read_hword(input int hword_index_in, input bit create_data_in = 1'b0);
      return get_hword(m_read_data, hword_index_in, create_data_in);
   endfunction : get_read_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_word
   //
   // This function is used to get a word (4 bytes) from the m_read_data object at the specified
   // word offset.  If the m_read_data handle is null and the create_data_in input is 1'b1, a new
   // m_read_data object based on this descriptor's line data template will be created (filled with
   // 0s) and the specified word will be returned.
   //
   // Input Variables:
   //
   // word_index_in:  This is the word offset within the m_read_data object to read (address
   //                 bits 5-2).
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_word_t: This is the m_read_data word at the specified word offset.
   //
   function evl_word_t get_read_word(input int word_index_in, input bit create_data_in = 1'b0);
      return get_word(m_read_data, word_index_in, create_data_in);
   endfunction : get_read_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_dword
   //
   // This function is used to get a doubleword (8 bytes) from the m_read_data object at the
   // specified doubleword offset.  If the m_read_data handle is null and the create_data_in input
   // is 1'b1, a new m_read_data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified doubleword will be returned.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_read_data object to read (address
   //                 bits 5-3).
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_dword_t: This is the m_read_data doubleword at the specified doubleword offset.
   //
   function evl_dword_t get_read_dword(input int dword_index_in, input bit create_data_in = 1'b0);
      return get_dword(m_read_data, dword_index_in, create_data_in);
   endfunction : get_read_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_qword
   //
   // This function is used to get a quadword (16 bytes) from the m_read_data object at the
   // specified quadword offset.  If the m_read_data handle is null and the create_data_in input is
   // 1'b1, a new m_read_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified quadword will be returned.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the m_read_data object to read (address
   //                 bits 5-4).
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_qword_t: This is the m_read_data quadword at the specified quadword offset.
   //
   function evl_qword_t get_read_qword(input int qword_index_in, input bit create_data_in = 1'b0);
      return get_qword(m_read_data, qword_index_in, create_data_in);
   endfunction : get_read_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_oword
   //
   // This function is used to get an octalword (32 bytes) from the m_read_data object at the
   // specified octalword offset.  If the m_read_data handle is null and the create_data_in input
   // is 1'b1, a new m_read_data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified octalword will be returned.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the m_read_data object to read (address
   //                 bit 5).
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_oword_t: This is the m_read_data octalword at the specified octalword offset.
   //
   function evl_oword_t get_read_oword(input int oword_index_in, input bit create_data_in = 1'b0);
      return get_oword(m_read_data, oword_index_in, create_data_in);
   endfunction : get_read_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_read_xword
   //
   // This function is used to get a hexword (64 bytes) from the m_read_data object at the
   // specified hexword offset.  If the m_read_data handle is null and the create_data_in input is
   // 1'b1, a new m_read_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified hexword will be returned.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the m_read_data object to read (always 0).
   // create_data_in: If this input is 1'b1, a new m_read_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_xword_t: This is the m_read_data hexword at the specified hexword offset.
   //
   function evl_xword_t get_read_xword(input int xword_index_in, input bit create_data_in = 1'b0);
      return get_xword(m_read_data, xword_index_in, create_data_in);
   endfunction : get_read_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_data
   //
   // This function is used to set the observed data object.  It is possible to set individual
   // observed data bytes with other functions in the descriptor, but the assumption here is that a
   // previously-created line data object has been initialized appropriately.  It is important to
   // note that this is NOT a copy of the object passed in; it is the SAME object.  Thus, if the
   // object is subsequently modified in some manner, the transaction descriptor's observed data
   // will also be modified.
   //
   // Input Variables:
   //
   // line_data_in: This is a handle to the line data object to be used as this transaction
   //               descriptor's observed data.
   //
   function void set_obs_data(input evl_line_data line_data_in);
      m_obs_data = line_data_in;
   endfunction : set_obs_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_data
   //
   // This function is used to get a handle to the observed data object.  There are a number of
   // input variables to control what to do in the event there is currently no observed line data
   // object.  It is important to note that this is NOT a handle to a copy of the observed data
   // object; it is the SAME object.  Thus, if the object is subsequently modified in some manner,
   // the transaction descriptor's observed data will also be modified.
   //
   // Input Variables:
   //
   // create_data_in:        If no observed data object currently exists and this variable is 1'b1,
   //                        a new line data object will be created, m_obs_data will be set to point
   //                        to this new object, and a handle to the new object will be returned.
   // random_data_in:        This input variable only has meaning when a new line data object is
   //                        created.  If set to 1'b1, the new line data object will be filled with
   //                        random data rather than 0s.
   // template_line_data_in: This input variable is only used when creating a new line data object.
   //                        If this variable is not null, the incoming template will be used when
   //                        creating the new line data object.  If it is null, the descriptor's
   //                        m_template_line_data object will be used for creation.
   //
   // Return Value:
   //
   // evl_line_data: This is a handle to the descriptor's observed data object.  If no observed data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_obs_data(input bit           create_data_in        = 1'b0,
                                       input bit           random_data_in        = 1'b0,
                                       input evl_line_data template_line_data_in = null);
      if ((m_obs_data == null) && (create_data_in == 1'b1)) begin
         m_obs_data = create_data(random_data_in, template_line_data_in);
      end
      return m_obs_data;
   endfunction : get_obs_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_byte
   //
   // This function is used to set a byte in the m_obs_data object at the specified byte offset.
   // If the m_obs_data handle is null, a new line data object based on this descriptor's line
   // data template will be created (filled with 0s) and the specified byte will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the byte offset within the m_obs_data object to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_obs_byte(input int byte_index_in, input byte byte_in);
      m_obs_data = set_byte(m_obs_data, byte_index_in, byte_in);
   endfunction : set_obs_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_hword
   //
   // This function is used to set a halfword (2 bytes) in the m_obs_data object at the specified
   // halfword offset.  If the m_obs_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified halfword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the m_obs_data object to modify (address
   //                 bits 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_obs_hword(input int hword_index_in, input evl_hword_t hword_in);
      m_obs_data = set_hword(m_obs_data, hword_index_in, hword_in);
   endfunction : set_obs_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_word
   //
   // This function is used to set a word (4 bytes) in the m_obs_data object at the specified word
   // offset.  If the m_obs_data handle is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified word will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within the m_obs_data object to modify (address
   //                bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_obs_word(input int word_index_in, input evl_word_t word_in);
      m_obs_data = set_word(m_obs_data, word_index_in, word_in);
   endfunction : set_obs_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_dword
   //
   // This function is used to set a doubleword (8 bytes) in the m_obs_data object at the
   // specified doubleword offset.  If the m_obs_data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // doubleword will be modified appropriately.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_obs_data object to modify (address
   //                 bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   function void set_obs_dword(input int dword_index_in, input evl_dword_t dword_in);
      m_obs_data = set_dword(m_obs_data, dword_index_in, dword_in);
   endfunction : set_obs_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_qword
   //
   // This function is used to set a quadword (16 bytes) in the m_obs_data object at the specified
   // quadword offset.  If the m_obs_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified quadword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the m_obs_data object to modify (address
   //                 bits 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_obs_qword(input int qword_index_in, input evl_qword_t qword_in);
      m_obs_data = set_qword(m_obs_data, qword_index_in, qword_in);
   endfunction : set_obs_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_oword
   //
   // This function is used to set an octalword (32 bytes) in the m_obs_data object at the
   // specified octalword offset.  If the m_obs_data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // octalword will be modified appropriately.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the m_obs_data object to modify (address
   //                 bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_obs_oword(input int oword_index_in, input evl_oword_t oword_in);
      m_obs_data = set_oword(m_obs_data, oword_index_in, oword_in);
   endfunction : set_obs_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_xword
   //
   // This function is used to set a hexword (64 bytes) in the m_obs_data object at the specified
   // hexword offset.  If the m_obs_data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified hexword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the m_obs_data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_obs_xword(input int xword_index_in, input evl_xword_t xword_in);
      m_obs_data = set_xword(m_obs_data, xword_index_in, xword_in);
   endfunction : set_obs_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_byte
   //
   // This function is used to get a byte from the m_obs_data object at the specified byte offset.
   // If the m_obs_data handle is null and the create_data_in input is 1'b1, a new m_obs_data
   // object based on this descriptor's line data template will be created (filled with 0s) and the
   // specified halfword will be returned.
   //
   // Input Variables:
   //
   // byte_index_in:  This is the byte offset within the m_obs_data object to read.
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // byte: This is the m_obs_data byte at the specified byte offset.
   //
   function byte get_obs_byte(input int byte_index_in, input bit create_data_in = 1'b0);
      return get_byte(m_obs_data, byte_index_in, create_data_in);
   endfunction : get_obs_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_hword
   //
   // This function is used to get a halfword (2 bytes) from the m_obs_data object at the
   // specified halfword offset.  If the m_obs_data handle is null and the create_data_in input is
   // 1'b1, a new m_obs_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified halfword will be returned.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the m_obs_data object to read (address
   //                 bits 5-1).
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_hword_t: This is the m_obs_data halfword at the specified halfword offset.
   //
   function evl_hword_t get_obs_hword(input int hword_index_in, input bit create_data_in = 1'b0);
      return get_hword(m_obs_data, hword_index_in, create_data_in);
   endfunction : get_obs_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_word
   //
   // This function is used to get a word (4 bytes) from the m_obs_data object at the specified
   // word offset.  If the m_obs_data handle is null and the create_data_in input is 1'b1, a new
   // m_obs_data object based on this descriptor's line data template will be created (filled with
   // 0s) and the specified word will be returned.
   //
   // Input Variables:
   //
   // word_index_in:  This is the word offset within the m_obs_data object to read (address
   //                 bits 5-2).
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_word_t: This is the m_obs_data word at the specified word offset.
   //
   function evl_word_t get_obs_word(input int word_index_in, input bit create_data_in = 1'b0);
      return get_word(m_obs_data, word_index_in, create_data_in);
   endfunction : get_obs_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_dword
   //
   // This function is used to get a doubleword (8 bytes) from the m_obs_data object at the
   // specified doubleword offset.  If the m_obs_data handle is null and the create_data_in input
   // is 1'b1, a new m_obs_data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified doubleword will be returned.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_obs_data object to read (address
   //                 bits 5-3).
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_dword_t: This is the m_obs_data doubleword at the specified doubleword offset.
   //
   function evl_dword_t get_obs_dword(input int dword_index_in, input bit create_data_in = 1'b0);
      return get_dword(m_obs_data, dword_index_in, create_data_in);
   endfunction : get_obs_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_qword
   //
   // This function is used to get a quadword (16 bytes) from the m_obs_data object at the
   // specified quadword offset.  If the m_obs_data handle is null and the create_data_in input is
   // 1'b1, a new m_obs_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified quadword will be returned.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the m_obs_data object to read (address
   //                 bits 5-4).
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_qword_t: This is the m_obs_data quadword at the specified quadword offset.
   //
   function evl_qword_t get_obs_qword(input int qword_index_in, input bit create_data_in = 1'b0);
      return get_qword(m_obs_data, qword_index_in, create_data_in);
   endfunction : get_obs_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_oword
   //
   // This function is used to get an octalword (32 bytes) from the m_obs_data object at the
   // specified octalword offset.  If the m_obs_data handle is null and the create_data_in input
   // is 1'b1, a new m_obs_data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified octalword will be returned.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the m_obs_data object to read (address
   //                 bit 5).
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_oword_t: This is the m_obs_data octalword at the specified octalword offset.
   //
   function evl_oword_t get_obs_oword(input int oword_index_in, input bit create_data_in = 1'b0);
      return get_oword(m_obs_data, oword_index_in, create_data_in);
   endfunction : get_obs_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_obs_xword
   //
   // This function is used to get a hexword (64 bytes) from the m_obs_data object at the
   // specified hexword offset.  If the m_obs_data handle is null and the create_data_in input is
   // 1'b1, a new m_obs_data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified hexword will be returned.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the m_obs_data object to read (always 0).
   // create_data_in: If this input is 1'b1, a new m_obs_data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_xword_t: This is the m_obs_data hexword at the specified hexword offset.
   //
   function evl_xword_t get_obs_xword(input int xword_index_in, input bit create_data_in = 1'b0);
      return get_xword(m_obs_data, xword_index_in, create_data_in);
   endfunction : get_obs_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_write_qword_en
   //
   // This function is used to get the per-line quadword enables for store operations.
   //
   // Return Value:
   //
   // evl_line_qword_en_t: This is the per-line quadword enables.
   //
   function evl_line_qword_en_t get_write_qword_en();
      if (m_write_data != null) begin
         return m_write_data.get_qword_en();
      end
      return ~evl_line_qword_en_t'(0);
   endfunction : get_write_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_graduated
   //
   // This function is used to mark the transaction descriptor as graduated.
   //
   function void set_graduated();
      m_graduated = 1'b1;
   endfunction : set_graduated


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_graduated
   //
   // This function is used to get the graduation state of the transaction descriptor.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if the transaction has graduated.
   //
   function bit has_graduated();
      return m_graduated;
   endfunction : has_graduated


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_committed
   //
   // This function is used to mark the transaction descriptor as committed.
   //
   function void set_committed();
      m_committed = 1'b1;
   endfunction : set_committed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_committed
   //
   // This function is used to get the commitment state of the transaction descriptor.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if the transaction has been committed.
   //
   function bit has_committed();
      return m_committed;
   endfunction : has_committed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_visible
   //
   // This function is used to mark the transaction descriptor as visible to other threads/CPUs.
   // This information is only meaningful for transactions that modify data.
   //
   function void set_visible();
      m_visible = 1'b1;
   endfunction : set_visible


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_visible
   //
   // This function is used to get the visibility of a modifying transaction descriptor.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if the modification is visible to other threads or CPUs.
   //
   function bit is_visible();
      return m_visible;
   endfunction : is_visible


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_can_drop
   //
   // This function is used to mark the transaction descriptor as a candidate for dropping should a
   // resource or tag conflict be detected.  This is intended to be used solely by stimulus agents;
   // when driving fully-random traffic, it is often legal to just toss out transactions with
   // conflicts rather than retrying them.
   //
   function void set_can_drop();
      m_can_drop = 1'b1;
   endfunction : set_can_drop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: can_drop_trans
   //
   // This function is used to get the dropability state of a transaction descriptor.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if this transaction can be dropped when a tag or resource
   //      conflict is detected.
   //
   function bit can_drop_trans();
      return m_can_drop;
   endfunction : can_drop_trans


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_id_queue
   //
   // This function is used to set the bus request ID queue handle (used for stimulus).
   //
   // Input Variables:
   //
   // bus_req_id_queue_in: This is a handle to which the bus request ID queue handle will be set.
   //
   function void set_bus_req_id_queue(input evl_bus_req_id_queue bus_req_id_queue_in);
      m_req_id_queue = bus_req_id_queue_in;
   endfunction : set_bus_req_id_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_id_queue
   //
   // This function is used to get the bus request ID queue handle (used for stimulus).
   //
   // Return Value:
   //
   // evl_bus_req_id_queue: This is a handle to the bus request ID queue.
   //
   function evl_bus_req_id_queue get_bus_req_id_queue();
      return m_req_id_queue;
   endfunction : get_bus_req_id_queue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   // This function is a redefinition of the UVM do_copy() function.  do_copy() is called by the
   // UVM clone() function.  Note that not all of the information in the descriptor is copied with
   // this function; that is intentional ... only limited information is supposed to be copied when
   // cloning a descriptor.
   //
   // Input Variables:
   //
   // rhs: This is a handle to the descriptor from which to copy information.
   //
   virtual function void do_copy(uvm_object rhs);
      evl_trans_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clone_desc
   //
   // This function is provided mainly as a convenience to create a copy of this transaction
   // descriptor.
   //
   // Input Variables:
   //
   // agent_in: This is a handle to the agent creating a copy of the transaction descriptor.
   //
   // Return Value:
   //
   // evl_trans_desc: This a handle to cloned transaction descriptor.
   //
   function evl_trans_desc clone_desc(input evl_verif_comp agent_in);
      evl_trans_desc trans_desc;

      trans_desc = new(get_abstract_name());
      trans_desc.do_copy(this);
      m_master_agent = agent_in;
      return trans_desc;
   endfunction : clone_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_child_desc
   //
   // This function is a redefinition of the evl_mem_desc add_child_desc() function.  It simply
   // performs the evl_mem_desc add_child_desc() function and then sets the m_req_desc handle to the
   // object if it is the first child request descriptor.
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the descriptor that will be a child of this object.
   //
   // Return Value:
   //
   // int: The index into the queue used for the child descriptor is returned.  Thus, if this is the
   //      fourth child added to the queue, a value of 3 is returned.  There may be no need to know
   //      about the return value.
   //
   virtual function int add_child_desc(input evl_mem_desc mem_desc_in);
      int child_index;

      child_index = super.add_child_desc(mem_desc_in);
      if ((mem_desc_in.get_desc_type() == EVL_DESC_REQ) && (child_index == 0)) begin
         $cast(m_req_desc, mem_desc_in);
      end
      return child_index;
   endfunction : add_child_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_desc
   //
   // This function is provided mainly as a convenience.  It will get the selected request
   // descriptor from its child descriptor queue and cast it into a request descriptor.  If there
   // are no request descriptor children or the selected child does not exist, null will be
   // returned.
   //
   // Input Variables:
   //
   // child_index: This is the index to use for accessing handles in the EVL_DESC_REQ child queue.
   //
   // Return Value:
   //
   // evl_req_desc: This is a handle to the selected child request descriptor.
   //
   virtual function evl_req_desc get_req_desc(input int child_index = 0);
      if (child_index == 0) begin
         return m_req_desc;
      end
      return super.get_req_desc(child_index);
   endfunction : get_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_table_action
   //
   // This function is used to process the specified table descriptor.  Although it is currently
   // empty, the intent is to fill out this function as the needs of stimulus agents and scoreboards
   // are determined.
   //
   // Input Variables:
   //
   // table_desc: This is a handle to the table descriptor to use for processing.
   //
   // Return Value:
   //
   // bit: The return value will be 1'b0 if further evaluation is required.
   //
   function bit process_table_action(input evl_table_desc table_desc);
      return 1'b0;
   endfunction : process_table_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_data_action
   //
   // This function is used to process the specified data action (which is a member of a table
   // descriptor).  Although it is currently empty, the intent is to fill out this function as the
   // needs of stimulus agents and scoreboards are determined.
   //
   // Input Variables:
   //
   // data_action: This is the data action to perform.
   //
   // Return Value:
   //
   // bit: The return value will be 1'b0 if further evaluation is required.
   //
   function bit process_data_action(input evl_data_action_t data_action);
      return 1'b0;
   endfunction : process_data_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_tag_action
   //
   // This function is used to process the specified tag action (which is a member of a table
   // descriptor).  Although it is currently empty, the intent is to fill out this function as the
   // needs of stimulus agents and scoreboards are determined.
   //
   // Input Variables:
   //
   // tag_action: This is the tag action to perform.
   //
   // Return Value:
   //
   // bit: The return value will be 1'b0 if further evaluation is required.
   //
   function bit process_tag_action(input evl_cache_st_t tag_action);
      return 1'b0;
   endfunction : process_tag_action


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_type
   //
   // This function is used to set the bus type for the originating request descriptor.
   //
   // Input Variables:
   //
   // delay_key: This is the bus type to use for the originating request descriptor.
   //
   virtual function void set_bus_type(input evl_bus_type_t bus_type_in);
      if (m_req_desc != null) begin
         m_req_desc.set_bus_type(bus_type_in);
      end
   endfunction : set_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_type
   //
   // This function is used to get the bus type for the originating request descriptor.
   //
   // Return Value:
   //
   // evl_bus_type_t: This function returns the bus type for the originating request descriptor.
   //
   virtual function evl_bus_type_t get_bus_type();
      if (m_req_desc != null) begin
         return m_req_desc.get_bus_type();
      end
      return EVL_BUS_TYPE_UNKNOWN;
   endfunction : get_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   // This function is used to get a name for the specified bus command (if there is an originating
   // request descriptor).  If the input variable is `EVL_Bus_Idle, the bus command in the
   // originating request descriptor is used for evaluation.  If it is something other than
   // `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // string: This function returns the command name for the evaluated bus command.
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.get_bus_cmd_name(bus_cmd_in);
      end
      return "EVL_Bus_Idle";
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_requires_rsp
   //
   // This function is used to determine if the specified bus command requires a response (if there
   // is an originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus
   // command in the originating request descriptor is used for evaluation.  If it is something
   // other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command requires a response.
   //
   virtual function bit bus_cmd_requires_rsp(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_requires_rsp(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_requires_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   // This function is used to determine if the specified bus command is a read (if there is an
   // originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus command in
   // the originating request descriptor is used for evaluation.  If it is something other than
   // `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a read.
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_is_read(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   // This function is used to determine if the specified bus command is a write (if there is an
   // originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus command in
   // the originating request descriptor is used for evaluation.  If it is something other than
   // `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a write.
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_is_write(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   // This function is used to determine if the specified bus command is a cache operation (if there
   // is an originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus
   // command in the originating request descriptor is used for evaluation.  If it is something
   // other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a cache operation.
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_is_cache_op(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_scratchpad
   //
   // This function is used to determine if the specified bus command is a scratchpad operation (if
   // there is an originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus
   // command in the originating request descriptor is used for evaluation.  If it is something
   // other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a scratchpad operation.
   //
   virtual function bit bus_cmd_is_scratchpad(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_is_scratchpad(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_scratchpad


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   // This function is used to determine if the specified bus command is an atomic operation (if
   // there is an originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus
   // command in the originating request descriptor is used for evaluation.  If it is something
   // other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is an atomic operation.
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_is_atomic(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   // This function is used to determine if the specified bus command expects read data (if there is
   // an originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus command
   // in the originating request descriptor is used for evaluation.  If it is something other than
   // `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command expects read data.
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_has_read_data(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   // This function is used to determine if the specified bus command has write data (if there is an
   // originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus command in
   // the originating request descriptor is used for evaluation.  If it is something other than
   // `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command has write data.
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_has_write_data(bus_cmd_in);
      end
      return 1'b0;
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_unique_tag
   //
   //
   // Function: bus_cmd_has_write_data
   //
   // This function is used to determine if the specified bus command has a unique bus tag (i.e., if
   // it is illegal to reuse a particular bus tag before a response is sent ... only if there is an
   // originating request descriptor).  If the input variable is `EVL_Bus_Idle, the bus command in
   // the originating request descriptor is used for evaluation.  If it is something other than
   // `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the originating request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command should use a unique bus tag.
   //
   virtual function bit bus_cmd_has_unique_tag(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_req_desc != null) begin
         return m_req_desc.bus_cmd_has_unique_tag(bus_cmd_in, bus_type_in);
      end
      return 1'b0;
   endfunction : bus_cmd_has_unique_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   // This function is used to get a name for the specific bus response type (if there is an
   // originating request descriptor).  If the input variable is `EVL_Rsp_Null, the bus response
   // type in the originating request descriptor is used for evaluation.  If it is something other
   // than `EVL_Rsp_Null, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in the originating request descriptor is used for
   //                  evaluation.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // string: This function returns the command name for the evaluated bus response type.
   //
   virtual function string get_bus_rsp_name(input evl_bus_rsp_type_t bus_rsp_in = `EVL_Rsp_Null, input int transfer_idx_in = 0);
      if (m_req_desc != null) begin
         return m_req_desc.get_bus_rsp_name(bus_rsp_in, transfer_idx_in);
      end
      return "EVL_Rsp_Null";
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   // This function is used to determine if the specified bus response type has data (if there is an
   // originating request descriptor).  If the input variable is `EVL_Rsp_Null, the bus response
   // type in the originating request descriptor is used for evaluation.  If it is something other
   // than `EVL_Rsp_Null, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in the originating request descriptor is used for
   //                  evaluation.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus response type has data.
   //
   virtual function bit bus_rsp_has_data(input evl_bus_rsp_type_t bus_rsp_in = `EVL_Rsp_Null, input int transfer_idx_in = 0);
      if (m_req_desc != null) begin
         return m_req_desc.bus_rsp_has_data(bus_rsp_in, transfer_idx_in);
      end
      return 1'b0;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   // This function is used to determine if the specified bus response type is an error response (if
   // there is an originating request descriptor).  If the input variable is `EVL_Rsp_Null, the bus
   // response type in the originating request descriptor is used for evaluation.  If it is
   // something other than `EVL_Rsp_Null, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in the originating request descriptor is used for
   //                  evaluation.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus response type is an error response.
   //
   virtual function bit bus_rsp_has_error(input evl_bus_rsp_type_t bus_rsp_in = `EVL_Rsp_Null, input int transfer_idx_in = 0);
      if (m_req_desc != null) begin
         return m_req_desc.bus_rsp_has_error(bus_rsp_in, transfer_idx_in);
      end
      return 1'b0;
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cmd_has_read_data
   //
   // This function returns 1'b1 if this transaction expects read data
   //
   virtual function bit cmd_has_read_data();
      return m_cmd inside {`EVL_CMDS_WITH_READ_DATA} ? 1'b1 : 1'b0;
   endfunction : cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cmd_has_write_data
   //
   // This function returns 1'b1 if this transaction expects write data
   //
   virtual function bit cmd_has_write_data();
      return m_cmd inside {`EVL_CMDS_WITH_WRITE_DATA} ? 1'b1 : 1'b0;
   endfunction : cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cmd_is_cacheop
   //
   // This function returns 1'b1 if this transaction is a cache operation
   //
   virtual function bit cmd_is_cacheop();
      return m_cmd inside {`EVL_COP_CMDS} ? 1'b1 : 1'b0;
   endfunction : cmd_is_cacheop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cmd_is_set_way_cacheop
   //
   // This function returns 1'b1 if this transaction is a cache operation targetting
   // an explicit set and way
   //
   virtual function bit cmd_is_set_way_cacheop();
      return m_cmd inside {`EVL_IDX_COP_CMDS} ? 1'b1 : 1'b0;
   endfunction : cmd_is_set_way_cacheop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cmd_is_idx_cacheop
   //
   // This function is an alias to the cmd_is_set_way_cacheop function
   //
   virtual function bit cmd_is_idx_cacheop();
      return cmd_is_set_way_cacheop();
   endfunction : cmd_is_idx_cacheop


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_done
   //
   // This function is used to mark the descriptor as complete.
   //
   virtual function void set_done();
      super.set_done();
      if ((m_parent_desc != null) && (m_parent_desc != this)) begin
        if (m_parent_desc.all_children_are_done() == 1'b1) begin
           m_parent_desc.set_done();
        end
      end
      if (m_get_stats == 1'b1) begin
         process_desc_stats();
      end
   endfunction : set_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_desc_stats
   //
   // This function is used to process descriptor statistics.
   //
   virtual function void process_desc_stats();
      if (`evl_log_check_verbosity(UVM_MEDIUM, VMOD_DESC_STATS) == 0) begin
         return;
      end
      if (get_child_desc(EVL_DESC_TRANS) != null) begin
         return;
      end
//    if ((m_ancestor_desc != null) && (m_ancestor_desc != this)) begin
//       m_ancestor_desc.process_desc_stats();
//       return;
//    end
//    if ((m_parent_desc != null) && (m_parent_desc != this)) begin
//       m_parent_desc.process_desc_stats();
//       return;
//    end
      if ((is_done() == 1'b0) || (desc_family_is_done() == 1'b0)) begin
         return;
      end
      if (`evl_log_check_verbosity(UVM_MEDIUM, VMOD_DESC_STATS) != 0) begin
         gather_desc_stats(0, m_desc_stats);
         if (m_desc_stats.size() > 1) begin
            int    limit = m_desc_stats.size();
            string msg_out = "";

            for (int ii = 0; ii < limit; ii++) begin
               msg_out = $sformatf("%0s%0s", msg_out, print_desc_stats(m_desc_stats[ii]));
            end
            `evl_log_mdesc(UVM_MEDIUM, VMOD_DESC_STATS, get_abstract_name(), this, $sformatf("Stats:%0s", msg_out))
         end
      end
   endfunction : process_desc_stats


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_desc_stats
   //
   // This function is used to print the descriptor statistics.
   //
   function string print_desc_stats(ref evl_desc_stat_t desc_stats);
      string prefix;

      case (desc_stats.bus_type)
        EVL_BUS_TYPE_ETLINK:  prefix = $sformatf(" #%0d-ETLINK", desc_stats.level);
        EVL_BUS_TYPE_AXI:     prefix = $sformatf(" #%0d-AXI", desc_stats.level);
        EVL_BUS_TYPE_APB:     prefix = $sformatf(" #%0d-APB", desc_stats.level);
        EVL_BUS_TYPE_JTAG:    prefix = $sformatf(" #%0d-JTAG", desc_stats.level);
        EVL_BUS_TYPE_DBG_NOC: prefix = $sformatf(" #%0d-DBG_NOC", desc_stats.level);
        EVL_BUS_TYPE_NEMI:    prefix = $sformatf(" #%0d-NEMI", desc_stats.level);
        default:              prefix = $sformatf(" #%0d", desc_stats.level);
      endcase

      if (desc_stats.done_time > 0) begin
         real done_time = $itor(desc_stats.done_time) / 1000.0;

         if (desc_stats.process_time > 0) begin
            real process_time = $itor(desc_stats.process_time) / 1000.0;;

            if (desc_stats.issue_time > 0) begin
               real issue_time = $itor(desc_stats.issue_time) / 1000.0;;

               return $sformatf("%0s %4.3f,%4.3f,%4.3f", prefix, issue_time, process_time, done_time);
            end
            else begin
               return $sformatf("%0s -,%4.3f,%4.3f", prefix, process_time, done_time);
            end
         end
         else if (desc_stats.issue_time > 0) begin
            real issue_time = $itor(desc_stats.issue_time) / 1000.0;;

            return $sformatf("%0s %4.3f,-,%4.3f", prefix, issue_time, done_time);
         end
         else begin
            return $sformatf("%0s -,-,%4.3f", prefix, done_time);
         end
      end
      return $sformatf("%0s -,-,-", prefix);
   endfunction : print_desc_stats


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_mem_desc", input evl_verif_comp agent_in = null);
      super.new(name_in);
      m_desc_type = EVL_DESC_TRANS;
      if (agent_in != null) begin
         m_master_agent = agent_in;
         set_abstract_name(m_master_agent.get_abstract_name());
      end
      m_global_attrs.save_trans_desc(this);
   endfunction : new

endclass : evl_trans_desc
