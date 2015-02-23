//=============================================================================
//���z�R���g���[���[�N���X
//2�ȏ㑶�݂��Ă͂����Ȃ��̂ŃV���O���g����
//=============================================================================
//=============================================================================
//�C���N���[�h
//=============================================================================
#include "VC.h"
#include "GamePad.h"
#include "Keyboard.h"

//=============================================================================
//�ÓI�����o�ϐ���`
//=============================================================================
VC* VC::Self = new VC;//���ȃ|�C���^

//=============================================================================
//�萔��`
//=============================================================================
static UINT Key[COMMAND_MAX][3] =
{
	{DIK_Q,DIK_RETURN,DIK_SPACE},
	{DIK_E,DIK_X,-1},
	{DIK_RETURN,DIK_P,-1},
	{DIK_BACKSPACE,DIK_V,-1},
	{DIK_LEFT,-1,-1},
	{DIK_RIGHT,-1,-1},
	{DIK_UP,-1,-1},
	{DIK_DOWN,-1,-1},
	{DIK_SPACE,-1,-1},
	{DIK_P,-1,-1},
};//�L�[�{�[�h�̃R�}���h���X�g

static UINT Pad[COMMAND_MAX][3] = 
{
	{ GAMEPAD_A,GAMEPAD_START,-1 },
	{ GAMEPAD_B,-1,-1 },
	{ GAMEPAD_START,-1,-1 },
	{ GAMEPAD_BACK,-1,-1 },
	{ GAMEPAD_DPAD_LEFT,-1,-1 },
	{ GAMEPAD_DPAD_RIGHT,-1,-1 },
	{ GAMEPAD_DPAD_UP,-1,-1 },
	{ GAMEPAD_DPAD_DOWN,-1,-1 },
	{ GAMEPAD_RIGHT_SHOULDER,-1,-1 },
	{ GAMEPAD_LEFT_SHOULDER,-1,-1 },
};//�Q�[���p�b�h�̃R�}���h���X�g

//=============================================================================
//�R���X�g���N�^
//=============================================================================
VC::VC()
{
	_GamePad = nullptr;
	_Keyboard = nullptr;
	_Mouse = nullptr;
	DInput = nullptr;
}

//=============================================================================
//������
//=============================================================================
HRESULT VC::Init(HWND hWnd)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);

	//�Q�[���p�b�h��������
	_GamePad = CGamePad::Instance();
	_GamePad->Init();

	HRESULT hr;
	if (DInput == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&DInput,NULL);

		if (FAILED(hr))
		{
			MessageBox(NULL,"�C���v�b�g�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���","ERROR!",(MB_ICONERROR | MB_OK));
			return hr;
		}
	}

	
	//�L�[�{�[�h��������
	_Keyboard = CKeyboard::Instance();
	if (FAILED(_Keyboard->Init(DInput,hInstance,hWnd)))
	{
		MessageBox(nullptr,"�L�[�{�[�h�̏������Ɏ��s���܂���","ERROR!",MB_OK|MB_ICONERROR);
		return E_FAIL;
	}


	return S_OK;

}

//=============================================================================
//�I������
//=============================================================================
void VC::Uninit(void)
{
	if (_GamePad != nullptr)
	{
		_GamePad->Uninit();
		_GamePad = nullptr;
	}

	if (_Keyboard != nullptr)
	{
		_Keyboard->Uninit();
		_Keyboard = nullptr;
	}

	if (DInput != nullptr)
	{
		DInput->Release();
		DInput = nullptr;
	}

	delete this;
}

