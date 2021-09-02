/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "InputListener.h"
#include  "CoolingBox.h"
#include  "Sausage.h"
#include  "Helper.h"

/*
*********************************************************************************************************
*                                       GLOBAL VARIABLES
*********************************************************************************************************
*/
volatile boolean timerUsedFlag = 1;

/*
*********************************************************************************************************
*                                      LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
OS_TMR* SausageTimer;

/*
*********************************************************************************************************
*                                       Functions
*********************************************************************************************************
*/

void createNewSausage() {

	INT8U err;

	processError(&err, "Create new Saus Pend Sem");
	OSTmrStop((OS_TMR*)SausageTimer, OS_TMR_OPT_NONE, NULL, (INT8U*)&err);
	processError(&err, "Create new Saus Timer Stop");
	createWurst(PartitionPtr);
	OSSemPost(SemBox);
}
void TimerCreateSausageCallback(void* p_arg) {
	
	createNewSausage();
	timerUsedFlag = 1;
	printCurrentState("Timer erzeugt Wurst.");
	OSTimeDlyHMSM(0, 0, 1, 0);
}

void Butcher(void* p_arg) {

	INT8U err;

	while (1) {

		//printCurrentState("ds");

		// User Input abfangen
		char userInput = (char*)OSQAccept(msgQueueButcher, &err);
		processError(&err, "Butcher OSQAccept");

		// Wenn der Input korrekt ist, Wurst erzeugen. Sonst schauen ob Box Leer und ggf. Timer starten (60 Sekunden)
		if (userInput == 'w') {
			createNewSausage();
			printCurrentState("Fleischer erzeugt Wurst.");
			OSTimeDlyHMSM(0, 0, 1, 0);
		} else if (userInput == 'e') {
			printCurrentState("Schlachter trinkt bier.");
			OSMboxPend(mailboxButcher, 0, &err);
			processError(&err, "Schlachter OSMbox Pend");
			printCurrentState("Schlachter trinkt nicht mehr.");
			OSTimeDlyHMSM(0, 0, 0, 100);
		} else if (getCount(coolingBox) == 0 && timerUsedFlag) {
			timerUsedFlag = 0;
			SausageTimer = OSTmrCreate((INT32U)15,
				(INT32U)15,
				(INT8U)OS_TMR_OPT_PERIODIC,
				(OS_TMR_CALLBACK)TimerCreateSausageCallback,
				(void*)0,
				(INT8U*)"Sausage Timer",
				(INT8U*)&err);

			printCurrentState("Box leer, Timer startet.");
			processError(&err, "Butcher init Timer");

			OSTmrStart((OS_TMR*)SausageTimer,
				(INT8U*)&err);
			processError(&err, "Butcher start Timer");
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}