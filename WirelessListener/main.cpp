
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

extern "C"
{
#include "osdep/osdep.h"
#include "osdep/common.h"
#include "eapol.h"
#include "pcap.h"
}

#define MAX_CARDS 8
#define REFRESH_RATE 100000  /* default delay in us between updates */
#define DEFAULT_HOPFREQ 250  /* default delay in ms between channel hopping */
#define NB_PWR  5       /* size of signal power ring buffer */
#define NB_PRB 10       /* size of probed ESSID ring buffer */
#define MAX_IE_ELEMENT_SIZE 256
#define NULL_MAC  (unsigned char*)"\x00\x00\x00\x00\x00\x00"
#define BROADCAST (unsigned char*)"\xFF\xFF\xFF\xFF\xFF\xFF"
#define SPANTREE  (unsigned char*)"\x01\x80\xC2\x00\x00\x00"
#define CDP_VTP   (unsigned char*)"\x01\x00\x0C\xCC\xCC\xCC"
#define	STD_OPN		0x0001
#define	STD_WEP		0x0002
#define	STD_WPA		0x0004
#define	STD_WPA2	0x0008
#define	STD_OPN		0x0001
#define	STD_WEP		0x0002
#define	STD_WPA		0x0004
#define	STD_WPA2	0x0008
#define STD_FIELD	(STD_OPN | STD_WEP | STD_WPA | STD_WPA2)
#define	ENC_WEP		0x0010
#define	ENC_TKIP	0x0020
#define	ENC_WRAP	0x0040
#define	ENC_CCMP	0x0080
#define ENC_WEP40	0x1000
#define	ENC_WEP104	0x0100
#define ENC_FIELD	(ENC_WEP | ENC_TKIP | ENC_WRAP | ENC_CCMP | ENC_WEP40 | ENC_WEP104)
#define	AUTH_OPN	0x0200
#define	AUTH_PSK	0x0400
#define	AUTH_MGT	0x0800
#define AUTH_FIELD	(AUTH_OPN | AUTH_PSK | AUTH_MGT)
#define STD_QOS         0x2000

int *frequencies;

struct WPS_info
{
	unsigned char version; /* WPS Version */
	unsigned char state; /* Current WPS state */
	unsigned char ap_setup_locked; /* AP setup locked */
	unsigned int meth; /* WPS Config Methods */
};

struct pkt_buf
{
    struct pkt_buf  *next;      /* next packet in list */
    unsigned char   *packet;    /* packet */
    unsigned short  length;     /* packet length */
    struct timeval  ctime;      /* capture time */
};

struct oui {
	char id[9]; /* TODO: Don't use ASCII chars to compare, use unsigned char[3] (later) with the value (hex ascii will have to be converted) */
	char manuf[128]; /* TODO: Switch to a char * later to improve memory usage */
	struct oui *next;
};

struct AP_info
{
	struct AP_info *prev; /* prev. AP in list         */
	struct AP_info *next; /* next  AP in list         */

	time_t tinit, tlast; /* first and last time seen */

	int channel; /* AP radio channel         */
	int max_speed; /* AP maximum speed in Mb/s */
	int avg_power; /* averaged signal power    */
	int best_power; /* best signal power    */
	int power_index; /* index in power ring buf. */
	int power_lvl[NB_PWR]; /* signal power ring buffer */
	int preamble; /* 0 = long, 1 = short      */
	int security; /* ENC_*, AUTH_*, STD_*     */
	int beacon_logged; /* We need 1 beacon per AP  */
	int dict_started; /* 1 if dict attack started */
	int ssid_length; /* length of ssid           */
	float gps_loc_min[5]; /* min gps coordinates      */
	float gps_loc_max[5]; /* max gps coordinates      */
	float gps_loc_best[5]; /* best gps coordinates     */

	unsigned long nb_bcn; /* total number of beacons  */
	unsigned long nb_pkt; /* total number of packets  */
	unsigned long nb_data; /* number of  data packets  */
	unsigned long nb_data_old; /* number of data packets/sec*/
	int nb_dataps; /* number of data packets/sec*/
	struct timeval tv; /* time for data per second */

	unsigned char bssid[6]; /* the access point's MAC   */
	char *manuf; /* the access point's manufacturer */
	unsigned char essid[MAX_IE_ELEMENT_SIZE];
	/* ascii network identifier */
	unsigned long long timestamp;
	/* Timestamp to calculate uptime   */

	unsigned char lanip[4]; /* last detected ip address */
	/* if non-encrypted network */

	unsigned char **uiv_root; /* unique iv root structure */
	/* if wep-encrypted network */

	int rx_quality; /* percent of captured beacons */
	int fcapt; /* amount of captured frames   */
	int fmiss; /* amount of missed frames     */
	unsigned int last_seq; /* last sequence number        */
	struct timeval ftimef; /* time of first frame         */
	struct timeval ftimel; /* time of last frame          */
	struct timeval ftimer; /* time of restart             */

	char *key; /* if wep-key found by dict */
	int essid_stored; /* essid stored in ivs file? */

	char decloak_detect; /* run decloak detection? */
	struct pkt_buf *packets; /* list of captured packets (last few seconds) */
	char is_decloak; /* detected decloak */

	// This feature eats 48Mb per AP
	int EAP_detected;
	unsigned char *data_root; /* first 2 bytes of data if */
	/* WEP network; used for    */
	/* detecting WEP cloak	  */
	/* + one byte to indicate   */
	/* (in)existence of the IV  */

	int marked;
	int marked_color;
	struct WPS_info wps;
};

/* linked list of detected clients */

struct ST_info
{
	struct ST_info *prev; /* the prev client in list   */
	struct ST_info *next; /* the next client in list   */
	struct AP_info *base; /* AP this client belongs to */
	time_t tinit, tlast; /* first and last time seen  */
	unsigned long nb_pkt; /* total number of packets   */
	unsigned char stmac[6]; /* the client's MAC address  */
	char *manuf; /* the client's manufacturer */
	int probe_index; /* probed ESSIDs ring index  */
	char probes[NB_PRB][MAX_IE_ELEMENT_SIZE];
	/* probed ESSIDs ring buffer */
	int ssid_length[NB_PRB]; /* ssid lengths ring buffer  */
	int power; /* last signal power         */
	int rate_to; /* last bitrate to station   */
	int rate_from; /* last bitrate from station */
	struct timeval ftimer; /* time of restart           */
	int missed; /* number of missed packets  */
	unsigned int lastseq; /* last seen sequence number */
	struct WPA_hdsk wpa; /* WPA handshake data        */
	int qos_to_ds; /* does it use 802.11e to ds */
	int qos_fr_ds; /* does it receive 802.11e   */
	int channel; /* Channel station is seen   */
	/*  Not used yet		  */
};

