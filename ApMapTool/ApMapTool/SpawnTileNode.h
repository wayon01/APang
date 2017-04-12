#pragma once
#include "TileNode.h"
class SpawnTileNode :
	public TileNode {
public:
	SpawnTileNode(int id, int x, int y, int z, int stageId);
	~SpawnTileNode();
};

