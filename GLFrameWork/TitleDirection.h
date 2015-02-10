//=============================================================================
//
// �^�C�g�����o
//
// Auther : Takahiro Kikushima
//
//=============================================================================

//=============================================================================
// include guard
//=============================================================================
#ifndef _TITLE_DIRECTION_H_
#define _TITLE_DIRECTION_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CTitleDirection :public CPolygon2D
{
public:

	CTitleDirection();
	~CTitleDirection(){}

	void Update(void);
	void Draw(void);

	void SetAlpha(float a){ _Color.a = a; }

	//===========================================================================
	// Create
	//---------------------------------------------------------------------------
	//	VECTOR3 �F ���S���W
	//	VECTOR2 �F �傫��
	//	VECTOR3 �F ��]
	//	COLOR   �F ���_�J���[
	//===========================================================================
	static CTitleDirection* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

	void SetDrawFlag(bool flag){ drawFlag = flag; }
	void DrawDisable(void);


private:

	int   number;
	bool  drawFlag;
	VECTOR2	Size;
	bool	UDflag;
	int Count;
};

#endif

// end of file