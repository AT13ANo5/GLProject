//=============================================================================
//
// ���U�Q�[�W [LoadGauge.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
#ifndef _LOAD_GAUGE_H_
#define _LOAD_GAUGE_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

//=============================================================================
// class
//=============================================================================
class CLoadGauge :public CPolygon2D
{
public:

  CLoadGauge();
  ~CLoadGauge(){}

  void Init(void);
  void Update(void);

  //===========================================================================
  // Create
  //---------------------------------------------------------------------------
  //	VECTOR3 �F ���S���W
  //	VECTOR2 �F �傫��
  //	VECTOR3 �F ��]
  //	COLOR   �F ���_�J���[
  //===========================================================================
  static CLoadGauge* Create(const VECTOR3& pos,const VECTOR2& size,const VECTOR3& rot=VECTOR3(0,0,0),const COLOR& color=COLOR(1.0f,1.0f,1.0f,1.0f));

  // Getter / Setter
  void SetRate(float rate){ rateCurrent = rate; }
  void SetRate(int currentCount, int maxCount){ rateCurrent = (float)currentCount / (float)maxCount; }
  float GetRate(void){ return rateCurrent; }

  void SetDefaultColor(COLOR col);

private:

  float rateCurrent;
  COLOR color;
};

#endif

// end of file