#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "InputListener.h"
#include  "CoolingBox.h"
#include  "Sausage.h"

OS_TMR* SausageTimer;
boolean timerUsedFlag = 1;


char* event1 = "";
char* event2 = "";
char* event3 = "";
char* event4 = "";
char* event5 = "";
char* event6 = "";

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


 void printState(char* event) {

	event1 = event2;
	event2 = event3;
	event3 = event4;
	event4 = event5;
	event5 = event6;
	event6 = event;

	char buf[30];

	PC_DispClrScr(5);

	PC_DispStr(0, 0, "##############################################################", 6);
	PC_DispStr(0, 1, "Events", 6);
	PC_DispStr(0, 2, event1, 6);
	PC_DispStr(0, 3, event2, 6);
	PC_DispStr(0, 4, event3, 6);
	PC_DispStr(0, 5, event4, 6);
	PC_DispStr(0, 6, event5, 6);
	PC_DispStr(0, 7, event6, 6);
	PC_DispStr(0, 8, "##############################################################", 6);
	
	

	PC_DispStr(0, 9, "##############################################################", 3);
	PC_DispStr(15, 10, "Kuehlbox", 3);

	snprintf(buf, 30, "Anzahl Wuerste: %d", sausagesCountBox); // puts string into buffer
	PC_DispStr(10, 11, buf, 3);

	for (size_t i = 0; i < sausagesCountBox; i++) {
		PC_DispStr(15 + i, 12, "/", 3);
	}
	PC_DispStr(0, 13, "##############################################################", 3);
	PC_DispStr(0, 14, "##############################################################", 4);
	PC_DispStr(0, 15, "Grill", 4);

	for (size_t i = 0; i < sausagesCountGrill; i++) {
		
		SausageNode currentSaus = GetWurstAtIndex(sausagesCountGrill);
			PC_DispStr(0, 16, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideOne);
			PC_DispStr(1, 16, buf, 4);

			PC_DispStr(6, 16, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideTwo);
			PC_DispStr(7, 16, buf, 4);
		
			PC_DispStr(12, 16, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideThree);
			PC_DispStr(13, 16, buf, 4);

			PC_DispStr(18, 16, "|", 4);
			snprintf(buf, 30, "%d%", currentSaus->value.sideFour);
			PC_DispStr(19, 16, buf, 4);
		
	}

}


 void Butcher(void* p_arg) {

	INT8U err;

	while (1) {

		printState("hez");

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