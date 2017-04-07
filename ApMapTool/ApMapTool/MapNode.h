#pragma once
#include <vector>
#include "TileNode.h"

struct MapNode {

	std::vector<TileNode*> m_map;
	int size_x;
	int size_y;
	int size_z;

	int id = 0;

};

