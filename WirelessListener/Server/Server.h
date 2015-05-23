/*
 * File:   Server.h
 * Author: mainn_000
 *
 * Created on May 24, 2015, 1:48 AM
 */

#include <memory>
#include <vector>

#include "wif_ptr.h"

extern "C"
{
#include "core.h"
}

class Interface;

class Server
{
public:
	Server(std::string cards);
	virtual ~Server();

	void init_cards(std::string cards);

private:
	std::vector<std::unique_ptr<Interface>> cards;

};
