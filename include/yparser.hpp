#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>


class YParser{
	
	public:
		
		YAML::Node conf;

		void init(std::string address){
			
			conf = YAML::LoadFile(address);

		}

		void show(std::string something){
			
			if(!conf[something])
				return ;

			std::cout << something <<" : " << conf[something].as<std::string>() << std::endl;
			
		}

		void showAll(){
			
			std::cout << conf << std::endl;
		}


			

};
