/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/**
 *  @Component      tcRegtest
 *
 *  @Filename       tcRegtest.h
 *
 *  @Description    Local Test Case Header file needed for tcRegtest compilation
 *
 *//*======================================================================== */

#ifndef __TC_REGTEST_H
#define __TC_REGTEST_H

#ifdef __cplusplus
extern
{
#endif

/* Include Peripheral API header */
#include "<put_include_file_name_here>"

/* Define Test Case Function Name */
#define tcRegtest_TestFunction  <put_tc_name_here>_TestFunction

/* Define Peripheral Address */
uint32_t regBaseAddress = (uint32_t)<put_IP_base_address_here>;

/* Define Peripheral Register set */
REGTEST_t *pRegTable = <put_regtest_table_here>;


#ifdef __cplusplus
}
#endif

#endif	/* __TC_REGTEST_H */


/*     <EOF>     */

