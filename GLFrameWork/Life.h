//=============================================================================
//2Dライフクラス
//Kikushima
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//クラス定義
class CLife :public CPolygon2D
{
public:

  static const int LIFE_MAX = 3;

	CLife();
	~CLife(){}
	void Init(void);
//	void Uninit(void);
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
	//ゲッター
	//=============================================================================
	int GetLife(void){ return LifePoint; }//現在のライフ取得
  int SetLife(int life){ LifePoint = life; }

private:

  int LifePoint;//現在のライフ
  CPolygon2D* Life[LIFE_MAX];

};

#endif