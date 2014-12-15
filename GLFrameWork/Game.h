#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CMeshGround;
class CMeshSphere;
class CModel;
class CPlayer;
class CLoadGauge;
class CLoadString;

class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
private:
	static const float  RADIUS_SKY;   // ‹ó‚Ì”¼Œa

  CLoadGauge*   loadGauge;  // ‘•“UƒQ[ƒW
  CLoadString*  loadString; // ‘•“U’†‚Ì•¶š

  int           testTimer;};

#endif