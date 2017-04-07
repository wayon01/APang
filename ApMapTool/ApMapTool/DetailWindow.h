#pragma once
#include "ChildWindow.h"
#include "TileDetailStruct.h"
#include <vector>
#include "TileNode.h"

class DetailWindow : public ChildWindow {
public:
	DetailWindow();
	~DetailWindow();

	LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) override;

	void LoadCurrentTileInfo();
	void DeleteHWnd();

private:
	std::vector<TileDetail> m_detail;
	TileNode* m_currentTile = nullptr;
};

