#include "monitor.h"
#include <iostream>

PhiMonitor::PhiMonitor(size_t num) : state(num, THINKING)
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
	std::cout << "philosopher " << phi_id << " is hungry" << std::endl;
	while ((state[right(phi_id)] == EATING || state[left(phi_id)] == EATING)){
		std::cout << "philosopher " << phi_id << " is waiting for " << right(phi_id) << " "<<left(phi_id) << std::endl;
		state[phi_id] = HUNGRY;
		cond.wait(lock);
	}
	std::cout << "philosopher " << phi_id << " is eating" << std::endl;
	state[phi_id] = EATING;
}

void PhiMonitor::putdown(size_t phi_id)
{
	/* TODO */
	std::unique_lock<std::mutex> lock(mutex);
	
	state[phi_id] = THINKING;
	
	cond.notify_all();
	std::cout << "philosopher " << phi_id << " is thinking" << std::endl;
	lock.unlock();
	

}


