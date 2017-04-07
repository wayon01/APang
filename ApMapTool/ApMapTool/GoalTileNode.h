#pragma once
#include "TileNode.h"
class GoalTileNode : public TileNode {
public:
	GoalTileNode(int id, int x, int y, int z, int stageId);
	~GoalTileNode();

public:
	//int stageId;
};

