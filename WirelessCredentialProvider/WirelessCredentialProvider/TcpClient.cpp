#include "TcpClient.h"
#include "Client.h"

#include <easylogging++.h>
#include <array_deleter.h>
#include <alloc_buffer.h>

#include <cstdlib>
#include <memory>

static const int BUFFER_START_SIZE = 1024;

TcpClient::TcpClient() :
	socket(nullptr), timer_reconnect(nullptr), data_needed(0), data_readed(0), data_allocated(BUFFER_START_SIZE)
{
	this->data = static_cast<char*>(malloc(this->data_allocated));
}

TcpClient::~TcpClient()
{
	free(this->data);
}

void TcpClient::connect(uv_loop_t *loop, TcpClient *client, std::string host, int port)
{
	LOG(INFO) << "Trying to connect to " << host;

	struct sockaddr_in dest;
	uv_connect_t* connect = new uv_connect_t;
	uv_tcp_t* socket = new uv_tcp_t;

	uv_tcp_init(loop, socket);
	uv_ip4_addr(host.c_str(), port, &dest);

	socket->data = client;

	uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, &TcpClient::on_connect);
}

void TcpClient::on_connect(uv_connect_t *connection, int status)
{
	TcpClient *client = static_cast<TcpClient*>(connection->handle->data);

	if (client->timer_reconnect != nullptr)
	{
		uv_timer_stop(client->timer_reconnect);
		client->timer_reconnect = nullptr;
	}

	if (status != 0)
	{
		LOG(INFO) << "Failed to connect";
		client->timer_reconnect = new uv_timer_t;
		client->timer_reconnect->data = client;
		uv_timer_init(connection->handle->loop, client->timer_reconnect);
		uv_timer_start(client->timer_reconnect, &TcpClient::on_reconnect, 1000, 0);

		return;
	}

	client->socket = reinterpret_cast<uv_tcp_t*>(connection->handle);

	uv_read_start(connection->handle, &alloc_buffer, &TcpClient::on_segment);

	client->handle_connection();
}

void TcpClient::on_reconnect(uv_timer_t* handle)
{
	TcpClient *client = static_cast<TcpClient*>(handle->data);
	TcpClient::connect(handle->loop, client, "192.168.56.101", 12343);
}


void TcpClient::on_segment(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	TcpClient *client = static_cast<TcpClient*>(stream->data);
	std::unique_ptr<char, array_deleter<char>> base_guard(buf->base);

	if (nread < 0)
	{
		return;
	}

	client->data_readed += nread;
	if (client->data_readed >= client->data_allocated)
	{
		client->data_allocated *= 2;

		char* reallocated = static_cast<char*>(realloc(client->data, client->data_allocated));
		if (reallocated == NULL)
			return;

		client->data = reallocated;
	}

	memcpy(client->data + client->data_readed - nread, buf->base, nread);

	if (client->data_needed == 0)
	{
		ssize_t size_len = sizeof(uint32_t);
		if (client->data_readed < size_len)
			return;

		memcpy(&client->data_needed, client->data, size_len);
		client->data_readed -= size_len;

		memmove(client->data, client->data + size_len, client->data_readed);
	}

	if (client->data_readed < client->data_needed)
		return;

	char* packet = static_cast<char*>(malloc(client->data_needed));
	ssize_t packet_len = client->data_needed;

	client->data_readed -= client->data_needed;

	memcpy(packet, client->data, client->data_needed);
	memmove(client->data, client->data + client->data_needed, client->data_readed);

	client->data_needed = 0;

	client->handle_packet(packet, packet_len);
}

void TcpClient::on_write(uv_write_t *req, int status)
{
	std::unique_ptr<uv_write_t> req_guard(req);
}

void TcpClient::send_data(char* data, size_t len)
{
	uv_write_t *req = new uv_write_t;
	uv_buf_t wrbuf = uv_buf_init(data, len);

	uv_write(req, reinterpret_cast<uv_stream_t*>(this->socket), &wrbuf, 1, &TcpClient::on_write);
}