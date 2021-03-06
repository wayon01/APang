#pragma once
#include "TileNode.h"
#include <vector>
#include "RouteSimulation.h"
#include <thread>
#include "MapNode.h"
#include "TileShortcut.h"

typedef MapNode* MapVector;


class MapProc {
public:
	MapProc();
	~MapProc();

	void SetTile(int id, int x, int y, int z);
	void SetTile(int id, int x, int y, int z, int nextStageId);
	void SetDecoTile(int id, int x, int y, int z, bool isEnable);
	void SetWarpTile(int id, int x, int y, int z, int nextStageId, std::string name, std::string targetName);
	void SetMapSize(int x, int y, int z);
	void RefreshMap();
	void ClearMap();
	void ClearMap(int stageId);
	void ClearAllMap();
	void DeleteTile(int x, int y, int z);
	void DeleteTile(int stageId, int x, int y, int z);
	void DeleteMap(int stageId);

	std::vector<TileNode*> GetTiles() {
		if(m_map_stage.size() > 0)
			return m_map_stage[m_stageId]->m_map;

		return std::vector<TileNode*>();
	}

	TileNode* GetTile(ivec3 posId);

	TileShortcut GetTileShortcut(std::string name);

	std::map<int, MapVector> GetMapStage() const {
		return m_map_stage;
	}

	void MapStagePushBack(int stageId, MapVector obj) {
		m_map_stage.insert(std::pair<int, MapVector>(stageId, obj));
	}

	bool isEmptyStage() const {
		return m_map_stage.size() <= 0;
	}

	vec3 getMapSize() {
		vec3 tmp(m_map_stage[m_stageId]->size_x, m_map_stage[m_stageId]->size_y, m_map_stage[m_stageId]->size_z);
		return tmp;
	}

	TileNode& GetCurTile() const {
		return *m_curTile;
	}

	int GetStageId() const {
		return m_stageId;
	}

	void SetCurTileId(int id) {
		m_curTile->id = id;
	}

	void SetStageId(int id) {
		m_stageId = id;
	}

	void SetCurTile(Vector3<int> pos) {
		m_curTile->x = pos.x;
		m_curTile->y = pos.y;
		m_curTile->z = pos.z;
		SetTilePositionByIdPos(m_curTile, pos.x, pos.y, pos.z);
	}

	void SetTilePositionByIdPos(TileNode* node, int x, int y, int z);
	bool isNullptr(int x, int y, int z);
	bool isNullptr(int stageId, int x, int y, int z);
	void StartRouteSimulation();

private:
	//int size_x;
	//int size_y;
	//int size_z;

	int m_stageId;

	std::map<int, MapVector> m_map_stage;
	std::vector<TileShortcut> m_map_shortcut;
	TileNode* m_curTile;

	RouteSimulation* m_routeSimulation;
	//std::thread simulationThread;

public:
	bool b_isMapSizeChanged;
};

