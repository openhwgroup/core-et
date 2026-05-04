/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <inttypes.h>

#include "macros.h"
#include "cacheops.h"
#include "minion.h"
#include "et_test_common.h"
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif

#define CACHE_LINE 64

#define PORT_0 0
#define PORT_1 1
#define PORT_2 2
#define PORT_3 3

#define HART_PORT_0 0x800ULL
#define HART_PORT_1 0x840ULL
#define HART_PORT_2 0x880ULL
#define HART_PORT_3 0x8C0ULL

#define CREDINC0_ESR  0x803400C0L //[29:22] replaced by specific shire
#define CREDINC1_ESR  0x803400C8L
#define CREDINC2_ESR  0x803400D0L
#define CREDINC3_ESR  0x803400D8L

#define FCC_CREDITINC_0 0x18UL
#define FCC_CREDITINC_1 0x19UL
#define FCC_CREDITINC_2 0x1AUL
#define FCC_CREDITINC_3 0x1BUL

#define SHIRE_0_MASK    0x00000001UL
#define ALL_SHIRE_MASK  0xFFFFFFFFUL

#define MINION_0_MASK   0x00000001UL
#define ALL_MINION_MASK 0xFFFFFFFFUL

#define USER_MODE_ADDR       (0x8035FFF8ULL)
#define SUPERVISOR_MODE_ADDR (0x8075FFF8ULL)
#define MACHINE_MODE_ADDR    (0x80F5FFF8ULL)
#define UARCH_MODE_ADDR      (0x80B5FFF8ULL)

#define USER_MODE        0x0L
#define SUPERVISOR_MODE  0x1L
#define MACHINE_MODE     0x2L
#define UARCH_MODE       0x3L

#define ET_LINK_ADDR 0x8035FFF0UL
#define BROADCAST_ADDR (ET_LINK_ADDR)

#define MAX_BARRIER_COUNTER 8
#define MAX_100_LOOP 100

#define MAX_MINIONS_JOINED_TO_CNT_BARRIER (32 -1)

#define REGION_HART_PORT ((1ULL<<32)|(1ULL << 11))

#define FLB_COUNTER_0_TO_USE 0
#define FLB_COUNTER_1_TO_USE 1
#define FLB_COUNTER_2_TO_USE 2
#define FLB_COUNTER_3_TO_USE 3
#define FLB_COUNTER_4_TO_USE 4
#define FLB_COUNTER_5_TO_USE 5
#define FLB_COUNTER_6_TO_USE 6
#define FLB_COUNTER_7_TO_USE 7

#define PORT_UMODE_ENABLE            1
#define PORT_SCPWAY_0                0
#define PORT_OOB_DISABLE             0
#define PORT_OOB_ENABLE              1
#define MAX_MSGS_SIZE                3
#define MAX_MSG_PAYLOAD_SIZE (1<<MAX_MSGS_SIZE)
#define MAX_MSG_AGENTS_PER_PORT      8
#define EXPECTED_MINION_TO_REPORTED 32
/* force different set */
#define RCV_MSG_PORT_0_OFFSET 0
#define RCV_MSG_PORT_1_OFFSET 8
#define RCV_MSG_PORT_2_OFFSET 16
#define RCV_MSG_PORT_3_OFFSET 24

#define MEM_L1  0
#define MEM_L2  1
#define MEM_L3  2
#define MEM_MEM 3

#define SHIRE_ESR_ADR_REGION 0x1AUL

#define ALL_MINIONS_OTHER_THAN_ME  0xFFFFFFFEULL

#define FCC_COUNTERS  4
#define FCC_COUNTER_0 0
#define FCC_COUNTER_1 1
#define FCC_COUNTER_2 2
#define FCC_COUNTER_3 3

void broadcast (long esr2broadcast, long shire2broadcast, long data)
{
  //It's assumed we are always are in usermode
  volatile uint64_t* addr_broadcast;
  long data2 = ((USER_MODE<<59)|((SHIRE_ESR_ADR_REGION<<54)|(esr2broadcast << 40))|shire2broadcast);

  addr_broadcast  = reinterpret_cast<volatile uint64_t*> BROADCAST_ADDR;
  *addr_broadcast = data;
  addr_broadcast  = reinterpret_cast<volatile uint64_t*> USER_MODE_ADDR;
  *addr_broadcast = data2;
}

void evicting (bool tmask, uint8_t dstlevel, uint8_t set, uint8_t way, uint8_t numlines)
{
  uint64_t value = 0;

  value = (tmask?(1UL << 63):0);
  value |= (((uint64_t) dstlevel) << 58);
  value |= (((uint64_t) set) << 14);
  value |= (((uint64_t) way) << 6);
  value |= (numlines-1);

  evict_sw(value);
}

void lock(uint8_t way, uint64_t pa)
{
  uint64_t value = 0;

  value =  (((uint64_t) way) << 55);
  value |= (pa & 0xFFFFFFFFC0);

  lock_sw(value);
}

void sendFCC(uint64_t shire_id, uint8_t counter_id, uint64_t minion_mask)
{
  uint64_t fcc_id[FCC_COUNTERS] = { 0x018, 0x019, 0x01a, 0x01b};
  volatile uint64_t* addr_fcc;

  addr_fcc  = reinterpret_cast<volatile uint64_t*> (( 1ULL << 32) | (SHIRE_ESR_ADR_REGION << 17) | (shire_id << 22) | (fcc_id[counter_id] << 3));
  *addr_fcc = minion_mask;
}

