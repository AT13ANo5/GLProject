//=============================================================================
// CSandCloud�N���X [SandCloud.h]
// Author :Mai Tanabe
//=============================================================================
#ifndef _MY_SAND_CLOUD_H
#define _MY_SAND_CLOUD_H
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "Billboard.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CSandCloud : public CBillboard
{
public:
	CSandCloud(){};
	~CSandCloud(){};

	static void Create(const VECTOR3& pos);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_cntAnim;
};

#endif