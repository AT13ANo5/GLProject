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
	Size = VECTOR2(833.33f, 166.66f);
	Count = 0;
	UDflag = false;
	Title = CPolygon2D::Create(VECTOR3(SCREEN_WIDTH / 2, 0, 0), VECTOR2(Size.x, Size.y));
	Title->SetTex(CTexture::Texture(TEX_TITLELOGO));
}

void CTitleDirection::Uninit(void)
{
	SafeRelease(Title);
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
	
	if (Title != nullptr)
	{
		Count++;

		if (Count <= 120)
		{
			Title->AddPos(0, 1.5f, 0);
			Title->AddSize(1.5f, 1.5f);
		}
		if (Count >= 120)
		{
			if (UDflag == true)
			{
				Title->AddSize(-1.0f, -1.0f);
			}
			else
			{
				Title->AddSize(1.0f, 1.0f);
			}

			if (Count >= 180)
			{
				Count = 120;
				UDflag = !UDflag;
			}
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