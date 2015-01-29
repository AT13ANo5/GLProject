//------------------------------------------------------------------------------
//
//  []
// Author : 
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// インクルード
//------------------------------------------------------------------------------
#include <math.h>
#include <float.h>

#include "main.h"
#include "Game.h"
#include "ManagerGL.h"
#include "Keyboard.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "PlayerCamera.h"


#include "UI.h"

// 静的メンバ変数
const float CGame::RADIUS_SKY = 1500.0f;   // 空の半径
CPlayer* Player = nullptr;//プレイヤー
const float	CGame::RADIUS_DEFENSE_CHARACTER = 10.0f;	// キャラクターの防御半径
const float	CGame::HEIGHT_DEFENSE_CHARACTER = 10.0f;	// キャラクターの防御中心高さ
const float	CGame::RADIUS_OFFENSE_BULLET = 10.0f;		// 砲弾の攻撃半径
const float	CGame::HEIGHT_OFFENSE_BULLET = 10.0f;		// 砲弾の攻撃中心高さ
const float	CGame::RADIUS_PUSH_CHARACTER = 10.0f;		// キャラクターの押し戻し半径
const float	CGame::HEIGHT_PUSH_CHARACTER = 10.0f;		// キャラクターの押し戻し中心高さ
const float	CGame::RADIUS_DEFENSE_ROCK = 10.0f;			// 岩の防御半径
const float	CGame::HEIGHT_DEFENSE_ROCK = 10.0f;			// 岩の防御中心高さ
const float	CGame::RADIUS_PUSH_ROCK = 10.0f;			// 岩の押し戻し半径
const float	CGame::HEIGHT_PUSH_ROCK = 10.0f;			// 岩の押し戻し中心高さ

const int	CGame::MAX_ROCK = 100;						// 岩の数

// 定数
namespace{

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
CGame::CGame()
{

	UI = nullptr;
	ppRock_ = nullptr;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
CGame::~CGame()
{

}

//------------------------------------------------------------------------------
// ゲームの初期化
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CGame::Init(void)
{
	//CPolygon3D* polygon = CPolygon3D::Create(VECTOR3(-200.0f,0,0),VECTOR2(250.0f,250.0f),VECTOR3(0,0,90.0f));
	//polygon->SetTex(CTexture::Texture(TEX_LIGHT));
	//polygon->SetColor(GREEN(1.0f));
	CPolygon3D::Create(VECTOR3(0,-100.0f,0),VECTOR2(500.0f,500.0f),VECTOR3(0.0f,0,0));	// 地形生成

	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(100.0f,100.0f),VECTOR2(20.0f,20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// 空生成
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));
#ifdef _DEBUG
	// デバッグワイヤーフレーム
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// プレイヤー生成
	Player = CPlayer::Create(CModel::RINCHAN, VECTOR3(0.0f, 30.0f, 0.0f), 0);
	Player->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));
	Player->SetRot(0.0f,180.0f,0.0f);

	//プレイヤーカメラ生成
	CPlayerCamera::Create(Player,300.0f);

	// UI初期化
	UI = new CUI;
	UI->Init();
  UI->SetPlayer(Player);

	// 岩の生成
	ppRock_ = new CModel*[MAX_ROCK];
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		VECTOR3	positionRock(0.1f * ((rand() % 10000) - 5000), 0.0f, 0.1f * ((rand() % 10000) - 5000));
		ppRock_[cntRock] = CModel::Create(CModel::ROCK, positionRock);
		PushBackObjectByField(ppRock_[cntRock]);
	}

}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	// 岩の破棄
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		SafeRelease(ppRock_[cntRock]);
	}
	SafeDeletes(ppRock_);
	// プレイヤー破棄
	SafeRelease(Player);

	// 空破棄
	if (Sky != nullptr)
	{
		Sky->Release();
		Sky = nullptr;
	}

	// 地形破棄
	if (Ground != nullptr)
	{
		Ground->Release();
		Ground = nullptr;
	}

	// UI破棄
  SafeDelete(UI);

	CCamera::ReleaseAll();
	CObject::ReleaseAll();
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
// 引数
//  なし
// 戻り値
//  なし
//------------------------------------------------------------------------------
void CGame::Update(void)
{
	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	float	HeightGround;		// 地形の高さ
	HeightGround = Ground->GetHeight(Player->Pos(),&NormalGround);

	// 回転を求める
	VECTOR3	VectorUpPlayer;		// 上方向ベクトル
	VECTOR3	VectorNormalYZ;		// YZ平面上の法線ベクトル
	VECTOR3	VectorNormalXY;		// XY平面上の法線ベクトル
	float	AnglePlayerX;		// プレイヤー回転X軸
	float	AnglePlayerZ;		// プレイヤー回転Z軸
	VectorUpPlayer.x = VectorUpPlayer.z = 0.0f;
	VectorUpPlayer.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglePlayerX = -acosf(VECTOR3::Dot(VectorNormalYZ,VectorUpPlayer));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = -acosf(VECTOR3::Dot(VectorNormalXY,VectorUpPlayer));

	// プレイヤー情報のデバッグ表示
	VECTOR3	positionPlayer = Player->Pos();
	VECTOR3	rotaionPlayer = Player->Rot();
	Console::SetCursorPos(1,1);
	Console::Print("Pos : (%9.3f, %9.3f, %9.3f)",positionPlayer.x,HeightGround,positionPlayer.z);
	Console::Print("Rot : (%9.3f, %9.3f, %9.3f)",180.0f / PI * AnglePlayerX,rotaionPlayer.y,180.0f / PI * AnglePlayerZ);

	// プレイヤーに設定する
	Player->SetPosY(HeightGround);
	Player->SetRotX(AnglePlayerX * 180.0f / PI);
	Player->SetRotZ(AnglePlayerZ * 180.0f / PI);

	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}


	// 攻撃判定
	CheckHit();

	// キャラクター同士の押し戻し
	PushBackCharacter();

	// キャラクターと岩の押し戻し
	PushBackRock();

	// 地形との押し戻し
	PushBackField();

  // UIのアップデート
  UI->Update();
}

