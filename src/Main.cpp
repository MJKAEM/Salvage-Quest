#include <iostream>
#include <string>
#include "Commands/Commands.h"

int main() {
	Commands::Initialize();

	while (true) {
		std::string input;
		std::cin >> input;

		unsigned long long commandId = Commands::GetCommandFromString(input);

		std::cout << commandId << std::endl;

		if (commandId == 0) {
			break;
		}
	}

	return 0;
}
