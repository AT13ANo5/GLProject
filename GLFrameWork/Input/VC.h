//=============================================================================
//仮想コントローラークラス
//=============================================================================
#ifndef _VC_H_
#define _VC_H_
//=============================================================================
//インクルード＆ライブラリリンク
//=============================================================================
#include "../main.h"
//=============================================================================
//前方宣言
//=============================================================================
class CGamePad;
class CKeyboard;
class CMouse;

//=============================================================================
//列挙
//=============================================================================
typedef enum
{
	COMMAND_OK = 0,
	COMMAND_CANCEL,
	COMMAND_START,
	COMMAND_SELECT,
	COMMAND_LEFT,
	COMMAND_RIGHT,
	COMMAND_UP,
	COMMAND_DOWN,
	COMMAND_SHOT,
	COMMAND_SCORE,
	COMMAND_MAX

}COMMAND_ID;//コマンドリスト

//クラス定義
class VC
{
public:

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	CGamePad* GamePad(void);
	CKeyboard* Keyboard(void);
	CMouse* Mouse(void);

	bool Press(COMMAND_ID id){ return Command.Press[id]; }
	bool Trigger(COMMAND_ID id){ return Command.Trigger[id]; }
	bool Release(COMMAND_ID id){ return Command.Release[id]; }
	bool Repeat(COMMAND_ID id){ return Command.Repeat[id]; }
	VECTOR2 LeftStick(void){ return Command.LeftStick; }
	VECTOR2 RightStick(void){ return Command.RightStick; }
	float SholderL(void){ return Command.SholderL; }
	float SholderR(void){ return Command.SholderR; }

	void SetVibration(float left,int frameL,float right,int frameR);
	void SetVibrationL(float left,int frameL);
	void SetVibrationR(float right,int frameR);

	static VC* Instance(void){return Self;}
private:
	VC();
	VC(const VC& other){};
	VC &operator=(const VC& other){};

	static VC* Self;

	typedef struct COMMAND
	{
		COMMAND()
		{
			for (int cnt = 0;cnt < COMMAND_MAX;cnt++)
			{
				Press[cnt] = Trigger[cnt] = Release[cnt] = Repeat[cnt] = false;
			}
			SholderL = SholderR = 0;
			LeftStick = RightStick = VECTOR2(0,0);
		}
		bool Press[COMMAND_MAX];
		bool Trigger[COMMAND_MAX];
		bool Release[COMMAND_MAX];
		bool Repeat[COMMAND_MAX];

		VECTOR2 LeftStick;
		VECTOR2 RightStick;
		float SholderL;
		float SholderR;

	}COMMAND;
	
	COMMAND Command;

	CGamePad	*_GamePad;
	CKeyboard	*_Keyboard;
	CMouse		*_Mouse;
	LPDIRECTINPUT8 DInput;
};

#endif