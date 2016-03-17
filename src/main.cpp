#include <gtkmm.h>
#include "gui.h"
#include "SNES.h"
#include <gtkmm/application.h>
#include <iostream>
#include <json/json.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>  
#include <termio.h>
#include <curses.h>



std::string getFileContents(const char *filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
void set_keypress(void) {
	struct termios new_settings, stored_settings;
	// Get the current settings
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	
	// Remove printing of special characters
	new_settings.c_lflag &= (~ICANON);
	
	// Remove wait time
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0,&stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
}

bool quitting () // called when Gtk::Main quits
{
    return true;
}

int main(int argc, char* argv[])
{
	if ( argc == 1 ) 
	{
		auto app = Gtk::Application::create("gameSelection.window");
		Gui window;
		//~ Gtk::Main::signal_quit().connect (sigc::ptr_fun(&quit));
		app->run(window);
			
	} else {
		std::string arg1(argv[1]);
		//~ std::string comp();
		if ((arg1.compare("--cli") == 0) || (arg1.compare("-c") == 0))  
		{
			std::cout << "\033[1;31mGAME SELECTION \033[0m\n\n";
			Json::Value root;
			Json::Reader reader;
			reader.parse(getFileContents("/home/mel/roms/config.json"),root);
			Json::Value games_ = root["games"];
			SNES snes;
			int i = 1;
			for (Json::Value::iterator itr = games_.begin(); itr != games_.end(); itr++)
			{
				Json::Value game = (*itr);
				std::string value = game["name"].asString();
				std::cout << "\033[1;34m" << i << "\033[0m" << "\t" << value << '\n';
				i++;
			}
			std::cout << "\033[1mSelected Game:  \033[1;37m";
			std::string response;
			std::stringstream str;
			set_keypress();
			char c;
			while (true)
			{
				c = std::getchar();
				if (c==27) {
					std::cout << std::endl;
					return 0;
				} else if (c=='\n'){
					response = str.str();
					std::cout << "\033[0m";
					int responseInt;
					try {
						responseInt = std::stoi(response);
						str.str(std::string());
					}
					catch(std::invalid_argument& e) {
						std::cout << response << " is not a valid game code.  " << std::endl;
						responseInt = 0;
						str.str(std::string());
					}
					catch(std::out_of_range& e) {
						std::cout << response << " is not a valid game code.  " << std::endl;
						responseInt = 0;
						str.str(std::string());
					}
					if (games_.isValidIndex(responseInt - 1) && responseInt != 0) {
						int index = responseInt -1;
						snes.load(games_[index]["save_path"].asString(),games_[index]["system"].asString());
						snes.run(games_[index]["path"].asString(), games_[index]["system"].asString());
						snes.save(games_[index]["save_path"].asString(), games_[index]["system"].asString());
						return 0;
					} else if (responseInt != 0) {
						std::cout << response << " is not a valid game code.  " << std::endl;
					}
					
				}
				str << c;
			}
		} else if ((arg1.compare("--help")== 0) || (arg1.compare("-h")== 0)) {
			std::cout<<"Usage: "<< argv[0] << std::endl;
			return 0;			
		} else if ((arg1.compare("--gui")== 0) || (arg1.compare("-g")== 0)) {
			auto app = Gtk::Application::create("gameSelection.window");
			Gui window;
				return app->run(window);
		} else {
			std::cout<<"Usage: "<< argv[0] << std::endl;
			std::cout << argv[1] << std::endl;
			return 1;
		}
	}
	return 1;
}

