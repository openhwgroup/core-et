//--------------------------------------------------------------------------------------------------
//
// Class: evl_line_data
//
class evl_line_data extends evl_object;

   `uvm_object_utils(evl_line_data)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Variables and Objects
   //
   // m_setup_done:        This static variable is set by the first created instance of this object.
   //                      It prevents subsequent instance creations from filling out the
   //                      m_dword_mask_byte[] array.
   //                      -------------------------------------------------------------------------
   // m_dword_mask_byte[]: This static array is used to simplify selecting bytes for updating when
   //                      not all bytes of the doubleword are changing.
   //
   static bit         m_setup_done = 1'b0;
   static evl_dword_t m_dword_mask_byte[`EVL_DWORD_BYTES+2];


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_dwords[]:   This array contains a full cache line of data arranged as doublewords.
   //               --------------------------------------------------------------------------------
   // m_valids[]:   This array contains byte-valid bits for each byte in the cache line, arranged as
   //               per-doubleword instances.
   //               --------------------------------------------------------------------------------
   // m_unknowns[]: This array contains byte-unknown bits for each byte in the cache line, arranged
   //               as per-doubleword instances.  A particular byte may valid (its m_valids[] bit is
   //               set), but the data may be unknown.  This particular case may never exist in the
   //               current environment.
   //
   evl_dword_t         m_dwords[`EVL_LINE_DWORDS];
   evl_dword_byte_en_t m_valids[`EVL_LINE_DWORDS];
   evl_dword_byte_en_t m_unknowns[`EVL_LINE_DWORDS];
   evl_dword_byte_en_t m_modified[`EVL_LINE_DWORDS];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: post_process_data
   //
   // This function is intended to be redefined in classes extended from evl_line_data.  The model
   // is that any parity or ECC generation after data has been modified would be done here.  This
   // function is called at the end of data-modifying functions (create_data(),
   // set_bytes_in_dword(), validate_line(), zero_and_validate_line(), and copy_data_from_line()).
   //
   virtual function void post_process_data();
      return;
   endfunction : post_process_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_data
   //
   // This function is used to create valid, known data in this object.  If the input variable
   // rand_gen is null, this object will be filled with 0s.  If it is not null, it will be used to
   // generate random data.  As mentioned, note that all bytes will be valid after this function; if
   // you want to initialize data but then mark it all as invalid, a subsequent call to
   // invalidate_line() will be required.
   //
   // Input Variables:
   //
   // rand_gen: This is a handle to an evl_rand_gen random generator.  If it is null, m_dwords[]
   //           will contain all zeroes.
   //
   function void create_data(input evl_rand_gen rand_gen = null);
      if (rand_gen != null) begin
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            m_dwords[ii]   = rand_gen.get_dword();
            m_valids[ii]   = ~evl_dword_byte_en_t'(0);
            m_unknowns[ii] = evl_dword_byte_en_t'(0);
            m_modified[ii] = evl_dword_byte_en_t'(0);
         end
      end
      else begin
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            m_dwords[ii]   = evl_dword_t'(0);
            m_valids[ii]   = evl_dword_byte_en_t'(0);
            m_unknowns[ii] = evl_dword_byte_en_t'(0);
            m_modified[ii] = evl_dword_byte_en_t'(0);
         end
      end
      post_process_data();
   endfunction : create_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bytes_in_dword
   //
   // This function is used to set specific bytes in a selected doubleword.  This function is the
   // central method used by all of the other "set_nnn()" functions.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the m_write_data object to modify
   //                 (address bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   // byte_en_in:     This is a doubleword byte enable specifying which bytes to modify.
   // unknown_in:     This is a doubleword byte enable specifying which bytes to mark as unknown.
   //                 Only those bytes being modified can be marked as unknown.
   // modified_in:    This is a doubleword byte enable specifying which bytes to mark as modified.
   //                 Only those bytes being modified can be marked as modified.
   //
   function void set_bytes_in_dword(input int                 dword_index_in,
                                    input evl_dword_t         dword_in,
                                    input evl_dword_byte_en_t byte_en_in,
                                    input evl_dword_byte_en_t unknown_in  = evl_dword_byte_en_t'(0),
                                    input evl_dword_byte_en_t modified_in = evl_dword_byte_en_t'(0));
      int         dword_index = dword_index_in & ((1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);
      evl_dword_t dword_mask;

      if (byte_en_in == evl_dword_byte_en_t'(0)) begin
         return;
      end
      if (byte_en_in == ~evl_dword_byte_en_t'(0)) begin
         dword_mask = ~evl_dword_t'(0);
      end
      else if (byte_en_in == ((~evl_dword_byte_en_t'(0)) >> `EVL_WORD_BYTES)) begin
         dword_mask = m_dword_mask_byte[`EVL_DWORD_BYTES];
      end
      else if (byte_en_in == ((~evl_dword_byte_en_t'(0)) << `EVL_WORD_BYTES)) begin
         dword_mask = m_dword_mask_byte[`EVL_DWORD_BYTES+1];
      end
      else begin
         int                 ii;
         evl_dword_byte_en_t byte_en;

         ii         = 0;
         byte_en    = byte_en_in;
         dword_mask = evl_dword_t'(0);
         while (byte_en != evl_dword_byte_en_t'(0)) begin
            if (byte_en[0:0] == 1'b1) begin
               dword_mask |= m_dword_mask_byte[ii];
            end
            byte_en = byte_en >> 1;
            ii++;
         end
      end
      m_dwords[dword_index]   &= ~dword_mask;
      m_dwords[dword_index]   |= dword_mask & dword_in;
      m_valids[dword_index]   |= byte_en_in;
      m_unknowns[dword_index] &= ~byte_en_in;
      m_unknowns[dword_index] |= byte_en_in & unknown_in;
      m_modified[dword_index] |= byte_en_in & modified_in;
      post_process_data();
   endfunction : set_bytes_in_dword


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
   // unknown_in:    This is a doubleword byte enable specifying which bytes to mark as unknown.
   //                Only those bytes being modified can be marked as unknown.
   // modified_in:   This is a doubleword byte enable specifying which bytes to mark as modified.
   //
   function void set_bytes(input int                 byte_index_in,
                           input int                 length_in,
                           input evl_xword_t         xword_in,
                           input evl_xword_byte_en_t byte_en_in,
                           input evl_xword_byte_en_t unknown_in  = evl_xword_byte_en_t'(0),
                           input evl_xword_byte_en_t modified_in = evl_xword_byte_en_t'(0));
      int                 byte_index;
      int                 dword_offset;
      int                 dword_length;
      int                 dword_index;
      int                 dword_index_init;
      int                 dword_shift;
      evl_xword_t         xword;
      evl_xword_byte_en_t byte_en;
      evl_xword_byte_en_t unknown;
      evl_xword_byte_en_t modified;
      evl_dword_t         dword;
      evl_dword_byte_en_t dword_byte_en;
      evl_dword_byte_en_t dword_unknown;
      evl_dword_byte_en_t dword_modified;

      if (byte_en_in == evl_xword_byte_en_t'(0)) begin
         return;
      end

      // Only keep offset inside cacheline
      byte_index       = byte_index_in & ((1 << `EVL_LINE_ADDR_LSB) - 1);
      dword_offset     = byte_index & `EVL_DWORD_OFFSET_MASK;
      dword_index_init = int'((byte_index & const_dword_paddr_mask & ~const_line_paddr_mask) >> `EVL_DWORD_ADDR_LSB);

      // align everything to dword
      xword    = xword_in    << (dword_offset << 3);
      byte_en  = byte_en_in  <<  dword_offset;
      unknown  = unknown_in  <<  dword_offset;
      modified = modified_in <<  dword_offset;

      dword_length = (length_in + dword_offset + (`EVL_DWORD_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB;
      for (int ii = 0; ii < dword_length; ii++) begin
         dword_index    = dword_index_init + ii;
         dword_shift    = dword_index - dword_index_init;
         dword          = evl_dword_t'((xword >> (dword_shift << 6)) & ~(evl_dword_t'(0)));
         dword_byte_en  = evl_dword_byte_en_t'((byte_en  >> (dword_shift << 3)) & ~(evl_dword_byte_en_t'(0)));
         dword_unknown  = evl_dword_byte_en_t'((unknown  >> (dword_shift << 3)) & ~(evl_dword_byte_en_t'(0)));
         dword_modified = evl_dword_byte_en_t'((modified >> (dword_shift << 3)) & ~(evl_dword_byte_en_t'(0)));
         set_bytes_in_dword(dword_index, dword, dword_byte_en, dword_unknown, dword_modified);
         // Stop when we populated the top dword of the line
         if (dword_index == (`EVL_LINE_DWORDS - 1)) begin
            break;
         end
      end
   endfunction : set_bytes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_byte
   //
   // This function is used to set a byte at the specified offset.
   //
   // Input Variables:
   //
   // byte_index_in: This is the offset within this object to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_byte(input int  byte_index_in,
                          input byte byte_in,
                          input bit  byte_en_in = 1'b1);
      int                 offset  = byte_index_in & ((1 << `EVL_DWORD_ADDR_LSB) - 1);
      evl_dword_t         dword   = evl_dword_t'(byte_in);
      evl_dword_byte_en_t byte_en = evl_dword_byte_en_t'(byte_en_in) & evl_dword_byte_en_t'(1);

      set_bytes_in_dword((byte_index_in >> (`EVL_DWORD_ADDR_LSB)),
                         (dword         << (offset << 3)),
                         (byte_en       << (offset)));
   endfunction : set_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_hword
   //
   // This function is used to set a halfword (2 bytes) at the specified halfword offset.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within this object to modify (address bits 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_hword(input int                 hword_index_in,
                           input evl_hword_t         hword_in,
                           input evl_hword_byte_en_t byte_en_in = ~evl_hword_byte_en_t'(0));
      int                 offset  = hword_index_in & ((1 << (`EVL_DWORD_ADDR_LSB - `EVL_HWORD_ADDR_LSB)) - 1);
      evl_dword_t         dword   = evl_dword_t'(hword_in);
      evl_dword_byte_en_t byte_en = evl_dword_byte_en_t'(byte_en_in) & evl_dword_byte_en_t'((1 << `EVL_HWORD_BYTES) - 1);

      set_bytes_in_dword((hword_index_in >> (`EVL_DWORD_ADDR_LSB - `EVL_HWORD_ADDR_LSB)),
                         (dword          << (offset << (`EVL_HWORD_ADDR_LSB + 3))),
                         (byte_en        << (offset << `EVL_HWORD_ADDR_LSB)));
   endfunction : set_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_word
   //
   // This function is used to set a word (4 bytes) at the specified word offset.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within this object to modify (address bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_word(input int                word_index_in,
                          input evl_word_t         word_in,
                          input evl_word_byte_en_t byte_en_in = ~evl_word_byte_en_t'(0));
      int                 offset  = word_index_in & ((1 << (`EVL_DWORD_ADDR_LSB - `EVL_WORD_ADDR_LSB)) - 1);
      evl_dword_t         dword   = evl_dword_t'(word_in);
      evl_dword_byte_en_t byte_en = evl_dword_byte_en_t'(byte_en_in) & evl_dword_byte_en_t'((1 << `EVL_WORD_BYTES) - 1);

      set_bytes_in_dword((word_index_in >> (`EVL_DWORD_ADDR_LSB - `EVL_WORD_ADDR_LSB)),
                         (dword         << (offset << (`EVL_WORD_ADDR_LSB + 3))),
                         (byte_en       << (offset << `EVL_WORD_ADDR_LSB)));
   endfunction : set_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_dword
   //
   // This function is used to set a doubleword (8 bytes) at the specified doubleword offset.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to modify (address bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   function void set_dword(input int                 dword_index_in,
                           input evl_dword_t         dword_in,
                           input evl_dword_byte_en_t byte_en_in = ~evl_dword_byte_en_t'(0));
      set_bytes_in_dword(dword_index_in, dword_in, byte_en_in);
   endfunction : set_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_qword
   //
   // This function is used to set a quadword (16 bytes) at the specified quadword offset.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within this object to modify (address bits 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_qword(input int                 qword_index_in,
                           input evl_qword_t         qword_in,
                           input evl_qword_byte_en_t byte_en_in = ~evl_qword_byte_en_t'(0));
      int                 dword_index = qword_index_in << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB);
      evl_qword_t         qword       = qword_in;
      evl_qword_byte_en_t byte_en     = byte_en_in;

      repeat (1 << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         set_bytes_in_dword(dword_index, evl_dword_t'(qword), evl_dword_byte_en_t'(byte_en));
         qword   = qword >> (`EVL_DWORD_BYTES << 3);
         byte_en = byte_en >> (`EVL_DWORD_BYTES);
         dword_index++;
      end
   endfunction : set_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_oword
   //
   // This function is used to set an octalword (32 bytes) at the specified octalword offset.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within this object to modify (address bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_oword(input int                 oword_index_in,
                           input evl_oword_t         oword_in,
                           input evl_oword_byte_en_t byte_en_in = ~evl_oword_byte_en_t'(0));
      int                 dword_index = oword_index_in << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB);
      evl_oword_t         oword       = oword_in;
      evl_oword_byte_en_t byte_en     = byte_en_in;

      repeat (1 << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         set_bytes_in_dword(dword_index, evl_dword_t'(oword), evl_dword_byte_en_t'(byte_en));
         oword   = oword >> (`EVL_DWORD_BYTES << 3);
         byte_en = byte_en >> (`EVL_DWORD_BYTES);
         dword_index++;
      end
   endfunction : set_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_xword
   //
   // This function is used to set an hexword (64 bytes) at the specified hexword offset.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within this object to modify (always 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_xword(input int                 xword_index_in,
                           input evl_xword_t         xword_in,
                           input evl_xword_byte_en_t byte_en_in = ~evl_xword_byte_en_t'(0));
      int                 dword_index = xword_index_in << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB);
      evl_xword_t         xword       = xword_in;
      evl_xword_byte_en_t byte_en     = byte_en_in;

      repeat (1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         set_bytes_in_dword(dword_index, evl_dword_t'(xword), evl_dword_byte_en_t'(byte_en));
         xword   = xword >> (`EVL_DWORD_BYTES << 3);
         byte_en = byte_en >> (`EVL_DWORD_BYTES);
         dword_index++;
      end
   endfunction : set_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_byte
   //
   // This function is used to get a byte at the specified offset.
   //
   // Input Variables:
   //
   // byte_index_in: This is the offset within this object to read.
   //
   // Return Value:
   //
   // byte: This is the byte at the specified offset.
   //
   function byte get_byte(input int byte_index_in);
      int offset = (byte_index_in & ((1 << `EVL_DWORD_ADDR_LSB) - 1)) << 3;

      return byte'(get_dword(byte_index_in >> `EVL_DWORD_ADDR_LSB) >> offset);
   endfunction : get_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_hword
   //
   // This function is used to get a halfword (2 bytes) at the specified halfword offset.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within this object to read (address bits 5-1).
   //
   // Return Value:
   //
   // evl_hword_t: This is the halfword at the specified halfword offset.
   //
   function evl_hword_t get_hword(input int hword_index_in);
      int offset = (hword_index_in & ((1 << (`EVL_DWORD_ADDR_LSB - `EVL_HWORD_ADDR_LSB)) - 1)) << (`EVL_HWORD_ADDR_LSB + 3);

      return evl_hword_t'(get_dword(hword_index_in >> (`EVL_DWORD_ADDR_LSB - `EVL_HWORD_ADDR_LSB)) >> offset);
   endfunction : get_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_word
   //
   // This function is used to get a word (4 bytes) at the specified word offset.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within this object to read (address bits 5-2).
   //
   // Return Value:
   //
   // evl_word_t: This is the word at the specified word offset.
   //
   function evl_word_t get_word(input int word_index);
      int offset = (word_index & ((1 << (`EVL_DWORD_ADDR_LSB - `EVL_WORD_ADDR_LSB)) - 1)) << (`EVL_WORD_ADDR_LSB + 3);

      return (get_dword(word_index >> (`EVL_DWORD_ADDR_LSB - `EVL_WORD_ADDR_LSB)) >> offset);
   endfunction : get_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword
   //
   // This function is used to get a doubleword (8 bytes) at the specified doubleword offset.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to read (address bits 5-3).
   //
   // Return Value:
   //
   // evl_dword_t: This is the doubleword at the specified doubleword offset.
   //
   function evl_dword_t get_dword(input int dword_index_in);
      int dword_index = dword_index_in & ((1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);

      return m_dwords[dword_index];
   endfunction : get_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword
   //
   // This function is used to get a quadword (16 bytes) at the specified quadword offset.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within this object to read (address bits 5-4).
   //
   // Return Value:
   //
   // evl_qword_t: This is the quadword at the specified quadword offset.
   //
   function evl_qword_t get_qword(input int qword_index_in);
      int         dword_index = (qword_index_in << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) | ((1 << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);
      evl_qword_t qword;

      qword = evl_qword_t'(get_dword(dword_index));
      repeat ((1 << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1) begin
         dword_index--;
         qword = qword << (`EVL_DWORD_BYTES << 3);
         qword |= evl_qword_t'(get_dword(dword_index));
      end
      return qword;
   endfunction : get_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_oword
   //
   // This function is used to get an octalword (32 bytes) at the specified octalword offset.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within this object to read (address bit 5).
   //
   // Return Value:
   //
   // evl_oword_t: This is the octalword at the specified octalword offset.
   //
   function evl_oword_t get_oword(input int oword_index_in);
      int         dword_index = (oword_index_in << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) | ((1 << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);
      evl_oword_t oword;

      oword = evl_oword_t'(get_dword(dword_index));
      repeat ((1 << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1) begin
         dword_index--;
         oword = oword << (`EVL_DWORD_BYTES << 3);
         oword |= evl_oword_t'(get_dword(dword_index));
      end
      return oword;
   endfunction : get_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_xword
   //
   // This function is used to get a hexword (64 bytes) at the specified hexword offset.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within this object to read (always 0).
   //
   // Return Value:
   //
   // evl_xword_t: This is the hexword at the specified hexword offset.
   //
   function evl_xword_t get_xword(input int xword_index_in);
      int         dword_index = (xword_index_in << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) | ((1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);
      evl_xword_t xword;

      xword = evl_xword_t'(get_dword(dword_index));
      repeat ((1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1) begin
         dword_index--;
         xword = xword << (`EVL_DWORD_BYTES << 3);
         xword |= evl_xword_t'(get_dword(dword_index));
      end
      return xword;
   endfunction : get_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword_valids
   //
   // This function is used to get the valid bytes at a specified doubleword offset.  The
   // information is returned as a bit-per-byte value (a doubleword byte enable).
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to read (address bits 5-3).
   //
   // Return Value:
   //
   // evl_dword_byte_en_t: This is a bit map of valid bytes at the specified doubleword offset.
   //
   function evl_dword_byte_en_t get_dword_valids(input int dword_index_in);
      int dword_index = dword_index_in & ((1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);

      return m_valids[dword_index];
   endfunction : get_dword_valids


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_xword_valids
   //
   // This function is used to get the valid bytes at a specified hexword offset.  The information
   // is returned as a bit-per-byte value (a hexword byte enable).
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within this object to read (always 0).
   //
   // Return Value:
   //
   // evl_xword_byte_en_t: This is a bit map of valid bytes at the specified hexword offset.
   //
   function evl_xword_byte_en_t get_xword_valids(input int xword_index_in);
      int                 dword_index = (xword_index_in << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) | ((1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);
      evl_xword_byte_en_t xword_valids;

      xword_valids = evl_xword_byte_en_t'(get_dword_valids(dword_index));
      repeat ((1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1) begin
         dword_index--;
         xword_valids = xword_valids << (`EVL_DWORD_BYTES);
         xword_valids |= evl_xword_byte_en_t'(get_dword_valids(dword_index));
      end
      return xword_valids;
   endfunction : get_xword_valids


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword_unknowns
   //
   // This function is used to get find the valid, unknown bytes at a specified doubleword offset.
   // The information is returned as a bit-per-byte value (a doubleword byte enable).
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to read (address bits 5-3).
   //
   // Return Value:
   //
   // evl_dword_byte_en_t: This is a bit map of valid, unknown bytes at the specified doubleword
   //                      offset.
   //
   function evl_dword_byte_en_t get_dword_unknowns(input int dword_index_in);
      int dword_index = dword_index_in & ((1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);

      return m_unknowns[dword_index] & m_valids[dword_index];
   endfunction : get_dword_unknowns


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword_modified
   //
   // This function is used to get find the valid, modified bytes at a specified doubleword offset.
   // The information is returned as a bit-per-byte value (a doubleword byte enable).
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to read (address bits 5-3).
   //
   // Return Value:
   //
   // evl_dword_byte_en_t: This is a bit map of valid, modified bytes at the specified doubleword
   //                      offset.
   //
   function evl_dword_byte_en_t get_dword_modified(input int dword_index_in);
      int dword_index = dword_index_in & ((1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);

      return m_modified[dword_index] & m_valids[dword_index];
   endfunction : get_dword_modified


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword_en
   //
   // This function is used to get the per-line quadword enables for store operations.
   //
   // Return Value:
   //
   // evl_line_qword_en_t: This is the per-line quadword enables.
   //
   function evl_line_qword_en_t get_qword_en();
      evl_line_qword_en_t qword_en;

      qword_en = evl_line_qword_en_t'(0);
      for (int ii = `EVL_LINE_DWORDS - 1; ii >= 0; ii = ii - 2) begin
         qword_en = qword_en << 1;
         if ((m_valids[ii] != evl_dword_byte_en_t'(0)) || (m_valids[ii - 1] != evl_dword_byte_en_t'(0))) begin
            qword_en[0:0] = 1'b1;
         end
      end
      return qword_en;
   endfunction : get_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_modified_qword_en
   //
   // This function is used to clear the per-line quadword enables
   //
   function void clr_modified_qword_en();
      for (int ii = `EVL_LINE_DWORDS - 1; ii >= 0; ii--) begin
         m_modified[ii] = evl_dword_byte_en_t'(0);
      end
   endfunction : clr_modified_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_modified_qword_en
   //
   // This function is used to set the per-line quadword enables
   //
   function void set_modified_qword_en();
      for (int ii = `EVL_LINE_DWORDS - 1; ii >= 0; ii = ii - 1) begin
         m_modified[ii] = ~evl_dword_byte_en_t'(0);
      end
   endfunction : set_modified_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_modified_qword_en
   //
   // This function is used to get the per-line quadword enables for store operations.
   //
   // Return Value:
   //
   // evl_line_qword_en_t: This is the per-line quadword enables.
   //
   function evl_line_qword_en_t get_modified_qword_en();
      evl_line_qword_en_t qword_en;

      qword_en = evl_line_qword_en_t'(0);
      for (int ii = `EVL_LINE_DWORDS - 1; ii >= 0; ii = ii - 2) begin
         qword_en = qword_en << 1;
         if ( ((m_valids[ii]     & m_modified[ii])     != evl_dword_byte_en_t'(0)) ||
              ((m_valids[ii - 1] & m_modified[ii - 1]) != evl_dword_byte_en_t'(0)) ) begin
            qword_en[0:0] = 1'b1;
         end
      end
      return qword_en;
   endfunction : get_modified_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: copy_modified_to_valid
   //
   // This function is used to BLAH.
   //
   function void copy_modified_to_valid();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_valids[ii] = m_valids[ii] & m_modified[ii];
      end
   endfunction : copy_modified_to_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_line
   //
   // This function is used to invalidate all bytes in this cache line.
   //
   virtual function void invalidate_line();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_valids[ii]   = evl_dword_byte_en_t'(0);
         m_unknowns[ii] = evl_dword_byte_en_t'(0);
         m_modified[ii] = evl_dword_byte_en_t'(0);
      end
   endfunction : invalidate_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_byte
   //
   // This function is used to invalidate a specific byte in this cache line.
   //
   virtual function void invalidate_byte(input int byte_index_in);
      int                 offset  = (byte_index_in >> `EVL_DWORD_ADDR_LSB) & (`EVL_LINE_DWORDS - 1);
      evl_dword_byte_en_t byte_en = evl_dword_byte_en_t'(1);

      byte_en = byte_en << (byte_index_in & (`EVL_DWORD_BYTES - 1));
      m_valids[offset]   = ~byte_en & m_valids[offset];
      m_unknowns[offset] = ~byte_en & m_unknowns[offset];
      m_modified[offset] = ~byte_en & m_modified[offset];
   endfunction : invalidate_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_hword
   //
   // This function is used to invalidate a specific halfword in this cache line.
   //
   virtual function void invalidate_hword(input int hword_index_in);
      int                 offset  = (hword_index_in >> (`EVL_DWORD_ADDR_LSB - `EVL_HWORD_ADDR_LSB)) & (`EVL_LINE_DWORDS - 1);
      evl_dword_byte_en_t byte_en = evl_dword_byte_en_t'((1 << `EVL_HWORD_BYTES) - 1);

      byte_en = byte_en << ((hword_index_in & ((`EVL_DWORD_BYTES / `EVL_HWORD_BYTES) - 1)) << `EVL_HWORD_ADDR_LSB);
      m_valids[offset]   = ~byte_en & m_valids[offset];
      m_unknowns[offset] = ~byte_en & m_unknowns[offset];
      m_modified[offset] = ~byte_en & m_modified[offset];
   endfunction : invalidate_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_word
   //
   // This function is used to invalidate a specific word in this cache line.
   //
   virtual function void invalidate_word(input int word_index_in);
      int                 offset  = (word_index_in >> (`EVL_DWORD_ADDR_LSB - `EVL_WORD_ADDR_LSB)) & (`EVL_LINE_DWORDS - 1);
      evl_dword_byte_en_t byte_en = evl_dword_byte_en_t'((1 << `EVL_WORD_BYTES) - 1);

      byte_en = byte_en << ((word_index_in & ((`EVL_DWORD_BYTES / `EVL_WORD_BYTES) - 1)) << `EVL_WORD_ADDR_LSB);
      m_valids[offset]   = ~byte_en & m_valids[offset];
      m_unknowns[offset] = ~byte_en & m_unknowns[offset];
      m_modified[offset] = ~byte_en & m_modified[offset];
   endfunction : invalidate_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_dword
   //
   // This function is used to invalidate a specific doubleword in this cache line.
   //
   virtual function void invalidate_dword(input int dword_index_in);
      int offset = dword_index_in & (`EVL_LINE_DWORDS - 1);

      m_valids[offset]   = evl_dword_byte_en_t'(0);
      m_unknowns[offset] = evl_dword_byte_en_t'(0);
      m_modified[offset] = evl_dword_byte_en_t'(0);
   endfunction : invalidate_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_qword
   //
   // This function is used to invalidate a specific quadword in this cache line.
   //
   virtual function void invalidate_qword(input int qword_index_in);
      int offset = qword_index_in << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB);

      repeat (1 << (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         m_valids[offset]   = evl_dword_byte_en_t'(0);
         m_unknowns[offset] = evl_dword_byte_en_t'(0);
         m_modified[offset] = evl_dword_byte_en_t'(0);
         offset++;
      end
   endfunction : invalidate_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_oword
   //
   // This function is used to invalidate a specific octalword in this cache line.
   //
   virtual function void invalidate_oword(input int oword_index_in);
      int offset = oword_index_in << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB);

      repeat (1 << (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         m_valids[offset]   = evl_dword_byte_en_t'(0);
         m_unknowns[offset] = evl_dword_byte_en_t'(0);
         m_modified[offset] = evl_dword_byte_en_t'(0);
         offset++;
      end
   endfunction : invalidate_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_xword
   //
   // This function is used to invalidate a specific octalword in this cache line.
   //
   virtual function void invalidate_xword(input int xword_index_in);
      int offset = xword_index_in << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB);

      repeat (1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         m_valids[offset]   = evl_dword_byte_en_t'(0);
         m_unknowns[offset] = evl_dword_byte_en_t'(0);
         m_modified[offset] = evl_dword_byte_en_t'(0);
         offset++;
      end
   endfunction : invalidate_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: validate_on_size_and_offset
   //
   // This function is used to validate data based on the size and offset of a request.
   //
   virtual function void validate_on_size_and_offset(input evl_paddr_t addr,
                                                     input int         size_msb);
      int                 offset;
      evl_xword_byte_en_t mask;

      offset = ((`EVL_XWORD_BYTES - 1) & int'(addr)) >> size_msb;
      offset = offset << size_msb;
      mask   = ((evl_xword_byte_en_t'(1) << (1 << size_msb)) - 1) << offset;

      offset = 0;
      invalidate_line();
      repeat (1 << (`EVL_XWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) begin
         m_valids[offset] = evl_dword_byte_en_t'(mask);
         mask = mask >> `EVL_DWORD_BYTES;
         offset++;
      end
   endfunction : validate_on_size_and_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: make_line_unknown
   //
   // This function is used to mark all bytes in this cache line as unknown.
   //
   virtual function void make_line_unknown();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_unknowns[ii] = ~evl_dword_byte_en_t'(0);
      end
   endfunction : make_line_unknown


   //-----------------------------------------------------------------------------------------------
   //
   // Function: validate_line
   //
   // This function is used to validate all bytes in this cache line.
   // If the given random generator is non null, the previously invalid data will be randomised
   //
   virtual function void validate_line(input evl_rand_gen rand_gen = null);
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         if ((rand_gen != null) && (m_valids[ii] == evl_dword_byte_en_t'(0))) begin
            m_dwords[ii] = rand_gen.get_dword();
         end
         m_valids[ii] = ~evl_dword_byte_en_t'(0);
      end
      post_process_data();
   endfunction : validate_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_and_validate_line
   //
   // This function is used to zero all bytes in this cache line and then mark them as valid.
   //
   virtual function void zero_and_validate_line();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_dwords[ii]   = evl_dword_t'(0);
         m_valids[ii]   = ~evl_dword_byte_en_t'(0);
         m_unknowns[ii] = evl_dword_byte_en_t'(0);
         m_modified[ii] = evl_dword_byte_en_t'(0);
      end
      post_process_data();
   endfunction : zero_and_validate_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_bytes_are_valid
   //
   // Blah
   //
   virtual function bit all_bytes_are_valid();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         if (m_valids[ii] != ~evl_dword_byte_en_t'(0)) begin
            return 1'b0;
         end
      end
      return 1'b1;
   endfunction : all_bytes_are_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_bytes_are_modified
   //
   // Blah
   //
   virtual function bit all_bytes_are_modified();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         if ((m_valids[ii] & m_modified[ii]) != ~evl_dword_byte_en_t'(0)) begin
            return 1'b0;
         end
      end
      return 1'b1;
   endfunction : all_bytes_are_modified


   //-----------------------------------------------------------------------------------------------
   //
   // Function: data_is_all_zero
   //
   // Blah
   //
   virtual function bit data_is_all_zero();
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         if ((m_valids[ii] != ~evl_dword_byte_en_t'(0)) || (m_unknowns[ii] != evl_dword_byte_en_t'(0)) || (m_dwords[ii] != evl_dword_t'(0))) begin
            return 1'b0;
         end
      end
      return 1'b1;
   endfunction : data_is_all_zero


   //-----------------------------------------------------------------------------------------------
   //
   // Function: copy_data_from_line
   //
   // This function is used to copy data from a source line into this cache line.  The two cache
   // lines need not be of the same type, but both must be derived from evl_line_data.
   //
   // Input Variables:
   //
   // source_line: This is the source cache line from which to copy data.
   //
   virtual function void copy_data_from_line(input evl_line_data source_line, input bit pass_modified_bytes = 1'b0);
      if (source_line == null) begin
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            m_dwords[ii]   = evl_dword_t'(0);
            m_valids[ii]   = evl_dword_byte_en_t'(0);
            m_unknowns[ii] = evl_dword_byte_en_t'(0);
            m_modified[ii] = evl_dword_byte_en_t'(0);
         end
      end
      else begin
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            m_dwords[ii]   = source_line.get_dword(ii);
            m_valids[ii]   = source_line.get_dword_valids(ii);
            m_unknowns[ii] = source_line.get_dword_unknowns(ii);
            if (pass_modified_bytes == 1'b1) begin
               m_modified[ii] = source_line.get_dword_modified(ii);
            end
            else begin
               m_modified[ii] = evl_dword_byte_en_t'(0);
            end
         end
      end
      post_process_data();
   endfunction : copy_data_from_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: zero_all_data
   //
   // This function is used to zero all of the data in this object.
   //
   virtual function void zero_all_data();
      invalidate_line();
      set_xword(0, evl_xword_t'(0));
      post_process_data();
   endfunction : zero_all_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: merge_modified_byte_enables
   //
   // This function is used to merge data from a source line into this cache line.  The two cache
   // lines need not be of the same type, but both must be derived from evl_line_data.
   //
   // Input Variables:
   //
   // source_line: This is the source cache line from which to merge data.
   //
   virtual function void merge_modified_byte_enables(input int dword_index_in, input evl_dword_byte_en_t byte_en_in);
      int dword_index = dword_index_in & ((1 << (`EVL_LINE_ADDR_LSB - `EVL_DWORD_ADDR_LSB)) - 1);

      m_modified[dword_index] |= byte_en_in;
   endfunction : merge_modified_byte_enables


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
   virtual function void merge_data_from_line(input evl_line_data source_line, input bit mark_modified_bytes = 1'b0);
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         if (source_line.get_dword_valids(ii) != evl_dword_byte_en_t'(0)) begin
            if (mark_modified_bytes == 1'b1) begin
               set_bytes_in_dword(ii, source_line.get_dword(ii), source_line.get_dword_valids(ii), source_line.get_dword_unknowns(ii), source_line.get_dword_valids(ii));
            end
            else begin
               set_bytes_in_dword(ii, source_line.get_dword(ii), source_line.get_dword_valids(ii), source_line.get_dword_unknowns(ii));
            end
         end
      end
      post_process_data();
   endfunction : merge_data_from_line


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clone_line_data
   //
   // This function is used to make a copy of this cache line and return a handle to the copy.
   //
   // Return Value:
   //
   // evl_line_data: This is a handle to the cloned object.
   //
   virtual function evl_line_data clone_line_data();
      evl_line_data line_data;

      $cast(line_data, this.clone());
      return line_data;
   endfunction : clone_line_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_invalid_and_unknown
   //
   // This function is used to format doubleword data appropriately.  Invalid bytes are indicated
   // with a "--" string, and valid but unkown bytes are indicated with a "??" string..
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to format (address bits 5-3).
   //
   // Return Value:
   //
   // string: This function returns a string specifying the contents of the specified doubleword.
   //
   virtual function string format_invalid_and_unknown(input evl_dword_t         dword_in,
                                                      input evl_dword_byte_en_t valids_in   = ~evl_dword_byte_en_t'(0),
                                                      input evl_dword_byte_en_t unknowns_in = evl_dword_byte_en_t'(0));
      evl_dword_t         dword;
      evl_dword_byte_en_t valids;
      evl_dword_byte_en_t unknowns;

      dword    = dword_in;
      valids   = valids_in;
      unknowns = unknowns_in;
      if ((valids != ~evl_dword_byte_en_t'(0)) || ((valids & unknowns) != evl_dword_byte_en_t'(0))) begin
         string message;

         message  = "";
         for (int ii = 0; ii < `EVL_DWORD_BYTES; ii++) begin
            if (valids[0:0] == 1'b0) begin
               message = $sformatf("--%0s", message);
            end
            else if (unknowns[0:0] == 1'b1) begin
               message = $sformatf("??%0s", message);
            end
            else begin
               message = $sformatf("%02x%0s", dword[7:0], message);
            end
            dword    = dword >> 8;
            valids   = valids >> 1;
            unknowns = unknowns >> 1;
         end
         return message;
      end
      return $sformatf("%x", dword);
   endfunction : format_invalid_and_unknown


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_invalid_and_unknown_by_index
   //
   // This function is used to format doubleword data appropriately.  Invalid bytes are indicated
   // with a "--" string, and valid but unkown bytes are indicated with a "??" string..
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within this object to format (address bits 5-3).
   //
   // Return Value:
   //
   // string: This function returns a string specifying the contents of the specified doubleword.
   //
   virtual function string format_invalid_and_unknown_by_index(input int dword_index_in, input evl_dword_byte_en_t byte_ens = ~evl_dword_byte_en_t'(0));
      int index = dword_index_in & (`EVL_LINE_DWORDS - 1);

      return format_invalid_and_unknown(m_dwords[index], m_valids[index] & byte_ens, m_unknowns[index]);
   endfunction : format_invalid_and_unknown_by_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_dwords
   //
   // This function is used to compare doublewords.
   //
   // Input Variables:
   //
   // byte_en_in:  This input specifies the doubleword bytes to compare.
   // obs_dword:   This input is the observed doubleword.
   // exp_dword:   This input is the expected doubleword.
   // status_only: This input should be set to 1'b1 if no error message should be generated.
   //
   // Output Variables:
   //
   // err_message: This string is an error message should a mismatch be detected.  It will only be
   //              set if the status_only input is 1'b0.
   //
   // Return Value:
   //
   // bit: A 1'b1 is returned if a mismatch is detected.
   //
   virtual function bit compare_dwords(output string              err_message,
                                       input  evl_dword_byte_en_t byte_en_in,
                                       input  evl_dword_t         obs_dword,
                                       input  evl_dword_t         exp_dword,
                                       input  evl_dword_byte_en_t exp_unknowns = evl_dword_byte_en_t'(0),
                                       input  bit                 status_only = 1'b0);
      evl_dword_t dword_mask;

      if (byte_en_in == evl_dword_byte_en_t'(0)) begin
         return "";
      end
      if (byte_en_in == ~evl_dword_byte_en_t'(0)) begin
         dword_mask = ~evl_dword_t'(0);
      end
      else if (byte_en_in == ((~evl_dword_byte_en_t'(0)) >> `EVL_WORD_BYTES)) begin
         dword_mask = m_dword_mask_byte[`EVL_DWORD_BYTES];
      end
      else if (byte_en_in == ((~evl_dword_byte_en_t'(0)) << `EVL_WORD_BYTES)) begin
         dword_mask = m_dword_mask_byte[`EVL_DWORD_BYTES+1];
      end
      else begin
         int                 ii;
         evl_dword_byte_en_t byte_en;

         ii         = 0;
         byte_en    = byte_en_in;
         dword_mask = evl_dword_t'(0);
         while (byte_en != evl_dword_byte_en_t'(0)) begin
            if (byte_en[0:0] == 1'b1) begin
               dword_mask |= m_dword_mask_byte[ii];
            end
            byte_en = byte_en >> 1;
            ii++;
         end
      end
      if (exp_unknowns == ~evl_dword_byte_en_t'(0)) begin
         dword_mask = evl_dword_t'(0);
      end
      else if (exp_unknowns == ((~evl_dword_byte_en_t'(0)) >> `EVL_WORD_BYTES)) begin
         dword_mask &= ~m_dword_mask_byte[`EVL_DWORD_BYTES];
      end
      else if (exp_unknowns == ((~evl_dword_byte_en_t'(0)) << `EVL_WORD_BYTES)) begin
         dword_mask &= ~m_dword_mask_byte[`EVL_DWORD_BYTES+1];
      end
      else begin
         int                 ii;
         evl_dword_byte_en_t unknowns;

         ii       = 0;
         unknowns = exp_unknowns;
         while (unknowns != evl_dword_byte_en_t'(0)) begin
            if (unknowns[0:0] == 1'b1) begin
               dword_mask &= ~m_dword_mask_byte[ii];
            end
            unknowns = unknowns >> 1;
            ii++;
         end
      end
      if (((obs_dword ^ exp_dword) & dword_mask) != evl_dword_t'(0)) begin
         if (status_only == 1'b0) begin
            err_message = $sformatf("expected 0x%0s, observed 0x%0s", format_invalid_and_unknown(exp_dword, byte_en_in), format_invalid_and_unknown(obs_dword, byte_en_in));
         end
         return 1'b1;
      end
      return 1'b0;
   endfunction : compare_dwords


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_xword_against_my_data
   //
   // This function is used to compare an incoming hex word against the data in this object.
   //
   // Input Variables:
   //
   // byte_en_in: This input specifies the hex word bytes to compare.
   // xword_in:   This input is the hex word against which to compare.
   //
   // Return Value:
   //
   // bit: A 1'b1 is returned if a mismatch is detected.
   //
   virtual function bit compare_xword_against_my_data(input evl_xword_byte_en_t byte_en_in,
                                                      input evl_xword_t         xword_in);
      string              err_message;
      evl_xword_byte_en_t xword_byte_en;
      evl_dword_byte_en_t dword_byte_en;
      evl_xword_t         xword;
      evl_dword_t         dword;

      xword_byte_en = byte_en_in;
      xword         = xword_in;
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         dword_byte_en = evl_dword_byte_en_t'(xword_byte_en);
         xword_byte_en = xword_byte_en >> `EVL_DWORD_BYTES;
         dword         = evl_dword_t'(xword);
         xword         = xword >> (`EVL_DWORD_BYTES << 3);
         if ((dword_byte_en & m_valids[ii]) != dword_byte_en) begin
            return 1'b1;
         end
         if (compare_dwords(err_message, dword_byte_en, m_dwords[ii], dword, evl_dword_byte_en_t'(0), 1'b1) == 1'b1) begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction : compare_xword_against_my_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_data
   //
   // This function is used to compare data in this data object to the expected data.
   //
   // Input Variables:
   //
   // exp_data:         This is the data against which to compare this object
   //
   // Input Variables:
   //
   // err_messages_out: This array will hold error messages in case there are mismatches
   //
   virtual function bit compare_data(input  evl_line_data exp_data,
                                     output string        err_messages_out[]);
      string err_messages[$];
      bit    have_mismatch;

      have_mismatch = 1'b0;
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         string err_message;

         if (compare_dwords(err_message, exp_data.get_dword_valids(ii), m_dwords[ii], exp_data.get_dword(ii), exp_data.get_dword_unknowns(ii)) == 1'b1) begin
            have_mismatch = 1'b1;
            err_messages.push_back($sformatf("data mismatch for dword %0d ... %0s", ii, err_message));
         end
      end
      if (have_mismatch == 1'b0) begin
         err_messages_out = {};
      end
      else begin
         err_messages_out = new[err_messages.size()];

         foreach (err_messages[ii]) begin
            err_messages_out[ii] = err_messages[ii];
         end
      end
      return have_mismatch;
   endfunction : compare_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_data_and_byte_ens
   //
   // This function is used to compare data in this data object to the expected data.
   //
   // Input Variables:
   //
   // abstract_name_in: This is the abstract name to use when printing.  If it is "", then the
   //                   abstract name in this object will be used.
   // paddr:            This is the physical address for this object.  If it is ~evl_paddr_t'(0), no
   //                   address information will be printed.
   //
   virtual function bit compare_data_and_byte_ens(input  evl_line_data exp_data,
                                                  output string        err_messages_out[]);
      string err_messages[$];
      bit    have_mismatch;

      have_mismatch = 1'b0;
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         if (exp_data.get_dword_valids(ii) != m_valids[ii]) begin
            have_mismatch = 1'b1;
            err_messages.push_back($sformatf("byte/write enable mismatch for dword %0d (0x%x, not 0x%x)", ii, m_valids[ii], exp_data.get_dword_valids(ii)));
         end
      end
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         string err_message;

         if (compare_dwords(err_message, exp_data.get_dword_valids(ii), m_dwords[ii], exp_data.get_dword(ii), exp_data.get_dword_unknowns(ii)) == 1'b1) begin
            have_mismatch = 1'b1;
            err_messages.push_back($sformatf("data mismatch for dword %0d ... %0s", ii, err_message));
         end
      end
      if (have_mismatch == 1'b0) begin
         err_messages_out = {};
      end
      else begin
         err_messages_out = new[err_messages.size()];

         foreach (err_messages[ii]) begin
            err_messages_out[ii] = err_messages[ii];
         end
      end
      return have_mismatch;
   endfunction : compare_data_and_byte_ens


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_normalized_data
   //
   // This function is used to compare data in this data object to the expected data.
   //
   // Input Variables:
   //
   // abstract_name_in: This is the abstract name to use when printing.  If it is "", then the
   //                   abstract name in this object will be used.
   // paddr:            This is the physical address for this object.  If it is ~evl_paddr_t'(0), no
   //                   address information will be printed.
   //
   virtual function evl_line_data get_normalized_data(input evl_paddr_t addr_in, input int length_in);
      int           offset = int'((addr_in & (`EVL_LINE_BYTES - 1)) >> `EVL_DWORD_ADDR_LSB);
      evl_line_data new_data;

      new_data = clone_line_data();
      if (length_in > `EVL_OWORD_BYTES) begin
         return new_data;
      end

      new_data.invalidate_line();
      if (length_in > `EVL_QWORD_BYTES) begin
         for (int ii = 0; ii < (`EVL_OWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB); ii++) begin
            new_data.set_bytes_in_dword(ii, m_dwords[offset], m_valids[offset], m_unknowns[offset], m_modified[offset]);
            offset++;
         end
      end
      else if (length_in > `EVL_DWORD_BYTES) begin
         for (int ii = 0; ii < (`EVL_QWORD_ADDR_LSB - `EVL_DWORD_ADDR_LSB); ii++) begin
            new_data.set_bytes_in_dword(ii, m_dwords[offset], m_valids[offset], m_unknowns[offset], m_modified[offset]);
            offset++;
         end
      end
      else if (length_in > `EVL_WORD_BYTES) begin
         new_data.set_bytes_in_dword(0, m_dwords[offset], m_valids[offset], m_unknowns[offset], m_modified[offset]);
      end
      else if (length_in > `EVL_HWORD_BYTES) begin
         int shift_val = int'(addr_in & (`EVL_DWORD_BYTES - 1) & const_word_paddr_mask);

         new_data.set_bytes_in_dword(0, ((m_dwords[offset]   >> (shift_val << 3)) & `EVL_WORD_MASK),
                                        ((m_valids[offset]   >> shift_val) & ((1 << `EVL_WORD_BYTES) - 1)),
                                        ((m_unknowns[offset] >> shift_val) & ((1 << `EVL_WORD_BYTES) - 1)),
                                        ((m_modified[offset] >> shift_val) & ((1 << `EVL_WORD_BYTES) - 1)));
      end
      else if (length_in > 1) begin
         int shift_val = int'(addr_in & (`EVL_DWORD_BYTES - 1) & const_hword_paddr_mask);

         new_data.set_bytes_in_dword(0, ((m_dwords[offset]   >> (shift_val << 3)) & `EVL_HWORD_MASK),
                                        ((m_valids[offset]   >> shift_val) & ((1 << `EVL_HWORD_BYTES) - 1)),
                                        ((m_unknowns[offset] >> shift_val) & ((1 << `EVL_HWORD_BYTES) - 1)),
                                        ((m_modified[offset] >> shift_val) & ((1 << `EVL_HWORD_BYTES) - 1)));
      end
      else begin
         int shift_val = int'(addr_in & (`EVL_DWORD_BYTES - 1));

         new_data.set_bytes_in_dword(0, ((m_dwords[offset]   >> (shift_val << 3)) & evl_dword_t'(8'hff)),
                                        ((m_valids[offset]   >> shift_val) & 1),
                                        ((m_unknowns[offset] >> shift_val) & 1),
                                        ((m_modified[offset] >> shift_val) & 1));
      end
      post_process_data();
      return new_data;
   endfunction : get_normalized_data


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
   // paddr:            This is the physical address for this object.  If it is ~evl_paddr_t'(0), no
   //                   address information will be printed.
   //
   virtual function void print_obj(input string      abstract_name_in = "",
                                   input evl_paddr_t paddr            = ~evl_paddr_t'(0));
      if (paddr == ~evl_paddr_t'(0)) begin
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            `evl_log(UVM_NONE, VMOD_GLOBAL, abstract_name_in, $sformatf("m_dwords[%0d]: 0x%0s", ii, format_invalid_and_unknown(m_dwords[ii], m_valids[ii], m_unknowns[ii])))
         end
      end
      else begin
         for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
            `evl_log_addr(UVM_NONE, VMOD_GLOBAL, abstract_name_in, paddr, $sformatf("m_dwords[%0d]: 0x%0s", ii, format_invalid_and_unknown(m_dwords[ii], m_valids[ii], m_unknowns[ii])))
         end
      end
   endfunction : print_obj


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
      evl_line_data source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      set_abstract_name(source_obj.get_abstract_name());
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_dwords[ii]   = source_obj.m_dwords[ii];
         m_valids[ii]   = source_obj.m_valids[ii];
         m_unknowns[ii] = source_obj.m_unknowns[ii];
         m_modified[ii] = source_obj.m_modified[ii];
      end
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_line_data");
      super.new(name_in);
      if (m_setup_done == 1'b0) begin
         m_setup_done = 1'b1;
         m_dword_mask_byte[0] = evl_dword_t'(255);
         for (int ii = 1; ii < `EVL_DWORD_BYTES; ii++) begin
            m_dword_mask_byte[ii] = m_dword_mask_byte[ii-1] << 8;
         end
         m_dword_mask_byte[`EVL_DWORD_BYTES]   = (~evl_dword_t'(0)) >> (`EVL_DWORD_BYTES << 2);
         m_dword_mask_byte[`EVL_DWORD_BYTES+1] = ~m_dword_mask_byte[`EVL_DWORD_BYTES];
      end
      for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
         m_dwords[ii]   = evl_dword_t'(0);
         m_valids[ii]   = evl_dword_byte_en_t'(0);
         m_unknowns[ii] = evl_dword_byte_en_t'(0);
         m_modified[ii] = evl_dword_byte_en_t'(0);
      end
   endfunction : new

endclass : evl_line_data
