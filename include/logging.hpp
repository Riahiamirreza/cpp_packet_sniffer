#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

/*
void logger(std::string msg, std::string stdout_ = "console"){
	
	auto console = spdlog::stdout_color_mt(stdout_);
	console->info(msg);
}
*/


/*
 *
 *  This class will create
 *  log file for each of it's
 *  object.
 *  it will be more complete soon!
 *
 */

class Logger{

	public:
		int logs = 0; //number of times this object has created log

		Logger(){


			
		}

		void log(std::string msg, int level ){
		
			if(true){

				log_(msg, level);
				logs ++;
			
			} else {
				
				//log__(msg, level, file_name);
				logs ++;
			}
		}


	private:

		/*

		void log__(std::string msg, int level = 3, std::string file = file_name){
			
			auto console = spdlog::basic_logger_mt("loggerr", file);

			switch(level){
				case 0:
					console->critical(msg);
					break;
				case 1:
					console->error(msg);
					break;
				case 2:
					console->warn(msg);
					break;
				case 3:
					console->info(msg);
					break;
				case 4:
					console->debug(msg);
					break;
				case 5: 
					console->trace(msg);
					break;

			}		


		}

		*/

		void log_(std::string msg, int level = 3){
		

			static auto console = spdlog::stdout_color_mt("console");
			switch(level){
				case 0:
					console->critical(msg);
					break;
				case 1:
					console->error(msg);
					break;
				case 2:
					console->warn(msg);
					break;
				case 3:
					console->info(msg);
					break;
				case 4:
					console->debug(msg);
					break;
				case 5: 
					console->trace(msg);
					break;

			}

		}

};


