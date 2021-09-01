#include  "Sausage.h"

volatile SausageNode coolingBox;

SausageNode createWurst(OS_MEM* partitionPtr) {

	INT8U err;

	SausageNode newNode = OSMemGet(partitionPtr, &err);;

	newNode->next = coolingBox;
	newNode->value.sideOne = 0;
	newNode->value.sideTwo = 0;
	newNode->value.sideThree = 0;
	newNode->value.sideFour = 0;
	newNode->value.currentSide = 1;

	coolingBox = newNode;

	printf("Fleiwscher: Erzeuge neue Wurst!\n");
	printf("Kuehlbox: Es sind %d Wuerste in der Box!\n", getCount(coolingBox));

	return coolingBox;
}