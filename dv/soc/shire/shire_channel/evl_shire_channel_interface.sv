`include "rtl/inc/soc.vh"

`ifndef __EVL_SHIRE_CHANNEL_PKG
   `include "dv/soc/shire/shire_channel/evl_shire_channel_pkg.sv"
`endif

`include "dv/common/axi/axi_vc/evl_axi_pkg.sv"
`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

module evl_shire_channel_interface #(parameter int SHIRE_INST_ID             = 0,
                                     parameter int NUM_NEIGH                 = 1,
                                     parameter int NUM_RBOX                  = 1,
                                     parameter int NUM_BANKS                 = 1,
                                     parameter int NUM_UC                    = 1,
                                     parameter int NUM_ICACHE                = 0,
                                     parameter int APB_ESR_IDX_COUNT         = 1,
                                     parameter bit DV_NO_MONITOR_SHIRE_CACHE = 1'b0,
                                     parameter bit DV_NO_OBJECTION           = 1'b1) (
      input                                                  reset_c,
      input                                                  clock,
      input                                                  evl_shire_cache_en,
      input wire APB_to_all_ESRs_t   [APB_ESR_IDX_COUNT-1:0] APB_ESR_req_mon,
      input wire APB_from_all_ESRs_t [APB_ESR_IDX_COUNT-1:0] APB_ESR_rsp_mon,
      input                                                  qos_dram
   );

   `ifndef ET_USE_ASIC_MEMS
      `ifndef ET_USE_BIST_RTL
         `ifndef _SHIRE_CHANNEL_ECC_SUPPORT
            `define _SHIRE_CHANNEL_ECC_SUPPORT 1
         `endif // ifndef _SHIRE_CHANNEL_ECC_SUPPORT
      `endif // ifndef ET_USE_BIST_RTL
   `endif // ifndef ET_USE_ASIC_MEMS


   //--------------------------------------------------------------------------------------------------
   //
   // This file is intended to be included in the Shire top-level module.  It includes DV-specific
   // objects and methods for the Esperanto Verification Library.
   //
   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_shire_channel_pkg::*;
   import evl_ucblock_pkg::*;

   localparam int lp_row_1 = (`ICACHE_NR_ROWS > 1) ? 1 : 0;
   localparam int lp_row_2 = (`ICACHE_NR_ROWS > 2) ? 2 : 0;
   localparam int lp_row_3 = (`ICACHE_NR_ROWS > 3) ? 3 : 0;

   localparam int lp_mem_1 = (`ICACHE_NR_MEMS_PER_ROW > 1) ? 1 : 0;
   localparam int lp_mem_2 = (`ICACHE_NR_MEMS_PER_ROW > 2) ? 2 : 0;
   localparam int lp_mem_3 = (`ICACHE_NR_MEMS_PER_ROW > 3) ? 3 : 0;

   //
   // Create a Shire Channel RTL port instance.
   //
   int m_sc_sb_en       = 0;
   int m_uc_sb_en       = 0;
   bit m_start_monitor  = 1'b0;
   bit m_no_objection   = 1'b0;
   bit m_shire_cache_en = 1'b0;
   bit m_reset_on       = 1'b0;
   bit m_reset_off      = 1'b0;
   bit m_register_me    = evl_rtl_instance_map::add_instance_info($sformatf("%m"),
                                                                  SHIRE_CHANNEL_RTL_AGENT,
                                                                  ~evl_agent_id_t'(0),
                                                                  "evl_shire_channel_env",
                                                                  $sformatf("pk_num_banks=%0d,pk_num_rbox=%0d,pk_num_icache=%0d", NUM_BANKS, NUM_RBOX, NUM_ICACHE),
                                                                  "", "", { 1 });

   evl_rtl_port               m_rtl_port;
   evl_shire_channel_rtl_port m_channel_rtl_port;

   genvar esr_idx;
   generate
      for (esr_idx = 0; esr_idx < APB_ESR_IDX_COUNT; esr_idx++) begin: g_apb
         evl_apb_interface #(.AGENT_ID   (esr_idx),
                             .ADDR_WIDTH (`APB_AD_SHIRE_TOP_WIDTH),
                             .DATA_WIDTH (`ESR_APB_D_WIDTH)) m_apb_esr (
               .isolate      ( 1'b0                                 ),
               .pclk         ( clock                                ),
               .presetn      ( ~reset_c                             ),
               .penable      ( APB_ESR_req_mon[esr_idx].apb_penable ),
               .psel         ( APB_ESR_req_mon[esr_idx].apb_psel    ),
               .pwrite       ( APB_ESR_req_mon[esr_idx].apb_pwrite  ),
               .paddr        ( APB_ESR_req_mon[esr_idx].apb_paddr   ),
               .pwdata       ( APB_ESR_req_mon[esr_idx].apb_pwdata  ),
               .pready       ( APB_ESR_rsp_mon[esr_idx].apb_pready  ),
               .prdata       ( APB_ESR_rsp_mon[esr_idx].apb_prdata  ),
               .pslverr      ( APB_ESR_rsp_mon[esr_idx].apb_pslverr )
            );
      end
   endgenerate


   initial begin
      if (`et_value_plusargs("sc_sb_en=%d", m_sc_sb_en) == 0) begin
         m_sc_sb_en = 1;
      end
      if (`et_value_plusargs("uc_sb_en=%d", m_uc_sb_en) == 0) begin
         m_uc_sb_en = 1;
      end
      #1ns m_start_monitor <= 1'b1;
      m_no_objection = DV_NO_OBJECTION;
   end


   generate
      if ((NUM_ICACHE > 0) && (NUM_NEIGH > 0)) begin :g_ic

         `ifdef _SHIRE_CHANNEL_ECC_SUPPORT

            //--------------------------------------------------------------------------------------
            //
            // Function: mod_get_cache_data
            //
            function automatic int mod_get_cache_data(output evl_dword_t cdata[],
                                                      input  int         neigh_in,
                                                      input  evl_paddr_t caddr,
                                                      input  int         cway);
               int         set;
               int         mem;
               int         row;
               bit [143:0] qword0;
               bit [143:0] qword1;
               bit [143:0] qword2;
               bit [143:0] qword3;
               evl_paddr_t paddr;

               qword0 = {144{1'b1}};
               qword1 = {144{1'b1}};
               qword2 = {144{1'b1}};
               qword3 = {144{1'b1}};
               cdata  = new[`EVL_LINE_DWORDS];
               paddr  = (((caddr >> `EVL_LINE_ADDR_LSB) << ($clog2(`ICACHE_WAYS))) & ((`ICACHE_SETS * `ICACHE_WAYS) - 1)) + ((neigh_in % NUM_NEIGH) * (`ICACHE_SETS * `ICACHE_WAYS));
               set    = (int'(paddr) & (`ICACHE_MEM_WORDS - 1) & (~(`ICACHE_WAYS - 1))) + (cway & (`ICACHE_WAYS - 1));
               mem    = (int'(paddr) >> $clog2(`ICACHE_MEM_WORDS)) & (`ICACHE_NR_MEMS_PER_ROW - 1);
               row    = (int'(paddr) >> $clog2(`ICACHE_MEM_WORDS * `ICACHE_NR_MEMS_PER_ROW)) & (`ICACHE_NR_ROWS - 1);
               if (row == 0) begin
                  qword0 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[0].mem.mem[set];
                  qword1 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[lp_mem_1].mem.mem[set];
                  qword2 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[lp_mem_2].mem.mem[set];
                  qword3 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[lp_mem_3].mem.mem[set];
               end
               else if ((`ICACHE_NR_ROWS > 1) && (row == 1)) begin
                  qword0 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[0].mem.mem[set];
                  qword1 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[lp_mem_1].mem.mem[set + 1];
                  qword2 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[lp_mem_2].mem.mem[set + 2];
                  qword3 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[lp_mem_3].mem.mem[set + 3];
               end
               else if ((`ICACHE_NR_ROWS > 2) && (row == 2)) begin
                  qword0 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[0].mem.mem[set];
                  qword1 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[lp_mem_1].mem.mem[set + 1];
                  qword2 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[lp_mem_2].mem.mem[set + 2];
                  qword3 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[lp_mem_3].mem.mem[set + 3];
               end
               else if ((`ICACHE_NR_ROWS > 3) && (row == 3)) begin
                  qword0 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[0].mem.mem[set];
                  qword1 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[lp_mem_1].mem.mem[set + 1];
                  qword2 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[lp_mem_2].mem.mem[set + 2];
                  qword3 = ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[lp_mem_3].mem.mem[set + 3];
               end
               cdata[0] = qword0[63:0];
               cdata[1] = qword0[127:64];
               cdata[2] = qword1[63:0];
               cdata[3] = qword1[127:64];
               cdata[4] = qword2[63:0];
               cdata[5] = qword2[127:64];
               cdata[6] = qword3[63:0];
               cdata[7] = qword3[127:64];
               return 1;
            endfunction : mod_get_cache_data


            //-----------------------------------------------------------------------------------
            //
            // Function: mod_set_cache_bit_err
            //
            function automatic int mod_set_cache_bit_err(input int neigh_in,
                                                         input int cindex,
                                                         input int cbit0,
                                                         input int cbit1 = -1,
                                                         input int cway  = 0,
                                                         input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
               int         set;
               int         mem;
               int         row;
               int         err_bit0;
               int         err_bit1;
               int         err_type;
               evl_paddr_t paddr;

               paddr  = (((cindex >> `EVL_LINE_ADDR_LSB) << ($clog2(`ICACHE_WAYS))) & ((`ICACHE_SETS * `ICACHE_WAYS) - 1)) + ((neigh_in % NUM_NEIGH) * (`ICACHE_SETS * `ICACHE_WAYS));
               set    = (int'(paddr) & (`ICACHE_MEM_WORDS - 1) & (~(`ICACHE_WAYS - 1))) + (cway & (`ICACHE_WAYS - 1));
               mem    = (int'(paddr) >> $clog2(`ICACHE_MEM_WORDS)) & (`ICACHE_NR_MEMS_PER_ROW - 1);
               row    = (int'(paddr) >> $clog2(`ICACHE_MEM_WORDS * `ICACHE_NR_MEMS_PER_ROW)) & (`ICACHE_NR_ROWS - 1);
               err_bit0 = cbit0;
               err_bit1 = cbit1;
               err_type = cerr;
               if ((cbit0 > 71) || (cbit1 > 71)) begin
                  return -1;
               end
               if ((cindex & 8) != 0) begin
                  if (err_bit0 >= 0) begin
                     err_bit0 += 72;
                  end
                  if (err_bit1 >= 0) begin
                     err_bit1 += 72;
                  end
               end
               if (cbit0 < 0) begin
                  err_type = int'(EVL_ECC_ERR_NONE);
               end

               if (row == 0) begin
                  if (mem == 0) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[0].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 1) && (mem == 1)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[lp_mem_1].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 2) && (mem == 2)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[lp_mem_2].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 3) && (mem == 3)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[0].MEM_ROW[lp_mem_3].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
               end
               else if ((`ICACHE_NR_ROWS > 1) && (row == 1)) begin
                  if (mem == 0) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[0].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 1) && (mem == 1)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[lp_mem_1].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 2) && (mem == 2)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[lp_mem_2].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 3) && (mem == 3)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_1].MEM_ROW[lp_mem_3].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
               end
               else if ((`ICACHE_NR_ROWS > 2) && (row == 2)) begin
                  if (mem == 0) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[0].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 1) && (mem == 1)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[lp_mem_1].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 2) && (mem == 2)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[lp_mem_2].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 3) && (mem == 3)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_2].MEM_ROW[lp_mem_3].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
               end
               else if ((`ICACHE_NR_ROWS > 3) && (row == 3)) begin
                  if (mem == 0) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[0].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 1) && (mem == 1)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[lp_mem_1].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 2) && (mem == 2)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[lp_mem_2].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
                  else if ((`ICACHE_NR_MEMS_PER_ROW > 3) && (mem == 3)) begin
                     return ICACHE_MEMS.icache_mems_i.ICACHE_SRAM[0].data_ram_wrap.ROWS[lp_row_3].MEM_ROW[lp_mem_3].mem.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                  end
               end
               return -1;
            endfunction : mod_set_cache_bit_err


            class my_shire_channel_rtl_port extends evl_shire_channel_rtl_port;

               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_set
               //
               virtual function int get_cache_set(input evl_paddr_t caddr,
                                                  input int         cway  = 0,
                                                  input int         ctype = 0);
                  evl_paddr_t paddr;
                  int         set;

                  if (ctype != int'(SC_RAM_TYPE_ICACHE_DATA)) begin
                     return caddr;
                  end
                  paddr = ((caddr >> `EVL_LINE_ADDR_LSB) << ($clog2(`ICACHE_WAYS))) & ((`ICACHE_MEM_WORDS * `ICACHE_NR_MEMS_PER_ROW * `ICACHE_NR_ROWS) - 1);
                  set   = (int'(paddr) & (`ICACHE_MEM_WORDS - 1) & (~(`ICACHE_WAYS - 1))) + (cway & (`ICACHE_WAYS - 1));
                  return set;
               endfunction : get_cache_set


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_bank
               //
               virtual function int get_cache_bank(input evl_paddr_t caddr,
                                                   input int         cway  = 0,
                                                   input int         ctype = 0);
                  return 0;
               endfunction : get_cache_bank


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_subbank
               //
               virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                                      input int         cway  = 0,
                                                      input int         ctype = 0);
                  return 0;
               endfunction : get_cache_subbank


               //-----------------------------------------------------------------------------------
               //
               // Function: get_cache_data
               //
               virtual function int get_cache_data(output evl_dword_t cdata[],
                                                   input  int         cindex,
                                                   input  int         cway  = 0,
                                                   input  int         ctype = 0);
                  if ((ctype == int'(SC_RAM_TYPE_ICACHE0_DATA)) || (ctype == int'(SC_RAM_TYPE_ICACHE_DATA))) begin
                     return mod_get_cache_data(cdata, 0, cindex, cway);
                  end
                  else if (ctype == int'(SC_RAM_TYPE_ICACHE1_DATA)) begin
                     return mod_get_cache_data(cdata, 1, cindex, cway);
                  end
                  else if (ctype == int'(SC_RAM_TYPE_ICACHE2_DATA)) begin
                     return mod_get_cache_data(cdata, 2, cindex, cway);
                  end
                  else if (ctype == int'(SC_RAM_TYPE_ICACHE3_DATA)) begin
                     return mod_get_cache_data(cdata, 3, cindex, cway);
                  end
                  return -1;
               endfunction : get_cache_data


               //-----------------------------------------------------------------------------------
               //
               // Function: set_cache_bit_err
               //
               virtual function int set_cache_bit_err(input int cindex,
                                                      input int cbit0,
                                                      input int cbit1 = -1,
                                                      input int cway  = 0,
                                                      input int ctype = 0,
                                                      input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
                  if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE_DATA, SC_RAM_TYPE_ICACHE0_DATA }) begin
                     if (mod_set_cache_bit_err(0, cindex, cbit0, cbit1, cway, cerr) <= 0) begin
                        return -1;
                     end
                  end
                  else if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE1_DATA }) begin
                     if (mod_set_cache_bit_err(1, cindex, cbit0, cbit1, cway, cerr) <= 0) begin
                        return -1;
                     end
                  end
                  else if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE2_DATA }) begin
                     if (mod_set_cache_bit_err(2, cindex, cbit0, cbit1, cway, cerr) <= 0) begin
                        return -1;
                     end
                  end
                  else if (evl_cache_ram_type_t'(ctype) inside { SC_RAM_TYPE_ICACHE3_DATA }) begin
                     if (mod_set_cache_bit_err(3, cindex, cbit0, cbit1, cway, cerr) <= 0) begin
                        return -1;
                     end
                  end
                  else begin
                     return -1;
                  end
                  return 1;
               endfunction : set_cache_bit_err


               //-----------------------------------------------------------------------------------
               //
               //
               //
               function new(input string full_name_in     = "evl_shire_channel_rtl_port",
                            input string param_list_in    = "",
                            input int    skip_levels_in[] = {});
                  super.new(full_name_in, param_list_in, skip_levels_in);
               endfunction

            endclass : my_shire_channel_rtl_port

            my_shire_channel_rtl_port my_rtl_port = new($sformatf("%m"),
                                                        $sformatf("pk_num_banks=%0d,pk_num_rbox=%0d,pk_num_icache=%0d,pk_no_uc_sb=%0d",
                                                           NUM_BANKS, NUM_RBOX, NUM_ICACHE, DV_NO_MONITOR_SHIRE_CACHE),
                                                        { 1 });

            initial begin
               m_rtl_port         = my_rtl_port;
               m_channel_rtl_port = my_rtl_port;
            end

         `else // ifdef _SHIRE_CHANNEL_ECC_SUPPORT

            evl_shire_channel_rtl_port my_rtl_port = new($sformatf("%m"),
                                                         $sformatf("pk_num_banks=%0d,pk_num_rbox=%0d,pk_num_icache=%0d,pk_no_uc_sb=%0d",
                                                            NUM_BANKS, NUM_RBOX, NUM_ICACHE, DV_NO_MONITOR_SHIRE_CACHE),
                                                         { 1 });

            initial begin
               m_rtl_port         = my_rtl_port;
               m_channel_rtl_port = my_rtl_port;
            end

         `endif // !ifdef _SHIRE_CHANNEL_ECC_SUPPORT

      end
      else begin :g_ic

         evl_shire_channel_rtl_port my_rtl_port = new($sformatf("%m"),
                                                      $sformatf("pk_num_banks=%0d,pk_num_rbox=%0d,pk_num_icache=%0d,pk_no_uc_sb=%0d",
                                                         NUM_BANKS, NUM_RBOX, NUM_ICACHE, DV_NO_MONITOR_SHIRE_CACHE),
                                                      { 1 });

         initial begin
            m_rtl_port         = my_rtl_port;
            m_channel_rtl_port = my_rtl_port;
         end

      end

   endgenerate


   always @(posedge clock) begin
      evl_ucblock_packet_u ucblock_packet;

      if (m_start_monitor === 1'b1) begin
         if ((reset_c === 1'b1) && (m_reset_on === 1'b0)) begin
            m_channel_rtl_port.assert_reset();
         end
         else if (reset_c === 1'b0) begin
            if (m_reset_off === 1'b0) begin
               m_channel_rtl_port.deassert_reset();
            end
            if ((m_shire_cache_en == 1'b0) && ((m_no_objection == 1'b1) || (evl_shire_cache_en === 1'b1))) begin
               m_shire_cache_en <= 1'b1;
               m_channel_rtl_port.set_int_param_by_name("pk_shire_cache_enabled", 1);
            end
            if ((DV_NO_MONITOR_SHIRE_CACHE == 1'b0) && (m_uc_sb_en != 0)) begin
               if (uc_blocks.neigh_uc_req_valid & uc_blocks.neigh_uc_req_ready) begin
                  ucblock_packet.ucblock_info.etlink_port_id = uc_blocks.neigh_uc_req_info.port_id;
                  ucblock_packet.ucblock_info.axi_req_id     = uc_blocks.uncacheable_noc.first_free;
                  ucblock_packet.ucblock_info.etlink_bus_cmd = evl_bus_cmd_t'(uc_blocks.neigh_uc_req_info.req_info.opcode);
                  ucblock_packet.ucblock_info.etlink_req_id  = evl_bus_req_id_t'(uc_blocks.neigh_uc_req_info.req_info.id);
                  ucblock_packet.ucblock_info.etlink_req_src = evl_bus_req_source_t'(uc_blocks.neigh_uc_req_info.req_info.source);
                  ucblock_packet.ucblock_info.uc_l3_enabled  = uc_blocks.l3_enabled;
                  ucblock_packet.ucblock_info.uc_scp_enabled = uc_blocks.remote_scp_enabled;
                  ucblock_packet.ucblock_info.uc_mem_qos     = qos_dram;
                  m_channel_rtl_port.send_event(UCBLOCK_ETLINK_REQ_INFO, SHIRE_INST_ID, evl_trans_id_t'(uc_blocks.neigh_uc_req_info.trans_id), ucblock_packet);
                  `evl_log(UVM_HIGH, VMOD_UC_SB, m_channel_rtl_port.get_abstract_name(),
                     $sformatf("ucblock: sent rtl event etlink, req ID 0x%0x, trans ID 0x%0x, AXI ID 0x%0x, address 0x%0x",
                        ucblock_packet.ucblock_info.etlink_req_id,
                        ucblock_packet.ucblock_info.etlink_trans_id,
                        ucblock_packet.ucblock_info.axi_req_id,
                        uc_blocks.neigh_uc_req_info.req_info.address))
               end
               if (uc_blocks.esr_enables) begin
                  ucblock_packet.ucblock_info.uc_fcc_enables_in       = uc_blocks.esr_enables.fcc;
                  ucblock_packet.ucblock_info.uc_flb_enables_in       = uc_blocks.esr_enables.barriers;
                  ucblock_packet.ucblock_info.uc_esr_wdata_in         = uc_blocks.esr_wdata;
                  ucblock_packet.ucblock_info.uc_to_neighen_fcc_out   = uc_blocks.uc_to_neigh_fcc_target;
                  ucblock_packet.ucblock_info.uc_to_neighdata_fcc_out = uc_blocks.uc_to_neigh_fcc;
                  m_channel_rtl_port.send_event(UCBLOCK_ESR_REQ_INFO, SHIRE_INST_ID, evl_trans_id_t'(uc_blocks.neigh_uc_req_info.trans_id), ucblock_packet);
                  `evl_log(UVM_HIGH, VMOD_UC_SB, m_channel_rtl_port.get_abstract_name(),
                     $sformatf("ucblock: sent rtl event ESR Enables, FCC enables 0x%0x, FLB enables 0x%0x, data 0x%0x, fccout 0x%0x, fccdataout 0x%0x",
                        ucblock_packet.ucblock_info.uc_fcc_enables_in,
                        ucblock_packet.ucblock_info.uc_flb_enables_in,
                        ucblock_packet.ucblock_info.uc_esr_wdata_in,
                        ucblock_packet.ucblock_info.uc_to_neighen_fcc_out,
                        ucblock_packet.ucblock_info.uc_to_neighdata_fcc_out))
               end // if (uc_blocks.esr_enables)
               if (uc_blocks.flb_neigh_l2_req_valid) begin
                  ucblock_packet.ucblock_info.uc_flb_req_valid = uc_blocks.flb_neigh_l2_req_valid;
                  ucblock_packet.ucblock_info.uc_flb_req_data  = uc_blocks.flb_neigh_l2_req_data;
                  m_channel_rtl_port.send_event(UCBLOCK_FLB_REQ_INFO, SHIRE_INST_ID, evl_trans_id_t'(uc_blocks.neigh_uc_req_info.trans_id), ucblock_packet);
                  `evl_log(UVM_MEDIUM, VMOD_UC_SB, m_channel_rtl_port.get_abstract_name(),
                     $sformatf("ucblock: sent rtl event ESR flbreq 0x%0x, flbreqdata[3:0] 0x%0x 0x%0x 0x%0x 0x%0x",
                        ucblock_packet.ucblock_info.uc_flb_req_valid, ucblock_packet.ucblock_info.uc_flb_req_data[3][12:0],
                        ucblock_packet.ucblock_info.uc_flb_req_data[2][12:0], ucblock_packet.ucblock_info.uc_flb_req_data[1][12:0],
                        ucblock_packet.ucblock_info.uc_flb_req_data[0][12:0]))
               end
               if (uc_blocks.flb_l2_neigh_resp_valid) begin
                  ucblock_packet.ucblock_info.uc_flb_resp_valid = uc_blocks.flb_l2_neigh_resp_valid;
                  ucblock_packet.ucblock_info.uc_flb_resp_data  = uc_blocks.flb_l2_neigh_resp_data;
                  m_channel_rtl_port.send_event(UCBLOCK_FLB_RSP_INFO, SHIRE_INST_ID, evl_trans_id_t'(uc_blocks.neigh_uc_req_info.trans_id), ucblock_packet);
                  `evl_log(UVM_MEDIUM, VMOD_UC_SB, m_channel_rtl_port.get_abstract_name(),
                     $sformatf("ucblock: sent rtl event ESR flb rsp 0x%0x, flb rsp data 0x%0x",
                        ucblock_packet.ucblock_info.uc_flb_resp_valid, ucblock_packet.ucblock_info.uc_flb_resp_data))
               end
            end
         end
         m_reset_on  <= ~reset_c;
         m_reset_off <= reset_c;
         m_channel_rtl_port.ping_clock();
      end
   end

endmodule
