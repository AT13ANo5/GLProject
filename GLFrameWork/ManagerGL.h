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

enum
{
	SCENE_SPLASH = 0,
	SCENE_TITLE,
	SCENE_CONNECTION,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
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

	static void ChangeScene(short next);

	CRenderer* GetRenderer(void){return Render;}
private:
	CScene* Scene;
	CRenderer* Render;
	CCamera* pCamera;
	CLight* Light;
	CMouse* Mouse;
	CKeyboard* Keyboard;

	static short NextScene;
	static bool ChangeFlag;
};

#endif