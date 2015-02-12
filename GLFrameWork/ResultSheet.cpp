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
  drawFlag = true;
	alphaMax = 1.0f;
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
  if (drawFlag == true){
		if (Color().a < alphaMax){
			float alpha = Color().a + ALPHA_MOVE_SPEED;
			if (alpha >= alphaMax){
				alpha = alphaMax;
			}
			_Color.a = alpha;
		}
	}else {
		if (Color().a > 0.0f){
			float alpha = Color().a - ALPHA_MOVE_SPEED;
			_Color.a = alpha;
		}
	}
}

//=============================================================================
// SetColor
//-----------------------------------------------------------------------------
// col	:	êF
//=============================================================================
void CResultSheet::SetColor(COLOR col)
{
	_Color = col;
}


// end of file