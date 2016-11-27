#include "thread.h"
#include "../general.h"



void lock(spinlock *lock) {
	while (atomic_exchange_explicit(&lock->locked, LOCKED, memory_order_acquire) == LOCKED);
}

void unlock(spinlock *lock) {
	atomic_store_explicit(&lock->locked, UNLOCKED, memory_order_release);
}