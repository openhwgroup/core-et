//--------------------------------------------------------------------------------------------------
//
// Class: evl_req_desc_callback
//
// This callback class is used to handle request and response data assignment for cascading data
// from one request descriptor to another.  With the addition of multi-burst support in AXI, it is
// possible for evl_line_data chunk assignment (in m_req_data[] or m_rsp_data[]) to occur some time
// after a scoreboard handles a request (this is true of AXI writes in any case).  To allow a
// scoreboard to cascade a request/response from one descriptor to another and then move on without
// forking a subprocess, it is necessary to provide this callback mechanism.
//
// The working assumption is that there can be 1 callback instance (and only 1) for each
// descriptor-to-descriptor full data transfer.  In other words, if descriptor A is supposed to
// cascade request data to descriptor B, there should be at most 1 callback instance (which is
// intended to handle the entire descA.m_req_data[] to descB.m_req_data[] transfer).  If there
// are multiple calls to the *request descriptor's* cascade_rxx_data_to_dst() function from a
// scoreboard, there may be multiple callback instances created (it currently seems unnecessary to
// perform more than one *request descriptor* cascade_rxx_data_to_dst() call from a scoreboard for
// transferring data from one descriptor to another).
//
class evl_req_desc_callback extends evl_void;
   evl_req_desc m_src_req_desc;
   evl_req_desc m_dst_req_desc;
   int          m_line_idx;
   bit          m_use_size;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cascade_req_data_to_dst
   //
   function bit cascade_req_data_to_dst();
      `evl_log_mdesc(UVM_MEDIUM, VMOD_REQ_DESC, m_src_req_desc.get_abstract_name(), m_src_req_desc, $sformatf("cascade_req_data_to_dst callback (%0s -> %0s)", m_src_req_desc.sprint_obj(), m_dst_req_desc.sprint_obj()))
      return m_src_req_desc.cascade_req_data_to_dst(m_dst_req_desc, 1'b1);
   endfunction : cascade_req_data_to_dst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cascade_rsp_data_to_dst
   //
   function bit cascade_rsp_data_to_dst();
      `evl_log_mdesc(UVM_MEDIUM, VMOD_REQ_DESC, m_src_req_desc.get_abstract_name(), m_src_req_desc, $sformatf("cascade_rsp_data_to_dst callback (%0s -> %0s)", m_src_req_desc.sprint_obj(), m_dst_req_desc.sprint_obj()))
      return m_src_req_desc.cascade_rsp_data_to_dst(m_dst_req_desc, m_line_idx, m_use_size, 1'b1);
   endfunction : cascade_rsp_data_to_dst


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input evl_req_desc src_req_desc,
                input evl_req_desc dst_req_desc,
                input int          line_idx = -1,
                input bit          use_size = 1'b0);
      m_src_req_desc = src_req_desc;
      m_dst_req_desc = dst_req_desc;
      m_line_idx     = line_idx;
      m_use_size     = use_size;
   endfunction : new
endclass : evl_req_desc_callback


//--------------------------------------------------------------------------------------------------
//
// Class: evl_req_desc
//
class evl_req_desc extends evl_mem_desc;

   `uvm_object_utils(evl_req_desc)

   //-----------------------------------------------------------------------------------------------
   //
   // Local Type Definitions
   //
   // The request structure defined below is used to simplify copying information when spawning one
   // descriptor from another.  It contains the following fields:
   //
   // bus_type:       This is the type of bus on which this request descriptor is running.  As an
   //                 example, the bus type might be ET Link (EVL_BUS_TYPE_ETLINK) or AXI
   //                 (EVL_BUS_TYPE_AXI).
   //                 ------------------------------------------------------------------------------
   // bus_cmd:        This is the bus command encoding to use for a bus request.  A value of all 1s
   //                 indicates an invalid or idle command.
   //                 ------------------------------------------------------------------------------
   // bus_bank:       This is the shire cache bank number for the bus operation.
   //                 ------------------------------------------------------------------------------
   // bus_req_id:     This is the request ID for the bus operation.
   //                 ------------------------------------------------------------------------------
   // bus_req_size:   This is an encoded value of the size (in bytes) of the bus operation.  A
   //                 single-byte request has a size of 0, a two-byte request has a size of 1, a
   //                 four-byte request has a size of 2, an eight-byte request has a size of 3, etc.
   //                 ET Link write operations use this field as a 4-bit quadword enable mask.
   //                 ------------------------------------------------------------------------------
   // bus_req_len:    This is the number of data bursts - 1.
   //                 ------------------------------------------------------------------------------
   // bus_qword_en:   This is the quadword enable field for ET link requests.
   //                 ------------------------------------------------------------------------------
   // bus_lock:       This is the bus lock signal available on the AXI bus.
   //                 ------------------------------------------------------------------------------
   // bus_cache_attr: This is the cache attribute for the bus operation.  ET link uses a single bit
   //                 (cacheable or not), while AXI has a more complex encoding.
   //                 ------------------------------------------------------------------------------
   // bus_prot:       This is the PROT field on the AXI bus.
   //                 ------------------------------------------------------------------------------
   // bus_region:     This is the REGION field on the AXI bus.
   //                 ------------------------------------------------------------------------------
   // bus_req_user:   This is the user-defined field for requests on the AXI bus.
   //                 ------------------------------------------------------------------------------
   // bus_req_param:  This is the request param (used for TileLink).
   //
   // The response structure defined below is used to simplify copying information when transferring
   // responses from one descriptor to another.  It contains the following fields:
   //
   // rsp_valid:      This bit, when set, indicates the response is valid.
   //                 ------------------------------------------------------------------------------
   // bus_rsp_type:   This is the response type.  The actual enconding of bits differs depending on
   //                 the bus type on which this bus request was made.
   //                 ------------------------------------------------------------------------------
   // bus_rsp_id:     This the response ID (i.e., bus tag) for the response.  Presumably, this
   //                 field should always match bus_req_id, but the message case may cause a
   //                 difference.
   //                 ------------------------------------------------------------------------------
   // bus_rsp_size:   This is an encoded value of the size (in bytes) of the response.  A single
   //                 byte response has a size of 0, a two-byte response has a size of 1, a four
   //                 byte response has a size of 2, an eight-byte response has a size of 3, etc.
   //                 ------------------------------------------------------------------------------
   // bus_qword_en:   This is the quadword enable field for ET link responses.
   //                 ------------------------------------------------------------------------------
   // bus_rsp_dest:   This is the destination for the original bus operation. Presumably, this
   //                 should match bus_req_source (except in ET link messages).
   //                 ------------------------------------------------------------------------------
   // bus_rsp_user:   This is the user-defined field for responses on the AXI bus.
   //                 ------------------------------------------------------------------------------
   // bus_rsp_param:  This is the response param (used for TileLink).
   //
   typedef struct packed unsigned {
         evl_bus_type_t       bus_type;
         evl_bus_cmd_t        bus_cmd;
         evl_sc_bank_id_t     bus_bank;
         evl_bus_req_id_t     bus_req_id;
         evl_bus_req_size_t   bus_req_size;
         evl_bus_req_len_t    bus_req_len;
         evl_line_qword_en_t  bus_qword_en;
         evl_bus_lock_t       bus_lock;
         evl_bus_cache_attr_t bus_cache_attr;
         evl_bus_prot_t       bus_prot;
         evl_bus_region_t     bus_region;
         evl_bus_req_user_t   bus_req_user;
         evl_paddr_t          bus_line_idx_mask;
         evl_bus_subopcode_t  bus_req_param;
      } evl_req_desc_req_s;

   typedef struct packed unsigned {
         bit                  rsp_valid;
         evl_bus_rsp_type_t   bus_rsp_type;
         evl_bus_rsp_id_t     bus_rsp_id;
         evl_bus_rsp_size_t   bus_rsp_size;
         evl_line_qword_en_t  bus_qword_en;
         evl_bus_rsp_dest_t   bus_rsp_dest;
         evl_bus_rsp_user_t   bus_rsp_user;
         evl_bus_subopcode_t  bus_rsp_param;
      } evl_req_desc_rsp_s;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_bus_req_attrs: This packed array contains commonly-copied variables when copying request
   //                  information from one request descriptor to another.
   //                  -----------------------------------------------------------------------------
   // m_bus_rsp_attrs: This queue of packed arrays contains commonly-copied variables when copying
   //                  response information from one request descriptor to another.
   //                  -----------------------------------------------------------------------------
   // m_observed:      This bit is set to 1'b1 if the request descriptor has been observed by a
   //                  downstream object (such as a slave agent or slave scoreboard).
   //                  -----------------------------------------------------------------------------
   // m_bus_parser:    This is a handle to a bus-type-specific parser.  Bus parsers are used to
   //                  decode command and response type information as well as provide string
   //                  printing functions for bus requests and responses.  By using bus parsers, the
   //                  request descriptor class can be bus-agnostic.
   //
   evl_req_desc_req_s m_bus_req_attrs    = '{ bus_type:          EVL_BUS_TYPE_UNKNOWN,
                                              bus_cmd:           `EVL_Bus_Idle,
                                              bus_bank:          ~evl_sc_bank_id_t'(0),
                                              bus_req_id:        evl_bus_req_id_t'(0),
                                              bus_req_size:      evl_bus_req_size_t'(0),
                                              bus_req_len:       evl_bus_req_len_t'(0),
                                              bus_qword_en:      evl_line_qword_en_t'(0),
                                              bus_lock:          evl_bus_lock_t'(0),
                                              bus_cache_attr:    evl_bus_cache_attr_t'(0),
                                              bus_prot:          evl_bus_prot_t'(0),
                                              bus_region:        evl_bus_region_t'(0),
                                              bus_req_user:      evl_bus_req_user_t'(0),
                                              bus_line_idx_mask: evl_paddr_t'(0),
                                              bus_req_param:     evl_bus_subopcode_t'(0)
                                            };
   evl_req_desc_rsp_s m_bus_rsp_attrs[$];
   bit                m_observed          = 1'b0;
   bit                m_correlated        = 1'b0;
   bit                m_do_parent_check   = 1'b0;
   bit                m_ignore_st_on_rsp  = 1'b0;
   bit                m_is_original_req   = 1'b0;
   bit                m_have_all_data     = 1'b0;
   bit                m_x_last            = 1'b0;
   evl_bus_parser     m_bus_parser        = null;
   int                m_cur_burst         = 0;
   evl_req_desc       m_bus_req_desc      = null;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects for Data
   //
   // The following variables are used for supporting write request data and read response data.
   // Access to these variables must be handled carefully (i.e., only with function and task calls),
   // so they are protected.
   //
   // m_req_data_avail:       This variable is the count of line data objects currently available
   //                         for write request data.
   //                         ----------------------------------------------------------------------
   // m_rsp_data_avail:       This variable is the count of line data objects currently available
   //                         for read response data.
   //                         ----------------------------------------------------------------------
   // m_req_data_event:       This variable is incremented whenever an m_req_data[] object is
   //                         changed; it is used by the wait_for_req_data_by_index() task.
   //                         ----------------------------------------------------------------------
   // m_rsp_data_event:       This variable is incremented whenever an m_rsp_data[] object is
   //                         changed; it is used by the wait_for_rsp_data_by_index() task.
   //                         ----------------------------------------------------------------------
   // m_req_data[]:           This is a queue of line data objects that contain request write data.
   //                         All data is stored in cache line order, and each queue object has room
   //                         for a full cache line of data, regardless of how much write data is
   //                         required.
   //                         ----------------------------------------------------------------------
   // m_rsp_data[]:           This is a queue of line data objects that contain response read data.
   //                         All data is stored in cache line order, and each queue object has room
   //                         for a full cache line of data, regardless of how much read data is
   //                         provided.
   //                         ----------------------------------------------------------------------
   // m_req_data_callbacks[]: This is a queue of request data callbacks to perform when handling
   //                         delayed request data assignment during request data cascading.
   //                         ----------------------------------------------------------------------
   // m_rsp_data_callbacks[]: This is a queue of response data callbacks to perform when handling
   //                         delayed response data assignment during response data cascading.
   //
   protected byte                  m_req_data_avail = 0;
   protected byte                  m_rsp_data_avail = 0;
   protected byte                  m_req_data_event = 0;
   protected byte                  m_rsp_data_event = 0;
   protected evl_line_data         m_req_data[$];
   protected evl_line_data         m_rsp_data[$];
   protected evl_req_desc_callback m_req_data_callbacks[$];
   protected evl_req_desc_callback m_rsp_data_callbacks[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_issued
   //
   // This function is used to mark the descriptor as issued.
   //
   virtual function void set_issued();
      if ((m_root_desc != null) && (m_root_desc.get_desc_type() == EVL_DESC_TRANS)) begin
         m_root_desc.set_issued();
      end
      super.set_issued();
   endfunction : set_issued


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ignore_st_on_rsp
   //
   // Blah.
   //
   function void set_ignore_st_on_rsp();
      m_ignore_st_on_rsp = 1'b1;
   endfunction : set_ignore_st_on_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: ignore_st_on_rsp
   //
   // Blah.
   //
   function bit ignore_st_on_rsp();
      return m_ignore_st_on_rsp;
   endfunction : ignore_st_on_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_as_original_req
   //
   // Blah.
   //
   function void set_as_original_req();
      m_is_original_req = 1'b1;
   endfunction : set_as_original_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_original_req
   //
   // Blah.
   //
   function bit is_original_req();
      return m_is_original_req;
   endfunction : is_original_req


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_cmd
   //
   // This function is used to set the bus command for this descriptor.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for the bus operation.  The actual interpretation
   //             of this field is bus-type dependent.
   //
   function void set_bus_cmd(input evl_bus_cmd_t bus_cmd_in);
      m_bus_req_attrs.bus_cmd = bus_cmd_in;
   endfunction : set_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_bus_cmd
   //
   // This function is used to set the bus command for this descriptor.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for the bus operation.  The actual interpretation
   //             of this field is bus-type dependent.
   //
   function void set_abstract_bus_cmd(input evl_abstract_bus_cmd_t bus_cmd_in);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      set_bus_cmd(m_bus_parser.convert_bus_cmd(bus_cmd_in, m_bus_req_attrs.bus_type));
   endfunction : set_abstract_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_bus_cmd
   //
   // This function is used to invalidate the current bus command.  It is likely only used in a
   // master stimulus agent.
   //
   function void clr_bus_cmd();
      m_bus_req_attrs.bus_cmd = ~evl_bus_cmd_t'(0);
   endfunction : clr_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd
   //
   // This function is used to get the bus command for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_cmd_t: This function returns the bus command for the bus operation.  The actual
   //                interpretation of the command is bus-type dependent.
   //
   function evl_bus_cmd_t get_bus_cmd();
      return m_bus_req_attrs.bus_cmd;
   endfunction : get_bus_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_bank
   //
   // This function is used to set the shire cache bank number for this descriptor.
   //
   // Input Variables:
   //
   // bus_bank_in: This is the value to which to set the bus bank.
   //
   function void set_bus_bank(input evl_sc_bank_id_t bus_bank_in);
      m_bus_req_attrs.bus_bank = bus_bank_in;
   endfunction : set_bus_bank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_bank
   //
   // This function is used to get the shire cache bank of this descriptor.
   //
   // Return Value:
   //
   // evl_sc_bank_id_t: This function returns the value of the shire cache bank.
   //
   function evl_sc_bank_id_t get_bus_bank();
      return m_bus_req_attrs.bus_bank;
   endfunction : get_bus_bank


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_id
   //
   // This function is used to set the bus request ID for this descriptor (i.e., the bus tag).
   //
   // Input Variables:
   //
   // bus_req_id_in: This is the value to which to set the bus request ID.
   //
   function void set_bus_req_id(input evl_bus_req_id_t bus_req_id_in);
      m_bus_req_attrs.bus_req_id = bus_req_id_in;
   endfunction : set_bus_req_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_id
   //
   // This function is used to get the bus request ID of this descriptor.
   //
   // Return Value:
   //
   // evl_bus_req_id_t: This function returns the value of the bus request ID.
   //
   function evl_bus_req_id_t get_bus_req_id();
      return m_bus_req_attrs.bus_req_id;
   endfunction : get_bus_req_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_size
   //
   // This function is used to set the bus request size field for this descriptor.  This field is
   // an encoded value reflecting the power-of-2 size of the request.
   //
   // Input Variables:
   //
   // bus_req_size_in: This is the value to which to set the bus request size.
   //
   function void set_bus_req_size(input evl_bus_req_size_t bus_req_size_in);
      m_bus_req_attrs.bus_req_size = bus_req_size_in;
      if (m_bus_req_attrs.bus_req_len > 0) begin
         m_bus_req_attrs.bus_line_idx_mask = (((evl_paddr_t'(1) << bus_req_size_in) * (m_bus_req_attrs.bus_req_len + 1)) - 1) & ~evl_paddr_t'(`EVL_LINE_BYTES - 1);
      end
      else begin
         m_bus_req_attrs.bus_line_idx_mask = evl_paddr_t'(0);
      end
   endfunction : set_bus_req_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_size
   //
   // This function is used to get the bus request size of this request descriptor.
   //
   // Return Value:
   //
   // evl_bus_req_size_t: This function returns the value of the bus request size.
   //
   function evl_bus_req_size_t get_bus_req_size();
      return m_bus_req_attrs.bus_req_size;
   endfunction : get_bus_req_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_len
   //
   // This function is used to set the number of data bursts for this request descriptor.
   //
   // Input Variables:
   //
   // bus_req_len_in: This is the number of data bursts - 1.
   //
   function void set_bus_req_len(input evl_bus_req_len_t bus_req_len_in);
      m_bus_req_attrs.bus_req_len = bus_req_len_in;
      if (bus_req_len_in > 0) begin
         m_bus_req_attrs.bus_line_idx_mask = (((evl_paddr_t'(1) << m_bus_req_attrs.bus_req_size) * (bus_req_len_in + 1)) - 1) & ~evl_paddr_t'(`EVL_LINE_BYTES - 1);
      end
      else begin
         m_bus_req_attrs.bus_line_idx_mask = evl_paddr_t'(0);
      end
   endfunction : set_bus_req_len


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_len
   //
   // This function is used to get the number of data bursts from this request descriptor.
   //
   // Return Value:
   //
   // evl_bus_req_len_t: This function returns the number of data bursts - 1.
   //
   function evl_bus_req_len_t get_bus_req_len();
      return m_bus_req_attrs.bus_req_len;
   endfunction : get_bus_req_len


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cur_burst
   //
   // This function is used to get the current burst index for either write requests or read
   // responses.
   //
   // Return Value:
   //
   // evl_bus_req_len_t: This function returns the current burst number.
   //
   function evl_bus_req_len_t get_cur_burst();
      if (m_cur_burst >= ~evl_bus_req_len_t'(0)) begin
         return ~evl_bus_req_len_t'(0);
      end
      return evl_bus_req_len_t'(m_cur_burst);
   endfunction : get_cur_burst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pop_cur_burst
   //
   // This function is used to increment the current burst index for either write requests or read
   // responses.
   //
   // Return Value:
   //
   // int: This function returns 1 if this is the last burst or 0 if it is not the last burst.
   //
   function bit pop_cur_burst();
      if ((m_cur_burst >= m_bus_req_attrs.bus_req_len) || (m_cur_burst == ~evl_bus_req_len_t'(0))) begin
         m_cur_burst = int'(m_bus_req_attrs.bus_req_len) + 1;
         set_have_all_data();
         return 1'b1;
      end
      m_cur_burst++;
      return 1'b0;
   endfunction : pop_cur_burst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_have_all_data
   //
   // BLAH
   //
   function void set_have_all_data();
      m_have_all_data = 1'b1;
   endfunction : set_have_all_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_x_last
   //
   // BLAH.
   //
   function void set_x_last();
      m_x_last = 1'b1;
   endfunction : set_x_last


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_x_last
   //
   // BLAH.
   //
   function bit get_x_last();
      return m_x_last;
   endfunction : get_x_last


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_burst_data
   //
   // Blah.
   //
   task wait_for_burst_data(input int line_idx);
      #0 wait (m_cur_burst > line_idx);
   endtask : wait_for_burst_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_all_req_data
   //
   // Blah.
   //
   function bit have_all_req_data();
      if ((m_have_all_data == 1'b0) || (m_req_data.size() == 0)) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : have_all_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_all_data
   //
   // Blah.
   //
   task wait_for_all_data();
      #0 wait (m_have_all_data == 1'b1);
   endtask : wait_for_all_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_burst_idx
   //
   // This function is used to get the m_req_data[] or m_rsp_data[] index to use based on the
   // incoming offset.  It is assumed the incoming offset is a byte offset unless the shift input is
   // set to a non-zero value.  The shift value should be 1 for a halfword offset, 2 for a word
   // offset, etc.  If the incoming offset is negative, the request descriptor address is used as
   // the offset.
   //
   // Input Variables:
   //
   // offset_in: This input is the byte offset to use.  If the shift input is non-zero, then this
   //            input is something other than a byte offset.
   // shift:     This input is 0 if the incoming offset is a byte offset, 1 if the incoming offset
   //            is a halfword offset, 2 if the incoming offset is a word offset, etc.
   //
   // Return Value:
   //
   // int: This function returns the m_req_data[] or m_rsp_data[] index to use based on the incoming
   //      offset.
   //
   function int get_line_burst_idx(input int offset_in, input int shift = 0);
      if (offset_in < 0) begin
         return (int'(get_paddr() & m_bus_req_attrs.bus_line_idx_mask) >> `EVL_LINE_ADDR_LSB);
      end
      return (int'((offset_in << shift) & m_bus_req_attrs.bus_line_idx_mask) >> `EVL_LINE_ADDR_LSB);
   endfunction : get_line_burst_idx


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_max_line_burst_idx
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_max_line_burst_idx();
      return get_line_burst_idx(((1 << m_bus_req_attrs.bus_req_size) * (m_bus_req_attrs.bus_req_len + 1)) - 1);
   endfunction : get_max_line_burst_idx


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_qword_en
   //
   // This function is used to set the request quadword enables for this request descriptor.
   //
   // Input Variables:
   //
   // bus_qword_en_in: This is quadword enable field to use.
   //
   function void set_bus_req_qword_en(input evl_line_qword_en_t bus_qword_en_in);
      m_bus_req_attrs.bus_qword_en = bus_qword_en_in;
   endfunction : set_bus_req_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_qword_en
   //
   // This function is used to get the request quadword enables for this request descriptor.
   //
   // Return Value:
   //
   // evl_line_qword_en_t: This function returns the quadword enables for this request.
   //
   function evl_line_qword_en_t get_bus_req_qword_en();
      return m_bus_req_attrs.bus_qword_en;
   endfunction : get_bus_req_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_bytes
   //
   // This function is used to get the number of bytes requested (either as a read or a write) for
   // this request descriptor.
   //
   // Return Value:
   //
   // int: This function returns the number of byte requested by this descriptor.
   //
   function int get_bus_req_bytes();
      return ((1 << m_bus_req_attrs.bus_req_size) * (m_bus_req_attrs.bus_req_len + 1));
   endfunction : get_bus_req_bytes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_lock
   //
   // This function is used to set the value of the LOCK field for this descriptor.
   //
   // Input Variables:
   //
   // bus_lock_in: This is the value for the LOCK field.
   //
   function void set_bus_lock(input evl_bus_lock_t bus_lock_in);
      m_bus_req_attrs.bus_lock = bus_lock_in;
   endfunction : set_bus_lock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_lock
   //
   // This function is used to get set the value of the LOCK field for the AXI bus.
   //
   // Return Value:
   //
   // evl_bus_lock_t: This function returns the value of the LOCK field in this descriptor.
   //
   function evl_bus_lock_t get_bus_lock();
      return m_bus_req_attrs.bus_lock;
   endfunction : get_bus_lock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_cache_attr
   //
   // This function is used to set the value of the CACHE field for this descriptor.
   //
   // Input Variables:
   //
   // bus_cache_attr_in: This is the value of the CACHE field.
   //
   function void set_bus_cache_attr(input evl_bus_cache_attr_t bus_cache_attr_in);
      m_bus_req_attrs.bus_cache_attr = bus_cache_attr_in;
   endfunction : set_bus_cache_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cache_attr
   //
   // This function is used to get the value of the CACHE field for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_cache_attr_t: This function returns the value of the CACHE field in this descriptor.
   //
   function evl_bus_cache_attr_t get_bus_cache_attr();
      return m_bus_req_attrs.bus_cache_attr;
   endfunction : get_bus_cache_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_prot
   //
   // This function is used to set the value of the PROT field for this descriptor.
   //
   // Input Variables:
   //
   // bus_prot_in: This is the value of the PROT field.
   //
   function void set_bus_prot(input evl_bus_prot_t bus_prot_in);
      m_bus_req_attrs.bus_prot = bus_prot_in;
   endfunction : set_bus_prot


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_prot
   //
   // This function is used to get the value of the PROT field for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_prot_t: This function returns the value of the PROT field in this descriptor.
   //
   function evl_bus_prot_t get_bus_prot();
      return m_bus_req_attrs.bus_prot;
   endfunction : get_bus_prot


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_region
   //
   // This function is used to set the value of the REGION field for this descriptor.
   //
   // Input Variables:
   //
   // bus_region_in: This is the value of the REGION field.
   //
   function void set_bus_region(input evl_bus_region_t bus_region_in);
      m_bus_req_attrs.bus_region = bus_region_in;
   endfunction : set_bus_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_region
   //
   // This function is used to get the value of the REGION field for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_region_t: This function returns the value of the REGION field in this descriptor.
   //
   function evl_bus_region_t get_bus_region();
      return m_bus_req_attrs.bus_region;
   endfunction : get_bus_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_user
   //
   // This function is used to set the value of the request user-defined field for this descriptor.
   //
   // Input Variables:
   //
   // bus_req_user_in: This is the value of the request user-defined field.
   //
   function void set_bus_req_user(input evl_bus_req_user_t bus_req_user_in);
      m_bus_req_attrs.bus_req_user = bus_req_user_in;
   endfunction : set_bus_req_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_user
   //
   // This function is used to get the value of the request user-defined field for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_req_user_t: This function returns the value of the request user-defined field in this
   //                     descriptor.
   //
   function evl_bus_req_user_t get_bus_req_user();
      return m_bus_req_attrs.bus_req_user;
   endfunction : get_bus_req_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_param
   //
   // This function is used to set the value of the requested bus param field for this descriptor.
   //
   // Input Variables:
   //
   // bus_req_param_in: This is the value of the requested bus state field.
   //
   function void set_bus_req_param(input evl_bus_subopcode_t bus_req_param_in);
      m_bus_req_attrs.bus_req_param = bus_req_param_in;
   endfunction : set_bus_req_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_param
   //
   // This function is used to get the value of the requested bus param field for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_subopcode_t: This function returns the value of the requested bus param field in this
   //                      descriptor.
   //
   function evl_bus_subopcode_t get_bus_req_param();
      return m_bus_req_attrs.bus_req_param;
   endfunction : get_bus_req_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_valid
   //
   // This function is used to set the response valid bit for the specified response transfer.
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_valid(input int transfer_idx_in = 0, input int unsigned cycle = 32'h7fffffff);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      if (cycle < 32'h7fffffff) begin
         set_rsp_cycle(cycle);
      end
      m_bus_rsp_attrs[transfer_idx].rsp_valid = 1'b1;
   endfunction : set_bus_rsp_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_bus_rsp_valid
   //
   // This function is used to clear the response valid bit for the specified response transfer.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void clr_bus_rsp_valid(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         m_bus_rsp_attrs[transfer_idx].rsp_valid = 1'b0;
      end
   endfunction : clr_bus_rsp_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_is_valid
   //
   // This function is used to get the response valid bit for the specified response transfer.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // bit: This bit will be 1'b1 if the response for the specified response transfer is valid.
   //
   function bit bus_rsp_is_valid(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].rsp_valid;
      end
      return 1'b0;
   endfunction : bus_rsp_is_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_type
   //
   // This function is used to set the bus response type for the specified response transfer.
   //
   // Input Variables:
   //
   // bus_rsp_type_in: This is the bus response type to be set for the specified response transfer.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_type(input evl_bus_rsp_type_t bus_rsp_type_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_rsp_type = bus_rsp_type_in;
   endfunction : set_bus_rsp_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_rsp_type
   //
   // This function is used to set the bus response type for the specified response transfer.
   //
   // Input Variables:
   //
   // bus_rsp_type_in: This is the bus response type to be set for the specified response transfer.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_abstract_rsp_type(input evl_abstract_rsp_type_t bus_rsp_type_in, input int transfer_idx_in = 0);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      set_bus_rsp_type(m_bus_parser.convert_bus_rsp(bus_rsp_type_in, m_bus_req_attrs.bus_type), transfer_idx_in);
   endfunction : set_abstract_rsp_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_type
   //
   // This function is used to get the bus response type for the specified response transfer.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_bus_rsp_type_t: This function returns bus response type for the specified response
   //                     transfer.
   //
   function evl_bus_rsp_type_t get_bus_rsp_type(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_rsp_type;
      end
      return `EVL_Rsp_Null;
   endfunction : get_bus_rsp_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_id
   //
   // This function is used to set the bus response ID for this descriptor (i.e., the bus tag).
   //
   // Input Variables:
   //
   // bus_rsp_id_in:   This is the value to which to set the bus response ID.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_id(input evl_bus_rsp_id_t bus_rsp_id_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_rsp_id = bus_rsp_id_in;
   endfunction : set_bus_rsp_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_id
   //
   // This function is used to get the bus response ID of this descriptor.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_bus_req_id_t: This function returns the value of the bus response ID.
   //
   function evl_bus_rsp_id_t get_bus_rsp_id(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_rsp_id;
      end
      return evl_bus_rsp_id_t'(0);
   endfunction : get_bus_rsp_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_size
   //
   // This function is used to set the bus response size field for this descriptor.  This field is
   // an encoded value reflecting the power-of-2 size of the response.
   //
   // Input Variables:
   //
   // bus_rsp_size_in: This is the value to which to set the bus response size.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_size(input evl_bus_rsp_size_t bus_rsp_size_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_rsp_size = bus_rsp_size_in;
   endfunction : set_bus_rsp_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_size
   //
   // This function is used to get the bus response size of this request descriptor.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_bus_rsp_size_t: This function returns the value of the bus response size.
   //
   function evl_bus_rsp_size_t get_bus_rsp_size(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_rsp_size;
      end
      return evl_bus_rsp_size_t'(0);
   endfunction : get_bus_rsp_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_qword_en
   //
   // This function is used to set the response quadword enables for this request descriptor.
   //
   // Input Variables:
   //
   // bus_qword_en_in: This is quadword enable field to use.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_qword_en(input evl_line_qword_en_t bus_qword_en_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_qword_en = bus_qword_en_in;
   endfunction : set_bus_rsp_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_qword_en
   //
   // This function is used to get the response quadword enables for this request descriptor.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_line_qword_en_t: This function returns the quadword enables for this request.
   //
   function evl_line_qword_en_t get_bus_rsp_qword_en(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_qword_en;
      end
      return evl_line_qword_en_t'(0);
   endfunction : get_bus_rsp_qword_en


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_dest
   //
   // This function is used to set the bus response dest for this descriptor.  This field is
   // currently only used in the ET Link.
   //
   // Input Variables:
   //
   // bus_rsp_dest_in:   This is the value to which to set the bus response destination.
   // transfer_idx_in:   This is the transfer number for the response.  For narrower interfaces, it
   //                    is possible that multiple data bursts will be required to complete the
   //                    request.  The first data burst is transfer 0, the second is transfer 1,
   //                    etc.  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_dest(input evl_bus_rsp_dest_t bus_rsp_dest_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_rsp_dest = bus_rsp_dest_in;
   endfunction : set_bus_rsp_dest


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_dest
   //
   // This function is used to get the bus response destination of this descriptor.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_bus_rsp_dest_t: This function returns the value of the bus response dest.
   //
   function evl_bus_rsp_dest_t get_bus_rsp_dest(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_rsp_dest;
      end
      return evl_bus_rsp_dest_t'(0);
   endfunction : get_bus_rsp_dest


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_user
   //
   // This function is used to set the user-defined bus field for the specified response transfer.
   //
   // Input Variables:
   //
   // bus_rsp_user_in: This is the user-defined bus field to be set for the specified response
   //                  transfer.
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_user(input evl_bus_rsp_user_t bus_rsp_user_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_rsp_user = bus_rsp_user_in;
   endfunction : set_bus_rsp_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_user
   //
   // This function is used to get the user-defined bus field for the specified response transfer.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_bus_rsp_user_t: This function returns the user-defined bus field for the specified
   //                     response transfer.
   //
   function evl_bus_rsp_user_t get_bus_rsp_user(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_rsp_user;
      end
      return evl_bus_rsp_user_t'(0);
   endfunction : get_bus_rsp_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_rsp_param
   //
   // This function is used to set the bus response param field for the specified response transfer.
   //
   // Input Variables:
   //
   // bus_rsp_param_in: This is the bus response param field to be set for the specified response
   //                   transfer.
   // transfer_idx_in:  This is the transfer number for the response.  For narrower interfaces, it
   //                   is possible that multiple data bursts will be required to complete the
   //                   request.  The first data burst is transfer 0, the second is transfer 1, etc.
   //                   Currently, a maximum of 16 transfers is supported.
   //
   function void set_bus_rsp_param(input evl_bus_subopcode_t bus_rsp_param_in, input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      while (m_bus_rsp_attrs.size() <= transfer_idx) begin
         evl_req_desc_rsp_s bus_rsp_attrs = '{ rsp_valid:     1'b0,
                                               bus_rsp_type:  `EVL_Rsp_Null,
                                               bus_rsp_id:    evl_bus_rsp_id_t'(0),
                                               bus_rsp_size:  evl_bus_rsp_size_t'(0),
                                               bus_qword_en:  evl_line_qword_en_t'(0),
                                               bus_rsp_dest:  evl_bus_rsp_dest_t'(0),
                                               bus_rsp_user:  evl_bus_rsp_user_t'(0),
                                               bus_rsp_param: evl_bus_subopcode_t'(0)
                                             };

         m_bus_rsp_attrs.push_back(bus_rsp_attrs);
      end
      m_bus_rsp_attrs[transfer_idx].bus_rsp_param = bus_rsp_param_in;
   endfunction : set_bus_rsp_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_param
   //
   // This function is used to get the bus response param field for the specified response transfer.
   //
   // Input Variables:
   //
   // transfer_idx_in: This is the transfer number for the response.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.
   //
   // Return Value:
   //
   // evl_bus_subopcode_t: This function returns the bus response state field for the specified
   //                      response transfer.
   //
   function evl_bus_subopcode_t get_bus_rsp_param(input int transfer_idx_in = 0);
      int transfer_idx = transfer_idx_in & 15;

      if (transfer_idx < m_bus_rsp_attrs.size()) begin
         return m_bus_rsp_attrs[transfer_idx].bus_rsp_param;
      end
      return evl_bus_subopcode_t'(0);
   endfunction : get_bus_rsp_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_desc
   //
   // This function is used to set the local m_bus_req_desc handle to the incoming reqest
   // descriptor.  This function should only be called when an observed request descriptor is
   // created by a bus verification component/monitor and linked to a sent request descriptor.  The
   // m_bus_req_desc handle in the sent request descriptor should then be set to the observed
   // request descriptor.
   //
   virtual function void set_bus_req_desc(input evl_req_desc req_desc);
      m_bus_req_desc = req_desc;
   endfunction : set_bus_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_desc
   //
   // This function is used to get the local m_bus_req_desc handle (see above).
   //
   virtual function evl_req_desc get_bus_req_desc();
      return m_bus_req_desc;
   endfunction : get_bus_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_spawned_bus_child
   //
   // This function will return 1'b1 if this descriptor is an observed bus child descriptor and it
   // has an expected request descriptor that points to it..
   //
   virtual function bit is_spawned_bus_child();
      if ((m_bus_child == 1'b0) || (m_parent_desc == null) || (m_bus_req_desc != null)) begin
         return 1'b0;
      end
      if (m_parent_desc.get_bus_req_desc() == this) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : is_spawned_bus_child


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_observed
   //
   virtual function void set_observed();
      set_issued();
      m_observed = 1'b1;
      //
      // Because of the way bus descriptors are handled, make sure the parent desciptor is marked as
      // observed as well.
      //
      if ((m_parent_desc != null) && (is_spawned_bus_child() == 1'b1)) begin
         m_parent_desc.set_observed();
      end
   endfunction : set_observed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_been_observed
   //
   virtual function bit has_been_observed();
      return m_observed;
   endfunction : has_been_observed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_correlated
   //
   // This function is used to mark the descriptor as correlated (which means that a matching parent
   // descriptor has been found).
   //
   virtual function void set_correlated();
      m_correlated = 1'b1;
   endfunction : set_correlated


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_correlated
   //
   virtual function bit is_correlated();
      return m_correlated;
   endfunction : is_correlated


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_parent_check
   //
   // This function is used to mark the descriptor as correlated (which means that a check against a
   // matching parent should occur, regardless of whether a matching parent has been found or not).
   //
   virtual function void set_parent_check();
      m_do_parent_check = 1'b1;
   endfunction : set_parent_check


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_parent_check
   //
   virtual function bit do_parent_check();
      return m_do_parent_check;
   endfunction : do_parent_check


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_done
   //
   // This function is used to mark the descriptor as complete.  The oddities in this version of the
   // function are used to make sure a bus pair (expected and observed) are both marked as done.
   //
// virtual function void set_done();
//    if (is_done() == 1'b1) begin
//       return;
//    end
//    super.set_done();
//    if (m_bus_req_desc != null) begin
//       m_bus_req_desc.set_done();
//    end
//    else if (is_spawned_bus_child() == 1'b1) begin
//       m_parent_desc.set_done();
//    end
// endfunction : set_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_data
   //
   // This function is used to set the request data object.  It is possible to set individual
   // request data bytes with other functions in the descriptor, but the assumption here is that a
   // previously-created line data object has been initialized appropriately.  It is important to
   // note that this is NOT a copy of the object passed in; it is the SAME object.  Thus, if the
   // object is subsequently modified in some manner, the request descriptor's request data will
   // also be modified.
   //
   // Input Variables:
   //
   // line_data_in: This is a handle to the line data object to be used as this request descriptor's
   //               request data.
   // line_idx:     This is the m_req_data[] entry number to update.
   //
   function void set_req_data(input evl_line_data line_data_in, input evl_bus_req_len_t line_idx = 0);
      int cur_size = m_req_data.size();

      if ((line_data_in != null) || (cur_size > 0)) begin
         m_req_data_avail = 0;
         while (cur_size <= line_idx) begin
            cur_size++;
            m_req_data.push_back(null);
         end
         m_req_data[line_idx] = line_data_in;
         for (int ii = m_req_data_callbacks.size() - 1; ii >= 0; ii--) begin
            if (m_req_data_callbacks[ii].cascade_req_data_to_dst() == 1'b1) begin
               m_req_data_callbacks.delete(ii);
            end
         end
         foreach (m_req_data[ii]) begin
            if (m_req_data[ii] == null) begin
               m_req_data_event++;
               return;
            end
         end
         m_req_data_avail = m_req_data.size();
         m_req_data_event++;
      end
   endfunction : set_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_data
   //
   // This function is used to get a handle to the request data object.  There are a number of input
   // variables to control what to do in the event there is currently no request line data object.
   // It is important to note that this is NOT a handle to a copy of the request data object; it is
   // the SAME object.  Thus, if the object is subsequently modified in some manner, the request
   // descriptor's request data will also be modified.
   //
   // Input Variables:
   //
   // create_data_in:        If no request data object currently exists and this variable is 1'b1, a
   //                        new line data object will be created, m_req_data[] will be set to point
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
   // evl_line_data: This is a handle to the descriptor's request data object.  If no request data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_req_data(input evl_bus_req_len_t line_idx              = 0,
                                       input bit               create_data_in        = 1'b0,
                                       input bit               random_data_in        = 1'b0,
                                       input evl_line_data     template_line_data_in = null);
      if (m_req_data.size() == 0) begin
         if (create_data_in == 1'b0) begin
            return null;
         end
         set_req_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      else if ((m_req_data[line_idx] == null) && (create_data_in == 1'b1)) begin
         set_req_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      return m_req_data[line_idx];
   endfunction : get_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_data_copy
   //
   // This function is used to get a handle to a copy of the request data object.  There are a
   // number of input variables to control what to do in the event there is currently no request
   // line data object.
   //
   // Input Variables:
   //
   // create_data_in:        If no request data object currently exists and this variable is 1'b1, a
   //                        new line data object will be created, m_req_data[] will be set to point
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
   // evl_line_data: This is a handle to the descriptor's request data object.  If no request data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_req_data_copy(input evl_bus_req_len_t line_idx              = 0,
                                            input bit               create_data_in        = 1'b0,
                                            input bit               random_data_in        = 1'b0,
                                            input evl_line_data     template_line_data_in = null);
      evl_line_data data_copy;

      if (m_req_data.size() == 0) begin
         if (create_data_in == 1'b0) begin
            return null;
         end
         set_req_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      else if ((m_req_data[line_idx] == null) && (create_data_in == 1'b1)) begin
         set_req_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      $cast(data_copy, m_req_data[line_idx].clone());
      return data_copy;
   endfunction : get_req_data_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_req_data_by_index
   //
   // This function is used to inquire about the state of a particular m_req_data[] entry.
   //
   // Input Variables:
   //
   // line_idx: This is the m_req_data[] index to use when inquiring about a request data object.
   //           If this is negative, then all request data objects must become valid rather than
   //           just a single request data object.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if valid data is available in the m_req_data[] object(s).
   //
   function bit have_req_data_by_index(input int line_idx = -1);
      if (line_idx < 0) begin
         for (int ii = get_max_line_burst_idx(); ii >= 0; ii--) begin
            if ((m_req_data.size() <= ii) || (m_req_data[ii] == null)) begin
               return 1'b0;
            end
         end
      end
      else if ((m_req_data.size() <= line_idx) || (m_req_data[line_idx] == null)) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : have_req_data_by_index


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_req_data_by_index
   //
   // This task is used to wait until the request data at a particular index becomes available.
   //
   // Input Variables:
   //
   // line_idx: This is the m_req_data[] index to use when inquiring about a request data object.
   //           If this is negative, then all request data objects must become valid rather than
   //           just a single request data object.
   //
   task wait_for_req_data_by_index(input int line_idx = -1);
      byte req_data_event;

      if (line_idx < 0) begin
         for (int ii = get_max_line_burst_idx(); ii >= 0; ii--) begin
            while (1) begin
               req_data_event = m_req_data_event;
               if ((m_req_data.size() > ii) && (m_req_data[ii] != null)) begin
                  break;
               end
               #0 wait (m_req_data_event != req_data_event);
            end
         end
         return;
      end
      while (1) begin
         req_data_event = m_req_data_event;
         if ((m_req_data.size() > line_idx) && (m_req_data[line_idx] != null)) begin
            return;
         end
         #0 wait (m_req_data_event != req_data_event);
      end
   endtask : wait_for_req_data_by_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cascade_req_data_to_dst
   //
   // This task is used to set the request data object from a source request descriptor to a
   // destination request descriptor.  Because data can lag, it may be necessary wait until a
   // non-null source request data object is available.
   //
   // Input Variables:
   //
   // dst_req_desc:  This is a handle to the destination request descriptor.
   // from_callback: This input will be 1'b1 if the call has been made from a callback object.
   //
   function bit cascade_req_data_to_dst(input evl_req_desc dst_req_desc,
                                        input bit          from_callback = 1'b0);
      byte req_data_event = m_req_data_event;

      if (dst_req_desc == null) begin
         return 1'b0;
      end
      if (have_req_data_by_index() == 1'b0) begin
         if (from_callback == 1'b0) begin
            evl_req_desc_callback req_desc_callback = new(this, dst_req_desc);

            m_req_data_callbacks.push_back(req_desc_callback);
         end
         if (req_data_event != m_req_data_event) begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: request data was updated before creation of callback (%0s)", sprint_obj()))
         end
         return 1'b0;
      end
      dst_req_desc.set_req_data(get_req_data());
      return 1'b1;
   endfunction : cascade_req_data_to_dst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_byte
   //
   // This function is used to set a byte in the request data object at the specified byte offset.
   // If the request data handle is null, a new line data object based on this descriptor's line
   // data template will be created (filled with 0s) and the specified byte will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the byte offset within the request data object to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_req_byte(input int byte_index_in, input byte byte_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(byte_index_in), 1'b1);

      void'(set_byte(req_data, byte_index_in, byte_in));
   endfunction : set_req_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_hword
   //
   // This function is used to set a halfword (2 bytes) in the request data object at the specified
   // halfword offset.  If the request data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified halfword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the request data object to modify (address
   //                 bits 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_req_hword(input int hword_index_in, input evl_hword_t hword_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(hword_index_in, `EVL_HWORD_ADDR_LSB), 1'b1);

      void'(set_hword(req_data, hword_index_in, hword_in));
   endfunction : set_req_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_word
   //
   // This function is used to set a word (4 bytes) in the request data object at the specified word
   // offset.  If the request data handle is null, a new line data object based on this descriptor's
   // line data template will be created (filled with 0s) and the specified word will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within the request data object to modify (address
   //                bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_req_word(input int word_index_in, input evl_word_t word_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(word_index_in, `EVL_WORD_ADDR_LSB), 1'b1);

      void'(set_word(req_data, word_index_in, word_in));
   endfunction : set_req_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_dword
   //
   // This function is used to set a doubleword (8 bytes) in the request data object at the
   // specified doubleword offset.  If the request data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // doubleword will be modified appropriately.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the request data object to modify
   //                 (address bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   function void set_req_dword(input int dword_index_in, input evl_dword_t dword_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(dword_index_in, `EVL_DWORD_ADDR_LSB), 1'b1);

      void'(set_dword(req_data, dword_index_in, dword_in));
   endfunction : set_req_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_qword
   //
   // This function is used to set a quadword (16 bytes) in the request data object at the specified
   // quadword offset.  If the request data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified quadword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the request data object to modify (address
   //                 bits 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_req_qword(input int qword_index_in, input evl_qword_t qword_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(qword_index_in, `EVL_QWORD_ADDR_LSB), 1'b1);

      void'(set_qword(req_data, qword_index_in, qword_in));
   endfunction : set_req_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_oword
   //
   // This function is used to set an octalword (32 bytes) in the request data object at the
   // specified octalword offset.  If the request data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // octalword will be modified appropriately.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the request data object to modify (address
   //                 bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_req_oword(input int oword_index_in, input evl_oword_t oword_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(oword_index_in, `EVL_OWORD_ADDR_LSB), 1'b1);

      void'(set_oword(req_data, oword_index_in, oword_in));
   endfunction : set_req_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_xword
   //
   // This function is used to set a hexword (64 bytes) in the request data object at the specified
   // hexword offset.  If the request data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified hexword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the request data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_req_xword(input int xword_index_in, input evl_xword_t xword_in);
      evl_line_data req_data = get_req_data(get_line_burst_idx(xword_index_in, `EVL_XWORD_ADDR_LSB), 1'b1);

      void'(set_xword(req_data, xword_index_in, xword_in));
   endfunction : set_req_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_req_data
   //
   // This function is used to set request data appropriately.  It is assumed that the data will be
   // right-justified to the size of the write.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the request data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_obs_req_data(input evl_paddr_t         addr_in,
                                  input int                 byte_count,
                                  input evl_xword_t         xword_in,
                                  input evl_xword_byte_en_t byte_en_in);
      int           offset = int'(addr_in) & 32'h7fffffff;
      evl_line_data req_data;

      if (m_req_data.size() == 0) begin
         invalidate_req_data();
      end
      req_data = get_req_data(get_line_burst_idx(offset, 0));
      offset   = offset & (`EVL_LINE_BYTES - 1);
      if (byte_count > 32) begin
         req_data.set_xword(offset >> `EVL_XWORD_ADDR_LSB, xword_in, byte_en_in);
      end
      else if (byte_count > 16) begin
         req_data.set_oword(offset >> `EVL_OWORD_ADDR_LSB, evl_oword_t'(xword_in), evl_oword_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 8) begin
         req_data.set_qword(offset >> `EVL_QWORD_ADDR_LSB, evl_qword_t'(xword_in), evl_qword_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 4) begin
         req_data.set_dword(offset >> `EVL_DWORD_ADDR_LSB, evl_dword_t'(xword_in), evl_dword_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 2) begin
         req_data.set_word(offset >> `EVL_WORD_ADDR_LSB, evl_word_t'(xword_in), evl_word_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 1) begin
         req_data.set_hword(offset >> `EVL_HWORD_ADDR_LSB, evl_hword_t'(xword_in), evl_hword_byte_en_t'(byte_en_in));
      end
      else begin
         req_data.set_byte(offset, xword_in[7:0], byte_en_in[0:0]);
      end
   endfunction : set_obs_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_byte
   //
   // This function is used to get a byte from the request data object at the specified byte offset.
   // If the request data handle is null and the create_data_in input is 1'b1, a new request data
   // object based on this descriptor's line data template will be created (filled with 0s) and the
   // specified halfword will be returned.
   //
   // Input Variables:
   //
   // byte_index_in:  This is the byte offset within the request data object to read.
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // byte: This is the request data byte at the specified byte offset.
   //
   function byte get_req_byte(input int byte_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(byte_index_in), create_data_in);

      return get_byte(req_data, byte_index_in, create_data_in);
   endfunction : get_req_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_hword
   //
   // This function is used to get a halfword (2 bytes) from the request data object at the
   // specified halfword offset.  If the request data handle is null and the create_data_in input is
   // 1'b1, a new request data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified halfword will be returned.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the request data object to read (address
   //                 bits 5-1).
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_hword_t: This is the request data halfword at the specified halfword offset.
   //
   function evl_hword_t get_req_hword(input int hword_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(hword_index_in, `EVL_HWORD_ADDR_LSB), create_data_in);

      return get_hword(req_data, hword_index_in, create_data_in);
   endfunction : get_req_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_word
   //
   // This function is used to get a word (4 bytes) from the request data object at the specified
   // word offset.  If the request data handle is null and the create_data_in input is 1'b1, a new
   // request data object based on this descriptor's line data template will be created (filled with
   // 0s) and the specified word will be returned.
   //
   // Input Variables:
   //
   // word_index_in:  This is the word offset within the request data object to read (address
   //                 bits 5-2).
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_word_t: This is the request data word at the specified word offset.
   //
   function evl_word_t get_req_word(input int word_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(word_index_in, `EVL_WORD_ADDR_LSB), create_data_in);

      return get_word(req_data, word_index_in, create_data_in);
   endfunction : get_req_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_dword
   //
   // This function is used to get a doubleword (8 bytes) from the request data object at the
   // specified doubleword offset.  If the request data handle is null and the create_data_in input
   // is 1'b1, a new request data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified doubleword will be returned.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the request data object to read (address
   //                 bits 5-3).
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_dword_t: This is the request data doubleword at the specified doubleword offset.
   //
   function evl_dword_t get_req_dword(input int dword_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(dword_index_in, `EVL_DWORD_ADDR_LSB), create_data_in);

      return get_dword(req_data, dword_index_in, create_data_in);
   endfunction : get_req_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_qword
   //
   // This function is used to get a quadword (16 bytes) from the request data object at the
   // specified quadword offset.  If the request data handle is null and the create_data_in input is
   // 1'b1, a new request data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified quadword will be returned.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the request data object to read (address
   //                 bits 5-4).
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_qword_t: This is the request data quadword at the specified quadword offset.
   //
   function evl_qword_t get_req_qword(input int qword_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(qword_index_in, `EVL_QWORD_ADDR_LSB), create_data_in);

      return get_qword(req_data, qword_index_in, create_data_in);
   endfunction : get_req_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_oword
   //
   // This function is used to get an octalword (32 bytes) from the request data object at the
   // specified octalword offset.  If the request data handle is null and the create_data_in input
   // is 1'b1, a new request data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified octalword will be returned.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the request data object to read (address
   //                 bit 5).
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_oword_t: This is the request data octalword at the specified octalword offset.
   //
   function evl_oword_t get_req_oword(input int oword_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(oword_index_in, `EVL_OWORD_ADDR_LSB), create_data_in);

      return get_oword(req_data, oword_index_in, create_data_in);
   endfunction : get_req_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_xword
   //
   // This function is used to get a hexword (64 bytes) from the request data object at the
   // specified hexword offset.  If the request data handle is null and the create_data_in input is
   // 1'b1, a new request data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified hexword will be returned.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the request data object to read (always 0).
   // create_data_in: If this input is 1'b1, a new request data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_xword_t: This is the request data hexword at the specified hexword offset.
   //
   function evl_xword_t get_req_xword(input int xword_index_in, input bit create_data_in = 1'b0);
      evl_line_data req_data = get_req_data(get_line_burst_idx(xword_index_in, `EVL_XWORD_ADDR_LSB), create_data_in);

      return get_xword(req_data, xword_index_in, create_data_in);
   endfunction : get_req_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_req_data
   //
   // This function is used to BLAH.
   //
   function void invalidate_req_data();
      for (int ii = get_max_line_burst_idx(); ii >= 0; ii--) begin
         evl_line_data req_data = get_req_data(ii, 1'b1);

         req_data.invalidate_line();
      end
   endfunction : invalidate_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_data
   //
   // This function is used to set the response data object.  It is possible to set individual
   // response data bytes with other functions in the descriptor, but the assumption here is that a
   // previously-created line data object has been initialized appropriately.  It is important to
   // note that this is NOT a copy of the object passed in; it is the SAME object.  Thus, if the
   // object is subsequently modified in some manner, the request descriptor's response data will
   // also be modified.
   //
   // Input Variables:
   //
   // line_data_in: This is a handle to the line data object to be used as this request descriptor's
   //               response data.
   // line_idx:     This is the m_rsp_data[] entry number to update.
   //
   function void set_rsp_data(input evl_line_data line_data_in, input evl_bus_req_len_t line_idx = 0);
      int cur_size = m_rsp_data.size();

      if ((line_data_in != null) || (cur_size > 0)) begin
         m_rsp_data_avail = 0;
         while (cur_size <= line_idx) begin
            cur_size++;
            m_rsp_data.push_back(null);
         end
         m_rsp_data[line_idx] = line_data_in;
         for (int ii = m_rsp_data_callbacks.size() - 1; ii >= 0; ii--) begin
            if (m_rsp_data_callbacks[ii].cascade_rsp_data_to_dst() == 1'b1) begin
               m_rsp_data_callbacks.delete(ii);
            end
         end
         foreach (m_rsp_data[ii]) begin
            if (m_rsp_data[ii] == null) begin
               m_rsp_data_event++;
               return;
            end
         end
         m_rsp_data_avail = m_rsp_data.size();
         m_rsp_data_event++;
      end
   endfunction : set_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_data
   //
   // This function is used to get a handle to the response data object.  There are a number of
   // input variables to control what to do in the event there is currently no response line data
   // object.  It is important to note that this is NOT a handle to a copy of the response data
   // object; it is the SAME object.  Thus, if the object is subsequently modified in some manner,
   // the request descriptor's response data will also be modified.
   //
   // Input Variables:
   //
   // line_idx:              This is used to select which m_rsp_data[] entry to use.
   // create_data_in:        If no response data object currently exists and this variable is 1'b1,
   //                        a new line data object will be created, m_rsp_data[] will be set to
   //                        point to this new object, and a handle to the new object will be
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
   // evl_line_data: This is a handle to the descriptor's response data object.  If no response data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_rsp_data(input evl_bus_req_len_t line_idx              = 0,
                                       input bit               create_data_in        = 1'b0,
                                       input bit               random_data_in        = 1'b0,
                                       input evl_line_data     template_line_data_in = null);
      if (m_rsp_data.size() == 0) begin
         if (create_data_in == 1'b0) begin
            return null;
         end
         set_rsp_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      else if ((m_rsp_data[line_idx] == null) && (create_data_in == 1'b1)) begin
         set_rsp_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      return m_rsp_data[line_idx];
   endfunction : get_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_data_copy
   //
   // This function is used to get a handle to a copy of the response data object.  There are a
   // number of input variables to control what to do in the event there is currently no response
   // line data object.
   //
   // Input Variables:
   //
   // line_idx:              This is used to select which m_rsp_data[] entry to use.
   // create_data_in:        If no response data object currently exists and this variable is 1'b1,
   //                        a new line data object will be created, m_rsp_data[] will be set to
   //                        point to this new object, and a handle to the new object will be
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
   // evl_line_data: This is a handle to the descriptor's response data object.  If no response data
   //                object exists and the create_data_in input variable is 1'b0, the return value
   //                will be null.
   //
   function evl_line_data get_rsp_data_copy(input evl_bus_req_len_t line_idx              = 0,
                                            input bit               create_data_in        = 1'b0,
                                            input bit               random_data_in        = 1'b0,
                                            input evl_line_data     template_line_data_in = null);
      evl_line_data data_copy;

      if (m_rsp_data.size() == 0) begin
         if (create_data_in == 1'b0) begin
            return null;
         end
         set_rsp_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      else if ((m_rsp_data[line_idx] == null) && (create_data_in == 1'b1)) begin
         set_rsp_data(create_data(random_data_in, template_line_data_in), line_idx);
      end
      $cast(data_copy, m_rsp_data[line_idx].clone());
      return data_copy;
   endfunction : get_rsp_data_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_rsp_data_by_index
   //
   // This function is used to inquire about the state of a particular m_rsp_data[] entry.
   //
   // Input Variables:
   //
   // line_idx: This is the m_rsp_data[] index to use when inquiring about a request data object.
   //           If this is negative, then all request data objects must become valid rather than
   //           just a single request data object.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 is returned if valid data is available in the m_rsp_data[] object(s).
   //
   function bit have_rsp_data_by_index(input int line_idx = -1);
      if (line_idx < 0) begin
         for (int ii = get_max_line_burst_idx(); ii >= 0; ii--) begin
            if ((m_rsp_data.size() <= ii) || (m_rsp_data[ii] == null)) begin
               return 1'b0;
            end
         end
      end
      else if ((m_rsp_data.size() <= line_idx) || (m_rsp_data[line_idx] == null)) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : have_rsp_data_by_index


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_rsp_data_by_index
   //
   // This task is used to wait until the request data at a particular index becomes available.
   //
   // Input Variables:
   //
   // line_idx: This is the m_rsp_data[] index to use when inquiring about a request data object.
   //           If this is negative, then all request data objects must become valid rather than
   //           just a single request data object.
   //
   task wait_for_rsp_data_by_index(input int line_idx = -1);
      byte rsp_data_event;

      rsp_data_event = m_rsp_data_event;
      if (line_idx < 0) begin
         for (int ii = get_max_line_burst_idx(); ii >= 0; ii--) begin
            while (1) begin
               if ((m_rsp_data.size() > ii) && (m_rsp_data[ii] != null)) begin
                  break;
               end
               #0 wait (m_rsp_data_event != rsp_data_event);
               rsp_data_event = m_rsp_data_event;
            end
         end
         return;
      end
      while (1) begin
         if ((m_rsp_data.size() > line_idx) && (m_rsp_data[line_idx] != null)) begin
            return;
         end
         #0 wait (m_rsp_data_event != rsp_data_event);
         rsp_data_event = m_rsp_data_event;
      end
   endtask : wait_for_rsp_data_by_index


   //-----------------------------------------------------------------------------------------------
   //
   // Function: cascade_rsp_data_to_dst
   //
   // This task is used to set the response data object from a source request descriptor to a
   // destination request descriptor.  Because there may be multiple beats of data, it may be
   // necessary wait until a non-null source response data object is available.
   //
   // Input Variables:
   //
   // dst_req_desc:  This is a handle to the destination request descriptor.
   // line_idx_in:   This is the m_rsp_data[] index to use; if it is negative, all m_rsp_data[]
   //                entries will be used.
   // use_size:      This input should be set to 1'b1 if response should use the request's data size
   //                when filling out the response data.  The side effect is to control which bytes
   //                to compare on the bus.
   // from_callback: This input will be 1'b1 if the call has been made from a callback object.
   //
   function bit cascade_rsp_data_to_dst(input evl_req_desc dst_req_desc,
                                        input int          line_idx_in   = -1,
                                        input bit          use_size      = 1'b0,
                                        input bit          from_callback = 1'b0);
      byte rsp_data_event = m_rsp_data_event;
      int  line_idx       = (line_idx_in < 0) ? 0 : line_idx_in;

      if (dst_req_desc == null) begin
         return 1'b0;
      end
      if (have_rsp_data_by_index(line_idx_in) == 1'b0) begin
         if (from_callback == 1'b0) begin
            evl_req_desc_callback req_desc_callback = new(this, dst_req_desc, line_idx_in, use_size);

            m_rsp_data_callbacks.push_back(req_desc_callback);
         end
         if (rsp_data_event != m_rsp_data_event) begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: response data was updated before creation of callback (%0s)", sprint_obj()))
         end
         return 1'b0;
      end
      if (use_size == 1'b1) begin
         dst_req_desc.copy_sized_rsp_data(get_rsp_data(line_idx), line_idx);
      end
      else if (get_bus_req_size() < dst_req_desc.get_bus_req_size()) begin
         evl_line_data cur_rsp_data;
         evl_line_data new_rsp_data;

         new_rsp_data = get_rsp_data(line_idx);
         if (new_rsp_data != null) begin
            cur_rsp_data = dst_req_desc.get_rsp_data(line_idx);
            if (cur_rsp_data == null) begin
               cur_rsp_data = dst_req_desc.get_rsp_data(line_idx, 1'b1);
            end
            cur_rsp_data.merge_data_from_line(new_rsp_data);
         end
      end
      else begin
         dst_req_desc.set_rsp_data(get_rsp_data(line_idx), line_idx);
      end
      return 1'b1;
   endfunction : cascade_rsp_data_to_dst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_byte
   //
   // This function is used to set a byte in the response data object at the specified byte offset.
   // If the response data handle is null, a new line data object based on this descriptor's line
   // data template will be created (filled with 0s) and the specified byte will be modified
   // appropriately.
   //
   // Input Variables:
   //
   // byte_index_in: This is the byte offset within the response data object to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   function void set_rsp_byte(input int byte_index_in, input byte byte_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(byte_index_in), 1'b1);

      void'(set_byte(rsp_data, byte_index_in, byte_in));
   endfunction : set_rsp_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_hword
   //
   // This function is used to set a halfword (2 bytes) in the response data object at the specified
   // halfword offset.  If the response data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified halfword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the response data object to modify (address
   //                 bits 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   function void set_rsp_hword(input int hword_index_in, input evl_hword_t hword_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(hword_index_in, `EVL_HWORD_ADDR_LSB), 1'b1);

      void'(set_hword(rsp_data, hword_index_in, hword_in));
   endfunction : set_rsp_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_word
   //
   // This function is used to set a word (4 bytes) in the response data object at the specified
   // word offset.  If the response data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified word will
   // be modified appropriately.
   //
   // Input Variables:
   //
   // word_index_in: This is the word offset within the response data object to modify (address
   //                bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   function void set_rsp_word(input int word_index_in, input evl_word_t word_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(word_index_in, `EVL_WORD_ADDR_LSB), 1'b1);

      void'(set_word(rsp_data, word_index_in, word_in));
   endfunction : set_rsp_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_dword
   //
   // This function is used to set a doubleword (8 bytes) in the response data object at the
   // specified doubleword offset.  If the response data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) and the
   // specified doubleword will be modified appropriately.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the response data object to modify
   //                 (address bits 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   function void set_rsp_dword(input int dword_index_in, input evl_dword_t dword_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(dword_index_in, `EVL_DWORD_ADDR_LSB), 1'b1);

      void'(set_dword(rsp_data, dword_index_in, dword_in));
   endfunction : set_rsp_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_qword
   //
   // This function is used to set a quadword (16 bytes) in the response data object at the
   // specified quadword offset.  If the response data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // quadword will be modified appropriately.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the response data object to modify (address
   //                 bits 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   function void set_rsp_qword(input int qword_index_in, input evl_qword_t qword_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(qword_index_in, `EVL_QWORD_ADDR_LSB), 1'b1);

      void'(set_qword(rsp_data, qword_index_in, qword_in));
   endfunction : set_rsp_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_oword
   //
   // This function is used to set an octalword (32 bytes) in the response data object at the
   // specified octalword offset.  If the response data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // octalword will be modified appropriately.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the response data object to modify
   //                 (address bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   function void set_rsp_oword(input int oword_index_in, input evl_oword_t oword_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(oword_index_in, `EVL_OWORD_ADDR_LSB), 1'b1);

      void'(set_oword(rsp_data, oword_index_in, oword_in));
   endfunction : set_rsp_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_xword
   //
   // This function is used to set a hexword (64 bytes) in the response data object at the specified
   // hexword offset.  If the response data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified hexword
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the response data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_rsp_xword(input int xword_index_in, input evl_xword_t xword_in);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(xword_index_in, `EVL_XWORD_ADDR_LSB), 1'b1);

      void'(set_xword(rsp_data, xword_index_in, xword_in));
   endfunction : set_rsp_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_obs_rsp_data
   //
   // This function is used to set response data appropriately.  It is assumed that the data will be
   // right-justified to the size of the read.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the response data object to modify (always
   //                 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   function void set_obs_rsp_data(input evl_paddr_t         addr_in,
                                  input int                 byte_count,
                                  input evl_xword_t         xword_in,
                                  input evl_xword_byte_en_t byte_en_in);
      int           offset = int'(addr_in) & 32'h7fffffff;
      evl_line_data rsp_data;

      if (m_rsp_data.size() == 0) begin
         invalidate_rsp_data();
      end
      rsp_data = get_rsp_data(get_line_burst_idx(offset, 0));
      offset   = offset & (`EVL_LINE_BYTES - 1);
      if (byte_count > 32) begin
         rsp_data.set_xword(offset >> `EVL_XWORD_ADDR_LSB, xword_in, byte_en_in);
      end
      else if (byte_count > 16) begin
         rsp_data.set_oword(offset >> `EVL_OWORD_ADDR_LSB, evl_oword_t'(xword_in), evl_oword_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 8) begin
         rsp_data.set_qword(offset >> `EVL_QWORD_ADDR_LSB, evl_qword_t'(xword_in), evl_qword_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 4) begin
         rsp_data.set_dword(offset >> `EVL_DWORD_ADDR_LSB, evl_dword_t'(xword_in), evl_dword_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 2) begin
         rsp_data.set_word(offset >> `EVL_WORD_ADDR_LSB, evl_word_t'(xword_in), evl_word_byte_en_t'(byte_en_in));
      end
      else if (byte_count > 1) begin
         rsp_data.set_hword(offset >> `EVL_HWORD_ADDR_LSB, evl_hword_t'(xword_in), evl_hword_byte_en_t'(byte_en_in));
      end
      else begin
         rsp_data.set_byte(offset, xword_in[7:0], byte_en_in[0:0]);
      end
   endfunction : set_obs_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: validate_rsp_data_on_size_and_offset
   //
   function void validate_rsp_data_on_size_and_offset();
      evl_line_data rsp_data;

      rsp_data = get_rsp_data(get_line_burst_idx(-1));
      if (rsp_data != null) begin
         rsp_data.validate_on_size_and_offset(get_paddr(), get_bus_req_size());
      end
   endfunction : validate_rsp_data_on_size_and_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_byte
   //
   // This function is used to get a byte from the response data object at the specified byte
   // offset.  If the response data handle is null and the create_data_in input is 1'b1, a new
   // response data object based on this descriptor's line data template will be created (filled
   // with 0s) and the specified halfword will be returned.
   //
   // Input Variables:
   //
   // byte_index_in:  This is the byte offset within the response data object to read.
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // byte: This is the response data byte at the specified byte offset.
   //
   function byte get_rsp_byte(input int byte_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(byte_index_in), create_data_in);

      return get_byte(rsp_data, byte_index_in, create_data_in);
   endfunction : get_rsp_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_hword
   //
   // This function is used to get a halfword (2 bytes) from the response data object at the
   // specified halfword offset.  If the response data handle is null and the create_data_in input
   // is 1'b1, a new response data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified halfword will be returned.
   //
   // Input Variables:
   //
   // hword_index_in: This is the halfword offset within the response data object to read (address
   //                 bits 5-1).
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_hword_t: This is the response data halfword at the specified halfword offset.
   //
   function evl_hword_t get_rsp_hword(input int hword_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(hword_index_in, `EVL_HWORD_ADDR_LSB), create_data_in);

      return get_hword(rsp_data, hword_index_in, create_data_in);
   endfunction : get_rsp_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_word
   //
   // This function is used to get a word (4 bytes) from the response data object at the specified
   // word offset.  If the response data handle is null and the create_data_in input is 1'b1, a new
   // response data object based on this descriptor's line data template will be created (filled
   // with 0s) and the specified word will be returned.
   //
   // Input Variables:
   //
   // word_index_in:  This is the word offset within the response data object to read (address
   //                 bits 5-2).
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_word_t: This is the response data word at the specified word offset.
   //
   function evl_word_t get_rsp_word(input int word_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(word_index_in, `EVL_WORD_ADDR_LSB), create_data_in);

      return get_word(rsp_data, word_index_in, create_data_in);
   endfunction : get_rsp_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_dword
   //
   // This function is used to get a doubleword (8 bytes) from the response data object at the
   // specified doubleword offset.  If the response data handle is null and the create_data_in input
   // is 1'b1, a new response data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified doubleword will be returned.
   //
   // Input Variables:
   //
   // dword_index_in: This is the doubleword offset within the response data object to read (address
   //                 bits 5-3).
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_dword_t: This is the response data doubleword at the specified doubleword offset.
   //
   function evl_dword_t get_rsp_dword(input int dword_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(dword_index_in, `EVL_DWORD_ADDR_LSB), create_data_in);

      return get_dword(rsp_data, dword_index_in, create_data_in);
   endfunction : get_rsp_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_qword
   //
   // This function is used to get a quadword (16 bytes) from the response data object at the
   // specified quadword offset.  If the response data handle is null and the create_data_in input
   // is 1'b1, a new response data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified quadword will be returned.
   //
   // Input Variables:
   //
   // qword_index_in: This is the quadword offset within the response data object to read (address
   //                 bits 5-4).
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_qword_t: This is the response data quadword at the specified quadword offset.
   //
   function evl_qword_t get_rsp_qword(input int qword_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(qword_index_in, `EVL_QWORD_ADDR_LSB), create_data_in);

      return get_qword(rsp_data, qword_index_in, create_data_in);
   endfunction : get_rsp_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_oword
   //
   // This function is used to get an octalword (32 bytes) from the response data object at the
   // specified octalword offset.  If the response data handle is null and the create_data_in input
   // is 1'b1, a new response data object based on this descriptor's line data template will be
   // created (filled with 0s) and the specified octalword will be returned.
   //
   // Input Variables:
   //
   // oword_index_in: This is the octalword offset within the response data object to read (address
   //                 bit 5).
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_oword_t: This is the response data octalword at the specified octalword offset.
   //
   function evl_oword_t get_rsp_oword(input int oword_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(oword_index_in, `EVL_OWORD_ADDR_LSB), create_data_in);

      return get_oword(rsp_data, oword_index_in, create_data_in);
   endfunction : get_rsp_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_xword
   //
   // This function is used to get a hexword (64 bytes) from the response data object at the
   // specified hexword offset.  If the response data handle is null and the create_data_in input is
   // 1'b1, a new response data object based on this descriptor's line data template will be created
   // (filled with 0s) and the specified hexword will be returned.
   //
   // Input Variables:
   //
   // xword_index_in: This is the hexword offset within the response data object to read (always 0).
   // create_data_in: If this input is 1'b1, a new response data object will be created if none
   //                 currently exists.
   //
   // Return Value:
   //
   // evl_xword_t: This is the response data hexword at the specified hexword offset.
   //
   function evl_xword_t get_rsp_xword(input int xword_index_in, input bit create_data_in = 1'b0);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(xword_index_in, `EVL_XWORD_ADDR_LSB), create_data_in);

      return get_xword(rsp_data, xword_index_in, create_data_in);
   endfunction : get_rsp_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: invalidate_rsp_data
   //
   // This function is used to BLAH.
   //
   function void invalidate_rsp_data();
      for (int ii = get_max_line_burst_idx(); ii >= 0; ii--) begin
         evl_line_data rsp_data = get_rsp_data(ii, 1'b1);

         rsp_data.invalidate_line();
      end
   endfunction : invalidate_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_type
   //
   // This function is used to set the bus type for this request descriptor.
   //
   // Input Variables:
   //
   // delay_key: This is the bus type to use for this request descriptor.
   //
   virtual function void set_bus_type(input evl_bus_type_t bus_type_in);
      m_bus_req_attrs.bus_type = bus_type_in;
      m_bus_parser             = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
   endfunction : set_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_type
   //
   // This function is used to get the bus type for this request descriptor.
   //
   // Return Value:
   //
   // evl_bus_type_t: This function returns the bus type for this request descriptor.
   //
   virtual function evl_bus_type_t get_bus_type();
      return m_bus_req_attrs.bus_type;
   endfunction : get_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   // This function is used to get a name for the specified bus command.  If the input variable is
   // `EVL_Bus_Idle, the bus command in this request descriptor is used for evaluation.  If it is
   // something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // string: This function returns the command name for the evaluated bus command.
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.get_bus_cmd_name(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.get_bus_cmd_name(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_requires_rsp
   //
   // This function is used to determine if the specified bus command requires a response.  If the
   // input variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for
   // evaluation.  If it is something other than `EVL_Bus_Idle, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command requires a response.
   //
   virtual function bit bus_cmd_requires_rsp(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_requires_rsp(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_requires_rsp(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_requires_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   // This function is used to determine if the specified bus command is a read.  If the input
   // variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for evaluation.
   // If it is something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a read.
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_is_read(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_is_read(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   // This function is used to determine if the specified bus command is a write.  If the input
   // variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for evaluation.
   // If it is something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a write.
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_is_write(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_is_write(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   // This function is used to determine if the specified bus command is a cache operation.  If the
   // input variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for
   // evaluation.  If it is something other than `EVL_Bus_Idle, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a cache operation.
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_is_cache_op(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_is_cache_op(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_scratchpad
   //
   // This function is used to determine if the specified bus command is a scratchpad operation.  If
   // the input variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for
   // evaluation.  If it is something other than `EVL_Bus_Idle, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a scratchpad operation.
   //
   virtual function bit bus_cmd_is_scratchpad(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_is_scratchpad(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_is_scratchpad(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_is_scratchpad


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   // This function is used to determine if the specified bus command is an atomic operation.  If
   // the input variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for
   // evaluation.  If it is something other than `EVL_Bus_Idle, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is an atomic operation.
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_is_atomic(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_is_atomic(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_is_atomic


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_acquire
   //
   // This function is used to determine if the specified bus command is a TileLink Acquire.  If the
   // input variable is `EVL_Bus_Idle, the bus command in the request descriptor is used for
   // evaluation.  If it is something other than `EVL_Bus_Idle, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a TileLink Acquire.
   //
   virtual function bit bus_cmd_is_acquire(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_is_acquire(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_is_acquire(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_is_acquire


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   // This function is used to determine if the specified bus command expects read data.  If the
   // input variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for
   // evaluation.  If it is something other than `EVL_Bus_Idle, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command expects read data.
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_has_read_data(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_has_read_data(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   // This function is used to determine if the specified bus command has write data.  If the input
   // variable is `EVL_Bus_Idle, the bus command in this request descriptor is used for evaluation.
   // If it is something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command has write data.
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_has_write_data(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_has_write_data(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_has_write_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_unique_tag
   //
   //
   // Function: bus_cmd_has_write_data
   //
   // This function is used to determine if the specified bus command has a unique bus tag (i.e., if
   // it is illegal to reuse a particular bus tag before a response is sent).  If the input variable
   // is `EVL_Bus_Idle, the bus command in this request descriptor is used for evaluation.  If it is
   // something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in this request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command should use a unique bus tag.
   //
   virtual function bit bus_cmd_has_unique_tag(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_cmd_in == `EVL_Bus_Idle) begin
         return m_bus_parser.bus_cmd_has_unique_tag(m_bus_req_attrs.bus_cmd, m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_cmd_has_unique_tag(bus_cmd_in, m_bus_req_attrs.bus_type);
   endfunction : bus_cmd_has_unique_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   // This function is used to get a name for the specific bus response type.  If the input variable
   // is `EVL_Rsp_Null, the bus response type in this request descriptor is used for evaluation.  If
   // it is something other than `EVL_Rsp_Null, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in this request descriptor is used for evaluation.
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
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_rsp_in == ~evl_bus_rsp_type_t'(0)) begin
         return m_bus_parser.get_bus_rsp_name(get_bus_rsp_type(transfer_idx_in), m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.get_bus_rsp_name(bus_rsp_in, m_bus_req_attrs.bus_type);
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   // This function is used to determine if the specified bus response type has data.  If the input
   // variable is `EVL_Rsp_Null, the bus response type in this request descriptor is used for
   // evaluation.  If it is something other than `EVL_Rsp_Null, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in this request descriptor is used for evaluation.
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
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_rsp_in == ~evl_bus_rsp_type_t'(0)) begin
         return m_bus_parser.bus_rsp_has_data(get_bus_rsp_type(transfer_idx_in), m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_rsp_has_data(bus_rsp_in, m_bus_req_attrs.bus_type);
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   // This function is used to determine if the specified bus response type is an error response.
   // If the input variable is `EVL_Rsp_Null, the bus response type in this request descriptor is
   // used for evaluation.  If it is something other than `EVL_Rsp_Null, the input variable is used
   // for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in this request descriptor is used for evaluation.
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
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      if (bus_rsp_in == ~evl_bus_rsp_type_t'(0)) begin
         return m_bus_parser.bus_rsp_has_error(get_bus_rsp_type(transfer_idx_in), m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.bus_rsp_has_error(bus_rsp_in, m_bus_req_attrs.bus_type);
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to format a request string based on the bus type.
   //
   // Input Variables:
   //
   // style_in:        This is a way to specify the way in which to print the object.
   // transfer_idx_in: This is the transfer number for the request.  For narrower interfaces, it is
   //                  possible that multiple data bursts will be required to complete the request.
   //                  The first data burst is transfer 0, the second is transfer 1, etc.
   //                  Currently, a maximum of 16 transfers is supported.  If this value is less
   //                  than 0, all of the request data will be printed.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the request information.
   //
   virtual function string sprint_obj(input int style_in = 0, input int transfer_idx_in = -1);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.sprint_obj(this, style_in, transfer_idx_in, m_bus_req_attrs.bus_type);
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   // This function is used to format a response string based on the bus type.
   //
   // Input Variables:
   //
   // dword_start: This is the starting doubleword for the response.
   // dword_end:   This is the ending doubleword for the response.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the response information.
   //
   virtual function string sprint_rsp(input int dword_start = 0, input int dword_end = `EVL_LINE_DWORDS - 1);
      if (m_bus_parser == null) begin
         m_bus_parser = evl_bus_parser::get_bus_parser(m_bus_req_attrs.bus_type);
      end
      return m_bus_parser.sprint_rsp(this, dword_start, dword_end, m_bus_req_attrs.bus_type);
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_req_data
   //
   // This function is used to format request data appropriately.  Invalid bytes are indicated with
   // a "--" string.
   //
   // Input Variables:
   //
   // dword_offset_start_in: This is the starting doubleword offset to format.
   // dword_offset_end_in:   This is the ending doubleword offset to format.
   //
   // Return Value:
   //
   // string: This function returns a string specifying the contents of the requst data buffer.
   //
   function string format_req_data(input int dword_offset_start_in = 0, input int dword_offset_end_in = `EVL_LINE_DWORDS - 1);
      evl_line_data req_data = get_req_data(get_line_burst_idx(dword_offset_start_in, `EVL_DWORD_ADDR_LSB));

      if (req_data != null) begin
         string data_str;
         int    dword_offset_start;
         int    dword_offset_end;

         dword_offset_end = dword_offset_end_in & (`EVL_LINE_DWORDS - 1);
         if (dword_offset_start_in < 0) begin
            dword_offset_start = 0;
         end
         else begin
            dword_offset_start = dword_offset_start_in & (`EVL_LINE_DWORDS - 1);
         end
         if (dword_offset_end < dword_offset_start) begin
            dword_offset_end = dword_offset_start;
         end
         data_str = $sformatf("%0s", req_data.format_invalid_and_unknown_by_index(dword_offset_start));
         for (int ii = dword_offset_start + 1; ii <= dword_offset_end; ii++) begin
            data_str = $sformatf("%0s_%0s", req_data.format_invalid_and_unknown_by_index(ii), data_str);
         end
         return data_str;
      end
      return "-- null request data --";
   endfunction : format_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: format_rsp_data
   //
   // This function is used to format response data appropriately.  Invalid bytes are indicated with
   // a "--" string.
   //
   // Input Variables:
   //
   // dword_offset_start_in: This is the starting doubleword offset to format.
   // dword_offset_end_in:   This is the ending doubleword offset to format.
   //
   // Return Value:
   //
   // string: This function returns a string specifying the contents of the response data buffer.
   //
   function string format_rsp_data(input int dword_offset_start_in = 0, input int dword_offset_end_in = `EVL_LINE_DWORDS - 1);
      evl_line_data rsp_data = get_rsp_data(get_line_burst_idx(dword_offset_start_in, `EVL_DWORD_ADDR_LSB));

      if (rsp_data != null) begin
         string data_str;
         int    dword_offset_start;
         int    dword_offset_end;

         dword_offset_end = dword_offset_end_in & (`EVL_LINE_DWORDS - 1);
         if (dword_offset_start_in < 0) begin
            dword_offset_start = 0;
         end
         else begin
            dword_offset_start = dword_offset_start_in & (`EVL_LINE_DWORDS - 1);
         end
         if (dword_offset_end < dword_offset_start) begin
            dword_offset_end = dword_offset_start;
         end
         data_str = $sformatf("%0s", rsp_data.format_invalid_and_unknown_by_index(dword_offset_start));
         for (int ii = dword_offset_start + 1; ii <= dword_offset_end; ii++) begin
            data_str = $sformatf("%0s_%0s", rsp_data.format_invalid_and_unknown_by_index(ii), data_str);
         end
         return data_str;
      end
      return "-- null response data --";
   endfunction : format_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag_id
   //
   // This function is redefined to return the request ID.
   //
   // Return Value:
   //
   // int: This is an integer representing the appropriate tag ID.
   //
   virtual function int get_tag_id();
      if (get_bus_req_id() == ~evl_bus_req_id_t'(0)) begin
         return -1;
      end
      return int'(get_bus_req_id());
   endfunction : get_tag_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_tag_id
   //
   // This function is redefined to return the request ID.
   //
   // Return Value:
   //
   // int: This is an integer representing the appropriate tag ID.
   //
   function int get_req_tag_id();
      evl_bus_req_id_t bus_req_id;

      bus_req_id = get_bus_req_id();
      if (bus_req_id == ~evl_bus_req_id_t'(0)) begin
         return -1;
      end
      if (get_bus_type() == EVL_BUS_TYPE_ETLINK) begin
         return ((int'(get_bus_req_source()) << `EVL_BUS_REQ_ID_WIDTH) | int'(bus_req_id));
      end
      return int'(bus_req_id);
   endfunction : get_req_tag_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_tag_id
   //
   // This function is redefined to return the request ID.
   //
   // Return Value:
   //
   // int: This is an integer representing the appropriate tag ID.
   //
   function int get_rsp_tag_id();
      evl_bus_req_id_t bus_rsp_id;

      bus_rsp_id = get_bus_rsp_id();
      if (bus_rsp_id == ~evl_bus_req_id_t'(0)) begin
         return -1;
      end
      if (get_bus_type() == EVL_BUS_TYPE_ETLINK) begin
         return ((int'(get_bus_rsp_dest()) << `EVL_BUS_REQ_ID_WIDTH) | int'(bus_rsp_id));
      end
      return int'(bus_rsp_id);
   endfunction : get_rsp_tag_id


   //------------------------------------------------------------------------------------------------
   //
   // Function: replicate_req_info
   //
   // This function is used to replicate all pertinent request information from one request
   // deescriptor to another.  Note that the request data handle is copied; what that means is that
   // both the source request descriptor and this request descriptor will point to the same request
   // data object.
   //
   // Input Variables:
   //
   // src_req_desc: This is the request descriptor from which to copy information.
   //
   function void replicate_req_info(input evl_req_desc src_req_desc);
      m_bus_req_attrs = src_req_desc.m_bus_req_attrs;
      if (bus_cmd_has_write_data() == 1'b1) begin
         set_req_data(src_req_desc.get_req_data());
      end
   endfunction : replicate_req_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: replicate_rsp_info
   //
   // This function is used to replicate all pertinent response information from one request
   // deescriptor to another.  Note that the response data handle is copied; what that means is that
   // both the source request descriptor and this request descriptor will point to the same response
   // data object.
   //
   // Input Variables:
   //
   // src_req_desc: This is the request descriptor from which to copy information.
   //
   function void replicate_rsp_info(input evl_req_desc src_req_desc,
                                    input int          line_idx = -1,
                                    input bit          use_size = 1'b0,
                                    input bit          no_data  = 1'b0);
      m_bus_rsp_attrs.delete();
      foreach (src_req_desc.m_bus_rsp_attrs[ii]) begin
         m_bus_rsp_attrs.push_back(src_req_desc.m_bus_rsp_attrs[ii]);
      end
      if ((bus_cmd_has_read_data() == 1'b1) && (no_data == 1'b0)) begin
         void'(src_req_desc.cascade_rsp_data_to_dst(this, line_idx, use_size));
      end
   endfunction : replicate_rsp_info


   //-----------------------------------------------------------------------------------------------
   //
   // Function: copy_sized_rsp_data
   //
   // Input Variables:
   //
   // src_line: This is the line data from which to get information.
   //
   function void copy_sized_rsp_data(input evl_line_data src_line, input int line_idx = 0);
      if (line_idx <= 0) begin
         invalidate_rsp_data();
      end
      if (src_line == null) begin
         return;
      end
      case (1 << get_bus_req_size())
         1:       set_rsp_byte(-1, src_line.get_byte(get_paddr()));
         2:       set_rsp_hword(-1, src_line.get_hword(get_paddr() >> `EVL_HWORD_ADDR_LSB));
         4:       set_rsp_word (-1, src_line.get_word (get_paddr() >> `EVL_WORD_ADDR_LSB));
         8:       set_rsp_dword(-1, src_line.get_dword(get_paddr() >> `EVL_DWORD_ADDR_LSB));
         16:      set_rsp_qword(-1, src_line.get_qword(get_paddr() >> `EVL_QWORD_ADDR_LSB));
         32:      set_rsp_oword(-1, src_line.get_oword(get_paddr() >> `EVL_OWORD_ADDR_LSB));
         default: set_rsp_xword(-1, src_line.get_xword(get_paddr() >> `EVL_XWORD_ADDR_LSB));
      endcase
   endfunction : copy_sized_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_copy
   //
   // This function is a redefinition of the UVM do_copy() function.  do_copy() is called by the
   // UVM clone() function.  Note that not all of the information in the request descriptor is
   // copied with this function; that is intentional ... only limited information is supposed to be
   // copied when cloning a request descriptor.
   //
   // Input Variables:
   //
   // rhs: This is a handle to the request descriptor from which to copy information.
   //
   virtual function void do_copy(uvm_object rhs);
      evl_req_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_bus_req_attrs = source_obj.m_bus_req_attrs;
      m_bus_parser    = source_obj.m_bus_parser;
   endfunction : do_copy


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
   // Task: wait_for_req_data
   //
   // This task is used to wait/block until m_req_data has information and it does not contain null
   // handles.
   //
   // Input Variables:
   //
   //    none
   //
   // Return Value:
   //
   //    none
   //
   task wait_for_req_data();
      #0 wait (m_req_data_avail > 0);
   endtask : wait_for_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_rsp_data
   //
   // This task is used to wait/block until m_rsp_data has information and it does not contain null
   // handles.
   //
   // Input Variables:
   //
   //    none
   //
   // Return Value:
   //
   //    none
   //
   task wait_for_rsp_data();
      #0 wait (m_rsp_data_avail > 0);
   endtask : wait_for_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_req_attrs
   //
   // This function is used to compare this descriptor's attributes to an expected version
   //
   // Input Variables:
   //
   // exp_req_desc:     This is the memory descriptor against which to compare this object's
   //                   attributes.
   //
   // Input Variables:
   //
   // err_messages_out: This array will hold error messages in case there is(are) mismatch(es)
   //
   virtual function bit compare_req_attrs(input  evl_req_desc exp_req_desc,
                                          output string       err_messages_out[]);
      string err_messages[$];

      if (get_bus_cmd() != exp_req_desc.get_bus_cmd()) begin
         err_messages.push_back($sformatf("bus_cmd mismatch: have %0s, expected %0s (%s)", get_bus_cmd_name(), exp_req_desc.get_bus_cmd_name(), sprint_obj()));
      end
      //
      // If there was no prediction for the bus bank, ignore the comparison.
      //
      if ((exp_req_desc.get_bus_bank() != ~evl_sc_bank_id_t'(0)) && (get_bus_bank() != ~evl_sc_bank_id_t'(0)) && (get_bus_bank() != exp_req_desc.get_bus_bank())) begin
         err_messages.push_back($sformatf("bus_bank mismatch: have %0d, expected %0d (%s)", get_bus_bank(), exp_req_desc.get_bus_bank(), sprint_obj()));
      end
      //if (get_bus_req_id() != exp_req_desc.get_bus_req_id()) begin
      //   err_messages.push_back($sformatf("bus_req_id mismatch: have %0p, expected %0p" (%s), get_bus_req_id(), exp_req_desc.get_bus_req_id(), sprint_obj()));
      //end
      if (get_bus_req_size() != exp_req_desc.get_bus_req_size()) begin
         err_messages.push_back($sformatf("bus_req_size mismatch: have %0d, expected %0d (%s)", get_bus_req_size(), exp_req_desc.get_bus_req_size(), sprint_obj()));
      end
      if (get_bus_req_len() != exp_req_desc.get_bus_req_len()) begin
         err_messages.push_back($sformatf("bus_req_len mismatch: have %0d, expected %0d (%s)", get_bus_req_len(), exp_req_desc.get_bus_req_len(), sprint_obj()));
      end
      if (get_bus_lock() != exp_req_desc.get_bus_lock()) begin
         err_messages.push_back($sformatf("bus_lock mismatch: have 0x%0x, expected 0x%0x (%s)", get_bus_lock(), exp_req_desc.get_bus_lock(), sprint_obj()));
      end
      if (get_bus_cache_attr() != exp_req_desc.get_bus_cache_attr()) begin
         err_messages.push_back($sformatf("bus_cache_attr mismatch: have 0x%0x, expected 0x%0x (%s)", get_bus_cache_attr(), exp_req_desc.get_bus_cache_attr(), sprint_obj()));
      end
      if (get_bus_prot() != exp_req_desc.get_bus_prot()) begin
         err_messages.push_back($sformatf("bus_prot mismatch: have 0x%0x, expected 0x%0x (%s)", get_bus_prot(), exp_req_desc.get_bus_prot(), sprint_obj()));
      end
      if ((get_bus_type() == EVL_BUS_TYPE_AXI) && (get_bus_qos() != exp_req_desc.get_bus_qos())) begin
         err_messages.push_back($sformatf("bus_qos mismatch: have 0x%0x, expected 0x%0x (%s)", get_bus_qos(), exp_req_desc.get_bus_qos(), sprint_obj()));
      end
      if (get_bus_region() != exp_req_desc.get_bus_region()) begin
         err_messages.push_back($sformatf("bus_region mismatch: have 0x%0x, expected 0x%0x (%s)", get_bus_region(), exp_req_desc.get_bus_region(), sprint_obj()));
      end
      if (get_bus_req_user() != exp_req_desc.get_bus_req_user()) begin
         err_messages.push_back($sformatf("bus_req_user mismatch: have 0x%0x, expected 0x%0x (%s)", get_bus_req_user(), exp_req_desc.get_bus_req_user(), sprint_obj()));
      end

      if (err_messages.size() > 0) begin
         err_messages_out = new[err_messages.size()];

         foreach(err_messages[ii]) begin
            err_messages_out[ii] = err_messages[ii];
         end
      end

      return (err_messages.size() > 0);
   endfunction : compare_req_attrs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_req_data
   //
   // This function is used to compare request data in this data object to the expected data.
   //
   // Input Variables:
   //
   // exp_req_desc:     This is the request descriptor against which to compare this object's
   //                   request data.
   //
   // Output Variables:
   //
   // err_messages_out: This array will hold error messages in case there are mismatches.
   //
   function bit compare_req_data(input  evl_req_desc exp_req_desc,
                                 output string       err_messages_out[],
                                 input  int          line_idx = -1);
      evl_line_data req_data;

      if (line_idx > m_req_data.size()) begin
         err_messages_out = new[1];
         err_messages_out[0] = $sformatf("compare_req_data(): request data entry %0d is null, cannot compare", line_idx);
         return 1'b1;
      end
      if (line_idx < 0) begin
         bit    status = 1'b0;
         string message_queue[$];

         foreach (m_req_data[ii]) begin
            req_data = get_req_data(ii);
            if (req_data == null) begin
               message_queue.push_back($sformatf("compare_req_data(): request data entry %0d is null, cannot compare", ii));
               status = 1'b1;
            end
            else if (exp_req_desc.get_req_data(ii) == null) begin
               message_queue.push_back($sformatf("compare_req_data(): expected request data entry %0d is null, cannot compare", ii));
               status = 1'b1;
            end
            else begin
               string messages[];

               req_data.compare_data(exp_req_desc.get_req_data(ii), messages);
               if ((messages.size() != 0) || (messages[0] != "")) begin
                  foreach (messages[jj]) begin
                     message_queue.push_back(messages[jj]);
                  end
                  status = 1'b1;
               end
            end
         end
         if (message_queue.size() > 0) begin
            err_messages_out = new[message_queue.size()];
            foreach (message_queue[ii]) begin
               err_messages_out[ii] = message_queue[ii];
            end
         end
         return status;
      end

      req_data = get_req_data(line_idx);
      if (req_data == null) begin
         err_messages_out = new[1];
         err_messages_out[0] = $sformatf("compare_req_data(): request data entry %0d is null, cannot compare", line_idx);
         return 1'b1;
      end
      if (exp_req_desc.get_req_data(line_idx) == null) begin
         err_messages_out = new[1];
         err_messages_out[0] = $sformatf("compare_req_data(): expected request data entry %0d is null, cannot compare", line_idx);
         return 1'b1;
      end
      return req_data.compare_data(exp_req_desc.get_req_data(line_idx), err_messages_out);
   endfunction : compare_req_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_rsp_data
   //
   // This function is used to compare request data in this data object to the expected data.
   //
   // Input Variables:
   //
   // exp_req_desc:     This is the response descriptor against which to compare this object's
   //                   response data.
   //
   // Output Variables:
   //
   // err_messages_out: This array will hold error messages in case there are mismatches.
   //
   function bit compare_rsp_data(input  evl_req_desc exp_req_desc,
                                 output string       err_messages_out[],
                                 input  int          line_idx = -1);
      evl_line_data rsp_data;

      if (line_idx > m_rsp_data.size()) begin
         err_messages_out = new[1];
         err_messages_out[0] = $sformatf("compare_rsp_data(): response data entry %0d is null, cannot compare", line_idx);
         return 1'b1;
      end
      if (line_idx < 0) begin
         bit    status = 1'b0;
         string message_queue[$];

         foreach (m_rsp_data[ii]) begin
            rsp_data = get_rsp_data(ii);
            if (rsp_data == null) begin
               message_queue.push_back($sformatf("compare_rsp_data(): response data entry %0d is null, cannot compare", ii));
               status = 1'b1;
            end
            else if (exp_req_desc.get_rsp_data(ii) == null) begin
               message_queue.push_back($sformatf("compare_rsp_data(): expected response data entry %0d is null, cannot compare", ii));
               status = 1'b1;
            end
            else begin
               string messages[];

               rsp_data.compare_data(exp_req_desc.get_rsp_data(ii), messages);
               if ((messages.size() != 0) || (messages[0] != "")) begin
                  foreach (messages[jj]) begin
                     message_queue.push_back(messages[jj]);
                  end
                  status = 1'b1;
               end
            end
         end
         if (message_queue.size() > 0) begin
            err_messages_out = new[message_queue.size()];
            foreach (message_queue[ii]) begin
               err_messages_out[ii] = message_queue[ii];
            end
         end
         return status;
      end

      rsp_data = get_rsp_data(line_idx);
      if (rsp_data == null) begin
         err_messages_out = new[1];
         err_messages_out[0] = $sformatf("compare_rsp_data(): response data entry %0d is null, cannot compare", line_idx);
         return 1'b1;
      end
      if (exp_req_desc.get_rsp_data(line_idx) == null) begin
         err_messages_out = new[1];
         err_messages_out[0] = $sformatf("compare_rsp_data(): expected response data entry %0d is null, cannot compare", line_idx);
         return 1'b1;
      end
      return rsp_data.compare_data(exp_req_desc.get_rsp_data(line_idx), err_messages_out);
   endfunction : compare_rsp_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_rsp_xword
   //
   // This function is used to compare the specified hex word bytes against this descriptor's
   // response data.
   //
   // Input Variables:
   //
   // xword_byte_en_in: This input specifies the bytes to compare.
   // xword_in:         This is the hex word against which to compare.
   //
   // Return Value:
   //
   // bit: A value of 1'b0 is returned if the comparison is a match.
   //
   function bit compare_rsp_xword(input evl_xword_byte_en_t xword_byte_en_in,
                                  input evl_xword_t         xword_in,
                                  input evl_bus_req_len_t   line_idx = 0);
      evl_line_data rsp_data = get_rsp_data(line_idx);

      if (rsp_data == null) begin
         return 1'b1;
      end
      return rsp_data.compare_xword_against_my_data(xword_byte_en_in, xword_in);
   endfunction : compare_rsp_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_mem_desc", input evl_verif_comp agent_in = null);
      super.new(name_in);
      m_desc_type = EVL_DESC_REQ;
      if (agent_in != null) begin
         m_master_agent = agent_in;
      end
   endfunction : new

endclass : evl_req_desc
