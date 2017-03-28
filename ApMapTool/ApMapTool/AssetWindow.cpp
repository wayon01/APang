#include "stdafx.h"
#include "AssetWindow.h"
#include <string>
#include "ResourceManager.h"

#define ID_LISTBOX 100


AssetWindow::AssetWindow() {
}


AssetWindow::~AssetWindow() {
}


LRESULT AssetWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	if (isLoad) return 0;

	isLoad = true;
	hList = CreateWindowA("listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY | WS_VSCROLL | WS_BORDER, 0, 0, WindowRect.right, WindowRect.bottom, this->hWnd, (HMENU)ID_LISTBOX, hInst, NULL);

	LoadAssetList();
	return 0;
}


LRESULT AssetWindow::OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
				RESMGR->GetMapProc()->SetCurTileId(RESMGR->GetAssetProc()->GetAssetID(i));
				break;
			}
		}
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}


void AssetWindow::SetWindowSize(int x, int y, int w, int h) {
	__super::SetWindowSize(x, y, w, h);

	SetWindowPos(hList, nullptr, 0, 0, w, h, NULL);

}


void AssetWindow::LoadAssetList() {
	RESMGR->GetAssetProc()->LoadAssetList();

	AssetProc* asset_proc = RESMGR->GetAssetProc();

	SendMessageA(hList, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < asset_proc->getAssetInfoSize(); i++)
		SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)asset_proc->GetAssetName(i).c_str());

}


void AssetWindow::SaveAssetList() const {
	RESMGR->GetAssetProc()->SaveAssetList();


}


void AssetWindow::SetAssetInfo(std::string name, std::string strId, std::string filePath) {

	RESMGR->GetAssetProc()->SetAssetInfo(name, strId, filePath);
	SaveAssetList();
	LoadAssetList();
}
