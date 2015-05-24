/*
 * File:   Interface.h
 * Author: mainn_000
 *
 * Created on May 24, 2015, 2:44 AM
 */

#include "wif_ptr.h"
#include "iface_ptr.h"

#include <atomic>

namespace ev
{
	class async;
	class io;
}

class Interface
{
public:
	Interface(wif_ptr wi, std::string iface);
	virtual ~Interface();

	void start(struct ev_loop *loop);
	void stop();

	static std::function<bool(iface_ptr&)> compare_with(std::string needed);

private:
	std::string iface;
	std::atomic<bool> stopped;
	wif_ptr wi;
	int fd;

	std::unique_ptr<ev::io> watcher_read;
	std::unique_ptr<ev::io> watcher_write;

	void on_read(ev::io &w, int revets);
	void on_write(ev::io &w, int revets);
	void on_notify(ev::async &w, int revets);
};