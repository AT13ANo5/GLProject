#ifndef _CONFETTI_H_
#define _CONFETTI_H_

#include "main.h"
#include "Polygon3D.h"

class CConfetti :public CPolygon3D
{
public:
	static CConfetti* Create(const VECTOR3& pos);
	static CConfetti* Create(void);

	void Init(void);
	void Update(void);
	void Draw(void);

private:

	VECTOR3 Speed;
	int frame;
	VECTOR3 SpinSpeed;
};

#endif