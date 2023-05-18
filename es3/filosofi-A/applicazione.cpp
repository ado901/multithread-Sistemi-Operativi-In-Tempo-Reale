#include <iostream>
#include <thread>
#include <list>
#include <chrono>
#include <sstream>

#include "monitor.h"

void phi_think()
{
	// Placeholder for "real world" code
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void phi_eat()
{
	// Placeholder for "real world" code
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

void philosopher_algo(unsigned int id, PhiMonitor & mon)
{
	std::ostringstream think;
	std::ostringstream hungry;
	std::ostringstream eat;

	think << "philosopher " << id << " is thinking" << std::endl;
	hungry << "philosopher " << id << " is hungry" << std::endl;
	eat << "philosopher " << id << " is eating" << std::endl;

	for (unsigned int i = 0; i < 10; ++i)
	{
		

		phi_think();

		

		mon.pickup(id);

		phi_eat();

		mon.putdown(id);
	}
}

int main()
{
	PhiMonitor monitor(3);
	std::list<std::thread> philosophers;

	for(unsigned int id = 0; id < monitor.size(); ++id)
		philosophers.emplace_back(philosopher_algo, id, std::ref(monitor));

	for (auto & th : philosophers)
		th.join();

	return 0;
}
