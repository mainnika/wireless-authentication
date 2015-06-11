#pragma once

#include "Client.h"
#include "Thread.h"

class ThreadedClient :
	public Client, public Thread
{
public:
	ThreadedClient();
	virtual ~ThreadedClient();
	
	virtual void on_run();
	virtual void on_stop();

};

