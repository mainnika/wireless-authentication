#pragma once

#include "TcpClient.h"

#include "packets/impl/packets.pb.h"

#include <google/protobuf/message.h>

class Protocol : public TcpClient
{
public:

private:
	virtual void handle_packet(char* data, size_t len);

public:
	Protocol();
	virtual ~Protocol();

	template<class Packet>
	void send_packet(Packet &message)
	{
		message.set_id(message.id());

		int offset = sizeof (int);
		int size = message.ByteSize();
		char *data = new char[size + offset];

		memcpy(data, &size, offset);
		message.SerializeToArray(data + offset, size);

		this->send_data(data, size + offset);
	}
};