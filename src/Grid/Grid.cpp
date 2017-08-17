#include "Grid.h"

#include <fstream>
#include <iostream>

Grid::Grid(std::string dataFilePath) {
	std::ifstream file(dataFilePath);

	// Check that the file is found and valid.
	if (!file.good()) {
		std::cerr << "Failed to load resource: " << dataFilePath << std::endl;
		_exit(0);
	}

	// Read width and height headers.
	std::string rawInput;

	std::getline(file, rawInput, '\t');
	std::getline(file, rawInput);

	Grid::width = std::stoull(rawInput);

	std::getline(file, rawInput, '\t');
	std::getline(file, rawInput);
	Grid::height = std::stoull(rawInput);

	// Read file until empty.
	for (unsigned long long row = 0; row < Grid::height; row++) {
		for (unsigned long long column = 0; column < Grid::width - 1; column++) {
			std::getline(file, rawInput, '\t');
			Grid::areas.push_back(rawInput);
		}

		std::getline(file, rawInput);
		Grid::areas.push_back(rawInput);
	}

	file.close();
}
