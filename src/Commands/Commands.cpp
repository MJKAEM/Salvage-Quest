#include "Commands.h"

#include <iostream>
#include <fstream>
#include "../DataFileNames.h"

std::map<std::string, unsigned long long> Commands::commandToIdMap;

void Commands::Initialize() {
	Commands::ReadDataFile();
}

void Commands::ReadDataFile(){
	std::ifstream file(RawData::commandsFile);

	// Check that the file is found and valid.
	if (!file.good()) {
		std::cerr << "Failed to load resource: " << RawData::commandsFile << std::endl;
		std::exit(0);
	}

	// Ignore first line.
	std::string ignore;
	std::getline(file, ignore);

	// Read file until empty for all commands.
	std::string rawCommand;
	while (std::getline(file, rawCommand, '\t')) {
		std::string rawId;

		std::getline(file, rawId);
		unsigned long long id = std::stoull(rawId);

		Commands::commandToIdMap.insert(std::pair<std::string, unsigned long long>(rawCommand, id));
	}

	file.close();
}

unsigned long long Commands::GetCommandFromString(std::string rawCommand) {
	return Commands::commandToIdMap.count(rawCommand) ? Commands::commandToIdMap.at(rawCommand) : Commands::commandToIdMap.at("invalid");
}
