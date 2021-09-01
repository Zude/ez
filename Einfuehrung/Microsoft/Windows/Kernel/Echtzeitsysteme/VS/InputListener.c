#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"

OS_EVENT* msgQueueButcher;
void* messageStorageButcher[100];
OS_STK ListenerTaskStk[100];

void InputListener(void* p_arg)  {

	INT8U err;
	INT16S input;

	while (1) {
		while (!PC_GetKey(&input)) {
			OSTimeDlyHMSM(0, 0, 0, 100);
		}

		switch (input)
		{
		case 'w':
			OSQPost(msgQueueButcher, (void*)'w');
			break;
		default:
			break;
		}

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}