#include <iostream>
#include <string>

#include "Commands/Commands.h"
#include "DataFileNames.h"
#include "Grid/Grid.h"

int main() {
	Commands::Initialize();
	Grid overworld(RawData::overworldFile);

	while (true) {
		std::string input;
		std::cin >> input;

		unsigned long long commandId = Commands::GetCommandFromString(input);

		std::cout << commandId << std::endl;

		if (commandId == 0) {
			break;
		} else if (commandId == 4) {
			std::cout << overworld.ToString() << std::endl;
		}
	}

	return 0;
}
