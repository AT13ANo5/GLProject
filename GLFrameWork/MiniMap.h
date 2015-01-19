#ifndef _MINIMAP_H_
#define _MINIMAP_H_
#include "main.h"

class CPolygon2D;
class CMiniMap
{
public:
	CMiniMap();
	void Init(void);
	void Update(void);

	void SetPlayer(int id,const VECTOR3& pos,float rotY);
	static void SetFieldSize(const VECTOR3 & size){ FieldSize = size; }

private:
	VECTOR3 PlayerPos[PLAYER_MAX];
	VECTOR3 PlayerRot[PLAYER_MAX];
	CPolygon2D* Map;
	CPolygon2D* Player[PLAYER_MAX];
	VECTOR2 MiniMapSize;
	static VECTOR3 FieldSize;
};

#endif