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







/*
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
static void Physik(void* p_arg) {

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
		} else {
			currentTemp -= tempSetFac;
			printf("Temperatur um %d auf %d verringert.\n", tempSetFac, currentTemp);
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
	OSTaskCreate(Griller,
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
