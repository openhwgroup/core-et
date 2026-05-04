//--------------------------------------------------------------------------------------------------
//
// Class: evl_ptw_desc
//
class evl_ptw_desc extends evl_mem_desc;

   `uvm_object_utils(evl_ptw_desc)

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
      evl_vm_mode_t     vm_mode;
      evl_paddr_t       root_ppn;
      evl_privilege_t   privilege;
   } evl_ptw_desc_req_s;


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
   evl_ptw_desc_req_s m_req_attrs    = '{ vm_mode:     vm_mode_bare,
                                          root_ppn:    evl_paddr_t'(0),
                                          privilege:   prv_user
                                        };
   bit                  m_observed     = 1'b0;
   evl_page_table_entry m_rsp_pte      = null;
   bit                  m_access_fault = 1'b0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_vm_mode
   //
   //   This function is used to set the virtual memory mode used by this request
   //
   virtual function void set_vm_mode(input evl_vm_mode_t vm_mode_in);
      m_req_attrs.vm_mode = vm_mode_in;
   endfunction : set_vm_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_vm_mode
   //
   //   This function is used to get the virtual memory mode used by this request
   //
   virtual function evl_vm_mode_t get_vm_mode();
      return m_req_attrs.vm_mode;
   endfunction : get_vm_mode


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_root_ppn
   //
   //   This function is used to set the root physical page number used by this request
   //
   virtual function void set_root_ppn(input evl_paddr_t root_ppn_in);
      m_req_attrs.root_ppn = root_ppn_in & const_page_paddr_mask;
   endfunction : set_root_ppn


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_root_ppn
   //
   //   This function is used to get the root physical page number used by this request
   //
   virtual function evl_paddr_t get_root_ppn();
      return m_req_attrs.root_ppn;
   endfunction : get_root_ppn


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_privilege
   //
   //   This function is used to set the privilege level of this request
   //
   virtual function void set_privilege(input evl_privilege_t privilege_in);
      m_req_attrs.privilege = privilege_in;
   endfunction : set_privilege


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_privilege
   //
   //   This function is used to get the privilege level of this request
   //
   virtual function evl_privilege_t get_privilege();
      return m_req_attrs.privilege;
   endfunction : get_privilege




   //-----------------------------------------------------------------------------------------------
   //
   // Function: rsp_is_valid
   //
   //   This function returns 1 if the descriptor has a valid page table entry response set
   //
   virtual function bit rsp_is_valid();
      return (m_rsp_pte == null) ? 1'b0 : 1'b1;
   endfunction : rsp_is_valid


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_rsp_pte
   //
   //   This function is used to set the response page table entry object 
   //
   virtual function void set_rsp_pte(input evl_page_table_entry pte_in);
      m_rsp_pte = pte_in;
   endfunction : set_rsp_pte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_rsp_pte
   //
   //   This function is used to get the response page table entry object 
   //
   virtual function evl_page_table_entry get_rsp_pte();
      return m_rsp_pte;
   endfunction : get_rsp_pte


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_access_fault
   //
   //   This function is used to specify that this request caused an access fault
   //
   virtual function void set_access_fault();
      m_access_fault = 1'b1;
   endfunction : set_access_fault


   //-----------------------------------------------------------------------------------------------
   //
   // Function: clr_access_fault
   //
   //   This function is used to clear the access fault indication
   //
   virtual function void clr_access_fault();
      m_access_fault = 1'b0;
   endfunction : clr_access_fault


   //-----------------------------------------------------------------------------------------------
   //
   // Function: has_access_fault
   //
   //   This function returns 1 if the request caused an access fault
   //
   virtual function bit has_access_fault();
      return m_access_fault;
   endfunction : has_access_fault


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_observed
   //
   virtual function void set_observed();
      set_issued();
      m_observed = 1'b1;
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
      return $sformatf("PTW Req for VA: 0x%x PA: 0x%x", get_vaddr(), get_paddr());
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
      evl_ptw_desc source_obj;

      super.do_copy(rhs);
      $cast(source_obj, rhs);
      m_req_attrs  = source_obj.m_req_attrs;
   endfunction : do_copy


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compare_req_attrs
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
   // virtual function bit compare_req_attrs(input  evl_ptw_desc exp_req_desc,
   //                                        output string       err_messages_out[]);
   //    string err_messages[$];

   //    if (get_bus_cmd() != exp_req_desc.get_bus_cmd()) begin
   //       err_messages.push_back($sformatf("bus_cmd mismatch: have %0p, expected %0p", get_bus_cmd(), exp_req_desc.get_bus_cmd()));
   //    end
   //    if (get_bus_bank() != exp_req_desc.get_bus_bank()) begin
   //       err_messages.push_back($sformatf("bus_bank mismatch: have %0p, expected %0p", get_bus_bank(), exp_req_desc.get_bus_bank()));
   //    end
   //    if (get_bus_req_id() != exp_req_desc.get_bus_req_id()) begin
   //       err_messages.push_back($sformatf("bus_req_id mismatch: have %0p, expected %0p", get_bus_req_id(), exp_req_desc.get_bus_req_id()));
   //    end
   //    if (get_bus_req_size() != exp_req_desc.get_bus_req_size()) begin
   //       err_messages.push_back($sformatf("bus_req_size mismatch: have %0p, expected %0p", get_bus_req_size(), exp_req_desc.get_bus_req_size()));
   //    end
   //    if (get_bus_req_len() != exp_req_desc.get_bus_req_len()) begin
   //       err_messages.push_back($sformatf("bus_req_len mismatch: have %0p, expected %0p", get_bus_req_len(), exp_req_desc.get_bus_req_len()));
   //    end
   //    if (get_bus_lock() != exp_req_desc.get_bus_lock()) begin
   //       err_messages.push_back($sformatf("bus_lock mismatch: have %0p, expected %0p", get_bus_lock(), exp_req_desc.get_bus_lock()));
   //    end
   //    if (get_bus_cache_attr() != exp_req_desc.get_bus_cache_attr()) begin
   //       err_messages.push_back($sformatf("bus_cache_attr mismatch: have %0p, expected %0p", get_bus_cache_attr(), exp_req_desc.get_bus_cache_attr()));
   //    end
   //    if (get_bus_prot() != exp_req_desc.get_bus_prot()) begin
   //       err_messages.push_back($sformatf("bus_prot mismatch: have %0p, expected %0p", get_bus_prot(), exp_req_desc.get_bus_prot()));
   //    end
   //    if (get_bus_priority() != exp_req_desc.get_bus_priority()) begin
   //       err_messages.push_back($sformatf("bus_priority mismatch: have %0p, expected %0p", get_bus_priority(), exp_req_desc.get_bus_priority()));
   //    end
   //    if (get_bus_region() != exp_req_desc.get_bus_region()) begin
   //       err_messages.push_back($sformatf("bus_region mismatch: have %0p, expected %0p", get_bus_region(), exp_req_desc.get_bus_region()));
   //    end
   //    if (get_bus_req_user() != exp_req_desc.get_bus_req_user()) begin
   //       err_messages.push_back($sformatf("bus_req_user mismatch: have %0p, expected %0p", get_bus_req_user(), exp_req_desc.get_bus_req_user()));
   //    end

   //    if (err_messages.size() > 0) begin
   //       err_messages_out = {};
   //    end
   //    else begin
   //       err_messages_out = new[err_messages.size()];

   //       foreach (err_messages[ii]) begin
   //          err_messages_out[ii] = err_messages[ii];
   //       end
   //    end
   //    return (err_messages.size() > 0);
   // endfunction : compare_req_attrs


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_mem_desc", input evl_verif_comp agent_in = null);
      super.new(name_in);
      m_desc_type = EVL_DESC_PTW;
      if (agent_in != null) begin
         m_master_agent = agent_in;
      end
   endfunction : new

endclass : evl_ptw_desc

