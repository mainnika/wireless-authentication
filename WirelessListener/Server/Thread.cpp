#include "Thread.h"

#include <glog/logging.h>
#include <ev++.h>

void* thread_worker(void *thread_ptr)
{
	Thread *thread = static_cast<Thread*>(thread_ptr);
	thread->init();

	return nullptr;
}

Thread::Thread():
	loop(new ev::dynamic_loop(ev::AUTO)), notifier(new ev::async(*this->loop)),
	thread_id(0), stopped(true)
{}

Thread::~Thread()
{}

void Thread::start()
{
	LOG(INFO) << "Thread " << (void*)this << " starting";
	pthread_create(&this->thread_id, nullptr, thread_worker, this);
}

void Thread::init()
{
	this->notifier->set<Thread, &Thread::notify_handler>(this);
	this->notifier->start();
	this->stopped = false;

	LOG(INFO) << "Thread " << (void*)this << " started";

	this->on_start();
	this->loop->run(0);
	this->on_stop();

	LOG(INFO) << "Thread " << (void*)this << " stopped";
}

struct ev_loop* Thread::get_loop() const
{
	return *this->loop;
}

void Thread::stop()
{
	if (this->stopped)
		return;

	LOG(INFO) << "Thread " << (void*)this << " stopping";

	this->stopped = true;
	this->notifier->send();

	this->join();

	this->thread_id = 0;
}

void Thread::join()
{
	if (this->thread_id == 0)
		return;

	pthread_join(this->thread_id, nullptr);
}

void Thread::notify()
{
	if (this->stopped)
		return;

	this->notifier->send();
}

void Thread::on_notify()
{}

void Thread::on_start()
{}

void Thread::on_stop()
{}

void Thread::notify_handler(ev::async &, int)
{
	if (this->stopped)
	{
		this->notifier->stop();
		this->loop->break_loop(ev::ALL);
		return;
	}

	LOG(INFO) << "Thread " << (void*)this << " notified";

	this->on_notify();
}