/* linked list of detected macs through ack, cts or rts frames */

struct NA_info
{
	struct NA_info *prev; /* the prev client in list   */
	struct NA_info *next; /* the next client in list   */
	time_t tinit, tlast; /* first and last time seen  */
	unsigned char namac[6]; /* the stations MAC address  */
	int power; /* last signal power         */
	int channel; /* captured on channel       */
	int ack; /* number of ACK frames      */
	int ack_old; /* old number of ACK frames  */
	int ackps; /* number of ACK frames/s    */
	int cts; /* number of CTS frames      */
	int rts_r; /* number of RTS frames (rx) */
	int rts_t; /* number of RTS frames (tx) */
	int other; /* number of other frames    */
	struct timeval tv; /* time for ack per second   */
};

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
	struct AP_info *ap_1st, *ap_end;
	struct ST_info *st_1st, *st_end;
	struct NA_info *na_1st, *na_end;
	struct oui *manufList;

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

	int f_index; /* outfiles index       */
	FILE *f_txt; /* output csv file      */
	FILE *f_kis; /* output kismet csv file      */
	FILE *f_kis_xml; /* output kismet netxml file */
	FILE *f_gps; /* output gps file      */
	FILE *f_cap; /* output cap file      */
	FILE *f_ivs; /* output ivs file      */
	FILE *f_xor; /* output prga file     */

	char * batt; /* Battery string       */
	int channel[MAX_CARDS]; /* current channel #    */
	int frequency[MAX_CARDS]; /* current frequency #    */
	int ch_pipe[2]; /* current channel pipe */
	int cd_pipe[2]; /* current card pipe    */
	int gc_pipe[2]; /* gps coordinates pipe */
	float gps_loc[5]; /* gps coordinates      */
	int save_gps; /* keep gps file flag   */
	int usegpsd; /* do we use GPSd?      */
	int *channels;
	//     int *frequencies;
	int singlechan; /* channel hopping set 1*/
	int singlefreq; /* frequency hopping: 1 */
	int chswitch; /* switching method     */
	int f_encrypt; /* encryption filter    */
	int update_s; /* update delay in sec  */

	int is_wlanng[MAX_CARDS]; /* set if wlan-ng       */
	int is_orinoco[MAX_CARDS]; /* set if orinoco       */
	int is_madwifing[MAX_CARDS]; /* set if madwifi-ng    */
	int is_zd1211rw[MAX_CARDS]; /* set if zd1211rw    */
	volatile int do_exit; /* interrupt flag       */
	//    struct winsize ws;      /* console window size  */

	char * elapsed_time; /* capture time			*/

	int one_beacon; /* Record only 1 beacon?*/

	unsigned char sharedkey[3][4096]; /* array for 3 packets with a size of \
                               up to 4096Byte */
	time_t sk_start;
	char *prefix;
	int sk_len;
	int sk_len2;

	int * own_channels; /* custom channel list  */
	int * own_frequencies; /* custom frequency list  */

	int record_data; /* do we record data?   */
	int asso_client; /* only show associated clients */

	char * iwpriv;
	char * iwconfig;
	char * wlanctlng;
	char * wl;

	unsigned char wpa_bssid[6]; /* the wpa handshake bssid   */
	char message[512];
	char decloak;

	char is_berlin; /* is the switch --berlin set? */
	int numaps; /* number of APs on the current list */
	int maxnumaps; /* maximum nubers of APs on the list */
	int maxaps; /* number of all APs found */
	int berlin; /* number of seconds it takes in berlin to fill the whole screen with APs*/
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

	char* s_file; /* source file to read packets */
	char* s_iface; /* source interface to read from */
	FILE *f_cap_in;
	//    struct pcap_file_header pfh_in;
	int detect_anomaly; /* Detect WIPS protecting WEP in action */

	char *freqstring;
	int freqoption;
	int chanoption;
	int active_scan_sim; /* simulates an active scan, sending probe requests */

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

	pthread_mutex_t mx_print; /* lock write access to ap LL   */
	pthread_mutex_t mx_sort; /* lock write access to ap LL   */

	unsigned char selected_bssid[6]; /* bssid that is selected */

	int ignore_negative_one;
	u_int maxsize_essid_seen;
	int show_manufacturer;
	int show_uptime;
	int file_write_interval;
	u_int maxsize_wps_seen;
	int show_wps;
}
G;

int abg_chans [] ={
		   1, 7, 13, 2, 8, 3, 14, 9, 4, 10, 5, 11, 6, 12,
		   36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
		   112, 116, 120, 124, 128, 132, 136, 140, 149,
		   153, 157, 161, 184, 188, 192, 196, 200, 204,
		   208, 212, 216, 0
};

int bg_chans [] ={
		  1, 7, 13, 2, 8, 3, 14, 9, 4, 10, 5, 11, 6, 12, 0
};

int a_chans [] ={
		 36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
		 112, 116, 120, 124, 128, 132, 136, 140, 149,
		 153, 157, 161, 184, 188, 192, 196, 200, 204,
		 208, 212, 216, 0
};

