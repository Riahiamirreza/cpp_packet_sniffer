#include "include/packet.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;


static int log_level;
Logger lg;
 
 
/*
 *
 * This function will be call automatically
 * by pcap_loop funcion to handle every packet
 *
 */

void handler(u_char *data, const struct pcap_pkthdr* pkthdr, const u_char *packet){

	lg.log("main:handler: A NEW PACKET!", log_level);
	Packet pk(data, pkthdr, packet); 
	lg.log("main:handler: network   protocol " + pk.network_protocol + " code: " + pk.show_bytes(12,14), log_level);
	if(pk.network_protocol == "IPv4"){
		
		lg.log("main:handler: transport protocol " + pk.transport_protocol + " code: " + pk.show_bytes(23,24), log_level);
		lg.log("main:handler: source ip      :" + pk.show_src_ip(true), log_level );
		lg.log("main:handler: destination ip :" + pk.show_dst_ip(true), log_level );
	}

	
	
}


/*
 *
 * this function create loop on network traffic.
 * loop(100, bluetooth-monitor );
 * this will create a 100 rounded loop 
 * on device "Bluetooth-monitor"
 *
 */

int loop_(int rounds, char *dev ){

	pcap_t *descr;
	char err[PCAP_ERRBUF_SIZE];
	descr = pcap_open_live(dev, BUFSIZ, 0, -1, err);
	if(descr == NULL){

		lg.log(err,2);
		return 1;
	}	

	pcap_loop(descr, rounds, handler, NULL);
	return 0;
	
}

int main(int argc, char *argv[]){

	if(argc > 3)
		log_level = atoi(argv[3]);
	else
		log_level = 3;


	loop_(atoi(argv[2]), argv[1]);
	return 0;

}

