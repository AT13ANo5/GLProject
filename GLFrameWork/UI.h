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

class CLife;
class CMiniMap;
class CLoadGauge;
class CLoadString;
class CPlayer;
class CNumber2D;
class CReport;
class CNumberManager;


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

	// player
	void SetPlayer(CPlayer** p){ player = p; }

	CMiniMap* MiniMap(void){return miniMap;}

private:

	CLife*			    life;
	CMiniMap*		    miniMap;
	CLoadGauge*		  loadGauge;
	CLoadString*    loadString; // 装填中の文字
	CReport*        reportBg;
	CReport*        report;
	CNumberManager* numberManager;

	CPlayer**	    	player;

};

#endif

// end of file