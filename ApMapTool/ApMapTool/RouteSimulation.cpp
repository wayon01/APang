#include "stdafx.h"
#include "RouteSimulation.h"
#include "MacroDef.h"
#include "ResourceManager.h"


RouteSimulation::RouteSimulation(): m_tileArea(nullptr), size_x(0), size_y(0), size_z(0) {

}


RouteSimulation::~RouteSimulation() {

}


void RouteSimulation::SetMapSize(int x, int y, int z) {

	if (m_tileArea != nullptr) {
		for (int i = 0; i < size_y; i++) {
			SAFE_DELETE(m_tileArea[i].x);
			SAFE_DELETE(m_tileArea[i].z);
		}
	}
	SAFE_DELETE_ARRAY(m_tileArea);

	m_tileArea = new TileArea[y];

	for (int i = 0; i < y; i++) {
		m_tileArea[i].x = new TileAreaValue[x];
		m_tileArea[i].z = new TileAreaValue[z];

	}

	size_x = x;
	size_y = y;
	size_z = z;
}


void RouteSimulation::ClearMap() {
	if (m_tileArea != nullptr) {
		for (int i = 0; i < size_y; i++) {
			SAFE_DELETE(m_tileArea[i].x);
			SAFE_DELETE(m_tileArea[i].z);
		}
	}
	SAFE_DELETE_ARRAY(m_tileArea);
}



void RouteSimulation::Start(std::vector<TileNode*> map) {
	if (m_tileArea == nullptr) {

		return;
	}
	if (size_x <= 0 || size_y <= 0 || size_z <= 0) return;

	Vector3<int> curPos = Vector3<int>{ -1, -1, -1 };

	SetTileArea(map, curPos);

	if(curPos.x < 0 || curPos.y < 0 || curPos.z < 0) { //스폰타일이 없어!
		MessageBoxA(nullptr, "스폰 타일이 없습니다.", "중단된 시뮬레이션", MB_OK | MB_ICONWARNING);
		return;
	}

	m_isPositionX = m_isSurfaceFront = true;
	m_arrivedCount = 0;

	//탐색시작
	//Vector2<int> findPos{curPos.x, curPos.y};
	FindWay(curPos, 0);
	std::string resultStr = "경로는 총 ";
	resultStr.append(std::to_string(m_arrivedCount));
	resultStr.append("가지가 있습니다.");
	MessageBoxA(nullptr, resultStr.c_str(), "결과", MB_OK | MB_ICONINFORMATION);
}


void RouteSimulation::SetTileArea(std::vector<TileNode*> map, Vector3<int>& pos) const {

	int spawnTileID = RESMGR->GetAssetProc()->getSpawnId();

	for(auto i = map.begin(); i != map.end(); ++i) {

		auto node = *i;
		int x = node->x;
		int y = node->y;
		int z = node->z;

		if(node->id == spawnTileID) {
			pos.x = node->x;
			pos.y = node->y;
			pos.z = node->z;
		}

		if (m_tileArea[y].z[z].id_front == -1 ||
			m_tileArea[y].z[z].id_front > x) {
			m_tileArea[y].z[z].front = node;
			m_tileArea[y].z[z].id_front = x;
		}

		if (m_tileArea[y].z[z].id_back == -1 ||
			m_tileArea[y].z[z].id_back < x) {
			m_tileArea[y].z[z].back = node;
			m_tileArea[y].z[z].id_back = x;
		}

		if (m_tileArea[y].x[x].id_front == -1 ||
			m_tileArea[y].x[x].id_front > z) {
			m_tileArea[y].x[x].front = node;
			m_tileArea[y].x[x].id_front = z;
		}

		if (m_tileArea[y].x[x].id_back == -1 ||
			m_tileArea[y].x[x].id_back < z) {
			m_tileArea[y].x[x].back = node;
			m_tileArea[y].x[x].id_back = z;
		}
	}

}


