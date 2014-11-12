#include "Mouse.h"

HWND CMouse::m_phWnd;
POINT CMouse::pos;

DIMOUSESTATE2 CMouse::MouseStatus;
BYTE CMouse::TriggerMouse[3];
BYTE CMouse::ReleaseMouse[3];
int CMouse::WheelStatus;
//=============================================================================
//初期化
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance,HWND hWnd)
{
	DIPROPDWORD dipdw;
	if(FAILED(CInput::Init(hInstance,hWnd)))
	{
		return E_FAIL;
	}
	// 「DirectInputデバイス」オブジェクトの作成
	if( FAILED( DInput->CreateDevice( GUID_SysMouse,&pInputDevice, NULL ) ) )
	{
		return E_FAIL;
	} 
	
	// デバイスをマウスに設定
	if( FAILED( pInputDevice->SetDataFormat( &c_dfDIMouse2 ) ) )
	{
		return E_FAIL;
	}
	// 協調レベルの設定
	if( FAILED( pInputDevice->SetCooperativeLevel( 
		hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )) )
	{
		return E_FAIL;
	}

	if(FAILED(pInputDevice->SetProperty(DIPROP_AXISMODE,&dipdw.diph)))
	{
		return E_FAIL;
	}


	// デバイスを「取得」する
	if(FAILED(pInputDevice->Acquire()))
	{
		return E_FAIL;
	}
	m_phWnd = hWnd;
	WheelStatus = WHEEL_NEUTRAL;
	return S_OK;
}
//=============================================================================
//更新
//=============================================================================
void CMouse::Update(void)
{
	DIMOUSESTATE2 CurrentMouseStatus;
	if(FAILED(pInputDevice->GetDeviceState(sizeof(DIMOUSESTATE2),&CurrentMouseStatus)))
	{
		pInputDevice->Acquire();
		return ;
	}
	POINT Pos;
	GetCursorPos(&Pos);
	ScreenToClient(m_phWnd,&Pos);	//相対座標を絶対座標に変換

	MouseStatus.lX = Pos.x;
	MouseStatus.lY = Pos.y;
	if(CurrentMouseStatus.lZ - MouseStatus.lZ == 0)
	{
		WheelStatus = WHEEL_NEUTRAL;
	}
	else if(CurrentMouseStatus.lZ - MouseStatus.lZ > 0)
	{
		WheelStatus = WHEEL_UP;
	}
	else
	{
		WheelStatus = WHEEL_DOWN;
	}
	//MouseStatus.lZ = CurrentMouseStatus.lZ;
	//マウスポインタがウィンドウの外にある時は動かない
	if(MouseStatus.lX<0)
	{
		MouseStatus.lX = 0;
	}
	else if(MouseStatus.lX>SCREEN_WIDTH)
	{
		MouseStatus.lX = (LONG)SCREEN_WIDTH;
	}
	if(MouseStatus.lY<0)
	{
		MouseStatus.lY = 0;
	}
	else if(MouseStatus.lY>SCREEN_HEIGHT)
	{
		MouseStatus.lY = (LONG)SCREEN_HEIGHT;
	}
	
	//キー入力判定
	for(int cnt=0;cnt<3;cnt++)
	{
		TriggerMouse[cnt] = (MouseStatus.rgbButtons[cnt]^CurrentMouseStatus.rgbButtons[cnt])&CurrentMouseStatus.rgbButtons[cnt];
		ReleaseMouse[cnt] = (MouseStatus.rgbButtons[cnt]^CurrentMouseStatus.rgbButtons[cnt])&~CurrentMouseStatus.rgbButtons[cnt];
		MouseStatus.rgbButtons[cnt] = CurrentMouseStatus.rgbButtons[cnt];
	}
}

//=============================================================================
// マウスのX座標取得
//=============================================================================
float CMouse::GetPosX(void)
{
	return (float)MouseStatus.lX;
}
//=============================================================================
// マウスのY座標取得
//=============================================================================
float CMouse::GetPosY(void)
{
	return (float)MouseStatus.lY;
}
//=============================================================================
// マウスのウィールを取得
//=============================================================================
int CMouse::GetWheel(void)
{
	return WheelStatus;
}
//=============================================================================
// マウスのプレス取得
//=============================================================================
bool CMouse::GetPress(int nKey)
{
	return MouseStatus.rgbButtons[nKey] ? true:false;
}
//=============================================================================
// マウスのトリガー取得
//=============================================================================
bool CMouse::GetTrigger(int nKey)
{
	return TriggerMouse[nKey] ? true:false;
}
//=============================================================================
// マウスのリリース取得
//=============================================================================
bool CMouse::GetRelease(int nKey)
{
	return ReleaseMouse[nKey] ? true:false;
}