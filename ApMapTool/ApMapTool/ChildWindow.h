#pragma once
class ChildWindow {
protected:
	HWND hWnd = nullptr; // ������ ������ �ڵ�.
	HINSTANCE hInst;
	static int bRegisterClass; //������Ŭ����(WNDCLASS)�� ��ϵǾ����� ��Ÿ���� ����.
	int Color; // 0�̸� �Ķ���, 1�̸� ������ ���.

	RECT WindowRect;

	virtual LRESULT OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

public:
	ChildWindow(); // constructor
	int Create(HINSTANCE hInstance, int x, int y, int width, int height,
	           HWND hParentWnd);
	LRESULT WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void SetWindowColor(int color);
	virtual void SetWindowSize(int x, int y, int w, int h);


protected:
	bool isInited = false;
	bool isQuit = false;

private:
	bool m_isButtonDown = false;
};
