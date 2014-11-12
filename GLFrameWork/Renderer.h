#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

#define SCENE_MAX (10)
class CObject;
class CRenderer
{
public:
	CRenderer(HWND hWnd){Init(hWnd);}
	~CRenderer(){}
	static HRESULT Init(HWND hWnd);
	static void Uninit(HWND hWnd);
	void Update(void);
	void Draw(void);
	void AddScene(CObject* Scene);

private:
	static CObject* pScenes[];
	static HDC OpenGLDevice;	//OpenGLのデバイス
	static HGLRC OpenGLContext; //OpenGLのコンテキストハンドル
	static int SceneNum;

};


#endif