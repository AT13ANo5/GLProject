//=================================================================================
//ビルボードクラス
//
//Rotはzだけ回る
//=================================================================================
#ifndef _CBillboard_H_
#define _CBillboard_H_

//=================================================================================
//インクルード
//=================================================================================
#include "main.h"
#include "Object.h"

//=============================================================================
//OpenGLのシーン3Dクラス
//=============================================================================
class CBillboard :public CObject
{
public:
	CBillboard();
	virtual ~CBillboard(){}
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//=================================================================================
	//生成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：大きさ
	//	VECTOR3：回転
	//	COLOR：頂点カラー
	//
	//XY平面で生成される
	//=================================================================================
	static CBillboard* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot = VECTOR3(0,0,0),const COLOR& color = COLOR(1.0f,1.0f,1.0f,1.0f));

	//=================================================================================
	//セッター
	//=================================================================================
	void SetSize(float x,float y){ _Size.x = x;_Size.y = y; ResizeVtx();}
	void SetSize(const VECTOR2& pos){ _Size = pos; ResizeVtx(); }
	void SetSizeX(float x){ _Size.x = x; ResizeVtx(); }
	void SetSizeY(float y){ _Size.y = y; ResizeVtx(); }

	void SetUV(float x,float y,float width,float height);
	void SetUV_X(float x){ uv.x = x;uv.Set(); }
	void SetUV_Y(float y){ uv.y = y;uv.Set(); }
	void SetUV_Width(float width){ uv.width = width;uv.Set(); }
	void SetUV_Height(float height){ uv.height = height;uv.Set(); }

	void SetTex(TEX_INFO tex){ Texture = tex;uv.InverseH = tex.InverseH;uv.InverseV = tex.InverseV; }

	//=================================================================================
	//加算
	//=================================================================================
	void AddSize(float x,float y){ _Size.x += x;_Size.y += y; ResizeVtx(); }
	void AddSize(const VECTOR2& pos){ _Size += pos; ResizeVtx(); }
	void AddSizeX(float x){ _Size.x += x; ResizeVtx(); }
	void AddSizeY(float y){ _Size.y += y; ResizeVtx(); }

	void AddUV_X(float x){ uv.x += x;uv.Set(); }
	void AddUV_Y(float y){ uv.y += y;uv.Set(); }
	void AddUV_Width(float width){ uv.width += width;uv.Set(); }
	void AddUV_Height(float height){ uv.height += height;uv.Set(); }

	//=================================================================================
	//ゲッター
	//=================================================================================
	VECTOR2 Size(void)const{return _Size;}
	UV Uv(void)const{ return uv; }

	void SetDrawFlag(bool drawFlag){DrawFlag = drawFlag;}	// 描画フラグセッター

private:

	UV uv;
	VECTOR3 Vtx[4];
	VECTOR3 Nor[4];
	MATERIAL Material;

	void ResizeVtx(void);

	bool DrawFlag;											// 描画フラグ（2015/02/16 宗像）

protected:
	VECTOR2 _Size;
};

#endif