//=============================================================================
//
// ‘•“U’†‚Ì•¶Žš [LoadString.cpp]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "LoadString.h"
#include "Texture.h"

//=============================================================================
// macro
//=============================================================================
namespace{
  const float ALPHA_MOVE_SPEED = 0.025f;
}


//=============================================================================
// constructor
//=============================================================================
CLoadString::CLoadString() :CPolygon2D()
{
  drawFlag  = true;
  alphaMode = true;
}

//=============================================================================
// create
//=============================================================================
CLoadString* CLoadString::Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot,const COLOR& color)
{
  CLoadString* Scene = new CLoadString;
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
void CLoadString::Init(void)
{
  Vtx[0] = VECTOR3(_Size.x, 0, 0);
  Vtx[1] = VECTOR3(0, 0, 0);
  Vtx[2] = VECTOR3(_Size.x, _Size.y, 0);
  Vtx[3] = VECTOR3(0, _Size.y, 0);
}

//=============================================================================
// update
//=============================================================================
void CLoadString::Update(void)
{
  if (drawFlag == true){
    if (alphaMode == true){
      _Color.a += ALPHA_MOVE_SPEED;
      if (_Color.a >= 1.0f){
        alphaMode = false;
      }
    } else {
      _Color.a -= ALPHA_MOVE_SPEED;
      if (_Color.a <= 0.0f){
        alphaMode = true;
      }
    }
  }


  //  if (_Color.a <= 1.0f){
  //    _Color.a += ALPHA_MOVE_SPEED;
  //    if (_Color.a >= 0.0f){

  //    }
  //  }
  //  else if (_Color.a >= 0.0f){
  //    _Color.a -= ALPHA_MOVE_SPEED;
  //  }
}

//=============================================================================
// drawEnable
//=============================================================================
void CLoadString::DrawEnable(void)
{
  drawFlag = true;
  alphaMode = false;
  _Color.a = 1.0f;
}

//=============================================================================
// drawDisable
//=============================================================================
void CLoadString::DrawDisable(void)
{
  drawFlag = false;
  _Color.a = 0.0f;
}

// end of file