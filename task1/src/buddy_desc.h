#ifndef __BUDDY_DESC_H__
#define __BUDDY_DESC_H__

#include "buddy_node.h"
#include "general.h"

typedef struct descriptor {
	Node *node;
	int ord;	
	int isFree;
} Descriptor;


Descriptor nullDescriptor();
#endif // __BUDDY_DESC_H__