#include "jparser.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <sys/types.h>


class Process{


	public:
	
		Logger lg;                    // initializing the logger object.

		std::string conf_file_address = "p3-advanced.conf.d/p3-advanced.conf";
		std::string dir_address;      
		std::string sub_dir;
		std::string file_name;        // it's name of current file.
		std::vector<std::string> lof; // list of files
		int end_of_file = 0; 	      // if this flag is on, the files of the directory are over.
		int end_of_dir  = 0; 	      // if this flag is on, the dir of the directory are over.
		int loglevel    = 3;
		int len            ;	      // it's number of 
		int new_file_   = 0;
		int packet_loglevel;
		int clevel;                   // current network layer.
		JParser jfile;
		Json::Value::const_iterator iter;
		std::vector<std::vector<std::string>> all_data;

		Process(){}

		/*
		 * This function will get the config
		 * variables and store them.
		 */

		void setconf(std::string dir_address_, int loglevel, std::vector<std::vector<std::string>> all_data_){

			lg.set_log_level(loglevel);
			//dir_address = dir_address_;
			//lg.log("Proc:Proc:dir_address: " + dir_address,4);
			all_data = all_data_;
		
		}

		/*
		 * this function will initialize
		 * the values.
		 */

		void init(std::string level){
		
			if(level == "network")
				clevel = 0;

			else if(level == "transport")
				clevel = 1;
			
			else if(level == "application")
				clevel = 2;
			
			else 
				clevel = 0;

			new_file_ = 1;
			lof = all_data[clevel];
		}
		/*
		void init(std::string level){
			
			sub_dir = dir_address + "/" + level;
			//char addr[ sub_dir.length() + 1];
			//strcpy(addr,sub_dir.c_str());
			lof = file(sub_dir);
			lg.log("Proc:init: " + std::to_string(lof.size()), 5);
			lg.log("Proc:init: " + sub_dir, 5);
			new_file_ = 1; 

		}
		*/

		/*
		 * this function will jump to
		 * the next file in lof (list of files)
		 */

		void next_file(){
		
			static int counter = 0;
			if(new_file_){
				counter = 0;
				new_file_    = 0;
			}

			jfile.init_string(all_data[clevel][counter], loglevel);

			file_name = jfile.get_name();
			lg.log("Proc:next_file: " + all_data[clevel][counter], 5);
			lg.log("Proc:next_file: " + file_name, 4);

			jfile.set_size();
			len = jfile.total_kvs;
			counter++;
		}
		/*
		void next_file(){


			static int counter = 0;
			if(new_file_){
				counter   = 0;
				new_file_ = 0;
			}

			jfile.init(sub_dir + "/" + lof[counter], 5);
			file_name = lof[counter];
			lg.log("Proc:next_file: " + sub_dir + "/" + lof[counter], 5);
			counter++;
			
			if(!end_of_dir){
				jfile.set_size();
				len = jfile.total_kvs;
			}

		}
		*/


		/*
		 * this function will jump to the next
		 * object in the current file.
		 */

		void next_step(){
			
			iter = jfile.iterate_json();
			lg.log("proc: next step", 5);
		}

		/*
		 * this function will return key of
		 * the current state.
		 */

		std::vector<int> get_keys(){

			std::vector<int> keys;
			std::string str_keys = iter.key().asString();
			lg.log("proc : " + str_keys, 5);
			std::replace(str_keys.begin(), str_keys.end(), '-', ' ');
			std::stringstream ss(str_keys);
			int temp;
			while (ss >> temp)
				keys.push_back(temp);

			return keys;
		}

		std::string get_key(){
			
			std::string str_key = iter.key().asString();
			return str_key;
		}
		
		/*
		 * this function will return
		 * the value of the 
		 * current state.
		 *
		 */

		std::string get_value(){
			
			std::string val = iter->asString();
			return val;

		}



		
		/*
		 * this function will return
		 * a list of all files in the
		 * given address
		 *
		 */

		std::vector<std::string> file(std::string address_) {
        
			int size = address_.length();
			char address[size+1];
			strcpy(address, address_.c_str());
	        	DIR *dr;
	        	struct dirent *en;
   
  			dr = opendir(address); //open all directory
   
  			std::vector<std::string> files;
   
  		        if (dr) {
			        while ((en = readdir(dr)) != NULL) {
			                if(en->d_type != DT_DIR){
                        			std::string *name = new std::string(en->d_name);
	                        		files.push_back(*name);
        		      		}
        		}
	        	
			closedir(dr); //close all directory

 		      	}

	        	return files;
		}


};
