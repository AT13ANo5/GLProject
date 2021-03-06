//=============================================================================
//
// 2Dライフクラス [Life.h]
//
// Auther : Kikushima
//          Masato Masuda
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CLife :public CPolygon2D
{
public:

  static const int LIFE_MAX = 3;

	CLife();
	~CLife(){}
	void Init(const VECTOR3& pos, const VECTOR2& size);
	void Update(void);
  void Draw(void){}

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：大きさ
	//	VECTOR3：回転
	//	COLOR：	頂点カラー
	//=============================================================================
	static CLife* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//=============================================================================
	//ライフ増減
	//=============================================================================
  void LifeDamage(void){ LifePoint--; if (LifePoint < 0){ LifePoint = 0; } }	//ダメージ
  void LifeReset(void){ LifePoint = LIFE_MAX; }	//復活
	//=============================================================================
	// ライフ取得 設定
	//=============================================================================
	int GetLife(void){ return LifePoint; }//現在のライフ取得
  void SetLife(int life){ LifePoint = life; }

private:

  int LifePoint;//現在のライフ
  CPolygon2D* Life[LIFE_MAX];

};

#endif

// end of file