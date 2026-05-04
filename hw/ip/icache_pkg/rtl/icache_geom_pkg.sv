// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off UNUSEDPARAM */  // Geometry constants are consumed incrementally by standalone-Icache blocks.
package icache_geom_pkg;

  // ════════════════════════════════════════════════════════
  // Standalone-Icache geometry and SRAM layout
  // ════════════════════════════════════════════════════════

  localparam int unsigned IcacheBlockBits      = 512;
  localparam int unsigned IcacheBlockBytes     = IcacheBlockBits / 8;
  localparam int unsigned IcacheBlockAddrSize  = $clog2(IcacheBlockBytes);

  localparam int unsigned IcacheSets           = 128;
  localparam int unsigned IcacheSetAddrWidth   = $clog2(IcacheSets);
  localparam int unsigned IcacheWays           = 4;
  localparam int unsigned IcacheWayAddrWidth   = $clog2(IcacheWays);
  localparam int unsigned IcacheLruSetSize     = IcacheWays * IcacheWays;

  localparam int unsigned IcacheMemWords       = 512;
  localparam int unsigned IcacheMemWidth       = 144;
  localparam int unsigned IcacheMemUsedBits    = 128;
  localparam int unsigned IcacheMemEccBits     = IcacheMemWidth - IcacheMemUsedBits;
  localparam int unsigned IcacheMemAdWidth     = $clog2(IcacheMemWords);

  localparam int unsigned IcacheNrMems         =
      (IcacheWays * IcacheSets * IcacheBlockBits) / (IcacheMemWords * IcacheMemUsedBits);
  localparam int unsigned IcacheNrMemsPerRow   = IcacheBlockBits / IcacheMemUsedBits;
  localparam int unsigned IcacheNrRows         = IcacheNrMems / IcacheNrMemsPerRow;
  localparam int unsigned IcacheColAdWidth     = $clog2(IcacheNrMemsPerRow);
  localparam int unsigned IcacheRowAdWidth     = $clog2(IcacheNrRows);
  localparam int unsigned IcacheSramAddrWidth  = IcacheRowAdWidth + IcacheMemAdWidth;
  localparam int unsigned IcacheSramDataWidth  = IcacheNrMemsPerRow * IcacheMemWidth;

  localparam int unsigned IcacheMicroCacheSize   = 16;
  localparam int unsigned IcacheMicroCacheAdWidth = $clog2(IcacheMicroCacheSize);

endpackage : icache_geom_pkg
/* verilator lint_on UNUSEDPARAM */
