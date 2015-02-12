//=============================================================================
// CSpark�N���X [Spark.h]
// Author :Mai Tanabe
//=============================================================================
#ifndef _MY_SPARK_H
#define _MY_SPARK_H
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "Billboard.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CSpark : public CBillboard
{
public:
	CSpark(){};
	~CSpark(){};

	static void Create(const VECTOR3& pos);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_cntAnim;
};

#endif