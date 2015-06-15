#include "Client.h"

#include "CSampleProvider.h"
#include "CommandWindow.h"

#include <SettingsHelper.h>
#include <easylogging++.h>
#include <iostream>
#include <locale>
#include <codecvt>


Client::Client() :
logged(false)
{
	SettingsHelper helper;
	SettingsHelper::Settings settings;

	try{

		try{
			settings = helper.load();
		}
		catch (std::exception load_error)
		{
			LOG(WARNING) << load_error.what();
			settings = helper.init();
		}

	}
	catch (std::exception init_error)
	{
		LOG(ERROR) << init_error.what();
		std::abort();
	}

	this->id = settings.id;

	LOG(INFO) << "Init application with id " << this->id;
}

Client::~Client()
{
}

bool Client::is_logged_in() const
{
	return this->logged;
}

void Client::set_provider(CSampleProvider *provider)
{
	this->_pProvider = provider;
}

void Client::set_window(CCommandWindow *window)
{
	this->_pWindow = window;
}

void Client::on_hello(packets::Hello& packet)
{
	LOG(INFO) << "Hello received, requesting auth";
	this->_pWindow->SetWindowTitle(L"TRY AUTH");

	std::string key = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(this->id);

	packets::AuthorizeTry request;
	request.set_key(key);

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
		this->_pWindow->SetWindowTitle(L"FAIL, BLOCKED");
		return;
	}

	case packets::AuthorizeStatus_Status::AuthorizeStatus_Status_ALLOWED:
	{
		LOG(INFO) << "Server allowed connection, requesting account";
		this->_pWindow->SetWindowTitle(L"OK, REQUESTING");
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
		this->_pWindow->SetWindowTitle(L"FAIL, NO ACCOUNT");
		return;
	}
	
	auto account = packet.account();

	LOG(INFO) << "There's an account: " << account;

	this->logged = true;
	this->_pWindow->SetWindowTitle(L"OK");
	this->_pProvider->OnConnectStatusChanged();
}

void Client::handle_connection()
{
	LOG(INFO) << "Connected to server, hello sending";
	this->_pWindow->SetWindowTitle(L"Connected");

	packets::Hello hello;
	this->send_packet(hello);
}