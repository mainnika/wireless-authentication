/*
 * File:   Server.cpp
 * Author: mainn_000
 *
 * Created on May 24, 2015, 1:48 AM
 */

#include "Server.h"

#include "Interface.h"
#include "Interfaces.h"

#include "free_deleter.h"

#include <ev++.h>
#include <algorithm>
#include <glog/logging.h>

Server::Server(std::string cards)
{
	this->init_cards(cards);

	LOG(INFO) << "Server started";
}

Server::~Server()
{
	LOG(INFO) << "Server destroyed";
}

void Server::init_cards(std::string cards)
{
	std::vector<Interface*> ifaces;
	std::unique_ptr<char, free_delete<char>> buffer(strdup(cards.c_str()));
	char *str = buffer.get();
	char *raw;

	while ((raw = strsep(&str, ",")) != NULL)
	{
		std::string iface(raw);

		auto iter = std::find_if(this->cards.begin(), this->cards.end(), Interface::compare_with(iface));

		if (iter != this->cards.end())
		{
			LOG(WARNING) << "Duplicated initialize for interface " << iface;
			continue;
		}

		wif_ptr wi(wi_open(iface.c_str()));

		if (wi == nullptr)
		{
			LOG(WARNING) << "Unable to open interface " << iface;
			continue;
		}

		ifaces.push_back(new Interface(std::move(wi), iface));
	}

	for (auto *iface : ifaces)
		this->ifaces.add(std::unique_ptr<Interface>(iface));
}

void Server::start()
{
	this->ifaces.start();
}

void Server::stop()
{
	this->ifaces.stop();
	this->ifaces.join();
}

