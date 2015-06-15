#pragma once

#include "Thread.h"

class Timer;

class Timers : public Thread
{
	uv_timer_t timer;

	static void notifier(uv_timer_t* handle);

public:
	Timers();
	virtual ~Timers();

	Timer* create_timer(int timeout, int repeat);
};

