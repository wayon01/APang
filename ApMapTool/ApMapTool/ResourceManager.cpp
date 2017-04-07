#include "stdafx.h"
#include "ResourceManager.h"
IMPLEMENT_SINGLETON(ResourceManager);

ResourceManager::ResourceManager(): m_mapProc(new MapProc()), m_assetProc(new AssetProc()) {
}


ResourceManager::~ResourceManager() {
	SAFE_DELETE(m_mapProc);
	SAFE_DELETE(m_assetProc);
}


void ResourceManager::SetTile(int id, int x, int y, int z) const {

	if(id == RESMGR->GetAssetProc()->getGoalId()) {
		m_mapProc->SetTile(id, x, y, z, -1);
	}else {
		m_mapProc->SetTile(id, x, y, z);
	}
	

}


void ResourceManager::SetTile(int stageId, int id, std::vector<std::string> value) const {
	int id__ = m_mapProc->GetStageId();

	int x = std::atoi(value[1].c_str());
	int y = std::atoi(value[2].c_str());
	int z = std::atoi(value[3].c_str());

	m_mapProc->SetStageId(stageId);

	if(value[0] == "GoalTile") {
		if(value.size() == 5)
			m_mapProc->SetTile(id, x, y, z, std::atoi(value[4].c_str()));
		else
			m_mapProc->SetTile(id, x, y, z, -1);
	}else {
		m_mapProc->SetTile(id, x, y, z);
	}
	
	m_mapProc->SetStageId(id__);
}


void ResourceManager::SetMapSize(int x, int y, int z) const {
	m_mapProc->SetMapSize(x, y, z);
}


void ResourceManager::SetMapSize(int stageId, int x, int y, int z) const {
	int id = m_mapProc->GetStageId();
	m_mapProc->SetStageId(stageId);
	m_mapProc->SetMapSize(x, y, z);
	m_mapProc->SetStageId(id);

}


void ResourceManager::SetTitleUnSaved() const {
	b_isChanged = true;
	std::string tmp = m_mainProc->getTitleName();
	tmp.append("*");
	SetWindowTextA(m_MainhWnd, tmp.c_str());
}


void ResourceManager::SetTitleSaved() const {
	b_isChanged = false;
	SetWindowTextA(m_MainhWnd, m_mainProc->getTitleName().c_str());
}
