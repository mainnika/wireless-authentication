#pragma once

#include "Protocol.h"

class Client : public Protocol
{
public:
	Client();
	virtual ~Client();

	void on_hello(packets::Hello &packet);
	void on_test(packets::Test &packet);
};