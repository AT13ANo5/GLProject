//=============================================================================
//2Dライフクラス
//	Kikushima
//	Masato Masuda
//=============================================================================
#pragma once
#ifndef _TIME_H_
#define _TIME_H_
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
//定数
//=============================================================================
static const int MAX_DIGIT = 3;


//=============================================================================
//クラス定義
//=============================================================================
class CNumber2D;

class CTime :public CPolygon2D
{
public:
	CTime();
	~CTime(){}
	void Init(void);
	void Uninit(void);
	void Update(void);

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：大きさ
	//	VECTOR3：回転
	//	COLOR：	頂点カラー
	//=============================================================================
	static CTime* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//=============================================================================
	// colorの設定
	void SetColor(const COLOR col);

	// タイム取得
	int GetTime(void){ return Timer; }

	// 
	void SetUpdateFlag(bool flag){ updateFlag = flag; }

private:

	CNumber2D* Nomber[MAX_DIGIT];
	int TimeCount;
	int Timer;
	VECTOR3 Pos;
	VECTOR2 Size;
	bool updateFlag;
};

#endif

// EOF