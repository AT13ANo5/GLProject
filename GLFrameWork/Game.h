#ifndef _GAME_H_
#define _GAME_H_
#include"Scene.h"

class CGame :public CScene
{
public:
	CGame();
	~CGame();

	void Init(void);
	void Uninit(void);
	void Update(void);
private:

};

#endif