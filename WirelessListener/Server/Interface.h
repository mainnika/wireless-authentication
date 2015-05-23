/*
 * File:   Interface.h
 * Author: mainn_000
 *
 * Created on May 24, 2015, 2:44 AM
 */

#include "wif_ptr.h"

class Interface
{
public:
	Interface(wif_ptr wi);
	virtual ~Interface();

	static std::function<bool(std::unique_ptr<Interface>&)> compare_with(std::string needed);

private:
	std::string if_name;
	int fd;
	rx_info ri;
	wif_ptr wi;
};