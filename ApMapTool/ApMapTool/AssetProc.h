#pragma once
#include <string>
#include <vector>

class AssetProc {
public:
	AssetProc();
	~AssetProc();

	void LoadAssetList();
	void SaveAssetList();

	int GetAssetID(int index) const {
		return m_assetList[index].id;
	}

	std::string GetAssetName(int index) const {
		return m_assetList[index].m_name;
	}

	int getAssetInfoSize() const {
		return m_assetList.size();
	}


	std::string getFilePath(int index) {
		return m_assetList[index].m_filePath;

	}

	void SetAssetInfo(std::string name, std::string strId, std::string filePath) {
		SetTileId(name, strId, filePath);
		SaveAssetList();
	}

	void SetAssetInfo(std::string name, std::string strId, std::string filePath, std::string tileName) {
		SetTileId(name, strId, filePath, tileName);
		SaveAssetList();
	}

	int getSpawnId() const {
		return m_spawnId;
	}

	int getGoalId() const {
		return m_goalId;
	}

	int getPortalId() const {
		return m_portalId;
	}

	int* getTextureId(int index) {
		return m_assetList[index].m_textureId;
	}

	void setTextureId(int index, int id) {
		m_assetList[index].m_textureId[0] = id;
	}

	int FindTileId(std::string idStr) const;
	std::string FindTileIdStr(int id) const;
	std::string FindDecoTileIdStr(int id) const;
	int FindTextureIdtoId(int id);

	bool isAddedTile() const {
		return bIsAddedTile;
	}

	void SetAddedTile(bool active) {
		bIsAddedTile = active;
	}


private:
	void SetTileId(std::string name, std::string strId, std::string filePath);
	void SetTileId(std::string name, std::string strId, std::string filePath, std::string tileName);

private:
	struct ASSETINFO {
		int id;
		std::string m_name;
		std::string m_strId;
		std::string m_filePath;
		std::string m_tileName;
		int m_textureId[1];
	};
	std::vector<ASSETINFO> m_assetList;

	std::vector<int> m_attackerId;
	std::vector<int> m_decorationId;
	int m_spawnId;
	int m_goalId;
	int m_portalId;

	int m_curId = 0;

	bool bIsAddedTile = false;
};

