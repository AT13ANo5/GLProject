//=================================================================================
//オブジェクトクラス
//
//大部分のクラスの元になるクラス
//
//ゲッターが存在するメンバ変数は、変数名の前にアンダーバーが付加している
//ゲッターはGet〇〇では無く、メンバ変数名のアンダーバーをとって、後ろに()をつける
//=================================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
//=================================================================================
//インクルード
//=================================================================================
#include "main.h"
#include "Texture.h"

//=================================================================================
//マクロ定義
//=================================================================================
#define LAYER_NUM (8)

//クラス定義
class CObject
{
public:
	CObject(int Prioriy=3);
	virtual ~CObject();
	virtual void Init(void){};
	virtual void Uninit(void){};
	virtual void Update(void)=0;
	virtual void Draw(void)=0;
	void Release(void);

	static void Clear(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);

	//=================================================================================
	//セッター
	//=================================================================================
	//Position
	virtual void SetPos(float x,float y,float z){ _Pos.x = x;_Pos.y = y;_Pos.z = z; }
	virtual void SetPos(const VECTOR3& pos){ _Pos = pos; }
	virtual void SetPosX(float x){ _Pos.x = x; }
	virtual void SetPosY(float y){ _Pos.y = y; }
	virtual void SetPosZ(float z){ _Pos.z = z; }

	//Rotation
	virtual void SetRot(float x,float y,float z){ _Rot.x = x;_Rot.y = y;_Rot.z = z; }
	virtual void SetRot(const VECTOR3& rot){ _Rot = rot; }
	virtual void SetRotX(float x){ _Rot.x = x; }
	virtual void SetRotY(float y){ _Rot.y = y; }
	virtual void SetRotZ(float z){ _Rot.z = z; }

 //Position
 virtual void SetDestPos(float x,float y,float z){ _DestPos.x = x;_DestPos.y = y;_DestPos.z = z; }
 virtual void SetDestPos(const VECTOR3& pos){ _DestPos = pos; }
 virtual void SetDestPosX(float x){ _DestPos.x = x; }
 virtual void SetDestPosY(float y){ _DestPos.y = y; }
 virtual void SetDestPosZ(float z){ _DestPos.z = z; }

 //Rotation
 virtual void SetDestRot(float x,float y,float z){ _DestRot.x = x;_DestRot.y = y;_DestRot.z = z; }
 virtual void SetDestRot(const VECTOR3& rot){ _DestRot = rot; }
 virtual void SetDestRotX(float x){ _DestRot.x = x; }
 virtual void SetDestRotY(float y){ _DestRot.y = y; }
 virtual void SetDestRotZ(float z){ _DestRot.z = z; }


	//Scale
	virtual void SetScl(float x,float y,float z){ _Scl.x = x;_Scl.y = y;_Scl.z = z; }
	virtual void SetScl(const VECTOR3& Scl){ _Scl = Scl; }
	virtual void SetSclX(float x){ _Scl.x = x; }
	virtual void SetSclY(float y){ _Scl.y = y; }
	virtual void SetSclZ(float z){ _Scl.z = z; }

	//Color
	virtual void SetColor(const COLOR& color){ _Color = color; }
	virtual void SetRed(float r){_Color.r = r;}
	virtual void SetBlue(float b){_Color.b = b;}
	virtual void SetGreen(float g){_Color.g = g;}
	virtual void SetAlpha(float a){_Color.a = a;}

	//テクスチャ
	virtual void SetTex(TEX_INFO tex){Texture = tex;}

	//クォータニオン
	void SetAxisRotation(const VECTOR3& axis)		{ VectorAxisRotation = axis; }		// 回転軸
	void SetRotationAxis(float rotation)			{ RotationAxis = rotation; }		// 回転量

	//=================================================================================
	//加算
	//=================================================================================
	//Position
	virtual void AddPos(float x,float y,float z){ _Pos.x += x;_Pos.y += y;_Pos.z += z; }
	virtual void AddPos(const VECTOR3& pos){ _Pos += pos; }
	virtual void AddPosX(float x){ _Pos.x += x; }
	virtual void AddPosY(float y){ _Pos.y += y; }
	virtual void AddPosZ(float z){ _Pos.z += z; }

	//Rotation
	virtual void AddRot(float x,float y,float z){ _Rot.x += x;_Rot.y += y;_Rot.z += z; }
	virtual void AddRot(const VECTOR3& rot){ _Rot += rot; }
	virtual void AddRotX(float x){ _Rot.x += x; }
	virtual void AddRotY(float y){ _Rot.y += y; }
	virtual void AddRotZ(float z){ _Rot.z += z; }

 //Position
 virtual void AddDestPos(float x,float y,float z){ _DestPos.x += x;_DestPos.y += y;_DestPos.z += z; }
 virtual void AddDestPos(const VECTOR3& pos){ _DestPos += pos; }
 virtual void AddDestPosX(float x){ _DestPos.x += x; }
 virtual void AddDestPosY(float y){ _DestPos.y += y; }
 virtual void AddDestPosZ(float z){ _DestPos.z += z; }

 //Rotation
 virtual void AddDestRot(float x,float y,float z){ _Rot.x += x;_Rot.y += y;_Rot.z += z; }
 virtual void AddDestRot(const VECTOR3& rot){ _Rot += rot; }
 virtual void AddDestRotX(float x){ _Rot.x += x; }
 virtual void AddDestRotY(float y){ _Rot.y += y; }
 virtual void AddDestRotZ(float z){ _Rot.z += z; }

	//Scale
	virtual void AddScl(float x,float y,float z){ _Scl.x += x;_Scl.y += y;_Scl.z += z; }
	virtual void AddScl(const VECTOR3& Scl){ _Scl = Scl; }
	virtual void AddSclX(float x){ _Scl.x += x; }
	virtual void AddSclY(float y){ _Scl.y += y; }
	virtual void AddSclZ(float z){ _Scl.z += z; }

	//Color
	virtual void AddColor(const COLOR& color){ _Color += color; }
	virtual void AddRed(float r){ _Color.r += r; }
	virtual void AddBlue(float b){ _Color.b += b; }
	virtual void AddGreen(float g){ _Color.g += g; }
	virtual void AddAlpha(float a){ _Color.a += a; }

	//=================================================================================
	//ゲッター
	//=================================================================================
	virtual VECTOR3 Pos(void)const{ return _Pos; }
 virtual VECTOR3 Rot(void)const{ return _Rot; }
 virtual VECTOR3 DestPos(void)const{ return _DestRot; }
 virtual VECTOR3 DestRot(void)const{ return _DestRot; }
	virtual VECTOR3 Scl(void)const{ return _Scl; }
	virtual COLOR Color(void)const{return _Color;}
	//=================================================================================

protected:

	int Priority;
	VECTOR3 _Pos;
	VECTOR3 _Rot;
	VECTOR3 _Scl;
	COLOR _Color;
	TEX_INFO Texture;
 VECTOR3 _DestPos;
 VECTOR3 _DestRot;

	VECTOR3	VectorAxisRotation;		// クォータニオン回転軸
	float	RotationAxis;			// 回転角度

private:
	void LinkList();
	void UnlinkList();

	static CObject* Top[LAYER_NUM];
	static CObject* Cur[LAYER_NUM];

	CObject* Next;
	CObject* Prev;

	bool DeathFlag;

};

#endif