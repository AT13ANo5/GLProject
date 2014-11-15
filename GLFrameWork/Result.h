#ifndef _RESULT_H_
#define _RESULT_H_
#include "Scene.h"

class CResult :public CScene
{
public:
	CResult();
	~CResult();

	void Init(void);
	void Uninit(void);
	void Update(void);

private:

};
#endif