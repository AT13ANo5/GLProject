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
#include "Input/VC.h"
#include "Billboard.h"
#include "Effect3D.h"
#include "Explosion.h"
#include "Smoke.h"
#include "Ballistic.h"
#include "Polygon3D.h"
#include "Texture.h"
#include "BattleAreaCylinder.h"
#include "MeshGround.h"
#include "MeshSphere.h"
#include "Model.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "PlayerCamera.h"
#include "MiniMap.h"
#include "SoundAL.h"
#include "Shadow.h"

#include "UI.h"

// 静的メンバ変数
const float CGame::RADIUS_SKY = 2500.0f;   // 空の半径
CUI* CGame::UI = nullptr;
CPlayer** CGame::Player;
CPlayer* Player = nullptr;//プレイヤー

CGame::PHSE CGame::gamePhase;

int CGame::gamePhaseCnt = 0;
const float	CGame::RADIUS_DEFENSE_CHARACTER = 12.0f;	// キャラクターの防御半径
const float	CGame::HEIGHT_DEFENSE_CHARACTER = 0.0f;		// キャラクターの防御中心高さ
const float	CGame::RADIUS_OFFENSE_BULLET = 10.0f;		// 砲弾の攻撃半径
const float	CGame::HEIGHT_OFFENSE_BULLET = 0.0f;		// 砲弾の攻撃中心高さ
const float	CGame::RADIUS_PUSH_CHARACTER = 10.0f;		// キャラクターの押し戻し半径
const float	CGame::HEIGHT_PUSH_CHARACTER = 0.0f;		// キャラクターの押し戻し中心高さ
const float	CGame::RADIUS_DEFENSE_ROCK = 28.0f;			// 岩の防御半径
const float	CGame::HEIGHT_DEFENSE_ROCK = 0.0f;			// 岩の防御中心高さ
const float	CGame::RADIUS_PUSH_ROCK = 38.0f;			// 岩の押し戻し半径
const float	CGame::HEIGHT_PUSH_ROCK = 0.0f;				// 岩の押し戻し中心高さ
const float CGame::FIELD_PANEL_SIZE = 35.0f;			//フィールドのパネル一枚のサイズ
const float	CGame::HEIGHT_PLAYER_TO_FIELD = 10.0f;		// プレイヤーと地面の差

const float	CGame::RADIUS_AREA_BATTLE = 1000.0f;		// 戦闘エリア半径
const float	CGame::HEIGHT_WALL = 500.0f;				// 壁の高さ

const int	CGame::MAX_ROCK = 20;						// 岩の数

const VECTOR3 CGame::PLAYER_POSITION_LIST[PLAYER_MAX] = {
 VECTOR3(-0.0f,100.0f,800),
 VECTOR3(690,100.0f,400),
 VECTOR3(690,100.0f,-400),
 VECTOR3(0,100.0f,-800),
 VECTOR3(-700,100.0f,-400),
 VECTOR3(-700,100.0f,400),
};

const VECTOR3 CGame::PLAYER_ROTATION_LIST[PLAYER_MAX] = {
 VECTOR3(0,180,0),
 VECTOR3(0,240.0f,0),
 VECTOR3(0,300.0f,0),
 VECTOR3(0,0,0),
 VECTOR3(0,60.0f,0),
 VECTOR3(0,120.0f,0),
};

const VECTOR3 CGame::ROCK_POSITION_LIST[] = {
	VECTOR3(-214.0f,100.0f,421.0f),
	VECTOR3(359.0f,100.0f,188.0f),
	VECTOR3(94.0f,100.0f,-458.0f),
	VECTOR3(-198.0f,100.0f,222.0f),
	VECTOR3(419.0f,100.0f,293.0f),
	VECTOR3(-325.0f,100.0f,164.0f),
	VECTOR3(-471.0f,100.0f,-115.0f),
	VECTOR3(368.0f,100.0f,-373.0f),
	VECTOR3(-476.0f,100.0f,231.0f),
	VECTOR3(-249.0f,100.0f,-319.0f),
	VECTOR3(-243.0f,100.0f,481.0f),
	VECTOR3(345.0f,100.0f,-253.0f),
	VECTOR3(444.0f,100.0f,-118.0f),
	VECTOR3(186.0f,100.0f,27.0f),
	VECTOR3(387.0f,100.0f,-438.0f),
	VECTOR3(-12.0f,100.0f,-439.0f),
	VECTOR3(496.0f,100.0f,-332.0f),
	VECTOR3(-247.0f,100.0f,143.0f),
	VECTOR3(-302.0f,100.0f,-296.0f),
	VECTOR3(-171.0f,100.0f,-274.0f),

};

