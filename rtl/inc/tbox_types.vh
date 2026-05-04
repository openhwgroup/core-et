// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

typedef struct packed{
    logic [10:0]    data;
    logic [7:0]     id;
} texsend_step1_t;

typedef struct packed{
    logic [10:0]    data;
    logic           thread_id;
} tbpull_fifo_t;

typedef enum logic [1:0] {
  COMPONENT_R, 
  COMPONENT_G,
  COMPONENT_B,
  COMPONENT_A 
} components_t;

typedef struct packed {
   // most significant field
   logic [15:0] anisoRatioQuad1;
   logic [15:0] anisoRatioQuad0;
   logic [7:0]  deltaTQ1;
   logic [7:0]  deltaSQ1;
   logic [7:0]  deltaTQ0;
   logic [7:0]  deltaSQ0;
   logic [31:0] reserved5;
   logic [15:0] lodQuad1;
   logic [15:0] lodQuad0;
   // Second DWORD --------------------
        // Sampler State     32 bits
   logic [1:0]  reserved4;
   logic [COMPONENT_SWIZZLE_SZ-1:0] componentSwizzle;
   logic [1:0]  borderColorMode;
   logic [2:0]  addressModeW;
   logic [2:0]  addressModeV;
   logic [2:0]  addressModeU;
   logic [2:0]  compareOp;
   logic        anisoFilter;
   logic        mipmapFilter;
   logic        magFilter;
   logic        minFilter;  
        // Image State       32 bits
   logic [3:0]  reserved3;
   logic [11:0] borderDescId;
   logic [3:0]  reserved2;
   logic [11:0] imageId;
   // First DWORD --------------------
        // Image Instruction 64 bits
   logic [23:0] reserved1;
   logic [7:0]  pixelMask;
   logic [11:0] reserved0;
   logic [1:0]  samplerRequestPackets;
   logic [3:0]  texelOffsetI;
   logic [3:0]  texelOffsetJ;
   logic [3:0]  texelOffsetK;
   logic [1:0]  componentSelection;
   logic [3:0]  imageSamplingOperation;
   // less significant field
} addressHeader_t;


/** Defines tbpull entry used in address input table stage */
typedef struct packed
{
    logic [MINION_ID_SZ+PORT_ID_SZ-1:0]             id; // hart_id+port_id
    logic [MAX_ENTRIES_PER_SAMPLE_REQUEST_LOG-1:0]  size;
    logic [MAX_ENTRIES_PER_SAMPLE_REQUEST_LOG-1:0]  counter;
    logic [NUM_ELEMS_INPUT_BUFFER_LOG-1:0]          ptr;
    logic                                           valid;
} tbpull_info_t;

/** Defines tbpull input interface with minions */
typedef struct packed
{
    logic [MINION_ID_SZ+PORT_ID_SZ-1:0]             id;
    logic [MAX_ENTRIES_PER_SAMPLE_REQUEST_LOG-1:0]  size;    
} sample_request_t;

/** Defines parameter data input interface with minions */
typedef struct packed {
   /** 256bit data from  minion*/
   logic [TEX_SEND_DATA_SZ-1:0] data;
   
   /** Minion ID: 0 to 15*/
   logic [MINION_ID_SZ-1:0]     minionId;

   /** Port ID: 0 to 3*/
   logic [PORT_ID_SZ-1:0]     portId;
   
   /** Determines what type of information is sent in 'data'
    * 0 = header
    * 1 = u
    * 2 = v
    * 3 = w
    * 4 = ?
    * 5 = ?
    * 6 reserved
    * 7 reserved
    */
   logic [TEX_SEND_ENTRY_SZ-1:0] entry;
   
} addressInIO_t;

typedef struct packed {
   logic [(MAX_ENTRIES_PER_SAMPLE_REQUEST*TEX_SEND_DATA_SZ)-1:0]  sample_request_data;
   logic                                                          valid;
   logic                                                          ready;
   logic [MAX_ENTRIES_PER_SAMPLE_REQUEST_LOG-1:0]                 counter;
} addressInTableRegOutIO_t;