//=============================================================================
//�X�V
//=============================================================================
void VC::Update(void)
{
	//���ꂼ����X�V
	if (_GamePad != nullptr)
	{
		_GamePad->Update();
	}
	if (_Keyboard != nullptr)
	{
		_Keyboard->Update();
	}

	//�R�}���h���X�g���X�V
	COMMAND CurrentCommand;
	for (int cnt = 0;cnt < COMMAND_MAX;cnt++)
	{
		for (int num = 0;num < 3;num++)
		{
			//�ǂ�������True�Ȃ�True
			if (Key[cnt][num] != -1)
			{
				CurrentCommand.Press[cnt] |= _Keyboard->GetPress(Key[cnt][num]);
				CurrentCommand.Trigger[cnt] |= _Keyboard->GetTrigger(Key[cnt][num]);
				CurrentCommand.Release[cnt] |= _Keyboard->GetRelease(Key[cnt][num]);
				CurrentCommand.Repeat[cnt] |= _Keyboard->GetRepeat(Key[cnt][num]);
			}

			if (Pad[cnt][num] != -1)
			{
				CurrentCommand.Press[cnt]	|= _GamePad->Press(Pad[cnt][num]);
				CurrentCommand.Trigger[cnt] |= _GamePad->Trigger(Pad[cnt][num]);
				CurrentCommand.Release[cnt] |= _GamePad->Release(Pad[cnt][num]);
				CurrentCommand.Repeat[cnt]	|= _GamePad->Repeate(Pad[cnt][num]);
			}
		}
	}

	//�X�e�B�b�N�̓Q�[���p�b�h��D�悷��
	CurrentCommand.LeftStick.x = _GamePad->StickX(LEFT);
	CurrentCommand.LeftStick.y = _GamePad->StickY(LEFT);
	CurrentCommand.RightStick.x = _GamePad->StickX(RIGHT);
	CurrentCommand.RightStick.y = _GamePad->StickY(RIGHT);
	CurrentCommand.SholderL = _GamePad->LRTrigger(LEFT);
	CurrentCommand.SholderR = _GamePad->LRTrigger(RIGHT);

	//�X�e�B�b�N���|��Ă��Ȃ���΃L�[�{�[�h��D�悷��

	//���X�e�B�b�N
	if (CurrentCommand.LeftStick.x == 0 && CurrentCommand.LeftStick.y == 0)
	{
		if (_Keyboard->GetPress(DIK_A))
		{
			CurrentCommand.LeftStick.x = -1.0f;
		}
		else if (_Keyboard->GetPress(DIK_D))
		{
			CurrentCommand.LeftStick.x = 1.0f;
		}
		if (_Keyboard->GetPress(DIK_W))
		{
			CurrentCommand.LeftStick.y = 1.0f;
		}
		else if (_Keyboard->GetPress(DIK_S))
		{
			CurrentCommand.LeftStick.y = -1.0f;
		}
	}

	//�E�X�e�B�b�N
	if (CurrentCommand.RightStick.x == 0 && CurrentCommand.RightStick.y == 0)
	{
		if (_Keyboard->GetPress(DIK_LEFT))
		{
			CurrentCommand.RightStick.x = -1.0f;
		}
		else if (_Keyboard->GetPress(DIK_RIGHT))
		{
			CurrentCommand.RightStick.x = 1.0f;
		}
		if (_Keyboard->GetPress(DIK_UP))
		{
			CurrentCommand.RightStick.y = 1.0f;
		}
		else if (_Keyboard->GetPress(DIK_DOWN))
		{
			CurrentCommand.RightStick.y = -1.0f;
		}
	}

	//�g���K�[
	if (CurrentCommand.SholderL == 0)
	{
		if (_Keyboard->GetPress(DIK_F))
		{
			CurrentCommand.SholderL = 1.0f;
		}
	}
	if (CurrentCommand.SholderR == 0)
	{
		if (_Keyboard->GetPress(DIK_R))
		{
			CurrentCommand.SholderR = 1.0f;
		}
	}

	Command = CurrentCommand;
}
//=============================================================================
//�U���ݒ�(����)
//�����F
//	left �c �����[�^�[�̐U���̋���(0�`1)
//	frameL �c �����[�^�[�����t���[���U�������邩
//	right �c �E���[�^�[�̐U���̋���(0�`1)
//	frameR �c �E���[�^�[�����t���[���U�������邩
//=============================================================================
void VC::SetVibration(float left,int frameL,float right,int frameR)
{
	_GamePad->SetVibration(left,frameL,right,frameR);
}

//=============================================================================
//�����[�^�[�̐U���ݒ�
//=============================================================================
void VC::SetVibrationL(float left,int frameL)
{
	_GamePad->SetVibrationL(left,frameL);
}

//=============================================================================
//�E���[�^�[�̐U���ݒ�
//=============================================================================
void VC::SetVibrationR(float right,int frameR)
{
	_GamePad->SetVibrationR(right,frameR);
}

//=============================================================================
//�Q�[���p�b�h�擾
//=============================================================================
CGamePad* VC::GamePad(void)
{
	return _GamePad;
}

//=============================================================================
//�L�[�{�[�h�擾
//=============================================================================
CKeyboard* VC::Keyboard(void)
{
	return _Keyboard;
}

//=============================================================================
//�}�E�X�擾
//=============================================================================
CMouse* VC::Mouse(void)
{
	return _Mouse;
}