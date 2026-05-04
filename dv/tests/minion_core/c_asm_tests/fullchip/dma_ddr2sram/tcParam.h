/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

 typedef struct {
    uint64_t channel;
    uint64_t blockSize;
    uint64_t src_index;  
    uint64_t dst_index;  
    uint64_t src_offset;      
    uint64_t dst_offset;  
    uint64_t num_of_trans;    
 } tcParam_t;
 
/*
@RP RNG 0 3 0
@RP RNG 1 0x1F 0x1F
@RP VAL 0,2,4 0
@RP VAL 0,2,4 0
@RP RNG 0x00000000 0x00000FFF 0x200
@RP RNG 0x00000000 0x00000FFF 0x300
@RP RNG 3 9 3
*/
