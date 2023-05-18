#include <iostream>
#include <iomanip>
#include <list>
#include <thread>
#include <mutex>
const int N = 3;
const int M = 10;
std::mutex mutex;
int count = 0;

void print(unsigned int id)
{
	for (int i = 0; i < N; ++i )
	{
		//mutex.lock();
		std::unique_lock<std::mutex> lock(mutex);
		int old_value = count;
		int new_value = count + 1;
		count = new_value;
		//mutex.unlock(); avrei potuto farlo anche qui
		std::cout << "<child " << std::setw(2) << id << ">: value stored="
			<< old_value << ", new value=" << new_value << std::endl;
		
		//mutex.unlock();
	}
}

int main()
{
	std::list<std::thread> childs;

	std::cout << "<main>: Starting child threads..." << std::endl;
	for (int id = 0; id < M; ++id)
		childs.emplace_back(print, id);

	for (auto it = childs.begin(); it != childs.end(); ++it)
		it -> join();

	std::cout << "<main>: value stored=" << count << "." << std::endl;

	if ( count == N * M )
		std::cout << "<main>:   Ok, good sons!!!" << std::endl;
	else
		std::cout << "<main>:   Uh, what's wrong?" << std::endl;

	return 0;
}

