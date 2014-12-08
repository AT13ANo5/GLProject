#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "CInput.h"
class CMouse:public CInput
{
public:
	enum
	{
		MOUSE_LEFT =0,
		MOUSE_RIGHT,
		MOUSE_CENTER,
		MOUSE_MAX,
	};

	enum
	{
		WHEEL_NEUTRAL=0,
		WHEEL_UP,
		WHEEL_DOWN,
		WHEEL_MAX
	};

	HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	void Update(void);

	static float GetPosX(void);	//X座標取得
	static float GetPosY(void);	//Y座標取得
	static int GetWheel(void);	//ウィールを取得
	static bool GetPress(int nKey);	//プレス情報
	static bool GetTrigger(int nKey);	//トリガー情報
	static bool GetRelease(int nKey);	//リリース情報
	static void SetHWND(HWND hWnd){m_phWnd = hWnd;}
private:
	static DIMOUSESTATE2 MouseStatus;				//マウスのステータス保存用
	static BYTE TriggerMouse[3];							//マウスボタンのトリガー情報
	static BYTE ReleaseMouse[3];							//マウスボタンのリリース情報
	static int WheelStatus;								//ウィールの情報
	static HWND m_phWnd;
	static POINT pos;
};

#endif