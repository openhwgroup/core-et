//--------------------------------------------------------------------------------------------------
//
// Class: evl_line_queue
//
class evl_line_queue extends uvm_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   evl_line_desc m_descs[$];


   //-----------------------------------------------------------------------------------------------
   //
   // Function: push_desc
   //
   function int push_desc(input evl_line_desc desc_in);
      foreach (m_descs[ii]) begin
         if (m_descs[ii] == desc_in) begin
            m_descs.delete(ii);
            break;
         end
      end
      m_descs.push_back(desc_in);
      return m_descs.size();
   endfunction : push_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_desc
   //
   function evl_line_desc get_desc(input int index_in = 0);
      if (m_descs.size() > index_in) begin
         return m_descs[index_in];
      end
      return null;
   endfunction: get_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: pop_desc
   //
   function evl_line_desc pop_desc();
      if (m_descs.size() == 0) begin
         return null;
      end
      return m_descs.pop_front();
   endfunction: pop_desc


   //-----------------------------------------------------------------------------------------------
   //
   // Function: size
   //
   function int size();
      return m_descs.size();
   endfunction: size


   //-----------------------------------------------------------------------------------------------
   //
   // Function: delete_all_descs
   //
   virtual function void delete_all_descs();
      m_descs = {};
   endfunction : delete_all_descs

endclass : evl_line_queue
