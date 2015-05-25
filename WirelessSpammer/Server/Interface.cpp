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
#include <fcntl.h>

extern "C"
{
#include "core.h"
}

#define PROBE_REQ       \
    "\xDD\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xCC\xCC\xCC\xCC\xCC\xCC"  \
    "\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00"


Interface::Interface(wif_ptr wi, std::string iface) :
	iface(iface), stopped(true),
	watcher_read(new ev::io()), watcher_write(new ev::io())
{
	this->wi = std::move(wi);
	this->fd = wi_fd(this->wi.get());

	LOG_ASSERT(fcntl( this->fd, F_SETFL, O_NONBLOCK ) >= 0);

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
}

void Interface::on_write(ev::io& w, int revets)
{
//	LOG(INFO) << "Interface " << this->iface << " on_write()";

	if (this->stopped)
		return;

	unsigned char h80211[4096];
	memcpy(h80211, PROBE_REQ, 24);

	int written = wi_write(this->wi.get(), h80211, 24, NULL);

	LOG(INFO) << "Written " << written << " bytes to " << this->iface;
}

void Interface::on_notify(ev::async& w, int revets)
{
//	LOG(INFO) << "Interface " << this->iface << " on_notify()";
}
