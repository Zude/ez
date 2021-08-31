#include <stdio.h>
#include <stdlib.h>
#include "box.h"

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "app_cfg.h"



Node* createWurst(Node* list, OS_MEM* parition) {

	INT8U err;

	Node* newNode = OSMemGet(parition, &err);;
	newNode = 1;
	newNode->wurst.seite1 = 0;
	newNode->wurst.seite2 = 0;
	newNode->wurst.seite3 = 0;
	newNode->wurst.seite4 = 0;

	newNode->next = list->next;
	list->next = newNode;
	return newNode;
}

void DeleteWurst(Node* prevNode, Node* toBeRemoved, OS_MEM* parition)
{
	if (prevNode)
		prevNode->next = toBeRemoved->next;

	OSMemPut(parition, toBeRemoved);
}
