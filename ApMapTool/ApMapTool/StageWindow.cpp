#include "stdafx.h"
#include "StageWindow.h"
#include <Windows.h>
#include "ResourceManager.h"

#define ID_LISTBOX 100


StageWindow::StageWindow(DetailWindow& detail_window) : ChildWindow(), m_detailWindow(detail_window) {
	
}


StageWindow::~StageWindow() {
	
}


LRESULT StageWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	if (isLoad) return 0;

	isLoad = true;
	hList = CreateWindowA("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY | WS_VSCROLL | WS_BORDER, 0, 0, WindowRect.right, WindowRect.bottom, this->hWnd, (HMENU)ID_LISTBOX, hInst, NULL);

	LoadStageList();
}

LRESULT StageWindow::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	if (GetAsyncKeyState(VK_DELETE) & 0x8001) {
		if (MessageBoxA(hWnd, "진짜 삭제할거에요?", "경고", MB_OKCANCEL | MB_ICONWARNING) == IDOK) {
			//RESMGR->GetMapProc()->DeleteMap()
		}
	}

	if (GetAsyncKeyState(VK_F5) & 0x8001) {
		RESMGR->getMainProc()->DebugGame();
	}

	return 0;
}


LRESULT StageWindow::OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
				RESMGR->GetMapProc()->SetStageId(FindStageId(i));
				RESMGR->GetMapProc()->SetCurTile(ivec3{ 0, 0, 0 });
				m_detailWindow.LoadCurrentTileInfo();
				break;
			}
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}


void StageWindow::SetWindowSize(int x, int y, int w, int h) {
	__super::SetWindowSize(x, y, w, h);

	SetWindowPos(hList, nullptr, 0, 30, w, h, NULL);
}


void StageWindow::LoadStageList() {

	auto tmp = RESMGR->GetMapProc()->GetMapStage();
	std::string tmp_str;

	SendMessageA(hList, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < tmp.size(); i++) {
		tmp_str = "구역 ";
		tmp_str.append(std::to_string(tmp[i]->id));
		SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)tmp_str.c_str());
	}

}


int StageWindow::FindStageId(int id) {

	auto tmp = RESMGR->GetMapProc()->GetMapStage();

	for (int i = 0; i < tmp.size(); i++) {
		auto obj = tmp.at(i);

		if(obj == nullptr) {
			continue;
		}

		if(obj->id == id) {
			return i;
		}
	}

	return -1;
}
