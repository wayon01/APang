#include "stdafx.h"
#include "TileNode.h"


TileNode::TileNode(int id, int x, int y, int z): id(id), x(x), y(y), z(z) {
}


TileNode::~TileNode() {}


void TileNode::setPosition(float x, float y, float z) {
	Position.x = x;
	Position.y = y;
	Position.z = z;
}
