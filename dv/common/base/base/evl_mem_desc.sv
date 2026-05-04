//--------------------------------------------------------------------------------------------------
//
// Class: evl_mem_desc
//
// This class is the base class of all memory descriptor objects (transaction descriptors and
// request descriptors).  Members and methods are, for the part, common and used by both transaction
// descriptors and request descriptors.  The reason evl_mem_desc exists (apart from some shared
// members and methods) is to allow a generic cache component (and its line and way descriptors) to
// be memory-descriptor agnostic.  For an L1 cache in a CPU model, for instance, the object creating
// and using the cache component and its members will typically be a transaction descriptor.  For L2
// and L3 caches in higher-level models, the object creating and using the cache component and its
// members will be a request descriptor.
//
// The packed struct evl_mem_desc_s is used to store commonly-copied members when creating a request
// descriptor from either a parent transaction descriptor or parent request descriptor.  It
// simplifies the do_copy() code a bit.
//
virtual class evl_mem_desc extends evl_desc_seq_item;

   //-----------------------------------------------------------------------------------------------
   //
   // Local Type Definitions
   //
   // The structure defined below is used to simplify copying information when spawning one
   // descriptor from another.  It contains the following fields:
   //
   // trid:              This is the transaction ID for this transaction thread.
   //                    ---------------------------------------------------------------------------
   // thread_id:         This is the thread ID of this descriptor.
   //                    ---------------------------------------------------------------------------
   // core_id:           This is the core ID of the component generating this descriptor.
   //                    ---------------------------------------------------------------------------
   // neighborhood_id:   This is the neighborhood ID of the component generating this descriptor.
   //                    ---------------------------------------------------------------------------
   // shire_id:          This is the shire ID of the component generating this descriptor.
   //                    ---------------------------------------------------------------------------
   // cmd_length:        This is the length (in bytes) of the command.
   //                    ---------------------------------------------------------------------------
   // pc_vaddr:          This is the virtual PC address of the command.
   //                    ---------------------------------------------------------------------------
   // pc_vaddr_valid:    This bit is set to 1'b1 when a virtual PC address is assigned.
   //                    ---------------------------------------------------------------------------
   // vaddr:             This is the virtual address of the command.
   //                    ---------------------------------------------------------------------------
   // vaddr_valid:       This bit is set to 1'b1 when a virtual address is assigned.
   //                    ---------------------------------------------------------------------------
   // paddr:             This is the physical address of the command.
   //                    ---------------------------------------------------------------------------
   // paddr_valid:       This bit is set to 1'b1 when a physical address is assigned.
   //                    ---------------------------------------------------------------------------
   // cached:            If this bit is 1'b1, the memory operation is cached.
   //                    ---------------------------------------------------------------------------
   // addr_sp:           This is the abstract address space for the command.
   //                    ---------------------------------------------------------------------------
   // req_source:        This is the source for the operation.  The ET link allows us to specify
   //                    the ultimate source of a request.
   //                    ---------------------------------------------------------------------------
   // trans_attr:        This structure is used for oddball operations (which require information
   //                    in the data field).  It contains the following fields:
   //                    ---------------------------------------------------------------------------
   //   scp_fill_addr:   This is the fill address for L2 scratchpad fill cache ops.
   //                    ---------------------------------------------------------------------------
   //   cop_start_level: This is the starting cache level for flushes and evicts.
   //                    ---------------------------------------------------------------------------
   //   cop_dest_level:  This is the destination level for flushes, evicts, and prefetches.
   //                    ---------------------------------------------------------------------------
   //   cop_final_state: This is the final state for unlock cache ops.
   //                    ---------------------------------------------------------------------------
   //   src_shire_id:    This is the source shire ID for AXI atomic requests.
   //                    ---------------------------------------------------------------------------
   //   atomic_conf:     This is the atomic operation to perform.
   //                    ---------------------------------------------------------------------------
   //   atomic_transid:  This is the transaction ID for atomic operations.
   //                    ---------------------------------------------------------------------------
   //   atomic_offset:   This is the line offset address for atomic operations.
   //                    ---------------------------------------------------------------------------
   //   subopcode_dest:  This is the destination for partial load/store support.
   //                    ---------------------------------------------------------------------------
   //   message_conf:    This is the message information.
   //                    ---------------------------------------------------------------------------
   //   coop_mask:       This is the n-hot minion bit mask for cooperative loads.
   //                    ---------------------------------------------------------------------------
   //   coop_id:         This is the cooperative id for cooperative loads.
   //                    ---------------------------------------------------------------------------
   // qos:               This is the QOS field on the AXI bus.
   //                    ---------------------------------------------------------------------------
   // vc:                This is the virtual channel for an AXI request.
   //
   typedef struct packed unsigned {
         evl_trans_id_t        trid;
         evl_thread_id_t       thread_id;
         evl_core_id_t         core_id;
         evl_neighborhood_id_t neighborhood_id;
         evl_shire_id_t        shire_id;
         evl_cmd_length_t      cmd_length;
         evl_vaddr_t           pc_vaddr;
         bit                   pc_vaddr_valid;
         evl_vaddr_t           vaddr;
         bit                   vaddr_valid;
         evl_paddr_t           paddr;
         bit                   paddr_valid;
         bit                   cached;
         evl_addr_sp_t         addr_sp;
         evl_bus_req_source_t  req_source;
         evl_trans_attr_s      trans_attr;
         evl_bus_qos_t         qos;
         evl_bus_req_vc_t      vc;
      } evl_mem_desc_s;


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // m_default_timeout: This is the default transaction timeout value (in cycles).
   // m_get_stats:       This is an indicator to gather descriptor statistics.
   //
   static int m_default_timeout = (`et_value_plusargs("trans_timeout=%d", m_default_timeout) != 0) ? m_default_timeout : 40000;
   static bit m_get_stats       = (`et_test_plusargs("gatherDescStats") == 0) ? 1'b0 : 1'b1;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_mem_desc_attrs:      This packed array contains commonly-copied variables when spawning a
   //                        request descriptor from either a transaction descriptor or another
   //                        request descriptor.
   //                        -----------------------------------------------------------------------
   // m_bus_child:           This variable is intended to be set if the descriptor is a bus VC
   //                        child.  The way EVL bus components work is that an observed descriptor
   //                        is created from bus information; this descriptor is then linked as a
   //                        child of the expected descriptor for the bus.
   //                        -----------------------------------------------------------------------
   // m_issued:              This variable is intended to reflect the issue state of a descriptor;
   //                        it is context-dependent and may be meaningless.  For request
   //                        descriptors, m_issued is intended to be set when the request in
   //                        question has been observed on the bus.
   //                        -----------------------------------------------------------------------
   // m_processed:           This variable is intended to reflect the issue state of a descriptor;
   //                        it is context-dependent and may be meaningless.  For request
   //                        descriptors, m_processed is intended to be set when the request in
   //                        question has been first processed by the slave DUT.
   //                        -----------------------------------------------------------------------
   // m_done:                This variable is intended to reflect the completion state of either a
   //                        transaction descriptor or a request descriptor.
   //                        -----------------------------------------------------------------------
   // m_root_desc:           This is the root descriptor for the entire transaction thread.  For
   //                        now, a root descriptor will be a transaction descriptor.  Assuming the
   //                        library evolves to include instruction descriptors, a root descriptor
   //                        could some day be an instruction descriptor (and this member object
   //                        will need to be moved into a parent class).
   //                        -----------------------------------------------------------------------
   // m_ancestor_desc:       This is a handle to the top-level transaction descriptor for this
   //                        descriptor thread.  It is possible that this is the same as
   //                        m_root_desc, but that is not necessarily the case ... an ancestor
   //                        descriptor can only have request descriptors as children, but a root
   //                        descriptor can have both transaction descripts and request descriptors
   //                        as children.
   //                        -----------------------------------------------------------------------
   // m_parent_desc:         This is a handle to the descriptor that spawned this descriptor.
   //                        -----------------------------------------------------------------------
   // m_line_desc:           This is a handle to the cache line descriptor for this memory
   //                        descriptor.  For a component model with a cache (or memory), this will
   //                        be a handle to the line descriptor for the cache/memory line at the
   //                        specified physical address.
   //                        -----------------------------------------------------------------------
   // m_template_data:       This is a handle to a line data template to be used for any data
   //                        objects created by this memory descriptor.
   //                        -----------------------------------------------------------------------
   // m_template_line:       This is a handle to a line descriptor template to be used for any line
   //                        descriptors created by this memory descriptor.
   //                        -----------------------------------------------------------------------
   // m_rand_gen:            This is a handle to a common random generator that can be used to
   //                        generate random data, etc.  For stimulus repeatability (and uniqueness
   //                        of data from memory descriptors), this is typically set to a random
   //                        generator in the component that creates this descriptor.
   //                        -----------------------------------------------------------------------
   // m_delay_wrapper:       This is an object that contains one or more delays that can be
   //                        referenced with delay keys.  Delay keys are basically identical to
   //                        integer and object parameter keys (i.e., they are set by `evl_param_key
   //                        macro calls).  Delays from a stimulus agent are passed to the driver
   //                        with this object.  Because both transaction and request descriptors can
   //                        have a delay wrapper, and because all spawned request descriptors point
   //                        back to the original transaction descriptor, it is possible for an
   //                        initial transaction to specify all delays to be used at slave objects.
   //                        -----------------------------------------------------------------------
   // m_rsp_cycle:           This is the cycle in which a response was set for this descriptor.
   //                        -----------------------------------------------------------------------
   // m_timeout_value:       This is the current state of the timeout counter.
   //                        -----------------------------------------------------------------------
   // m_child_desc_queues[]: This associative array is used to track all children of this memory
   //                        descriptor.  This array is an array of queues.  The array is indexed by
   //                        the type of descriptor being queued.  Thus, it is possible to have a
   //                        queue of transaction descriptors (EVL_DESC_TRANS) and another queue of
   //                        request descriptors (EVL_DESC_REQ).
   //                        -----------------------------------------------------------------------
   // m_depend_queue:        This queue is used to track all descriptors that are dependencies of
   //                        this descriptor.  A common use would be to ensure that this descriptor
   //                        is not "processed" before all its dependencies are done.
   //
   //
   // The diagram below illustrates a transaction thread:
   //
   //    Desc #1
   //    +=========================+<-------------+------------------------------------+
   //    |                         |              |                                    |
   //    | m_parent_desc --------->|              |                                    |
   //    | m_root_desc ----------->|              |                                    |
   //    | m_child_desc_queues[] ->|-----+        |                                    |
   //    |                       ->|--+  |        |                                    |
   //    +=========================+  |  |        |                                    |
   //                                 |  |        |                                    |
   //                                 |  |        |                                    |
   //      +--------------------------+  |        |                                    |
   //      |                             |        |                                    |
   //      |  +--------------------------+        |                                    |
   //      |  |                                   |                                    |
   //      |  |    Desc #2                        |                                    |
   //      |  +--> +=========================+ <--|---------------------------------+  |
   //      |       |                         |    |                                 |  |
   //      |       | m_parent_desc --+------>|----+                                 |  |
   //      |       | m_root_desc ----+       |    |    Desc #4                      |  |
   //      |       | m_child_desc_queues[] ->|-------> +=========================+  |  |
   //      |       +=========================+    |    |                         |  |  |
   //      |                                      |    | m_parent_desc --------->|--+  |
   //      |                                      |    | m_root_desc ----------->|-----+
   //      |                                      |    | m_child_desc_queues[] ->|     |
   //      |                                      |    +=========================+     |
   //      |                                      |                                    |
   //      |       Desc #3                        |                                    |
   //      +-----> +=========================+ <--|---------------------------------+  |
   //              |                         |    |                                 |  |
   //              | m_parent_desc --+------>|----+                                 |  |
   //              | m_root_desc ----+       |         Desc #5                      |  |
   //              | m_child_desc_queues[] ->|-------> +=========================+  |  |
   //              +=========================+         |                         |  |  |
   //                                                  | m_parent_desc --------->|--+  |
   //                                                  | m_root_desc ----------->|-----+
   //                                                  | m_child_desc_queues[] ->|
   //                                                  +=========================+
   //
   // - Desc #1 is the root of the transaction thread.  It has spawned two childrean
   //   (Desc #2 and Desc #3).  Desc #1 has handles to both of its children.
   // - Both the root and parent descriptor handles for Desc #2 point to Desc #1.
   // - Both the root and parent descriptor handles for Desc #3 point to Desc #1.
   // - Desc #2 has spawned a single child (Desc #4).  Desc #2 has a handle to its child.
   // - The root descriptor for Desc #4 is Desc #1, and its parent descriptor is Desc #2.
   // - Desc #3 has spawned a single child (Desc #5).  Desc #3 has a handle to its child.
   // - The root descriptor for Desc #5 is Desc #1, and its parent descriptor is Desc #3.
   //
   evl_mem_desc_s     m_mem_desc_attrs = '{ trid:            0,
                                            thread_id:       ~evl_thread_id_t'(0),
                                            core_id:         ~evl_core_id_t'(0),
                                            neighborhood_id: ~evl_neighborhood_id_t'(0),
                                            shire_id:        ~evl_shire_id_t'(0),
                                            cmd_length:      0,
                                            pc_vaddr:        0,
                                            pc_vaddr_valid:  1'b0,
                                            vaddr:           0,
                                            vaddr_valid:     1'b0,
                                            paddr:           0,
                                            paddr_valid:     1'b0,
                                            cached:          1'b0,
                                            addr_sp:         Mem,
                                            req_source:      evl_bus_req_source_t'(0),
                                            trans_attr:      evl_trans_attr_s'(0),
                                            qos:             evl_bus_qos_t'(0),
                                            vc:              ~evl_bus_req_vc_t'(0)
                                          };
   bit                m_created_by_bus = 1'b0;
   bit                m_bus_child      = 1'b0;
   bit                m_issued         = 1'b0;
   bit                m_processed      = 1'b0;
   bit                m_done           = 1'b0;
   evl_mem_desc       m_root_desc      = null;
   evl_mem_desc       m_parent_desc    = null;
   evl_mem_desc       m_ancestor_desc  = null;
   evl_line_desc      m_line_desc      = null;
   evl_line_data      m_template_data  = null;
   evl_line_desc      m_template_line  = null;
   evl_rand_gen       m_rand_gen       = null;
   evl_delay_wrapper  m_delay_wrapper  = null;
   int unsigned       m_rsp_cycle      = 32'h7fffffff;
   realtime           m_create_time    = 0;
   realtime           m_issue_time     = 0;
   realtime           m_process_time   = 0;
   realtime           m_done_time      = 0;
   int                m_timeout_value;
   evl_mem_desc_queue m_child_desc_queues[evl_desc_type_t];
   evl_mem_desc       m_depend_queue[$];
   evl_trans_desc     m_killed_trans_descs[$];
   evl_req_desc       m_killed_req_descs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_id
   //
   // This function is used to set the transaction ID for a transaction thread.  Transaction IDs
   // have the following format:
   //
   //     +----------+----------+---------+-----------------------------+
   //     |   8-bit  |  4-bit   |  4-bit  |                             |
   //     | shire id | neigh id | core id | 24-bit temporally-unique id |
   //     +----------+----------+---------+-----------------------------+
   //
   // The 24-bit unique ID is tracked on a per-{shire, neighborhood, core} basis, so multiple
   // neighborhoods may have the same 24-bit value, but the entire transaction ID is unique because
   // {shire, neighborhood, core} is included.
   //
   // The intention is that this function should be called (using the default input value) when a
   // new transaction descriptor is first created; the same transaction ID will be used for all
   // request descriptors spawned from the original transaction descriptor (and any of their
   // children, grandchildren, etc.).
   //
   // When using the `evl_get_next_trid() macro to generate the TrID, the 24-bit value will never be
   // all 0s or all 1s; this prevents a TrID of all 0s or all 1s from being a valid TrID.
   //
   // Input Variables:
   //
   // trans_id_in: This is the transaction ID to use for setting the TrID.  If it is 0, a unique
   //              TrID will be generated using the `evl_get_next_trid() macro.
   //
   function void set_trans_id(input evl_trans_id_t trans_id_in = evl_trans_id_t'(0));
      if (trans_id_in != evl_trans_id_t'(0)) begin
         m_mem_desc_attrs.trid = trans_id_in;
      end
      else if (m_mem_desc_attrs.trid == evl_trans_id_t'(0)) begin
         m_mem_desc_attrs.trid = `evl_get_next_trid(m_mem_desc_attrs.shire_id, m_mem_desc_attrs.neighborhood_id, m_mem_desc_attrs.core_id);
      end
   endfunction : set_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_trans_id
   //
   // This function is used to return the transaction ID for this descriptor.
   //
   // Return Value:
   //
   // evl_trans_id_t: This is the transaction descriptor for this descriptor.
   //
   function evl_trans_id_t get_trans_id();
      return m_mem_desc_attrs.trid;
   endfunction : get_trans_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_shire_id
   //
   // This function is used to set the shire ID for this descriptor.  The shire ID should be set to
   // the shire ID of the component that created this descriptor.  If the component has no shire ID,
   // a value of ~evl_shire_id_t'(0) should be used (which is the default setting upon creation).
   //
   // Input Variables:
   //
   // shire_id_in: This is value to which to set the shire ID.
   //
   function void set_shire_id(input evl_shire_id_t shire_id_in);
      m_mem_desc_attrs.shire_id = shire_id_in;
   endfunction : set_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_shire_id
   //
   // This function is used to return the shire ID for this descriptor.
   //
   // Return Value:
   //
   // evl_shire_id_t: This is the shire ID for this descriptor.
   //
   function evl_shire_id_t get_shire_id();
      return m_mem_desc_attrs.shire_id;
   endfunction : get_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_neighborhood_id
   //
   // This function is used to set the neighborhood ID for this descriptor.  The neighborhood ID
   // should be set to the neighborhood ID of the component that created this descriptor.  If the
   // component has no neighborhood ID, a value of ~evl_neighborhood_id_t'(0) should be used (which
   // is the default setting upon creation).
   //
   // Input Variables:
   //
   // neighborhood_id_in: This is value to which to set the neighborhood ID.
   //
   function void set_neighborhood_id(input evl_neighborhood_id_t neighborhood_id_in);
      m_mem_desc_attrs.neighborhood_id = neighborhood_id_in;
   endfunction : set_neighborhood_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_neighborhood_id
   //
   // This function is used to return the neighborhood ID for this descriptor.
   //
   // Return Value:
   //
   // evl_neighborhood_id_t: This is the neighborhood ID for this descriptor.
   //
   function evl_neighborhood_id_t get_neighborhood_id();
      return m_mem_desc_attrs.neighborhood_id;
   endfunction : get_neighborhood_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_core_id
   //
   // This function is used to set the core ID for this descriptor.  The core ID should be set
   // to the core ID of the component that created this descriptor.  If the component has no
   // core ID, a value of ~evl_core_id_t'(0) should be used (which is the default setting upon
   // creation).
   //
   // Input Variables:
   //
   // core_id_in: This is value to which to set the core ID.
   //
   function void set_core_id(input evl_core_id_t core_id_in);
      m_mem_desc_attrs.core_id = core_id_in;
   endfunction : set_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_core_id
   //
   // This function is used to return the core ID for this descriptor.
   //
   // Return Value:
   //
   // evl_core_id_t: This is the core ID for this descriptor.
   //
   function evl_core_id_t get_core_id();
      return m_mem_desc_attrs.core_id;
   endfunction : get_core_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_thread_id
   //
   // This function is used to set the thread ID for this descriptor.  The thread ID should be set
   // to the thread ID of the component that created this descriptor.  If the component has no
   // thread ID, a value of ~evl_thread_id_t'(0) should be used (which is the default setting upon
   // creation).
   //
   // Input Variables:
   //
   // thread_id_in: This is value to which to set the thread ID.
   //
   function void set_thread_id(input evl_thread_id_t thread_id_in);
      m_mem_desc_attrs.thread_id = thread_id_in;
   endfunction : set_thread_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_thread_id
   //
   // This function is used to return the thread ID for this descriptor.
   //
   // Return Value:
   //
   // evl_thread_id_t: This is the thread ID for this descriptor.
   //
   function evl_thread_id_t get_thread_id();
      return m_mem_desc_attrs.thread_id;
   endfunction : get_thread_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_data_template
   //
   // This function is used to set the data template handle to be used for all data objects created
   // by this descriptor.  The notion here is that some descriptors need to create data with no
   // parity, some may need to create data with parity, and others may need to create data with some
   // form of ECC.
   //
   // Input Variables:
   //
   // line_data_in: This is the value to which to set the data template.
   //
   function void set_data_template(input evl_line_data line_data_in);
      m_template_data = line_data_in;
   endfunction : set_data_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_data_template
   //
   // This function is used to return a handle to the data template for this descriptor.
   //
   // Return Value:
   //
   // evl_line_data: This is the handle to the data template for this descriptor.
   //
   function evl_line_data get_data_template();
      return m_template_data;
   endfunction : get_data_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_line_template
   //
   // This function is used to set the line descriptor template handle to be used for all data
   // objects created by this descriptor.
   //
   // Input Variables:
   //
   // line_desc_in: This is the value to which to set the line descriptor template.
   //
   function void set_line_template(input evl_line_desc line_desc_in);
      m_template_line = line_desc_in;
   endfunction : set_line_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_template
   //
   // This function is used to return a handle to the line descriptor template for this descriptor.
   //
   // Return Value:
   //
   // evl_line_desc: This is the handle to the line descriptor template for this descriptor.
   //
   function evl_line_desc get_line_template();
      return m_template_line;
   endfunction : get_line_template


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rand_gen
   //
   // This function is used to set the random generator handle for this descriptor.
   //
   // Input Variables:
   //
   // rand_gen_in: This is the value to which to set the random generator handle.
   //
   function void set_rand_gen(input evl_rand_gen rand_gen_in);
      m_rand_gen = rand_gen_in;
   endfunction : set_rand_gen


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rand_gen
   //
   // This function is used to return a handle to the random generator for this descriptor.
   //
   // Return Value:
   //
   // evl_rand_gen: This is the handle to the random generator for this descriptor.
   //
   function evl_rand_gen get_rand_gen();
      return m_rand_gen;
   endfunction : get_rand_gen


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cmd
   //
   // This function is used to set the abstract transaction command for this descriptor.  This
   // virtual function is redefined in both the transaction and request descriptors.
   //
   // Input Variables:
   //
   // cmd_in: This is the value to which to set the transaction command.
   //
   virtual function void set_cmd(input evl_cmd_t cmd_in);
      return;
   endfunction : set_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cmd
   //
   // This function is used to return the transaction command for this transaction thread.  This
   // virtual function is redefined in both the transaction and request descriptors.
   //
   // Return Value:
   //
   // evl_cmd_t: This is the abstract transaction command for this transaction thread.
   //
   virtual function evl_cmd_t get_cmd();
      return NOCMD;
   endfunction : get_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cmd_length
   //
   // This function is used to set the transaction command length for this descriptor.  Note that in
   // all cases this is intended to be the command length (in bytes) of the originator of the
   // transaction thread.
   //
   // Input Variables:
   //
   // cmd_length_in: This is the value to which to set the command length.
   //
   function void set_cmd_length(input evl_cmd_length_t cmd_length_in);
      m_mem_desc_attrs.cmd_length = cmd_length_in;
   endfunction : set_cmd_length


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cmd_length
   //
   // This function is used to return the transaction command length for this descriptor.
   //
   // Return Value:
   //
   // evl_cmd_length_t: This is the transaction command length for this descriptor.
   //
   function evl_cmd_length_t get_cmd_length();
      return m_mem_desc_attrs.cmd_length;
   endfunction : get_cmd_length


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_pc_vaddr
   //
   // This function is used to set the virtual address for this descriptor.
   //
   // Input Variables:
   //
   // vaddr_in: This is the value to which to set the PC virtual address.
   //
   function void set_pc_vaddr(input evl_vaddr_t vaddr_in);
      m_mem_desc_attrs.pc_vaddr       = vaddr_in;
      m_mem_desc_attrs.pc_vaddr_valid = 1'b1;
   endfunction : set_pc_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_pc_vaddr
   //
   // This function is used to return the virtual PC address for this descriptor.
   //
   // Return Value:
   //
   // evl_vaddr_t: This is the virtual PC address for this descriptor.
   //
   function evl_vaddr_t get_pc_vaddr();
      return m_mem_desc_attrs.pc_vaddr;
   endfunction : get_pc_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pc_vaddr_is_valid
   //
   // This function returns 1 if the virtual PC address of this descriptor is valid.
   //
   function bit pc_vaddr_is_valid();
      return m_mem_desc_attrs.pc_vaddr_valid;
   endfunction : pc_vaddr_is_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_vaddr
   //
   // This function is used to set the virtual address for this descriptor.
   //
   // Input Variables:
   //
   // vaddr_in: This is the value to which to set the virtual address.
   //
   function void set_vaddr(input evl_vaddr_t vaddr_in);
      m_mem_desc_attrs.vaddr       = vaddr_in;
      m_mem_desc_attrs.vaddr_valid = 1'b1;
   endfunction : set_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_vaddr
   //
   // This function is used to return the virtual address for this descriptor.
   //
   // Return Value:
   //
   // evl_vaddr_t: This is the virtual address for this descriptor.
   //
   function evl_vaddr_t get_vaddr();
      return m_mem_desc_attrs.vaddr;
   endfunction : get_vaddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: vaddr_is_valid
   //
   // This function returns 1 if the virtual address of this descriptor is valid.
   //
   function bit vaddr_is_valid();
      return m_mem_desc_attrs.vaddr_valid;
   endfunction : vaddr_is_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_paddr
   //
   // This function is used to set the physical address for this descriptor.
   //
   // Input Variables:
   //
   // paddr_in: This is the value to which to set the physical address.
   //
   function void set_paddr(input evl_paddr_t paddr_in);
      m_mem_desc_attrs.paddr       = paddr_in;
      m_mem_desc_attrs.paddr_valid = 1'b1;
   endfunction : set_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_paddr
   //
   // This function is used to return the physical address for this descriptor.
   //
   // Return Value:
   //
   // evl_paddr_t: This is the physical address for this descriptor.
   //
   function evl_paddr_t get_paddr();
      return m_mem_desc_attrs.paddr;
   endfunction : get_paddr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: paddr_is_valid
   //
   // This function returns 1 if the physical address of this descriptor is valid.  Otherwise only
   // the virtual address might be valid and the function returns 0.
   //
   function bit paddr_is_valid();
      return m_mem_desc_attrs.paddr_valid;
   endfunction : paddr_is_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: dword_addr_match
   //
   // This function is used to compare an incoming address against the physical address; if they
   // point to the same doubleword, a 1'b1 is returned.
   //
   // Input Variables:
   //
   // addr_in: This is the value to use for comparison.
   //
   // Return Value:
   //
   // bit: This output will be 1'b1 if the physical address and the input address point to the same
   //      doubleword.
   //
   function bit dword_addr_match(input evl_paddr_t addr_in);
      return (((addr_in ^ m_mem_desc_attrs.paddr) & const_dword_paddr_mask) == evl_paddr_t'(0)) ? 1'b1 : 1'b0;
   endfunction : dword_addr_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: qword_addr_match
   //
   // This function is used to compare an incoming address against the physical address; if they
   // point to the same quadword, a 1'b1 is returned.
   //
   // Input Variables:
   //
   // addr_in: This is the value to use for comparison.
   //
   // Return Value:
   //
   // bit: This output will be 1'b1 if the physical address and the input address point to the same
   //      quadword.
   //
   function bit qword_addr_match(input evl_paddr_t addr_in);
      return (((addr_in ^ m_mem_desc_attrs.paddr) & const_qword_paddr_mask) == evl_paddr_t'(0)) ? 1'b1 : 1'b0;
   endfunction : qword_addr_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: line_addr_match
   //
   // This function is used to compare an incoming address against the physical address; if they
   // point to the same cache line, a 1'b1 is returned.
   //
   // Input Variables:
   //
   // addr_in: This is the value to use for comparison.
   //
   // Return Value:
   //
   // bit: This output will be 1'b1 if the physical address and the input address point to the same
   //      cache line.
   //
   function bit line_addr_match(input evl_paddr_t addr_in);
      return (((addr_in ^ m_mem_desc_attrs.paddr) & const_line_paddr_mask) == evl_paddr_t'(0)) ? 1'b1 : 1'b0;
   endfunction : line_addr_match


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cached
   //
   // This function is used to set the cacheability bit this descriptor (in other words, to mark the
   // address as a cached address).
   //
   function void set_cached();
      m_mem_desc_attrs.cached = 1'b1;
   endfunction : set_cached


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_uncached
   //
   // This function is used to clear the cacheability bit this descriptor (in other words, to mark
   // the address as an uncached address).
   //
   function void set_uncached();
      m_mem_desc_attrs.cached = 1'b0;
   endfunction : set_uncached


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_cached
   //
   // This function will return 1'b1 if the address for this descriptor is cached.
   //
   // Return Value:
   //
   // bit: This value is 1'b1 if the transaction is cached.
   //
   function bit is_cached();
      return m_mem_desc_attrs.cached;
   endfunction : is_cached


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_uncached
   //
   // This function will return 1'b1 if the address for this descriptor is uncached.
   //
   // Return Value:
   //
   // bit: This value is 1'b1 if the transaction is uncached.
   //
   function bit is_uncached();
      return ((m_mem_desc_attrs.cached == 1'b1) ? 1'b0 : 1'b1);
   endfunction : is_uncached


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_addr_sp
   //
   // This function is used to set the abstract address space for this descriptor.
   //
   // Input Variables:
   //
   // addr_sp_in: This is the value to which to set the abstract address space address.
   //
   function void set_addr_sp(input evl_addr_sp_t addr_sp_in);
      m_mem_desc_attrs.addr_sp = addr_sp_in;
   endfunction : set_addr_sp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_addr_sp
   //
   // This function is used to return the abstract address space for this descriptor.
   //
   // Return Value:
   //
   // evl_addr_sp_t: This is the abstract address space for this descriptor.
   //
   function evl_addr_sp_t get_addr_sp();
      return m_mem_desc_attrs.addr_sp;
   endfunction : get_addr_sp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_source
   //
   // This function is used to set the bus request source for this descriptor.  This field is
   // currently only used in the ET Link.  The ET Link interprets the bus request source as
   // follows: 0 = I-cache or page table walker; 1 = Minion D-cache.
   //
   // Input Variables:
   //
   // evl_bus_req_source_t: This is the value to which to set the bus request source.
   //
   function void set_bus_req_source(input evl_bus_req_source_t bus_req_source_in);
      m_mem_desc_attrs.req_source = bus_req_source_in;
   endfunction : set_bus_req_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_source
   //
   // This function is used to get the bus request source of this descriptor.
   //
   // Return Value:
   //
   // evl_bus_req_source_t: This function returns the value of the bus request source.
   //
   function evl_bus_req_source_t get_bus_req_source();
      return m_mem_desc_attrs.req_source;
   endfunction : get_bus_req_source


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_trans_attr
   //
   function void set_bus_trans_attr(input evl_trans_attr_s bus_trans_attr_in);
      m_mem_desc_attrs.trans_attr = bus_trans_attr_in;
   endfunction : set_bus_trans_attr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_cop_start_level
   //
   // This function is used to set the starting hierarchical cache level for a cache op.  The
   // starting level can be L1, L2, or L3.
   //
   // Input Variables:
   //
   // bus_cop_start_level_in: This is the starting hierarchical cache level for a cache op.
   //
   function void set_bus_cop_start_level(input evl_bus_cop_start_level_t bus_cop_start_level_in);
      m_mem_desc_attrs.trans_attr.cop_start_level = bus_cop_start_level_in;
   endfunction : set_bus_cop_start_level


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cop_start_level
   //
   // This function is used to get the starting hierarchical cache level for a cache op.  The
   // starting level can be L1, L2, or L3.
   //
   // Return Value:
   //
   // evl_bus_cop_start_level_t: This is the destination hierarchical cache level for a cache op.
   //
   function evl_bus_cop_start_level_t get_bus_cop_start_level();
      return m_mem_desc_attrs.trans_attr.cop_start_level;
   endfunction : get_bus_cop_start_level


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_cop_dest_level
   //
   // This function is used to set the ending hierarchical cache level for a cache op.  The ending
   // level can be L2, L3, or memory.
   //
   // Input Variables:
   //
   // bus_cop_dest_level_in: This is the destination hierarchical cache level for a cache op.
   //
   function void set_bus_cop_dest_level(input evl_bus_cop_dest_level_t bus_cop_dest_level_in);
      m_mem_desc_attrs.trans_attr.cop_dest_level = bus_cop_dest_level_in;
   endfunction : set_bus_cop_dest_level


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cop_dest_level
   //
   // This function is used to get the ending hierarchical cache level for a cache op.  The ending
   // level can be L2, L3, or memory.
   //
   // Return Value:
   //
   // evl_bus_cop_dest_level_t: This is the destination hierarchical cache level for a cache op.
   //
   function evl_bus_cop_dest_level_t get_bus_cop_dest_level();
      return m_mem_desc_attrs.trans_attr.cop_dest_level;
   endfunction : get_bus_cop_dest_level


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_cop_final_state
   //
   // This function is used to specify the final state of a cache line for an unlock cache op.
   //
   // Input Variables:
   //
   // bus_cop_final_state_in: This is the final cache line state for an unlock cache op.  A value
   //                         of 1'b1 will cause the line to be retained after unlocking.
   //
   function void set_bus_cop_final_state(input bit bus_cop_final_state_in);
      m_mem_desc_attrs.trans_attr.cop_final_state = bus_cop_final_state_in;
   endfunction : set_bus_cop_final_state


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cop_final_state
   //
   // This function is used to get the final state of a cache line for an unlock cache op.
   //
   // Return Value:
   //
   // bit: A value of 1'b1 specifies that the cache line should be retained after unlocking.
   //
   function bit get_bus_cop_final_state();
      return m_mem_desc_attrs.trans_attr.cop_final_state;
   endfunction : get_bus_cop_final_state


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_scp_fill_addr
   //
   // This function is used to set the fill address for L2 fill cache operations.  The value in this
   // field has no meaning for other types of cache ops.
   //
   // Input Variables:
   //
   // bus_scp_fill_addr_in: This is the physical scratchpad RAM address for the fill.
   //
   function void set_bus_scp_fill_addr(input evl_paddr_t bus_scp_fill_addr_in);
      m_mem_desc_attrs.trans_attr.scp_fill_addr = bus_scp_fill_addr_in;
   endfunction : set_bus_scp_fill_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_scp_fill_addr
   //
   // This function is used to get the fill address for L2 fill cache operations.  The value in this
   // field has no meaning for other types of cache ops.
   //
   // Return Value:
   //
   // evl_paddr_t: This is the physical scratchpad RAM address for the fill.
   //
   function evl_paddr_t get_bus_scp_fill_addr();
      return m_mem_desc_attrs.trans_attr.scp_fill_addr;
   endfunction : get_bus_scp_fill_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_src_shire_id
   //
   // This function is used to set the source shire ID for atomic operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // bus_src_shire_id_in: This is the source shire ID for the atomic operation.
   //
   function void set_bus_src_shire_id(input evl_shire_id_t bus_src_shire_id_in);
      m_mem_desc_attrs.trans_attr.src_shire_id = bus_src_shire_id_in;
   endfunction : set_bus_src_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_src_shire_id
   //
   // This function is used to get the source shire ID for atomic operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_shire_id_t: This is the source shire ID for the atomic operation.
   //
   function evl_shire_id_t get_bus_src_shire_id();
      return m_mem_desc_attrs.trans_attr.src_shire_id;
   endfunction : get_bus_src_shire_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_atomic_conf
   //
   // This function is used to set the conf field for atomic operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // bus_atomic_conf_in: This is the conf value for the atomic operation.
   //
   function void set_bus_atomic_conf(input evl_bus_atomic_conf_t bus_atomic_conf_in);
      m_mem_desc_attrs.trans_attr.atomic_conf = bus_atomic_conf_in;
   endfunction : set_bus_atomic_conf


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_atomic_conf
   //
   // This function is used to get the conf value for atomic operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_bus_atomic_conf_t: This is the conf value for the atomic operation.
   //
   function evl_bus_atomic_conf_t get_bus_atomic_conf();
      return m_mem_desc_attrs.trans_attr.atomic_conf;
   endfunction : get_bus_atomic_conf


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_atomic_transid
   //
   // This function is used to set the transaction ID field for atomic operations.  The value in
   // this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // bus_atomic_transid_in: This is the transaction ID value for the atomic operation.
   //
   function void set_bus_atomic_transid(input evl_bus_atomic_transid_t bus_atomic_transid_in);
      m_mem_desc_attrs.trans_attr.atomic_transid = bus_atomic_transid_in;
   endfunction : set_bus_atomic_transid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_atomic_transid
   //
   // This function is used to get the transaction ID value for atomic operations.  The value in
   // this field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_bus_atomic_transid_t: This is the transaction ID value for the atomic operation.
   //
   function evl_bus_atomic_transid_t get_bus_atomic_transid();
      return m_mem_desc_attrs.trans_attr.atomic_transid;
   endfunction : get_bus_atomic_transid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_atomic_offset
   //
   // This function is used to set the offset field for atomic operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // addr_in: This is the offset value for the atomic operation.
   //
   function void set_bus_atomic_offset(input evl_paddr_t addr_in);
      m_mem_desc_attrs.trans_attr.atomic_offset = evl_line_offset_t'(addr_in);
   endfunction : set_bus_atomic_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_atomic_offset
   //
   // This function is used to get the offset value for atomic operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_paddr_t: This is the offset value for the atomic operation.
   //
   function evl_paddr_t get_bus_atomic_offset();
      return evl_paddr_t'(m_mem_desc_attrs.trans_attr.atomic_offset);
   endfunction : get_bus_atomic_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_subopcode_dest
   //
   // This function is used to set the subopcode destination field for loads and stores.  The value
   // in this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // bus_subopcode_dest_in: This is the subopcode destination value.
   //
   function void set_bus_subopcode_dest(input evl_bus_subopcode_t bus_subopcode_dest_in);
      m_mem_desc_attrs.trans_attr.subopcode_dest = bus_subopcode_dest_in;
   endfunction : set_bus_subopcode_dest


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_subopcode_dest
   //
   // This function is used to get the subopcode destination value for loads and stores.  The value
   // in this field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_bus_subopcode_t: This is the subopcode destination value.
   //
   function evl_bus_subopcode_t get_bus_subopcode_dest();
      return m_mem_desc_attrs.trans_attr.subopcode_dest;
   endfunction : get_bus_subopcode_dest


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_message_conf
   //
   // This function is used to set the conf field for message operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // bus_message_conf_in: This is the conf value for the message operation.
   //
   function void set_bus_message_conf(input evl_bus_message_conf_t bus_message_conf_in);
      m_mem_desc_attrs.trans_attr.message_conf = bus_message_conf_in;
   endfunction : set_bus_message_conf


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_message_conf
   //
   // This function is used to get the conf value for message operations.  The value in this
   // field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_bus_message_conf_t: This is the conf value for the message operation.
   //
   function evl_bus_message_conf_t get_bus_message_conf();
      return m_mem_desc_attrs.trans_attr.message_conf;
   endfunction : get_bus_message_conf


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_minion_coop_mask
   //
   // This function is used to set the minon mask of the cooperative minions field for cooperative
   // load operations.  The value in this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // coop_mask_in: This is the n-hot minion mask field for a cooperative load.
   //
   function void set_bus_minion_coop_mask(input evl_bus_minion_coop_mask_t minion_coop_mask_in);
      m_mem_desc_attrs.trans_attr.coop_minion_mask = minion_coop_mask_in;
   endfunction : set_bus_minion_coop_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_neigh_coop_mask
   //
   // This function is used to set the neigh mask of the cooperative minions field for cooperative
   // load operations.  The value in this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // coop_mask_in: This is the n-hot neigh mask field for a cooperative load.
   //
   function void set_bus_neigh_coop_mask(input evl_bus_neigh_coop_mask_t neigh_coop_mask_in);
      m_mem_desc_attrs.trans_attr.coop_neigh_mask = neigh_coop_mask_in;
   endfunction : set_bus_neigh_coop_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_neigh_coop_mask
   //
   // This function is used to set the neigh mask of the cooperative minions field for cooperative
   // load operations.  The value in this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // coop_mask_in: This is the n-hot neigh mask field for a cooperative load.
   //
   function evl_bus_neigh_coop_mask_t get_bus_neigh_coop_mask();
      return m_mem_desc_attrs.trans_attr.coop_neigh_mask;
   endfunction : get_bus_neigh_coop_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_minion_coop_mask
   //
   // This function is used to get the minion cooperation field for cooperative load operations
   //
   // Return Value:
   //
   // evl_bus_coop_mask_t: This is the n-hot minion cooperative mask field for a cooperative load.
   //
   function evl_bus_minion_coop_mask_t get_bus_minion_coop_mask();
      return m_mem_desc_attrs.trans_attr.coop_minion_mask;
   endfunction : get_bus_minion_coop_mask


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_coop_id
   //
   // This function is used to set the coop id field for cooperative load operations.  The value in
   // this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // coop_id_in: This is the cooperative id field for a cooperative load.
   //
   function void set_bus_coop_id(input evl_bus_coop_id_t coop_id_in);
      m_mem_desc_attrs.trans_attr.coop_id = coop_id_in;
   endfunction : set_bus_coop_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_coop_way
   //
   // This function is used to set the coop way field for cooperative store operations.  The value
   // in this field has no meaning for other opcodes.
   //
   // Input Variables:
   //
   // coop_way_in: This is the cooperative way field for a cooperative store (in the subopcode)
   //
   function void set_bus_coop_way(input evl_bus_coop_way_t coop_way_in);
      m_mem_desc_attrs.trans_attr.coop_way = coop_way_in;
   endfunction : set_bus_coop_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_coop_id
   //
   // This function is used to set the target field for cooperative load operations.  The value in
   // this field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_bus_coop_id_t: This is the cooperative id field for a cooperative load.
   //
   function evl_bus_coop_id_t get_bus_coop_id();
      return m_mem_desc_attrs.trans_attr.coop_id;
   endfunction : get_bus_coop_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_coop_way
   //
   // This function is used to get the coop_way field for cooperative store operations.  The value
   // in this field has no meaning for other opcodes.
   //
   // Return Value:
   //
   // evl_bus_coop_way_t: This is the cooperative id field for a cooperative store.
   //
   function evl_bus_coop_way_t get_bus_coop_way();
      return m_mem_desc_attrs.trans_attr.coop_way;
   endfunction : get_bus_coop_way


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_coop_target
   //
   // This function is used to get the coop_target field from the trans desc.  The value in the
   // field may be copied to the neigh mask or minion mask in a req_desc by a stim agent.
   //
   // Return Value:
   //
   // evl_bus_coop_target_t: This is a value generated by the stimulus which may end up being a
   // minion mask or a neigh mask.
   //
   function evl_bus_coop_target_t get_bus_coop_target();
      return m_mem_desc_attrs.trans_attr.coop_target;
   endfunction : get_bus_coop_target


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_qos
   //
   // This function is used to set the value of the QOS field for this descriptor.
   //
   // Input Variables:
   //
   // bus_qos_in: This is the value of the QOS field.
   //
   virtual function void set_bus_qos(input evl_bus_qos_t bus_qos_in);
      m_mem_desc_attrs.qos = bus_qos_in;
   endfunction : set_bus_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_qos
   //
   // This function is used to get the value of the QOS field for this descriptor.
   //
   // Return Value:
   //
   // evl_bus_qos_t: This function returns the value of the QOS field in this descriptor.
   //
   virtual function evl_bus_qos_t get_bus_qos();
      return m_mem_desc_attrs.qos;
   endfunction : get_bus_qos


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_vc
   //
   function void set_vc(input evl_bus_req_vc_t vc_in);
      m_mem_desc_attrs.vc = vc_in;
   endfunction : set_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_vc
   //
   function evl_bus_req_vc_t get_vc();
      return m_mem_desc_attrs.vc;
   endfunction : get_vc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_attrs
   //
   // This function is used to compare this descriptor's attributes to an expected version
   //
   // Input Variables:
   //
   // exp_data:         This is the memory descriptor against which to compare this object's
   //                   attributes
   //
   // Input Variables:
   //
   // err_messages_out: This array will hold error messages in case there is(are) mismatch(es)
   //
   virtual function bit compare_attrs(input  evl_mem_desc exp_mem_desc,
                                      output string       err_messages_out[]);
      string err_messages[$];

      if (get_neighborhood_id() != exp_mem_desc.get_neighborhood_id()) begin
         err_messages.push_back($sformatf("neighborhood_id mismatch: have %0p, expected %0p", get_neighborhood_id(), exp_mem_desc.get_neighborhood_id()));
      end
      if (get_shire_id() != exp_mem_desc.get_shire_id()) begin
         err_messages.push_back($sformatf("shire_id mismatch: have %0p, expected %0p", get_shire_id(), exp_mem_desc.get_shire_id()));
      end
      if (get_paddr() != exp_mem_desc.get_paddr()) begin
         err_messages.push_back($sformatf("paddr mismatch: have 0x%0x, expected 0x%0x", get_paddr(), exp_mem_desc.get_paddr()));
      end
      if (get_addr_sp() != exp_mem_desc.get_addr_sp()) begin
         err_messages.push_back($sformatf("addr_sp mismatch: have %0p, expected %0p", get_addr_sp(), exp_mem_desc.get_addr_sp()));
      end
      if (get_bus_req_source() != exp_mem_desc.get_bus_req_source()) begin
         err_messages.push_back($sformatf("req_source mismatch: have %0p, expected %0p", get_bus_req_source(), exp_mem_desc.get_bus_req_source()));
      end
      if (get_bus_cop_start_level() != exp_mem_desc.get_bus_cop_start_level()) begin
         err_messages.push_back($sformatf("cop_start_level mismatch: have %0p, expected %0p", get_bus_cop_start_level(), exp_mem_desc.get_bus_cop_start_level()));
      end
      if (get_bus_cop_dest_level() != exp_mem_desc.get_bus_cop_dest_level()) begin
         err_messages.push_back($sformatf("cop_dest_level mismatch: have %0p, expected %0p", get_bus_cop_dest_level(), exp_mem_desc.get_bus_cop_dest_level()));
      end
      if (get_bus_cop_final_state() != exp_mem_desc.get_bus_cop_final_state()) begin
         err_messages.push_back($sformatf("cop_final_state mismatch: have %0p, expected %0p", get_bus_cop_final_state(), exp_mem_desc.get_bus_cop_final_state()));
      end
      if (get_bus_scp_fill_addr() != exp_mem_desc.get_bus_scp_fill_addr()) begin
         err_messages.push_back($sformatf("scp_fill_addr mismatch: have 0x%0x, expected 0x%0x", get_bus_scp_fill_addr(), exp_mem_desc.get_bus_scp_fill_addr()));
      end
      if (get_bus_atomic_conf() != exp_mem_desc.get_bus_atomic_conf()) begin
         err_messages.push_back($sformatf("bus_atomic_conf mismatch: have %0p, expected %0p", get_bus_atomic_conf(), exp_mem_desc.get_bus_atomic_conf()));
      end
      if (get_bus_message_conf() != exp_mem_desc.get_bus_message_conf()) begin
         err_messages.push_back($sformatf("bus_message_conf mismatch: have %0p, expected %0p", get_bus_message_conf(), exp_mem_desc.get_bus_message_conf()));
      end
      if (get_vc() != exp_mem_desc.get_vc()) begin
         err_messages.push_back($sformatf("vc mismatch: have 0x%0x, expected 0x%0x", get_vc(), exp_mem_desc.get_vc()));
      end
      if (get_bus_coop_id() != exp_mem_desc.get_bus_coop_id()) begin
         err_messages.push_back($sformatf("coop id mismatch: have %0p, expected %0p", get_bus_coop_id(), exp_mem_desc.get_bus_coop_id()));
      end

      if (err_messages.size() > 0) begin
         err_messages_out = new[err_messages.size()];

         foreach(err_messages[ii]) begin
            err_messages_out[ii] = err_messages[ii];
         end
      end

      return (err_messages.size() > 0);
   endfunction : compare_attrs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_created_by_bus
   //
   // This function is used to mark this descriptor as having been created by a bus VC/monitor.
   //
   function void set_created_by_bus();
      m_created_by_bus = 1'b1;
   endfunction : set_created_by_bus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: created_by_bus
   //
   // This function will return 1'b1 if this descriptor has been created by a bus VC/monitor.
   //
   function bit created_by_bus();
      return m_created_by_bus;
   endfunction : created_by_bus


   //-----------------------------------------------------------------------------------------------
   //
   // Function: mark_as_bus_child
   //
   // This function is used to mark this descriptor as an observed bus child descriptor.
   //
   function void mark_as_bus_child();
      m_bus_child = 1'b1;
   endfunction : mark_as_bus_child


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_bus_child
   //
   // This function will return 1'b1 if this descriptor is an observed bus child descriptor.
   //
   function bit is_bus_child();
      return m_bus_child;
   endfunction : is_bus_child


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_spawned_bus_child
   //
   // This function will return 1'b1 if this descriptor is an observed bus child descriptor and it
   // has an expected request descriptor that points to it..
   //
   virtual function bit is_spawned_bus_child();
      return 1'b0;
   endfunction : is_spawned_bus_child


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_req_desc
   //
   // This function is used to set the local m_bus_req_desc handle to the incoming reqest
   // descriptor (which only exists in the evl_req_desc extension of evl_mem_desc).  This function
   // should only be called when an observed request descriptor is created by a bus verification
   // component/monitor and linked to a sent request descriptor.  The m_bus_req_desc handle in the
   // sent request descriptor should then be set to the observed request descriptor.
   //
   virtual function void set_bus_req_desc(input evl_req_desc req_desc);
      return;
   endfunction : set_bus_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_req_desc
   //
   // This function is used to get the local m_bus_req_desc handle (see above).
   //
   virtual function evl_req_desc get_bus_req_desc();
      return null;
   endfunction : get_bus_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_issued
   //
   // This function is used to mark the descriptor as issued.
   //
   virtual function void set_issued();
      if ((m_issued == 1'b0) && (m_create_time > 0)) begin
         m_issue_time = $realtime;
      end
      m_issued = 1'b1;
   endfunction : set_issued


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_issued
   //
   // This function will return 1'b1 if this descriptor has been issued, otherwise it will return
   // 1'b0.
   //
   function bit has_issued();
      return m_issued;
   endfunction : has_issued


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_issue
   //
   // This task is used to wait until this descriptor has been issued.
   //
   task wait_for_issue();
      #0 wait (m_issued == 1'b1);
   endtask : wait_for_issue


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_observed
   //
   virtual function void set_observed();
      return;
   endfunction : set_observed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_been_observed
   //
   virtual function bit has_been_observed();
      return m_issued;
   endfunction : has_been_observed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_correlated
   //
   // This function is used to mark the descriptor as correlated (which means that a matching parent
   // descriptor has been found).
   //
   virtual function void set_correlated();
      return;
   endfunction : set_correlated


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_correlated
   //
   virtual function bit is_correlated();
      return 1'b0;
   endfunction : is_correlated


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_parent_check
   //
   // This function is used to mark the descriptor as correlated (which means that a check against a
   // matching parent should occur, regardless of whether a matching parent has been found or not).
   //
   virtual function void set_parent_check();
      return;
   endfunction : set_parent_check


   //-----------------------------------------------------------------------------------------------
   //
   // Function: do_parent_check
   //
   virtual function bit do_parent_check();
      return 1'b0;
   endfunction : do_parent_check


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_processed
   //
   // This function is used to mark the descriptor as having been processed for the first time.
   //
   virtual function void set_processed();
      if ((m_processed == 1'b0) && (m_create_time > 0)) begin
         m_process_time = $realtime;
      end
      m_processed = 1'b1;
   endfunction : set_processed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_parent_processed
   //
   // This function is used to mark the parent descriptor as having been processed for the first
   // time.
   //
   virtual function void set_parent_processed();
      if (is_spawned_bus_child() == 1'b1) begin
         m_parent_desc.set_processed();
      end
      set_processed();
   endfunction : set_parent_processed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_root_processed
   //
   // This function is used to mark the root descriptor as having been processed for the first
   // time.
   //
   virtual function void set_root_processed();
      if (m_root_desc != null) begin
         m_root_desc.set_processed();
      end
      set_parent_processed();
   endfunction : set_root_processed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_been_processed
   //
   // This function will return 1'b1 if this descriptor has been processed, otherwise it will return
   // 1'b0.
   //
   function bit has_been_processed();
      return m_processed;
   endfunction : has_been_processed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_cycle
   //
   // This function is used to set the cycle in which a response is set by a scoreboard.  Normally
   // something like this would not be necessary, but ET link write requests do not use unique tags,
   // so there is no way to know which request completes when a write response is observed.
   //
   function void set_rsp_cycle(input int unsigned cycle);
      m_rsp_cycle = cycle;
   endfunction : set_rsp_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_cycle
   //
   // This function will return the value of the cycle in which a response is set by a scoreboard.
   //
   function int unsigned get_rsp_cycle();
      return m_rsp_cycle;
   endfunction : get_rsp_cycle


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_done
   //
   // This function is used to mark the descriptor as complete.
   //
   virtual function void set_done();
      if ((m_done == 1'b0) && (m_create_time > 0)) begin
         m_done_time = $realtime;
      end
      m_done = 1'b1;
   endfunction : set_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_trans_done
   //
   // This function is used to mark the transaction descriptor as complete.
   //
   virtual function void set_trans_done();
      if (m_desc_type == EVL_DESC_TRANS) begin
         set_done();
      end
      else if ((m_parent_desc != null) && (m_parent_desc.get_desc_type() == EVL_DESC_TRANS)) begin
         m_parent_desc.set_done();
      end
   endfunction : set_trans_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: is_done
   //
   // This function will return 1'b1 if this descriptor is complete, otherwise it will return 1'b0.
   //
   function bit is_done();
      return m_done;
   endfunction : is_done


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_done
   //
   // This task is used to wait until this descriptor completes.
   //
   task wait_for_done();
      #0 wait (m_done == 1'b1);
   endtask : wait_for_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_desc_stats
   //
   // This function is used to process descriptor statistics.
   //
   virtual function void process_desc_stats();
      if (m_root_desc != null) begin
         m_root_desc.process_desc_stats();
      end
   endfunction : process_desc_stats


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gather_desc_stats
   //
   // This function is used to gather descriptor statistics.
   //
   virtual function void gather_desc_stats(input int level, ref evl_desc_stat_t desc_stats[$]);
      int             new_level = level;
      evl_desc_stat_t desc_stat;

      if ((m_ancestor_desc == null) || (m_ancestor_desc == this)) begin
         new_level = 0;
      end
      if ((m_create_time > 0) && (m_done_time > 0)) begin
         desc_stat.level    = new_level;
         desc_stat.bus_type = get_bus_type();
         if (m_ancestor_desc != null) begin
            desc_stat.issue_time   = (has_issued()         == 1'b1) ? $rtoi(1000.0 * (m_issue_time   - m_ancestor_desc.get_create_time())) : 0;
            desc_stat.process_time = (has_been_processed() == 1'b1) ? $rtoi(1000.0 * (m_process_time - m_ancestor_desc.get_create_time())) : 0;
            desc_stat.done_time    = (is_done()            == 1'b1) ? $rtoi(1000.0 * (m_done_time    - m_ancestor_desc.get_create_time())) : 0;
         end
         else begin
            desc_stat.issue_time   = (has_issued()         == 1'b1) ? $rtoi(1000.0 * (m_issue_time   - m_create_time)) : 0;
            desc_stat.process_time = (has_been_processed() == 1'b1) ? $rtoi(1000.0 * (m_process_time - m_create_time)) : 0;
            desc_stat.done_time    = (is_done()            == 1'b1) ? $rtoi(1000.0 * (m_done_time    - m_create_time)) : 0;
         end
         desc_stats.push_back(desc_stat);
         new_level++;
      end
      foreach (m_child_desc_queues[ii]) begin
         m_child_desc_queues[ii].gather_desc_stats(new_level, desc_stats);
      end
   endfunction : gather_desc_stats


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cache_st
   //
   virtual function evl_cache_st_t get_cache_st();
      if (m_line_desc == null) begin
         return I;
      end
      return m_line_desc.get_cache_st();
   endfunction : get_cache_st


   //-----------------------------------------------------------------------------------------------
   //
   // Function: create_data
   //
   // This function is used to create a new line data object and either populate it with 0s or with
   // random data.  If an incoming line data template handle is provided, that template is used as a
   // basis for creation.  If no incoming line data template handle is provided, the line data
   // template handle within this descriptor (m_template_line_data) is used; if that handle is null,
   // an evl_line_data object is created.
   //
   // The random_data input variable can be set to 1 to specify generation of random data in the
   // created line data object.  A handle to the created line data object is returned on completion.
   //
   // Input Variables:
   //
   // random_data:           If this input is 1'b1, random data will be generated.  If not, the
   //                        created data object will contain 0s.
   // template_line_data_in: If this input handle is not null, it will be used as a basis for
   //                        creation of the new line data object.  If it is null, the descriptor's
   //                        line data template will be used instead.
   //
   // Return Value:
   //
   // evl_line_data: This is handle to the created object.
   //
   function evl_line_data create_data(input bit random_data = 1'b0, input evl_line_data template_line_data_in = null);
      evl_line_data line_data;

      if (template_line_data_in != null) begin
         line_data = template_line_data_in.clone_line_data();
      end
      else if (m_template_data != null) begin
         line_data = m_template_data.clone_line_data();
      end
      else begin
         line_data = new(get_abstract_name());
      end
      if (random_data == 1'b0) begin
         line_data.create_data(null);
      end
      else begin
         line_data.create_data(m_rand_gen);
      end
      return line_data;
   endfunction : create_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_byte_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_byte_offset(input int byte_offset_in);
      if (byte_offset_in < 0) begin
         return int'(get_paddr() & (`EVL_LINE_BYTES - 1));
      end
      return byte_offset_in & (`EVL_LINE_BYTES - 1);
   endfunction : get_byte_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_hword_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_hword_offset(input int hword_offset_in);
      if (hword_offset_in < 0) begin
         return int'(get_paddr() >> `EVL_HWORD_ADDR_LSB) & (`EVL_LINE_HWORDS - 1);
      end
      return hword_offset_in & (`EVL_LINE_HWORDS - 1);
   endfunction : get_hword_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_word_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_word_offset(input int word_offset_in);
      if (word_offset_in < 0) begin
         return int'(get_paddr() >> `EVL_WORD_ADDR_LSB) & (`EVL_LINE_WORDS - 1);
      end
      return word_offset_in & (`EVL_LINE_WORDS - 1);
   endfunction : get_word_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_dword_offset(input int dword_offset_in);
      if (dword_offset_in < 0) begin
         return int'(get_paddr() >> `EVL_DWORD_ADDR_LSB) & (`EVL_LINE_DWORDS - 1);
      end
      return dword_offset_in & (`EVL_LINE_DWORDS - 1);
   endfunction : get_dword_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_qword_offset(input int qword_offset_in);
      if (qword_offset_in < 0) begin
         return int'(get_paddr() >> `EVL_QWORD_ADDR_LSB) & (`EVL_LINE_QWORDS - 1);
      end
      return qword_offset_in & (`EVL_LINE_QWORDS - 1);
   endfunction : get_qword_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_oword_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_oword_offset(input int oword_offset_in);
      if (oword_offset_in < 0) begin
         return int'(get_paddr() >> `EVL_OWORD_ADDR_LSB) & (`EVL_LINE_OWORDS - 1);
      end
      return oword_offset_in & (`EVL_LINE_OWORDS - 1);
   endfunction : get_oword_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_xword_offset
   //
   // BLAH
   //
   // Return Value:
   //
   // int: BLAH
   //
   function int get_xword_offset(input int xword_offset_in);
      if (xword_offset_in < 0) begin
         return int'(get_paddr() >> `EVL_XWORD_ADDR_LSB) & (`EVL_LINE_XWORDS - 1);
      end
      return xword_offset_in & (`EVL_LINE_XWORDS - 1);
   endfunction : get_xword_offset


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bytes
   //
   // This function is used to set data of arbitrary length and alignment at the specified offset in
   // the line data object provided.  If the incoming line data handle is null, a new line data
   // object based on this descriptor's line data template will be created (filled with 0s) and the
   // specified data will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:     This is a handle to the line data object to modify.  If it is null, a new one
   //                will be created.
   // byte_index_in: This is the offset within the cache line to modify.
   // length_in:     This is the length of the data to modify
   // xword_in:      This is the data to use for the modification
   // byte_en_in:    This is a doubleword byte enable specifying which bytes to modify.
   // unknown_in:    This is a doubleword byte enable specifying which bytes to mark as unknown.
   //                Only those bytes being modified can be marked as unknown.
   // modified_in:   This is a doubleword byte enable specifying which bytes to mark as modified.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_bytes(input evl_line_data       line_data,
                                    input int                 byte_index_in,
                                    input int                 length_in,
                                    input evl_xword_t         xword_in,
                                    input evl_xword_byte_en_t byte_en_in,
                                    input evl_xword_byte_en_t unknown_in  = evl_xword_byte_en_t'(0),
                                    input evl_xword_byte_en_t modified_in = evl_xword_byte_en_t'(0));
      int index = byte_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & ~const_line_paddr_mask);
      end
      line_data.set_bytes(index, length_in, xword_in, byte_en_in, unknown_in, modified_in);
      return line_data;
   endfunction : set_bytes


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_byte
   //
   // This function is used to set a byte at the specified byte offset in the line data object
   // provided.  If the incoming line data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) and the specified byte will
   // be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:     This is a handle to the line data object to modify.  If it is null, a new one
   //                will be created.
   // byte_index_in: This is the byte offset within the cache line to modify.
   // byte_in:       This is the byte value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_byte(input evl_line_data line_data, input int byte_index_in, input byte byte_in);
      int index = byte_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & ~const_line_paddr_mask);
      end
      line_data.set_byte(index, byte_in);
      return line_data;
   endfunction : set_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_hword
   //
   // This function is used to set a halfword (2 bytes) at the specified halfword offset in the line
   // data object provided.  If the incoming line data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // halfword will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:      This is a handle to the line data object to modify.  If it is null, a new one
   //                 will be created.
   // hword_index_in: This is the halfword offset within the cache line to modify (address bits
   //                 5-1).
   // hword_in:       This is the halfword value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_hword(input evl_line_data line_data, input int hword_index_in, input evl_hword_t hword_in);
      int index = hword_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask) >> `EVL_HWORD_ADDR_LSB;
      end
      line_data.set_hword(index, hword_in);
      return line_data;
   endfunction : set_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_word
   //
   // This function is used to set a word (4 bytes) at the specified word offset in the line data
   // object provided.  If the incoming line data handle is null, a new line data object based on
   // this descriptor's line data template will be created (filled with 0s) and the specified word
   // will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:     This is a handle to the line data object to modify.  If it is null, a new one
   //                will be created.
   // word_index_in: This is the word offset within the cache line to modify (address bits 5-2).
   // word_in:       This is the word value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_word(input evl_line_data line_data, input int word_index_in, input evl_word_t word_in);
      int index = word_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_word_paddr_mask & ~const_line_paddr_mask) >> `EVL_WORD_ADDR_LSB;
      end
      line_data.set_word(index, word_in);
      return line_data;
   endfunction : set_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_dword
   //
   // This function is used to set a doubleword (8 bytes) at the specified doubleword offset in the
   // line data object provided.  If the incoming line data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) and the
   // specified doubleword will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:      This is a handle to the line data object to modify.  If it is null, a new one
   //                 will be created.
   // dword_index_in: This is the doubleword offset within the cache line to modify (address bits
   //                 5-3).
   // dword_in:       This is the doubleword value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_dword(input evl_line_data line_data, input int dword_index_in, input evl_dword_t dword_in);
      int index = dword_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_dword_paddr_mask & ~const_line_paddr_mask) >> `EVL_DWORD_ADDR_LSB;
      end
      line_data.set_dword(index, dword_in);
      return line_data;
   endfunction : set_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_qword
   //
   // This function is used to set a quadword (16 bytes) at the specified quadword offset in the
   // line data object provided.  If the incoming line data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) and the
   // specified quadword will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:      This is a handle to the line data object to modify.  If it is null, a new one
   //                 will be created.
   // qword_index_in: This is the quadword offset within the cache line to modify (address bits
   //                 5-4).
   // qword_in:       This is the quadword value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_qword(input evl_line_data line_data, input int qword_index_in, input evl_qword_t qword_in);
      int index = qword_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_qword_paddr_mask & ~const_line_paddr_mask) >> `EVL_QWORD_ADDR_LSB;
      end
      line_data.set_qword(index, qword_in);
      return line_data;
   endfunction : set_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_oword
   //
   // This function is used to set an octalword (32 bytes) at the specified octalword offset in the
   // line data object provided.  If the incoming line data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) and the
   // specified octalword will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:      This is a handle to the line data object to modify.  If it is null, a new one
   //                 will be created.
   // oword_index_in: This is the octalword offset within the cache line to modify (address bit 5).
   // oword_in:       This is the octalword value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_oword(input evl_line_data line_data, input int oword_index_in, input evl_oword_t oword_in);
      int index = oword_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_oword_paddr_mask & ~const_line_paddr_mask) >> `EVL_OWORD_ADDR_LSB;
      end
      line_data.set_oword(index, oword_in);
      return line_data;
   endfunction : set_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_xword
   //
   // This function is used to set a hexword (64 bytes) at the specified hexword offset in the line
   // data object provided.  If the incoming line data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) and the specified
   // hexword will be modified appropriately.
   //
   // Input Variables:
   //
   // line_data:      This is a handle to the line data object to modify.  If it is null, a new one
   //                 will be created.
   // xword_index_in: This is the hexword offset within the cache line to modify (always 0).
   // xword_in:       This is the hexword value to use for modification.
   //
   // Return Value:
   //
   // evl_line_data: This will either be the input handle passed in (if it is not null) or it will
   //                be a handle to a created object.
   //
   function evl_line_data set_xword(input evl_line_data line_data, input int xword_index_in, input evl_xword_t xword_in);
      int index = xword_index_in;

      if (line_data == null) begin
         line_data = create_data();
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_xword_paddr_mask & ~const_line_paddr_mask) >> `EVL_XWORD_ADDR_LSB;
      end
      line_data.set_xword(index, xword_in);
      return line_data;
   endfunction : set_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_byte
   //
   // This function is used to get a byte at the specified byte offset in the line data object
   // provided.  If the incoming line data handle is null, a new line data object based on this
   // descriptor's line data template will be created (filled with 0s) *if* the create_data_in input
   // is set to 1.  Note that the incoming line data object is pass-by-reference ... this allows
   // another function (such as get_write_byte() in the transaction descriptor) to pass in a handle
   // such is m_write_data, and if a new object is created, the m_write_data handle will point to
   // it.  In the event no line data object exists and none is supposed to be created, 0s are
   // returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // byte_index_in:  This is the byte offset within the cache line to read.
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // byte: This is the byte value read.
   //
   function byte get_byte(ref evl_line_data line_data, input int byte_index_in, input bit create_data_in = 1'b0);
      int index = byte_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_hword_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & ~const_line_paddr_mask);
      end
      return line_data.get_byte(index);
   endfunction : get_byte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_hword
   //
   // This function is used to get a halfword (2 bytes) at the specified halfword offset in the line
   // data object provided.  If the incoming line data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) *if* the
   // create_data_in input is set to 1.  Note that the incoming line data object is
   // pass-by-reference ... this allows another function (such as get_write_hword() in the
   // transaction descriptor) to pass in a handle such is m_write_data, and if a new object is
   // created, the m_write_data handle will point to it.  In the event no line data object exists
   // and none is supposed to be created, 0s are returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // hword_index_in: This is the halfword offset within the cache line to read (address bits
   //                 5-1).
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // evl_hword_t: This is the halfword value read.
   //
   function evl_hword_t get_hword(ref evl_line_data line_data, input int hword_index_in, input bit create_data_in = 1'b0);
      int index = hword_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_hword_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_hword_paddr_mask & ~const_line_paddr_mask) >> `EVL_HWORD_ADDR_LSB;
      end
      return line_data.get_hword(index);
   endfunction : get_hword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_word
   //
   // This function is used to get a word (4 bytes) at the specified word offset in the line data
   // object provided.  If the incoming line data handle is null, a new line data object based on
   // this descriptor's line data template will be created (filled with 0s) *if* the create_data_in
   // input is set to 1.  Note that the incoming line data object is pass-by-reference ... this
   // allows another function (such as get_write_word() in the transaction descriptor) to pass in a
   // handle such is m_write_data, and if a new object is created, the m_write_data handle will
   // point to it.  In the event no line data object exists and none is supposed to be created, 0s
   // are returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // word_index_in:  This is the word offset within the cache line to read (address bits
   //                 5-2).
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // evl_word_t: This is the word value read.
   //
   function evl_word_t get_word(ref evl_line_data line_data, input int word_index_in, input bit create_data_in = 1'b0);
      int index = word_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_word_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_word_paddr_mask & ~const_line_paddr_mask) >> `EVL_WORD_ADDR_LSB;
      end
      return line_data.get_word(index);
   endfunction : get_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword
   //
   // This function is used to get a doubleword (8 bytes) at the specified doubleword offset in the
   // line data object provided.  If the incoming line data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) *if* the
   // create_data_in input is set to 1.  Note that the incoming line data object is
   // pass-by-reference ... this allows another function (such as get_write_dword() in the
   // transaction descriptor) to pass in a handle such is m_write_data, and if a new object is
   // created, the m_write_data handle will point to it.  In the event no line data object exists
   // and none is supposed to be created, 0s are returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // dword_index_in: This is the doubleword offset within the cache line to read (address bits
   //                 5-3).
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // evl_dword_t: This is the doubleword value read.
   //
   function evl_dword_t get_dword(ref evl_line_data line_data, input int dword_index_in, input bit create_data_in = 1'b0);
      int index = dword_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_dword_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_dword_paddr_mask & ~const_line_paddr_mask) >> `EVL_DWORD_ADDR_LSB;
      end
      return line_data.get_dword(index);
   endfunction : get_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword
   //
   // This function is used to get a quadword (16 bytes) at the specified quadword offset in the
   // line data object provided.  If the incoming line data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) *if* the
   // create_data_in input is set to 1.  Note that the incoming line data object is
   // pass-by-reference ... this allows another function (such as get_write_qword() in the
   // transaction descriptor) to pass in a handle such is m_write_data, and if a new object is
   // created, the m_write_data handle will point to it.  In the event no line data object exists
   // and none is supposed to be created, 0s are returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // qword_index_in: This is the quadword offset within the cache line to read (address bits
   //                 5-3).
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // evl_qword_t: This is the quadword value read.
   //
   function evl_qword_t get_qword(ref evl_line_data line_data, input int qword_index_in, input bit create_data_in = 1'b0);
      int index = qword_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_qword_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_qword_paddr_mask & ~const_line_paddr_mask) >> `EVL_QWORD_ADDR_LSB;
      end
      return line_data.get_qword(index);
   endfunction : get_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_oword
   //
   // This function is used to get an octalword (32 bytes) at the specified octalword offset in the
   // line data object provided.  If the incoming line data handle is null, a new line data object
   // based on this descriptor's line data template will be created (filled with 0s) *if* the
   // create_data_in input is set to 1.  Note that the incoming line data object is
   // pass-by-reference ... this allows another function (such as get_write_oword() in the
   // transaction descriptor) to pass in a handle such is m_write_data, and if a new object is
   // created, the m_write_data handle will point to it.  In the event no line data object exists
   // and none is supposed to be created, 0s are returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // oword_index_in: This is the octalword offset within the cache line to read (address bit 5).
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // evl_oword_t: This is the octalword value read.
   //
   function evl_oword_t get_oword(ref evl_line_data line_data, input int oword_index_in, input bit create_data_in = 1'b0);
      int index = oword_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_oword_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_oword_paddr_mask & ~const_line_paddr_mask) >> `EVL_OWORD_ADDR_LSB;
      end
      return line_data.get_oword(index);
   endfunction : get_oword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_xword
   //
   // This function is used to get a hexword (64 bytes) at the specified hexword offset in the line
   // data object provided.  If the incoming line data handle is null, a new line data object based
   // on this descriptor's line data template will be created (filled with 0s) *if* the
   // create_data_in input is set to 1.  Note that the incoming line data object is
   // pass-by-reference ... this allows another function (such as get_write_xword() in the
   // transaction descriptor) to pass in a handle such is m_write_data, and if a new object is
   // created, the m_write_data handle will point to it.  In the event no line data object exists
   // and none is supposed to be created, 0s are returned.
   //
   // Input Variables:
   //
   // line_data:      This pass-by-reference handle points to the line data object to read.  If it
   //                 is null, a new one may be created.
   // xword_index_in: This is the hexword offset within the cache line to modify (always 0).
   // create_data_in: If this input is 1, a new line data object will be created if none exists.
   //
   // Return Value:
   //
   // evl_xword_t: This is the hexword value read.
   //
   function evl_xword_t get_xword(ref evl_line_data line_data, input int xword_index_in, input bit create_data_in = 1'b0);
      int index = xword_index_in;

      if (line_data == null) begin
         if (create_data_in == 1'b1) begin
            line_data = create_data();
         end
         else begin
            return evl_xword_t'(0);
         end
      end
      if (index < 0) begin
         index = int'(get_paddr() & const_xword_paddr_mask & ~const_line_paddr_mask) >> `EVL_XWORD_ADDR_LSB;
      end
      return line_data.get_xword(index);
   endfunction : get_xword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_line_desc
   //
   // Blah.
   //
   // Input Variables:
   //
   // line_desc_in: This is a handle to Blah
   //
   function void set_line_desc(input evl_line_desc line_desc_in);
      m_line_desc = line_desc_in;
   endfunction : set_line_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_line_desc
   //
   // Blah.
   //
   function evl_line_desc get_line_desc();
      return m_line_desc;
   endfunction : get_line_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_root_desc
   //
   // This function is used to set the root descriptor for this transaction thread.  The root
   // descriptor at the head of the thread will either be null or point to itself.
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the root descriptor for the transaction thread.
   //
   function void set_root_desc(input evl_mem_desc mem_desc_in);
      m_root_desc = mem_desc_in;
   endfunction : set_root_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_root_desc
   //
   // This function is used to get the root descriptor for this transaction thread.  If the root
   // descriptor is null, a pointer to this descriptor will be returned *if* it is a transaction
   // descriptor.
   //
   // Return Value:
   //
   // evl_mem_desc: This is a handle to the root descriptor.
   //
   function evl_mem_desc get_root_desc();
      return ((m_root_desc != null) ? m_root_desc : ((m_desc_type == EVL_DESC_TRANS) ? this : null));
   endfunction : get_root_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_ancestor_desc
   //
   // This function is used to set the ancestor descriptor for this request thread.  The ancestor
   // descriptor at the head of the thread will either be null or point to itself.  Note that if
   // this descriptor is a transaction descriptor, m_ancestor_desc will not be set.
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the ancestor descriptor for the request thread.
   //
   function void set_ancestor_desc(input evl_mem_desc mem_desc_in);
      if (m_desc_type != EVL_DESC_TRANS) begin
         m_ancestor_desc = mem_desc_in;
      end
   endfunction : set_ancestor_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_ancestor_desc
   //
   // This function is used to get the ancestor descriptor for this request thread.  If the ancestor
   // descriptor is null, a pointer to this descriptor will be returned *if* it is a transaction
   // descriptor.
   //
   // Return Value:
   //
   // evl_mem_desc: This is a handle to the ancestor descriptor.
   //
   function evl_mem_desc get_ancestor_desc();
      return ((m_ancestor_desc != null) ? m_ancestor_desc : ((m_desc_type == EVL_DESC_TRANS) ? this : null));
   endfunction : get_ancestor_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_parent_desc
   //
   // This function is used to set the parent descriptor for this object.
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the parent descriptor for this object.
   //
   function void set_parent_desc(input evl_mem_desc mem_desc_in);
      m_parent_desc = mem_desc_in;
   endfunction : set_parent_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_parent_desc
   //
   // This function is used to get the parent descriptor for this object.  If there is no parent,
   // null will be returned.
   //
   // Return Value:
   //
   // evl_mem_desc: This is a handle to the parent descriptor.
   //
   function evl_mem_desc get_parent_desc();
      return m_parent_desc;
   endfunction : get_parent_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_child_desc
   //
   // This function is used to add a descriptor as a child of this descriptor.  Children are
   // quarantined into queues based on the descriptor type.  Thus, all children that are request
   // descriptors will be in one queue, and all children that are transaction descriptors will be in
   // another queue.  This function is extended in evl_trans_desc to simplify some usage for the
   // first request descriptor spawned from a transaction descriptor.
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
      evl_desc_type_t child_desc_type;

      child_desc_type = mem_desc_in.get_desc_type();
      if (!m_child_desc_queues.exists(child_desc_type)) begin
         m_child_desc_queues[child_desc_type] = new(get_abstract_name(), child_desc_type);
      end
      mem_desc_in.set_trans_id(get_trans_id());
      mem_desc_in.set_parent_desc(this);
      mem_desc_in.set_root_desc(get_root_desc());
      mem_desc_in.set_ancestor_desc(get_ancestor_desc());
      return m_child_desc_queues[child_desc_type].push_desc(mem_desc_in);
   endfunction : add_child_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_child_desc
   //
   // This function is used to get a specific child descriptor.  Because children are quarantined
   // into queues based on the descriptor type, it is necessary to specify what type of child
   // descriptor is desired.  By default, a handle to the child at the head of its queue is
   // returned, but you can also specify an index into the queue to get another child.
   //
   // Input Variables:
   //
   // desc_type:   This is the type of descriptor requested (e.g., EVL_DESC_REQ or EVL_DESC_TRANS).
   // child_index: This is the index to use for accessing handles in the specified queue.
   //
   // Return Value:
   //
   // evl_mem_desc: This is the child descriptor found at the specified index in the specified
   //               queue.  If no child descriptor exists there, null is returned.
   //
   function evl_mem_desc get_child_desc(input evl_desc_type_t desc_type,
                                        input int             child_index = 0);
      if (!m_child_desc_queues.exists(desc_type)) begin
         return null;
      end
      return m_child_desc_queues[desc_type].get_desc(child_index);
   endfunction : get_child_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: remove_child_desc
   //
   // This function is used to remove a descriptor from the m_child_desc_queues of this descriptor.
   // (if it exists)
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the descriptor that will be removed from the
   //              m_child_desc_queues list.
   //
   virtual function void remove_child_desc(input evl_mem_desc mem_desc_in);
      if (!m_child_desc_queues.exists(mem_desc_in.get_desc_type())) begin
         return;
      end
      void'(m_child_desc_queues[mem_desc_in.get_desc_type()].pop(mem_desc_in));
   endfunction : remove_child_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_parent
   //
   // This function is used to query whether this descriptor has a parent.
   //
   // Return Value:
   //
   // Returns 1 if this descriptor has a parent.
   //
   virtual function bit has_parent();
      if ((m_parent_desc == null) || (m_parent_desc == this)) begin
         return 1'b0;
      end
      return 1'b1;
   endfunction : has_parent


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_children
   //
   // This function is used to query whether there are any children of this descriptor.
   //
   // Return Value:
   //
   // Returns 1 if all there are any child descriptors of this descriptor.
   //
   virtual function bit has_children();
      foreach (m_child_desc_queues[desc_type]) begin
         if (m_child_desc_queues[desc_type].size() > 0) begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction : has_children


   //-----------------------------------------------------------------------------------------------
   //
   // Function: desc_family_is_done
   //
   // This function is used to query whether all descriptors in this descriptor family are done.
   //
   // Return Value:
   //
   // Returns 1 if all the child descriptors of this descriptor are done, otherwise returns 0.
   //
   virtual function bit desc_family_is_done();
      if ((m_parent_desc != null) && (m_parent_desc != this)) begin
         return m_parent_desc.desc_family_is_done();
      end
      return all_children_are_done();
   endfunction : desc_family_is_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: all_children_are_done
   //
   // This function is used to query whether all children descriptor of this descriptor are done.
   //
   // Return Value:
   //
   // Returns 1 if all the child descriptors of this descriptor are done, otherwise returns 0.
   //
   virtual function bit all_children_are_done();
      foreach (m_child_desc_queues[ii]) begin
         if (m_child_desc_queues[ii].all_children_are_done() == 1'b0) begin
            return 1'b0;
         end
      end
      return 1'b1;
   endfunction : all_children_are_done


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_root_trans_desc
   //
   // This function is provided mainly as a convenience.  It will cast the root descriptor into a
   // transaction descriptor if it is truly a transcation descriptor.  Note that a handle to this
   // object will be returned if the root descriptor is null and this object is a transaction
   // descriptor.
   //
   // Return Value:
   //
   // evl_trans_desc: This is a handle to the root transaction descriptor for this transaction
   //                 thread.
   //
   function evl_trans_desc get_root_trans_desc();
      evl_trans_desc trans_desc;

      if (m_root_desc == null) begin
         if (get_desc_type() == EVL_DESC_TRANS) begin
            $cast(trans_desc, m_root_desc);
            return trans_desc;
         end
         return null;
      end
      if (m_root_desc.get_desc_type() != EVL_DESC_TRANS) begin
         return null;
      end
      $cast(trans_desc, m_root_desc);
      return trans_desc;
   endfunction : get_root_trans_desc


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
      evl_req_desc req_desc;

      if (!m_child_desc_queues.exists(EVL_DESC_REQ)) begin
         return null;
      end
      $cast(req_desc, m_child_desc_queues[EVL_DESC_REQ].get_desc(child_index));
      return req_desc;
   endfunction : get_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_parent_req_desc
   //
   // This function is used to get a handle to the parent descriptor *only* if the parent is a
   // request descriptor (it is possible for the parent to be a transaction descriptor instead).
   //
   // Return Value:
   //
   // evl_req_desc: This is a handle to the parent descriptor.  If the parent descriptor is not a
   //               request descriptor, null will be returned.
   //
   function evl_req_desc get_parent_req_desc();
      evl_req_desc req_desc;

      if (!($cast(req_desc, m_parent_desc))) begin
         return null;
      end
      return req_desc;
   endfunction : get_parent_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_dep_desc
   //
   // This function is used to add a descriptor from which this descriptor is dependent.
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the descriptor which will be added to the dependency list
   //
   // Return Value:
   //
   // int: The index into the queue used for the child descriptor is returned.  Thus, if this is the
   //      fourth child added to the queue, a value of 3 is returned.  There may be no need to know
   //      about the return value.
   //
   virtual function int add_dep_desc(input evl_mem_desc mem_desc_in);
      m_depend_queue.push_back(mem_desc_in);
      return (m_depend_queue.size() - 1);
   endfunction : add_dep_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dep_desc
   //
   // This function is used to return a specific dependency descriptor
   //
   // Input Variables:
   //
   // index_in: This is the index in the dependency queue
   //
   // Return Value:
   //
   // evl_mem_desc: The descriptor at the provided index if it is valid, otherwise null
   //
   virtual function evl_mem_desc get_dep_desc(input int index_in);
      return ((index_in < 0) || (index_in >= m_depend_queue.size())) ? null : m_depend_queue[index_in];
   endfunction : get_dep_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: remove_dep_desc
   //
   // This function is used to remove a descriptor from the dependency list of this descriptor.
   //
   // Input Variables:
   //
   // mem_desc_in: This is a handle to the descriptor that will be removed from the dependency list
   //
   //
   virtual function void remove_dep_desc(input evl_mem_desc mem_desc_in);
      foreach (m_depend_queue[ii]) begin
         if (m_depend_queue[ii] == mem_desc_in) begin
            m_depend_queue.delete(ii);
         end
      end
   endfunction : remove_dep_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: depend_size
   //
   // This function is used to know how many items this descriptor depends on
   //
   // Input Variables:
   //
   // ignore_done: If this flag is set to 1, any dependency that has m_done == 1 will be ignored
   //
   // Return Value:
   //
   // int: If ignore_done is 1, returns the number of dependencies which are not done,
   //      otherwise return the number of dependencies.
   //
   virtual function int depend_size(input bit ignore_done = 1'b1);
      if (ignore_done == 1'b0) begin
         return m_depend_queue.size();
      end
      else begin
         int count = 0;

         foreach (m_depend_queue[ii]) begin
            if ((m_depend_queue[ii] != null) && (m_depend_queue[ii].is_done() == 1'b0)) begin
               count++;
            end
         end
         return count;
      end
   endfunction : depend_size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_dependency
   //
   // This function is used to know if this descriptor has dependencies.
   //
   // Input Variables:
   //
   // ignore_done: If this flag is set to 1, any dependency that has m_done == 1 will be ignored
   //
   // Return Value:
   //
   // bit: If ignore_done is 1, returns 1 if there are dependencies that are not done, else 0
   //      otherwise return 1 if there are dependencies, else 0
   //
   virtual function bit has_dependency(input bit ignore_done = 1'b1);
      return (depend_size(ignore_done) != 0) ? 1'b1 : 1'b0;
   endfunction : has_dependency


   //-----------------------------------------------------------------------------------------------
   //
   // Function: depends_on
   //
   // This function is used to know if this descriptor depends on another specific descriptor
   //
   // Input Variables:
   //
   // mem_desc_in: the descriptor for which a dependency is to be checked
   //
   // Return Value:
   //
   // bit: return 1 if the given descriptor is found in this descriptor's dependency list,
   //      otherwise return 0
   //
   virtual function bit depends_on(input evl_mem_desc mem_desc_in);
      foreach (m_depend_queue[ii]) begin
         if (m_depend_queue[ii] == mem_desc_in) begin
            return 1'b1;
         end
      end
      return 1'b0;
   endfunction : depends_on


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_dep_descs
   //
   // This function prints the descriptors on which this one depends
   //
   virtual function void print_dep_descs(input bit                 ignore_done         = 1'b1,
                                         input int                 verbosity_in        = int'(UVM_MEDIUM),
                                         input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL);
      foreach (m_depend_queue[ii]) begin
         if ((ignore_done == 1'b0) || (m_depend_queue[ii].is_done() == 1'b0)) begin
            `evl_log_mdesc(verbosity_in, verbosity_module_in, get_abstract_name(), this, $sformatf("m_depend_queue[%0d] => %0s (TRID -%10x-)%0s",
               ii, m_depend_queue[ii].sprint_obj(), m_depend_queue[ii].get_trans_id(), (m_depend_queue[ii].is_done() == 1'b1) ? " (done)" : ""))
         end
      end
   endfunction : print_dep_descs


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_tag_id
   //
   // This function is used to get an appropriate tag ID for logging.  For request descriptors, this
   // function gets redefined to return the request ID.
   //
   // Return Value:
   //
   // int: This is an integer representing the appropriate tag ID.
   //
   virtual function int get_tag_id();
      return -1;
   endfunction : get_tag_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_delay_wrapper
   //
   // This function is used to set the delay wrapper for this descriptor.  A delay wrapper contains
   // one or more delays that can be selected by a delay key (such as pk_req_delay or pk_rsp_delay).
   // Bus drivers and RTL backpressure drivers use delays to apply stimulus at the correct time.
   //
   // Input Variables:
   //
   // delay_wrapper_in: This is a handle to the delay wrapper to use for this descriptor.
   //
   function void set_delay_wrapper(input evl_delay_wrapper delay_wrapper_in);
      m_delay_wrapper = delay_wrapper_in;
   endfunction : set_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay_wrapper
   //
   // This function is used to get a handle to the delay wrapper in this descriptor.
   //
   // Return Value:
   //
   // evl_delay_wrapper: This is a handle to the delay wrapper in this descriptor.
   //
   function evl_delay_wrapper get_delay_wrapper();
      return m_delay_wrapper;
   endfunction : get_delay_wrapper


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_delay
   //
   // This function is used to remove a specific delay from the delay wrapper in this descriptor.
   // Delays are specified by delay keys, which are simply integer values created by the
   // `evl_param_key macro.  By convention, delays are named "pk_<type>_delay".
   //
   // Input Variables:
   //
   // delay_key: This is the integer mapped to the desired delay key name.
   //
   function void reset_delay(input int delay_key);
      if (m_delay_wrapper == null) begin
         return;
      end
      m_delay_wrapper.reset_delay(delay_key);
   endfunction : reset_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_delay
   //
   // This function is used to add a specific delay from the delay wrapper in this descriptor.
   //
   // Input Variables:
   //
   // delay_key:           This is the integer mapped to the desired delay key name.
   // delay_in:            This is the delay value to add for that key.
   // force_on_delay_list: If this input is set to 1'b1, the delay will be forced into a queue of
   //                      delays for that delay key.  Once a queue of delays is created for a
   //                      specific delay key, subsequent add_delay() calls for the same delay key
   //                      will cause the queue to grow.  If a specific delay key has not been
   //                      converted to a queue, multiple calls to add_delay() for that key will
   //                      simply overwrite a single value.
   //
   function void add_delay(input int delay_key, input evl_delay_t delay_in, input bit force_on_delay_list = 1'b0);
      if (m_delay_wrapper == null) begin
         m_delay_wrapper = new($sformatf("%0s(DLY)", get_abstract_name()));
      end
      m_delay_wrapper.add_delay(delay_key, delay_in, force_on_delay_list);
   endfunction : add_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_delay
   //
   // This function is used to determine if a specific delay is available in this descriptor.
   // Because it is possible to have a queue of delays for a specific delay key, an optional index
   // into the queue can be specified.
   //
   // Input Variables:
   //
   // delay_key: This is the integer mapped to the desired delay key name.
   // index:     This is an index into a delay queue.  If the specified delay is not in a queue, any
   //            index value other than 0 will cause this function to return 1'b0.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the specified delay is in this descriptor.
   //
   function bit have_delay(input int delay_key, input int index = 0);
      if (m_delay_wrapper == null) begin
         return 1'b0;
      end
      return m_delay_wrapper.have_delay(delay_key, index);
   endfunction : have_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_delay
   //
   // This function is used to get the value for a specific delay in this descriptor.  Because it is
   // possible to have a queue of delays for a specific delay key, an optional index into the queue
   // can be specified.
   //
   // Output Variables:
   //
   // delay_out: This is the value of the requested delay.  If the delay cannot be found, this
   //            output will be 0.
   //
   // Input Variables:
   //
   // delay_key: This is the integer mapped to the desired delay key name.
   // index:     This is an index into a delay queue.  If the specified delay is not in a queue, any
   //            index value other than 0 will cause this function to return 1'b0 and the delay_out
   //            value to be 0.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the specified delay is in this descriptor.
   //
   function bit get_delay(output evl_delay_t delay_out, input int delay_key, input int index = 0);
      if (m_delay_wrapper == null) begin
         delay_out = evl_delay_t'(0);
         return 1'b0;
      end
      return m_delay_wrapper.get_delay(delay_out, delay_key, index);
   endfunction : get_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_next_delay
   //
   // This function is used to get the next available value for a specific delay in this descriptor.
   // If the specified delay is not in a queue, the same value will be returned on each call to
   // get_next_delay().  If the specified delay is in a queue, values from progressively deeper
   // entries in the queue will be returned for each get_next_delay() call; once the last delay in
   // the queue is reached, the next call to get_next_delay() will get the 0th entry, starting the
   // whole process over again.
   //
   // Output Variables:
   //
   // delay_out: This is the value of the requested delay.  If the delay cannot be found, this
   //            output will be 0.
   //
   // Input Variables:
   //
   // delay_key: This is the integer mapped to the desired delay key name.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the specified delay is in this descriptor.
   //
   function bit get_next_delay(output evl_delay_t delay_out, input int delay_key, input evl_delay_t default_delay = evl_delay_t'(0));
      if (m_delay_wrapper == null) begin
         delay_out = default_delay;
         return 1'b0;
      end
      return m_delay_wrapper.get_next_delay(delay_out, delay_key, default_delay);
   endfunction : get_next_delay


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_timeout_value
   //
   // This function is used to set the current timeout counter.
   //
   // Input Variables:
   //
   // timeout_value_in: This is value to which to set the timeout counter.
   //
   function void set_timeout_value(input int timeout_value_in);
      m_timeout_value = timeout_value_in;
   endfunction : set_timeout_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: reset_timeout_value
   //
   // This function is used to reset the current timeout counter to its default value.
   //
   function void reset_timeout_value();
      m_timeout_value = m_default_timeout;
   endfunction : reset_timeout_value


   //-----------------------------------------------------------------------------------------------
   //
   // Function: check_for_timeout
   //
   // This function is used to reset the current timeout counter to its default value.
   //
   function bit check_for_timeout(output bit now_done, input int cycle_decrement);
      if (is_done() == 1'b1) begin
         now_done = 1'b1;
         return 0;
      end
      now_done = 1'b0;
      m_timeout_value -= cycle_decrement;
      return ((m_timeout_value <= 0) ? 1'b1 : 1'b0);
   endfunction : check_for_timeout


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_bus_type
   //
   // This function is used to set the bus type for a request descriptor.  There are currently two
   // supported bus types: EVL_BUS_TYPE_ETLINK and EVL_BUS_TYPE_AXI.
   //
   // Input Variables:
   //
   // delay_key: This is the bus type to use for a request descriptor.
   //
   virtual function void set_bus_type(input evl_bus_type_t bus_type_in);
      return;
   endfunction : set_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_type
   //
   // This function is used to get the bus type for a request descriptor.
   //
   // Return Value:
   //
   // evl_bus_type_t: This function returns the bus type for a request descriptor.
   //
   virtual function evl_bus_type_t get_bus_type();
      return EVL_BUS_TYPE_UNKNOWN;
   endfunction : get_bus_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_cmd_name
   //
   // This function is used to get a name for the specified bus command.  If the input variable is
   // `EVL_Bus_Idle, the bus command in the request descriptor is used for evaluation.  If it is
   // something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // string: This function returns the command name for the evaluated bus command.
   //
   virtual function string get_bus_cmd_name(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return "EVL_Bus_Idle";
   endfunction : get_bus_cmd_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_requires_rsp
   //
   // This function is used to determine if the specified bus command requires a response.  If the
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
   // bit: This function returns 1'b1 if the evaluated bus command requires a response.
   //
   virtual function bit bus_cmd_requires_rsp(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
   endfunction : bus_cmd_requires_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_read
   //
   // This function is used to determine if the specified bus command is a read.  If the input
   // variable is `EVL_Bus_Idle, the bus command in the request descriptor is used for evaluation.
   // If it is something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a read.
   //
   virtual function bit bus_cmd_is_read(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
   endfunction : bus_cmd_is_read


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_write
   //
   // This function is used to determine if the specified bus command is a write.  If the input
   // variable is `EVL_Bus_Idle, the bus command in the request descriptor is used for evaluation.
   // If it is something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command is a write.
   //
   virtual function bit bus_cmd_is_write(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
   endfunction : bus_cmd_is_write


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_cache_op
   //
   // This function is used to determine if the specified bus command is a cache operation.  If the
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
   // bit: This function returns 1'b1 if the evaluated bus command is a cache operation.
   //
   virtual function bit bus_cmd_is_cache_op(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
   endfunction : bus_cmd_is_cache_op


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_scratchpad
   //
   // This function is used to determine if the specified bus command is a scratchpad operation.  If
   // the input variable is `EVL_Bus_Idle, the bus command in the request descriptor is used for
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
   // bit: This function returns 1'b1 if the evaluated bus command is a scratchpad operation.
   //
   virtual function bit bus_cmd_is_scratchpad(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
   endfunction : bus_cmd_is_scratchpad


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_is_atomic
   //
   // This function is used to determine if the specified bus command is an atomic operation.  If
   // the input variable is `EVL_Bus_Idle, the bus command in the request descriptor is used for
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
   // bit: This function returns 1'b1 if the evaluated bus command is an atomic operation.
   //
   virtual function bit bus_cmd_is_atomic(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
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
      return 1'b0;
   endfunction : bus_cmd_is_acquire


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_read_data
   //
   // This function is used to determine if the specified bus command expects read data.  If the
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
   // bit: This function returns 1'b1 if the evaluated bus command expects read data.
   //
   virtual function bit bus_cmd_has_read_data(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
      return 1'b0;
   endfunction : bus_cmd_has_read_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_cmd_has_write_data
   //
   // This function is used to determine if the specified bus command has write data.  If the input
   // variable is `EVL_Bus_Idle, the bus command in the request descriptor is used for evaluation.
   // If it is something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command has write data.
   //
   virtual function bit bus_cmd_has_write_data(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle);
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
   // it is illegal to reuse a particular bus tag before a response is sent.  If the input variable
   // is `EVL_Bus_Idle, the bus command in the request descriptor is used for evaluation.  If it is
   // something other than `EVL_Bus_Idle, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_cmd_in: This is the bus command to use for evaluation.  If it is `EVL_Bus_Idle, the bus
   //             command in the request descriptor is used for evaluation.
   //
   // Return Value:
   //
   // bit: This function returns 1'b1 if the evaluated bus command should use a unique bus tag.
   //
   virtual function bit bus_cmd_has_unique_tag(input evl_bus_cmd_t bus_cmd_in = `EVL_Bus_Idle, input evl_bus_type_t bus_type_in = EVL_BUS_TYPE_UNKNOWN);
      return 1'b0;
   endfunction : bus_cmd_has_unique_tag


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_bus_rsp_name
   //
   // This function is used to get a name for the specific bus response type.  If the input variable
   // is `EVL_Rsp_Null, the bus response type in the request descriptor is used for evaluation.  If
   // it is something other than `EVL_Rsp_Null, the input variable is used for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in the request descriptor is used for evaluation.
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
      return "EVL_Rsp_Null";
   endfunction : get_bus_rsp_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_data
   //
   // This function is used to determine if the specified bus response type has data.  If the input
   // variable is `EVL_Rsp_Null, the bus response type in the request descriptor is used for
   // evaluation.  If it is something other than `EVL_Rsp_Null, the input variable is used for
   // evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in the request descriptor is used for evaluation.
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
      return 1'b0;
   endfunction : bus_rsp_has_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: bus_rsp_has_error
   //
   // This function is used to determine if the specified bus response type is an error response.
   // If the input variable is `EVL_Rsp_Null, the bus response type in the request descriptor is
   // used for evaluation.  If it is something other than `EVL_Rsp_Null, the input variable is used
   // for evaluation.
   //
   // Input Variables:
   //
   // bus_rsp_in:      This is the bus response type to use for evaluation.  If it is `EVL_Bus_Idle,
   //                  the bus response type in the request descriptor is used for evaluation.
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
      return 1'b0;
   endfunction : bus_rsp_has_error


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_cmd
   //
   // This function is used to format a command with address.
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the command and addres sinformation.
   //
   virtual function string sprint_cmd();
      if (get_cmd() inside { ATOMIC }) begin
         int             atomic_conf;
         int             atomic_size;
         evl_atomic_op_t atomic_op;
         string          atomic_op_str;

         atomic_conf = int'(get_bus_atomic_conf());
         atomic_op   = evl_atomic_op_t'(atomic_conf & `EVL_BUS_ATOMIC_OP_MASK);
         atomic_size = ((atomic_conf & `EVL_BUS_ATOMIC_CONF_DW_OP) == 0) ? 4 : 8;
         if ((atomic_conf & `EVL_BUS_ATOMIC_CONF_VEC_OP) == 0) begin
            if (atomic_op inside { SC_AMO_0C, SC_AMO_0D, SC_AMO_0E, SC_AMO_0F }) begin
               atomic_op_str = $sformatf("0x%0x (", int'(atomic_op));
            end
            else begin
               atomic_op_str = $sformatf("%s (", atomic_op.name());
               atomic_op_str = atomic_op_str.substr(3, atomic_op_str.len() - 1);
            end
         end
         else begin
            if (atomic_op inside { SC_AMOCOMPEXCH, SC_AMO_0C, SC_AMO_0D, SC_AMO_0E, SC_AMO_0F }) begin
               atomic_op_str = $sformatf("0x%0x (vec, ", int'(atomic_op));
            end
            else begin
               atomic_op_str = $sformatf("%s (vec, ", atomic_op.name());
               atomic_op_str = atomic_op_str.substr(3, atomic_op_str.len() - 1);
            end
         end
         if ((atomic_conf & `EVL_BUS_ATOMIC_CONF_L3_OP) == 0) begin
            atomic_op_str = $sformatf("%s%0dB)", atomic_op_str, atomic_size);
         end
         else begin
            atomic_op_str = $sformatf("%s%0dB, L3)", atomic_op_str, atomic_size);
         end
         return $sformatf("%p %s 0x%x", get_cmd(), atomic_op_str, get_paddr());
      end
      else if (get_cmd() inside { CACHE_SCP_FILL }) begin
         if (get_bus_scp_fill_addr() == 'h0) begin  // if scp_fill_addr has NOT been assgined
            if (vaddr_is_valid() == 1'b1) begin
               return $sformatf("%p 0x%x (va)", get_cmd(), get_vaddr());
            end
            else begin
               return $sformatf("%p 0x%x (pa)", get_cmd(), get_paddr());
            end
         end
         else begin  // if scp_fill_addr has been assgined
            if (paddr_is_valid() == 1'b1) begin
               return $sformatf("%p 0x%x -> 0x%x", get_cmd(), get_bus_scp_fill_addr(), get_paddr());
            end
            else begin
               return $sformatf("%p 0x%x -> 0x%x", get_cmd(), get_bus_scp_fill_addr(), get_vaddr());
            end
         end
      end
      else if (!(get_cmd() inside { `EVL_JTAG_CMDS })) begin
         if (vaddr_is_valid() == 1'b1) begin
            return $sformatf("%p 0x%x (va)", get_cmd(), get_vaddr());
         end
         return $sformatf("%p 0x%x", get_cmd(), get_paddr());
      end
      else if (get_cmd() inside { JTAG_RESET, JTAG_RUN_TEST }) begin
         return $sformatf("%p", get_cmd());
      end
      else if (get_cmd() inside { JTAG_STEP }) begin
         return $sformatf("%p %0d %0d", get_cmd(), get_paddr() & evl_paddr_t'(1), (get_paddr() >> 1) & evl_paddr_t'(1));
      end
      else begin
         return $sformatf("%p %0d", get_cmd(), get_paddr());
      end
   endfunction : sprint_cmd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to print request information consistently.  It is intended to be
   // redefined in extended classes to print useful, concise information for requests.
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
   // string: This is a formatted string to be used for logging.
   //
   virtual function string sprint_obj(input int style_in = 0, input int transfer_idx_in = -1);
      return $sformatf("%s (id %0d%0s)", sprint_cmd(), get_inst_id(), ((is_done() == 1'b1) ? ", done" : ""));
   endfunction : sprint_obj


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_delays
   //
   // This function is used to the delay wrapper contents.
   //
   // Return Value:
   //
   // string: This is a formatted string to be used for logging.
   //
   virtual function string sprint_delays();
      if (m_delay_wrapper != null) begin
         return m_delay_wrapper.sprint_obj();
      end
      return "";
   endfunction : sprint_delays


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_rsp
   //
   // This function is used to print response information consistently.  It is intended to be
   // redefined in extended classes to print useful, concise information for responses.
   //
   // Input Variables:
   //
   // dword_start: This is the starting doubleword for the response.
   // dword_end:   This is the ending doubleword for the response.
   //
   // Return Value:
   //
   // string: This is a formatted string to be used for logging.
   //
   virtual function string sprint_rsp(input int dword_start = 0, input int dword_end = `EVL_LINE_DWORDS - 1);
      return "";
   endfunction : sprint_rsp


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_from_root
   //
   // This function is used to print the descriptor chain starting from the root descriptor.
   //
   function void print_from_root(input int verbosity_in = int'(UVM_MEDIUM), input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL);
      if (`evl_log_check_verbosity(verbosity_in, verbosity_module_in) == 0) begin
         return;
      end
      if ((m_root_desc != this) && (m_root_desc != null)) begin
         m_root_desc.print_from_this();
      end
      else begin
         print_from_this();
      end
   endfunction : print_from_root


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_from_ancestor
   //
   // This function is used to print the descriptor chain starting from the ancestor descriptor.
   //
   function void print_from_ancestor(input int verbosity_in = int'(UVM_MEDIUM), input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL);
      if (`evl_log_check_verbosity(verbosity_in, verbosity_module_in) == 0) begin
         return;
      end
      if ((m_ancestor_desc != this) && (m_ancestor_desc != null)) begin
         m_ancestor_desc.print_from_this();
      end
      else begin
         print_from_this();
      end
   endfunction : print_from_ancestor


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_from_this
   //
   // This function is used to print the descriptor chain starting from this descriptor.
   //
   function void print_from_this(input int                 verbosity_in = int'(UVM_MEDIUM),
                                 input evl_verbosity_mod_t verbosity_module_in = VMOD_GLOBAL,
                                 input string              prefix = "");
      if (`evl_log_check_verbosity(verbosity_in, verbosity_module_in) == 0) begin
         return;
      end
      `evl_log_mdesc(verbosity_in, verbosity_module_in, get_abstract_name(), this, $sformatf("%0s%0s", prefix, sprint_obj()))
      if (prefix == "") begin
         prefix = "+ ";
      end
      else begin
         prefix = $sformatf("  %0s", prefix);
      end
      foreach (m_child_desc_queues[ii]) begin
         m_child_desc_queues[ii].print_from_this(verbosity_in, verbosity_module_in, prefix);
      end
   endfunction : print_from_this


   //-----------------------------------------------------------------------------------------------
   //
   // Function: copy_to_desc
   //
   // This function is used to copy information from this descriptor into a new descriptor.  This
   // function is intended to be used to copy shared information from a transaction descriptor into
   // a request descriptor.
   //
   // Input Variables:
   //
   // mem_desc: This is a handle to the descriptor in which to copy information.
   //
   function void copy_to_desc(input evl_mem_desc mem_desc);
      mem_desc.m_mem_desc_attrs = m_mem_desc_attrs;
   endfunction : copy_to_desc


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
      evl_mem_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_mem_desc_attrs = source_obj.m_mem_desc_attrs;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // reset_create_time
   //
   // This function is used to turn off status collection for this descriptor.
   //
   function void reset_create_time();
      m_create_time = 0;
   endfunction : reset_create_time


   //-----------------------------------------------------------------------------------------------
   //
   // get_create_time
   //
   // This function is used to get the creation time for this descriptor.
   //
   function realtime get_create_time();
      return m_create_time;
   endfunction : get_create_time


   //-----------------------------------------------------------------------------------------------
   //
   // add_killed_trans_desc
   //
   // This function is used to build the list of killed transaction descriptors.
   //
   function void add_killed_trans_desc(input evl_trans_desc trans_desc);
      m_killed_trans_descs.push_back(trans_desc);
   endfunction : add_killed_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // add_killed_req_desc
   //
   // This function is used to build the list of killed request descriptors.
   //
   function void add_killed_req_desc(input evl_req_desc req_desc);
      m_killed_req_descs.push_back(req_desc);
   endfunction : add_killed_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // get_killed_trans_desc
   //
   // This function is used to extract the list of killed transaction descriptors.
   //
   function evl_trans_desc get_killed_trans_desc(input int index);
      if ((index < 0) || (index >= m_killed_trans_descs.size())) begin
         return null;
      end
      return m_killed_trans_descs[index];
   endfunction : get_killed_trans_desc


   //-----------------------------------------------------------------------------------------------
   //
   // get_killed_req_desc
   //
   // This function is used to extract the list of killed request descriptors.
   //
   function evl_req_desc get_killed_req_desc(input int index);
      if ((index < 0) || (index >= m_killed_req_descs.size())) begin
         return null;
      end
      return m_killed_req_descs[index];
   endfunction : get_killed_req_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_mem_desc");
      super.new(name_in);
      m_timeout_value = m_default_timeout;
      if (m_get_stats == 1'b1) begin
         m_create_time = $realtime;
      end
   endfunction : new

endclass : evl_mem_desc
