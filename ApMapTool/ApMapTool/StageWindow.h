#pragma once
#include "ChildWindow.h"
#include "DetailWindow.h"

class StageWindow : public ChildWindow {
public:
	StageWindow(DetailWindow& detail_window);
	~StageWindow();

	LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;
	void SetWindowSize(int x, int y, int w, int h) override;

	void LoadStageList();

private:

	static int FindStageId(int id);

private:
	HWND hList;
	bool isLoad = false;
	DetailWindow& m_detailWindow;
};

