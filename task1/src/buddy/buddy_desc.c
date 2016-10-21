#include "buddy_desc.h"

Descriptor nullDescriptor() {
	Descriptor tmp;
	tmp.node = NULL;
	tmp.ord = 0;
	tmp.isFree = 1;
	return tmp;
}
