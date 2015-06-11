#pragma once

#include "NoCopy.h"
#include "Thread.h"

#include <atomic>
#include <memory>
#include <string>
#include <uv.h>
#include <tbb/concurrent_queue.h>

class UdpServer : public Thread, private NoCopy
{
private:
	std::atomic<bool> opened;

	uv_udp_t server;

	std::string host;
	int port;

	static void receive_new_message(uv_udp_t *server, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags);

	virtual void on_run();
	virtual void on_stop();

public:
	UdpServer(int port, std::string host);
	virtual ~UdpServer();

};

