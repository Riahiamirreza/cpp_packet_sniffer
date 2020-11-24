#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>
#include "logging.hpp"


class JParser{


	public:
		Json::Reader reader;
		Json::Value obj;
		Logger lg;
		int log_sev;
		size_t obj_s;
		int obj_size;
		int total_kvs;
		int new_file = 0;

		/*
		 * this function will open the file in 
		 * address and set it's config in variables.
		 */

		void init(std::string address, int log_level ){

			lg.set_log_level(log_level);
			std::ifstream ifs(address);
			reader.parse(ifs, obj);
			obj_s = obj.size();
			obj_size = static_cast<int>(obj_s);
			new_file = 1;
			lg.log("JParser:init:", 5);
		}

		void init_string(std::string json_data, int log_level){
		
			lg.set_log_level(log_level);
			reader.parse(json_data, obj);
			obj_s = obj.size();
			obj_size = static_cast<int>(obj_s);
			new_file = 1;
			lg.log("JParser:init_string:", 5);
		}

		
		std::string read_json_file(std::string address){
			
			Json::Value val;
			Json::Reader reader;
			Json::FastWriter fast;
			std::ifstream ifs(address);
			reader.parse(ifs, val);
			std::string sfast = fast.write(val);
			return sfast;

		}

		std::string get_name(){
			
			lg.log("JParser:get_name: " + obj[0].asString(),5);
			return obj[0].asString();
		}

	       /* 
      		 * This function will 
		 * find the size of 
		 * file and put this size in
		 * variable "total_kvs" which is 
		 * abbreviation of total key-values stored
		 * in the current file.
	    	 *	
	    	*/

		void set_size(){
			
			total_kvs = 0;
			lg.log("JParser:set_size:obj_size :"+ std::to_string(obj_size), 5);
			for(int i=1; i <= obj_size - 1  ; i++) 
				total_kvs = total_kvs + static_cast<int>(obj[i].size());
			

		}

		
		/*
		 * This function will return type of
		 * data is stored in the value side
		 * of the given key.
		 */

		int check_type(Json::Value val){
			
			return val.type();
		}

		/*
		 * this function will iterate over 
		 * json objects each time is called.
		 */

		Json::Value::const_iterator iterate_json(){


			static int i = 1;
			static Json::Value::const_iterator it_ = obj[i].begin();
			if(new_file){
				i = 1;
				it_ = obj[i].begin();
				new_file = 0;
			}
			if(it_ == obj[i].end() && i < static_cast<int>(obj_s)){
				i++;
				it_ = obj[i].begin();
				lg.log("jparser: iter reached to max value", 5);

			} else if(i == static_cast<int>(obj_s) ){

				i = 1;
				lg.log("jparser: i reached to max value",  5);
				Json::Value::const_iterator null;
				return null;
			}

			for(; it_ != obj[i].end();){
						
				lg.log("jparser: kvs " + std::to_string(total_kvs), 5);
				Json::Value::const_iterator it__ = it_;
				it_++;		
				return it__;

			}
							
			return it_;

		}

};
