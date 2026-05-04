//==================================================================================================
//
//
// Class: evl_param_xref
//
// This class is the repository for integer and object parameter keys.  Parameter keys are initially
// specified as strings, but these string values are converted to more-easily digestible integers
// that can be used as associative array keys into the m_int_params[] and m_obj_params[] arrays
// instantiated in EVL classes.
//
// By convention, parameter keys are named "pk_<some_string>".  Similarly, within a particular EVL
// class, the actual integer containing the key is a static int named pk_<some_string>.  As an
// example, if we want to create a string-to-integer mapping for 2 parameter keys, one named
// "pk_store_commit" and one named "pk_store_visible", the following code is used in the class
// definition:
//
//    static int pk_store_commit  = evl_param_xref::get_param_key("pk_store_commit");
//    static int pk_store_visible = evl_param_xref::get_param_key("pk_store_visible");
//
// To simplify coding, a macro is provided to do this:
//
//    `evl_param_key(pk_store_commit);
//    `evl_param_key(pk_store_visible);
//
// Parameter keys can then be used within class.  As an example, you declare a few functions to
// to use these parameter keys for monitoring and modifying state in a transaction descriptor:
//
//    if (trans_desc.get_int_param(pk_store_commit) > 0) begin
//       trans_desc.set_int_param(pk_store_visible, 1);
//    end
//
// In this example, the static variables pk_store_commit and pk_store_visible are used to index
// appropriately into the m_int_params[] array in the transaction descriptor.
//
class evl_param_xref;

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   static evl_param_xref m_param_xref = evl_param_xref::get_inst();
   static int            m_next_key   = 1;
   static int            m_param_keys[string];
   static string         m_param_names[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_inst
   //
   static function evl_param_xref get_inst();
      if (m_param_xref == null) begin
         evl_param_xref param_xref;

         param_xref = new();
      end
      return m_param_xref;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_param_key
   //
   static function int get_param_key(input string param_name);
      if (m_param_xref == null) begin
         void'(evl_param_xref::get_inst());
      end
      if (!m_param_keys.exists(param_name)) begin
         m_param_keys[param_name] = m_next_key++;
         m_param_names[m_param_keys[param_name]] = param_name;
      end
      return m_param_keys[param_name];
   endfunction : get_param_key


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: param_key_exists
   //
   static function bit param_key_exists(input int param_key);
      if (m_param_xref == null) begin
         void'(evl_param_xref::get_inst());
      end
      if (m_param_names.exists(param_key)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : param_key_exists


   //-----------------------------------------------------------------------------------------------
   //
   // Static Function: get_param_name
   //
   static function string get_param_name(input int param_key);
      if (m_param_xref == null) begin
         void'(evl_param_xref::get_inst());
      end
      if (!m_param_names.exists(param_key)) begin
         return "**unknown**";
      end
      return m_param_names[param_key];
   endfunction : get_param_name


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   local function new();
      if (m_param_xref == null) begin
         m_param_xref = this;
         void'(evl_base_pkg::evl_log_create());
      end
      `evl_log(`UVM_HIGH, VMOD_CONFIG, "ParamXRef", $sformatf("created parameter name cross-reference map"))
   endfunction : new

endclass : evl_param_xref
