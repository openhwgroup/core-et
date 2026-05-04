// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// RBOX types

////////////////////////////////////////////////////////////////////////////////
// ENUMS
////////////////////////////////////////////////////////////////////////////////

typedef enum logic [2:0] 
{
    RBOX_INPCK_TRIANGLE_WITH_TILE_64X64   = 3'd0,
    RBOX_INPCK_TRIANGLE_WITH_TILE_128X128 = 3'd1,
    RBOX_INPCK_LARGE_TRIANGLE             = 3'd2,
    RBOX_INPCK_FULLY_COVERED_TILE         = 3'd3,
    RBOX_INPCK_LARGE_TRIANGLE_TILE        = 3'd4,
    RBOX_INPCK_RBOX_STATE                 = 3'd5,
    RBOX_INPCK_FRAG_SHADING_STATE         = 3'd6,
    RBOX_INPCK_END_OF_BUFFER              = 3'd7
} rbox_inpck_type_t;

typedef enum logic [1:0]
{
    RBOX_OUTPCK_STATE_INFO = 0,
    RBOX_OUTPCK_QUAD_INFO  = 1,
    RBOX_OUTPCK_END_PHASE  = 2
} rbox_outpck_type_t;

typedef enum logic [2:0]
{
    RBOX_TILE_SIZE_64X64 = 3'd0,
    RBOX_TILE_SIZE_64X32 = 3'd1,
    RBOX_TILE_SIZE_32X32 = 3'd2,
    RBOX_TILE_SIZE_16X16 = 3'd3,
    RBOX_TILE_SIZE_8X8   = 3'd4,
    RBOX_TILE_SIZE_4X4   = 3'd5
} rbox_tile_size_t;

typedef enum logic 
{
    RBOX_TRI_FACING_FRONT = 0,
    RBOX_TRI_FACING_BACK  = 1
} rbox_tri_facing_t;

typedef enum logic [8:0]
{
    RBOX_DS_FORMAT_D16_UNORM           = 9'd124,
    RBOX_DS_FORMAT_X8_D24_UNORM_PACK32 = 9'd125,
    RBOX_DS_FORMAT_D32_SFLOAT          = 9'd126,
    RBOX_DS_FORMAT_D16_UNORM_S8_UINT   = 9'd128,
    RBOX_DS_FORMAT_D24_UNORM_S8_UINT   = 9'd129,
    RBOX_DS_FORMAT_D32_SFLOAT_S8_UINT  = 9'd130
} rbox_ds_format_t;

typedef enum logic [2:0]
{
    RBOX_COMP_OP_NEVER            = 3'd0,
    RBOX_COMP_OP_LESS             = 3'd1,
    RBOX_COMP_OP_EQUAL            = 3'd2,
    RBOX_COMP_OP_LESS_OR_EQUAL    = 3'd3,
    RBOX_COMP_OP_GREATER          = 3'd4,
    RBOX_COMP_OP_NOT_EQUAL        = 3'd5,
    RBOX_COMP_OP_GREATER_OR_EQUAL = 3'd6,
    RBOX_COMP_OP_ALWAYS           = 3'd7
} rbox_comp_op_t;

typedef enum logic [2:0]
{
    RBOX_STENCIL_OP_KEEP      = 3'd0,
    RBOX_STENCIL_OP_ZERO      = 3'd1,
    RBOX_STENCIL_OP_REPLACE   = 3'd2,
    RBOX_STENCIL_OP_INC_CLAMP = 3'd3,
    RBOX_STENCIL_OP_DEC_CLAMP = 3'd4,
    RBOX_STENCIL_OP_INVERT    = 3'd5,
    RBOX_STENCIL_OP_INC_WRAP  = 3'd6,
    RBOX_STENCIL_OP_DEC_WRAP  = 3'd7
} rbox_stencil_op_t;


typedef enum logic [1:0]
{
    RBOX_MSAA_MODE_2X  = 2'd0,
    RBOX_MSAA_MODE_4X  = 2'd1,
    RBOX_MSAA_MODE_8X  = 2'd2,
    RBOX_MSAA_MODE_16X = 2'd3
} rbox_msaa_mode_t;



typedef enum logic [`RBOX_MSAA_SAMP_POS_RNG]
{
    RBOX_MSAA_POS_0000 = 'b0000,
    RBOX_MSAA_POS_0625 = 'b0001,
    RBOX_MSAA_POS_1250 = 'b0010,
    RBOX_MSAA_POS_1875 = 'b0011,
    RBOX_MSAA_POS_2500 = 'b0100,
    RBOX_MSAA_POS_3125 = 'b0101,
    RBOX_MSAA_POS_3750 = 'b0110,
    RBOX_MSAA_POS_4375 = 'b0111,
    RBOX_MSAA_POS_5000 = 'b1000,
    RBOX_MSAA_POS_5625 = 'b1001,
    RBOX_MSAA_POS_6250 = 'b1010,
    RBOX_MSAA_POS_6875 = 'b1011,
    RBOX_MSAA_POS_7500 = 'b1100,
    RBOX_MSAA_POS_8125 = 'b1101,
    RBOX_MSAA_POS_8750 = 'b1110,
    RBOX_MSAA_POS_9375 = 'b1111
} rbox_msaa_pos_t;

typedef enum logic [3:0] 
{
    RBOX_TRAVERSE_START = 4'b0001,
    RBOX_TRAVERSE_QUAD  = 4'b0010,
    RBOX_TRAVERSE_ROW   = 4'b0100,
    RBOX_TRAVERSE_END   = 4'b1000
} rbox_traverse_state_t;

typedef enum logic [1:0] 
{
    RBOX_TRAVERSE_IDX_START = 2'd0,
    RBOX_TRAVERSE_IDX_QUAD  = 2'd1,
    RBOX_TRAVERSE_IDX_ROW   = 2'd2,
    RBOX_TRAVERSE_IDX_END   = 2'd3
} rbox_traverse_state_idx_t;


typedef enum logic [7:0]
{
    RBOX_READY    = 8'h00,
    RBOX_WORKING  = 8'h01,
    RBOX_FINISHED = 8'h02,
    RBOX_ERROR    = 8'h80
} rbox_status_t;

typedef logic [`RBOX_MSAA_SAMPLE_RNG]        rbox_msaa_sample_idx_t;

typedef rbox_msaa_pos_t [1:0]                rbox_msaa_sample_pos_t;

