//=============================================================================
//シリンダークラス
//=============================================================================
#ifndef _BATTLE_AREA_CYLINDER_
#define _BATTLE_AREA_CYLINDER_
//=============================================================================
//インクルード
//=============================================================================
#include "MeshCylinder.h"

//クラス定義
class CBattleAreaCylinder :public CMeshCylinder
{
public:
	CBattleAreaCylinder(int priority = 0);
	~CBattleAreaCylinder();

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3:底の中心
	//	float:	ポリゴン一枚の高さ
	//	VECTOR2:分割数
	//	float:	半径
	//	VECTOR2:テクスチャのリピート数
	//=============================================================================
	static CBattleAreaCylinder* Create(VECTOR3 pos, float PanelHeight, VECTOR2 PanelNum, float radius, VECTOR2 TexDivide = VECTOR2(3, 1));

	void Init(void);
	void Draw(void);


private:
	COLOR*	Color;		// 頂点色

};
#endif