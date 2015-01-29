//=============================================================================
//
// 2Dライフクラス [Life.cpp]
//
// Auther : Kikushima
//          Masato Masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "Polygon2D.h"
#include "Texture.h"
#include "Life.h"

//=============================================================================
// constructor
//=============================================================================
CLife::CLife()
{
  memset(Life, NULL, sizeof(Life));
  LifePoint = LIFE_MAX;

}

//=============================================================================
// init
//=============================================================================
void CLife::Init(void)
{
  CPolygon2D::Init();

  const VECTOR2 SIZE = VECTOR2(100.0f, 100.0f);

  for (int i = 0; i < LIFE_MAX; i++)
  {
    const VECTOR3 pos = VECTOR3((SIZE.x / 2) + SIZE.x*i, SIZE.y/2, 0.0f);
    Life[i] = CPolygon2D::Create(pos, SIZE);
    Life[i]->SetTex(CTexture::Texture(TEX_LIFE));
  }
}

//=============================================================================
// create
//=============================================================================
CLife* CLife::Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot, const COLOR& color)
{
  CLife* life = new CLife();

  life->Init();

  return life;
}

//=============================================================================
// update
//=============================================================================
void CLife::Update(void)
{
  for (int i = 0; i < LIFE_MAX; i++){
    if (i <= LifePoint-1){
      Life[i]->SetAlpha(1.0f);
    } else {
      Life[i]->SetAlpha(0.0f);
    }
  }
}

// end of file