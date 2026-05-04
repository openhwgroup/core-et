//--------------------------------------------------------------------------------------------------
//
// Class: evl_minion_shire_config_test
//
class evl_minion_shire_config_test extends evl_stimulus;

   `uvm_object_utils(evl_minion_shire_config_test)


   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_minion_shire_config_test m_minion_shire_config_test = new();

   //-----------------------------------------------------------------------------------------------
   //
   // Class: evl_ms_config_test_seq
   //
   class evl_ms_config_test_seq extends evl_sequence_mdir;

      //--------------------------------------------------------------------------------------------
      //
      // Task: write_configuration
      //
      task write_configuration();
         evl_trans_desc    trans_desc;
         evl_trans_desc    trans_descs[$];
         evl_dword_t       dword;
         evl_paddr_t       reg_addrs[$] = {'h1fff40298,    'h1fff40250, 'h1fff402c8,    'h1fff40250,
                                           'h1fff402c8,    'h1fff40258, 'h1fff40200,    'h1fff40268,
                                           'h1fff40270,    'h1fff40278, 'h1fff40280,    'h1fff402d0,
                                           'h1fff40250,    'h1fff40250, 'h1fff40298,    'h1fff40250,
                                           'h1fff402c8,    'h1fff402c8, 'h1fff402c8,    'h1fff40240,
                                           'h1fff40010,    'h1fff40008};

         evl_paddr_t       reg_data[$]  = {'h3,             'h0,          'h0,             'h2c01,
                                           'h181,           'ha000101e8,  'h3027e002e0003, 'h100000a02be0033,
                                           'h10001000000d5, 'h10001,      'h100000000,     'ha002000080001,
                                           'h2c02,          'h2c0e,       'h4,             'h2c00,
                                           'h1182,          'h118e,       'h3180,          'h0,
                                           'hffffffff,      'h392bf00};

         foreach (reg_addrs[ii]) begin
            dword = reg_data[ii];
            `evl_log(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), $sformatf("Writing 0x%x to 0x%x", dword, reg_addrs[ii]))
            issue(trans_desc, ST, reg_addrs[ii], 8, evl_bus_req_source_t'(0), { dword, dword, dword, dword, dword, dword, dword, dword }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
            trans_descs.push_back(trans_desc);
         end
         
         //
         // Wait for all of the writes to complete.
         //
         foreach (trans_descs[ii]) begin
            trans_descs[ii].wait_for_done();
         end

         //
         // Now read the configuration registers.
         //
         trans_descs = {};
         foreach (reg_addrs[ii]) begin
            issue(trans_desc, LD, reg_addrs[ii], 8, evl_bus_req_source_t'(0), { }, null, evl_line_qword_en_t'(0), evl_trans_attr_s'(0), 1'b0);
            trans_descs.push_back(trans_desc);
         end
         foreach (trans_descs[ii]) begin
            trans_descs[ii].wait_for_done();
         end
         foreach (reg_addrs[ii]) begin
            dword = trans_descs[ii].get_read_dword(0);
            `evl_log_mdesc(UVM_MEDIUM, VMOD_TEST, m_agent.get_abstract_name(), trans_descs[ii], $sformatf("Read 0x%x for %0x", dword, reg_addrs[ii]))
         end
         blocking_issue(trans_desc, BARRIER, 0);

      endtask : write_configuration


      //--------------------------------------------------------------------------------------------
      //
      // Task: configure_phase
      //
      virtual task configure_phase(input uvm_phase phase);
         int value;

         phase.raise_objection(phase);
         fork
            begin
               super.configure_phase(phase);
            end
            begin
               write_configuration();
            end
         join
         phase.drop_objection(phase);
      endtask : configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Task: post_configure_phase
      //
      virtual task post_configure_phase(input uvm_phase phase);
         disable_stimulus(`EVL_ON_ICE_DISABLE);
         // m_global_config_object.print_config();
         // m_config_object.print_config();
         super.configure_phase(phase);
      endtask : post_configure_phase


      //--------------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string name_in = "evl_ms_config_test_seq", input evl_verif_comp agent_in = null);
         super.new(name_in);
         // m_config_object   = new(name_in);
         m_allow_unaligned = 1'b1;
      endfunction

   endclass : evl_ms_config_test_seq


   //-----------------------------------------------------------------------------------------------
   //
   // Function: prepare_test_for_use
   //
   virtual function void prepare_test_for_use();
      evl_verif_comp verif_comp_list[];
      evl_verif_comp neigh_comp_list[];

      if (`et_test_plusargs("minion_shire_config_test") == 1) begin
         if (evl_verif_comp_map::get_verif_comp_list(verif_comp_list, FROMSYS_MASTER_STIM_AGENT) != 0) begin
            foreach (verif_comp_list[ii]) begin
               evl_shire_id_t shire_id = verif_comp_list[ii].get_shire_id();
               evl_ms_config_test_seq ms_config_test_seq;
               ms_config_test_seq = new($sformatf("MinShireConfig-%0d", shire_id), verif_comp_list[ii]);
               ms_config_test_seq.set_abstract_name("Minion Shire Configuration");
               verif_comp_list[ii].add_sequence_wrapper(ms_config_test_seq.get_wrapper());
            end
         end
      end
   endfunction : prepare_test_for_use


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_minion_shire_config_test");
      super.new(name_in);
   endfunction

endclass : evl_minion_shire_config_test
