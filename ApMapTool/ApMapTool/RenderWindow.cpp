#include "stdafx.h"
#include "RenderWindow.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "TileNode.h"
#include "ResourceManager.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")

#define CUR_UP 1
#define CUR_DOWN -1
#define CUR_LEFT 2
#define CUR_RIGHT -2

GLubyte* LoadBmp(const char* Path, int* Width, int* Height) {

	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER* fh = NULL;
	BITMAPINFOHEADER* ih;
	BYTE* pRaster;


	hFile = CreateFileA(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		return NULL;
	}


	FileSize = GetFileSize(hFile, NULL);
	fh = (BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile, fh, FileSize, &dwRead, NULL);
	CloseHandle(hFile);


	int len = FileSize - fh->bfOffBits;
	pRaster = (GLubyte *)malloc(len);
	memcpy(pRaster, (BYTE *)fh + fh->bfOffBits, len);


	// RGB로 순서를 바꾼다.

	for (BYTE* p = pRaster; p < pRaster + len - 3; p += 3) {
		BYTE b = *p;
		*p = *(p + 2);
		*(p + 2) = b;

	}


	ih = (BITMAPINFOHEADER *)((PBYTE)fh + sizeof(BITMAPFILEHEADER));
	*Width = ih->biWidth;
	*Height = ih->biHeight;


	free(fh);
	return pRaster;

}

// 진짜 윈도우 프로시저.
static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM
                                lParam) {
	// 이 메시지를 받은 윈도우의 프로퍼티를 조사하여,
	// 적절한 인스턴스의 WndProc 함수로 호출을 넘겨줌.
	ChildWindow* pChildWindow = (ChildWindow *)GetProp(hWnd,
	                                                   L"PTR_INSTANCE");
	return pChildWindow->WndProc(hWnd, Msg, wParam, lParam);
}


LRESULT RenderWindow::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {



	renderThread = std::thread([&] { OnFrame(hWnd); });
	m_curRotationY = -90;

	return 0;
}


LRESULT RenderWindow::OnLButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	isMouseMoving = true;

	POINT mousePos;

	mousePos.x = LOWORD(lParam);
	mousePos.y = HIWORD(lParam);

	if (m_mousePos.x >= 0) {
		float Y = (mousePos.x - m_mousePos.x) * m_sensitivity;
		float X = (mousePos.y - m_mousePos.y) * m_sensitivity;

		m_mouseAccrueRotation.y += Y;
		m_mouseAccrueRotation.x += X;

		if (abs(m_mouseAccrueRotation.y) > 15) {
			m_worldRotationY += Y;
		}
		if (abs(m_mouseAccrueRotation.x) > 15) {
			m_worldRotationX += X;
		}
	}


	m_mousePos = mousePos;

	return 0;
}


LRESULT RenderWindow::OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	isMouseMoving = false;

	m_mousePos = POINT{-1, -1};
	m_mouseAccrueRotation = vec2{0, 0};

	m_worldRotationX = 0;
	m_worldRotationY = ((static_cast<int>(m_worldRotationY) + (m_worldRotationY > 0 ? 45 : -45)) / 90) * 90;
	
	if (m_worldRotationY >= 360) {
		m_worldRotationY = 360 - m_worldRotationY;
	} else if (m_worldRotationY < 0) {
		m_worldRotationY = 360 + m_worldRotationY;
	}

	m_curRotationY = m_worldRotationY;

	return 0;
}


