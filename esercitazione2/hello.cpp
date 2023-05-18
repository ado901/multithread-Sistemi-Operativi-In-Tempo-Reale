#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <random>
#include "rt/priority.h"
#include "barrier.h"
#include "rt/affinity.h"

const std::string hello("Hello World!");

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(10, 50);

void print_char(size_t i, barrier & b)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
	b.wait();
	std::cout << hello[i];
	std::cout.flush();
}

int main()
{
	barrier b(hello.size());
	std::list<std::thread> threads;
	for(size_t id = 0; id < hello.size(); ++id)
	{
		std::thread th(print_char, id, std::ref(b));
		rt::set_priority(th, rt::priority::rt_max-id);
		rt::set_affinity(th, 1);
		std::cout << "\nThread " << id << " priority: " << rt::get_priority(th) << std::endl;
		std::cout << "Thread " << id << " affinity: " << rt::get_affinity(th) << std::endl;

		threads.push_back(std::move(th));
	}

	for (auto & th : threads)
		th.join();

	std::cout << std::endl;

	return 0;
}

