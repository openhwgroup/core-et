//--------------------------------------------------------------------------------------------------
//
// Class: evl_memshire_scoreboard
//
//
class evl_memshire_scoreboard extends evl_scoreboard_mem;

   `uvm_component_utils(evl_memshire_scoreboard)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // pk_etlink_cmd:            This parameter key is used to store the ET link command as an int
   //                           param in the request descriptor.  For AXI store operations, the ET
   //                           link command is encoded in the AWUSER bits.  For the memshire, the
   //                           only ET link command of any real value is ET_LINK_REQ_Atomic, but
   //                           when a new request is observed we always assign an ET link command
   //                           (ET_LINK_Req_Write or ET_LINK_Req_Read, for example).
   //                           --------------------------------------------------------------------
   // pk_ms_sb_error_intensity: This parameter key is used to alter the effect of DUT errors
   //                           (convert them to warnings of variable verbosity).
   //
   `evl_param_key(pk_debug_no_inst)
   `evl_param_key(pk_etlink_cmd)
   `evl_param_key(pk_ms_sb_error_intensity)
   `evl_param_key(pk_desc_is_atomic)


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_cache:                   This class instance is the memory model.  It contains state and
   //                            data for every cache-line sized chunk of data in DDR.
   //                            -------------------------------------------------------------------
   // m_memshire_rtl_port:       This class handle is a copy of the m_rtl_port handle cast
   //                            appropriately.
   //                            -------------------------------------------------------------------
   // m_template_req_data_axi:   This class instance is a request data template to be used when
   //                            creating new request descriptors.
   //                            -------------------------------------------------------------------
   // m_template_line_desc_axi:  This class instance is a line descriptor template to be used when
   //                            creating new request descriptors.
   //                            -------------------------------------------------------------------
   // m_template_trans_desc_axi: This class instance is a transaction descriptor template to be used
   //                            when creating new request descriptors.
   //                            -------------------------------------------------------------------
   // m_template_req_desc_axi:   This class instance is a request descriptor template to be used
   //                            when creating new request descriptors.
   //                            -------------------------------------------------------------------
   // m_input_req_queues[]:      This associative array contains request descriptor queues for each
   //                            request source.  To simplify matters, request sources are
   //                            identified by the bus request ID bits set for a particular source
   //                            (with some modification for high and low priority).  Thus,
   //                            `EVL_MEMSHIRE_REQ_ID_FOR_MESH is the index for low-priority
   //                            requests from the mesh, (`EVL_MEMSHIRE_REQ_ID_FOR_MESH | 1) is the
   //                            index for high-priority requests from the mesh,
   //                            `EVL_MEMSHIRE_REQ_ID_FOR_UST is the index for requests from the
   //                            UltraSoc port, and `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC is for requests
   //                            from the atomic state machine.
   //                            -------------------------------------------------------------------
   // m_axi_port_names[]:        This associative array is simply for debug messages and contains
   //                            unique names for each of the possible input AXI ports.
   //                            -------------------------------------------------------------------
   // m_axi_req_queues[]:        This associative array contains request descriptor queues for
   //                            requests directed to the DDR controller.  There are 4 possible DDR
   //                            destinations:
   //                               low-priority even-address port (`EVL_DDR_LO_EVEN_PORT)
   //                               low-priority odd-address port (`EVL_DDR_LO_ODD_PORT)
   //                               high-priority even-address port (`EVL_DDR_HI_EVEN_PORT)
   //                               high-priority odd-address port (`EVL_DDR_HI_ODD_PORT)
   //                            and one possible Atomic_Response destination
   //                               atomic response port (`EVL_ATOMIC_TO_MESH_PORT)
   //                            -------------------------------------------------------------------
   // m_axi_port_names[]:        This associative array is simply for debug messages and contains
   //                            unique names for each of the 4 possible DDR destinations.
   //                            -------------------------------------------------------------------
   // m_pending_ddr_reqs[]:      This associative array is used to track all active requests sent to
   //                            a downstream agent (like a DDR interface).
   //                            -------------------------------------------------------------------
   // m_atomic_cache[]:          This queue is used to track memory lines cached due to atomic
   //                            operations.  Currently, the atomic cache is a fully-associative
   //                            4-entry cache.
   //                            -------------------------------------------------------------------
   // m_atomic_cache_depth:      See above ... this specifies the size of the atomic cache.
   //
   evl_memshire_reg_block m_regs;
   evl_memshire_cache     m_cache;
   evl_memshire_rtl_port  m_memshire_rtl_port;
   evl_line_data          m_template_req_data_axi;
   evl_line_desc          m_template_line_desc_axi;
   evl_trans_desc         m_template_trans_desc_axi;
   evl_req_desc           m_template_req_desc_axi;
   evl_req_desc_queue     m_input_req_queues[int];
   string                 m_axi_port_names[int];
   evl_req_desc_queue     m_axi_req_queues[int];
   evl_req_desc_queue     m_pending_ddr_reqs[int];
   evl_req_desc           m_atomic_cache_descs[$];
   int                    m_atomic_cache_depth = `EVL_MEMSHIRE_ATOMIC_CACHE_DEPTH;
   int                    m_num_atomic_cache_entries;
   evl_req_desc           m_pending_cache_writes[$];
   evl_req_desc           m_pending_atomic_reqs[$];
   int                    m_debug_no_inst;
   bit                    m_reg_access_is_from_sbm;

   evl_memshire_scoreboard_cover      m_memshire_scoreboard_cover;


   //-----------------------------------------------------------------------------------------------
   function set_memshire_reg_block(input evl_memshire_reg_block reg_block_in);
      m_regs = reg_block_in;
   endfunction: set_memshire_reg_block


   //-----------------------------------------------------------------------------------------------
   //
   // Function: addr_is_odd
   //
   // This function returns 1'b1 if the incoming address should be directed to an odd-address DDR
   // port.
   //
   // Input Variables:
   //
   // addr_in: This is the address to check.
   //
   // Return Value:
   //
   // bit: This value will be 1'b1 if the input address is for an odd-address DDR port.
   //
   function bit addr_is_odd(input evl_paddr_t addr_in);
      return ((addr_in & evl_paddr_t'(32'h200)) == evl_paddr_t'(0)) ? 1'b0 : 1'b1;
   endfunction : addr_is_odd


   //-----------------------------------------------------------------------------------------------
   //
   // Function: compute_ddr_addr
   //
   // This function is used to obtain a compressed DDR address from an input address.  The
   // compressed address is the same as the input address with the addr_mask register applied, the
   // even/odd selection bit removed, and the shire ID bits removed.  The address is then 
   // truncated to the appropriate final width in the event that the even/odd and shireId bits 
   // aren't all removed.
   //
   // Input Variables:
   //
   // addr_in: This is the address to compress.
   //
   // Return Value:
   //
   // evl_paddr_t: This is the compressed address.
   //
   function evl_paddr_t compute_ddr_addr(input evl_paddr_t addr_in,
                                         input bit         create_debug_addr);
      evl_paddr_t  addr;
      evl_paddr_t  esr_addr_mask;
      int unsigned esr_remove_bit_sel[$];

      addr = addr_in;

      // Step 1) strip off the MSB 
      addr[`EVL_PADDR_MSB] = 1'b0;

      // Step 2) apply the addr_mask register setting
      esr_addr_mask = ~(~(get_reg_field_value(ESR_MS_ADDR_MASK)) << 28);
      addr &= esr_addr_mask;

      // Step 3) check the addr_remove_bit_selN register settings and remove bits as appropriate
      if (create_debug_addr == 1'b1) begin
         esr_remove_bit_sel.push_back(get_reg_field_value(ESR_MS_ADDR_DEBUG_MC_BIT_SEL));
      end
      else begin
         esr_remove_bit_sel.push_back(get_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL3));
         esr_remove_bit_sel.push_back(get_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL2));
         esr_remove_bit_sel.push_back(get_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL1));
         esr_remove_bit_sel.push_back(get_reg_field_value(ESR_MS_ADDR_REMOVE_BIT_SEL0));
      end

      foreach (esr_remove_bit_sel[ii]) begin
         addr = ((~evl_paddr_t'((1 << esr_remove_bit_sel[ii])-1) & (addr >> 1)) | (evl_paddr_t'((1 << esr_remove_bit_sel[ii])-1) & addr));
      end

      // Step 4) truncate the address to 35 bits because that's the physical limit of that interface
      addr &= ((1 << `MEM_MASTER_AXI_ADDR_SIZE) -1);

      return addr;
   endfunction : compute_ddr_addr


   //-----------------------------------------------------------------------------------------------
   //
   // Function: find_observed_request
   //
   // This function is used by downstream bus agents (i.e., the 4 DDR bus agents) to correlate an
   // observed bus request with a predicted bus request.  The bus agents each have a handle to their
   // master port agent, which is the memshire agent (i.e., the parent of this scoreboard).  When a
   // handle to a master port agent exists, bus agents perform find_observed_request() calls in the
   // master port agent when a new bus request is observed.  For the DDR bus agents, those calls
   // will be directed here, to this function.
   //
   // Input Variables:
   //
   // port_type_in:    This is the agent type of the bus agent requesting correlation.  The reason
   //                  this is necessary is because some scoreboards support requests from different
   //                  types of bus agents.  For DDR agents, this will be AXI_BUS_AGENT.
   // port_in:         This is the port number of the bus agent requesting correlation.  For DDR
   //                  agents, this will be `EVL_DDR_LO_EVEN_PORT to `EVL_DDR_HI_ODD_PORT.
   // paired_req_desc: This is the observed bus request descriptor.
   // bus_cmd_in:      This is the observed bus command and is only necessary if no paired_req_desc
   //                  handle is available.
   // address_in:      This is the observed bus address and is only necessary if no paired_req_desc
   //                  handle is available.
   // req_id_in:       This is the observed bus tag (request ID) and is only necessary if no
   //                  paired_req_desc handle is available.
   // source_id_in:    This is the observed bus source ID (for ET link requests) and is only
   //                  necessary if no paired_req_desc handle is available.
   // trans_id_in:     This is the observed transaction ID and is included only if it is available.
   //                  For DDR bus agents, there is no access to a transaction ID.
   //
   // Return Value:
   //
   // evl_req_desc: This is the predicted request descriptor that matches the specified bus
   //               request information.  If a match is found, the paired request descriptor will be
   //               added as a child of this request descriptor.  If no match is found, a null
   //               handle is returned.
   //
   virtual function evl_req_desc find_observed_request(input evl_agent_type_t     port_type_in,
                                                       input int                  port_in,
                                                       input int                  subport_in,
                                                       input evl_req_desc         paired_req_desc,
                                                       input evl_bus_cmd_t        bus_cmd_in   = `EVL_Bus_Idle,
                                                       input evl_paddr_t          address_in   = ~evl_paddr_t'(0),
                                                       input evl_bus_req_id_t     req_id_in    = ~evl_bus_req_id_t'(0),
                                                       input evl_bus_req_source_t source_id_in = ~evl_bus_req_source_t'(0),
                                                       input evl_trans_id_t       trans_id_in  = ~evl_trans_id_t'(0));
      int              index = -1;
      int              limit = 0;
      evl_req_desc     req_desc;
      evl_bus_cmd_t    bus_cmd;
      evl_paddr_t      address;
      evl_bus_req_id_t req_id;
      evl_req_desc     req_descs[];

      if ((paired_req_desc == null) || (!(m_axi_req_queues.exists(port_in)))) begin
         return null;
      end
      else begin
         bus_cmd = (bus_cmd_in != `EVL_Bus_Idle)         ? bus_cmd_in : paired_req_desc.get_bus_cmd();
         req_id  = (req_id_in  != ~evl_bus_req_id_t'(0)) ? req_id_in  : paired_req_desc.get_bus_req_id();
         address = (address_in != ~evl_paddr_t'(0))      ? address_in : paired_req_desc.get_paddr();
      end

      //
      // Because the AXI read and write channels are independent, we need to look for different
      // requests depending on whether the observed request is a read or a write.
      //
      if (paired_req_desc.bus_cmd_is_write(bus_cmd) == 1'b1) begin
         req_desc = m_axi_req_queues[port_in].find_first_write();
         if (req_desc == null) begin
            `dut_error(get_abstract_name(), $sformatf("%0s on %0s is an unexpected request", paired_req_desc.sprint_obj(), m_axi_port_names[port_in]))
            return null;
         end

         //
         // If we have found what we are looking for, finish up.
         //
         if ((req_desc.get_paddr() == address) && (req_desc.get_bus_req_id() == req_id)) begin
            req_desc.add_child_desc(paired_req_desc);
            m_axi_req_queues[port_in].pop(req_desc);
            return req_desc;
         end

         //
         // Search for a matching write request.  If one is found, pair it up with the incoming
         // request descriptor and clean up the queue.  If not, well, no pairing happens, and no
         // cleaning happens.
         //
         limit = m_axi_req_queues[port_in].find_all_writes(req_descs);
         if (limit > 0) begin
            for (int ii = 0; ii < limit; ii++) begin
               if ((req_descs[ii].get_paddr() == address) && (req_descs[ii].get_bus_req_id() == req_id)) begin
                  index = ii;
                  break;
               end
            end
         end
         if (index >= 0) begin
            `dut_error(get_abstract_name(), $sformatf("expected %0s on %0s, not %0s", req_desc.sprint_obj(), m_axi_port_names[port_in], req_descs[index].sprint_obj()))
            req_desc = req_descs[index];
            for (int ii = 0; ii < index; ii++) begin
               `dut_warning(get_abstract_name(), $sformatf("popping %0s from the expected request queue for %0s", req_descs[ii].sprint_obj(), m_axi_port_names[port_in]))
               void'(m_axi_req_queues[port_in].pop(req_descs[ii]));
            end
            req_desc.add_child_desc(paired_req_desc);
            m_axi_req_queues[port_in].pop(req_desc);
            return req_desc;
         end
      end
      else begin
         req_desc = m_axi_req_queues[port_in].find_first_read();
         if (req_desc == null) begin
            `dut_error(get_abstract_name(), $sformatf("%0s on %0s is an unexpected request", paired_req_desc.sprint_obj(), m_axi_port_names[port_in]))
            return null;
         end

         //
         // If we have found what we are looking for, finish up.
         //
         if ((req_desc.get_paddr() == address) && (req_desc.get_bus_req_id() == req_id)) begin
            req_desc.add_child_desc(paired_req_desc);
            m_axi_req_queues[port_in].pop(req_desc);
            return req_desc;
         end

         //
         // Search for a matching read request.  If one is found, pair it up with the incoming
         // request descriptor and clean up the queue.  If not, well, no pairing happens, and no
         // cleaning happens.
         //
         limit = m_axi_req_queues[port_in].find_all_reads(req_descs);
         if (limit > 0) begin
            for (int ii = 0; ii < limit; ii++) begin
               if ((req_descs[ii].get_paddr() == address) && (req_descs[ii].get_bus_req_id() == req_id)) begin
                  index = ii;
                  break;
               end
            end
         end
         if (index >= 0) begin
            `dut_error(get_abstract_name(), $sformatf("expected %0s on %0s, not %0s", req_desc.sprint_obj(), m_axi_port_names[port_in], req_descs[index].sprint_obj()))
            req_desc = req_descs[index];
            for (int ii = 0; ii < index; ii++) begin
               `dut_warning(get_abstract_name(), $sformatf("popping %0s from the expected request queue for %0s", req_descs[ii].sprint_obj(), m_axi_port_names[port_in]))
               void'(m_axi_req_queues[port_in].pop(req_descs[ii]));
            end
            req_desc.add_child_desc(paired_req_desc);
            m_axi_req_queues[port_in].pop(req_desc);
            return req_desc;
         end
      end

      //
      // No match was found and the head of the queue is not null.
      //
      `dut_error(get_abstract_name(), $sformatf("expected %0s on %0s, not %0s", req_desc.sprint_obj(), m_axi_port_names[port_in], paired_req_desc.sprint_obj()))
      return null;
   endfunction : find_observed_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_atomic_req_act
   //
   // This function is used to add a bus request action for subsequent bus request lookups.  The
   // first 4 input variables form the lookup index, and the remaining 5 input variables comprise
   // the actions to performed.
   //
   // Input Variables:
   //
   // axi_bus_cmd_in:    This is the AXI bus command of an incoming request.
   // etlink_bus_cmd_in: This is the ET link command of an incoming request.
   // model_st_in:       This is the cache model state for the line specified by the incoming
   //                    request.
   // qualifier_in:      This is a qualifier used to modify lookup parameters.
   // tag_action_in:     This is the tag action to perform based on the calculated index.
   // data_action_in:    This is the data action to perform based on the calculated index.
   // bus_cmd_in:        This is the bus command to issue based on the calculated index; if it is
   //                    BusIdle, no new bus request will be sent.
   // bus_rsp_in:        This is the bus response to issue based on the calculated index; if it is
   //                    RspNull, no bus response will be sent.
   // err_code_in:       This is the error code/miscellaneous action to perform based on the
   //                    calculated index.
   //
   function void add_atomic_req_act(input evl_axi_req_type_t      axi_bus_cmd_in,
                                    input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                    input evl_cache_st_t          model_st_in,
                                    input evl_ms_qual_t           qualifier_in,
                                    input evl_cache_st_t          tag_action_in,
                                    input evl_data_action_t       data_action_in,
                                    input evl_abstract_bus_cmd_t  bus_cmd_in,
                                    input evl_abstract_rsp_type_t bus_rsp_in,
                                    input int                     err_code_in);
      if (m_cache != null) begin
         m_cache.add_bus_req_act(MS_TABLE_ATOMIC_HIT, evl_bus_cmd_t'(axi_bus_cmd_in), evl_bus_cmd_t'(etlink_bus_cmd_in), model_st_in, qualifier_in, tag_action_in, data_action_in, evl_bus_cmd_t'(bus_cmd_in), evl_bus_rsp_type_t'(bus_rsp_in), err_code_in, 1'b0);
      end
   endfunction : add_atomic_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_cache_wr_act
   //
   // This function is used to add a lookup action for write events to the atomic cache.  The
   // first 4 input variables form the lookup index, and the remaining 5 input variables comprise
   // the actions to performed.
   //
   // Input Variables:
   //
   // axi_bus_cmd_in:    This is the AXI bus command of an incoming request.
   // etlink_bus_cmd_in: This is the ET link command of an incoming request.
   // model_st_in:       This is the cache model state for the line specified by the incoming
   //                    request.
   // qualifier_in:      This is a qualifier used to modify lookup parameters.
   // tag_action_in:     This is the tag action to perform based on the calculated index.
   // data_action_in:    This is the data action to perform based on the calculated index.
   // bus_cmd_in:        This is the bus command to issue based on the calculated index; if it is
   //                    BusIdle, no new bus request will be sent.
   // bus_rsp_in:        This is the bus response to issue based on the calculated index; if it is
   //                    RspNull, no bus response will be sent.
   // err_code_in:       This is the error code/miscellaneous action to perform based on the
   //                    calculated index.
   //
   function void add_cache_wr_act(input evl_axi_req_type_t      axi_bus_cmd_in,
                                  input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                  input evl_cache_st_t          model_st_in,
                                  input evl_ms_qual_t           qualifier_in,
                                  input evl_cache_st_t          tag_action_in,
                                  input evl_data_action_t       data_action_in,
                                  input evl_abstract_bus_cmd_t  bus_cmd_in,
                                  input evl_abstract_rsp_type_t bus_rsp_in,
                                  input int                     err_code_in);
      if (m_cache != null) begin
         m_cache.add_bus_req_act(MS_TABLE_CACHE_WR, evl_bus_cmd_t'(axi_bus_cmd_in), evl_bus_cmd_t'(etlink_bus_cmd_in), model_st_in, qualifier_in, tag_action_in, data_action_in, evl_bus_cmd_t'(bus_cmd_in), evl_bus_rsp_type_t'(bus_rsp_in), err_code_in, 1'b0);
      end
   endfunction : add_cache_wr_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_req_act
   //
   // This function is used to add a bus request action for subsequent bus request lookups.  The
   // first 4 input variables form the lookup index, and the remaining 5 input variables comprise
   // the actions to performed.
   //
   // Input Variables:
   //
   // axi_bus_cmd_in:    This is the AXI bus command of an incoming request.
   // etlink_bus_cmd_in: This is the ET link command of an incoming request.
   // model_st_in:       This is the cache model state for the line specified by the incoming
   //                    request.
   // qualifier_in:      This is a qualifier used to modify lookup parameters.
   // tag_action_in:     This is the tag action to perform based on the calculated index.
   // data_action_in:    This is the data action to perform based on the calculated index.
   // bus_cmd_in:        This is the bus command to issue based on the calculated index; if it is
   //                    BusIdle, no new bus request will be sent.
   // bus_rsp_in:        This is the bus response to issue based on the calculated index; if it is
   //                    RspNull, no bus response will be sent.
   // err_code_in:       This is the error code/miscellaneous action to perform based on the
   //                    calculated index.
   //
   function void add_bus_req_act(input evl_axi_req_type_t      axi_bus_cmd_in,
                                 input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_ms_qual_t           qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_bus_cmd_t  bus_cmd_in,
                                 input evl_abstract_rsp_type_t bus_rsp_in,
                                 input int                     err_code_in);
      if (m_cache != null) begin
         m_cache.add_bus_req_act(MS_TABLE_SERIALIZE, evl_bus_cmd_t'(axi_bus_cmd_in), evl_bus_cmd_t'(etlink_bus_cmd_in), model_st_in, qualifier_in, tag_action_in, data_action_in, evl_bus_cmd_t'(bus_cmd_in), evl_bus_rsp_type_t'(bus_rsp_in), err_code_in, 1'b0);
      end
   endfunction : add_bus_req_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_req_act_nc
   //
   // This function is used to add a bus request action for subsequent bus request lookups.  The
   // first 4 input variables form the lookup index, and the remaining 5 input variables comprise
   // the actions to performed.
   //
   // Input Variables:
   //
   // axi_bus_cmd_in:    This is the AXI bus command of an incoming request.
   // etlink_bus_cmd_in: This is the ET link command of an incoming request.
   // model_st_in:       This is the cache model state for the line specified by the incoming
   //                    request.
   // qualifier_in:      This is a qualifier used to modify lookup parameters.
   // tag_action_in:     This is the tag action to perform based on the calculated index.
   // data_action_in:    This is the data action to perform based on the calculated index.
   // bus_cmd_in:        This is the bus command to issue based on the calculated index; if it is
   //                    BusIdle, no new bus request will be sent.
   // bus_rsp_in:        This is the bus response to issue based on the calculated index; if it is
   //                    RspNull, no bus response will be sent.
   // err_code_in:       This is the error code/miscellaneous action to perform based on the
   //                    calculated index.
   //
   function void add_bus_req_act_nc(input evl_axi_req_type_t      axi_bus_cmd_in,
                                    input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                    input evl_cache_st_t          model_st_in,
                                    input evl_ms_qual_t           qualifier_in,
                                    input evl_cache_st_t          tag_action_in,
                                    input evl_data_action_t       data_action_in,
                                    input evl_abstract_bus_cmd_t  bus_cmd_in,
                                    input evl_abstract_rsp_type_t bus_rsp_in,
                                    input int                     err_code_in);
      if (m_cache != null) begin
         m_cache.add_bus_req_act(MS_TABLE_SERIALIZE, evl_bus_cmd_t'(axi_bus_cmd_in), evl_bus_cmd_t'(etlink_bus_cmd_in), model_st_in, qualifier_in, tag_action_in, data_action_in, evl_bus_cmd_t'(bus_cmd_in), evl_bus_rsp_type_t'(bus_rsp_in), err_code_in, 1'b1);
      end
   endfunction : add_bus_req_act_nc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_bus_rsp_act
   //
   // This function is used to add a bus request action for subsequent bus response lookups.  The
   // first 5 input variables form the lookup index, and the remaining 5 input variables comprise
   // the actions to performed.
   //
   // Input Variables:
   //
   // axi_bus_cmd_in:    This is the AXI bus command of the original request.
   // etlink_bus_cmd_in: This is the ET link command of the original request.
   // model_st_in:       This is the cache model state for the line specified by the original
   //                    request.
   // bus_rsp_in:        This is the observed bus response.
   // qualifier_in:      This is a qualifier used to modify lookup parameters.
   // tag_action_in:     This is the tag action to perform based on the calculated index.
   // data_action_in:    This is the data action to perform based on the calculated index.
   // new_bus_cmd_in:    This is the bus command to issue based on the calculated index; if it is
   //                    BusIdle, no new bus request will be sent.
   // new_bus_rsp_in:    This is the bus response to issue based on the calculated index; if it is
   //                    RspNull, no bus response will be sent.
   // err_code_in:       This is the error code/miscellaneous action to perform based on the
   //                    calculated index.
   //
   function void add_bus_rsp_act(input evl_axi_req_type_t      axi_bus_cmd_in,
                                 input evl_etlink_req_opcode_t etlink_bus_cmd_in,
                                 input evl_cache_st_t          model_st_in,
                                 input evl_axi_resp_t          bus_rsp_in,
                                 input evl_ms_qual_t           qualifier_in,
                                 input evl_cache_st_t          tag_action_in,
                                 input evl_data_action_t       data_action_in,
                                 input evl_abstract_rsp_type_t new_bus_rsp_in,
                                 input evl_abstract_bus_cmd_t  new_bus_cmd_in,
                                 input int                     err_code_in);
      if (m_cache != null) begin
         m_cache.add_bus_rsp_act(axi_bus_cmd_in, etlink_bus_cmd_in, model_st_in, bus_rsp_in, qualifier_in, tag_action_in, data_action_in, evl_bus_rsp_type_t'(new_bus_rsp_in), evl_bus_cmd_t'(new_bus_cmd_in), err_code_in, 1'b0);
      end
   endfunction : add_bus_rsp_act


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bus_request
   //
   // This function is called whenever a new bus request is observed on either the mesh-to-memory
   // AXI interface or the UltraSoc-to-memory AXI interface.  The incoming request descriptor is
   // pushed onto the appropriate m_input_req_queues[] queue.  Note that if the incoming request is
   // an atomic operation, the destination m_input_req_queues[] is set to
   // `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC.  This function is called from the parent memshire agent.
   //
   // Input Variables:
   //
   // req_desc:    This is the incominmg request descriptor.
   // port_req_id: This is the AXI port ID (either mesh-to-memory or UltraSoc-to-memory) from which
   //              the request descriptor comes.  It should be set to either
   //              `EVL_MEMSHIRE_REQ_ID_FOR_MESH or `EVL_MEMSHIRE_REQ_ID_FOR_UST.
   // use_qos:     This input is set to 1'b1 if it is valid to interpret the channel QOS
   //              information in the request descriptor.  Channel QOS information can only come
   //              from the mesh-to-memory interface (On UST interface QOS has no meaning here)
   //
   function void process_bus_request(input evl_req_desc req_desc, input evl_bus_req_id_t port_req_id, input bit use_qos);
      int port_queue_id;

      port_queue_id = int'(port_req_id);

      if (!(m_input_req_queues.exists(port_queue_id))) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_input_req_queues[0x%0x] does not exist for %0s (%0s, line %0d)", port_queue_id, req_desc.sprint_obj(), `__FILE__, `__LINE__))
         return;
      end

      //
      // If it is not OK to evaluate the QOS information (i.e. this interface doesn't use QOS), force it to 0.
      // Otherwise check if it's a value that is expected/legal
      //
      if (use_qos == 1'b0) begin
         req_desc.set_bus_qos(evl_bus_qos_t'(0));
      end
      else begin
         evl_bus_qos_t req_qos;

         req_qos = req_desc.get_bus_qos();

         if (!(req_qos inside {evl_bus_qos_t'(0), evl_bus_qos_t'(1), evl_bus_qos_t'(3)})) begin
            `dut_error(get_abstract_name(), $sformatf("Memshire received a request from port [%0s] with unexpected QOS value of 0x%0x (%0s, line %0d)", m_axi_port_names[port_queue_id], req_qos, `__FILE__, `__LINE__))
         end
      end

      //
      // If the incoming request is a write, we need to interpret the AWUSER bits.  If it is a read,
      // we set the ET link command to ET_LINK_REQ_Read.
      //
      if (req_desc.bus_cmd_is_write() == 1'b1) begin
         evl_etlink_req_opcode_t etlink_bus_cmd = evl_etlink_req_opcode_t'(req_desc.get_bus_req_user());

         req_desc.set_int_param(pk_etlink_cmd, int'(etlink_bus_cmd));
         if (etlink_bus_cmd == ET_LINK_REQ_Atomic) begin
            port_queue_id = `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC;
         end
      end
      else begin
         req_desc.set_int_param(pk_etlink_cmd, int'(ET_LINK_REQ_Read));
      end

      // Always set observed so that the response will be checked whenever it shows up
      req_desc.set_observed();

      `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("received a request from %0s (%0s ... using queue ID 0x%0x)", m_axi_port_names[port_queue_id], req_desc.sprint_obj(), port_queue_id))
      if (m_input_req_queues.exists(port_queue_id)) begin
         if (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) begin
            m_pending_atomic_reqs.push_back(req_desc);
         end
         m_input_req_queues[port_queue_id].push(req_desc);
      end
   endfunction : process_bus_request


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_bus_response
   //
   // This function is called whenever a new bus response is observed on the DDR interfaces.  This
   // function is called from the parent memshire agent.
   //
   // Input Variables:
   //
   // req_desc:    This is the incoming request descriptor from the bus agent ... it *should* match
   //              an outstanding request.
   // port_number: This is the port number from which the response is received.
   //
   function void process_bus_response(input evl_req_desc req_desc, input int port_number);
      evl_table_desc     table_desc;
      evl_line_desc      line_desc;
      evl_req_desc       src_req_desc;
      evl_bus_cmd_t      src_bus_cmd;
      evl_bus_cmd_t      bus_cmd;
      evl_bus_cmd_t      etlink_cmd;
      evl_req_desc_queue outstanding_reqs;
      evl_req_desc       outstanding_req_desc;


      `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("received a response from %0s (%0s)", m_axi_port_names[port_number], req_desc.sprint_obj()))

      if (req_desc.bus_cmd_is_read() == 1'b1) begin
         outstanding_reqs = m_pending_ddr_reqs[port_number].filter_read_bus_req_id(req_desc.get_bus_req_id());
         if (outstanding_reqs == null) begin
            `dut_error(get_abstract_name(), $sformatf("unexpected response received for ID 0x%x (%0s)", req_desc.get_bus_req_id(), req_desc.sprint_obj()))
            return;
         end

         outstanding_req_desc = outstanding_reqs.find_first_read();
         if (outstanding_req_desc != req_desc) begin
            `dut_error(get_abstract_name(), $sformatf("expected the response received for ID 0x%x to be for %0s, not %0s", req_desc.get_bus_req_id(), outstanding_req_desc.sprint_obj(), req_desc.sprint_obj()))
         end
      end
      else begin
         outstanding_reqs = m_pending_ddr_reqs[port_number].filter_write_bus_req_id(req_desc.get_bus_req_id());
         if (outstanding_reqs == null) begin
            `dut_error(get_abstract_name(), $sformatf("unexpected response received for ID 0x%x (%0s)", req_desc.get_bus_req_id(), req_desc.sprint_obj()))
            return;
         end

         outstanding_req_desc = outstanding_reqs.find_first_write();
         if (outstanding_req_desc != req_desc) begin
            `dut_error(get_abstract_name(), $sformatf("expected the response received for ID 0x%x to be for %0s, not %0s", req_desc.get_bus_req_id(), outstanding_req_desc.sprint_obj(), req_desc.sprint_obj()))
         end
      end

      src_req_desc = req_desc.get_parent_req_desc();

      //
      // Do the lookup here and process it.  Note that we know the processing order at response
      // time, so this is when we will check the cache contents and/or update them.  In addition, we
      // need to forward the response to the parent request descriptor and mark the pending request
      // descriptor as done.
      //

      if (src_req_desc == null) begin
         `dut_error_mdesc(get_abstract_name(), req_desc, $sformatf("no source request could be found for %0s", req_desc.sprint_obj()))
      end
      else begin
         //src_bus_cmd = (src_req_desc.bus_cmd_is_read() == 1'b1) ? evl_bus_cmd_t'(ET_LINK_REQ_Read) : evl_bus_cmd_t'(src_req_desc.get_bus_req_user());
         if (src_req_desc.bus_cmd_is_read() == 1'b1) begin
            src_bus_cmd = evl_bus_cmd_t'(ET_LINK_REQ_Read);
         end
         else begin
            if (src_req_desc.get_bus_req_user() != evl_bus_req_user_t'(0)) begin
               src_bus_cmd = evl_bus_cmd_t'(src_req_desc.get_bus_req_user());
            end
            else begin
               src_bus_cmd = evl_bus_cmd_t'(ET_LINK_REQ_Write);
            end
         end

         m_cache.bus_rsp_lookup(table_desc, line_desc, src_req_desc, req_desc.get_bus_cmd(), src_bus_cmd, req_desc.get_bus_rsp_type(), MSQ_None);

         if (`evl_log_check_verbosity(UVM_FULL, VMOD_MS_SB) != 0) begin
            line_desc.print_obj(get_abstract_name());
         end

         //
         // Process the tag action as necessary.
         //
         if (table_desc.get_tag_action() != St_Ignore) begin
            line_desc.set_cache_st(table_desc.get_tag_action());
         end

         //
         // Process the data action as necessary.
         //
         if (table_desc.get_data_action() inside { Data_CacheFill }) begin
            if (req_desc.get_rsp_data() != null) begin
               line_desc.merge_data_from_line(req_desc.get_rsp_data(), 1'b1);
            end
            else begin
               `dut_error(get_abstract_name(), $sformatf("TB_FAIL: no response DATA found for m_pending_ddr_reqs[0x%0x] (%0s, line %0d)", req_desc.get_bus_req_id(),  `__FILE__, `__LINE__))
            end
         end
         else if (table_desc.get_data_action() inside { Data_FwdAndCheck, Data_FwdRd }) begin
            // also check the data here?
            src_req_desc.set_rsp_data(req_desc.get_rsp_data());
         end

         //
         // Process the bus response type as necessary.
         //
         if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
            `evl_log_mdesc(UVM_HIGH, VMOD_SC_SB, get_abstract_name(), req_desc, $sformatf("forwarding bus response to %0s", src_req_desc.sprint_obj()))
            src_req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
            src_req_desc.set_bus_rsp_valid(0, get_cycle_count());
         end

      end

      //
      // Remove the pending request descriptor if all of the response information has been received.
      //
      m_pending_ddr_reqs[port_number].pop(outstanding_req_desc);

      req_desc.set_done();

   endfunction : process_bus_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_atomic_hit_event
   //
   // This function is used to process grey-box information (an atomic hit event).  Grey-box
   // information is sent from the evl_memshire_interface via calls to send_packet() in the RTL
   // port.  These are then processed by a task in the parent memshire agent, which then calls
   // process_rtl_event() in this scoreboard.
   //
   // Input Variables:
   //
   // info:        This structure contains the RTL event information.
   //
   function void process_atomic_hit_event(input evl_memshire_atomic_t info);
      int                    port_queue_id;
      evl_paddr_t            addr;
      evl_req_desc           req_desc;
      evl_bus_cmd_t          bus_cmd;
      evl_bus_cmd_t          etlink_cmd;
      evl_ms_qual_t          qualifier;
      evl_table_desc         table_desc;
      evl_line_desc          line_desc;
      int unsigned           atomic_cache_size;
      int unsigned           atomic_cache_hit_index;


      if (info.is_atomic == 1'b1) begin
         port_queue_id = `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC;
      end
      else begin
         port_queue_id = `EVL_MEMSHIRE_REQ_ID_FOR_MESH;
      end
      if (!(m_input_req_queues.exists(port_queue_id))) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_input_req_queues[0x%0x] does not exist for 0x%x, ID 0x%x (%0s, line %0d)", port_queue_id, info.addr, info.req_id, `__FILE__, `__LINE__))
         return;
      end

      if ((info.write_req == 1'b1) || (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC)) begin
         req_desc = m_input_req_queues[port_queue_id].find_first_write();
      end
      else begin
         req_desc = m_input_req_queues[port_queue_id].find_first_read();
      end
      if (req_desc == null) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: process_atomic_hit: no request found for m_input_req_queues[0x%0x] (0x%x, ID 0x%x) (%0s, line %0d)", port_queue_id, info.addr, info.req_id, `__FILE__, `__LINE__))
         return;
      end

      if (req_desc.get_paddr() != info.addr) begin
         `dut_error(get_abstract_name(), $sformatf("expected the address to be 0x%x, not 0x%x", req_desc.get_paddr(), info.addr))
         return;
      end

      if ((req_desc.get_bus_req_id() != info.req_id) && (port_queue_id != `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC)) begin
         `dut_error(get_abstract_name(), $sformatf("expected the bus request ID to be 0x%x, not 0x%x", req_desc.get_bus_req_id(), info.req_id))
      end

      m_input_req_queues[port_queue_id].pop(req_desc);
      `evl_log_mdesc(UVM_MEDIUM, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing an atomic hit event for %0s", req_desc.sprint_obj()))

      if (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) begin
         bus_cmd    = AXI_REQ_WRITE;
         etlink_cmd = ET_LINK_REQ_Atomic;
      end
      else if (info.write_req == 1'b1) begin
         bus_cmd    = AXI_REQ_WRITE;
         etlink_cmd = ET_LINK_REQ_Write;
      end
      else begin
         bus_cmd    = AXI_REQ_READ;
         etlink_cmd = ET_LINK_REQ_Read;
      end

      //sample coverage
      m_memshire_scoreboard_cover.sample_ms_atomic_cache(1'b1, etlink_cmd, req_desc.get_bus_qos());

      qualifier = MSQ_None;

      m_cache.bus_req_lookup(table_desc, line_desc, MS_TABLE_ATOMIC_HIT, bus_cmd, etlink_cmd, req_desc, qualifier);

      //
      // Process the data action as necessary
      //
      if (table_desc.get_data_action() inside { Data_MemWr, Data_ReqWr}) begin
         if (req_desc.get_req_data() != null) begin
            if (etlink_cmd == ET_LINK_REQ_Atomic) begin
               req_desc.set_int_param(pk_desc_is_atomic, 1);
            end
            m_pending_cache_writes.push_back(req_desc);
         end
         else begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: process_atomic_hit: no request DATA found for m_input_req_queues[0x%0x] (0x%x, ID 0x%x) (%0s, line %0d)", port_queue_id, info.addr, info.req_id, `__FILE__, `__LINE__))
         end
      end
      else if (table_desc.get_data_action() == Data_MemRd) begin
         req_desc.set_rsp_data(line_desc.get_line_data_copy());
      end

      //
      // Process the response action as necessary.
      //
      // Always set observed so that the response will be checked whenever it shows up
      req_desc.set_observed();
      if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
         req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
      end
     
      //
      // Update the atomic cache LRU status if it is an Atomic Hit
      //
      // Look for the match in the atomic cache/queue and remove it from the
      // queue, then push this one onto the back
      //
      // we could just search the cache until we find the first match, but
      // let's search the whole thing in case we catch any duplicate addresses
      if (etlink_cmd == ET_LINK_REQ_Atomic) begin
         atomic_cache_hit_index = -1;
         atomic_cache_size = m_atomic_cache_descs.size();

         for (int ii = 0; ii < atomic_cache_size; ii++) begin
            if (req_desc.get_paddr() == m_atomic_cache_descs[ii].get_paddr()) begin
               if (atomic_cache_hit_index != -1) begin
                  `dut_error(get_abstract_name(), $sformatf("Found two entries in the atomic cache with the same address (0x%0x)!  Entry %0d: %0s   and  Entry %0d: %0s", req_desc.get_paddr(), atomic_cache_hit_index, m_atomic_cache_descs[atomic_cache_hit_index].sprint_obj(), ii, m_atomic_cache_descs[ii].sprint_obj()))
               end
               else begin
                  atomic_cache_hit_index = ii;
               end
            end
         end

         if (atomic_cache_hit_index == -1) begin
            `dut_error(get_abstract_name(), $sformatf("TB Error: MS_SB could not find an entry in the atomic cache with an address of 0x%0x", req_desc.get_paddr()))
         end
         else begin
            m_atomic_cache_descs.delete(atomic_cache_hit_index);
            m_atomic_cache_descs.push_back(req_desc);
            `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("Removing entry %0d from the atomic cache queue and placing this at the end: %0s", atomic_cache_hit_index, req_desc.sprint_obj()))
         end
      end
      
   endfunction : process_atomic_hit_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_atomic_rsp_push_event
   //
   function void process_atomic_rsp_push_event(input evl_memshire_atomic_t info);
      evl_req_desc           req_desc;
      evl_bus_cmd_t          bus_cmd;
      evl_bus_cmd_t          etlink_cmd;
      evl_ms_qual_t          qualifier;
      evl_table_desc         table_desc;
      evl_line_desc          line_desc;

      if (m_pending_atomic_reqs.size() == 0) begin
         `dut_error(get_abstract_name(), $sformatf("process_atomic_rsp_push: no pending atomic cache write found (%0s, line %0d)", `__FILE__, `__LINE__))
         return;
      end
      req_desc = m_pending_atomic_reqs.pop_front();

      `evl_log_mdesc(UVM_MEDIUM, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing an atomic response push event for %0s (hit vector:%b)", req_desc.sprint_obj(), info.hit_vector))

      m_cache.bus_req_lookup(table_desc, line_desc, MS_TABLE_SERIALIZE, AXI_REQ_WRITE, ET_LINK_REQ_Atomic, req_desc, MSQ_SndRsp);

      //
      // Process the bus action as necessary.
      //
      if (table_desc.get_bus_cmd() == BusWriteAtomic) begin
         evl_line_data atomic_response;   // response of the atomic operation, needs to be here for axi_req to use
         evl_qword_t   qword;

         qword = req_desc.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB - 1));
         req_desc.set_bus_atomic_conf(qword[`EVL_BUS_ATOMIC_CONF_WIDTH - 2:0]);
         req_desc.set_bus_atomic_offset(qword >> (`EVL_BUS_ATOMIC_CONF_WIDTH - 1));

         atomic_response = line_desc.get_atomic_return_data(req_desc, 1'b0);

         predict_atomic_response(req_desc, atomic_response);
      end
   endfunction : process_atomic_rsp_push_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_serialization_event
   //
   // This function is used to process grey-box information (a serialization event).  Grey-box
   // information is sent from the evl_memshire_interface via calls to send_packet() in the RTL
   // port.  These are then processed by a task in the parent memshire agent, which then calls
   // process_rtl_event() in this scoreboard.
   //
   // Input Variables:
   //
   // hi_priority: This bit is set to 1 if the serialization event is a high-priority request.
   // info:        This structure contains the RTL event information.
   //
   function void process_serialization_event(input bit hi_priority, input evl_memshire_serialize_t info);
      int                    port_queue_id;
      evl_paddr_t            addr;
      evl_req_desc           req_desc;
      evl_req_desc           axi_req_desc;
      evl_abstract_bus_cmd_t new_bus_cmd;
      evl_bus_cmd_t          bus_cmd;
      evl_bus_cmd_t          etlink_cmd;
      evl_ms_qual_t          qualifier;
      evl_table_desc         table_desc;
      evl_line_desc          line_desc;
      bit                    esr_atomic_evict_start;
      string                 event_type;
      bit                    is_debug;
      
      qualifier = MSQ_None;

      port_queue_id = int'(info.grants);
      if (!(m_input_req_queues.exists(port_queue_id))) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_input_req_queues[0x%0x] does not exist for 0x%x, ID 0x%x (%0s, line %0d)", port_queue_id, info.addr, info.req_id, `__FILE__, `__LINE__))
         return;
      end

      is_debug = (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_UST) ? 1'b1 : 1'b0;
      
      // Four possible scenarios here:
      //
      // 1) Write & AtomicID -> atomic cache eviction, pull from cache/queue
      // 2) Write & MeshID -> regular write, pull from write input queue
      // 3) Read & AtomicID -> atomic miss, pull from write input queue
      // 4) Read & MeshID -> regular read, pull from read input queue
      //
      if (info.write_req == 1'b1) begin
         // #1
         if (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) begin
            int unsigned atomic_cache_size;

            atomic_cache_size = m_atomic_cache_descs.size();

            if (atomic_cache_size == 0) begin
               `dut_error(get_abstract_name(), $sformatf("Received a serialization event for an eviction but the Scoreboard atomic cache is empty!!"))
               return;
            end
         
            // Check if this is an eviction due to capacity or the SM:
            // 1) eviction due to atomic miss -> LRU entry is evicted
            // 2) eviction due to atomic SM -> no particular order to evictions,
            //     so we take a look at what is being evicted and check that
            //     it is indeed an entry in the "cache" that we are tracking
            
            esr_atomic_evict_start = get_reg_field_value(ESR_MS_ATOMIC_EVICT_START);
         
            if (esr_atomic_evict_start == 1'b1) begin
               int unsigned atomic_index;

               atomic_index = -1;

               for (int ii = 0; ii < atomic_cache_size; ii++) begin
                  if (m_atomic_cache_descs[ii].get_paddr() == info.addr) begin
                     atomic_index = ii;
                     break;
                  end
               end

               if (atomic_index != -1) begin
                  req_desc = m_atomic_cache_descs[atomic_index];
                  m_atomic_cache_descs.delete(atomic_index);
               end
               else begin
                  `dut_error(get_abstract_name(), $sformatf("The atomic SM is attempting to evict address 0x%0x, but the scoreboard does not have that entry in its cache!", info.addr))
                  return;
               end

               // If this is the last entry for the State Machine to evict
               // then reset the EVICT_START field because this field is
               // hardware-modifiable and we won't otherwise see the updated value 
               // next time.
               if (atomic_cache_size == 1) begin
                  set_reg_field_value(ESR_MS_ATOMIC_EVICT_START, 0);
               end

               event_type = "an Atomic State Machine Eviction";
            end
            else begin
               req_desc = m_atomic_cache_descs.pop_front();

               event_type = "an Atomic Capacity Eviction";
            end

            // An atomic eviction that originated as a high-priority atomic hit request
            // would have QOS==1, so we need to clear that away because the
            // eviction is now a low-priority arb event
            if (req_desc.get_bus_qos() != evl_bus_qos_t'(0)) begin
               req_desc.set_bus_qos(evl_bus_qos_t'(0));
            end

            qualifier = MSQ_Victim;

            // update the number of entries in the atomic cache
            m_num_atomic_cache_entries--;

         end
         // #2
         else begin
            req_desc = m_input_req_queues[port_queue_id].find_first_write();
            m_input_req_queues[port_queue_id].pop(req_desc);

            event_type = (hi_priority == 1'b1) ? "a High-Priority Write" : "a Low-Priority Write";
            
            if (is_debug == 0) begin
               m_memshire_scoreboard_cover.sample_ms_atomic_cache(1'b0, ET_LINK_REQ_Write, hi_priority);
            end
         end
      end
      else begin
         // #3
         if (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) begin
            req_desc = m_input_req_queues[port_queue_id].find_first_write();
            m_input_req_queues[port_queue_id].pop(req_desc);

            // An atomic request that comes in with QOS==1 and is a miss, will
            // generate a fill request on the low-priority AR arbitration, so
            // we need to clear the QOS
            if (req_desc.get_bus_qos() != evl_bus_qos_t'(0)) begin
               req_desc.set_bus_qos(evl_bus_qos_t'(0));
            end

            event_type = "an Atomic Miss (Read Fill)";

            if (is_debug == 0) begin
               m_memshire_scoreboard_cover.sample_ms_atomic_cache(1'b0, ET_LINK_REQ_Atomic, hi_priority);
            end
         end
         // #4
         else begin
            req_desc = m_input_req_queues[port_queue_id].find_first_read();
            m_input_req_queues[port_queue_id].pop(req_desc);

            event_type = (hi_priority == 1'b1) ? "a High-Priority Read" : "a Low-Priority Read";

            if (is_debug == 0) begin
               m_memshire_scoreboard_cover.sample_ms_atomic_cache(1'b0, ET_LINK_REQ_Read, hi_priority);
            end
         end
      end
      if (req_desc == null) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: process_serialization: no request found for m_input_req_queues[0x%0x] (0x%x, ID 0x%x) (%0s, line %0d)", port_queue_id, info.addr, info.req_id, `__FILE__, `__LINE__))
         return;
      end
      else begin
         `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing %0s serialization event for %0s", event_type, req_desc.sprint_obj()))
      end

      if (req_desc.get_paddr() != info.addr) begin
         `dut_error(get_abstract_name(), $sformatf("expected the address to be 0x%x, not 0x%x", req_desc.get_paddr(), info.addr))
      end

      if ((req_desc.get_bus_req_id() != info.req_id) && (port_queue_id != `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC)) begin
         `dut_error(get_abstract_name(), $sformatf("expected the bus request ID to be 0x%x, not 0x%x", req_desc.get_bus_req_id(), info.req_id))
      end

      // Check the QOS settings based on what kind of event we're processing
      if (hi_priority == 1'b1) begin
         if (req_desc.get_bus_qos() != evl_bus_qos_t'(1)) begin
            `dut_error(get_abstract_name(), $sformatf("serialization is for a HIGH-priority event, but the QOS is 0x%0x!", req_desc.get_bus_qos()))
         end
      end
      else begin
         if (!(req_desc.get_bus_qos() inside {evl_bus_qos_t'(0), evl_bus_qos_t'(3)})) begin
            `dut_error(get_abstract_name(), $sformatf("serialization is for a LOW-priority event, but the QOS is 0x%0x!", req_desc.get_bus_qos()))
         end
      end

      //
      // Do the lookup here and process it.
      //
      new_bus_cmd = ((port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) || (info.write_req == 1'b0)) ?  BusRead : BusWrite;

      if (port_queue_id == `EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) begin
         bus_cmd    = AXI_REQ_WRITE;
         etlink_cmd = ET_LINK_REQ_Atomic;
      end
      else if (info.write_req == 1'b1) begin
         bus_cmd    = AXI_REQ_WRITE;
         etlink_cmd = ET_LINK_REQ_Write;
      end
      else begin
         bus_cmd    = AXI_REQ_READ;
         etlink_cmd = ET_LINK_REQ_Read;
      end

      //
      // Perform the cache lookup.
      //
      m_cache.bus_req_lookup(table_desc, line_desc, MS_TABLE_SERIALIZE, bus_cmd, etlink_cmd, req_desc, qualifier);

      //
      // Process the tag action as necessary.
      //
      if (table_desc.get_tag_action() != St_Ignore) begin
         line_desc.set_cache_st(table_desc.get_tag_action());
      end

      //
      // Process the data action as necessary.
      //
      //
      // Eventually the merge_data should happen on the response
      //
      if (table_desc.get_data_action() inside { Data_FwdAndWr}) begin
         if (req_desc.get_req_data() != null) begin
            line_desc.merge_data_from_line(req_desc.get_req_data(), 1'b1);
         end
         else begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: no request DATA found for m_input_req_queues[0x%0x] (0x%x, ID 0x%x) (%0s, line %0d)", port_queue_id, info.addr, info.req_id, `__FILE__, `__LINE__))
         end
      end

      //
      // Process the response action as necessary.
      //
      //Always set observed so that the response will be checked whenever it shows up
      req_desc.set_observed();
      if (evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()) != RspNull) begin
         req_desc.set_abstract_rsp_type(evl_abstract_rsp_type_t'(table_desc.get_bus_rsp()));
         req_desc.set_bus_rsp_valid(0, get_cycle_count());
      end
      
      
      //
      // Process the bus action as necessary.
      //
      if (table_desc.get_bus_cmd() != BusIdle) begin
         int unsigned esr_mc_bit_sel;
         bit [1:0]    esr_mc_en;
         bit          mc_sel;
         
         
          //TODO: Quick fix for RTLMIN-2200, check if it is appropriate
          logic [`MEMSHIRE_AXI_SLAVE_CONVERTED_ID_SIZE-1:0] converted_axi_slave_id = info.req_id[`MEMSHIRE_AXI_SLAVE_CONVERTED_ID_SIZE-1:0];


         $cast(axi_req_desc, req_desc.clone());
         axi_req_desc.set_trans_id(req_desc.get_trans_id());
         axi_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(bus_cmd));
         axi_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(new_bus_cmd));
         axi_req_desc.set_abstract_bus_cmd(evl_abstract_bus_cmd_t'(table_desc.get_bus_cmd()));
         axi_req_desc.set_paddr(compute_ddr_addr(req_desc.get_paddr(), is_debug));
         axi_req_desc.set_bus_req_user(0);
         axi_req_desc.set_bus_req_id(info.grants | converted_axi_slave_id);

         if (table_desc.get_bus_cmd() inside { BusWrite }) begin
            evl_line_data req_data;

            req_data = req_desc.get_req_data();
            // make a copy of the req data for the axi req desc rather than giving it a pointer to the same data
            axi_req_desc.set_req_data(req_data.clone_line_data());
         end
         if (table_desc.get_bus_cmd() inside { BusVictim }) begin
            evl_line_data req_data;

            req_data = line_desc.get_line_data();
            // make a copy of the req data for the axi req desc rather than giving it a pointer to the same data
            axi_req_desc.set_req_data(req_data.clone_line_data());
         end

         req_desc.add_child_desc(axi_req_desc);

         addr = req_desc.get_paddr();
         if (is_debug == 1'b1) begin
            esr_mc_bit_sel = get_reg_field_value(ESR_MS_ADDR_DEBUG_MC_BIT_SEL);
         end
         else begin
            esr_mc_bit_sel = get_reg_field_value(ESR_MS_ADDR_MC_BIT_SEL);
         end
         esr_mc_en      = get_reg_field_value(ESR_MS_MC_EN);

         // figure out which mem controller the request should be send to
         case (esr_mc_en)
            2'b11: begin
               if (esr_mc_bit_sel != 63) begin
                  mc_sel = addr[esr_mc_bit_sel];
               end
               else if (esr_mc_bit_sel > 39) begin
                  mc_sel = 0;
               end
            end
            2'b10: begin
               mc_sel = 1;
            end
            2'b01: begin
               mc_sel = 0;
            end
            2'b00: begin
               // y'all f'ed up
               mc_sel = 0;
            end
            default: begin
               mc_sel = 0;
            end
         endcase

         if (req_desc.get_bus_qos() == 1) begin
            if (mc_sel == 1'b0) begin
               m_axi_req_queues[`EVL_DDR_HI_EVEN_PORT].push(axi_req_desc);
               m_pending_ddr_reqs[`EVL_DDR_HI_EVEN_PORT].push(axi_req_desc);
            end
            else begin
               m_axi_req_queues[`EVL_DDR_HI_ODD_PORT].push(axi_req_desc);
               m_pending_ddr_reqs[`EVL_DDR_HI_ODD_PORT].push(axi_req_desc);
            end
         end
         else begin
            if (mc_sel == 1'b0) begin
               m_axi_req_queues[`EVL_DDR_LO_EVEN_PORT].push(axi_req_desc);
               m_pending_ddr_reqs[`EVL_DDR_LO_EVEN_PORT].push(axi_req_desc);
            end
            else begin
               m_axi_req_queues[`EVL_DDR_LO_ODD_PORT].push(axi_req_desc);
               m_pending_ddr_reqs[`EVL_DDR_LO_ODD_PORT].push(axi_req_desc);
            end
         end // else: !if(req_desc.get_bus_qos() == 1)

         m_memshire_scoreboard_cover.sample_predict_downstream_request(m_regs, axi_req_desc, is_debug);
      end // if (table_desc.get_bus_cmd() != BusIdle)
      

      // keep track of pending atomic requests (for process_bus_respnse on an
      // atomic miss) and add to the "atomic_cache" queue
      if ((etlink_cmd == ET_LINK_REQ_Atomic) && (qualifier != MSQ_Victim)) begin
         req_desc.set_int_param(pk_desc_is_atomic, 1);
         m_pending_cache_writes.push_back(req_desc);
         m_atomic_cache_descs.push_back(req_desc);
         m_num_atomic_cache_entries++;
      end
   endfunction : process_serialization_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: predict_atomic_response
   //
   // This function is used to create the predicted request on the atomic_response port
   //
   // Input Variables:
   //
   // req_desc_in: This request descriptor contains the information used to
   // create the atomic_response request
   //
   function void predict_atomic_response(input evl_req_desc  req_desc_in,
                                         input evl_line_data req_data_in);
      evl_req_desc       axi_req_desc;
      evl_qword_t        axi_info_qword;
      evl_paddr_t        axi_lower_addr;
      evl_paddr_t        axi_shire_id;
      evl_bus_req_user_t axi_wuser_id;
      evl_paddr_t        atomic_rsp_addr;

      $cast(axi_req_desc, req_desc_in.clone());
      axi_req_desc.set_trans_id(req_desc_in.get_trans_id());
      axi_req_desc.set_abstract_bus_cmd(BusWrite);

      axi_info_qword  = req_desc_in.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB + 1));
      axi_lower_addr  = evl_paddr_t'(axi_info_qword >> `EVL_AXI_ATOMIC_ADDR_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_ADDR_MASK);
      axi_shire_id    = evl_paddr_t'(axi_info_qword >> `EVL_AXI_ATOMIC_SOURCE_SHIFT) & evl_paddr_t'(`EVL_AXI_ATOMIC_SOURCE_MASK);
      axi_wuser_id    = (axi_info_qword >> `EVL_AXI_ATOMIC_TRANSID_SHIFT) & evl_qword_t'(`EVL_AXI_ATOMIC_TRANSID_MASK);
      atomic_rsp_addr = (`SC_L3_ATOMIC_RSP_ADDR_OFFSET | evl_paddr_t'(axi_lower_addr) | (evl_paddr_t'(axi_shire_id) << `SC_L3_ATOMIC_RSP_SHIRE_ID_SHIFT));

      axi_req_desc.set_paddr(atomic_rsp_addr);
      axi_req_desc.set_bus_req_user(axi_wuser_id);
      axi_req_desc.set_bus_req_id(req_desc_in.get_bus_req_id());

      if ((req_desc_in.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_VEC_OP) == 0) begin
         if ((req_desc_in.get_bus_atomic_conf() & `EVL_BUS_ATOMIC_CONF_DW_OP) == 0) begin
            axi_req_desc.set_bus_req_size(`EVL_WORD_ADDR_LSB);
         end
         else begin
            axi_req_desc.set_bus_req_size(`EVL_DWORD_ADDR_LSB);
         end
      end
      else begin
         axi_req_desc.set_bus_req_size(`EVL_OWORD_ADDR_LSB);
      end

      // make a copy of the req data for the axi req desc rather than giving it a pointer to the same data
      axi_req_desc.set_req_data(req_data_in.clone_line_data());

      req_desc_in.add_child_desc(axi_req_desc);

      // add the req_desc to the correct queue
      m_axi_req_queues[`EVL_ATOMIC_TO_MESH_PORT].push(axi_req_desc);

      axi_req_desc.set_bus_req_id(0);
      axi_req_desc.set_bus_qos(2);
   endfunction : predict_atomic_response


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_cache_wr_event
   //
   // This function is used to process grey-box information (an atomic cache write event).  Grey-box
   // information is sent from the evl_memshire_interface via calls to send_packet() in the RTL
   // port.  These are then processed by a task in the parent memshire agent, which then calls
   // process_rtl_event() in this scoreboard.
   //
   // Input Variables:
   //
   // info:        This structure contains the RTL event information.
   //
   function void process_cache_wr_event();
      int                    port_queue_id;
      evl_paddr_t            addr;
      evl_req_desc           req_desc;
      evl_bus_cmd_t          bus_cmd;
      evl_bus_cmd_t          etlink_cmd;
      evl_ms_qual_t          qualifier;
      evl_table_desc         table_desc;
      evl_line_desc          line_desc;
      int unsigned           atomic_cache_size;
      int unsigned           atomic_cache_hit_index;


      if (m_pending_cache_writes.size() == 0) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_pending_cache_writes does not have a req_desc for a pending write/atomic!  (%0s, line %0d)",  `__FILE__, `__LINE__))
         return;
      end

      req_desc = m_pending_cache_writes.pop_front();

      // figure out if we're dealing with a write from an atomic or a regular write hit
      if (req_desc.have_int_param(pk_desc_is_atomic) == 1'b1) begin
         `evl_log_mdesc(UVM_MEDIUM, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing an atomic cache atomic-write event for %0s", req_desc.sprint_obj()))
         bus_cmd    = AXI_REQ_WRITE;
         etlink_cmd = ET_LINK_REQ_Atomic;
      end
      else begin
         `evl_log_mdesc(UVM_MEDIUM, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing an atomic cache write event for %0s", req_desc.sprint_obj()))
         bus_cmd    = AXI_REQ_WRITE;
         etlink_cmd = ET_LINK_REQ_Write;
      end

      qualifier = MSQ_None;

      m_cache.bus_req_lookup(table_desc, line_desc, MS_TABLE_CACHE_WR, bus_cmd, etlink_cmd, req_desc, qualifier);

      //
      // Process the tag action as necessary.
      //
      if (table_desc.get_tag_action() != St_Ignore) begin
         line_desc.set_cache_st(table_desc.get_tag_action());
      end

      //
      // Process the data action as necessary
      //
      if (table_desc.get_data_action() inside { Data_MemWr }) begin
         if (req_desc.get_req_data() != null) begin
            line_desc.merge_data_from_line(req_desc.get_req_data(), 1'b1);
         end
         else begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: process_cache_wr_event: no request DATA found for m_pending_cache_writes (%0s) (%0s, line %0d)", req_desc.sprint_obj(), `__FILE__, `__LINE__))
         end
      end

      //
      // Process the bus action as necessary.
      //
      if (table_desc.get_data_action() inside { Data_WrAtomic }) begin
         evl_line_data  atomic_response;   // response of the atomic operation, needs to be here for axi_req to use
         evl_qword_t    qword;

         qword = req_desc.get_req_qword(`SC_ATOMIC_DATA_SIZE / (`EVL_QWORD_MSB - 1));
         req_desc.set_bus_atomic_conf(qword[`EVL_BUS_ATOMIC_CONF_WIDTH - 2:0]);
         req_desc.set_bus_atomic_offset(qword >> (`EVL_BUS_ATOMIC_CONF_WIDTH - 1));

         //sample coverage:
         m_memshire_scoreboard_cover.sample_ms_atomics(req_desc, line_desc);

         atomic_response = line_desc.process_atomic(req_desc, 1'b0, 1'b1);
         line_desc.set_modified_qword_en();

         if (table_desc.get_bus_cmd() == BusWriteAtomic) begin
            predict_atomic_response(req_desc, atomic_response);
         end
      end
      
      if (req_desc.have_int_param(pk_desc_is_atomic) == 1'b1) begin
         if (`evl_log_check_verbosity(UVM_HIGH, VMOD_MS_SB) != 0) begin
            print_atomic_cache_lru();
         end
      end
   endfunction : process_cache_wr_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_reg_arb_event
   //
   // This function is used to process grey-box information (a register access event).  Grey-box
   // information is sent from the evl_memshire_interface via calls to send_packet() in the RTL
   // port.  These are then processed by a task in the parent memshire agent, which then calls
   // process_rtl_event() in this scoreboard.
   //
   // Input Variables:
   //
   // info:        This structure contains the RTL event information.
   //
   function void process_reg_arb_event(input evl_memshire_reg_arb_t info);

      if ((info.sel_bpam_mux == 1'b0) && (info.sel_ai_mux == 1'b0)) begin
         m_reg_access_is_from_sbm = 1'b1;
      end
      else begin
         m_reg_access_is_from_sbm = 1'b0;
      end
      `evl_log(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), $sformatf("Received a REG_ARB event.  sel_bpam_mux:%0b   sel_ai_mux:%0b   reg_access_is_from_sbm:%0b", info.sel_bpam_mux, info.sel_ai_mux, m_reg_access_is_from_sbm))

   endfunction : process_reg_arb_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_register_setup
   //
   // This function is called whenever a new register request is observed on the APB interface
   // This is the "setup" phase of the APB transfer (sel==1, enable==0)
   //
   // Input Variables:
   //
   // req_desc:    This is the incoming request descriptor.
   //
   function void process_register_setup(input evl_req_desc req_desc);
      int                    port_queue_id;
      evl_req_desc           src_req_desc;

      if (m_reg_access_is_from_sbm == 1'b1) begin
         port_queue_id = `EVL_MEMSHIRE_REQ_ID_FOR_SBM;

         if (!(m_input_req_queues.exists(port_queue_id))) begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_input_req_queues[0x%0x] does not exist for %0s (%0s, line %0d)", port_queue_id, req_desc.sprint_obj(), `__FILE__, `__LINE__))
            return;
         end

         // find the original incoming AXI request
         // don't pop it here because we'll need to look it up again in the "access" phase
         if (req_desc.bus_cmd_is_write() == 1'b1) begin
            src_req_desc = m_input_req_queues[port_queue_id].find_first_write();
         end 
         else begin
            src_req_desc = m_input_req_queues[port_queue_id].find_first_read();
         end

         if (src_req_desc != null) begin
            // link up the APB req_desc to the AXI desc
            src_req_desc.add_child_desc(req_desc);
            `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing a register setup event for %0s", req_desc.sprint_obj()))
         end
      end
      
   endfunction : process_register_setup


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_register_access
   //
   // This function is called whenever a new register request is observed on the APB interface
   // This is the "access" phase of the APB transfer (sel==1, enable==1)
   //
   // Input Variables:
   //
   // req_desc:    This is the incoming request descriptor.
   //
   function void process_register_access(input evl_req_desc req_desc);
      int                    port_queue_id;
      evl_req_desc           src_req_desc;

      if (m_reg_access_is_from_sbm == 1'b1) begin
         port_queue_id = `EVL_MEMSHIRE_REQ_ID_FOR_SBM;

         if (!(m_input_req_queues.exists(port_queue_id))) begin
            `dut_error(get_abstract_name(), $sformatf("TB_FAIL: m_input_req_queues[0x%0x] does not exist for %0s (%0s, line %0d)", port_queue_id, req_desc.sprint_obj(), `__FILE__, `__LINE__))
            return;
         end

         // find the original incoming AXI request
         if (req_desc.bus_cmd_is_write() == 1'b1) begin
            src_req_desc = m_input_req_queues[port_queue_id].find_first_write();
         end 
         else begin
            src_req_desc = m_input_req_queues[port_queue_id].find_first_read();
         end
         m_input_req_queues[port_queue_id].pop(src_req_desc);

         if (src_req_desc != null) begin
            // link up the APB req_desc to the AXI desc
            src_req_desc.add_child_desc(req_desc);
            `evl_log_mdesc(UVM_HIGH, VMOD_MS_SB, get_abstract_name(), req_desc, $sformatf("processing a register access event for %0s", req_desc.sprint_obj()))

            // fill in the response type and data (if it was a read)
            if (req_desc.bus_cmd_is_read() == 1'b1) begin
               src_req_desc.set_rsp_data(req_desc.get_rsp_data());
            end 

            if (req_desc.bus_rsp_has_error() == 1'b1) begin
               src_req_desc.set_bus_rsp_type(AXI_RESP_SLVERR);
            end
            else begin
               src_req_desc.set_bus_rsp_type(AXI_RESP_OKAY);
            end

            src_req_desc.set_bus_rsp_valid(0, get_cycle_count());
         end
      end

   endfunction : process_register_access


   //-----------------------------------------------------------------------------------------------
   //
   // Function: process_rtl_event
   //
   // This function is used to process all grey-box information sent from the evl_memshire_interface
   // via calls to send_packet() in the RTL port.  The send_packet() calls are processed by a task
   // in the parent memshire agent, which then calls this function.  It is the responsibility of
   // this function to parse the RTL information appropriately.
   //
   // Input Variables:
   //
   // rtl_event: This class instance contains the RTL event information.
   //
   function void process_rtl_event(evl_memshire_packet rtl_event);
      evl_memshire_packet_u packet_info;

      packet_info = rtl_event.get_info();

      if (rtl_event.get_packet_type() == MEMSHIRE_LO_SERIALIZE) begin
         process_serialization_event(1'b0, packet_info.serialize_info.info);
      end
      else if (rtl_event.get_packet_type() == MEMSHIRE_HI_SERIALIZE) begin
         process_serialization_event(1'b1, packet_info.serialize_info.info);
      end
      else if (rtl_event.get_packet_type() == MEMSHIRE_ATOMIC_HIT) begin
         process_atomic_hit_event(packet_info.atomic_info.info);
      end
      else if (rtl_event.get_packet_type() == MEMSHIRE_ATOMIC_RSP_PUSH) begin
         process_atomic_rsp_push_event(packet_info.atomic_info.info);
      end
      else if (rtl_event.get_packet_type() == MEMSHIRE_CACHE_WR) begin
         process_cache_wr_event();
      end
      else if (rtl_event.get_packet_type() == MEMSHIRE_REG_ARB) begin
         process_reg_arb_event(packet_info.reg_arb_info.info);
      end
//    else begin
//       `dut_error(get_abstract_name(), $sformatf("TB_FAIL: received an unknown RTL event (0x%0x)!", int'(rtl_event.get_packet_type())))
//    end
   endfunction : process_rtl_event


   //-----------------------------------------------------------------------------------------------
   //
   // Function: print_atomic_cache_lru
   //
   // This function is used to print out the current Atomic Cache LRU order
   //
   // Input Variables: 
   //    
   //    none
   //
   function print_atomic_cache_lru();
      `evl_log(UVM_MEDIUM, VMOD_MS_SB, get_abstract_name(), $sformatf("Current Atomic Cache (LRU listed first)"))
      for (int ii = 0; ii <  m_atomic_cache_descs.size(); ii++) begin
         `evl_log(UVM_MEDIUM, VMOD_MS_SB, get_abstract_name(), $sformatf("    [%0d]: %0s", ii, m_atomic_cache_descs[ii].sprint_obj()))
      end
   endfunction : print_atomic_cache_lru


   //-----------------------------------------------------------------------------------------------
   //
   // Function: initialize_regs
   //
   virtual function void initialize_regs();
      string        reg_field_names_no_check[$];

      // reset each reg model to the initial values
      m_regs.reset();

      // cherry-pick some fields we don't want to compare, as these are all 
      // hardware-modifiable so we can't have auto-checks on these fields
      // Many are tested/checked manually
      reg_field_names_no_check.push_back("ESR_MS_ATOMIC_EVICT_START");
      reg_field_names_no_check.push_back("ESR_MS_ATOMIC_STATE");

      reg_field_names_no_check.push_back("ESR_DDRC_U0_CACTIVE_DDRC");
      reg_field_names_no_check.push_back("ESR_DDRC_U0_CSYSACK_DDRC");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_CACTIVE_DDRC");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_CSYSACK_DDRC");
      reg_field_names_no_check.push_back("ESR_DDRC_U0_CACTIVE_LO");
      reg_field_names_no_check.push_back("ESR_DDRC_U0_CSYSACK_LO");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_CACTIVE_LO");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_CSYSACK_LO");
      reg_field_names_no_check.push_back("ESR_DDRC_U0_CACTIVE_HI");
      reg_field_names_no_check.push_back("ESR_DDRC_U0_CSYSACK_HI");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_CACTIVE_HI");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_CSYSACK_HI");

      reg_field_names_no_check.push_back("ESR_DDRC_U0_MRR_DATA_VALID");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_MRR_DATA_VALID");
      reg_field_names_no_check.push_back("ESR_DDRC_U0_MRR_DATA");
      reg_field_names_no_check.push_back("ESR_DDRC_U1_MRR_DATA");

      reg_field_names_no_check.push_back("ESR_CYC_COUNT_OVERFLOW_INTR");
      reg_field_names_no_check.push_back("ESR_PERF_COUNT1_OVERFLOW_INTR");
      reg_field_names_no_check.push_back("ESR_PERF_COUNT0_OVERFLOW_INTR");
      reg_field_names_no_check.push_back("ESR_MC1_DERATE_TEMP_LIMIT_INTR");
      reg_field_names_no_check.push_back("ESR_MC0_DERATE_TEMP_LIMIT_INTR");
      reg_field_names_no_check.push_back("ESR_MC1_SBR_DONE_INTR");
      reg_field_names_no_check.push_back("ESR_MC0_SBR_DONE_INTR");
      reg_field_names_no_check.push_back("ESR_MC1_ECC_CORRECTED_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_MC0_ECC_CORRECTED_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_DDRPHY_INTR");
      reg_field_names_no_check.push_back("ESR_DFI1_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_DFI0_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_MC1_DFI_ALERT_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_MC0_DFI_ALERT_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_MC1_ECC_UNCORRECTED_ERR_INTR");
      reg_field_names_no_check.push_back("ESR_MC0_ECC_UNCORRECTED_ERR_INTR");
      
      reg_field_names_no_check.push_back("ESR_DFI1_ERR_INFO");
      reg_field_names_no_check.push_back("ESR_DFI1_ERR");
      reg_field_names_no_check.push_back("ESR_DFI0_ERR_INFO");
      reg_field_names_no_check.push_back("ESR_DFI0_ERR");

      reg_field_names_no_check.push_back("ESR_P1_SO");
      reg_field_names_no_check.push_back("ESR_P1_SA");
      reg_field_names_no_check.push_back("ESR_P0_SO");
      reg_field_names_no_check.push_back("ESR_P0_SA");
      reg_field_names_no_check.push_back("ESR_CYC_SO");
      reg_field_names_no_check.push_back("ESR_CYC_SA");

      reg_field_names_no_check.push_back("ESR_CYC_CNTR");
      reg_field_names_no_check.push_back("ESR_P0_CNTR");
      reg_field_names_no_check.push_back("ESR_P1_CNTR");

      foreach (reg_field_names_no_check[jj]) begin
         uvm_reg_field reg_field;
         reg_field = m_regs.get_field_by_name(reg_field_names_no_check[jj]);
         reg_field.set_compare(UVM_NO_CHECK);
      end
      
      if (`evl_log_check_verbosity(UVM_HIGH, VMOD_MS_SB) != 0) begin
         m_regs.print();
      end
   endfunction: initialize_regs

   

   //------------------------------------------------------------------------------------------------
   //
   // Function: get_reg_field_value
   //
   // This function is used to check the current value of an ESR field in the
   // register model
   //
   // BEWARE: fields that are volatile (writeable by hardware) may not match
   // the RTL value!
   //
   function evl_dword_t get_reg_field_value(input evl_memshire_reg_field_names_t reg_field);
      uvm_reg_field field;
      evl_dword_t   field_value;

      field = m_regs.get_field_by_name($sformatf("%0s", reg_field.name()));

      if (field == null) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: tried to lookup the value for field %0s, but the field was not found!", reg_field.name()))
         field_value = ~evl_dword_t'(0);
      end
      else begin
         field_value = field.get();
      end

      return field_value;
   endfunction : get_reg_field_value


   //------------------------------------------------------------------------------------------------
   //
   // Function: set_reg_field_value
   //
   // This function is used to set the value of an ESR field in the register
   // model
   //
   function void set_reg_field_value(input evl_memshire_reg_field_names_t reg_field,
                                     input evl_dword_t                    data
                                     );
      uvm_reg_field field;

      field = m_regs.get_field_by_name($sformatf("%0s", reg_field.name()));

      if (field == null) begin
         `dut_error(get_abstract_name(), $sformatf("TB_FAIL: tried to set the value for field %0s, but the field was not found!", reg_field.name()))
      end
      else begin
         field.set(data);
      end
   endfunction : set_reg_field_value


   //------------------------------------------------------------------------------------------------
   //
   // Function: build_table
   //
   // This function is used to build the lookup tables for bus requests and bus responses.
   //
   virtual function void build_table();
      // "Atomic Hit"
      //                                                      Model                                      Bus       Bus
      //                 AXI Command     ET-link Opcode       State  Qual       Tag Action  Data Action  Command   Rsp Type   Err Code
      //                 --------------  -------------------  -----  ---------  ----------  -----------  --------  --------   ------------
      add_atomic_req_act(AXI_REQ_READ,   ET_LINK_REQ_Read,    E,     MSQ_None,  St_Ignore,  Data_MemRd,  BusIdle,  RspWData,  EC_IncrCount );
      add_atomic_req_act(AXI_REQ_WRITE,  ET_LINK_REQ_Write,   E,     MSQ_None,  St_Ignore,  Data_MemWr,  BusIdle,  RspAck,    EC_None      );
      add_atomic_req_act(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  E,     MSQ_None,  St_Ignore,  Data_ReqWr,  BusIdle,  RspAck,    EC_None      );

      // "Lo-Pri Arbitration/Serialization"
      //                                                   Model                                          Bus        Bus
      //              AXI Command     ET-link Opcode       State  Qual        Tag Action  Data Action     Command    Rsp Type  Err Code
      //              --------------  -------------------  -----  ---------   ----------  --------------  --------   --------  ------------
      add_bus_req_act(AXI_REQ_READ,   ET_LINK_REQ_Read,    I,     MSQ_None,   St_Ignore,  Data_None,      BusRead,   RspNull,  EC_IncrCount );
      add_bus_req_act(AXI_REQ_WRITE,  ET_LINK_REQ_Write,   I,     MSQ_None,   St_Ignore,  Data_FwdAndWr,  BusWrite,  RspNull,  EC_None      );
      add_bus_req_act(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  I,     MSQ_None,   St_Ignore,  Data_None,      BusRead,   RspNull,  EC_None      );
      add_bus_req_act(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  E,     MSQ_Victim, I,          Data_MemWr,     BusVictim, RspNull,  EC_None      );

      // "Atomic Response Push"
      //                                                      Model                                        Bus              Bus
      //                 AXI Command     ET-link Opcode       State  Qual         Tag Action  Data Action  Command          Rsp Type  Err Code
      //                 --------------  -------------------  -----  -----------  ----------  -----------  ---------------  --------  --------
      add_bus_req_act_nc(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  I,     MSQ_SndRsp,  St_Ignore,  Data_None,   BusWriteAtomic,  RspNull,  EC_None  );
      add_bus_req_act_nc(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  E,     MSQ_SndRsp,  St_Ignore,  Data_None,   BusWriteAtomic,  RspNull,  EC_None  );

      // "Bus Response"
      //                                                   Model                                                                       Bus
      //               AXI Command    ET-link Opcode       State  AXI Response    Qual       Tag Action  Data Action        Rsp Type   Command   Err Code
      //              --------------  -------------------  -----  --------------  ---------  ----------  -----------------  ---------  --------  --------
      add_bus_rsp_act(AXI_REQ_READ,   ET_LINK_REQ_Read,    I,     AXI_RESP_OKAY,  MSQ_None,  St_Ignore,  Data_FwdAndCheck,  RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(AXI_REQ_READ,   ET_LINK_REQ_Read,    E,     AXI_RESP_OKAY,  MSQ_None,  St_Ignore,  Data_FwdRd,        RspWData,  BusIdle,  EC_None  );
      add_bus_rsp_act(AXI_REQ_READ,   ET_LINK_REQ_Atomic,  I,     AXI_RESP_OKAY,  MSQ_None,  St_Ignore,  Data_CacheFill,    RspAck,    BusIdle,  EC_None  );
      add_bus_rsp_act(AXI_REQ_WRITE,  ET_LINK_REQ_Write,   I,     AXI_RESP_OKAY,  MSQ_None,  St_Ignore,  Data_None,         RspAck,    BusIdle,  EC_None  );
      add_bus_rsp_act(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  I,     AXI_RESP_OKAY,  MSQ_None,  St_Ignore,  Data_None,         RspNull,   BusIdle,  EC_None  );
      // The ReadRsp with state "E" is in the event that a ReadMiss & AtomicMiss to the same address at the same time, and the Atomic Fill completes before
      // the ReadMiss response, so the line is "E" when that response comes back.  This is okay.

      // "Cache Write"
      //                                                    Model                                         Bus       Bus
      //               AXI Command     ET-link Opcode       State  Qual       Tag Action  Data Action     Command   Rsp Type  Err Code
      //               --------------  -------------------  -----  ---------  ----------  --------------  --------  --------  --------
      add_cache_wr_act(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  I,     MSQ_None,  E,          Data_WrAtomic,  BusIdle,  RspNull,  EC_None  );
      add_cache_wr_act(AXI_REQ_WRITE,  ET_LINK_REQ_Write,   E,     MSQ_None,  St_Ignore,  Data_MemWr,     BusIdle,  RspNull,  EC_None  );
      add_cache_wr_act(AXI_REQ_WRITE,  ET_LINK_REQ_Atomic,  E,     MSQ_None,  St_Ignore,  Data_WrAtomic,  BusIdle,  RspNull,  EC_None  );
   endfunction : build_table


   //------------------------------------------------------------------------------------------------
   //
   // Function: build_phase
   //
   virtual function void build_phase(uvm_phase phase);
      super.build_phase(phase);
      $cast(m_memshire_rtl_port, m_rtl_port);
      if (m_memshire_rtl_port == null) begin
         `dut_error(get_abstract_name(), "TB_FAIL: m_rtl_port is null, SAD!")
      end
      else begin
         m_debug_no_inst = m_memshire_rtl_port.get_int_param(pk_debug_no_inst, 0);
      end

      set_abstract_name($sformatf("S%0d:MEMSHIRE(SB)", get_shire_id()));
      m_axi_port_names[`EVL_MEMSHIRE_REQ_ID_FOR_MESH]   = $sformatf("S%0d:MESH_MEM(AXI)", get_shire_id());
      m_axi_port_names[`EVL_MEMSHIRE_REQ_ID_FOR_UST]    = $sformatf("S%0d:UST_MEM(AXI)", get_shire_id());
      m_axi_port_names[`EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC] = $sformatf("S%0d:MESH_MEM(AXI)", get_shire_id());
      m_axi_port_names[`EVL_DDR_LO_EVEN_PORT]    = $sformatf("S%0d:MEM_DDR_LO-0(AXI)", get_shire_id());
      m_axi_port_names[`EVL_DDR_LO_ODD_PORT]     = $sformatf("S%0d:MEM_DDR_LO-1(AXI)", get_shire_id());
      m_axi_port_names[`EVL_DDR_HI_EVEN_PORT]    = $sformatf("S%0d:MEM_DDR_HI-0(AXI)", get_shire_id());
      m_axi_port_names[`EVL_DDR_HI_ODD_PORT]     = $sformatf("S%0d:MEM_DDR_HI-1(AXI)", get_shire_id());
      m_axi_port_names[`EVL_ATOMIC_TO_MESH_PORT] = $sformatf("S%0d:MEM_ATOMIC(AXI)", get_shire_id());

      m_cache = new("m_cache", this);
      m_cache.set_abstract_name($sformatf("%s($)", get_root_abstract_name()));
      m_cache.set_cache_type("$");
      m_cache.set_verbosity_mods(VMOD_MS_SB, VMOD_MS_SB_WARN);
      m_cache.enable_coverage();

      build_table();

      m_cache.build_coverage();
   endfunction : build_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);

      m_template_req_data_axi   = evl_line_data::type_id::create($sformatf("S%0d:DDR(AXI)", get_shire_id()));
      m_template_line_desc_axi  = evl_line_desc::type_id::create($sformatf("S%0d:DDR(AXI)", get_shire_id()));
      m_template_trans_desc_axi = evl_trans_desc::type_id::create($sformatf("S%0d:DDR(AXI)", get_shire_id()));
      m_template_req_desc_axi = evl_req_desc::type_id::create($sformatf("S%0d:DDR(AXI)", get_shire_id()));

      m_template_trans_desc_axi.set_shire_id(get_shire_id());
      m_template_req_desc_axi.set_shire_id(get_shire_id());

      m_template_line_desc_axi.set_data_template(m_template_req_data_axi);
      m_template_trans_desc_axi.set_data_template(m_template_req_data_axi);
      m_template_req_desc_axi.set_data_template(m_template_req_data_axi);
      m_template_req_desc_axi.set_bus_type(EVL_BUS_TYPE_AXI);

      m_template_trans_desc_axi.set_line_template(m_template_line_desc_axi);
      m_template_req_desc_axi.set_line_template(m_template_line_desc_axi);

      m_axi_req_queues[`EVL_DDR_LO_EVEN_PORT]    = new($sformatf("%0s(QLO0)", get_root_abstract_name()));
      m_axi_req_queues[`EVL_DDR_LO_ODD_PORT]     = new($sformatf("%0s(QLO1)", get_root_abstract_name()));
      m_axi_req_queues[`EVL_DDR_HI_EVEN_PORT]    = new($sformatf("%0s(QHI0)", get_root_abstract_name()));
      m_axi_req_queues[`EVL_DDR_HI_ODD_PORT]     = new($sformatf("%0s(QHI1)", get_root_abstract_name()));
      m_axi_req_queues[`EVL_ATOMIC_TO_MESH_PORT] = new($sformatf("%0s(ARSP)", get_root_abstract_name()));

      m_input_req_queues[int'(`EVL_MEMSHIRE_REQ_ID_FOR_MESH)   | int'(0)] = new($sformatf("%0s(MESHQ-LO)", get_root_abstract_name()));
      m_input_req_queues[int'(`EVL_MEMSHIRE_REQ_ID_FOR_MESH)   | int'(1)] = new($sformatf("%0s(MESHQ-HI)", get_root_abstract_name()));
      m_input_req_queues[int'(`EVL_MEMSHIRE_REQ_ID_FOR_UST)    | int'(0)] = new($sformatf("%0s(USTQ)", get_root_abstract_name()));
      m_input_req_queues[int'(`EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC) | int'(0)] = new($sformatf("%0s(ATOMICQ)", get_root_abstract_name()));
      m_input_req_queues[int'(`EVL_MEMSHIRE_REQ_ID_FOR_SBM)    | int'(0)] = new($sformatf("%0s(SBM)", get_root_abstract_name()));

      m_pending_ddr_reqs[`EVL_DDR_LO_EVEN_PORT] = new("ddr_LO0_pending_reqs");
      m_pending_ddr_reqs[`EVL_DDR_LO_ODD_PORT]  = new("ddr_LO1_pending_reqs");
      m_pending_ddr_reqs[`EVL_DDR_HI_EVEN_PORT] = new("ddr_HI0_pending_reqs");
      m_pending_ddr_reqs[`EVL_DDR_HI_ODD_PORT]  = new("ddr_HI1_pending_reqs");
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Function: main_phase
   //
   virtual task main_phase(uvm_phase phase);
      phase.raise_objection(this);
      fork
         begin
            super.main_phase(phase);
         end
         begin
            if ((m_memshire_rtl_port != null) && (m_memshire_scoreboard_cover != null)) begin
               m_memshire_scoreboard_cover.sample_ms_frequency(m_memshire_rtl_port);
            end
         end
      join
      phase.drop_objection(this);
   endtask : main_phase


   //----------------------------------------------------------------------------------------------
   //
   // Task: run_phase
   //
   virtual task run_phase(uvm_phase phase);
      fork
         // this is a task from the base evl_verif_comp that checks anything on the
         // m_active_mem_descs list for timeouts. The Scoreboard is going to add and remove
         // things from that list.
         super.run_phase(phase);
         watch_for_timeouts();
      join_none;
      initialize_regs();
      m_num_atomic_cache_entries = 0;
   endtask: run_phase


// //----------------------------------------------------------------------------------------------
// //
// // Task: check_phase
// //
// virtual function void check_phase(uvm_phase phase);
//    int            line_list_size;
//    evl_line_desc  line_list[];
//    evl_req_desc   cbuf_req_descs[$];
//    evl_paddr_t    addr;
//    evl_cache_st_t cache_st;

//    super.check_phase(phase);
//    m_shire_cache_reqq.print_active_reqq_entries();
//    if (m_global_attrs.get_error_count() > 0) begin
//       return;
//    end
//    line_list_size = m_cache.get_valid_line_list(line_list);

//    //
//    // Gather all of the coalescing buffer request descriptors and addresses that are currently
//    // modeled in the scoreboard.
//    //
//    foreach (m_coalescing_buffer[bank_id]) begin
//       evl_req_desc req_desc;

//       req_desc = m_coalescing_buffer[bank_id].pop();
//       while (req_desc != null) begin
//          cbuf_req_descs.push_back(req_desc);
//          m_mdl_cbuf_addr_queues[bank_id].push_back(req_desc.get_paddr());
//          req_desc = m_coalescing_buffer[bank_id].pop();
//       end
//    end

//    //
//    // First, compare RTL coalescing buffer addresses to those in that are modeled in the
//    // scoreboard.  If there is an address in RTL that is not modeled, flag an error.  If there is
//    // a match, delete the model address (so we can see if there are any left over when we are
//    // done).
//    //
//    for (int bank_id = 0; bank_id < `SC_BANKS; bank_id++) begin
//       bit have_data;

//       for (int ii = m_rtl_cbuf_addr_queues[bank_id].size() - 1; ii >= 0; ii--) begin
//          have_data = 1'b0;
//          for (int jj = m_mdl_cbuf_addr_queues[bank_id].size() - 1; jj >= 0; jj--) begin
//             if (m_rtl_cbuf_addr_queues[bank_id][ii] == m_mdl_cbuf_addr_queues[bank_id][jj]) begin
//                have_data = 1'b1;
//                m_mdl_cbuf_addr_queues[bank_id].delete(jj);
//                break;
//             end
//          end
//          if (have_data == 1'b0) begin
//             `dut_error(get_abstract_name(), $sformatf("RTL has an unexpected address in the coalescing buffer for bank %0d (0x%x)", bank_id, m_rtl_cbuf_addr_queues[bank_id][ii]), pk_ms_sb_error_intensity, { VMOD_MS_SB_WARN, m_verbosity_mod })
//          end
//       end
//    end

//    //
//    // Now see if we have any modeled coalescing buffer addresses that are not found in RTL.
//    //
//    for (int bank_id = 0; bank_id < `SC_BANKS; bank_id++) begin
//       for (int ii = m_mdl_cbuf_addr_queues[bank_id].size() - 1; ii >= 0; ii--) begin
//          `dut_error(get_abstract_name(), $sformatf("RTL does not have an expected address in the coalescing buffer for bank %0d (0x%x)", bank_id, m_mdl_cbuf_addr_queues[bank_id][ii]), pk_ms_sb_error_intensity, { VMOD_MS_SB_WARN, m_verbosity_mod })
//       end
//    end

//    for (int ii = 0; ii < line_list_size; ii++) begin
//       addr     = line_list[ii].get_paddr();
//       cache_st = line_list[ii].get_cache_st();
//       if (cache_st inside { `EVL_LINE_WA_DIRTY_STATES }) begin
//          bit found_match = 1'b0;

//          for (int jj = cbuf_req_descs.size() - 1; jj >= 0; jj--) begin
//             if (cbuf_req_descs[jj].line_addr_match(addr) == 1'b1) begin
//                found_match = 1'b1;
//                cbuf_req_descs.delete(jj);
//                break;
//             end
//          end
//          if (found_match == 1'b0) begin
//             `dut_error(get_abstract_name(), $sformatf("cache line for 0x%x has a state of %p, but there is no coalescing buffer entry for it", addr, cache_st), pk_ms_sb_error_intensity, { VMOD_MS_SB_WARN, m_verbosity_mod })
//          end
//       end
//    end
//    for (int jj = cbuf_req_descs.size() - 1; jj >= 0; jj--) begin
//       `dut_error_mdesc(get_abstract_name(), cbuf_req_descs[jj], $sformatf("%0s is in the coalescing buffer, but its state is %p", cbuf_req_descs[jj].sprint_obj(), cbuf_req_descs[jj].get_cache_st()), pk_ms_sb_error_intensity, { VMOD_MS_SB_WARN, m_verbosity_mod })
//    end
// endfunction : check_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_verif_comp", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("MEMSHIRE(SB)");
      m_axi_port_names[`EVL_MEMSHIRE_REQ_ID_FOR_MESH]   = "MESH_MEM(AXI)";
      m_axi_port_names[`EVL_MEMSHIRE_REQ_ID_FOR_UST]    = "UST_MEM(AXI)";
      m_axi_port_names[`EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC] = "MESH_MEM(AXI)";
      m_axi_port_names[`EVL_DDR_LO_EVEN_PORT]    = "MEM_DDR_LO-0(AXI)";
      m_axi_port_names[`EVL_DDR_LO_ODD_PORT]     = "MEM_DDR_LO-1(AXI)";
      m_axi_port_names[`EVL_DDR_HI_EVEN_PORT]    = "MEM_DDR_HI-0(AXI)";
      m_axi_port_names[`EVL_DDR_HI_ODD_PORT]     = "MEM_DDR_HI-1(AXI)";
      m_axi_port_names[`EVL_ATOMIC_TO_MESH_PORT] = "MEM_ATOMIC(AXI)";

      m_memshire_scoreboard_cover = new();
   endfunction : new

endclass : evl_memshire_scoreboard
