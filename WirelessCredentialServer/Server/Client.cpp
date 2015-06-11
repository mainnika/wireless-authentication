#include "Client.h"
#include "Server.h"

#include "easylogging++.h"

#include <iostream>

Client::Client() :
logged(false)
{

}

Client::~Client()
{
}

bool Client::is_logged() const
{
	return this->logged;
}

void Client::on_hello(packets::Hello& packet)
{
	packets::Hello response;
	this->send_packet(response);
}

void Client::on_auth(packets::AuthorizeTry& packet)
{
	packets::AuthorizeStatus response;
	auto key = packet.key();

	try{
		this->account = Server::get()->accounts.get_by_key(key);
		this->logged = true;

		LOG(INFO) << "Requested account " << this->account << " by key " << key;
		response.set_status(packets::AuthorizeStatus_Status::AuthorizeStatus_Status_ALLOWED);
	}
	catch (std::runtime_error error)
	{
		LOG(WARNING) << "Key error (" << key << "): " << error.what();
		response.set_status(packets::AuthorizeStatus_Status::AuthorizeStatus_Status_BLOCKED);
	}

	this->send_packet(response);
}

void Client::on_accounts(packets::AccountRequest& packet)
{
	packets::AccountResponse response;

	if (!is_logged())
	{
		LOG(WARNING) << "Client is not logged in";
		this->send_packet(response);
		return;
	}

	response.set_account(this->account);
	this->send_packet(response);
}

void Client::handle_connection()
{
	LOG(INFO) << "CONNECTED";
}