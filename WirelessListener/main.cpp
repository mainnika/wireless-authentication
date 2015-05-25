#include <iostream>
#include <getopt.h>
#include <ev++.h>
#include <glog/logging.h>

#include "Server.h"

void on_shutdown(ev::sig &w, int)
{
	w.loop.break_loop(ev::ALL);
}

int main(int argc, char* argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::LogToStderr();

	LOG(INFO) << "Using libev " << ev::version_major() << "." << ev::version_minor();
	LOG_ASSERT(ev::recommended_backends() & EVBACKEND_EPOLL);

	std::string iface = "mon0";

	char opt;
	while ((opt = getopt(argc, argv, "i:")) != -1)
	{
		switch (opt)
		{
			case 'i':
				iface.assign(optarg);
				break;
			default:
				LOG(ERROR) << "Invalid usage";
				return -1;
		}
	}

	Server server(iface);
	server.start();

	ev::loop_ref loop = ev::get_default_loop();

	ev::sig signal_watcher_int(loop);
	signal_watcher_int.set<on_shutdown>();
	signal_watcher_int.start(SIGINT);

	ev::sig signal_watcher_term(loop);
	signal_watcher_term.set<on_shutdown>();
	signal_watcher_term.start(SIGTERM);

	loop.run(0);

	server.stop();

	LOG(INFO) << "Done";
}