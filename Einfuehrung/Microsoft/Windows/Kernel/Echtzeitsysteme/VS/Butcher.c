#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "InputListener.h"
#include  "CoolingBox.h"
#include  "Sausage.h"

OS_TMR* SausageTimer;
boolean timerUsedFlag = 1;

char* event1;
char* event2;
char* event3;

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


static void printState(char* event) {

	char buf[30];

	PC_DispClrScr(5);

		PC_DispStr(0, 0, "##############################################################", 5);
	

	PC_DispStr(0, 3, "##############################################################", 3);
	PC_DispStr(15, 4, "Kuehlbox", 3);

	snprintf(buf, 30, "Anzahl Wuerste: %d", sausagesCountBox); // puts string into buffer
	PC_DispStr(10, 5, buf, 3);

	for (size_t i = 0; i < sausagesCountBox; i++) {
		PC_DispStr(15 + i, 6, "/", 3);
	}
	PC_DispStr(0, 7, "##############################################################", 3);

	PC_DispStr(0, 9, "##############################################################", 4);
	PC_DispStr(15, 10, "Grill", 4);

	for (size_t i = 0; i < sausagesCountGrill; i++) {
		
		SausageNode currentSaus = GetWurstAtIndex(sausagesCountGrill);
			PC_DispStr(15, 11, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideOne);
			PC_DispStr(16, 11, buf, 4);

			PC_DispStr(21, 11, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideTwo);
			PC_DispStr(22, 11, buf, 4);
		
			PC_DispStr(27, 11, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideThree);
			PC_DispStr(28, 11, buf, 4);

			PC_DispStr(33, 11, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideFour);
			PC_DispStr(34, 11, buf, 4);
		
	}

	PC_DispStr(0, 15, "##############################################################", 4);
	PC_DispStr(15, 15, "Events", 4);
	PC_DispStr(0, 15, "##############################################################", 4);



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

		printState("");

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