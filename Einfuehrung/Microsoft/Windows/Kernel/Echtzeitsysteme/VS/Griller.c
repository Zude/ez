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
volatile int currentTemp = 150;
int tempFac = 100;
int tempSetFac = 10;

/*
*********************************************************************************************************
*                                       Functions
*********************************************************************************************************
*/

/*
 *	Wendet die Wurst
 *
 * 	Arguments : wurst Wurst die gewendet werden soll
 */
void turningSausage(SausageNode sausage, SausageNode prev) {

	switch (sausage->value.currentSide)
	{
	case 1:
		if (sausage->value.sideOne > 80) {
			sausage->value.currentSide = 2;
			printCurrentState("Griller dreht Wurst auf Seite 2.");
			OSTimeDlyHMSM(0, 0, 5, 0);
		} break;
	case 2:if (sausage->value.sideTwo > 80) {
		sausage->value.currentSide = 3;
		printCurrentState("Griller dreht Wurst auf Seite 3.");
		OSTimeDlyHMSM(0, 0, 5, 0);
	} break;
	case 3: if (sausage->value.sideThree > 80) {
		sausage->value.currentSide = 4;
		printCurrentState("Griller dreht Wurst auf Seite 4.");
		OSTimeDlyHMSM(0, 0, 5, 0);
	} break;
	default:
		deleteWurst(prev, sausage);
		printCurrentState("Griller nimmt fertige Wurst weg.");
		OSTimeDlyHMSM(0, 0, 10, 0);
		break;
	}
}

 void Griller(void* p_arg) {

	INT8U err;

	while (1) {

		// Input des Users per MQueue checken
		char userInput = (char*)OSQAccept(msgQueueGriller, &err);
		processError(&err, "Griller userInput");

		// Wenn der Input korrekt ist, Wurst erzeugen. Sonst schauen ob Box Leer und ggf. Timer starten (60 Sekunden)
		if (userInput == 'g') {
			OSSemPend(SemBox, 0, &err);
			processError(&err, "Griller userInput");
			transferSausage(coolingBox, grill);
			printCurrentState("Griller nimmt Wurst und grillt sie.");
			OSTimeDlyHMSM(0, 0, 5, 0);
		}
		else if (userInput == 'o') {
			currentTemp += tempSetFac;
			printCurrentState("Griller erhoeht Temperatur.");
		}
		else if (userInput == 'p') {
			currentTemp -= tempSetFac;
			printCurrentState("Griller verringert Temperatur.");
		}
		else {

			int grillSize = getCount(grill);

			// Kontrolliere Zufalls Wurst, wenn min 1 vorhanden
			if (grillSize > 0)
			{
				int index = rand() % ((grillSize - 1) + 1 - 0) + 0;

				OSSemPend(SemGrill, 0, &err);
				SausageNode sausToCheck = GetWurstAtIndex(index);
				SausageNode prevSaus = GetWurstAtIndex(index-1);
				printCurrentState("Griller kontrolliert Wurst.");
				turningSausage(sausToCheck, prevSaus);
				OSSemPost(SemGrill);
				OSTimeDlyHMSM(0, 0, 0, 100);
			}

			OSTimeDlyHMSM(0, 0, 0, 100);
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}