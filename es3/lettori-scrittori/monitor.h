#ifndef MONITOR_H
#define MONITOR_H

#include <mutex>
#include <condition_variable>
enum class Preference {READER, WRITER};
enum class Type {STRONG, WEAK};
const Type type = Type::WEAK;
const Preference preference = Preference::WRITER;
class RWMonitor
{
	public:
		RWMonitor();

		void rlock();
		void runlock();

		void wlock();
		void wunlock();
	
	private:
		unsigned int num_readers;  // 0,1,2,...
		unsigned int num_writers;  // 0,1

		unsigned int readers_in_queue;
		unsigned int writers_in_queue;

		std::mutex mutex;

		
		std::condition_variable read_queue;
		std::condition_variable write_queue;
		
		
		std::condition_variable queue;
};

#endif
