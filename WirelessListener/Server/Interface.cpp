/*
 * File:   Interface.cpp
 * Author: mainn_000
 *
 * Created on May 24, 2015, 2:44 AM
 */

#include "Interface.h"

#include <glog/logging.h>
#include <ev++.h>

extern "C"
{
#include "core.h"
}

Interface::Interface(wif_ptr wi, std::string iface) :
	iface(iface),
	watcher_read(new ev::io()), watcher_write(new ev::io()), notifier_write(new ev::async())
{
	this->wi = std::move(wi);
	this->fd = wi_fd(this->wi.get());

	this->watcher_read->set<Interface, &Interface::on_read>(this);
	this->watcher_write->set<Interface, &Interface::on_write>(this);
	this->notifier_write->set<Interface, &Interface::on_notify>(this);

	int channel = wi_get_channel(this->wi.get());

	LOG(INFO) << "Interface " << this->iface << " created, channel " << channel;
}

Interface::~Interface()
{
	wi_close(this->wi.get());
	LOG(INFO) << "Interface " << this->iface << " closed";
}

void Interface::listen(struct ev_loop *loop)
{
	this->watcher_read->set(loop);
	this->watcher_write->set(loop);
	this->notifier_write->set(loop);

	this->watcher_read->set(this->fd, EV_READ);
	this->watcher_write->set(this->fd, EV_WRITE);

	LOG(INFO) << "Interface " << this->iface << " started";
}

std::function<bool(std::unique_ptr<Interface>&) > Interface::compare_with(std::string needed)
{
	return [needed](std::unique_ptr<Interface>& some)
	{
		return some->iface == needed;
	};
}

void Interface::on_read(ev::io& w, int revets)
{

}

void Interface::on_write(ev::io& w, int revets)
{

}

void Interface::on_notify(ev::async& w, int revets)
{

}
