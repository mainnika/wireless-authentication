
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

extern "C" {
#include "osdep/osdep.h"
}

#define MAX_CARDS 8
#define REFRESH_RATE 100000  /* default delay in us between updates */
#define DEFAULT_HOPFREQ 250  /* default delay in ms between channel hopping */

int *frequencies;

int setup_card(char *iface, struct wif **wis)
{
	struct wif *wi;

	wi = wi_open(iface);
	if (!wi)
		return -1;
	*wis = wi;

	return 0;
}

int init_cards(const char* cardstr, char *iface[], struct wif **wi)
{
	char *buffer;
	char *buf;
	int if_count = 0;
	int i = 0, again = 0;

	buf = buffer = (char*) malloc(sizeof (char) * 1025);
	strncpy(buffer, cardstr, 1025);
	buffer[1024] = '\0';

	while (((iface[if_count] = strsep(&buffer, ",")) != NULL) && (if_count < MAX_CARDS))
	{
		again = 0;
		for (i = 0; i < if_count; i++)
		{
			if (strcmp(iface[i], iface[if_count]) == 0)
				again = 1;
		}
		if (again) continue;
		if (setup_card(iface[if_count], &(wi[if_count])) != 0)
		{
			free(buf);
			return -1;
		}
		if_count++;
	}

	free(buf);
	return if_count;
}

int detect_frequencies(struct wif *wi)
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

//int check_channel(struct wif *wi[], int cards)
//{
//	int i, chan;
//	for (i = 0; i < cards; i++)
//	{
//		chan = wi_get_channel(wi[i]);
//		if (G.ignore_negative_one == 1 && chan == -1) return 0;
//		if (G.channel[i] != chan)
//		{
//			memset(G.message, '\x00', sizeof (G.message));
//			snprintf(G.message, sizeof (G.message), "][ fixed channel %s: %d ", wi_get_ifname(wi[i]), chan);
//			wi_set_channel(wi[i], G.channel[i]);
//		}
//	}
//	return 0;
//}

//int check_frequency(struct wif *wi[], int cards)
//{
//	int i, freq;
//	for (i = 0; i < cards; i++)
//	{
//		freq = wi_get_freq(wi[i]);
//		if (freq < 0) continue;
//		if (G.frequency[i] != freq)
//		{
//			memset(G.message, '\x00', sizeof (G.message));
//			snprintf(G.message, sizeof (G.message), "][ fixed frequency %s: %d ", wi_get_ifname(wi[i]), freq);
//			wi_set_freq(wi[i], G.frequency[i]);
//		}
//	}
//	return 0;
//}

struct globals
{
//    struct AP_info *ap_1st, *ap_end;
//    struct ST_info *st_1st, *st_end;
//    struct NA_info *na_1st, *na_end;
//    struct oui *manufList;

    unsigned char prev_bssid[6];
    unsigned char f_bssid[6];
    unsigned char f_netmask[6];
    char **f_essid;
    int f_essid_count;
#ifdef HAVE_PCRE
    pcre *f_essid_regex;
#endif
    char *dump_prefix;
    char *f_cap_name;

    int f_index;            /* outfiles index       */
    FILE *f_txt;            /* output csv file      */
    FILE *f_kis;            /* output kismet csv file      */
    FILE *f_kis_xml;        /* output kismet netxml file */
    FILE *f_gps;            /* output gps file      */
    FILE *f_cap;            /* output cap file      */
    FILE *f_ivs;            /* output ivs file      */
    FILE *f_xor;            /* output prga file     */

    char * batt;            /* Battery string       */
    int channel[MAX_CARDS];           /* current channel #    */
    int frequency[MAX_CARDS];           /* current frequency #    */
    int ch_pipe[2];         /* current channel pipe */
    int cd_pipe[2];	    /* current card pipe    */
    int gc_pipe[2];         /* gps coordinates pipe */
    float gps_loc[5];       /* gps coordinates      */
    int save_gps;           /* keep gps file flag   */
    int usegpsd;            /* do we use GPSd?      */
    int *channels;
//     int *frequencies;
    int singlechan;         /* channel hopping set 1*/
    int singlefreq;         /* frequency hopping: 1 */
    int chswitch;	    /* switching method     */
    int f_encrypt;          /* encryption filter    */
    int update_s;	    /* update delay in sec  */

    int is_wlanng[MAX_CARDS];          /* set if wlan-ng       */
    int is_orinoco[MAX_CARDS];         /* set if orinoco       */
    int is_madwifing[MAX_CARDS];       /* set if madwifi-ng    */
    int is_zd1211rw[MAX_CARDS];       /* set if zd1211rw    */
    volatile int do_exit;            /* interrupt flag       */
//    struct winsize ws;      /* console window size  */

