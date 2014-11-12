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

BYTE CKeyboard::g_aKeyState[256];		
BYTE CKeyboard::TriggerKeyState[256];
BYTE CKeyboard::ReleaseKeyState[256];
BYTE CKeyboard::RepeateKeyState[256];
short CKeyboard::RepeateKeyFlame[256];

//=============================================================================
// ������
//=============================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;
	if(FAILED(CInput::Init(hInstance,hWnd)))
	{
		return E_FAIL;
	}
	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = DInput -> CreateDevice(GUID_SysKeyboard,&pInputDevice,NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�o�C�X�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}
	

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = pInputDevice->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(hr))
	{
		MessageBox(NULL,"�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pInputDevice->SetCooperativeLevel(hWnd,(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	if(FAILED(hr))
	{
		MessageBox(NULL,"�������[�h�̐ݒ�Ɏ��s���܂���","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	pInputDevice->Acquire();
	return S_OK;
}
//=============================================================================
// �X�V
//=============================================================================
void CKeyboard::Update(void)
{
	BYTE aKeyState[256] ={0};
	if(SUCCEEDED(pInputDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for(int nKey=0;nKey<256;nKey++)
		{
			TriggerKeyState[nKey] = (g_aKeyState[nKey]^aKeyState[nKey]) & aKeyState[nKey];	//�g���K�[�擾
			ReleaseKeyState[nKey] = (g_aKeyState[nKey]^aKeyState[nKey]) & g_aKeyState[nKey];//�����[�X�擾

			//���s�[�g�擾
			if((g_aKeyState[nKey]&aKeyState[nKey]))
			{
				RepeateKeyFlame[nKey]++;//������Ă���Ԃ̓J�E���g�A�b�v
			}
			else
			{
				RepeateKeyFlame[nKey] = 0;	//�����ꂽ��0�ɖ߂�
			}

			if(RepeateKeyFlame[nKey]>REPEATE_TRIGGER)
			{
				RepeateKeyState[nKey] = aKeyState[nKey];	//�w��t���[���������ꑱ������z�[���h
			}
			else
			{
				RepeateKeyState[nKey] = TriggerKeyState[nKey];	//�w�肳�ꂽ�t���[�����܂Ńg���K�[
			}
			g_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		pInputDevice->Acquire();
	}

}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool CKeyboard::GetPress(int nKey)
{
	return g_aKeyState[nKey] ? true:false;
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

