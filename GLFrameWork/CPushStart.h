//=============================================================================
//2Dエフェクトクラス
//=============================================================================
#ifndef _CPUSH_START_H_
#define _CPUSH_START_H_
//=============================================================================
//インクルード
//=============================================================================
#include "main.h"
#include "Object.h"

//クラス定義
class CPushStart :public CObject
{
public:
	CPushStart();
	virtual ~CPushStart(){}
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//=============================================================================
	//生成
	//引数：
	//	VECTOR3：中心座標
	//	VECTOR2：大きさ
	//	VECTOR3：回転
	//	COLOR：頂点カラー
	//
	//XY平面で生成される
	//=============================================================================
	static CPushStart* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot = VECTOR3(0,0,0),const COLOR& color = COLOR(1.0f,1.0f,1.0f,1.0f));

	//=============================================================================
	//セッター
	//=============================================================================
	void SetSize(float x,float y){ _Size.x = x;_Size.y = y; Resize();}
	void SetSize(const VECTOR2& pos){ _Size = pos; Resize(); }
	void SetSizeX(float x){ _Size.x = x; Resize();}
	void SetSizeY(float y){ _Size.y = y; Resize();}

	void SetTex(TEX_INFO tex){ Texture = tex;uv.InverseH = tex.InverseH;uv.InverseV = tex.InverseV; }

	//=============================================================================
	//加算
	//=============================================================================
	void AddSize(float x,float y){ _Size.x += x;_Size.y += y; Resize(); }
	void AddSize(const VECTOR2& pos){ _Size += pos; Resize(); }
	void AddSizeX(float x){ _Size.x += x; Resize();}
	void AddSizeY(float y){ _Size.y += y; Resize();}

	//=============================================================================
	//ゲッター
	//=============================================================================
	VECTOR2 Size(void)const{ return _Size; }

private:

	VECTOR2 _Size;
	VECTOR3 Vtx[4];
	UV uv;
	float Length;
	float Angle;

	float moveAlpha;

	void Resize(void);

};

#endif