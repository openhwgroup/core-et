/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : Directed self-checking test that exercices the basic functionality
//               of the Esperanto Messaging extensions (DWORD send/recv)
//               through scalar LD/ST from the cores.
//
// TX:
//    Each hart0 in a neighborhood setup a PORT for receiving data with (see port_cfg for more)
//    by locking a number of cache lines and.
//    Then it sends a number of messages (NUM_MSGS_PER_MINION),to its adjacent minion in the neighborhood -
//    (exception is last minion where it sends to the first one), and to all the other neighborhoods in
//    a shire.
//
//
// RX:
//    Each hart1 blocks on the respective port head, until it receives a message, then reads the data
//    and compares them again the expected.
//
// Note:
//       - Machine trap handler is configured in this test in such way that the test would branch to DEAD, should
//        an exception occur.
//       - Message size is set to DWORD and always contain the sender minion id.
// FUTURE:
//       Randomize
//          - WAY, SET, PORT
//          - Load/Store size
//

#include "macros.h"
#include "cacheops.h"
#include "minion.h"
#include "et_test_common.h"

#define NUM_MSGS_PER_MINION 8
#define NUM_PORTS 4


int main()
{
   uint64_t mid, tid, nid, sid;
   uint64_t i;
   volatile uint64_t *send_msg_payload = (uint64_t *) 0x18000000000 + get_minion_id()*NUM_MINIONS_PER_NEIGH;
   volatile uint64_t *gold_msg_payload = (uint64_t *) 0x18100000000 + get_minion_id()*NUM_MINIONS_PER_NEIGH;

   port_cfg_t port_cfg[NUM_PORTS];
   // Number of active minions exchanging messages through ports
   uint32_t active_minions = 8;

   cl_data_t __attribute__((aligned(L1_CL_SIZE))) rcv_buffer[NUM_MINIONS_PER_NEIGH*NUM_HARTS_PER_MINION];

   volatile uint64_t temp;
   volatile uint32_t offset;

   C_TEST_START;

   // Setup machine trap vector
   setup_mtrap_vector();

   //1.0 Get minion ID and thread id and lock scratchpad
   mid = get_minion_id();
   tid = get_thread_id();
   nid = get_neigh_id();
   sid = get_shire_id();

   // 1.1 Start test by configuring all the ports of each minion to be able to receive messages
   // PORT3, ENABLED, NO_OOB, UMODE, 64B_MSG, MAX_MSG = 8, SCP_SET0, SCP_WAY0

   for(int i=0; i < NUM_PORTS; ++i){
     port_cfg[i].port_id = (uint8_t) i;

     port_cfg[i].port_en       = 1;
     port_cfg[i].port_oob_en   = 0;
     port_cfg[i].umode         = 1;
     port_cfg[i].log_msg_size  = 3;
     port_cfg[i].max_msg_size  = 8;

     port_cfg[i].scp_way = (uint8_t) et_get_rand_word(0,1);
     port_cfg[i].scp_way += tid*2;
     port_cfg[i].scp_set = ( ((uint64_t) &rcv_buffer[mid+tid].data[tid]) >> 6) & 0xF;
   }

   // 1.1 Message payload
   // Always contain the source minion id on the least significant half-word
   for (i =0; i<NUM_MINIONS_PER_NEIGH; i++)
   {
       send_msg_payload[i] = get_minion_id();
       send_msg_payload[i] |= 0xcafe0000;
       send_msg_payload[i] |= (uint64_t) (0x0 +(i+1)*0x11111111)  << 32;

       gold_msg_payload[i] =  (mid == 0) ? active_minions-1 : get_minion_id()-1;
       gold_msg_payload[i] |= 0xcafe0000;
       gold_msg_payload[i] |= (uint64_t) (0x0 +(i+1)*0x11111111) << 32;

   }

   // 1.2 Lock some cache lines and then Write to PORT_CTRL register to enable the reception of port messages for all the ports
   for (i =0; i<NUM_PORTS; i++)
      setup_rx_port(port_cfg[i].port_id, port_cfg[i].umode, port_cfg[i].log_msg_size, port_cfg[i].max_msg_size, &rcv_buffer[mid+tid].data[tid], port_cfg[i].scp_way, port_cfg[i].port_oob_en);

   // 2.0 Send a message with your minion id (1.0) to your adjacent minion (locally and to other neigh)
   // Destination: N[i]M[(mid+1)]T[1]
   // i.e  N0M0T0->N0M1T1, N0MOT0->N1M0T1
   if (tid == 0)
   {
      // Send a message to all the minions adjacent to yourself in the same neighborhood, portID selected in a Round Robin way
      for (i=0; i<NUM_MSGS_PER_MINION; i++)
        send_dword(sid, (mid+1)%NUM_MINIONS_PER_NEIGH, 1UL, nid, (uint64_t) port_cfg[i%4].port_id, send_msg_payload[i], 0);

      // Send a message to all the minions in all the neighborhoods in the same shire
      //for (i=1; i<NUM_NEIGH_PER_SHIRE; i++)
      //   send_dword(0,(mid+1)%NUM_MINIONS_PER_NEIGH, 1UL, (uint64_t) i, (uint64_t) port_cfg.port_id, send_msg_payload[i], 0);

      C_TEST_PASS;
   }

   // 2.1 Receive the messages using blocking load to the respective PORT_HEAD that reads
   // the offset in the scratchpad for the received data
   // FUTURE: change loads to be back to back in case the offset is more that a DWORD or split a DWORD load
   // to two loads.
   else
   {
      if( mid < active_minions-1 )
      {
         for (int i=0; i<NUM_MSGS_PER_MINION; i++)
         {
            //FUTURE
            //Add some random instructions that miss the L1
            //
            offset = read_port_status(port_cfg[i%4].port_id);
            temp = read_dword(offset, &rcv_buffer[mid+tid].data[tid], 0);
            cmp_data(temp, gold_msg_payload[i]);
         }
         C_TEST_PASS;
      }
      // Note: Minion 7  / Hart 1, doesn't receive any message hence can't wait for messages
      else
         C_TEST_PASS
   }


}
