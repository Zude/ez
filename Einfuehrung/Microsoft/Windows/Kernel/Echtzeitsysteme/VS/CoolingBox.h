/*
*********************************************************************************************************
*
*                                          Input Listener
*
*********************************************************************************************************
*/
#ifndef COOLINGBOX_H
#define COOLINGBOX_H

/*
*********************************************************************************************************
*                                          INCLUDE FILES
*********************************************************************************************************
*/
#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "Sausage.h"

/*
*********************************************************************************************************
*                                          Defines
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          Globals
*********************************************************************************************************
*/
extern OS_MEM* PartitionPtr;
extern INT8U	Partition[100][32];

extern volatile SausageNode coolingBox;
extern volatile SausageNode grill;

extern OS_EVENT* SemFleischer;
extern OS_EVENT* SemBox;
extern OS_EVENT* SemGrill;

extern  boolean entzuendet;

/*
*********************************************************************************************************
*                                          Methods
*********************************************************************************************************
*/
/*
 *	Der InputListener horcht auf Tastatureingaben und leitet diese an andere Tasks weiter
 *
 * 	Arguments : p_arg nicht verwendet
 */
SausageNode createWurst(OS_MEM* partitionPtr);
void transferWurst();
SausageNode GetWurstAtIndex(int index);
int getCount(SausageNode head);
static void DeleteWurst(SausageNode prevNode, SausageNode toBeRemoved, OS_MEM* parition);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/
#endif  