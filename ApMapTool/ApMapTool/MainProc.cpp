#include "stdafx.h"
#include "MainProc.h"
#include <fstream>
#include "ResourceManager.h"
#include "GoalTileNode.h"
#include "WarpTileNode.h"
#include <iostream>
#include "DecoTile.h"


MainProc::MainProc() {

	char Path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, Path);

	m_exePath = Path;

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

	//if(getline(fin, buf)) {
	//	std::vector<std::string> list;
	//	Split(list, replace_all(buf, " ", ""), ",", true);

	//	RESMGR->SetMapSize(std::atoi(list[0].c_str()), std::atoi(list[1].c_str()), std::atoi(list[2].c_str()));
	//	buf.clear();
	//} else
	//	return;

	AssetProc* asset_proc = RESMGR->GetAssetProc();
	MapProc* map_proc = RESMGR->GetMapProc();

	int stageLength = 0;
	int stageCount = 0;
	bool isStageAvailable = false;

	while(getline(fin, buf)) {

		if (buf == "") continue;

		std::vector<std::string> lit;
		Split(lit, replace_all(buf, " ", ""), ",", true);

		//플레이어 이동 제한
		if(lit[0] == "#MOVEMENT") {
			m_goldValue = std::atoi(lit[1].c_str());
			m_silverValue = std::atoi(lit[2].c_str());
		}

		//스테이지
		if(lit[0] == "#STAGE") {
			//첫 스테이지일 경우
			if (!isStageAvailable) {
				//int index;
				//auto map_stage = map_proc->GetMapStage();
				stageLength = std::atoi(lit[1].c_str());
				map_proc->MapStagePushBack(stageLength, new MapNode());
				//index = map_proc->GetMapStage().size() - 1;
				isStageAvailable = true;
				

				map_proc->GetMapStage()[stageLength]->id = stageLength;
				map_proc->SetStageId(stageLength);

				continue;
			}
			//auto& map_stage = map_proc->GetMapStage();
			int tmp = std::atoi(lit[1].c_str());
			//int index;

			map_proc->MapStagePushBack(tmp, new MapNode());
			//index = map_proc->GetMapStage().size() - 1;
			stageLength = tmp;
			map_proc->GetMapStage()[tmp]->id = stageLength;
			map_proc->SetStageId(tmp);
			stageCount++;

			continue;
		}

		if(lit.size() == 3) {
			if (map_proc->GetMapStage().size() == 0) {
				map_proc->MapStagePushBack(0, new MapNode());
			}
			RESMGR->SetMapSize(stageCount, std::atoi(lit[0].c_str()), std::atoi(lit[1].c_str()), std::atoi(lit[2].c_str()));
			buf.clear();
			continue;
		}
		
		if (lit.size() < 4) continue;


		int id = 0;
		if(lit[0] == "SpawnTile") {
			id = asset_proc->getSpawnId();
		} else if (lit[0] == "GoalTile") {
			id = asset_proc->getGoalId();
		} else if (lit[0] == "PortalTile") {
			id = asset_proc->getPortalId();
		} else {

			std::vector<std::string> IdLit;
			Split(IdLit, lit[0], "#", true);

			if(IdLit[0] == "DecoTile") {
				id = asset_proc->FindTileId(IdLit[1]);

				if (map_proc->GetMapStage().size() == 0) {
					map_proc->MapStagePushBack(0, new MapNode());
				}

				lit[0] = "DecoTile";
				RESMGR->SetTile(stageCount, id, lit);
				buf.clear();

				continue;

			} else {
				id = asset_proc->FindTileId(lit[0]);
			}
		}

		if(map_proc->GetMapStage().size() == 0) {
			map_proc->MapStagePushBack(0, new MapNode());
		}
		RESMGR->SetTile(stageCount, id, lit);
		buf.clear();
	}

	fin.close();

}

void MainProc::SaveMapFile() const {
	SaveMapFile(m_filePath);
}



void MainProc::SaveMapFile(std::string str) const {

	std::ofstream fout(str);

	if(fout.fail()) {
		std::string asdf = strerror(errno);
		asdf.append(" a");
		return;
	}

	auto mapProc = RESMGR->GetMapProc();
	int stageId = mapProc->GetStageId();

	fout << "#MOVEMENT, " << m_goldValue << ", " << m_silverValue << std::endl;

	for(int stage = 0; stage < mapProc->GetMapStage().size(); stage++) {

		mapProc->SetStageId(stage);

		auto tiles = mapProc->GetTiles();
		vec3 mapSize = mapProc->getMapSize() + vec3{ 1, 1, 1 };

		fout << std::endl << "#STAGE, " << mapProc->GetMapStage()[stage]->id << std::endl;
		fout << static_cast<int>(mapSize.x) << ", " << static_cast<int>(mapSize.y) << ", " << static_cast<int>(mapSize.z) << std::endl << std::endl;

		AssetProc* asset_proc = RESMGR->GetAssetProc();

		for (int i = 0; i < tiles.size(); i++) {
			auto tile = tiles[i];

			std::string id;

			id = asset_proc->FindTileIdStr(tile->id);

			if (id == "NULL") {
				id = "NomalTile";
			}

			if(id == "GoalTile") {
				GoalTileNode* tmp_goal = static_cast<GoalTileNode*>(tile);
				fout << id << ", " << tile->x << ", " << tile->y << ", " << tile->z << ", " << tmp_goal->m_additionalIntegerType["Area ID"] << std::endl;
				continue;
			}

			if(id == "PortalTile") {
				WarpTileNode* tmp_warp = static_cast<WarpTileNode*>(tile);
				fout << id << ", " << tile->x << ", " << tile->y << ", " << tile->z << ", " << tmp_warp->m_additionalIntegerType["Area ID"] << ", " << tmp_warp->m_additionalStringType["타일 이름"] << ", " << tmp_warp->m_additionalStringType["이동할 타일 이름"] << std::endl;
				continue;
			}

			if(id == "DecoTile") {
				DecoTile* tmp_deco = static_cast<DecoTile*>(tile);
				std::string tmp_deco_enable = tmp_deco->m_additionalBooleanType["타일인식"] ? "true" : "false";
				fout << "DecoTile" << " # " << asset_proc->FindDecoTileIdStr(tile->id) << ", " << tile->x << ", " << tile->y << ", " << tile->z << ", " << tmp_deco_enable << std::endl;
				continue;
			}
			fout << id << ", " << tile->x << ", " << tile->y << ", " << tile->z << std::endl;
		}
	}
	
	fout << "#end" << std::endl;
	fout.close();

	mapProc->SetStageId(stageId);
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
	OFN.lpstrFilter = "앙팡 맵 파일(*.map)\0*.map\0모든 파일(*.*)\0*.*\0";
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


void MainProc::DebugGame() {
	SetCurrentDirectoryA(m_exePath.c_str());

	SaveMapFile(m_exePath + "\\debug_exe\\debug_test_Data\\StreamingAssets\\test.map");

	std::string strPath = m_exePath;

	strPath.append("\\debug_exe\\debug_test.exe");

	system(strPath.c_str());
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


