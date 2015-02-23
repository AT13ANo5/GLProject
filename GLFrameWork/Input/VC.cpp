//=============================================================================
//仮想コントローラークラス
//2つ以上存在してはいけないのでシングルトン化
//=============================================================================
//=============================================================================
//インクルード
//=============================================================================
#include "VC.h"
#include "GamePad.h"
#include "Keyboard.h"

//=============================================================================
//静的メンバ変数定義
//=============================================================================
VC* VC::Self = new VC;//自己ポインタ

//=============================================================================
//定数定義
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
};//キーボードのコマンドリスト

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
};//ゲームパッドのコマンドリスト

//=============================================================================
//コンストラクタ
//=============================================================================
VC::VC()
{
	_GamePad = nullptr;
	_Keyboard = nullptr;
	_Mouse = nullptr;
	DInput = nullptr;
}

//=============================================================================
//初期化
//=============================================================================
HRESULT VC::Init(HWND hWnd)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE);

	//ゲームパッドを初期化
	_GamePad = CGamePad::Instance();
	_GamePad->Init();

	HRESULT hr;
	if (DInput == NULL)
	{
		hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&DInput,NULL);

		if (FAILED(hr))
		{
			MessageBox(NULL,"インプットオブジェクトの作成に失敗しました","ERROR!",(MB_ICONERROR | MB_OK));
			return hr;
		}
	}

	
	//キーボードを初期化
	_Keyboard = CKeyboard::Instance();
	if (FAILED(_Keyboard->Init(DInput,hInstance,hWnd)))
	{
		MessageBox(nullptr,"キーボードの初期化に失敗しました","ERROR!",MB_OK|MB_ICONERROR);
		return E_FAIL;
	}


	return S_OK;

}

//=============================================================================
//終了処理
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
//更新
//=============================================================================
void VC::Update(void)
{
	//それぞれを更新
	if (_GamePad != nullptr)
	{
		_GamePad->Update();
	}
	if (_Keyboard != nullptr)
	{
		_Keyboard->Update();
	}

	//コマンドリストを更新
	COMMAND CurrentCommand;
	for (int cnt = 0;cnt < COMMAND_MAX;cnt++)
	{
		for (int num = 0;num < 3;num++)
		{
			//どっちかがTrueならTrue
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

	//スティックはゲームパッドを優先する
	CurrentCommand.LeftStick.x = _GamePad->StickX(LEFT);
	CurrentCommand.LeftStick.y = _GamePad->StickY(LEFT);
	CurrentCommand.RightStick.x = _GamePad->StickX(RIGHT);
	CurrentCommand.RightStick.y = _GamePad->StickY(RIGHT);
	CurrentCommand.SholderL = _GamePad->LRTrigger(LEFT);
	CurrentCommand.SholderR = _GamePad->LRTrigger(RIGHT);

	//スティックが倒れていなければキーボードを優先する

	//左スティック
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

	//右スティック
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

	//トリガー
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
//振動設定(両方)
//引数：
//	left … 左モーターの振動の強さ(0～1)
//	frameL … 左モーターを何フレーム振動させるか
//	right … 右モーターの振動の強さ(0～1)
//	frameR … 右モーターを何フレーム振動させるか
//=============================================================================
void VC::SetVibration(float left,int frameL,float right,int frameR)
{
	_GamePad->SetVibration(left,frameL,right,frameR);
}

//=============================================================================
//左モーターの振動設定
//=============================================================================
void VC::SetVibrationL(float left,int frameL)
{
	_GamePad->SetVibrationL(left,frameL);
}

//=============================================================================
//右モーターの振動設定
//=============================================================================
void VC::SetVibrationR(float right,int frameR)
{
	_GamePad->SetVibrationR(right,frameR);
}

//=============================================================================
//ゲームパッド取得
//=============================================================================
CGamePad* VC::GamePad(void)
{
	return _GamePad;
}

//=============================================================================
//キーボード取得
//=============================================================================
CKeyboard* VC::Keyboard(void)
{
	return _Keyboard;
}

//=============================================================================
//マウス取得
//=============================================================================
CMouse* VC::Mouse(void)
{
	return _Mouse;
}