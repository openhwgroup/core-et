`ifndef __EVL_SHIRE_CACHE_PKG

   `define __EVL_SHIRE_CACHE_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"
   `include "dv/common/axi/axi_vc/evl_axi_pkg.sv"
   `include "dv/shire_common/shire_cache/evl_shire_cache_types_pkg.sv"
   `include "dv/shire_common/shire_cache/evl_shire_cache_regs_pkg.sv"

   `define EVL_BASE_PORT_NUM_AXI 16

   `define SCQ_None         24'h000000
   `define SCQ_Victim       24'h000001
   `define SCQ_Stale        24'h000002
   `define SCQ_Local        24'h000004
   `define SCQ_Remote       24'h000008
   `define SCQ_Partial      24'h000010
   `define SCQ_CheckPartial 24'h000020
   `define SCQ_MergePartial 24'h000040
   `define SCQ_LateRead     24'h000080
   `define SCQ_SecondPass   24'h000100
   `define SCQ_UseSrcAddr   24'h000200
   `define SCQ_NoRegion     24'h000400
   `define SCQ_CheckLock    24'h000800
   `define SCQ_AllLocked    24'h001000
   `define SCQ_CheckSize    24'h002000
   `define SCQ_IdxAddr      24'h004000 // need to convert the index cacheop addr
   `define SCQ_Bypass       24'h008000
   `define SCQ_DownDis      24'h010000 // Downstream entity is disabled in some way
   `define SCQ_FromMesh     24'h020000
   `define SCQ_DecErr       24'h040000 // Predict a decode error of some sort (addr or shire id)
   `define SCQ_Undefined3   24'h080000
   `define SCQ_L1           24'h100000 // L1 src, L1 dst   (set to a safe non-zero value)
   `define SCQ_L1_L2        24'h400000 // L1 src, L2 dst   (0100)
   `define SCQ_L1_L3        24'h800000 // L1 src, L3 dst   (1000)
   `define SCQ_L1_Mem       24'hc00000 // L1 src, mem dst  (1100)
   `define SCQ_L2           24'h500000 // L2 src, L2 dst   (0101)
   `define SCQ_L2_L3        24'h900000 // L2 src, L3 dst   (1001)
   `define SCQ_L2_Mem       24'hd00000 // L2 src, mem dst  (1101)
   `define SCQ_L3           24'ha00000 // L3 src, L3 dst   (1010)
   `define SCQ_L3_Mem       24'he00000 // L3 src, mem dst  (1110)
   `define SCQ_Mem          24'hf00000 // Mem src, mem dst (1111)
   `define SCQ_SrcDstMask   24'hf00000


   //-----------------------------------------------------------------------------------------------
   //
   //
   //
   package evl_shire_cache_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_etlink_types_pkg::*;
      import evl_axi_types_pkg::*;
      import evl_shire_cache_types_pkg::*;
      import evl_shire_cache_regs_pkg::*;

      typedef bit [23:0]                 evl_sc_qual_t;
      typedef bit [`SC_RBUF_ID_SIZE-1:0] evl_sc_rbuf_id_t;


      typedef enum int {
            SHIRE_CACHE_NO_PACKET,
            SHIRE_CONFIG,
            SHIRE_CACHE_CONFIG,
            SHIRE_CACHE_PIPE_CONFIG,
            SHIRE_CACHE_L3_INDEX_CONFIG,
            SHIRE_CACHE_IDX_COP_REQ,
            SHIRE_CACHE_REQQ_ALLOC,
            SHIRE_CACHE_REQQ_DEALLOC,
            SHIRE_CACHE_RBUF_REQ,
            SHIRE_CACHE_TC_REQ,
            SHIRE_CACHE_TC_INFO,
            SHIRE_CACHE_TC_TAG_RSP,
            SHIRE_CACHE_CBUF_STATUS,
            SHIRE_CACHE_ALL_INV,
            SHIRE_CACHE_CB_INV_START,
            SHIRE_CACHE_RSP_REQQ,
            SHIRE_CACHE_RSP_DSTQ
         } evl_sc_event_t;


      typedef enum bit [3:0] {
            SC_TABLE_REQQ_REQ,
            SC_TABLE_RBUF_REQ,
            SC_TABLE_TC_REQ,
            SC_TABLE_TAG_RSP
         }  evl_sc_sub_table_t;


      //
      // The following packed structure is used to specify configuration changes.
      //
      typedef struct packed {
            bit [`NUM_NEIGH_R] neigh_id_3;
            bit [`NUM_NEIGH_R] neigh_id_2;
            bit [`NUM_NEIGH_R] neigh_id_1;
            bit [`NUM_NEIGH_R] neigh_id_0;
         } evl_shire_config_t;


      //
      // The following packed structure is used to specify configuration changes.
      //
      typedef struct packed {
            bit [3:0]   bank;
            bit [3:0]   num_ways;
            bit [7:0]   bank_lsb;
            bit [13:0]  set_size;
            bit         remote_enable;
            bit         remote_l3_enable;
            bit         bypass;
            evl_paddr_t set_base;
            bit         err_rsp_enable;
            bit [7:0]   err_interrupt_enable;
            bit         two_shire_aliasing_use_lsb;
            bit         all_shire_aliasing;
         } evl_sc_bank_config_t;


      //
      // The following packed structure is used to specify configuration changes for pipe control
      // and the coalescing buffer.
      //
      typedef struct packed {
            bit [3:0] bank;
            bit       l2_rbuf_enable;
            bit       scp_rbuf_enable;
            bit       zero_state_enable;
            bit       cbuf_enable;
            int       num_entries;
            bit [2:0] ram_delay;
         } evl_sc_pipe_config_t;


      //
      // The following packed structure is used to specify configuration changes for the L3 index bits.
      //
      typedef struct packed {
            bit [3:0] bank_0_bit;
            bit [3:0] bank_1_bit;
            bit [3:0] bank_2_bit;
            bit [3:0] subbank_0_bit;
            bit [3:0] subbank_1_bit;
            bit [3:0] subbank_2_bit;
            bit [3:0] shire_0_bit;
         } evl_sc_l3_index_config_t;


      //
      // The following packed structure is intended to be used for index cache ops.
      //
      typedef struct packed {
            evl_sc_reqq_opcode_t opcode;    // Opcode request
            evl_paddr_t          address;   // Address of the request
         } evl_sc_idx_cop_req_t;


      //
      // The following packed structure is used to specify REQQ allocation/deallocation.
      //
      typedef struct packed {
            bit [`SC_REQQ_DEPTH-1:0]  reqq_nhot;
            evl_bus_req_id_t          tag_id;
            evl_sc_reqq_id_t          reqq_id;
            evl_sc_reqq_id_t          paired_id;
            bit [7:0]                 port;
            evl_sc_reqq_orig_opcode_t orig_opcode;
            evl_paddr_t               address;
            evl_sc_address_space_t    address_space;
            bit                       second_alloc;
         } evl_sc_reqq_alloc_t;


      //
      // The following packed structure is intended to be a reproduction of sc_reqq_pipe_req_t.
      //
      typedef struct packed {
            evl_sc_reqq_id_t     reqq_id;
            evl_sc_reqq_opcode_t opcode;    // Opcode request
            evl_paddr_t          address;   // Address of the request
            bit                  zero_data; // indicates that all data being written is zero
            evl_line_qword_en_t  qwen;      // qword enable
            int                  sc_size;   // Size of the transaction
         } evl_sc_reqq_pipe_req_t;


      //
      // The following packed structure is intended to be a reproduction of sc_pipe_info_t.
      //
      typedef struct packed {
            evl_paddr_t         pipe_set;
            int                 pipe_sub_bank;
            bit                 tag_hit;
            evl_way_t           tag_ram_hit_way;
            bit                 victim;
            bit                 data_rd_req;
            bit                 data_wr_req;
            bit                 data_wr_zero;
            evl_line_qword_en_t data_wr_qwen;
            evl_line_qword_en_t dataq_sel_qwen;
            bit                 merge_wr_data_in_rd_rsp;
            bit                 merge_rd_data_in_wr_req;
            bit                 data_rsp_req;
            bit                 data_rsp_zero;
            evl_line_qword_en_t data_rsp_qwen;
         } evl_sc_pipe_info_t;


      //
      // The following packed structure is intended to be a reproduction of sc_pipe_rbuf_req_t.
      //
      typedef struct packed {
            evl_sc_reqq_id_t reqq_id;
            bit              scp;
            evl_paddr_t      address;   // Address of the request
            evl_sc_rbuf_id_t rbuf_id;
         } evl_sc_pipe_rbuf_req_t;


      //
      // The following packed structure is intended to be (mostly) a reproduction of sc_pipe_tag_rsp_t.
      //
      typedef struct packed {
            evl_sc_reqq_id_t      reqq_id;
            bit                   hit;
            bit                   victim;
            bit                   victim_silent;
            evl_paddr_t           victim_address;
            bit                   victim_write_around;
            evl_line_qword_en_t   victim_qwen;
            bit                   cbuf_match;
            evl_sc_pipe_err_rsp_t err_rsp;
         } evl_sc_pipe_tag_rsp_t;


      //
      // The following packed structure is intended to be used for CBUF final checks
      //
      typedef struct packed {
            bit [7:0]   entry_num;
            bit         valid;
            bit         last_one;
            evl_paddr_t address;
         } evl_dv_cbuf_status_t;


      //
      // The following packed structure is intended to be used to specify the REQQ ID for a
      // response.
      //
      typedef struct packed {
            bit [3:0]             bank;
            evl_sc_reqq_id_t      reqq_id;
            evl_neighborhood_id_t neigh_id;
         } evl_sc_rsp_reqq_t;


      //
      // Basic packet structure.
      //
      typedef struct packed {
            bit [167:0] info;
         } evl_sc_packet_s;


      //
      // Encapsulate evl_shire_config_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_shire_config_t):0] padding;
            evl_shire_config_t                    info;
         } evl_shire_config_s;


      //
      // Encapsulate evl_sc_bank_config_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_bank_config_t):0] padding;
            evl_sc_bank_config_t                    info;
         } evl_sc_bank_config_s;


      //
      // Encapsulate evl_sc_pipe_config_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_pipe_config_t):0] padding;
            evl_sc_pipe_config_t                    info;
         } evl_sc_pipe_config_s;


      //
      // Encapsulate evl_sc_l3_index_config_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_l3_index_config_t):0] padding;
            evl_sc_l3_index_config_t                    info;
         } evl_sc_l3_index_config_s;


      //
      // The following packed structure is intended to be used for index cache ops.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_idx_cop_req_t):0] padding;
            evl_sc_idx_cop_req_t                    info;
         } evl_sc_idx_cop_req_s;


      //
      // Encapsulate evl_sc_reqq_alloc_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_reqq_alloc_t):0] padding;
            evl_sc_reqq_alloc_t                    info;
         } evl_sc_reqq_alloc_s;


      //
      // Encapsulate evl_sc_pipe_tag_rsp_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_pipe_tag_rsp_t):0] padding;
            evl_sc_pipe_tag_rsp_t                    info;
         } evl_sc_pipe_tag_rsp_s;


      //
      // Encapsulate evl_sc_reqq_pipe_req_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_reqq_pipe_req_t):0] padding;
            evl_sc_reqq_pipe_req_t                    info;
         } evl_sc_reqq_pipe_req_s;


      //
      // Encapsulate evl_sc_pipe_rbuf_req_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_pipe_rbuf_req_t):0] padding;
            evl_sc_pipe_rbuf_req_t                    info;
         } evl_sc_pipe_rbuf_req_s;


      //
      // Encapsulate evl_sc_pipe_info_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_pipe_info_t):0] padding;
            evl_sc_pipe_info_t                    info;
         } evl_sc_pipe_info_s;


      //
      // Encapsulate evl_dv_cbuf_status_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_dv_cbuf_status_t):0] padding;
            evl_dv_cbuf_status_t                    info;
         } evl_dv_cbuf_status_s;


      //
      // Encapsulate evl_sc_rsp_reqq_t into a right-sized structure.
      //
      typedef struct packed {
            bit [167-$bits(evl_sc_rsp_reqq_t):0] padding;
            evl_sc_rsp_reqq_t                    info;
         } evl_sc_rsp_reqq_s;


      //
      // Encapsulate all of the structure definitions into a common union.
      //
      typedef union packed {
            evl_sc_packet_s          packet;
            evl_shire_config_s       shire_config_info;
            evl_sc_bank_config_s     bank_config_info;
            evl_sc_pipe_config_s     pipe_config_info;
            evl_sc_l3_index_config_s l3_index_config_info;
            evl_sc_idx_cop_req_s     idx_cop_req;
            evl_sc_reqq_alloc_s      reqq_alloc;
            evl_sc_pipe_rbuf_req_s   pipe_rbuf_req;
            evl_sc_reqq_pipe_req_s   pipe_req_tc;
            evl_sc_pipe_info_s       pipe_info_tc;
            evl_sc_pipe_tag_rsp_s    pipe_tag_rsp_tc;
            evl_dv_cbuf_status_s     cbuf_status;
            evl_sc_rsp_reqq_s        rsp_reqq;
         } evl_sc_packet_u;


      typedef class evl_l2_scoreboard;
      typedef class evl_l3_scoreboard;
      typedef class evl_sc_scoreboard_cache_cover;
      typedef class evl_sc_scoreboard_cache;
      typedef class evl_scp_scoreboard;
      typedef class evl_shire_cache_axi_meshq;
      typedef class evl_shire_cache_packet;
      typedef class evl_shire_cache_reqq;
      typedef class evl_shire_cache_rtl_port;
      typedef class evl_shire_cache_scoreboard_cover;
      typedef class evl_shire_cache_scoreboard;
      typedef class evl_shire_cache_config_cover;

      `include "dv/shire_common/shire_cache/evl_shire_cache_env.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_packet.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_rtl_port.sv"
      `include "dv/shire_common/shire_cache/evl_sc_scoreboard_cache.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_reqq.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_axi_meshq.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_scoreboard.sv"
      `include "dv/shire_common/shire_cache/evl_scp_scoreboard.sv"
      `include "dv/shire_common/shire_cache/evl_l2_scoreboard.sv"
      `include "dv/shire_common/shire_cache/evl_l3_scoreboard.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_config_cover.sv"
      `include "dv/shire_common/shire_cache/evl_sc_scoreboard_cache_cover.sv"
      `include "dv/shire_common/shire_cache/evl_shire_cache_scoreboard_cover.sv"

   endpackage : evl_shire_cache_pkg

`endif // !__EVL_SHIRE_CACHE_PKG
