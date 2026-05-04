/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _MEM_TABLE_HEADER_
#define _MEM_TABLE_HEADER_

#include "hal_device.h"

#define ITER_END_ADDR 0xFFFFFFFFFFFFFF

#define DRAM_LOW_MCODE_REGION_BASE_ADDR     0x8000000000
#define DRAM_LOW_MCODE_REGION_SIZE              0x800000
#define DRAM_LOW_OS_REGION_BASE_ADDR        0x8000800000
#define DRAM_LOW_OS_REGION_SIZE               0xFFF80000 
#define DRAM_LOW_MEMORY_REGION_BASE_ADDR    0x8100000000
#define DRAM_LOW_MEMORY_REGION_28GB_SIZE     0x700000000 //0x3F00000000  Reduced to 28GB
#define DRAM_LOW_MEMORY_REGION_12GB_SIZE     0x300000000 //0x3F00000000  Reduced to 12GB

//#define DRAM_HIGH_MCODE_REGION_BASE_ADDR    0xC000000000
//#define DRAM_HIGH_MCODE_REGION_SIZE             0x200000
//#define DRAM_HIGH_OS_REGION_BASE_ADDR       0xC000200000
//#define DRAM_HIGH_OS_REGION_SIZE              0xFFFE0000
//#define DRAM_HIGH_MEMORY_REGION_BASE_ADDR   0xC100000000
//#define DRAM_HIGH_MEMORY_REGION_SIZE        0x3F00000000  








typedef enum {U_MODE, S_MODE, M_MODE, X} ECpuMode;

typedef struct AccessModes_t{
    ECpuMode cpuMode;
    uint8_t accessCM;
    uint8_t accessMM;
    uint8_t accessSP;  
} AccessModes_t;

typedef struct MemTable_t{
    uint64_t baseAddr;
    uint64_t size;
    //AccessModes_t accessModes[3];    
} MemTable_t;

MemTable_t memTable[] = {
                        {DRAM_LOW_MCODE_REGION_BASE_ADDR,   DRAM_LOW_MCODE_REGION_SIZE},   //  {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        {DRAM_LOW_OS_REGION_BASE_ADDR,      DRAM_LOW_OS_REGION_SIZE},      //  {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        {DRAM_LOW_MEMORY_REGION_BASE_ADDR,  DRAM_LOW_MEMORY_REGION_12GB_SIZE},  // {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        {DRAM_LOW_MEMORY_REGION_BASE_ADDR,  DRAM_LOW_MEMORY_REGION_28GB_SIZE},  // {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        //{DRAM_HIGH_MCODE_REGION_BASE_ADDR,  DRAM_HIGH_MCODE_REGION_SIZE,  {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        //{DRAM_HIGH_OS_REGION_BASE_ADDR,     DRAM_HIGH_OS_REGION_SIZE,     {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        //{DRAM_HIGH_MEMORY_REGION_BASE_ADDR, DRAM_HIGH_MEMORY_REGION_SIZE, {{U_MODE, 0, 0, 1}, {S_MODE, 0, 0, 0}, {M_MODE, 1, 1, 1}}},
                        {ITER_END_ADDR, 0}    //{{X,0,0,0},{X,0,0,0},{X,0,0,0}}}        
};

#endif
