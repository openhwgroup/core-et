`ifndef __EVL_MEMSHIRE_PKG

`define __EVL_MEMSHIRE_PKG 1

`include "dv/common/base/evl_base_pkg.sv"
`include "dv/common/axi/axi_stim/evl_axi_stim_pkg.sv"
`include "dv/common/apb/apb_stim/evl_apb_stim_pkg.sv"
`include "dv/soc/memshire/evl_memshire_regs_pkg.sv"

`define EVL_MESH_TO_MEM_CTL_PORT 0
`define EVL_MESH_TO_SBM_PORT 1
`define EVL_UST_TO_MEM_CTL_PORT 2
`define EVL_ATOMIC_TO_MESH_PORT 3
`define EVL_DDR_LO_EVEN_PORT 4
`define EVL_DDR_HI_EVEN_PORT 5
`define EVL_DDR_LO_ODD_PORT 6
`define EVL_DDR_HI_ODD_PORT 7
`define EVL_DDR_APB_PORT 8

`define EVL_MEMSHIRE_REQ_ID_FOR_SBM evl_bus_req_id_t'(32'h10000)
`define EVL_MEMSHIRE_REQ_ID_FOR_MESH evl_bus_req_id_t'(32'h20000)
`define EVL_MEMSHIRE_REQ_ID_FOR_UST evl_bus_req_id_t'(32'h40000)
`define EVL_MEMSHIRE_REQ_ID_FOR_ATOMIC evl_bus_req_id_t'(32'h80000)


//-----------------------------------------------------------------------------------------------
//
//
//
package evl_memshire_types_pkg;

  `include "rtl/inc/esr_types_auto.vh"
  `include "rtl/inc/esr_types_legacy.vh"

  typedef esr_ms_mem_ctl_t evl_esr_ms_mem_ctl_t;
  typedef esr_ms_clk_gate_ctl_t evl_esr_ms_clk_gate_ctl_t;

endpackage : evl_memshire_types_pkg


package evl_memshire_pkg;

  import uvm_pkg::*;
  import evl_base_pkg::*;
  import evl_etlink_types_pkg::*;
  import evl_axi_types_pkg::*;
  import evl_axi_pkg::*;
  import evl_memshire_types_pkg::*;
  import evl_memshire_regs_pkg::*;

  typedef enum int {
    MESH_TO_MEM_ID    = `EVL_MESH_TO_MEM_CTL_PORT,
    MESH_TO_SBM_ID    = `EVL_MESH_TO_SBM_PORT,
    UST_TO_MEM_ID     = `EVL_UST_TO_MEM_CTL_PORT,
    ATOMIC_TO_MESH_ID = `EVL_ATOMIC_TO_MESH_PORT,
    MEM_LO_P0_ID      = `EVL_DDR_LO_EVEN_PORT,
    MEM_HI_P0_ID      = `EVL_DDR_HI_EVEN_PORT,
    MEM_LO_P1_ID      = `EVL_DDR_LO_ODD_PORT,
    MEM_HI_P1_ID      = `EVL_DDR_HI_ODD_PORT,
    DDR_APB_ID        = `EVL_DDR_APB_PORT
  } evl_memshire_port_t;


  typedef enum int {
    MEMSHIRE_NO_PACKET,
    MEMSHIRE_CONFIG,
    MEMSHIRE_LO_SERIALIZE,
    MEMSHIRE_HI_SERIALIZE,
    MEMSHIRE_ATOMIC_HIT,
    MEMSHIRE_ATOMIC_RSP_PUSH,
    MEMSHIRE_CACHE_WR,
    MEMSHIRE_REG_ARB
  } evl_memshire_event_t;

  typedef enum bit [3:0] {
    MS_TABLE_SERIALIZE,
    MS_TABLE_ATOMIC_HIT,
    MS_TABLE_CACHE_WR
  } evl_ms_sub_table_t;


  typedef enum byte {
    MSQ_None,
    MSQ_OneLo,
    MSQ_OneHi,
    MSQ_ManyLo,
    MSQ_ManyHi,
    MSQ_ManyLoHi,
    MSQ_Victim,
    MSQ_SndRsp
  } evl_ms_qual_t;


  //
  // The following packed structure is used to specify configuration changes.
  //
  typedef struct packed {
    bit [3:0]   bank;
    bit [3:0]   num_banks;
    bit [3:0]   num_ways;
    bit [7:0]   bank_lsb;
    bit [12:0]  set_size;
    evl_paddr_t set_base;
  } evl_memshire_config_t;


  //
  // The following packed structure is used to specify request serialization.
  //
  typedef struct packed {
    evl_bus_req_id_t grants;
    bit              write_req;
    evl_bus_req_id_t req_id;
    evl_paddr_t      addr;
  } evl_memshire_serialize_t;


  //
  // The following packed structure is used to specify request atomic hits.
  //
  typedef struct packed {
    bit [3:0]        hit_vector;
    bit              write_req;
    bit              is_atomic;
    evl_bus_req_id_t req_id;
    evl_paddr_t      addr;
  } evl_memshire_atomic_t;


  //
  // The following packed structure is used to specify register access arbitration.
  //
  typedef struct packed {
    bit sel_bpam_mux;
    bit sel_ai_mux;
  } evl_memshire_reg_arb_t;


  //
  // Basic packet structure.
  //
  typedef struct packed {bit [139:0] info;} evl_memshire_packet_s;


  //
  // Encapsulate evl_memshire_config_t into a right-sized structure.
  //
  typedef struct packed {
    bit [139-$bits(evl_memshire_config_t):0] padding;
    evl_memshire_config_t                    info;
  } evl_memshire_config_s;


  //
  // Encapsulate evl_memshire_serialize_t into a right-sized structure.
  //
  typedef struct packed {
    bit [139-$bits(evl_memshire_serialize_t):0] padding;
    evl_memshire_serialize_t                    info;
  } evl_memshire_serialize_s;

  //
  // Encapsulate evl_memshire_atomic_t into a right-sized structure.
  //
  typedef struct packed {
    bit [139-$bits(evl_memshire_atomic_t):0] padding;
    evl_memshire_atomic_t                    info;
  } evl_memshire_atomic_s;


  //
  // Encapsulate evl_memshire_config_t into a right-sized structure.
  //
  typedef struct packed {
    bit [139-$bits(evl_memshire_reg_arb_t):0] padding;
    evl_memshire_reg_arb_t                    info;
  } evl_memshire_reg_arb_s;


  //
  // Encapsulate all of the structure definitions into a common union.
  //
  typedef union packed {
    evl_memshire_packet_s    packet;
    evl_memshire_config_s    config_info;
    evl_memshire_serialize_s serialize_info;
    evl_memshire_atomic_s    atomic_info;
    evl_memshire_reg_arb_s   reg_arb_info;
  } evl_memshire_packet_u;



  typedef enum int {
    ESR_MS_ADDR_REMOVE_BIT_SEL0,
    ESR_MS_ADDR_REMOVE_BIT_SEL1,
    ESR_MS_ADDR_REMOVE_BIT_SEL2,
    ESR_MS_ADDR_REMOVE_BIT_SEL3,
    ESR_MS_ADDR_MC_BIT_SEL,
    ESR_MS_ADDR_DEBUG_MC_BIT_SEL,
    ESR_MS_ADDR_MASK,
    ESR_MS_MC_EN,
    ESR_MS_ATOMIC_EVICT_START,
    ESR_MS_ATOMIC_STATE,
    ESR_MS_MEM_REVISION_ID,
    ESR_DDRC_U0_CSYSREQ_LO,
    ESR_DDRC_U0_CSYSACK_LO,
    ESR_DDRC_U0_CACTIVE_LO,
    ESR_DDRC_U0_CSYSREQ_HI,
    ESR_DDRC_U0_CSYSACK_HI,
    ESR_DDRC_U0_CACTIVE_HI,
    ESR_DDRC_U1_CSYSREQ_LO,
    ESR_DDRC_U1_CSYSACK_LO,
    ESR_DDRC_U1_CACTIVE_LO,
    ESR_DDRC_U1_CSYSREQ_HI,
    ESR_DDRC_U1_CSYSACK_HI,
    ESR_DDRC_U1_CACTIVE_HI,
    ESR_DDRC_U1_CSYSREQ_DDRC,
    ESR_DDRC_U1_CSYSACK_DDRC,
    ESR_DDRC_U1_CACTIVE_DDRC,
    ESR_DDRC_U0_CSYSREQ_DDRC,
    ESR_DDRC_U0_CSYSACK_DDRC,
    ESR_DDRC_U0_CACTIVE_DDRC,
    ESR_DDRC_WRITE_AUTO_PRECHARGE_LO,
    ESR_DDRC_WRITE_AUTO_PRECHARGE_HI,
    ESR_DDRC_READ_AUTO_PRECHARGE_LO,
    ESR_DDRC_READ_AUTO_PRECHARGE_HI,
    ESR_DDRC_SCRUB_ADDR_START_MASK,
    ESR_DDRC_SCRUB_ADDR_RANGE_MASK,
    ESR_DDRC_U0_MRR_DATA,
    ESR_DDRC_U1_MRR_DATA,
    ESR_DDRC_U0_MRR_DATA_VALID,
    ESR_DDRC_U1_MRR_DATA_VALID,
    ESR_DDRC_U0_CORE_RESET,
    ESR_DDRC_U1_CORE_RESET,
    ESR_DDRC_APB_RESET,
    ESR_DDRC_U0_AXI_RESET,
    ESR_DDRC_U1_AXI_RESET,
    ESR_DDRC_PUB_RESET,
    ESR_DDRC_POWER_OK
  } evl_memshire_reg_field_names_t;


  typedef class evl_ddr_env;
  typedef class evl_ddr_rtl_port;
  typedef class evl_memshire_cache;
  typedef class evl_memshire_env;
  typedef class evl_memshire_packet;
  typedef class evl_memshire_rtl_port;
  typedef class evl_memshire_scoreboard_cover;
  typedef class evl_memshire_scoreboard;
  typedef class evl_memshire_side_env;

  `include "dv/soc/memshire/evl_ddr_rtl_port.sv"
  `include "dv/soc/memshire/evl_memshire_rtl_port.sv"
  `include "dv/soc/memshire/evl_memshire_packet.sv"
  `include "dv/soc/memshire/evl_memshire_env.sv"
  `include "dv/soc/memshire/evl_ddr_env.sv"
  `include "dv/soc/memshire/evl_memshire_scoreboard_cover.sv"
  `include "dv/soc/memshire/evl_memshire_scoreboard.sv"
  `include "dv/soc/memshire/evl_memshire_cache.sv"
  `include "dv/soc/memshire/evl_memshire_side_env.sv"

endpackage : evl_memshire_pkg

`endif  // !__EVL_MEMSHIRE_PKG
