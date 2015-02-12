//=============================================================================
//
// ���ѕ\ [ResultSheet.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include guard
//=============================================================================
#ifndef _RESULT_SHEET_H_
#define _RESULT_SHEET_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CResultSheet :public CPolygon2D
{
public:
	CResultSheet();
	~CResultSheet(){}
	void Update(void);

	//===========================================================================
	// Create
	//---------------------------------------------------------------------------
	//	VECTOR3 �F ���S���W
	//	VECTOR2 �F �傫��
	//	VECTOR3 �F ��]
	//	COLOR   �F ���_�J���[
	//===========================================================================
	static CResultSheet* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot=VECTOR3(0,0,0),const COLOR& color=COLOR(1.0f,1.0f,1.0f,1.0f));

	// Geter / Seter
	void DrawEnable(void){ drawFlag = true; }
	void DrawDisable(void){ drawFlag = false; }

	// color
	void SetColor(COLOR col);
	void SetAlphaMax(float al){ alphaMax = al; }

private:

	bool		drawFlag;
	float		alphaMax;
};

#endif

// end of file