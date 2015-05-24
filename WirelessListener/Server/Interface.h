/*
 * File:   Interface.h
 * Author: mainn_000
 *
 * Created on May 24, 2015, 2:44 AM
 */

#include "wif_ptr.h"

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

	void listen(struct ev_loop *loop);

	static std::function<bool(std::unique_ptr<Interface>&)> compare_with(std::string needed);

private:
	std::string iface;
	wif_ptr wi;
	int fd;

	std::unique_ptr<ev::io> watcher_read;
	std::unique_ptr<ev::io> watcher_write;
	std::unique_ptr<ev::async> notifier_write;

	void on_read(ev::io &w, int revets);
	void on_write(ev::io &w, int revets);
	void on_notify(ev::async &w, int revets);
};