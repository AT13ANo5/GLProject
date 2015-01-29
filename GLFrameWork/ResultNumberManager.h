//=============================================================================
//
// �������Ǘ������ [CResultNumberManager.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include guard
//=============================================================================
#ifndef _RESULT_NUMBER_MANAGER_H_
#define _RESULT_NUMBER_MANAGER_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Polygon2D.h"

class CNumber2D;

//=============================================================================
// class
//=============================================================================
class CResultNumberManager :public CPolygon2D
{
public:

  // �����̎��
  typedef enum _TYPE{
    TYPE_PLAYER_01,
    TYPE_PLAYER_02,
    TYPE_PLAYER_03,
    TYPE_PLAYER_04,
    TYPE_PLAYER_05,
    TYPE_PLAYER_06,
    TYPE_MAX
  } TYPE;

  static const int NUMBER_MAX = 6;
  static const int FIGURE_MAX = 2;
  typedef struct _NUMBER{
    CNumber2D*  _kill[FIGURE_MAX];    // KILL��������
    CNumber2D*  _death[FIGURE_MAX];   // DEATH��������
  } NUMBER;

  CResultNumberManager();
  ~CResultNumberManager(){}

  void Init(void);
  void Update(void);
  void Draw(void){}

  // Create
  static CResultNumberManager* Create();

  // ���l��ݒ�
  void SetNumberKill(TYPE type, int kill);
  void SetNumberDeath(TYPE type, int death);
  void SetNumber(TYPE type, int kill, int death);

  // Geter / Seter
  void SetDrawFlag(bool flag);
  // Geter / Seter
  void DrawEnable(void){ drawFlag = true; }
  void DrawDisable(void){ drawFlag = false; }

  VECTOR3 GetNumPosition(TYPE type);

private:

  bool    drawFlag;
  NUMBER  number[NUMBER_MAX];

};

#endif

// end of file