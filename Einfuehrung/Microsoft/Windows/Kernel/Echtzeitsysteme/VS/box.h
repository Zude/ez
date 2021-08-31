#ifndef BOX_H
#define BOX_H

#include <stdio.h>
#include <stdlib.h>
#include "box.h"

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  "pc.h"
#include  "app_cfg.h"

typedef struct {
	int seite1;
	int seite2;
	int seite3;
	int seite4;
	int aktuelleSeite;
} Wurst;

typedef struct {
	Wurst wurst;
	Node* next;
} Node;

Node* createWurst(Node* list, OS_MEM* parition);

void DeleteWurst(Node* prevNode, Node* toBeRemoved, OS_MEM* parition);

#endif /* BOX_H */
