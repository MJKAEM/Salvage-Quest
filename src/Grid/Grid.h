#ifndef GRID_GRID_H_
#define GRID_GRID_H_

#include <fstream>
#include <map>
#include <string>
#include <vector>

enum Direction {
	NORTH,
	WEST,
	EAST,
	SOUTH
};

class Grid {
private:
	std::vector<std::string> areas;
	unsigned long long width, height;
	std::string dataFilePath;
	std::string name, author, description;
	std::string targetVersion, mapVersion;

	Grid();
	void ReadHeaders(std::istream&);
	void ReadMetadata(std::istream&);
	void ReadShortcuts(std::istream&);

public:
	const unsigned long long GRID_SIZE_LIMIT = std::numeric_limits<unsigned long long>::max();

	Grid(std::string);
	unsigned long long GetAdjacentPosition(unsigned long long, Direction);
	std::string ToString();
	std::string ToString(unsigned long long);

	unsigned long long GetWidth();
	unsigned long long GetHeight();
	std::string GetName();
	std::string GetAuthor();
	std::string GetDescription();
	std::string GetTargetVersion();
	std::string GetMapVersion();
};

#endif /* GRID_GRID_H_ */
