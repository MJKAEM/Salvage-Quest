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
		std::exit(0);
	}

	Grid::ReadHeaders(file);

	file.close();
}

/**
 * Searches the data file for the __HEADER_BEGIN__ tag. Next, it reads the
 * headers and calls other header-related functions.
 */
void Grid::ReadHeaders() {
	std::ifstream file(Grid::dataFilePath);
	std::string line;

	// Find __HEADER_BEGIN__ tag.
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__HEADER_BEGIN__") {
			Grid::ReadHeaders(file);
			break;
		}
	}
}

/**
 * Reads the file header from a file and calls other header-related
 * functions until the __HEADER_END__ tag is reached. Assumes that some form
 * of processing has put the stream to directly after the newline of the
 * __HEADER_BEGIN__ tag.
 *
 * @param rawStream Raw input stream to read the headers from
 */
void Grid::ReadHeaders(std::istream& rawStream) {
	std::string line;

	while (std::getline(rawStream, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__METADATA_BEGIN__") {
			Grid::ReadMetadata(rawStream);
		} else if (tag == "__SHORTCUTS_BEGIN__") {
			Grid::ReadShortcuts(rawStream);
		} else if (tag == "__HEADER_END__") {
			break;
		}
	}
}

/**
 * Searches the data file for the __METADATA_BEGIN__ tag. Next, it reads the
 * metadata and stores the information to its corresponding variables.
 */
void Grid::ReadMetadata() {
	std::ifstream file(Grid::dataFilePath);
	std::string line;

	// Find __HEADER_BEGIN__ tag.
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__HEADER_BEGIN__") {
			break;
		}
	}

	// Find __METADATA_BEGIN__ tag.
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__METADATA_BEGIN__") {
			Grid::ReadMetadata(file);
			break;
		}
	}
}

/**
 * Reads metadata from a file and stores the information into their respective
 * variables until the __METADATA_END__ tag is reached. Prints out errors in
 * metadata information. Assumes that some form of processing has put the
 * stream to directly after the newline of the __METADATA_BEGIN__ tag.
 *
 * @param rawStream Raw input stream to read the metadata from
 */
void Grid::ReadMetadata(std::istream& rawStream) {
	std::string line;
	while (std::getline(rawStream, line)) {
		std::istringstream lineStream(line);

		std::string key;
		std::getline(lineStream, key, '\t');

		if (key == "__METADATA_END__") {
			break;
		}

		for (size_t i = 0; i < key.length(); i++) {
			key.at(i) = std::tolower(key.at(i));
		}

		std::string value;
		std::getline(lineStream, value, '\t');

		if (!Grid::additionalMetadataMap.emplace(key, value).second) {
			std::cerr << "Duplicate metadata entry in " << Grid::dataFilePath
					<< std::endl;
		}
	}

	Grid::width =
			Grid::additionalMetadataMap.count("width") ?
					std::stoull(Grid::additionalMetadataMap.at("width")) : 0;
	Grid::additionalMetadataMap.erase("width");
	Grid::height =
			Grid::additionalMetadataMap.count("height") ?
					std::stoull(Grid::additionalMetadataMap.at("height")) : 0;
	Grid::additionalMetadataMap.erase("height");
	Grid::name =
			Grid::additionalMetadataMap.count("name") ?
					Grid::additionalMetadataMap.at("name") : Grid::dataFilePath;
	Grid::additionalMetadataMap.erase("name");
	Grid::author =
			Grid::additionalMetadataMap.count("author") ?
					Grid::additionalMetadataMap.at("author") : "";
	Grid::additionalMetadataMap.erase("author");
	Grid::description =
			Grid::additionalMetadataMap.count("description") ?
					Grid::additionalMetadataMap.at("description") : "";
	Grid::additionalMetadataMap.erase("description");
	Grid::targetVersion =
			Grid::additionalMetadataMap.count("target version") ?
					Grid::additionalMetadataMap.at("target version") : "";
	Grid::additionalMetadataMap.erase("target version");
	Grid::fileVersion =
			Grid::additionalMetadataMap.count("file version") ?
					Grid::additionalMetadataMap.at("file version") : "";
	Grid::additionalMetadataMap.erase("file version");

	// Check that width and height are valid. Print error if check fails.
	if (width == 0 || height == 0) {
		std::cerr << "Width or height of " << Grid::dataFilePath
				<< " is zero or undefined!" << std::endl;
	} else if (Grid::GRID_SIZE_LIMIT / Grid::width < Grid::height) {
		std::cerr << "Grid size of " << Grid::dataFilePath << " exceed "
				<< Grid::GRID_SIZE_LIMIT << std::endl;
	}
}

