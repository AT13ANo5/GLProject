//=============================================================================
//
// 数字 [CNumber2D.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include guard
//=============================================================================
#ifndef _NUMBER_2D_H_
#define _NUMBER_2D_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CNumber2D :public CPolygon2D
{
public:

  CNumber2D();
  ~CNumber2D(){}

  void Update(void);
  void Draw(void);

  void SetAlpha(float a){ _Color.a = a; }
  // 数値を設定（UV座標いじるお）
  // num : 設定したい値
  void SetNumber(int num);

  //===========================================================================
  // Create
  //---------------------------------------------------------------------------
  //	VECTOR3 ： 中心座標
  //	VECTOR2 ： 大きさ
  //	VECTOR3 ： 回転
  //	COLOR   ： 頂点カラー
  //===========================================================================
  static CNumber2D* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

  // Geter / Seter
  // 描画するかどうか決めるよ
  void SetDrawFlag(bool flag){ drawFlag = flag; }
	bool GetDrawFlag(void) { return drawFlag; }
  void DrawDisable(void);

	// サイズ
	void SubSize(VECTOR2 size);

private:

  int   number;
  bool  drawFlag;
};

#endif

// end of file