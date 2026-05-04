// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module icache_data_ram_wrap
#(
    parameter ID = 0
)
(
    // System signals
    input  logic                                   clock,
    input  logic                                   reset,
    input  logic                                   dft__sram_clock,
    input  logic                                   dft__clk_override,

    // Neighborhood - Icache L1 SRAM interface
    input  logic                                   icache_req_write,
    input  logic [`ICACHE_SRAM_ADD_WIDTH-1:0]      icache_req_addr,
    input  logic                                   icache_req_valid,
    output logic                                   icache_req_ready,
    output logic [`ICACHE_SRAM_DATA_WIDTH-1:0]     icache_resp_dout,
    output logic                                   icache_resp_valid,
    input  logic                                   icache_resp_ready,
    // L2 - Neighborhood response
    input  et_link_rsp_info_t                      neigh_sc_rsp_info,
    input  logic                                   neigh_sc_rsp_valid,
    input  logic                                   neigh_sc_rsp_ready,
    // ESRs
    input  esr_shire_cache_ram_cfg_t               esr_shire_cache_ram_cfg,
    // BIST
    input  sc_icache_bist_req_t                    bist_req_info,
    output sc_icache_bist_rsp_t                    bist_rsp_info,
    // APB signals from BPAM for debug 
    input  logic [`ICACHE_DATA_APB_ADDR_WIDTH-1:0] apb_paddr,
    input  logic                                   apb_pwrite,
    input  logic                                   apb_psel,
    input  logic                                   apb_penable,
    input  logic [`bpam_shire_apb_data_width-1:0]  apb_pwdata,
    output logic                                   apb_pready,
    output logic [`bpam_shire_apb_data_width-1:0]  apb_prdata,
    output logic                                   apb_pslverr
);

localparam ROW_BITS = (`ICACHE_NR_ROWS == 1) ? 1 : `ICACHE_ROW_ADWIDTH;

// INTERNAL DECLARATIONS
logic [`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0]                      icache_req_din;
logic                                                                           f1_req_write;
logic                                                                           f1_req_dbg;
logic                                                                           f1_req_valid;
logic                                                                           f1_req_valid_qual;
logic                                                                           f1_req_ready;
logic                                                                           f1_mbi_ready;
logic                                                                           f2_req_valid;
logic                                                                           f2_req_ready;
logic                                                                           f2_mbi_access;
logic                                                                           f3_req_dbg;
logic                                                                           f3_req_valid;
logic                                                                           f3_req_ready;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] f3_dout;
logic                                                                           f4_req_dbg;
logic                                                                           f4_req_valid;
logic                                                                           f4_req_ready;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] f4_dout;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F0 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
et_link_rsp_info_t                  neigh_sc_rsp_info_reg;
logic                               neigh_sc_rsp_valid_reg;
logic                               neigh_sc_rsp_ready_reg;
logic                               f0_l2_miss_resp_valid;
logic                               f0_l2_miss_resp_done, f0_l2_miss_resp_done_next;
logic                               f0_waiting_l2_miss_resp;
logic                               f0_mbi_rd_en_xx;
logic                               f0_mbi_wr_en_xx;
logic [`ICACHE_MEM_ADWIDTH-1:0]     f0_mbi_addr_xx;
logic [$clog2(`ICACHE_NR_MEMS)-1:0] f0_mbi_mem_idx_xx;
logic [`NUM_NEIGH_R]                f0_mbi_neigh_idx_xx;
logic                               f0_mbi_access_xx;
logic [`ICACHE_MEM_WIDTH-1:0]       f0_mbi_data_xx;
logic                               f0_mbi_ready;
logic                               f0_mbi_accepted;
logic [`ICACHE_NR_MEMS-1:0]         f0_req_ren;
logic [`ICACHE_NR_MEMS-1:0]         f0_req_wen;
logic                               f0_req_write;
logic                               f0_req_dbg;
logic [`ICACHE_MEM_ADWIDTH-1:0]     f0_req_addr;
logic [ROW_BITS-1:0]                f0_req_row;
logic [`ICACHE_SRAM_DATA_WIDTH-1:0] f0_req_din;
logic                               f0_req_valid;
logic                               f0_req_ready;
logic                               f0_req_accepted;

