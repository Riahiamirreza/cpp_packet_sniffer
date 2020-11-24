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
		int loglevel;

		Logger(){


			
		}

		void set_log_level(int level){
		
			loglevel = level;

		}

		void log(std::string msg, int level ){
		
			if(level <= loglevel){
				log_(msg, level);
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

			if(loglevel == 4 )
				console->set_level(spdlog::level::level_enum::debug);

			//else if(loglevel == 5 )
			//	console->set_level(spdlog::level::level_enum::trace);

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


