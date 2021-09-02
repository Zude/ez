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
#include  "Griller.h"
#include  "Helper.h"

/*
*********************************************************************************************************
*                                            Functions
*********************************************************************************************************
*/
void Physics(void* p_arg) {

	INT8U err;

	while (1) {

		OSSemPend(SemGrill, 0, &err);
		processError(&err, "Physics pend");
			int count = 1;
			SausageNode current = grill;
			int temp = currentTemp / tempFac;

			while (current != NULL) {

				int currentSide;
				int cooked = 0;

				switch (current->value.currentSide)
				{
				case 1:
					currentSide = 1;
					current->value.sideOne += temp;
					cooked = current->value.sideOne;
					break;
				case 2:
					currentSide = 2;
					current->value.sideTwo += temp;
					cooked = current->value.sideTwo;
					break;
				case 3:
					currentSide = 3;
					current->value.sideThree += temp;
					cooked = current->value.sideThree;
					break;
				case 4:
					currentSide = 4;
					current->value.sideFour += temp;
					cooked = current->value.sideFour; break;
				default:
					currentSide = 0;
					break;
				}

				count++;

				if (cooked > 100)
				{
					entzuendet = 1;
					printCurrentState("ACHTUNG! Grill hat sich entzündet!.");
				}
				current = current->next;
			}
			OSSemPost(SemGrill);
			OSTimeDlyHMSM(0, 0, 5, 0);
	}
}