#include "Timers.h"

#include "Timer.h"

Timers::Timers()
{
	this->start();

	uv_timer_init(&this->get_loop(), &this->timer);
	this->timer.data = this;

	uv_timer_start(&this->timer, &Timers::notifier, 1000, 1000);
	this->notify();
}

void Timers::notifier(uv_timer_t* handle)
{
	Timers *timer = static_cast<Timers*>(handle->data);
	timer->notify();
}

Timers::~Timers()
{
	uv_timer_stop(&this->timer);
	this->stop();
}

Timer* Timers::create_timer(int timeout, int repeat)
{
	return new Timer(&this->get_loop(), timeout, repeat);
}
