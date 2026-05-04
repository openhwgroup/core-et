wire [512:0] w_raw_data  = {{(512-DATA_WIDTH){1'b0}}, w_data};
wire [64:0]  w_raw_strb  = {{(64-WSTRB_WIDTH){1'b0}}, w_strb};
wire [511:0] w_strb_data = w_raw_data[511:0] & { {8{w_raw_strb[63]}}, {8{w_raw_strb[62]}}, {8{w_raw_strb[61]}}, {8{w_raw_strb[60]}},
                                                 {8{w_raw_strb[59]}}, {8{w_raw_strb[58]}}, {8{w_raw_strb[57]}}, {8{w_raw_strb[56]}},
                                                 {8{w_raw_strb[55]}}, {8{w_raw_strb[54]}}, {8{w_raw_strb[53]}}, {8{w_raw_strb[52]}},
                                                 {8{w_raw_strb[51]}}, {8{w_raw_strb[50]}}, {8{w_raw_strb[49]}}, {8{w_raw_strb[48]}},
                                                 {8{w_raw_strb[47]}}, {8{w_raw_strb[46]}}, {8{w_raw_strb[45]}}, {8{w_raw_strb[44]}},
                                                 {8{w_raw_strb[43]}}, {8{w_raw_strb[42]}}, {8{w_raw_strb[41]}}, {8{w_raw_strb[40]}},
                                                 {8{w_raw_strb[39]}}, {8{w_raw_strb[38]}}, {8{w_raw_strb[37]}}, {8{w_raw_strb[36]}},
                                                 {8{w_raw_strb[35]}}, {8{w_raw_strb[34]}}, {8{w_raw_strb[33]}}, {8{w_raw_strb[32]}},
                                                 {8{w_raw_strb[31]}}, {8{w_raw_strb[30]}}, {8{w_raw_strb[29]}}, {8{w_raw_strb[28]}},
                                                 {8{w_raw_strb[27]}}, {8{w_raw_strb[26]}}, {8{w_raw_strb[25]}}, {8{w_raw_strb[24]}},
                                                 {8{w_raw_strb[23]}}, {8{w_raw_strb[22]}}, {8{w_raw_strb[21]}}, {8{w_raw_strb[20]}},
                                                 {8{w_raw_strb[19]}}, {8{w_raw_strb[18]}}, {8{w_raw_strb[17]}}, {8{w_raw_strb[16]}},
                                                 {8{w_raw_strb[15]}}, {8{w_raw_strb[14]}}, {8{w_raw_strb[13]}}, {8{w_raw_strb[12]}},
                                                 {8{w_raw_strb[11]}}, {8{w_raw_strb[10]}}, {8{w_raw_strb[9]}},  {8{w_raw_strb[8]}},
                                                 {8{w_raw_strb[7]}},  {8{w_raw_strb[6]}},  {8{w_raw_strb[5]}},  {8{w_raw_strb[4]}},
                                                 {8{w_raw_strb[3]}},  {8{w_raw_strb[2]}},  {8{w_raw_strb[1]}},  {8{w_raw_strb[0]}} };

`ASSERT_ALWAYS_MSG(ass_aw_id_valid, clk, reset, ((aw_valid == 1) |-> !($isunknown(aw_id))), "AW_ID signal is not valid when AW_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_aw_addr_valid, clk, reset,
//    ((aw_valid == 1) |-> (aw_addr[0] !== 1'bx) && (aw_addr[1] !== 1'bx) && (aw_addr[2] !== 1'bx) && (aw_addr[3] !== 1'bx) && (aw_addr[4] !== 1'bx) && (aw_addr[5] !== 1'bx) && (aw_addr[6] !== 1'bx) && (aw_addr[7] !== 1'bx) && (aw_addr[8] !== 1'bx) && (aw_addr[9] !== 1'bx) && (aw_addr[10] !== 1'bx) && (aw_addr[11] !== 1'bx) && (aw_addr[12] !== 1'bx) && (aw_addr[13] !== 1'bx) && (aw_addr[14] !== 1'bx) && (aw_addr[15] !== 1'bx) && (aw_addr[16] !== 1'bx) && (aw_addr[17] !== 1'bx) && (aw_addr[18] !== 1'bx) && (aw_addr[19] !== 1'bx) && (aw_addr[20] !== 1'bx) && (aw_addr[21] !== 1'bx) && (aw_addr[22] !== 1'bx) && (aw_addr[23] !== 1'bx) && (aw_addr[24] !== 1'bx) && (aw_addr[25] !== 1'bx) && (aw_addr[26] !== 1'bx) && (aw_addr[27] !== 1'bx) && (aw_addr[28] !== 1'bx) && (aw_addr[29] !== 1'bx) && (aw_addr[30] !== 1'bx) && (aw_addr[31] !== 1'bx) && (aw_addr[32] !== 1'bx) && (aw_addr[33] !== 1'bx) && (aw_addr[34] !== 1'bx) && (aw_addr[35] !== 1'bx) && (aw_addr[36] !== 1'bx) && (aw_addr[37] !== 1'bx) && (aw_addr[38] !== 1'bx) && (aw_addr[39] !== 1'bx) ),
//    "AW_ADDR signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_len_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_len))),
   "AW_LEN signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_size_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_size))),
   "AW_SIZE signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_burst_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_burst))),
   "AW_BURST signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_lock_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_lock))),
   "AW_LOCK signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_cache_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_cache))),
   "AW_CACHE signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_prot_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_prot))),
   "AW_PROT signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_qos_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_qos))),
   "AW_QOS signal is not valid when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_region_valid, clk, reset,
   ((aw_valid == 1) |-> !($isunknown(aw_region))),
   "AW_REGION signal is not valid when AW_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_aw_user_valid, clk, reset,
//    ((aw_valid == 1) |-> !($isunknown(aw_user))),
//    "AW_USER signal is not valid when AW_VALID is asserted");

// Allow dataless write request
`ASSERT_ALWAYS_MSG(ass_w_data_valid, clk, reset,
   ((w_valid == 1) |-> (w_strb[WSTRB_WIDTH-1:0] == {WSTRB_WIDTH{1'h0}}) || (!($isunknown(w_strb_data[DATA_WIDTH-1:0])))),
   "W_DATA signal is not valid when W_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_w_strb_valid, clk, reset,
   ((w_valid == 1) |-> !($isunknown(w_strb))),
   "W_STRB signal is not valid when W_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_w_last_valid, clk, reset,
   ((w_valid == 1) |-> !($isunknown(w_last))),
   "W_LAST signal is not valid when W_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_w_user_valid, clk, reset,
//    ((w_valid == 1) |-> !($isunknown(w_user))),
//    "W_USER signal is not valid when W_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_b_id_valid, clk, reset,
   ((b_valid == 1) |-> !($isunknown(b_id))),
   "B_ID signal is not valid when B_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_b_resp_valid, clk, reset,
   ((b_valid == 1) |-> !($isunknown(b_resp))),
   "B_RESP signal is not valid when B_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_b_user_valid, clk, reset,
//    ((b_valid == 1) |-> !($isunknown(b_user))),
//    "B_USER signal is not valid when B_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_id_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_id))),
   "AR_ID signal is not valid when AR_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_ar_addr_valid, clk, reset,
//    ((ar_valid == 1) |-> (ar_addr[0] !== 1'bx) && (ar_addr[1] !== 1'bx) && (ar_addr[2] !== 1'bx) && (ar_addr[3] !== 1'bx) && (ar_addr[4] !== 1'bx) && (ar_addr[5] !== 1'bx) && (ar_addr[6] !== 1'bx) && (ar_addr[7] !== 1'bx) && (ar_addr[8] !== 1'bx) && (ar_addr[9] !== 1'bx) && (ar_addr[10] !== 1'bx) && (ar_addr[11] !== 1'bx) && (ar_addr[12] !== 1'bx) && (ar_addr[13] !== 1'bx) && (ar_addr[14] !== 1'bx) && (ar_addr[15] !== 1'bx) && (ar_addr[16] !== 1'bx) && (ar_addr[17] !== 1'bx) && (ar_addr[18] !== 1'bx) && (ar_addr[19] !== 1'bx) && (ar_addr[20] !== 1'bx) && (ar_addr[21] !== 1'bx) && (ar_addr[22] !== 1'bx) && (ar_addr[23] !== 1'bx) && (ar_addr[24] !== 1'bx) && (ar_addr[25] !== 1'bx) && (ar_addr[26] !== 1'bx) && (ar_addr[27] !== 1'bx) && (ar_addr[28] !== 1'bx) && (ar_addr[29] !== 1'bx) && (ar_addr[30] !== 1'bx) && (ar_addr[31] !== 1'bx) && (ar_addr[32] !== 1'bx) && (ar_addr[33] !== 1'bx) && (ar_addr[34] !== 1'bx) && (ar_addr[35] !== 1'bx) && (ar_addr[36] !== 1'bx) && (ar_addr[37] !== 1'bx) && (ar_addr[38] !== 1'bx) && (ar_addr[39] !== 1'bx) ),
//    "AR_ADDR signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_len_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_len))),
   "AR_LEN signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_size_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_size))),
   "AR_SIZE signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_burst_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_burst))),
   "AR_BURST signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_lock_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_lock))),
   "AR_LOCK signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_cache_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_cache))),
   "AR_CACHE signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_prot_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_prot))),
   "AR_PROT signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_qos_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_qos))),
   "AR_QOS signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_region_valid, clk, reset,
   ((ar_valid == 1) |-> !($isunknown(ar_region))),
   "AR_REGION signal is not valid when AR_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_ar_user_valid, clk, reset,
//    ((ar_valid == 1) |-> !($isunknown(ar_user))),
//    "AR_USER signal is not valid when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_r_id_valid, clk, reset,
   ((r_valid == 1) |-> !($isunknown(r_id))),
   "R_ID signal is not valid when R_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_r_data_valid, clk, reset,
//    ((r_valid == 1) |-> (!($isunknown(r_data)) && ar_size == 3'b110) || ((!($isunknown(r_data[511:448])) || !($isunknown(r_data[63:0]))) && ar_size == 3'b011)),
//    "R_DATA signal is not valid when R_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_r_resp_valid, clk, reset,
   ((r_valid == 1) |-> !($isunknown(r_resp))),
   "R_RESP signal is not valid when R_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_r_last_valid, clk, reset,
   ((r_valid == 1) |-> !($isunknown(r_last))),
   "R_LAST signal is not valid when R_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_r_user_valid, clk, reset,
//    ((r_valid == 1) |-> !($isunknown(r_user))),
//    "R_USER signal is not valid when R_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_id_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_id) == aw_id ),
   "AW_ID signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_addr_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_addr) == aw_addr ),
   "AW_ADDR signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_len_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_len) == aw_len ),
   "AW_LEN signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_size_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_size) == aw_size ),
   "AW_SIZE signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_burst_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_burst) == aw_burst ),
   "AW_BURST signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_lock_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_lock) == aw_lock ),
   "AW_LOCK signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_cache_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_cache) == aw_cache ),
   "AW_CACHE signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_prot_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_prot) == aw_prot ),
   "AW_PROT signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_qos_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_qos) == aw_qos ),
   "AW_QOS signal is not stable when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_aw_region_stable, clk, reset,
   ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_region) == aw_region ),
   "AW_REGION signal is not stable when AW_VALID is asserted");

// FUTURE: Paul Gerlach broken
// `ASSERT_ALWAYS_MSG(ass_aw_user_stable, clk, reset,
//    ((aw_valid == 1) && (aw_ready == 0) && $past(aw_valid == 1) && $past(aw_ready == 0) |-> $past(aw_user) == aw_user ),
//    "AW_USER signal is not stable when AW_VALID is asserted");

// FUTURE: Paul Gerlach broken
// `ASSERT_ALWAYS_MSG(ass_w_data_stable, clk, reset,
//    ((w_valid == 1) && (w_ready == 0) && $past(w_valid == 1) && $past(w_ready == 0) |-> w_strb_data == $past(w_strb_data)),
//    "W_DATA signal is not stable when W_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_w_strb_stable, clk, reset,
   ((w_valid == 1) && (w_ready == 0) && $past(w_valid == 1) && $past(w_ready == 0) |-> $past(w_strb) == w_strb ),
   "W_STRB signal is not stable when W_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_w_last_stable, clk, reset,
   ((w_valid == 1) && (w_ready == 0) && $past(w_valid == 1) && $past(w_ready == 0) |-> $past(w_last) == w_last ),
   "W_LAST signal is not stable when W_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_w_user_stable, clk, reset,
//    ((w_valid == 1) && (w_ready == 0) && $past(w_valid == 1) && $past(w_ready == 0) |-> $past(w_user) == w_user ),
//    "W_USER signal is not stable when W_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_b_id_stable, clk, reset,
   ((b_valid == 1) && (b_ready == 0) && $past(b_valid == 1) && $past(b_ready == 0) |-> $past(b_id) == b_id ),
   "B_ID signal is not stable when B_VALID is asserted");


`ASSERT_ALWAYS_MSG(ass_b_resp_stable, clk, reset,
   ((b_valid == 1) && (b_ready == 0) && $past(b_valid == 1) && $past(b_ready == 0) |-> $past(b_resp) == b_resp ),
   "B_RESP signal is not stable when B_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_b_user_stable, clk, reset,
//    ((b_valid == 1) && (b_ready == 0) && $past(b_valid == 1) && $past(b_ready == 0) |-> $past(b_user) == b_user ),
//    "B_USER signal is not stable when B_VALID is asserted");

// `ASSERT_ALWAYS_MSG(ass_w_strb_0, clk, reset,
//    ( (w_valid == 0) && $past(w_valid == 1) && ($isunknown($past(w_strb)) == 0) |->  ( (w_strb == 0) || (w_strb === $past(w_strb)) ) ),
//    "W_STRB signal is not stable after W_VALID is de-asserted");

`ASSERT_ALWAYS_MSG(ass_aw_burst_value, clk, reset,
   ( (aw_valid == 1) |-> (aw_burst[1:0] != 2'b11) ),
   "AW_BURST signal has illegal value when AW_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_burst_value, clk, reset,
   ( (ar_valid == 1) |-> (ar_burst[1:0] != 2'b11) ),
   "AR_BURST signal has illegal value when AR_VALID is asserted");

`ASSERT_ALWAYS_MSG(ass_ar_valid_reset, clk, reset,
   ( (reset == 1) |-> (ar_valid !== 1) ),
   "AR_VALID signal is asserted during RESET");

`ASSERT_ALWAYS_MSG(ass_ar_valid_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(ar_valid)) ),
   "AR_VALID signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_ar_ready_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(ar_ready)) ),
   "AR_READY signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_aw_valid_reset, clk, reset,
   ( (reset == 1) |-> (aw_valid !== 1) ),
   "AW_VALID signal is asserted during RESET");

`ASSERT_ALWAYS_MSG(ass_aw_valid_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(aw_valid)) ),
   "AW_VALID signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_aw_ready_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(aw_ready)) ),
   "AW_READY signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_w_valid_reset, clk, reset,
   ( (reset == 1) |-> (w_valid !== 1) ),
   "W_VALID signal is asserted during RESET");

`ASSERT_ALWAYS_MSG(ass_w_valid_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(w_valid)) ),
   "W_VALID signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_w_ready_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(w_ready)) ),
   "W_READY signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_r_valid_reset, clk, reset,
   ( (reset == 1) |-> (r_valid !== 1) ),
   "R_VALID signal is asserted during RESET");

`ASSERT_ALWAYS_MSG(ass_r_valid_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(r_valid)) ),
   "R_VALID signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_r_ready_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(r_ready)) ),
   "R_READY signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_b_valid_reset, clk, reset,
   ( (reset == 1) |-> (b_valid !== 1) ),
   "B_VALID signal is asserted during RESET");

`ASSERT_ALWAYS_MSG(ass_b_valid_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(b_valid)) ),
   "B_VALID signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_b_ready_out_reset, clk, reset,
   ( (reset == 0) |-> !($isunknown(b_ready)) ),
   "B_READY signal is not valid out of RESET");

`ASSERT_ALWAYS_MSG(ass_aw_valid_ready, clk, reset,
   ( (aw_valid == 1) && (aw_ready == 0) |=> (aw_valid == 1) ),
   "AW_VALID was de-asserted before AW_READY signal was asserted");

`ASSERT_ALWAYS_MSG(ass_ar_valid_ready, clk, reset,
   ( (ar_valid == 1) && (ar_ready == 0) |=> (ar_valid == 1) ),
   "AR_VALID was de-asserted before AR_READY signal was asserted");

`ASSERT_ALWAYS_MSG(ass_w_valid_ready, clk, reset,
   ( (w_valid == 1) && (w_ready == 0) |=> (w_valid == 1) ),
   "W_VALID was de-asserted before W_READY signal was asserted");

`ASSERT_ALWAYS_MSG(ass_r_valid_ready, clk, reset,
   ( (r_valid == 1) && (r_ready == 0) |=> (r_valid == 1) ),
   "R_VALID was de-asserted before R_READY signal was asserted");

`ASSERT_ALWAYS_MSG(ass_b_valid_ready, clk, reset,
   ( (b_valid == 1) && (b_ready == 0) |=> (b_valid == 1) ),
   "B_VALID was de-asserted before B_READY signal was asserted");
