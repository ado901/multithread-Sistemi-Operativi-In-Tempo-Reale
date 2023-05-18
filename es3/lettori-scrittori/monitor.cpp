#include "monitor.h"

RWMonitor::RWMonitor() : num_readers(0), num_writers(0), readers_in_queue(0), writers_in_queue(0)
{
}
//STRONG WRITER POSSIBILE STARVATION PER READER
void RWMonitor::rlock()
{
	std::unique_lock<std::mutex> lock(mutex);
	//writer preference
	readers_in_queue++;
	if (preference==Preference::WRITER){
	if (num_writers > 0 || writers_in_queue > 0)
	{
		if (type==Type::STRONG){read_queue.wait(lock);} 
		else queue.wait(lock);
		
	}
	}
	else if (num_writers > 0){
			if (type==Type::STRONG){read_queue.wait(lock);} 
			else queue.wait(lock);
		}
		
	
	readers_in_queue--;
	
	/* TODO */
	num_readers++;
}

void RWMonitor::runlock()
{
	std::unique_lock<std::mutex> lock(mutex);
	num_readers--;
	//writer preference
	if(preference==Preference::WRITER){
		if (num_readers == 0 ){
			if (type==Type::STRONG){
				if (writers_in_queue > 0)
				{
					
					write_queue.notify_one();
					

				}
			}
			else queue.notify_one();
		
		
	}}
	//reader preference
	else{
		if (type==Type::WEAK){
			queue.notify_one();
		}
		else if (num_readers == 0 && writers_in_queue > 0)
		{
			write_queue.notify_one();
		}
	}
	
	/* TODO */
}


void RWMonitor::wlock()
{
	std::unique_lock<std::mutex> lock(mutex);
	writers_in_queue++;
	//writer preference
	if(preference== Preference::WRITER){
		if(num_writers>0 || writers_in_queue>0|| num_readers>0){
		
			if (type==Type::STRONG) write_queue.wait(lock);
			else queue.wait(lock);
	}
	}
	
	//reader preference
	else if(num_readers>0||num_writers>0||readers_in_queue>0||writers_in_queue>0){
		if (type==Type::STRONG){write_queue.wait(lock);} 
		else queue.wait(lock);
		
	}
	writers_in_queue--;
	num_writers++;
	/* TODO */
}

void RWMonitor::wunlock()
{
	std::unique_lock<std::mutex> lock(mutex);
	num_writers--;
	//writer preference
	if (preference==Preference::WRITER){
		if (type==Type::STRONG){
			if (writers_in_queue > 0)
			{
				write_queue.notify_one();
			}
			else if (readers_in_queue > 0)
			{
				read_queue.notify_one();
			}
		}
		else queue.notify_one();
	}
	//reader preference
	else if (preference==Preference::READER){
		if(type==Type::STRONG){
			if (readers_in_queue > 0)
			{
				read_queue.notify_one();
			}
			else if (writers_in_queue > 0)
			{
				write_queue.notify_one();
			}
		}
		else queue.notify_one();
	}
}

