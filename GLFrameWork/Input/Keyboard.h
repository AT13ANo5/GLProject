//=============================================================================
// �L�[�{�[�h�N���X
// CreateBy Arai Yuhki
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
//=============================================================================
// �C���N���[�h
//=============================================================================
#include"VC.h"
#include <Windows.h>
//=============================================================================
// �N���X��`
//=============================================================================
class CKeyboard
{
public:
	
	~CKeyboard();
	HRESULT Init(LPDIRECTINPUT8 DInput,HINSTANCE hInstance,HWND hWnd);
	static void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

	static CKeyboard* Instance(void){ return Self; }

private:
	CKeyboard(){}
	CKeyboard(const CKeyboard& other){};
	CKeyboard &operator=(const CKeyboard& other){};

	static CKeyboard* Self;
	BYTE KeyState[256];		//�L�[���͏��
	BYTE TriggerKeyState[256];	//�g���K�[�L�[���͏��
	BYTE ReleaseKeyState[256];	//�����[�X�L�[���͏��
	BYTE RepeateKeyState[256];	//���s�[�g�L�[���͏��
	short RepeateKeyFlame[256];	//���s�[�g�L�[�p�t���[���J�E���g

	LPDIRECTINPUTDEVICE8 InputDevice;

};

#endif