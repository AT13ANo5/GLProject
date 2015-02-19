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
#include "Explosion.h"

//=============================================================================
// class
//=============================================================================
class CPolygon2D;

class CTitleDirection :public CPolygon2D
{
public:

	CTitleDirection();
	~CTitleDirection(){}

	void Update(void);
	void Init(void);
	void Uninit(void);
	void Draw(void);

	//===========================================================================
	// Create
	//---------------------------------------------------------------------------
	//	VECTOR3 �F ���S���W
	//	VECTOR2 �F �傫��
	//	VECTOR3 �F ��]
	//	COLOR   �F ���_�J���[
	//===========================================================================
	static CTitleDirection* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

private:

	int   number;
	VECTOR2	Size;
	bool	UDflag;
	int Count;
	CPolygon2D* Title;
};

#endif

// end of file