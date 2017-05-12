#pragma once
#include "TileNode.h"
class DecoTile :
	public TileNode {
public:
	DecoTile(int id, int x, int y, int z, bool isTile);
	~DecoTile();
};

