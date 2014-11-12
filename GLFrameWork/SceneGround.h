#ifndef _SCENE_GROUND_H_
#define _SCENE_GROUND_H_
#include "main.h"
#include "Object.h"

class CMeshGround :public CObject
{

public:
	CMeshGround(int priority = 0);
	~CMeshGround();
	static CMeshGround* Create(VECTOR3 pos,VECTOR2 PanelSize,VECTOR2 PanelNum);

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	float GetHeight(VECTOR3 pos,VECTOR3* normal = nullptr);


private:

	VECTOR3 _Size;
	
	VECTOR2 PanelSize;
	VECTOR2 PanelNum;

	int IndexNum;
	int PolygonNum;
	int VertexNum;
	unsigned int Texture;

	VECTOR3* Vtx;
	VECTOR2* Tex;
	VECTOR3* Nor;
	VECTOR3* NormalMap;
	float* HeightMap;
	int* Index;
	int MapNum;

	void GetPanelIndex(VECTOR3 pos,int* OutIndexX,int* OutIndexY);
	float GetHeightPolygon(const VECTOR3& p0,const VECTOR3& p1,const VECTOR3& p2,VECTOR3& pos,VECTOR3* Normal = nullptr);
};


#endif