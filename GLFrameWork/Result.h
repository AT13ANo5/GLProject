#ifndef _RESULT_H_
#define _RESULT_H_
#include "Scene.h"

class CModel;
class CPolygon2D;
class CMeshGround;
class CMeshSphere;
class CResultSheet;class CResult :public CScene
{
public:

	CResult();
	~CResult();

	typedef enum{
		PHASE_RESULTSHEET = 1,
		PHASE_END,
		PHASE_MAX
	} PHASE;

	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	static const float  RADIUS_SKY;   // ãÛÇÃîºåa

	CPolygon2D*   Logo;
	CModel*       Player;
	CMeshGround*  Ground;       // ínå`
	CMeshSphere*  Sky;          // ãÛ
	CResultSheet* ResultSheet;  // ê¨ê—ï\
	PHASE         Phase;
};
#endif