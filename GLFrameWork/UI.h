//=============================================================================
//
// UIマネージャ [UI.h]
//
// Auther : Takahiro Kikushima
//          Masato Masuda
//
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

//=============================================================================
// include
//=============================================================================
#include "main.h"
#include "Renderer.h"
#include "NumberManager.h"

class CLife;
class CMiniMap;
class CLoadGauge;
class CLoadString;
class CPlayer;
class CNumber2D;
class CReport;
class CNumberManager;
class CTime;
class CPolygon2D;


//=============================================================================
// class
//=============================================================================
class CUI
{
public:

	// constructor / destructor
	CUI();
	~CUI();

	void Init(void);
	void Uninit(void);
	void Update(void);

	// getter setter
	// player
	void SetPlayer(CPlayer** p){ player = p; }

	// minimap
	CMiniMap* MiniMap(void){return miniMap;}

	// number
	void SetNumber(int num);
	void SetNumberDrawFlag(bool flag);

	// time
	void	SetTimeUpdateFlag(bool flag);
	int		GetTime(void);
	void	SetTime(int time);

	//=============================================================================
	// SetNumber
	//-----------------------------------------------------------------------------
	//  type  :  タイプ, kill  :  KILL数, DEATH  :  DEATH数
	//  typeは(CNumberManager::TYPE)にキャストして使ってね
	//=============================================================================
	void SetNumber(CNumberManager::TYPE type, int kill, int death);

	void setMyID(int _id){
		myID = _id;
	}

	// 文字の設定
	void SetStringTexture(TEX_INFO tex);
	void SetStringDrawFlag(bool draw);

	// カウント数字
	void SetCountSizeRefresh(void);

private:

	CLife*			    life;
	CMiniMap*		    miniMap;
	CLoadGauge*		  loadGauge;
	CLoadGauge*		  loadGaugeBase;
	CLoadString*    loadString; // 装填中の文字
	CReport*        reportBg;
	CReport*        report;
	CNumberManager* numberManager;
	CNumber2D*			counter;
	CTime*					Time;
	CPolygon2D*			timeFrame;
	CPlayer**				player;
	CPolygon2D*			string;

	int myID;
	bool counterActive;

};

#endif

// end of file