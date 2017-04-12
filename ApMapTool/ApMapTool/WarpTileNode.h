#pragma once
#include "GoalTileNode.h"
class WarpTileNode :
	public GoalTileNode {
public:
	WarpTileNode(int id, int x, int y, int z, int stageId, std::string name, std::string targetName);
	~WarpTileNode();
};

