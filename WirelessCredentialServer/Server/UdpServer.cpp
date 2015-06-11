#include "UdpServer.h"
#include "Server.h"

#include "tools/easylogging++.h"
#include "tools/alloc_buffer.h"
#include "tools/array_deleter.h"

#include <stdlib.h>
#include <stdio.h>

constexpr int DEFAULT_BACKLOG = 128;

UdpServer::UdpServer(int port, std::string host = "0.0.0.0") :
host(host), port(port)
{
	this->server.data = this;
}

UdpServer::~UdpServer()
{
}

void UdpServer::receive_new_message(uv_udp_t* server, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)
{
	UdpServer *self = static_cast<UdpServer*> (server->data);
	std::unique_ptr<char, array_deleter<char>> base_guard(buf->base);

	std::string message(buf->base);

	LOG(INFO) << "Receive UDP datagram '" << message << "', length: " << nread;
}

void UdpServer::on_run()
{
	struct sockaddr_in addr;
	uv_ip4_addr(this->host.c_str(), this->port, &addr);
	uv_udp_init(&this->get_loop(), &this->server);
	uv_udp_bind(&this->server, (const struct sockaddr*) &addr, 0);
	uv_udp_recv_start(&this->server, &alloc_buffer, &UdpServer::receive_new_message);

	LOG(INFO) << "UdpServer " << static_cast<void*> (this) << " started";
}

void UdpServer::on_stop()
{
	uv_close(reinterpret_cast<uv_handle_t*> (&this->server), nullptr);

	LOG(INFO) << "UdpServer " << static_cast<void*> (this) << " stopped";
}