int dump_add_packet(unsigned char *h80211, int caplen, struct rx_info *ri, int cardnum)
{
	int i, n, seq, msd, dlen, offset, clen, o;
	unsigned z;
	int type, length, numuni = 0, numauth = 0;
	struct pcap_pkthdr pkh;
	struct timeval tv;
	struct ivs2_pkthdr ivs2;
	unsigned char *p, *org_p, c;
	unsigned char bssid[6];
	unsigned char stmac[6];
	unsigned char namac[6];
	unsigned char clear[2048];
	int weight[16];
	int num_xor = 0;

	struct AP_info *ap_cur = NULL;
	struct ST_info *st_cur = NULL;
	struct NA_info *na_cur = NULL;
	struct AP_info *ap_prv = NULL;
	struct ST_info *st_prv = NULL;
	struct NA_info *na_prv = NULL;

	/* skip all non probe response frames in active scanning simulation mode */
	if (G.active_scan_sim > 0 && h80211[0] != 0x50)
		return (0);

	/* skip packets smaller than a 802.11 header */

	if (caplen < 24)
		goto write_packet;

	/* skip (uninteresting) control frames */

	if ((h80211[0] & 0x0C) == 0x04)
		goto write_packet;

	/* grab the sequence number */
	seq = ((h80211[22] >> 4)+(h80211[23] << 4));

	/* locate the access point's MAC address */

	switch (h80211[1] & 3)
	{
		case 0: memcpy(bssid, h80211 + 16, 6);
			break; //Adhoc
		case 1: memcpy(bssid, h80211 + 4, 6);
			break; //ToDS
		case 2: memcpy(bssid, h80211 + 10, 6);
			break; //FromDS
		case 3: memcpy(bssid, h80211 + 10, 6);
			break; //WDS -> Transmitter taken as BSSID
	}

	/* update our chained list of access points */

	ap_cur = G.ap_1st;
	ap_prv = NULL;

	while (ap_cur != NULL)
	{
		if (!memcmp(ap_cur->bssid, bssid, 6))
			break;

		ap_prv = ap_cur;
		ap_cur = ap_cur->next;
	}

	/* update the last time seen */

	ap_cur->tlast = time(NULL);

	/* only update power if packets comes from
	 * the AP: either type == mgmt and SA != BSSID,
	 * or FromDS == 1 and ToDS == 0 */

	if (((h80211[1] & 3) == 0 &&
	     memcmp(h80211 + 10, bssid, 6) == 0) ||
	    ((h80211[1] & 3) == 2))
	{
		ap_cur->power_index = (ap_cur->power_index + 1) % NB_PWR;
		ap_cur->power_lvl[ap_cur->power_index] = ri->ri_power;

		ap_cur->avg_power = 0;

		for (i = 0, n = 0; i < NB_PWR; i++)
		{
			if (ap_cur->power_lvl[i] != -1)
			{
				ap_cur->avg_power += ap_cur->power_lvl[i];
				n++;
			}
		}

		if (n > 0)
		{
			ap_cur->avg_power /= n;
			if (ap_cur->avg_power > ap_cur->best_power)
			{
				ap_cur->best_power = ap_cur->avg_power;
				memcpy(ap_cur->gps_loc_best, G.gps_loc, sizeof (float)*5);
			}
		}
		else
			ap_cur->avg_power = -1;

		/* every packet in here comes from the AP */

		if (G.gps_loc[0] > ap_cur->gps_loc_max[0])
			ap_cur->gps_loc_max[0] = G.gps_loc[0];
		if (G.gps_loc[1] > ap_cur->gps_loc_max[1])
			ap_cur->gps_loc_max[1] = G.gps_loc[1];
		if (G.gps_loc[2] > ap_cur->gps_loc_max[2])
			ap_cur->gps_loc_max[2] = G.gps_loc[2];

		if (G.gps_loc[0] < ap_cur->gps_loc_min[0])
			ap_cur->gps_loc_min[0] = G.gps_loc[0];
		if (G.gps_loc[1] < ap_cur->gps_loc_min[1])
			ap_cur->gps_loc_min[1] = G.gps_loc[1];
		if (G.gps_loc[2] < ap_cur->gps_loc_min[2])
			ap_cur->gps_loc_min[2] = G.gps_loc[2];
		//        printf("seqnum: %i\n", seq);

		if (ap_cur->fcapt == 0 && ap_cur->fmiss == 0) gettimeofday(&(ap_cur->ftimef), NULL);
		if (ap_cur->last_seq != 0) ap_cur->fmiss += (seq - ap_cur->last_seq - 1);
		ap_cur->last_seq = seq;
		ap_cur->fcapt++;
		gettimeofday(&(ap_cur->ftimel), NULL);

		//         if(ap_cur->fcapt >= QLT_COUNT) update_rx_quality();
	}

	switch (h80211[0])
	{
		case 0x80:
			ap_cur->nb_bcn++;
		case 0x50:
			/* reset the WPS state */
			ap_cur->wps.state = 0xFF;
			ap_cur->wps.ap_setup_locked = 0;
			break;
	}

	ap_cur->nb_pkt++;

	/* find wpa handshake */
	if (h80211[0] == 0x10)
	{
		/* reset the WPA handshake state */

		if (st_cur != NULL && st_cur->wpa.state != 0xFF)
			st_cur->wpa.state = 0;
		//        printf("initial auth %d\n", ap_cur->wpa_state);
	}

	/* locate the station MAC in the 802.11 header */

	switch (h80211[1] & 3)
	{
		case 0:

			/* if management, check that SA != BSSID */

			if (memcmp(h80211 + 10, bssid, 6) == 0)
				goto skip_station;

			memcpy(stmac, h80211 + 10, 6);
			break;

		case 1:

			/* ToDS packet, must come from a client */

			memcpy(stmac, h80211 + 10, 6);
			break;

		case 2:

			/* FromDS packet, reject broadcast MACs */

			if ((h80211[4] % 2) != 0) goto skip_station;
			memcpy(stmac, h80211 + 4, 6);
			break;

		default: goto skip_station;
	}

	/* update our chained list of wireless stations */

	st_cur = G.st_1st;
	st_prv = NULL;

	while (st_cur != NULL)
	{
		if (!memcmp(st_cur->stmac, stmac, 6))
			break;

		st_prv = st_cur;
		st_cur = st_cur->next;
	}

	if (st_cur->base == NULL ||
	    memcmp(ap_cur->bssid, BROADCAST, 6) != 0)
		st_cur->base = ap_cur;

	//update bitrate to station
	if ((st_cur != NULL) && (h80211[1] & 3) == 2)
		st_cur->rate_to = ri->ri_rate;

	/* update the last time seen */

	st_cur->tlast = time(NULL);

	/* only update power if packets comes from the
	 * client: either type == Mgmt and SA != BSSID,
	 * or FromDS == 0 and ToDS == 1 */

	if (((h80211[1] & 3) == 0 &&
	     memcmp(h80211 + 10, bssid, 6) != 0) ||
	    ((h80211[1] & 3) == 1))
	{
		st_cur->power = ri->ri_power;
		st_cur->rate_from = ri->ri_rate;
		if (ri->ri_channel > 0 && ri->ri_channel <= HIGHEST_CHANNEL)
			st_cur->channel = ri->ri_channel;
		else
			st_cur->channel = G.channel[cardnum];

		if (st_cur->lastseq != 0)
		{
			msd = seq - st_cur->lastseq - 1;
			if (msd > 0 && msd < 1000)
				st_cur->missed += msd;
		}
		st_cur->lastseq = seq;
	}

	st_cur->nb_pkt++;

skip_station:

	/* packet parsing: Probe Request */

	if (h80211[0] == 0x40 && st_cur != NULL)
	{
		p = h80211 + 24;

		while (p < h80211 + caplen)
		{
			if (p + 2 + p[1] > h80211 + caplen)
				break;

			if (p[0] == 0x00 && p[1] > 0 && p[2] != '\0' &&
			    (p[1] > 1 || p[2] != ' '))
			{
				//                n = ( p[1] > 32 ) ? 32 : p[1];
				n = p[1];

				for (i = 0; i < n; i++)
					if (p[2 + i] > 0 && p[2 + i] < ' ')
						goto skip_probe;

				/* got a valid ASCII probed ESSID, check if it's
				   already in the ring buffer */

				for (i = 0; i < NB_PRB; i++)
					if (memcmp(st_cur->probes[i], p + 2, n) == 0)
						goto skip_probe;

				st_cur->probe_index = (st_cur->probe_index + 1) % NB_PRB;
				memset(st_cur->probes[st_cur->probe_index], 0, 256);
				memcpy(st_cur->probes[st_cur->probe_index], p + 2, n); //twice?!
				st_cur->ssid_length[st_cur->probe_index] = n;

				for (i = 0; i < n; i++)
				{
					c = p[2 + i];
					if (c == 0 || (c > 126 && c < 160)) c = '.'; //could also check ||(c>0 && c<32)
					st_cur->probes[st_cur->probe_index][i] = c;
				}
			}

			p += 2 + p[1];
		}
	}

skip_probe:

	/* packet parsing: Beacon or Probe Response */

	if (h80211[0] == 0x80 || h80211[0] == 0x50)
	{
		if (!(ap_cur->security & (STD_OPN | STD_WEP | STD_WPA | STD_WPA2)))
		{
			if ((h80211[34] & 0x10) >> 4) ap_cur->security |= STD_WEP | ENC_WEP;
			else ap_cur->security |= STD_OPN;
		}

		ap_cur->preamble = (h80211[34] & 0x20) >> 5;

		unsigned long long *tstamp = (unsigned long long *) (h80211 + 24);
		ap_cur->timestamp = letoh64(*tstamp);

		p = h80211 + 36;

		while (p < h80211 + caplen)
		{
			if (p + 2 + p[1] > h80211 + caplen)
				break;

			//only update the essid length if the new length is > the old one
			if (p[0] == 0x00 && (ap_cur->ssid_length < p[1])) ap_cur->ssid_length = p[1];

			if (p[0] == 0x00 && p[1] > 0 && p[2] != '\0' &&
			    (p[1] > 1 || p[2] != ' '))
			{
				/* found a non-cloaked ESSID */

				//                n = ( p[1] > 32 ) ? 32 : p[1];
				n = p[1];

				memset(ap_cur->essid, 0, 256);
				memcpy(ap_cur->essid, p + 2, n);

				if (G.f_ivs != NULL && !ap_cur->essid_stored)
				{
					memset(&ivs2, '\x00', sizeof (struct ivs2_pkthdr));
					ivs2.flags |= IVS2_ESSID;
					ivs2.len += ap_cur->ssid_length;

					if (memcmp(G.prev_bssid, ap_cur->bssid, 6) != 0)
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy(G.prev_bssid, ap_cur->bssid, 6);
					}

					/* write header */
					if (fwrite(&ivs2, 1, sizeof (struct ivs2_pkthdr), G.f_ivs)
					    != (size_t) sizeof (struct ivs2_pkthdr))
					{
						perror("fwrite(IV header) failed");
						return ( 1);
					}

					/* write BSSID */
					if (ivs2.flags & IVS2_BSSID)
					{
						if (fwrite(ap_cur->bssid, 1, 6, G.f_ivs)
						    != (size_t) 6)
						{
							perror("fwrite(IV bssid) failed");
							return ( 1);
						}
					}

					/* write essid */
					if (fwrite(ap_cur->essid, 1, ap_cur->ssid_length, G.f_ivs)
					    != (size_t) ap_cur->ssid_length)
					{
						perror("fwrite(IV essid) failed");
						return ( 1);
					}

					ap_cur->essid_stored = 1;
				}

				for (i = 0; i < n; i++)
					if ((ap_cur->essid[i] > 0 && ap_cur->essid[i] < 32) ||
					    (ap_cur->essid[i] > 126 && ap_cur->essid[i] < 160))
						ap_cur->essid[i] = '.';
			}

			/* get the maximum speed in Mb and the AP's channel */

			if (p[0] == 0x01 || p[0] == 0x32)
			{
				if (ap_cur->max_speed < (p[1 + p[1]] & 0x7F) / 2)
					ap_cur->max_speed = (p[1 + p[1]] & 0x7F) / 2;
			}

			if (p[0] == 0x03)
				ap_cur->channel = p[2];

			p += 2 + p[1];
		}
	}

	/* packet parsing: Beacon & Probe response */

	if ((h80211[0] == 0x80 || h80211[0] == 0x50) && caplen > 38)
	{
		p = h80211 + 36; //ignore hdr + fixed params

		while (p < h80211 + caplen)
		{
			type = p[0];
			length = p[1];
			if (p + 2 + length > h80211 + caplen)
			{
				/*                printf("error parsing tags! %p vs. %p (tag: %i, length: %i,position: %i)\n", (p+2+length), (h80211+caplen), type, length, (p-h80211));
						exit(1);*/
				break;
			}

			if ((type == 0xDD && (length >= 8) && (memcmp(p + 2, "\x00\x50\xF2\x01\x01\x00", 6) == 0)) || (type == 0x30))
			{
				ap_cur->security &= ~(STD_WEP | ENC_WEP | STD_WPA);

				org_p = p;
				offset = 0;

				if (type == 0xDD)
				{
					//WPA defined in vendor specific tag -> WPA1 support
					ap_cur->security |= STD_WPA;
					offset = 4;
				}

				if (type == 0x30)
				{
					ap_cur->security |= STD_WPA2;
					offset = 0;
				}

				if (length < (18 + offset))
				{
					p += length + 2;
					continue;
				}

				if (p + 9 + offset > h80211 + caplen)
					break;
				numuni = p[8 + offset] + (p[9 + offset] << 8);

				if (p + (11 + offset) + 4 * numuni > h80211 + caplen)
					break;
				numauth = p[(10 + offset) + 4 * numuni] + (p[(11 + offset) + 4 * numuni] << 8);

				p += (10 + offset);

				if (type != 0x30)
				{
					if (p + (4 * numuni) + (2 + 4 * numauth) > h80211 + caplen)
						break;
				}
				else
				{
					if (p + (4 * numuni) + (2 + 4 * numauth) + 2 > h80211 + caplen)
						break;
				}

				for (i = 0; i < numuni; i++)
				{
					switch (p[i * 4 + 3])
					{
						case 0x01:
							ap_cur->security |= ENC_WEP;
							break;
						case 0x02:
							ap_cur->security |= ENC_TKIP;
							break;
						case 0x03:
							ap_cur->security |= ENC_WRAP;
							break;
						case 0x04:
							ap_cur->security |= ENC_CCMP;
							break;
						case 0x05:
							ap_cur->security |= ENC_WEP104;
							break;
						default:
							break;
					}
				}

				p += 2 + 4 * numuni;

				for (i = 0; i < numauth; i++)
				{
					switch (p[i * 4 + 3])
					{
						case 0x01:
							ap_cur->security |= AUTH_MGT;
							break;
						case 0x02:
							ap_cur->security |= AUTH_PSK;
							break;
						default:
							break;
					}
				}

				p += 2 + 4 * numauth;

				if (type == 0x30) p += 2;

				p = org_p + length + 2;
			}
			else if ((type == 0xDD && (length >= 8) && (memcmp(p + 2, "\x00\x50\xF2\x02\x01\x01", 6) == 0)))
			{
				ap_cur->security |= STD_QOS;
				p += length + 2;
			}
			else if ((type == 0xDD && (length >= 4) && (memcmp(p + 2, "\x00\x50\xF2\x04", 4) == 0)))
			{
				org_p = p;
				p += 6;
				int len = length, subtype = 0, sublen = 0;
				while (len >= 4)
				{
					subtype = (p[0] << 8) + p[1];
					sublen = (p[2] << 8) + p[3];
					if (sublen > len)
						break;
					switch (subtype)
					{
						case 0x104a: // WPS Version
							ap_cur->wps.version = p[4];
							break;
						case 0x1011: // Device Name
						case 0x1012: // Device Password ID
						case 0x1021: // Manufacturer
						case 0x1023: // Model
						case 0x1024: // Model Number
						case 0x103b: // Response Type
						case 0x103c: // RF Bands
						case 0x1041: // Selected Registrar
						case 0x1042: // Serial Number
							break;
						case 0x1044: // WPS State
							ap_cur->wps.state = p[4];
							break;
						case 0x1047: // UUID Enrollee
						case 0x1049: // Vendor Extension
						case 0x1054: // Primary Device Type
							break;
						case 0x1057: // AP Setup Locked
							ap_cur->wps.ap_setup_locked = p[4];
							break;
						case 0x1008: // Config Methods
						case 0x1053: // Selected Registrar Config Methods
							ap_cur->wps.meth = (p[4] << 8) + p[5];
							break;
						default: // Unknown type-length-value
							break;
					}
					p += sublen + 4;
					len -= sublen + 4;
				}
				p = org_p + length + 2;
			}
			else p += length + 2;
		}
	}

	/* packet parsing: Authentication Response */

	if (h80211[0] == 0xB0 && caplen >= 30)
	{
		if (ap_cur->security & STD_WEP)
		{
			//successful step 2 or 4 (coming from the AP)
			if (memcmp(h80211 + 28, "\x00\x00", 2) == 0 &&
			    (h80211[26] == 0x02 || h80211[26] == 0x04))
			{
				ap_cur->security &= ~(AUTH_OPN | AUTH_PSK | AUTH_MGT);
				if (h80211[24] == 0x00) ap_cur->security |= AUTH_OPN;
				if (h80211[24] == 0x01) ap_cur->security |= AUTH_PSK;
			}
		}
	}

	/* packet parsing: Association Request */

	if (h80211[0] == 0x00 && caplen > 28)
	{
		p = h80211 + 28;

		while (p < h80211 + caplen)
		{
			if (p + 2 + p[1] > h80211 + caplen)
				break;

			if (p[0] == 0x00 && p[1] > 0 && p[2] != '\0' &&
			    (p[1] > 1 || p[2] != ' '))
			{
				/* found a non-cloaked ESSID */

				n = (p[1] > 32) ? 32 : p[1];

				memset(ap_cur->essid, 0, 33);
				memcpy(ap_cur->essid, p + 2, n);

				if (G.f_ivs != NULL && !ap_cur->essid_stored)
				{
					memset(&ivs2, '\x00', sizeof (struct ivs2_pkthdr));
					ivs2.flags |= IVS2_ESSID;
					ivs2.len += ap_cur->ssid_length;

					if (memcmp(G.prev_bssid, ap_cur->bssid, 6) != 0)
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy(G.prev_bssid, ap_cur->bssid, 6);
					}

					/* write header */
					if (fwrite(&ivs2, 1, sizeof (struct ivs2_pkthdr), G.f_ivs)
					    != (size_t) sizeof (struct ivs2_pkthdr))
					{
						perror("fwrite(IV header) failed");
						return ( 1);
					}

					/* write BSSID */
					if (ivs2.flags & IVS2_BSSID)
					{
						if (fwrite(ap_cur->bssid, 1, 6, G.f_ivs)
						    != (size_t) 6)
						{
							perror("fwrite(IV bssid) failed");
							return ( 1);
						}
					}

					/* write essid */
					if (fwrite(ap_cur->essid, 1, ap_cur->ssid_length, G.f_ivs)
					    != (size_t) ap_cur->ssid_length)
					{
						perror("fwrite(IV essid) failed");
						return ( 1);
					}

					ap_cur->essid_stored = 1;
				}

				for (i = 0; i < n; i++)
					if (ap_cur->essid[i] < 32 ||
					    (ap_cur->essid[i] > 126 && ap_cur->essid[i] < 160))
						ap_cur->essid[i] = '.';
			}

			p += 2 + p[1];
		}
		if (st_cur != NULL)
			st_cur->wpa.state = 0;
	}

	/* packet parsing: some data */

	if ((h80211[0] & 0x0C) == 0x08)
	{
		/* update the channel if we didn't get any beacon */

		if (ap_cur->channel == -1)
		{
			if (ri->ri_channel > 0 && ri->ri_channel <= HIGHEST_CHANNEL)
				ap_cur->channel = ri->ri_channel;
			else
				ap_cur->channel = G.channel[cardnum];
		}

		/* check the SNAP header to see if data is encrypted */

		z = ((h80211[1] & 3) != 3) ? 24 : 30;

		/* Check if 802.11e (QoS) */
		if ((h80211[0] & 0x80) == 0x80)
		{
			z += 2;
			if (st_cur != NULL)
			{
				if ((h80211[1] & 3) == 1) //ToDS
					st_cur->qos_to_ds = 1;
				else
					st_cur->qos_fr_ds = 1;
			}
		}
		else
		{
			if (st_cur != NULL)
			{
				if ((h80211[1] & 3) == 1) //ToDS
					st_cur->qos_to_ds = 0;
				else
					st_cur->qos_fr_ds = 0;
			}
		}

		if (z == 24)
		{
			if (list_check_decloak(&(ap_cur->packets), caplen, h80211) != 0)
			{
				list_add_packet(&(ap_cur->packets), caplen, h80211);
			}
			else
			{
				ap_cur->is_decloak = 1;
				ap_cur->decloak_detect = 0;
				list_tail_free(&(ap_cur->packets));
				memset(G.message, '\x00', sizeof (G.message));
				snprintf(G.message, sizeof ( G.message) - 1,
					 "][ Decloak: %02X:%02X:%02X:%02X:%02X:%02X ",
					 ap_cur->bssid[0], ap_cur->bssid[1], ap_cur->bssid[2],
					 ap_cur->bssid[3], ap_cur->bssid[4], ap_cur->bssid[5]);
			}
		}

		if (z + 26 > (unsigned) caplen)
			goto write_packet;

		if (h80211[z] == h80211[z + 1] && h80211[z + 2] == 0x03)
		{
			//            if( ap_cur->encryption < 0 )
			//                ap_cur->encryption = 0;

			/* if ethertype == IPv4, find the LAN address */

			if (h80211[z + 6] == 0x08 && h80211[z + 7] == 0x00 &&
			    (h80211[1] & 3) == 0x01)
				memcpy(ap_cur->lanip, &h80211[z + 20], 4);

			if (h80211[z + 6] == 0x08 && h80211[z + 7] == 0x06)
				memcpy(ap_cur->lanip, &h80211[z + 22], 4);
		}
		//        else
		//            ap_cur->encryption = 2 + ( ( h80211[z + 3] & 0x20 ) >> 5 );


		if (ap_cur->security == 0 || (ap_cur->security & STD_WEP))
		{
			if ((h80211[1] & 0x40) != 0x40)
			{
				ap_cur->security |= STD_OPN;
			}
			else
			{
				if ((h80211[z + 3] & 0x20) == 0x20)
				{
					ap_cur->security |= STD_WPA;
				}
				else
				{
					ap_cur->security |= STD_WEP;
					if ((h80211[z + 3] & 0xC0) != 0x00)
					{
						ap_cur->security |= ENC_WEP40;
					}
					else
					{
						ap_cur->security &= ~ENC_WEP40;
						ap_cur->security |= ENC_WEP;
					}
				}
			}
		}

		if (z + 10 > (unsigned) caplen)
			goto write_packet;

		if (ap_cur->security & STD_WEP)
		{
			/* WEP: check if we've already seen this IV */

			if (!uniqueiv_check(ap_cur->uiv_root, &h80211[z]))
			{
				/* first time seen IVs */

				if (G.f_ivs != NULL)
				{
					memset(&ivs2, '\x00', sizeof (struct ivs2_pkthdr));
					ivs2.flags = 0;
					ivs2.len = 0;

					/* datalen = caplen - (header+iv+ivs) */
					dlen = caplen - z - 4 - 4; //original data len
					if (dlen > 2048) dlen = 2048;
					//get cleartext + len + 4(iv+idx)
					num_xor = known_clear(clear, &clen, weight, h80211, dlen);
					if (num_xor == 1)
					{
						ivs2.flags |= IVS2_XOR;
						ivs2.len += clen + 4;
						/* reveal keystream (plain^encrypted) */
						for (n = 0; n < (ivs2.len - 4); n++)
						{
							clear[n] = (clear[n] ^ h80211[z + 4 + n]) & 0xFF;
						}
						//clear is now the keystream
					}
					else
					{
						//do it again to get it 2 bytes higher
						num_xor = known_clear(clear + 2, &clen, weight, h80211, dlen);
						ivs2.flags |= IVS2_PTW;
						//len = 4(iv+idx) + 1(num of keystreams) + 1(len per keystream) + 32*num_xor + 16*sizeof(int)(weight[16])
						ivs2.len += 4 + 1 + 1 + 32 * num_xor + 16 * sizeof (int);
						clear[0] = num_xor;
						clear[1] = clen;
						/* reveal keystream (plain^encrypted) */
						for (o = 0; o < num_xor; o++)
						{
							for (n = 0; n < (ivs2.len - 4); n++)
							{
								clear[2 + n + o * 32] = (clear[2 + n + o * 32] ^ h80211[z + 4 + n]) & 0xFF;
							}
						}
						memcpy(clear + 4 + 1 + 1 + 32 * num_xor, weight, 16 * sizeof (int));
						//clear is now the keystream
					}

					if (memcmp(G.prev_bssid, ap_cur->bssid, 6) != 0)
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy(G.prev_bssid, ap_cur->bssid, 6);
					}

					if (fwrite(&ivs2, 1, sizeof (struct ivs2_pkthdr), G.f_ivs)
					    != (size_t) sizeof (struct ivs2_pkthdr))
					{
						perror("fwrite(IV header) failed");
						return ( 1);
					}

					if (ivs2.flags & IVS2_BSSID)
					{
						if (fwrite(ap_cur->bssid, 1, 6, G.f_ivs) != (size_t) 6)
						{
							perror("fwrite(IV bssid) failed");
							return ( 1);
						}
						ivs2.len -= 6;
					}

					if (fwrite(h80211 + z, 1, 4, G.f_ivs) != (size_t) 4)
					{
						perror("fwrite(IV iv+idx) failed");
						return ( 1);
					}
					ivs2.len -= 4;

					if (fwrite(clear, 1, ivs2.len, G.f_ivs) != (size_t) ivs2.len)
					{
						perror("fwrite(IV keystream) failed");
						return ( 1);
					}
				}

				uniqueiv_mark(ap_cur->uiv_root, &h80211[z]);

				ap_cur->nb_data++;
			}

			// Record all data linked to IV to detect WEP Cloaking
			if (G.f_ivs == NULL && G.detect_anomaly)
			{
				// Only allocate this when seeing WEP AP
				if (ap_cur->data_root == NULL)
					ap_cur->data_root = data_init();

				// Only works with full capture, not IV-only captures
				if (data_check(ap_cur->data_root, &h80211[z], &h80211[z + 4])
				== CLOAKING && ap_cur->EAP_detected == 0)
				{

					//If no EAP/EAP was detected, indicate WEP cloaking
					memset(G.message, '\x00', sizeof (G.message));
					snprintf(G.message, sizeof ( G.message) - 1,
						 "][ WEP Cloaking: %02X:%02X:%02X:%02X:%02X:%02X ",
						 ap_cur->bssid[0], ap_cur->bssid[1], ap_cur->bssid[2],
						 ap_cur->bssid[3], ap_cur->bssid[4], ap_cur->bssid[5]);

				}
			}

		}
		else
		{
			ap_cur->nb_data++;
		}

		z = ((h80211[1] & 3) != 3) ? 24 : 30;

		/* Check if 802.11e (QoS) */
		if ((h80211[0] & 0x80) == 0x80) z += 2;

		if (z + 26 > (unsigned) caplen)
			goto write_packet;

		z += 6; //skip LLC header

		/* check ethertype == EAPOL */
		if (h80211[z] == 0x88 && h80211[z + 1] == 0x8E && (h80211[1] & 0x40) != 0x40)
		{
			ap_cur->EAP_detected = 1;

			z += 2; //skip ethertype

			if (st_cur == NULL)
				goto write_packet;

			/* frame 1: Pairwise == 1, Install == 0, Ack == 1, MIC == 0 */

			if ((h80211[z + 6] & 0x08) != 0 &&
			    (h80211[z + 6] & 0x40) == 0 &&
			    (h80211[z + 6] & 0x80) != 0 &&
			    (h80211[z + 5] & 0x01) == 0)
			{
				memcpy(st_cur->wpa.anonce, &h80211[z + 17], 32);
				st_cur->wpa.state = 1;
			}


			/* frame 2 or 4: Pairwise == 1, Install == 0, Ack == 0, MIC == 1 */

			if (z + 17 + 32 > (unsigned) caplen)
				goto write_packet;

			if ((h80211[z + 6] & 0x08) != 0 &&
			    (h80211[z + 6] & 0x40) == 0 &&
			    (h80211[z + 6] & 0x80) == 0 &&
			    (h80211[z + 5] & 0x01) != 0)
			{
				if (memcmp(&h80211[z + 17], ZERO, 32) != 0)
				{
					memcpy(st_cur->wpa.snonce, &h80211[z + 17], 32);
					st_cur->wpa.state |= 2;

				}

				if ((st_cur->wpa.state & 4) != 4)
				{
					st_cur->wpa.eapol_size = (h80211[z + 2] << 8)
						+ h80211[z + 3] + 4;

					if (caplen - z < st_cur->wpa.eapol_size || st_cur->wpa.eapol_size == 0 ||
					    caplen - z < 81 + 16 || st_cur->wpa.eapol_size > sizeof (st_cur->wpa.eapol))
					{
						// Ignore the packet trying to crash us.
						st_cur->wpa.eapol_size = 0;
						goto write_packet;
					}

					memcpy(st_cur->wpa.keymic, &h80211[z + 81], 16);
					memcpy(st_cur->wpa.eapol, &h80211[z], st_cur->wpa.eapol_size);
					memset(st_cur->wpa.eapol + 81, 0, 16);
					st_cur->wpa.state |= 4;
					st_cur->wpa.keyver = h80211[z + 6] & 7;
				}
			}

			/* frame 3: Pairwise == 1, Install == 1, Ack == 1, MIC == 1 */

			if ((h80211[z + 6] & 0x08) != 0 &&
			    (h80211[z + 6] & 0x40) != 0 &&
			    (h80211[z + 6] & 0x80) != 0 &&
			    (h80211[z + 5] & 0x01) != 0)
			{
				if (memcmp(&h80211[z + 17], ZERO, 32) != 0)
				{
					memcpy(st_cur->wpa.anonce, &h80211[z + 17], 32);
					st_cur->wpa.state |= 1;
				}

				if ((st_cur->wpa.state & 4) != 4)
				{
					st_cur->wpa.eapol_size = (h80211[z + 2] << 8)
						+ h80211[z + 3] + 4;

					if (caplen - (unsigned) z < st_cur->wpa.eapol_size || st_cur->wpa.eapol_size == 0 ||
					    caplen - (unsigned) z < 81 + 16 || st_cur->wpa.eapol_size > sizeof (st_cur->wpa.eapol))
					{
						// Ignore the packet trying to crash us.
						st_cur->wpa.eapol_size = 0;
						goto write_packet;
					}

					memcpy(st_cur->wpa.keymic, &h80211[z + 81], 16);
					memcpy(st_cur->wpa.eapol, &h80211[z], st_cur->wpa.eapol_size);
					memset(st_cur->wpa.eapol + 81, 0, 16);
					st_cur->wpa.state |= 4;
					st_cur->wpa.keyver = h80211[z + 6] & 7;
				}
			}

			if (st_cur->wpa.state == 7)
			{
				memcpy(st_cur->wpa.stmac, st_cur->stmac, 6);
				memcpy(G.wpa_bssid, ap_cur->bssid, 6);
				memset(G.message, '\x00', sizeof (G.message));
				snprintf(G.message, sizeof ( G.message) - 1,
					 "][ WPA handshake: %02X:%02X:%02X:%02X:%02X:%02X ",
					 G.wpa_bssid[0], G.wpa_bssid[1], G.wpa_bssid[2],
					 G.wpa_bssid[3], G.wpa_bssid[4], G.wpa_bssid[5]);


				if (G.f_ivs != NULL)
				{
					memset(&ivs2, '\x00', sizeof (struct ivs2_pkthdr));
					ivs2.flags = 0;
					ivs2.len = 0;

					ivs2.len = sizeof (struct WPA_hdsk);
					ivs2.flags |= IVS2_WPA;

					if (memcmp(G.prev_bssid, ap_cur->bssid, 6) != 0)
					{
						ivs2.flags |= IVS2_BSSID;
						ivs2.len += 6;
						memcpy(G.prev_bssid, ap_cur->bssid, 6);
					}

					if (fwrite(&ivs2, 1, sizeof (struct ivs2_pkthdr), G.f_ivs)
					    != (size_t) sizeof (struct ivs2_pkthdr))
					{
						perror("fwrite(IV header) failed");
						return ( 1);
					}

					if (ivs2.flags & IVS2_BSSID)
					{
						if (fwrite(ap_cur->bssid, 1, 6, G.f_ivs) != (size_t) 6)
						{
							perror("fwrite(IV bssid) failed");
							return ( 1);
						}
						ivs2.len -= 6;
					}

					if (fwrite(&(st_cur->wpa), 1, sizeof (struct WPA_hdsk), G.f_ivs) != (size_t) sizeof (struct WPA_hdsk))
					{
						perror("fwrite(IV wpa_hdsk) failed");
						return ( 1);
					}
				}
			}
		}
	}


