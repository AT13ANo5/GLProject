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
	static HDC OpenGLDevice;	//OpenGL�̃f�o�C�X
	static HGLRC OpenGLContext; //OpenGL�̃R���e�L�X�g�n���h��

};


#endif