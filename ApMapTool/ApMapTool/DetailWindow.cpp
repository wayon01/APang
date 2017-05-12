#include "stdafx.h"
#include "DetailWindow.h"
#include "TileNode.h"
#include "ResourceManager.h"


DetailWindow::DetailWindow() {
	
}


DetailWindow::~DetailWindow() {
	
}


LRESULT DetailWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	LoadCurrentTileInfo();

	return 0;
}


LRESULT DetailWindow::OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
	case WM_COMMAND: {
		if (m_currentTile == nullptr) break;
		WORD cmdMsg = LOWORD(wParam);

		for (int i = 0; i < m_detail.size(); i++) {
			TileDetail& tmp = m_detail[i];

			if (cmdMsg == i) {

				if (tmp.id == ID_INT) {

					switch (HIWORD(wParam)) {
					case EN_CHANGE: {
						char str[128];
						GetWindowTextA(tmp.value, str, 128);
						SetWindowTextA(hWnd, str);

						m_currentTile->m_additionalIntegerType[tmp.strName] = std::atoi(str);
						RESMGR->SetTitleUnSaved();

						break;
					}
					}
				} else if (tmp.id == ID_STRING) {
					switch (HIWORD(wParam)) {
					case EN_CHANGE: {
						char str[128];
						GetWindowTextA(tmp.value, str, 128);
						SetWindowTextA(hWnd, str);

						m_currentTile->m_additionalStringType[tmp.strName] = str;
						RESMGR->SetTitleUnSaved();

						break;
					}
					}

				}else if(tmp.id == ID_BOOL) {
					if (SendMessage(tmp.value, BM_GETCHECK, 0, 0) == BST_UNCHECKED) {
						SendMessage(tmp.value, BM_SETCHECK, BST_CHECKED, 0);
						m_currentTile->m_additionalBooleanType[tmp.strName] = true;
						RESMGR->SetTitleUnSaved();
					} else {
						SendMessage(tmp.value, BM_SETCHECK, BST_UNCHECKED, 0);
						m_currentTile->m_additionalBooleanType[tmp.strName] = false;
						RESMGR->SetTitleUnSaved();
					}
				}
			}
		}
		break;
	}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);

}


LRESULT DetailWindow::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	if(GetAsyncKeyState(VK_DELETE) & 0x8001) {
		if (MessageBoxA(hWnd, "진짜 삭제할거에요?", "경고", MB_OKCANCEL | MB_ICONWARNING) == IDOK) {

		}
	}

	if (GetAsyncKeyState(VK_F5) & 0x8001) {
		RESMGR->getMainProc()->DebugGame();
	}

	return 0;
}


void DetailWindow::LoadCurrentTileInfo() {

	DeleteHWnd();

	TileNode cur = RESMGR->GetMapProc()->GetCurTile();
	m_currentTile = RESMGR->GetMapProc()->GetTile(ivec3{ cur.x, cur.y, cur.z });

	if(m_currentTile == nullptr) {
		return;
	}

	int x = 0;
	int y = 30;

	for (const auto& pair : m_currentTile->m_additionalBooleanType) {
		TileDetail tmp;

		tmp.name = CreateWindowA("static", pair.first.c_str(), WS_CHILD | WS_VISIBLE,
			x, y, WindowRect.right, WindowRect.bottom, hWnd, (HMENU)-1, hInst, NULL);
		tmp.strName = pair.first;

		int sx = x + pair.first.size() * 10;
		tmp.value = CreateWindowA("button", NULL, WS_CHILD | WS_VISIBLE | BS_CHECKBOX
			, sx, y, sx + 200, 25, hWnd, (HMENU)m_detail.size(), hInst, NULL);
		SendMessage(tmp.value, BM_SETCHECK, pair.second ? BST_CHECKED : BST_UNCHECKED, 0);

		tmp.id = ID_BOOL;

		m_detail.push_back(tmp);

		y += 25;
	}

	for(const auto& pair : m_currentTile->m_additionalIntegerType) {
		TileDetail tmp;

		tmp.name = CreateWindowA("static", pair.first.c_str(), WS_CHILD | WS_VISIBLE,
			x, y, WindowRect.right, WindowRect.bottom, hWnd, (HMENU)-1, hInst, NULL);
		tmp.strName = pair.first;

		int sx = x + pair.first.size() * 10;
		tmp.value = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER
			, sx, y, sx + 200, 25, hWnd, (HMENU)m_detail.size(), hInst, NULL);
		SetWindowTextA(tmp.value, std::to_string(pair.second).c_str());

		tmp.id = ID_INT;

		m_detail.push_back(tmp);

		y += 25;
	}

	for (const auto& pair : m_currentTile->m_additionalStringType) {
		TileDetail tmp;

		tmp.name = CreateWindowA("static", pair.first.c_str(), WS_CHILD | WS_VISIBLE,
			x, y, WindowRect.right, WindowRect.bottom, hWnd, (HMENU)-1, hInst, NULL);
		tmp.strName = pair.first;

		int sx = x + pair.first.size() * 10;
		tmp.value = CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER
			, sx, y, sx + 200, 25, hWnd, (HMENU)m_detail.size(), hInst, NULL);
		SetWindowTextA(tmp.value, pair.second.c_str());

		tmp.id = ID_STRING;

		m_detail.push_back(tmp);

		y += 25;
	}

}

void DetailWindow::DeleteHWnd() {
	for(int i=0; i < m_detail.size(); i++) {
		DestroyWindow(m_detail[i].value);
		DestroyWindow(m_detail[i].name);
	}

	m_detail.clear();
}
