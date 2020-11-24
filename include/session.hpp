#include "packet.hpp"





/*
struct sessions{




 * This structure holds
 * session infomation.
 * and packets which
 * transmitted through 
 * the session.
 


	int id;
	int number_packets_transmitted_through_this_session;  // a very clear name for this variable.
	std::string src_ip;
	std::string dst_ip;
	int src_port;
	int dst_port;
	std::string protocol;
	std::string state_of_session;                        // this state can be "connected" or "connecting...", et cetera.
	std::vector<Packet> packets;                         // all packets that have transmitted through this session.
	

};

*/


class Session{

	public:

		int src_port;
		int dst_port;
		std::string src_ip;
		std::string dst_ip;
		std::string protocol;
		std::vector<Packet> packets;
		Logger lg;

		Session(Packet pk, int loglevel){
		
			lg.set_log_level(loglevel);

			src_port    = pk.to_int(pk.getItem("src_port"));
			dst_port    = pk.to_int(pk.getItem("dst_port"));
			src_ip      = pk.show_src_ip(false);
			dst_ip      = pk.show_dst_ip(false);
			protocol    = pk.transport_protocol;

			packets.push_back(pk);	
		
		}
		bool is_valid(Packet pk){
			
			lg.log(std::to_string(pk.to_int(pk.getItem("src_port")))+" "+std::to_string(pk.to_int(pk.getItem("dst_port"))) 
					+ std::to_string(dst_port) + " " + std::to_string(src_port), 4);
				
			if((pk.to_int(pk.getItem("src_port")) == src_port  ||
			    pk.to_int(pk.getItem("src_port")) == dst_port) && 
			   (pk.to_int(pk.getItem("dst_port")) == src_port  ||
			    pk.to_int(pk.getItem("dst_port")) == dst_port) && 
			   (pk.show_src_ip(false)             == src_ip    ||
			    pk.show_src_ip(false)             == dst_ip)   &&
			   (pk.show_dst_ip(false)             == src_ip    ||
			    pk.show_dst_ip(false)             == dst_ip)   &&
			    pk.transport_protocol == protocol)

				return true;
			else
				return false;
		}


		void add_packet(Packet pk){
			
			packets.push_back(pk);
		}

		std::string show(){
		
			std::string out = "ip :["  + src_ip   + "]  [" + dst_ip   + "]";
			out = out + " protocol : " + protocol;
			out = out + " ports  : ["  + std::to_string(src_port) + "]  [" + std::to_string(dst_port) + "]";
			out = out + " " + std::to_string(packets.size()) + " packets in session";

			return out;
		}


                                                                
};



