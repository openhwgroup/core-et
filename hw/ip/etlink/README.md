# etlink

ET-Link bus protocol type definitions. This is a protocol definition only — there is no ET-Link RTL module.

ET-Link is the request/response interconnect protocol used between IP blocks (RBOX, TBOX, minion cores, etc.) and the Shire Cache. It uses a valid/ready handshake with structured request and response packets.

## Package: `etlink_pkg`

Import with `import etlink_pkg::*;` or use qualified names like `etlink_pkg::req_t`.

### Types

| Type | Description |
|------|-------------|
| `req_t` | Full request (8-bit ID, 4-bit source, 40-bit address, 512-bit data) |
| `neigh_req_t` | Neighborhood request (8-bit ID, 4-bit source, 40-bit address, 256-bit data) |
| `rsp_t` | Full response (8-bit ID, 4-bit dest, 512-bit data) |
| `neigh_rsp_t` | Neighborhood response (8-bit ID, 4-bit dest, 256-bit data) |
| `req_nodata_t` | Address-only request (no data field) |
| `rsp_nodata_t` | Ack-only response (no data field) |

### Enums

| Enum | Values |
|------|--------|
| `req_opcode_e` | `ReqRead`, `ReqWrite`, `ReqMsgSendData`, `ReqFlush`, etc. (17 opcodes) |
| `rsp_opcode_e` | `RspAck`, `RspAckData`, `RspMsgRcvData`, `RspErr` |
| `size_e` | `SizeByte` through `SizeLine` (8b to 512b) |
| `neigh_source_e` | `SrcMinion0`..`SrcMinion7`, `SrcIcache`, `SrcPtw0`/`1`, `SrcTbox`, `SrcCoop` |

### Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `PaSize` | 40 | Physical address width |
| `IdSize` | 8 | Request/response ID width |
| `SourceSize` | 4 | Source/destination field width |
| `DataSize` | 512 | Cache line data width |
| `NeighDataSize` | 256 | Neighborhood message data width |

### Protocol

```
         Requester                    Responder
            │                             │
            │──── req (valid/ready) ─────>│
            │     {opcode, addr, data}    │
            │                             │
            │<─── rsp (valid/ready) ──────│
            │     {opcode, data}          │
            │                             │
```

- **Read:** `ReqRead` → `RspAckData` (response carries data)
- **Write:** `ReqWrite` → `RspAck` (response acknowledges write)
- **Message:** `ReqMsgSendData` → `RspAck` (send data to HART message port)
- Valid/ready handshake: transfer occurs when both valid and ready are high on the rising edge.
- ID field is used to route responses back to the requester (e.g., FIFO slot, HART ID).

### Differences from original CORE-ET `et_link_types.vh`

| Aspect | Original | New | Rationale |
|--------|----------|-----|-----------|
| Definition style | `define macro with parameterized widths | SV package with fixed widths | Clean, no macro pollution |
| Naming | `et_link_req_info_t`, `ET_LINK_REQ_Read` | `etlink_pkg::req_t`, `etlink_pkg::ReqRead` | lowRISC naming convention |
| Variants | 5+ typedef variants per struct (different data widths) | `req_t` / `rsp_t` (full), `neigh_req_t` / `neigh_rsp_t` (256-bit), `req_nodata_t` | Keep the common variants explicit while avoiding macro-generated typedef sprawl |
| System constants | Scattered across `shire_defines.vh`, `et_link_defines.vh` | All in `etlink_pkg` | Self-contained package |
