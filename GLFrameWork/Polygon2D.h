//=============================================================================
//2Dポリゴンクラス
//=============================================================================
#ifndef _POLYGON_2D_H_
#define _POLYGON_2D_H_
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Object.h"

//クラス定義
class CPolygon2D:public CObject
{
public:
	CPolygon2D();
	virtual ~CPolygon2D(){}
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//=============================================================================
	//作成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：大きさ
	//	VECTOR3：回転
	//	COLOR：	頂点カラー
	//=============================================================================
	static CPolygon2D* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot=VECTOR3(0,0,0),const COLOR& color=COLOR(1.0f,1.0f,1.0f,1.0f));

	//=============================================================================
	//セッター
	//=============================================================================
	void SetSize(float x,float y){ _Size.x = x;_Size.y = y;Resize();}
	void SetSize(const VECTOR2& pos){ _Size = pos; Resize(); }
	void SetSizeX(float x){ _Size.x = x; Resize();}
	void SetSizeY(float y){ _Size.y = y; Resize();}

	void SetUV(float x,float y,float width,float height);
	void SetUV_X(float x){ uv.x = x;uv.Set(); }
	void SetUV_Y(float y){ uv.y = y;uv.Set(); }
	void SetUV_Width(float width){ uv.width = width;uv.Set(); }
	void SetUV_Height(float height){ uv.height = height;uv.Set(); }
	void SetTex(TEX_INFO tex){ Texture = tex;uv.InverseH = tex.InverseH;uv.InverseV = tex.InverseV; }

	//=============================================================================
	//加算
	//=============================================================================
	void AddSize(float x,float y){ _Size.x += x;_Size.y += y;Resize(); }
	void AddSize(const VECTOR2& pos){ _Size += pos; Resize(); }
	void AddSizeX(float x){ _Size.x += x; Resize();}
	void AddSizeY(float y){ _Size.y += y; Resize();}

	void AddUV_X(float x){ uv.x += x;uv.Set(); }
	void AddUV_Y(float y){ uv.y += y;uv.Set(); }
	void AddUV_Width(float width){ uv.width += width;uv.Set(); }
	void AddUV_Height(float height){ uv.height += height;uv.Set(); }

	//=============================================================================
	//ゲッター
	//=============================================================================
	VECTOR2 Size(void)const{ return _Size; }
	UV Uv(void)const{ return uv; }

private:

	VECTOR2 _Size;
	VECTOR3 Vtx[4];
	UV uv;

	void Resize(void);

};

#endif