typedef logic [(`RBOX_MAX_RENDER_DIM_L-1):2] rbox_tile_pos_t;

typedef logic [`RBOX_DEPTH_COEF_RNG]         rbox_depth_coef_t;

typedef logic [`RBOX_DEPTH_SAMP_RNG]         rbox_depth_sample_t;

typedef logic [`RBOX_EDGE_SAMP_RNG]          rbox_edge_sample_t;

typedef logic [`RBOX_EDGE_COEF_R]            rbox_edge_coef_t;

typedef logic [`RBOX_STEP_R]                 rbox_step_t;


////////////////////////////////////////////////////////////////////////////////
// STRUCTS
////////////////////////////////////////////////////////////////////////////////


typedef struct packed
{
    logic [`RBOX_64T_EDGE_COEF_PAD] b_reserved;
    logic [`RBOX_64T_EDGE_COEF_RNG] b_coef; // 2's complement 9.14
    logic [`RBOX_64T_EDGE_COEF_PAD] a_reserved;
    logic [`RBOX_64T_EDGE_COEF_RNG] a_coef; // 2's complement 9.14
} rbox_edge_eq_64x64_t;


typedef struct packed
{
    logic [`RBOX_128T_EDGE_COEF_PAD] b_reserved;
    logic [`RBOX_128T_EDGE_COEF_RNG] b_coef; // 2's complement 9.15
    logic [`RBOX_128T_EDGE_COEF_PAD] a_reserved;
    logic [`RBOX_128T_EDGE_COEF_RNG] a_coef; // 2's complement 9.15
} rbox_edge_eq_128x128_t;

typedef struct packed
{
    // Coefficients : 2's complement 9.25
    logic [`RBOX_LT_EDGE_COEF_PAD]    b_reserved;
    logic [`RBOX_LT_EDGE_COEF_HI_RNG] b_coef_hi;
    logic [`RBOX_LT_EDGE_COEF_PAD]    a_reserved;
    logic [`RBOX_LT_EDGE_COEF_HI_RNG] a_coef_hi;
    logic [`RBOX_LT_EDGE_COEF_LO_RNG] b_coef_lo;
    logic [`RBOX_LT_EDGE_COEF_LO_RNG] a_coef_lo;
} rbox_edge_eq_large_t;

typedef struct packed
{
    logic [31:0] b_coef;    // UNORM24 or FLOAT32
    logic [31:0] a_coef;    // UNORM24 or FLOAT32
} rbox_depth_eq_t;

typedef struct packed
{
    logic [`RBOX_64T_EDGE_SAMP_PAD] edge_reserved;
    logic [`RBOX_64T_EDGE_SAMP_RNG] edge_sample;     // 2's complement 15.14
} rbox_edge_sample_64x64_t;

typedef struct packed
{
    logic [`RBOX_128T_EDGE_SAMP_PAD] edge_reserved;
    logic [`RBOX_128T_EDGE_SAMP_RNG] edge_sample;     // 2's complement 15.15
} rbox_edge_sample_128x128_t;

typedef struct packed
{
    logic [`RBOX_LT_EDGE_SAMP_PAD] edge_reserved;
    logic [`RBOX_LT_EDGE_SAMP_RNG] edge_sample;     // 2's complement 15.25
} rbox_edge_sample_large_t;

typedef struct packed
{
    logic [7:0]       ref_value;
    logic [7:0]       write_mask;
    logic [7:0]       compare_mask;
    rbox_comp_op_t    compare_op;
    rbox_stencil_op_t depth_fail_op;
    rbox_stencil_op_t pass_op;
    rbox_stencil_op_t fail_op;
} rbox_stencil_state_t; 


typedef struct packed
{
    logic [63:0]                   reserved1;
    logic [31:0]                   depth_sample;
    rbox_edge_sample_64x64_t [2:0] edge_sample;
    rbox_tile_size_t               tile_size;
    rbox_tile_pos_t                tile_top;
    rbox_tile_pos_t                tile_left;
    rbox_inpck_type_t              inpck_type;
} rbox_inpckt_fully_covered_tile_t;

typedef struct packed
{
    rbox_edge_sample_large_t [2:0] edge_sample;
    logic [31:0]                   depth_sample;
    rbox_tile_size_t               tile_size;
    rbox_tile_pos_t                tile_top;
    rbox_tile_pos_t                tile_left;
    rbox_inpck_type_t              inpck_type;
} rbox_inpck_large_triangle_tile_t;

typedef struct packed
{
    logic [31:0]                   depth_sample;
    rbox_edge_sample_64x64_t [2:0] edge_sample;
    logic [63:0]                   triangle_data_addr;
    rbox_depth_eq_t                depth_eq;
    rbox_edge_eq_64x64_t [2:0]     edge_eq;
    logic [30:0]                   reserved0;
    rbox_tile_size_t               tile_size;
    rbox_tri_facing_t              tri_facing;
    rbox_tile_pos_t                tile_top;
    rbox_tile_pos_t                tile_left;
    rbox_inpck_type_t              inpck_type;
} rbox_inpck_triangle_with_tile_64x64_t;

typedef struct packed
{
    logic [31:0]                     depth_sample;
    rbox_edge_sample_128x128_t [2:0] edge_sample;
    logic [63:0]                     triangle_data_addr;
    rbox_depth_eq_t                  depth_eq;
    rbox_edge_eq_128x128_t [2:0]     edge_eq;
    logic [30:0]                     reserved0;
    rbox_tile_size_t                 tile_size;
    rbox_tri_facing_t                tri_facing;
    rbox_tile_pos_t                  tile_top;
    rbox_tile_pos_t                  tile_left;
    rbox_inpck_type_t                inpck_type;
} rbox_inpck_triangle_with_tile_128x128_t;

typedef struct packed
{
    logic [63:0]               triangle_data_addr;
    logic [31:0]               reserved1;
    rbox_depth_eq_t            depth_eq;
    rbox_edge_eq_large_t [2:0] edge_eq;
    logic [59:0]               reserved0;
    rbox_tri_facing_t          tri_facing;
    rbox_inpck_type_t          inpck_type;
} rbox_inpck_large_triangle_t;

typedef struct packed
{
    // QW7
    logic [27:0]         reserved4;
    rbox_stencil_state_t stencil_back_state;
    // QW6
    logic [27:0]         reserved3;
    rbox_stencil_state_t stencil_front_state;
    // QW5
    logic [7:0]          reserved2;
    logic [13:0]         scissor_width;       
    logic [13:0]         scissor_height;
    logic [13:0]         scissor_start_y;
    logic [13:0]         scissor_start_x;
    // QW4
    logic [31:0]         depth_max;
    logic [31:0]         depth_min;
    // QW3
    logic [31:0]         depth_bound_max;
    logic [31:0]         depth_bound_min;
    // QW2
    logic [18:0]         reserved1;
    logic [1:0]          shire_layout_height;
    logic [1:0]          shire_layout_width;
    logic [1:0]          hart_tile_height;
    logic [1:0]          hart_tile_width;
    logic                fragment_shader_per_sample;
    logic                fragment_shader_reads_coverage;
    logic                fragment_shader_reads_depth;
    logic                fragment_shader_reads_bary;
    logic                fragment_shader_disabled;
    logic                stencil_test_enable;
    logic                early_frag_tests_enable;
    rbox_comp_op_t       depth_test_compare_op;
    logic                depth_test_write_enable;
    logic                depth_test_enable;
    logic                depth_bound_enable;
    logic                depth_clamp_enable;
    logic [12:0]         ds_buffer_row_pitch;
    logic                ds_buffer_tiled;
    rbox_ds_format_t     ds_buffer_format;
    // QW1
    logic [63:0]         ds_buffer_addr;
    // QW0
    logic [31:0]         msaa_min_sample_shading;
    logic [15:0]         msaa_sample_mask;
    logic                msaa_alpha_to_one;
    logic                msaa_alpha_to_coverage;
    logic                msaa_shading_enable;
    logic [3:0]          msaa_samples;
    logic                msaa_enable;
} rbox_state_pck_t;

typedef struct packed
{
    rbox_state_pck_t  state;
    logic [4:0]       reserved0;
    rbox_inpck_type_t inpck_type;
} rbox_inpck_rbox_state_t;

typedef struct packed
{
    // QW0
    logic [63:0]      reserved1;    
    logic [63:0]      fshader_data_addr;
    logic [63:0]      fshader_function_addr;
    logic [60:0]      reserved0;
    rbox_inpck_type_t inpck_type;
} rbox_inpck_fsh_state_t;

typedef struct packed
{
    logic [63:0]       reserved1;
    logic [63:0]       fshader_data_addr;
    logic [63:0]       fshader_function_addr;
    logic [61:0]       reserved0;
    rbox_outpck_type_t outpck_type;
} rbox_outpck_fsh_state_t;

typedef struct packed
{
    logic [63:0]                            quad1_triangle_ptr;
    logic [63:0]                            quad0_triangle_ptr;
    logic [31:0]                            reserved6;
    logic [7:0]                             mask;
    logic [7:$bits(rbox_msaa_sample_idx_t)] reserved5;
    rbox_msaa_sample_idx_t                  sample_idx;
    logic [15:`RBOX_MAX_RENDER_DIM_L]       reserved4;
    logic [`RBOX_MAX_RENDER_DIM_R]          quad1_y;
    logic [15:`RBOX_MAX_RENDER_DIM_L]       reserved3;
    logic [`RBOX_MAX_RENDER_DIM_R]          quad0_y;
    logic [15:`RBOX_MAX_RENDER_DIM_L]       reserved2;
    logic [`RBOX_MAX_RENDER_DIM_R]          quad1_x;
    logic [15:`RBOX_MAX_RENDER_DIM_L]       reserved1;
    logic [`RBOX_MAX_RENDER_DIM_R]          quad0_x;
    logic [15:$bits(rbox_outpck_type_t)]    reserved0;
    rbox_outpck_type_t outpck_type;
} rbox_outpck_quad_info_t;

typedef struct packed
{
    logic [253:0]      reserved0;
    rbox_outpck_type_t outpck_type;
} rbox_outpck_end_phase_t;

typedef struct packed
{
    logic                msaa_enable;
    logic [3:0]          msaa_samples;
    logic                msaa_shading_enable;
    logic                msaa_alpha_to_coverage;
    logic                msaa_alpha_to_one;
    logic [15:0]         msaa_sample_mask;
    logic [31:0]         msaa_min_sample_shading;
    logic [63:0]         ds_buffer_addr;
    rbox_ds_format_t     ds_buffer_format;
    logic                ds_buffer_tiled;
    logic [12:0]         ds_buffer_row_pitch;
    logic                depth_clamp_enable;
    logic                depth_bound_enable;
    logic                depth_test_enable;
    logic                depth_test_write_enable;
    rbox_comp_op_t       depth_test_compare_op;
    logic                early_frag_tests_enable;
    logic                stencil_test_enable;
    logic                fragment_shader_disabled;
    logic                fragment_shader_reads_bary;
    logic                fragment_shader_reads_depth;
    logic                fragment_shader_reads_coverage;
    logic                fragment_shader_per_sample;
    logic [1:0]          hart_tile_width;
    logic [1:0]          hart_tile_height;
    logic [1:0]          shire_layout_width;
    logic [1:0]          shire_layout_height;
    logic [31:0]         depth_bound_min;
    logic [31:0]         depth_bound_max;
    logic [31:0]         depth_min;
    logic [31:0]         depth_max;
    logic [13:0]         scissor_start_x;
    logic [13:0]         scissor_start_y;
    logic [13:0]         scissor_height;
    logic [13:0]         scissor_width;       
    rbox_stencil_state_t stencil_front_state;
    rbox_stencil_state_t stencil_back_state;
} rbox_state_t;

typedef struct packed
{
    logic [63:0] fshader_function_addr;
    logic [63:0] fshader_data_addr;
} rbox_fsh_state_t;

typedef struct packed
{
    rbox_tri_facing_t      tri_facing;
    rbox_edge_coef_t [2:0] edge_eq_a;
    rbox_edge_coef_t [2:0] edge_eq_b;
    logic [2:0]            top_or_left_edge;
    rbox_depth_coef_t      depth_eq_a;
    rbox_depth_coef_t      depth_eq_b;
    logic [63:0]           triangle_data_addr;
} rbox_triangle_state_t;

typedef struct packed
{
    rbox_tile_size_t                 size;
    logic [`RBOX_MAX_RENDER_DIM_L:2] left;
    logic [`RBOX_MAX_RENDER_DIM_L:2] top;
    logic                            fully_covered;
    rbox_edge_sample_t [2:0]         edge_sample;
    rbox_depth_sample_t              depth_sample;
} rbox_tile_state_t;

typedef struct packed
{
    logic [`RBOX_MAX_TILE_DIM_R]         x;
    logic [`RBOX_MAX_TILE_DIM_R]         y;
    logic [3:0]                          mask;
    logic [3:0][31:0]                    i;
    logic [3:0][31:0]                    j;
    logic [3:0][31:0]                    depth;
    rbox_msaa_sample_idx_t               sample_idx;
    logic [3:0][`RBOX_MAX_MSAA_SAMP_RNG] msaa_coverage;
} rbox_quad_data_t;


typedef struct packed
{
    logic [`RBOX_MAX_TILE_DIM_R] x;
    logic [`RBOX_MAX_TILE_DIM_R] y;
    rbox_msaa_sample_idx_t       sample_idx;
    logic [3:0]                  mask;
    logic                        valid;
    logic                        test_done;
    logic [`RBOX_HART_I_R]       hart_id;
    logic                        start_pack;      // Force quad to start a new pair of quads for output.
} rbox_quad_info_t;

typedef struct packed
{
    logic [`RBOX_MAX_RENDER_DIM_R] x;
    logic [`RBOX_MAX_RENDER_DIM_R] y;
    rbox_msaa_sample_idx_t         sample_idx;
    logic [3:0]                    mask;
    logic [3:0][31:0]              depth;
} rbox_quad_test_t;

typedef struct packed
{
    logic [3:0]                 mask;           // Coverage mask
    logic [`RBOX_DEPTH_BUF_I_R] depth_buf_ptr;  // Pointer to the depth buffer line with the quad depth data
    logic [5:0]                 quad_offset;    // Quad offset inside the depth data (2x2 quads for 32-bit formats, 2x4 quads for 16-bit formats)
    logic                       valid;
} rbox_depth_lfifo_info_t;

typedef struct packed
{
    logic        infinite;
    logic        nan;
    logic        zero;
    logic        subnorm;
    logic [7:0]  e;         // exponent ([-126, 126] in excess 127, 0 and 0xff as special codes)
    logic        s;         // sign
    logic [23:0] m;         // mantissa (unsigned 1.23 magnitude)
    logic [5:0]  e_delta;   // exponent delta for renormalization ([-24, +1])
    logic [4:0]  m_shift;   // mantissa right shift for renormalization ([0, 24])
} rbox_internal_depth_t;

typedef struct packed
{
    logic [63:1] reserved;
    logic        enable;
} rbox_esr_config_t;

typedef struct packed 
{
    logic                             page_active1;
    logic [62:(32+`RBOX_2M_PG_PA_SZ)] reserved1;
    logic [`RBOX_2M_PG_PA_RNG]        page1;
    logic                             page_active0;
    logic [30:`RBOX_2M_PG_PA_SZ]      reserved0;
    logic [`RBOX_2M_PG_PA_RNG]        page0;
} rbox_esr_in_buf_pg_t;

typedef struct packed
{
    logic [31:`RBOX_IN_BUF_LINE_SZ] reserved1;
    logic [`RBOX_IN_BUF_LINE_RNG]   line_count;
    logic [31:`RBOX_IN_BUF_LINE_SZ] reserved0;
    logic [`RBOX_IN_BUF_LINE_RNG]   start_offset; 
} rbox_esr_in_buf_cfg_t;

typedef struct packed
{
    logic                        page_active;
    logic [30:`RBOX_2M_PG_PA_SZ] reserved;
    logic [`RBOX_2M_PG_PA_RNG]   page;
} rbox_esr_out_buf_pg_t;


typedef struct packed
{
    logic [31:`RBOX_ESR_OUT_BUF_CFG_DW1_USED_BITS] reserved1;
    logic [`RBOX_MAX_PCKT_MSG_L_R]                 max_pckts_msg;
    logic [`RBOX_MAX_HART_MSG_R]                   hart_max_msg;
    logic [`CORE_NR_MSG_PORTS_RANGE]               hart_port_id;
    logic [`RBOX_HART_BUF_SZ_R]                    hart_buf_lines;
    logic [31:`RBOX_OUT_BUF_LINE_SZ]               reserved0;
    logic [`RBOX_OUT_BUF_LINE_RNG]                 start_offset;
} rbox_esr_out_buf_cfg_t;

