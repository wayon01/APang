#include "stdafx.h"
#include "AssetProc.h"
#include "MainProc.h"
#include <fstream>
#include "MacroDef.h"


AssetProc::AssetProc(): m_spawnId(TILE_NONE), m_goalId(TILE_NONE), m_portalId(TILE_NONE) {
	m_curId = 0;
}


AssetProc::~AssetProc() {}


void AssetProc::LoadAssetList() {

	m_assetList.clear();
	m_curId = 0;

	std::ifstream fin;
	fin.open("asset.ini");
	if (!fin.is_open()) {



		fin.close();

		return;
	}

	std::string buf;
	while (getline(fin, buf)) {

		if (buf == "##end") break;
		if (buf == "") continue;

		std::vector<std::string> lit;
		MainProc::Split(lit, MainProc::replace_all(buf, " ", ""), ",", true);

		if (lit.size() != 3) continue;

		SetTileId(lit[0], lit[1], lit[2]);
		//m_assetList.push_back(ASSETINFO{ std::atoi(lit[1].c_str()), lit[0], lit[2] });

		buf.clear();

	}

	fin.close();
}


void AssetProc::SaveAssetList() {

	std::ofstream fout("asset.ini");
	for (int i = 0; i < m_assetList.size(); i++) {
		auto asset = m_assetList[i];

		fout << asset.m_name << ", " << asset.m_strId << ", " << asset.m_filePath << std::endl;
	}

	fout << "##end" << std::endl;

	fout.close();
}


int AssetProc::FindTileId(std::string idStr) const {

	for(auto& assetTmp : m_assetList) {
		if(assetTmp.m_strId == idStr) {
			return assetTmp.id;
		}
	}

	return TILE_NONE;

}


std::string AssetProc::FindTileIdStr(int id) const {
	for (auto& assetTmp : m_assetList) {
		if (assetTmp.id == id) {
			return assetTmp.m_strId;
		}
	}

	return "NULL";
}


int AssetProc::FindTextureIdtoId(int id) {

	for (auto& assetTmp : m_assetList) {
		if (assetTmp.id == id) {
			if(assetTmp.m_textureId != nullptr)
			return assetTmp.m_textureId[0];
		}
	}

	return TILE_NONE;

}


void AssetProc::SetTileId(std::string name, std::string strId, std::string filePath) {

	m_curId++;

	if(strId == "SpawnTile" || strId == "spawnTile" || strId == "spawn tile") {
		m_assetList.push_back(ASSETINFO{ m_curId, name, "SpawnTile", filePath });
		m_spawnId = m_curId;
		return;
	}

	if(strId == "GoalTile" || strId == "goalTile" || strId == "goal tile") {
		m_assetList.push_back(ASSETINFO{ m_curId, name, "GoalTile", filePath });
		m_goalId = m_curId;
		return;
	}

	if (strId == "PortalTile" || strId == "portalTile" || strId == "portal tile") {
		m_assetList.push_back(ASSETINFO{ m_curId, name, "PortalTile", filePath });
		m_portalId = m_curId;
		return;
	}

	if (strId == "DecoTile" || strId == "decoTile" || strId == "deco tile") {
		m_assetList.push_back(ASSETINFO{ m_curId, name, "DecoTile", filePath });
		m_decorationId.push_back(m_curId);
		return;
	}

	if (strId == "AttackTile" || strId == "attackTile" || strId == "attack tile") {
		m_assetList.push_back(ASSETINFO{ m_curId, name, "AttackTile", filePath });
		m_attackerId.push_back(m_curId);
		return;
	}

	m_assetList.push_back(ASSETINFO{ m_curId, name, strId, filePath });
	bIsAddedTile = true;

}
