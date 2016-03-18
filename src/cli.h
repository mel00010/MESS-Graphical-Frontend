#include "SNES.h"
#include <stdlib.h>
#include <string>
#include <json/json.h>

class Cli
{
	public:
		Cli(std::string configFile);
		~Cli();
	protected:
		Json::Value root;
		Json::Reader reader;
		SNES snes;
		void set_keypress(void);
		std::string getFileContents(const char *filename);
};
