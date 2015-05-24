/*
 * File:   Interface.cpp
 * Author: mainn_000
 *
 * Created on May 24, 2015, 2:44 AM
 */

#include "Interface.h"

#include <iostream>
#include <glog/logging.h>
#include <ev++.h>

extern "C"
{
#include "core.h"
}

Interface::Interface(wif_ptr wi, std::string iface) :
	iface(iface), stopped(true),
	watcher_read(new ev::io()), watcher_write(new ev::io())
{
	this->wi = std::move(wi);
	this->fd = wi_fd(this->wi.get());

	this->watcher_read->set<Interface, &Interface::on_read>(this);
	this->watcher_write->set<Interface, &Interface::on_write>(this);

	int channel = wi_get_channel(this->wi.get());

	LOG(INFO) << "Interface " << this->iface << " created, channel " << channel;
}

Interface::~Interface()
{
	LOG(INFO) << "Interface " << this->iface << " closed";
}

void Interface::start(struct ev_loop *loop)
{
	this->stopped = false;
	this->watcher_read->set(loop);
	this->watcher_write->set(loop);

	this->watcher_read->start(this->fd, EV_READ);
	this->watcher_write->start(this->fd, EV_WRITE);

	LOG(INFO) << "Interface " << this->iface << " started";
}

void Interface::stop()
{
	this->stopped = true;
	this->watcher_read->stop();
	this->watcher_write->stop();

	LOG(INFO) << "Interface " << this->iface << " stopped";
}

std::function<bool(iface_ptr&) > Interface::compare_with(std::string needed)
{
	return [needed](iface_ptr& some)
	{
		return some->iface == needed;
	};
}

void Interface::on_read(ev::io& w, int revets)
{
//	LOG(INFO) << "Interface " << this->iface << " on_read()";

	unsigned char buffer[4096];
	struct rx_info ri;

	int caplen = wi_read(this->wi.get(), buffer, sizeof (buffer), &ri);

	if (this->stopped || caplen <= 0)
		return;

	if (caplen < 24)
	{
		LOG(INFO) << "Capped unknow packet with len " << caplen;
		return;
	}

	uint32_t type = buffer[0];
	uint32_t subtype = buffer[1];

	LOG(INFO) << "Capped packet with type " << std::hex << type << " and len " << std::dec << caplen;
}

void Interface::on_write(ev::io& w, int revets)
{
//	LOG(INFO) << "Interface " << this->iface << " on_write()";
}

void Interface::on_notify(ev::async& w, int revets)
{
//	LOG(INFO) << "Interface " << this->iface << " on_notify()";
}
