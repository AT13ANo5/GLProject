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

	static float GetPosX(void);	//X���W�擾
	static float GetPosY(void);	//Y���W�擾
	static int GetWheel(void);	//�E�B�[�����擾
	static bool GetPress(int nKey);	//�v���X���
	static bool GetTrigger(int nKey);	//�g���K�[���
	static bool GetRelease(int nKey);	//�����[�X���
	static void SetHWND(HWND hWnd){m_phWnd = hWnd;}
private:
	static DIMOUSESTATE2 MouseStatus;				//�}�E�X�̃X�e�[�^�X�ۑ��p
	static BYTE TriggerMouse[3];							//�}�E�X�{�^���̃g���K�[���
	static BYTE ReleaseMouse[3];							//�}�E�X�{�^���̃����[�X���
	static int WheelStatus;								//�E�B�[���̏��
	static HWND m_phWnd;
	static POINT pos;
};

#endif