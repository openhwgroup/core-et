//#############################################################################
//###                         Minion Shire Disable                          ###
//#############################################################################

// This is a bit map of the Minion shires to build.  A 1 means do NOT build ...
// the whole Minion shire will be excluded, including the meshstop.
`ifndef DV_MINION_SHIRE_DISABLE
   `define DV_MINION_SHIRE_DISABLE        36'hffffffffe
`endif


//#############################################################################
//###                           Minion Shire Stub                           ###
//#############################################################################

// This is a bit map of the Minion shires to stub (if a DV_MINION_SHIRE_DISABLE
// bit is set, that takes precedence).  A 1 means replace the Minion shire with
// a stub that includes a meshstop.
`ifndef DV_MINION_SHIRE_STUB
   `define DV_MINION_SHIRE_STUB           36'h000000000
`endif


//#############################################################################
//###                         Minion Shire Stimulus                         ###
//#############################################################################

// This is a bit map of the Minion shires to stub (if a DV_MINION_SHIRE_DISABLE
// bit is set, that takes precedence) and replace with stimulus agents to drive
// requests/responses into the NoC AXI ports; this is intended to be used in a
// NoC standalone environment.  Note that when a DV_MINION_SHIRE_STIM bit is 1,
// the corresponding DV_MINION_SHIRE_STUB will also be set to 1 by default
// (there is no need to specifically set the DV_MINION_SHIRE_STUB bit).
`ifndef DV_MINION_SHIRE_STIM
   `define DV_MINION_SHIRE_STIM           36'h000000000
`endif


//#############################################################################
//###                           Minion Shire Lite                           ###
//#############################################################################

// This is a bit map of the Minion shire caches to replace with a "lite" shire
// cache version (which simply forwards incoming L3 requests to the system
// mesh port).  If a DV_MINION_SHIRE_DISABLE bit, a DV_MINION_SHIRE_STUB bit,
// or a DV_MINION_SHIRE_STIM bit is set, the DV_MINION_SHIRE_LITE bit will be
// overridden.
`ifndef DV_MINION_SHIRE_LITE
   `define DV_MINION_SHIRE_LITE           36'h000000000
`endif


//#############################################################################
//###                        Minion Shire Cache Stub                        ###
//#############################################################################

// This is a bit map of the Minion shire caches to replace with stubs.  The
// assumption is that this will be used for neighborhood/core standalone
// testing in which the shire cache will be replaced with a stimulus model.
`ifndef DV_MINION_SHIRE_CACHE_STUB
   `define DV_MINION_SHIRE_CACHE_STUB     36'hfffffffff
`endif


//#############################################################################
//###                      Neighborhood/Core Subsystem                      ###
//#############################################################################

// This is a bit map of the Minion shires in which real neighborhoods will be
// allowed.  For shire cache standalone testing, we replace all neighborhoods
// within a particular shire.
`ifndef DV_MINION_SHIRE_ALL_NEIGH_STUB
   `define DV_MINION_SHIRE_ALL_NEIGH_STUB 36'hffffffffe
`endif


// This is a bit map of the neighborhoods within a shire to replace with stubs.
// Note that if a shire's DV_MINION_SHIRE_ALL_NEIGH_STUB bit is set to 1, this
// define will have no meaning for that shire (in other words, it is as if
// DV_MINION_SHIRE_NEIGH_STUB for that shire is set to all 1s).
`ifndef DV_MINION_SHIRE_NEIGH_STUB
   `define DV_MINION_SHIRE_NEIGH_STUB     8'hfe
`endif


// This is a bit map of the Minions within a neighborhood to replace with stubs.
// Note that if a shire's DV_MINION_SHIRE_ALL_NEIGH_STUB bit is set to 1 or if
// a neighborhood's DV_MINION_SHIRE_NEIGH_STUB bit is set to to 1, this define
// will have no meaning for that neighborhood (in other words, it is as if
// DV_MINION_STUB for that neighborhood is set to all 1s).
`ifndef DV_MINION_STUB
   `define DV_MINION_STUB                 8'h00
`endif


//#############################################################################
//###                         Memory Shire Disable                          ###
//#############################################################################

// This is a bit map of the memory shires to build.  A 1 means do NOT build ...
// the whole memory shire will be excluded, including the meshstop.
`ifndef DV_MEMSHIRE_DISABLE
   `define DV_MEMSHIRE_DISABLE            32'hffffffff
`endif


//#############################################################################
//###                            DDR RTL Disable                            ###
//#############################################################################

// This is a bit map of DDR RTL to build.  A 1 means do NOT build ... the whole
// DDR controller RTL will be excluded.
`ifndef DV_DDR_DISABLE
   `ifdef USE_REAL_DDRC
      // when using real DDR, default to the same as memshire disable
      `define DV_DDR_DISABLE              `DV_MEMSHIRE_DISABLE
   `else
      `define DV_DDR_DISABLE              32'hffffffff
   `endif
`endif


// Build real IO Shire (a 0 means build the real)
`ifndef DV_IO_SHIRE_STUB
   `define DV_IO_SHIRE_STUB               1
`endif


// Do not build the PCI Shire
`define DV_PSHIRE_DISABLE                 1


//#############################################################################
//###                              NoC Disable                              ###
//#############################################################################

// If NOC_DISABLE is defined (regardless of the value), the NoC will not be
// built.  This can be overridden with +define+NOC_ENABLE=1
`ifndef NOC_DISABLE
   `define NOC_DISABLE                    1
`endif
