/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author
*
* @brief tb.c Test Bench interface
*
* SW for various SW <-> SV synchronization
*/



/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */
#include "cpu.h"
#include "soc.h"

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "tb.h"

#include "macros.h"

#include "serial.h"


//#include "api.h"
//#include "spy.h"


/* =============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * =============================================================================
 */

TB_t *pTB;



/* =============================================================================
 * LOCAL TYPES AND DEFINITIONS
 * =============================================================================
 */



/* =============================================================================
 * LOCAL VARIABLES DECLARATIONS
 * =============================================================================
 */

uint64_t *pTb_Id;
uint64_t *pTb_Param;

uint64_t noPrintf;

uint64_t *printArgs;



/* =============================================================================
 * LOCAL FUNCTIONS DECLARATIONS
 * =============================================================================
 */

void tb_write( uint64_t Id, uint64_t Param );



/* =============================================================================
 * EXTERNAL VARIABLES DECLARATIONS
 * =============================================================================
 */

// tb Area

/* From Scatter File  */

extern uint64_t __tb_Id;
extern uint64_t __tb_Param;
extern uint64_t __tb_base_address;
//extern uint64_t __msg_base_address;
extern uint64_t __spy_base_address;
extern uint64_t __tcParam_base_address;
extern uint64_t __tb_end_address;
//extern uint64_t __msg_end_address;
extern uint64_t __spy_end_address;
//extern uint64_t __tcParam_end_address;
extern uint64_t __soc_end_address;


// From Soc.h
extern uint32_t thisCpu;


//from tcParam.h
void* tc_param_random_ptr;
void* tc_param_default_ptr;
int tc_param_seed;

 /* =============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */



/* =============================================================================
 * FUNCTIONS
 * =============================================================================
 */



/*==================== Function Separator =============================*/
void tb_init(void)
{

    et_status_t est;
    uint32_t err;
    uint64_t addr;

    /* Set Global Pointers */
    pTB = (TB_t *)&__tb_base_address;
    /* Init tbId/Param  pointers */
    pTb_Id    = &__tb_Id;
//    pTb_Param = &__tb_Param;

    pTb_Id[0]    = 0;
    pTb_Id[1]    = 0;
    pTb_Id[2]    = 0xCCCC;


    /* Set CPU Id */
    pTB->cpuId = CPU;

    /* Configure tb from scatter file */
/*
    pTB->msgQueueAddress = (uint64_t)&__msg_base_address;
    pTB->msgQueueSize    = (uint64_t)(&__msg_end_address) - (uint64_t)(&__msg_base_address);
*/
    pTB->msgQueueAddress = 0;
    pTB->msgQueueSize    = 0;

    pTB->spyAddress      = (uint64_t)&__spy_base_address;
    pTB->spySize         = (uint64_t)(&__spy_end_address) - (uint64_t)(&__spy_base_address);

    pTB->printfAddress   = 0;
    pTB->printfSize      = 0;
    pTB->printModeSelect = 0;   // FUTURE: add Enums. UVM printf

    /* Set TC param area */
    pTB->tcParamAddress  = ( void *)&__tcParam_base_address;
    pTB->tcParamSelect   = TB_TC_PARAM_RANDOM;

    addr = (uint64_t)&__soc_end_address;
    addr += 0x10;
    addr &= ~0xF;

    printArgs = (uint64_t *)addr;

    /* Set TC status as started */
    pTB->tcStatus = TB_TC_STARTED;
    __asm__ __volatile__ ("fence\n");

    /* Set master TC status as started */
    pTB->masterTcStatus = TB_TC_STARTED;
    __asm__ __volatile__ ("fence\n");

    noPrintf  = 0;

    printDbg( "Completed tb_init()\n" );
} /* tb_init() */



