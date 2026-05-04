//--------------------------------------------------------------------------------------------------
//
// Class: evl_rand_gen
//
class evl_rand_gen extends evl_void;

   //-----------------------------------------------------------------------------------------------
   //
   // Member Variables and Objects
   //
   static evl_rand_gen m_rand_gen    = evl_rand_gen::get_inst();
   static int unsigned m_global_seed = 0;


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_int
   //
   static function evl_rand_gen get_inst(input string name_in = "rand_root");
      if (m_rand_gen == null) begin
         m_rand_gen = new(name_in);
      end
      return m_rand_gen;
   endfunction : get_inst


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_int
   //
   function int get_int(input int unsigned max_value = ~0);
      return $urandom_range(max_value);
   endfunction : get_int


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_word
   //
   function evl_word_t get_word();
      return $urandom;
   endfunction : get_word


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_word_range
   //
   function evl_word_t get_word_range(input evl_word_t upper, input evl_word_t lower);
      return $urandom_range(upper, lower);
   endfunction : get_word_range


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_dword
   //
   function evl_dword_t get_dword();
      return { $urandom, $urandom };
   endfunction : get_dword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: get_qword
   //
   function evl_qword_t get_qword();
      return { $urandom, $urandom, $urandom, $urandom };
   endfunction : get_qword


   //-----------------------------------------------------------------------------------------------
   //
   // Function: gen_seed_from_string
   //
   static function int gen_seed_from_string(input string name_in);
      longint unsigned seed;
      int              idx;

      seed = longint'(m_global_seed);
      idx  = name_in.len() - 1;
      while (idx >= 0) begin
         longint unsigned str_val;

         str_val = longint'(0);
         for (int ii = 0; (ii < 8) && (idx >= 0); ii++) begin
            str_val = (str_val << 8) | (longint'(name_in.getc(idx)));
            idx--;
         end
         seed = ((seed << 1) | (64'h1 & ((seed >> 8) ^ (seed >> 33) ^ (seed >> 60) ^ (seed >> 63)))) ^ str_val;
      end
      seed += (seed >> 32);
      return ($urandom($urandom(int'(seed))));
   endfunction : gen_seed_from_string


   //-----------------------------------------------------------------------------------------------
   //
   // Constructor
   //
   function new(input string name_in = "evl_rand_gen");
      super.new(name_in);
      if (m_rand_gen == null) begin
         m_rand_gen    = this;
         m_global_seed = $urandom;
         if (m_global_seed == 0) begin
            m_global_seed = 1;
         end
         void'(evl_base_pkg::evl_log_create());
      end
      srandom(gen_seed_from_string(name_in));
      `evl_log(UVM_HIGH, VMOD_CONFIG, "RandGen", $sformatf("created a random generator (%0s, global_seed %0d, id %0d)", get_abstract_name(), m_global_seed, get_inst_id()))
   endfunction : new

endclass : evl_rand_gen
