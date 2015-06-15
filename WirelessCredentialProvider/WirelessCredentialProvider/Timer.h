#pragma once

#include <uv.h>
#include <atomic>
#include <functional>

class Timer
{
public:
	typedef std::function<void(void* arg)> cb_t;

private:
	uv_timer_t timer;
	std::atomic<bool> started;

	cb_t cb;
	void* arg;

	int timeout;
	int repeat;

	static void on_timer_internal(uv_timer_t* handle);
	
public:
	Timer(uv_loop_t *loop, int timeout, int repeat = 0);
	virtual ~Timer();

	void set_cb(cb_t cb, void* arg);
	void start();
	void stop();
};

