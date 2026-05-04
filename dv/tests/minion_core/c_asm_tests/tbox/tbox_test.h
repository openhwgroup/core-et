/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _TBOX_TEST_H_
#define _TBOX_TEST_H_

#include <inttypes.h>
#include <cstring>

// Initial defines for Sample Request header components

/*Image Sampling Operation*/
#define IMAGE_SAMPLING_OPERATION_SAMPLE         0   // aniso header / may use aniso
#define IMAGE_SAMPLING_OPERATION_SAMPLE_L       1	// 4 LODS header
#define IMAGE_SAMPLING_OPERATION_SAMPLE_C       2   // aniso header / may use aniso
#define IMAGE_SAMPLING_OPERATION_SAMPLE_C_L     3	// 4 LODS header
#define IMAGE_SAMPLING_OPERATION_GATHER4        4	// aniso header / cannot use aniso
#define IMAGE_SAMPLING_OPERATION_GATHER4_C      5	// aniso header / cannot use aniso
#define IMAGE_SAMPLING_OPERATION_GATHER4_PO     6	// aniso header / cannot use aniso
#define IMAGE_SAMPLING_OPERATION_GATHER4_PO_C   7	// aniso header / cannot use aniso
#define IMAGE_SAMPLING_OPERATION_LD             8	// 4 LODS header
#define IMAGE_SAMPLING_OPERATION_SIZE           9
/* Component Selection : Header[5:4], 2b

Component selection for GATHER4, GATHER4_PO.  Otherwise ignored.
	2?b00 : Red
	2?b01 : Green
	2?b10 : Blue
	2?b11 : Alpha

*/
#define COMPONENT_SELECTION_RED     0
#define COMPONENT_SELECTION_GREEN   1
#define COMPONENT_SELECTION_BLUE    2
#define COMPONENT_SELECTION_ALPHA   3
#define COMPONENT_SELECTION_SIZE    4

#define NEAREST_FILTER              0
#define LINEAR_FILTER               1

#define ANISO_DISABLED              0
#define ANISO_ENABLED               1


/*
Compare Operation : Header[102:100] 3b
Comparison operation for SAMPLE_C, SAMPLE_C_L, GATHER4_C and GATHER4_PO_C operations.  Otherwise ignored.  Based on Vulkan encodings.
	3?b000 : Never
	3?b001 : Less
	3?b010 : Equal
	3?b011 : Less Or Equal
    3?b100 : Greater
	3?b101 : Not Equal
	3?b110 : Greater Or Equal
	3?b111 : Always
*/
#define COMP_NEVER                0
#define COMP_LESS                 1
#define COMP_EQUAL                2
#define COMP_LESS_OR_EQUAL        3
#define COMP_GREATER              4
#define COMP_NOT_EQUAL            5
#define COMP_GREATER_OR_EQUAL     6
#define COMP_ALWAYS               7
#define COMP_SIZE                 8

/*
	Address Mode : Header[111:103], 9 bits
Texel Address Mode for the horizontal, vertical and depth dimensions. Based on Vulkan encodings.  

NOTE : MIRRORED_REPEAT and MIRROR_CLAMP_TO_EDGE may not be supported in the HW implementation!!!

		AddressModeU : Header[105:103], 3 bits
		AddressModeV : Header[108:106], 3 bits
		AddressModeW : Header[111:109], 3 bits
			3?b000 : REPEAT
			3?b001 : MIRRORED_REPEAT
			3?b010 : CLAMP_TO_EDGE
			3?b011 : CLAMP_TO_BORDER
			3?b100 : MIRROR_CLAMP_TO_EDGE
			others : reserved
*/
#define ADDR_MODE_REPEAT                0
#define ADDR_MODE_MIRRORED_REPEAT       1
#define ADDR_MODE_CLAMP_TO_EDGE         2
#define ADDR_MODE_CLAMP_TO_BORDER       3
#define ADDR_MODE_MIRROR_CLAMP_TO_EDGE  4
#define ADDR_MODE_SIZE                  5

/*
	Border Color Mode : Header[113:112], 2 bits
NOTE : The hardware implementation of this feature is not defined yet and may change.
		2?b00 : TRANSPARENT_BLACK
		2?b01 : OPAQUE_BLACK
		2?b10 : OPAQUE_WHITE
		2?b11 : FROM_IMAGE_DESCRIPTOR
*/
#define BORDER_COLOR_TRANSPARENT_BLACK      0
#define BORDER_COLOR_OPAQUE_BLACK           1
#define BORDER_COLOR_OPAQUE_WHITE           2
#define BORDER_COLOR_FROM_IMAGE_DESCRIPTOR  3
#define BORDER_COLOR_SIZE                   4

