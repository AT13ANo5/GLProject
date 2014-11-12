#ifndef _SCENE_FIELD_H_
#define _SCENE_FIELD_H_
#include "Object.h"
class CMeshField:CObject
{
public:
	CMeshField(int priority=0);
	~CMeshField();
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