#ifndef SNES_H
#define SNES_H
#include <string>

class SNES
{
public:
	SNES();
	virtual ~SNES();
	void load(const std::string save_path, const std::string system); 
	void save(const std::string save_path, const std::string system);
	void run(const std::string path, const std::string system); 
protected:
	std::string filterFiles(const std::string path, const std::string letter);
};
#endif //SNES_H
