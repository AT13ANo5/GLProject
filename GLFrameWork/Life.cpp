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
void CLife::Init(const VECTOR3& pos, const VECTOR2& size)
{
	CPolygon2D::Init();

	const VECTOR2 SIZE = VECTOR2(100.0f, 100.0f);

	for (int i = 0; i < LIFE_MAX; i++)
	{
		const float posAddX = 10.0f;
		const VECTOR3 lifePos = VECTOR3(pos.x + ((size.x / 2) + ((size.x + posAddX) * i)), pos.y + (size.y / 2), 0.0f);
		Life[i] = CPolygon2D::Create(lifePos, size);
		Life[i]->SetTex(CTexture::Texture(TEX_LIFE));
	}
}

//=============================================================================
// create
//=============================================================================
CLife* CLife::Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot, const COLOR& color)
{
  CLife* life = new CLife();

  life->Init(pos, size);

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