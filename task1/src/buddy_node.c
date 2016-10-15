#include "buddy_node.h"
Node nullNode() {
	Node tmp;
	tmp.next = tmp.prev = NULL;
	tmp.begin = 0;
	return tmp;
}