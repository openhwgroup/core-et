//==================================================================================================
//
// This file is intended to be included in Esperanto Verification Library base classes derived
// directly from UVM classes.  For example, there is an evl_object class derived from uvm_object and
// an evl_component class derived from uvm_component.  Even though uvm_component is derived from
// uvm_object, evl_component cannot be derived from evl_object (it must be derived from uvm_object
// via uvm_component).  This file includes standard members and methods available in all derived
// EVL classes and is a way to get members/methods available in evl_object to also be available in
// evl_component, etc.
//

   //-----------------------------------------------------------------------------------------------
   //
   // Static Member Variables and Objects
   //
   // m_global_attrs: This is a handle to the global attributes object.  Virtually all EVL classes
   //                 have a handle to this singleton.
   //
   static evl_global_attrs m_global_attrs = evl_global_attrs::get_inst();


   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_int_params: This is an associative array of integer parameters, accessed by parameter keys.
   //               Refer to the description below for more information.
   //               --------------------------------------------------------------------------------
   // m_obj_params: This is an associative array of object parameters, accessed by parameter keys.
   //               Refer to the description below for more information.
   //               --------------------------------------------------------------------------------
   // m_int_pings:  This is an associative array of integer parameter ping objects, accessed by
   //               parameter keys.  Refer to the description below for more information.
   //               --------------------------------------------------------------------------------
   // m_obj_pings:  This is an associative array of object parameter ping objects, accessed by
   //               parameter keys.  Refer to the description below for more information.
   //
   //
   // Integer Parameters and Object Parameters
   //
   // These elements form a way to implement a poor man's version of Aspect-Oriented Programming.
   // For those familiar with Verisity's e language, any extensions of an object would eventually be
   // available in all versions of that object (the root class and the extended class); one simply
   // needed to cast the object appropriately.
   //
   // As an example, assume a particular object class is used in a number of different packages (the
   // Minion scoreboard package, the Maxion scoreboard package, the ET Link bus package, the ET Link
   // stimulus agent package, the shire cache scoreboard package, the AXI bus package, and the AXI
   // stimulus agent package).  Each of these packages may have some state information that must be
   // included the object class, but no other packages require this same status information.  In
   // addition, object instances of this class object are created in the base package, and a
   // particular object instance may be shared across all of the packages during its lifetime.
   //
   // With Verisity's e language, simply extending the base object class in each of the packages
   // will actually cause additional members and methods to be added to the base object class, but
   // it is necessary to cast the object class appropriately to access these additional members and
   // methods.  If a particular package is not needed for a simulation, the package need not be read
   // in and its extensions to the base class object will not be added.  This AOP model has the
   // advantage of compartmentalize code changes only to the packages in which they are needed.
   //
   // In System Verilog, changes to the base package would be required as package changes occur.
   // UVM attempts to alleviate this issue a bit through the use of the factory, but this only works
   // if the extended class includes all of the members and methods needed for all packages.
   //
   // The EV library adds integer and object parameters to allow code compartmentalization.  These
   // parameters are implemented as associative arrays that are accessed by parameter keys.  Each
   // instance of a particular object may have some state accessed by a unique key that is used by
   // one or more packages (or one or more classes within a package).  The only requirement is that
   // everyone must use the same key for accessing the same piece of state.
   //
   // Because we are human beings, the keys are most naturally some string (like "i_have_an_error"
   // or "this_object_is_stale").  Strings are not particularly simulation-speed friendly, however,
   // so a mechanism is provided to map strings into unique integers; these integers then become the
   // keys.  Because this mapping only needs to be performed once, the keys used within a class
   // definition can be implemented as static objects initialized during elaboration.  The naming
   // convention we use is to name the string "pk_<name>", which will result in a static integer of
   // the same name:
   //
   //    class my_object extends evl_object;
   //
   //       static int pk_i_have_an_error = evl_param_xref::get_param_key("pk_i_have_an_error");
   //
   //       ...
   //
   //    endclass : my_object
   //
   // There is a macro available to simplify coding:
   //
   //    class my_object extends evl_object;
   //
   //       `evl_param_key(pk_i_have_an_error)
   //
   //       ...
   //
   //    endclass : my_object
   //
   // A particular key mapping can be used in any number of classes.  There is a central authority
   // that performs mapping of a particular string to a unique integer.  Once the mapping is done,
   // any subsequent requests for a mapping of that string will get the same integer as a result.
   //
   // One downside with this mechanism is that we lose compile-time errors, so if a particular
   // string in one class does not *exactly* match the string in another class, they will have
   // different keys.  There is no run-time error checking that can be performed for this type of
   // error.  Luckily, we are all competent coders and will not make mistakes like this.
   //
   // To support the wait_for_int_param() and wait_for_obj_param() tasks (which are supposed to wait
   // for changes in integer and object parameters), m_int_pings[] and m_obj_pings[] entries are
   // created as necessary.
   //
   int            m_int_params[int];
   uvm_void       m_obj_params[int];
   evl_param_ping m_int_pings[int];
   evl_param_ping m_obj_pings[int];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_int_param
   //
   // This function is used to set an integer parameter using a particular key *only* if it does not
   // already exist.
   //
   // Input Variables:
   //
   // param_key:   This is the integer key (i.e., the associative array index) to use for the
   //              integer parameter.
   // param_value: This is the value to assign for the given integer parameter.
   //
   virtual function void init_int_param(input int param_key, input int param_value);
      if (!m_int_params.exists(param_key)) begin
         m_int_params[param_key] = param_value;
      end
   endfunction : init_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param
   //
   // This function is used to set an integer parameter using a particular key.  Whenever an integer
   // parameter is set, it is possible that some waiting task to be notified.  This is done by
   // finding a companion m_int_pings[] entry and, if it exists, pinging an event in it.
   //
   // Input Variables:
   //
   // param_key:   This is the integer key (i.e., the associative array index) to use for the
   //              integer parameter.
   // param_value: This is the value to assign for the given integer parameter.
   //
   virtual function void set_int_param(input int param_key, input int param_value);
      if (m_int_pings.exists(param_key)) begin
         if ((!m_int_params.exists(param_key)) || (m_int_params[param_key] != param_value)) begin
            m_int_params[param_key] = param_value;
            m_int_pings[param_key].ping();
         end
         else begin
            m_int_params[param_key] = param_value;
         end
      end
      else begin
         m_int_params[param_key] = param_value;
      end
   endfunction : set_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_int_param
   //
   // This function is used to increment an integer parameter using a particular key.  Whenever an
   // integer parameter is incremented, it is possible that some waiting task to be notified.  This
   // is done by finding a companion m_int_pings[] entry and, if it exists, pinging an event in it.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the
   //            integer parameter.
   //
   virtual function int incr_int_param(input int param_key);
      if (!m_int_params.exists(param_key)) begin
         set_int_param(param_key, 1);
      end
      else begin
         set_int_param(param_key, m_int_params[param_key] + 1);
      end
      return m_int_params[param_key];
   endfunction : incr_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_int_param
   //
   // This function is used to decrement an integer parameter using a particular key.  Whenever an
   // integer parameter is decremented, it is possible that some waiting task to be notified.  This
   // is done by finding a companion m_int_pings[] entry and, if it exists, pinging an event in it.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the
   //            integer parameter.
   // min_value: This is the minimum value the integer paramter should take.
   //
   virtual function int decr_int_param(input int param_key, input int min_value = 0);
      if (!m_int_params.exists(param_key) || (m_int_params[param_key] <= (min_value + 1))) begin
         set_int_param(param_key, min_value);
      end
      else begin
         set_int_param(param_key, m_int_params[param_key] - 1);
      end
      return m_int_params[param_key];
   endfunction : decr_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_int_param
   //
   // This function is used to determine if a particular integer parameter exists in this object.
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
      if (m_int_params.exists(param_key)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : have_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_int_param
   //
   // This function is used to get the integer parameter at the specified key.
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
      if (!m_int_params.exists(param_key)) begin
         return param_default;
      end
      return m_int_params[param_key];
   endfunction : get_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_int_param
   //
   // This task is used to wait for an integer parameter at a particular key to get modified.  With
   // different input variables, it is possible to wait for a particular value, wait for a
   // particular range, or wait for any change.
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
      if (!m_int_pings.exists(param_key)) begin
         m_int_pings[param_key] = new();
      end
      #0;
      if (m_int_params.exists(param_key)) begin
         if ((m_int_params[param_key] >= min_param_value) && (m_int_params[param_key] <= max_param_value)) begin
            if (invert_context == 1'b0) begin
               return;
            end
         end
         else if (invert_context == 1'b1) begin
            return;
         end
      end
      if (timeout <= 0) begin
         forever begin
            m_int_pings[param_key].wait_for_ping();
            if (m_int_params.exists(param_key)) begin
               if (max_param_value < min_param_value) begin
                  return;
               end
               if ((m_int_params[param_key] >= min_param_value) && (m_int_params[param_key] <= max_param_value)) begin
                  if (invert_context == 1'b0) begin
                     return;
                  end
               end
               else if (invert_context == 1'b1) begin
                  return;
               end
            end
         end
      end
      else begin
         fork
            begin
               #(timeout);
               if (timeout_err != "") begin
                  `dut_error(get_abstract_name(), $sformatf("wait_for_int_param() timeout: %s", timeout_err))
               end
            end
            forever begin
               m_int_pings[param_key].wait_for_ping();
               if (m_int_params.exists(param_key)) begin
                  if (max_param_value < min_param_value) begin
                     break;
                  end
                  if ((m_int_params[param_key] >= min_param_value) && (m_int_params[param_key] <= max_param_value)) begin
                     if (invert_context == 1'b0) begin
                        break;
                     end
                  end
                  else if (invert_context == 1'b1) begin
                     break;
                  end
               end
            end
         join_any
         disable fork;
      end
   endtask : wait_for_int_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: init_int_param_by_name
   //
   // This function is used to set an integer parameter using a particular key *only* if it has not
   // been set yet (in this case the key is specified as the string name for it).
   //
   // Input Variables:
   //
   // param_name:  This is the string name for the key.
   // param_value: This is the value to assign for the given integer parameter.
   //
   function void init_int_param_by_name(input string param_name, input int param_value);
      init_int_param(evl_param_xref::get_param_key(param_name), param_value);
   endfunction : init_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_int_param_by_name
   //
   // This function is used to set an integer parameter using a particular key, but the key in this
   // case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name:  This is the string name for the key.
   // param_value: This is the value to assign for the given integer parameter.
   //
   function void set_int_param_by_name(input string param_name, input int param_value);
      set_int_param(evl_param_xref::get_param_key(param_name), param_value);
   endfunction : set_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: incr_int_param_by_name
   //
   // This function is used to increment an integer parameter using a particular key, but the key in
   // this case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   //
   virtual function int incr_int_param_by_name(input string param_name);
      return incr_int_param(evl_param_xref::get_param_key(param_name));
   endfunction : incr_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: decr_int_param_by_name
   //
   // This function is used to decrement an integer parameter using a particular key, but the key in
   // this case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   // min_value:  This is the minimum value the integer paramter should take.
   //
   virtual function int decr_int_param_by_name(input string param_name, input int min_value = 0);
      return decr_int_param(evl_param_xref::get_param_key(param_name), min_value);
   endfunction : decr_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_int_param_by_name
   //
   // This function is used to determine if a particular integer parameter exists in this object,
   // but the key in this case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   //
   // Return Value:
   //
   // bit: A 1'b1 is returned if an integer parameter at the specified key exists.
   //
   function bit have_int_param_by_name(input string param_name);
      return have_int_param(evl_param_xref::get_param_key(param_name));
   endfunction : have_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_int_param_by_name
   //
   // This function is used to get the integer parameter at the specified key, but the key in this
   // case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name:    This is the string name for the key.
   // param_default: This is the default value to return if no integer parameter exists at the
   //                specified key.
   //
   // Return Value:
   //
   // int: This is the value of the integer parameter at the specified key.  If no integer parameter
   //      exists at that key, then the param_default value will be returned.
   //
   function int get_int_param_by_name(input string param_name, input int param_default = -1);
      return get_int_param(evl_param_xref::get_param_key(param_name), param_default);
   endfunction : get_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_int_param_by_name
   //
   // This task is used to wait for an integer parameter at a particular key to get modified.  but
   // the key in this case the key is specified as the string name for it.  With different input
   // variables, it is possible to wait for a particular value, wait for a particular range, or wait
   // for any change.
   //
   // Input Variables:
   //
   // param_name:      This is the string name for the key.
   // min_param_value: This is the minimum integer parameter value to wait for; it this value is
   //                  greater than max_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // max_param_value: This is the maximum integer parameter value to wait for; it this value is
   //                  less than min_param_value, then waiting will stop when any change to the
   //                  integer parameter occurs.
   // invert_context:  If this is 1'b1, return on condition == FALSE rather than TRUE.
   //
   task wait_for_int_param_by_name(input string param_name,
                                   input int    min_param_value = 32'h7fffffff,
                                   input int    max_param_value = 32'h7ffffffe,
                                   input bit    invert_context  = 1'b0,
                                   input int    timeout         = 0,
                                   input string timeout_err     = "");
      wait_for_int_param(evl_param_xref::get_param_key(param_name), min_param_value, max_param_value, invert_context, timeout, timeout_err);
   endtask : wait_for_int_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_object_param
   //
   // This function is used to set an object parameter using a particular key.  If the incoming
   // object parameter is null, any existing entry at that key will be deleted.  Whenever an object
   // parameter is set, it is possible that some waiting task to be notified.  This is done by
   // finding a companion m_obj_pings[] entry and, if it exists, pinging an event in it.
   //
   // Input Variables:
   //
   // param_key:       This is the integer key (i.e., the associative array index) to use for the
   //                  object parameter.
   // param_object_in: This is the value to assign for the given integer parameter.
   //
   virtual function void set_object_param(input int param_key, input uvm_void param_object_in);
      if (m_obj_params.exists(param_key)) begin
         if (param_object_in == null) begin
            m_obj_params.delete(param_key);
            return;
         end
      end
      m_obj_params[param_key] = param_object_in;
      if (m_obj_pings.exists(param_key)) begin
         m_obj_pings[param_key].ping();
      end
   endfunction : set_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_object_param
   //
   // This function is used to determine if a particular object parameter exists in this object.
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
      if (m_obj_params.exists(param_key)) begin
         return 1'b1;
      end
      return 1'b0;
   endfunction : have_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object_param
   //
   // This function is used to get the object parameter handle at the specified key.  If there is no
   // object parameter at the specified key, the incoming object handle provided will be assigned to
   // that key and returned.
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
      if (!m_obj_params.exists(param_key)) begin
         if (param_object_in == null) begin
            return null;
         end
         m_obj_params[param_key] = param_object_in;
      end
      return m_obj_params[param_key];
   endfunction : get_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_object_param
   //
   // This task is used to wait for an object parameter at a particular key to get modified.
   //
   // Input Variables:
   //
   // param_key: This is the integer key (i.e., the associative array index) to use for the object
   //            parameter ping.
   //
   virtual task wait_for_object_param(input int param_key);
      if (!m_obj_pings.exists(param_key)) begin
         m_obj_pings[param_key] = new();
      end
      m_obj_pings[param_key].wait_for_ping();
   endtask : wait_for_object_param


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_object_param_by_name
   //
   // This function is used to set an object parameter using a particular key, but the key in this
   // case the key is specified as the string name for it.  If the incoming object parameter is
   // null, any existing entry at that key will be deleted.
   //
   // Input Variables:
   //
   // param_name:      This is the string name for the key.
   // param_object_in: This is the value to assign for the given integer parameter.
   //
   function void set_object_param_by_name(input string param_name, input uvm_void param_object_in);
      set_object_param(evl_param_xref::get_param_key(param_name), param_object_in);
   endfunction : set_object_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: have_object_param_by_name
   //
   // This function is used to determine if a particular object parameter exists in this object, but
   // the key in this case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   //
   // Return Value:
   //
   // bit: A 1'b1 is returned if an object parameter at the specified key exists.
   //
   function bit have_object_param_by_name(input string param_name);
      return have_object_param(evl_param_xref::get_param_key(param_name));
   endfunction : have_object_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_object_param_by_name
   //
   // This function is used to get the object parameter handle at the specified key, but the key in
   // this case the key is specified as the string name for it.  If there is no object parameter at
   // the specified key, the incoming object handle provided will be assigned to that key and
   // returned.
   //
   // Input Variables:
   //
   // param_name:      This is the string name for the key.
   // param_object_in: This is the default handle to assign at the parameter key if none actually
   //                  exists at that key.
   //
   // Return Value:
   //
   // uvm_void: This is the value of the object parameter at the specified key.  If no object
   //           parameter exists at that key, then the param_object_in handle will be returned.
   //
   function uvm_void get_object_param_by_name(input string param_name, input uvm_void param_object_in = null);
      return get_object_param(evl_param_xref::get_param_key(param_name), param_object_in);
   endfunction : get_object_param_by_name


   //-----------------------------------------------------------------------------------------------
   //
   // Task: wait_for_object_param_by_name
   //
   // This function is used to set an object parameter using a particular key, but the key in this
   // case the key is specified as the string name for it.
   //
   // Input Variables:
   //
   // param_name: This is the string name for the key.
   //
   task wait_for_object_param_by_name(input string param_name);
      wait_for_object_param(evl_param_xref::get_param_key(param_name));
   endtask : wait_for_object_param_by_name