typedef struct packed
{
    logic [15:1] reserved1;
    logic        flush;
    logic [15:1] reserved0;
    logic        start;
} rbox_esr_start_t;

typedef struct packed
{
    logic [15:`RBOX_HARTS_L]                  reserved1;
    logic [`RBOX_HART_I_R]                    hart_id;
    logic [15:`RBOX_ESR_CONSUME_W0_USED_BITS] reserved0;
    logic [`RBOX_MAX_HART_MSG_R]              msg_credits;
    logic [`RBOX_HART_MSG_CREDIT_R]           pckt_credits;
} rbox_esr_consume_t;

typedef struct packed
{
    logic [31:$bits(rbox_status_t)] reserved;
    rbox_status_t                   status;
} rbox_esr_status_t;

typedef struct packed
{
    logic                         s1_config_error;
    logic                         s1_started;
    logic                         s1_done;
    logic                         s1_sc_rbox_rsp_is_read_data;
    logic [`RBOX_IN_BUF_LINE_RNG] s1_in_pending_lines_ff;
    logic [`RBOX_IN_BUF_LINE_RNG] s1_in_process_pckts_ff;
    logic [`RBOX_IN_BUF_LINE_RNG] s1_in_buf_off;
    logic [(`SC_BANKS+1)-1:0]     s1_in_sc_req_valid_ff;
    logic [`PA_RANGE]             s1_req_pa;
    logic                         s1_in_fifo_full;
    logic                         s1_in_fifo_empty;
    logic                         s1_in_fifo_alloc;
    logic [`RBOX_FIFO_PTR_RNG]    s1_in_fifo_alloc_ptr;
    logic [`RBOX_FIFO_PTR_RNG]    s1_in_fifo_push_ptr;
    logic                         s1_in_fifo_push;
    logic                         s1_in_fifo_push_done;

    logic                         s2_load_rbox_state;
    logic                         s2_load_fsh_state;
    logic                         s2_load_triangle_state;
    logic                         s2_load_tile_state;
    logic                         s2_input_pck_accept;

    logic                         s3_end_of_buffer_ff;
    logic                         s3_tile_valid_ff;

    logic                         s4_out_buf_info_valid;
    logic                         s4_out_buf_packet_valid;
    rbox_quad_info_t [1:0]        s4_out_buf_info;
    logic [`RBOX_HART_I_R]        s4_hart_id;
    logic [(`SC_BANKS+1)-1:0]     s4_out_msg_sc_req_valid_ff;
    logic [(`SC_BANKS+1)-1:0]     s4_out_pck_sc_req_valid_ff;
    logic [`PA_RANGE]             s4_out_msg_req_pa;
    logic [`PA_RANGE]             s4_out_pck_req_pa;
} rbox_sm_ctrl_t;


////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

function automatic logic [`RBOX_MAX_TILE_DIM_R] get_quads_in_tile_x_f;
    input rbox_tile_size_t tile_sz;
    case (tile_sz)
        RBOX_TILE_SIZE_64X64,
        RBOX_TILE_SIZE_64X32 : get_quads_in_tile_x_f = 'd31;
        RBOX_TILE_SIZE_32X32 : get_quads_in_tile_x_f = 'd15;
        RBOX_TILE_SIZE_16X16 : get_quads_in_tile_x_f = 'd7;
        RBOX_TILE_SIZE_8X8   : get_quads_in_tile_x_f = 'd3;
        RBOX_TILE_SIZE_4X4   : get_quads_in_tile_x_f = 'd1;
        default              : get_quads_in_tile_x_f = 'd0;
    endcase
endfunction

function automatic logic [`RBOX_MAX_TILE_DIM_R] get_quads_in_tile_y_f;
    input rbox_tile_size_t tile_sz;
    case (tile_sz)
        RBOX_TILE_SIZE_64X64 : get_quads_in_tile_y_f = 'd31;
        RBOX_TILE_SIZE_64X32,
        RBOX_TILE_SIZE_32X32 : get_quads_in_tile_y_f = 'd15;
        RBOX_TILE_SIZE_16X16 : get_quads_in_tile_y_f = 'd7;
        RBOX_TILE_SIZE_8X8   : get_quads_in_tile_y_f = 'd3;
        RBOX_TILE_SIZE_4X4   : get_quads_in_tile_y_f = 'd1;
        default              : get_quads_in_tile_y_f = 'd0;
    endcase
endfunction

function automatic logic [`RBOX_MAX_TILE_DIM_R] get_tile_quad_offset_f;
    input logic [13:2] tile_pos;
    get_tile_quad_offset_f = {tile_pos[2 +: (`RBOX_MAX_TILE_DIM_L-2)], 2'b0};
endfunction


function automatic logic f_msaa_mode_is_valid;
    input logic [3:0] msaa_mode;
    case (msaa_mode)
        4'd1, 4'd3, 4'd7, 4'd15 : f_msaa_mode_is_valid = 1'b1;
        default                 : f_msaa_mode_is_valid = 1'b0;
    endcase
endfunction

function automatic rbox_msaa_mode_t f_convert_msaa_mode;
    input logic [3:0] msaa_mode;
    case (msaa_mode)
        4'd1    : f_convert_msaa_mode = RBOX_MSAA_MODE_2X;
        4'd3    : f_convert_msaa_mode = RBOX_MSAA_MODE_4X;
        4'd7    : f_convert_msaa_mode = RBOX_MSAA_MODE_8X;
        4'd15   : f_convert_msaa_mode = RBOX_MSAA_MODE_16X;
        default : f_convert_msaa_mode = RBOX_MSAA_MODE_2X;
    endcase
endfunction


function automatic logic [1:0] rbox_inpck_sz_f;
    input rbox_inpck_type_t inpck_type;
    case (inpck_type)
        RBOX_INPCK_TRIANGLE_WITH_TILE_64X64   : rbox_inpck_sz_f = 'd2;
        RBOX_INPCK_TRIANGLE_WITH_TILE_128X128 : rbox_inpck_sz_f = 'd2;
        RBOX_INPCK_LARGE_TRIANGLE             : rbox_inpck_sz_f = 'd2;
        RBOX_INPCK_FULLY_COVERED_TILE         : rbox_inpck_sz_f = 'd1;
        RBOX_INPCK_LARGE_TRIANGLE_TILE        : rbox_inpck_sz_f = 'd1;
        RBOX_INPCK_RBOX_STATE                 : rbox_inpck_sz_f = 'd2;
        RBOX_INPCK_FRAG_SHADING_STATE         : rbox_inpck_sz_f = 'd1;
        RBOX_INPCK_END_OF_BUFFER              : rbox_inpck_sz_f = 'd1;
    endcase
endfunction


function automatic rbox_state_t rbox_state_from_inpck_f;
    input rbox_state_pck_t inpck;

    rbox_state_from_inpck_f = '0;
    rbox_state_from_inpck_f.msaa_enable                    = inpck.msaa_enable                   ; 
    rbox_state_from_inpck_f.msaa_samples                   = inpck.msaa_samples                  ;
    rbox_state_from_inpck_f.msaa_shading_enable            = inpck.msaa_shading_enable           ;
    rbox_state_from_inpck_f.msaa_alpha_to_coverage         = inpck.msaa_alpha_to_coverage        ;
    rbox_state_from_inpck_f.msaa_alpha_to_one              = inpck.msaa_alpha_to_one             ;
    rbox_state_from_inpck_f.msaa_sample_mask               = inpck.msaa_sample_mask              ;
    rbox_state_from_inpck_f.msaa_min_sample_shading        = inpck.msaa_min_sample_shading       ;
    rbox_state_from_inpck_f.ds_buffer_addr                 = inpck.ds_buffer_addr                ;
    rbox_state_from_inpck_f.ds_buffer_format               = inpck.ds_buffer_format              ;
    rbox_state_from_inpck_f.ds_buffer_tiled                = inpck.ds_buffer_tiled               ;
    rbox_state_from_inpck_f.ds_buffer_row_pitch            = inpck.ds_buffer_row_pitch           ;
    rbox_state_from_inpck_f.depth_clamp_enable             = inpck.depth_clamp_enable            ;
    rbox_state_from_inpck_f.depth_bound_enable             = inpck.depth_bound_enable            ;
    rbox_state_from_inpck_f.depth_test_enable              = inpck.depth_test_enable             ;
    rbox_state_from_inpck_f.depth_test_write_enable        = inpck.depth_test_write_enable       ;
    rbox_state_from_inpck_f.depth_test_compare_op          = inpck.depth_test_compare_op         ;
    rbox_state_from_inpck_f.early_frag_tests_enable        = inpck.early_frag_tests_enable       ;
    rbox_state_from_inpck_f.stencil_test_enable            = inpck.stencil_test_enable           ;
    rbox_state_from_inpck_f.fragment_shader_disabled       = inpck.fragment_shader_disabled      ;
    rbox_state_from_inpck_f.fragment_shader_reads_bary     = inpck.fragment_shader_reads_bary    ;
    rbox_state_from_inpck_f.fragment_shader_reads_depth    = inpck.fragment_shader_reads_depth   ;
    rbox_state_from_inpck_f.fragment_shader_reads_coverage = inpck.fragment_shader_reads_coverage;
    rbox_state_from_inpck_f.fragment_shader_per_sample     = inpck.fragment_shader_per_sample    ;
    rbox_state_from_inpck_f.hart_tile_width                = inpck.hart_tile_width               ;
    rbox_state_from_inpck_f.hart_tile_height               = inpck.hart_tile_height              ;
    rbox_state_from_inpck_f.shire_layout_width             = inpck.shire_layout_width            ;
    rbox_state_from_inpck_f.shire_layout_height            = inpck.shire_layout_height           ;
    rbox_state_from_inpck_f.depth_bound_min                = inpck.depth_bound_min               ;
    rbox_state_from_inpck_f.depth_bound_max                = inpck.depth_bound_max               ;
    rbox_state_from_inpck_f.depth_min                      = inpck.depth_min                     ;
    rbox_state_from_inpck_f.depth_max                      = inpck.depth_max                     ;
    rbox_state_from_inpck_f.scissor_start_x                = inpck.scissor_start_x               ;
    rbox_state_from_inpck_f.scissor_start_y                = inpck.scissor_start_y               ;
    rbox_state_from_inpck_f.scissor_height                 = inpck.scissor_height                ;
    rbox_state_from_inpck_f.scissor_width                  = inpck.scissor_width                 ;
    rbox_state_from_inpck_f.stencil_front_state            = inpck.stencil_front_state           ;
    rbox_state_from_inpck_f.stencil_back_state             = inpck.stencil_back_state            ;
endfunction


function automatic rbox_fsh_state_t rbox_fsh_state_from_inpck_f;
    input rbox_inpck_fsh_state_t inpck;
    rbox_fsh_state_from_inpck_f.fshader_function_addr = inpck.fshader_function_addr; 
    rbox_fsh_state_from_inpck_f.fshader_data_addr     = inpck.fshader_data_addr;
endfunction

function automatic rbox_edge_coef_t rbox_lt_edge_coef_f;
    input rbox_edge_eq_large_t     in_edge_eq;
    input logic                    coef_idx;
    logic [`RBOX_LT_EDGE_COEF_RNG] in_coef;
    in_coef = coef_idx ? {in_edge_eq.b_coef_hi, in_edge_eq.b_coef_lo}
                       : {in_edge_eq.a_coef_hi, in_edge_eq.a_coef_lo};
    rbox_lt_edge_coef_f = `SX($bits(rbox_edge_coef_t), in_coef) << (`RBOX_EDGE_COEF_FRC_SZ - `RBOX_LT_EDGE_COEF_FRC_BITS);
endfunction

function automatic logic rbox_is_top_or_left_edge_f;
    input rbox_edge_coef_t a;
    input rbox_edge_coef_t b;
    rbox_is_top_or_left_edge_f = (a == 0) ? b[`RBOX_EDGE_COEF_SZ-1] : ~a[`RBOX_EDGE_COEF_SZ-1];
endfunction

function automatic rbox_triangle_state_t rbox_lt_triangle_state_f;
    input rbox_inpck_large_triangle_t inpck;
    rbox_lt_triangle_state_f.tri_facing         = inpck.tri_facing;
    rbox_lt_triangle_state_f.triangle_data_addr = inpck.triangle_data_addr;
    for (integer e = 0; e < 3; e++)
    begin
        rbox_lt_triangle_state_f.edge_eq_a[e] = rbox_lt_edge_coef_f(inpck.edge_eq[e], 0);
        rbox_lt_triangle_state_f.edge_eq_b[e] = rbox_lt_edge_coef_f(inpck.edge_eq[e], 1);
        rbox_lt_triangle_state_f.top_or_left_edge[e] = rbox_is_top_or_left_edge_f(rbox_lt_triangle_state_f.edge_eq_a[e],
                                                                                  rbox_lt_triangle_state_f.edge_eq_b[e]);
    end
    rbox_lt_triangle_state_f.depth_eq_a = inpck.depth_eq.a_coef;
    rbox_lt_triangle_state_f.depth_eq_b = inpck.depth_eq.b_coef;
endfunction

function automatic rbox_edge_coef_t rbox_128t_edge_coef_f;
    input rbox_edge_eq_128x128_t     in_edge_eq;
    input logic                      coef_idx;
    logic [`RBOX_128T_EDGE_COEF_RNG] in_coef;
    in_coef = coef_idx ? in_edge_eq.b_coef : in_edge_eq.a_coef;
    rbox_128t_edge_coef_f = `SX($bits(rbox_edge_coef_t), in_coef) << (`RBOX_EDGE_COEF_FRC_SZ - `RBOX_128T_EDGE_COEF_FRC_BITS);
endfunction

function automatic rbox_triangle_state_t rbox_128t_triangle_state_f;
    input rbox_inpck_triangle_with_tile_128x128_t inpck;
    rbox_128t_triangle_state_f.tri_facing         = inpck.tri_facing;
    rbox_128t_triangle_state_f.triangle_data_addr = inpck.triangle_data_addr;
    for (integer e = 0; e < 3; e++)
    begin
        rbox_128t_triangle_state_f.edge_eq_a[e] = rbox_128t_edge_coef_f(inpck.edge_eq[e], 0);
        rbox_128t_triangle_state_f.edge_eq_b[e] = rbox_128t_edge_coef_f(inpck.edge_eq[e], 1);
        rbox_128t_triangle_state_f.top_or_left_edge[e] = rbox_is_top_or_left_edge_f(rbox_128t_triangle_state_f.edge_eq_a[e],
                                                                                    rbox_128t_triangle_state_f.edge_eq_b[e]);
    end
    rbox_128t_triangle_state_f.depth_eq_a = inpck.depth_eq.a_coef;
    rbox_128t_triangle_state_f.depth_eq_b = inpck.depth_eq.b_coef;
endfunction

function automatic rbox_edge_coef_t rbox_64t_edge_coef_f;
    input rbox_edge_eq_64x64_t      in_edge_eq;
    input logic                     coef_idx;
    logic [`RBOX_64T_EDGE_COEF_RNG] in_coef;
    in_coef = coef_idx ? in_edge_eq.b_coef : in_edge_eq.a_coef;
    rbox_64t_edge_coef_f = `SX($bits(rbox_edge_coef_t), in_coef) << (`RBOX_EDGE_COEF_FRC_SZ - `RBOX_64T_EDGE_COEF_FRC_BITS);
endfunction

function automatic rbox_triangle_state_t rbox_64t_triangle_state_f;
    input rbox_inpck_triangle_with_tile_64x64_t inpck;
    rbox_64t_triangle_state_f.tri_facing         = inpck.tri_facing;
    rbox_64t_triangle_state_f.triangle_data_addr = inpck.triangle_data_addr;
    for (integer e = 0; e < 3; e++)
    begin
        rbox_64t_triangle_state_f.edge_eq_a[e] = rbox_64t_edge_coef_f(inpck.edge_eq[e], 0);
        rbox_64t_triangle_state_f.edge_eq_b[e] = rbox_64t_edge_coef_f(inpck.edge_eq[e], 1);
        rbox_64t_triangle_state_f.top_or_left_edge[e] = rbox_is_top_or_left_edge_f(rbox_64t_triangle_state_f.edge_eq_a[e],
                                                                                   rbox_64t_triangle_state_f.edge_eq_b[e]);
    end
    rbox_64t_triangle_state_f.depth_eq_a = inpck.depth_eq.a_coef;
    rbox_64t_triangle_state_f.depth_eq_b = inpck.depth_eq.b_coef;
endfunction


function automatic rbox_edge_sample_t rbox_ltt_edge_sample_f;
    input rbox_edge_sample_large_t in_sample;
    rbox_ltt_edge_sample_f = `SX($bits(rbox_edge_sample_t), in_sample.edge_sample) << (`RBOX_EDGE_SAMP_FRC_BITS - `RBOX_LT_EDGE_SAMP_FRC_BITS);
endfunction

function automatic rbox_tile_state_t rbox_lt_tile_state_f;
    input rbox_inpck_large_triangle_tile_t inpck;
    rbox_lt_tile_state_f.size          = inpck.tile_size;
    rbox_lt_tile_state_f.left          = inpck.tile_left;
    rbox_lt_tile_state_f.top           = inpck.tile_top;
    rbox_lt_tile_state_f.fully_covered = 1'b0;
    for (integer e = 0; e < 3; e++)
        rbox_lt_tile_state_f.edge_sample[e] = rbox_ltt_edge_sample_f(inpck.edge_sample[e]);
    rbox_lt_tile_state_f.depth_sample = inpck.depth_sample;
endfunction

function automatic rbox_edge_sample_t rbox_128t_edge_sample_f;
    input rbox_edge_sample_128x128_t in_sample;
    rbox_128t_edge_sample_f = `SX($bits(rbox_edge_sample_t), in_sample.edge_sample) << (`RBOX_EDGE_SAMP_FRC_BITS - `RBOX_128T_EDGE_SAMP_FRC_BITS);
endfunction

function automatic rbox_tile_state_t rbox_128t_tile_state_f;
    input rbox_inpck_triangle_with_tile_128x128_t inpck;
    rbox_128t_tile_state_f.size          = inpck.tile_size;
    rbox_128t_tile_state_f.left          = inpck.tile_left;
    rbox_128t_tile_state_f.top           = inpck.tile_top;
    rbox_128t_tile_state_f.fully_covered = 1'b0;
    for (integer e = 0; e < 3; e++)
        rbox_128t_tile_state_f.edge_sample[e] = rbox_128t_edge_sample_f(inpck.edge_sample[e]);
    rbox_128t_tile_state_f.depth_sample = inpck.depth_sample;
endfunction

function automatic rbox_edge_sample_t rbox_64t_edge_sample_f;
    input rbox_edge_sample_64x64_t in_sample;
    rbox_64t_edge_sample_f = `SX($bits(rbox_edge_sample_t), in_sample.edge_sample) << (`RBOX_EDGE_SAMP_FRC_BITS - `RBOX_64T_EDGE_SAMP_FRC_BITS);
endfunction

function automatic rbox_tile_state_t rbox_64t_tile_state_f;
    input rbox_inpck_triangle_with_tile_64x64_t inpck;
    rbox_64t_tile_state_f.size          = inpck.tile_size;
    rbox_64t_tile_state_f.left          = inpck.tile_left;
    rbox_64t_tile_state_f.top           = inpck.tile_top;
    rbox_64t_tile_state_f.fully_covered = 1'b0;
    for (integer e = 0; e < 3; e++)
        rbox_64t_tile_state_f.edge_sample[e] = rbox_64t_edge_sample_f(inpck.edge_sample[e]);
    rbox_64t_tile_state_f.depth_sample = inpck.depth_sample;
endfunction

function automatic rbox_tile_state_t rbox_fct_tile_state_f;
    input rbox_inpckt_fully_covered_tile_t inpck;
    rbox_fct_tile_state_f.size          = inpck.tile_size;
    rbox_fct_tile_state_f.left          = inpck.tile_left;
    rbox_fct_tile_state_f.top           = inpck.tile_top;
    rbox_fct_tile_state_f.fully_covered = 1'b1;
    for (integer e = 0; e < 3; e++)
        rbox_fct_tile_state_f.edge_sample[e] = rbox_64t_edge_sample_f(inpck.edge_sample[e]);
    rbox_fct_tile_state_f.depth_sample = inpck.depth_sample;
endfunction

function automatic rbox_internal_depth_t f32_to_depth_internal_f;
    input logic [31:0] in;
          logic        exp_special;
          logic        exp_zero;
          logic        mant_zero;
          logic        denorm;
    exp_special = (in[30:23] == '1);
    exp_zero    = (in[30:23] == '0);
    mant_zero   = (in[22:0]  == '0);
    f32_to_depth_internal_f.infinite = exp_special &&  mant_zero;
    f32_to_depth_internal_f.nan      = exp_special && !mant_zero;
    f32_to_depth_internal_f.zero     = exp_zero    &&  mant_zero;
    f32_to_depth_internal_f.subnorm  = exp_zero    && !mant_zero;
    f32_to_depth_internal_f.e        = in[30:23];
    f32_to_depth_internal_f.s        = in[31];
    f32_to_depth_internal_f.m        = {!exp_zero, in[22:0]};
    f32_to_depth_internal_f.e_delta  = '0;
    f32_to_depth_internal_f.m_shift  = '0;
endfunction

function automatic logic rbox_sample_inside_triangle_f;
    input rbox_edge_sample_t [2:0] edge_sample;
    input logic              [2:0] edge_is_top_or_left;
    rbox_sample_inside_triangle_f = 1'b1;
    for (integer e = 0; e < 3; e++)
    begin
        rbox_sample_inside_triangle_f = rbox_sample_inside_triangle_f &&
                                        (  (~edge_sample[e][`RBOX_EDGE_SAMP_BITS-1] && (edge_sample[e][`RBOX_EDGE_SAMP_BITS-2:0] != 0)) ||
                                           ((edge_sample[e] == 0) && edge_is_top_or_left[e]));
    end
endfunction


function automatic logic [`RBOX_HART_I_R] f_quad_to_hart;
    input logic [1:0]                  hart_tile_width;
    input logic [1:0]                  hart_tile_height;
    input logic [1:0]                  shire_layout_width;
    input logic [1:0]                  shire_layout_height;
    input logic [`RBOX_MAX_TILE_DIM_R] quad_x;
    input logic [`RBOX_MAX_TILE_DIM_R] quad_y;

    logic [`RBOX_MAX_TILE_DIM_R]       quad_hart_x;
    logic [`RBOX_MAX_TILE_DIM_R]       quad_hart_y;

    // Note : HART Tile dimensions are in quads.
    quad_hart_x = quad_x >> (hart_tile_width  + 1);
    quad_hart_y = quad_y >> (hart_tile_height + 1);

    case ({shire_layout_height, shire_layout_width})
        {2'd0, 2'd0} : f_quad_to_hart = '0;                                                       //  1 HART  per Shire
        {2'd0, 2'd1} : f_quad_to_hart = (`RBOX_HARTS_L)'(                   quad_hart_x[0]   );   //  2 HARTs per Shire
        {2'd0, 2'd2} : f_quad_to_hart = (`RBOX_HARTS_L)'(                   quad_hart_x[1:0] );   //  4 HARTs per Shire
        {2'd0, 2'd3} : f_quad_to_hart = (`RBOX_HARTS_L)'(                   quad_hart_x[2:0] );   //  8 HARTs per Shire

        {2'd1, 2'd0} : f_quad_to_hart = (`RBOX_HARTS_L)'( quad_hart_y[0]                     );   //  2 HARTs per Shire
        {2'd1, 2'd1} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[0],   quad_hart_x[0]  });   //  4 HARTs per Shire
        {2'd1, 2'd2} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[0],   quad_hart_x[1:0]});   //  8 HARTs per Shire
        {2'd1, 2'd3} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[0],   quad_hart_x[2:0]});   // 16 HARTs per Shire

        {2'd2, 2'd0} : f_quad_to_hart = (`RBOX_HARTS_L)'( quad_hart_y[1:0]                   );   //  4 HARTs per Shire
        {2'd2, 2'd1} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[1:0], quad_hart_x[0]  });   //  8 HARTs per Shire
        {2'd2, 2'd2} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[1:0], quad_hart_x[1:0]});   // 16 HARTs per Shire
        {2'd2, 2'd3} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[1:0], quad_hart_x[2:0]});   // 32 HARTs per Shire

        {2'd3, 2'd0} : f_quad_to_hart = (`RBOX_HARTS_L)'( quad_hart_y[2:0]                   );   //  8 HARTs per Shire
        {2'd3, 2'd1} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[2:0], quad_hart_x[0]  });   // 16 HARTs per Shire
        {2'd3, 2'd2} : f_quad_to_hart = (`RBOX_HARTS_L)'({quad_hart_y[2:0], quad_hart_x[1:0]});   // 32 HARTs per Shire
        {2'd3, 2'd3} : f_quad_to_hart =                  {quad_hart_y[2:0], quad_hart_x[2:0]};    // 64 HARTs per Shire
        default      : f_quad_to_hart = '0;
    endcase   
