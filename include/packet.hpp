#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pcap.h>
#include <ctime> 
#include <regex>
#include <stdint.h> 
#include "process.hpp"


//using namespace std;

class Packet{

/*

 * This class will manage and
 * manipulate a packet data.
 * Every new packet will be a 
 * new object of Packet class.
 * And it's data will extract and
 * store in it's variables.

*/


	
	public :
		
		Process proc;
		Logger lg;           // declreration of Logger object.  
		int length;          // length of packet.
		time_t ptime;        // the time which packet was captured.
		std::string network_protocol;
		std::string transport_protocol;
		std::string application_protocol;
		std::vector<std::string> packet_data_keys;
		std::vector<std::string> packet_data_values;
		int src_ip[4];
		int dst_ip[4];
		int loglevel;

		Packet(u_char *data, const struct pcap_pkthdr* pkthdr, 
				const u_char *packet, 
				std::string dir_address, int loglevel_,
			       	std::vector<std::vector<std::string>> all_data){

			loglevel = loglevel_;
			lg.set_log_level(loglevel);
			lg.log("Packet:Packet:", 5);
			int byte = 0;
			int limit = pkthdr->len;
			const u_char *ptr = packet;
			char packet_content[5000];


			proc.setconf(dir_address,loglevel_, all_data);
			//assigning pointer value to an attribute of this object.
			while(byte++ < limit){
				packet_content[byte-1] = *ptr;
				ptr++;
			}
			
			packet_ptr = packet_content; 
			length = pkthdr->len;
			ptime = time(NULL);

			network_protocol   = check_protocol("network");

			
			//if protocol is IPv4 we define transport layer protocol and ip address.
			if(network_protocol == "IPv4"){

				transport_protocol = check_protocol("transport"); 

				/*
				if(transport_protocol == "tcp")
					application_protocol = check_protocol("application");
				*/

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

		std::string show_bytes(int begin, int end){
			
			int len = end - begin;
			char bytes[len];
			std::string strbytes = "";

			for(int i=begin; i<end; i++){
				char byte = (unsigned char) packet_ptr[i];
				sprintf(bytes,"%x", byte & 0xff);
				strbytes = strbytes + bytes;

			}

			lg.log("Packet:show_bytes:strbytes:"+strbytes, 5);
			return strbytes;
		}


		/*
		 * this function will find the protocol 
		 * of packet in the given layer.
		 * for example:
		 * check_protocol("transport");
		 * will return the transport protocol as string.
		 */

		std::string check_protocol(std::string layer){
			
			int fin = 1;
			proc.init(layer);
        		for(int i=0; i<proc.lof.size(); i++){
		                proc.next_file();
                		for(int j=0; j< proc.len; j++){
		                        proc.next_step();
					lg.log("Packet:check_protocol:len:"+std::to_string(proc.len),4);
		                        std::vector<int> k = proc.get_keys();
                		        std::string v = proc.get_value();
					if(check_kv(k,v) == false){
						fin = 0;
						lg.log("Packet:check_protocol:false " + proc.file_name,4);
						break;
					}else{
						lg.log("Packet:check_protocol:true " + proc.file_name ,4);
						fin = 1;
					}

                		}
				
				if(fin){
					lg.log("Packet:check_protocol:return "+ proc.file_name, 4);
					return proc.file_name;
				}
        		}

			lg.log("Packet::check_protocol:return NULL value " + std::to_string(fin), 4);
			return "NULL";	
		}

		
		/*
		 *
		 * this function will return size of 
		 * IPv4 packets based on it's bytes
		 *
	         */

		std::string get_size_IPv4(){

		        return std::to_string(256*(show_byte(16) & 0xff) + (show_byte(17) & 0xff) + 14);

		}

		/* 
		 * this function will check the given 
		 * key,value and return true if the key value
		 * are set, and false if not.
		 */

		bool check_kv(std::vector<int> keys, std::string value){
			

			lg.log("Packet::check_kv:keys: " + show_bytes(keys[0],keys[1])+ " VALUE:" + value , 4);
			
			if(value.find("~~") != std::string::npos){
			
				packet_struct(keys,value);
				return true;
			
			}
				
			if(value.find("~pklen") != std::string::npos){

				value = get_size_IPv4();
				if(value == std::to_string(length))
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
		 * this function will append
		 * all of packet data into packet_data_values
		 * and their keys in packet_data_keys
		 */

		void packet_struct(std::vector<int> key, std::string value){
		
			packet_data_keys.push_back(value.substr(2,value.size()));
			packet_data_values.push_back(show_bytes(key[0],key[1]));

		}

		int to_int(std::string str, int step = 0){

			int ret;
			std::stringstream ss; 
			ss << std::hex << str;
			ss >> ret;
			return static_cast<int>(ret);


		}

		/*
		 * this function will convert the
		 * src_ip to string and returns it.
		 */

		std::string show_src_ip(bool color = false){
			
			std::string ip = std::to_string(src_ip[0]) + "." + std::to_string(src_ip[1]) + "." + std::to_string(src_ip[2]) + "." + std::to_string(src_ip[3]);
			if(color)
				ip = "\033[032;7m" + ip + "\033[0m";

			return ip;
		}

		/*
		 * this function will convert the
		 * src_ip to string and returns it.
		 */

		std::string show_dst_ip(bool color = false){
			
			std::string ip = std::to_string(dst_ip[0]) + "." + std::to_string(dst_ip[1]) + "." + std::to_string(dst_ip[2]) + "." + std::to_string(dst_ip[3]);
			if(color)
				ip = "\033[032;7m" + ip + "\033[0m";

			return ip;
		}

		std::string getItem(std::string item){

			return packet_data_values[getIndex(packet_data_keys,item)];
		
		}

	private:

		char *packet_ptr;
		
		/*
		bool parse(std::vector<int> key, std::string value){
			
			std::vector<std::string> vallist = split(value, "~or");
			int flag = 0;
			lg.log("Packet:parse: string: " + show_bytes(key[0],key[1]) + " keys: " + std::to_string(key[0],),4);

			for(int i=0; i<vallist.size(); i++)

				if(hex2string(show_bytes(key[0],key[1])) == vallist[i])
					return true;	

			return false;

		}
		
		std::vector<std::string> split(const std::string str, const std::string regex_str){

			    std::regex regexz(regex_str);
			    std::vector<std::string> list(std::sregex_token_iterator(str.begin(), str.end(), regexz, -1),
							  std::sregex_token_iterator());
			    return list;
		}
	


		std::string hex2string( std::string in) {
		
	    		std::string output;

			lg.log("Packet:hex2string: input: " + in, 4);
			    if ((in.length() % 2) != 0) {
				throw std::runtime_error("String is not valid length ...");
			    }

			    size_t cnt = in.length() / 2;

			    for (size_t i = 0; cnt > i; ++i) {
				uint32_t s = 0;
				std::stringstream ss;
				ss << std::hex << in.substr(i * 2, 2);
				ss >> s;

				output.push_back(static_cast<unsigned char>(s));
			    }

			    return output;
		}
		*/

		/*
		 *
		 * this function get a serie of characters
		 * and  size, then return the string 
		 * type of them.
		 *
		 */
		std::string convertToString(char *a, int size){
			
			std::string s = "";
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
				std::string byte = show_bytes(i+26,i+27);
				src_ip[i] = stoi(byte,nullptr,16);

			}

			for(int i=0; i<4; i++){
				std::string byte = show_bytes(i+30,i+31);
				dst_ip[i] = stoi(byte,nullptr,16);

			}
		}


		int getIndex(std::vector<std::string> v, std::string K){

			auto it = std::find(v.begin(), v.end(), K);

			// If element was found
			if (it != v.end()){
				// calculating the index
				// of K
				int index = it - v.begin();
				return index;
			}
			else {
				// If the element is not
				// present in the vector
				return -1;
			}
		}
};




