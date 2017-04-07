#include "stdafx.h"
#include "MapProc.h"
#include "MacroDef.h"
#include "ResourceManager.h"
#include "GoalTileNode.h"


MapProc::MapProc(): /*size_x(0), size_y(0), size_z(0),*/ m_curTile(new TileNode(0, 0, 0, 0)) {
	m_routeSimulation = new RouteSimulation();
	m_stageId = 0;

	auto tmp = new MapNode();
	m_map_stage.insert(std::pair<int, MapVector>(tmp->id, tmp));
}


MapProc::~MapProc() {
	SAFE_DELETE(m_curTile);
	ClearAllMap();

	delete m_routeSimulation;
}


void MapProc::SetTile(int id, int x, int y, int z) {

	if ((x < 0) || (y < 0) || (z < 0)) return;
	if (x > m_map_stage[m_stageId]->size_x || y > m_map_stage[m_stageId]->size_y || z > m_map_stage[m_stageId]->size_z) return;
	if (!isNullptr(x, y, z)) return;


	TileNode* tmp = new TileNode(id, x, y, z);

	SetTilePositionByIdPos(tmp, x, y, z);

	m_map_stage[m_stageId]->m_map.push_back(tmp);

	if(id == RESMGR->GetAssetProc()->getSpawnId()) {
		m_curTile->x = x;
		m_curTile->y = y;
		m_curTile->z = z;
		SetTilePositionByIdPos(m_curTile, x, y, z);
	}

}


void MapProc::SetTile(int id, int x, int y, int z, int nextStageId) {
	if ((x < 0) || (y < 0) || (z < 0)) return;
	if (x > m_map_stage[m_stageId]->size_x || y > m_map_stage[m_stageId]->size_y || z > m_map_stage[m_stageId]->size_z) return;
	if (!isNullptr(x, y, z)) return;


	GoalTileNode* tmp = new GoalTileNode(id, x, y, z, nextStageId);

	SetTilePositionByIdPos(tmp, x, y, z);

	m_map_stage[m_stageId]->m_map.push_back(tmp);

	if (id == RESMGR->GetAssetProc()->getSpawnId()) {
		m_curTile->x = x;
		m_curTile->y = y;
		m_curTile->z = z;
		SetTilePositionByIdPos(m_curTile, x, y, z);
	}
}


void MapProc::SetMapSize(int x, int y, int z) {


	m_routeSimulation->SetMapSize(x, y, z);


	m_map_stage[m_stageId]->size_x = x-1;
	m_map_stage[m_stageId]->size_y = y-1;
	m_map_stage[m_stageId]->size_z = z-1;

	b_isMapSizeChanged = true;
	m_curTile->x = m_curTile->y = m_curTile->z = 0;
	SetTilePositionByIdPos(m_curTile, 0, 0, 0);
}


void MapProc::RefreshMap() {

	for (std::vector<TileNode*>::iterator iter = m_map_stage[m_stageId]->m_map.begin(); iter != m_map_stage[m_stageId]->m_map.end();) {
		auto tmp = *iter;

		if (tmp->x > m_map_stage[m_stageId]->size_x || tmp->y > m_map_stage[m_stageId]->size_y || tmp->z > m_map_stage[m_stageId]->size_z) {
			iter = m_map_stage[m_stageId]->m_map.erase(iter);
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
	ClearMap(m_stageId);
}


void MapProc::ClearMap(int stageId) {
	for (int i = 0; i < m_map_stage[stageId]->m_map.size(); i++) {
		TileNode* tmp = m_map_stage[stageId]->m_map[i];

		SAFE_DELETE(tmp);

	}
	m_map_stage[stageId]->m_map.clear();

	m_routeSimulation->ClearMap();
}


void MapProc::ClearAllMap() {
	int i = 0;
	for(auto mapObj : m_map_stage) {
		ClearMap(i);
		auto tmp = mapObj.second;
		SAFE_DELETE(tmp);

		i++;
	}
	m_map_stage.clear();
	m_stageId = 0;
}


void MapProc::DeleteTile(int x, int y, int z) {
	DeleteTile(m_stageId, x, y, z);
}


void MapProc::DeleteTile(int stageId, int x, int y, int z) {
	for (auto iter = m_map_stage[stageId]->m_map.begin(); iter != m_map_stage[stageId]->m_map.end(); ) {
		auto tmp = *iter;
		if (tmp->x == x && tmp->y == y && tmp->z == z) {
			iter = m_map_stage[stageId]->m_map.erase(iter);
			SAFE_DELETE(tmp);
			break;
		} else
			++iter;
	}
}


void MapProc::DeleteMap(int stageId) {
		
	int i = 0;
	for(auto iter = m_map_stage.begin(); iter != m_map_stage.end(); i++) {
		auto tmp = iter->second;
		if(tmp->id == stageId) {
			ClearMap(i);
			iter = m_map_stage.erase(iter);
			SAFE_DELETE(tmp);
			break;
		}
	}

}


TileNode* MapProc::GetTile(ivec3 posId) {
	for (auto iter = m_map_stage[m_stageId]->m_map.begin(); iter != m_map_stage[m_stageId]->m_map.end(); ) {
		auto tmp = *iter;
		if (tmp->x == posId.x && tmp->y == posId.y && tmp->z == posId.z) {
			return tmp;
		}

		++iter;
	}
	return nullptr;
}


void MapProc::SetTilePositionByIdPos(TileNode* node, int x, int y, int z) {
	float deltaX = (m_map_stage[m_stageId]->size_x * TILE_SIZE) / 2.f;
	float deltaY = (m_map_stage[m_stageId]->size_y * TILE_SIZE) / 2.f;
	float deltaZ = (m_map_stage[m_stageId]->size_z * TILE_SIZE) / 2.f;

	node->setPosition(-(x * TILE_SIZE - deltaX), y * TILE_SIZE - deltaY, (z * TILE_SIZE - deltaZ));

}


bool MapProc::isNullptr(int x, int y, int z) {
	return isNullptr(m_stageId, x, y, z);
	
}


bool MapProc::isNullptr(int stageId, int x, int y, int z) {
	Vector3<int> id = Vector3<int>{ x, y, z };

	for (auto i = m_map_stage[stageId]->m_map.begin(); i != m_map_stage[stageId]->m_map.end(); ++i) {
		auto tmp = *i;
		Vector3<int> tile_id = Vector3<int>{ tmp->x, tmp->y, tmp->z };

		if (id == tile_id) {
			return false;
		}
	}

	return true;
}


void MapProc::StartRouteSimulation() {
	m_routeSimulation->Start(m_map_stage[m_stageId]->m_map);
}
