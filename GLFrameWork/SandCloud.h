//=============================================================================
// CSandCloudクラス [SandCloud.h]
// Author :Mai Tanabe
//=============================================================================
#ifndef _MY_SAND_CLOUD_H
#define _MY_SAND_CLOUD_H
//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "Billboard.h"

//=============================================================================
// クラス定義
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