#include <gtkmm.h>
#include "gui.h"
#include "cli.h"
//~ #include <gtkmm/application.h>
#include <iostream>
#include <string>
#include <stdlib.h>




void help(std::string filename, std::string command = "", bool error = false) {
	std::cout<<"Usage: "<< filename << std::endl;
	if ( command != "" && error == true) {
		std::cout<< "Argument " << command << " not valid "<< std::endl;
		exit(1);
	} else if (command != "" && error == false) {
		std::cout<< command << std::endl;
		exit(0);
	}
}
int main(int argc, char* argv[])
{
	// Check if argument
	if ( argc == 1 ) 
	{
		auto app = Gtk::Application::create("gameSelection.window");
		Gui window;
		app->run(window);
			
	} else {
		// Loop through arguments
		for (int i = 1; i < argc + 1; i++){
			std::string arg(argv[i]);
			if ((std::string(argv[i]).compare("--cli") == 0) || (std::string(argv[i]).compare("-c") == 0))  
			{
				if (argc > i+1) {
					if (((std::string(argv[i+1]).compare("--file") == 0) || (std::string(argv[i+1]).compare("-f") == 0)) && (argc > i+2))
					{
						Cli cli(argv[i+2]);
						i = i+2;
					} else {
						std::cout << "No argument found after -f:  Ignoring" << std::endl;
						//~ Cli cli("/home/mel/roms/config.json");
						return 1;
					}
				} else {
					Cli cli("/home/mel/roms/config.json");
				}
			} else if ((std::string(argv[i]).compare("--help")== 0) || (std::string(argv[i]).compare("-h")== 0)) {
				if (argc > i+1) {
					help(argv[0], argv[i+1]);
				} else {
					help(argv[0]);
				}
				return 0;			
			} else if ((std::string(argv[i]).compare("--gui")== 0) || (std::string(argv[i]).compare("-g")== 0)) {
				auto app = Gtk::Application::create("gameSelection.window");
				Gui window;
					return app->run(window);
			} else {
				help(argv[0], argv[i], true);
			}	
		}
	}
	return 1;
}