LRESULT RenderWindow::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	MapProc* map = RESMGR->GetMapProc();
	TileNode& cur = map->GetCurTile();
	vec3 mapSize = m_mapSize = map->getMapSize();

	if (GetAsyncKeyState(VkKeyScanA('a')) & 0x8001) {
		int rotationY = abs(m_curRotationY);

		if (rotationY == 90 || rotationY == 270)
			MoveCurTile(cur, rotationY == 90 ? CUR_RIGHT : CUR_LEFT);
		else if (rotationY == 0 || rotationY == 180)
			MoveCurTile(cur, rotationY == 0 ? CUR_UP: CUR_DOWN);

	}
	else if (GetAsyncKeyState(VkKeyScanA('d')) & 0x8001) {
		int rotationY = abs(m_curRotationY);

		if (rotationY == 90 || rotationY == 270)
			MoveCurTile(cur, rotationY == 90 ? CUR_LEFT : CUR_RIGHT);
		else if (rotationY == 0 || rotationY == 180)
			MoveCurTile(cur, rotationY == 0 ? CUR_DOWN : CUR_UP);
	}

	if (GetAsyncKeyState(VK_SHIFT)) {
		if (GetAsyncKeyState(VkKeyScanA('s')) & 0x8001) {
			if (cur.y > 0) {
				cur.y--;
			}
			else
				cur.y = 0;
		}
		else if (GetAsyncKeyState(VkKeyScanA('w')) & 0x8001) {
			if (cur.y < mapSize.y) {
				cur.y++;
			}
			else
				cur.y = mapSize.y;
		}
	}
	else {
		if (GetAsyncKeyState(VkKeyScanA('s')) & 0x8001) {
			int rotationY = abs(m_curRotationY);

			if (rotationY == 90 || rotationY == 270)
				MoveCurTile(cur, rotationY == 90 ? CUR_UP : CUR_DOWN);
			else if (rotationY == 0 || rotationY == 180)
				MoveCurTile(cur, rotationY == 0 ? CUR_LEFT : CUR_RIGHT);
		}
		else if (GetAsyncKeyState(VkKeyScanA('w')) & 0x8001) {
			int rotationY = abs(m_curRotationY);

			if (rotationY == 90 || rotationY == 270)
				MoveCurTile(cur, rotationY == 90 ? CUR_DOWN : CUR_UP);
			else if (rotationY == 0 || rotationY == 180)
				MoveCurTile(cur, rotationY == 0 ? CUR_RIGHT : CUR_LEFT);
		}
	}


	map->SetTilePositionByIdPos(&cur, cur.x, cur.y, cur.z);

	if (GetAsyncKeyState(VkKeyScanA('e')) & 0x8001) {
		RESMGR->SetTitleUnSaved();
		RESMGR->SetTile(cur.id, cur.x, cur.y, cur.z);
	}else if(GetAsyncKeyState(VK_DELETE) & 0x8001 || GetAsyncKeyState(VkKeyScanA('q')) & 0x8001) {
		RESMGR->SetTitleUnSaved();
		RESMGR->GetMapProc()->DeleteTile(cur.x, cur.y, cur.z);
	}

	if(GetAsyncKeyState(VK_RETURN) & 0x8001) {
		ivec3 pos = ivec3{ cur.x, cur.y, cur.z };
		auto tmp = map->GetTile(pos);
		if(tmp != nullptr && tmp->id == RESMGR->GetAssetProc()->getPortalId()) {
			auto target = map->GetTileShortcut(tmp->m_additionalStringType["이동할 타일 이름"]);
			if(target.tile != nullptr && target.stage != -1) {
				map->SetStageId(target.stage);
				map->SetCurTile(target.pos);

			}
		}
	}

	if (GetAsyncKeyState(VK_F5) & 0x8001) {
		RESMGR->getMainProc()->DebugGame();
	}

	m_detailWindow.LoadCurrentTileInfo();

	return 0;
}


LRESULT RenderWindow::OnDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	isQuit = true;
	renderThread.join();

	PostQuitMessage(0);
	__super::OnDestroy(hWnd, wParam, lParam);

	return 0;
}


LRESULT RenderWindow::OnMouseWheel(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	if ((SHORT)HIWORD(wParam) > 0) { //마우스휠을 올릴 경우
		m_mouseScale += 0.3f;
	}

	else { //마우스휠을 내릴 경우
		m_mouseScale *= 0.7f;
	}

	return 0;
}


LRESULT RenderWindow::OnDefault(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	if (GetAsyncKeyState(VkKeyScanA('e')) || GetAsyncKeyState(VK_DELETE) || GetAsyncKeyState(VkKeyScanA('q'))) {
		
	}

	switch (Msg) {
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);

}


