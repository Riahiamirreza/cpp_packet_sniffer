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
		JParser jfile;
		Json::Value::const_iterator iter;

		Process(){
			
			JParser jconfig;
			jconfig.init(conf_file_address, 4);
			dir_address = jconfig.obj["directory"].asString();
			loglevel = jconfig.obj["Process:loglevel"].asInt();
			packet_loglevel = jconfig.obj["Packet:loglevel"].asInt();
			lg.log("Packet:Packet:dir_address: " + dir_address,loglevel);

		}

		/*
		 * this function will initialize
		 * the values.
		 */

		void init(std::string level){
			
			sub_dir = dir_address + "/" + level;
			char addr[ sub_dir.length() + 1];
			strcpy(addr,sub_dir.c_str());
			lof = file(addr);
			lg.log(std::to_string(lof.size()),loglevel);
			lg.log(sub_dir,loglevel);
			new_file_ = 1;

		}


		/*
		 * this function will jump to
		 * the next file in lof (list of files)
		 */

		void next_file(){


			static int counter = 0;
			if(new_file_){
				counter   = 0;
				new_file_ = 0;
			}

			jfile.init(sub_dir + "/" + lof[counter], 5);
			file_name = lof[counter];
			lg.log("Proc:next_file: " + sub_dir + "/" + lof[counter], loglevel);
			counter++;
			
			/*
			static int counter = 0;
			if(counter < lof.size()){
				jfile.init(sub_dir + "/" + lof[counter], 5);
				file_name = lof[counter];
				lg.log("Proc:next_file: " + sub_dir + "/" + lof[counter], loglevel);
				counter++;
			}
			else{
				end_of_dir = 1;
			}
			*/

			if(!end_of_dir){
				jfile.set_size();
				len = jfile.total_kvs;
			}

		}


		/*
		 * this function will jump to the next
		 * object in the current file.
		 */

		void next_step(){
			
			iter = jfile.iterate_json();
			lg.log("proc: next step", loglevel);
			//if(iter.key().asString().length() == 0 )
			//	end_of_file = 0;
		}

		/*
		 * this function will return key of
		 * the current state.
		 */

		std::vector<int> get_keys(){

			std::vector<int> keys;
			std::string str_keys = iter.key().asString();
			lg.log("proc : " + str_keys, loglevel);
			std::replace(str_keys.begin(), str_keys.end(), '-', ' ');
			std::stringstream ss(str_keys);
			int temp;
			while (ss >> temp)
				keys.push_back(temp);

			return keys;
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


	private:

		
		/*
		 * this function will return
		 * a list of all files in the
		 * given address
		 *
		 */

		std::vector<std::string> file(char *address) {
        
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