typedef struct packed {
   logic [(MAX_ENTRIES_PER_SAMPLE_REQUEST*TEX_SEND_DATA_SZ)-1:0]  sample_request_data;
   logic [MINION_ID_SZ-1:0]      minionId;
   logic [PORT_ID_SZ-1:0]        portId;
} addressInTableOutIO_t;

typedef struct packed {
   /** Pulse sent to minion when the data starts to be processed */
   logic                          ack;
   /** Minion ID to which the ACK is sent*/
   logic [MINION_ID_SZ-1:0]       ackId;
} addressAckOut_t;


/** Address module output interface to future TAGs */
typedef struct packed {
   logic [COMPONENT_SWIZZLE_SZ-1:0] componentSwizzle;
   logic                            gatherOperation;
   logic                            comparisonFilter;
   logic [OP_MODE_SZ-1:0]           operationMode;
   logic [IMAGE_ID_SZ-1:0]          imageId;
   logic [LOD_SZ-1:0]               lod;
   logic [COORD_OUT_IJ_SZ-1:0]      iUL;
   logic [COORD_OUT_IJ_SZ-1:0]      jUL;
   logic [COORD_OUT_K_SZ-1:0]       kUL;
   logic [BETA_SZ-1:0]              betaI;
   logic [BETA_SZ-1:0]              betaJ;
   logic [ACCUM_WEIGHT-1:0]         accum_weight;
   logic [3:0]                      texel_mask;
   logic [3:0]                      border_mask;
   logic [1:0]                      border_color_mode;
   logic                            wrap_texel_i;
   logic                            wrap_texel_j;
   logic                            lastSample;
   logic                            lastPixel;
   logic [PIXELS_IN_HEADER_SZ-1:0]  pixelId;
   logic [PORT_ID_SZ-1:0]           portId;
   logic [MINION_ID_SZ-1:0]         minionId;
   logic [ENTRY_IDX_SZ-1:0]         imgInfoWay;
   logic [IMAGE_FORMAT_BPT_SZ-1:0] formatBpt;
   logic                            isSigned;
   logic [NR_COMPONENTS-1:0]        saveComp;
} addressOutIO_t; 


typedef struct packed {
   logic [I_OFFSET_SZ-1:0] i;
   logic [J_OFFSET_SZ-1:0] j;
   logic                   bank;
   logic [LINE_IDX_SZ-1:0] way;
   logic                   hit;
   logic                   valid;
} futureTags_DataPerTexel_t;

typedef struct packed {
   logic [COMPONENT_SWIZZLE_SZ-1:0] componentSwizzle;
   logic                            gatherOperation;
   logic                            comparisonFilter;
   logic [OP_MODE_SZ-1:0]           operationMode;
   logic [IMAGE_FORMAT_BPT_SZ-1:0]  formatBpt;
   logic                            isSigned;
   futureTags_DataPerTexel_t        ul;
   futureTags_DataPerTexel_t        ur;
   futureTags_DataPerTexel_t        ll;
   futureTags_DataPerTexel_t        lr;
   logic [3:0]                      texel_mask;
   logic [3:0]                      border_mask;
   logic [1:0]                      border_color_mode;
   logic [BETA_SZ-1:0]              betaI;
   logic [BETA_SZ-1:0]              betaJ;
   logic                            lastPixel;
   logic [PIXELS_IN_HEADER_SZ-1:0]  pixelId;
   logic                            lastSample;
   logic [ACCUM_WEIGHT-1:0]         accum_weight;
   logic [MINION_ID_SZ-1:0]         minionId;
   logic [PORT_ID_SZ-1:0]           portId;
   logic [NR_COMPONENTS-1:0]        saveComp;
}  futureTagsDataIO_t;

