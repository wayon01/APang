#include "stdafx.h"
#include "WarpTileNode.h"


WarpTileNode::WarpTileNode(int id, int x, int y, int z, int stageId, std::string name, std::string targetName) : GoalTileNode(id, x, y, z, stageId) {
	m_additionalIntegerType["Area ID"] = stageId;
	m_additionalStringType.insert(std::pair<std::string, std::string>("타일 이름", name));
	m_additionalStringType.insert(std::pair<std::string, std::string>("이동할 타일 이름", targetName));
}


WarpTileNode::~WarpTileNode() {}
