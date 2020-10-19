#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pcap.h>
#include <ctime> 
#include "logging.hpp"


using namespace std;


string dir = "protocol_detection/eth_type/";
static int log_level;
Logger lg;
 

class Packet{

	public :
		int length;          // length of packet.
		time_t ptime;        // the time which packet was captured.
		string network_protocol;
		string transport_protocol;
		string application_protocol;
		string eth_dir;

		Packet(u_char *data, const struct pcap_pkthdr* pkthdr, const u_char *packet, string eth_address){

			int byte = 0;
			int limit = pkthdr->len;
			const u_char *ptr = packet;
			char packet_content[5000];
			while(byte++ < limit){
				packet_content[byte-1] = *ptr;
				ptr++;
			}
			
			packet_ptr = packet_content; 
			length = pkthdr->len;
			ptime = time(NULL);
			eth_dir = eth_address;
			set_eth_protocol(eth_address);

			//cout << "a packet captured!" << endl;
		}

		
		void show_byte(int byte){
			
			printf("%x\n", (unsigned char) packet_ptr[byte]);
		}



		/*
		 * this function will return
		 * the value stored in a
		 * sequence of bytes in packet.
		 * show_bytes(5, 8);
		 * will return value of 5,6 and 7th bytes of 
		 * packet a string.
		 *
		 */

		string show_bytes(int begin, int end){
			
			int len = end - begin;
			char bytes[len];
			string strbytes = "";

			for(int i=begin; i<end; i++){
				char byte = (unsigned char) packet_ptr[i];
				sprintf(bytes,"%x", byte & 0xff);
				//cout << bytes;
				strbytes = strbytes + bytes;

			}

			//string strbytes = convertToString(bytes, len);
			return strbytes;
		}


		/*
		 * this function get the address of file 
		 * and find the corresponded file with the
		 * address.
		 * the data in file is the protocl of the packet.
		 * It will return the data.
		 *
		 */

		string eth_protocol(string address){

			string eth_type;
			ifstream f;
			f.open(address);
			if(f){
				f >> eth_type;
				return eth_type;
			}else{
				return "-1";
			}

		}

		/*
		 * this function 
		 * assign output of the
		 * "eth_protocol" to the
		 * "network_protocol" attribute
		 */

		void set_eth_protocol(string address){
			
			address = address + show_bytes(12,14);
			network_protocol = eth_protocol(address);

		}

	private:

		char *packet_ptr;

		/*
		 *
		 * this function get a serie of characters
		 * and  size, then return the string 
		 * type of them.
		 *
		 */
		string convertToString(char *a, int size){
			
			string s = "";
			for(int i=0; i< size; i++)
				s = s + a[i];
			return s;
		}
};



/*
 *
 * This function will be call automatically
 * by pcap_loop funcion to handle every packet
 *
 */

void handler(u_char *data, const struct pcap_pkthdr* pkthdr, const u_char *packet){

	Packet pk(data, pkthdr, packet, dir); 
	//cout << pk.network_protocol << endl;
	string msg = pk.network_protocol + " | " + pk.show_bytes(12,14);
	lg.log( msg, log_level);
	

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

	if(argc > 2)
		log_level = atoi(argv[3]);
	else
		log_level = 3;


	loop_(atoi(argv[2]), argv[1]);
	return 0;

}

