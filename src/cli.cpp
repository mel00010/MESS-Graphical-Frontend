#include "cli.h"
#include "SNES.h"
#include <iostream>
#include <json/json.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>  
#include <termio.h>
#include <curses.h>

std::string Cli::getFileContents(const char *filename)
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
	} else {
		std::cout << "\033[1mError File " << filename << " not found\033[0m" << std::endl;
		exit(1);
	}
	//~ throw(errno);
}
void Cli::set_keypress(void)
{
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

Cli::Cli (std::string configFile) 
{
	try 
	{
		reader.parse(getFileContents(configFile.c_str()),root);	
	}
	catch (std::bad_alloc& e) 
	{
		std::cout << "\033[1mError reading file " << configFile << "\033[0m"<< std::endl;
		exit(1);
	}
	//Generate menu
	std::cout << "\033[1;31mGAME SELECTION \033[0m" << std::endl << std::endl;
	//~ Json::Value games_ = root["games"];
	int i = 1;
	for (Json::Value::iterator itr = root["games"].begin(); itr != root["games"].end(); itr++)
	{
		Json::Value game = (*itr);
		std::string value = game["name"].asString();
		std::cout << "\033[1;34m" << i << "\033[0m" << "\t" << value << '\n';
		i++;
	}
	std::cout << "\033[1mSelected Game:  \033[1;37m";
	
	//Get input from user
	std::string response;
	std::stringstream str;
	set_keypress();
	char character;
	while (true)
	{
		character = std::getchar();
		if (character==27) {  //Check if character is ESCAPE, if so exit. 
			std::cout << std::endl;
			exit(0);
			//~ return 0;
		} else if (character=='\n'){ //Check if character is ENTER
			response = str.str();
			std::cout << "\033[0m";
			int responseInt;
			try { //Try to parse input into an integer
				responseInt = std::stoi(response);
				str.str(std::string());
			}
			catch(std::invalid_argument& e) { //Catch errors from stoi
				std::cout << response << " is not a valid game code.  " << std::endl;
				responseInt = 0;
				str.str(std::string());
			}
			catch(std::out_of_range& e) { //Catch errors from stoi
				std::cout << response << " is not a valid game code.  " << std::endl;
				responseInt = 0;
				str.str(std::string());
			}
			if (root["games"].isValidIndex(responseInt - 1) && responseInt != 0) { // Load and run game
				int index = responseInt -1; //People count from 1, the Json array counts from 0
				snes.load(root["games"][index]["save_path"].asString(),root["games"][index]["system"].asString());
				snes.run(root["games"][index]["path"].asString(), root["games"][index]["system"].asString());
				snes.save(root["games"][index]["save_path"].asString(), root["games"][index]["system"].asString());
				//~ return 0;
				exit(0);
			} else if (responseInt != 0) { // If we get this far, the code must be invalid
				std::cout << response << " is not a valid game code.  " << std::endl;
			}
			
		}
		str << character;
	}
	//~ return 0;
	exit(0);
}
Cli::~Cli()
{
}
