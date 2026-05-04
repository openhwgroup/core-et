// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// l2 reorder fifo
// NOTE: If FIFO depth changes, update definitions in
// l2_defines.vh, et_link_defines.vh and tbox_tb_defines.h accordingly

`define TEXSEND_FIFO_DATA_SIZE 11
`define TEXSEND_FIFO_ID_SIZE 5

`define TBOX_L2_FIFO_DEPTH          32
`define TBOX_L2_FIFO_DEPTH_IDX_SZ   $clog2(`TBOX_L2_FIFO_DEPTH)

`define TBOX_NEIGH_RSP_FIFO_DEPTH   2

`define TBOX_NEIGH_REQ_FIFO_DEPTH   2

// generic constants
localparam PORTS_PER_MINION = 4;
localparam PORT_ID_SZ = $clog2(PORTS_PER_MINION);

localparam MINIONS_PER_TBOX = 16;//32 FAIL // 8 minions, 2 threads each
localparam MINION_ID_SZ = $clog2(MINIONS_PER_TBOX);
localparam ADDRESS_POINTER_SZ = 64;
localparam NUM_TEXELS_CACHELINE_32BPT = 16;
localparam NUM_TEXELS_CACHELINE_64BPT = 8;
localparam NUM_TEXELS_CACHELINE_128BPT = 4;

localparam NR_COMPONENTS = 4;

// address input table constants

localparam ELEM_SIZE                          = 256;
localparam NUM_ELEMS_INPUT_BUFFER             = 16;
localparam NUM_ELEMS_INPUT_BUFFER_LOG         = $clog2(NUM_ELEMS_INPUT_BUFFER);

localparam MAX_NUM_INFLIGHT_TBPULL            = 8;
localparam MAX_NUM_INFLIGHT_TBPULL_LOG        = $clog2(MAX_NUM_INFLIGHT_TBPULL);

localparam MAX_ENTRIES_PER_SAMPLE_REQUEST     = 6;
localparam MAX_ENTRIES_PER_SAMPLE_REQUEST_LOG = $clog2(MAX_ENTRIES_PER_SAMPLE_REQUEST);

localparam NUM_ANISO_VALUES                   = 17; // 0..16
localparam ANISO_SZ                           = $clog2(NUM_ANISO_VALUES);
// image information table constants
localparam BASE_POINTER_SZ = ADDRESS_POINTER_SZ;
localparam IMAGE_FORMAT_BPT_SZ = 2;
localparam IMAGE_TYPE_SZ = 3;
localparam IMAGE_FORMAT_SZ = 9;
localparam TEXTURE_DIM_IJ_SZ = 16; /** bit width of dimensions i and j (width/height)*/
localparam TEXTURE_DIM_K_SZ = 15;  /** bit width for depth*/
localparam BASE_ARRAY_LAYER_SZ = 12;
localparam ARRAY_LAYER_COUNT_SZ = 12;
localparam BASE_MIP_LEVEL_SZ = 5;
localparam MIP_LEVEL_COUNT_SZ = 5;
localparam MAX_MIP_LEVEL_COUNT = 17;
localparam SWIZZLE_SZ = 3;
localparam COMPONENT_SWIZZLE_SZ = SWIZZLE_SZ * NR_COMPONENTS;
localparam ROW_PITCH_SZ = 14;
localparam MIP_PITCH_SZ = 5;
localparam MIP_COUNT_SCALE_SZ = 5;
localparam PACKED_LEVEL_SZ = 4;
localparam ELEMENT_PITCH_SZ = 30;
localparam IMGINFO_RESERVED0_SZ = 9;
localparam IMGINFO_RESERVED1_SZ = 4;
localparam IMGINFO_RESERVED2_SZ = 4;
localparam MEM_ENTRY_SZ = 256;
localparam MEM_ENTRY_SZ_BYTES = MEM_ENTRY_SZ / 8;
localparam TABLE_ENTRIES = 32;
localparam ENTRY_IDX_SZ = $clog2(TABLE_ENTRIES);
localparam OP_MODE_SZ = 3;

localparam PA_SIZE_TBOX = 40;
// possible values for operationMode
// comparison operation
localparam OP_MODE_COMP_NEVER            = 3'd0;
localparam OP_MODE_COMP_LESS             = 3'd1;
localparam OP_MODE_COMP_EQUAL            = 3'd2;
localparam OP_MODE_COMP_LESS_OR_EQUAL    = 3'd3;
localparam OP_MODE_COMP_GREATER          = 3'd4;
localparam OP_MODE_COMP_NOT_EQUAL        = 3'd5;
localparam OP_MODE_COMP_GREATER_OR_EQUAL = 3'd6;
localparam OP_MODE_COMP_ALWAYS           = 3'd7;
// gather operation
localparam OP_MODE_CH_SEL_R              = 3'd0;
localparam OP_MODE_CH_SEL_G              = 3'd1;
localparam OP_MODE_CH_SEL_B              = 3'd2;
localparam OP_MODE_CH_SEL_A              = 3'd3;

// values for textureSamplingOperation
`define  TEX_OP_SAMPLE         4'b0000
`define  TEX_OP_SAMPLE_L       4'b0001
`define  TEX_OP_SAMPLE_C       4'b0010
`define  TEX_OP_SAMPLE_C_L     4'b0011
`define  TEX_OP_GATHER4        4'b0100
`define  TEX_OP_GATHER4_C      4'b0101
`define  TEX_OP_GATHER4_PO     4'b0110
`define  TEX_OP_GATHER4_PO_C   4'b0111
`define  TEX_OP_LD             4'b1000

// possible values for formatBpt
localparam IMAGE_FORMAT_BPT_32b = 2'd0;
localparam IMAGE_FORMAT_BPT_64b = 2'd1;
localparam IMAGE_FORMAT_BPT_128b = 2'd2;

// possible values for swizzle
localparam SWIZZLE_NONE     = 3'd0; // Color component is not sent to the Minion
localparam SWIZZLE_IDENTITY = 3'd1;
localparam SWIZZLE_ZERO     = 3'd2;
localparam SWIZZLE_ONE      = 3'd3;
localparam SWIZZLE_RED      = 3'd4;
localparam SWIZZLE_GREEN    = 3'd5;
localparam SWIZZLE_BLUE     = 3'd6;
localparam SWIZZLE_ALPHA    = 3'd7;

localparam BORDER_COLOR_MODE_TRANSPARENT_BLACK     = 2'b00;
localparam BORDER_COLOR_MODE_OPAQUE_BLACK          = 2'b01;
localparam BORDER_COLOR_MODE_OPAQUE_WHITE          = 2'b10;
localparam BORDER_COLOR_MODE_FROM_IMAGE_DESCRIPTOR = 2'b11;

// FLOAT constants
localparam SFLOAT16_ONE = 16'h3C00;
localparam SFLOAT32_ONE = 32'h3F800000;

// address constants
// input table constants
localparam TEX_SEND_DATA_SZ = 256;    /** Size of each data packet sent by minion*/

/** Entries (data types) sent by minion: header(0), u(1), v(2) and w(3) */
localparam TEX_SEND_ENTRY_HEADER = 0;
localparam TEX_SEND_ENTRY_U = 1;
localparam TEX_SEND_ENTRY_V = 2;
localparam TEX_SEND_ENTRY_W = 3;
localparam TEX_SEND_ENTRY_COUNT = 6;
localparam TEX_SEND_ENTRY_SZ = $clog2(TEX_SEND_ENTRY_COUNT);

// Address block constants
localparam PIXELS_IN_HEADER  = 8;
localparam PIXELS_IN_HEADER_SZ = $clog2(PIXELS_IN_HEADER);
localparam TEX_SEND_COORD_SZ = 32; /** Bit width of each coordinate u/v received from minion (float32) */
localparam TEX_SEND_ADDRESS_MODE_SZ = 9;
localparam ADDRESS_MODE_SZ = 3;

/** Address output size for u and betas */
localparam COORD_OUT_IJ_SZ = 16;
localparam COORD_OUT_K_SZ = 15;
localparam BETA_SZ = 8;
localparam ACCUM_WEIGHT = 16;
localparam ACCUM_ANISO_WEIGHT = 8;

/** Latencies in pipeline for texel coordinates processing*/
localparam ADDRESS_MODE_LATENCY = 1;
localparam ADDR_DIM_MUL_LATENCY = 1;
localparam ADDR_CVT_UFIXED_LATENCY = 1;
localparam ADD_HALF_PIXEL_LATENCY = 1;
localparam ADDR_PIPELINE_LATENCY = ADDRESS_MODE_LATENCY + ADDR_DIM_MUL_LATENCY + ADDR_CVT_UFIXED_LATENCY + ADD_HALF_PIXEL_LATENCY;

/** LOD */
localparam LOD_IN_SZ = 16;
localparam LOD_SZ = 5;
localparam LOD_SZ_FRAC = 8;
localparam LOD_SZ_FIXED =  LOD_SZ +  LOD_SZ_FRAC;


/** constants for image info table*/
localparam IMAGE_ID_SZ = 12;

localparam IMG_TYPE_1D = 0;
localparam IMG_TYPE_2D = 1;
localparam IMG_TYPE_CUBE = 2;
localparam IMG_TYPE_3D = 3;
localparam IMG_TYPE_1D_ARRAY = 4;
localparam IMG_TYPE_2D_ARRAY = 5;
localparam IMG_TYPE_CUBE_ARRAY = 6;
localparam IMG_TYPE_COUNT = 7;

// future tags
localparam NR_BANKS = 2;
localparam LINES_PER_BANK =  32;
localparam LINE_IDX_SZ = $clog2(LINES_PER_BANK);
localparam TAG_I_BITS = COORD_OUT_IJ_SZ-1;
localparam TAG_J_BITS = COORD_OUT_IJ_SZ-2;
localparam TAG_SZ = IMAGE_ID_SZ + LOD_SZ + (TAG_I_BITS) + (TAG_J_BITS) + COORD_OUT_K_SZ;

localparam J_BANK_SEL32_BIT = 2;
localparam J_BANK_SEL64_BIT = 2;
localparam J_BANK_SEL128_BIT = 1;
localparam I_BANK_SEL32_BIT = 2;
localparam I_BANK_SEL64_BIT = 1;
localparam I_BANK_SEL128_BIT = 1;

localparam J_OFFSET_SZ = J_BANK_SEL32_BIT;
localparam I_OFFSET_SZ = I_BANK_SEL32_BIT;

// virtual address
localparam ADDRESS_COMPUTE_LATENCY = 2;
localparam CACHE_LINE_SIZE = 64;
localparam CACHE_LINE_SIZE_L = $clog2(64);
localparam DEC_LINE_OFFSET_SZ = 3;

// l2 reorder fifo
localparam L2_FIFO_DEPTH = `TBOX_L2_FIFO_DEPTH;
localparam L2_FIFO_DEPTH_IDX_SZ = `TBOX_L2_FIFO_DEPTH_IDX_SZ;
localparam L2_CACHE_LINE_SZ = 512;

// tmux
localparam TMUX_FROM_NORM_LATENCY = 3;
localparam TMUX_IN_LATENCY = 1+TMUX_FROM_NORM_LATENCY;
localparam TMUX_LATENCY = TMUX_IN_LATENCY + 1;
localparam COL_BITS_32BPT = 2;
localparam ROW_BITS_32BPT = 2;
localparam COL_BITS_64BPT = 1;
localparam ROW_BITS_64BPT = 2;
localparam COL_BITS_128BPT = 1;
localparam ROW_BITS_128BPT = 1;
localparam TMUX_SERIAL_ITERS = NR_COMPONENTS;
localparam TMUX_SERIAL_ITERS_IDX_SZ = $clog2(TMUX_SERIAL_ITERS);

// decompress constants
//number of texels per cache line
localparam CACHE_LINE_UNORM8_TEX_NUM = NUM_TEXELS_CACHELINE_32BPT;
localparam CACHE_LINE_FLOAT16_TEX_NUM = NUM_TEXELS_CACHELINE_64BPT;
localparam CACHE_LINE_FLOAT32_TEX_NUM = NUM_TEXELS_CACHELINE_128BPT;
// input decompress constants
localparam TEX_L2_DATA_SZ = 512;    /** Size of L2 data*/
localparam TEX_L2_IMAGE_FORMAT_SZ = 9;  /** Size of L2 compress data selector*/
//output deompress constants
localparam TEX_L1_DATA_SZ = 512;    /** Size of L1 line, 8 pixelsx4channels*/ 
//generic
localparam L2_LINE_OFFSET_SZ = 64;
localparam DEC_SERIAL_ITERS = 4;
localparam DEC_SERIAL_ITERS_IDX_SZ = $clog2(DEC_SERIAL_ITERS);
localparam UNORM8_PARALLEL_TEX_NUM = CACHE_LINE_UNORM8_TEX_NUM/DEC_SERIAL_ITERS;
localparam FLOAT16_PARALLEL_TEX_NUM = CACHE_LINE_FLOAT16_TEX_NUM/DEC_SERIAL_ITERS;
localparam FLOAT32_PARALLEL_TEX_NUM = CACHE_LINE_FLOAT32_TEX_NUM/DEC_SERIAL_ITERS;
//formats field size
localparam R8G8B8_SZ = 24;
localparam R8G8B8_CHANNEL_SZ = 8;
localparam A8R8G8B8_SZ = 32;
localparam A8R8G8B8_CHANNEL_SZ = 8;
localparam A16R16G16B16_SZ = 64;
localparam A16R16G16B16_CHANNEL_SZ = 16;
localparam BC1_COLOR_SZ = 32;
localparam BC1_COLOR_IDX_SZ = 2;
localparam BC2_ALPHA_SZ = 64;
localparam BC2_ALPHA_CH_SZ = 4;
localparam BC2_COLOR_SZ = 32;
localparam BC2_COLOR_IDX_SZ = 2;
localparam BC3_ALPHA_SZ = 16;
localparam BC3_ALPHA_IDX_SZ = 3;
localparam BC3_COLOR_SZ = 32;
localparam BC3_COLOR_IDX_SZ = 2;
localparam BC4_COLOR_SZ = 16;
localparam BC4_COLOR_IDX_SZ = 3;
localparam BC5_COLOR_SZ = 16;
localparam BC5_COLOR_IDX_SZ = 3;
// NORM_PACK32 (to FLOAT16)
localparam NORM_PACK32_TO_F16_LATENCY = 3;
localparam NORM_PACK32_TO_F16_SZ = 32;
// NORM (to FLOAT32)
localparam NORM_TO_F32_LATENCY = 3;
localparam NORM_TO_F32_SZ = 64;
// SHARED_EXP
localparam SHARED_EXP_LATENCY = 0;
localparam SHARED_EXP_SZ = 32;
// S3TC/RGTC
localparam S3TC_RGTC_CHINTRPL_LATENCY = 2;
localparam S3TC_RGTC_LATENCY = 2 + S3TC_RGTC_CHINTRPL_LATENCY;
localparam S3TC_RGTC_COLOR_IDX_SZ = 6;
localparam S3TC_RGTC_COLOR_SZ = 32;
localparam S3TC_RGTC_ALPHA_IDX_SZ = 3;
localparam S3TC_RGTC_ALPHA_SZ = 16;
//formats field offsets
localparam BC1_COLOR_OFFSET = 0;
localparam BC1_COLOR_IDX_OFFSET = 32;
localparam BC2_COLOR_OFFSET = BC2_ALPHA_SZ;
localparam BC2_COLOR_IDX_OFFSET = BC2_ALPHA_SZ+BC2_COLOR_SZ;
localparam BC3_ALPHA_OFFSET = 0;
localparam BC3_ALPHA_IDX_OFFSET = 16;
localparam BC3_COLOR_OFFSET = 64;
localparam BC3_COLOR_IDX_OFFSET = 96;
localparam BC4_COLOR_OFFSET = 0;
localparam BC4_COLOR_IDX_OFFSET = 16;
localparam BC5_COLOR_R_OFFSET = 0;
localparam BC5_COLOR_G_OFFSET = 64;
localparam BC5_COLOR_R_IDX_OFFSET = 16;
localparam BC5_COLOR_G_IDX_OFFSET = 80;
//bc6h
localparam BC6H_COLOR_IDX_EP_LATENCY = 3;
localparam BC6H_CHINTRPL_LATENCY = 2;
localparam BC6H_LATENCY = 1+BC6H_COLOR_IDX_EP_LATENCY+1+BC6H_CHINTRPL_LATENCY+1;
localparam BC6H_DATA_BLOCK_SZ = 128;
localparam BC6H_DATA_MODE_SZ = 5;
localparam BC6H_MODE_SZ = 4;
localparam BC6H_ONE_SUBSET = 1'b0;
localparam BC6H_TWO_SUBSETS = 1'b1;
localparam BC6H_SUBSETS_NUM = 2;
localparam BC6H_CH_NUM = 3;
localparam BC6H_ENDPOINT_NUM = BC6H_SUBSETS_NUM*2;
localparam BC6H_ENDPOINT_SZ = 16;
localparam BC6H_EPMSB_SZ = $clog2(BC6H_ENDPOINT_SZ);
localparam BC6H_PARTITION_SET_ID_SZ = 5;
localparam BC6H_ANCHOR_IDX_SZ = 4;
localparam BC6H_COLOR_IDX_SZ = 4;
localparam BC6H_INTERPL_WEIGHT_SZ = 8;
localparam BC6H_CH_SZ = 16;
localparam BC6H_BC7_NUM_TEXELS = 16;
//bc7
localparam BC7_COLOR_IDX_EP_LATENCY = 1;
localparam BC7_CHINTRPL_LATENCY = 2;
localparam BC7_LATENCY = 1+BC7_COLOR_IDX_EP_LATENCY+1+BC7_CHINTRPL_LATENCY+1;
localparam BC7_DATA_BLOCK_SZ = 128;
localparam BC7_MODE_SZ = 4;
localparam BC7_SUBSETS_NUM = 3;
localparam BC7_SUBSETS_NUM_SZ = 2;
localparam BC7_CH_NUM = 4;   
localparam BC7_ENDPOINT_NUM = BC7_SUBSETS_NUM*BC7_CH_NUM;
localparam BC7_ENDPOINT_SZ = 8;
localparam BC7_PARTITION_SET_ID_SZ = 6;
localparam BC7_PARTITION_IDX_SZ = 2;
localparam BC7_MODE0_ENDPOINT_CH_SZ = 4;
localparam BC7_MODE0_ENDPOINT_START_OFFSET = 5;
localparam BC7_MODE0_ENDPOINT_END_OFFSET = BC7_MODE0_ENDPOINT_START_OFFSET+BC7_MODE0_ENDPOINT_CH_SZ;
localparam BC7_MODE1_ENDPOINT_CH_SZ = 6;
localparam BC7_MODE1_ENDPOINT_START_OFFSET = 8;
localparam BC7_MODE1_ENDPOINT_END_OFFSET = BC7_MODE1_ENDPOINT_START_OFFSET+BC7_MODE1_ENDPOINT_CH_SZ;
localparam BC7_MODE2_ENDPOINT_CH_SZ = 5;
localparam BC7_MODE2_ENDPOINT_START_OFFSET = 9;
localparam BC7_MODE2_ENDPOINT_END_OFFSET = BC7_MODE2_ENDPOINT_START_OFFSET+BC7_MODE2_ENDPOINT_CH_SZ;
localparam BC7_MODE3_ENDPOINT_CH_SZ = 7;
localparam BC7_MODE3_ENDPOINT_START_OFFSET = 10;
localparam BC7_MODE3_ENDPOINT_END_OFFSET = BC7_MODE3_ENDPOINT_START_OFFSET+BC7_MODE3_ENDPOINT_CH_SZ;
localparam BC7_MODE4_ENDPOINT_CH_SZ = 5;
localparam BC7_MODE4_ENDPOINT_ALPHA_CH_SZ = 6;
localparam BC7_MODE4_ENDPOINT_START_OFFSET = 8;
localparam BC7_MODE4_ENDPOINT_END_OFFSET = BC7_MODE4_ENDPOINT_START_OFFSET+BC7_MODE4_ENDPOINT_CH_SZ;
localparam BC7_MODE4_ENDPOINT_START_ALPHA_OFFSET = 38;
localparam BC7_MODE4_ENDPOINT_END_ALPHA_OFFSET = BC7_MODE4_ENDPOINT_START_ALPHA_OFFSET+BC7_MODE4_ENDPOINT_ALPHA_CH_SZ;
localparam BC7_MODE5_ENDPOINT_CH_SZ = 7;
localparam BC7_MODE5_ENDPOINT_ALPHA_CH_SZ = 8;
localparam BC7_MODE5_ENDPOINT_START_OFFSET = 8;
localparam BC7_MODE5_ENDPOINT_END_OFFSET = BC7_MODE5_ENDPOINT_START_OFFSET+BC7_MODE5_ENDPOINT_CH_SZ;
localparam BC7_MODE5_ENDPOINT_START_ALPHA_OFFSET = 50; 
localparam BC7_MODE5_ENDPOINT_END_ALPHA_OFFSET = BC7_MODE5_ENDPOINT_START_ALPHA_OFFSET+BC7_MODE5_ENDPOINT_ALPHA_CH_SZ; 
localparam BC7_MODE6_ENDPOINT_CH_SZ = 7;
localparam BC7_MODE6_ENDPOINT_ALPHA_CH_SZ = 7;
localparam BC7_MODE6_ENDPOINT_START_OFFSET = 7;
localparam BC7_MODE6_ENDPOINT_END_OFFSET = BC7_MODE6_ENDPOINT_START_OFFSET+BC7_MODE6_ENDPOINT_CH_SZ;
localparam BC7_MODE6_ENDPOINT_START_ALPHA_OFFSET = 49;
localparam BC7_MODE6_ENDPOINT_END_ALPHA_OFFSET = BC7_MODE6_ENDPOINT_START_ALPHA_OFFSET+BC7_MODE6_ENDPOINT_ALPHA_CH_SZ; 
localparam BC7_MODE7_ENDPOINT_CH_SZ = 5;
localparam BC7_MODE7_ENDPOINT_ALPHA_CH_SZ = 5;   
localparam BC7_MODE7_ENDPOINT_START_OFFSET = 14;
localparam BC7_MODE7_ENDPOINT_END_OFFSET = BC7_MODE7_ENDPOINT_START_OFFSET+BC7_MODE7_ENDPOINT_CH_SZ;
localparam BC7_MODE7_ENDPOINT_START_ALPHA_OFFSET = 74;
localparam BC7_MODE7_ENDPOINT_END_ALPHA_OFFSET = BC7_MODE7_ENDPOINT_START_ALPHA_OFFSET+BC7_MODE7_ENDPOINT_ALPHA_CH_SZ; 
localparam BC7_ANCHOR_IDX_NUM = 3;
localparam BC7_ANCHOR_IDX_SZ = 4;
localparam BC7_COLOR_ALPHA_IDX_BITS_SZ = 3;
localparam BC7_COLOR_ALPHA_IDX_SZ = 4;
localparam BC7_INTERPL_WEIGHT_SZ = 7;
localparam BC7_ROTATION_BITS_SZ = 2;

//color definition
localparam MIN_COLOR_UNORM8 = 8'h00;
localparam MAX_COLOR_UNORM8 = 8'hFF;
localparam MIN_COLOR_SNORM8 = 8'h81;
localparam MAX_COLOR_SNORM8 = 8'h7F;
//alpha definition
localparam ALPHA_FULLY_TRANSP_UNORM8 = 8'h00;
localparam ALPHA_FULLY_OPAQUE_UNORM8 = 8'hFF;
localparam ALPHA_FULLY_OPAQUE_SNORM8 = 8'h7F;
localparam ALPHA_FULLY_OPAQUE_SFLOAT16 = SFLOAT16_ONE;
localparam ALPHA_FULLY_OPAQUE_SFLOAT32 = SFLOAT32_ONE;

//SRGB
localparam SRGB_E = 4;
localparam SRGB_M = 7;
localparam SRGB_SZ = SRGB_E+SRGB_M;

//Vulkan formats
localparam VK_FORMAT_UNDEFINED = 0;
localparam VK_FORMAT_R4G4_UNORM_PACK8 = 1;
localparam VK_FORMAT_R4G4B4A4_UNORM_PACK16 = 2;
localparam VK_FORMAT_B4G4R4A4_UNORM_PACK16 = 3;
localparam VK_FORMAT_R5G6B5_UNORM_PACK16 = 4;
localparam VK_FORMAT_B5G6R5_UNORM_PACK16 = 5;
localparam VK_FORMAT_R5G5B5A1_UNORM_PACK16 = 6;
localparam VK_FORMAT_B5G5R5A1_UNORM_PACK16 = 7;
localparam VK_FORMAT_A1R5G5B5_UNORM_PACK16 = 8;
localparam VK_FORMAT_R8_UNORM = 9;
localparam VK_FORMAT_R8_SNORM = 10;
localparam VK_FORMAT_R8_USCALED = 11;
localparam VK_FORMAT_R8_SSCALED = 12;
localparam VK_FORMAT_R8_UINT = 13;
localparam VK_FORMAT_R8_SINT = 14;
localparam VK_FORMAT_R8_SRGB = 15;
localparam VK_FORMAT_R8G8_UNORM = 16;
localparam VK_FORMAT_R8G8_SNORM = 17;
localparam VK_FORMAT_R8G8_USCALED = 18;
localparam VK_FORMAT_R8G8_SSCALED = 19;
localparam VK_FORMAT_R8G8_UINT = 20;
localparam VK_FORMAT_R8G8_SINT = 21;
localparam VK_FORMAT_R8G8_SRGB = 22;
localparam VK_FORMAT_R8G8B8_UNORM = 23;
localparam VK_FORMAT_R8G8B8_SNORM = 24;
localparam VK_FORMAT_R8G8B8_USCALED = 25;
localparam VK_FORMAT_R8G8B8_SSCALED = 26;
localparam VK_FORMAT_R8G8B8_UINT = 27;
localparam VK_FORMAT_R8G8B8_SINT = 28;
localparam VK_FORMAT_R8G8B8_SRGB = 29;
localparam VK_FORMAT_B8G8R8_UNORM = 30;
localparam VK_FORMAT_B8G8R8_SNORM = 31;
localparam VK_FORMAT_B8G8R8_USCALED = 32;
localparam VK_FORMAT_B8G8R8_SSCALED = 33;
localparam VK_FORMAT_B8G8R8_UINT = 34;
localparam VK_FORMAT_B8G8R8_SINT = 35;
localparam VK_FORMAT_B8G8R8_SRGB = 36;
localparam VK_FORMAT_R8G8B8A8_UNORM = 37;
localparam VK_FORMAT_R8G8B8A8_SNORM = 38;
localparam VK_FORMAT_R8G8B8A8_USCALED = 39;
localparam VK_FORMAT_R8G8B8A8_SSCALED = 40;
localparam VK_FORMAT_R8G8B8A8_UINT = 41;
localparam VK_FORMAT_R8G8B8A8_SINT = 42;
localparam VK_FORMAT_R8G8B8A8_SRGB = 43;
localparam VK_FORMAT_B8G8R8A8_UNORM = 44;
localparam VK_FORMAT_B8G8R8A8_SNORM = 45;
localparam VK_FORMAT_B8G8R8A8_USCALED = 46;
localparam VK_FORMAT_B8G8R8A8_SSCALED = 47;
localparam VK_FORMAT_B8G8R8A8_UINT = 48;
localparam VK_FORMAT_B8G8R8A8_SINT = 49;
localparam VK_FORMAT_B8G8R8A8_SRGB = 50;
localparam VK_FORMAT_A8B8G8R8_UNORM_PACK32 = 51;
localparam VK_FORMAT_A8B8G8R8_SNORM_PACK32 = 52;
localparam VK_FORMAT_A8B8G8R8_USCALED_PACK32 = 53;
localparam VK_FORMAT_A8B8G8R8_SSCALED_PACK32 = 54;
localparam VK_FORMAT_A8B8G8R8_UINT_PACK32 = 55;
localparam VK_FORMAT_A8B8G8R8_SINT_PACK32 = 56;
localparam VK_FORMAT_A8B8G8R8_SRGB_PACK32 = 57;
localparam VK_FORMAT_A2R10G10B10_UNORM_PACK32 = 58;
localparam VK_FORMAT_A2R10G10B10_SNORM_PACK32 = 59;
localparam VK_FORMAT_A2R10G10B10_USCALED_PACK32 = 60;
localparam VK_FORMAT_A2R10G10B10_SSCALED_PACK32 = 61;
localparam VK_FORMAT_A2R10G10B10_UINT_PACK32 = 62;
localparam VK_FORMAT_A2R10G10B10_SINT_PACK32 = 63;
localparam VK_FORMAT_A2B10G10R10_UNORM_PACK32 = 64;
localparam VK_FORMAT_A2B10G10R10_SNORM_PACK32 = 65;
localparam VK_FORMAT_A2B10G10R10_USCALED_PACK32 = 66;
localparam VK_FORMAT_A2B10G10R10_SSCALED_PACK32 = 67;
localparam VK_FORMAT_A2B10G10R10_UINT_PACK32 = 68;
localparam VK_FORMAT_A2B10G10R10_SINT_PACK32 = 69;
localparam VK_FORMAT_R16_UNORM = 70;
localparam VK_FORMAT_R16_SNORM = 71;
localparam VK_FORMAT_R16_USCALED = 72;
localparam VK_FORMAT_R16_SSCALED = 73;
localparam VK_FORMAT_R16_UINT = 74;
localparam VK_FORMAT_R16_SINT = 75;
localparam VK_FORMAT_R16_SFLOAT = 76;
localparam VK_FORMAT_R16G16_UNORM = 77;
localparam VK_FORMAT_R16G16_SNORM = 78;
localparam VK_FORMAT_R16G16_USCALED = 79;
localparam VK_FORMAT_R16G16_SSCALED = 80;
localparam VK_FORMAT_R16G16_UINT = 81;
localparam VK_FORMAT_R16G16_SINT = 82;
localparam VK_FORMAT_R16G16_SFLOAT = 83;
localparam VK_FORMAT_R16G16B16_UNORM = 84;
localparam VK_FORMAT_R16G16B16_SNORM = 85;
localparam VK_FORMAT_R16G16B16_USCALED = 86;
localparam VK_FORMAT_R16G16B16_SSCALED = 87;
localparam VK_FORMAT_R16G16B16_UINT = 88;
localparam VK_FORMAT_R16G16B16_SINT = 89;
localparam VK_FORMAT_R16G16B16_SFLOAT = 90;
localparam VK_FORMAT_R16G16B16A16_UNORM = 91;
localparam VK_FORMAT_R16G16B16A16_SNORM = 92;
localparam VK_FORMAT_R16G16B16A16_USCALED = 93;
localparam VK_FORMAT_R16G16B16A16_SSCALED = 94;
localparam VK_FORMAT_R16G16B16A16_UINT = 95;
localparam VK_FORMAT_R16G16B16A16_SINT = 96;
localparam VK_FORMAT_R16G16B16A16_SFLOAT = 97;
localparam VK_FORMAT_R32_UINT = 98;
localparam VK_FORMAT_R32_SINT = 99;
localparam VK_FORMAT_R32_SFLOAT = 100;
localparam VK_FORMAT_R32G32_UINT = 101;
localparam VK_FORMAT_R32G32_SINT = 102;
localparam VK_FORMAT_R32G32_SFLOAT = 103;
localparam VK_FORMAT_R32G32B32_UINT = 104;
localparam VK_FORMAT_R32G32B32_SINT = 105;
localparam VK_FORMAT_R32G32B32_SFLOAT = 106;
localparam VK_FORMAT_R32G32B32A32_UINT = 107;
localparam VK_FORMAT_R32G32B32A32_SINT = 108;
localparam VK_FORMAT_R32G32B32A32_SFLOAT = 109; 
localparam VK_FORMAT_R64_UINT = 110;
localparam VK_FORMAT_R64_SINT = 111;
localparam VK_FORMAT_R64_SFLOAT = 112;
localparam VK_FORMAT_R64G64_UINT = 113;
localparam VK_FORMAT_R64G64_SINT = 114;
localparam VK_FORMAT_R64G64_SFLOAT = 115;
localparam VK_FORMAT_R64G64B64_UINT = 116;
localparam VK_FORMAT_R64G64B64_SINT = 117;
localparam VK_FORMAT_R64G64B64_SFLOAT = 118;
localparam VK_FORMAT_R64G64B64A64_UINT = 119;
localparam VK_FORMAT_R64G64B64A64_SINT = 120;
localparam VK_FORMAT_R64G64B64A64_SFLOAT = 121;
localparam VK_FORMAT_B10G11R11_UFLOAT_PACK32 = 122;
localparam VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 = 123;
localparam VK_FORMAT_D16_UNORM = 124;
localparam VK_FORMAT_X8_D24_UNORM_PACK32 = 125;
localparam VK_FORMAT_D32_SFLOAT = 126;
localparam VK_FORMAT_S8_UINT = 127;
localparam VK_FORMAT_D16_UNORM_S8_UINT = 128;
localparam VK_FORMAT_D24_UNORM_S8_UINT = 129;
localparam VK_FORMAT_D32_SFLOAT_S8_UINT = 130;
localparam VK_FORMAT_BC1_RGB_UNORM_BLOCK = 131;
localparam VK_FORMAT_BC1_RGB_SRGB_BLOCK = 132;
localparam VK_FORMAT_BC1_RGBA_UNORM_BLOCK = 133;
localparam VK_FORMAT_BC1_RGBA_SRGB_BLOCK = 134;
localparam VK_FORMAT_BC2_UNORM_BLOCK = 135;
localparam VK_FORMAT_BC2_SRGB_BLOCK = 136;
localparam VK_FORMAT_BC3_UNORM_BLOCK = 137;
localparam VK_FORMAT_BC3_SRGB_BLOCK = 138;
localparam VK_FORMAT_BC4_UNORM_BLOCK = 139;
localparam VK_FORMAT_BC4_SNORM_BLOCK = 140;
localparam VK_FORMAT_BC5_UNORM_BLOCK = 141;
localparam VK_FORMAT_BC5_SNORM_BLOCK = 142;
localparam VK_FORMAT_BC6H_UFLOAT_BLOCK = 143;
localparam VK_FORMAT_BC6H_SFLOAT_BLOCK = 144;
localparam VK_FORMAT_BC7_UNORM_BLOCK = 145;
localparam VK_FORMAT_BC7_SRGB_BLOCK = 146;
localparam VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK = 147;
localparam VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK = 148;
localparam VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK = 149;
localparam VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK = 150;
localparam VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK = 151;
localparam VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK = 152;
localparam VK_FORMAT_EAC_R11_UNORM_BLOCK = 153;
localparam VK_FORMAT_EAC_R11_SNORM_BLOCK = 154;
localparam VK_FORMAT_EAC_R11G11_UNORM_BLOCK = 155;
localparam VK_FORMAT_EAC_R11G11_SNORM_BLOCK = 156;
localparam VK_FORMAT_ASTC_4x4_UNORM_BLOCK = 157;
localparam VK_FORMAT_ASTC_4x4_SRGB_BLOCK = 158;
localparam VK_FORMAT_ASTC_5x4_UNORM_BLOCK = 159;
localparam VK_FORMAT_ASTC_5x4_SRGB_BLOCK = 160;
localparam VK_FORMAT_ASTC_5x5_UNORM_BLOCK = 161;
localparam VK_FORMAT_ASTC_5x5_SRGB_BLOCK = 162;
localparam VK_FORMAT_ASTC_6x5_UNORM_BLOCK = 163;
localparam VK_FORMAT_ASTC_6x5_SRGB_BLOCK = 164;
localparam VK_FORMAT_ASTC_6x6_UNORM_BLOCK = 165;
localparam VK_FORMAT_ASTC_6x6_SRGB_BLOCK = 166;
localparam VK_FORMAT_ASTC_8x5_UNORM_BLOCK = 167;
localparam VK_FORMAT_ASTC_8x5_SRGB_BLOCK = 168;
localparam VK_FORMAT_ASTC_8x6_UNORM_BLOCK = 169;
localparam VK_FORMAT_ASTC_8x6_SRGB_BLOCK = 170;
localparam VK_FORMAT_ASTC_8x8_UNORM_BLOCK = 171;
localparam VK_FORMAT_ASTC_8x8_SRGB_BLOCK = 172;
localparam VK_FORMAT_ASTC_10x5_UNORM_BLOCK = 173;
localparam VK_FORMAT_ASTC_10x5_SRGB_BLOCK = 174;
localparam VK_FORMAT_ASTC_10x6_UNORM_BLOCK = 175;
localparam VK_FORMAT_ASTC_10x6_SRGB_BLOCK = 176;
localparam VK_FORMAT_ASTC_10x8_UNORM_BLOCK = 177;
localparam VK_FORMAT_ASTC_10x8_SRGB_BLOCK = 178;
localparam VK_FORMAT_ASTC_10x10_UNORM_BLOCK = 179;
localparam VK_FORMAT_ASTC_10x10_SRGB_BLOCK = 180;
localparam VK_FORMAT_ASTC_12x10_UNORM_BLOCK = 181;
localparam VK_FORMAT_ASTC_12x10_SRGB_BLOCK = 182;
localparam VK_FORMAT_ASTC_12x12_UNORM_BLOCK = 183;
localparam VK_FORMAT_ASTC_12x12_SRGB_BLOCK = 184; 

// blender
localparam BLENDER_W_INT_SZ = 1;
localparam BLENDER_W_BETA_FRAC_SZ = 2*BETA_SZ;
localparam BLENDER_W_BETA_BITS    = BLENDER_W_INT_SZ + BLENDER_W_BETA_FRAC_SZ;
localparam BLENDER_W_FRAC_SZ = ACCUM_WEIGHT + BLENDER_W_BETA_FRAC_SZ;
localparam BLENDER_W_BITS    = BLENDER_W_INT_SZ + BLENDER_W_FRAC_SZ;
localparam BLENDER_W_LATENCY = 2; 
localparam BLENDER_CH_LATENCY = 6;

// pixel accum
localparam OUT_DATA_WIDTH = 1024;
localparam TBOX_OUT_FIFO_SIZE = 8;

// Interface for inspecting memories post-silicon
localparam DBG_MEMORY_COUNT = 27;
localparam DBG_MEMORY_DATA_SZ = 64;
localparam DBG_MEMORY_ADDR_SZ = 12;
