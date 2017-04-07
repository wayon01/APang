#pragma once
#include "MacroDef.h"
#include "MainProc.h"
#include "MapProc.h"
#include "AssetProc.h"

#define RESMGR ResourceManager::getInstance()

class MainProc;
class ResourceManager {
public:
	DECLARE_SINGLETONE(ResourceManager);
	~ResourceManager();

	void SetTile(int id, int x, int y, int z) const;
	void SetTile(int stageId, int id, std::vector<std::string>) const;
	void SetMapSize(int x, int y, int z) const;
	void SetMapSize(int stageId, int x, int y, int z) const;
	MapProc* GetMapProc() const {
		return m_mapProc;
	}

	AssetProc* GetAssetProc() const {
		return m_assetProc;
	}

	void setMainHWND(HWND hWnd) {
		m_MainhWnd = hWnd;
	}

	HWND getMainHWND() const {
		return m_MainhWnd;
	}
	void SetMainProc(MainProc* m) {
		m_mainProc = m;
	}

	MainProc* getMainProc() const {
		return m_mainProc;
	}

	void SetTitleUnSaved() const;
	void SetTitleSaved() const;

public:
	mutable bool b_isChanged;

private:
	MapProc* m_mapProc;
	AssetProc* m_assetProc;
	HWND m_MainhWnd;

	MainProc* m_mainProc;
};

