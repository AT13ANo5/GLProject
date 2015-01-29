//=============================================================================
//
// êîéöÇä«óùÇ∑ÇÈÇÊ [CResultNumberManager.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "ResultNumberManager.h"
#include "Texture.h"
#include "Number2D.h"
#include <math.h>


//=============================================================================
// macro
//=============================================================================

namespace{
  const float ALPHA_MOVE_SPEED = 0.05f;

  const float   NUMBER_SCAL = 0.33f;
  const VECTOR2 NUMBER_SIZE = VECTOR2(128.0f*0.3f, 100.0f*NUMBER_SCAL); // 128 100
  const VECTOR3 PLAYER_1_POS = VECTOR3(435.0f, 570.0f, 0.0f);
  const VECTOR2 DEATH_POS_ADD = VECTOR2(90.0f, NUMBER_SIZE.y + 13.0f);
  const float   RIGHT_POS_ADD = 630.0f;

  const VECTOR3 NUMBER_POS[CResultNumberManager::NUMBER_MAX] = {
    VECTOR3(PLAYER_1_POS.x, PLAYER_1_POS.y, 0.0f ),
    VECTOR3(PLAYER_1_POS.x, PLAYER_1_POS.y + DEATH_POS_ADD.y * 1, 0.0f),
    VECTOR3(PLAYER_1_POS.x, PLAYER_1_POS.y + DEATH_POS_ADD.y * 2, 0.0f),
    VECTOR3(PLAYER_1_POS.x + RIGHT_POS_ADD, PLAYER_1_POS.y, 0.0f),
    VECTOR3(PLAYER_1_POS.x + RIGHT_POS_ADD, PLAYER_1_POS.y + DEATH_POS_ADD.y * 1, 0.0f),
    VECTOR3(PLAYER_1_POS.x + RIGHT_POS_ADD, PLAYER_1_POS.y + DEATH_POS_ADD.y * 2, 0.0f)
  };
}


//=============================================================================
// constructor
//=============================================================================
CResultNumberManager::CResultNumberManager() :CPolygon2D()
{
  drawFlag = true;
  _Color = COLOR(1.0f, 1.0f, 1.0f, 0.0f);
  memset(number, NULL, sizeof(number));
}

//=============================================================================
// create
//=============================================================================
CResultNumberManager* CResultNumberManager::Create(void)
{
  CResultNumberManager* Scene = new CResultNumberManager;

  Scene->Init();

  return Scene;
}

//=============================================================================
// update
//=============================================================================
void CResultNumberManager::Init(void)
{
  // numberêî
  for (int num = 0; num < NUMBER_MAX; ++num){
    // åÖêîï™
    for (int figure = 0; figure < FIGURE_MAX; ++figure){

      VECTOR3 pos = NUMBER_POS[ num ];
      VECTOR3 deathPos = VECTOR3(pos.x + DEATH_POS_ADD.x, pos.y, 0.0f);
      const float numWidthScale = 0.8f;
      number[num]._kill[figure] = CNumber2D::Create(VECTOR3(pos.x + (figure*NUMBER_SIZE.x * numWidthScale), pos.y, pos.z), NUMBER_SIZE);
      number[num]._death[figure] = CNumber2D::Create(VECTOR3(deathPos.x + (figure*NUMBER_SIZE.x * numWidthScale), pos.y, pos.z), NUMBER_SIZE);
      number[num]._kill[figure]->SetTex(CTexture::Texture(TEX_NUMBER));
      number[num]._death[figure]->SetTex(CTexture::Texture(TEX_NUMBER));
      number[num]._kill[figure]->SetNumber(0);
      number[num]._death[figure]->SetNumber(0);
    }
    SetNumber((TYPE)num, num * 10, num * 10);
  }
}

//=============================================================================
// update
//=============================================================================
void CResultNumberManager::Update(void)
{
  if (drawFlag == true){
    if (Color().a < 1.0f){
      float alpha = Color().a + ALPHA_MOVE_SPEED;
      _Color.a = alpha;

      for (int num = 0; num < NUMBER_MAX; ++num){
        for (int figure = 0; figure < FIGURE_MAX; ++figure){
          number[num]._death[figure]->SetAlpha( alpha );
          number[num]._kill[figure]->SetAlpha(alpha);
        }
      } // for
    }
  }
  else {
    if (Color().a > 0.0f){
      float alpha = Color().a - ALPHA_MOVE_SPEED;
      _Color.a = alpha;

      for (int num = 0; num < NUMBER_MAX; ++num){
        for (int figure = 0; figure < FIGURE_MAX; ++figure){
          number[num]._death[figure]->SetAlpha(alpha);
          number[num]._kill[figure]->SetAlpha(alpha);
        }
      } // for
    }
  }
}

//=============================================================================
// SetNumberKill
//=============================================================================
void CResultNumberManager::SetNumberKill(TYPE type, int kill)
{
  int numCheck[FIGURE_MAX] = { 0 };

  for (int figure = 0; figure < FIGURE_MAX; ++figure){

    int curNum = ((int)kill) % (int)pow(10.0f, FIGURE_MAX - figure) / (int)pow(10.0f, FIGURE_MAX - figure - 1);
    numCheck[figure] = curNum;
    number[type]._kill[figure]->SetNumber(curNum);
    number[type]._kill[figure]->SetDrawFlag(true);
  }

  if (numCheck[0] == 0){
    number[type]._kill[0]->SetDrawFlag(false);
  }
}

//=============================================================================
// SetNumberDeath
//=============================================================================
void CResultNumberManager::SetNumberDeath(TYPE type, int death)
{
  int numCheck[FIGURE_MAX] = { 0 };

  for (int figure = 0; figure < FIGURE_MAX; ++figure){

    int curNum = ((int)death) % (int)pow(10.0f, FIGURE_MAX - figure) / (int)pow(10.0f, FIGURE_MAX - figure - 1);
    numCheck[figure] = curNum;
    number[type]._death[figure]->SetNumber(curNum);
    number[type]._death[figure]->SetDrawFlag(true);
  }

  if (numCheck[0] == 0){
    number[type]._death[0]->SetDrawFlag(false);
  }
}

//=============================================================================
// SetNumber
//=============================================================================
void CResultNumberManager::SetNumber(TYPE type, int kill, int death)
{
  SetNumberKill(type, kill);
  SetNumberDeath(type, death);
}

//=============================================================================
// SetDrawFlag
//-----------------------------------------------------------------------------
// ï`âÊON/OFF
//=============================================================================
void CResultNumberManager::SetDrawFlag(bool flag)
{
  for (int num = 0; num < NUMBER_MAX; ++num){
    for (int figure = 0; figure < FIGURE_MAX; ++figure){
      number[num]._kill[figure]->SetDrawFlag(flag);
      number[num]._death[figure]->SetDrawFlag(flag);
    }
    if (flag == true){
      SetNumber((TYPE)num, num * 1, num * 10);
    }
  }
}

//=============================================================================
// GetNumPosition
//-----------------------------------------------------------------------------
//=============================================================================
VECTOR3 CResultNumberManager::GetNumPosition(TYPE type)
{
  return NUMBER_POS[(int)type];
}


// end of file