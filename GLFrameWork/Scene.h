#ifndef _SCENE_H_
#define _SCENE_H_

#include "netData.h"

class CScene
{
public:
	CScene(){ Frame = 0; }
	virtual ~CScene(){}
	virtual void Init(void)=0;
	virtual void Uninit(void)=0;
	virtual void Update(void)=0;
	virtual void SetPlayerState(NET_DATA _netData, DATA_TYPE _dataType){};

protected:
	int Frame;
};

#endif