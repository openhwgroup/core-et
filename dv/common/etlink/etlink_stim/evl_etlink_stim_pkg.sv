`ifndef __EVL_ETLINK_STIM_PKG

   `define __EVL_ETLINK_STIM_PKG 1

   `include "dv/common/base/evl_base_pkg.sv"
   `include "dv/common/etlink/etlink_vc/evl_etlink_pkg.sv"

   `define EVL_ETLINK_REQ_NONE          (`EVL_Bus_Idle)
   `define EVL_ETLINK_REQ_READ          (evl_bus_cmd_t'(ET_LINK_REQ_Read))
   `define EVL_ETLINK_REQ_WRITE         (evl_bus_cmd_t'(ET_LINK_REQ_Write))
   `define EVL_ETLINK_REQ_WRITE_AROUND  (evl_bus_cmd_t'(ET_LINK_REQ_WriteAround))
   `define EVL_ETLINK_REQ_READCOOP      (evl_bus_cmd_t'(ET_LINK_REQ_ReadCoop))
   `define EVL_ETLINK_REQ_MSG_SEND_DATA (evl_bus_cmd_t'(ET_LINK_REQ_MsgSendData))
   `define EVL_ETLINK_REQ_ATOMIC        (evl_bus_cmd_t'(ET_LINK_REQ_Atomic))
   `define EVL_ETLINK_REQ_FLUSH         (evl_bus_cmd_t'(ET_LINK_REQ_Flush))
   `define EVL_ETLINK_REQ_FLUSH_TO_MEM  (evl_bus_cmd_t'(ET_LINK_REQ_FlushToMem))
   `define EVL_ETLINK_REQ_EVICT         (evl_bus_cmd_t'(ET_LINK_REQ_Evict))
   `define EVL_ETLINK_REQ_EVICT_TO_MEM  (evl_bus_cmd_t'(ET_LINK_REQ_EvictToMem))
   `define EVL_ETLINK_REQ_LOCK          (evl_bus_cmd_t'(ET_LINK_REQ_Lock))
   `define EVL_ETLINK_REQ_UNLOCK        (evl_bus_cmd_t'(ET_LINK_REQ_Unlock))
   `define EVL_ETLINK_REQ_SCP_FILL      (evl_bus_cmd_t'(ET_LINK_REQ_ScpFill))
   `define EVL_ETLINK_REQ_PREFETCH      (evl_bus_cmd_t'(ET_LINK_REQ_Prefetch))
   `define EVL_ETLINK_RSP_NONE          (`EVL_Bus_Idle)
   `define EVL_ETLINK_RSP_ACK           (evl_bus_cmd_t'(ET_LINK_RSP_Ack))
   `define EVL_ETLINK_RSP_ACK_DATA      (evl_bus_cmd_t'(ET_LINK_RSP_AckData))
   `define EVL_ETLINK_RSP_MSG_RCV_DATA  (evl_bus_cmd_t'(ET_LINK_RSP_MsgRcvData))

   package evl_etlink_stim_pkg;

      import uvm_pkg::*;
      import evl_base_pkg::*;
      import evl_etlink_types_pkg::*;
      import evl_etlink_pkg::*;

      typedef class evl_stim_agent_etlink;
      typedef class evl_stim_cache_etlink;
      typedef class evl_neigh_agent_etlink;
      typedef class evl_l2_agent_etlink;

      `include "dv/common/etlink/etlink_stim/evl_stim_agent_etlink.sv"
      `include "dv/common/etlink/etlink_stim/evl_stim_cache_etlink.sv"
      `include "dv/common/etlink/etlink_stim/evl_neigh_agent_etlink.sv"
      `include "dv/common/etlink/etlink_stim/evl_l2_agent_etlink.sv"
      `include "dv/common/etlink/etlink_stim/evl_minion_agent_etlink.sv"
   endpackage : evl_etlink_stim_pkg

`endif // !__EVL_ETLINK_STIM_PKG
