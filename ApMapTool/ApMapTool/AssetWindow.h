#pragma once
#include "ChildWindow.h"
#include <vector>

class AssetWindow : public ChildWindow {
public:
	AssetWindow();
	~AssetWindow();

	LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;
	void SetWindowSize(int x, int y, int w, int h) override;

	void LoadAssetList();
	void SaveAssetList() const;

	void SetAssetInfo(std::string name, std::string strId, std::string filePath);
	void SetAssetInfo(std::string name, std::string strId, std::string filePath, std::string tileName);

	

private:
	HWND hList;
	bool isLoad = false;
};

