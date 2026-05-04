// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0



typedef struct packed {
   logic                     rnw;
   logic                     wa;
   logic [`PA_RANGE]         address;
   logic [`SC_LINE_DATA_SIZE-1:0] data;
} mesh_request_conf_t;

typedef struct packed {
   mesh_request_conf_t request;
   logic [`L2_MESH_REQ_ID_R] id;
} l2_mesh_request_t;

typedef struct packed {
   logic [`SC_LINE_DATA_SIZE-1:0] data;
   logic [`L2_MESH_REQ_ID_R] id;   
} l2_mesh_reply_t;

typedef struct packed {
   logic [`NUM_SHIRE_IDS_R] shire_id;
   logic [`SC_BANK_ID_SIZE-1:0]   l2_bank;
   l2_mesh_request_t info;
} mesh_request_t;

typedef struct packed {
   logic [`NUM_SHIRES_R] shire_id;
   logic [`SC_BANK_ID_SIZE-1:0] l2_bank;
   logic               uc;
   l2_mesh_reply_t info;
} mesh_reply_t;
