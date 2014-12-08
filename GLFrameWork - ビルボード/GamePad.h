//=============================================================================
// ゲームパッド処理
//CreateBy Arai Yuhki
//=============================================================================
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
//=============================================================================
// インクルード
//=============================================================================
#include "CInput.h"
//=============================================================================
// クラス定義
//=============================================================================
class CGamePad:public CInput
{
public:
enum
{
	A_BUTTON = 0,
	B_BUTTON,
	X_BUTTON,
	Y_BUTTON,
	L1_BUTTON,
	R1_BUTTON,
	SELECT_BUTTON,
	START_BUTTON,
	L3_BUTTON,
	R3_BUTTON,
	MAX
};

	CGamePad(){}
	~CGamePad(){}
	HRESULT Init(HINSTANCE hInstance,HWND hWnd);
	void Update(void);

	float GetlX(void);
	float GetlY(void);
	float GetlRX(void);
	float GetlRY(void);
	
	bool GetPress(int nButton){return PadState.rgbButtons[nButton]? true:false;}
	bool GetTrigger(int nButton){return PadTriggerState.rgbButtons[nButton]? true:false;}
	bool GetRelease(int nButton){return PadReleaseState.rgbButtons[nButton]? true:false;}

	
	static BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE *pdidInstance,VOID *pContext);//ゲームパッドコールバック関数
	static BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext );

private:
	DIJOYSTATE PadState;
	DIJOYSTATE PadTriggerState;
	DIJOYSTATE PadReleaseState;

	DIDEVCAPS g_diDevCaps;
	
};

#endif