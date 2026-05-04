//==================================================================================================
//
// This file is intended to be included in Esperanto Verification Library base class wrappers.  The
// assumption here is that there is a member in the wrapper class that is a handle to EVL object
// being wrapped.  This member must be named "m_object".
//
// Wrappers should not need access to their own integer and object parameter arrays (although we
// could allow this if necessary), but they intent is that wrappers simply act as proxies for typed
// objects.  As a result, if a base class object only has handles to untyped base class wrappers, we
// want to be able to get access to the typed base class's integer and object parameters using
// methods in the untyped base class wrappers.
//

   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param
   //
   // This function is used to access the typed object's set_int_param() function.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key:   This is the integer key (i.e., the associative array index) to use for the
   //              integer parameter.
   // param_value: This is the value to assign for the given integer parameter.
   //
   virtual function void set_int_param(input int param_key, input int param_value);
      if (m_object != null) begin
         m_object.set_int_param(param_key, param_value);
      end
   endfunction : set_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_int_param
   //
   // This function is used to access the typed object's have_int_param() function.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the integer
   //            parameter.
   //
   // Return Value:
   //
   // bit: A 1'b1 is returned if an integer parameter at the specified key exists.
   //
   virtual function bit have_int_param(input int param_key);
      if (m_object != null) begin
         return m_object.have_int_param(param_key);
      end
      return 1'b0;
   endfunction : have_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_int_param
   //
   // This function is used to access the typed object's get_int_param() function.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key:     This is the integer key (i.e., the associative array index) to use for the
   //                integer parameter.
   // param_default: This is the default value to return if no integer parameter exists at the
   //                specified key.
   //
   // Return Value:
   //
   // int: This is the value of the integer parameter at the specified key.  If no integer parameter
   //      exists at that key, then the param_default value will be returned.
   //
   virtual function int get_int_param(input int param_key, input int param_default = -1);
      if (m_object != null) begin
         return m_object.get_int_param(param_key, param_default);
      end
      return param_default;
   endfunction : get_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_int_param
   //
   // This function is used to access the typed object's wait_for_int_param() task.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key:       This is the integer key (i.e., the associative array index) to use for the
   //                  integer parameter ping.
   // min_param_value: This is the minimum integer parameter value to wait for; it this value is
   //                  greater than max_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // max_param_value: This is the maximum integer parameter value to wait for; it this value is
   //                  less than min_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // invert_context:  If this is 1'b1, return on condition == FALSE rather than TRUE.
   //
   virtual task wait_for_int_param(input int    param_key,
                                   input int    min_param_value = 32'h7fffffff,
                                   input int    max_param_value = 32'h7ffffffe,
                                   input bit    invert_context  = 1'b0,
                                   input int    timeout         = 0,
                                   input string timeout_err     = "");
      if (m_object != null) begin
         m_object.wait_for_int_param(param_key, min_param_value, max_param_value, invert_context, timeout, timeout_err);
      end
   endtask : wait_for_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_object_param
   //
   // This function is used to access the typed object's set_object_param() function.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key:       This is the integer key (i.e., the associative array index) to use for the
   //                  object parameter.
   // param_object_in: This is the value to assign for the given integer parameter.
   //
   virtual function void set_object_param(input int param_key, input uvm_void param_object_in);
      if (m_object != null) begin
         m_object.set_object_param(param_key, param_object_in);
      end
   endfunction : set_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_object_param
   //
   // This function is used to access the typed object's have_object_param() function.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the object
   //            parameter.
   //
   // Return Value:
   //
   // bit: A 1'b1 is returned if an object parameter at the specified key exists.
   //
   virtual function bit have_object_param(input int param_key);
      if (m_object != null) begin
         return m_object.have_object_param(param_key);
      end
      return 1'b0;
   endfunction : have_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object_param
   //
   // This function is used to access the typed object's get_object_param() function.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key:       This is the integer key (i.e., the associative array index) to use for the
   //                  object parameter.
   // param_object_in: This is the default handle to assign at the parameter key if none actually
   //                  exists at that key.
   //
   // Return Value:
   //
   // uvm_void: This is the value of the object parameter at the specified key.  If no object
   //           parameter exists at that key, then the param_object_in handle will be returned.
   //
   virtual function uvm_void get_object_param(input int param_key, input uvm_void param_object_in = null);
      if (m_object != null) begin
         return m_object.get_object_param(param_key, param_object_in);
      end
      return param_object_in;
   endfunction : get_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_object_param
   //
   // This function is used to access the typed object's wait_for_object_param() task.  Refer to
   // evl_base_inc.svh for more information.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the object
   //            parameter ping.
   //
   virtual task wait_for_object_param(input int param_key);
      if (m_object != null) begin
         m_object.wait_for_object_param(param_key);
      end
   endtask : wait_for_object_param
