#ifndef SNES_H
#define SNES_H
#include <string>
#include <json/json.h>

class SNES
{
public:
	SNES(Json::Value root, int index);
	virtual ~SNES();
protected:
	std::string filterFiles(const std::string path, const std::string letter);
	void load(const std::string save_path, const std::string system); 
	void save(const std::string save_path, const std::string system);
	void run(const std::string path, const std::string system); 
};
#endif //SNES_H
