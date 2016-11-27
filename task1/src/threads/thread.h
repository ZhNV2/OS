#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdatomic.h>

#define LOCKED 1
#define UNLOCKED 0

typedef struct spinlock {
	atomic_int locked;
} spinlock;

void lock(spinlock *lock);
void unlock(spinlock *lock);

#endif // __THREAD_H__