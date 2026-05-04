//--------------------------------------------------------------------------------------------------
//
//
// Class: evl_axi_bus_req
//
// This class object is used to transfer information between the AXI RTL port and UVM monitors and
// drivers.  The actual pin monitor/driver is the evl_axi_interface module that is instantiated in
// the testbench.  The evl_axi_interface module and its companion monitor (and driver) must have a
// handle to the same AXI RTL port object instance.
//
// When a request (or request data) is observed, the evl_axi_interface module sends monitored
// request information to the AXI RTL port by filling out a packed structure and then sending that
// packed structure to the RTL port.  When a particular structure is received by the RTL port, the
// information is transferred to an instance of this class object, and that class object instance is
// then sent to the UVM monitor.
//
// Similarly, a UVM driver sends an instance of this class object to the RTL port, and then that
// information is transferred to a packed structure that is then consumed by the evl_axi_interface
// module.
//
// By dealing only with packed objects in the evl_axi_interface module, it is possible to create
// either synthesizeable modules or DPI calls to be used in an emulation platform while maintaining
// essentially the same evl_axi_interface module code.
//
class evl_axi_bus_req;

   //-----------------------------------------------------------------------------------------------
   //
   // Global Variables
   //
   // m_global_inst_id: This is the global instance ID and can be used to create unique IDs to
   //                   identigy instances of this class object.
   //
   static int m_global_inst_id = 1;


   //-----------------------------------------------------------------------------------------------
   //
   // Variables for Monitoring and Driving
   //
   // m_inst_id:    This is the instance ID for this class object.  Instance IDs of this class
   //               object are intented to be unique.
   //               --------------------------------------------------------------------------------
   // m_trans_id:   This is the EVL transaction ID (it is only used if the Esperanto Verification
   //               Library is fully included).
   //               --------------------------------------------------------------------------------
   // m_req_type:   This is the AXI request type (read or write).
   //               --------------------------------------------------------------------------------
   // m_ax_id:      This is the ID for the AXI request.
   //               --------------------------------------------------------------------------------
   // m_ax_addr:    This is the physical address for the AXI reqest.
   //               --------------------------------------------------------------------------------
   // m_ax_len:     This is the length field for the AXI reqest (number of beats).  Refer to an AXI
   //               spec for more information.
   //               --------------------------------------------------------------------------------
   // m_ax_size:    This is the size field for the AXI request (number of bytes per beat).  Refer to
   //               an AXI spec for more information.
   //               --------------------------------------------------------------------------------
   // m_ax_burst:   This is the burst field for the AXI request.  Refer to an AXI spec for more
   //               information.
   //               --------------------------------------------------------------------------------
   // m_ax_lock:    This is the lock field for the AXI request.  Refer to an AXI spec for more
   //               information.
   //               --------------------------------------------------------------------------------
   // m_ax_cache:   This is the cache field for the AXI request.  Refer to an AXI spec for more
   //               information.
   //               --------------------------------------------------------------------------------
   // m_ax_prot:    This is the protection field for the AXI request.  Refer to an AXI spec for more
   //               information.
   //               --------------------------------------------------------------------------------
   // m_ax_qos:     This is the quality-of-service field for the AXI request.  Refer to an AXI spec
   //               for more information.
   //               --------------------------------------------------------------------------------
   // m_ax_region:  This is the region field for the AXI request.  Refer to an AXI spec for more
   //               information.
   //               --------------------------------------------------------------------------------
   // m_ax_user:    This is the user field for the AXI request.  Refer to an AXI spec for more
   //               information.
   //               --------------------------------------------------------------------------------
   // m_ax_vc:      This is the virtual channel for the AXI request.  A value of all 1s signifies no
   //               virtual channel.
   //               --------------------------------------------------------------------------------
   // m_data_only:  This bit is set to 1'b1 for request-data-only (i.e., W channel) transfers.
   //               --------------------------------------------------------------------------------
   // m_xref_valid: This bit is set to 1'b1 if this class object contains both request information
   //               and request data.
   //               --------------------------------------------------------------------------------
   // m_w_last:     This bit is set to 1'b1 for the last data transfer of a request.
   //               --------------------------------------------------------------------------------
   // m_w_data:     This is the 64-byte data associated with the request.  When sending single
   //               beats, this variable is normalized (i.e., bit 0 is always the least significant
   //               bit of the data beat).  When holding data for a write request (i.e., all data
   //               beats have been received and m_xref_valid is set to 1'b1), the data in this
   //               variable is cache-line aligned (i.e., bit 0 always corresponds to bit 0 of the
   //               cache line).
   //               --------------------------------------------------------------------------------
   // m_w_strb:     This is the 64-bit write mask associated with the request.  When sending single
   //               beats, this variable is normalized (i.e., bit 0 is always the least significant
   //               byte of the data beat).  When holding data for a write request (i.e., all data
   //               beats have been received and m_xref_valid is set to 1'b1), the data in this
   //               variable is cache-line aligned (i.e., bit 0 always corresponds to byte 0 of the
   //               cache line).
   //               --------------------------------------------------------------------------------
   // m_w_user:     This is the user field for the AXI request data transfer (typically parity).
   //
   int                  m_inst_id    = 0;
   evl_trans_id_t       m_trans_id   = evl_trans_id_t'(0);
   evl_axi_req_type_t   m_req_type   = AXI_REQ_WRITE;
   evl_bus_req_id_t     m_ax_id      = evl_bus_req_id_t'(0);
   evl_paddr_t          m_ax_addr    = evl_paddr_t'(0);
   evl_bus_req_len_t    m_ax_len     = evl_bus_req_len_t'(0);
   evl_bus_req_size_t   m_ax_size    = evl_bus_req_size_t'(0);
   evl_bus_req_burst_t  m_ax_burst   = evl_bus_req_burst_t'(0);
   evl_bus_lock_t       m_ax_lock    = evl_bus_lock_t'(0);
   evl_bus_cache_attr_t m_ax_cache   = evl_bus_cache_attr_t'(0);
   evl_bus_prot_t       m_ax_prot    = evl_bus_prot_t'(0);
   evl_bus_qos_t        m_ax_qos     = evl_bus_qos_t'(0);
   evl_bus_region_t     m_ax_region  = evl_bus_region_t'(0);
   evl_bus_req_user_t   m_ax_user    = evl_bus_req_user_t'(0);
   evl_bus_req_vc_t     m_ax_vc      = ~evl_bus_req_vc_t'(0);
   bit                  m_data_only  = 1'b0;
   bit                  m_xref_valid = 1'b0;
   bit                  m_w_last     = 1'b0;
   evl_xword_t          m_w_data     = evl_xword_t'(0);
   evl_xword_byte_en_t  m_w_strb     = evl_xword_byte_en_t'(0);
   evl_bus_req_user_t   m_w_user     = evl_bus_req_user_t'(0);
   int                  m_source     = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Variables for Driving Only
   //
   // m_bus_req_delay:      This is the delay (in cycles) before AW/AR is placed onto the signal
   //                       pins.  This variable is only used for driving requests onto an AXI bus.
   //                       ------------------------------------------------------------------------
   // m_bus_req_data_delay: This is the delay before W is placed onto the signal pins.  This
   //                       variable is only used for driving request data onto an AXI bus.
   //                       ------------------------------------------------------------------------
   // m_cycle:              This is the cycle number in which to drive the request information.
   //                       This variable is only used for driving requests onto an AXI bus.
   //                       ------------------------------------------------------------------------
   // m_data_cycle:         This is the cycle number in which to drive request data.  This variable
   //                       is only used for driving request data onto an AXI bus.
   //
   int                  m_cycle              = 0;
   int                  m_data_cycle         = 0;
   evl_delay_t          m_bus_req_delay      = evl_delay_t'(0);
   evl_delay_t          m_bus_req_data_delay = evl_delay_t'(0);


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_inst_id
   //
   virtual function int get_inst_id();
      return m_inst_id;
   endfunction : get_inst_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_id
   //    This function is used to set the trans_id of this bus request
   //
   // Input Variables:
   //    trans_id_in: This is the trans_id of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_trans_id(input evl_trans_id_t trans_id_in);
      m_trans_id = trans_id_in;
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_id
   //    This function is used to get the trans_id of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_trans_id_t: This is the trans_id of the request
   //
   virtual function evl_trans_id_t get_trans_id();
      return m_trans_id;
   endfunction : get_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_req_type
   //    This function is used to set the request type (read/write) of this bus request
   //
   // Input Variables:
   //    req_type_in: This is the axi request type of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_req_type(input evl_axi_req_type_t req_type_in);
      m_req_type = req_type_in;
   endfunction : set_req_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_req_type
   //    This function is used to get the request type (read/write) of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_axi_req_type_t: This is the axi request type of the request
   //
   virtual function evl_axi_req_type_t get_req_type();
      return m_req_type;
   endfunction : get_req_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_only
   //    This function is used to decide if this object is a data-only
   //    object (i.e. W channel)
   //
   // Input Variables:
   //    data_only_in: true or false
   //
   // Return Value:
   //    none
   //
   virtual function void set_data_only(input bit data_only_in);
      m_data_only = data_only_in;
   endfunction : set_data_only


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_data_only
   //    This function is used to determine if this object is a data-only
   //    object (i.e. W channel)
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    bit: true or false
   //
   virtual function bit get_data_only();
      return m_data_only;
   endfunction : get_data_only


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_delay
   //    This function is used to set the bus request (i.e. AW or AR channel) delay of this request
   //
   // Input Variables:
   //    delay_in: This is the bus delay of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_bus_req_delay(input evl_delay_t delay_in);
      if (delay_in < 0) begin
         m_bus_req_delay = 0;
      end
      else begin
         m_bus_req_delay = delay_in;
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_delay
   //    This function is used to get the bus request (i.e. AW or AR channel) delay of this request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_delay_t: This is the bus delay of the request
   //
   virtual function evl_delay_t get_bus_req_delay();
      return m_bus_req_delay;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_data_delay
   //    This function is used to set the bus delay of the data portion (i.e. W-channel) of this request
   //
   // Input Variables:
   //    delay_in: This is the delay of the data
   //
   // Return Value:
   //    none
   //
   virtual function void set_bus_req_data_delay(input evl_delay_t delay_in);
      if (delay_in < 0) begin
         m_bus_req_data_delay = 0;
      end
      else begin
         m_bus_req_data_delay = delay_in;
      end
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_delay
   //    This function is used to get the bus delay of the data portion (i.e. W-channel) of this request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_delay_t: This is the bus delay of the data
   //
   virtual function evl_delay_t get_bus_req_data_delay();
      return m_bus_req_data_delay;
   endfunction


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_id
   //    This function is used to set the ax_id of this bus request
   //
   // Input Variables:
   //    ax_id_in: This is the ID of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_id(input evl_bus_req_id_t ax_id_in);
      m_ax_id = ax_id_in;
   endfunction : set_ax_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_id
   //    This function is used to get the ax_id of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_id_t: This is the ID of the request
   //
   virtual function evl_bus_req_id_t get_ax_id();
      return m_ax_id;
   endfunction : get_ax_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_addr
   //    This function is used to set the ax_addr of this bus request
   //
   // Input Variables:
   //    ax_addr_in: This is the ADDR of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_addr(input evl_paddr_t ax_addr_in);
      m_ax_addr = ax_addr_in;
   endfunction : set_ax_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_addr
   //    This function is used to get the ax_addr of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_paddr_t: This is the ADDR of the request
   //
   virtual function evl_paddr_t get_ax_addr();
      return m_ax_addr;
   endfunction : get_ax_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_len
   //    This function is used to set the ax_len of this bus request
   //
   // Input Variables:
   //    ax_len_in: This is the LEN of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_len(input evl_bus_req_len_t ax_len_in);
      m_ax_len = ax_len_in;
   endfunction : set_ax_len


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_len
   //    This function is used to get the ax_len of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_len_t: This is the LEN of the request
   //
   virtual function evl_bus_req_len_t get_ax_len();
      return m_ax_len;
   endfunction : get_ax_len


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_size
   //    This function is used to set the ax_size of this bus request
   //
   // Input Variables:
   //    ax_size_in: This is the SIZE of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_size(input evl_bus_req_size_t ax_size_in);
      m_ax_size = ax_size_in;
   endfunction : set_ax_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_size
   //    This function is used to get the ax_size of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_size_t: This is the SIZE of the request
   //
   virtual function evl_bus_req_size_t get_ax_size();
      return m_ax_size;
   endfunction : get_ax_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_burst
   //    This function is used to set the ax_burst of this bus request
   //
   // Input Variables:
   //    ax_burst_in: This is the BURST of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_burst(input evl_bus_req_burst_t ax_burst_in);
      m_ax_burst = ax_burst_in;
   endfunction : set_ax_burst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_burst
   //    This function is used to get the ax_burst of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_burst_t: This is the BURST of the request
   //
   virtual function evl_bus_req_burst_t get_ax_burst();
      return m_ax_burst;
   endfunction : get_ax_burst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_lock
   //    This function is used to set the ax_lock of this bus request
   //
   // Input Variables:
   //    ax_lock_in: This is the LOCK of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_lock(input evl_bus_lock_t ax_lock_in);
      m_ax_lock = ax_lock_in;
   endfunction : set_ax_lock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_lock
   //    This function is used to get the ax_lock of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_lock_t: This is the LOCK of the request
   //
   virtual function evl_bus_lock_t get_ax_lock();
      return m_ax_lock;
   endfunction : get_ax_lock


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_cache
   //    This function is used to set the ax_cache of this bus request
   //
   // Input Variables:
   //    ax_cache_in: This is the CACHE of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_cache(input evl_bus_cache_attr_t ax_cache_in);
      m_ax_cache = ax_cache_in;
   endfunction : set_ax_cache


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_cache
   //    This function is used to get the ax_cache of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_cache_attr_t: This is the CACHE of the request
   //
   virtual function evl_bus_cache_attr_t get_ax_cache();
      return m_ax_cache;
   endfunction : get_ax_cache


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_prot
   //    This function is used to set the ax_prot of this bus request
   //
   // Input Variables:
   //    ax_prot_in: This is the PROT of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_prot(input evl_bus_prot_t ax_prot_in);
      m_ax_prot = ax_prot_in;
   endfunction : set_ax_prot


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_prot
   //    This function is used to get the ax_prot of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_prot_t: This is the PROT of the request
   //
   virtual function evl_bus_prot_t get_ax_prot();
      return m_ax_prot;
   endfunction : get_ax_prot


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_qos
   //    This function is used to set the ax_qos of this bus request
   //
   // Input Variables:
   //    ax_qos_in: This is the QOS of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_qos(input evl_bus_qos_t ax_qos_in);
      m_ax_qos = ax_qos_in;
   endfunction : set_ax_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_qos
   //    This function is used to get the ax_qos of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_qos_t: This is the QOS of the request
   //
   virtual function evl_bus_qos_t get_ax_qos();
      return m_ax_qos;
   endfunction : get_ax_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_region
   //    This function is used to set the ax_region of this bus request
   //
   // Input Variables:
   //    ax_region_in: This is the REGION of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_region(input evl_bus_region_t ax_region_in);
      m_ax_region = ax_region_in;
   endfunction : set_ax_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_region
   //    This function is used to get the ax_region of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_region_t: This is the REGION of the request
   //
   virtual function evl_bus_region_t get_ax_region();
      return m_ax_region;
   endfunction : get_ax_region


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_user
   //    This function is used to set the ax_user of this bus request
   //
   // Input Variables:
   //    ax_user_in: This is the USER of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_user(input evl_bus_req_user_t ax_user_in);
      m_ax_user = ax_user_in;
   endfunction : set_ax_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_user
   //    This function is used to get the ax_user of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_user_t: This is the USER of the request
   //
   virtual function evl_bus_req_user_t get_ax_user();
      return m_ax_user;
   endfunction : get_ax_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ax_vc
   //    This function is used to set the ax_vc field of this bus request
   //
   // Input Variables:
   //    ax_vc_in: This is the virtual channel for this request
   //
   // Return Value:
   //    none
   //
   virtual function void set_ax_vc(input evl_bus_req_vc_t ax_vc_in);
      m_ax_vc = ax_vc_in;
   endfunction : set_ax_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ax_vc
   //    This function is used to get the ax_vc field of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_vc_t: This is the virtual channel for this request
   //
   virtual function evl_bus_req_vc_t get_ax_vc();
      return m_ax_vc;
   endfunction : get_ax_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_w_data
   //    This function is used to set the w_data of this bus request
   //
   // Input Variables:
   //    w_data_in: This is the DATA of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_w_data(input evl_xword_t w_data_in);
      m_w_data = w_data_in;
   endfunction : set_w_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_w_data
   //    This function is used to get the w_data of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_xword_t: This is the DATA of the request
   //
   virtual function evl_xword_t get_w_data();
      return m_w_data;
   endfunction : get_w_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_w_strb
   //    This function is used to set the w_strb of this bus request
   //
   // Input Variables:
   //    w_strb_in: This is the STRB of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_w_strb(input evl_xword_byte_en_t w_strb_in);
      m_w_strb = w_strb_in;
   endfunction : set_w_strb


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_w_strb
   //    This function is used to get the w_strb of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_xword_byte_en_t: This is the STRB of the request
   //
   virtual function evl_xword_byte_en_t get_w_strb();
      return m_w_strb;
   endfunction : get_w_strb


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_w_last
   //    This function is used to BLAH.
   //
   // Return Value:
   //    none
   //
   virtual function void set_w_last();
      m_w_last = 1'b1;
   endfunction : set_w_last


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_w_last
   //    This function is used to BLAH.
   //
   // Return Value:
   //    bit: BLAH
   //
   virtual function bit get_w_last();
      return m_w_last;
   endfunction : get_w_last


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_w_user
   //    This function is used to set the w_user of this bus request
   //
   // Input Variables:
   //     w_user_in: This is the W_USER of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_w_user(input evl_bus_req_user_t w_user_in);
      m_w_user = w_user_in;
   endfunction : set_w_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_w_user
   //    This function is used to get the w_user of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_bus_req_user_t: This is the W_USER of the request
   //
   virtual function evl_bus_req_user_t get_w_user();
      return m_w_user;
   endfunction : get_w_user


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_source
   //    This function is used to set the source port of the bus request.
   //
   // Input Variables:
   //    source_in: This is the source port of the request.
   //
   // Return Value:
   //    none
   //
   virtual function void set_source(input int source_in);
      m_source = source_in;
   endfunction : set_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_source
   //    This function is used to get the source port of the bus request.
   //
   // Return Value:
   //    int: This is the source port of the request.
   //
   virtual function int get_source();
      return m_source;
   endfunction : get_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cycle
   //    This function is used to set the cycle of this bus request
   //
   // Input Variables:
   //     cycle_in: This is the cycle of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_cycle(input int cycle_in);
      m_cycle = cycle_in;
   endfunction : set_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cycle
   //    This function is used to get the cycle of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    int: This is the cycle of the request
   //
   virtual function int get_cycle();
      return m_cycle;
   endfunction : get_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_cycle
   //    This function is used to set the cycle of this bus request
   //
   // Input Variables:
   //     cycle_in: This is the cycle of the request
   //
   // Return Value:
   //    none
   //
   virtual function void set_data_cycle(input int cycle_in);
      m_data_cycle = cycle_in;
   endfunction : set_data_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_data_cycle
   //    This function is used to get the cycle of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    int: This is the cycle of the request
   //
   virtual function int get_data_cycle();
      return m_data_cycle;
   endfunction : get_data_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_offset
   //    This function is used to get the address offset (bits 5:0) of this bus request
   //
   // Input Variables:
   //    none
   //
   // Return Value:
   //    evl_paddr_t: This is the ADDR of the request
   //
   virtual function bit [5:0] get_addr_offset();
      return m_ax_addr[5:0];
   endfunction : get_addr_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new();
      m_inst_id = m_global_inst_id++;
   endfunction : new

endclass : evl_axi_bus_req
