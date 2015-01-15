//=============================================================================
//
// ‘•“UƒQ[ƒW [LoadGauge.cpp]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "LoadGauge.h"
#include "Texture.h"

//=============================================================================
// macro
//=============================================================================
namespace{
  const COLOR DEFAULT_COLOR = COLOR(1.0f, 1.0f, 1.0f, 1.0f );
  const COLOR LOAD_COLOR = COLOR(1.0f, 0.0f, 0.0f, 1.0f);
}


//=============================================================================
// constructor
//=============================================================================
CLoadGauge::CLoadGauge() :CPolygon2D()
{
  rateCurrent = 1.0f;
  color = DEFAULT_COLOR;
}

//=============================================================================
// create
//=============================================================================
CLoadGauge* CLoadGauge::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CLoadGauge* Scene = new CLoadGauge;
	Scene->_Pos = pos;
	Scene->_Rot = rot;
	Scene->_Size = size;
	Scene->_Color = color;
	Scene->Init();

	return Scene;
}

//=============================================================================
// iinit
//=============================================================================
void CLoadGauge::Init(void)
{
  Vtx[0] = VECTOR3(_Size.x * rateCurrent, 0, 0);
  Vtx[1] = VECTOR3(0, 0, 0);
  Vtx[2] = VECTOR3(_Size.x * rateCurrent, _Size.y, 0);
  Vtx[3] = VECTOR3(0, _Size.y, 0);
}

//=============================================================================
// update
//=============================================================================
void CLoadGauge::Update(void)
{
  if (rateCurrent < 1.0f){
    SetColor(LOAD_COLOR);
  } else {
    SetColor(color);
  }

  Vtx[0] = VECTOR3(_Size.x * rateCurrent, 0, 0);
  Vtx[1] = VECTOR3(0, 0, 0);
  Vtx[2] = VECTOR3(_Size.x * rateCurrent, _Size.y, 0);
  Vtx[3] = VECTOR3(0, _Size.y, 0);
}

void CLoadGauge::SetDefaultColor(COLOR col)
{
  color = col;
  SetColor(color);
}

// end of file