/*
	Component Swizzle : Header[125:114], 12 bits
3-bit swizzle mode per Component.
SwizzleRed : Header[116:114], 3 bits
SwizzleGreen : Header[119:117], 3 bits
SwizzleBlue : Header[122:120], 3 bits
SwizzleAlpha : Header[125:123], 3 bits
	3?b000 : None (color component is not sent to the Minion)
	3?b001 : Identity
	3?b010 : Zero
	3?b011 : One
	3?b100 : Red
	3?b101 : Green
	3?b111 : Blue
	3?b111 : Alpha
	Reserved : Header[127:126], 2 bits
*/
#define SWIZZLE_NONE    0
#define SWIZZLE_IDENTIY 1
#define SWIZZLE_ZERO    2
#define SWIZZLE_ONE     3
#define SWIZZLE_RED     4
#define SWIZZLE_GREEN   5
#define SWIZZLE_BLUE    6
#define SWIZZLE_ALPHA   7
#define SWIZZLE_SIZE    8

//end initial defines for Sample Request header components-------------------------------

// NeighESRs Neigh# ESR@
//     013fd      X XXXX
//     013fd      X 8010 image table base address esr
//     013fd      X 8008 status esr
//     013fd      X 8000 control esr
#define TBOX_CONTROL_ESR_PTR    0x80108000ULL
#define TBOX_STATUS_ESR_PTR     0x80108008ULL
#define TBOX_IMG_TABLE_BASE_PTR 0x80108010ULL

//Other ESR
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

#define CTC(X) ({ extern int __attribute__((error("assertion failure: '" #X "' not true"))) compile_time_check(); ((X)?0:compile_time_check()),0; })

#define LOCK_VA         0x8df
//#define SCP_CONF_CSR    0x51f
#define PORT0_CTRL      0x9cc    // PORTCTRL. Supervisor mode. Start port address for minion. Each minion has 4 ports.
#define PORT0_HEAD      0xcc8    // Blocking port address
#define PORT0_HEADNB    0xccc  // Non blocking port address

#define DIV_CEIL(a,b) (( (a)/(b) ) + ((a) % (b) > 0 ? 1 : 0 ))
#define NR_PORTS 4

typedef struct
{
   // DWORD
   uint64_t imageSamplingOperation  :   4,
            componentSelection      :   2,
            texelOffsetK            :   4,
            texelOffsetJ            :   4,
            texelOffsetI            :   4,
            samplerRequestPackets   :   2,
            reserved0               :   12;

   // DWORD
   uint64_t pixelMask               :   8,
            reserved1               :   24,  
            imageId                 :   12,
            reserved2               :   4,
            borderDescId            :   12,
            reserved3               :   4;

   // DWORD
   uint64_t minFilter               :   1,  
            magFilter               :   1,
            mipmapFilter            :   1,
            anisoFilter             :   1,
            compareOp               :   3, // Comparison operation for SAMPLE_C, SAMPLE_C_L, GATHER4_C and GATHER4_PO_C operations. Previously opmode. 
            addressModeU            :   3,
            addressModeV            :   3,
            addressModeW            :   3,
            borderColorMode         :   2,
            swizzler                :   3,
            swizzleg                :   3,
            swizzleb                :   3,
            swizzlea                :   3,
            reserved4               :   2;
   union 
   {
        uint16_t lod_array[8];
        struct
        {
            // DWORD
            uint64_t    lodQuad0    :   16,
                        lodQuad1    :   16,
                        reserved5   :   32;
            // DWORD
            uint64_t    deltaSQ0    :   8,
                        deltaTQ0    :   8,
                        deltaSQ1    :   8,
                        deltaTQ1    :   8,
                        anisoRatioQuad0 :   16,
                        anisoRatioQuad1 :   16;
        } lodaniso;
    } lodaniso;
} sample_request_info;

typedef struct{
    uint32_t                id;
    sample_request_info     info;
} sample_request_t;

typedef union
{
   uint8_t              data[32];
   sample_request_info  info;
} SampleRequest;

struct ImageInfo
{
      uint64_t address;          // first 64 b
      unsigned type         :  3;// second 64 b
      unsigned format       :  9;
      unsigned width        : 16;
      unsigned height       : 16;
      unsigned depth        : 12;
      unsigned reserved0    :  8;
      unsigned arraybase    : 12;// third 64 b
      unsigned arraycount   : 12;
      unsigned basemip      :  5;
      unsigned mipcount     :  5;
      unsigned swizzler     :  3;
      unsigned swizzleg     :  3;
      unsigned swizzleb     :  3;
      unsigned swizzlea     :  3;
      unsigned reserved1    : 18;
      unsigned rowpitch     : 10;// fourth 64 bits
      unsigned mippitchl0   :  5;
      unsigned mippitchl1   :  5;
      unsigned elementpitch : 30;
      unsigned tiled        :  1;
      unsigned packedlayout :  1;
      unsigned packedmip    :  4;
      unsigned packedlevel  :  4;
      unsigned reserved2    :  4;
} __attribute__((packed));

typedef union 
{
    float coords[8];
    struct {
        float coord_quad0[4];
        float coord_quad1[4];
    } info;
} tboxSendCoord_t;

typedef struct
{
  uint32_t log2size[NR_PORTS];
  uint32_t maxMsgs[NR_PORTS];
  uint32_t way[NR_PORTS];
  uint32_t firstSet[NR_PORTS];
  uint32_t valid[NR_PORTS];
  uint32_t count[NR_PORTS];
  uint32_t blocking[NR_PORTS];
  uint32_t enable_oob[NR_PORTS];
} test_config_t;

#endif
