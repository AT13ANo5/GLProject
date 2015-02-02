//=============================================================================
//シリンダークラス
//=============================================================================
#ifndef _MESH_CYLINDER_
#define _MESH_CYLINDER_
//=============================================================================
//インクルード
//=============================================================================
#include "Object.h"

//クラス定義
class CMeshCylinder:public CObject
{
public:
	CMeshCylinder(int priority=0);
	~CMeshCylinder();

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3:底の中心
	//	float:	ポリゴン一枚の高さ
	//	VECTOR2:分割数
	//	float:	半径
	//	VECTOR2:テクスチャのリピート数
	//=============================================================================
	static CMeshCylinder* Create(VECTOR3 pos,float PanelHeight,VECTOR2 PanelNum,float radius,VECTOR2 TexDivide=VECTOR2(3,1));

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:

	float PanelHeight;
	VECTOR2 PanelNum;
	VECTOR2 TexDiv;
	int IndexNum;
	int PolygonNum;
	int VertexNum;
	float Radius;
	GLuint DrawList;
	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	int* Index;

};
#endif