void RenderWindow::OnFrame(HWND _hWnd) {

	hdc = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;

	memset(&pfd, 0, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	nPixelFormat = ChoosePixelFormat(hdc, &pfd);

#ifdef NDEBUG
	SetPixelFormat(hdc, nPixelFormat, &pfd);
#endif
	

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	glOrtho(-WindowRect.right, WindowRect.right, -WindowRect.bottom, WindowRect.bottom, -500, 500);
	gluLookAt(50, 0, 0, 0, 0, 0, 0, 10, 0);


	//glEnable(GL_COLOR_MATERIAL);


	//texture
	//SetTextures();
	//int w, h;
	//data = LoadBmp("test.bmp", &w, &h);
	//glGenTextures(1, &m_texture[0]);					// Create The Texture
	//// Typical Texture Generation Using Data From The Bitmap
	//glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//free(data);

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glBindTexture(GL_TEXTURE_2D, 0);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading

	while (!isQuit) {
		Update(0);
		Render(0);
		//m_worldRotationY += 0.5f;
		Sleep(10);
	}

	DeleteTextures();

	wglMakeCurrent(hdc, nullptr);
	wglDeleteContext(hrc);
	ReleaseDC(hWnd, hdc);

	delete[] spotStartPosition;

}


void RenderWindow::Render(float elapsedTime) const {

	glViewport(0, 0, WindowRect.right, WindowRect.bottom);

	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_RESCALE_NORMAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, spotStartPosition);

	GLfloat ambient[4] = {1,1,1,1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_DEPTH_TEST);

	if(RESMGR->GetMapProc()->isEmptyStage()) {
		glEnable(GL_DEPTH_TEST);
	}
	const auto tiles = RESMGR->GetMapProc()->GetTiles();

	glPushMatrix();

	glRotatef(m_worldRotationX, 0, 0, -1);
	glRotatef(m_worldRotationY, 0, 1, 0);

	

	for (int i = 0; i < tiles.size(); i++) {
		const auto& tile = tiles[i];
		glPushMatrix();
		glScalef(m_mouseScale, m_mouseScale, m_mouseScale);
		glTranslatef(tile->Position.x, tile->Position.y, tile->Position.z);
		drawBox(TILE_SIZE, RESMGR->GetAssetProc()->FindTextureIdtoId(tile->id));
		glPopMatrix();
	}

	

	glEnd();
	glFrontFace(GL_CCW);

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glColor3f(0, 1, 0);
	glLineWidth(3);
	TileNode cur = RESMGR->GetMapProc()->GetCurTile();
	glScalef(m_mouseScale, m_mouseScale, m_mouseScale);
	glPushMatrix();
	glTranslatef(cur.Position.x, cur.Position.y, cur.Position.z);
	glutWireCube(TILE_SIZE + 2);
	glPopMatrix();


	/* 미세한 격자를 그린다. */
	glLineWidth(1);
	glBegin(GL_LINES);
	
	for (float xtmp = -mapSizePos.x; xtmp <= mapSizePos.x; xtmp += TILE_SIZE) {
		glColor3f(1, 0, 0);
		glVertex3f(xtmp, -mapSizePos.y, mapSizePos.z);
		glVertex3f(xtmp, mapSizePos.y, mapSizePos.z);
		glColor3f(1, 1, 0);
		glVertex3f(xtmp, -mapSizePos.y, -mapSizePos.z);
		glVertex3f(xtmp, mapSizePos.y, -mapSizePos.z);
	}

	for (float xtmp = -mapSizePos.y; xtmp <= mapSizePos.y; xtmp += TILE_SIZE) {
		glColor3f(1, 0, 0);
		glVertex3f(-mapSizePos.x, xtmp, mapSizePos.z);
		glVertex3f(mapSizePos.x, xtmp, mapSizePos.z);
		glColor3f(1, 1, 0);
		glVertex3f(-mapSizePos.x, xtmp, -mapSizePos.z);
		glVertex3f(mapSizePos.x, xtmp, -mapSizePos.z);
		glColor3f(0, 0, 1);
		glVertex3f(mapSizePos.x, xtmp, mapSizePos.z);
		glVertex3f(mapSizePos.x, xtmp, -mapSizePos.z);
		glColor3f(0, 1, 1);
		glVertex3f(-mapSizePos.x, xtmp, mapSizePos.z);
		glVertex3f(-mapSizePos.x, xtmp, -mapSizePos.z);

	}

	for (float xtmp = -mapSizePos.z; xtmp <= mapSizePos.z; xtmp += TILE_SIZE) {
		glColor3f(0, 0, 1);
		glVertex3f(mapSizePos.x, -mapSizePos.y, xtmp);
		glVertex3f(mapSizePos.x, mapSizePos.y, xtmp);
		glColor3f(0, 1, 1);
		glVertex3f(-mapSizePos.x, -mapSizePos.y, xtmp);
		glVertex3f(-mapSizePos.x, mapSizePos.y, xtmp);
	}
	glScalef(m_mouseScale, m_mouseScale, m_mouseScale);
	glEnd();

	glPopMatrix();
	glFinish();
}


