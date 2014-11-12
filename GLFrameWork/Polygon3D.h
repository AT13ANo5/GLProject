#ifndef _POLYGON_3D_H_
#define _POLYGON_3D_H_
#include "main.h"
#include "Object.h"


//=============================================================================
//OpenGLのシーン3Dクラス
//=============================================================================
class CPolygon3D:public CObject
{
public:
	CPolygon3D();
	~CPolygon3D(){}
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPolygon3D* Create(VECTOR3 pos,VECTOR3 size,VECTOR3 rot=VECTOR3(0,0,0),COLOR color=COLOR(1.0f,1.0f,1.0f,1.0f));

	void SetSize(float x,float y,float z){ _Size.x = x;_Size.y = y;_Size.z = z; }
	void SetSize(const VECTOR3& pos){ _Size = pos; }
	void SetSizeX(float x){ _Size.x = x; }
	void SetSizeY(float y){ _Size.y = y; }
	void SetSizeZ(float z){ _Size.z = z; }
	void SetUV(float x,float y,float width,float height);
	void SetUV_X(float x){ uv.x = x;uv.Set(); }
	void SetUV_Y(float y){ uv.y = y;uv.Set(); }
	void SetUV_Width(float width){ uv.width = width;uv.Set(); }
	void SetUV_Height(float height){ uv.height = height;uv.Set(); }

	void SetTex(TEX_INFO tex){ Texture = tex;uv.InverseH = tex.InverseH;uv.InverseV = tex.InverseV; }

	void AddSize(float x,float y,float z){ _Size.x += x;_Size.y += y;_Size.z += z; }
	void AddSize(const VECTOR3& pos){ _Size += pos; }
	void AddSizeX(float x){ _Size.x += x; }
	void AddSizeY(float y){ _Size.y += y; }
	void AddSizeZ(float z){ _Size.z += z; }
	void AddUV_X(float x){ uv.x += x;uv.Set(); }
	void AddUV_Y(float y){ uv.y += y;uv.Set(); }
	void AddUV_Width(float width){ uv.width += width;uv.Set(); }
	void AddUV_Height(float height){ uv.height += height;uv.Set(); }

	VECTOR3 Size(void)const{ return _Size; }

private:

	VECTOR3 _Size;
	UV uv;
	VECTOR3 Vtx[4];
	VECTOR3 Nor[4];
	MATERIAL Material;

};

#endif