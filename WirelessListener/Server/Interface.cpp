/*
 * File:   Interface.cpp
 * Author: mainn_000
 *
 * Created on May 24, 2015, 2:44 AM
 */

#include "Interface.h"

Interface::Interface(wif_ptr wi)
{
	this->wi = std::move(wi);
}

Interface::~Interface()
{
}

std::function<bool(std::unique_ptr<Interface>&) > Interface::compare_with(std::string needed)
{
	return [needed](std::unique_ptr<Interface>& some)
	{
		return some->if_name == needed;
	};
}
