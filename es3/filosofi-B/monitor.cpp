#include "monitor.h"

PhiMonitor::PhiMonitor(size_t num) : cond(num), state(num, THINKING)
{
}

inline size_t PhiMonitor::left(size_t id) const
{
	return (id + size() - 1) % size();
}

inline size_t PhiMonitor::right(size_t id) const
{
	return (id + 1) % size();
}

void PhiMonitor::pickup(size_t phi_id)
{
	std::unique_lock<std::mutex> lock(mutex);
	if ((state[right(phi_id)] == EATING || state[left(phi_id)] == EATING)){
		state[phi_id] = HUNGRY;
		cond[phi_id].wait(lock);
	}
	state[phi_id] = EATING;
	/* TODO */
}

void PhiMonitor::putdown(size_t phi_id)
{
	std::unique_lock<std::mutex> lock(mutex);
	state[phi_id] = THINKING;
	if (state[left(phi_id)] == HUNGRY && state[left(left(phi_id))] != EATING)
		cond[left(phi_id)].notify_one();
	if (state[right(phi_id)] == HUNGRY && state[right(right(phi_id))] != EATING)
		cond[right(phi_id)].notify_one();
	/* TODO */
}


