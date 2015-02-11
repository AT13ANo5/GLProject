//=============================================================================
//
// リザルトのUIを管理するよ [ResultUIManager.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "ResultUIManager.h"
#include "Texture.h"
#include "ResultSheet.h"
#include "ResultNumberManager.h"
#include "ManagerGL.h"

namespace{
  // sheet
  const float   RESULT_SHEET_SCALE = 1.0f;
  const VECTOR2 RESULT_SHEET_SIZE = VECTOR2(SCREEN_WIDTH * RESULT_SHEET_SCALE, 256.0f * RESULT_SHEET_SCALE);
  const VECTOR3 RESULT_SHEET_POS = VECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 200.0f, 0);

  // name
  const float   NAME_SCALE = 1.2f;
  const VECTOR2 NAME_SIZE = VECTOR2(193 * NAME_SCALE, 32 * NAME_SCALE);
  const float   NAME_POS_X_SUB = 170.0f;

  // texture
  const int NAME_TEX[CResultNumberManager::NUMBER_MAX] = {
    TEX_PLAYER1, TEX_PLAYER2, TEX_PLAYER3, TEX_PLAYER4, TEX_PLAYER5, TEX_PLAYER6
  };
}

//=============================================================================
// constructor
//=============================================================================
CResultUIManager::CResultUIManager() :CObject(LAYER_NUM - 2)
{
  ResultSheet = nullptr;
  ResultNumber = nullptr;
  memset(PlayerName, NULL, sizeof(PlayerName));
}

//=============================================================================
// create
//=============================================================================
CResultUIManager* CResultUIManager::Create(void)
{
  CResultUIManager* Scene = new CResultUIManager;
  Scene->Init();

  return Scene;
}

//=============================================================================
// init
//=============================================================================
void CResultUIManager::Init(void)
{
  // 成績表
  ResultSheet = CResultSheet::Create(RESULT_SHEET_POS, RESULT_SHEET_SIZE);
  ResultSheet->SetTex(CTexture::Texture(TEX_RESULT_TEXT));
  ResultSheet->SetAlpha(0.0f);

  // 成績表の番号
  ResultNumber = CResultNumberManager::Create();

  int* ranking = CManager::getRanking();

  // プレイヤー名
  for (int player = 0; player < PLAYER_MAX; ++player){
    VECTOR3 pos = ResultNumber->GetNumPosition((CResultNumberManager::TYPE)player);
    pos.x -= NAME_POS_X_SUB;
    PlayerName[player] = CResultSheet::Create(pos, NAME_SIZE);
    PlayerName[player]->SetTex(CTexture::Texture(NAME_TEX[ranking[player]]));


	ResultNumber->SetNumber((CResultNumberManager::TYPE)player,
		CManager::userInfo[ranking[player]].kill,
		CManager::userInfo[ranking[player]].death);
  }
}

//=============================================================================
// uninit
//=============================================================================
void CResultUIManager::Uninit(void)
{
	delete this;
}

//=============================================================================
// update
//=============================================================================
void CResultUIManager::Update(void)
{
}

//=============================================================================
// draw
//=============================================================================
void CResultUIManager::Draw(void)
{
}

//=============================================================================
// SetResultSheetDisable
//-----------------------------------------------------------------------------
// 成績表を良い感じに消す
//=============================================================================
void CResultUIManager::SetResultSheetDisable(void)
{
  ResultSheet->DrawDisable();
}

//=============================================================================
// SetResultNumberDisable
//-----------------------------------------------------------------------------
// キルデスを良い感じに消す
//=============================================================================
void CResultUIManager::SetResultNumberDisable(void)
{
  ResultNumber->DrawDisable();
}

//=============================================================================
// SetResultPlayerNameDisable
//-----------------------------------------------------------------------------
// プレイヤー名を良い感じに消す
//=============================================================================
void CResultUIManager::SetResultPlayerNameDisable(void)
{
  for (int player = 0; player < PLAYER_MAX; ++player){
    PlayerName[player]->DrawDisable();
  }
}

//=============================================================================
// SetNumber
//-----------------------------------------------------------------------------
//  type   :  タイプ
//  kill   :  KILL数
//  DEATH  :  DEATH数
//=============================================================================
void CResultUIManager::SetNumber(CResultNumberManager::TYPE type, int kill, int death)
{
  ResultNumber->SetNumber(type, kill, death);
}


// end of file