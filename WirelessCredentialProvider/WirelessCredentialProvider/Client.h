#pragma once

#include "Protocol.h"
#include "Timers.h"
#include "Timer.h"

class CSampleProvider;
class CCommandWindow;

class Client : public Protocol
{
private:
	Timers timers;

	CSampleProvider *_pProvider;
	CCommandWindow *_pWindow;
	Timer *reauth_timer;

	bool logged;
	std::wstring id;

	static void on_auth_timer(void *arg);

public:
	Client();
	virtual ~Client();

	bool is_logged_in() const;

	void set_provider(CSampleProvider *provider);
	void set_window(CCommandWindow *window);

	void on_hello(packets::Hello &packet);
	void on_auth(packets::AuthorizeStatus &packet);
	void on_accounts(packets::AccountResponse &packet);

	void send_auth();

	virtual void handle_connection();

};