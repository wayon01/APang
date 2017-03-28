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

	if(curPos.x < 0 || curPos.y < 0 || curPos.z < 0) { //����Ÿ���� ����!
		MessageBoxA(nullptr, "���� Ÿ���� �����ϴ�.", "�ߴܵ� �ùķ��̼�", MB_OK | MB_ICONWARNING);
		return;
	}

	m_isPositionX = m_isSurfaceFront = true;
	m_arrivedCount = 0;

	//Ž������
	//Vector2<int> findPos{curPos.x, curPos.y};
	FindWay(curPos, 0);
	std::string resultStr = "��δ� �� ";
	resultStr.append(std::to_string(m_arrivedCount));
	resultStr.append("������ �ֽ��ϴ�.");
	MessageBoxA(nullptr, resultStr.c_str(), "���", MB_OK | MB_ICONINFORMATION);
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

	//�̹� ���Ҵ� Ÿ���̸� �Ÿ���
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

		//Ÿ�� ���� �� Ÿ�� ���Ҵ� Ÿ�Ϸ� ����
		tile = GetSurfaceTile(curPos);
		//
		if (tile == nullptr) {
			continue;
		}

		if (tile->id == RESMGR->GetAssetProc()->getGoalId()) {
			m_arrivedCount++;
			break;
		}

		//�÷��̾ ����
		//if(!(curPos == Vector3<int>{tile->x, tile->y, tile->z})) {
		//	continue;
		//}

		int x = m_isPositionX ? tile->x : tile->z;
		int y = tile->y;

		//��
		if (x - 1 >= 0) {
			AnalyzingTile(-1, 0, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);

			//�»�
			if (y - 1 >= 0) {
				AnalyzingTile(-1, -1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}
		}

		//��
		if (x + 1 < row_size) {
			AnalyzingTile(1, 0, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);

			//����
			if (y + 1 < column_size) {
				AnalyzingTile(1, 1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}
		}

		//��
		if (y - 1 >= 0) {

			//���
			if (x + 1 < row_size) {
				AnalyzingTile(1, -1, curPos, isPositionX, isSurfaceFront, prevPos[rotationId], prevPos, rotationId);
			}
		}

		//��
		if (y + 1 < column_size) {

			//����
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
	
	//x���� �������� z���� �������� Ȯ�� �� �̵��� Ŀ�� �������� ����մϴ�.
	Vector3<int> tmpPos = curPos + (m_isPositionX ? Vector3<int>{row, column, 0} : Vector3<int>{ 0, column, row });
	//�� �� �ִ� ���������� Ȯ���մϴ�.
	TileNode* tile = GetSurfaceTile(tmpPos);
	if (tile == nullptr) return false;
	Vector3<int> resultPos = Vector3<int>{ tile->x, tile->y, tile->z };
	if (OnJudgePlayerMovable(curPos, *tile) && !isOverlapTile(resultPos, prevAllPos)) {
		//�̵�
		prevPos = resultPos;
		FindWay(resultPos, rotationId);
		//ȭ���� ��ȯ�Ǿ� ���� �� �����Ƿ� �ǵ����ϴ�.
		m_isPositionX = isPositionX;
		m_isSurfaceFront = isSurfaceFront;
		return true;
	}

	return false;
}


bool RouteSimulation::isOverlapTile(Vector3<int> pos, Vector3<int> prevPos[4]) const {

	//�ߺ��̵� ����
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


	//Ŭ���� Ÿ�� ���� �ٸ� Ÿ���� ���� ��� ����ó��
	if (tile.y < size_y - 1 && GetSurfaceTile(Vector3<int>{tile.x, tile.y + 1, tile.z}) != nullptr) {
		return false;
	}


	//�÷��̾� ��
	if (heightLevel == 1) {
		Vector3<int> tempTile = curPos + Vector3<int>{0, 2, 0};
		//�÷��̾� �ٷ� ���� õ���� ���� ���
		if (tempTile.y > size_y - 1 || GetSurfaceTile(tempTile) == nullptr) {
			//x�� ���� ������ �´� ���
			if (m_isPositionX && abs(tile.x - curPos.x) <= 1) {
				return true;
			}
			//z�� ���� ������ �´� ���
			if (!m_isPositionX && abs(tile.z - curPos.z) <= 1) {
				return true;
			}
			//���ǿ� �������� �ʴ� ���
			return false;
		}

	//�÷��̾� �߽�
	} else if (heightLevel == 0) {
		//x�� ���� ������ �´� ���
		if (m_isPositionX && abs(tile .x - curPos.x) <= 1) {
			return true;
		}
		//z�� ���� ������ �´� ���
		if (!m_isPositionX && abs(tile.z - curPos.z) <= 1) {
			return true;
		}
		//���ǿ� �������� �ʴ� ���
		return false;
	}

	//�÷��̾� �Ʒ�
	else if (heightLevel == -1) {
		Vector3<int> tempTile = Vector3<int>{ tile.x, tile.y + 2, tile.z };
		//�÷��̾� ���� �ִ� ��ֹ��� �������� ���� ���
		if (tempTile.y > size_y - 1 || GetSurfaceTile(tempTile) == nullptr) {
			//x�� ���� ������ �´� ���
			if (m_isPositionX && abs(tile.x - curPos.x) <= 1) {
				return true;
			}
			//z�� ���� ������ �´� ���
			if (!m_isPositionX && abs(tile.z - curPos.z) <= 1) {
				return true;
			}
			//���ǿ� �������� �ʴ� ���
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
