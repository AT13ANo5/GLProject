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
//#include "Model.h"
#include "CPlayer.h"
#include "PlayerCamera.h"
#include "ManagerGL.h"


CPolygon2D* Title = nullptr;
CPolygon2D* Word = nullptr;
CPlayer* Model = nullptr;

//=============================================================================
// macro
//=============================================================================
namespace{
}


//=============================================================================
// constructor
//=============================================================================
CTitleDirection::CTitleDirection() :CPolygon2D()
{
	number = 0;
	drawFlag = true;
	Size = VECTOR2(750.0f,180.0f);
	Count = 0;
	UDflag = false;
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

	Word = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH/2,0,0),VECTOR2(Title->Size.x,Title->Size.y));
	Word->SetTex(CTexture::Texture(TEX_TITLELOGO));

	return Title;
}

//=============================================================================
// update
//=============================================================================
void CTitleDirection::Update(void)
{
	Count++;

	if (Count <= 120)
	{
		Word->AddPos(0, 1.5f, 0);
		Word->AddSize(1.5f, 1.5f);
	}
	if (Count >= 120)
	{
		if (UDflag == true)
		{
			Word->AddSize(-1.0f, -1.0f);

		}
		else
		{
			Word->AddSize(1.0f, 1.0f);
		}
		if (Count >= 240)
		{
			Count = 180;
			UDflag = !UDflag;
		}
	}
}

//=============================================================================
// draw
//=============================================================================
void CTitleDirection::Draw(void)
{

}

// end of file