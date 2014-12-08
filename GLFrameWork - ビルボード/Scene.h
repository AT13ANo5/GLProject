#ifndef _SCENE_H_
#define _SCENE_H_

class CScene
{
public:
	CScene(){ Frame = 0; }
	virtual ~CScene(){}
	virtual void Init(void)=0;
	virtual void Uninit(void)=0;
	virtual void Update(void)=0;

protected:
	int Frame;
};

#endif