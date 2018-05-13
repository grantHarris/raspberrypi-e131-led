#ifndef __STOPPABLE_H__
#define __STOPPABLE_H__
#include <iostream>
#include <assert.h>
#include <chrono>
#include <future>


class Stoppable
{
	std::promise<void> exit_signal;
	std::future<void> future_obj;
public:
	Stoppable() : future_obj(exit_signal.get_future()){}
	Stoppable(Stoppable && obj) : exit_signal(std::move(obj.exit_signal)), future_obj(std::move(obj.future_obj)){}
	Stoppable & operator=(Stoppable && obj){
		exit_signal = std::move(obj.exit_signal);
		future_obj = std::move(obj.future_obj);
		return *this;
	}

	bool stop_requested(){
		if (future_obj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
			return false;
		return true;
	}

	void stop(){
		exit_signal.set_value();
	}
};
 #endif /* __STOPPABLE_H__ */
