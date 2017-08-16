#include <iostream>
#include "Commands/Commands.h"

int main() {
	Commands::Initialize();

	std::cout << Commands::GetCommandFromString("lol") << std::endl;
	return 0;
}
