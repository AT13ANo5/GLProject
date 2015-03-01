//=============================================================================
//
// タイトル演出
//
// Auther : Takahiro Kikushima
//
//=============================================================================

//=============================================================================
// include
//=============================================================================
#include "TitleDirection.h"
#include "Texture.h"
#include "CPlayer.h"
#include "PlayerCamera.h"
#include "ManagerGL.h"
#include "math.h"
#include "Explosion.h"
#include "Polygon2D.h"

//=============================================================================
// constructor
//=============================================================================
CTitleDirection::CTitleDirection()// :CPolygon2D()
{

}

void CTitleDirection::Init(void)
{
	Title = nullptr;

	number = 0;
	Size = VECTOR2(700.0f, 200.0f);
	Count = 0;
	UDflag = false;
	Title = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2, 110, 0), VECTOR2(Size.x, Size.y));
	Title->SetTex(CTexture::Texture(TEX_TITLELOGO));
}

void CTitleDirection::Uninit(void)
{
	SafeRelease(Title);
 delete this;
}

//=============================================================================
// create
//=============================================================================
CTitleDirection* CTitleDirection::Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot, const COLOR& color)
{
	CTitleDirection* Title = new CTitleDirection;
	Title->_Size = size;
	Title->_Rot = rot;
	Title->_Size = size;
	Title->_Color = color;
	Title->Init();

	return Title;
}

//=============================================================================
// update
//=============================================================================
void CTitleDirection::Update(void)
{

}

//=============================================================================
// draw
//=============================================================================
void CTitleDirection::Draw(void)
{

}

// end of file