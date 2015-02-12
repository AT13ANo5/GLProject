//=============================================================================
//
// Map2D [ Map2D.h]
//
// create : masato masuda
//
//=============================================================================

//=============================================================================
// include Guard
//=============================================================================
#ifndef _CMAP_2D_H_
#define _CMAP_2D_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CMap2D :public CPolygon2D
{
public:

	CMap2D();
	~CMap2D(){}
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTex2(TEX_INFO tex){ texBg = tex; uvStencil.InverseH = tex.InverseH; uvStencil.InverseV = tex.InverseV; }

	//=============================================================================
	// �쐬
	// �����F
	//	VECTOR3�F���S���W
	//	VECTOR2�F�傫��
	//	VECTOR3�F��]
	//	COLOR�F	���_�J���[
	//=============================================================================
	static CMap2D* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot=VECTOR3(0,0,0),const COLOR& color=COLOR(1.0f,1.0f,1.0f,1.0f));

private:

	TEX_INFO texBg;

	VECTOR2 _SizeStencil;
	VECTOR3 VtxStencil[4];
	UV uvStencil;
	MATERIAL MaterialStencil;

};

#endif


// end of file