//=============================================================================
//
// ”š‚ğŠÇ—‚·‚é‚æ [CNumberManager.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "NumberManager.h"
#include "Texture.h"
#include "Number2D.h"
#include <math.h>


//=============================================================================
// macro
//=============================================================================
namespace{
  const float   NUMBER_SCAL = 0.5f;
  const VECTOR2 NUMBER_SIZE = VECTOR2(128.0f*NUMBER_SCAL, 100.0f*NUMBER_SCAL); // 128 100
  const VECTOR3 PLAYER_1_POS = VECTOR3(450.0f + 60.0f, 231.0f, 0.0f);
  const VECTOR2 DEATH_POS_ADD = VECTOR2(290.0f, NUMBER_SIZE.y + 17.0f);
}


//=============================================================================
// constructor
//=============================================================================
CNumberManager::CNumberManager() :CPolygon2D()
{
  drawFlag = true;
  memset(number, NULL, sizeof(number));
}

//=============================================================================
// create
//=============================================================================
CNumberManager* CNumberManager::Create(void)
{
  CNumberManager* Scene = new CNumberManager;

  Scene->Init();

  return Scene;
}

//=============================================================================
// update
//=============================================================================
void CNumberManager::Init(void)
{
  // number”
  for (int num = 0; num < NUMBER_MAX; ++num){
    // Œ…”•ª
    for (int figure = 0; figure < FIGURE_MAX; ++figure){

      VECTOR3 deathPos = VECTOR3(PLAYER_1_POS.x + DEATH_POS_ADD.x, PLAYER_1_POS.y, 0.0f);
      number[num]._kill[figure] = CNumber2D::Create(VECTOR3(PLAYER_1_POS.x + (figure*NUMBER_SIZE.x), PLAYER_1_POS.y + (num*DEATH_POS_ADD.y), PLAYER_1_POS.z), NUMBER_SIZE);
      number[num]._death[figure] = CNumber2D::Create(VECTOR3(deathPos.x + (figure*NUMBER_SIZE.x), PLAYER_1_POS.y + (num*DEATH_POS_ADD.y), PLAYER_1_POS.z), NUMBER_SIZE);
      number[num]._kill[figure]->SetTex(CTexture::Texture(TEX_NUMBER));
      number[num]._death[figure]->SetTex(CTexture::Texture(TEX_NUMBER));
      number[num]._kill[figure]->SetNumber(0);
      number[num]._death[figure]->SetNumber(0);
    }
    SetNumber((TYPE)num, num * 1, num * 10);
  }
}

//=============================================================================
// update
//=============================================================================
void CNumberManager::Update(void)
{

}

//=============================================================================
// SetNumberKill
//=============================================================================
void CNumberManager::SetNumberKill(TYPE type, int kill)
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
void CNumberManager::SetNumberDeath(TYPE type, int death)
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
void CNumberManager::SetNumber(TYPE type, int kill, int death)
{
  SetNumberKill(type, kill);
  SetNumberDeath(type, death);
}

//=============================================================================
// SetDrawFlag
//-----------------------------------------------------------------------------
// •`‰æON/OFF
//=============================================================================
void CNumberManager::SetDrawFlag(bool flag)
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



// end of file