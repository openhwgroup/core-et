# axi

AXI4 protocol type definitions.

## Package: `axi_pkg`

Defines parameterized AXI4 channel structs and shire-cache-specific instantiations.

### Constants

| Constant | Value | Description |
|----------|-------|-------------|
| `ScMasterIdSize` | 9 | Master port ID width (2-3 bank + 6 reqq ID) |
| `ScMasterAddrSize` | 40 | Address width |
| `ScMasterDataSize` | 512 | Data width (cache line) |
| `ScSlaveIdSize` | 19 | Slave port ID (master + bridge source bits) |
| `MeshSourceBridgeIdSize` | 10 | Bridge source ID width |

### Channel types

| Type | Description |
|------|-------------|
| `sc_master_ar_t` | Master read address channel |
| `sc_master_aw_t` | Master write address channel |
| `sc_master_w_t` | Master write data channel |
| `sc_master_r_t` | Master read data channel |
| `sc_master_b_t` | Master write response channel |
| `sc_slave_ar_t` | Slave read address channel (wider ID) |
| `sc_slave_aw_t` | Slave write address channel (wider ID) |
| `sc_slave_r_t` | Slave read data channel (wider ID) |
| `sc_slave_b_t` | Slave write response channel (wider ID) |
| `sc_master_port_t` | Composite master port (all 5 channels + valid/ready) |
| `sc_slave_port_t` | Composite slave port (all 5 channels + valid/ready) |

### Response enum

`resp_e`: `RespOkay`, `RespExOkay`, `RespSlvErr`, `RespDecErr`

### Differences from original

| Original | New | Rationale |
|----------|-----|-----------|
| `axi_defines.vh` + `axi_types.vh` | `axi_pkg.sv` | SV package, no defines |
| `SC_MESH_MASTER_AXI_*` defines | `ScMaster*` localparams | UpperCamelCase params |
| `SC_AXI_Ax_CHANNEL_TYPE_DEF` macro | Explicit struct typedefs | No macro templates |
| `axi_resp_t` | `resp_e` | lowRISC enum naming |
