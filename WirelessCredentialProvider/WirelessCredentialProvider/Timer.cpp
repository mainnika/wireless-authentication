#include "Timer.h"

#include <easylogging++.h>

Timer::Timer(uv_loop_t *loop, int timeout, int repeat) :
	started(false), arg(nullptr), timeout(timeout), repeat(repeat)
{
	uv_timer_init(loop, &this->timer);
	this->timer.data = this;
}

void Timer::on_timer_internal(uv_timer_t* handle)
{
	Timer* timer = static_cast<Timer*>(handle->data);

	LOG(INFO) << "Timer " << static_cast<void*>(timer) << " tick";

	if (timer->cb)
	{
		timer->cb(timer->arg);
	}
}

void Timer::start()
{
	if (this->started.exchange(true))
	{
		LOG(WARNING) << "Timer " << static_cast<void*>(this) << " already started";
		return;
	}

	LOG(INFO) << "Timer " << static_cast<void*>(this) << " started";

	uv_timer_start(&this->timer, &Timer::on_timer_internal, this->timeout, this->repeat);
}

void Timer::set_cb(cb_t cb, void* arg)
{
	this->cb = cb;
	this->arg = arg;
}

void Timer::stop()
{
	if (!this->started.exchange(false))
	{
		LOG(WARNING) << "Timer " << static_cast<void*>(this) << " already stopperd";
		return;
	}

	LOG(INFO) << "Timer " << static_cast<void*>(this) << " stopped";

	uv_timer_stop(&this->timer);
}

Timer::~Timer()
{
	this->stop();
}
