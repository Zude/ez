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
OS_TMR* SausageTimer;
volatile boolean timerUsedFlag = 1;




/*
*********************************************************************************************************
*                                       Functions
*********************************************************************************************************
*/
void TimerCreateSausageCallback(void* p_arg) {
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


 


 void Butcher(void* p_arg) {

	INT8U err;

	while (1) {

		printCurrentState("ds");

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
				(OS_TMR_CALLBACK)TimerCreateSausageCallback,
				(void*)0,
				(INT8U*)"My Timer #1",
				(INT8U*)&err);
			OSTmrStart((OS_TMR*)SausageTimer,
				(INT8U*)&err);
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}