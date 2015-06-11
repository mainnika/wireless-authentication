#include "Accounts.h"

Accounts::accounts_t Accounts::accounts =
{
	{"RNRQgLUGcFkPWpLxFzMsuRRLHf7meDVwaaPnG2zwHs7MpZjBWV", "mainnika"}
};

Accounts::Accounts()
{
}

Accounts::~Accounts()
{
}

std::string Accounts::get_by_key(std::string key)
{
	auto finded = Accounts::accounts.find(key);
	if (finded == Accounts::accounts.end())
	{
		throw std::runtime_error("Account not found");
		return {};
	}

	return finded->second;
}
