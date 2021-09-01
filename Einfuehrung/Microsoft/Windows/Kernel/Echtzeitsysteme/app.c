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
#include  "Butcher.h"
#include  "Griller.h"
#include  "app_cfg.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


OS_STK	FleischerTaskStk[FLEISCHER_TASK_STK_SIZE];
OS_STK	GrillmeisterTaskStk[GRILLMEISTER_TASK_STK_SIZE];
OS_STK	PhysikTaskStk[PHYSIK_TASK_STK_SIZE];
OS_STK	FeuerwehrTaskStk[FEUERWEHR_TASK_STK_SIZE];

OS_EVENT* MSG_box;

volatile INT16S key;


volatile int currentTemp = 150;
 int tempFac = 100;
 int tempSetFac = 10;

 boolean entzuendet = 0;


 static  OS_EVENT* msgqueue;
 static  void* MessageStorage[100];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* Counts no. of nodes in linked list */






static void processError(char ProcessName[], INT8U error) {

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
static void Grillmeister(void* p_arg) {

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
		} else if (userInput == 'o') {
			currentTemp += tempSetFac;
		} else if (userInput == 'p') {
			currentTemp -= tempSetFac;
		} else {
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



/*
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
static void Physik(void* p_arg) {

	INT8U err;

	while (1) {

		while (key != 'o' && key != 'p') {

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
		if (key == 'o') {
			currentTemp += tempSetFac;
			printf("Temperatur um %d auf %d erhoeht.\n", tempSetFac, currentTemp);
		} else {
			currentTemp -= tempSetFac;
			printf("Temperatur um %d auf %d verringert.\n", tempSetFac, currentTemp);
		}
		key = NULL; 
		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

/*
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
static void Feuerwehr(void* p_arg) {

	while (1) {
		// check alle 60 s ob brennt
		// wenn ja loesche alle wuerste weg
	}
}




/*
	Main
*/
int	main(void)
{
#if OS_TASK_NAME_EN > 0u
	CPU_INT08U  os_err;
#endif

	// Foglender Code wird benoetigt um den Simulator zu Initialisieren.
	// Dies wird nur für den Simulator gebraucht und ist nicht teil des offiziellen MicroC/OS-II.
	CPU_IntInit();
	Mem_Init();                                                 /* Initialize Memory Managment Module                   */
	CPU_IntDis();                                               /* Disable all Interrupts                               */
	CPU_Init();                                                 /* Initialize the uC/CPU								*/
	OSInit();                                                   /* Initialize uC/OS-II                                  */

	// was das ?
#if OS_TASK_NAME_EN > 0u
	OSTaskNameSet(FLEISCHER_TASK_PRIORITY,
		(INT8U*)"Creator Task",
		&os_err);
#endif

	// Speicher initialisieren, spaeter dynamisch aenderbar machen
	INT8U partErr;
	PartitionPtr = OSMemCreate(Partition, 100, 64, &partErr);

	MSG_box = OSMboxCreate((void*)NULL);
	msgQueueButcher = OSQCreate(&messageStorageButcher, 10);
	msgQueueGriller = OSQCreate(&messageStorageGriller, 10);

	 //Fleischer initialisieren
	SemFleischer = OSSemCreate(1);
	SemBox = OSSemCreate(0);
	SemGrill = OSSemCreate(0);
	OSTaskCreate(Fleischer,
		(void*)0,
		&FleischerTaskStk[FLEISCHER_TASK_STK_SIZE - 1],
		FLEISCHER_TASK_PRIORITY);

	// Grillmeister initialisieren
	OSTaskCreate(Grillmeister,
		(void*)0,
		&GrillmeisterTaskStk[GRILLMEISTER_TASK_STK_SIZE - 1],
		GRILLMEISTER_TASK_PRIORITY);

	// Listener initialisieren
	OSTaskCreate(InputListener,
		(void*)0,
		&ListenerTaskStk[LISTENER_TASK_STK_SIZE - 1],
		LISTENER_TASK_PRIORITY);

	// Physik initialisieren


	// Feuerwehr initialisieren
	OSTaskCreate(Feuerwehr,
		(void*)0,
		&FeuerwehrTaskStk[FEUERWEHR_TASK_STK_SIZE - 1],
		FEUERWEHR_TASK_PRIORITY);

	// Multitasking Starten
	OSStart();

	while (DEF_ON) {                                            /* Should Never Get Here.                               */
		;
	}
}
