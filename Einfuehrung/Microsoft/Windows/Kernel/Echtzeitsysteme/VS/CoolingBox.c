#include  "Sausage.h"

volatile WurstNode coolingBox;

WurstNode createWurst(OS_MEM* partitionPtr) {

	INT8U err;

	WurstNode newNode = OSMemGet(partitionPtr, &err);;

	newNode->next = coolingBox;
	newNode->value.seite1 = 0;
	newNode->value.seite2 = 0;
	newNode->value.seite3 = 0;
	newNode->value.seite4 = 0;
	newNode->value.aktuelleSeite = 1;

	coolingBox = newNode;

	printf("Fleiwscher: Erzeuge neue Wurst!\n");
	printf("Kuehlbox: Es sind %d Wuerste in der Box!\n", getCount(coolingBox));

	return coolingBox;
}