#pragma once

#include "NoCopy.h"

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <uv.h>

class TcpClient : private NoCopy
{
	friend class Clients;

private:

	uv_tcp_t *socket;
	uv_timer_t *timer_reconnect;

	char* data;
	int data_needed;
	int data_readed;
	int data_allocated;

	static void on_connect(uv_connect_t *connection, int status);
	static void on_reconnect(uv_timer_t* handle);
	static void on_segment(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
	static void on_write(uv_write_t *req, int status);

protected:

	virtual void handle_packet(char* data, size_t len) = 0;
	virtual void handle_connection()
	{};

	void send_data(char* data, size_t len);

public:
	static void connect(uv_loop_t *loop, TcpClient *client, std::string host, int port);

	TcpClient();
	virtual ~TcpClient();
};
