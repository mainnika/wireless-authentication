/*
 * File:   Server.cpp
 * Author: mainn_000
 *
 * Created on May 24, 2015, 1:48 AM
 */

#include "Server.h"

#include "Interface.h"

#include "free_deleter.h"

#include <ev++.h>
#include <algorithm>
#include <glog/logging.h>

Server::Server(std::string cards)
{
	LOG(INFO) << "Using libev " << ev::version_major() << "." << ev::version_minor();
	LOG_ASSERT(ev::recommended_backends() & EVBACKEND_EPOLL);

	this->init_cards(cards);

	LOG_ASSERT(!this->cards.empty());
	LOG(INFO) << "Server started";
}

Server::~Server()
{
	LOG(INFO) << "Server destroyed";
}

void Server::init_cards(std::string cards)
{
	char *buffer = strdup(cards.c_str());
	char *copy_of_buffer_ptr = buffer;
	char *raw;

	while ((raw = strsep(&buffer, ",")) != NULL)
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

		this->cards.emplace_back(new Interface(std::move(wi), iface));
	}

	free(copy_of_buffer_ptr);
}