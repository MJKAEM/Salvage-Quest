#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <map>
#include <string>

class Commands {
private:
	static std::map<std::string, unsigned long long> commandToIdMap;
public:
	static void Initialize();
	static void ReadDataFile();
	static unsigned long long GetCommandFromString(std::string);
};

#endif /* COMMANDS_H_ */
