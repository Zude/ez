#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "InputListener.h"
#include  "CoolingBox.h"
#include  "Sausage.h"
#include  "Griller.h"

/*
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
void Physics(void* p_arg) {

	INT8U err;

	while (1) {

		while (1) {

			//	char hi = (char*)OSMboxQuery(MSG_box);
				//	OSQPost(msgqueue, (void*)'c');
				//printf("YO: %c\n", hi);
			int count = 1;
			SausageNode current = grill;

			int temp = currentTemp / tempFac;

			while (current != NULL) {

				int currentSide;
				int braunung = 0;

				// wende Physik an (temp 150 grad * factor)

				switch (current->value.currentSide)
				{
				case 1:
					currentSide = 1;
					current->value.sideOne += temp;
					braunung = current->value.sideOne;
					break;
				case 2:
					currentSide = 2;
					current->value.sideTwo += temp;
					braunung = current->value.sideTwo;
					break;
				case 3:
					currentSide = 3;
					current->value.sideThree += temp;
					braunung = current->value.sideThree;
					break;
				case 4:
					currentSide = 4;
					current->value.sideFour += temp;
					braunung = current->value.sideFour; break;
				default:
					currentSide = 0;
					break;
				}

				printf("Wurst %d, Seite %d ist zu %d gebraeunt! \n", count, currentSide, braunung);
				count++;

				if (braunung > 100)
				{
					entzuendet = 1;
					printf("Achtung, der Grill brennt! \n");
				}
				current = current->next;
			}
			OSTimeDlyHMSM(0, 0, 1, 0);
		}
		if (1) {
			currentTemp += tempSetFac;
			printf("Temperatur um %d auf %d erhoeht.\n", tempSetFac, currentTemp);
		}
		else {
			currentTemp -= tempSetFac;
			printf("Temperatur um %d auf %d verringert.\n", tempSetFac, currentTemp);
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}