const VECTOR3 CGame::ROCK_ROTATION_LIST[] = {
	VECTOR3(10.0f, 10.0f, 10.0f),
	VECTOR3(20.0f, 20.0f, 20.0f),
	VECTOR3(30.0f, 30.0f, 30.0f),
	VECTOR3(40.0f, 40.0f, 40.0f),
	VECTOR3(50.0f, 50.0f, 50.0f),
	VECTOR3(60.0f, 95.0f, 60.0f),
	VECTOR3(70.0f, 70.0f, 70.0f),
	VECTOR3(80.0f, 110.0f, 80.0f),
	VECTOR3(90.0f, 90.0f, 90.0f),
	VECTOR3(100.0f, 100.0f, 100.0f),
	VECTOR3(110.0f, 110.0f, 110.0f),
	VECTOR3(120.0f, 120.0f, 120.0f),
	VECTOR3(130.0f, 130.0f, 130.0f),
	VECTOR3(140.0f, 140.0f, 140.0f),
	VECTOR3(150.0f, 150.0f, 150.0f),
	VECTOR3(160.0f, 160.0f, 160.0f),
	VECTOR3(170.0f, 170.0f, 170.0f),
	VECTOR3(180.0f, 185.0f, 180.0f),
	VECTOR3(190.0f, 190.0f, 190.0f),
	VECTOR3(200.0f, 200.0f, 200.0f)
};

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
	gamePhase = PHASE_3;
	gamePhaseCnt = 0;
	gameEndCount = 0;
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
	CSoundAL::Play(CSoundAL::BGM_GAME);
	//地形生成
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(FIELD_PANEL_SIZE,FIELD_PANEL_SIZE),VECTOR2(0,0),1.5f);
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// 空生成
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f,0.0f,0.0f),VECTOR2(16.0f,8.0f),RADIUS_SKY);
	Sky->SetTex(CTexture::Texture(TEX_SKY));

	// 境界線生成
	CylinderArea = nullptr;
	CylinderArea = CBattleAreaCylinder::Create(VECTOR3(0.0f,0.0f,0.0f),HEIGHT_WALL,VECTOR2(64.0f,1.0f),RADIUS_AREA_BATTLE,VECTOR2(1,-0.5f));
	CylinderArea->SetTex(CTexture::Texture(TEX_WALL));
	CylinderArea->SetAlpha(0.5f);

	Player = new CPlayer*[PLAYER_MAX];
	// プレイヤー生成
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		if (i % 2 == 1)
			Player[i] = CPlayer::Create(CModel::RINCHAN,PLAYER_POSITION_LIST[i],i);
		else
			Player[i] = CPlayer::Create(CModel::YOUJO,PLAYER_POSITION_LIST[i],i);

		Player[i]->SetTex(CTexture::Texture(TEX_YOUJO_RED + i));
		Player[i]->SetRot(PLAYER_ROTATION_LIST[i]);
		Player[i]->setBarrelTex(TEX_YOUJO_RED + i);

		if (i == CManager::netData.charNum)
		{
			Player[i]->SetPlayerFlag(true);
			Player[i]->CreateBallistic();
		}
	}
		//プレイヤーカメラ生成
	CPlayerCamera::Create(Player[CManager::netData.charNum],35.0f);

	// 【テスト】各プレイヤーの色をセット
	for(int i = 0; i < PLAYER_MAX; i++)
	{
		switch(i)
		{
			// 赤
			case 0:
				Player[i]->SetPlayerColor(RED(0.5f));
				break;

			// 青
			case 1:
				Player[i]->SetPlayerColor(BLUE(0.5f));
				break;

			// 水
			case 2:
				Player[i]->SetPlayerColor(CYAN(0.5f));
				break;

			// 橙
			case 3:
				Player[i]->SetPlayerColor(COLOR(1.0f, 0.7f, 0.0f, 0.5f));
				break;
		
			// 白
			case 4:
				Player[i]->SetPlayerColor(WHITE(0.5f));
				break;

			// P
			case 5:
				Player[i]->SetPlayerColor(YELLOW(0.5f));
				break;

			default:
				break;
		}
	}

	// プレイヤーの入力を止める
	for (int i = 0; i < PLAYER_MAX; i++){
		Player[i]->SetInputFlag(false);
	}

	// UI初期化
	UI = new CUI;
	UI->Init();
	UI->setMyID(CManager::netData.charNum);
	UI->SetPlayer(Player);
	UI->MiniMap()->SetFieldSize(VECTOR3(RADIUS_AREA_BATTLE*2.25f,0,RADIUS_AREA_BATTLE*2.25f));

	// 岩の生成
	ppRock_ = new CModel*[MAX_ROCK];

	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		ppRock_[cntRock] = CModel::Create(CModel::ROCK,ROCK_POSITION_LIST[cntRock]);
		ppRock_[cntRock]->SetRot(ROCK_ROTATION_LIST[cntRock]);
		ppRock_[cntRock]->SetScl(1,1,1);
		ppRock_[cntRock]->SetTex(CTexture::Texture(TEX_ROCK));
		PushBackObjectByField(ppRock_[cntRock], 10.0f);
	}

	// 影生成
	Shadow = new CShadow*[2 * PLAYER_MAX];
	for (int cntShadow = 0; cntShadow < 2 * PLAYER_MAX; ++cntShadow)
	{
		Shadow[cntShadow] = nullptr;
		Shadow[cntShadow] = CShadow::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(1.0f, 1.0f));
		if (cntShadow < PLAYER_MAX)
		{
			Shadow[cntShadow]->SetScl(30, 30, 30);
		}
		else
		{
			Shadow[cntShadow]->SetScl(20, 20, 20);
		}
		Shadow[cntShadow]->SetTex(CTexture::Texture(TEX_SHADOW));
	}

	CManager::gameStartFlag = true;

	CManager::sendGameStart();
}

