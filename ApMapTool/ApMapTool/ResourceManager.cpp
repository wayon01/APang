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

	m_mapProc->SetTile(id, x, y, z);

}


void ResourceManager::SetMapSize(int x, int y, int z) const {
	m_mapProc->SetMapSize(x, y, z);
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
