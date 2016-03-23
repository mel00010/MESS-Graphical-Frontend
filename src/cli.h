#ifndef CLI_H
#define CLI_H
#include <json/json.h>

class Cli
{
	public:
		Cli(Json::Value root);
		virtual ~Cli();
	protected:
		void set_keypress(void);
};
#endif //CLI_H
