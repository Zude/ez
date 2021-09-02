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
#include <stdlib.h>
#include <string.h>

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
	PC_DispStr(15, 15, "Grill", 4);
	snprintf(buf, 30, "Aktuelle Temperatur: %d Grad.", currentTemp);
	PC_DispStr(10, 16,buf, 4);
	snprintf(buf, 30, "Anzahl Wurst: %d.", sausagesCountGrill);
	PC_DispStr(10, 17, buf, 4);

	if (entzuendet) {
		PC_DispStr(5, 18, "GRILL BRENNT! GRILL BRENNT!", 9);
	}
	for (size_t i = 0; i < sausagesCountGrill; i++) {

		SausageNode currentSaus = GetWurstAtIndex(sausagesCountGrill);
		PC_DispStr(0, 19 + i, "|", 4);
		snprintf(buf, 30, "%d%", currentSaus->value.sideOne);
		PC_DispStr(1, 19 + i, buf, 4);

		PC_DispStr(6, 19 + i, "|", 4);
		snprintf(buf, 30, "%d%", currentSaus->value.sideTwo);
		PC_DispStr(7, 19 + i, buf, 4);

		PC_DispStr(12, 19 + i, "|", 4);
		snprintf(buf, 30, "%d%", currentSaus->value.sideThree);
		PC_DispStr(13, 19 + i, buf, 4);

		PC_DispStr(18, 19 + i, "|", 4);
		snprintf(buf, 30, "%d%", currentSaus->value.sideFour);
		PC_DispStr(19, 19 + i, buf, 4);

	}

}

char* concat(const char* s1, const char* s2) {
	char* result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
	// in real code you would check for errors in malloc here
	strcpy_s(result, 500, s1, 50);
	strcat_s(result, 500, s2, 50);
	return result;
}

void processError(INT8U error, char* name) {

	switch (error) {
	case OS_ERR_PRIO_EXIST: 
		printCurrentState(concat("Error: Prioritaet bereits vorhanden.", name));
		break;
	case OS_ERR_PRIO_INVALID: 
		printCurrentState(concat("Error: Prioritaet invalide.", name)); 
		break;
	case OS_ERR_ILLEGAL_CREATE_RUN_TIME:
		printCurrentState(concat("Error: Es sind bereits kritische Tasks gestartet.", name)); 
		break;
	case OS_ERR_TASK_CREATE_ISR:
		printCurrentState(concat("Error: Es wurde versucht einen Task auf Interruptebene zu erstellen.", name)); 
		break;
	case OS_ERR_TMR_INVALID_DLY: 
		printCurrentState(concat("Error: you specified an invalid delay.", name)); 
		break;
	case OS_ERR_TMR_INVALID_PERIOD: 
		printCurrentState(concat("Error: you specified an invalid period", name)); 
		break;
	case OS_ERR_TMR_INVALID_OPT:
		printCurrentState(concat("Error: you specified an invalid option", name)); 
		break;
	case OS_ERR_TMR_ISR: 
		printCurrentState(concat("Error: if the call was made from an ISR", name)); 
		break;
	case OS_ERR_TMR_NON_AVAIL: 
		printCurrentState(concat("Error: if there are no free timers from the timer pool", name)); 
		break;
	default: break;
	}
}