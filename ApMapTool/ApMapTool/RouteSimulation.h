#pragma once
#include "TileNode.h"
#include <vector>

struct TileAreaValue {
	TileNode* front = nullptr;
	TileNode* back = nullptr;

	int id_front = -1;
	int id_back = -1;

	bool isPassed = false;

};

struct TileArea {
	TileAreaValue* x = nullptr;
	TileAreaValue* z = nullptr;
};

class RouteSimulation {
public:
	RouteSimulation();
	~RouteSimulation();

	void SetMapSize(int x, int y, int z);
	void ClearMap();

	TileArea* getTileArea() const {
		return m_tileArea;
	}

	void Start(std::vector<TileNode*> map);
	
private:
	void SetTileArea(std::vector<TileNode*> map, Vector3<int>& pos) const;
	void FindWay(Vector3<int>& curPos, int rotationId);
	bool AnalyzingTile(int row, int column, const Vector3<int>& curPos, bool isPositionX,
		bool isSurfaceFront, Vector3<int>& prevPos, Vector3<int> prevAllPos[4], int rotationId);
	bool isOverlapTile(Vector3<int> pos, Vector3<int> prevPos[4]) const;

	bool OnJudgePlayerMovable(Vector3<int> curPos, TileNode& tile) const;
	TileNode* GetSurfaceTile(Vector3<int> pos) const;
	//TileNode* GetSurfaceTile(Vector2<int> pos) const;
	void SetTilePassed(bool passed, Vector3<int> pos) const;
	bool isTilePassed(Vector3<int> pos) const;
	bool isTilePassed(Vector3<int> pos, bool isPositionX) const;
	//Vector2<int> RefreshCurPos(TileNode* tile, bool isPositionX) const;

private:
	TileArea* m_tileArea;

	int size_x;
	int size_y;
	int size_z;

	bool m_isPositionX;
	bool m_isSurfaceFront;

	int m_arrivedCount;
	
};

