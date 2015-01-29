//=============================================================================
//
// UIマネージャ [UI.cpp]
//
// Auther : Takahiro Kikushima
//          Masato Masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "UI.h"
#include "life.h"
#include "LoadGauge.h"
#include "LoadString.h"
#include "MiniMap.h"
#include "Report.h"
#include "NumberManager.h"
#include "Keyboard.h"

#include "CPlayer.h"

namespace{

	// 装填ゲージ
	const float     GAUGE_POS_Y = 600.0f;
	const float     GAUGE_POS_X = 70.0f;
	const VECTOR3   GAUGE_POS = VECTOR3(GAUGE_POS_X,GAUGE_POS_Y,0.0f);
	const VECTOR2   GAUGE_SIZE = VECTOR2(300.0f,50.0f);
	const float     GAUGE_STR_OFFSET = 50.0f;
	const VECTOR3   GAUGE_STR_POS = VECTOR3(GAUGE_POS_X + GAUGE_STR_OFFSET,GAUGE_POS_Y,0.0f);
	const VECTOR2   GAUGE_STR_SIZE = VECTOR2(180.0f,50.0f);
	const COLOR     GAUGE_COLOR = COLOR(0.0f,1.0f,0.0f,1.0f);
	// 装填ゲージアイコン
	const float     ICON_SIZE = 50.0f;
	const VECTOR3   ICON_POS = VECTOR3(20.0f + ICON_SIZE / 2,GAUGE_POS_Y + ICON_SIZE / 2,0.0f);
	// 成績表
	const VECTOR3   REPORT_BG_POS = VECTOR3(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f,0.0f);
	const COLOR     REPORT_BG_COLOR = COLOR(0.0f,0.05f,0.0f,0.6f);
	// ライフ
	const VECTOR3   life_POS = VECTOR3(60.0f,60.0f,0.0f);
}

//=============================================================================
// constructor
//=============================================================================
CUI::CUI()
{
	life = nullptr;
	miniMap = nullptr;
	loadGauge = nullptr;
	loadString = nullptr;
	reportBg = nullptr;
	report = nullptr;
	numberManager = nullptr;
}

//=============================================================================
// destructor
//=============================================================================
CUI::~CUI()
{
}

//=============================================================================
// init
//=============================================================================
void CUI::Init(void)
{
	//ライフ生成
	life = CLife::Create(life_POS,VECTOR2(100.0f,100.0f));

	// 装填ゲージ
	CLoadGauge* load_gauge = nullptr;
	load_gauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	load_gauge->SetDefaultColor(COLOR(0,0,0,1));

	loadGauge = CLoadGauge::Create(GAUGE_POS,GAUGE_SIZE);
	loadGauge->SetDefaultColor(GAUGE_COLOR);

	loadString = CLoadString::Create(GAUGE_STR_POS,GAUGE_STR_SIZE);
	loadString->SetTex(CTexture::Texture(TEX_RELOAD));
	loadString->DrawEnable();

	// 弾アイコン
	CPolygon2D* canonIcon = nullptr;
	canonIcon = CPolygon2D::Create(ICON_POS,VECTOR2(ICON_SIZE,ICON_SIZE));
	canonIcon->SetTex(CTexture::Texture(TEX_GAUGE_ICON));

	// 成績表の背景
	reportBg = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH,SCREEN_HEIGHT));
	reportBg->SetColor(REPORT_BG_COLOR);

	// 成績表
	const float scl = 0.8f;
	report = CReport::Create(REPORT_BG_POS,VECTOR2(SCREEN_WIDTH * scl,SCREEN_HEIGHT * scl));
	report->SetTex(CTexture::Texture(TEX_REPORT));// 成績表の数値

	// 番号マネージャー
	numberManager = CNumberManager::Create();

	// 成績表隠す
	reportBg->SetDrawFlag(false);
	report->SetDrawFlag(false);
	numberManager->SetDrawFlag(false);

	//ミニマップの初期化
	miniMap = new CMiniMap;
	miniMap->Init();
	CMiniMap::SetFieldSize(VECTOR3(0,0,0));//サイズ仮置き
}

//=============================================================================
// Release
//=============================================================================
void CUI::Uninit(void)
{
	SafeDelete(miniMap);

	SafeRelease(numberManager);

	SafeRelease(reportBg);

	SafeRelease(report);

	SafeRelease(life);

	SafeRelease(loadGauge);

	SafeRelease(loadString);

	player = nullptr;
}

//=============================================================================
// update
//=============================================================================
void CUI::Update(void)
{
	// playerあれば処理
	if (player != nullptr){

		// 装填ゲージ
		const float currentTimer = (float)player[0]->ReloadTimer();
		const float maxTimer = (float)PLAYER_RELOAD_TIME;
		const float rate = currentTimer / maxTimer;

		loadGauge->SetRate(rate);
		if (rate >= 1.0f){
			loadString->DrawDisable();
		}
		if (rate <= 0.0f){
			loadString->DrawEnable();
		}
		for (int cnt = 0;cnt < PLAYER_MAX;cnt++)
		{
			miniMap->SetPlayer(cnt,player[cnt]->Pos(),player[cnt]->Rot().y);

		}

		miniMap->Update();

		// ライフ
		int lifeP = player[0]->PlayerLife();
		life->SetLife(lifeP);
	}

	// 成績表
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

}


// end of file