typedef struct packed {
   logic [COORD_OUT_IJ_SZ-1:0] iAddress;
   logic [COORD_OUT_IJ_SZ-1:0] jAddress;
   logic [COORD_OUT_K_SZ-1:0]  kAddress;
   logic                       enable;
} futureTagsVAddr_Request_t;

typedef struct packed {
   logic [LOD_SZ-1:0] lod;
   futureTagsVAddr_Request_t[NR_BANKS-1:0] request;
   logic [ENTRY_IDX_SZ-1:0] imgInfoWay;
} futureTagsVirtualAddressIO_t;


typedef struct packed{ 
   logic [TEX_L1_DATA_SZ-1:0] data;
   logic [TEX_L2_IMAGE_FORMAT_SZ-1:0] imageFormat;
   logic [DEC_LINE_OFFSET_SZ-1:0]     lineOffset; // in units of 8B
   logic                              lineSel;
   logic                              tiled;
} decompressL2IO_t;


/** image info table entry in memory*/
typedef struct packed{
  // most significant field
   logic [PACKED_LEVEL_SZ-1:0]       packedlevel;       
   logic [PACKED_LEVEL_SZ-1:0]       packedmip;         
   logic                             packedlayout;      
   logic                             tiled;             
   logic [ELEMENT_PITCH_SZ -1: 0]    elementPitch;      
   logic [MIP_COUNT_SCALE_SZ-1:0]    mipCountScaleBy4;  
   logic [MIP_COUNT_SCALE_SZ-1:0]    mipCountScaleBy8;  
   logic [MIP_PITCH_SZ-1:0]          mipPitch_l1;       
   logic [MIP_PITCH_SZ-1:0]          mipPitch_l0;       
   logic [ROW_PITCH_SZ-1:0]          rowPitch;          
   logic [COMPONENT_SWIZZLE_SZ-1:0]  componentSwizzle;  
   logic [BASE_MIP_LEVEL_SZ-1:0]     baseMipLevel;      
   logic [BASE_ARRAY_LAYER_SZ-1:0]   baseArrayLayer;    
   logic [ARRAY_LAYER_COUNT_SZ-1:0]  arrayLayerCount;   
   logic [MIP_LEVEL_COUNT_SZ-1:0]    mipLevelCount;     
   logic [TEXTURE_DIM_K_SZ-1:0]      texDepth;          
   logic [TEXTURE_DIM_IJ_SZ-1:0]     texHeight;         
   logic [TEXTURE_DIM_IJ_SZ-1:0]     texWidth;          
   logic [IMAGE_FORMAT_SZ-1:0]       imageFormat;       
   logic [IMAGE_TYPE_SZ-1:0]         imageType;         
   logic [BASE_POINTER_SZ-1:0]       basePointer;       
  // least significant field
} imageInformationMemEntry_t;

