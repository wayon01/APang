#include "stdafx.h"
#include "WarpTileNode.h"


WarpTileNode::WarpTileNode(int id, int x, int y, int z, int stageId, std::string name, std::string targetName) : GoalTileNode(id, x, y, z, stageId) {
	m_additionalIntegerType["Area ID"] = stageId;
	m_additionalStringType.insert(std::pair<std::string, std::string>("Ÿ�� �̸�", name));
	m_additionalStringType.insert(std::pair<std::string, std::string>("�̵��� Ÿ�� �̸�", targetName));
}


WarpTileNode::~WarpTileNode() {}
