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
		rot.x -= 3.0f;
	}
	else if(CKeyboard::GetPress(DIK_DOWN))
	{
		rot.x += 3.0f;
	}

	// キャラクターの回転
	AddRot(rot);

	// 値の丸め込み
	if( Rot().y > 360.0f)
	{
		SetRotY(Rot().y - 2 * 360.0f);
	}
	else if (Rot().y < -360.0f)
	{
		SetRotY(Rot().y + 2 * 360.0f);
	}

	// キャラクターの移動
	AddPos(Movement);

	// 減速
	Movement *= 0.95f;
 Barrel->SetPos(_Pos);

	// 攻撃
	//if(CKeyboard::GetTrigger(DIK_SPACE))
	//{
	//	if(Bullet == nullptr)
	//	{
	//		//LaunchFlag = true;
	//		//Bullet = CBullet::Create(_Pos, VECTOR2(40.0f, 40.0f), VECTOR3(0.0f, 0.0f, 0.0f), WHITE(0.5f));
	//		Bullet = CBullet::Create(_Pos, VECTOR2(BULLET_SIZE, BULLET_SIZE), _Rot, WHITE(0.5f));
	//	}
	//}

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
	Console::SetCursorPos(1,1);
	Console::Print("Rot.x:%f\nRot.y:%f\n",_Rot.x,_Rot.y);
#endif

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
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