void CGame::subTimer()
{
	//	UIのタイマー減らす処理
	UI->SubTime();
}

//=============================================================================
//	プレイヤー情報のセット関数
//=============================================================================
void CGame::SetPlayerState(NET_DATA _netData,DATA_TYPE _dataType)
{
	//	自分以外のデータなら
	if (_netData.charNum != CManager::netData.charNum)
	{
		switch (_dataType)
		{
		case DATA_TYPE_POS:

			Player[_netData.charNum]->SetPosX(_netData.data_pos.posX);
			Player[_netData.charNum]->SetPosY(_netData.data_pos.posY);
			Player[_netData.charNum]->SetPosZ(_netData.data_pos.posZ);

			break;

		case DATA_TYPE_ROT:

			/*Player[_netData.charNum]->SetAxisRotation(VECTOR3(_netData.data_rot.X, _netData.data_rot.Y, _netData.data_rot.Z));
			Player[_netData.charNum]->SetRotationAxis(_netData.data_rot.rot);
			Player[_netData.charNum]->SetRotY(_netData.data_rot.yRotation);*/
			Player[_netData.charNum]->SetRotY(_netData.data_rot.rotY);
			
			break;

		case DATA_TYPE_CANNONROT:

			Player[_netData.charNum]->setBarrelRot(
				VECTOR3(_netData.data_cannonRot.rotX,
				_netData.data_cannonRot.rotY,
				_netData.data_cannonRot.rotZ
				));

			break;

		case DATA_TYPE_CANNON:

			if (_netData.data_cannon.flag == true)
			{
				Player[_netData.charNum]->BlastBullet();
			}

			break;

		case DATA_TYPE_DEATH:

			break;

		case DATA_TYPE_KILL:

			break;
		}
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
	CManager::SendKillDeath(Player[CManager::netData.charNum]->getKillCount(),
							Player[CManager::netData.charNum]->getDeathCount());

	CManager::gameStartFlag = false;

	// 影の破棄
	for (int chtShadow = 0; chtShadow < 2 * PLAYER_MAX; ++chtShadow)
	{
		SafeDelete(Shadow[chtShadow]);
	}
	SafeDelete(Shadow);

	// 岩の破棄
	for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
	{
		SafeRelease(ppRock_[cntRock]);
		ppRock_[cntRock] = nullptr;
	}
	SafeDeletes(ppRock_);

	// プレイヤー破棄
	//SafeRelease(Player);
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		SafeRelease(Player[i]);
	}

	SafeDeletes(Player);

	// 境界線の破棄
	SafeRelease(CylinderArea);

	// 空破棄

	SafeRelease(Sky);

	// 地形破棄
	SafeRelease(Ground);

	// UI破棄
	UI->Uninit();
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
	VC* vc = VC::Instance();
	// 最初のカウントダウン
	StartCount();

	if (vc->Trigger(COMMAND_OK))
	{
		if (CManager::netData.charNum == 0)
		{
			CManager::SendChangeResult();
			CManager::ChangeScene(SCENE_RESULT);
		}
	}

	// 空の位置プレイヤーに合わせる
	Sky->SetPosX(Player[CManager::netData.charNum]->Pos().x);
	Sky->SetPosZ(Player[CManager::netData.charNum]->Pos().z);
	for (int loop = 0;loop < PLAYER_MAX;loop++)
	{
		if (Player[loop]->PlayerLife() <= 0)
		{
			VECTOR3	NormalGround;		// 地形の法線
			VECTOR3 Respawn = VECTOR3(rand() % 100 + 0.0f,0,rand() % 100 + 0.0f);
			Respawn.y = Ground->GetHeight(Respawn,&NormalGround);

			Player[loop]->SetDeath(Respawn, loop);
		}
	}
	// 攻撃判定
	CheckHitPlayer();

	// 攻撃判定
	CheckHitRock();

	// キャラクター同士の押し戻し
	PushBackCharacter();

	// キャラクターと岩の押し戻し
	PushBackRock();

	// 地形との押し戻し
	PushBackField();

	// 地形との判定
	IsLandField();

	// 行動可能範囲判定
	PushBackBattleArea();

	// 着弾地点判定
	HitBulletToField();

	// 影を合わせる
	for (int cntShadow = 0; cntShadow < PLAYER_MAX; ++cntShadow)
	{
		VECTOR3	positionShadow = Player[cntShadow]->Pos();
		positionShadow.y -= HEIGHT_PLAYER_TO_FIELD;
		Shadow[cntShadow]->SetPos(positionShadow);
		PushBackObjectByField(Shadow[cntShadow], 1.0f);
	}
	for (int cntShadow = 0; cntShadow < PLAYER_MAX; ++cntShadow)
	{
		if (NeedsSkipBullet(Player[cntShadow]))
		{
			Shadow[cntShadow + PLAYER_MAX]->SetPos(VECTOR3(0.0f, 1000.0f, 0.0f));
			continue;
		}
		VECTOR3	positionShadow = Player[cntShadow]->Bullet()->Pos();
		Shadow[cntShadow + PLAYER_MAX]->SetPos(positionShadow);
		PushBackObjectByField(Shadow[cntShadow + PLAYER_MAX], 1.0f);
	}

	// UIのアップデート
	UI->Update();
}

