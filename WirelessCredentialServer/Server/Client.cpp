#include "Client.h"

#include "tools/easylogging++.h"

#include <iostream>

Client::Client()
{
}

Client::~Client()
{
}

void Client::on_hello(packets::Hello& packet)
{
	packets::Hello response;
	response.set_name(packet.name());

	this->send_packet(response);
}

void Client::on_test(packets::Test& packet)
{
	LOG(INFO) << "WOW TEST" << packet.hello();
}

void Client::handle_connection()
{
	LOG(INFO) << "CONNECTED";
}