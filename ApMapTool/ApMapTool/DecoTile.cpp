#include "stdafx.h"
#include "DecoTile.h"


DecoTile::DecoTile(int id, int x, int y, int z, bool isTile) : TileNode(id ,x, y, z) {
	m_additionalBooleanType.insert(std::pair<std::string, bool>("Ÿ���ν�", isTile));
}


DecoTile::~DecoTile() {}
