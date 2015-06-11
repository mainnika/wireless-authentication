#pragma once

#include "Protocol.h"

class Client : public Protocol
{

private:
	bool logged;
	std::string account;

	bool is_logged() const;

public:
	Client();
	virtual ~Client();

	void on_hello(packets::Hello &packet);
	void on_auth(packets::AuthorizeTry &packet);
	void on_accounts(packets::AccountRequest &packet);

	virtual void handle_connection();

};