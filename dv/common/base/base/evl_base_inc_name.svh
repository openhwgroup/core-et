//==================================================================================================
//
// This file is intended to be included in Esperanto Verification Library base classes derived
// directly from UVM classes.  This include file simply adds abstract name support.
//

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   // m_abstract_name:  The EV library makes extensive use of abstract names to provide concise and
   //                   informative logging.  These names should be relatively short (24 characters
   //                   or less).
   //
   string m_abstract_name = "";


   //-----------------------------------------------------------------------------------------------
   //
   // Function: set_abstract_name
   //
   // This function is used to set the abstract name for this object.
   //
   // Input Variables:
   //
   // name_in: This is the string to which to set the abstract name.
   //
   virtual function void set_abstract_name(input string name_in);
      m_abstract_name = name_in;
   endfunction : set_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_abstract_name
   //
   // This function is used to get the abstract name of this object.
   //
   // Return Value:
   //
   // string: This is the abstract name of the object.
   //
   virtual function string get_abstract_name();
      return m_abstract_name;
   endfunction : get_abstract_name


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_root_abstract_name
   //
   // This function is used to get the root abstract name of this object.  The root abstract name is
   // defined as any portion of the abstract name string before the first "(", although the actual
   // implementation will define the termination character as anything other than "a-z", "A-Z",
   // "0-9", "_", "-", or ":".
   //
   // Return Value:
   //
   // string: This is the root abstract name of the object.
   //
   virtual function string get_root_abstract_name(input string name_in = "");
      int    limit;
      string tmp_name;

      if (name_in == "") begin
         tmp_name = m_abstract_name;
      end
      else begin
         tmp_name = name_in;
      end
      limit = tmp_name.len() - 1;
      for (int ii = 0; ii <= limit; ii++) begin
         byte cc = tmp_name.getc(ii);

         if (!( ((cc >= byte'("A")) && (cc <= byte'("Z"))) ||
                ((cc >= byte'("a")) && (cc <= byte'("z"))) ||
                ((cc >= byte'("0")) && (cc <= byte'("9"))) ||
                (cc == byte'("_")) ||
                (cc == byte'("-")) ||
                (cc == byte'(":")) )) begin
            tmp_name = tmp_name.substr(0, ii - 1);
            return tmp_name;
         end
      end
      return tmp_name;
   endfunction : get_root_abstract_name
