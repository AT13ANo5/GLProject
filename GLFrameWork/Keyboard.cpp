//=============================================================================
// キーボードクラス
// Createby Arai Yuhki
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "Keyboard.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define REPEATE_TRIGGER (20)

BYTE CKeyboard::g_aKeyState[256];		
BYTE CKeyboard::TriggerKeyState[256];
BYTE CKeyboard::ReleaseKeyState[256];
BYTE CKeyboard::RepeateKeyState[256];
short CKeyboard::RepeateKeyFlame[256];

//=============================================================================
// 初期化
//=============================================================================
HRESULT CKeyboard::Init(HINSTANCE hInstance,HWND hWnd)
{
	HRESULT hr;
	if(FAILED(CInput::Init(hInstance,hWnd)))
	{
		return E_FAIL;
	}
	// デバイスオブジェクトを作成
	hr = DInput -> CreateDevice(GUID_SysKeyboard,&pInputDevice,NULL);
	if(FAILED(hr))
	{
		MessageBox(NULL,"デバイスオブジェクトの作成に失敗しました","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}
	

	// データフォーマットを設定
	hr = pInputDevice->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(hr))
	{
		MessageBox(NULL,"データフォーマットの設定に失敗しました","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = pInputDevice->SetCooperativeLevel(hWnd,(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	if(FAILED(hr))
	{
		MessageBox(NULL,"強調モードの設定に失敗しました","ERROR!",(MB_ICONERROR|MB_OK));
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	pInputDevice->Acquire();
	return S_OK;
}
//=============================================================================
// 更新
//=============================================================================
void CKeyboard::Update(void)
{
	BYTE aKeyState[256] ={0};
	if(SUCCEEDED(pInputDevice->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for(int nKey=0;nKey<256;nKey++)
		{
			TriggerKeyState[nKey] = (g_aKeyState[nKey]^aKeyState[nKey]) & aKeyState[nKey];	//トリガー取得
			ReleaseKeyState[nKey] = (g_aKeyState[nKey]^aKeyState[nKey]) & g_aKeyState[nKey];//リリース取得

			//リピート取得
			if((g_aKeyState[nKey]&aKeyState[nKey]))
			{
				RepeateKeyFlame[nKey]++;//押されている間はカウントアップ
			}
			else
			{
				RepeateKeyFlame[nKey] = 0;	//離されたら0に戻す
			}

			if(RepeateKeyFlame[nKey]>REPEATE_TRIGGER)
			{
				RepeateKeyState[nKey] = aKeyState[nKey];	//指定フレーム数押され続けたらホールド
			}
			else
			{
				RepeateKeyState[nKey] = TriggerKeyState[nKey];	//指定されたフレーム数までトリガー
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
// キーボードのプレス状態を取得
//=============================================================================
bool CKeyboard::GetPress(int nKey)
{
	return g_aKeyState[nKey] ? true:false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool CKeyboard::GetTrigger(int nKey)
{
	return TriggerKeyState[nKey] ? true:false;
}
//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool CKeyboard::GetRelease(int nKey)
{
	return ReleaseKeyState[nKey] ? true:false;
}
//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool CKeyboard::GetRepeat(int nKey)
{
	return RepeateKeyState[nKey] ? true:false;
}

