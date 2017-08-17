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

/**
 * Gets the adjacent position in the specified direction from the source position.
 * If the adjacent position is out of bounds, then return the source position.
 *
 * @param sourcePosition Position to consider the 'center'
 * @param direction Cardinal direction of the adjacent position
 */
unsigned long long Grid::GetAdjacentPosition(unsigned long long sourcePosition, Direction direction) {
	switch(direction) {
	case NORTH:
		return (sourcePosition >= Grid::width) ? (sourcePosition - Grid::width) : sourcePosition;

	case WEST:
		return (sourcePosition % Grid::width != 0) ? (sourcePosition - 1) : sourcePosition;

	case EAST:
		return ((sourcePosition % Grid::width) < (Grid::width - 1)) ? (sourcePosition + 1) : sourcePosition;

	case SOUTH:
		return (sourcePosition < (Grid::width * (Grid::height - 1))) ? (sourcePosition + Grid::width) : sourcePosition;

	default:
		std::cerr << "Invalid direction at: " << __FILE__ << ", line: " << __LINE__ << std::endl;
		_exit(0);
		return sourcePosition;
	}
}
