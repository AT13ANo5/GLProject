//=============================================================================
// キーボードクラス
// CreateBy Arai Yuhki
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
//=============================================================================
// インクルード
//=============================================================================
#include "CInput.h"
//=============================================================================
// クラス定義
//=============================================================================
class CKeyboard:public CInput
{
public:
	CKeyboard(){}
	~CKeyboard(){}
	HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	void Update(void);

	static bool GetPress(int nKey);
	static bool GetTrigger(int nKey);
	static bool GetRelease(int nKey);
	static bool GetRepeat(int nKey);

private:
	static BYTE g_aKeyState[256];		//キー入力情報
	static BYTE TriggerKeyState[256];	//トリガーキー入力情報
	static BYTE ReleaseKeyState[256];	//リリースキー入力情報
	static BYTE RepeateKeyState[256];	//リピートキー入力情報
	static short RepeateKeyFlame[256];	//リピートキー用フレームカウント
	//static LPDIRECTINPUTDEVICE8 pInputDevice;

};

#endif