    char * elapsed_time;	/* capture time			*/

    int one_beacon;         /* Record only 1 beacon?*/

    unsigned char sharedkey[3][4096]; /* array for 3 packets with a size of \
                               up to 4096Byte */
    time_t sk_start;
    char *prefix;
    int sk_len;
    int sk_len2;

    int * own_channels;	    /* custom channel list  */
    int * own_frequencies;	    /* custom frequency list  */

    int record_data;		/* do we record data?   */
    int asso_client;        /* only show associated clients */

    char * iwpriv;
    char * iwconfig;
    char * wlanctlng;
    char * wl;

    unsigned char wpa_bssid[6];   /* the wpa handshake bssid   */
    char message[512];
    char decloak;

    char is_berlin;           /* is the switch --berlin set? */
    int numaps;               /* number of APs on the current list */
    int maxnumaps;            /* maximum nubers of APs on the list */
    int maxaps;               /* number of all APs found */
    int berlin;               /* number of seconds it takes in berlin to fill the whole screen with APs*/
    /*
     * The name for this option may look quite strange, here is the story behind it:
     * During the CCC2007, 10 august 2007, we (hirte, Mister_X) went to visit Berlin
     * and couldn't resist to turn on airodump-ng to see how much access point we can
     * get during the trip from Finowfurt to Berlin. When we were in Berlin, the number
     * of AP increase really fast, so fast that it couldn't fit in a screen, even rotated;
     * the list was really huge (we have a picture of that). The 2 minutes timeout
     * (if the last packet seen is higher than 2 minutes, the AP isn't shown anymore)
     * wasn't enough, so we decided to create a new option to change that timeout.
     * We implemented this option in the highest tower (TV Tower) of Berlin, eating an ice.
     */

    int show_ap;
    int show_sta;
    int show_ack;
    int hide_known;

    int hopfreq;

    char*   s_file;         /* source file to read packets */
    char*   s_iface;        /* source interface to read from */
    FILE *f_cap_in;
//    struct pcap_file_header pfh_in;
    int detect_anomaly;     /* Detect WIPS protecting WEP in action */

    char *freqstring;
    int freqoption;
    int chanoption;
    int active_scan_sim;    /* simulates an active scan, sending probe requests */

    /* Airodump-ng start time: for kismet netxml file */
    char * airodump_start_time;

    int output_format_pcap;
    int output_format_csv;
    int output_format_kismet_csv;
    int output_format_kismet_netxml;
    pthread_t input_tid;
    int sort_by;
    int sort_inv;
    int start_print_ap;
    int start_print_sta;
    int selected_ap;
    int selected_sta;
    int selection_ap;
    int selection_sta;
    int mark_cur_ap;
    int num_cards;
    int skip_columns;
    int do_pause;
    int do_sort_always;

    pthread_mutex_t mx_print;			 /* lock write access to ap LL   */
    pthread_mutex_t mx_sort;			 /* lock write access to ap LL   */

    unsigned char selected_bssid[6];	/* bssid that is selected */

    int ignore_negative_one;
    u_int maxsize_essid_seen;
    int show_manufacturer;
    int show_uptime;
    int file_write_interval;
    u_int maxsize_wps_seen;
    int show_wps;
}
G;

int abg_chans [] =
{
    1, 7, 13, 2, 8, 3, 14, 9, 4, 10, 5, 11, 6, 12,
    36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
    112, 116, 120, 124, 128, 132, 136, 140, 149,
    153, 157, 161, 184, 188, 192, 196, 200, 204,
    208, 212, 216,0
};

int bg_chans  [] =
{
    1, 7, 13, 2, 8, 3, 14, 9, 4, 10, 5, 11, 6, 12, 0
};

int a_chans   [] =
{
    36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
    112, 116, 120, 124, 128, 132, 136, 140, 149,
    153, 157, 161, 184, 188, 192, 196, 200, 204,
    208, 212, 216,0
};

