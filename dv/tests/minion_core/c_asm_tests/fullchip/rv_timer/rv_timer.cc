/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$ 
* @version $Release$ 
* @date $Date$ 
* @author 
*
* @brief rvTimerInterrupt.c Main function for all TCs 
*
* Setup SoC to enable TC run 
*/ 
/** 
 *  @Component    Test Function Configuration 
 * 
 *  @Filename     rvTimerInterrupt.c 
 * 
 *  @Description  Contains Test Function
 * 
 *//*======================================================================== */ 



/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */ 

#include "cpu.h"
#include "soc.h"
#include "inth.h" 
#include "macros.h"
#include "rvtimer.h"
#include "tb.h"
#include "minion.h" 

#define MAX_MTIME_CMD  0xFFFFFFFFFFFFFFFF

volatile uint64_t irqCnt;
volatile uint64_t trapCnt;
Rvtimer* RVtimer;
/*Return true if master minion core*/
uint8_t check_master(void)
{ 
  uint64_t hart_id = get_hart_id();
          
  if( (hart_id & 1) == 0) {
     uint64_t minion_id = ((hart_id>>1) & 0x1F);
     uint64_t shire_id  = ((hart_id>>6) & 0x3F);
     if (shire_id == 32 && minion_id == 0) return 1;  
     else return 0;
  }   
  return 0;
}


uint64_t CmpOffset = 150;

void intHandler( uint64_t irqNo ) {
 
    irqCnt ++;
    
    if( irqCnt >= trapCnt ) 
        RVtimer -> mtimecmp = MAX_MTIME_CMD;
    else 
        RVtimer -> mtimecmp = RVtimer -> mtime + CmpOffset; //this should clear MTIP bit in MIP register
}


/* rvTimerInterrupt Test Function */
int main( void )
{

    et_status_t est = ET_PASS;
    
    uint64_t irqNo;
    uint64_t rrVal;
    uint64_t i;
    uint32_t target;

    trapCnt    = 4; 
  
    RVtimer = (Rvtimer*)(R_SHIRE_ESR_BASEADDR + 0xFF800000); //magic number, JIRA VERIF-743
    irqCnt  = 0;
    target  = 10;
    irqNo   = SP_MACHINE_TIMER_IRQ; 

    if (check_master()){
       inth_init(); 
       inth_enableInterrupt(irqNo, target);
       write_esr(PP_MACHINE, 32, REGION_OTHER, SHIRE_OTHER_MTIME_LOCAL_TARGET, 1);
       est += inth_setupInterrupt ( irqNo, intHandler, INTH_DEFAULT_PRIORITY  );

       rrVal   = RVtimer -> mtime;
       RVtimer -> mtimecmp = rrVal + CmpOffset;
       i = 0;
       while( irqCnt < trapCnt ) {
         rrVal = RVtimer -> mtime;
         i ++;
       }

       est += inth_disableInterrupt( irqNo, target );
    }
    if( est )   
      return ET_FAIL;
    else
      return ET_PASS;
    

} /* rvTimerInterrupt_Test() */

/*****     < EOF >     *****/
