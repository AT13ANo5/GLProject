//=============================================================================
//
// ���U���g��UI���Ǘ������ [ResultUIManager.h]
//
// Auther : masato masuda
//
//=============================================================================

//=============================================================================
// include guard
//=============================================================================
#ifndef _RESULT_UI_MANAGER_H_
#define _RESULT_UI_MANAGER_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Object.h"
#include "ResultNumberManager.h"

class CResultSheet;
class CResultNumberManager;

//=============================================================================
// class
//=============================================================================
class CResultUIManager :public CObject
{
public:

  CResultUIManager();
  virtual ~CResultUIManager(){}

  void Init(void);
  void Update(void);
  void Uninit(void);
  void Draw(void);

  // draw��off
  void SetResultSheetDisable(void);
  void SetResultNumberDisable(void);
  void SetResultPlayerNameDisable(void);

  //=============================================================================
  // ���l�ݒ�
  //-----------------------------------------------------------------------------
  //  type  :  �^�C�v, kill  :  KILL��, DEATH  :  DEATH��
  //=============================================================================
  void SetNumber(CResultNumberManager::TYPE type, int kill, int death);

  // create
	static CResultUIManager* Create(void);

protected:

  CResultSheet* ResultSheet;  // ���ѕ\
  CResultNumberManager* ResultNumber;  // ���ѕ\�ԍ�
  CResultSheet* PlayerName[PLAYER_MAX];

};

#endif

// end of file