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
#include  "FireFighter.h"
#include  "Physics.h"

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

OS_STK	ButcherTaskStk[BUTCHER_TASK_STK_SIZE];
OS_STK	GrillerTaskStk[GRILLER_TASK_STK_SIZE];
OS_STK	PhysicsTaskStk[PHYSICS_TASK_STK_SIZE];
OS_STK	FireFighterTaskStk[FIREFIGHTER_TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTIONS
*********************************************************************************************************
*/



/*
	Main
*/
int	main(void) {

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

	INT8U partErr;

	// Speicher Initialisieren
	PartitionPtr = OSMemCreate(Partition, 100, 64, &partErr);

	// Queues Initialisieren
	msgQueueButcher = OSQCreate(&messageStorageButcher, 10);
	msgQueueGriller = OSQCreate(&messageStorageGriller, 10);

	// Mailboxes Initialisieren
	mailboxGriller = OSMboxCreate(NULL);
	mailboxButcher = OSMboxCreate(NULL);

	// Semaphoren Initialisieren
	SemFleischer = OSSemCreate(0);
	SemBox = OSSemCreate(0);
	SemGrill = OSSemCreate(1);

	 //Fleischer initialisieren
	OSTaskCreate(Butcher,
		(void*)0,
		&ButcherTaskStk[BUTCHER_TASK_STK_SIZE - 1],
		BUTCHER_TASK_PRIORITY);

	// Grillmeister initialisieren
	OSTaskCreate(Griller,
		(void*)0,
		&GrillerTaskStk[GRILLER_TASK_STK_SIZE - 1],
		GRILLER_TASK_PRIORITY);

	// Listener initialisieren
	OSTaskCreate(InputListener,
		(void*)0,
		&ListenerTaskStk[LISTENER_TASK_STK_SIZE - 1],
		LISTENER_TASK_PRIORITY);

	// Physik initialisieren
	OSTaskCreate(Physics,
		(void*)0,
		&PhysicsTaskStk[PHYSICS_TASK_STK_SIZE - 1],
		PHYSICS_TASK_PRIORITY);

	// Feuerwehr initialisieren
	OSTaskCreate(FireFighter,
		(void*)0,
		&FireFighterTaskStk[FIREFIGHTER_TASK_STK_SIZE - 1],
		FIREFIGHTER_TASK_PRIORITY);

	// Multitasking Starten
	OSStart();

	while (DEF_ON) {                                            /* Should Never Get Here.                               */
		;
	}
}

