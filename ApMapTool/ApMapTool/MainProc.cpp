#include "stdafx.h"
#include "MainProc.h"
#include <fstream>
#include "ResourceManager.h"


MainProc::MainProc() {
	m_fileName = "untitled.map";
	RESMGR->SetMapSize(6, 6, 6);
}


MainProc::~MainProc() {
}


void MainProc::setFilePath(std::string path) {
	m_filePath = path;
	int index = path.rfind("\\");
	m_fileName = path.substr(index + 1, path.length() - 1);

}


void MainProc::ReadMapFile() {

	std::ifstream fin;
	fin.open(m_filePath);

	std::string buf;

	if(getline(fin, buf)) {
		std::vector<std::string> list;
		Split(list, replace_all(buf, " ", ""), ",", true);

		RESMGR->SetMapSize(std::atoi(list[0].c_str()), std::atoi(list[1].c_str()), std::atoi(list[2].c_str()));
		buf.clear();
	} else
		return;

	AssetProc* asset_proc = RESMGR->GetAssetProc();

	while(getline(fin, buf)) {

		if (buf == "") continue;

		std::vector<std::string> lit;
		Split(lit, replace_all(buf, " ", ""), ",", true);
		
		if(lit.size() != 4) continue;

		int id = 0;
		if(lit[0] == "SpawnTile") {
			id = asset_proc->getSpawnId();
		} else if (lit[0] == "GoalTile") {
			id = asset_proc->getGoalId();
		} else {
			id = asset_proc->FindTileId(lit[0]);
		}

		RESMGR->SetTile(id, std::atoi(lit[1].c_str()), std::atoi(lit[2].c_str()), std::atoi(lit[3].c_str()));
		buf.clear();
	}

	fin.close();

}


void MainProc::SaveMapFile() {

	std::ofstream fout(m_filePath);

	auto mapProc = RESMGR->GetMapProc();
	auto tiles = mapProc->GetTiles();
	vec3 mapSize = mapProc->getMapSize() + vec3{ 1, 1, 1 };

	fout << static_cast<int>(mapSize.x) << ", " << static_cast<int>(mapSize.y) << ", " << static_cast<int>(mapSize.x) << std::endl << std::endl;

	AssetProc* asset_proc = RESMGR->GetAssetProc();

	for(int i = 0; i < tiles.size(); i++) {
		auto tile = tiles[i];

		std::string id;

		id = asset_proc->FindTileIdStr(tile->id);

		if(id == "NULL") {
			id = "NomalTile";
		}

		fout << id << ", " << tile->x << ", " << tile->y << ", " << tile->z << std::endl;
	}

	fout.close();

}


void MainProc::AutoSaveFile() {

	if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VkKeyScanA('s')) & 0x8001) {
		if (m_fileName != "untitled.map") {
			SaveMapFile();
			RESMGR->SetTitleSaved();
		} else {
			AutoSaveFileWithPath();
		}
	}

}


void MainProc::AutoSaveFileWithPath() {
	OPENFILENAMEA OFN;
	char * b = new char[MAX_PATH];
	strcpy_s(b, m_fileName.length() + 1, m_fileName.c_str());
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = RESMGR->getMainHWND();
	OFN.lpstrFilter = "¾ÓÆÎ ¸Ê ÆÄÀÏ(*.map)\0*.map\0¸ðµç ÆÄÀÏ(*.*)\0*.*\0";
	OFN.lpstrFile = b;
	OFN.nMaxFile = 256;

	if (GetSaveFileNameA(&OFN)) {
		//if (!RESMGR->b_isChanged) {
		//	break;
		//}
		setFilePath(b);
		SaveMapFile();
		RESMGR->SetTitleSaved();
	}
}


void  MainProc::Split(std::vector<std::string>& lst, std::string input, const std::string& separators,
	bool remove_empty = true) {

	while (true) {
		int index = input.find(separators);
		if (index != std::string::npos) {
			if (index == 0 && remove_empty) continue;
			lst.push_back(input.substr(0, index));
		} else {
			lst.push_back(input);
			break;
		}
		input = input.substr(index + separators.size());

	}
}

std::string MainProc::replace_all(
	__in const std::string &message,
	__in const std::string &pattern,
	__in const std::string &replace
) {

	std::string result = message;
	std::string::size_type pos = 0;
	std::string::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != std::string::npos) {
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}

	return result;
}


