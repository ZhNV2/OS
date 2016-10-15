#ifndef __BUDDY_NODE_H__
#define __BUDDY_NODE_H__
#include "general.h"

typedef struct node {
	uint32_t begin;
	struct node *next, *prev;
} Node;

Node nullNode();

#endif // __BUDDY_NODE_H__