/** image info table entry, with reserved fields (as written in memory)*/
typedef struct packed{
   // most significant field
   logic [IMGINFO_RESERVED1_SZ-1:0]  reserved1;         // 255..252 -- Fourth 64b
   logic [PACKED_LEVEL_SZ-1:0]       packedlevel;       // 251..248
   logic [PACKED_LEVEL_SZ-1:0]       packedmip;         // 247..244
   logic                             packedlayout;      // 243
   logic                             tiled;             // 242
   logic [ELEMENT_PITCH_SZ -1: 0]    elementPitch;      // 241..212
   logic [MIP_COUNT_SCALE_SZ-1:0]    mipCountScaleBy4;  // 211..207
   logic [MIP_COUNT_SCALE_SZ-1:0]    mipCountScaleBy8;  // 206..202
   logic [MIP_PITCH_SZ-1:0]          mipPitch_l1;       // 201..197
   logic [MIP_PITCH_SZ-1:0]          mipPitch_l0;       // 196..192
   logic [IMGINFO_RESERVED0_SZ-1:0]  reserved0;         // 191..183 -- Third 64b
   logic [ROW_PITCH_SZ-1:0]          rowPitch;          // 182..169
   logic [COMPONENT_SWIZZLE_SZ-1:0]  componentSwizzle;  // 168..157
   logic [BASE_MIP_LEVEL_SZ-1:0]     baseMipLevel;      // 156..152
   logic [BASE_ARRAY_LAYER_SZ-1:0]   baseArrayLayer;    // 151..140
   logic [ARRAY_LAYER_COUNT_SZ-1:0]  arrayLayerCount;   // 139..128
   logic [MIP_LEVEL_COUNT_SZ-1:0]    mipLevelCount;     // 127..123 -- Second 64b
   logic [TEXTURE_DIM_K_SZ-1:0]      texDepth;          // 122..108
   logic [TEXTURE_DIM_IJ_SZ-1:0]     texHeight;         // 107..92
   logic [TEXTURE_DIM_IJ_SZ-1:0]     texWidth;          // 91..76
   logic [IMAGE_FORMAT_SZ-1:0]       imageFormat;       // 75..67
   logic [IMAGE_TYPE_SZ-1:0]         imageType;         // 66..64
   logic [BASE_POINTER_SZ-1:0]       basePointer;       // 63..0    -- First 64b
  // least significant field
} imageInformationMemEntryPadding_t;
 
typedef struct packed{
   logic [COMPONENT_SWIZZLE_SZ-1:0]  componentSwizzle;
   logic [BASE_MIP_LEVEL_SZ-1:0]     baseMipLevel;   
   logic [BASE_ARRAY_LAYER_SZ-1:0]   baseArrayLayer; 
   logic [ARRAY_LAYER_COUNT_SZ-1:0]  arrayLayerCount;
   logic [MIP_LEVEL_COUNT_SZ-1:0]    mipLevelCount; 
   logic [TEXTURE_DIM_K_SZ-1:0]      texDepth;      
   logic [TEXTURE_DIM_IJ_SZ-1:0]     texHeight;     
   logic [TEXTURE_DIM_IJ_SZ-1:0]     texWidth;      
   logic [IMAGE_TYPE_SZ-1:0]         imageType;     
   logic [IMAGE_FORMAT_BPT_SZ-1:0]   formatBpt;
   logic                             isSigned;
   logic                             hasAlpha;
} imageInformationTableEntry_t;

typedef struct packed{
   // fields sent to vAddr
   logic [PACKED_LEVEL_SZ-1:0]   packedlevel;   // 4
   logic [PACKED_LEVEL_SZ-1:0]   packedmip;     // 4
   logic                         packedlayout;  // 1
   logic                         tiled;         // 1
   logic [ELEMENT_PITCH_SZ-1: 0] elementPitch;  // 30
   logic [MIP_PITCH_SZ-1:0]      mipPitch_l1;   // 5
   logic [MIP_PITCH_SZ-1:0]      mipPitch_l0;   // 5
   logic [ROW_PITCH_SZ-1:0]      rowPitch;      // 10
   logic [TEXTURE_DIM_K_SZ-1:0]  depth;         // 15
   logic [TEXTURE_DIM_IJ_SZ-1:0] height;        // 16
   logic [TEXTURE_DIM_IJ_SZ-1:0] width;         // 16
   logic [IMAGE_FORMAT_SZ-1:0]   imageFormat;   // 9
   logic [IMAGE_TYPE_SZ-1:0]     imageType;     // 3
   logic [BASE_POINTER_SZ-1:0]   basePointer;   // 64
} imageInformationVaddressEntry_t;

/** address interface with vAddr*/
typedef struct packed{
   logic [ENTRY_IDX_SZ-1:0] way;
   logic [$bits(imageInformationVaddressEntry_t)-1:0] data;
} imageInformationVAddrIO_t;

