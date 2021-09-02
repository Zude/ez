#include  "Sausage.h"

OS_MEM* PartitionPtr;
INT8U	Partition[100][32];

volatile SausageNode coolingBox;
volatile SausageNode grill;

volatile int sausagesCountBox = 0;
volatile int sausagesCountGrill = 0;

OS_EVENT* SemFleischer;
OS_EVENT* SemBox;
OS_EVENT* SemGrill;

volatile int currentTemp = 150;
int tempFac = 100;
int tempSetFac = 10;

boolean entzuendet = 0;

SausageNode createWurst() {

	INT8U err;

	SausageNode newNode = OSMemGet(PartitionPtr, &err);;

	newNode->next = coolingBox;
	newNode->value.sideOne = 80;
	newNode->value.sideTwo = 80;
	newNode->value.sideThree = 80;
	newNode->value.sideFour = 80;
	newNode->value.currentSide = 1;

	coolingBox = newNode;

	sausagesCountBox++;
	printf("Fleiwscher: Erzeuge neue Wurst!\n");
	printf("Kuehlbox: Es sind %d Wuerste in der Box!\n", getCount(coolingBox));

	return coolingBox;
}

void transferSausage()
{
	if (coolingBox->next != NULL)
	{
		SausageNode transfer = coolingBox->next;

		coolingBox->next = grill;
		grill = coolingBox;

		coolingBox = transfer;
		sausagesCountBox--;
		sausagesCountGrill++;
	}
	else {
		coolingBox->next = grill;
		grill = coolingBox;
		coolingBox = NULL;
		sausagesCountBox--;
		sausagesCountGrill++;
	}
}

int getCount(SausageNode head)
{
	int count = 0; // Initialize count
	SausageNode current = head; // Initialize current
	while (current != NULL)
	{
		count++;
		current = current->next;
	}
	return count;
}

SausageNode GetWurstAtIndex(int index) {

	if (index < 0)
	{
		return NULL;
	}
	SausageNode current = grill;

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

void deleteWurst(SausageNode prevNode, SausageNode toBeRemoved)
{
	if (prevNode)
		prevNode->next = toBeRemoved->next;

	sausagesCountGrill--;

	OSMemPut(PartitionPtr, toBeRemoved);
}

void deleteAllOnGrill() {
	 SausageNode current = grill;
	 SausageNode next;

	while (current != NULL)
	{
		next = current->next;
		OSMemPut(PartitionPtr, current);
		current = next;
		sausagesCountGrill--;
	}

	grill = NULL;
}
