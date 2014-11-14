#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

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

private:
	static HDC OpenGLDevice;	//OpenGLのデバイス
	static HGLRC OpenGLContext; //OpenGLのコンテキストハンドル

};


#endif