endfunction


function automatic logic [`PA_RANGE] f_hart_msg_port_pa;
    input logic [`RBOX_HART_I_R]           hart_id;
    input logic [`CORE_NR_MSG_PORTS_RANGE] port_id;
    logic [`AD_ESR_PORT_MSG_SIZE-1:0] message_id;
    message_id = `AD_ESR_PORT_MSG_COMMON | (port_id << `AD_ESR_PORT_ID_START);
    f_hart_msg_port_pa = (`PA_SIZE)'({1'b1 , `AD_ESR_PROT_BITS_UMODE, `LOCAL_SHIRE_ID, `AD_ESR_AGENT_TYPE_MINION, 8'(hart_id), message_id});
endfunction

function automatic rbox_msaa_sample_pos_t get_msaa_sample_pos_f;
    input rbox_msaa_mode_t       msaa_mode;
    input rbox_msaa_sample_idx_t msaa_sample;

    case (msaa_mode)
        RBOX_MSAA_MODE_2X :
            begin
                case (msaa_sample)
                    'd0     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_2500; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_2500; end   // (0.25, 0.25)
                    'd1     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_7500; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_7500; end   // (0.75, 0.75)
                    default : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0000; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0000; end
                endcase
            end
        RBOX_MSAA_MODE_4X :
            begin
                case (msaa_sample)
                    'd0     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_3750; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_1250; end   // (0.375, 0.125)
                    'd1     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_8750; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_3750; end   // (0.875, 0.375)
                    'd2     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_1250; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_6250; end   // (0.125, 0.625)
                    'd3     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_6250; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_8750; end   // (0.625, 0.875)
                    default : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0000; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0000; end
                endcase
            end
        RBOX_MSAA_MODE_8X :
            begin
                case (msaa_sample)
                    'd0     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_5625; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_3125; end   // (0.5625, 0.3125)
                    'd1     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_4375; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_6875; end   // (0.4375, 0.6875)
                    'd2     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_8125; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_5625; end   // (0.8125, 0.5625)
                    'd3     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_3125; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_1875; end   // (0.3125, 0.1875)
                    'd4     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_1875; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_8125; end   // (0.1875, 0.8125)
                    'd5     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0625; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_4375; end   // (0.0625, 0.4375)
                    'd6     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_6875; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_9375; end   // (0.6875, 0.9375)
                    'd7     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_9375; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0625; end   // (0.9375, 0.0625)
                    default : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0000; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0000; end
                endcase
            end
        RBOX_MSAA_MODE_16X :
            begin
                case (msaa_sample)
                    'd0     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_5625; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_5625; end   // (0.5625, 0.5625)
                    'd1     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_4375; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_3125; end   // (0.4375, 0.3125)
                    'd2     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_3125; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_6250; end   // (0.3125, 0.625 )
                    'd3     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_7500; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_4375; end   // (0.75,   0.4375)
                    'd4     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_1875; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_3750; end   // (0.1875, 0.375 )
                    'd5     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_6250; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_8125; end   // (0.625,  0.8125)
                    'd6     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_8125; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_6875; end   // (0.8125, 0.6875)
                    'd7     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_6875; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_1875; end   // (0.6875, 0.1875)
                    'd8     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_3750; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_8750; end   // (0.375,  0.875 )
                    'd9     : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_5000; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0625; end   // (0.5,    0.0625)
                    'd10    : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_2500; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_1250; end   // (0.25,   0.125 )
                    'd11    : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_1250; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_7500; end   // (0.125,  0.75  )
                    'd12    : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0000; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_5000; end   // (0.0,    0.5   )
                    'd13    : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_9375; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_2500; end   // (0.9375, 0.25  )
                    'd14    : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_8750; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_9375; end   // (0.875,  0.9375)
                    'd15    : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0625; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0000; end   // (0.0625, 0.0   )
                    default : begin get_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0000; get_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0000; end
                endcase
            end
    endcase
endfunction

// The pixel offset is already added in triangle setup.
function automatic rbox_msaa_sample_pos_t get_non_msaa_sample_pos_f;
    get_non_msaa_sample_pos_f[0] = RBOX_MSAA_POS_0000;
    get_non_msaa_sample_pos_f[1] = RBOX_MSAA_POS_0000;
endfunction

