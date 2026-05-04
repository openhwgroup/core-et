
`ifndef __ET_TB_DEFINES_VH__

   `define __ET_TB_DEFINES_VH__ 1

   `define ET_DPI_PKT_WIDTH     256
   `define ET_DPI_PKT_MSB       (`ET_DPI_PKT_WIDTH - 1)

   `define ET_FIFO_EVENT_WIDTH  8
   `define ET_FIFO_EVENT_MSB    (`ET_FIFO_EVENT_WIDTH - 1)

   `define ET_FIFO_NULL_EVENT   `ET_FIFO_EVENT_WIDTH'd0
   `define ET_FIFO_RETIRE_EVENT `ET_FIFO_EVENT_WIDTH'd1

   `ifdef ET_SIMULATION // ------------------------------------------------------------------------{

      `ifdef ET_REG_DELAY_ONLY

         `ifndef SNPS_SEQ_DELAY
            `define SNPS_SEQ_DELAY 10
         `endif // ifndef SNPS_SEQ_DELAY

         `ifndef SNPS_LAT_DELAY
            `define SNPS_LAT_DELAY 10
         `endif // ifndef SNPS_LAT_DELAY

         `ifndef SNPS_CKGTLT_DELAY
            `define SNPS_CKGTLT_DELAY 0
         `endif // ifndef SNPS_CKGTLT_DELAY

         `ifndef SNPS_COMBO_DELAY
            `define SNPS_COMBO_DELAY 0
         `endif // ifndef SNPS_COMBO_DELAY

         `ifndef SNPS_CKGT_DELAY
            `define SNPS_CKGT_DELAY 0
         `endif // ifndef SNPS_CKGT_DELAY

      `endif // ifdef ET_REG_DELAY_ONLY

      `ifndef USE_ET_PARAMS_IN_BUILD
         `define USE_ET_PARAMS_IN_BUILD 1
      `endif

   `endif // ifdef ET_SIMULATION ------------------------------------------------------------------}


   `ifdef USE_ET_PARAMS_IN_BUILD // ---------------------------------------------------------------{

      `include "timescale.vh"

      `ifdef NOC_ENABLE
         `undef NOC_DISABLE
      `endif

      `ifndef DV_MINION_SHIRE_STUB
         `define DV_MINION_SHIRE_STUB {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_STIM
         `define DV_MINION_SHIRE_STIM {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_LITE
         `define DV_MINION_SHIRE_LITE {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_ALL_NEIGH_DISABLE
         `define DV_MINION_SHIRE_ALL_NEIGH_DISABLE {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_ALL_NEIGH_STUB
         `define DV_MINION_SHIRE_ALL_NEIGH_STUB {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_CACHE_STUB
         `define DV_MINION_SHIRE_CACHE_STUB {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_CHANNEL_STUB
         `define DV_MINION_SHIRE_CHANNEL_STUB {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_GFX_ENABLE
         `define DV_MINION_SHIRE_GFX_ENABLE {36{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_NEIGH_STUB
         `define DV_MINION_SHIRE_NEIGH_STUB {8{1'b0}}
      `endif

      `ifndef DV_MINION_SHIRE_NEIGH_DISABLE
         `define DV_MINION_SHIRE_NEIGH_DISABLE {8{1'b0}}
      `endif

      `ifndef DV_MINION_DISABLE
         `define DV_MINION_DISABLE {8{1'b0}}
      `endif

      `ifndef DV_MINION_STUB
         `define DV_MINION_STUB {8{1'b0}}
      `endif

      //
      // If the NOC is disabled, the default is to disable the PSHIRE.
      //
      `ifdef NOC_DISABLE
         `ifndef DV_NO_PSHIRE_DISABLE
            `ifdef DV_PSHIRE_DISABLE
               `define _DV_PSHIRE_DISABLE_ 1
            `endif
         `endif
      `else // NOC_DISABLE
         `undef _DV_PSHIRE_DISABLE_
      `endif // !ifdef NOC_DISABLE
      `ifndef _DV_PSHIRE_DISABLE_
         `ifdef DV_PSHIRE_NOC_ONLY
            `define _DV_PCIE_SS_STUB_ 1
         `endif // ifdef DV_PSHIRE_NOC_ONLY
      `endif // ifndef _DV_PSHIRE_DISABLE_

      `ifdef EVL_SIMULATION // --------------------------------------------------------------------{

         `ifdef USE_IOSHIRE_COMPS
            `ifndef USE_SPIO_ES
               `ifndef USE_HDL_TEST_COMPS
                  `define USE_HDL_TEST_COMPS 1
               `endif // ifndef USE_SPIO_ES
            `endif // ifndef USE_SPIO_ES
         `endif // ifndef USE_IOSHIRE_COMPS

         `include "dv/common/base/evl_base_pkg.sv"

      `else // ifdef EVL_SIMULATION }--------------------------------------------------------------{

         `ifdef ET_IP_SIMULATION
            `include "dv/common/base/evl_base_pkg.sv"
         `else // ifdef ET_IP_SIMULATION
            `ifndef ET_DRIVE_UNUSED_AXI
               `define ET_DRIVE_UNUSED_AXI 1
            `endif // ifndef ET_DRIVE_UNUSED_AXI
         `endif // !ifdef ET_IP_SIMULATION

      `endif // !ifdef EVL_SIMULATION -------------------------------------------------------------}

   `else // ifdef USE_ET_PARAMS_IN_BUILD }---------------------------------------------------------{

      `ifdef ET_IP_SIMULATION
         `include "dv/common/base/evl_base_pkg.sv"
      `endif // !ifdef ET_IP_SIMULATION

   `endif // !ifdef USE_ET_PARAMS_IN_BUILD --------------------------------------------------------}

`endif // ifndef __ET_TB_DEFINES_VH__

`include "rtl/inc/soc.vh"
