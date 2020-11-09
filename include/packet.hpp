#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pcap.h>
#include <ctime> 
#include <stdint.h> 
#include "process.hpp"


using namespace std;

class Packet{


	public :
		
		Process proc;
		Logger lg;           // declreration of Logger object.  
		int length;          // length of packet.
		time_t ptime;        // the time which packet was captured.
		string network_protocol;
		string transport_protocol;
		string application_protocol;
		int src_ip[4];
		int dst_ip[4];
		int loglevel;

		Packet(u_char *data, const struct pcap_pkthdr* pkthdr, const u_char *packet){

			//loglevel = log_sev;
			loglevel = proc.packet_loglevel;
			int byte = 0;
			int limit = pkthdr->len;
			const u_char *ptr = packet;
			char packet_content[5000];


			//assigning pointer value to an attribute of this object.
			while(byte++ < limit){
				packet_content[byte-1] = *ptr;
				ptr++;
			}
			
			packet_ptr = packet_content; 
			length = pkthdr->len;
			ptime = time(NULL);

			network_protocol   = check_protocol("network");
			//lg.log( check_transport_protocol(), loglevel);
			//set_eth_protocol(eth_address);

			
			//if protocol is IPv4 we define transport layer protocol and ip address.
			if(network_protocol == "IPv4"){
				transport_protocol = check_protocol("transport"); 
				set_ip_address();
			}

		}

		/*
		 * this class will return the 
		 * specified byte.
		 * for example:
		 * show_byte(1);
		 * will return byte with index 0;
		 */

		char show_byte(int byte){
			
			return (unsigned char) packet_ptr[byte];
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

			return strbytes;
		}


		/*
		 * this function will find the protocol 
		 * of packet in the given layer.
		 * for example:
		 * check_protocol("transport");
		 * will return the transport protocol as string.
		 */

		string check_protocol(std::string layer){
			
			int fin = 1;
			proc.init(layer);
        		for(int i=0; i<proc.lof.size(); i++){
		                proc.next_file();
                		for(int j=0; j< proc.len; j++){
		                        proc.next_step();
		                        std::vector<int> k = proc.get_keys();
                		        std::string v = proc.get_value();
					if(check_kv(k,v) == false){
						fin = 0;
						break;
					}else{
						fin = 1;
					}

                		}
				
				if(fin){
					lg.log("Packet:check_protocol:return "+ proc.file_name, loglevel);
					return proc.file_name;
				}
        		}

			lg.log("Packet::check_protocol:return NULL value " + std::to_string(fin) , loglevel);
			return "NULL";	
		}

		
		/*
		 *
		 * this function will return size of 
		 * IPv4 packets based on it's bytes
		 *
	         */

		string get_size_IPv4(){

		        //lg.log("main:handler: 16-17: " + to_string(256*(pk.show_byte(16) & 0xff) + (pk.show_byte(17) & 0xff) + 14), log_level);
		        return to_string(256*(show_byte(16) & 0xff) + (show_byte(17) & 0xff) + 14);

		}

		/* 
		 * this function will check the given 
		 * key,value and return true if the key value
		 * are set, and false if not.
		 */

		bool check_kv(std::vector<int> keys, std::string value){
			

			lg.log("Packet::check_kv:keys: " + show_bytes(keys[0],keys[1])+ " VALUE:" + value , loglevel);
			
			if(value.find("~pklen") != string::npos){

				value = get_size_IPv4();
				if(value == to_string(length))
					return true;
				else
					return false;
			}

			if(show_bytes(keys[0],keys[1]) == value)

				return true;
			else
				return false;
		}
	

		/*
		 * this function will convert the
		 * src_ip to string and returns it.
		 */

		string show_src_ip(bool color = false){
			
			string ip = to_string(src_ip[0]) + "." + to_string(src_ip[1]) + "." + to_string(src_ip[2]) + "." + to_string(src_ip[3]);
			if(color)
				ip = "\033[032;7m" + ip + "\033[0m";

			return ip;
		}

		/*
		 * this function will convert the
		 * src_ip to string and returns it.
		 */

		string show_dst_ip(bool color = false){
			
			string ip = to_string(dst_ip[0]) + "." + to_string(dst_ip[1]) + "." + to_string(dst_ip[2]) + "." + to_string(dst_ip[3]);
			if(color)
				ip = "\033[032;7m" + ip + "\033[0m";

			return ip;
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

		
		/*
		 * this function will assign the 
		 * PACKET ip to the variabls.
		 */

		void set_ip_address(){
			
			if( network_protocol != "IPv4")
				return ;

			for(int i=0; i<4; i++){
				string byte = show_bytes(i+26,i+27);
				src_ip[i] = stoi(byte,nullptr,16);

			}

			for(int i=0; i<4; i++){
				string byte = show_bytes(i+30,i+31);
				dst_ip[i] = stoi(byte,nullptr,16);

			}
		}
};



