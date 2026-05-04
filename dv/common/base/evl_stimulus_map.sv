//--------------------------------------------------------------------------------------------------
//
// Class: evl_stimulus_map
//
class evl_stimulus_map extends evl_test;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_stimulus_map m_stimulus_map = evl_stimulus_map::get_inst();
   static evl_stimulus     m_stimulus_tests[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_stimulus_map::get_inst
   //
   static function evl_stimulus_map get_inst();
      if (m_stimulus_map == null) begin
         evl_stimulus_map stimulus_map;

         stimulus_map = new();
      end
      return m_stimulus_map;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: evl_stimulus_map::add_stimulus_to_map
   //
   static function void add_stimulus_to_map(input evl_stimulus stimulus_in);
      if (m_stimulus_map == null) begin
         void'(evl_stimulus_map::get_inst());
      end
      m_stimulus_map.add_stimulus_to_map_local(stimulus_in);
   endfunction : add_stimulus_to_map


   //-----------------------------------------------------------------------------------------------
   //
   // Function: add_stimulus_to_map_local
   //
   function void add_stimulus_to_map_local(input evl_stimulus stimulus_in);
      if (!(stimulus_in inside { m_stimulus_tests })) begin
         m_stimulus_tests.push_back(stimulus_in);
      end
   endfunction : add_stimulus_to_map_local


   //-----------------------------------------------------------------------------------------------
   //
   // Function: start_of_simulation_phase
   //
   virtual function void start_of_simulation_phase(uvm_phase phase);
      super.start_of_simulation_phase(phase);
      foreach (m_stimulus_tests[ii]) begin
         m_stimulus_tests[ii].prepare_test_for_use();
      end
   endfunction : start_of_simulation_phase


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   local function new(input string name_in = "evl_stimulus_map", input uvm_component parent_in = null);
      super.new(name_in, parent_in);
      set_abstract_name("StimulusMap");
      if (m_stimulus_map == null) begin
         m_stimulus_map = this;
         void'(evl_base_pkg::evl_log_create());
      end
      `evl_log(UVM_HIGH, VMOD_CONFIG, get_abstract_name(), $sformatf("created stimulus map (id %0d)", get_inst_id()))
   endfunction : new

endclass : evl_stimulus_map
