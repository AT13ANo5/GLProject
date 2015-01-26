//=============================================================================
//
// UIマネージャ [UI.h]
//
// Auther : Takahiro Kikushima
//
//=============================================================================

#ifndef _UI_H_
#define _UI_H_
#include "main.h"
#include "Renderer.h"
//#include "Number2D.h"
//#include "MeshGround.h"
#include "LoadGauge.h"
#include "LoadString.h"

class CLife;
class CMiniMap;
class CLoadGauge;
class CLoadString;
class CPlayer;
class CNumber2D;

class CUI
{
public:
	CUI();
	virtual ~CUI();
	void Init(void);
	void Uninit(void);
	void Update(void);

	void SetPlayer(CPlayer* player){ Player = player; }
	//void SetGround(CMeshGround* Ground){ this->Ground = Ground; }

private:
	CLife*			Life;
	CMiniMap*		MiniMap;
	CLoadGauge*		loadGauge;
	CLoadString*    loadString; // 装填中の文字
	//CNumber2D*		Time; //未実装
	CPlayer*		Player;
	//CMeshGround*	Ground;

};

#endif