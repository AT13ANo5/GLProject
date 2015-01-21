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
#include "Report.h"

#include "LoadGauge.h"
#include "LoadString.h"
#include "Life.h"
#include "NumberManager.h"
#include "MiniMap.h"

// 静的メンバ変数
const float CGame::RADIUS_SKY = 1500.0f;   // 空の半径

// 定数
namespace{
  const float     GAUGE_POS_Y = 600.0f;
  const float     GAUGE_POS_X = 70.0f;
  const VECTOR3   GAUGE_POS = VECTOR3(GAUGE_POS_X, GAUGE_POS_Y, 0.0f);
  const VECTOR2   GAUGE_SIZE = VECTOR2(300.0f,50.0f);
  const float     GAUGE_STR_OFFSET = 50.0f;
  const VECTOR3   GAUGE_STR_POS = VECTOR3(GAUGE_POS_X + GAUGE_STR_OFFSET, GAUGE_POS_Y, 0.0f);
  const VECTOR2   GAUGE_STR_SIZE = VECTOR2(180.0f,50.0f);
  const COLOR     GAUGE_COLOR = COLOR(0.0f, 1.0f, 0.0f, 1.0f);
  const float     ICON_SIZE = 50.0f;
  const VECTOR3   ICON_POS = VECTOR3(20.0f + ICON_SIZE / 2, GAUGE_POS_Y + ICON_SIZE / 2, 0.0f);
  const VECTOR3   REPORT_BG_POS = VECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0.0f);
  const COLOR     REPORT_BG_COLOR = COLOR(0.0f, 0.05f, 0.0f, 0.6f);
  const VECTOR3   LIFE_POS = VECTOR3(60.0f,60.0f,0.0f);
}


CGame::CGame()
{
	loadGauge = nullptr;
	loadString = nullptr;
	reportBg = nullptr;
	report = nullptr;
	MiniMap = nullptr;
	numberManager = nullptr;
}
CGame::~CGame()
{

}

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
	Player = CPlayer::Create(CModel::RINCHAN, VECTOR3(0.0f, 30.0f, 0.0f));	Player->SetTex(CTexture::Texture(TEX_YOUJO_BLUE));
	Player->SetRot(0.0f,180.0f,0.0f);

	//プレイヤーカメラ生成
	CPlayerCamera::Create(Player,300.0f);

	//ライフ生成
	CLife::Create(LIFE_POS,VECTOR2(100.0f,100.0f));

	// 装填ゲージ	
	CLoadGauge* load_gauge = nullptr;
	load_gauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	load_gauge->SetDefaultColor(COLOR(0,0,0,1));

	loadGauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	loadGauge->SetDefaultColor(GAUGE_COLOR);
//	loadGauge->SetTex(CTexture::Texture(TEX_MIKU));
	// 装填中文字
	loadString = CLoadString::Create(GAUGE_STR_POS,GAUGE_STR_SIZE);
	loadString->SetTex(CTexture::Texture(TEX_RELOAD));
	loadString->DrawEnable();
	

	// 弾アイコン
	CPolygon2D* canonIcon = nullptr;
	canonIcon = CPolygon2D::Create(ICON_POS,VECTOR2(ICON_SIZE,ICON_SIZE));
	canonIcon->SetTex(CTexture::Texture(TEX_GAUGE_ICON));

	//ミニマップの初期化
	MiniMap = new CMiniMap;
	MiniMap->Init();
	CMiniMap::SetFieldSize(Ground->Size());

	// 成績表の背景
	reportBg = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	reportBg->SetColor(REPORT_BG_COLOR);

	// 成績表
	const float scl = 0.8f;
	report = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH * scl,SCREEN_HEIGHT * scl));
	report->SetTex(CTexture::Texture(TEX_REPORT));// 成績表の数値
	numberManager = CNumberManager::Create();

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
	SafeDelete(MiniMap);
	CCamera::ReleaseAll();	CObject::ReleaseAll();
}

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
	Console::Print("Pos : (%9.3f, %9.3f, %9.3f)\n",positionPlayer.x,HeightGround,positionPlayer.z);
	Console::Print("Rot : (%9.3f, %9.3f, %9.3f)\n",180.0f / PI * AnglePlayerX,rotaionPlayer.y,180.0f / PI * AnglePlayerZ);

	// プレイヤーに設定する
	Player->SetPosY(HeightGround);
	Player->SetRotX(AnglePlayerX * 180.0f / PI);
	Player->SetRotZ(AnglePlayerZ * 180.0f / PI);

	// 装填ゲージ
	const float currentTimer = (float)Player->ReloadTimer();
	const float maxTimer = (float)PLAYER_RELOAD_TIME;
	const float rate = currentTimer / maxTimer;

	loadGauge->SetRate(rate);
	if (rate >= 1.0f){
		loadString->DrawDisable();
	}
	if (rate <= 0.0f){
		loadString->DrawEnable();
	}
	if (CKeyboard::GetTrigger(DIK_RETURN))
	{
		CManager::ChangeScene(SCENE_RESULT);
	}

	if (CKeyboard::GetPress(DIK_P)){
		reportBg->SetDrawFlag(true);
		report->SetDrawFlag(true);
		numberManager->SetDrawFlag(true);
	}
	else {
		reportBg->SetDrawFlag(false);
		report->SetDrawFlag(false);
		numberManager->SetDrawFlag(false);
	}
	MiniMap->SetPlayer(0,Player->Pos(),Player->Rot().y);

	MiniMap->Update();

}