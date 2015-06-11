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
	LOG(INFO) << "Hello received, requesting auth";

	packets::AuthorizeTry request;
	request.set_key("RNRQgLUGcFkPWpLxFzMsuRRLHf7meDVwaaPnG2zwHs7MpZjBWV");

	this->send_packet(request);
}

void Client::on_auth(packets::AuthorizeStatus& packet)
{
	auto status = packet.status();

	switch (status)
	{

	case packets::AuthorizeStatus_Status::AuthorizeStatus_Status_BLOCKED:
	{
		LOG(WARNING) << "Server blocked connection";
		return;
	}

	case packets::AuthorizeStatus_Status::AuthorizeStatus_Status_ALLOWED:
	{
		LOG(INFO) << "Server allowed connection, requesting account";
		break;
	}

	}

	packets::AccountRequest request;
	this->send_packet(request);
}

void Client::on_accounts(packets::AccountResponse &packet)
{
	if (!packet.has_account())
	{
		LOG(INFO) << "This host has no accounts authorized";
		return;
	}
	
	auto account = packet.account();

	LOG(INFO) << "There's an account: " << account;
}

void Client::handle_connection()
{
	LOG(INFO) << "Connected to server, hello sending";

	packets::Hello hello;
	this->send_packet(hello);
}