//==============================================================================
// 攻撃判定の当たり判定
//==============================================================================
void CGame::CheckHit(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	int			numPlayer = 1;					// プレイヤー数
	for (int cntBullet = 0; cntBullet < numPlayer; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player;

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntPlayer = 0; cntPlayer < numPlayer; ++cntPlayer)
		{
			// プレイヤーを取得
			CPlayer*	pPlayerDefense = nullptr;		// 防御側プレイヤー

			// プレイヤーが判定可能か確認
			if (NeedsSkipPlayer(pPlayerDefense))
			{
				continue;
			}
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// 当たり判定
			VECTOR3	positionOffense = pPlayerOffense->Bullet()->Pos();		// 攻撃判定中心座標
			VECTOR3	positionDefense = pPlayerDefense->Pos();				// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;									// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;								// 判定の中心同士の距離
			positionOffense.y += HEIGHT_OFFENSE_BULLET;
			positionDefense.y += HEIGHT_DEFENSE_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z;
			if (distanceOffenseAndDefense < (RADIUS_DEFENSE_CHARACTER + RADIUS_OFFENSE_BULLET) * (RADIUS_DEFENSE_CHARACTER + RADIUS_OFFENSE_BULLET))
			{
				// 当たったときの処理
			}
		}
	}
}

//==============================================================================
// キャラクター同士の押し戻し
//==============================================================================
void CGame::PushBackCharacter(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	int			numPlayer = 1;					// プレイヤー数
	for (int cntBullet = 0; cntBullet < numPlayer; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player;

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntPlayer = 0; cntPlayer < numPlayer; ++cntPlayer)
		{
			// プレイヤーを取得
			CPlayer*	pPlayerDefense = nullptr;		// 防御側プレイヤー

			// プレイヤーが判定可能か確認
			if (NeedsSkipPlayer(pPlayerDefense))
			{
				continue;
			}
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// 当たり判定
			VECTOR3	positionOffense = pPlayerOffense->Pos();		// 攻撃判定中心座標
			VECTOR3	positionDefense = pPlayerDefense->Pos();		// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;							// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;						// 判定の中心同士の距離
			positionOffense.y += HEIGHT_PUSH_CHARACTER;
			positionDefense.y += HEIGHT_PUSH_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z );
			if (distanceOffenseAndDefense < 2.0f * RADIUS_PUSH_CHARACTER)
			{
				// 押し戻し
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * 0.51f * (2.0f * RADIUS_PUSH_CHARACTER - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					pPlayerOffense->AddPos(vectorPushBack);
					vectorPushBack *= -1.0f;
					pPlayerDefense->AddPos(vectorPushBack);
				}
				else
				{
					pPlayerOffense->AddPosX(RADIUS_PUSH_CHARACTER);
					pPlayerDefense->AddPosX(-RADIUS_PUSH_CHARACTER);
				}
			}
		}
	}
}

