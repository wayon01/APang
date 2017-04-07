// ApMapTool.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"
#include "ApMapTool.h"
#include <commctrl.h>
#include "ChildWindow.h"
#include "RenderWindow.h"
#include "MainProc.h"
#include "ResourceManager.h"
#include "AssetWindow.h"
#include "StageWindow.h"

#pragma comment (lib, "Comctl32.lib")
#define _CRT_SECURE_NO_WARNINGS

#define MAX_LOADSTRING 100
#define ID_TOOLBAR 100
#define MAX_CHILDWIN 4

// ���� ����:
HINSTANCE hInst; // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING]; // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING]; // �⺻ â Ŭ���� �̸��Դϴ�.

HWND hToolBar;
DetailWindow detailWindow;
RenderWindow renderWindow(detailWindow);
AssetWindow assetWindow;
StageWindow stageWindow(detailWindow);
ChildWindow childWindow[4];

MainProc* mainProc;

RECT WindowSize;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MapSizeDig(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AddAssetDig(HWND, UINT, WPARAM, LPARAM);
void InitToolbar(HWND hWnd);
void SaveFile(HWND hWnd);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                           _In_opt_ HINSTANCE hPrevInstance,
                           _In_ LPWSTR lpCmdLine,
                           _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_APMAPTOOL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APMAPTOOL));

	MSG msg;

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APMAPTOOL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_APMAPTOOL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	WindowSize = RECT{ 0, 0, 1280, 720 };

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		WindowSize.left + 100, WindowSize.top + 100, WindowSize.right, WindowSize.bottom, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	RESMGR->setMainHWND(hWnd);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_MAPSIZE:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAPSIZEDIG), hWnd, MapSizeDig);
			break;
		case IDM_ADDASSET:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDASSETDIG), hWnd, AddAssetDig);
			break;
		case IDM_ADDAREA: {
			MapVector tmp = new MapNode();
			tmp->id = RESMGR->GetMapProc()->GetMapStage().size();
			RESMGR->GetMapProc()->MapStagePushBack(tmp->id, tmp);
			stageWindow.LoadStageList();
			RESMGR->SetTitleUnSaved();
		}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 11: { //�ҷ�����
			if (RESMGR->b_isChanged) {
				if (MessageBoxA(hWnd, "��� �����ϸ� ���� ���� ������� �ʽ��ϴ�.", "���", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			}
			OPENFILENAMEA OFN;
			char lpstrFile[MAX_PATH] = "";
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = "���� �� ����(*.map)\0*.map\0��� ����(*.*)\0*.*\0";
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;

			if (GetOpenFileNameA(&OFN)) {
				if (RESMGR->b_isChanged) {
					if (MessageBoxA(hWnd, "��� �����ϸ� ���� ���� ������� �ʽ��ϴ�.", "���", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
						return DefWindowProc(hWnd, message, wParam, lParam);
					}
				}
				RESMGR->GetMapProc()->ClearAllMap();
				mainProc->setFilePath(lpstrFile);
				mainProc->ReadMapFile();
				stageWindow.LoadStageList();
				SetWindowTextA(hWnd, mainProc->getTitleName().c_str());
				RESMGR->SetTitleSaved();
				detailWindow.LoadCurrentTileInfo();
			}
		}
				 break;
		case 12: {
			SaveFile(hWnd);
		}
				 break;

		case ID_32773: {
			RESMGR->GetMapProc()->StartRouteSimulation();
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_CREATE: {
		mainProc = new MainProc();
		RESMGR->SetMainProc(mainProc);
		SetWindowTextA(hWnd, mainProc->getTitleName().c_str());
		renderWindow.m_detailWindow = detailWindow;
		renderWindow.Create(hInst, WindowSize.left, WindowSize.top + 30, WindowSize.right / 2, WindowSize.bottom / 2, hWnd);
		renderWindow.SetWindowColor(0xffffff);
		assetWindow.Create(hInst, WindowSize.right / 2, WindowSize.top + 30, WindowSize.right / 2, WindowSize.bottom / 2, hWnd);
		detailWindow.Create(hInst, WindowSize.left, WindowSize.bottom / 2, WindowSize.right / 2, WindowSize.bottom / 2 - 30, hWnd);
		stageWindow.Create(hInst, WindowSize.right / 2, WindowSize.bottom / 2, WindowSize.right / 2, WindowSize.bottom / 2 - 30, hWnd);
		InitToolbar(hWnd);
		//HWND childHwnd = CreateWindowEx(WS_EX_CLIENTEDGE, szWindowClass, NULL, WS_CHILD | WS_VISIBLE, 0, 300, 300, 300, hWnd, NULL, hInst, NULL);

	}
	case WM_LBUTTONDOWN: {
	}
						 break;
	case WM_SIZE: {
		SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);

		GetClientRect(hWnd, &WindowSize);
		renderWindow.SetWindowSize(WindowSize.left, WindowSize.top + 30, WindowSize.right  * (2 / 3.0f), WindowSize.bottom * (2 / 3.0f));
		assetWindow.SetWindowSize(WindowSize.right  * (2 / 3.0f), WindowSize.top + 30, WindowSize.right / 3, WindowSize.bottom / 2);
		detailWindow.SetWindowSize(WindowSize.left, WindowSize.bottom * (2 / 3.0f), WindowSize.right  * (2 / 3.0f), WindowSize.bottom / 3);
		stageWindow.SetWindowSize(WindowSize.right  * (2 / 3.0f), WindowSize.bottom / 2, WindowSize.right / 3, WindowSize.bottom / 2);

	}
				  break;
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
	}
				   break;
	case WM_DESTROY:
		if (RESMGR->b_isChanged) {
			if (MessageBoxA(hWnd, "��� �����ϸ� ���� ���� ������� �ʽ��ϴ�.", "���", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		PostQuitMessage(0);
		ResourceManager::getInstance()->delInstance();
		delete mainProc;
		break;

	case WM_KEYDOWN: {
		
		if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VkKeyScanA('s')) & 0x8001) {
			if(mainProc->getFileName() != "untitled.map") {
				mainProc->setFilePath(mainProc->getFilePath());
				mainProc->SaveMapFile();
				RESMGR->SetTitleSaved();
			}else {
				SaveFile(hWnd);
			}
		}

		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND: {
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

		}
			break;
	}
	return (INT_PTR)FALSE;
}

// �� ũ�� ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK MapSizeDig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		break;
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND: {
			if (LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

			if(LOWORD(wParam) == IDOK) {
			
				auto hX = GetDlgItem(hDlg, IDC_EDITX);
				auto hY = GetDlgItem(hDlg, IDC_EDITY);
				auto hZ = GetDlgItem(hDlg, IDC_EDITZ);

				
				char hxStr[8];
				char hyStr[8];
				char hzStr[8];

				GetWindowTextA(hX, hxStr, 8);
				GetWindowTextA(hY, hyStr, 8);
				GetWindowTextA(hZ, hzStr, 8);

				int x = std::atoi(hxStr);
				int y = std::atoi(hyStr);
				int z = std::atoi(hzStr);

				if (x < 0 || y < 0 || z < 0) break;

				vec3 mapSize = RESMGR->GetMapProc()->getMapSize() + vec3{ 1, 1, 1 };

				if(x < mapSize.x || y < mapSize.y || z < mapSize.z) {
					if (MessageBoxA(hDlg, "���� ũ�Ⱑ �۾����鼭 �Ϻ� �� �����Ͱ� ������ϴ�.", "���", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
						break;
					}
				}

				RESMGR->SetTitleUnSaved();
				RESMGR->SetMapSize(x, y, z);
				RESMGR->GetMapProc()->RefreshMap();

				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}

		}
			 break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK AddAssetDig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			if(LOWORD(wParam) == IDOK) {
				auto hName = GetDlgItem(hDlg, IDC_ASSETNAMEEDIT);
				auto hId = GetDlgItem(hDlg, IDC_ASSETIDEDIT);
				auto hFilePath = GetDlgItem(hDlg, IDC_TEXTUREPATH);


				char hNameStr[256];
				char hIdStr[256];
				char hFilePathStr[256];

				GetWindowTextA(hName, hNameStr, 256);
				GetWindowTextA(hId, hIdStr, 256);
				GetWindowTextA(hFilePath, hFilePathStr, 256);

				assetWindow.SetAssetInfo(hNameStr, hIdStr, hFilePathStr);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

	}
	 break;
	}
	return (INT_PTR)FALSE;
}


void InitToolbar(HWND hWnd) {
	TBBUTTON ToolBtn[20] = {
		{STD_FILENEW,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_FILEOPEN,11,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_FILESAVE,12,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_CUT,13,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_COPY,14,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PASTE,15,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_DELETE,16,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_UNDO,17,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_REDOW,18,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_PRINT,19,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PRINTPRE,20,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_FIND,21,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_REPLACE,22,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PROPERTIES,23,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_HELP,24,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0}
	};

	hToolBar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER,
	                           ID_TOOLBAR, 15, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, ToolBtn, 20,
	                           16, 16, 16, 16, sizeof(TBBUTTON));
}


void SaveFile(HWND hWnd) {

	OPENFILENAMEA OFN;
	char * b = new char[MAX_PATH];
	strcpy_s(b, mainProc->getFileName().length() + 1, mainProc->getFileName().c_str());
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = "���� �� ����(*.map)\0*.map\0��� ����(*.*)\0*.*\0";
	OFN.lpstrFile = b;
	OFN.nMaxFile = 256;

	if (GetSaveFileNameA(&OFN)) {
		//if (!RESMGR->b_isChanged) {
		//	break;
		//}
		mainProc->setFilePath(b);
		mainProc->SaveMapFile();
		RESMGR->SetTitleSaved();
	}
}
