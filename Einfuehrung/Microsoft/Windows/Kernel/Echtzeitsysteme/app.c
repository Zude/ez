/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "app_cfg.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
struct Wurst {
	int aktuelleSeite;
	int seite1;
	int seite2;
	int seite3;
	int seite4;
};

struct Node {
	struct Wurst value;
	struct Node* next;
};

typedef struct Node* WurstNode;

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
OS_EVENT* SemFleischer;
OS_EVENT* SemBox;

OS_STK	FleischerTaskStk[FLEISCHER_TASK_STK_SIZE];
OS_STK	GrillmeisterTaskStk[GRILLMEISTER_TASK_STK_SIZE];
OS_STK	PhysikTaskStk[PHYSIK_TASK_STK_SIZE];
OS_STK	FeuerwehrTaskStk[FEUERWEHR_TASK_STK_SIZE];

OS_MEM* PartitionPtr;
INT8U	Partition[100][32];

OS_EVENT* MSG_box;

volatile WurstNode kuehlbox;
volatile WurstNode grill;
int wuersteInKuehlbox;

OS_TMR  *MyTmr1;

volatile INT16S key;

boolean test = 1;
volatile int currentTemp = 150;
 int tempFac = 100;
 int tempSetFac = 10;

 boolean entzuendet = 0;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* Counts no. of nodes in linked list */
int getCount(WurstNode head)
{
	int count = 0; // Initialize count
	WurstNode current = head; // Initialize current
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}

WurstNode GetWurstAtIndex(int index)
{

	WurstNode current = grill;

	// the index of the
	// node we're currently
	// looking at
	int count = 0;
	while (current != NULL) {
		if (count == index)
			return current;
		count++;
		current = current->next;
	}
}

static WurstNode createWurst() {

	INT8U err;

	WurstNode newNode = OSMemGet(PartitionPtr, &err);;

	newNode->next = kuehlbox;
	newNode->value.seite1 = 0;
	newNode->value.seite2 = 0;
	newNode->value.seite3 = 0;
	newNode->value.seite4 = 0;
	newNode->value.aktuelleSeite = 1;

	kuehlbox = newNode;

	printf("Fleiwscher: Erzeuge neue Wurst!\n");
	printf("Kuehlbox: Es sind %d Wuerste in der Box!\n", getCount(kuehlbox));

	return kuehlbox;
}

static void DeleteWurst(WurstNode prevNode, WurstNode toBeRemoved, OS_MEM* parition)
{
	if (prevNode)
		prevNode->next = toBeRemoved->next;

	OSMemPut(parition, toBeRemoved);
}

static void transferWurst()
{
	WurstNode transfer = kuehlbox->next;

	kuehlbox->next = grill;
	grill = kuehlbox;

	kuehlbox = transfer;
	printf("Grillmeister: Entnehme Wurst aus Box und plaziere auf Grill!\n");
	printf("Kuehlbox: Es sind %d Wuerste in der Box!\n", getCount(kuehlbox));
	printf("Grill: Es sind %d Wuerste auf den Grill!\n", getCount(grill));
}

void MyTmrCallbackFnct1(void* p_arg)
{
	INT8U err;
	// Falls eine Wurst bereits erzeugt wird, warten
	OSSemPend(SemFleischer, 0, &err);

	OSTmrStop((OS_TMR*)MyTmr1, OS_TMR_OPT_NONE, NULL, (INT8U*)&err);

	createWurst();
	OSSemPost(SemBox);

	OSTimeDlyHMSM(0, 0, 1, 0);
	OSSemPost(SemFleischer);
	test = 1;

}



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

