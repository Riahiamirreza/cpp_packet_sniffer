#include "include/setconf.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;


Logger lg;
Config conf; 
std::vector<Session> ses;
 
/*
 *
 * This function will be call automatically
 * by pcap_loop funcion to handle every packet
 *
 */

void handler(u_char *data, const struct pcap_pkthdr* pkthdr, const u_char *packet){


	Packet pk(data, pkthdr, packet, conf.directory, conf.loglevel, conf.all_json_data); 
	
	
	if(pk.transport_protocol == "tcp" || pk.transport_protocol == "udp")
		lg.log("length: " + std::to_string(pk.length) + " " + pk.network_protocol \
				+ " " + pk.transport_protocol \
				+ " src_ip:" + pk.show_src_ip(true) \
				+ " dst_ip:" + pk.show_dst_ip(true) \
				+ " " + pk.packet_data_keys[0] +" :"\
				+ std::to_string(pk.to_int(pk.packet_data_values[0])) \
				+ " " + pk.packet_data_keys[1] +" :"+ std::to_string(pk.to_int(pk.packet_data_values[1])), 3);
	
	else if(pk.transport_protocol == "icmp")
		lg.log(pk.network_protocol \
				+ " " + pk.transport_protocol \
				+ " src_ip:" + pk.show_src_ip(true) \
				+ " dst_ip:" + pk.show_dst_ip(true) ,3);
	
	/*
	if(pk.transport_protocol == "tcp" || pk.transport_protocol == "udp"){

		int flag = 1;
		for(int i=0; i<ses.size(); i++){
			
			if(ses[i].is_valid(pk)){
				ses[i].add_packet(pk);
				flag = 0;
				lg.log(ses[i].show(), 3);
				break;
			}
		}

		if(flag){

			Session session_(pk, conf.loglevel);
			ses.push_back(session_);
			//lg.log(session_.show() + std::to_string(ses.size()), 3);
		}

	}

	*/
	/*
	if(pk.transport_protocol == "tcp" || pk.transport_protocol == "udp")
		lg.log(std::to_string(pk.to_int(pk.getItem("dst_port"))),3);
	*/
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


	conf.set_config();
	conf.set_json_data();
	lg.set_log_level(conf.loglevel);
	
	/*
	for(int i=0; i<conf.all_json_data.size(); i++){
		for(int j=0; j<conf.all_json_data[i].size(); j++)
			lg.log(conf.all_json_data[i][j], 3);
	}
	*/

	loop_(atoi(argv[2]), argv[1]);
	return 0;

}