write_packet:

	if (ap_cur != NULL)
	{
		if (h80211[0] == 0x80 && G.one_beacon)
		{
			if (!ap_cur->beacon_logged)
				ap_cur->beacon_logged = 1;
			else return ( 0);
		}
	}

	if (G.record_data)
	{
		if (((h80211[0] & 0x0C) == 0x00) && ((h80211[0] & 0xF0) == 0xB0))
		{
			/* authentication packet */
			check_shared_key(h80211, caplen);
		}
	}

	if (ap_cur != NULL)
	{
		if (ap_cur->security != 0 && G.f_encrypt != 0 && ((ap_cur->security & G.f_encrypt) == 0))
		{
			return (1);
		}

		if (is_filtered_essid(ap_cur->essid))
		{
			return (1);
		}

	}

	/* this changes the local ap_cur, st_cur and na_cur variables and should be the last check befor the actual write */
	if (caplen < 24 && caplen >= 10 && h80211[0])
	{
		/* RTS || CTS || ACK || CF-END || CF-END&CF-ACK*/
		//(h80211[0] == 0xB4 || h80211[0] == 0xC4 || h80211[0] == 0xD4 || h80211[0] == 0xE4 || h80211[0] == 0xF4)

		/* use general control frame detection, as the structure is always the same: mac(s) starting at [4] */
		if (h80211[0] & 0x04)
		{
			p = h80211 + 4;
			while (p <= h80211 + 16 && p <= h80211 + caplen)
			{
				memcpy(namac, p, 6);

				if (memcmp(namac, NULL_MAC, 6) == 0)
				{
					p += 6;
					continue;
				}

				if (memcmp(namac, BROADCAST, 6) == 0)
				{
					p += 6;
					continue;
				}

				if (G.hide_known)
				{
					/* check AP list */
					ap_cur = G.ap_1st;
					ap_prv = NULL;

					while (ap_cur != NULL)
					{
						if (!memcmp(ap_cur->bssid, namac, 6))
							break;

						ap_prv = ap_cur;
						ap_cur = ap_cur->next;
					}

					/* if it's an AP, try next mac */

					if (ap_cur != NULL)
					{
						p += 6;
						continue;
					}

					/* check ST list */
					st_cur = G.st_1st;
					st_prv = NULL;

					while (st_cur != NULL)
					{
						if (!memcmp(st_cur->stmac, namac, 6))
							break;

						st_prv = st_cur;
						st_cur = st_cur->next;
					}

					/* if it's a client, try next mac */

					if (st_cur != NULL)
					{
						p += 6;
						continue;
					}
				}

				/* not found in either AP list or ST list, look through NA list */
				na_cur = G.na_1st;
				na_prv = NULL;

				while (na_cur != NULL)
				{
					if (!memcmp(na_cur->namac, namac, 6))
						break;

					na_prv = na_cur;
					na_cur = na_cur->next;
				}

				/* update our chained list of unknown stations */
				/* if it's a new mac, add it */

				if (na_cur == NULL)
				{
					if (!(na_cur = (struct NA_info *) malloc(
										 sizeof ( struct NA_info))))
					{
						perror("malloc failed");
						return ( 1);
					}

					memset(na_cur, 0, sizeof ( struct NA_info));

					if (G.na_1st == NULL)
						G.na_1st = na_cur;
					else
						na_prv->next = na_cur;

					memcpy(na_cur->namac, namac, 6);

					na_cur->prev = na_prv;

					gettimeofday(&(na_cur->tv), NULL);
					na_cur->tinit = time(NULL);
					na_cur->tlast = time(NULL);

					na_cur->power = -1;
					na_cur->channel = -1;
					na_cur->ack = 0;
					na_cur->ack_old = 0;
					na_cur->ackps = 0;
					na_cur->cts = 0;
					na_cur->rts_r = 0;
					na_cur->rts_t = 0;
				}

				/* update the last time seen & power*/

				na_cur->tlast = time(NULL);
				na_cur->power = ri->ri_power;
				na_cur->channel = ri->ri_channel;

				switch (h80211[0] & 0xF0)
				{
					case 0xB0:
						if (p == h80211 + 4)
							na_cur->rts_r++;
						if (p == h80211 + 10)
							na_cur->rts_t++;
						break;

					case 0xC0:
						na_cur->cts++;
						break;

					case 0xD0:
						na_cur->ack++;
						break;

					default:
						na_cur->other++;
						break;
				}

				/*grab next mac (for rts frames)*/
				p += 6;
			}
		}
	}

	if (G.f_cap != NULL && caplen >= 10)
	{
		pkh.caplen = pkh.len = caplen;

		gettimeofday(&tv, NULL);

		pkh.tv_sec = tv.tv_sec;
		pkh.tv_usec = (tv.tv_usec & ~0x1ff) + ri->ri_power + 64;

		n = sizeof ( pkh);

		if (fwrite(&pkh, 1, n, G.f_cap) != (size_t) n)
		{
			perror("fwrite(packet header) failed");
			return ( 1);
		}

		fflush(stdout);

		n = pkh.caplen;

		if (fwrite(h80211, 1, n, G.f_cap) != (size_t) n)
		{
			perror("fwrite(packet data) failed");
			return ( 1);
		}

		fflush(stdout);
	}

	return ( 0);
}


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

	{ // init
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