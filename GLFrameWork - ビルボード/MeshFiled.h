//=============================================================================
//フィールドクラス
//=============================================================================
#ifndef _SCENE_FIELD_H_
#define _SCENE_FIELD_H_

//=============================================================================
//インクルード
//=============================================================================
#include "Object.h"

//クラス定義
class CMeshField:CObject
{
public:
	CMeshField(int priority=0);
	~CMeshField();

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：ポリゴン一枚のサイズ
	//	VECTOR2：分割数
	//=============================================================================
	static CMeshField* Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum);

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:

	VECTOR2 PanelSize;
	VECTOR2 PanelNum;

	int IndexNum;
	int PolygonNum;
	int VertexNum;

	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	int* Index;
};

#endif