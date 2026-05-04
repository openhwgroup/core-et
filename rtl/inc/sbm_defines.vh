// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef _SBM_DEFINES_
`define _SBM_DEFINES_


`define NEIGHS_RANGE_LO 0
`define BANKS_RANGE_LO `NEIGHS_RANGE_HI
`define SHIRE_RANGE_LO `BANKS_RANGE_HI

`define NEIGHS_RANGE_HI `NUM_NEIGH
`define BANKS_RANGE_HI (`BANKS_RANGE_LO + `SC_BANKS )

`define NEIGHS_RANGE (`NEIGHS_RANGE_HI-1):`NEIGHS_RANGE_LO
`define BANKS_RANGE  (`BANKS_RANGE_HI-1):`BANKS_RANGE_LO

`define BANKS_ALL_RANGE_LO `NEIGHS_RANGE_HI
`define BANKS_ALL_RANGE_HI (`BANKS_ALL_RANGE_LO + `SC_BANKS )
`define SHIRE_ALL_RANGE_LO `BANKS_ALL_RANGE_HI
`define SHIRE_ALL_RANGE_HI (`BANKS_ALL_RANGE_LO + `SC_BANKS )

`define SBM_WRITE_CREDITS 8 // same value as noc
`define SBM_READ_CREDITS  8 //can be changed

`define SBM_CREDIT_NUMBER (`SBM_WRITE_CREDITS + `SBM_READ_CREDITS)
`define SBM_BUFFER_SIZE $bits(sbm_buffer_struct_t) 
                       
// Macro search first valid

`define firstBvalid(out,valid,start,end_) \
  begin \
    out = '0; \
    for ( int i = start; i < end_; i ++) \
        if ( valid[i] ) out = i;  \
  end

`define searchError(error,valid,data,start,end_) \
  begin \
    error = 1'b0; \
    for ( int i = start; i < end_; i ++) \
        if ( valid[i] && (data[i].RESP != 2'b00 )) error = 1'b1; \
  end

`define countValids(counter,vec_done,axi_id,vector_valid,vector_id,start,end_) \
  begin \
    counter = '0; \
    for ( int i = start; i < end_; i ++) \
    begin \
        if ( vector_valid[i] && vector_id[i].ID == axi_id) \
        begin \
            counter = counter + 1; \
            vec_done[i] = 1'b1; \
        end  \
    end \
  end
`endif
