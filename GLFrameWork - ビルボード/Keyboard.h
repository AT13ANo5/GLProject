//=============================================================================
// �L�[�{�[�h�N���X
// CreateBy Arai Yuhki
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "CInput.h"
//=============================================================================
// �N���X��`
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
	static BYTE g_aKeyState[256];		//�L�[���͏��
	static BYTE TriggerKeyState[256];	//�g���K�[�L�[���͏��
	static BYTE ReleaseKeyState[256];	//�����[�X�L�[���͏��
	static BYTE RepeateKeyState[256];	//���s�[�g�L�[���͏��
	static short RepeateKeyFlame[256];	//���s�[�g�L�[�p�t���[���J�E���g
	//static LPDIRECTINPUTDEVICE8 pInputDevice;

};

#endif