#include  "Sausage.h"

OS_MEM* PartitionPtr;
INT8U	Partition[100][32];

volatile SausageNode coolingBox;
volatile SausageNode grill;

OS_EVENT* SemFleischer;
OS_EVENT* SemBox;
OS_EVENT* SemGrill;

SausageNode createWurst() {

	INT8U err;

	SausageNode newNode = OSMemGet(PartitionPtr, &err);;

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

void transferWurst()
{
	SausageNode transfer = coolingBox->next;

	coolingBox->next = grill;
	grill = coolingBox;

	coolingBox = transfer;
	printf("Grillmeister: Entnehme Wurst aus Box und plaziere auf Grill!\n");
	printf("Kuehlbox: Es sind %d Wuerste in der Box!\n", getCount(coolingBox));
	printf("Grill: Es sind %d Wuerste auf den Grill!\n", getCount(grill));
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

SausageNode GetWurstAtIndex(int index)
{

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

static void DeleteWurst(SausageNode prevNode, SausageNode toBeRemoved, OS_MEM* parition)
{
	if (prevNode)
		prevNode->next = toBeRemoved->next;

	OSMemPut(parition, toBeRemoved);
}
