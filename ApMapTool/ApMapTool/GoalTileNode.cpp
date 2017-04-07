#include "stdafx.h"
#include "GoalTileNode.h"
#include <string>


GoalTileNode::GoalTileNode(int id, int x, int y, int z, int stageId) : TileNode(id, x, y, z)/*, stageId(stageId)*/ {
	m_additionalIntegerType.insert(std::pair<std::string, int>("Area ID", stageId));
}


GoalTileNode::~GoalTileNode() {}
