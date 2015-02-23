//=============================================================================
//
// ���� [CNumber2D.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include guard
//=============================================================================
#ifndef _NUMBER_2D_H_
#define _NUMBER_2D_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CNumber2D :public CPolygon2D
{
public:

  CNumber2D();
  ~CNumber2D(){}

  void Update(void);
  void Draw(void);

  void SetAlpha(float a){ _Color.a = a; }
  // ���l��ݒ�iUV���W�����邨�j
  // num : �ݒ肵�����l
  void SetNumber(int num);

  //===========================================================================
  // Create
  //---------------------------------------------------------------------------
  //	VECTOR3 �F ���S���W
  //	VECTOR2 �F �傫��
  //	VECTOR3 �F ��]
  //	COLOR   �F ���_�J���[
  //===========================================================================
  static CNumber2D* Create(const VECTOR3& pos, const VECTOR2& size, const VECTOR3& rot = VECTOR3(0, 0, 0), const COLOR& color = COLOR(1.0f, 1.0f, 1.0f, 1.0f));

  // Geter / Seter
  // �`�悷�邩�ǂ������߂��
  void SetDrawFlag(bool flag){ drawFlag = flag; }
	bool GetDrawFlag(void) { return drawFlag; }
  void DrawDisable(void);

	// �T�C�Y
	void SubSize(VECTOR2 size);

private:

  int   number;
  bool  drawFlag;
};

#endif

// end of file