//==============================================================================
// 攻撃判定の当たり判定 プレイヤー
//==============================================================================
void CGame::CheckHitPlayer(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player[cntBullet];

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
		{
			// 自身とは判定しない
			if (cntBullet == cntPlayer)
			{
				continue;
			}

			// プレイヤーを取得
			CPlayer*	pPlayerDefense = Player[cntPlayer];		// 防御側プレイヤー

			// プレイヤーが判定可能か確認
			if (NeedsSkipPlayer(pPlayerDefense))
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
				// 弾の消滅
				pPlayerOffense->ReleaseBullet();

				// 当たったときの処理
				pPlayerDefense->AddPlayerLife(-1);
				pPlayerDefense->SetState(PLAYER_STATE_DAMAGE);

				// エフェクト：爆発　弾がプレイヤーに当たったとき
				if (pPlayerDefense->PlayerLife() > 0)
				{
					if (pPlayerDefense->GetPreyerFlag())
					{
						VC::Instance()->SetVibration(0.7f,30,0.7f,30);
					}
					CSoundAL::Play(CSoundAL::SE_HIT,pPlayerDefense->Pos());
					CSoundAL::Play(CSoundAL::SE_DAMAGE,pPlayerDefense->Pos());
				}
				CExplosion::Create(pPlayerDefense->Pos());

				//	長崎
				if (pPlayerDefense->PlayerLife() == 0)
				{
					//	殺された数加算
					pPlayerDefense->addDeathCount();
					CManager::SendDeath(pPlayerDefense->getDeathCount(), pPlayerDefense->getPlayerID());

					//	殺した数加算
					pPlayerOffense->addKillCount();
					CManager::SendKill(pPlayerOffense->getKillCount(), pPlayerOffense->getPlayerID());

					if (CManager::userInfo[pPlayerDefense->getPlayerID()].entryFlag == false)
					{
						if (CManager::netData.charNum == 0)
							CManager::SendDeathFlag(pPlayerDefense->getPlayerID());
					}
				}

				// 処理終了
				break;
			}
		}
	}
}

