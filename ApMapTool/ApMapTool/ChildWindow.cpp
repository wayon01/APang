#include "stdafx.h"
#include "ChildWindow.h"


int ChildWindow::bRegisterClass = false;

// 진짜 윈도우 프로시저.
static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM
                                lParam) {
	// 이 메시지를 받은 윈도우의 프로퍼티를 조사하여,
	// 적절한 인스턴스의 WndProc 함수로 호출을 넘겨줌.
	ChildWindow* pChildWindow = (ChildWindow *)GetProp(hWnd,
	                                                   L"PTR_INSTANCE");
	return pChildWindow->WndProc(hWnd, Msg, wParam, lParam);
}


// constructor. 배경색을 나타내는 Color 변수를 초기화함.
ChildWindow::ChildWindow(void) {
	Color = 0xeeeeee;
}


// 차일드 윈도우 생성해주는 function.
int ChildWindow::Create(HINSTANCE hInstance, int x, int y, int width, int height, HWND hParentWnd) {
	TCHAR* ClassName = L"ChildWindow";

	WindowRect = RECT{x, y, width, height};

	// 윈도우 클래스는 딱 한번만 등록시키도록 함.
	if (!bRegisterClass) {
		bRegisterClass = true;

		WNDCLASS WndClass;
		WndClass.cbClsExtra = WndClass.cbWndExtra = 0;
		WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		//WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
		WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = ::WndProc;
		WndClass.lpszClassName = ClassName;
		WndClass.lpszMenuName = nullptr;
		WndClass.style = 0;
		RegisterClass(&WndClass);
	}

	// 윈도우 생성.
	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, ClassName, ClassName, WS_CHILD |
	                      WS_VISIBLE, x, y, width, height, hParentWnd, NULL, hInstance, NULL);

	hInst = hInstance;
	// 윈도우 프로퍼티에 현재 인스턴스 포인터 저장해두기.
	SetProp(hWnd, L"PTR_INSTANCE", (HANDLE)this);

	return true;
}


// 클래스의 멤버 함수로 내장된 윈도우 프로시저. 전역 WndProc로부터 호출됨.
LRESULT ChildWindow::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (this == nullptr) return DefWindowProc(hWnd, Msg, wParam, lParam);
	if (!isInited && hWnd != nullptr) {
		OnCreate(hWnd, wParam, lParam);
		isInited = true;
	}

	switch (Msg) {
		case WM_DESTROY: return OnDestroy(hWnd, wParam, lParam);
		case WM_PAINT: return OnPaint(hWnd, wParam, lParam);
		case WM_KEYDOWN:
			return OnKeyDown(hWnd, wParam, lParam);
		case WM_MOUSEWHEEL:
			return OnMouseWheel(hWnd, wParam, lParam);
		case WM_LBUTTONDOWN: {
			SetFocus(hWnd);
			m_isButtonDown = true;
			return OnLButtonDown(hWnd, wParam, lParam);
		}
		case WM_MOUSEMOVE: {
			if (m_isButtonDown)
				return OnLButtonDown(hWnd, wParam, lParam);

			break;
		}
		case WM_LBUTTONUP: {
			m_isButtonDown = false;
			return OnLButtonUp(hWnd, wParam, lParam);
		}
	}

	return OnDefault(hWnd, Msg, wParam, lParam);
}


void ChildWindow::SetWindowColor(int color) {
	Color = color;
	InvalidateRect(hWnd, NULL, true);
}


void ChildWindow::SetWindowSize(int x, int y, int w, int h) {
	WindowRect = RECT{ x, y, w, h };
	SetWindowPos(hWnd, nullptr, x, y, w, h, NULL);
}


// WM_DESTROY.
LRESULT ChildWindow::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	RemoveProp(hWnd, L"PTR_INSTANCE");
	return 0;
}


// WM_PAINT.
LRESULT ChildWindow::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);

	// 윈도우의 사각형 구하기.
	RECT rect;
	GetClientRect(hWnd, &rect);

	// Color 변수에 따라 색깔 칠하기.
	HBRUSH hMyBrush = CreateSolidBrush(Color);
	FillRect(hDC, &rect, hMyBrush);
	DeleteObject(hMyBrush);

	EndPaint(hWnd, &ps);
	return 0;
}


// WM_LBUTTONDOWN.
LRESULT ChildWindow::OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//Color = (Color == 0) ? 1 : 0;
	//InvalidateRect(hWnd, NULL, true);
	return 0;
}


LRESULT ChildWindow::OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	return 0;
}


LRESULT ChildWindow::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	return 0;
}


LRESULT ChildWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	return 0;
}


LRESULT ChildWindow::OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	return 0;
}


LRESULT ChildWindow::OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}
