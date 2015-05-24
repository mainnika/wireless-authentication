#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "Server.h"

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

int main(int argc, char* argv[])
{
	google::InitGoogleLogging(argv[0]);
	google::LogToStderr();

	Server server("mon0");

//	char *iface[MAX_CARDS];
//	const char *s_iface = "mon0";
//	fd_set rfds;
//	int caplen = 0, i, fdh = 0;
//	int fd_raw[MAX_CARDS];
//	int num_cards = 0;
//	struct timeval tv0;
//	struct rx_info ri;
//	struct wif * wi[MAX_CARDS];
//	unsigned char buffer[4096];
//
//	srand(time(NULL));
//
//	std::fill_n(fd_raw, MAX_CARDS, -1);
//	tv0.tv_sec = 0;
//	tv0.tv_usec = REFRESH_RATE;
//	FD_ZERO(&rfds);
//
//	num_cards = init_cards(s_iface, iface, wi);
//	for (i = 0; i < num_cards; i++)
//	{
//		fd_raw[i] = wi_fd(wi[i]);
//		if (fd_raw[i] > fdh)
//			fdh = fd_raw[i];
//
//		FD_SET(fd_raw[i], &rfds);
//	}
//
//	if (select(fdh + 1, &rfds, NULL, NULL, &tv0) < 0)
//	{
//		LOG(ERROR) << "Error select()";
//		return ( 1);
//	}
//
//	for (i = 0; i < num_cards; i++)
//	{
//		if (!FD_ISSET(fd_raw[i], &rfds))
//			continue;
//
//		memset(buffer, 0, sizeof (buffer));
//		if ((caplen = wi_read(wi[i], buffer, sizeof (buffer), &ri)) == -1)
//		{
//			LOG(ERROR) << "Error wi_read() from " << iface[i];
//			return ( 2);
//		}
//
//		parse_packet(buffer, caplen, &ri, i);
//	}

	LOG(INFO) << "Done";
}