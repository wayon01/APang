#include "stdafx.h"
#include "SpawnTileNode.h"


SpawnTileNode::SpawnTileNode(int id, int x, int y, int z, int stageId) : TileNode(id, x, y, z) {
	m_additionalIntegerType.insert(std::pair<std::string, int>("¸µÅ©µÉ ID", stageId));
}


SpawnTileNode::~SpawnTileNode() {}
