#pragma once

#include "Protocol.h"

class Client : public Protocol
{
public:
	Client();
	virtual ~Client();

	void on_hello(packets::Hello &packet);
	void on_auth(packets::AuthorizeStatus &packet);
	void on_accounts(packets::AccountResponse &packet);

	virtual void handle_connection();

};