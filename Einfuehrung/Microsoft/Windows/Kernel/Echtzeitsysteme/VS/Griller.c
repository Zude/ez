#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "InputListener.h"
#include  "CoolingBox.h"
#include  "Sausage.h"

volatile int currentTemp = 150;
int tempFac = 100;
int tempSetFac = 10;

static void wurstWenden(SausageNode wurst) {

	switch (wurst->value.currentSide)
	{
	case 1:
		if (wurst->value.sideOne > 80) {
			wurst->value.currentSide = 2;
			OSTimeDlyHMSM(0, 0, 5, 0);
		} break;
	case 2:if (wurst->value.sideTwo > 80) {
		wurst->value.currentSide = 3;
		OSTimeDlyHMSM(0, 0, 5, 0);
	} break;
	case 3: if (wurst->value.sideThree > 80) {
		wurst->value.currentSide = 4;
		OSTimeDlyHMSM(0, 0, 5, 0);
	} break;
	default:
		// delete
		OSTimeDlyHMSM(0, 0, 10, 0);
		break;
	}
}

/*
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
 void Griller(void* p_arg) {

	INT8U err;

	while (1) {

		// Input des Users per MQueue checken
		char userInput = (char*)OSQAccept(msgQueueGriller, &err);

		// Wenn der Input korrekt ist, Wurst erzeugen. Sonst schauen ob Box Leer und ggf. Timer starten (60 Sekunden)
		if (userInput == 'g') {
			OSSemPend(SemBox, 0, &err);
			transferWurst(coolingBox, grill);
			OSSemPost(SemBox);
			OSTimeDlyHMSM(0, 0, 5, 0);
		}
		else if (userInput == 'o') {
			currentTemp += tempSetFac;
		}
		else if (userInput == 'p') {
			currentTemp -= tempSetFac;
		}
		else {
			// kontrolliere Wurst 
			int grillSize = getCount(grill);
			// zufallszahl ermitteln

			//SemFleischer2
			if (grillSize > 0)
			{

				int index = rand() % ((grillSize - 1) + 1 - 0) + 0;

				SausageNode wurstToCheck = GetWurstAtIndex(index);

				OSSemPend(SemGrill, 0, &err);
				wurstWenden(wurstToCheck);
				OSSemPost(SemGrill);

				OSTimeDlyHMSM(0, 0, 0, 100);
			}

			OSTimeDlyHMSM(0, 0, 0, 100);
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}