typedef struct packed{
   logic [ADDRESS_POINTER_SZ-1:0]                address;
   logic [L2_FIFO_DEPTH_IDX_SZ-1:0]              id;
} l2ReorderFifo_req_t;

typedef struct packed {
   logic [L2_CACHE_LINE_SZ-1:0]                  data;
   logic [L2_FIFO_DEPTH_IDX_SZ-1:0]              id;
} l2ReorderFifo_rep_t;

// virtual address
typedef struct packed {
   logic [BASE_POINTER_SZ-1:0]                       address;
   logic [IMAGE_FORMAT_SZ-1:0]                       imageFormat;
   logic                                             lineSel;
   logic                                             tiled;
} virtualAddressL2IO_t;


// decompress

/** Float32 Channel Bundle */
typedef struct packed{
   /** Channel ALPHA */
   float32_t chA;
   /** Channel BLUE */
   float32_t chB;
   /** Channel GREEN */
   float32_t chG;
   /** Channel RED */
   float32_t chR;
} float32ChBundle_t;

/** Float16 Channel Bundle */
typedef struct packed{
   /** Channel ALPHA */
   float16_t chA;
   /** Channel BLUE */
   float16_t chB;
   /** Channel GREEN */
   float16_t chG;
   /** Channel RED */
   float16_t chR;
} float16ChBundle_t;

/** Unorm8 Channel Bundle */
typedef struct packed{
   /** Channel ALPHA */
   logic [7:0] chA;
   /** Channel BLUE */
   logic [7:0] chB;
   /** Channel GREEN */
   logic [7:0] chG;
   /** Channel RED */
   logic [7:0] chR;
} unorm8ChBundle_t;

/** Snorm8 Channel Bundle */
typedef struct packed{
   /** Channel ALPHA */
   logic signed [7:0] chA;
   /** Channel BLUE */
   logic signed [7:0] chB;
   /** Channel GREEN */
   logic signed [7:0] chG;
   /** Channel RED */
   logic signed [7:0] chR;
} snorm8ChBundle_t;

typedef union packed
{
    unorm8ChBundle_t un8;
    snorm8ChBundle_t sn8;
} norm8ChBundle_t;

/** Cache Line with 2x2 texels in Float32 format */
typedef float32ChBundle_t [CACHE_LINE_FLOAT32_TEX_NUM-1:0] cacheLine2x2Float32_t;

/** Cache Line with 4x2 texels in Float16 format */
typedef float16ChBundle_t [CACHE_LINE_FLOAT16_TEX_NUM-1:0] cacheLine4x2Float16_t;

/** Cache Line with 4x4 texels in Unorm8 format */
typedef unorm8ChBundle_t [CACHE_LINE_UNORM8_TEX_NUM-1:0] cacheLine4x4Unorm8_t;

typedef union packed
{
    cacheLine2x2Float32_t f32;
    cacheLine4x2Float16_t f16;
    cacheLine4x4Unorm8_t  u8;
} cacheLine_t;

// tmux
typedef struct packed {
   /** Column index for texel UL */
   logic [1:0] i;
   /** Row index for texel UL */
   logic [1:0] j;
   /** Bank where texel UL is */
   logic       bank;
   /** Data for texel UL is valid*/
   logic       valid;
} tmuxInPerTexel_t;


