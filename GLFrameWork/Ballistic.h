//------------------------------------------------------------------------------
//
// 弾道 [Ballistic.h]
// Author : AT-13A-273 Mai Tanabe
//
//------------------------------------------------------------------------------
#ifndef _Ballistic_H_
#define _Ballistic_H_

//------------------------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------------------------
#define MARK_MAX  (50)

//------------------------------------------------------------------------------
// ヘッダインクルード
//------------------------------------------------------------------------------
#include "CBullet.h"
#include "Polygon3D.h"

//------------------------------------------------------------------------------
// クラス宣言
//------------------------------------------------------------------------------
class CBillboard;

class CBallistic
{
public:
	CBallistic();
	~CBallistic();

	void Init(COLOR color);
	void Update(VECTOR3 pos, VECTOR3 rot);

	static CBallistic* Create(COLOR color);

	CBillboard* GetMark(int no){return BallisticMark[no];}
	CPolygon3D* GetLanding(void){return Landing;}

	void SetLanding(VECTOR3 pos){Landing->SetPos(pos);}
	
	void SetDrawFlag(bool drawFlag)
	{
		for(int i = 0; i < MARK_MAX; i++)
		{
			BallisticMark[i]->SetDrawFlag(drawFlag);
			Landing->SetDrawFlag(drawFlag);
		}
	}

private:
	CBillboard* BallisticMark[MARK_MAX];	// 弾道のマーク
	CPolygon3D* Landing;					// 着弾マーク
	float		LandingAddRot;
};

#endif
//------------------------------------------------------------------------------
// EOF