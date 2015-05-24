/*
 * File:   Interfaces.h
 * Author: mainn_000
 *
 * Created on May 24, 2015, 7:09 AM
 */

#pragma once

#include "Thread.h"

#include <queue>
#include <mutex>

class Interface;

class Interfaces : public Thread
{
public:
	Interfaces();
	virtual ~Interfaces();

	void add(std::unique_ptr<Interface> iface);

private:
	std::mutex mutex;
	std::queue<std::unique_ptr<Interface>> not_started;
	std::vector<std::unique_ptr<Interface>> started;

	virtual void on_notify();
	virtual void on_start();

};