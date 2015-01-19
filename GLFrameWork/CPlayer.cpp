//------------------------------------------------------------------------------
//
// プレイヤー [CPlayer.cpp]
// Author : AT-13A-273 Shinnosuke Munakata
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// ヘッダインクルード
//------------------------------------------------------------------------------
#include <math.h>

#include "main.h"
#include "CPlayer.h"
#include "Keyboard.h"
#include "Camera.h"
#include "CBullet.h"
#include "CommonGL.h"

//------------------------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------------------------
// 引数
//  なし
//------------------------------------------------------------------------------
CPlayer::CPlayer():CModel()
{
	// 移動速度
	Speed = PLAYER_MOVE_SPEED;

	// 弾初期化
	Bullet = nullptr;

	// 装填時間
	_ReloadTimer = PLAYER_RELOAD_TIME;

	// 砲身のX軸回転量
	BarrelRotX = 0.0f;
}

//------------------------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------------------------
// 引数
//  なし
//------------------------------------------------------------------------------
CPlayer::~CPlayer()
{

}

//------------------------------------------------------------------------------
// 初期化
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	// 弾
	Bullet = nullptr;

	// 継承元の初期化
	CModel::Init();

	// 砲身
	Barrel = CModel::Create(TANK_BARREL,_Pos);
	Barrel->Init();
	Barrel->SetTex(CTexture::Texture(TEX_YOUJO_RED));
}

//------------------------------------------------------------------------------
// 更新
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CPlayer::Update()
{
	VECTOR3 rot = VECTOR3(0.0f, 0.0f, 0.0f);

	// 移動
	// 上
	if(CKeyboard::GetPress(DIK_W))
	{
		Movement.x += sinf(DEG2RAD(_Rot.y)) * Speed;
		Movement.z += cosf(DEG2RAD(_Rot.y)) * Speed;
	}

	// 下
	else if (CKeyboard::GetPress(DIK_S))
	{
		Movement.x -= sinf(DEG2RAD(_Rot.y)) * Speed;
		Movement.z -= cosf(DEG2RAD(_Rot.y)) * Speed;
	}

	// 左
	if(CKeyboard::GetPress(DIK_A))
	{
		rot.y += 3.0f;
	}

	// 右
	else if(CKeyboard::GetPress(DIK_D))
	{
		rot.y -= 3.0f;
	}

	if(CKeyboard::GetPress(DIK_UP))
	{
		BarrelRotX -= 3.0f;
	}
	else if(CKeyboard::GetPress(DIK_DOWN))
	{
		BarrelRotX += 3.0f;
	}

	// キャラクターの回転
	AddRot(rot);

	// 値の丸め込み
	// プレイヤーの回転量
	if(Rot().y > 360.0f)
	{
		SetRotY(Rot().y - 2 * 360.0f);
	}
	else if(Rot().y < -360.0f)
	{
		SetRotY(Rot().y + 2 * 360.0f);
	}

	// 砲身
	if( BarrelRotX > 0.0f)
	{
		BarrelRotX = 0.0f;
	}
	else if(BarrelRotX < -45.0f)
	{
		BarrelRotX = -45.0f;
	}
	
	// キャラクターの移動値を加算
	AddPos(Movement);

	// 減速
	Movement *= 0.95f;

	// 砲身の位置、回転を更新
	Barrel->SetPos(_Pos);			// 位置
	Barrel->SetRot(_Rot);			// 回転
	Barrel->AddRotX(BarrelRotX);	// 上で設定した回転量に砲身のX軸回転量を加算

	// 弾の発射
	if(LaunchFlag == false)
	{
		if(CKeyboard::GetTrigger(DIK_SPACE))
		{
			Bullet = CBullet::Create(_Pos, VECTOR2(BULLET_SIZE, BULLET_SIZE), _Rot, WHITE(0.5f));
			LaunchFlag = true;
			_ReloadTimer = 0;
		}
	}
	else
	{
		_ReloadTimer++;

		if(_ReloadTimer >= PLAYER_RELOAD_TIME)
		{
			LaunchFlag = false;
			_ReloadTimer = PLAYER_RELOAD_TIME;
		}
	}

#ifdef _DEBUG
	// デバッグ用
	// 連射
	if(CKeyboard::GetPress(DIK_V))
	{
		Bullet = CBullet::Create(_Pos, VECTOR2(BULLET_SIZE, BULLET_SIZE), VECTOR3(BarrelRotX, _Rot.y, _Rot.z), WHITE(0.5f));
	}
#endif

#ifdef _DEBUG
	Console::SetCursorPos(1,1);
	Console::Print("Rot.x:%f\nRot.y:%f\n",_Rot.x,_Rot.y);
	Console::SetCursorPos(1, 3);
	Console::Print("barrel@rot.x : %f rot.y : %f rot.z : %f", Barrel->Rot().x, Barrel->Rot().y, Barrel->Rot().z);
#endif

}

//------------------------------------------------------------------------------
// 生成
//------------------------------------------------------------------------------
// 引数
//  id : モデルのID
//  pos : 初期位置
// 戻り値
//  CPlayer* : 生成したプレイヤーのポインタ
//------------------------------------------------------------------------------
CPlayer* CPlayer::Create(int id,const VECTOR3& pos)
{
	CPlayer* model = new CPlayer;
	if (model == nullptr){ return nullptr; }

	model->ModelID = id;
	model->_Pos = pos;
	model->Init();

	return model;
}

//------------------------------------------------------------------------------
// EOF