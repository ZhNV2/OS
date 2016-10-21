#include "buddy_node.h"
Node nullNode() {
	Node tmp;
	tmp.begin = 0;
	tmp.next = NULL;
	tmp.prev = NULL;
	return tmp;
}