void RenderWindow::Update(float elapsedTime) {

	RESMGR->getMainProc()->AutoSaveFile();

	if (m_worldRotationY >= 360) {
		m_worldRotationY = 360 - m_worldRotationY;
	}
	else if (m_worldRotationY <= -360) {
		m_worldRotationY = 360 + m_worldRotationY;
	}

	if(RESMGR->GetAssetProc()->isAddedTile()) {
		RESMGR->GetAssetProc()->SetAddedTile(false);
		SetTextures();
	}


	vec3 mapSize = RESMGR->GetMapProc()->getMapSize();

	mapSizePos.x = (mapSize.x * TILE_SIZE) / 2.f + TILE_SIZE / 2;
	mapSizePos.y = (mapSize.y * TILE_SIZE) / 2.f + TILE_SIZE / 2;
	mapSizePos.z = (mapSize.z * TILE_SIZE) / 2.f + TILE_SIZE / 2;

	


}


RenderWindow::RenderWindow(DetailWindow& detail_window) : ChildWindow(), m_detailWindow(detail_window) {
}


RenderWindow::~RenderWindow() {
}


int RenderWindow::Create(HINSTANCE hInstance, int x, int y, int width, int height, HWND hParentWnd) {
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
		WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		WndClass.hInstance = hInstance;
		WndClass.lpfnWndProc = ::WndProc;
		WndClass.lpszClassName = ClassName;
		WndClass.lpszMenuName = NULL;
		WndClass.style = 0;
		RegisterClass(&WndClass);
	}

	// 윈도우 생성.
	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, ClassName, ClassName, WS_CHILD |
	                                      WS_VISIBLE, x, y, width, height, hParentWnd, NULL, hInstance, NULL);

	// 윈도우 프로퍼티에 현재 인스턴스 포인터 저장해두기.
	SetProp(hWnd, L"PTR_INSTANCE", (HANDLE)this);

	//init GLEW
	glewInit();

	return true;
}


void RenderWindow::SetTextures() {
	AssetProc* asset_proc = RESMGR->GetAssetProc();

	int size = asset_proc->getAssetInfoSize();

	for(int i=0; i < size; i++) {
		
		int w, h;
		int* id = asset_proc->getTextureId(i);

		if (id[0] != 0)
			continue;

		data = LoadBmp(asset_proc->getFilePath(i).c_str(), &w, &h);
		if(data == nullptr) {
			continue;
		}
		glGenTextures(1, reinterpret_cast<GLuint*>(id));					// Create The Texture
															// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, id[0]);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		free(data);

	}

}


void RenderWindow::DeleteTextures() {

	//AssetProc* asset_proc = RESMGR->GetAssetProc();

	//int size = asset_proc->getAssetInfoSize();

	//for (int i = 0; i < size; i++) {

	//	int* id = asset_proc->getTextureId(i);

	//	if (id == nullptr) {
	//		continue;
	//	}

	//	//glDeleteTextures(1, reinterpret_cast<GLuint*>(id));
	//}

}


void RenderWindow::drawBox(GLfloat size, GLuint textureid) const {

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	glBindTexture(GL_TEXTURE_2D, textureid);

	for (int i = 5; i >= 0; i--) {
		glBegin(GL_QUADS);
		glNormal3fv(&n[i][0]);
		glTexCoord2f(0, 1);
		glVertex3fv(&v[faces[i][0]][0]);

		glTexCoord2f(0, 0);
		glVertex3fv(&v[faces[i][1]][0]);

		glTexCoord2f(1, 0);
		glVertex3fv(&v[faces[i][2]][0]);

		glTexCoord2f(1, 1);
		glVertex3fv(&v[faces[i][3]][0]);
		glEnd();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}


void RenderWindow::MoveCurTile(TileNode& cur, int flag) {

	if (flag == CUR_LEFT) {
		if (cur.x > 0) {
			cur.x--;
		} else
			cur.x = 0;
	} else if (flag == CUR_RIGHT) {
		if (cur.x < m_mapSize.x) {
			cur.x++;
		} else
			cur.x = m_mapSize.x;
	}
	else if (flag == CUR_DOWN) {
		if (cur.z > 0) {
			cur.z--;
		} else
			cur.z = 0;
	} else if (flag == CUR_UP) {
		if (cur.z < m_mapSize.z) {
			cur.z++;
		} else
			cur.z = m_mapSize.z;
	}

}

