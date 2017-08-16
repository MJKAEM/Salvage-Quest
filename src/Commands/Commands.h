#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include <map>

class Commands {
private:
	static std::map<std::string, unsigned int> stringToCommandMap;
public:
	static void Initialize();
	static unsigned int GetCommandFromString(std::string);
};

#endif /* COMMANDS_H_ */
