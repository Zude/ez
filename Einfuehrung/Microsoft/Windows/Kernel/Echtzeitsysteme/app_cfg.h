/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*                                             uC/OS-III
*                                           
*
* 
*********************************************************************************************************
*/

#ifndef  _APP_CFG_H_
#define  _APP_CFG_H_


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>

/*
*********************************************************************************************************
*                                           TASK PRIORITIES
*********************************************************************************************************
*/
#define BUTCHER_TASK_PRIORITY 12
#define PHYSICS_TASK_PRIORITY 9
#define GRILLER_TASK_PRIORITY 11
#define FIREFIGHTER_TASK_PRIORITY 10


/*
*********************************************************************************************************
*                                          TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define FIREFIGHTER_TASK_STK_SIZE 512
#define GRILLER_TASK_STK_SIZE 512
#define PHYSICS_TASK_STK_SIZE 512
#define BUTCHER_TASK_STK_SIZE 512
#define  OS_TASK_TMR_PRIO                                   (OS_LOWEST_PRIO - 2u)

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  APP_TRACE_LEVEL                   TRACE_LEVEL_DBG
#define  APP_TRACE                         printf

#define  APP_TRACE_INFO(x)    ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)     ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_TRACE x) : (void)0)


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.              */

