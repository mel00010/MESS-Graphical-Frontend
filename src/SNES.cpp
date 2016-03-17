#include "SNES.h"
#include <sys/stat.h>
#include <unistd.h> 
#include <sys/types.h>
//~ #include <dirent.h>
#include <errno.h>
#include <iostream>
#include <vector>

#include <string>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <time.h>   
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/regex.hpp>



SNES::SNES()
{
	
}
std::string SNES::filterFiles(const std::string path, const std::string letter) 
{
	std::string regular_expression = "^"+letter;
	boost::regex expression(regular_expression);
	boost::filesystem::path p(path);
	std::vector<boost::filesystem::path> files;
	//~ pred predicate;
	//~ auto f = std::bind(&pred::operator(), &predicate, expression, std::placeholders::_1);
	boost::filesystem::directory_iterator end_itr;
	for( boost::filesystem::directory_iterator i( path ); i != end_itr; ++i )
	{
		
	    // Skip if not a file
	    if( !boost::filesystem::is_regular_file( i->status() ) ) continue;
	    boost::smatch what;
	    // Skip if no match for V2:
	    if( !boost::regex_search(i->path().filename().string(), what, expression ) ) continue;
	    // For V3:
	    //if( !boost::regex_match( i->path().filename(), what, my_filter ) ) continue;
	
	    // File matches, store it
	    files.push_back( i->path() );
	}
	if (!files.empty())
	{
		sort(files.begin(), files.end(),
		[](const boost::filesystem::path& p1, const boost::filesystem::path& p2)
			{
				return boost::filesystem::last_write_time(p1) < boost::filesystem::last_write_time(p2);
			});
		return files.front().string();
	}
	return "false";
}



void SNES::load(const std::string save_path, const std::string system) 
{
	static const std::string letters[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","auto"};
	std::vector<std::string> files;
	for( const auto& letter : letters)
	{
		std::string file = filterFiles(save_path, letter+"_");
		if(file != "false") {
			//~ cout << file << '\n';
			files.push_back(file);
			boost::filesystem::path source(file);
			std::string dest_path ="/home/mel/.mess/sta/"+system+"/"+letter+".sta";
			//~ cout << dest_path;
			boost::filesystem::path dest(dest_path);
			boost::filesystem::copy_file(source,dest,boost::filesystem::copy_option::overwrite_if_exists);
		}
	}
	//~ cout << system << '\n';
	//~ cout << save_path << '\n';
}
void SNES::save(const std::string save_path, const std::string system) {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%Y_%m_%d_%I_%M_%S",timeinfo);
	std::string date(buffer);
	static const std::string letters[] = {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","auto"};
	std::vector<std::string> files;
	for( const auto& letter : letters)
	{
		std::string file = filterFiles("/home/mel/.mess/sta/"+system+"/", letter);
		if(file != "false") {
			//~ cout << file << '\n';
			files.push_back(file);
			boost::filesystem::path source(file);
			std::string dest_path =save_path+"/"+letter+"_"+date+".sta";
			//~ cout << dest_path << "\n";
			boost::filesystem::path dest(dest_path);
			boost::filesystem::copy_file(source,dest,boost::filesystem::copy_option::overwrite_if_exists);
			boost::filesystem::remove(source);
		}
	}
}
void SNES::run(const std::string path, const std::string system) 
{
	std::string command = "mess "+system+" -autosave -centerh -centerv  -cart " + path;
	//~ const char *command = _command.c_str();
	std::system(command.c_str());
	//~ cout << command << '\n';
}

SNES::~SNES()
{
}
