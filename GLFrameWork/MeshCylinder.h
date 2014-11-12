#ifndef _MESH_CYLINDER_
#define _MESH_CYLINDER_
#include "Object.h"

class CMeshCylinder:public CObject
{
public:
	CMeshCylinder(int priority=0);
	~CMeshCylinder();
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

	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	int* Index;

};
#endif