/** TMux Input Bundle*/
typedef struct packed{
   /** Component Swizzle */
   logic [COMPONENT_SWIZZLE_SZ-1:0] componentSwizzle;
   /** Comparison and gather operations, mode and reference */
   logic                     gatherOperation;
   logic                     comparisonFilter;
   logic [31:0]              comparisonReference;
   logic [OP_MODE_SZ-1:0]    operationMode;
   /** Image format */
   logic [IMAGE_FORMAT_BPT_SZ-1:0] imageFormatBpt;
   logic                           isSigned;
   /** Cache line from Bank 0 */
   logic [L2_CACHE_LINE_SZ-1:0] bankDataB0;
   /** Cache line from Bank 1 */
   logic [L2_CACHE_LINE_SZ-1:0] bankDataB1;
   /** info per each of the 4 texels */
   tmuxInPerTexel_t ul;
   tmuxInPerTexel_t ur;
   tmuxInPerTexel_t ll;
   tmuxInPerTexel_t lr;
   /** Beta*i */
   logic [BETA_SZ-1:0]       betaI;
   /** Beta*j */
   logic [BETA_SZ-1:0]       betaJ;
   /** texel and edge masks */
   logic [3:0]               texel_mask;
   logic [3:0]               border_mask;
   logic [1:0]               border_color_mode;
   /** Id of minion that launched the request*/
   logic [MINION_ID_SZ-1:0]  minionId;
   logic [PORT_ID_SZ-1:0]    portId;
   /** Last Pixel Flag */
   logic                            lastPixel;
   logic [PIXELS_IN_HEADER_SZ-1:0]  pixelId;
   /* weight of this sample, and last Sample flag */
   logic                            lastSample;
   logic [ACCUM_WEIGHT-1:0]         accum_weight;
   /* propagate the components to be saved*/ 
   logic [NR_COMPONENTS-1:0]        saveComp;
} tmuxInIO_t;

// TMux Output Bundle
typedef struct packed{
   /** Component Swizzle */
   logic [COMPONENT_SWIZZLE_SZ-1:0] componentSwizzle;
   /** Comparison and gather operations, mode and reference */
   logic                     gatherOperation;
   logic                     comparisonFilter;
   logic [31:0]              comparisonReference;
   logic [OP_MODE_SZ-1:0]    operationMode;
   /** Format and component sent (if float32) */
   logic                     isFloat32;
   components_t              comp;
   logic                     lastComp;
   /** Texel UL */
   logic [64-1:0]            texelUL;
   /** Texel UR */
   logic [64-1:0]            texelUR;
   /** Texel LL */
   logic [64-1:0]            texelLL;
   /** Texel LR */
   logic [64-1:0]            texelLR;
   /** Beta*i */
   logic [BETA_SZ-1:0]       betaI;
   /** Beta*j */
   logic [BETA_SZ-1:0]       betaJ;
   /** Id of minion that launched the request */
   logic [MINION_ID_SZ-1:0]  minionId;
   logic [PORT_ID_SZ-1:0]    portId;
   /** Last Pixel Flag */
   logic                     lastPixel;
   logic [PIXELS_IN_HEADER_SZ-1:0]  pixelId;
   /* weight of this sample, and last Sample flag */
   logic                            lastSample;
   logic [ACCUM_WEIGHT-1:0]         accum_weight;
   /* propagate the components to be saved*/ 
   logic [NR_COMPONENTS-1:0]        saveComp;
} tmuxOutIO_t;

typedef struct packed {
    logic [4:0] tb_ibuff_oc;
    logic [2:0] tb_inf_tbpull_req;
} addressInputTable_tex_status_t;

// blender
typedef struct packed{
   /** Component Swizzle */
   logic [COMPONENT_SWIZZLE_SZ-1:0] componentSwizzle;
   /** Format */
   logic isFloat32;
   /** Output sample */
   logic [128-1:0]                      sample;
   /** Id of minion that launched the request*/
   logic [MINION_ID_SZ-1:0]             minionId;
   logic [PORT_ID_SZ-1:0]               portId;
   /** Last Pixel Flag */
   logic                                lastPixel;
   logic [PIXELS_IN_HEADER_SZ-1:0]      pixelId;
   logic [NR_COMPONENTS-1:0]            saveComp;
} blenderOutIO_t;

/** Texel Float16 Bundle */
typedef struct packed {
  /** Upper-Left texel */
  float16_t tUL;
  /** Upper-Right texel */
  float16_t tUR;
  /** Lower-Left texel */
  float16_t tLL;
  /** Lower-Right texel */
  float16_t tLR;
} texelFloat16Bundle_t;

