#include "Client.h"

#include <easylogging++.h>
#include <iostream>

Client::Client()
{
}

Client::~Client()
{
}

void Client::on_hello(packets::Hello& packet)
{
	packets::Test response;
	response.set_some(42);
	response.set_hello("OLOLOWWOWOWO IT WORKS");

	this->send_packet(response);
	LOG(INFO) << "HELLO RECEIVED";
}

void Client::on_test(packets::Test& packet)
{
	LOG(INFO) << "WOW TEST" << packet.hello();
}

void Client::handle_connection()
{
	LOG(INFO) << "CONNECTED";

	packets::Hello hello;
	hello.set_name("WAKAWAKAWA");

	this->send_packet(hello);
}