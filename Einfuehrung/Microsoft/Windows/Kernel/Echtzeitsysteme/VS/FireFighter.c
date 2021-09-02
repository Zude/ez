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
*                                            Functions
*********************************************************************************************************
*/
 void FireFighter(void* p_arg) {

	 INT8U err;
	 processError(&err, "Griller userInput");

	while (1) {
		printCurrentState("Feuerwehr kontrolliert Grill");
		if (entzuendet) {
			OSSemPend(SemGrill, 0, &err);
			printCurrentState("Feuerwehr loescht Grill!");
			deleteAllOnGrill();
			entzuendet = 0;
			OSSemPost(SemGrill);
		}

		OSTimeDlyHMSM(0, 0, 60, 0);
	}
}
