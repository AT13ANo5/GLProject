//=============================================================================
//ライトクラス
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_
//=============================================================================
//インクルード
//=============================================================================
#include "CommonGL.h"
//=============================================================================
//クラス宣言
//=============================================================================
class CLight
{
public:
	CLight();
	~CLight(){}
	void Create(VECTOR4 Vec,COLOR Dif= COLOR(1.0f,1.0f,1.0f,1.0f),COLOR Amb= COLOR(0.3f,0.3f,0.3f,1.0f),COLOR Speq=COLOR(1.0f,1.0f,1.0f,1.0f));
	void Update(void);

	void SetPos(VECTOR4 vec){Pos=vec;}
	void SetDiffuse(COLOR color){Diffuse=color;}
	void SetAmbient(COLOR color){Ambient=color;}
	void SetSpecular(COLOR color){Specular=color;}
private:
	VECTOR4 Pos;
	COLOR Diffuse;
	COLOR Ambient;
	COLOR Specular;
};

#endif