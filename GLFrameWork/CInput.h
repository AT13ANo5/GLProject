#ifndef _CINPUT_H_
#define _CINPUT_H_
#include "main.h"

class CInput
{
public:
	CInput();
	virtual ~CInput(){}
	virtual HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void)=0;

protected:
	static LPDIRECTINPUT8 DInput;
	LPDIRECTINPUTDEVICE8 pInputDevice;
};

#endif