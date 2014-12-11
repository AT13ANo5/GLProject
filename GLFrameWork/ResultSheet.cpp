//=============================================================================
//
// ê¨ê—ï\ [ResultSheet.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "ResultSheet.h"
#include <math.h>
#include "Texture.h"

//=============================================================================
// macro
//=============================================================================
namespace{
  const float ALPHA_MOVE_SPEED = 0.05f;
}


//=============================================================================
// constructor
//=============================================================================
CResultSheet::CResultSheet() :CPolygon2D()
{
  draw = true;
//  Alpha = 1.0f;
}

//=============================================================================
// create
//=============================================================================
CResultSheet* CResultSheet::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
	CResultSheet* Scene = new CResultSheet;
	Scene->_Pos = pos;
	Scene->_Rot = rot;
	Scene->_Size = size;
	Scene->_Color = color;
	Scene->Init();

	return Scene;
}

//=============================================================================
// update
//=============================================================================
void CResultSheet::Update(void)
{
  if (draw == true){
    if (Color().a < 1.0f){
      float alpha = Color().a + ALPHA_MOVE_SPEED;
      _Color.a = alpha;
    }
  }else {
    if (Color().a > 0.0f){
      float alpha = Color().a - ALPHA_MOVE_SPEED;
      _Color.a = alpha;
    }
  }
}

// end of file