void RouteSimulation::FindWay(Vector3<int>& curPos, int rotationId) {

	TileNode* tile;
	bool isPositionX = true, isSurfaceFront = true;
	int row_size, column_size = size_y;

	Vector3<int> prevPos[4] = { Vector3<int>{-1, -1, -1} };

	switch (rotationId) {
	case 0: {
		isPositionX = isSurfaceFront = true;
	}
			break;
	case 1: {
		isPositionX = false;
		isSurfaceFront = true;

	}
			break;
	case 2: {
		isPositionX = true;
		isSurfaceFront = false;
	}
			break;
	case 3: {
		isPositionX = isSurfaceFront = false;
	}
			break;
	}

	m_isPositionX = isPositionX;
	m_isSurfaceFront = isSurfaceFront;
	tile = GetSurfaceTile(curPos);

	if (tile == nullptr) {
		return;
	}

	if (tile->id == RESMGR->GetAssetProc()->getGoalId()) {
		m_arrivedCount++;
		return;
	}

	//이미 돌았던 타일이면 거르기
	if (isTilePassed(curPos, rotationId % 2 == 0)) {
		return;
	}


	SetTilePassed(true, curPos);

	

	for(int i = 0; i < 4; i++, rotationId++) {

		if(rotationId >= 4) {
			rotationId -= 4;
		}

		switch (rotationId) {
		case 0: {
			isPositionX = isSurfaceFront = true;
		}
				break;
		case 1: {
			isPositionX = false;
			isSurfaceFront = true;
			
		}
				break;
		case 2: {
			isPositionX = true;
			isSurfaceFront = false;
		}
				break;
		case 3: {
			isPositionX = isSurfaceFront = false;
		}
				break;
		}


		m_isPositionX = isPositionX;
		m_isSurfaceFront = isSurfaceFront;
		row_size = m_isPositionX ? size_x : size_z;

		//타일 설정 및 타일 돌았던 타일로 설정
		tile = GetSurfaceTile(curPos);
		//
		if (tile == nullptr) {
			continue;
		}

		if (tile->id == RESMGR->GetAssetProc()->getGoalId()) {
			m_arrivedCount++;
			break;
		}

		//플레이어가 막힘
		//if(!(curPos == Vector3<int>{tile->x, tile->y, tile->z})) {
		//	continue;
		//}

		int x = m_isPositionX ? tile->x : tile->z;
		int y = tile->y;

		//좌
		if (x - 1 >= 0) {
			AnalyzingTile(-1, 0, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);

			//좌상
			if (y - 1 >= 0) {
				AnalyzingTile(-1, -1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}
		}

		//우
		if (x + 1 < row_size) {
			AnalyzingTile(1, 0, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);

			//우하
			if (y + 1 < column_size) {
				AnalyzingTile(1, 1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}
		}

		//상
		if (y - 1 >= 0) {

			//우상
			if (x + 1 < row_size) {
				AnalyzingTile(1, -1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}
		}

		//하
		if (y + 1 < column_size) {

			//좌하
			if (x - 1 >= 0) {
				AnalyzingTile(-1, 1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}

		}

		//================================
		column_size = size_y;
		
	}

	SetTilePassed(false, curPos);

}


bool RouteSimulation::AnalyzingTile(int row, int column, const Vector3<int>& curPos, bool isPositionX, bool isSurfaceFront
									, Vector3<int>& prevPos, Vector3<int> prevAllPos[4], int rotationId) {
	
	//x축이 기준인지 z축이 기준인지 확인 후 이동할 커서 포지션을 계산합니다.
	Vector3<int> tmpPos = curPos + (m_isPositionX ? Vector3<int>{row, column, 0} : Vector3<int>{ 0, column, row });
	//갈 수 있는 포지션인지 확인합니다.
	TileNode* tile = GetSurfaceTile(tmpPos);
	if (tile == nullptr) return false;
	Vector3<int> resultPos = Vector3<int>{ tile->x, tile->y, tile->z };
	if (OnJudgePlayerMovable(curPos, *tile) && !isOverlapTile(resultPos, prevAllPos)) {
		//이동
		prevPos = resultPos;
		FindWay(resultPos, rotationId);
		//화면이 전환되어 있을 수 있으므로 되돌립니다.
		m_isPositionX = isPositionX;
		m_isSurfaceFront = isSurfaceFront;
		return true;
	}

	return false;
}


bool RouteSimulation::isOverlapTile(Vector3<int> pos, Vector3<int> prevPos[4]) const {

	//중복이동 방지
	for (int j = 0; j < 4; j++) {
		if (prevPos[j] == Vector3<int>{-1, -1, -1}) continue;
		if (prevPos[j] == pos) {
			return true;
		}
	}

	return false;

}


bool RouteSimulation::OnJudgePlayerMovable(Vector3<int> curPos, TileNode& tile) const {

	if(&tile == nullptr) {
		return false;
	}

	int heightLevel = tile.y - curPos.y;


	//클릭한 타일 위에 다른 타일이 있을 경우 예외처리
	if (tile.y < size_y - 1 && GetSurfaceTile(Vector3<int>{tile.x, tile.y + 1, tile.z}) != nullptr) {
		return false;
	}


	//플레이어 위
	if (heightLevel == 1) {
		Vector3<int> tempTile = curPos + Vector3<int>{0, 2, 0};
		//플레이어 바로 위에 천장이 없을 경우
		if (tempTile.y > size_y - 1 || GetSurfaceTile(tempTile) == nullptr) {
			//x축 기준 조건이 맞는 경우
			if (m_isPositionX && abs(tile.x - curPos.x) <= 1) {
				return true;
			}
			//z축 기준 조건이 맞는 경우
			if (!m_isPositionX && abs(tile.z - curPos.z) <= 1) {
				return true;
			}
			//조건에 만족하지 않는 경우
			return false;
		}

	//플레이어 중심
	} else if (heightLevel == 0) {
		//x축 기준 조건이 맞는 경우
		if (m_isPositionX && abs(tile .x - curPos.x) <= 1) {
			return true;
		}
		//z축 기준 조건이 맞는 경우
		if (!m_isPositionX && abs(tile.z - curPos.z) <= 1) {
			return true;
		}
		//조건에 만족하지 않는 경우
		return false;
	}

	//플레이어 아래
	else if (heightLevel == -1) {
		Vector3<int> tempTile = Vector3<int>{ tile.x, tile.y + 2, tile.z };
		//플레이어 옆에 있는 장애물이 존재하지 않을 경우
		if (tempTile.y > size_y - 1 || GetSurfaceTile(tempTile) == nullptr) {
			//x축 기준 조건이 맞는 경우
			if (m_isPositionX && abs(tile.x - curPos.x) <= 1) {
				return true;
			}
			//z축 기준 조건이 맞는 경우
			if (!m_isPositionX && abs(tile.z - curPos.z) <= 1) {
				return true;
			}
			//조건에 만족하지 않는 경우
			return false;
		}

	}


	return false;
}


TileNode* RouteSimulation::GetSurfaceTile(Vector3<int> pos) const {
	TileAreaValue tile = m_isPositionX ? m_tileArea[pos.y].x[pos.x]
		: m_tileArea[pos.y].z[pos.z];

	return m_isSurfaceFront ? tile.front : tile.back;
}


//TileNode* RouteSimulation::GetSurfaceTile(Vector2<int> pos) const {
//	TileAreaValue tile = m_isPositionX ? m_tileArea[pos.y].x[pos.x]
//		: m_tileArea[pos.y].z[pos.x];
//
//	return m_isSurfaceFront ? tile.front : tile.back;
//}


void RouteSimulation::SetTilePassed(bool passed, Vector3<int> pos) const {

	m_tileArea[pos.y].x[pos.x].isPassed = passed;
	m_tileArea[pos.y].z[pos.z].isPassed = passed;

}


bool RouteSimulation::isTilePassed(Vector3<int> pos) const {

	return m_isPositionX ? m_tileArea[pos.y].x[pos.x].isPassed
		: m_tileArea[pos.y].z[pos.z].isPassed;

}


bool RouteSimulation::isTilePassed(Vector3<int> pos, bool isPositionX) const {
	return isPositionX ? m_tileArea[pos.y].x[pos.x].isPassed
		: m_tileArea[pos.y].z[pos.z].isPassed;
}


//Vector2<int> RouteSimulation::RefreshCurPos(TileNode* tile, bool isPositionX) const {
//	return isPositionX ? Vector2<int>{tile->x, tile->y} :
//		Vector2<int>{ tile->z, tile->y };
//}
