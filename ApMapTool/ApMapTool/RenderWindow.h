#pragma once
#include "ChildWindow.h"
#include <thread>
#include "Vector.h"
#include <GL/glew.h>

class TileNode;

class RenderWindow : public ChildWindow {
protected:
	LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
	LRESULT OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) override;
	void OnFrame();
	void Render(float elapsedTime) const;
	void Update(float elapsedTime);
public:
	RenderWindow();
	~RenderWindow();

	int Create(HINSTANCE hInstance, int x, int y, int width, int height,
	           HWND hParentWnd);

	void SetTextures();
	void DeleteTextures();
private:
	void drawBox(GLfloat size, GLuint textureid) const;
	void MoveCurTile(TileNode& cur, int flag);
private:
	HDC hdc;
	HGLRC hrc;
	std::thread renderThread;

	vec3 mapSizePos;
	vec3 m_mapSize;

	float m_worldRotationY = 270.f;
	float m_worldRotationX = 0.f;
	int m_curRotationY;

	float m_gridPositionXZ = 0.f;
	float m_gridPositionZZ = 0.f;
	int m_gridXY = 0.f;
	bool b_isGridX;

	float *spotStartPosition = new float[4]{ 200, 250, 150, 0.1f }; //±¤¿øÀ§Ä¡(µ¿Â÷ÁÂÇ¥)

	bool isMouseMoving = false;
	POINT m_mousePos = POINT{ -1, -1 };
	vec2 m_mouseAccrueRotation = vec2{ 0, 0 };

	float m_mouseScale = 1.f;

	GLubyte* data;

	GLfloat n[6][3] =
	{
		{ -1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 0.0, -1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 0.0, 0.0, -1.0 }
	};
	GLint faces[6][4] =
	{
		{ 0, 1, 2, 3 },
		{ 3, 2, 6, 7 },
		{ 7, 6, 5, 4 },
		{ 4, 5, 1, 0 },
		{ 5, 6, 2, 1 },
		{ 7, 4, 0, 3 }
	};
	mutable GLfloat v[8][3];

public:
	float m_sensitivity = 0.5f;
};