/** Texel Float32 Bundle */
typedef struct packed {
  /** Upper-Left texel */
  float32_t tUL;
  /** Upper-Right texel */
  float32_t tUR;
  /** Lower-Left texel */
  float32_t tLL;
  /** Lower-Right texel */
  float32_t tLR;
} texelFloat32Bundle_t;


// pixel accum
typedef struct packed{
   logic [OUT_DATA_WIDTH-1:0] data;
   logic [MINION_ID_SZ-1:0]  minionId;
   logic [PORT_ID_SZ-1:0]    portId;
   logic [NR_COMPONENTS-1:0] saveComp;
} pixelAccumOutIO_t;

typedef struct packed{
    logic [LOD_SZ-1:0] lod0;
    logic [LOD_SZ-1:0] lod1;
    logic              trilinear;
    logic [LOD_SZ_FRAC-1:0] mip_beta;
    logic [LOD_SZ_FRAC-1:0] mip_beta_inv;
} triliniear_conf_t;

// `ifndef _TBOX_IFS_
//  `define _TBOX_IFS_
// and allow some of the previous structs to be used as read/ready interfaces
`define ENQIO_IF(if_name, bits_type) \
interface if_name; \
   logic valid; \
   logic ready; \
   bits_type bits;  \
   logic fire; \
   assign fire = valid & ready; \
   modport master (output  valid, input  ready, output bits, input fire);  \
   modport slave (input  valid, output ready, input bits, input fire);  \
endinterface

`define VALID_IF(if_name, bits_type) \
interface if_name; \
   logic valid; \
   bits_type bits; \
   modport master (output  valid, output bits);  \
   modport slave (input  valid, input bits);  \
endinterface

//`endif

localparam IMG_INFO_TABLE_WIDTH = $bits(imageInformationTableEntry_t);
localparam VADDR_MEM_WIDTH = $bits(imageInformationVaddressEntry_t);

`ENQIO_IF(sample_request_if, sample_request_t)
`ENQIO_IF(addressInTableOutIO_if, addressInTableOutIO_t)
`ENQIO_IF(addressOutIO_if, addressOutIO_t)
`ENQIO_IF(futureTagsDataIO_if, futureTagsDataIO_t)
`ENQIO_IF(decompressL2IO_if, decompressL2IO_t)
`ENQIO_IF(imageInformationL2Req_if, logic [PA_SIZE_TBOX-1:0])
`VALID_IF(imageInformationL2Rep_if, logic [MEM_ENTRY_SZ-1:0])
`ENQIO_IF(imageInformationRdRep_if, logic [IMG_INFO_TABLE_WIDTH+ENTRY_IDX_SZ-1:0])
`ENQIO_IF(l2ReorderFifo_req_if, l2ReorderFifo_req_t)
`VALID_IF(l2ReorderFifo_rep_if, l2ReorderFifo_rep_t)
`ENQIO_IF(virtualAddressL2IO_if, virtualAddressL2IO_t)
`ENQIO_IF(futureTagsVirtualAddressIO_if, futureTagsVirtualAddressIO_t)
`VALID_IF(imageInformationVAddrIO_if, imageInformationVAddrIO_t)
`VALID_IF(addressInIO_if, addressInIO_t)
`ENQIO_IF(cacheDataL2IO_if, logic [TEX_L1_DATA_SZ-1:0])
`ENQIO_IF(tmuxInIO_if, tmuxInIO_t)
`ENQIO_IF(tmuxOutIO_if, tmuxOutIO_t)
`VALID_IF(addressAckOut_if, addressAckOut_t)
`ENQIO_IF(pixelAccumOutIO_if, pixelAccumOutIO_t)
`ENQIO_IF(blenderOutIO_if, blenderOutIO_t)
