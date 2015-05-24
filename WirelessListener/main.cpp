#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "Server.h"

#include <ev++.h>
#include <glog/logging.h>

extern "C"
{
#include "core.h"
#include "common.h"
#include "pcap.h"
}

#define MAX_CARDS 8
#define REFRESH_RATE 100000  /* default delay in us between updates */

int detect_frequencies(struct wif *wi, int* frequencies)
{
	int start_freq = 2192;
	int end_freq = 2732;
	int max_freq_num = 2048; //should be enough to keep all available channels
	int freq = 0, i = 0;

	printf("Checking available frequencies, this could take few seconds.\n");

	frequencies = (int*) malloc((max_freq_num + 1) * sizeof (int)); //field for frequencies supported
	memset(frequencies, 0, (max_freq_num + 1) * sizeof (int));
	for (freq = start_freq; freq <= end_freq; freq += 5)
	{
		if (wi_set_freq(wi, freq) == 0)
		{
			frequencies[i] = freq;
			i++;
		}
		if (freq == 2482)
		{
			//special case for chan 14, as its 12MHz away from 13, not 5MHz
			freq = 2484;
			if (wi_set_freq(wi, freq) == 0)
			{
				frequencies[i] = freq;
				i++;
			}
			freq = 2482;
		}
	}

	//again for 5GHz channels
	start_freq = 4800;
	end_freq = 6000;
	for (freq = start_freq; freq <= end_freq; freq += 5)
	{
		if (wi_set_freq(wi, freq) == 0)
		{
			frequencies[i] = freq;
			i++;
		}
	}

	printf("Done.\n");
	return 0;
}

int check_monitor(struct wif *wi[], int *fd_raw, int *fdh, int cards)
{
	int i, monitor;
	char ifname[64];

	for (i = 0; i < cards; i++)
	{
		monitor = wi_get_monitor(wi[i]);
		if (monitor != 0)
		{
			strncpy(ifname, wi_get_ifname(wi[i]), sizeof (ifname) - 1);
			ifname[sizeof (ifname) - 1] = 0;

			wi_close(wi[i]);
			wi[i] = wi_open(ifname);
			if (!wi[i])
			{
				printf("Can't reopen %s\n", ifname);
				exit(1);
			}

			fd_raw[i] = wi_fd(wi[i]);
			if (fd_raw[i] > *fdh)
				*fdh = fd_raw[i];
		}
	}
	return 0;
}

int parse_packet(unsigned char *h80211, int caplen, struct rx_info *ri, int cardnum)
{
	if (caplen < 24)
	{
		LOG(INFO) << "Capped unknow packet with len " << caplen;
		return 0;
	}

	uint8_t type = h80211[0];
	uint8_t subtype = h80211[1];

	LOG(INFO) << "Capped packet with type " << type << " and len " << caplen;
	return 0;
}

void on_shutdown(ev::sig &w, int)
{
	w.loop.break_loop(ev::ALL);
}

class Test : public Thread
{
public:
	Test():
		Thread()
	{};
	virtual ~Test()
	{}

private:
	virtual void on_notify()
	{
		LOG(INFO) << "notify";
	}

};

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