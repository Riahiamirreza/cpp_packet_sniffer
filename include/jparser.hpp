
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
		//int i = 0;
		//Json::Value::const_iterator it_ ;

		/*
		 * this function will open the file in 
		 * address and set it's config in variables.
		 */

		void init(std::string address, int log_level = 3){

			std::ifstream ifs(address);
			reader.parse(ifs, obj);
			obj_s = obj.size();
			obj_size = static_cast<int>(obj_s);
			log_sev = log_level;
			//it_ = obj[i].begin();
			new_file = 1;
			lg.log("JParser:init:", log_sev);
		}


		void set_size(){
			
			total_kvs = 0;
			lg.log("JParser:set_size:obj_size :"+ std::to_string(obj_size), log_sev);
			for(int i=0; i <= obj_size - 1  ; i++){

				total_kvs = total_kvs + static_cast<int>(obj[i].size());
				//lg.log("jparser : size " + std::to_string(static_cast<int>(obj[i].size())),log_sev);
				//lg.log("jparser : size " + std::to_string(static_cast<int>(obj[i].size())),log_sev);
			}

		}

		void show(){
			
			
			for(int i=0; i< static_cast<int>(obj_s) ; i++){

				for(Json::Value::const_iterator it=obj[i].begin(); it != obj[i].end(); ++it){
					
					//std::cout << it.key().asString() << " : "<< it->asInt() << std::endl;
					//std::cout << it.key().asString() << std::endl;
					std::cout << it->type() << std::endl;
					switch (check_type(*it)){
						
						case 0:
							std::cout << "null" << std::endl;
							break;
						case 1:
							std::cout << "int" << std::endl;
							break;
						case 2:
							std::cout << "uint" << std::endl;
							break;
						case 3:
							std::cout << "real" << std::endl;
							break;
						case 4:
							std::cout << "string" << std::endl;
							break;
						case 5:
							std::cout << "bool" << std::endl;
							break;
						case 6:
							std::cout << "array" << std::endl;
							break;
						case 7:
							std::cout << "obj" << std::endl;
							break;

					}
				}
				std::cout << "\n";
			}

			std::cout << obj_s << std::endl;
			std::cout << static_cast<int>(obj_s) << std::endl;
		}

		int check_type(Json::Value val){
			
			return val.type();
		}

		/*
		 * this function will iterate over 
		 * json objects each time is called.
		 */

		Json::Value::const_iterator iterate_json(){


			static int i = 0;
			static Json::Value::const_iterator it_ = obj[i].begin();
			if(new_file){
				i = 0;
				it_ = obj[i].begin();
				new_file = 0;
			}
			//lg.log("jparser: iter reached to max value", log_sev);
			if(it_ == obj[i].end() && i < static_cast<int>(obj_s)){
				i++;
				it_ = obj[i].begin();
				lg.log("jparser: iter reached to max value", log_sev);

			} else if(i == static_cast<int>(obj_s) ){

				i = 0;
				lg.log("jparser: i reached to max value", log_sev);
				Json::Value::const_iterator null;
				return null;
			}

			for(; it_ != obj[i].end();){
						
				//std::cout << it_.key().asString() << i << std::endl; 
				lg.log("jparser: kvs " + std::to_string(total_kvs), log_sev);
				//lg.log("jparser: " + std::to_string(obj[i].size()), log_sev);
				Json::Value::const_iterator it__ = it_;
				it_++;		
				return it__;

			}
							

		}

};
