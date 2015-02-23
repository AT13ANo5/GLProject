//------------------------------------------------------------------------------
//
// �e�� [Ballistic.h]
// Author : AT-13A-273 Mai Tanabe
//
//------------------------------------------------------------------------------
#ifndef _Ballistic_H_
#define _Ballistic_H_

//------------------------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------------------------
#define MARK_MAX  (50)

//------------------------------------------------------------------------------
// �w�b�_�C���N���[�h
//------------------------------------------------------------------------------
#include "CBullet.h"
#include "Polygon3D.h"

//------------------------------------------------------------------------------
// �N���X�錾
//------------------------------------------------------------------------------
class CBillboard;

class CBallistic
{
public:
	CBallistic();
	~CBallistic();

	void Init(COLOR color);
	void Update(VECTOR3 pos, VECTOR3 rot);

	static CBallistic* Create(COLOR color);

	CBillboard* GetMark(int no){return BallisticMark[no];}
	CPolygon3D* GetLanding(void){return Landing;}

	void SetLanding(VECTOR3 pos){Landing->SetPos(pos);}
	
	void SetDrawFlag(bool drawFlag)
	{
		for(int i = 0; i < MARK_MAX; i++)
		{
			BallisticMark[i]->SetDrawFlag(drawFlag);
			Landing->SetDrawFlag(drawFlag);
		}
	}

private:
	CBillboard* BallisticMark[MARK_MAX];	// �e���̃}�[�N
	CPolygon3D* Landing;					// ���e�}�[�N
	float		LandingAddRot;
};

#endif
//------------------------------------------------------------------------------
// EOF