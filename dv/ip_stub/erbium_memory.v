//--------------------------------------------------------------------------------------------------
//
// This module is a modified copy of the Zebu memories used in ET-SoC1
// Modifications:
// - Remove EVL
// - Support for LOCK (TBD)
//
`include "soc.vh"

module erbium_memory
  import erbium_pkg::*;
#(
  parameter erbium_pkg::erbium_memory_type_t MEMORY_TYPE,
  parameter int ID_WIDTH = 20,
  parameter int ADDR_WIDTH = 35,
  parameter int DATA_WIDTH = 512,
  parameter int AXLEN_WIDTH = 8,
  parameter int AXSIZE_WIDTH = 3,
  parameter int AXBURST_WIDTH = 2,
  parameter int AXCACHE_WIDTH = 4,
  parameter int AXPROT_WIDTH = 3,
  parameter int AXQOS_WIDTH = 4,
  parameter int WSTRB_WIDTH = DATA_WIDTH / 8,
  parameter int MEM_SIZE = (2048 * 1024)
  )
(
 input wire                     clock,
 input wire                     resetn,

      // AW channel (low priority)
 input wire                     aw_valid_lo,
 input wire [ID_WIDTH-1:0]      aw_id_lo,
 input wire [ADDR_WIDTH-1:0]    aw_addr_lo,
 input wire [AXLEN_WIDTH-1:0]   aw_len_lo,
 input wire [AXSIZE_WIDTH-1:0]  aw_size_lo,
 input wire [AXBURST_WIDTH-1:0] aw_burst_lo,
 input wire                     aw_lock_lo,
 input wire [AXCACHE_WIDTH-1:0] aw_cache_lo,
 input wire [AXPROT_WIDTH-1:0]  aw_prot_lo,
 input wire [AXQOS_WIDTH-1:0]   aw_qos_lo,
 output reg                     aw_ready_lo,

      // W channel (low priority)
 input wire                     w_valid_lo,
 input wire [DATA_WIDTH-1:0]    w_data_lo,
 input wire [WSTRB_WIDTH-1:0]   w_strb_lo,
 input wire                     w_last_lo,
 output reg                     w_ready_lo,

      // B channel (low priority)
 output reg                     b_valid_lo,
 output reg [ID_WIDTH-1:0]      b_id_lo,
 output axi_resp_t              b_resp_lo,
 input wire                     b_ready_lo,

      // AR channel (low priority)
 input wire                     ar_valid_lo,
 input wire [ID_WIDTH-1:0]      ar_id_lo,
 input wire [ADDR_WIDTH-1:0]    ar_addr_lo,
 input wire [AXLEN_WIDTH-1:0]   ar_len_lo,
 input wire [AXSIZE_WIDTH-1:0]  ar_size_lo,
 input wire [AXBURST_WIDTH-1:0] ar_burst_lo,
 input wire                     ar_lock_lo,
 input wire [AXCACHE_WIDTH-1:0] ar_cache_lo,
 input wire [AXPROT_WIDTH-1:0]  ar_prot_lo,
 input wire [AXQOS_WIDTH-1:0]   ar_qos_lo,
 output reg                     ar_ready_lo,

      // R channel (low priority)
 output reg                     r_valid_lo,
 output reg [ID_WIDTH-1:0]      r_id_lo,
 output reg [DATA_WIDTH-1:0]    r_data_lo,
 output axi_resp_t              r_resp_lo,
 output reg                     r_last_lo,
 input wire                     r_ready_lo
   );


   localparam int ADDR_MSB  = $clog2(MEM_SIZE) - 1;
   localparam int ADDR_LSB  = $clog2(DATA_WIDTH / 8);
   localparam int MEM_DEPTH = 1 << (ADDR_MSB - ADDR_LSB + 1);

   localparam [2:0] wr_st_idle    = 3'b000;
   localparam [2:0] wr_st_capt_hi = 3'b001;
   localparam [2:0] wr_st_resp_hi = 3'b010;
   localparam [2:0] wr_st_capt_lo = 3'b101;
   localparam [2:0] wr_st_resp_lo = 3'b110;

   localparam rd_st_idle = 1'b0;
   localparam rd_st_resp = 1'b1;
// Unconnected ports declaration
   // AW channel (high priority)
   wire                     aw_valid_hi = '0;
   wire [ID_WIDTH-1:0]      aw_id_hi = '0;
   wire [ADDR_WIDTH-1:0]    aw_addr_hi = '0;
   wire [AXLEN_WIDTH-1:0]   aw_len_hi = '0;
   wire [AXSIZE_WIDTH-1:0]  aw_size_hi = '0;
   wire [AXBURST_WIDTH-1:0] aw_burst_hi = '0;
   wire                     aw_lock_hi = '0;
   wire [AXCACHE_WIDTH-1:0] aw_cache_hi = '0;
   wire [AXPROT_WIDTH-1:0]  aw_prot_hi = '0;
   wire [AXQOS_WIDTH-1:0]   aw_qos_hi = '0;
   reg                      aw_ready_hi;

   // W channel (high priority)
   wire                     w_valid_hi = '0;
   wire [DATA_WIDTH-1:0]    w_data_hi  = '0;
   wire [WSTRB_WIDTH-1:0]   w_strb_hi = '0;
   wire                     w_last_hi = '0;
   reg                      w_ready_hi;

   // B channel (high priority)
   reg                      b_valid_hi;
   reg [ID_WIDTH-1:0]       b_id_hi;
   axi_resp_t    b_resp_hi;
   wire                     b_ready_hi = 1'b1;

   // AR channel (high priority)
   wire                     ar_valid_hi  = '0;
   wire [ID_WIDTH-1:0]      ar_id_hi  = '0;;
   wire [ADDR_WIDTH-1:0]    ar_addr_hi  = '0;;
   wire [AXLEN_WIDTH-1:0]   ar_len_hi  = '0;;
   wire [AXSIZE_WIDTH-1:0]  ar_size_hi  = '0;;
   wire [AXBURST_WIDTH-1:0] ar_burst_hi  = '0;;
   wire                     ar_lock_hi  = '0;;
   wire [AXCACHE_WIDTH-1:0] ar_cache_hi  = '0;;
   wire [AXPROT_WIDTH-1:0]  ar_prot_hi  = '0;;
   wire [AXQOS_WIDTH-1:0]   ar_qos_hi  = '0;
   reg                      ar_ready_hi;

   // R channel (high priority)
   reg                      r_valid_hi;
   reg [ID_WIDTH-1:0]       r_id_hi;
   reg [DATA_WIDTH-1:0]     r_data_hi;
   axi_resp_t               r_resp_hi;
   reg                      r_last_hi;
   wire                     r_ready_hi = 1'b1;
// END unconnected old ports - signal declarations

   reg [2:0]             wr_state;
   reg                   rd_state_lo;
   reg                   rd_state_hi;

   reg [DATA_WIDTH-1:0]  w_data_mask;
   reg [WSTRB_WIDTH-1:0] w_strb_r;

   reg [ADDR_WIDTH-1:0]  aw_addr_lo_r;
   reg [AXLEN_WIDTH:0]   aw_len_lo_r;
   reg [ID_WIDTH-1:0]    aw_id_lo_r;
   reg [ADDR_WIDTH-1:0]  ar_addr_lo_r;
   reg [AXLEN_WIDTH:0]   ar_len_lo_r;
   reg [ID_WIDTH-1:0]    ar_id_lo_r;
   reg                   ar_lock_lo_r;
   reg                   aw_lock_lo_r;      

   reg [ADDR_WIDTH-1:0]  aw_addr_hi_r;
   reg [AXLEN_WIDTH:0]   aw_len_hi_r;
   reg [ID_WIDTH-1:0]    aw_id_hi_r;
   reg [ADDR_WIDTH-1:0]  ar_addr_hi_r;
   reg [AXLEN_WIDTH:0]   ar_len_hi_r;
   reg [ID_WIDTH-1:0]    ar_id_hi_r;

   bit [DATA_WIDTH-1:0]  memory[MEM_DEPTH]; // This can be changed to "reg" for ZeBu if necessary


   integer i;
   always @(posedge clock or negedge resetn) begin
      if (resetn == 1'b0) begin
         w_data_mask = {DATA_WIDTH{1'b0}};
         w_strb_r    = '0;
         wr_state     <= wr_st_idle;
         rd_state_lo  <= rd_st_idle;
         rd_state_hi  <= rd_st_idle;

         aw_addr_lo_r <= 0;
         aw_len_lo_r  <= 0;
         aw_ready_lo  <= 1'b0;
         w_ready_lo   <= 1'b0;
         ar_ready_lo  <= 1'b0;
         b_valid_lo   <= 1'b0;
         r_valid_lo   <= 1'b0;
         r_last_lo    <= 1'b1;

         aw_addr_hi_r <= 0;
         aw_len_hi_r  <= 0;
         aw_ready_hi  <= 1'b0;
         w_ready_hi   <= 1'b0;
         ar_ready_hi  <= 1'b0;
         b_valid_hi   <= 1'b0;
         r_valid_hi   <= 1'b0;
         r_last_hi    <= 1'b1;
      end else begin
         //
         // Write state machine
         //
         case (wr_state)
            wr_st_capt_hi: begin
                  aw_ready_hi <= 1'b0;
                  if ((w_valid_hi == 1'b1) && (w_ready_hi == 1'b1)) begin
                     for (i=0; i < WSTRB_WIDTH; i++) begin
                       if (w_strb_r[i]) w_data_mask[i*8+:8] = 8'hff;
                     end
                     memory[aw_addr_hi_r[ADDR_MSB:ADDR_LSB]] <= (w_data_mask & w_data_hi) |
                                                                (~w_data_mask & memory[aw_addr_hi_r[ADDR_MSB:ADDR_LSB]]);
                     if (aw_len_hi_r > 0) begin
                        aw_addr_hi_r <= aw_addr_hi_r + 1;
                        aw_len_hi_r  <= aw_len_hi_r - { {(AXLEN_WIDTH - 1){1'b0}}, 1'b1 };
                        wr_state     <= wr_st_capt_hi;
                        b_valid_hi   <= 1'b0;
                     end
                     else if (aw_len_hi_r == 0) begin
                        w_ready_hi   <= 1'b0;
                        aw_addr_hi_r <= aw_addr_hi_r + 1;
                        aw_len_hi_r  <= {AXLEN_WIDTH{1'b0}};
                        wr_state     <= wr_st_resp_hi;
                        b_id_hi      <= aw_id_hi_r;
                        b_resp_hi    <= AXI_RESP_OKAY;
                        b_valid_hi   <= 1'b1;
                     end
                  end
                  else begin
                     b_valid_hi <= 1'b0;
                  end
               end

            wr_st_resp_hi: begin
                  if ((b_ready_hi == 1'b1) && (b_valid_hi == 1'b1)) begin
                     b_valid_hi <= 1'b0;
                     wr_state   <= wr_st_idle;
                     if (aw_valid_lo == 1'b1) begin
                        aw_ready_lo <= 1'b1;
                     end
                     else begin
                        aw_ready_hi <= 1'b1;
                     end
                  end
               end

            wr_st_capt_lo: begin
                  aw_ready_lo <= 1'b0;
                  if ((w_valid_lo == 1'b1) && (w_ready_lo == 1'b1)) begin
                     for (i=0; i < WSTRB_WIDTH; i++) begin
                       if (w_strb_r[i]) w_data_mask[i*8+:8] = 8'hff;
                     end
                     memory[aw_addr_lo_r[ADDR_MSB:ADDR_LSB]] <= (w_data_mask & w_data_lo) |
                                                                (~w_data_mask & memory[aw_addr_lo_r[ADDR_MSB:ADDR_LSB]]);
                     if (aw_len_lo_r > 0) begin
                        aw_addr_lo_r <= aw_addr_lo_r + 1;
                        aw_len_lo_r  <= aw_len_lo_r - { {(AXLEN_WIDTH - 1){1'b0}}, 1'b1 };
                        wr_state     <= wr_st_capt_lo;
                        b_valid_lo   <= 1'b0;
                     end
                     else if (aw_len_lo_r == 0) begin
                        w_ready_lo   <= 1'b0;
                        aw_addr_lo_r <= aw_addr_lo_r + 1;
                        aw_len_lo_r  <= {AXLEN_WIDTH{1'b0}};
                        wr_state     <= wr_st_resp_lo;
                        b_id_lo      <= aw_id_lo_r;
                        b_resp_lo    <= AXI_RESP_OKAY;
                        b_valid_lo   <= 1'b1;
                     end
                  end
                  else begin
                     b_valid_lo <= 1'b0;
                  end // else: !if((w_valid_lo == 1'b1) && (w_ready_lo == 1'b1))
                  if (aw_lock_lo == 1'b1) begin
                     b_resp_lo <= AXI_RESP_EXOKAY;
                  end
               end

            wr_st_resp_lo: begin
                  if ((b_ready_lo == 1'b1) && (b_valid_lo == 1'b1)) begin
                     b_valid_lo <= 1'b0;
                     wr_state   <= wr_st_idle;
                     if (aw_valid_hi == 1'b1) begin
                        aw_ready_hi <= 1'b1;
                     end
                     else begin
                        aw_ready_lo <= 1'b1;
                     end
                  end
               end

            default: begin
                  w_data_mask = {DATA_WIDTH{1'b0}};
                  w_strb_r    = w_strb_lo;
                  if (aw_valid_hi == 1'b1) begin
                     if (aw_ready_hi == 1'b1) begin
                        wr_state    <= wr_st_capt_hi;
                        w_ready_hi  <= 1'b1;
                        aw_ready_hi <= 1'b1;
                        aw_ready_lo <= 1'b0;
                     end
                     else if ((aw_valid_lo == 1'b1) && (aw_ready_lo == 1'b1)) begin
                        wr_state    <= wr_st_capt_lo;
                        w_ready_lo  <= 1'b1;
                        aw_ready_hi <= 1'b0;
                        aw_ready_lo <= 1'b1;
                     end
                     else begin
                        aw_ready_hi <= 1'b1;
                        aw_ready_lo <= 1'b0;
                     end
                  end
                  else if (aw_valid_lo == 1'b1) begin
                     if (aw_ready_lo == 1'b1) begin
                        wr_state    <= wr_st_capt_lo;
                        w_ready_lo  <= 1'b1;
                        aw_ready_hi <= 1'b0;
                        aw_ready_lo <= 1'b1;
                     end
                     else begin
                        aw_ready_hi <= 1'b0;
                        aw_ready_lo <= 1'b1;
                     end
                  end
                  aw_addr_lo_r <= aw_addr_lo;
                  aw_id_lo_r   <= aw_id_lo;
                  aw_len_lo_r  <= aw_len_lo;
                  b_valid_lo   <= 1'b0;
                  aw_addr_hi_r <= aw_addr_hi;
                  aw_id_hi_r   <= aw_id_hi;
                  aw_len_hi_r  <= aw_len_hi;
                  b_valid_hi   <= 1'b0;
               end
         endcase

         //
         // Read state machine (high priority)
         //
         case (rd_state_hi)
            rd_st_resp: begin
                  r_id_hi   <= ar_id_hi_r;
                  r_resp_hi <= AXI_RESP_OKAY;
                  if ((r_valid_hi == 1'b1) && (r_ready_hi == 1'b1)) begin
                     r_data_hi <= memory[ar_addr_hi_r[ADDR_MSB:ADDR_LSB]];
                     if (ar_len_hi_r != 0) begin
                        r_last_hi    <= (ar_len_hi_r <= { {(AXLEN_WIDTH - 1){1'b0}}, 1'b1 }) ? 1'b1 : 1'b0;
                        r_valid_hi   <= 1'b1;
                        ar_ready_hi  <= 1'b0;
                        ar_addr_hi_r <= ar_addr_hi_r + 1;
                        ar_len_hi_r  <= ar_len_hi_r - { {(AXLEN_WIDTH - 1){1'b0}}, 1'b1 };
                     end
                     else begin
                        rd_state_hi <= rd_st_idle;
                        r_last_hi   <= 1'b1;
                        r_valid_hi  <= 1'b0;
                        ar_ready_hi <= 1'b1;
                     end
                  end
                  else begin
                     r_valid_hi  <= 1'b1;
                     ar_ready_hi <= 1'b0;
                  end
               end

            rd_st_idle: begin
                  if ((ar_valid_hi == 1'b1) && (ar_ready_hi == 1'b1)) begin
                     rd_state_hi <= rd_st_resp;
                     ar_ready_hi <= 1'b0;
                     r_data_hi   <= memory[ar_addr_hi[ADDR_MSB:ADDR_LSB]];
                     r_id_hi     <= ar_id_hi;
                     r_resp_hi   <= AXI_RESP_OKAY;
                     r_last_hi   <= (ar_len_hi == {AXLEN_WIDTH{1'b0}}) ? 1'b1 : 1'b0;
                     r_valid_hi  <= 1'b1;
                  end
                  else begin
                     rd_state_hi <= rd_st_idle;
                     ar_ready_hi <= 1'b1;
                     r_data_hi   <= {DATA_WIDTH{1'b0}};
                     r_id_hi     <= {ID_WIDTH{1'b0}};
                     r_resp_hi   <= AXI_RESP_OKAY;
                     r_last_hi   <= 1'b1;
                     r_valid_hi  <= 1'b0;
                  end
                  ar_addr_hi_r <= ar_addr_hi;
                  ar_id_hi_r   <= ar_id_hi;
                  ar_len_hi_r  <= ar_len_hi;
               end
         endcase

         //
         // Read state machine (low priority)
         //
         case (rd_state_lo)
            rd_st_resp: begin
                  r_id_lo   <= ar_id_lo_r;
                  r_resp_lo <= AXI_RESP_OKAY;
                  if ((r_valid_lo == 1'b1) && (r_ready_lo == 1'b1)) begin
                     r_data_lo <= memory[ar_addr_lo_r[ADDR_MSB:ADDR_LSB]];
                     if (ar_len_lo_r != 0) begin
                        r_last_lo    <= (ar_len_lo_r <= { {(AXLEN_WIDTH - 1){1'b0}}, 1'b1 }) ? 1'b1 : 1'b0;
                        r_valid_lo   <= 1'b1;
                        ar_ready_lo  <= 1'b0;
                        ar_addr_lo_r <= ar_addr_lo_r + 1;
                        ar_len_lo_r  <= ar_len_lo_r - { {(AXLEN_WIDTH - 1){1'b0}}, 1'b1 };
                     end
                     else begin
                        rd_state_lo <= rd_st_idle;
                        r_last_lo   <= 1'b1;
                        r_valid_lo  <= 1'b0;
                        ar_ready_lo <= 1'b1;
                     end // else: !if(ar_len_lo_r != 0)
                     if (ar_lock_lo == 1'b1) begin
                        r_resp_lo <= AXI_RESP_EXOKAY;
                     end
                  end
                  else begin
                     r_valid_lo  <= 1'b1;
                     ar_ready_lo <= 1'b0;
                  end
               end

            rd_st_idle: begin
                  if ((ar_valid_lo == 1'b1) && (ar_ready_lo == 1'b1)) begin
                     rd_state_lo <= rd_st_resp;
                     ar_ready_lo <= 1'b0;
                     r_data_lo   <= memory[ar_addr_lo[ADDR_MSB:ADDR_LSB]];
                     r_id_lo     <= ar_id_lo;
                     r_resp_lo   <= AXI_RESP_OKAY;
                     r_last_lo   <= (ar_len_lo == {AXLEN_WIDTH{1'b0}}) ? 1'b1 : 1'b0;
                     r_valid_lo  <= 1'b1;
                  end
                  else begin
                     rd_state_lo <= rd_st_idle;
                     ar_ready_lo <= 1'b1;
                     r_data_lo   <= {DATA_WIDTH{1'b0}};
                     r_id_lo     <= {ID_WIDTH{1'b0}};
                     r_resp_lo   <= AXI_RESP_OKAY;
                     r_last_lo   <= 1'b1;
                     r_valid_lo  <= 1'b0;
                  end // else: !if((ar_valid_lo == 1'b1) && (ar_ready_lo == 1'b1))
                  if (ar_lock_lo == 1'b1) begin
                     r_resp_lo <= AXI_RESP_EXOKAY;
                  end
                  ar_addr_lo_r <= ar_addr_lo;
                  ar_id_lo_r   <= ar_id_lo;
                  ar_len_lo_r  <= ar_len_lo;
               end
         endcase
      end
   end

   `ifdef ET_SIMULATION
      typedef logic[ADDR_WIDTH-1:0] mram_addr_t;
      typedef logic [31:0]          word_t;
   
      string      abstract_name;
      mram_addr_t m_cur_addr_mask   = mram_addr_t'(0);
      mram_addr_t m_cur_ignore_mask = mram_addr_t'(0);
      mram_addr_t m_cur_match_mask  = mram_addr_t'(0);
      mram_addr_t m_cur_match_value = mram_addr_t'(0);


      //--------------------------------------------------------------------------------------------
      //
      // Function: add_addr_template
      //
      function automatic void add_addr_template(input mram_addr_t addr_mask_in,
                                                input mram_addr_t addr_value_in,
                                                input bit         addr_ignore,
                                                input bit         clear_first);
         if (clear_first == 1'b1) begin
            m_cur_addr_mask   = mram_addr_t'(0);
            m_cur_ignore_mask = mram_addr_t'(0);
            m_cur_match_mask  = mram_addr_t'(0);
            m_cur_match_value = mram_addr_t'(0);
         end
         if (addr_mask_in != mram_addr_t'(0)) begin
            m_cur_addr_mask |= addr_mask_in;
            if (addr_ignore == 1'b1) begin
               m_cur_ignore_mask |= addr_mask_in;
               m_cur_match_mask  &= ~m_cur_ignore_mask;
               m_cur_match_value &= m_cur_match_mask;
            end
            else begin
               m_cur_match_mask  |= (addr_mask_in & ~m_cur_ignore_mask);
               m_cur_match_value |= (addr_mask_in & addr_value_in);
               m_cur_match_value &= m_cur_match_mask;
            end
         end
      endfunction : add_addr_template

   //--------------------------------------------------------------------------------------------
   // Function: find_file
   // Searches for a file in current directory, then under REPOROOT if not found.
   // Returns full path if found, empty string if not found.
   //--------------------------------------------------------------------------------------------
   function automatic string find_file(input string file_name);
      int fd;
      string full_path;
      string repo_root;
      
      // Empty filename
      if (file_name == "") return "";
      
      // Try current directory first
      fd = $fopen(file_name, "r");
      if (fd != 0) begin
         $fclose(fd);
         return file_name;
      end
      
      // If absolute path failed, give up
      if (file_name[0] == "/") return "";
      
      // Try under REPOROOT
      if ($value$plusargs("REPOROOT=%s", repo_root)) begin
         // Ensure ends with /
         if (repo_root[repo_root.len()-1] != "/") begin
            repo_root = {repo_root, "/"};
         end
         
         full_path = {repo_root, file_name};
         fd = $fopen(full_path, "r");
         if (fd != 0) begin
            $fclose(fd);
            return full_path;
         end
      end
      
      return "";  // Not found
   endfunction

      //--------------------------------------------------------------------------------------------
      //
      // Function: perform_load_mem
      //
      function automatic void perform_load_mem(input string file_name);
         int                  fd;
         string               full_file_name;
         string               line;
         int                  mod_limit;
         int                  addr_shifts[$];
         mram_addr_t          addr_masks[$];
         mram_addr_t          retain_masks[$];
         mram_addr_t          paddr;
         bit [DATA_WIDTH-1:0] data_xword;
         bit [DATA_WIDTH-1:0] data_write_mask;
         bit [DATA_WIDTH-1:0] data_write_value;
         int                  data_index;
         int                  data_shift;

         full_file_name = find_file(file_name);
         if (full_file_name == "") begin
            return;
         end

         addr_shifts  = {};
         addr_masks   = {};
         retain_masks = {};
         if (m_cur_addr_mask != mram_addr_t'(0)) begin
            mram_addr_t addr_mask;
            mram_addr_t retain_mask;
            int         hi_shift;
            int         cur_shift;
            int         bit_index;

            addr_mask   = m_cur_addr_mask;
            retain_mask = mram_addr_t'(0);
            hi_shift    = 0;
            cur_shift   = 0;
            bit_index   = 0;
            while (addr_mask != 0) begin
               if (addr_mask[0:0] == 1'b1) begin
                  while (addr_mask[0:0] == 1'b1) begin
                     bit_index++;
                     cur_shift++;
                     addr_mask >>= 1;
                  end
                  addr_shifts.push_back(cur_shift);
                  retain_masks.push_back(retain_mask);
                  addr_masks.push_back(~retain_mask);
               end
               retain_mask = (retain_mask << 1) | mram_addr_t'(1);
               bit_index++;
               addr_mask >>= 1;
            end
         end
         mod_limit = addr_shifts.size();

         fd = $fopen(full_file_name, "r");
         $display("%t:%s:loading the memory image file \"%s\"", $time, abstract_name, full_file_name);
    
         while (1) begin
            mram_addr_t new_paddr;
            word_t  data;

            if ($fgets(line, fd) == 0) begin
               break;
            end
            if ($sscanf(line, "%x %x", paddr, data) == 2) begin
               bit have_match;

               new_paddr = paddr;
               if (m_cur_match_mask == mram_addr_t'(0)) begin
                  have_match = 1'b1;
               end
               else if (((m_cur_match_value ^ paddr) & m_cur_match_mask) != mram_addr_t'(0)) begin
                  have_match = 1'b0;
               end
               else begin
                  have_match = 1'b1;
                  for (int ii = 0; ii < mod_limit; ii++) begin
                     new_paddr &= retain_masks[ii];
                     new_paddr |= ((paddr >> addr_shifts[ii]) & addr_masks[ii]);
                  end
               end
               if (have_match == 1'b1) begin
                  data_index         = int'(new_paddr[ADDR_MSB:ADDR_LSB]);
                  data_xword         = memory[data_index];
                  data_shift         = int'(new_paddr[ADDR_LSB-1:0]) << 3;
                  data_write_mask    = ({ {(DATA_WIDTH-32){1'b0}}, {32{1'b1}} }) << data_shift;
                  data_write_value   = ({ {(DATA_WIDTH-32){1'b0}}, data }) << data_shift;
                  data_xword         = (~data_write_mask & data_xword) | (data_write_mask & data_write_value);
                  memory[data_index] = data_xword;
                  if (paddr != new_paddr) begin
                     $display("%t:%s:converted address 0x%x to 0x%x (loaded 0x%x)", $time, abstract_name, paddr, new_paddr, data);
                  end
               end
            end
         end
         $fclose(fd);
      endfunction : perform_load_mem

      string m_mem_image_path;
      string m_mem_image;

      initial begin
         wait (resetn == 1'b0);
         abstract_name = "UNKNOWN";
         if (MEMORY_TYPE == erbium_pkg::MRAM) begin
            abstract_name = "MRAM";
            add_addr_template(mram_addr_t'(1) << 31, mram_addr_t'(0) << 31, 1'b0, 1'b1);
            add_addr_template(mram_addr_t'(1) << 30, mram_addr_t'(1) << 30, 1'b0, 1'b0);
         end
         else if (MEMORY_TYPE == erbium_pkg::SRAM) begin
            abstract_name = "SRAM";
            add_addr_template(mram_addr_t'(32'hFFFFF000), mram_addr_t'(32'h0200E000), 1'b0, 1'b1);
         end 
         else if (MEMORY_TYPE == erbium_pkg::BOOTROM) begin
            $error("Unsupported use of erbium_memory %s", `__FILE__);
         end
         if ($value$plusargs("memImagePath=%s", m_mem_image_path) == 0) begin
            m_mem_image_path = "";
         end
         else if (m_mem_image_path.len() > 0 && m_mem_image_path[m_mem_image_path.len() - 1] != "/") begin
            m_mem_image_path = $sformatf("%0s/", m_mem_image_path);
         end
         if ($value$plusargs("memImage=%s", m_mem_image) == 0) begin
            m_mem_image = "memImage.hex";
         end
         m_mem_image = $sformatf("%0s%0s", m_mem_image_path, m_mem_image);
         perform_load_mem("memImage.hex");
      end
   `endif

endmodule