/**
 * Searches the data file for the __SHORTCUTS_BEGIN__ tag. Next, it reads the
 * shortcuts and stores the information into a map of shortcuts to area names.
 */
void Grid::ReadShortcuts() {
	std::ifstream file(Grid::dataFilePath);
	std::string line;

	// Find __HEADER_BEGIN__ tag.
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__HEADER_BEGIN__") {
			break;
		}
	}

	// Find __SHORTCUTS_BEGIN tag.
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__SHORTCUTS_BEGIN__") {
			Grid::ReadShortcuts(file);
			break;
		}
	}
}

/**
 * Reads shortcuts from a file and stores the information into a map of
 * shortcuts to area names until the __SHORTCUTS_END__ tag is reached. Assumes
 * that some form of processing has put the stream directly after the newline
 * of the __SHORTCUTS_BEGIN__ tag.
 *
 * @param rawStream Raw input stream to read the shortcuts from
 */
void Grid::ReadShortcuts(std::istream& rawStream) {
	std::string line;
	while (std::getline(rawStream, line)) {
		std::istringstream lineStream(line);

		std::string key;
		std::getline(lineStream, key, '\t');

		if (key == "__SHORTCUTS_END__") {
			break;
		}

		std::string value;
		std::getline(lineStream, value, '\t');

		if (!Grid::shortcutToAreaNameMap.emplace(key, value).second) {
			std::cerr << "Duplicate shortcut entry in " << Grid::dataFilePath
					<< std::endl;
		}

		Grid::shortcutToAreaNameMap.emplace(key, value);
	}
}

/**
 * Searches the data file for the __BODY_START__ tag. Next, it reads the body
 * and stores the information into a vector of area names until the
 * __BODY_END__ tag is reached or the areas specified have been read.
 */
void Grid::ReadBody() {
	std::ifstream file(Grid::dataFilePath);
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream lineStream(line);

		std::string tag;
		std::getline(lineStream, tag, '\t');

		if (tag == "__BODY_BEGIN__") {
			Grid::ReadBody(file);
			break;
		}
	}
}

/**
 * Reads the body from a file and stores the information into a vector of area
 * names until the __BODY_END__ tag is reached or the areas specified have
 * been read. Assumes that some form of processing has put the stream directly
 * after the newline of the __BODY_BEGIN__ tag.
 *
 * @param rawStream Raw input stream to read the body from
 */
void Grid::ReadBody(std::istream& rawStream) {
	std::string line;

	while (std::getline(rawStream, line)) {
		std::istringstream lineStream(line);
		std::string areaName;
		unsigned long long column = 0;

		// Check if the leftmost entry is the end tag and break if it is.
		// If not, then read as body.
		if (std::getline(lineStream, areaName, '\t')) {
			if (areaName == "__BODY_END__" || column >= Grid::width) {
				break;
			}

			do {
				if (column >= Grid::width) {
					break;
				}

				Grid::areas.push_back(areaName);
				column++;
			} while (std::getline(lineStream, areaName, '\t'));
		}

		// Fill in empty remaining slots.
		for (unsigned long long i = column; i < Grid::width; i++) {
			Grid::areas.push_back("");
		}
	}

	Grid::areas.resize(Grid::width * Grid::height);
}

/**
 * Gets the adjacent position in the specified direction from the source
 * position. If the adjacent position is out of bounds, then return the source
 * position.
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
		std::exit(0);
		return sourcePosition;
	}
}

std::string Grid::ToString() {
	return Grid::ToString(Grid::areas.size());
}

std::string Grid::ToString(unsigned long long position) {
	if (Grid::areas.empty() != Grid::width * Grid::height) {
		Grid::ReadBody();
	}

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

unsigned long long Grid::GetWidth() {
	return Grid::width;
}

unsigned long long Grid::GetHeight() {
	return Grid::height;
}

std::string Grid::GetName() {
	return Grid::name;
}

std::string Grid::GetAuthor() {
	return Grid::author;
}

std::string Grid::GetDescription() {
	return Grid::description;
}

std::string Grid::GetTargetVersion() {
	return Grid::targetVersion;
}

std::string Grid::GetMapVersion() {
	return Grid::fileVersion;
}