//         CLK    RST    EN                                                          DOUT                    DIN                        DEF
`RST_EN_FF(clock, reset, neigh_sc_rsp_valid_reg^neigh_sc_rsp_valid,                  neigh_sc_rsp_valid_reg, neigh_sc_rsp_valid,        1'b0)
`RST_EN_FF(clock, reset, neigh_sc_rsp_ready_reg^neigh_sc_rsp_ready,                  neigh_sc_rsp_ready_reg, neigh_sc_rsp_ready,        1'b0)
`EN_FF    (clock,        neigh_sc_rsp_valid & neigh_sc_rsp_ready,                    neigh_sc_rsp_info_reg,  neigh_sc_rsp_info)
`RST_EN_FF(clock, reset, f0_l2_miss_resp_valid | (f1_req_valid_qual & f1_req_ready), f0_l2_miss_resp_done,   f0_l2_miss_resp_done_next, 1'b0)

////////////////////////////////////////////////////////////////////////////////
// Spy L2 - Neighborhood response to catch fill to Icache
////////////////////////////////////////////////////////////////////////////////
assign f0_l2_miss_resp_valid = neigh_sc_rsp_valid_reg && neigh_sc_rsp_ready_reg &&
                               (neigh_sc_rsp_info_reg.opcode != ET_LINK_RSP_MsgRcvData) &&
                               (et_link_neigh_source_t'(neigh_sc_rsp_info_reg.dest) == ET_LINK_NEIGH_SRC_Icache);

always_comb
begin
    f0_l2_miss_resp_done_next = f0_l2_miss_resp_done;

    // When miss response is catched, mark it as done
    if (f0_l2_miss_resp_valid)
        f0_l2_miss_resp_done_next = 1'b1;
    // When request goes into the pipeline, clear flag
    else if (f1_req_valid_qual && f1_req_ready)
        f0_l2_miss_resp_done_next = 1'b0;
end

assign f0_waiting_l2_miss_resp = f1_req_write && !f1_req_dbg && !f0_l2_miss_resp_done;

////////////////////////////////////////////////////////////////////////////////
// ECC generator for input data from L2 fills
////////////////////////////////////////////////////////////////////////////////
logic [`ET_LINE_DATA_SIZE-1:0]   neigh_sc_rsp_data;
logic [`ICACHE_ECC_BLOCKS*8-1:0] neigh_sc_rsp_ecc;

genvar ecc_idx;
generate for (ecc_idx = 0; ecc_idx < `ICACHE_ECC_BLOCKS; ecc_idx++)
begin: ECC_BLOCKS
    ecc_gen
    #(
        .DATA_SIZE   ( 64                                           )
    )
    icache_ecc_gen
    (
       .data         ( neigh_sc_rsp_info_reg.data[ecc_idx*64 +: 64] ),
       .data_and_ecc ( {neigh_sc_rsp_ecc[ecc_idx*8 +: 8],
                        neigh_sc_rsp_data[ecc_idx*64 +: 64]}        )
    );
end
endgenerate

////////////////////////////////////////////////////////////////////////////////
// Write info
////////////////////////////////////////////////////////////////////////////////
generate if (`ICACHE_NR_MEMS_PER_ROW > 1)
begin: DIN_WRWIDTH_GT_MWIDTH
    always_comb
    begin
        for(integer unsigned mem = 0 ; mem < `ICACHE_NR_MEMS_PER_ROW; mem++)
            icache_req_din[mem] = {neigh_sc_rsp_ecc [`ICACHE_MEM_ECC_BITS  * mem[`ICACHE_COL_ADWIDTH-1:0] +: `ICACHE_MEM_ECC_BITS ],
                                   neigh_sc_rsp_data[`ICACHE_MEM_USED_BITS * mem[`ICACHE_COL_ADWIDTH-1:0] +: `ICACHE_MEM_USED_BITS]};
    end
end
else
begin: DIN_WRWIDTH_EQ_MWIDTH
    assign icache_req_din[0] = {neigh_sc_rsp_ecc [0 +: `ICACHE_MEM_ECC_BITS ],
                                neigh_sc_rsp_data[0 +: `ICACHE_MEM_USED_BITS]};
end
endgenerate

////////////////////////////////////////////////////////////////////////////////
// Extract row and memory enables from address
////////////////////////////////////////////////////////////////////////////////
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0] icache_req_ren;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0] icache_req_wen;
logic [ROW_BITS-1:0]                                     icache_req_row;

// ADDRESS DECODING
// - Ways of a specific set are stored in consecutive entries in memory
// - If required bandwidth is greater than memory width, more than one memory panel per row is used
// - If memory depth is not enough to store all the sets, more than one row of memory panels is used
//   In that case, consecutive sets are stored consecutively

generate if (`ICACHE_NR_ROWS > 1)
begin: GT1ROW
    assign icache_req_row = icache_req_addr[`ICACHE_MEM_ADWIDTH +: `ICACHE_ROW_ADWIDTH];

    always_comb
    begin
        icache_req_ren = '0;
        icache_req_wen = '0;
        // Enable selected row (the full row is accessed)
        if (icache_req_write)
            icache_req_wen[icache_req_row] = '1;
        else
            icache_req_ren[icache_req_row] = '1;
    end
end
else
begin: ONEROW
    assign icache_req_row = 1'b0;

    // The full row is accessed
    assign icache_req_ren = icache_req_write ? '0 : '1;
    assign icache_req_wen = icache_req_write ? '1 : '0;
end
endgenerate

////////////////////////////////////////////////////////////////////////////////
// Debug access
////////////////////////////////////////////////////////////////////////////////
icache_data_dbg_state_t                                    dbg_state, dbg_state_next;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   dbg_req_ren, dbg_req_ren_next;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   dbg_req_wen, dbg_req_wen_next;
logic                                                      dbg_req_write, dbg_req_write_next;
logic [`ICACHE_DBG_DATA_MEM_CHUNK_ADDR_WIDTH-1:0]          dbg_req_chunk, dbg_req_chunk_next;
logic [`ICACHE_COL_ADWIDTH-1:0]                            dbg_req_col, dbg_req_col_next;
logic                                                      dbg_req_ecc, dbg_req_ecc_next;
logic [`ICACHE_MEM_ADWIDTH-1:0]                            dbg_req_addr, dbg_req_addr_next;
logic [ROW_BITS-1:0]                                       dbg_req_row, dbg_req_row_next;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   dbg_req_access;
logic [`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] dbg_req_din;
logic                                                      dbg_req_valid, dbg_req_valid_next;
logic                                                      dbg_req_ready;
logic [`bpam_shire_apb_data_width-1:0]                     dbg_dout_ecc;
logic [`bpam_shire_apb_data_width-1:0]                     dbg_dout;
logic                                                      apb_pready_next;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   mem_req_ren;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   mem_req_wen;
logic                                                      mem_req_write;
logic [`ICACHE_MEM_ADWIDTH-1:0]                            mem_req_addr;
logic [ROW_BITS-1:0]                                       mem_req_row;
logic                                                      mem_req_dbg;
logic [`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] mem_req_din;
logic                                                      mem_req_valid;
logic                                                      mem_req_ready;

//          CLK    RST    EN                                DOUT           DIN                 DEF
`RST_FF    (clock, reset,                                   dbg_state,     dbg_state_next,     icache_data_dbg_state_Idle)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_ren,   dbg_req_ren_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_wen,   dbg_req_wen_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_write, dbg_req_write_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_chunk, dbg_req_chunk_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_col,   dbg_req_col_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_ecc,   dbg_req_ecc_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_addr,  dbg_req_addr_next)
`EN_FF     (clock,        dbg_req_valid_next,               dbg_req_row,   dbg_req_row_next)
`RST_EN_FF (clock, reset, dbg_req_valid^dbg_req_valid_next, dbg_req_valid, dbg_req_valid_next, 1'b0)
`EN_FF     (clock,        apb_pready_next & ~apb_pwrite,    apb_prdata,    dbg_dout)
`RST_EN_FF (clock, reset, apb_pready^apb_pready_next,       apb_pready,    apb_pready_next,    1'b0)

// FSM
always_comb
begin
    dbg_state_next     = dbg_state;
    dbg_req_write_next = dbg_req_write;
    dbg_req_valid_next = dbg_req_valid;
    apb_pready_next    = apb_pready;

    // Idle
    if (dbg_state == icache_data_dbg_state_Idle)
    begin
        // Generate and store valid debug request
        if (apb_psel)
        begin
            dbg_state_next     = icache_data_dbg_state_WaitReadReady;
            dbg_req_write_next = 1'b0;
            dbg_req_valid_next = 1'b1;
        end
    end
    // Wait for read request to be accepted
    else if (dbg_state == icache_data_dbg_state_WaitReadReady)
    begin
        if (dbg_req_ready)
        begin
            dbg_req_valid_next = 1'b0;
            // Go to read state
            // If it is a write access, we first need to read data to overwrite selected chunk
            dbg_state_next = icache_data_dbg_state_Read;
        end
    end
    // Wait for read data
    else if (dbg_state == icache_data_dbg_state_Read)
    begin
        if (f4_req_valid && f4_req_dbg)
        begin
            if (apb_pwrite)
            begin
                // If it is a write access, overwrite data chunk
                dbg_state_next     = icache_data_dbg_state_WaitWriteReady;
                dbg_req_write_next = 1'b1;
                dbg_req_valid_next = 1'b1;
            end
            else
            begin
                // If it is a read access, send back read data
                dbg_state_next  = icache_data_dbg_state_Done;
                apb_pready_next = 1'b1;
            end
        end
    end
    // Wait for write request to be accepted
    else if (dbg_state == icache_data_dbg_state_WaitWriteReady)
    begin
        if (dbg_req_ready)
        begin
            dbg_req_valid_next = 1'b0;
            // Go to write state
            dbg_state_next = icache_data_dbg_state_Write;
        end
    end
    // Wait for data to be written
    else if (dbg_state == icache_data_dbg_state_Write)
    begin
        if (f4_req_valid && f4_req_dbg)
        begin
            // Acknowledge write
            dbg_state_next  = icache_data_dbg_state_Done;
            apb_pready_next = 1'b1;
        end
    end
    // Disable pready signal and go to idle
    else if (dbg_state == icache_data_dbg_state_Done)
    begin
        dbg_state_next  = icache_data_dbg_state_Idle;
        apb_pready_next = 1'b0;
    end
end

// Decode data chunk (might be ecc chunk), row, col, address and memory enables from APB address
assign dbg_req_chunk_next = apb_paddr[0 +: `ICACHE_DBG_DATA_MEM_CHUNK_ADDR_WIDTH];
assign dbg_req_col_next   = apb_paddr[`ICACHE_DBG_DATA_BLOCK_CHUNK_ADDR_WIDTH-2 -: `ICACHE_COL_ADWIDTH];
assign dbg_req_ecc_next   = apb_paddr[`ICACHE_DBG_DATA_BLOCK_CHUNK_ADDR_WIDTH-1];
assign dbg_req_addr_next  = apb_paddr[`ICACHE_DBG_DATA_BLOCK_CHUNK_ADDR_WIDTH +: `ICACHE_MEM_ADWIDTH];

generate if (`ICACHE_NR_ROWS > 1)
begin: DBG_GT1ROW
    assign dbg_req_row_next = apb_paddr[`ICACHE_DBG_DATA_BLOCK_CHUNK_ADDR_WIDTH+`ICACHE_MEM_ADWIDTH +: `ICACHE_ROW_ADWIDTH];

    always_comb
    begin
        for(integer unsigned row = 0; row < `ICACHE_NR_ROWS; row++)
        begin
            for(integer unsigned mem = 0 ; mem < `ICACHE_NR_MEMS_PER_ROW; mem++)
            begin
                // Enable accessed row and column according to selected chunk
                // If selected chunk is ECC info, enable all columns
                dbg_req_access[row][mem] = (dbg_req_row_next == row[0 +: `ICACHE_ROW_ADWIDTH]) &&
                                           ((dbg_req_col_next == mem[0 +: `ICACHE_COL_ADWIDTH]) || dbg_req_ecc_next);

                dbg_req_ren_next[row][mem] = dbg_req_access[row][mem] & ~dbg_req_write_next;
                dbg_req_wen_next[row][mem] = dbg_req_access[row][mem] & dbg_req_write_next;
            end
        end
    end
end
else
begin: DBG_ONEROW
    assign dbg_req_row_next = 1'b0;

    always_comb
    begin
        for(integer unsigned mem = 0 ; mem < `ICACHE_NR_MEMS_PER_ROW; mem++)
        begin
            // Enable accessed column according to selected chunk
            // If selected chunk is ECC info, enable all columns
            dbg_req_access[0][mem] = (dbg_req_col_next == mem[0 +: `ICACHE_COL_ADWIDTH]) || dbg_req_ecc_next;

            dbg_req_ren_next[0][mem] = dbg_req_access[0][mem] & ~dbg_req_write_next;
            dbg_req_wen_next[0][mem] = dbg_req_access[0][mem] & dbg_req_write_next;
        end
    end
end
endgenerate

// Take debug write data from memory data out
// and overwrite selected chunk
always_comb
begin
    dbg_req_din = f4_dout;
    if (dbg_req_ecc)
    begin
        for (integer i = 0; i < `ICACHE_NR_MEMS_PER_ROW; i++)
            dbg_req_din[i][`ICACHE_MEM_WIDTH-1 -: `ICACHE_MEM_ECC_BITS] = apb_pwdata[i*`ICACHE_MEM_ECC_BITS +: `ICACHE_MEM_ECC_BITS];
    end
    else
        dbg_req_din[dbg_req_col][dbg_req_chunk*`bpam_shire_apb_data_width +: `bpam_shire_apb_data_width] = apb_pwdata;
end

// Take debug read data from memory data out
always_comb
begin
    for (integer i = 0; i < `ICACHE_NR_MEMS_PER_ROW; i++)
        dbg_dout_ecc[i*`ICACHE_MEM_ECC_BITS +: `ICACHE_MEM_ECC_BITS] = f4_dout[dbg_req_row][i][`ICACHE_MEM_WIDTH-1 -: `ICACHE_MEM_ECC_BITS];
end
assign dbg_dout = dbg_req_ecc ? dbg_dout_ecc :
                                f4_dout[dbg_req_row][dbg_req_col][dbg_req_chunk*`bpam_shire_apb_data_width +: `bpam_shire_apb_data_width];
assign apb_pslverr = 1'b0;

assign icache_req_ready = mem_req_ready;
assign dbg_req_ready    = mem_req_ready & ~icache_req_valid;

// Debug muxing
assign mem_req_ren   = icache_req_valid      ? icache_req_ren                            : dbg_req_ren;
assign mem_req_wen   = icache_req_valid      ? icache_req_wen                            : dbg_req_wen;
assign mem_req_write = icache_req_valid      ? icache_req_write                          : dbg_req_write;
assign mem_req_addr  = icache_req_valid      ? icache_req_addr[0 +: `ICACHE_MEM_ADWIDTH] : dbg_req_addr;
assign mem_req_din   = f0_l2_miss_resp_valid ? icache_req_din                            : dbg_req_din;
assign mem_req_row   = icache_req_row;
assign mem_req_dbg   = ~icache_req_valid;
assign mem_req_valid = icache_req_valid | dbg_req_valid;

// BIST and mission mode will not access simultaneously,
// so no need to alternate access
assign mem_req_ready = f0_req_ready;

////////////////////////////////////////////////////////////////////////////////
// BIST muxing
////////////////////////////////////////////////////////////////////////////////
assign f0_mbi_rd_en_xx     = bist_req_info.mbi_sel & bist_req_info.mbi_rd_en;
assign f0_mbi_wr_en_xx     = bist_req_info.mbi_sel & bist_req_info.mbi_wr_en;
assign f0_mbi_addr_xx      = bist_req_info.mbi_addr[0 +: `ICACHE_MEM_ADWIDTH];
assign f0_mbi_mem_idx_xx   = bist_req_info.mbi_addr[`ICACHE_MEM_ADWIDTH +: $clog2(`ICACHE_NR_MEMS)];
assign f0_mbi_neigh_idx_xx = bist_req_info.mbi_addr[`ICACHE_MEM_ADWIDTH + $clog2(`ICACHE_NR_MEMS) +: `NUM_NEIGH_L];
assign f0_mbi_access_xx    = (f0_mbi_rd_en_xx | f0_mbi_wr_en_xx) && (f0_mbi_neigh_idx_xx == ID[`NUM_NEIGH_R]);
assign f0_mbi_data_xx      = bist_req_info.mbi_wdata;
assign f0_mbi_accepted     = f0_mbi_access_xx & f0_mbi_ready;

assign f0_req_ren      = f0_mbi_access_xx ? (1'b1 << f0_mbi_mem_idx_xx) & {`ICACHE_NR_MEMS {f0_mbi_rd_en_xx}} : mem_req_ren;
assign f0_req_wen      = f0_mbi_access_xx ? (1'b1 << f0_mbi_mem_idx_xx) & {`ICACHE_NR_MEMS {f0_mbi_wr_en_xx}} : mem_req_wen;
assign f0_req_write    = f0_mbi_access_xx ? f0_mbi_wr_en_xx                                                   : mem_req_write;
assign f0_req_dbg      = mem_req_dbg;
assign f0_req_addr     = f0_mbi_access_xx ? f0_mbi_addr_xx                                                    : mem_req_addr;
assign f0_req_row      = mem_req_row;
assign f0_req_din      = f0_mbi_access_xx ? {`ICACHE_NR_MEMS_PER_ROW{f0_mbi_data_xx}}                         : mem_req_din;
assign f0_req_valid    = mem_req_valid;
assign f0_req_accepted = f0_req_valid & f0_req_ready;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F1 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   f1_req_ren;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   f1_req_wen;
logic [`ICACHE_MEM_ADWIDTH-1:0]                            f1_req_addr;
logic [ROW_BITS-1:0]                                       f1_req_row;
logic [`ICACHE_NR_MEMS_PER_ROW-1:0][`ICACHE_MEM_WIDTH-1:0] f1_req_din;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   f1_ren;
logic [`ICACHE_NR_ROWS-1:0][`ICACHE_NR_MEMS_PER_ROW-1:0]   f1_wen;
logic                                                      f1_mbi_access;
logic [$clog2(`ICACHE_NR_MEMS)-1:0]                        f1_mbi_mem_idx;

//         CLK    RST    EN                                                         DOUT            DIN               DEF
`RST_EN_FF(clock, reset, f0_req_accepted | f0_mbi_accepted,                         f1_req_ren,     f0_req_ren,       1'b0)
`RST_EN_FF(clock, reset, f0_req_accepted | f0_mbi_accepted,                         f1_req_wen,     f0_req_wen,       1'b0)
`RST_EN_FF(clock, reset, f0_req_accepted | f0_mbi_accepted,                         f1_req_addr,    f0_req_addr,      '0)
`EN_FF    (clock,        f0_req_accepted,                                           f1_req_row,     f0_req_row)
`EN_FF    (clock,        f0_req_accepted | f0_mbi_accepted | f0_l2_miss_resp_valid, f1_req_din,     f0_req_din)
`RST_EN_FF(clock, reset, f0_req_accepted | f0_mbi_accepted,                         f1_req_write,   f0_req_write,     1'b0)
`RST_EN_FF(clock, reset, f0_req_accepted,                                           f1_req_dbg,     f0_req_dbg,       1'b0)
`RST_EN_FF(clock, reset, f0_req_ready & (f1_req_valid^f0_req_valid),                f1_req_valid,   f0_req_valid,     1'b0)
`RST_EN_FF(clock, reset, f0_mbi_ready,                                              f1_mbi_access,  f0_mbi_access_xx, 1'b0)
`EN_FF    (clock,        f0_mbi_accepted,                                           f1_mbi_mem_idx, f0_mbi_mem_idx_xx)

// Store request in F1 until it is ready to go
// (it may have to wait for write data from L2 response)
assign f0_req_ready = (f1_req_ready && !f0_waiting_l2_miss_resp) || !f1_req_valid;
assign f0_mbi_ready = f1_mbi_ready || !f1_mbi_access;

assign f1_ren = f1_req_ren & {`ICACHE_NR_MEMS{f1_req_valid_qual | f1_mbi_access}};
assign f1_wen = f1_req_wen & {`ICACHE_NR_MEMS{f1_req_valid_qual | f1_mbi_access}};

assign f1_req_valid_qual = f1_req_valid && !f0_waiting_l2_miss_resp;

////////////////////////////////////////////////////////////////////////////////
// Memory instantiation
////////////////////////////////////////////////////////////////////////////////
genvar r, j;
generate for (r = 0; r < `ICACHE_NR_ROWS; r++)
begin : ROWS
    for (j = 0 ; j < `ICACHE_NR_MEMS_PER_ROW; j++)
    begin : MEM_ROW
        logic icache_f1_mem_en;
        logic icache_f1_mem_we;
        logic clock_en;
        logic sram_clock_pre;
        logic sram_clock;
        logic sram_clock_en;

        assign icache_f1_mem_en = f1_ren[r][j] || f1_wen[r][j];
        assign icache_f1_mem_we = f1_wen[r][j];

        assign clock_en      = icache_f1_mem_en & ~sram_clock_pre; // NOTE: iache_f1_mem_en is on for 2 cycles, so only generate clock on first of two clocks that the enable is high
        assign sram_clock_en = sram_clock_pre || clock_en;

        //         CLK    RST    EN             DOUT            DIN       DEF
        `RST_EN_FF(clock, reset, sram_clock_en, sram_clock_pre, clock_en, 1'b0)

        et_clk_mux2 clk_mux (.x(sram_clock), .d0(sram_clock_pre), .d1(dft__sram_clock), .s(dft__clk_override));

        // synopsys translate_off
        logic clock_en_delay;

        //         CLK    RST    EN                           DOUT            DIN       DEF
        `RST_EN_FF(clock, reset, |{clock_en_delay, clock_en}, clock_en_delay, clock_en, 1'b0)

        // Check that memory clock enable is not asserted in consecutive clock cycles
        `ASSERT_ALWAYS(ERROR_ICACHE_DATA_RAM_clock_en_busy_check, clock, reset, clock_en |-> !clock_en_delay )
        // synopsys translate_on

        icache_data_ram  icache_data_ram  (
          .clk  ( sram_clock                                                ),
          .addr ( f1_req_addr                                               ),
          .ce   ( icache_f1_mem_en && !esr_shire_cache_ram_cfg.dft__ram_rei ),
          .we   ( icache_f1_mem_we && !esr_shire_cache_ram_cfg.dft__ram_wei ),
          .din  ( f1_req_din[j]                                             ),
          .dout ( f3_dout[r][j]                                             )
        );
    end // block: MEM_ROW
end // block: ROWS
endgenerate

// Memory is 2-cycle access, thus F2 is a memory wait stage
// We need to wait for F2 stage to be valid and for F3 stage to be ready
assign f1_req_ready = f2_req_valid && f2_req_ready;
// For BIST access, we just need to wait for F2 stage to be valid
assign f1_mbi_ready = f2_mbi_access;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F2 Stage (Memory wait stage)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
logic [ROW_BITS-1:0]                f2_req_row;
logic [`ICACHE_SRAM_DATA_WIDTH-1:0] f2_req_din;
logic                               f2_req_write;
logic                               f2_req_dbg;
logic                               f2_req_valid_next;
logic                               f2_req_accepted;
logic                               f2_mbi_rd_valid;
logic                               f2_mbi_rd_valid_next;
logic [$clog2(`ICACHE_NR_MEMS)-1:0] f2_mbi_mem_idx;
logic                               f2_mbi_access_next;

// Propagate signals from F1 to F2
assign f2_req_row     = f1_req_row;
assign f2_req_din     = f1_req_din;
assign f2_req_write   = f1_req_write;
assign f2_req_dbg     = f1_req_dbg;
assign f2_mbi_mem_idx = f1_mbi_mem_idx;

//         CLK    RST    EN                                    DOUT             DIN                   DEF
`RST_EN_FF(clock, reset, f2_req_valid^f2_req_valid_next,       f2_req_valid,    f2_req_valid_next,    1'b0)
`RST_EN_FF(clock, reset, f2_mbi_access^f2_mbi_access_next,     f2_mbi_access,   f2_mbi_access_next,   1'b0)
`RST_EN_FF(clock, reset, f2_mbi_rd_valid^f2_mbi_rd_valid_next, f2_mbi_rd_valid, f2_mbi_rd_valid_next, 1'b0)

always_comb
begin
    // Propagate valid from F1 when new valid data is loaded
    if (!f2_req_valid)
        f2_req_valid_next = f1_req_valid_qual;
    // Deassert F2 valid when it is read by F3
    else
        f2_req_valid_next = ~f2_req_ready;

    // Propagate BIST access from F1 when new access happens
    if (!f2_mbi_access)
        f2_mbi_access_next = f1_mbi_access;
    else
        f2_mbi_access_next = 1'b0;
end
assign f2_mbi_rd_valid_next = f2_mbi_access_next && |f1_req_ren;

assign f2_req_accepted = f2_req_valid && f2_req_ready;
assign f2_req_ready    = f3_req_ready || !f3_req_valid;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F3 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
logic [ROW_BITS-1:0]                f3_req_row;
logic [`ICACHE_SRAM_DATA_WIDTH-1:0] f3_req_din;
logic                               f3_req_write;
logic                               f3_req_accepted;
logic                               f3_mbi_rd_valid;
logic [$clog2(`ICACHE_NR_MEMS)-1:0] f3_mbi_mem_idx;

//         CLK    RST    EN                                          DOUT             DIN              DEF
`EN_FF    (clock,        f2_req_accepted,                            f3_req_row,      f2_req_row)
`EN_FF    (clock,        f2_req_accepted,                            f3_req_din,      f2_req_din)
`EN_FF    (clock,        f2_req_accepted,                            f3_req_write,    f2_req_write)
`EN_FF    (clock,        f2_req_accepted,                            f3_req_dbg,      f2_req_dbg)
`RST_EN_FF(clock, reset, f2_req_ready & (f3_req_valid^f2_req_valid), f3_req_valid,    f2_req_valid,    1'b0)
`RST_EN_FF(clock, reset, f3_mbi_rd_valid^f2_mbi_rd_valid,            f3_mbi_rd_valid, f2_mbi_rd_valid, 1'b0)
`EN_FF    (clock,        f2_mbi_rd_valid,                            f3_mbi_mem_idx,  f2_mbi_mem_idx)

assign f3_req_accepted = f3_req_valid && f3_req_ready;
assign f3_req_ready    = f4_req_ready || !f4_req_valid;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F4 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
logic [ROW_BITS-1:0]                               f4_req_row;
logic [`ICACHE_SRAM_DATA_WIDTH-1:0]                f4_req_din;
logic                                              f4_req_write;
logic [`ICACHE_NR_MEMS-1:0][`ICACHE_MEM_WIDTH-1:0] f4_mbi_dout;
logic                                              f4_mbi_rd_valid;
logic [$clog2(`ICACHE_NR_MEMS)-1:0]                f4_mbi_mem_idx;

//         CLK    RST    EN                                          DOUT             DIN              DEF
`EN_FF    (clock,        f3_req_accepted,                            f4_req_row,      f3_req_row)
`EN_FF    (clock,        f3_req_accepted,                            f4_req_din,      f3_req_din)
`EN_FF    (clock,        f3_req_accepted,                            f4_req_write,    f3_req_write)   
`EN_FF    (clock,        f3_req_accepted,                            f4_req_dbg,      f3_req_dbg)
`EN_FF    (clock,        f3_req_accepted | f3_mbi_rd_valid,          f4_dout,         f3_dout)
`RST_EN_FF(clock, reset, f3_req_ready & (f4_req_valid^f3_req_valid), f4_req_valid,    f3_req_valid,    1'b0)
`RST_EN_FF(clock, reset, f4_mbi_rd_valid^f3_mbi_rd_valid,            f4_mbi_rd_valid, f3_mbi_rd_valid, 1'b0)
`EN_FF    (clock,        f3_mbi_rd_valid,                            f4_mbi_mem_idx,  f3_mbi_mem_idx)

// Send data to icache response output
// -----------------------------------
// Write data is also sent back so that it's filled into L0 micro cache
assign icache_resp_dout  = f4_req_write ? f4_req_din : f4_dout[f4_req_row];
assign icache_resp_valid = f4_req_valid & ~f4_req_dbg;

// Send data to BIST output
// ------------------------
assign f4_mbi_dout = f4_dout;

assign f4_req_ready = icache_resp_ready;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F5 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
logic                         f5_mbi_rd_valid;
logic [`ICACHE_MEM_WIDTH-1:0] f5_mbi_rd_data;

//         CLK    RST    EN                               DOUT             DIN              DEF
`RST_EN_FF(clock, reset, f5_mbi_rd_valid^f4_mbi_rd_valid, f5_mbi_rd_valid, f4_mbi_rd_valid, 1'b0)
`EN_FF    (clock,        f4_mbi_rd_valid,                 f5_mbi_rd_data,  f4_mbi_dout[f4_mbi_mem_idx])

// Send data to BIST output
// ------------------------
// NOTE: only send rd_data when rd_valid because this output is OR'd one level
// of hierarchy up across all icache_data_ram_wrap modules to send a single 
// rd_data back to the bist interface
assign bist_rsp_info.mbi_rdata = {`ICACHE_MEM_WIDTH{f5_mbi_rd_valid}} & f5_mbi_rd_data; 

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Power Stats
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// synopsys translate_off

// configure verbosity level
logic  verbose;
int    core_verbose_level;
int    default_verbose_level;
int    global_verbose_level;
logic  global_found;

initial
begin
    verbose = 1'b0;
end

always @(negedge reset)
begin
   if (!`et_value_plusargs("GLOBAL_LOG_LEVEL=%d", global_verbose_level))
       global_verbose_level = 0; //DEBUG
   else
       global_found = 1'b1;

   if (!`et_value_plusargs("DEFAULT_LOG_LEVEL=%d", default_verbose_level))
       default_verbose_level = global_found? global_verbose_level : 1 /*INFO*/;

   if (!`et_value_plusargs("CORE_RTL_LOG_LEVEL=%d", core_verbose_level) ) 
       core_verbose_level = default_verbose_level;

   // and only show messages if core verbose level = debug, and global verbose level = debug
   verbose = (core_verbose_level == 0) && (global_verbose_level == 0);
end

always @(negedge clock)
begin
     if (verbose) begin
          if(!f1_req_write & f1_req_valid_qual & f1_req_ready)
            $display("POWER STAT: Icache read: %d bits", `ICACHE_BLOCK_BITS);

          if(f1_req_write & f1_req_valid_qual & f1_req_ready)
            $display("POWER STAT: Icache write: %d bits", `ICACHE_BLOCK_BITS);
     end
end

// synopsys translate_on


// LINT
wire unused_ok = &{
   `ifndef ET_ASCENT_LINT
   1'b0,
   `endif
   apb_penable, // unused
   bist_req_info.mbist_on, // possible usage for enabling clock gater if required
   esr_shire_cache_ram_cfg, // not all bits of this struct are used
   neigh_sc_rsp_info_reg.wdata,
   neigh_sc_rsp_info_reg.qwen,
   neigh_sc_rsp_info_reg.size,
   neigh_sc_rsp_info_reg.id
};


endmodule
