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

	Grid();
	void ReadHeaders(std::istream&);
	void ReadMetadata(std::istream&);
	void ReadShortcuts(std::istream&);

public:
	Grid(std::string);
	unsigned long long GetAdjacentPosition(unsigned long long, Direction);
	std::string ToString();
	std::string ToString(unsigned long long);
};

#endif /* GRID_GRID_H_ */
