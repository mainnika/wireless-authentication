#include "Protocol.h"
#include "Client.h"

#include <easylogging++.h>
#include <free_deleter.h>

#define HANDLE_PACKET(PACKET_CLASS, RECEIVER_ID, RECEIVER_HANDLER) \
	{ \
		PACKET_CLASS packet; \
		int id = packet.id(); \
		if (packet.ParseFromArray(data, len) && packet.id() == id) \
		{ \
			(static_cast<RECEIVER_ID*>(this)->*(&RECEIVER_ID::RECEIVER_HANDLER))(packet); \
			return; \
		} \
	}

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

void Protocol::handle_packet(char* data, size_t len)
{
	std::unique_ptr<char, free_delete<char>> data_guard(data);

	HANDLE_PACKET(packets::Hello, Client, on_hello);
	HANDLE_PACKET(packets::Test, Client, on_test);
}