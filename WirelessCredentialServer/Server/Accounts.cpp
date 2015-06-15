#include "Accounts.h"

Accounts::Accounts()
{
}

Accounts::~Accounts()
{
}

void Accounts::add_unregistered(std::string key)
{
	this->unregistered[key] = key;

//	DEBUG ONLY
	this->register_key_for_account(key, "mainnika");
}

void Accounts::register_key_for_account(std::string key, std::string account)
{
	this->unregistered.erase(key);
	this->accounts[key] = account;
}

std::string Accounts::get_by_key(std::string key)
{
	auto finded = this->accounts.find(key);
	if (finded == this->accounts.end())
	{
		this->add_unregistered(key);
		throw std::runtime_error("Account not found");
	}

	return finded->second;
}
