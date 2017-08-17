#ifndef GRID_GRID_H_
#define GRID_GRID_H_

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

	Grid();

public:
	Grid(std::string);
	unsigned long long GetAdjacentPosition(unsigned long long, Direction);
};

#endif /* GRID_GRID_H_ */
