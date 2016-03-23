#include <gtkmm.h>
#include "gui.h"
#include "cli.h"
#include "parser.h"
#include <json/json.h>
//~ #include <gtkmm/application.h>
#include <iostream>
#include <string>
#include <stdlib.h>




void help(std::string filename, std::string command = "", bool error = false)
{
	std::cout<<"Usage: \t" << filename << "[--gui|-g][--file|-f configFile]"<< std::endl;
	//~ std::cout<< "\t" << filename << "[--gui|-g][--file|-f configFile]" << std::endl;
	std::cout<< "\t" << filename << "[--cli|-c][--file|-f filename]" << std::endl;
	std::cout<< "Options:\
	\n\t-g, --gui\tForces the the program to use a CUI\
	\n\t-c, --cli\tForces the program to use a CLI\n\t-f       \t Specifies a config file to use for the game list\
	\n\t    --file\t Same as -f" << std::endl;
	if ( command != "" && error == true) {
		std::cout<< "Argument " << command << " not valid "<< std::endl;
		exit(1);
	//~ } else if (command != "" && error == false) {
		//~ std::cout << 
		//~ std::cout<< command << std::endl;
		//~ exit(0);
	}
}
int main(int argc, char* argv[])
{
	// Check if argument
	if ( argc == 1 ) 
	{
		Parser parser;
		Json::Value root = parser.parse(std::string("/home/mel/roms/config.json"));
		auto app = Gtk::Application::create("gameSelection.window");
		Gui window(root);
		app->run(window);
			
	} else {
		// Loop through arguments
		for (int i = 1; i < argc + 1; i++){
			std::string arg(argv[i]);
			if ((std::string(argv[i]).compare("--gui") == 0) || (std::string(argv[i]).compare("-g") == 0))  
			{
				if (argc > i+1) {
					if (((std::string(argv[i+1]).compare("--file") == 0) || (std::string(argv[i+1]).compare("-f") == 0)) && (argc > i+2))
					{
						Parser parser;
						Json::Value root = parser.parse(argv[i+2]);
						auto app = Gtk::Application::create("gameSelection.window");
						Gui window(root);
							app->run(window);
						i = i+2;
					} else if ((std::string(argv[i+1]).compare("--file") == 0) || (std::string(argv[i+1]).compare("-f") == 0)) {
						std::cout << "Error no file specified after " << argv[i+1] << std::endl;
						//~ Cli cli("/home/mel/roms/config.json");
						return 1;
					}
				} else {
					Parser parser;
					Json::Value root = parser.parse(std::string("/home/mel/roms/config.json"));
					auto app = Gtk::Application::create("gameSelection.window");
					Gui window(root);
						app->run(window);
				}
			} else if ((std::string(argv[i]).compare("--help")== 0) || (std::string(argv[i]).compare("-h")== 0)) {
				if (argc > i+1) {
					help(argv[0], argv[i+1]);
				} else {
					help(argv[0]);
				}
				return 0;			
			} else if ((std::string(argv[i]).compare("--cli") == 0) || (std::string(argv[i]).compare("-c") == 0))  
			{
				if (argc > i+1) {
					if (((std::string(argv[i+1]).compare("--file") == 0) || (std::string(argv[i+1]).compare("-f") == 0)) && (argc > i+2))
					{
						Parser parser;
						Json::Value root = parser.parse(argv[i+2]);
						Cli cli(root);
						i = i+2;
					} else {
						std::cout << "Error no file specified after -f" << std::endl;
						//~ Cli cli("/home/mel/roms/config.json");
						return 1;
					}
				} else {
					Parser parser;
					Json::Value root = parser.parse(std::string("/home/mel/roms/config.json"));
					Cli cli(root);
				}
			} else if ((std::string(argv[i]).compare("--file") == 0) || (std::string(argv[i]).compare("-f") == 0)) {
				if (argc > i+1) {
					Parser parser;
					Json::Value root = parser.parse(argv[i+1]);
					auto app = Gtk::Application::create("gameSelection.window");
					Gui window(root);
					i++;
					return app->run(window);	
				} else {
					std::cout << "Error no file specified after " << argv[i] << std::endl;
					return 1;
				}
			} else {
				help(argv[0], argv[i], true);
			}	
		}
	}
	return 1;
}

