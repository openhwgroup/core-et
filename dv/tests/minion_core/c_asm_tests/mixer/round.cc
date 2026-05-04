/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "mixer.h"
#include "mtwister.h"

//////////////////////////////////////////////////
//
//  Author: Roger Espasa
//
//  The mixer program is a random scheduler that schedules random kernels onto
//  the available minions. 
//
//  The kernels to be run are described in the kernel_info_t data structure.
//  This structure contains a pointer to each kernel, a string witht the kernel
//  name and a kernel_usage_t value that indicates how many resources does the
//  kernel need. 
//
//  Mixer operates in "rounds". On a given round, the master mixers makes a
//  random decision and assigns a kernel from the list to each slave minion.
//  some kernels can be run on a single minion, other need a full shire and
//  others may even need the full array to run. 
//
//  Slave minions are sitting on an endless loop waiting to receive from the
//  master minion instructions.  The master minion sends a couple integers to
//  each slave minion < kernel_id, count >.  This is done using hardware
//  messages (as an additional measure to stress test the system).  Using the
//  kernel_id, the slave minion knows where to jump (using the kernel_info_t
//  structure).  The 'count' is a repeat count, indicating how many times the
//  kernel should be repeated.  Upon receiving a new tuple, the slave minion
//  records in memory the current cycle time, as well as the kernel_id and the
//  count.  Each time the kernel returns control to the slave endless loop, the
//  slave minion records the end time and decrements the iteration count. This is to
//  be used for debug in case a kernel fails.
//
//  After the 'count' iterations of the kernel have been completed, the slave
//  minion sends a message back to the master minion to indicate completion and
//  goes back to waiting for a new tuple to arrive. 
//
//  The intention of mixer is to eventually be running non-stop on the Zebu
//  system to prove the SoC stability. It is therefore conceivable that a
//  kernel either fails or hangs during execution. In failure case, the kernel
//  will invoke C_TEST_FAIL. This will write a failure code to a validation csr
//  and do a WFI. The expectation is that Zebu will be monitoring in parallel
//  all 1024 validation CSRs and stop collecting ts FSDB upon seeing an error.
//  If the test hangs, neither Zebu nor the master scheduler can directly figure it out.
//  The master scheduler will need to record the times at which it sends work to a
//  given minion and, if it sees too much time has elapsed since receiving an
//  answer from that minion, it should assume it's hung.
//
//  The master minion also operates an endless scheduling loop composed of 
//  three steps:
//    - create_round: randomly picks what to run on each minion
//    - dispatch_round: sends messages to all slaves telling them what to do
//    - wait_round: waits (with timeout) for all slave minions to indicate completion
//

struct
{
    int count;  // Number of times to run the kernel
    int kid;    // Kernel ID
}
random_round[2048];


void set_thread(unsigned int i, unsigned int kid, unsigned int count)
{
          random_round[i].count = count;
          random_round[i].kid   = kid;
}

#define RUN_FC_PROB 0.7

// Special case where we only try to fill the available T1s.
//void create_round_t1(MTRand r, int nshires)
//{
//    int i;
//}

// Create Round randomly decides how to fill up all the available threads
// (which is dictated by nshires). The first-level decision it needs
// to make is whether it picks for this round a full-chip test or not
// (i.e., a test that uses all 2048 threads in the machine, like virus70).
// If it does, fill up the vector(s) with just that one kernel. 
// If it doesn't then fill up the available threads with non-full-chip
// kernels
void create_round(MTRand r, int nshires)
{
    
  // Before we start, clean up the round array
  for (int i = 0; i < 2048; i++ )
  {
      random_round[i].count = 0;
  }

  // First decision to be made: do we have any full chip tests? And if so, 
  // do we have enough shires to run them? If the answer is yes to both,
  // then flip a coin and decide with probability RUN_FC_PROB whether
  // we actually run a full chip test or not
  if ( nshires == 32 && NKERNELS_FC > 0 && genRand(&r) <= RUN_FC_PROB )
  {
      // pick a random kernel id from the FC group
      unsigned int kid  = (unsigned int)(NKERNELS_FC * genRand(&r));

      // if it uses the full chip, just fill up both T0 and T1 with this kid
      if ( kernels[kid].type == USES_ALL_SHIRES )
      {
          for (int i = 0; i < 2048; i += 2 )
          { 
              set_thread(i,   kid, 1);
              set_thread(i+1, kid, 1);
          }
          return;
      }
      // If it uses the FC but just T0, then fill up all T0s with this kid
      // but try to fill up the T1's with something else
      else if ( kernels[kid].type == USES_ALL_SHIRES_T0 )
      {
          for (int i = 0; i < 2048; i += 2 )
          { 
              set_thread(i,   kid, 1);
          }
          //create_round_t1(r, nshires);
          return;
      }
  }


  // If we get here, we're not doing a full-chip kernel. Let's fill-up
  // the available shires with smaller kernels
  int maxt = nshires * 32 * 2;
  for (int i=0; i < maxt; ) 
  {
      int shire = i / 64;
      // pick a random kernel id
      unsigned int kid  = (unsigned int)(NKERNELS * genRand(&r));

      // How many threads/minions does this kernel need?
      if ( kernels[kid].type == USES_ONE_SHIRE && (maxt-i) >= 64 )
      {
          // let's move on to next shire, and we leave this one partially empty
          shire++;
          i = shire * 64;

          // Fill up the 64 threads in shire with the current kid
          for (int count = 0; count < 64; count++ )
          {
              set_thread(i,   kid, 1);
              i ++;
          }
      }
      else if ( kernels[kid].type == USES_ONE_MINION )
      {
          // if we're not on a T0, advance by one
          if (i & 0x1) i++;

          // Put this kid on boths T0 and T1
          set_thread(i,   kid, 1);
          set_thread(i+1, kid, 1);
          i += 2;
      }
      else if ( kernels[kid].type == USES_ONE_THREAD )
      {
          set_thread(i, kid, 1);
          i++;
      }
      else if ( kernels[kid].type == USES_ONE_THREAD_T0 )
      {
          // if we're not on a T0, advance by one
          if (i & 0x1) i++;
          set_thread(i, kid, 1);
          i++;
      }
      else if ( kernels[kid].type == USES_ONE_THREAD_T1 )
      {
          // if we're not on a T1, advance by one
          if ((i & 0x1) == 0) i++;
          set_thread(i, kid, 1);
          i++;
      }
  }
}

void dump_round(int nshires)
{
    for (int s=0; s < nshires; s++ )
    {
        printf("S%2d TO: ",s);
        for (int i=0; i < 64; i += 2)
        {
            int idx = s * 64 + i;
            printf("%2d,", random_round[i].kid);
        }
        printf("\n");
        printf("S%2d T1: ",s);
        for (int i=1; i < 64; i += 2)
        {
            int idx = s * 64 + i;
            printf("%2d,", random_round[i].kid);
        }
        printf("\n");
    }
}


#define MASTER_SHIRE 32

int main()
{
    MTRand r = seedRand(1337);

    create_round(r, MASTER_SHIRE);
    dump_round(MASTER_SHIRE);
    return 0;
}
