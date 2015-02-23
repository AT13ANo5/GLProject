//=============================================================================
// �L�[�{�[�h�N���X
// Createby Arai Yuhki
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "Keyboard.h"
//=============================================================================
// �}�N����`
//=============================================================================
#define REPEATE_TRIGGER (20)

CKeyboard* CKeyboard::Self = new CKeyboard;

CKeyboard::~CKeyboard()
{
	if (InputDevice != nullptr)
	{
		InputDevice->Release();
		InputDevice = nullptr;
	}
}

//=============================================================================
// ������
//=============================================================================
HRESULT CKeyboard::Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd)
{

	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = DInput->CreateDevice(GUID_SysKeyboard,&InputDevice,NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,"�f�o�C�X�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���","ERROR!",(MB_ICONERROR | MB_OK));
		return hr;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = InputDevice->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = InputDevice->SetCooperativeLevel(hWnd,(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	if(FAILED(hr))
	{
		MessageBox(NULL,"�������[�h�̐ݒ�Ɏ��s���܂���","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	InputDevice->Acquire();
	return S_OK;
}

void CKeyboard::Uninit(void)
{
	delete Self;
	Self = nullptr;
}

//=============================================================================
// �X�V
//=============================================================================
void CKeyboard::Update(void)
{
	BYTE CurrentKeyState[256] ={0};
	if(SUCCEEDED(InputDevice->GetDeviceState(sizeof(CurrentKeyState),&CurrentKeyState[0])))
	{
		for(int nKey=0;nKey<256;nKey++)
		{
			TriggerKeyState[nKey] = (KeyState[nKey]^CurrentKeyState[nKey]) & CurrentKeyState[nKey];	//�g���K�[�擾
			ReleaseKeyState[nKey] = (KeyState[nKey]^CurrentKeyState[nKey]) & KeyState[nKey];//�����[�X�擾

			//���s�[�g�擾
			if((KeyState[nKey]&CurrentKeyState[nKey]))
			{
				RepeateKeyFlame[nKey]++;//������Ă���Ԃ̓J�E���g�A�b�v
			}
			else
			{
				RepeateKeyFlame[nKey] = 0;	//�����ꂽ��0�ɖ߂�
			}

			if(RepeateKeyFlame[nKey]>REPEATE_TRIGGER)
			{
				RepeateKeyState[nKey] = CurrentKeyState[nKey];	//�w��t���[���������ꑱ������z�[���h
			}
			else
			{
				RepeateKeyState[nKey] = TriggerKeyState[nKey];	//�w�肳�ꂽ�t���[�����܂Ńg���K�[
			}
			KeyState[nKey] = CurrentKeyState[nKey];
		}
	}
	else
	{
		InputDevice->Acquire();
	}

}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool CKeyboard::GetPress(int nKey)
{
	return KeyState[nKey] ? true:false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool CKeyboard::GetTrigger(int nKey)
{
	return TriggerKeyState[nKey] ? true:false;
}
//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool CKeyboard::GetRelease(int nKey)
{
	return ReleaseKeyState[nKey] ? true:false;
}
//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool CKeyboard::GetRepeat(int nKey)
{
	return RepeateKeyState[nKey] ? true:false;
}

