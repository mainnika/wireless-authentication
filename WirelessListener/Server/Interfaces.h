/*
 * File:   Interfaces.h
 * Author: mainn_000
 *
 * Created on May 24, 2015, 7:09 AM
 */

#pragma once

#include "Thread.h"
#include "iface_ptr.h"

#include <queue>
#include <mutex>

class Interface;

class Interfaces : public Thread
{
public:
	Interfaces();
	virtual ~Interfaces();

	void add(iface_ptr iface);

private:
	std::mutex mutex;
	std::queue<iface_ptr> not_started;
	std::vector<iface_ptr> started;

	virtual void on_notify();
	virtual void on_start();
	virtual void on_stop();


};