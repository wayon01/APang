#include "stdafx.h"
#include "MapProc.h"
#include "MacroDef.h"
#include "ResourceManager.h"


MapProc::MapProc(): size_x(0), size_y(0), size_z(0), m_curTile(new TileNode(0, 0, 0, 0)) {
	m_routeSimulation = new RouteSimulation();
}


MapProc::~MapProc() {
	SAFE_DELETE(m_curTile);
	ClearMap();

	delete m_routeSimulation;
}


void MapProc::SetTile(int id, int x, int y, int z) {

	if ((x < 0) || (y < 0) || (z < 0)) return;
	if (x > size_x || y > size_y || z > size_z) return;
	if (!isNullptr(x, y, z)) return;


	TileNode* tmp = new TileNode(id, x, y, z);

	SetTilePositionByIdPos(tmp, x, y, z);

	m_map.push_back(tmp);

	if(id == RESMGR->GetAssetProc()->getSpawnId()) {
		m_curTile->x = x;
		m_curTile->y = y;
		m_curTile->z = z;
		SetTilePositionByIdPos(m_curTile, x, y, z);
	}

}


void MapProc::SetMapSize(int x, int y, int z) {


	m_routeSimulation->SetMapSize(x, y, z);


	size_x = x-1;
	size_y = y-1;
	size_z = z-1;

	b_isMapSizeChanged = true;
	m_curTile->x = m_curTile->y = m_curTile->z = 0;
	SetTilePositionByIdPos(m_curTile, 0, 0, 0);
}


void MapProc::RefreshMap() {

	for (std::vector<TileNode*>::iterator iter = m_map.begin(); iter != m_map.end();) {
		auto tmp = *iter;

		if (tmp->x > size_x || tmp->y > size_y || tmp->z > size_z) {
			iter = m_map.erase(iter);
			delete tmp;
			continue;
		}

		SetTilePositionByIdPos(tmp, tmp->x, tmp->y, tmp->z);

		++iter;
	}

	m_curTile->x = m_curTile->y = m_curTile->z = 0;
	SetTilePositionByIdPos(m_curTile, 0, 0, 0);

}


void MapProc::ClearMap() {
	for (int i = 0; i < m_map.size(); i++) {
		TileNode* tmp = m_map[i];

		SAFE_DELETE(tmp);

	}
	m_map.clear();

	m_routeSimulation->ClearMap();
}


void MapProc::DeleteTile(int x, int y, int z) {
	for (auto iter = m_map.begin(); iter != m_map.end(); ) {
		auto tmp = *iter;
		if (tmp->x == x && tmp->y == y && tmp->z == z) {
			iter = m_map.erase(iter);
			SAFE_DELETE(tmp);
			break;
		} else
			++iter;
	}
}


void MapProc::SetTilePositionByIdPos(TileNode* node, int x, int y, int z) const {
	float deltaX = (size_x * TILE_SIZE) / 2.f;
	float deltaY = (size_y * TILE_SIZE) / 2.f;
	float deltaZ = (size_z * TILE_SIZE) / 2.f;

	node->setPosition(-(x * TILE_SIZE - deltaX), y * TILE_SIZE - deltaY, (z * TILE_SIZE - deltaZ));

}


bool MapProc::isNullptr(int x, int y, int z) {

	Vector3<int> id = Vector3<int>{ x, y, z };

	for (auto i = m_map.begin(); i != m_map.end(); ++i) {
		auto tmp = *i;
		Vector3<int> tile_id = Vector3<int>{ tmp->x, tmp->y, tmp->z };

		if(id == tile_id) {
			return false;
		}
	}

	return true;
}


void MapProc::StartRouteSimulation() {
	m_routeSimulation->Start(m_map);
}
