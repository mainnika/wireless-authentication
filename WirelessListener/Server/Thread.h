#pragma once

#include <pthread.h>
#include <memory>

namespace ev
{
	class dynamic_loop;
	class async;
	class timer;
}

class ev_loop;

class Thread
{
	friend void* thread_worker(void *thread_ptr);

public:
	Thread();
	virtual ~Thread();

	struct ev_loop* get_loop() const;

	void stop();
	void join();
	void start();

protected:
	virtual void notify();
	virtual void on_notify();
	virtual void on_start();
	virtual void on_stop();

private:
	std::unique_ptr<ev::dynamic_loop> loop;
	std::unique_ptr<ev::async> notifier;

	pthread_t thread_id;

	bool stopped;

	void notify_handler(ev::async &w, int revents);

	void init();
};