//=============================================================================
//球体クラス(スカイドーム)
//=============================================================================
#ifndef _MESH_SPHERE_
#define _MESH_SPHERE_
//=============================================================================
//インクルード
//=============================================================================
#include "Object.h"

//クラス定義
class CMeshSphere:public CObject
{
public:
	CMeshSphere(int priority=0);
	~CMeshSphere();

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：分割数
	//	float：	半径
	//	VECTOR2：テクスチャのリピート数
	//=============================================================================
	static CMeshSphere* Create(VECTOR3 pos,VECTOR2 PanelNum,float radius,VECTOR2 TexDivide=VECTOR2(3,1));

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	VECTOR2 PanelNum;
	VECTOR2 TexDiv;
	int IndexNum;
	int PolygonNum;
	int VertexNum;
	float Radius;

	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	int* Index;

};
#endif