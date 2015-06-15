#pragma once

#include "Protocol.h"

class CSampleProvider;
class CCommandWindow;

class Client : public Protocol
{
private:
	CSampleProvider *_pProvider;
	CCommandWindow *_pWindow;

	bool logged;
	std::wstring id;

public:
	Client();
	virtual ~Client();

	bool is_logged_in() const;

	void set_provider(CSampleProvider *provider);
	void set_window(CCommandWindow *window);

	void on_hello(packets::Hello &packet);
	void on_auth(packets::AuthorizeStatus &packet);
	void on_accounts(packets::AccountResponse &packet);

	virtual void handle_connection();

};