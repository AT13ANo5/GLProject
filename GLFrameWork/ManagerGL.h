#ifndef _MANAGER_GL_H_
#define _MANAGER_GL_H_
#include "main.h"
#include "Renderer.h"

struct LIGHT
{
	VECTOR4 Pos;
	COLOR Diffuse;
	COLOR Ambient;
	COLOR Specular;
};

class CRenderer;
class CScene;
class CCamera;
class CLight;
class CMouse;
class CKeyboard;

class CManager
{
public:
	CManager();
	virtual ~CManager();
	void Init(HINSTANCE hInstance,HWND hWnd);
	void Uninit(HWND hWnd);
	void Update(void);
	void Draw(void);
	CRenderer* GetRenderer(void){return Render;}
private:
	CRenderer* Render;
	CCamera* pCamera;
	CLight* Light;
	CMouse* Mouse;
	CKeyboard* Keyboard;
};

#endif