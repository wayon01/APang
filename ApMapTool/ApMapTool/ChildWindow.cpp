#include "stdafx.h"
#include "ChildWindow.h"


int ChildWindow::bRegisterClass = false;

// ��¥ ������ ���ν���.
static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM
                                lParam) {
	// �� �޽����� ���� �������� ������Ƽ�� �����Ͽ�,
	// ������ �ν��Ͻ��� WndProc �Լ��� ȣ���� �Ѱ���.
	ChildWindow* pChildWindow = (ChildWindow *)GetProp(hWnd,
	                                                   L"PTR_INSTANCE");
	return pChildWindow->WndProc(hWnd, Msg, wParam, lParam);
}


// constructor. ������ ��Ÿ���� Color ������ �ʱ�ȭ��.
ChildWindow::ChildWindow(void) {
	Color = 0xeeeeee;
}


// ���ϵ� ������ �������ִ� function.
int ChildWindow::Create(HINSTANCE hInstance, int x, int y, int width, int height, HWND hParentWnd) {
	TCHAR* ClassName = L"ChildWindow";

	WindowRect = RECT{x, y, width, height};

	// ������ Ŭ������ �� �ѹ��� ��Ͻ�Ű���� ��.
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

	// ������ ����.
	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, ClassName, ClassName, WS_CHILD |
	                      WS_VISIBLE, x, y, width, height, hParentWnd, NULL, hInstance, NULL);

	hInst = hInstance;
	// ������ ������Ƽ�� ���� �ν��Ͻ� ������ �����صα�.
	SetProp(hWnd, L"PTR_INSTANCE", (HANDLE)this);

	return true;
}


// Ŭ������ ��� �Լ��� ����� ������ ���ν���. ���� WndProc�κ��� ȣ���.
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

	// �������� �簢�� ���ϱ�.
	RECT rect;
	GetClientRect(hWnd, &rect);

	// Color ������ ���� ���� ĥ�ϱ�.
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