//==============================================================================
// 攻撃判定の当たり判定 岩
//==============================================================================
void CGame::CheckHitRock(void)
{
	// 攻撃側の決定
	CPlayer*	pPlayerOffense = nullptr;		// 攻撃側プレイヤー
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player[cntBullet];

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// プレイヤーを取得
			CObject*	pRock = ppRock_[cntRock];		// 防御側岩

			// 当たり判定
			VECTOR3	positionOffense = pPlayerOffense->Bullet()->Pos();		// 攻撃判定中心座標
			VECTOR3	positionDefense = pRock->Pos();							// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;									// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;								// 判定の中心同士の距離
			float	scalingRock;											// 岩の大きさ
			scalingRock = (pRock->Scl().x < pRock->Scl().z ? pRock->Scl().x : pRock->Scl().z);
			positionOffense.y += HEIGHT_OFFENSE_BULLET;
			positionDefense.y += HEIGHT_DEFENSE_CHARACTER;
			vectorOffenseToDefense = positionDefense - positionOffense;
			distanceOffenseAndDefense = vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z;

			if (distanceOffenseAndDefense < (RADIUS_DEFENSE_ROCK * scalingRock + RADIUS_OFFENSE_BULLET) * (RADIUS_DEFENSE_ROCK * scalingRock + RADIUS_OFFENSE_BULLET))
			{
				// 弾の消滅
				pPlayerOffense->ReleaseBullet();

				// エフェクト：爆発　弾が岩に当たったとき
				CSmoke::Create(positionOffense);

				// 処理終了
				break;
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
	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// プレイヤーを取得
		pPlayerOffense = Player[cntBullet];

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(pPlayerOffense))
		{
			continue;
		}

		// 防御側の決定
		for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
		{
			// プレイヤーを取得
			CPlayer*	pPlayerDefense = Player[cntPlayer];		// 防御側プレイヤー

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
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);
			if (distanceOffenseAndDefense < 2.0f * RADIUS_PUSH_CHARACTER)
			{
				// 押し戻し
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * 0.51f * (2.0f * RADIUS_PUSH_CHARACTER - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					pPlayerDefense->AddPos(vectorPushBack);
					vectorPushBack *= -1.0f;
					pPlayerOffense->AddPos(vectorPushBack);
				}
				else
				{
					pPlayerOffense->AddPosX(RADIUS_PUSH_CHARACTER);
					pPlayerDefense->AddPosX(-RADIUS_PUSH_CHARACTER);
				}

				// エフェクト：火花　プレイヤー同士のぶつかり
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
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// プレイヤーを取得
		pPlayer = Player[cntPlayer];

		// プレイヤーが判定可能か確認
		if (NeedsSkipPlayer(pPlayer))
		{
			continue;
		}

		// 防御側の決定
		for (int cntRock = 0; cntRock < MAX_ROCK; ++cntRock)
		{
			// 岩を取得
			CObject*	pRock = ppRock_[cntRock];		// 岩オブジェクト

			// 当たり判定
			VECTOR3	positionPlayer = pPlayer->Pos();	// 攻撃判定中心座標
			VECTOR3	positionRock = pRock->Pos();		// 防御判定中心座標
			VECTOR3	vectorOffenseToDefense;				// 攻撃判定から防御判定へのベクトル
			float	distanceOffenseAndDefense;			// 判定の中心同士の距離
			float	scalingRock;						// 岩の大きさ
			scalingRock = (pRock->Scl().x < pRock->Scl().z ? pRock->Scl().x : pRock->Scl().z);
			positionPlayer.y += HEIGHT_PUSH_CHARACTER;
			positionRock.y += HEIGHT_PUSH_ROCK;
			vectorOffenseToDefense = positionRock - positionPlayer;
			distanceOffenseAndDefense = sqrtf(vectorOffenseToDefense.x * vectorOffenseToDefense.x + vectorOffenseToDefense.y * vectorOffenseToDefense.y + vectorOffenseToDefense.z * vectorOffenseToDefense.z);

			if (distanceOffenseAndDefense < RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock)
			{
				// 押し戻し
				if (distanceOffenseAndDefense < -FLT_EPSILON || distanceOffenseAndDefense > FLT_EPSILON)
				{
					VECTOR3	vectorPushBack = vectorOffenseToDefense * -(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock - distanceOffenseAndDefense) / distanceOffenseAndDefense;
					vectorPushBack.y = 0.0f;
					pPlayer->AddPos(vectorPushBack);
				}
				else
				{
					pPlayer->AddPosX(RADIUS_PUSH_CHARACTER + RADIUS_PUSH_ROCK * scalingRock);
				}

				// エフェクト：火花　プレイヤーと岩のぶつかり
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
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		pPlayerCurrent = Player[cntPlayer];

		// 対象のステートを確認
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// 押し戻し
		PushBackObjectByField(pPlayerCurrent, HEIGHT_PLAYER_TO_FIELD);
	}
}

//==============================================================================
// 地形との判定
//==============================================================================
void CGame::IsLandField(void)
{
	// 判定
	VECTOR3	 nor;			// 法線
	float	 height;		// 高さ
	VECTOR3	 bulletPos;		// 対象オブジェクト座標

	CPlayer* pPlayerCurrent = nullptr;		// 対象プレイヤー

	for (int cntBullet = 0; cntBullet < PLAYER_MAX; ++cntBullet)
	{
		// 対象プレイヤーの取得
		pPlayerCurrent = Player[cntBullet];

		// 弾が存在しなければ判定しない
		if (NeedsSkipBullet(pPlayerCurrent))
		{
			continue;
		}

		// 対象オブジェクト座標を取得
		bulletPos = pPlayerCurrent->Bullet()->Pos();

		// 判定
		height = Ground->GetHeight(bulletPos - VECTOR3(0.0f, BULLET_SIZE * 0.5f, 0.0f), &nor);
		if (height >= bulletPos.y)
		{
			// 弾の消滅処理
			CSoundAL::Play(CSoundAL::SE_IMPACT, bulletPos);
			pPlayerCurrent->ReleaseBullet();

			// エフェクト：爆発　弾と地形の判定
			CSmoke::Create(VECTOR3(bulletPos.x, height, bulletPos.z));
		}
	}
}

//==============================================================================
// オブジェクトの地形による押し戻し
//==============================================================================
void CGame::PushBackObjectByField(CObject* pObject, float offsetY)
{
	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	float	HeightGround;		// 地形の高さ
	HeightGround = Ground->GetHeight(pObject->Pos(), &NormalGround) + offsetY;

	//********************************************************
	// 2015_02_12 姿勢制御用の処理を追加 ここから
	//********************************************************
	// 回転を求める
	VECTOR3	vectorUp(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
	VECTOR3	vectorAxisRotation;				// 回転軸
	float	rotation = 0.0f;				// 回転量
	VECTOR3::Cross(&vectorAxisRotation, NormalGround, vectorUp);
	if (vectorAxisRotation.x < FLT_EPSILON && vectorAxisRotation.x > -FLT_EPSILON)
	{
		if (vectorAxisRotation.z < FLT_EPSILON && vectorAxisRotation.z > -FLT_EPSILON)
		{
			if (vectorAxisRotation.y < FLT_EPSILON && vectorAxisRotation.y > -FLT_EPSILON)
			{
				vectorAxisRotation.y = 1.0f;
			}
		}
	}
	vectorAxisRotation.Normalize();
	rotation = VECTOR3::Dot(NormalGround, vectorUp);
	if (rotation <= 1.0f && rotation >= -1.0f)
	{
		rotation = RAD_TO_DEG * acosf(rotation);
	}
	else
	{
		rotation = 0.0f;
	}

	// キャラクターに設定する
	pObject->SetPosY(HeightGround);
	pObject->SetAxisRotation(vectorAxisRotation);
	pObject->SetRotationAxis(rotation);
	//********************************************************
	// 2015_02_12 姿勢制御用の処理を追加 ここまで
	//********************************************************
}

//==============================================================================
// プレイヤー判定スキップ
//==============================================================================
void CGame::PushBackBattleArea(void)
{
	// 判定
	CPlayer*	pPlayerCurrent = nullptr;		// 対象オブジェクト
	for (int cntPlayer = 0; cntPlayer < PLAYER_MAX; ++cntPlayer)
	{
		// 対象オブジェクトを取得
		pPlayerCurrent = Player[cntPlayer];

		// 対象のステートを確認
		if (NeedsSkipPlayer(pPlayerCurrent))
		{
			continue;
		}

		// 押し戻し
		VECTOR3	vectorPlayerToCenter = Ground->Pos() - pPlayerCurrent->Pos();
		vectorPlayerToCenter.y = 0.0f;
		float	distanceFromCenter = vectorPlayerToCenter.x * vectorPlayerToCenter.x + vectorPlayerToCenter.y * vectorPlayerToCenter.y + vectorPlayerToCenter.z * vectorPlayerToCenter.z;
		if (distanceFromCenter > (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER) * (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER))
		{
			float	distancePushBack = sqrtf(distanceFromCenter) - (RADIUS_AREA_BATTLE - RADIUS_PUSH_CHARACTER);
			vectorPlayerToCenter.Normalize();
			pPlayerCurrent->AddPos(vectorPlayerToCenter * distancePushBack);
		}
	}
}

//==============================================================================
// プレイヤー判定スキップ
//==============================================================================
bool CGame::NeedsSkipPlayer(CPlayer* pPlayer)
{
	// エラーチェック
	if (pPlayer == nullptr)
	{
		return true;
	}

	// ステートを確認
	int		statePlayer = pPlayer->State();
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
	if (!pPlayer->BulletUseFlag())
	{
		return true;
	}

	// スキップしない
	return false;
}

//==============================================================================
// 着弾地点判定
//==============================================================================
void CGame::HitBulletToField(void)
{
	VECTOR3		nor;
	float		height;
	CBillboard* mark;
	VECTOR3		markPos;
	CBallistic* ballistic = Player[CManager::netData.charNum]->GetBallistic();
	CPolygon3D* landing = ballistic->GetLanding();

	for(int cnt = 0; cnt < MARK_MAX; ++cnt)
	{
		// 初期化
		nor = VECTOR3(0.0f,0.0f,0.0f);
		height = 0.0f;

		// マーク情報
		mark = ballistic->GetMark(cnt);
		markPos = mark->Pos();

		// 高さ判定
		height = Ground->GetHeight(markPos - VECTOR3(0.0f, BULLET_SIZE * 0.5f, 0.0f), &nor);
		if(height >= markPos.y)
		{
			// 回転を求める
			VECTOR3	vectorUp(0.0f, 1.0f, 0.0f);		// 上方向ベクトル
			VECTOR3	vectorAxisRotation;				// 回転軸
			float	rotation = 0.0f;				// 回転量
			VECTOR3::Cross(&vectorAxisRotation, nor, vectorUp);
			if (vectorAxisRotation.x < FLT_EPSILON && vectorAxisRotation.x > -FLT_EPSILON)
			{
				if (vectorAxisRotation.z < FLT_EPSILON && vectorAxisRotation.z > -FLT_EPSILON)
				{
					if (vectorAxisRotation.y < FLT_EPSILON && vectorAxisRotation.y > -FLT_EPSILON)
					{
						vectorAxisRotation.y = 1.0f;
					}
				}
			}
			vectorAxisRotation.Normalize();
			rotation = VECTOR3::Dot(nor, vectorUp);
			if (rotation <= 1.0f && rotation >= -1.0f)
			{
				rotation = RAD_TO_DEG * acosf(rotation);
			}
			else
			{
				rotation = 0.0f;
			}

			// 着弾マークに設定する
			landing->SetPos(VECTOR3(markPos.x, height + 0.5f, markPos.z));
			landing->SetAxisRotation(vectorAxisRotation);
			landing->SetRotationAxis(rotation);
			break;
		}
	}
}
//==============================================================================
// 最初のカウントダウンのやつ
//==============================================================================
void CGame::StartCount(void)
{
	const int PHASE_COUNT_3 = 60 * 1;
	const int PHASE_COUNT_2 = 60 * 2;
	const int PHASE_COUNT_1 = 60 * 3;
	const int PHASE_COUNT_START = 60 * 4;
	const int PHASE_COUNT_START_FIN = 60 * 5;
	const int PHASE_COUNT_END = 60 * 3;

	switch (gamePhase){

		case PHASE_NONE:
		{
			break;
		}
		case PHASE_3:
		{
			if (gamePhaseCnt == PHASE_COUNT_3){
				UI->SetNumber(3);
				UI->SetNumberDrawFlag(true);
				UI->SetTimeUpdateFlag(false);
				UI->SetCountSizeRefresh();
				gamePhase = PHASE_2;
				CSoundAL::Play(CSoundAL::SE_COUNT_DOWN);
			}
			break;
		}
		case PHASE_2:
		{
			if (gamePhaseCnt == PHASE_COUNT_2){
				UI->SetNumber(2);
				UI->SetCountSizeRefresh();
				gamePhase = PHASE_1;
				CSoundAL::Play(CSoundAL::SE_COUNT_DOWN);
			}
			break;
		}
		case PHASE_1:
		{
			if (gamePhaseCnt == PHASE_COUNT_1){
				UI->SetNumber(1);
				UI->SetCountSizeRefresh();
				gamePhase = PHASE_START;
				CSoundAL::Play(CSoundAL::SE_COUNT_DOWN);
			}
			break;
		}
		case PHASE_START:
		{
			if (gamePhaseCnt == PHASE_COUNT_START){
				UI->SetStringDrawFlag(true);
				UI->SetNumber(0);
				UI->SetNumberDrawFlag(false);
				UI->SetTimeUpdateFlag(true);
				gamePhase = PHASE_START_FIN;
				CSoundAL::Play(CSoundAL::SE_START);

				CManager::SendReborn(1);
				CManager::SendReborn(2);
				CManager::SendReborn(3);
				CManager::SendReborn(4);
				CManager::SendReborn(5);
				// プレイヤーの入力を止める
				for (int i = 0; i < PLAYER_MAX; i++){
					Player[i]->SetInputFlag(true);
				}
			}
				break;
		}
		case PHASE_START_FIN:
		{
			if (gamePhaseCnt == PHASE_COUNT_START_FIN){
				UI->SetStringDrawFlag(false);
				gamePhase = PHASE_PLAY;
			}
			break;
		}
		case PHASE_PLAY:
		{
			if (UI->GetTime() == 0){
				gamePhase = PHASE_END;
				UI->SetStringDrawFlag(true);
				UI->SetStringTexture(CTexture::Texture(TEX_END));

				UI->SetTimeUpdateFlag(false);
				UI->ReportDrawDisable();

				// プレイヤーの攻撃を止める
				for (int i = 0; i < PLAYER_MAX; i++){
					Player[i]->SetEndGameFlag(true);
				}
			}
			break;
		}
		case PHASE_END:
		{
			gameEndCount++;
			if (gameEndCount >= PHASE_COUNT_END){
				CManager::ChangeScene(SCENE_RESULT);
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
// EOF
