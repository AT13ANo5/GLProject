//=============================================================================
//
// リザルトのUIを管理するよ [ResultUIManager.h]
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

  // drawのoff
  void SetResultSheetDisable(void);
  void SetResultNumberDisable(void);
  void SetResultPlayerNameDisable(void);

  //=============================================================================
  // 数値設定
  //-----------------------------------------------------------------------------
  //  type  :  タイプ, kill  :  KILL数, DEATH  :  DEATH数
  //=============================================================================
  void SetNumber(CResultNumberManager::TYPE type, int kill, int death);

  // create
	static CResultUIManager* Create(void);

protected:

  CResultSheet* ResultSheet;  // 成績表
  CResultNumberManager* ResultNumber;  // 成績表番号
  CResultSheet* PlayerName[PLAYER_MAX];

};

#endif

// end of file