/*==================== Function Separator =============================*/
void tb_finish( uint32_t status )
{

    uint32_t i;
    uint32_t error;
    TB_t *pSlaveTB;

    pTB->tcStatus = TB_TC_FINISH | ( status & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    error = ( status & 0xFFFF ) ? 1 : 0;

    /* All Slaves Completed, Stop Simulation */
    i = ( error ) ? TB_TC_FAIL : TB_TC_PASS;
    pTB->masterTcStatus = TB_TC_FINISH | i;


   /* Send Error status to stop Simulation */
   tb_write( TB_STOP , status & 0xFFFF );

   /* Stop BEMU */
   if( error ) {
      C_TEST_FAIL;
   }
   else {
      C_TEST_PASS;
   }


    __asm__ __volatile__ ("fence\n");

    while( 1 ) {
        WFI;
        printDbg( "Unexpected Wakeup while TC is finished !!!\n" );
    }

} /* tb_finish() */



/*==================== Function Separator =============================*/
void tb_fatalError( uint32_t error )
{

    TB_t *pMasterTB;

    pTB->tcStatus = TB_TC_FATAL_ERROR | ( error & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    /* At least one CPU detected Fatal Error */
    /* So stop simulation immediatelly */
    /* FUTURE: Use some msg  or irq to master CPU */

        while( 1 ) {
//        __WFI();
//        printDbg( "Unexpected Wakeup while TC has Fatal Error !!!\n" );
    }

} /* tb_fatalError() */



/*==================== Function Separator =============================*/
void tb_setTCStatus( TB_TC_STATUS_t status )
{

    pTB->tcStatus = status;
    __asm__ __volatile__ ("fence\n");

} /* tb_setTCStatus() */


/*==================== Function Separator =============================*/
int tb_printf(char *format, ...)
{

    va_list a_list;

    uint64_t data_ptr_bckp;
    char ch;
    char *p;
    uint64_t num;
    char buf[12];
    char* str;
    unsigned long div;
    int length;
    
     va_start( a_list, format);

    while (*format){
        ch = *(format); 
        if (ch != '%') {
          SERIAL_putc(UART0, *format++);
       } else      {
           //ch = *(format++); 
            ch = *(++format);

            if (ch == '0') {
                ch = *(++format);
            }

            switch (ch) {
            case 'u':
            case 'd':
                div = 1000000000;
                num = va_arg(a_list, long);
        
                if (!num) {
                    SERIAL_putc(UART0, '0');
                } else {    
                    length = snprintf( NULL, 0, "%d", num );
                    str = malloc( length + 1 );
                    snprintf( str, length + 1, "%d", num );
                    for( int i = 0; i < length; i++ ) 
                        SERIAL_putc(UART0, *str++);
                    free(str);                
                       // sprintf(buf, "%d", num);
                       // for( int i = 0; i < 11; i++ ) 
                       //     SERIAL_putc(UART0, buf[i]);
                }
                break;
            case 'X':
            case 'x':
                    num = va_arg(a_list, int);
                if (!num) {
                    SERIAL_putc(UART0, '0');
                } else {   
                    length = snprintf( NULL, 0, "%x", num );
                    str = malloc( length + 1 );
                    snprintf( str, length + 1, "%x", num );
                    for( int i = 0; i < length; i++ ) 
                        SERIAL_putc(UART0, *str++);
                    free(str);                 
                        //sprintf(buf, "%x", num);
                        //for( int i = 0; i < 11; i++ ) 
                        //    SERIAL_putc(UART0, buf[i]);
                }
                break;
            case 'h':
                    num = va_arg(a_list, int);
                if (!num) {
                    SERIAL_putc(UART0, '0');
                } else {  
                    length = snprintf( NULL, 0, "%x", num );
                    str = malloc( length + 1 );
                    snprintf( str, length + 1, "%x", num );
                    for( int i = 0; i < length; i++ ) 
                        SERIAL_putc(UART0, *str++);
                    free(str);                  
                      //  sprintf(buf, "%x", num);
                      //  for( int i = 0; i < 11; i++ ) 
                      //      SERIAL_putc(UART0, buf[i]);
                }
                break;   
            case 'c':
                SERIAL_putc(UART0, ch);
                break;
            case 's':
                p = va_arg(a_list, char*);
                while ((ch = *p++))
                    SERIAL_putc(UART0, ch);
                break;
            default:
                break;
            }

            format++;

        } 

    }
   
    SERIAL_putc(UART0, '\n');

    va_end( a_list );

    return 0;

} /* tb_printf() */



/*==================== Function Separator =============================*/
int64_t tb_getRandom( char *format, ... )
{

    va_list a_list;

    uint64_t data_ptr_bckp;

    printArgs[0] = (uint64_t)format;
    va_start( a_list, format);

    // for( int i = 1; i < 16; i++ ) {
    //      printArgs[i] = va_arg( a_list, uint64_t );
    // }

    // tb_write( TB_RANDOM, printArgs );

    char word[64];
    int rand_mode = 0;
    int64_t val[16];
    int64_t res;
    int cnt = 0;
	char x, next_x;
	int word_index = 0;

    while (*format)
    { 
        x = *format;
        format++; 
        next_x = *format;
        
        if (x == ' ') 
        { 
        	if (strncmp(word, "RANGE",5) == 0){
                rand_mode = 0;
        	}else if(strncmp(word, "MEMBER",6) == 0){
                rand_mode = 1;
        	}else if(strncmp(word,"%0d",3) == 0){
                val[cnt] = va_arg(a_list, int64_t);
                cnt++;
        	}else{
        		val[cnt] = strtol(word, NULL, 10); //atoi(word);
        		cnt++;
        	} 

            word_index = 0;
            word[0] = '\0';
        } 
        else
        { 
          word[word_index] = x; 
          if(next_x == '\0'){
            if(strncmp(word,"%0d",3) == 0){
                val[cnt] = va_arg(a_list, int64_t);
                cnt++;
        	}else{
        		val[cnt] = strtol(word, NULL, 10); //atoi(word);
        		cnt++;
        	}  
          }
	
          word_index++;
        }

    }  

    srand(tc_param_seed);

    if(rand_mode == 0){
    	res = (rand() % (val[1] - val[0] + 1)) + val[0];
    }else{
    	res = rand() % (cnt + 1);
    	res = val[res];
    }

    va_end( a_list );


	// if(rand_mode == 0){
 //    	tb_printf("DEBUG tb_getRandom(): rand_mode   RANGE");
 //    }else{
 //    	tb_printf("DEBUG tb_getRandom(): rand_mode   MEMBER");
 //    }

	// for (int i = 0; i < cnt; i++)
	// {
	//     tb_printf("DEBUG tb_getRandom(): val[%d] == %d" , i,  val[i]);
	// }

	 tb_printf("DEBUG SEED == %d", tc_param_seed);
	 tb_printf("DEBUG tb_getRandom(): RESULT == %d" , res);

    return res;
    //return printArgs[0];

} /* tb_getRandom() */



/*==================== Function Separator =============================*/
void *tb_getTcParamPtr( uint64_t tcParamType )
{
    switch( tcParamType ) {
        case  TB_TC_PARAM_DEFAULT :
        	{
        		// tb_printf("DEBUG GORAN tb_getTcParamPtr(): tc_param_default_ptr   [%d]", tc_param_default_ptr);
            	return tc_param_default_ptr;
            }	
        case  TB_TC_PARAM_RANDOM :
        	{
        		// tb_printf("DEBUG GORAN tb_getTcParamPtr(): tc_param_random_ptr   [%d]", tc_param_random_ptr);
            	return tc_param_random_ptr;
            }	
        default:
            return NULL;
    }
} /* tb_getTcParamPtr() */



/*==================== Function Separator =============================*/
void tb_runningCPUs( uint64_t runningCPUs )
{

    tb_write( TB_RUNNING_CPUS, runningCPUs );

} /* tb_runningCPUs() */



/*==================== Function Separator =============================*/
void tb_spy( uint64_t spyVal )
{

    tb_write( TB_SPY, spyVal );

} /* tb_spy() */



/*==================== Function Separator =============================*/
void tb_sync(uint64_t syncVal)
{

    tb_write( TB_SYNC, syncVal );

} /* tb_Sync() */



/*==================== Function Separator =============================*/
void tb_buff(uint64_t *buff)
{

    tb_write( TB_BUFF, buff );

} /* tb_buff() */


/*==================== Function Separator =============================*/
void tb_measureClock( uint64_t clockID, uint64_t address )
{

    uint64_t clockMeasureDataStruct[2];
    clockMeasureDataStruct[0] = clockID;
    clockMeasureDataStruct[1] = address;

    tb_write(64, &clockMeasureDataStruct);

} /* tb_measureClock() */


/*==================== Function Separator =============================*/
void tb_genuine_boot(uint64_t action )
{

    tb_write(66, action);

} /* tb_genuine_boot() */


/*==================== Function Separator =============================*/
void tb_external_clk_sel(uint64_t strap_in )
{

    tb_write(69, strap_in);

} /* tb_external_clk_sel() */



/*==================== Function Separator =============================*/
void tb_sendSoCaddr( uint64_t SoCbaseAddr )
{

    tb_write(67, SoCbaseAddr ); /* fm_clockConfig_handler.sv */

} /* tb_sendSoCaddr() */

/*==================== Function Separator =============================*/
void tb_sendPadsAddr( uint64_t padsBaseAddr )
{

    tb_write(70, padsBaseAddr ); /* fm_forcePads_handler.sv */

} /* tb_sendPadsAddr() */

/*==================== Function Separator =============================*/
void tb_write( uint64_t Id, uint64_t Param )
{

/*
    uint64_t pData[3];

    switch( Id ) {
        case TB_STOP:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_TBCFG:
            pData[0] = 0;
            pData[1] = pTB;
            pData[2] = 0xABBA;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_SPY:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_SYNC:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_BUFF:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        default:
            pTb_Id[1] = Param;

    }
*/

    pTb_Id[1] = Param;
    __asm__ __volatile__ ("fence\n");
    pTb_Id[0] = Id;
    __asm__ __volatile__ ("fence\n");

   /* Clear Id to enable next one  */
    pTb_Id[0] = 0;
    __asm__ __volatile__ ("fence\n");


} /* tb_write() */



/*****     <EOF>     *****/
