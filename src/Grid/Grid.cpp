#include "Grid.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

Grid::Grid(std::string dataFilePath) {
	Grid::dataFilePath = dataFilePath;
	std::ifstream file(dataFilePath);

	// Check that the file is found and valid.
	if (!file.good()) {
		std::cerr << "Failed to load resource: " << dataFilePath << std::endl;
		_exit(0);
	}

	Grid::ReadHeaders(file);

	/*
	std::string rawInput;

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
	}*/

	file.close();
}

/**
 * Reads the file header for information and sets the Grid values listed..
 *
 * @param rawStream Raw input stream to read the headers from
 */
void Grid::ReadHeaders(std::istream &rawStream) {
	std::string line;

	while (std::getline(rawStream, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__METADATA_BEGIN__") {
			Grid::ReadMetadata(rawStream);
		} else if (tag == "__SHORTCUTS_BEGIN__") {
			Grid::ReadShortcuts(rawStream);
		}
	}
}

/**
 * Read metadata from a file and stores the information into their respective variables
 * until the __METADATA_END__ tag is reached. Prints out errors in metadata information.
 * Assumes that some form of processing has put rawFile to point right after the newline
 * of the __METADATA_BEGIN__ tag.
 *
 * @param rawStream Raw input stream to read the metadata from
 */
void Grid::ReadMetadata(std::istream &rawStream) {
	std::map<std::string, std::string> metadataMap;

	std::string line;
	while (std::getline(rawStream, line)) {
		std::istringstream lineStream(line);

		std::string key;
		std::getline(lineStream, key, '\t');

		if (key == "__METADATA_END__") {
			break;
		}

		for (size_t i = 0; i < key.length(); i++) {
			key[i] = std::tolower(key.at(i));
		}

		std::string value;
		std::getline(lineStream, value, '\t');

		if (!metadataMap.emplace(key, value).second) {
			std::cerr << "Duplicate metadata entry in " << Grid::dataFilePath << std::endl;
		}
	}

	Grid::width =
			metadataMap.find("width") ?
					std::stoull(metadataMap.at("width")) : 0;
	Grid::height =
			metadataMap.find("height") ?
					std::stoull(metadataMap.at("height")) : 0;
	Grid::name =
			metadataMap.find("name") ?
					metadataMap.at("name") : Grid::dataFilePath;
	Grid::author = metadataMap.find("author") ? metadataMap.at("author") : "";
	Grid::description =
			metadataMap.find("description") ?
					metadataMap.at("description") : "";
	Grid::targetVersion =
			metadataMap.find("target version") ?
					metadataMap.at("target version") : "";
	Grid::mapVersion =
			metadataMap.find("map version") ?
					metadataMap.at("map version") : "";

	// Check that width and height are valid. Print error if check fails.
	if (Grid::GRID_SIZE_LIMIT / Grid::width < Grid::height) {
		std::cerr << "Grid size of " << Grid::dataFilePath << " exceed "
				<< Grid::GRID_SIZE_LIMIT << std::endl;
	} else if (width == 0 || height == 0) {
		std::cerr << "Width or height of " << Grid::dataFilePath
				<< " is zero or undefined!" << std::endl;
	}
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
			stream << std::setw(maxNameLength);

			if ((row * Grid::width) + column == position) {
				stream << "X";
			} else {
				stream << Grid::areas.at((row * Grid::width) + column);
			}

			gridRepresentation.append(stream.str() + "|");
		}
		gridRepresentation.append("\n");
	}

	return gridRepresentation;
}
