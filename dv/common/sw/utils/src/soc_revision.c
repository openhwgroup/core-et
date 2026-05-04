/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/*
 * Functions to check the current SoC revision
 */

#include "cpu.h"
#include "soc.h"
#include "soc_revision.h"

#define OTP_SILICON_REVISION_INDEX 12
#define SILICON_REVISION_ZEBU 0x5b
#define SILICON_REVISION_VCS  0x0 // TODO
#define SILICON_REVISION_SI   0x0 // TODO

// Return the current Silicon Revision
// 8bits
// - 4 upper bits: Major revision
// - 4 lower bits: Lower revision
static uint8_t get_soc_revision(void)
{
    volatile uint32_t *eFusePtr = (uint32_t *)R_SP_EFUSE_BASEADDR;
    uint32_t eFuseVal = eFusePtr[OTP_SILICON_REVISION_INDEX];
    uint8_t  siliconRevision = ((uint8_t)(eFuseVal >> 24) & 0xFF);
    return siliconRevision;
}

// Return True if the current Silicon Revision
// is ZeBu
bool soc_revision_is_zebu(void)
{
  uint8_t rev = get_soc_revision();
  return SILICON_REVISION_ZEBU == rev;
}
