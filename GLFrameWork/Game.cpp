#include <math.h>

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
#include "PlayerCamera.h"

#include "LoadGauge.h"
#include "LoadString.h"
#include "Life.h"
// 静的メンバ変数
const float CGame::RADIUS_SKY = 500.0f;   // 空の半径

// 定数
namespace{
	const VECTOR3   GAUGE_POS = VECTOR3(20.0f,20.0f,0.0f);
	const VECTOR2   GAUGE_SIZE = VECTOR2(400.0f,80.0f);
	const VECTOR3   GAUGE_STR_POS = VECTOR3(80.0f,20.0f,0.0f);
	const VECTOR2   GAUGE_STR_SIZE = VECTOR2(150.0f,60.0f);
}


CGame::CGame()
{
	loadGauge = nullptr;
	loadString = nullptr;
	testTimer = 0;
}
CGame::~CGame()
{

}

void CGame::Init(void)
{
	// 地形生成
	Ground = nullptr;
	Ground = CMeshGround::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(300.0f, 300.0f), VECTOR2(20.0f, 20.0f));
	Ground->SetTex(CTexture::Texture(TEX_FIELD));

	// 空生成
	Sky = nullptr;
	Sky = CMeshSphere::Create(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR2(16.0f, 8.0f), RADIUS_SKY, VECTOR2(1.0f, 1.0f));
	Sky->SetTex(CTexture::Texture(TEX_SKY));

#ifdef _DEBUG
	// デバッグワイヤーフレーム
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// プレイヤー生成
	Player = CPlayer::Create(CModel::MIKU,VECTOR3(0.0f,0.0f,0.0f));
	Player->SetTex(CTexture::Texture(TEX_MIKU));
	Player->SetScl(20.0f,20.0f,20.0f);
	CPlayerCamera::Create(Player,300.0f);
	CLife::Create(VECTOR3(50.0f,40.0f,0.0f),VECTOR2(100.0f,100.0f));// 装填ゲージ
	CLoadGauge* load_gauge = nullptr;
	load_gauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	load_gauge->SetTex(CTexture::Texture(TEX_TEAM_LOGO));

	loadGauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	loadGauge->SetTex(CTexture::Texture(TEX_MIKU));

	// 装填中文字
	loadString = CLoadString::Create(GAUGE_STR_POS,GAUGE_STR_SIZE);
	loadString->SetTex(CTexture::Texture(TEX_CONNECTION));
	loadString->DrawEnable();
}

void CGame::Uninit(void)
{
	// プレイヤー破棄
	if (Player != nullptr)
	{
		Player->Release();
		Player = nullptr;
	}

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

	CCamera::Release();
	CObject::ReleaseAll();
}

void CGame::Update(void)
{
	const int max_time = 200;
	testTimer++;
	if (testTimer >= max_time + 50){
		testTimer = 0;
	}
	float rate = (float)testTimer / (float)max_time;
	if (rate > 1.0f){
		rate = 1.0f;
	}
	loadGauge->SetRate(rate);

	// 地形とのあたり判定
	VECTOR3	NormalGround;		// 地形の法線
	float	HeightGround;		// 地形の高さ
	HeightGround = Ground->GetHeight(Player->Pos(), &NormalGround);

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
	AnglePlayerX = acosf(VECTOR3::Dot(VectorNormalYZ, VectorUpPlayer));
	VectorNormalXY.x = NormalGround.x;
	VectorNormalXY.y = NormalGround.y;
	VectorNormalXY.z = 0.0f;
	VectorNormalXY.Normalize();
	AnglePlayerZ = acosf(VECTOR3::Dot(VectorNormalXY, VectorUpPlayer));

	// プレイヤー情報のデバッグ表示
	VECTOR3	positionPlayer = Player->Pos();
	VECTOR3	rotaionPlayer = Player->Rot();
	Console::SetCursorPos(1, 1);
	Console::Print("Pos : (%9.3f, %9.3f, %9.3f)\n", positionPlayer.x, HeightGround, positionPlayer.z);
	Console::Print("Rot : (%9.3f, %9.3f, %9.3f)\n", 180.0f / PI * AnglePlayerX, rotaionPlayer.y, 180.0f / PI * AnglePlayerZ);

	// プレイヤーに設定する
	Player->SetPosY(HeightGround);
	Player->SetRotX(AnglePlayerX * 180.0f / PI);
	Player->SetRotZ(AnglePlayerZ * 180.0f / PI);

	// シーン遷移
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}
}