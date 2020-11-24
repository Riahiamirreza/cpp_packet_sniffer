#include  "session.hpp"


class Config{

/*
 *
 *  This Class is used to help
 *  configure the program.
 *  log level and directory of 
 *  program data will be found when
 *  the function "set_config()" is
 *  called
 *
 */
	public:

		Logger lg;
		Json::Reader reader;
		Json::Value  obj;
		std::string base_address = "p3-advanced.conf.d/p3-advanced.conf" ;
		std::string loglevel_;
		bool flag = 1;
		int loglevel ;
		std::string directory;
		std::vector<std::vector<std::string>> all_json_data;

		void set_config(){
		
			std::ifstream conf(base_address);
			reader.parse(conf, obj);
			directory =  obj["directory"].asString();
			if(obj["loglevel"].type() == Json::intValue){
				loglevel = obj["loglevel"].asInt();
				flag = 0;
			}else if(obj["loglevel"].type() == Json::stringValue){
				loglevel_ = obj["loglevel"].asString();
				flag = 0;
			}
			if(obj["loglevel"].type() == Json::stringValue){

				if(loglevel_ == "CRITICAL" || loglevel_ == "FATAL")
					loglevel = 0; 

				else if( loglevel_ == "ERROR")
					loglevel = 1;

				else if( loglevel_ == "WARNING")
					loglevel = 2;

				else if( loglevel_ == "INFO")
					loglevel = 3;

				else if( loglevel_ == "DEBUG")
					loglevel = 4;

				else if( loglevel_ == "TRACE")
					loglevel = 5;
				else
					loglevel = 3;

				flag = 0;

			}


			if(flag){
				
				lg.set_log_level(3);
				lg.log("Config:set_config: loglevel not correct! used default value insted :\" INFO\" ", 2);
				loglevel = 3;
			}else{

				lg.set_log_level(loglevel);
			}


			if(directory.size() == 0){
				lg.log("Config:set_config: directory empty! used default value insted :\" protocols\" ", 2);
				directory = "protocols";

			}


			lg.log("Config:Config: directory: " + directory, 5);
		
			
		}

		void set_json_data(){
			
			Process proc;
			JParser jp;
			std::string dir_network  = directory + "/network";
			std::string dir_transport  = directory + "/transport";
			std::string dir_application  = directory + "/application";


			std::vector<std::string> files_network = proc.file(dir_network);
			std::vector<std::string> files_transport = proc.file(dir_transport);
			std::vector<std::string> files_application = proc.file(dir_application);

			std::vector<std::string> temp_1;
			std::vector<std::string> temp_2;
			std::vector<std::string> temp_3;

			for(int i=0; i<files_network.size(); i++){	
				temp_1.push_back(jp.read_json_file(dir_network + "/" + files_network[i]));		
				lg.log("Config:set_json_data: " + dir_network + "/" + files_network[i], 4);
			}
					
			all_json_data.push_back(temp_1);

			for(int i=0; i<files_transport.size(); i++){	
				temp_2.push_back(jp.read_json_file(dir_transport + "/" + files_transport[i]));
				lg.log("Config:set_json_data: " + dir_transport + "/" + files_transport[i], 4);
			}

			all_json_data.push_back(temp_2);

			for(int i=0; i<files_application.size(); i++)	
				temp_3.push_back(jp.read_json_file(dir_application + "/" + files_application[i]));

			all_json_data.push_back(temp_3);
		}

		
};
