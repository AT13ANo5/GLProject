//============================================================================
//�~�j�}�b�v�N���X
//============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_
//============================================================================
//�C���N���[�h
//============================================================================
#include "main.h"

//============================================================================
//�O���錾
//============================================================================
class CPolygon2D;
class CMap2D;

//============================================================================
//�N���X��`
//============================================================================
class CMiniMap
{
public:
	CMiniMap();
	void Init(void);
	void Update(void);

	void SetPlayer(int id,const VECTOR3& pos,float rotY);
	static void SetFieldSize(const VECTOR3 & size){ FieldSize = size; }
	static void SetSelfId(int id){_SelfId = id;}

	static int SelfId(void){return _SelfId;}

private:
	VECTOR3 PlayerPos[PLAYER_MAX];
	VECTOR3 PlayerRot[PLAYER_MAX];
	CMap2D* Map;
	CPolygon2D* Player[PLAYER_MAX];
	VECTOR2 MiniMapSize;
	static int _SelfId;
	static VECTOR3 FieldSize;
};

#endif