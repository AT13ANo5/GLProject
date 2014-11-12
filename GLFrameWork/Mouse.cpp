#include "Mouse.h"

HWND CMouse::m_phWnd;
POINT CMouse::pos;

DIMOUSESTATE2 CMouse::MouseStatus;
BYTE CMouse::TriggerMouse[3];
BYTE CMouse::ReleaseMouse[3];
int CMouse::WheelStatus;
//=============================================================================
//������
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance,HWND hWnd)
{
	DIPROPDWORD dipdw;
	if(FAILED(CInput::Init(hInstance,hWnd)))
	{
		return E_FAIL;
	}
	// �uDirectInput�f�o�C�X�v�I�u�W�F�N�g�̍쐬
	if( FAILED( DInput->CreateDevice( GUID_SysMouse,&pInputDevice, NULL ) ) )
	{
		return E_FAIL;
	} 
	
	// �f�o�C�X���}�E�X�ɐݒ�
	if( FAILED( pInputDevice->SetDataFormat( &c_dfDIMouse2 ) ) )
	{
		return E_FAIL;
	}
	// �������x���̐ݒ�
	if( FAILED( pInputDevice->SetCooperativeLevel( 
		hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND )) )
	{
		return E_FAIL;
	}

	if(FAILED(pInputDevice->SetProperty(DIPROP_AXISMODE,&dipdw.diph)))
	{
		return E_FAIL;
	}


	// �f�o�C�X���u�擾�v����
	if(FAILED(pInputDevice->Acquire()))
	{
		return E_FAIL;
	}
	m_phWnd = hWnd;
	WheelStatus = WHEEL_NEUTRAL;
	return S_OK;
}
//=============================================================================
//�X�V
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
	ScreenToClient(m_phWnd,&Pos);	//���΍��W���΍��W�ɕϊ�

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
	//�}�E�X�|�C���^���E�B���h�E�̊O�ɂ��鎞�͓����Ȃ�
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
	
	//�L�[���͔���
	for(int cnt=0;cnt<3;cnt++)
	{
		TriggerMouse[cnt] = (MouseStatus.rgbButtons[cnt]^CurrentMouseStatus.rgbButtons[cnt])&CurrentMouseStatus.rgbButtons[cnt];
		ReleaseMouse[cnt] = (MouseStatus.rgbButtons[cnt]^CurrentMouseStatus.rgbButtons[cnt])&~CurrentMouseStatus.rgbButtons[cnt];
		MouseStatus.rgbButtons[cnt] = CurrentMouseStatus.rgbButtons[cnt];
	}
}

//=============================================================================
// �}�E�X��X���W�擾
//=============================================================================
float CMouse::GetPosX(void)
{
	return (float)MouseStatus.lX;
}
//=============================================================================
// �}�E�X��Y���W�擾
//=============================================================================
float CMouse::GetPosY(void)
{
	return (float)MouseStatus.lY;
}
//=============================================================================
// �}�E�X�̃E�B�[�����擾
//=============================================================================
int CMouse::GetWheel(void)
{
	return WheelStatus;
}
//=============================================================================
// �}�E�X�̃v���X�擾
//=============================================================================
bool CMouse::GetPress(int nKey)
{
	return MouseStatus.rgbButtons[nKey] ? true:false;
}
//=============================================================================
// �}�E�X�̃g���K�[�擾
//=============================================================================
bool CMouse::GetTrigger(int nKey)
{
	return TriggerMouse[nKey] ? true:false;
}
//=============================================================================
// �}�E�X�̃����[�X�擾
//=============================================================================
bool CMouse::GetRelease(int nKey)
{
	return ReleaseMouse[nKey] ? true:false;
}