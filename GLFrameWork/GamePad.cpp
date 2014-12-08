//=============================================================================
// ゲームパッドの処理
// CreateBy Arai Yuhki
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "GamePad.h"
//=============================================================================
// 初期化
//=============================================================================
HRESULT CGamePad::Init(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;
	if(FAILED(CInput::Init(hInstance,hWnd)))
	{
		return E_FAIL;
	}

	hr = DInput->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoystickCallback,this,DIEDFL_ATTACHEDONLY);
	

	if(pInputDevice==NULL)
	{
		MessageBox(NULL,"ゲームパッドが接続されていません","INFO!",(MB_ICONINFORMATION|MB_OK));
	}
	else
	{
		hr = pInputDevice->SetDataFormat(&c_dfDIJoystick);
		if(FAILED(hr))
		{
			pInputDevice->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE|DISCL_FOREGROUND);
			return hr;
		}
		pInputDevice->GetCapabilities(&g_diDevCaps);
		pInputDevice->EnumObjects( EnumAxesCallback , this , DIDFT_AXIS );

		hr = pInputDevice->Poll();
		if(FAILED(hr))
		{
			hr = pInputDevice->Acquire();
			while(hr == DIERR_INPUTLOST)
			{
				hr = pInputDevice->Acquire();
			}
		}
	}
	return S_OK;
}
//=============================================================================
// コールバックその一
//=============================================================================
BOOL CALLBACK CGamePad::EnumJoystickCallback(const DIDEVICEINSTANCE *pdidInstance,VOID *pContext)
{
	CGamePad* pThis = reinterpret_cast<CGamePad*>(pContext);
	HRESULT hr;
	hr = DInput->CreateDevice(pdidInstance->guidInstance,&pThis->pInputDevice,NULL);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}
//=============================================================================
// コールバックその二
//=============================================================================
BOOL CALLBACK CGamePad::EnumAxesCallback( const DIDEVICEOBJECTINSTANCE *pdidoi , VOID *pContext )
{
	HRESULT     hr;
	DIPROPRANGE diprg;
	CGamePad* pThis = reinterpret_cast<CGamePad*>(pContext);
	diprg.diph.dwSize		= sizeof( DIPROPRANGE );
	diprg.diph.dwHeaderSize	= sizeof( DIPROPHEADER );
	diprg.diph.dwHow		= DIPH_BYID;
	diprg.diph.dwObj		= pdidoi->dwType;
	diprg.lMin				= 0 - 1000;
	diprg.lMax				= 0 + 1000;
	hr = pThis->pInputDevice->SetProperty( DIPROP_RANGE , &diprg.diph );
	
	if ( FAILED( hr ) )
	{
		return DIENUM_STOP;
	}
	
	return DIENUM_CONTINUE;
}
//=============================================================================
// 更新
//=============================================================================
void CGamePad::Update(void)
{
	if(pInputDevice != NULL)
	{
		DIJOYSTATE CurrentPadState;
		CurrentPadState.lX = 0;
		CurrentPadState.lY = 0;
		CurrentPadState.lZ = 0;

		pInputDevice->Poll();

		PadTriggerState.rgdwPOV[0] = -1;
		PadReleaseState.rgdwPOV[0] = -1;

		if(SUCCEEDED(pInputDevice->GetDeviceState(sizeof(DIJOYSTATE),&CurrentPadState)))
		{

			for(int cnt=0;cnt<32;cnt++)
			{// (g_aKeyState[nKey]^aKeyState[nKey]) & aKeyState[nKey];
				PadTriggerState.rgbButtons[cnt] = (PadState.rgbButtons[cnt]^CurrentPadState.rgbButtons[cnt])&CurrentPadState.rgbButtons[cnt];
				PadReleaseState.rgbButtons[cnt] = (PadState.rgbButtons[cnt]^CurrentPadState.rgbButtons[cnt])&PadState.rgbButtons[cnt];
			}
			for(int cnt=0;cnt<4;cnt++)
			{
				if(PadState.rgdwPOV[0] != CurrentPadState.rgdwPOV[0]&&CurrentPadState.rgdwPOV[0]!=-1)
				{
					PadTriggerState.rgdwPOV[cnt] = CurrentPadState.rgdwPOV[0];
				}
				else
				{
					PadTriggerState.rgdwPOV[cnt] = -1;
				}
				if((PadState.rgdwPOV[0] != CurrentPadState.rgdwPOV[0])&&PadState.rgdwPOV[0]!=-1)
				{
					PadReleaseState.rgdwPOV[cnt] = PadState.rgdwPOV[0];
				}
				else
				{
					PadReleaseState.rgdwPOV[cnt] = -1;
				}
			}
			PadState = CurrentPadState;
		}
		else
		{
			pInputDevice->Acquire();
		}
	}
	else
	{
		for(int cnt=0;cnt<32;cnt++)
		{
			PadState.rgbButtons[cnt] = false;
			PadTriggerState.rgbButtons[cnt] = false;
			PadReleaseState.rgbButtons[cnt] = false;
		}
	}
}

float CGamePad::GetlX(void)
{
	if(pInputDevice!=NULL)
	{
		return (float)PadState.lX;
	}
	else
	{
		return 0;
	}
}
float CGamePad::GetlY(void)
{
	if(pInputDevice!=NULL)
	{
		return (float)PadState.lY;
	}
	else
	{
		return 0;
	}
}
float CGamePad::GetlRX(void)
{
	if(pInputDevice!=NULL)
	{
		return (float)PadState.lRx;
	}
	else
	{
		return 0;
	}
}
float CGamePad::GetlRY(void)
{
	if(pInputDevice!=NULL)
	{
		return (float)PadState.lRy;
	}
	else
	{
		return 0;
	}
}