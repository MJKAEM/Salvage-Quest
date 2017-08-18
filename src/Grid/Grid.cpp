#include "Grid.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

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

	// Check that width and height do not exceed limit.
	if (std::numeric_limits<unsigned long long>::max() / Grid::width
			< Grid::height) {
		std::cerr << "Width and height of " << dataFilePath << " exceeds "
				<< std::numeric_limits<unsigned long long>::max() << std::endl;
		_exit(0);
	}

	// Read file until empty.
	for (unsigned long long row = 0; row < Grid::height; row++) {
		std::getline(file, rawInput);
		std::istringstream lineStream(rawInput);

		unsigned long long column = 0;
		while (std::getline(lineStream, rawInput, '\t')) {
			if (column >= Grid::width) {
				break;
			}

			Grid::areas.push_back(rawInput);
			column++;
		}

		while (column < Grid::width) {
			Grid::areas.push_back("");
			column++;
		}
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
unsigned long long Grid::GetAdjacentPosition(unsigned long long sourcePosition,
		Direction direction) {
	switch (direction) {
	case NORTH:
		return (sourcePosition >= Grid::width) ?
				(sourcePosition - Grid::width) : sourcePosition;

	case WEST:
		return (sourcePosition % Grid::width != 0) ?
				(sourcePosition - 1) : sourcePosition;

	case EAST:
		return ((sourcePosition % Grid::width) < (Grid::width - 1)) ?
				(sourcePosition + 1) : sourcePosition;

	case SOUTH:
		return (sourcePosition < (Grid::width * (Grid::height - 1))) ?
				(sourcePosition + Grid::width) : sourcePosition;

	default:
		std::cerr << "Invalid direction at: " << __FILE__ << ", line: "
				<< __LINE__ << std::endl;
		_exit(0);
		return sourcePosition;
	}
}

std::string Grid::ToString() {
	return Grid::ToString(Grid::areas.size());
}

std::string Grid::ToString(unsigned long long position) {
	size_t maxNameLength = 0;

	for (unsigned long long i = 0; i < Grid::areas.size(); i++) {
		maxNameLength = std::max<size_t>(Grid::areas.at(i).length(),
				maxNameLength);
	}

	std::string gridRepresentation;

	for (unsigned long long row = 0; row < Grid::height; row++) {
		gridRepresentation.append("|");
		for (unsigned long long column = 0; column < Grid::width; column++) {
			std::ostringstream stream;
			stream << std::setw(maxNameLength)
					<< Grid::areas.at((row * Grid::width) + column);
			gridRepresentation.append(stream.str() + "|");
		}
		gridRepresentation.append("\n");
	}

	return gridRepresentation;
}