int main()
{
    uint64_t hart_id = getHartId();
    uint64_t value = ((MAX_MINIONS_JOINED_TO_CNT_BARRIER << 5) | FLB_COUNTER_0_TO_USE);

    uint8_t  __attribute__((aligned(CACHE_LINE))) rcv_buffer[EXPECTED_MINION_TO_REPORTED][MAX_MSG_PAYLOAD_SIZE];

    uint64_t minion_id = ((hart_id>>1) & 0x1F);
    uint64_t shire_id  = ((hart_id>>6) & 0x3F);

    uint64_t *tmp;

    uint64_t data_offset;
    uint8_t  useport;
    uint8_t  rcv_msg_port;

    uint8_t  msg_iter;
    uint64_t msg_from_shires;
    uint8_t current_msg_iteration = 0;

    //only compute thread 0
    if( (hart_id & 1) == 0)
    {
      if (shire_id == 32)
      { //master shire

        if (minion_id == 0)
        {
          setup_rx_port(PORT_0, PORT_UMODE_ENABLE, MAX_MSGS_SIZE, MAX_MSG_AGENTS_PER_PORT,
                        &rcv_buffer[RCV_MSG_PORT_0_OFFSET][0], PORT_SCPWAY_0, PORT_OOB_DISABLE);

          setup_rx_port(PORT_1, PORT_UMODE_ENABLE, MAX_MSGS_SIZE, MAX_MSG_AGENTS_PER_PORT,
                        &rcv_buffer[RCV_MSG_PORT_1_OFFSET][0], PORT_SCPWAY_0, PORT_OOB_DISABLE);

          setup_rx_port(PORT_2, PORT_UMODE_ENABLE, MAX_MSGS_SIZE, MAX_MSG_AGENTS_PER_PORT,
                        &rcv_buffer[RCV_MSG_PORT_2_OFFSET][0], PORT_SCPWAY_0, PORT_OOB_DISABLE);

          setup_rx_port(PORT_3, PORT_UMODE_ENABLE, MAX_MSGS_SIZE, MAX_MSG_AGENTS_PER_PORT,
                        &rcv_buffer[RCV_MSG_PORT_3_OFFSET][0], PORT_SCPWAY_0, PORT_OOB_DISABLE);

          msg_from_shires = 0xFFFFFFFF;
          for (int i = 0; i < MAX_100_LOOP; i++)
          {
            if (msg_from_shires == 0xFFFFFFFF)
            {
              msg_from_shires ^= 0xFFFFFFFF;
              broadcast(FCC_CREDITINC_0, ALL_SHIRE_MASK, ALL_MINION_MASK);
              fence();
            }

            for (int j = 0; j < 32; j++)
            {
	      if (j < MAX_MSG_AGENTS_PER_PORT) 
	      {
		useport = PORT_0;
		rcv_msg_port = RCV_MSG_PORT_0_OFFSET;
	      }
	      else if (j < (MAX_MSG_AGENTS_PER_PORT*2))
	      {
		useport = PORT_1;
		rcv_msg_port = RCV_MSG_PORT_1_OFFSET;
	      }
	      else if (j < (MAX_MSG_AGENTS_PER_PORT*3))
	      {
		useport = PORT_2;
		rcv_msg_port = RCV_MSG_PORT_2_OFFSET;
	      }
	      else
	      {
		useport = PORT_3;
		rcv_msg_port = RCV_MSG_PORT_3_OFFSET;
	      }

	      data_offset = read_port_status(useport);
              tmp = (uint64_t *)&(rcv_buffer[rcv_msg_port][data_offset]);

              //debug check the value readed
              register long s asm("x21");
               __asm__ volatile
               (
                "ld x21, 0(%[tmp])\n"
                : [s] "=r" (s)
                : [tmp] "r" (tmp)
                : "memory"
                );
               //end debug

               //Extra check..
               //On each iteration we have to received 32 messages one by each shire and all of them has to have the
               //same iteration value (avoiding mixed messages from differents iterations it's an error)
               //With this info set bit mask for each shire and not send a broadcast unless all mask will filled accordingly
               //with the above premises.

               msg_iter = (uint8_t) ((*tmp & 0x00FF0000) >> 16);
	       if (current_msg_iteration == msg_iter)
	       {
		 msg_from_shires |= (uint64_t)(1ULL << (*tmp & 0x000000FF));
	       }
            }
	    current_msg_iteration++;
          }
        }
      }
      else
      {

        if (minion_id == 0)
        {
          for (long barrier_cnt = 0; barrier_cnt < MAX_BARRIER_COUNTER; barrier_cnt++)
          {
            initFLB(barrier_cnt);
          }
          sendFCC(shire_id, FCC_COUNTER_1, ALL_MINIONS_OTHER_THAN_ME);
        }
        else {
          waitFCC(FCC_COUNTER_1);
        }

        for (uint64_t i = 0; i < MAX_100_LOOP; i++)
        {
          waitFCC(FCC_COUNTER_0);
          if (setFLB(value)==1)
          {
            volatile uint64_t* addr_port;
            volatile uint64_t port;

	    if (shire_id < MAX_MSG_AGENTS_PER_PORT) 
	    {
	      port = HART_PORT_0;
	    }
	    else if (shire_id < (MAX_MSG_AGENTS_PER_PORT*2))
	    {
	      port = HART_PORT_1;
	    }
	    else if (shire_id < (MAX_MSG_AGENTS_PER_PORT*3))
	    {
	      port = HART_PORT_2;
	    }
	    else
	    {
	      port = HART_PORT_3;
	    }

            addr_port = reinterpret_cast<volatile uint64_t*>((1ULL << 32)|(32ULL << 22)|(0ULL << 13)|(0ULL << 12)|port);
            *addr_port = ((0xDDULL << 24)|(i << 16)|(minion_id << 8)|shire_id);
          }
        }
      }

    }

    C_TEST_PASS;
    // End marker
    return 0;
}
