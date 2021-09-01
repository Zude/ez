#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "InputListener.h"
#include  "CoolingBox.h"
#include  "Sausage.h"

OS_TMR* SausageTimer;
boolean timerUsedFlag = 1;

void MyTmrCallbackFnct1(void* p_arg) {
	INT8U err;
	// Falls eine Wurst bereits erzeugt wird, warten
	OSSemPend(SemFleischer, 0, &err);

	OSTmrStop((OS_TMR*)SausageTimer, OS_TMR_OPT_NONE, NULL, (INT8U*)&err);

	createWurst(PartitionPtr);
	OSSemPost(SemBox);

	OSTimeDlyHMSM(0, 0, 1, 0);
	OSSemPost(SemFleischer);
	timerUsedFlag = 1;

}

static void printState() {
	PC_DispStr(0, 0, "##############################################################", 5);
	PC_DispStr(0, 1, "##############################################################", 5);
}

/*
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
 void Fleischer(void* p_arg) {

	INT8U err;

	while (1) {

		printState();
	
		//PC_DispClrScr();
		// Input des Users per MQueue checken
		char userInput = (char*)OSQAccept(msgQueueButcher, &err);

		// Wenn der Input korrekt ist, Wurst erzeugen. Sonst schauen ob Box Leer und ggf. Timer starten (60 Sekunden)
		if (userInput == 'w') {
			OSSemPend(SemBox, 0, &err);
			OSTmrStop((OS_TMR*)SausageTimer, OS_TMR_OPT_NONE, NULL, (INT8U*)&err);
			createWurst(PartitionPtr);
			OSSemPost(SemBox);

			OSTimeDlyHMSM(0, 0, 1, 0);
		}
		else if (getCount(coolingBox) == 0 && timerUsedFlag) {
			timerUsedFlag = 0;
			SausageTimer = OSTmrCreate((INT32U)15,
				(INT32U)15,
				(INT8U)OS_TMR_OPT_PERIODIC,
				(OS_TMR_CALLBACK)MyTmrCallbackFnct1,
				(void*)0,
				(INT8U*)"My Timer #1",
				(INT8U*)&err);
			OSTmrStart((OS_TMR*)SausageTimer,
				(INT8U*)&err);
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}