//=============================================================================
//
// UIマネージャ [UI.cpp]
//
// Auther : Takahiro Kikushima
//
//=============================================================================

#include "UI.h"
#include "Life.h"
#include "LoadGauge.h"
#include "LoadString.h"
#include "MiniMap.h"
#include "CPlayer.h"

// 定数
namespace{
	const float     GAUGE_POS_Y = 600.0f;
	const float     GAUGE_POS_X = 70.0f;
	const VECTOR3   GAUGE_POS = VECTOR3(GAUGE_POS_X, GAUGE_POS_Y, 0.0f);
	const VECTOR2   GAUGE_SIZE = VECTOR2(300.0f, 50.0f);
	const float     GAUGE_STR_OFFSET = 50.0f;
	const VECTOR3   GAUGE_STR_POS = VECTOR3(GAUGE_POS_X + GAUGE_STR_OFFSET, GAUGE_POS_Y, 0.0f);
	const VECTOR2   GAUGE_STR_SIZE = VECTOR2(180.0f, 50.0f);
	const COLOR     GAUGE_COLOR = COLOR(0.0f, 1.0f, 0.0f, 1.0f);
	const float     ICON_SIZE = 50.0f;
	const VECTOR3   ICON_POS = VECTOR3(20.0f + ICON_SIZE / 2, GAUGE_POS_Y + ICON_SIZE / 2, 0.0f);
	const VECTOR3   REPORT_BG_POS = VECTOR3(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f, 0.0f);
	const COLOR     REPORT_BG_COLOR = COLOR(0.0f, 0.05f, 0.0f, 0.6f);
	const VECTOR3   LIFE_POS = VECTOR3(60.0f, 60.0f, 0.0f);
}

CUI::CUI()
{
	Life = nullptr;
	MiniMap = nullptr;
	loadGauge = nullptr;
	loadString = nullptr;
	//Ground = nullptr;
	//Player = nullptr;

}
CUI::~CUI()
{

}

void CUI::Init(void)
{
	//ライフ生成
	CLife::Create(LIFE_POS, VECTOR2(100.0f, 100.0f));

	// 装填ゲージ	
	CLoadGauge* load_gauge = nullptr;
	load_gauge = CLoadGauge::Create(GAUGE_POS, GAUGE_SIZE);
	load_gauge->SetDefaultColor(COLOR(0, 0, 0, 1));

	loadGauge = CLoadGauge::Create(GAUGE_POS, GAUGE_SIZE);
	loadGauge->SetDefaultColor(GAUGE_COLOR);
	
	loadGauge->SetTex(CTexture::Texture(TEX_MIKU));
	// 装填中文字
	loadString = CLoadString::Create(GAUGE_STR_POS, GAUGE_STR_SIZE);
	loadString->SetTex(CTexture::Texture(TEX_RELOAD));
	loadString->DrawEnable();

	// 弾アイコン
	CPolygon2D* canonIcon = nullptr;
	canonIcon = CPolygon2D::Create(ICON_POS, VECTOR2(ICON_SIZE, ICON_SIZE));
	canonIcon->SetTex(CTexture::Texture(TEX_GAUGE_ICON));

	//ミニマップの初期化
	MiniMap = new CMiniMap;
	MiniMap->Init();
	CMiniMap::SetFieldSize(VECTOR3(0,0,0));//サイズ仮置き

}

void CUI::Uninit(void)
{

	if (Life != nullptr)
	{
		Life->Release();
		Life = nullptr;
	}
	if (loadGauge != nullptr)
	{
		loadGauge->Release();
		loadGauge = nullptr;
	}
	if (loadString != nullptr)
	{
		loadString->Release();
		loadString = nullptr;
	}
//	Ground->Release();
//	Ground = nullptr;

	Player->Release();
	Player = nullptr;

	SafeDelete(MiniMap);
}

void CUI::Update(void)
{
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
	MiniMap->SetPlayer(0, Player->Pos(), Player->Rot().y);

	MiniMap->Update();
}