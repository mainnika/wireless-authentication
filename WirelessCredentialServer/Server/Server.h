#pragma once

#include "Clients.h"
#include "TcpServer.h"
#include "UdpServer.h"

#include "Config.h"

#include <tbb/concurrent_queue.h>
#include <memory>
#include <vector>

class Server : private NoCopy
{
private:
	static Server *instance;

public:
	TcpServer tcp;
	UdpServer udp;
	Clients clients;

	Server(Config &config);
	virtual ~Server();

	static Server* get();

	void start();
	void stop();
};
