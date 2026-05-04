//--------------------------------------------------------------------------------------------------
//
// Class: evl_pmu_desc
//
class evl_pmu_desc extends evl_mem_desc;

   `uvm_object_utils(evl_pmu_desc)

   //-----------------------------------------------------------------------------------------------
   //
   // Local Type Definitions
   //
   // vm_mode:         This is the virtual memory mode in which this request operates.
   //                  -----------------------------------------------------------------------------
   // root_ppn:        This is the root physical page number used by this request
   //                  -----------------------------------------------------------------------------
   // privilege:       This is the privilege level of this request (user/supervisor/machine)
   //
   typedef struct packed unsigned {
      evl_pmu_cntr_id_t       cntr_id;
      evl_pmu_event_type_t    event_type;
      evl_pmu_txn_type_t      txn_type;
      evl_dword_t             cntr_data;
   } evl_pmu_desc_req_s;


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_req_attrs:     This packed array contains commonly-copied variables when copying request
   //                  information from one request descriptor to another.
   //                  -----------------------------------------------------------------------------
   // m_observed:      This bit is set to 1'b1 if the request descriptor has been observed by a
   //                  downstream object (such as a slave agent or slave scoreboard).
   //                  -----------------------------------------------------------------------------
   // m_rsp_pte:       This is a handle to the page table entry object containing the response
   //                  -----------------------------------------------------------------------------
   // m_access_fault:  This bit is set to 1'b1 if the request caused an access fault
   //
   evl_pmu_desc_req_s m_req_attrs    = '{ cntr_id:    evl_pmu_cntr_id_t'(0),
                                          event_type: evl_pmu_event_type_t'(0),
                                          txn_type:   evl_pmu_txn_type_t'(0),
                                          cntr_data:  evl_dword_t'(0)
                                        };
   //bit                  m_observed     = 1'b0;
   //evl_page_table_entry m_rsp_pte      = null;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cntr_id
   //
   //   This function is used to set the PMU counter ID used by this request
   //
   virtual function void set_cntr_id(input evl_pmu_cntr_id_t cntr_id_in);
      m_req_attrs.cntr_id = cntr_id_in;
   endfunction : set_cntr_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cntr_id
   //
   //   This function is used to get the PMU counter ID used by this request
   //
   virtual function evl_pmu_cntr_id_t get_cntr_id();
      return m_req_attrs.cntr_id;
   endfunction : get_cntr_id


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_event_type
   //
   //   This function is used to set the PMU event type used by this request
   //
   virtual function void set_event_type(input evl_pmu_event_type_t event_type_in);
      m_req_attrs.event_type = event_type_in;
   endfunction : set_event_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_event_type
   //
   //   This function is used to get the PMU event type used by this request
   //
   virtual function evl_pmu_event_type_t get_event_type();
      return m_req_attrs.event_type;
   endfunction : get_event_type

   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_txn_type
   //
   //   This function is used to set the PMU transaction type used by this request
   //
   virtual function void set_txn_type(input evl_pmu_txn_type_t txn_type_in);
      m_req_attrs.txn_type = txn_type_in;
   endfunction : set_txn_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_txn_type
   //
   //   This function is used to get the PMU transaction type used by this request
   //
   virtual function evl_pmu_txn_type_t get_txn_type();
      return m_req_attrs.txn_type;
   endfunction : get_txn_type


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_cntr_data
   //
   //   This function is used to set the PMU counter data used by this request
   //
   virtual function void set_cntr_data(input evl_dword_t cntr_data_in);
      m_req_attrs.cntr_data = cntr_data_in;
   endfunction : set_cntr_data


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_cntr_data
   //
   //   This function is used to get the PMU counter data used by this request
   //
   virtual function evl_dword_t get_cntr_data();
      return m_req_attrs.cntr_data;
   endfunction : get_cntr_data

   //-----------------------------------------------------------------------------------------------
   //
   // Function: rsp_is_valid
   //
   //   This function returns 1 if the descriptor has a valid page table entry response set
   //
   //virtual function bit rsp_is_valid();
   //   return (m_rsp_pte == null) ? 1'b0 : 1'b1;
   //endfunction : rsp_is_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_observed
   //
   //virtual function void set_observed();
   //   m_observed = 1'b1;
   //endfunction : set_observed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_been_observed
   //
   //virtual function bit has_been_observed();
   //   return m_observed;
   //endfunction : has_been_observed


   //-----------------------------------------------------------------------------------------------
   //
   // Function: sprint_obj
   //
   // This function is used to format a request string
   //
   // Input Variables:
   //
   // Return Value:
   //
   // string: This function returns a formatted string with the request information.
   //
   virtual function string sprint_obj(input int style_in = 0, input int transfer_idx_in = -1);
      return $sformatf("%p for mhpmcounter%0d: 0x%x", get_txn_type(), get_cntr_id(), get_event_type());
   endfunction : sprint_obj


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
      evl_pmu_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_req_attrs  = source_obj.m_req_attrs;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_mem_desc", input evl_verif_comp agent_in = null);
      super.new(name_in);
      m_desc_type = EVL_DESC_PMU;
      if (agent_in != null) begin
         m_master_agent = agent_in;
      end
   endfunction : new

endclass : evl_pmu_desc

