/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/*typedef struct packed {
   logic [1:0] tbox3_id;
   logic [1:0] tbox2_id;
   logic [1:0] tbox1_id;
   logic [1:0] tbox0_id;
   logic rbox_en;
   logic [3:0] tbox_en;
   logic [3:0] neigh_en;
   logic cache_en;
   logic [7:0] shire_id;
} esr_shire_config_t;*/
/* ESR values */
#define ESR_SHIRE_CONFIG_32_EN  0x001f20
#define ESR_SHIRE_CONFIG_32_REMAP  0x001f00
#define ESR_SHIRE_CONFIG_32_DIS  0x000120
#define ESR_SHIRE_CONFIG_0_EN  0x001f00
#define ESR_SHIRE_CONFIG_0_REMAP  0x001f20
#define ESR_SHIRE_CONFIG_0_DIS 0x000100
#define ESR_SHIRE_CONFIG_EN  0x000100u
/* ESR addresses: REGION_NEIGH */
#define MINION_BOOT 0x1e003
/* ESR addresses: REGION_OTHER */
#define SHIRE_OTHER_CONFIG 0x08001
#define SHIRE_PLL_READ_DATA 0x08051
#define SHIRE_DLL_READ_DATA 0x0805c
#define SHIRE_OTHER_MTIME_LOCAL_TARGET 0x08043
#define SHIRE_OTHER_THREAD0_DISABLE 0x08048
#define SHIRE_OTHER_THREAD1_DISABLE 0x08002
#define SHIRE_OTHER_CTRL_CLOCKMUX 0x08053
#define SHIRE_OTHER_PLL_AUTO_CONFIG 0x0804a
#define SHIRE_OTHER_DLL_AUTO_CONFIG 0x08059
#define SHIRE_OTHER_PLL_CONFIG_DATA_0 0x0804b
#define SHIRE_OTHER_PLL_CONFIG_DATA_1 0x0804c
#define SHIRE_OTHER_PLL_CONFIG_DATA_2 0x0804d
#define SHIRE_OTHER_PLL_CONFIG_DATA_3 0x0804e
#define SHIRE_OTHER_DLL_CONFIG_DATA_0 0x0805a
#define SHIRE_OTHER_DLL_CONFIG_DATA_1 0x0805b

#define SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL_OFF   15
#define SHIRE_OTHER_PLL_AUTO_CONFIG_LOCK_RST_OFF   14
#define SHIRE_OTHER_PLL_AUTO_CONFIG_REG_NUM_OFF    10
#define SHIRE_OTHER_PLL_AUTO_CONFIG_REG_FIRST_OFF  4
#define SHIRE_OTHER_PLL_AUTO_CONFIG_WRITE_OFF      3
#define SHIRE_OTHER_PLL_AUTO_CONFIG_RUN_OFF        2
#define SHIRE_OTHER_PLL_AUTO_CONFIG_ENABLE_OFF     1
#define SHIRE_OTHER_PLL_AUTO_CONFIG_RESET_OFF      0

#define SHIRE_OTHER_DLL_AUTO_CONFIG_DLL_EN_OFF     13
#define SHIRE_OTHER_DLL_AUTO_CONFIG_PCLK_SEL_OFF   11
#define SHIRE_OTHER_DLL_AUTO_CONFIG_LOCK_RST_OFF   10
#define SHIRE_OTHER_DLL_AUTO_CONFIG_REG_NUM_OFF    7
#define SHIRE_OTHER_DLL_AUTO_CONFIG_REG_FIRST_OFF  4
#define SHIRE_OTHER_DLL_AUTO_CONFIG_WRITE_OFF      3
#define SHIRE_OTHER_DLL_AUTO_CONFIG_RUN_OFF        2
#define SHIRE_OTHER_DLL_AUTO_CONFIG_ENABLE_OFF     1
#define SHIRE_OTHER_DLL_AUTO_CONFIG_RESET_OFF      0

#define SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL       (2 << SHIRE_OTHER_PLL_AUTO_CONFIG_PCLK_SEL_OFF)
#define SHIRE_OTHER_DLL_AUTO_CONFIG_PCLK_SEL       (2 << SHIRE_OTHER_DLL_AUTO_CONFIG_PCLK_SEL_OFF)

/*typedef struct packed {
      logic [1:0] pclk_sel = 0
      logic lock_reset_disable = 0
      logic [3:0] reg_num = 0x16
      logic [5:0] reg_first = 00
      logic write = 0
      logic run = 0
      logic enable = 0
      logic reset = 1
} esr_pll_auto_config_t;*/

/*typedef struct packed {
      logic dll_enable;
      logic [1:0] pclk_sel;
      logic lock_reset_disable;
      logic [2:0] reg_last;
      logic [2:0] reg_first;
      logic write;
      logic run;
      logic enable;
      logic reset;
   } esr_dll_auto_config_t;
*/