//==============================================================================
// キャラクターと岩の押し戻し
//==============================================================================
void CGame::PushBackRock(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayer = nullptr;		// 攻撃側プレイヤー
	int			numPlayer = 1;			// プレイヤー数
	for (int cntPlayer = 0; cntPlayer < numPlayer; ++cntPlayer)
	{
		// プレイヤーを取得
		pPlayer = Player;

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(pPlayer))
		{
			continue;
		}

		// 防御側の決定
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// 岩を取得
			CObject*	pRock = ppRock_[cntRock ];		// 岩オブジェクト

			// 当たり判定
			VECTOR3	positionPlayer = pPlayer->Pos();	// 攻撃判定中心座標
			VECTOR3	positionRock = pRock->Pos();		// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;				// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;			// 判定の中心同士の距離
			positionPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positionPlayer;
			distanceOffenseAndDefense = sqrtf( vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z );
			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + HEIGHT_PUSH_ROCK)
			{
				// 押し戻し
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * -(RADIUS_PUSH_CHARACTER + HEIGHT_PUSH_ROCK - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					vectorPushBack.y = 0.0f;
					pPlayer->AddPos(vectorPushBack);
				}
				else
				{
					pPlayer->AddPosX(RADIUS_PUSH_CHARACTER + HEIGHT_PUSH_ROCK);
				}
			}
		}
	}
}

//==============================================================================
// 地形の押し戻し
//==============================================================================
void CGame::PushBackField(void)
{
	// 判定
	CPlayer*	pPlayerCurrent = nullptr;		// 対象オブジェクト
	int			numObject = 1;					// 対象オブジェクト数
	for (int cntPlayer = 0; cntPlayer < numObject; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		pPlayerCurrent = Player;

		// 対象のステートを確認
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// 押し戻し
		PushBackObjectByField(pPlayerCurrent);
	}
}

//==============================================================================
// オブジェクトの地形による押し戻し
//==============================================================================
void CGame::PushBackObjectByField(CObject* pObject)
{
	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	float	HeightGround;		// 地形の高さ
	HeightGround = Ground->GetHeight(pObject->Pos(), &NormalGround);

	// 回転を求める
	VECTOR3	VectorUppObject;		// 上方向ベクトル
	VECTOR3	VectorNormalYZ;			// YZ平面上の法線ベクトル
	VECTOR3	VectorNormalXY;			// XY平面上の法線ベクトル
	float	AnglepObjectX;			// プレイヤー回転X軸
	float	AnglepObjectZ;			// プレイヤー回転Z軸
	VectorUppObject.x = VectorUppObject.z = 0.0f;
	VectorUppObject.y = 1.0f;
	VectorNormalYZ.x = 0.0f;
	VectorNormalYZ.y = NormalGround.y;
	VectorNormalYZ.z = NormalGround.z;
	VectorNormalYZ.Normalize();
	AnglepObjectX = -acosf(VECTOR3::Dot(VectorNormalYZ, VectorUppObject));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglepObjectZ = -acosf(VECTOR3::Dot(VectorNormalXY, VectorUppObject));

	// キャラクターに設定する
	pObject->SetPosY(HeightGround);
	//	pObject->SetRotX(AngleObjectX * 180.0f / PI);
	//	pObject->SetRotZ(AngleObjectZ * 180.0f / PI);
}

//==============================================================================
// プレイヤー判定スキップ
//==============================================================================
bool CGame::NeedsSkipPlayer(CPlayer* pPlayer)
{
	// ステートを確認
	int		statePlayer = 0;// pPlayerCurrent->State();
	if (statePlayer == PLAYER_STATE_DEATH || statePlayer == PLAYER_STATE_RESPAWN)
	{
		return true;
	}

	// スキップしない
	return false;
}

//==============================================================================
// 砲弾判定スキップ
//==============================================================================
bool CGame::NeedsSkipBullet(CPlayer* pPlayer)
{
	// 弾が存在しないとき
//	if (!pPlayer->ExistsBullet())
//	{
//		return true;
//	}

	// スキップしない
	return false;
}

//------------------------------------------------------------------------------
// EOF