int main()
{
	int fd_raw[MAX_CARDS];
	int caplen = 0, i, fdh, fd_is_set;
	struct timeval tv0;
	fd_set rfds;
	unsigned char buffer[4096];
	char ifnam[64];
	struct wif * wi[MAX_CARDS];
	struct rx_info ri;
	char *iface[MAX_CARDS];

	{	// init
		srand(time(NULL));
		memset(&G, 0, sizeof ( G));

		G.chanoption = 0;
		G.freqoption = 0;
		G.num_cards = 0;
		fdh = 0;
		fd_is_set = 0;
		G.batt = NULL;
		G.chswitch = 0;
		G.usegpsd = 0;
		G.channels = bg_chans;
		G.one_beacon = 1;
		G.singlechan = 0;
		G.singlefreq = 0;
		G.dump_prefix = NULL;
		G.record_data = 0;
		G.f_cap = NULL;
		G.f_ivs = NULL;
		G.f_txt = NULL;
		G.f_kis = NULL;
		G.f_kis_xml = NULL;
		G.f_gps = NULL;
		G.f_xor = NULL;
		G.sk_len = 0;
		G.sk_len2 = 0;
		G.sk_start = 0;
		G.prefix = NULL;
		G.f_encrypt = 0;
		G.asso_client = 0;
		G.f_essid = NULL;
		G.f_essid_count = 0;
		G.active_scan_sim = 0;
		G.update_s = 0;
		G.decloak = 1;
		G.is_berlin = 0;
		G.numaps = 0;
		G.maxnumaps = 0;
		G.berlin = 120;
		G.show_ap = 1;
		G.show_sta = 1;
		G.show_ack = 0;
		G.hide_known = 0;
		G.maxsize_essid_seen = 5; // Initial value: length of "ESSID"
		G.show_manufacturer = 0;
		G.show_uptime = 0;
		G.hopfreq = DEFAULT_HOPFREQ;
		G.s_file = NULL;
		G.s_iface = NULL;
		G.f_cap_in = NULL;
		G.detect_anomaly = 0;
		G.airodump_start_time = NULL;
		G.output_format_pcap = 1;
		G.output_format_csv = 1;
		G.output_format_kismet_csv = 1;
		G.output_format_kismet_netxml = 1;
		G.file_write_interval = 5; // Write file every 5 seconds by default
		G.maxsize_wps_seen = 6;
		G.show_wps = 0;

		G.sort_by = 2;
		G.sort_inv = 1;

		G.start_print_ap = 1;
		G.start_print_sta = 1;
		G.selected_ap = 1;
		G.selected_sta = 1;
		G.selection_ap = 0;
		G.selection_sta = 0;
		G.mark_cur_ap = 0;
		G.skip_columns = 0;
		G.do_pause = 0;
		G.do_sort_always = 0;
		memset(G.selected_bssid, '\x00', 6);

		memset(G.sharedkey, '\x00', 512 * 3);
		memset(G.message, '\x00', sizeof (G.message));

		gettimeofday(&tv0, NULL);

		for (i = 0; i < MAX_CARDS; i++)
		{
			fd_raw[i] = -1;
			G.channel[i] = 0;
		}

		memset(G.f_bssid, '\x00', 6);
		memset(G.f_netmask, '\x00', 6);
		memset(G.wpa_bssid, '\x00', 6);
	}

	G.s_iface = strdup("mon0");

	G.num_cards = init_cards(G.s_iface, iface, wi);
	for (i = 0; i < G.num_cards; i++)
	{
		fd_raw[i] = wi_fd(wi[i]);
		if (fd_raw[i] > fdh)
			fdh = fd_raw[i];
	}

	{
		FD_ZERO(&rfds);
		for (i = 0; i < G.num_cards; i++)
		{
			FD_SET(fd_raw[i], &rfds);
		}

		tv0.tv_sec = G.update_s;
		tv0.tv_usec = (G.update_s == 0) ? REFRESH_RATE : 0;

		if (select(fdh + 1, &rfds, NULL, NULL, &tv0) < 0)
		{
			perror("select failed");

			/* Restore terminal */
			fprintf(stderr, "\33[?25h");
			fflush(stdout);

			return ( 1);
		}

		fd_is_set = 0;

		for (i = 0; i < G.num_cards; i++)
		{
			if (FD_ISSET(fd_raw[i], &rfds))
			{

				memset(buffer, 0, sizeof (buffer));
				if ((caplen = wi_read(wi[i], buffer, sizeof (buffer), &ri)) == -1)
				{
					//reopen in monitor mode

					strncpy(ifnam, wi_get_ifname(wi[i]), sizeof (ifnam) - 1);
					ifnam[sizeof (ifnam) - 1] = 0;

//					wi_close(wi[i]);
//					wi[i] = wi_open(ifnam);
//					if (!wi[i])
//					{
						printf("Can't reopen %s\n", ifnam);

						/* Restore terminal */
						fprintf(stderr, "\33[?25h");
						fflush(stdout);

						exit(1);
//					}
//
//					fd_raw[i] = wi_fd(wi[i]);
//					if (fd_raw[i] > fdh)
//						fdh = fd_raw[i];
//
//					break;
				}

				printf("Capped %d\n", caplen);

//				dump_add_packet(buffer, caplen, &ri, i);
			}
		}
	}

	printf("Done");
}