//=============================================================================
//2Dライフクラス
//	Kikushima
//	Masato Masuda
//=============================================================================

//=============================================================================
//インクルード
//=============================================================================
#include "Polygon2D.h"
#include <math.h>
#include "Texture.h"
#include "Time.h"
#include "Number2D.h"

//=============================================================================
// マクロ
//=============================================================================
namespace{
	const int MAX_TIME = 180;
	const int LIMIT_TIME = 10;
	const COLOR COLOR_NORMAL = COLOR(0.2f, 0.8f, 0.2f, 1.0f);
	const COLOR COLOR_HALF = COLOR(0.85f, 0.7f, 0.2f, 1.0f);
	const COLOR COLOR_LIMIT = COLOR(0.8f, 0.2f, 0.2f, 1.0f);
}


//CNumber2D* Nomber[MAX_DIGIT] = { nullptr };
//=============================================================================
//コストラクタ
//=============================================================================
CTime::CTime()
{
	_Pos = VECTOR3(0.0f, 0.0f, 0.0f);
	_Rot = VECTOR3(0.0f, 0.0f, 0.0f);

	memset(Nomber, NULL, sizeof(Nomber));
	updateFlag = true;
}
//=============================================================================
//初期化
//=============================================================================
void CTime::Init(void)
{
//	CPolygon2D::Init();

	TimeCount = 0;
	Timer = MAX_TIME;
	updateFlag = true;

	for (int i = 0; i < MAX_DIGIT; i++)
	{
		Nomber[i] = CNumber2D::Create(VECTOR3(Pos.x + (i * (Size.x*0.8f)), Pos.y, Pos.z), Size);
		Nomber[i]->SetTex((CTexture::Texture(TEX_NUMBER)));
		Nomber[i]->SetColor(COLOR_NORMAL);
	}

}
//=============================================================================
//作成
//=============================================================================
CTime* CTime::Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot, const COLOR& color)
{
	CTime* Time = new CTime();
	Time->Pos = pos;
	Time->Size = size;
	Time->Init();

	return Time;
}

//=============================================================================
//終了処理
//=============================================================================
void CTime::Uninit(void)
{
	delete this;
}
//=============================================================================
//更新
//=============================================================================
void CTime::Update(void)
{
	// color
	if (Timer == LIMIT_TIME){
		SetColor(COLOR_LIMIT);
	}else if (Timer == (MAX_TIME / 2)){
		SetColor(COLOR_HALF);
	}

	/*if (updateFlag == true){
		TimeCount++;
	}

	if (TimeCount == 60)
	{
		Timer--;
		//制限時間経過
		if (Timer <= 0)
		{
			//終了
			Timer = 0;
		}
		TimeCount = 0;
	}*/
	Nomber[0]->SetNumber(Timer/100);

	Nomber[1]->SetNumber(Timer/10);

	Nomber[2]->SetNumber(Timer);
}

//=============================================================================
// SetColor
//-----------------------------------------------------------------------------
// 色指定すると全部の数字の色かえるよ
//=============================================================================
void CTime::SetColor(const COLOR col)
{
	for (int i = 0; i < MAX_DIGIT; ++i){
		Nomber[i]->SetColor(col);
	}
}

// EOF