static void wurstWenden(WurstNode wurst) {

	switch (wurst->value.aktuelleSeite)
	{
	case 1:
		if (wurst->value.seite1 > 80) {
			wurst->value.aktuelleSeite = 2;
		} break;
	case 2:if (wurst->value.seite2 > 80) {
		wurst->value.aktuelleSeite = 3;
	} break;
	case 3: if (wurst->value.seite3 > 80) {
		wurst->value.aktuelleSeite = 4;
	} break;
	default:
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


		while (key != 'g') {
			// kontrolliere Wurst 
			int grillSize = getCount(grill);
			// zufallszahl ermitteln

			char hi = (char*)OSMboxAccept(MSG_box);
			printf("YO2: %c\n", hi);

			if (grillSize > 0)
			{
				int index = rand() % ((grillSize - 1) + 1 - 0) + 0;

				WurstNode wurstToCheck = GetWurstAtIndex(index);
				// drehen routine
				wurstWenden(wurstToCheck);
			}
			
			OSTimeDlyHMSM(0, 0, 0, 100);
		}
	 
		key = NULL;
		OSSemPend(SemBox, 0, &err);
		transferWurst(kuehlbox, grill);
		
		OSTimeDlyHMSM(0, 0, 5, 0);
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

		char hi = (char*)OSMboxAccept(MSG_box);
		printf("YO: %c\n", hi);
			int count = 1;
			WurstNode current = grill;

			int temp = currentTemp / tempFac;

		while (current != NULL) {

			int aktuelleSeite;
			int braunung = 0;

			// wende Physik an (temp 150 grad * factor)
			
			switch (current->value.aktuelleSeite)
			{
			case 1:
				aktuelleSeite = 1;
				current->value.seite1 += temp;
				braunung = current->value.seite1;
				 break;
			case 2:
				aktuelleSeite = 2;
				current->value.seite2 += temp;
				braunung = current->value.seite2;
			 break;
			case 3: 
				aktuelleSeite = 3;
				current->value.seite3 += temp;
				braunung = current->value.seite3; 
				break;
			case 4:
				aktuelleSeite = 4;
				current->value.seite4 += temp;
				braunung = current->value.seite4; break;
			default:
				aktuelleSeite = 0;
				break;
			}

			printf("Wurst %d, Seite %d ist zu %d gebraeunt! \n", count, aktuelleSeite, braunung);
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
 *	Der Erzeugertask "erzeugt" bei jedem Tastendruck ein Zeichen und gibt
 * 	es auf dem Bildschirm aus.
 *
 * 	Arguments : p_arg nicht verwendet
 */
static void Fleischer(void* p_arg) {

	INT8U err;


	while (1) {

		// Warte das der User w drueckt um eine Wurst zu erzeugen
		while (!PC_GetKey(&key) || key != 'w') {

			

			if (getCount(kuehlbox) == 0 && test) {
				test = 0;
				// aendern
		// timer initialisierung auslagern ?
				MyTmr1 = OSTmrCreate((INT32U)15,
					(INT32U)15,
					(INT8U)OS_TMR_OPT_PERIODIC,
					(OS_TMR_CALLBACK)MyTmrCallbackFnct1,
					(void*)0,
					(INT8U*)"My Timer #1",
					(INT8U*)&err);
				processError("TimerInit", (INT8U*)&err);

				OSTmrStart((OS_TMR*)MyTmr1,
					(INT8U*)&err);
				processError("TimerStart", (INT8U*)&err);
			}
			OSTimeDlyHMSM(0, 0, 0, 100);
		}

		// Falls eine Wurst bereits erzeugt wird, warten
		OSSemPend(SemBox, 0, &err);
		key = NULL;
		OSTmrStop((OS_TMR*)MyTmr1, OS_TMR_OPT_NONE, NULL,(INT8U*)&err);
		OSMboxPostOpt(MSG_box, (void*)'d', OS_POST_OPT_BROADCAST);
		createWurst();
		OSTimeDlyHMSM(0, 0, 1, 0);
		OSSemPost(SemBox);



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

	 //Fleischer initialisieren
	wuersteInKuehlbox = 0;
	SemFleischer = OSSemCreate(1);
	SemBox = OSSemCreate(0);
	OSTaskCreate(Fleischer,
		(void*)0,
		&FleischerTaskStk[FLEISCHER_TASK_STK_SIZE - 1],
		FLEISCHER_TASK_PRIORITY);

	// Grillmeister initialisieren
	OSTaskCreate(Grillmeister,
		(void*)0,
		&GrillmeisterTaskStk[GRILLMEISTER_TASK_STK_SIZE - 1],
		GRILLMEISTER_TASK_PRIORITY);

	// Physik initialisieren
	OSTaskCreate(Physik,
		(void*)0,
		&PhysikTaskStk[PHYSIK_TASK_STK_SIZE - 1],
		PHYSIK_TASK_PRIORITY);

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