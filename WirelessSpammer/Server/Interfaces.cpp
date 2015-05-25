/*
 * File:   Interfaces.cpp
 * Author: mainn_000
 *
 * Created on May 24, 2015, 7:09 AM
 */

#include "Interfaces.h"
#include "Interface.h"

Interfaces::Interfaces():
	Thread()
{}

Interfaces::~Interfaces()
{}

void Interfaces::add(iface_ptr iface)
{
	std::lock_guard<std::mutex> lock(this->mutex);
	this->not_started.push(std::move(iface));
	this->notify();
}

void Interfaces::on_notify()
{
	std::lock_guard<std::mutex> lock(this->mutex);
	while (!this->not_started.empty())
	{
		iface_ptr iface = std::move(this->not_started.front());
		this->not_started.pop();

		iface->start(this->get_loop());

		this->started.push_back(std::move(iface));
	}
}

void Interfaces::on_start()
{
	this->on_notify();
}

void Interfaces::on_stop()
{
	std::lock_guard<std::mutex> lock(this->mutex);

	while (!this->not_started.empty())
		this->not_started.pop();

	for (auto &iface : this->started)
		iface->stop();
}
