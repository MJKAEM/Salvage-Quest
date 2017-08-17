#ifndef GRID_GRID_H_
#define GRID_GRID_H_

#include <string>
#include <vector>

class Grid {
private:
	std::vector<std::string> areas;
	unsigned long long width, height;

	Grid();

public:
	Grid(std::string);
};

#endif /* GRID_GRID_H_ */
