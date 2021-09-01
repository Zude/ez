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

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
char* event1 = "";
char* event2 = "";
char* event3 = "";
char* event4 = "";
char* event5 = "";
char* event6 = "";

/*
*********************************************************************************************************
*                                       Functions
*********************************************************************************************************
*/

void processError(char ProcessName[], INT8U error) {

	switch (error) {
	case OS_ERR_PRIO_EXIST: printf("%s Error: Prioritaet bereits vorhanden.\n", ProcessName); break;
	case OS_ERR_PRIO_INVALID: printf("%s Error: Prioritaet invalide.\n", ProcessName); break;
	case OS_ERR_ILLEGAL_CREATE_RUN_TIME: printf("%s Error: Es sind bereits kritische Tasks gestartet.\n", ProcessName); break;
	case OS_ERR_TASK_CREATE_ISR: printf("%s Error: Es wurde versucht einen Task auf Interruptebene zu erstellen.\n", ProcessName); break;
	case OS_ERR_TMR_INVALID_DLY:printf("%s Error: you specified an invalid delay\n", ProcessName); break;
	case OS_ERR_TMR_INVALID_PERIOD:printf("%s  Error: you specified an invalid period\n", ProcessName); break;
	case OS_ERR_TMR_INVALID_OPT:printf("%s Error: you specified an invalid option\n", ProcessName); break;
	case OS_ERR_TMR_ISR:printf("%s Error: if the call was made from an ISR\n", ProcessName); break;
	case OS_ERR_TMR_NON_AVAIL:printf("%s  Error: if there are no free timers from the timer pool\n", ProcessName); break;
	default: break;
	}
}

void printCurrentState(char* event) {

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