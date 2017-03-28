#pragma once
#include "TileNode.h"
#include <vector>
#include "RouteSimulation.h"
#include <thread>


class MapProc {
public:
	MapProc();
	~MapProc();

	void SetTile(int id, int x, int y, int z);
	void SetMapSize(int x, int y, int z);
	void RefreshMap();
	void ClearMap();
	void DeleteTile(int x, int y, int z);

	std::vector<TileNode*> GetTiles() const {
		return m_map;
	}

	vec3 getMapSize() const {
		vec3 tmp(size_x, size_y, size_z);
		return tmp;
	}

	TileNode& GetCurTile() const {
		return *m_curTile;
	}

	void SetCurTileId(int id) {
		m_curTile->id = id;
	}

	void SetCurTile(Vector3<int> pos) {
		m_curTile->x = pos.x;
		m_curTile->y = pos.y;
		m_curTile->z = pos.z;
		SetTilePositionByIdPos(m_curTile, pos.x, pos.y, pos.z);
	}

	void SetTilePositionByIdPos(TileNode* node, int x, int y, int z) const;
	bool isNullptr(int x, int y, int z);
	void StartRouteSimulation();

private:
	int size_x;
	int size_y;
	int size_z;
	std::vector<TileNode*> m_map;
	TileNode* m_curTile;

	RouteSimulation* m_routeSimulation;
	//std::thread simulationThread;

public:
	bool b_isMapSizeChanged;
};

